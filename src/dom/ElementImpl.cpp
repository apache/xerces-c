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

/**
 * $Log$
 * Revision 1.5  2000/02/06 07:47:32  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.4  2000/01/22 01:38:30  andyh
 * Remove compiler warnings in DOM impl classes
 *
 * Revision 1.3  2000/01/05 01:16:08  andyh
 * DOM Level 2 core, namespace support added.
 *
 * Revision 1.2  1999/11/30 21:16:25  roddey
 * Changes to add the transcode() method to DOMString, which returns a transcoded
 * version (to local code page) of the DOM string contents. And I changed all of the
 * exception 'throw by pointer' to 'throw by value' style.
 *
 * Revision 1.1.1.1  1999/11/09 01:09:08  twl
 * Initial checkin
 *
 * Revision 1.3  1999/11/08 20:44:26  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */

#include "DeepNodeListImpl.hpp"
#include "DocumentImpl.hpp"
#include "DocumentTypeImpl.hpp"
#include "DOM_DOMException.hpp"
#include "DStringPool.hpp"
#include "ElementImpl.hpp"
#include "ElementDefinitionImpl.hpp"
#include "NamedNodeMapImpl.hpp"
#include "NodeVector.hpp"


ElementImpl::ElementImpl(DocumentImpl *ownerDoc, const DOMString &nam) :
NodeImpl(ownerDoc, nam, DOM_Node::ELEMENT_NODE, false, null)
{
    
    // If there is an ElementDefintion, set its Attributes up as
    // shadows behind our own.
    NamedNodeMapImpl *defaultAttrs = null;
    DocumentTypeImpl *doctype = (DocumentTypeImpl *) ownerDocument->getDoctype();
    if (doctype != null)
    {
        ElementDefinitionImpl *eldef=(ElementDefinitionImpl *)
            doctype->getElements()->getNamedItem(this->name);
        if(eldef!=null)
            defaultAttrs=(NamedNodeMapImpl *)eldef->getAttributes();
    }
    attributes = new NamedNodeMapImpl(ownerDoc,defaultAttrs);
};

//DOM Level 2
ElementImpl::ElementImpl(DocumentImpl *ownerDoc,
    const DOMString &fNamespaceURI, const DOMString &qualifiedName) :
NodeImpl(ownerDoc, fNamespaceURI, qualifiedName, DOM_Node::ELEMENT_NODE, false, null)
{
    
    // If there is an ElementDefintion, set its Attributes up as
    // shadows behind our own.
    NamedNodeMapImpl *defaultAttrs = null;
    DocumentTypeImpl *doctype = (DocumentTypeImpl *) ownerDocument->getDoctype();
    if (doctype != null)
    {
        ElementDefinitionImpl *eldef=(ElementDefinitionImpl *)
            doctype->getElements()->getNamedItem(this->name);
        if(eldef!=null)
            defaultAttrs=(NamedNodeMapImpl *)eldef->getAttributes();
    }
    attributes = new NamedNodeMapImpl(ownerDoc,defaultAttrs);
}

    
ElementImpl::ElementImpl(const ElementImpl &other, bool deep)
: NodeImpl(other, deep)
{
    attributes = other.attributes->cloneMap();
};


ElementImpl::~ElementImpl()
{
    if (attributes)
    {
        attributes->removeAll();
        NamedNodeMapImpl::removeRef(attributes);
    }
};


NodeImpl *ElementImpl::cloneNode(bool deep)
{
    return new ElementImpl(*this, deep);
};


DOMString ElementImpl::getAttribute(const DOMString &nam)
{
    static DOMString *emptyString = 0;
    AttrImpl * attr=(AttrImpl *)(attributes->getNamedItem(nam));
    return (attr==null) ? DStringPool::getStaticString("", &emptyString) : attr->getValue();
};



AttrImpl *ElementImpl::getAttributeNode(const DOMString &nam)
{
    return (AttrImpl *)(attributes->getNamedItem(nam));
};


