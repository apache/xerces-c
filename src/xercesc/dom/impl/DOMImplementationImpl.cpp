/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 2001-2004 The Apache Software Foundation.  All rights
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

/*
 * $Id$
 */

#include "DOMImplementationImpl.hpp"
#include "DOMDocumentImpl.hpp"
#include "DOMDocumentTypeImpl.hpp"
#include "DOMWriterImpl.hpp"

#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMDocumentType.hpp>
#include <xercesc/dom/DOMException.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLUniDefs.hpp>
#include <xercesc/util/XMLChar.hpp>
#include <xercesc/util/XMLRegisterCleanup.hpp>
#include <xercesc/util/XMLStringTokenizer.hpp>
#include <xercesc/util/XMLDOMMsg.hpp>
#include <xercesc/util/XMLMsgLoader.hpp>
#include <xercesc/parsers/DOMBuilderImpl.hpp>

XERCES_CPP_NAMESPACE_BEGIN


// ------------------------------------------------------------
//
//  Static constants.  These are lazily initialized on first usage.
//                     (Static constructors can not be safely used because
//                      of order of initialization dependencies.)
// ------------------------------------------------------------
static const XMLCh  g1_0[] =     // Points to "1.0"
        {chDigit_1, chPeriod, chDigit_0, chNull};
static const XMLCh  g2_0[] =      // Points to "2.0"
        {chDigit_2, chPeriod, chDigit_0, chNull};
static const XMLCh  g3_0[] =      // Points to "3.0"
        {chDigit_3, chPeriod, chDigit_0, chNull};
static const XMLCh  gTrav[] =     // Points to "Traversal"
        {chLatin_T, chLatin_r, chLatin_a, chLatin_v, chLatin_e, chLatin_r,
            chLatin_s, chLatin_a, chLatin_l, chNull};
static const XMLCh  gCore[] =     // Points to "Core"
        {chLatin_C, chLatin_o, chLatin_r, chLatin_e, chNull};
static const XMLCh  gRange[] =     // Points to "Range"
        {chLatin_R, chLatin_a, chLatin_n, chLatin_g, chLatin_e, chNull};
static const XMLCh  gLS[] =     // Points to "LS"
        {chLatin_L, chLatin_S, chNull};


// -----------------------------------------------------------------------
//  Message Loader for DOM
// -----------------------------------------------------------------------
static XMLMsgLoader  *sMsgLoader4DOM = 0;   // Points to the singleton instance
static XMLMutex      *sMutex4DOM = 0;

static void reinitMsgLoader4DOM()
{
	delete sMsgLoader4DOM;
	sMsgLoader4DOM = 0;
}

static void reinitMutex4DOM()
{
	delete sMutex4DOM;
	sMutex4DOM = 0;
}

static XMLMutex& getMutex4DOM()
{
    static XMLRegisterCleanup mutex4DOMCleanup;
    if (!sMutex4DOM)
    {
        XMLMutexLock lock(XMLPlatformUtils::fgAtomicMutex);

        // If we got here first, then register it and set the registered flag
        if (!sMutex4DOM)
        {
            sMutex4DOM = new XMLMutex;
            mutex4DOMCleanup.registerCleanup(reinitMutex4DOM);
        }
    }
    return *sMutex4DOM;
}

XMLMsgLoader* DOMImplementationImpl::getMsgLoader4DOM()
{
    static XMLRegisterCleanup msgLoader4DOMCleanup;
    if (!sMsgLoader4DOM)
    {
        XMLMutexLock lock(&getMutex4DOM());

        if (!sMsgLoader4DOM)
        {
            sMsgLoader4DOM = XMLPlatformUtils::loadMsgSet(XMLUni::fgXMLDOMMsgDomain);

            if (!sMsgLoader4DOM)
                XMLPlatformUtils::panic(PanicHandler::Panic_CantLoadMsgDomain);
            else
                msgLoader4DOMCleanup.registerCleanup(reinitMsgLoader4DOM);
        }
    }

    return sMsgLoader4DOM;
}

// -----------------------------------------------------------------------
//  Singleton DOMImplementationImpl
// -----------------------------------------------------------------------
static DOMImplementationImpl    *gDomimp = 0;   // Points to the singleton instance
                                            //  of DOMImplementation that is returnedreturned
                                            //  by any call to getImplementation().

static void reinitImplementation()
{
	delete gDomimp;
	gDomimp = 0;
}

//  getImplementation()  - Always returns the same singleton instance, which
//                         is lazily created on the first call.  Note that
//                         DOM_Implementation must be thread-safe because
//                         it is common to all DOM documents, and while a single
//                         document is not thread-safe within itself, we do
//                         promise that different documents can safely be
//                         used concurrently by different threads.
//
DOMImplementationImpl *DOMImplementationImpl::getDOMImplementationImpl()
{
    static XMLRegisterCleanup implementationCleanup;
    if (!gDomimp)
    {
        XMLMutexLock lock(&getMutex4DOM());

        if (!gDomimp)
        {
            gDomimp = new DOMImplementationImpl;
            implementationCleanup.registerCleanup(reinitImplementation);
        }
    }

    return gDomimp;
}

