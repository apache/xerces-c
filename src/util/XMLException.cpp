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
 * Revision 1.2  1999/12/02 19:02:57  roddey
 * Get rid of a few statically defined XMLMutex objects, and lazy eval them
 * using atomic compare and swap. I somehow let it get by me that we don't
 * want any static/global objects at all.
 *
 * Revision 1.1.1.1  1999/11/09 01:05:39  twl
 * Initial checkin
 *
 * Revision 1.3  1999/11/08 20:45:19  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <util/Mutexes.hpp>
#include <util/PlatformUtils.hpp>
#include <util/XMLException.hpp>
#include <util/XMLMsgLoader.hpp>
#include <util/XMLString.hpp>
#include <util/XMLUni.hpp>


// ---------------------------------------------------------------------------
//  Local data
//
//  gDefErrMsg
//      For now, if we cannot load an exception message, we load up the
//      default.
// ---------------------------------------------------------------------------
static const XMLCh  gDefErrMsg[] = 
{
        chLatin_C, chLatin_o, chLatin_u, chLatin_l, chLatin_d
    ,   chSpace, chLatin_n, chLatin_o, chLatin_t, chSpace
    ,   chLatin_l, chLatin_o, chLatin_a, chLatin_d, chSpace
    ,   chLatin_m, chLatin_e, chLatin_s, chLatin_s, chLatin_a
    ,   chLatin_g, chLatin_e, chNull
};


// ---------------------------------------------------------------------------
//  Local, static functions
// ---------------------------------------------------------------------------

//
//  We need to fault in this mutex. But, since its used for synchronization
//  itself, we have to do this the low level way using a compare and swap.
//
static XMLMutex& gMsgMutex()
{
    static XMLMutex* msgMutex = 0;
    if (!msgMutex)
    {
        XMLMutex* tmpMutex = new XMLMutex;
        if (XMLPlatformUtils::compareAndSwap((void**)&msgMutex, tmpMutex, 0))
        {
            // Someone beat us to it, so let's clean up ours
            delete tmpMutex;
        }
    }
    return *msgMutex;
}



// ---------------------------------------------------------------------------
//  Local methods
// ---------------------------------------------------------------------------

//
//  This method is a lazy evaluator for the message loader for exception
//  messages.
//
//  NOTE:   This is only ever called with the mutex locked so we don't
//          have to synchronize here!!
//
static XMLMsgLoader& gGetMsgLoader()
{
    // Fault it in on first request
    static XMLMsgLoader* gLoader = 0;
    if (!gLoader)
    {
        gLoader = XMLPlatformUtils::loadMsgSet(XMLUni::fgExceptDomain);
        if (!gLoader)
            XMLPlatformUtils::panic(XMLPlatformUtils::Panic_CantLoadMsgDomain);
    }

    // We got it, so return it
    return *gLoader;
}



// ---------------------------------------------------------------------------
//  XMLException: Virtual destructor
// ---------------------------------------------------------------------------
XMLException::~XMLException()
{
    delete [] fMsg;
    delete [] fSrcFile;
}


// ---------------------------------------------------------------------------
//  XMLException: Setter methods
// ---------------------------------------------------------------------------
void XMLException::setPosition(const char* const file, const unsigned int line)
{
    fSrcLine = line;
    delete [] fSrcFile;
    fSrcFile = XMLString::replicate(file);
}


// ---------------------------------------------------------------------------
//  XMLException: Hidden constructors and operators
// ---------------------------------------------------------------------------
XMLException::XMLException() :

    fCode(XML4CExcepts::NoError)
    , fSrcFile(0)
    , fSrcLine(0)
    , fMsg(0)
{
}


XMLException::XMLException( const   char* const     srcFile
                            , const unsigned int    srcLine) :

    fCode(XML4CExcepts::NoError)
    , fSrcFile(0)
    , fSrcLine(srcLine)
    , fMsg(0)
{
    fSrcFile = XMLString::replicate(srcFile);
}


XMLException::XMLException(const XMLException& toCopy) :

    fCode(toCopy.fCode)
    , fSrcFile(0)
    , fSrcLine(toCopy.fSrcLine)
    , fMsg(XMLString::replicate(toCopy.fMsg))
{
    if (toCopy.fSrcFile)
        fSrcFile = XMLString::replicate(toCopy.fSrcFile);
}


void XMLException::operator=(const XMLException& toAssign)
{
    if (this != &toAssign)
    {
        delete [] fSrcFile;
        fSrcFile = 0;
        delete [] fMsg;
        fMsg = 0;

        fSrcLine = toAssign.fSrcLine;
        fCode = toAssign.fCode;

        if (toAssign.fMsg)
            fMsg = XMLString::replicate(toAssign.fMsg);

        if (toAssign.fSrcFile)
            fSrcFile = XMLString::replicate(toAssign.fSrcFile);
    }
}



// ---------------------------------------------------------------------------
//  XMLException: Protected methods
// ---------------------------------------------------------------------------
void XMLException::loadExceptText(const XML4CExcepts::Codes toLoad)
{
    // Store the error code
    fCode = toLoad;

    // Load up the text into a local buffer
    const unsigned int msgSize = 2047;
    XMLCh errText[msgSize + 1];

    // Lock the message loader mutex and load the text
    {
        XMLMutexLock lockInit(&gMsgMutex());
        if (!gGetMsgLoader().loadMsg(toLoad, errText, msgSize))
        {
            fMsg = XMLString::replicate(gDefErrMsg);
            return;
        }
    }

    // We got the text so replicate it into the message member
    fMsg = XMLString::replicate(errText);
}


void
XMLException::loadExceptText(const  XML4CExcepts::Codes toLoad
                            , const XMLCh* const        text1
                            , const XMLCh* const        text2
                            , const XMLCh* const        text3
                            , const XMLCh* const        text4)
{
    // Store the error code
    fCode = toLoad;

    // Load up the text into a local buffer
    const unsigned int msgSize = 4095;
    XMLCh errText[msgSize + 1];

    // Lock the message loader mutex and load the text
    {
        XMLMutexLock lockInit(&gMsgMutex());
        if (!gGetMsgLoader().loadMsg(toLoad, errText, msgSize, text1, text2, text3, text4))
        {
            fMsg = XMLString::replicate(gDefErrMsg);
            return;
        }
    }

    // We got the text so replicate it into the message member
    fMsg = XMLString::replicate(errText);
}


void
XMLException::loadExceptText(const  XML4CExcepts::Codes toLoad
                            , const char* const         text1
                            , const char* const         text2
                            , const char* const         text3
                            , const char* const         text4)
{
    // Store the error code
    fCode = toLoad;

    // Load up the text into a local buffer
    const unsigned int msgSize = 4095;
    XMLCh errText[msgSize + 1];

    // Lock the message loader mutex and load the text
    {
        XMLMutexLock lockInit(&gMsgMutex());
        if (!gGetMsgLoader().loadMsg(toLoad, errText, msgSize, text1, text2, text3, text4))
        {
            fMsg = XMLString::replicate(gDefErrMsg);
            return;
        }
    }

    // We got the text so replicate it into the message member
    fMsg = XMLString::replicate(errText);
}