NamedNodeMapImpl *ElementImpl::getAttributes()
{
    return attributes;
};



DeepNodeListImpl *ElementImpl::getElementsByTagName(const DOMString &tagname)
{
    return new DeepNodeListImpl(this,tagname);
};


DOMString ElementImpl::getTagName()
{
    return name;
}  


bool ElementImpl::isElementImpl()
{
    return true;
};


void ElementImpl::removeAttribute(const DOMString &nam)
{
    if (readOnly)
        throw DOM_DOMException(
        DOM_DOMException::NO_MODIFICATION_ALLOWED_ERR, null);
    
    AttrImpl *att = (AttrImpl *) attributes->getNamedItem(nam);
    // Remove it (and let the NamedNodeMap recreate the default, if any)
    if (att != null)
    {
        attributes->removeNamedItem(nam);
	att->setOwnerElement(null);	//DOM Level 2
        if (att->nodeRefCount == 0)
            NodeImpl::deleteIf(att);
    }
};



AttrImpl *ElementImpl::removeAttributeNode(AttrImpl *oldAttr)
{
    if (readOnly)
        throw DOM_DOMException(
        DOM_DOMException::NO_MODIFICATION_ALLOWED_ERR, null);
    
    AttrImpl *found = (AttrImpl *) attributes->getNamedItem(oldAttr->getName());
    
    // If it is in fact the right object, remove it (and let the
    // NamedNodeMap recreate the default, if any)
    
    if (found == oldAttr)
    {
        attributes->removeNamedItem(oldAttr->getName());
	found->setOwnerElement(null);	//DOM Level 2
        return found;
    }
    else
        throw DOM_DOMException(DOM_DOMException::NOT_FOUND_ERR, null);
	return null;	// just to keep the compiler happy
};



void ElementImpl::setAttribute(const DOMString &nam, const DOMString &val)
{
    if (readOnly)
        throw DOM_DOMException(
        DOM_DOMException::NO_MODIFICATION_ALLOWED_ERR, null);
    
    AttrImpl *newAttr = (AttrImpl *) ownerDocument->createAttribute(nam);
    newAttr->setNodeValue(val);
    newAttr->setOwnerElement(this);	//DOM Level 2
    AttrImpl *oldAttr = (AttrImpl *)attributes->setNamedItem(newAttr);

    if (oldAttr) {
	oldAttr->setOwnerElement(null);	//DOM Level 2
	if (oldAttr->nodeRefCount == 0)
	    NodeImpl::deleteIf(oldAttr);
    }
};



AttrImpl * ElementImpl::setAttributeNode(AttrImpl *newAttr)
{
    if (readOnly)
        throw DOM_DOMException(
        DOM_DOMException::NO_MODIFICATION_ALLOWED_ERR, null);
    
    if (!(newAttr->isAttrImpl()))
        throw DOM_DOMException(DOM_DOMException::WRONG_DOCUMENT_ERR, null);
    AttrImpl *oldAttr = (AttrImpl *) attributes->getNamedItem(newAttr->getName());
    if (oldAttr)
	oldAttr->setOwnerElement(null);	    //DOM Level 2
    
    // This will throw INUSE if necessary
    attributes->setNamedItem(newAttr);
    
    // Attr node reference counting note:
    // If oldAttr's refcount is zero at this point, here's what happens...
    // 
    //      oldAttr is returned from this function to DOM_Attr::setAttributeNode,
    //      which wraps a DOM_Attr around the returned pointer and sends it
    //      up to application code, incrementing the reference count in the process.
    //      When the app DOM_Attr's destructor runs, the reference count is
    //      decremented back to zero and oldAttr will be deleted at that time.

    return oldAttr;
};


void ElementImpl::setNodeValue(const DOMString &x)
{
    throw DOM_DOMException(DOM_DOMException::NO_MODIFICATION_ALLOWED_ERR, null);
};



