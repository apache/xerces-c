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

#include "DOMDocumentTypeImpl.hpp"
#include <xercesc/dom/DOMNode.hpp>
#include <xercesc/dom/DOMException.hpp>

#include "DOMNamedNodeMapImpl.hpp"
#include "DOMDocumentImpl.hpp"
#include "DOMCasts.hpp"


DOMDocumentTypeImpl::DOMDocumentTypeImpl(DOMDocument *ownerDoc,
                                   const XMLCh *dtName,
                                   bool heap)
    : fNode(ownerDoc),
    fParent(ownerDoc),
    publicId(0),
    systemId(0),
    name(0),
    intSubsetReading(false),
    entities(0),
    notations(0),
    elements(0),
    internalSubset(0),
    fIsCreatedFromHeap(heap)
{
    if (ownerDoc) {
        name = ((DOMDocumentImpl *)ownerDoc)->getPooledString(dtName);
        entities = new (ownerDoc) DOMNamedNodeMapImpl(this);
        notations= new (ownerDoc) DOMNamedNodeMapImpl(this);
        elements = new (ownerDoc) DOMNamedNodeMapImpl(this);
    }
    else {
        name = XMLString::replicate(dtName);
        entities = new DOMNamedNodeMapImpl(this);
        notations= new DOMNamedNodeMapImpl(this);
        elements = new DOMNamedNodeMapImpl(this);
    }
};


//Introduced in DOM Level 2
DOMDocumentTypeImpl::DOMDocumentTypeImpl(DOMDocument *ownerDoc,
                                   const XMLCh *qualifiedName,
                                   const XMLCh *pubId,
                                   const XMLCh *sysId,
                                   bool heap)
    : fNode(ownerDoc),
    fParent(ownerDoc),
    publicId(0),
    systemId(0),
    name(0),
    intSubsetReading(false),
    entities(0),
    notations(0),
    elements(0),
    internalSubset(0),
    fIsCreatedFromHeap(heap)
{
    if (DOMDocumentImpl::indexofQualifiedName(qualifiedName) < 0)
        throw DOMException(DOMException::NAMESPACE_ERR, 0);

    if (ownerDoc) {
        DOMDocumentImpl *docImpl = (DOMDocumentImpl *)ownerDoc;
        publicId = docImpl->cloneString(pubId);
        systemId = docImpl->cloneString(sysId);
        internalSubset = docImpl->cloneString(internalSubset);
        name = ((DOMDocumentImpl *)ownerDoc)->getPooledString(qualifiedName);
        entities = new (ownerDoc) DOMNamedNodeMapImpl(this);
        notations= new (ownerDoc) DOMNamedNodeMapImpl(this);
        elements = new (ownerDoc) DOMNamedNodeMapImpl(this);
    }
    else {
        publicId = XMLString::replicate(pubId);
        systemId = XMLString::replicate(sysId);
        internalSubset = XMLString::replicate(internalSubset);
        name = XMLString::replicate(qualifiedName);
        entities = new DOMNamedNodeMapImpl(this);
        notations= new DOMNamedNodeMapImpl(this);
        elements = new DOMNamedNodeMapImpl(this);
    }
};


DOMDocumentTypeImpl::DOMDocumentTypeImpl(const DOMDocumentTypeImpl &other, bool heap, bool deep)
    : fNode(other.fNode),
    fParent(other.fParent),
    fChild(other.fChild),
    publicId(0),
    systemId(0),
    name(0),
    intSubsetReading(other.intSubsetReading),
    entities(0),
    notations(0),
    elements(0),
    internalSubset(0),
    fIsCreatedFromHeap(heap)
{
    if ((DOMDocumentImpl *)this->fNode.getOwnerDocument()) {
        name = other.name;
        if (deep)
            fParent.cloneChildren(&other);
        //DOM Level 2
        publicId        = other.publicId;
        systemId        = other.systemId;
        internalSubset  = other.internalSubset;
    }
    else {
        name = XMLString::replicate(other.name);
        publicId = XMLString::replicate(other.publicId);
        systemId = XMLString::replicate(other.systemId);
        internalSubset = XMLString::replicate(other.internalSubset);
    }

    entities = ((DOMNamedNodeMapImpl *)other.entities)->cloneMap(this);
    notations= ((DOMNamedNodeMapImpl *)other.notations)->cloneMap(this);
    elements = ((DOMNamedNodeMapImpl *)other.elements)->cloneMap(this);

}


