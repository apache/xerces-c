/*
 * The Apache Software License, Version 1.1
 * 
 * Copyright (c) 1999 The Apache Software Foundation.  All rights 
 * reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer. 
 * 
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 
 * 3. The end-user documentation included with the redistribution,
 *    if any, must include the following acknowledgment:  
 *       "This product includes software developed by the
 *        Apache Software Foundation (http://www.apache.org/)."
 *    Alternately, this acknowledgment may appear in the software itself,
 *    if and wherever such third-party acknowledgments normally appear.
 * 
 * 4. The names "Xerces" and "Apache Software Foundation" must
 *    not be used to endorse or promote products derived from this
 *    software without prior written permission. For written 
 *    permission, please contact apache\@apache.org.
 * 
 * 5. Products derived from this software may not be called "Apache",
 *    nor may "Apache" appear in their name, without prior written
 *    permission of the Apache Software Foundation.
 * 
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE APACHE SOFTWARE FOUNDATION OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * ====================================================================
 * 
 * This software consists of voluntary contributions made by many
 * individuals on behalf of the Apache Software Foundation, and was
 * originally based on software copyright (c) 1999, International
 * Business Machines, Inc., http://www.ibm.com .  For more information
 * on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 */

/**
  * $Log$
  * Revision 1.2  1999/12/02 19:02:56  roddey
  * Get rid of a few statically defined XMLMutex objects, and lazy eval them
  * using atomic compare and swap. I somehow let it get by me that we don't
  * want any static/global objects at all.
  *
  * Revision 1.1.1.1  1999/11/09 01:08:37  twl
  * Initial checkin
  *
  * Revision 1.3  1999/11/08 20:44:40  rahul
  * Swat for adding in Product name and CVS comment log variable.
  *
  */

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <util/Janitor.hpp>
#include <util/Mutexes.hpp>
#include <util/PlatformUtils.hpp>
#include <util/XMLMsgLoader.hpp>
#include <util/XMLString.hpp>
#include <framework/XMLErrorReporter.hpp>
#include <framework/XMLValidator.hpp>
#include <internal/XMLScanner.hpp>



// ---------------------------------------------------------------------------
//  Local static functions
// ---------------------------------------------------------------------------

//
//  We need to fault in this mutex. But, since its used for synchronization
//  itself, we have to do this the low level way using a compare and swap.
//
static XMLMutex& gValidatorMutex()
{
    static XMLMutex* validatorMutex = 0;
    if (!validatorMutex)
    {
        XMLMutex* tmpMutex = new XMLMutex;
        if (XMLPlatformUtils::compareAndSwap((void**)&validatorMutex, tmpMutex, 0))
        {
            // Someone beat us to it, so let's clean up ours
            delete tmpMutex;
        }
    }
    return *validatorMutex;
}

static XMLMsgLoader& getMsgLoader()
{
    static XMLMsgLoader* gMsgLoader = 0;

    if (!gMsgLoader)
    {
        XMLMutexLock lockInit(&gValidatorMutex());
        if (!gMsgLoader)
        {
            gMsgLoader = XMLPlatformUtils::loadMsgSet(XMLUni::fgValidityDomain);
            if (!gMsgLoader)
                XMLPlatformUtils::panic(XMLPlatformUtils::Panic_CantLoadMsgDomain);
        }
    }
    return *gMsgLoader;
}



// ---------------------------------------------------------------------------
//  XMLValidator: Error emitting methods
// ---------------------------------------------------------------------------

//
//  These methods are called whenever the scanner wants to emit an error.
//  It handles getting the message loaded, doing token replacement, etc...
//  and then calling the error handler, if its installed.
//
void XMLValidator::emitError(const XML4CValid::Codes toEmit)
{
    if (fErrorReporter)
    {
        // Load the message into a local for display
        const unsigned int msgSize = 1023;
        XMLCh errText[msgSize + 1];

        // Lock the mutex and load the text
        {
            XMLMutexLock lockInit(&gValidatorMutex());
            if (!getMsgLoader().loadMsg(toEmit, errText, msgSize))
            {
                // <TBD> Probably should load a default msg here
            }
        }

        //
        //  Create a LastExtEntityInfo structure and get the reader manager
        //  to fill it in for us. This will give us the information about
        //  the last reader on the stack that was an external entity of some
        //  sort (i.e. it will ignore internal entities.
        //
        ReaderMgr::LastExtEntityInfo lastInfo;
        fReaderMgr->getLastExtEntityInfo(lastInfo);

        fErrorReporter->error
        (
            toEmit
            , XMLUni::fgValidityDomain
            , XML4CValid::errorType(toEmit)
            , errText
            , lastInfo.systemId
            , lastInfo.publicId
            , lastInfo.lineNumber
            , lastInfo.colNumber
        );
    }

    // Bail out if its fatal an we are to give up on the first fatal error
    if (XML4CValid::isFatal(toEmit)
    &&  fScanner->getExitOnFirstFatal()
    &&  fScanner->getInException())
    {
        throw toEmit;
    }
}