void ElementImpl::setReadOnly(bool readOnl, bool deep)
{
    NodeImpl::setReadOnly(readOnl,deep);
    attributes->setReadOnly(readOnl,true);
};


//Introduced in DOM Level 2
DOMString ElementImpl::getAttributeNS(const DOMString &fNamespaceURI,
	const DOMString &fLocalName)
{
    static DOMString *emptyString = 0;
    AttrImpl * attr=(AttrImpl *)(attributes->getNamedItemNS(fNamespaceURI, fLocalName));
    return (attr==null) ? DStringPool::getStaticString("", &emptyString) : attr->getValue();
}


void ElementImpl::setAttributeNS(const DOMString &fNamespaceURI,
	const DOMString &qualifiedName, const DOMString &fValue)
{
    if (readOnly)
        throw DOM_DOMException(
	    DOM_DOMException::NO_MODIFICATION_ALLOWED_ERR, null);
    
    AttrImpl *newAttr = (AttrImpl *) ownerDocument->createAttributeNS(fNamespaceURI, qualifiedName);
    newAttr->setNodeValue(fValue);
    newAttr->setOwnerElement(this);
    AttrImpl *oldAttr = (AttrImpl *)attributes->setNamedItem(newAttr);

    if (oldAttr) {
	oldAttr->setOwnerElement(null);
	if (oldAttr->nodeRefCount == 0)
	    NodeImpl::deleteIf(oldAttr);
    }
}


void ElementImpl::removeAttributeNS(const DOMString &fNamespaceURI,
	const DOMString &fLocalName)
{
    if (readOnly)
        throw DOM_DOMException(
	    DOM_DOMException::NO_MODIFICATION_ALLOWED_ERR, null);
    
    AttrImpl *att = (AttrImpl *) attributes->getNamedItemNS(fNamespaceURI, fLocalName);
    // Remove it (and let the NamedNodeMap recreate the default, if any)
    if (att != null) {
        attributes->removeNamedItemNS(namespaceURI, localName);
	att->setOwnerElement(null);
        if (att->nodeRefCount == 0)
            NodeImpl::deleteIf(att);
    }
}


AttrImpl *ElementImpl::getAttributeNodeNS(const DOMString &fNamespaceURI,
	const DOMString &fLocalName)
{
    return (AttrImpl *)(attributes->getNamedItemNS(fNamespaceURI, fLocalName));
}


AttrImpl *ElementImpl::setAttributeNodeNS(AttrImpl *newAttr)
{
    if (readOnly)
        throw DOM_DOMException(
	    DOM_DOMException::NO_MODIFICATION_ALLOWED_ERR, null);
    
    if (newAttr -> getOwnerDocument() != this -> getOwnerDocument())
        throw DOM_DOMException(DOM_DOMException::WRONG_DOCUMENT_ERR, null);
    AttrImpl *oldAttr = (AttrImpl *) attributes->getNamedItemNS(
	newAttr->getNamespaceURI(), newAttr->getLocalName());
    if (oldAttr)
	oldAttr->setOwnerElement(null);
    
    // This will throw INUSE if necessary
    attributes->setNamedItem(newAttr);
    
    // Attr node reference counting note:
    // If oldAttr's refcount is zero at this point, here's what happens...
    // 
    //      oldAttr is returned from this function to DOM_Attr::setAttributeNode,
    //      which wraps a DOM_Attr around the returned pointer and sends it
    //      up to application code, incrementing the reference count in the process.
    //      When the app DOM_Attr's destructor runs, the reference count is
    //      decremented back to zero and oldAttr will be deleted at that time.

    return oldAttr;
}


DeepNodeListImpl *ElementImpl::getElementsByTagNameNS(const DOMString &fNamespaceURI,
	const DOMString &fLocalName)
{
    return new DeepNodeListImpl(this,fNamespaceURI, fLocalName);
}