DOMDocumentTypeImpl::~DOMDocumentTypeImpl()
{
    if (!(castToNodeImpl(this)->getOwnerDocument())) {
        XMLCh* temp = (XMLCh*) name;  // cast off const
        delete [] temp;

        temp = (XMLCh*) publicId;  // cast off const
        delete [] temp;

        temp = (XMLCh*) systemId;
        delete [] temp;

        temp = (XMLCh*) internalSubset;
        delete [] temp;

        delete entities;
        delete notations;
        delete elements;
    }
}


DOMNode *DOMDocumentTypeImpl::cloneNode(bool deep) const
{
    DOMNode* newNode = 0;
    if (castToNodeImpl(this)->getOwnerDocument())
        newNode = new (castToNodeImpl(this)->getOwnerDocument(), DOMDocumentImpl::DOCUMENT_TYPE_OBJECT) DOMDocumentTypeImpl(*this, false, deep);
    else
        newNode = new DOMDocumentTypeImpl(*this, true, deep);

    fNode.callUserDataHandlers(DOMUserDataHandler::NODE_CLONED, this, newNode);
    return newNode;
}

/**
 * NON-DOM
 * set the ownerDocument of this node and its children
 */
void DOMDocumentTypeImpl::setOwnerDocument(DOMDocument *doc) {
    if (castToNodeImpl(this)->getOwnerDocument()) {
        fNode.setOwnerDocument(doc);
        fParent.setOwnerDocument(doc);
    }
    else {
        if (doc) {
            DOMDocumentImpl *docImpl = (DOMDocumentImpl *)doc;

            XMLCh* temp = (XMLCh*) publicId; // cast off const
            publicId = docImpl->cloneString(publicId);
            delete [] temp;

            temp = (XMLCh*) systemId; // cast off const
            systemId = docImpl->cloneString(systemId);
            delete [] temp;

            temp = (XMLCh*) internalSubset; // cast off const
            systemId = docImpl->cloneString(internalSubset);
            delete [] temp;

            temp = (XMLCh*) name; // cast off const
            name = docImpl->cloneString(name);
            delete [] temp;

            fNode.setOwnerDocument(doc);
            fParent.setOwnerDocument(doc);

            DOMNamedNodeMap* entitiesTemp = ((DOMNamedNodeMapImpl *)entities)->cloneMap(this);
            DOMNamedNodeMap* notationsTemp = ((DOMNamedNodeMapImpl *)notations)->cloneMap(this);
            DOMNamedNodeMap* elementsTemp = ((DOMNamedNodeMapImpl *)elements)->cloneMap(this);

            delete entities;
            delete notations;
            delete elements;

            entities = entitiesTemp;
            notations = notationsTemp;
            elements = elementsTemp;

        }
    }
}

const XMLCh * DOMDocumentTypeImpl::getNodeName() const
{
    return name;
};


short DOMDocumentTypeImpl::getNodeType()  const {
    return DOMNode::DOCUMENT_TYPE_NODE;
};


DOMNamedNodeMap *DOMDocumentTypeImpl::getEntities() const
{
    return entities;
};



const XMLCh * DOMDocumentTypeImpl::getName() const
{
    return name;
};


DOMNamedNodeMap *DOMDocumentTypeImpl::getNotations() const
{
    return notations;
};


DOMNamedNodeMap *DOMDocumentTypeImpl::getElements() const
{
    return elements;
};


void DOMDocumentTypeImpl::setNodeValue(const XMLCh *val)
{
    fNode.setNodeValue(val);
};


void DOMDocumentTypeImpl::setReadOnly(bool readOnl, bool deep)
{
    fNode.setReadOnly(readOnl,deep);
    if (entities)
        ((DOMNamedNodeMapImpl *)entities)->setReadOnly(readOnl,true);
    if (notations)
        ((DOMNamedNodeMapImpl *)notations)->setReadOnly(readOnl,true);
};


//Introduced in DOM Level 2

const XMLCh * DOMDocumentTypeImpl::getPublicId() const
{
    return publicId;
}


const XMLCh * DOMDocumentTypeImpl::getSystemId() const
{
    return systemId;
}


const XMLCh * DOMDocumentTypeImpl::getInternalSubset() const
{
    return internalSubset;
}

bool DOMDocumentTypeImpl::isIntSubsetReading() const
{
    return intSubsetReading;
}


//set functions

void        DOMDocumentTypeImpl::setPublicId(const XMLCh *value)
{
    // revist.  Why shouldn't 0 be assigned like any other value?
    if (value == 0)
        return;

    if ((DOMDocumentImpl *)castToNodeImpl(this)->getOwnerDocument())
        publicId = ((DOMDocumentImpl *)castToNodeImpl(this)->getOwnerDocument())->getPooledString(value);
    else {
        XMLCh* temp = (XMLCh*) publicId; // cast off const
        delete [] temp;
        publicId = XMLString::replicate(value);
    }
}

