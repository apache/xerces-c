/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 2001-2002 The Apache Software Foundation.  All rights
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
#include <xercesc/util/XMLRegisterCleanup.hpp>
#include <xercesc/parsers/DOMBuilderImpl.hpp>

//
//  Static constants.  These are lazily initialized on first usage.
//                     (Static constructors can not be safely used because
//                      of order of initialization dependencies.)


static DOMImplementationImpl    *gDomimp;   // Points to the singleton instance
                                            //  of DOMImplementation that is returnedreturned
                                            //  by any call to getImplementation().

static const XMLCh  gXML[] =      // Points to "XML"
        {chLatin_X, chLatin_M, chLatin_L, 0};

static const XMLCh  g1_0[] =     // Points to "1.0"
        {chDigit_1, chPeriod, chDigit_0, 0};
static const XMLCh  g2_0[] =      // Points to "2.0"
        {chDigit_2, chPeriod, chDigit_0, 0};
static const XMLCh  gTrav[] =     // Points to "Traversal"
        {chLatin_T, chLatin_r, chLatin_a, chLatin_v, chLatin_e, chLatin_r,
            chLatin_s, chLatin_a, chLatin_l, 0};
static const XMLCh  gCore[] =     // Points to "Core"
        {chLatin_C, chLatin_o, chLatin_r, chLatin_e, 0};
static const XMLCh  gRange[] =     // Points to "Range"
        {chLatin_R, chLatin_a, chLatin_n, chLatin_g, chLatin_e, 0};


DOMImplementationImpl::DOMImplementationImpl() {
};



DOMImplementationImpl::DOMImplementationImpl(const DOMImplementationImpl & other)
{
};


DOMImplementationImpl::~DOMImplementationImpl()
{
};


DOMImplementationImpl & DOMImplementationImpl::operator = (const DOMImplementationImpl & other)
{
    return *this;
};


// -----------------------------------------------------------------------
//  Reset the singleton DOMImplementationImpl
// -----------------------------------------------------------------------
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
DOMImplementation *DOMImplementationImpl::getImplementation() {
	static XMLRegisterCleanup implementationCleanup;

    if (gDomimp == 0)
    {
        DOMImplementationImpl *t = new DOMImplementationImpl;
        if (XMLPlatformUtils::compareAndSwap((void **)&gDomimp, t, 0) != 0)
        {
            delete t;
        }
        else
        {
			implementationCleanup.registerCleanup(reinitImplementation);
        }

    }
    return gDomimp;
};

//
//  DOMImplementation::getImplementation.  DOMImplementation is supposed to
//                                              be a pure interface class.  This one static
//                                              function is the hook that lets things get started.
DOMImplementation *DOMImplementation::getImplementation()
{
    return DOMImplementationImpl::getImplementation();
}



bool  DOMImplementationImpl::hasFeature(const  XMLCh * feature,  const  XMLCh * version)
{
    bool anyVersion = (version == 0 || XMLString::stringLen(version) == 0);
    bool version1_0 = (XMLString::compareString(version, g1_0) == 0);
    bool version2_0 = (XMLString::compareString(version, g2_0) == 0);

    // Currently, we support only XML Level 1 version 1.0
    if (XMLString::compareIString(feature, gXML) == 0
        && (anyVersion || version1_0 || version2_0))
        return true;

    if (XMLString::compareIString(feature, gCore) == 0
        && (anyVersion || version1_0 || version2_0))
        return true;

    if (XMLString::compareIString(feature, gTrav) == 0
        && (anyVersion || version2_0))
        return true;

    if (XMLString::compareIString(feature, gRange) == 0
        && (anyVersion || version2_0))
        return true;

    return false;
}


//Introduced in DOM Level 2

DOMDocumentType *DOMImplementationImpl::createDocumentType(const XMLCh *qualifiedName,
	const XMLCh * publicId, const XMLCh *systemId)
{
    if(!DOMDocumentImpl::isXMLName(qualifiedName))
        throw DOMException(DOMException::INVALID_CHARACTER_ERR,0);
    return new DOMDocumentTypeImpl(0, qualifiedName, publicId, systemId);
}

DOMDocument *DOMImplementationImpl::createDocument(const XMLCh *namespaceURI,
	const XMLCh *qualifiedName, DOMDocumentType *doctype)
{
    return new DOMDocumentImpl(namespaceURI, qualifiedName, doctype);
}



DOMDocument *DOMImplementationImpl::createDocument()
{
        return new DOMDocumentImpl();
}


// DOM LS

DOMBuilder* DOMImplementationImpl::createDOMBuilder(const short mode,
                                                    const XMLCh* const schemaType)
{
    if (mode == DOMImplementationLS::MODE_ASYNCHRONOUS)
        throw DOMException(DOMException::NOT_SUPPORTED_ERR, 0);

    return new DOMBuilderImpl();
}


DOMWriter* DOMImplementationImpl::createDOMWriter()
{
    return new DOMWriterImpl();
}

DOMInputSource* DOMImplementationImpl::createDOMInputSource()
{
    throw DOMException(DOMException::NOT_SUPPORTED_ERR, 0);
}

