/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 2002 The Apache Software Foundation.  All rights
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
 * originally based on software copyright (c) 2001, International
 * Business Machines, Inc., http://www.ibm.com .  For more information
 * on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 */

/**
  * $Log$
  * Revision 1.2  2002/05/22 20:54:14  knoaman
  * Prepare for DOM L3 :
  * - Make use of the XMLEntityHandler/XMLErrorReporter interfaces, instead of using
  * EntityHandler/ErrorHandler directly.
  * - Add 'AbstractDOMParser' class to provide common functionality for XercesDOMParser
  * and DOMBuilder.
  *
  * Revision 1.1  2002/03/21 15:34:40  knoaman
  * Add support for reporting line/column numbers of schema errors.
  *
  */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/util/Mutexes.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/framework/XMLErrorCodes.hpp>
#include <xercesc/framework/XMLValidityCodes.hpp>
#include <xercesc/framework/XMLErrorReporter.hpp>
#include <xercesc/util/XMLRegisterCleanup.hpp>
#include <xercesc/validators/schema/XSDErrorReporter.hpp>
#include <xercesc/validators/schema/XSDLocator.hpp>

// ---------------------------------------------------------------------------
//  Local static data
// ---------------------------------------------------------------------------
static bool               sMsgMutexRegistered = false;
static XMLMutex*          sErrMsgMutex = 0;
static XMLRegisterCleanup cleanupErrMsgMutex;
static XMLMsgLoader*      gErrMsgLoader = 0;
static XMLMsgLoader*      gValidMsgLoader = 0;
static XMLRegisterCleanup cleanupErrMsgLoader;


// ---------------------------------------------------------------------------
//  Local, static functions
// ---------------------------------------------------------------------------
void XSDErrorReporter::reinitErrMsgLoader()
{
	delete gErrMsgLoader;
	gErrMsgLoader = 0;
	delete gValidMsgLoader;
	gValidMsgLoader = 0;
}


void XSDErrorReporter::reinitErrMsgMutex()
{
    delete sErrMsgMutex;
    sErrMsgMutex = 0;
    sMsgMutexRegistered = false;
}

//
//  We need to fault in this mutex. But, since its used for synchronization
//  itself, we have to do this the low level way using a compare and swap.
//
static XMLMutex& gErrMsgMutex()
{
    if (!sErrMsgMutex)
    {
        XMLMutex* tmpMutex = new XMLMutex;
        if (XMLPlatformUtils::compareAndSwap((void**)&sErrMsgMutex, tmpMutex, 0))
        {
            // Someone beat us to it, so let's clean up ours
            delete tmpMutex;
        }

        // Now lock it and try to register it
        XMLMutexLock lock(sErrMsgMutex);

        // If we got here first, then register it and set the registered flag
        if (!sMsgMutexRegistered)
        {
            cleanupErrMsgMutex.registerCleanup(XSDErrorReporter::reinitErrMsgMutex);
            sMsgMutexRegistered = true;
        }
    }
    return *sErrMsgMutex;
}


// ---------------------------------------------------------------------------
//  XSDErrorReporter: Constructors and Destructor
// ---------------------------------------------------------------------------
XSDErrorReporter::XSDErrorReporter(XMLErrorReporter* const errorReporter) :
    fExitOnFirstFatal(false)
    , fErrorReporter(errorReporter)
{

    //
    //  We have to do a little init that involves statics, so we have to
    //  use the mutex to protect it.
    //
    {
        XMLMutexLock lockInit(&gErrMsgMutex());

        // If we haven't loaded our message yet, then do that
        if (!gErrMsgLoader)
        {
            gErrMsgLoader = XMLPlatformUtils::loadMsgSet(XMLUni::fgXMLErrDomain);
            if (!gErrMsgLoader)
                XMLPlatformUtils::panic(XMLPlatformUtils::Panic_CantLoadMsgDomain);

            gValidMsgLoader = XMLPlatformUtils::loadMsgSet(XMLUni::fgValidityDomain);
            if (!gValidMsgLoader) {

                reinitErrMsgLoader();
                XMLPlatformUtils::panic(XMLPlatformUtils::Panic_CantLoadMsgDomain);
            }

            // Register this object to be cleaned up at termination
			cleanupErrMsgLoader.registerCleanup(reinitErrMsgLoader);
        }
    }
}