void XMLValidator::emitError( const   XML4CValid::Codes   toEmit
                            , const XMLCh* const        text1
                            , const XMLCh* const        text2
                            , const XMLCh* const        text3
                            , const XMLCh* const        text4)
{
    if (fErrorReporter)
    {
        //
        //  Load the message into alocal and replace any tokens found in
        //  the text.
        //
        const unsigned int maxChars = 2047;
        XMLCh errText[maxChars + 1];

        // Lock the mutex and load the text
        {
            XMLMutexLock lockInit(&gValidatorMutex());
            if (!getMsgLoader().loadMsg(toEmit, errText, maxChars, text1, text2, text3, text4))
            {
                // <TBD> Should probably load a default message here
            }
        }

        //
        //  Create a LastExtEntityInfo structure and get the reader manager
        //  to fill it in for us. This will give us the information about
        //  the last reader on the stack that was an external entity of some
        //  sort (i.e. it will ignore internal entities.
        //
        ReaderMgr::LastExtEntityInfo lastInfo;
        fReaderMgr->getLastExtEntityInfo(lastInfo);

        fErrorReporter->error
        (
            toEmit
            , XMLUni::fgValidityDomain
            , XML4CValid::errorType(toEmit)
            , errText
            , lastInfo.systemId
            , lastInfo.publicId
            , lastInfo.lineNumber
            , lastInfo.colNumber
        );
    }

    // Bail out if its fatal an we are to give up on the first fatal error
    if (XML4CValid::isFatal(toEmit)
    &&  fScanner->getExitOnFirstFatal()
    &&  fScanner->getInException())
    {
        throw toEmit;
    }
}

void XMLValidator::emitError( const   XML4CValid::Codes   toEmit
                            , const char* const         text1
                            , const char* const         text2
                            , const char* const         text3
                            , const char* const         text4)
{
    if (fErrorReporter)
    {
        //
        //  Load the message into alocal and replace any tokens found in
        //  the text.
        //
        const unsigned int maxChars = 2047;
        XMLCh errText[maxChars + 1];

        // Lock the mutex and load the text
        {
            XMLMutexLock lockInit(&gValidatorMutex());
            if (!getMsgLoader().loadMsg(toEmit, errText, maxChars, text1, text2, text3, text4))
            {
                // <TBD> Should probably load a default message here
            }
        }

        //
        //  Create a LastExtEntityInfo structure and get the reader manager
        //  to fill it in for us. This will give us the information about
        //  the last reader on the stack that was an external entity of some
        //  sort (i.e. it will ignore internal entities.
        //
        ReaderMgr::LastExtEntityInfo lastInfo;
        fReaderMgr->getLastExtEntityInfo(lastInfo);

        fErrorReporter->error
        (
            toEmit
            , XMLUni::fgValidityDomain
            , XML4CValid::errorType(toEmit)
            , errText
            , lastInfo.systemId
            , lastInfo.publicId
            , lastInfo.lineNumber
            , lastInfo.colNumber
        );
    }

    // Bail out if its fatal an we are to give up on the first fatal error
    if (XML4CValid::isFatal(toEmit)
    &&  fScanner->getExitOnFirstFatal()
    &&  fScanner->getInException())
    {
        throw toEmit;
    }
}


// ---------------------------------------------------------------------------
//  XMLValidator: Hidden Constructors
// ---------------------------------------------------------------------------
XMLValidator::XMLValidator(XMLErrorReporter* const errReporter) :

    fBufMgr(0)
    , fEmptyNamespaceId(-1)
    , fErrorReporter(errReporter)
    , fGlobalNamespaceId(-1)
    , fReaderMgr(0)
    , fScanner(0)
    , fUnknownNamespaceId(-1)
    , fXMLNamespaceId(-1)
    , fXMLNSNamespaceId(-1)
{
}