// ------------------------------------------------------------
// DOMImplementation Virtual interface
// ------------------------------------------------------------
bool  DOMImplementationImpl::hasFeature(const  XMLCh * feature,  const  XMLCh * version) const
{
    if (!feature)
        return false;

    bool anyVersion = (version == 0 || !*version);
    bool version1_0 = XMLString::equals(version, g1_0);
    bool version2_0 = XMLString::equals(version, g2_0);
    bool version3_0 = XMLString::equals(version, g3_0);

    // Currently, we support only XML Level 1 version 1.0
    if (XMLString::compareIString(feature, XMLUni::fgXMLString) == 0
        && (anyVersion || version1_0 || version2_0))
        return true;

    if (XMLString::compareIString(feature, gCore) == 0
        && (anyVersion || version1_0 || version2_0 || version3_0))
        return true;

    if (XMLString::compareIString(feature, gTrav) == 0
        && (anyVersion || version2_0))
        return true;

    if (XMLString::compareIString(feature, gRange) == 0
        && (anyVersion || version2_0))
        return true;

    if (XMLString::compareIString(feature, gLS) == 0
        && (anyVersion || version3_0))
        return true;

    return false;
}


//Introduced in DOM Level 2
DOMDocumentType *DOMImplementationImpl::createDocumentType(const XMLCh *qualifiedName,
	const XMLCh * publicId, const XMLCh *systemId)
{
    // assume XML 1.0 since we do not know its version yet.
    if(!XMLChar1_0::isValidName(qualifiedName, XMLString::stringLen(qualifiedName)))
        throw DOMException(DOMException::INVALID_CHARACTER_ERR,0);
    DOMDocumentTypeImpl* docType = new DOMDocumentTypeImpl(0, qualifiedName, publicId, systemId, true);
    return docType;
}

DOMDocument *DOMImplementationImpl::createDocument(const XMLCh *namespaceURI,
	const XMLCh *qualifiedName, DOMDocumentType *doctype,
    MemoryManager* const manager)
{
    return new (manager) DOMDocumentImpl(namespaceURI, qualifiedName, doctype, manager);
}


//Introduced in DOM Level 3
DOMImplementation* DOMImplementationImpl::getInterface(const XMLCh*){
    throw DOMException(DOMException::NOT_SUPPORTED_ERR, 0);
    return 0;
}

// Non-standard extension
DOMDocument *DOMImplementationImpl::createDocument(MemoryManager* const manager)
{
        return new (manager) DOMDocumentImpl(manager);
}

//
//  DOMImplementation::getImplementation.  DOMImplementation is supposed to
//                                              be a pure interface class.  This one static
//                                              function is the hook that lets things get started.
DOMImplementation *DOMImplementation::getImplementation()
{
    return (DOMImplementation*) DOMImplementationImpl::getDOMImplementationImpl();
}

bool DOMImplementation::loadDOMExceptionMsg
(
    const   DOMException::ExceptionCode  msgToLoad
    ,       XMLCh* const                 toFill
    , const unsigned int                 maxChars
)
{
    // load the text, the msgToLoad+XMLDOMMsgs::DOMEXCEPTION_ERRX+msgToLoad is the corresponding XMLDOMMsg Code
    return DOMImplementationImpl::getMsgLoader4DOM()->loadMsg(XMLDOMMsg::DOMEXCEPTION_ERRX+msgToLoad, toFill, maxChars);
}

bool DOMImplementation::loadDOMExceptionMsg
(
    const   DOMRangeException::RangeExceptionCode  msgToLoad
    ,       XMLCh* const                           toFill
    , const unsigned int                           maxChars
)
{
    // load the text, the XMLDOMMsgs::DOMRANGEEXCEPTION_ERRX+msgToLoad is the corresponding XMLDOMMsg Code
    return DOMImplementationImpl::getMsgLoader4DOM()->loadMsg(XMLDOMMsg::DOMRANGEEXCEPTION_ERRX+msgToLoad, toFill, maxChars);
}

// ------------------------------------------------------------
// DOMImplementationLS Virtual interface
// ------------------------------------------------------------
//Introduced in DOM Level 3
DOMBuilder* DOMImplementationImpl::createDOMBuilder(const short           mode,
                                                    const XMLCh* const,
                                                    MemoryManager* const  manager,
                                                    XMLGrammarPool* const gramPool)
{
    if (mode == DOMImplementationLS::MODE_ASYNCHRONOUS)
        throw DOMException(DOMException::NOT_SUPPORTED_ERR, 0);

    return new (manager) DOMBuilderImpl(0, manager, gramPool);
}


DOMWriter* DOMImplementationImpl::createDOMWriter(MemoryManager* const manager)
{
    return new (manager) DOMWriterImpl(manager);
}

DOMInputSource* DOMImplementationImpl::createDOMInputSource()
{
    throw DOMException(DOMException::NOT_SUPPORTED_ERR, 0);
    return 0;
}


// ------------------------------------------------------------
// DOMImplementationSource Virtual interface
// ------------------------------------------------------------
DOMImplementation* DOMImplementationImpl::getDOMImplementation(const XMLCh* features) const
{
    DOMImplementation* impl = DOMImplementation::getImplementation();

    XMLStringTokenizer tokenizer(features, XMLPlatformUtils::fgMemoryManager);
    const XMLCh* feature = 0;

    while (feature || tokenizer.hasMoreTokens()) {

        if (!feature)
            feature = tokenizer.nextToken();

        const XMLCh* version = 0;
        const XMLCh* token = tokenizer.nextToken();

        if (token && XMLString::isDigit(token[0]))
            version = token;

        if (!impl->hasFeature(feature, version))
            return 0;

        if (!version)
            feature = token;
    }
    return impl;
}


XERCES_CPP_NAMESPACE_END