void        DOMDocumentTypeImpl::setSystemId(const XMLCh *value)
{
    if (value == 0)
        return;

    if ((DOMDocumentImpl *)castToNodeImpl(this)->getOwnerDocument())
        systemId = ((DOMDocumentImpl *)castToNodeImpl(this)->getOwnerDocument())->getPooledString(value);
    else {
        XMLCh* temp = (XMLCh*) systemId; // cast off const
        delete [] temp;
        systemId = XMLString::replicate(value);
    }
}

void        DOMDocumentTypeImpl::setInternalSubset(const XMLCh *value)
{
    if (value == 0)
        return;

    if ((DOMDocumentImpl *)castToNodeImpl(this)->getOwnerDocument())
        internalSubset = ((DOMDocumentImpl *)castToNodeImpl(this)->getOwnerDocument())->getPooledString(value);
    else {
        XMLCh* temp = (XMLCh*) internalSubset; // cast off const
        delete [] temp;
        internalSubset = XMLString::replicate(value);
    }
}

void DOMDocumentTypeImpl::release()
{
    if (fNode.isOwned()) {
        if (fNode.isToBeReleased()) {
            if (fIsCreatedFromHeap) {
                fNode.callUserDataHandlers(DOMUserDataHandler::NODE_DELETED, 0, 0);
                DOMDocumentType* docType = this;
                delete docType;
            }
        }
        else
            throw DOMException(DOMException::INVALID_ACCESS_ERR,0);
    }
    else {
        if (fIsCreatedFromHeap) {
            fNode.callUserDataHandlers(DOMUserDataHandler::NODE_DELETED, 0, 0);
            DOMDocumentType* docType = this;
            delete docType;
        }
        else {
            DOMDocumentImpl* doc = (DOMDocumentImpl*) getOwnerDocument();
            if (doc) {
                fNode.callUserDataHandlers(DOMUserDataHandler::NODE_DELETED, 0, 0);
                doc->release(this, DOMDocumentImpl::DOCUMENT_TYPE_OBJECT);
            }
            else {
                // shouldn't reach here
                throw DOMException(DOMException::INVALID_ACCESS_ERR,0);
            }
        }
    }
}


