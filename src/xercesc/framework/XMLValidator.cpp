/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 1999-2001 The Apache Software Foundation.  All rights
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
  * Revision 1.3  2003/03/09 16:38:04  peiyongz
  * PanicHandler
  *
  * Revision 1.2  2002/11/04 15:00:21  tng
  * C++ Namespace Support.
  *
  * Revision 1.1.1.1  2002/02/01 22:21:52  peiyongz
  * sane_include
  *
  * Revision 1.13  2001/11/30 22:18:18  peiyongz
  * cleanUp function made member function
  * cleanUp object moved to file scope
  * double mutex lock removed
  *
  * Revision 1.12  2001/11/28 20:32:49  tng
  * Do not increment the error count if it is a warning.
  *
  * Revision 1.11  2001/10/24 23:46:52  peiyongz
  * [Bug 4342] fix the leak.
  *
  * Revision 1.10  2001/06/04 21:07:34  jberry
  * Increment scanner error count from schema validator, not just in scanner itself.
  *
  * Revision 1.9  2001/05/11 13:25:33  tng
  * Copyright update.
  *
  * Revision 1.8  2001/05/03 19:08:58  knoaman
  * Support Warning/Error/FatalError messaging.
  * Validity constraints errors are treated as errors, with the ability by user to set
  * validity constraints as fatal errors.
  *
  * Revision 1.7  2001/03/21 21:56:02  tng
  * Schema: Add Schema Grammar, Schema Validator, and split the DTDValidator into DTDValidator, DTDScanner, and DTDGrammar.
  *
  * Revision 1.6  2000/03/28 19:43:17  roddey
  * Fixes for signed/unsigned warnings. New work for two way transcoding
  * stuff.
  *
  * Revision 1.5  2000/03/02 19:54:25  roddey
  * This checkin includes many changes done while waiting for the
  * 1.1.0 code to be finished. I can't list them all here, but a list is
  * available elsewhere.
  *
  * Revision 1.4  2000/02/06 07:47:49  rahulj
  * Year 2K copyright swat.
  *
  * Revision 1.3  1999/12/08 00:15:06  roddey
  * Some small last minute fixes to get into the 3.0.1 build that is going to be
  * going out anyway for platform fixes.
  *
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
#include <xercesc/util/Janitor.hpp>
#include <xercesc/util/Mutexes.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLMsgLoader.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/XMLRegisterCleanup.hpp>
#include <xercesc/framework/XMLErrorReporter.hpp>
#include <xercesc/framework/XMLValidator.hpp>
#include <xercesc/internal/XMLScanner.hpp>

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  Local static functions
// ---------------------------------------------------------------------------

static XMLMutex* sMsgMutex = 0;
static XMLRegisterCleanup msgLoaderCleanup;

static XMLMsgLoader* sMsgLoader = 0;
static XMLRegisterCleanup validatorMutexCleanup;

//
//  We need to fault in this mutex. But, since its used for synchronization
//  itself, we have to do this the low level way using a compare and swap.
//
static XMLMutex& gValidatorMutex()
{
    if (!sMsgMutex)
    {
        XMLMutex* tmpMutex = new XMLMutex;
        if (XMLPlatformUtils::compareAndSwap((void**)&sMsgMutex, tmpMutex, 0))
        {
            // Someone beat us to it, so let's clean up ours
            delete tmpMutex;
        }
        else
        {
            validatorMutexCleanup.registerCleanup(XMLValidator::reinitMsgMutex);
        }
    }

    return *sMsgMutex;
}

static XMLMsgLoader& getMsgLoader()
{

	// Lock the mutex
	XMLMutexLock lockInit(&gValidatorMutex());

    if (!sMsgLoader)
	{
		sMsgLoader = XMLPlatformUtils::loadMsgSet(XMLUni::fgValidityDomain);
		if (!sMsgLoader)
			XMLPlatformUtils::panic(PanicHandler::Panic_CantLoadMsgDomain);

        //
        // Register this XMLMsgLoader for cleanup at Termination.
        //
        msgLoaderCleanup.registerCleanup(XMLValidator::reinitMsgLoader);
	
	}

    return *sMsgLoader;
}



// ---------------------------------------------------------------------------
//  XMLValidator: Error emitting methods
// ---------------------------------------------------------------------------

