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

#include "ElementNSImpl.hpp"
#include "DocumentImpl.hpp"
#include "DOM_DOMException.hpp"

#define null 0

ElementNSImpl::ElementNSImpl(DocumentImpl *ownerDoc, const DOMString &nam) :
    ElementImpl(ownerDoc, name)
{
    this->namespaceURI=null;	//DOM Level 2
    this->prefix=null;          //DOM Level 2
    this->localName=null;       //DOM Level 2
}

//Introduced in DOM Level 2
ElementNSImpl::ElementNSImpl(DocumentImpl *ownerDoc,
                             const DOMString &fNamespaceURI,
                             const DOMString &qualifiedName) :
    ElementImpl(ownerDoc, qualifiedName)
{
    DOMString xmlns = NodeImpl::getXmlnsString();
    DOMString xmlnsURI = NodeImpl::getXmlnsURIString();

    this->ownerDocument=ownerDoc;
    this->name = qualifiedName.clone();

    int index = DocumentImpl::indexofQualifiedName(qualifiedName);
    if (index < 0)
	throw DOM_DOMException(DOM_DOMException::NAMESPACE_ERR, null);
    if (index == 0) {	//qualifiedName contains no ':'
	this -> prefix = null;
	this -> localName = this -> name;
    } else {	//0 < index < this->name.length()-1
	this -> prefix = this->name.substringData(0, index);
	this -> localName =
            this->name.substringData(index+1, this->name.length()-index-1);
    }

    const DOMString& URI =
        mapPrefix(prefix, fNamespaceURI, DOM_Node::ELEMENT_NODE);
    this -> namespaceURI = URI == null ? DOMString(null) : URI.clone();
};  

ElementNSImpl::ElementNSImpl(const ElementNSImpl &other, bool deep) :
    ElementImpl(other, deep)
{
    this->namespaceURI = other.namespaceURI.clone();	//DOM Level 2
    this->prefix = other.prefix.clone();                //DOM Level 2
    this->localName = other.localName.clone();          //DOM Level 2
};

NodeImpl * ElementNSImpl::cloneNode(bool deep) {
    return new ElementNSImpl(*this, deep);
}

DOMString ElementNSImpl::getNamespaceURI()
{
    return namespaceURI;
}

DOMString ElementNSImpl::getPrefix()
{
    return prefix;
}

DOMString ElementNSImpl::getLocalName()
{
    return localName;
}

void ElementNSImpl::setPrefix(const DOMString &fPrefix)
{
    DOMString xml = NodeImpl::getXmlString();
    DOMString xmlURI = NodeImpl::getXmlURIString();
    DOMString xmlns = NodeImpl::getXmlnsString();
    DOMString xmlnsURI = NodeImpl::getXmlnsURIString();

    if (readOnly)
	throw DOM_DOMException(DOM_DOMException::NO_MODIFICATION_ALLOWED_ERR,
                               null);
    if(fPrefix != null && !DocumentImpl::isXMLName(fPrefix))
        throw DOM_DOMException(DOM_DOMException::INVALID_CHARACTER_ERR,null);
    if (namespaceURI == null || localName == null)
	throw DOM_DOMException(DOM_DOMException::NAMESPACE_ERR, null);

    if (fPrefix == null || fPrefix.length() == 0) {
	this -> prefix = null;
	name = localName;
	return;
    }

    XMLCh *p = fPrefix.rawBuffer();
    for (int i = fPrefix.length(); --i >= 0;)
	if (*p++ == chColon)	//prefix is malformed
	    throw DOM_DOMException(DOM_DOMException::NAMESPACE_ERR, null);
    if (fPrefix.equals(xml) && !namespaceURI.equals(xmlURI))
	throw DOM_DOMException(DOM_DOMException::NAMESPACE_ERR, null);

    name = this -> prefix = fPrefix;
    name = name + chColon + localName;    //nodeName is changed too
}
