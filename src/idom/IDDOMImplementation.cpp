/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 2001 The Apache Software Foundation.  All rights
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
 * $Log$
 * Revision 1.5  2002/01/23 20:12:43  tng
 * Update DOM/IDOM hasFeature method to correctly reflect current status.
 *
 * Revision 1.4  2001/10/23 23:05:03  peiyongz
 * [Bug#880] patch to PlatformUtils:init()/term() and related. from Mark Weaver
 *
 * Revision 1.3  2001/05/23 13:11:37  tng
 * IDOM: Memory fix.
 *
 * Revision 1.2  2001/05/11 13:25:40  tng
 * Copyright update.
 *
 * Revision 1.1.1.1  2001/04/03 00:14:21  andyh
 * IDOM
 *
 */

#include "IDDOMImplementation.hpp"
#include "IDOM_Document.hpp"
#include "IDOM_DocumentType.hpp"
#include "IDOM_DOMException.hpp"
#include "IDDocumentImpl.hpp"
#include "IDDocumentTypeImpl.hpp"
#include <util/PlatformUtils.hpp>
#include <util/XMLUniDefs.hpp>
#include <util/XMLRegisterCleanup.hpp>

//
//  Static constants.  These are lazily initialized on first usage.
//                     (Static constructors can not be safely used because
//                      of order of initialization dependencies.)


static IDDOMImplementation    *gDomimp;   // Points to the singleton instance
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


IDDOMImplementation::IDDOMImplementation() {
};



IDDOMImplementation::IDDOMImplementation(const IDDOMImplementation & other)
{
};


IDDOMImplementation::~IDDOMImplementation()
{
};


IDDOMImplementation & IDDOMImplementation::operator = (const IDDOMImplementation & other)
{
    return *this;
};


// -----------------------------------------------------------------------
//  Reset the singleton IDDOMImplementation
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
IDOM_DOMImplementation *IDDOMImplementation::getImplementation() {
	static XMLRegisterCleanup implementationCleanup;

    if (gDomimp == 0)
    {
        IDDOMImplementation *t = new IDDOMImplementation;
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
//  IDOM_DOMImplementation::getImplementation.  IDOM_DOMImplementation is supposed to
//                                              be a pure interface class.  This one static
//                                              function is the hook that lets things get started.
IDOM_DOMImplementation *IDOM_DOMImplementation::getImplementation()
{
    return IDDOMImplementation::getImplementation();
}



bool  IDDOMImplementation::hasFeature(const  XMLCh * feature,  const  XMLCh * version)
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

IDOM_DocumentType *IDDOMImplementation::createDocumentType(const XMLCh *qualifiedName,
	const XMLCh * publicId, const XMLCh *systemId)
{
    if(!IDDocumentImpl::isXMLName(qualifiedName))
        throw IDOM_DOMException(IDOM_DOMException::INVALID_CHARACTER_ERR,0);
    return new IDDocumentTypeImpl(0, qualifiedName, publicId, systemId);
}

IDOM_Document *IDDOMImplementation::createDocument(const XMLCh *namespaceURI,
	const XMLCh *qualifiedName, IDOM_DocumentType *doctype)
{
    return new IDDocumentImpl(namespaceURI, qualifiedName, doctype);
}



IDOM_Document *IDDOMImplementation::createDocument()
{
        return new IDDocumentImpl();
}