//
//  These methods are called whenever the scanner wants to emit an error.
//  It handles getting the message loaded, doing token replacement, etc...
//  and then calling the error handler, if its installed.
//
void XMLValidator::emitError(const XMLValid::Codes toEmit)
{
    // Bump the error count if it is not a warning
    if (XMLValid::errorType(toEmit) != XMLErrorReporter::ErrType_Warning)
        fScanner->incrementErrorCount();

    //	Call error reporter if we have one
    if (fErrorReporter)
    {
        // Load the message into a local for display
        const unsigned int msgSize = 1023;
        XMLCh errText[msgSize + 1];

        // load the text
		if (!getMsgLoader().loadMsg(toEmit, errText, msgSize))
		{
			// <TBD> Probably should load a default msg here
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
            , XMLValid::errorType(toEmit)
            , errText
            , lastInfo.systemId
            , lastInfo.publicId
            , lastInfo.lineNumber
            , lastInfo.colNumber
        );
    }

    // Bail out if its fatal an we are to give up on the first fatal error
    if (((XMLValid::isError(toEmit) && fScanner->getValidationConstraintFatal())
         || XMLValid::isFatal(toEmit))
    &&  fScanner->getExitOnFirstFatal()
    &&  !fScanner->getInException())
    {
        throw toEmit;
    }
}

void XMLValidator::emitError(const  XMLValid::Codes toEmit
                            , const XMLCh* const    text1
                            , const XMLCh* const    text2
                            , const XMLCh* const    text3
                            , const XMLCh* const    text4)
{
    // Bump the error count if it is not a warning
    if (XMLValid::errorType(toEmit) != XMLErrorReporter::ErrType_Warning)
        fScanner->incrementErrorCount();

    //	Call error reporter if we have one
    if (fErrorReporter)
    {
        //
        //  Load the message into alocal and replace any tokens found in
        //  the text.
        //
        const unsigned int maxChars = 2047;
        XMLCh errText[maxChars + 1];

        // load the text
		if (!getMsgLoader().loadMsg(toEmit, errText, maxChars, text1, text2, text3, text4))
		{
			// <TBD> Should probably load a default message here
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
            , XMLValid::errorType(toEmit)
            , errText
            , lastInfo.systemId
            , lastInfo.publicId
            , lastInfo.lineNumber
            , lastInfo.colNumber
        );
    }

    // Bail out if its fatal an we are to give up on the first fatal error
    if (((XMLValid::isError(toEmit) && fScanner->getValidationConstraintFatal())
         || XMLValid::isFatal(toEmit))
    &&  fScanner->getExitOnFirstFatal()
    &&  !fScanner->getInException())
    {
        throw toEmit;
    }
}

void XMLValidator::emitError(const  XMLValid::Codes toEmit
                            , const char* const     text1
                            , const char* const     text2
                            , const char* const     text3
                            , const char* const     text4)
{
    // Bump the error count if it is not a warning
    if (XMLValid::errorType(toEmit) != XMLErrorReporter::ErrType_Warning)
        fScanner->incrementErrorCount();

    //	Call error reporter if we have one
    if (fErrorReporter)
    {
        //
        //  Load the message into alocal and replace any tokens found in
        //  the text.
        //
        const unsigned int maxChars = 2047;
        XMLCh errText[maxChars + 1];

        // load the text
		if (!getMsgLoader().loadMsg(toEmit, errText, maxChars, text1, text2, text3, text4))
		{
			// <TBD> Should probably load a default message here
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
            , XMLValid::errorType(toEmit)
            , errText
            , lastInfo.systemId
            , lastInfo.publicId
            , lastInfo.lineNumber
            , lastInfo.colNumber
        );
    }

    // Bail out if its fatal an we are to give up on the first fatal error
    if (((XMLValid::isError(toEmit) && fScanner->getValidationConstraintFatal())
         || XMLValid::isFatal(toEmit))
    &&  fScanner->getExitOnFirstFatal()
    &&  !fScanner->getInException())
    {
        throw toEmit;
    }
}


// ---------------------------------------------------------------------------
//  XMLValidator: Hidden Constructors
// ---------------------------------------------------------------------------
XMLValidator::XMLValidator(XMLErrorReporter* const errReporter) :

    fBufMgr(0)
    , fErrorReporter(errReporter)
    , fReaderMgr(0)
    , fScanner(0)
{
}

// -----------------------------------------------------------------------
//  Notification that lazy data has been deleted
// -----------------------------------------------------------------------
void XMLValidator::reinitMsgMutex()
{
    delete sMsgMutex;
    sMsgMutex = 0;
}

// -----------------------------------------------------------------------
//  Reinitialise the message loader
// -----------------------------------------------------------------------
void XMLValidator::reinitMsgLoader()
{
	delete sMsgLoader;
	sMsgLoader = 0;
}

XERCES_CPP_NAMESPACE_END