// ---------------------------------------------------------------------------
//  XSDErrorReporter: Error reporting
// ---------------------------------------------------------------------------
void XSDErrorReporter::emitError(const unsigned int toEmit,
                                 const XMLCh* const msgDomain,
                                 const Locator* const aLocator)
{
    // Bump the error count if it is not a warning
//    if (XMLErrs::errorType(toEmit) != XMLErrorReporter::ErrType_Warning)
//        incrementErrorCount();

    //
    //  Load the message into alocal and replace any tokens found in
    //  the text.
    //
    const unsigned int msgSize = 1023;
    XMLCh errText[msgSize + 1];
    XMLMsgLoader* msgLoader = gErrMsgLoader;
    XMLErrorReporter::ErrTypes errType = XMLErrs::errorType((XMLErrs::Codes) toEmit);

    if (!XMLString::compareString(msgDomain, XMLUni::fgValidityDomain)) {

        errType = XMLValid::errorType((XMLValid::Codes) toEmit);
        msgLoader = gValidMsgLoader;
    }

    // Lock the mutex and load the text
    {
        XMLMutexLock lockInit(&gErrMsgMutex());

        if (!msgLoader->loadMsg(toEmit, errText, msgSize))
        {
                // <TBD> Should probably load a default message here
        }
    }

    if (fErrorReporter)
        fErrorReporter->error(toEmit, msgDomain, errType, errText, aLocator->getSystemId(),
                              aLocator->getPublicId(), aLocator->getLineNumber(),
                              aLocator->getColumnNumber());

    // Bail out if its fatal an we are to give up on the first fatal error
    if (errType == XMLErrorReporter::ErrType_Fatal && fExitOnFirstFatal)
        throw toEmit;
}

void XSDErrorReporter::emitError(const unsigned int toEmit,
                                 const XMLCh* const msgDomain,
                                 const Locator* const aLocator,
                                 const XMLCh* const text1,
                                 const XMLCh* const text2,
                                 const XMLCh* const text3,
                                 const XMLCh* const text4)
{
    // Bump the error count if it is not a warning
//    if (XMLErrs::errorType(toEmit) != XMLErrorReporter::ErrType_Warning)
//        incrementErrorCount();

    //
    //  Load the message into alocal and replace any tokens found in
    //  the text.
    //
    const unsigned int maxChars = 2047;
    XMLCh errText[maxChars + 1];
    XMLMsgLoader* msgLoader = gErrMsgLoader;
    XMLErrorReporter::ErrTypes errType = XMLErrs::errorType((XMLErrs::Codes) toEmit);

    if (!XMLString::compareString(msgDomain, XMLUni::fgValidityDomain)) {

        errType = XMLValid::errorType((XMLValid::Codes) toEmit);
        msgLoader = gValidMsgLoader;
    }

    // Lock the mutex and load the text
    {
        XMLMutexLock lockInit(&gErrMsgMutex());

        if (!msgLoader->loadMsg(toEmit, errText, maxChars, text1, text2, text3, text4))
        {
                // <TBD> Should probably load a default message here
        }
    }

    if (fErrorReporter)
        fErrorReporter->error(toEmit, msgDomain, errType, errText, aLocator->getSystemId(),
                              aLocator->getPublicId(), aLocator->getLineNumber(),
                              aLocator->getColumnNumber());

    // Bail out if its fatal an we are to give up on the first fatal error
    if (errType == XMLErrorReporter::ErrType_Fatal && fExitOnFirstFatal)
        throw toEmit;
}

