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
 * $Id$
 */

#include <util/XMLUniDefs.hpp>
#include "IDAttrNSImpl.hpp"
#include "IDDocumentImpl.hpp"
#include "IDOM_Document.hpp"
#include "IDOM_DOMException.hpp"

#include "assert.h"

static const XMLCh kEmptyString[] = {0};

IDAttrNSImpl::IDAttrNSImpl(IDOM_Document *ownerDoc, const XMLCh *nam) :
IDAttrImpl(ownerDoc, nam)
{
    this->namespaceURI=0;	//DOM Level 2
    this->localName=0;       //DOM Level 2
}

//Introduced in DOM Level 2
IDAttrNSImpl::IDAttrNSImpl(IDOM_Document *ownerDoc,
                           const XMLCh *fNamespaceURI,
                           const XMLCh *qualifiedName) :
IDAttrImpl(ownerDoc, qualifiedName)
{
    const XMLCh * xmlns = IDNodeImpl::getXmlnsString();
    const XMLCh * xmlnsURI = IDNodeImpl::getXmlnsURIString();
    this->fName = ((IDDocumentImpl *)ownerDoc)->getPooledString(qualifiedName);
    
    int index = IDDocumentImpl::indexofQualifiedName(qualifiedName);
    const XMLCh * prefix;
    if (index < 0)
        throw IDOM_DOMException(IDOM_DOMException::NAMESPACE_ERR, 0);
    bool xmlnsAlone = false;	//true if attribute name is "xmlns"
    if (index == 0) {	//qualifiedName contains no ':'
        if (XMLString::compareString(this->fName, xmlns) == 0) {
            if (XMLString::compareString(fNamespaceURI, xmlnsURI) != 0)
                throw IDOM_DOMException(IDOM_DOMException::NAMESPACE_ERR, 0);
            xmlnsAlone = true;
        }
        prefix = 0;
        this -> localName = this -> fName;
    } else {	//0 < index < this->name.length()-1
        XMLCh temp[4000];
        assert (index < 4000);   // idom_revisit.  Do a heap allocation if this fails.
        XMLString::copyNString(temp, fName, index);
        temp[index] = 0;
        prefix = ((IDDocumentImpl *)ownerDoc)->getPooledString(temp);
        //prefix = this->fName.substringData(0, index);
        this -> localName = ((IDDocumentImpl *)ownerDoc)->getPooledString(fName+index+1);
         //   this->fName.substringData(index+1, this->fName.length()-index-1);
    }
    
    const XMLCh * URI = xmlnsAlone ?
                xmlnsURI : IDNodeImpl::mapPrefix(prefix, fNamespaceURI, IDOM_Node::ATTRIBUTE_NODE);
    this -> namespaceURI = URI == 0 ? kEmptyString : ((IDDocumentImpl *)ownerDoc)->getPooledString(URI);
};  

IDAttrNSImpl::IDAttrNSImpl(const IDAttrNSImpl &other, bool deep) :
IDAttrImpl(other, deep)
{
    this->namespaceURI = other.namespaceURI;	//DOM Level 2
    this->localName = other.localName;          //DOM Level 2
};

IDOM_Node * IDAttrNSImpl::cloneNode(bool deep) const
{
    return new (getOwnerDocument()) IDAttrNSImpl(*this, deep);
};

const XMLCh * IDAttrNSImpl::getNamespaceURI() const
{
    return namespaceURI;
}

const XMLCh * IDAttrNSImpl::getPrefix() const
{
    //  idom_revisit.  Get fPrefix set correctly at the start of time.
    if (fPrefix)
        return fPrefix;
    
    int index = IDDocumentImpl::indexofQualifiedName(fName);
    if (index == 0)
        return 0;
    
    IDAttrNSImpl *This = (IDAttrNSImpl *)this;   // cast off const.
    This->fPrefix = new (getOwnerDocument()) XMLCh[index+1];
    XMLString::copyNString(fPrefix, fName, index);
    fPrefix[index] = 0;
    return fPrefix;
}

const XMLCh * IDAttrNSImpl::getLocalName() const
{
    return localName;
}

void IDAttrNSImpl::setPrefix(const XMLCh *prefix)
{
#ifdef idom_revist
    const XMLCh * xml = IDOM_Node::getXmlString();
    const XMLCh * xmlURI = IDOM_Node::getXmlURIString();
    const XMLCh * xmlns = IDOM_Node::getXmlnsString();
    const XMLCh * xmlnsURI = IDOM_Node::getXmlnsURIString();
    
    if (fNode.isReadOnly())
        throw IDOM_DOMException(IDOM_DOMException::NO_MODIFICATION_ALLOWED_ERR,
        0);
    if (namespaceURI == 0 || localName.equals(xmlns))
        throw IDOM_DOMException(IDOM_DOMException::NAMESPACE_ERR, 0);
    
    if (prefix != 0 && !DocumentImpl::isXMLName(prefix))
        throw IDOM_DOMException(IDOM_DOMException::INVALID_CHARACTER_ERR,0);
    
    if (prefix == 0 || prefix.length() == 0) {
        name = localName;
        return;
    }
    
    if (prefix.equals(xml) && !namespaceURI.equals(xmlURI) ||
        prefix.equals(xmlns) && !namespaceURI.equals(xmlnsURI))
        throw IDOM_DOMException(IDOM_DOMException::NAMESPACE_ERR, 0);
    
    const XMLCh *p = prefix.rawBuffer();
    for (int i = prefix.length(); --i >= 0;) {
        if (*p++ == chColon)	//prefix is malformed
            throw IDOM_DOMException(IDOM_DOMException::NAMESPACE_ERR, 0);
    }
    
    name = prefix + chColon + localName; //nodeName is changed too
#endif
    assert(false);  // idom_revisit   setPrefix not implemented yet.
}
