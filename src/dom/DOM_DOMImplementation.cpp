/*
 * The Apache Software License, Version 1.1
 * 
 * Copyright (c) 1999-2000 The Apache Software Foundation.  All rights
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

/*
 * $Log$
 * Revision 1.8  2000/04/27 02:52:42  lehors
 * global reorganization similar to what I've done in Java,
 * nodes now are much smaller.
 * The main changes are:
 * renamed NodeContainer to ParentNode,
 * introduced ChildNode and ChildAndParentNode,
 * all the boolean attributes have been changed to bit flags,
 * ownerDocument is no longer an attribute of NodeImpl, only Parent nodes have
 * it, leave nodes rely on their parent to get it, or get it from ownerNode when
 * they do not have a parent,
 * parent Nodes no longer have a direct pointer to the last child
 * instead the last child is stored as the previous sibling of
 * the first child.
 * I also added support for importing a DocumentType as it's done in Java,
 * and got the importNode mechanism back in sync with Java as well.
 *
 * Here are the most significant changes in size:
 * ElementImpl 52 -> 48
 * TextImpl    44 -> 32
 * AttrImpl    52 -> 36
 *
 * Revision 1.7  2000/03/02 19:53:54  roddey
 * This checkin includes many changes done while waiting for the
 * 1.1.0 code to be finished. I can't list them all here, but a list is
 * available elsewhere.
 *
 * Revision 1.6  2000/02/10 23:35:10  andyh
 * Update DOM_DOMImplementation::CreateDocumentType and
 * DOM_DocumentType to match latest from W3C
 *
 * Revision 1.5  2000/02/06 07:47:28  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.4  2000/01/19 21:39:19  andyh
 * DOM L2, fix problems with new style createDocument.
 *
 * Revision 1.3  2000/01/05 01:16:07  andyh
 * DOM Level 2 core, namespace support added.
 *
 * Revision 1.2  1999/11/30 21:16:25  roddey
 * Changes to add the transcode() method to DOMString, which returns a transcoded
 * version (to local code page) of the DOM string contents. And I changed all of the
 * exception 'throw by pointer' to 'throw by value' style.
 *
 * Revision 1.1.1.1  1999/11/09 01:08:56  twl
 * Initial checkin
 *
 * Revision 1.3  1999/11/08 20:44:15  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */

#include "DOM_DOMImplementation.hpp"
#include "DOM_Document.hpp"
#include "DOM_DocumentType.hpp"
#include "DOM_DOMException.hpp"
#include "DocumentImpl.hpp"
#include "DocumentTypeImpl.hpp"
#include "DStringPool.hpp"
#include "util/PlatformUtils.hpp"

//
//  Static constants.  These are lazily initialized on first usage.
//                     (Static constructors can not be safely used because
//                      of order of initialization dependencies.)

static DOM_DOMImplementation    *gDomimp;   // Points to the singleton instance
                                            //  of DOMImplementation that is returned
                                            //  by any call to getImplementation().

static DOMString                *gXML;      // Points to "XML"
static DOMString                *gxml;      // Points to "xml"
static DOMString                *g1_0;      // Points to "1.0"
static DOMString                *g2_0;      // Points to "2.0"
static DOMString                *gTrav;     // Points to "Traversal"


// Note #1136 - There needs to be a separate implementation class for
//              DOMImplementation, so that the user programming model
//              is consistent with the rest of the C++ DOM API, and
//              so that hasFeature will only work on the result of
//              getImplementation(), and not on DOM_DOMImplemenation objects
//              created with the default constructor.
//
DOM_DOMImplementation::DOM_DOMImplementation() {
};



DOM_DOMImplementation::DOM_DOMImplementation(const DOM_DOMImplementation & other)
{
};


DOM_DOMImplementation::~DOM_DOMImplementation()
{
};


DOM_DOMImplementation & DOM_DOMImplementation::operator = (const DOM_DOMImplementation & other)
{
    return *this;
};


//  getImplementation()  - Always returns the same singleton instance, which
//                         is lazily created on the first call.  Note that
//                         DOM_Implementation must be thread-safe because
//                         it is common to all DOM documents, and while a single
//                         document is not thread-safe within itself, we do
//                         promise that different documents can safely be
//                         used concurrently by different threads.
//
DOM_DOMImplementation &DOM_DOMImplementation::getImplementation() {
    if (gDomimp == 0)
    {
        DOM_DOMImplementation *t = new DOM_DOMImplementation;
        if (XMLPlatformUtils::compareAndSwap((void **)&gDomimp, t, 0) != 0)
            delete t;
    }
    return *gDomimp;
};


bool  DOM_DOMImplementation::hasFeature(const DOMString &feature,  const DOMString &version) 
{
    // Currently, we support only XML Level 1 version 1.0
    // Note #965:  A true case-insensitve compare is needed here.
    if(feature.equals(DStringPool::getStaticString("XML", &gXML)) ||
       feature.equals(DStringPool::getStaticString("xml", &gxml)))
    {
        if(version == null ||
           version.equals(DStringPool::getStaticString("1.0", &g1_0)) ||
           version.equals(DStringPool::getStaticString("2.0", &g2_0)) )
            return true;
    }
    if(feature.equals(DStringPool::getStaticString("Traversal", &gTrav)))
        return true;
    return false;
}


//Introduced in DOM Level 2

DOM_DocumentType DOM_DOMImplementation::createDocumentType(const DOMString &qualifiedName,
	const DOMString &publicId, const DOMString &systemId)
{
    if(!DocumentImpl::isXMLName(qualifiedName))
        throw DOM_DOMException(DOM_DOMException::INVALID_CHARACTER_ERR,null);
    return DOM_DocumentType(new DocumentTypeImpl(null, qualifiedName, publicId, systemId));
}

DOM_Document DOM_DOMImplementation::createDocument(const DOMString &namespaceURI,
	const DOMString &qualifiedName, const DOM_DocumentType &doctype)
{
    return DOM_Document(new DocumentImpl(namespaceURI, qualifiedName,
	doctype == null ? null : (DocumentTypeImpl *) doctype.fImpl));
}
