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
  * Revision 1.11  2003/12/24 15:24:16  cargilld
  * More updates to memory management so that the static memory manager.
  *
  * Revision 1.10  2003/03/09 17:05:01  peiyongz
  * PanicHandler
  *
  * Revision 1.9  2003/01/13 16:13:37  knoaman
  * We should load the validation message set.
  *
  * Revision 1.8  2003/01/09 15:30:09  tng
  * Performance: construct message loader only when required.
  *
  * Revision 1.7  2002/12/04 02:32:43  knoaman
  * #include cleanup.
  *
  * Revision 1.6  2002/11/27 18:05:38  tng
  * Schema Fix: cast the toEmit back to XMLErrs:Codes so that it can be caught by the Scanner properly.
  *
  * Revision 1.5  2002/11/15 21:58:04  peiyongz
  * Leave thread safety issue to message loader
  *
  * Revision 1.4  2002/11/04 14:49:42  tng
  * C++ Namespace Support.
  *
  * Revision 1.3  2002/09/24 20:12:48  tng
  * Performance: use XMLString::equals instead of XMLString::compareString
  *
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
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/framework/XMLErrorCodes.hpp>
#include <xercesc/framework/XMLValidityCodes.hpp>
#include <xercesc/framework/XMLErrorReporter.hpp>
#include <xercesc/util/XMLMsgLoader.hpp>
#include <xercesc/util/XMLRegisterCleanup.hpp>
#include <xercesc/validators/schema/XSDErrorReporter.hpp>
#include <xercesc/validators/schema/XSDLocator.hpp>

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  Local static data
// ---------------------------------------------------------------------------
static XMLMsgLoader*      gErrMsgLoader = 0;
static XMLMsgLoader*      gValidMsgLoader = 0;


// ---------------------------------------------------------------------------
//  Local, static functions
// ---------------------------------------------------------------------------
static void reinitErrMsgLoader()
{
	delete gErrMsgLoader;
	gErrMsgLoader = 0;
}

static void reinitValidMsgLoader()
{
	delete gValidMsgLoader;
	gValidMsgLoader = 0;
}

static XMLMsgLoader* getErrMsgLoader()
{
    static XMLRegisterCleanup cleanupErrMsgLoader;
    if (gErrMsgLoader == 0)
    {
        XMLMsgLoader* t = XMLPlatformUtils::loadMsgSet(XMLUni::fgXMLErrDomain);
        if (!t)
            XMLPlatformUtils::panic(PanicHandler::Panic_CantLoadMsgDomain);
        else {
            if (XMLPlatformUtils::compareAndSwap((void **)&gErrMsgLoader, t, 0) != 0)
            {
                delete t;
            }
            else
            {
                cleanupErrMsgLoader.registerCleanup(reinitErrMsgLoader);
            }
        }
    }
    return gErrMsgLoader;
}


static XMLMsgLoader* getValidMsgLoader()
{
    static XMLRegisterCleanup cleanupValidMsgLoader;
    if (gValidMsgLoader == 0)
    {
        XMLMsgLoader* t = XMLPlatformUtils::loadMsgSet(XMLUni::fgValidityDomain);
        if (!t)
            XMLPlatformUtils::panic(PanicHandler::Panic_CantLoadMsgDomain);
        else {
            if (XMLPlatformUtils::compareAndSwap((void **)&gValidMsgLoader, t, 0) != 0)
            {
                delete t;
            }
            else
            {
                cleanupValidMsgLoader.registerCleanup(reinitValidMsgLoader);
            }
        }
    }
    return gValidMsgLoader;
}

// ---------------------------------------------------------------------------
//  XSDErrorReporter: Constructors and Destructor
// ---------------------------------------------------------------------------
XSDErrorReporter::XSDErrorReporter(XMLErrorReporter* const errorReporter) :
    fExitOnFirstFatal(false)
    , fErrorReporter(errorReporter)
{

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
    XMLMsgLoader* msgLoader = getErrMsgLoader();
    XMLErrorReporter::ErrTypes errType = XMLErrs::errorType((XMLErrs::Codes) toEmit);

    if (XMLString::equals(msgDomain, XMLUni::fgValidityDomain)) {

        errType = XMLValid::errorType((XMLValid::Codes) toEmit);
        msgLoader = getValidMsgLoader();
    }

    if (!msgLoader->loadMsg(toEmit, errText, msgSize))
    {
                // <TBD> Should probably load a default message here
    }

    if (fErrorReporter)
        fErrorReporter->error(toEmit, msgDomain, errType, errText, aLocator->getSystemId(),
                              aLocator->getPublicId(), aLocator->getLineNumber(),
                              aLocator->getColumnNumber());

    // Bail out if its fatal an we are to give up on the first fatal error
    if (errType == XMLErrorReporter::ErrType_Fatal && fExitOnFirstFatal)
        throw (XMLErrs::Codes) toEmit;
}

void XSDErrorReporter::emitError(const unsigned int toEmit,
                                 const XMLCh* const msgDomain,
                                 const Locator* const aLocator,
                                 const XMLCh* const text1,
                                 const XMLCh* const text2,
                                 const XMLCh* const text3,
                                 const XMLCh* const text4,
                                 MemoryManager* const manager)
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
    XMLMsgLoader* msgLoader = getErrMsgLoader();
    XMLErrorReporter::ErrTypes errType = XMLErrs::errorType((XMLErrs::Codes) toEmit);

    if (XMLString::equals(msgDomain, XMLUni::fgValidityDomain)) {

        errType = XMLValid::errorType((XMLValid::Codes) toEmit);
        msgLoader = getValidMsgLoader();
    }

    if (!msgLoader->loadMsg(toEmit, errText, maxChars, text1, text2, text3, text4, manager))
    {
                // <TBD> Should probably load a default message here
    }

    if (fErrorReporter)
        fErrorReporter->error(toEmit, msgDomain, errType, errText, aLocator->getSystemId(),
                              aLocator->getPublicId(), aLocator->getLineNumber(),
                              aLocator->getColumnNumber());

    // Bail out if its fatal an we are to give up on the first fatal error
    if (errType == XMLErrorReporter::ErrType_Fatal && fExitOnFirstFatal)
        throw (XMLErrs::Codes) toEmit;
}

XERCES_CPP_NAMESPACE_END