//
// Delegation for functions inherited from Node
//

           DOMNode*         DOMDocumentTypeImpl::appendChild(DOMNode *newChild)          {return fParent.appendChild (newChild); };
           DOMNamedNodeMap* DOMDocumentTypeImpl::getAttributes() const                   {return fNode.getAttributes (); };
           DOMNodeList*     DOMDocumentTypeImpl::getChildNodes() const                   {return fParent.getChildNodes (); };
           DOMNode*         DOMDocumentTypeImpl::getFirstChild() const                   {return fParent.getFirstChild (); };
           DOMNode*         DOMDocumentTypeImpl::getLastChild() const                    {return fParent.getLastChild (); };
     const XMLCh*           DOMDocumentTypeImpl::getLocalName() const                    {return fNode.getLocalName (); };
     const XMLCh*           DOMDocumentTypeImpl::getNamespaceURI() const                 {return fNode.getNamespaceURI (); };
           DOMNode*         DOMDocumentTypeImpl::getNextSibling() const                  {return fChild.getNextSibling (); };
     const XMLCh*           DOMDocumentTypeImpl::getNodeValue() const                    {return fNode.getNodeValue (); };
           DOMDocument*     DOMDocumentTypeImpl::getOwnerDocument() const                {return fNode.getOwnerDocument (); };
     const XMLCh*           DOMDocumentTypeImpl::getPrefix() const                       {return fNode.getPrefix (); };
           DOMNode*         DOMDocumentTypeImpl::getParentNode() const                   {return fChild.getParentNode (this); };
           DOMNode*         DOMDocumentTypeImpl::getPreviousSibling() const              {return fChild.getPreviousSibling (this); };
           bool             DOMDocumentTypeImpl::hasChildNodes() const                   {return fParent.hasChildNodes (); };
           DOMNode*         DOMDocumentTypeImpl::insertBefore(DOMNode *newChild, DOMNode *refChild)
                                                                                         {return fParent.insertBefore (newChild, refChild); };
           void             DOMDocumentTypeImpl::normalize()                             {fParent.normalize (); };
           DOMNode*         DOMDocumentTypeImpl::removeChild(DOMNode *oldChild)          {return fParent.removeChild (oldChild); };
           DOMNode*         DOMDocumentTypeImpl::replaceChild(DOMNode *newChild, DOMNode *oldChild)
                                                                                         {return fParent.replaceChild (newChild, oldChild); };
           bool             DOMDocumentTypeImpl::isSupported(const XMLCh *feature, const XMLCh *version) const
                                                                                         {return fNode.isSupported (feature, version); };
           void             DOMDocumentTypeImpl::setPrefix(const XMLCh  *prefix)         {fNode.setPrefix(prefix); };
           bool             DOMDocumentTypeImpl::hasAttributes() const                   {return fNode.hasAttributes(); };
           bool             DOMDocumentTypeImpl::isSameNode(const DOMNode* other)        {return fNode.isSameNode(other); };
           void*            DOMDocumentTypeImpl::setUserData(const XMLCh* key, void* data, DOMUserDataHandler* handler)
                                                                                         {return fNode.setUserData(key, data, handler); };
           void*            DOMDocumentTypeImpl::getUserData(const XMLCh* key) const     {return fNode.getUserData(key); };
           const XMLCh*     DOMDocumentTypeImpl::getBaseURI() const                      {return fNode.getBaseURI(); };
           short            DOMDocumentTypeImpl::compareTreePosition(DOMNode* other)     {return fNode.compareTreePosition(other); };
           const XMLCh*     DOMDocumentTypeImpl::getTextContent() const                  {return fNode.getTextContent(); };
           void             DOMDocumentTypeImpl::setTextContent(const XMLCh* textContent){fNode.setTextContent(textContent); };
           const XMLCh*     DOMDocumentTypeImpl::lookupNamespacePrefix(const XMLCh* namespaceURI, bool useDefault) {return fNode.lookupNamespacePrefix(namespaceURI, useDefault); };
           bool             DOMDocumentTypeImpl::isDefaultNamespace(const XMLCh* namespaceURI) {return fNode.isDefaultNamespace(namespaceURI); };
           const XMLCh*     DOMDocumentTypeImpl::lookupNamespaceURI(const XMLCh* prefix) {return fNode.lookupNamespaceURI(prefix); };
           DOMNode*         DOMDocumentTypeImpl::getInterface(const XMLCh* feature)      {return fNode.getInterface(feature); };


bool DOMDocumentTypeImpl::isEqualNode(const DOMNode* arg)
{
    if (!fNode.isEqualNode(arg)) {
        return false;
    }

    DOMDocumentType* argDT = (DOMDocumentType*) arg;
    // check the string values
    if (!getPublicId()) {
        if (argDT->getPublicId()) {
            return false;
        }
    }
    else if (XMLString::compareString(getPublicId(), argDT->getPublicId())) {
        return false;
    }

    if (!getSystemId()) {
        if (argDT->getSystemId()) {
            return false;
        }
    }
    else if (XMLString::compareString(getSystemId(), argDT->getSystemId())) {
        return false;
    }

    if (!getInternalSubset()) {
        if (argDT->getInternalSubset()) {
            return false;
        }
    }
    else if (XMLString::compareString(getInternalSubset(), argDT->getInternalSubset())) {
        return false;
    }

    // check the notations
    if (getNotations()) {
        if (!argDT->getNotations())
            return false;

        DOMNamedNodeMap* map1 = getNotations();
        DOMNamedNodeMap* map2 = argDT->getNotations();

        XMLSize_t len = map1->getLength();
        if (len != map2->getLength()) {
            return false;
        }
        for (XMLSize_t i = 0; i < len; i++) {
            DOMNode* n1 = map1->item(i);
            DOMNode* n2 = map2->getNamedItem(n1->getNodeName());
            if (!n2 || !n1->isEqualNode(n2)) {
                return false;
            }
        }
    }
    else {
        if (argDT->getNotations())
            return false;
    }

    // check the entities
    if (getEntities()) {
        if (!argDT->getEntities())
            return false;

        DOMNamedNodeMap* map1 = getEntities();
        DOMNamedNodeMap* map2 = argDT->getEntities();

        XMLSize_t len = map1->getLength();
        if (len != map2->getLength()) {
            return false;
        }
        for (XMLSize_t i = 0; i < len; i++) {
            DOMNode* n1 = map1->item(i);
            DOMNode* n2 = map2->getNamedItem(n1->getNodeName());
            if (!n2 || !n1->isEqualNode(n2)) {
                return false;
            }
        }
    }
    else {
        if (argDT->getEntities())
            return false;
    }

    return fParent.isEqualNode(arg);
};

