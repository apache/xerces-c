/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 1999-2002 The Apache Software Foundation.  All rights
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

#include <xercesc/util/XMLUniDefs.hpp>
#include "AttrNSImpl.hpp"
#include "DocumentImpl.hpp"
#include "DOM_DOMException.hpp"

XERCES_CPP_NAMESPACE_BEGIN



AttrNSImpl::AttrNSImpl(DocumentImpl *ownerDoc, const DOMString &nam) :
    AttrImpl(ownerDoc, nam)
{
    this->namespaceURI=null;	//DOM Level 2
    this->localName=null;       //DOM Level 2
}

//Introduced in DOM Level 2
AttrNSImpl::AttrNSImpl(DocumentImpl *ownerDoc,
                       const DOMString &fNamespaceURI,
                       const DOMString &qualifiedName) :
    AttrImpl(ownerDoc, qualifiedName)
{
    DOMString xmlns = NodeImpl::getXmlnsString();
    DOMString xmlnsURI = NodeImpl::getXmlnsURIString();
    this->name = qualifiedName.clone();

    int index = DocumentImpl::indexofQualifiedName(qualifiedName);
    DOMString prefix;
    if (index < 0)
	throw DOM_DOMException(DOM_DOMException::NAMESPACE_ERR, null);
    bool xmlnsAlone = false;	//true if attribute name is "xmlns"
    if (index == 0) {	//qualifiedName contains no ':'
        if (this->name.equals(xmlns)) {
	    if (!fNamespaceURI.equals(xmlnsURI))
		throw DOM_DOMException(DOM_DOMException::NAMESPACE_ERR, null);
	    xmlnsAlone = true;
	}
	prefix = null;
	this -> localName = this -> name;
    } else {	//0 < index < this->name.length()-1
	prefix = this->name.substringData(0, index);
	this -> localName =
            this->name.substringData(index+1, this->name.length()-index-1);
    }

    const DOMString& URI = xmlnsAlone ?
        xmlnsURI : mapPrefix(prefix, fNamespaceURI, DOM_Node::ATTRIBUTE_NODE);
    this -> namespaceURI = URI == null ? DOMString(null) : URI.clone();
};

AttrNSImpl::AttrNSImpl(const AttrNSImpl &other, bool deep) :
    AttrImpl(other, deep)
{
    this->namespaceURI = other.namespaceURI.clone();	//DOM Level 2
    this->localName = other.localName.clone();          //DOM Level 2
};

NodeImpl * AttrNSImpl::cloneNode(bool deep)
{
    return new (getOwnerDocument()->getMemoryManager()) AttrNSImpl(*this, deep);
};

DOMString AttrNSImpl::getNamespaceURI()
{
    return namespaceURI;
}

DOMString AttrNSImpl::getPrefix()
{
    int index = DocumentImpl::indexofQualifiedName(name);
    if (index == 0)
        return null;
    else
        return name.substringData(0, index);
}

DOMString AttrNSImpl::getLocalName()
{
    return localName;
}

void AttrNSImpl::setPrefix(const DOMString &prefix)
{
    DOMString xml = NodeImpl::getXmlString();
    DOMString xmlURI = NodeImpl::getXmlURIString();
    DOMString xmlns = NodeImpl::getXmlnsString();
    DOMString xmlnsURI = NodeImpl::getXmlnsURIString();

    if (getOwnerDocument()->getErrorChecking()) {
        if (isReadOnly()) {
            throw DOM_DOMException(
                                 DOM_DOMException::NO_MODIFICATION_ALLOWED_ERR,
                                 null);
        }
        if (namespaceURI == null || localName.equals(xmlns)) {
            throw DOM_DOMException(DOM_DOMException::NAMESPACE_ERR, null);
        }
        if (prefix != null && !((DocumentImpl *)this->getOwnerDocument())->isXMLName(prefix)) {
            throw DOM_DOMException(DOM_DOMException::INVALID_CHARACTER_ERR,
                                   null);
        }
    }
    if (prefix == null || prefix.length() == 0) {
        name = localName;
        return;
    }
    if (getOwnerDocument()->getErrorChecking() &&
        (prefix.equals(xml) && !namespaceURI.equals(xmlURI) ||
         prefix.equals(xmlns) && !namespaceURI.equals(xmlnsURI))) {
        throw DOM_DOMException(DOM_DOMException::NAMESPACE_ERR, null);
    }
    const XMLCh *p = prefix.rawBuffer();
    for (int i = prefix.length(); --i >= 0;)
        if (*p++ == chColon)	//prefix is malformed
            throw DOM_DOMException(DOM_DOMException::NAMESPACE_ERR, null);

    name = prefix + chColon + localName; //nodeName is changed too
}

XERCES_CPP_NAMESPACE_END

