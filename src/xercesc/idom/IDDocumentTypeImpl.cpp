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

#include "IDDocumentTypeImpl.hpp"
#include "IDOM_Node.hpp"
#include "IDNamedNodeMapImpl.hpp"
#include "IDOM_DOMException.hpp"
#include "IDDocumentImpl.hpp"
#include "IDCasts.hpp"


IDDocumentTypeImpl::IDDocumentTypeImpl(IDOM_Document *ownerDoc,
                                   const XMLCh *dtName)
    : fNode(ownerDoc),
    fParent(ownerDoc),
    publicId(0),
    systemId(0),
    name(0),
    internalSubset(0), //DOM Level 2
    intSubsetReading(false),
    entities(0),
    notations(0),
    elements(0)
{
    if (ownerDoc) {
        name = ((IDDocumentImpl *)ownerDoc)->getPooledString(dtName);
        entities = new (ownerDoc) IDNamedNodeMapImpl(this);
        notations= new (ownerDoc) IDNamedNodeMapImpl(this);
        elements = new (ownerDoc) IDNamedNodeMapImpl(this);
    }
    else {
        name = XMLString::replicate(dtName);
        entities = new IDNamedNodeMapImpl(this);
        notations= new IDNamedNodeMapImpl(this);
        elements = new IDNamedNodeMapImpl(this);
    }
};


//Introduced in DOM Level 2
IDDocumentTypeImpl::IDDocumentTypeImpl(IDOM_Document *ownerDoc,
                                   const XMLCh *qualifiedName,
                                   const XMLCh *pubId,
                                   const XMLCh *sysId)
	: fNode(ownerDoc),
    fParent(ownerDoc),
    publicId(0),
    systemId(0),
    name(0),
    internalSubset(0), //DOM Level 2
    intSubsetReading(false),
    entities(0),
    notations(0),
    elements(0)
{
    if (IDDocumentImpl::indexofQualifiedName(qualifiedName) < 0)
        throw IDOM_DOMException(IDOM_DOMException::NAMESPACE_ERR, 0);

    if (ownerDoc) {
        IDDocumentImpl *docImpl = (IDDocumentImpl *)ownerDoc;
        publicId = docImpl->cloneString(pubId);
        systemId = docImpl->cloneString(sysId);
        name = ((IDDocumentImpl *)ownerDoc)->getPooledString(qualifiedName);
        entities = new (ownerDoc) IDNamedNodeMapImpl(this);
        notations= new (ownerDoc) IDNamedNodeMapImpl(this);
        elements = new (ownerDoc) IDNamedNodeMapImpl(this);
    }
    else {
        publicId = XMLString::replicate(pubId);
        systemId = XMLString::replicate(sysId);
        name = XMLString::replicate(qualifiedName);
        entities = new IDNamedNodeMapImpl(this);
        notations= new IDNamedNodeMapImpl(this);
        elements = new IDNamedNodeMapImpl(this);
    }
};


IDDocumentTypeImpl::IDDocumentTypeImpl(const IDDocumentTypeImpl &other, bool deep)
    : fNode(other.fNode),
    fParent(other.fParent),
    fChild(other.fChild),
    publicId(0),
    systemId(0),
    name(0),
    internalSubset(0), //DOM Level 2
    intSubsetReading(other.intSubsetReading),
    entities(0),
    notations(0),
    elements(0)
{
    if ((IDDocumentImpl *)this->fNode.getOwnerDocument()) {
        name = other.name;
        if (deep)
            fParent.cloneChildren(&other);
        //DOM Level 2
        publicId		= other.publicId;
        systemId		= other.systemId;
        internalSubset	= other.internalSubset;
    }
    else {
        name = XMLString::replicate(other.name);
        publicId = XMLString::replicate(other.publicId);
        systemId = XMLString::replicate(other.systemId);
        internalSubset = XMLString::replicate(other.internalSubset);
    }

    entities = ((IDNamedNodeMapImpl *)other.entities)->cloneMap(this);
    notations= ((IDNamedNodeMapImpl *)other.notations)->cloneMap(this);
    elements = ((IDNamedNodeMapImpl *)other.elements)->cloneMap(this);

}


IDDocumentTypeImpl::~IDDocumentTypeImpl()
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


IDOM_Node *IDDocumentTypeImpl::cloneNode(bool deep) const
{
    if (castToNodeImpl(this)->getOwnerDocument())
        return new (castToNodeImpl(this)->getOwnerDocument()) IDDocumentTypeImpl(*this, deep);

    return new IDDocumentTypeImpl(*this, deep);
}

/**
 * NON-DOM
 * set the ownerDocument of this node and its children
 */
void IDDocumentTypeImpl::setOwnerDocument(IDOM_Document *doc) {
    if (castToNodeImpl(this)->getOwnerDocument()) {
        fNode.setOwnerDocument(doc);
        fParent.setOwnerDocument(doc);
    }
    else {
        if (doc) {
            IDDocumentImpl *docImpl = (IDDocumentImpl *)doc;

            XMLCh* temp = (XMLCh*) publicId; // cast off const
            publicId = docImpl->cloneString(publicId);
            delete [] temp;

            temp = (XMLCh*) systemId; // cast off const
            systemId = docImpl->cloneString(systemId);
            delete [] temp;

            temp = (XMLCh*) internalSubset; // cast off const
            internalSubset = docImpl->cloneString(internalSubset);
            delete [] temp;

            temp = (XMLCh*) name; // cast off const
            name = docImpl->cloneString(name);
            delete [] temp;

            fNode.setOwnerDocument(doc);
            fParent.setOwnerDocument(doc);

            IDOM_NamedNodeMap* entitiesTemp = ((IDNamedNodeMapImpl *)entities)->cloneMap(this);
            IDOM_NamedNodeMap* notationsTemp = ((IDNamedNodeMapImpl *)notations)->cloneMap(this);
            IDOM_NamedNodeMap* elementsTemp = ((IDNamedNodeMapImpl *)elements)->cloneMap(this);

            delete entities;
            delete notations;
            delete elements;

            entities = entitiesTemp;
            notations = notationsTemp;
            elements = elementsTemp;

        }
    }
}

const XMLCh * IDDocumentTypeImpl::getNodeName() const
{
    return name;
};


short IDDocumentTypeImpl::getNodeType()  const {
    return IDOM_Node::DOCUMENT_TYPE_NODE;
};


IDOM_NamedNodeMap *IDDocumentTypeImpl::getEntities() const
{
    return entities;
};



const XMLCh * IDDocumentTypeImpl::getName() const
{
    return name;
};


IDOM_NamedNodeMap *IDDocumentTypeImpl::getNotations() const
{
    return notations;
};


IDOM_NamedNodeMap *IDDocumentTypeImpl::getElements() const
{
    return elements;
};


void IDDocumentTypeImpl::setNodeValue(const XMLCh *val)
{
    fNode.setNodeValue(val);
};


void IDDocumentTypeImpl::setReadOnly(bool readOnl, bool deep)
{
    fNode.setReadOnly(readOnl,deep);
    if (entities)
        ((IDNamedNodeMapImpl *)entities)->setReadOnly(readOnl,true);
    if (notations)
        ((IDNamedNodeMapImpl *)notations)->setReadOnly(readOnl,true);
};


//Introduced in DOM Level 2

const XMLCh * IDDocumentTypeImpl::getPublicId() const
{
    return publicId;
}


const XMLCh * IDDocumentTypeImpl::getSystemId() const
{
    return systemId;
}


const XMLCh * IDDocumentTypeImpl::getInternalSubset() const
{
    return internalSubset;
}

bool IDDocumentTypeImpl::isIntSubsetReading() const
{
    return intSubsetReading;
}


//set functions

void        IDDocumentTypeImpl::setPublicId(const XMLCh *value)
{
    // idom_revist.  Why shouldn't 0 be assigned like any other value?
    if (value == 0)
        return;

    if ((IDDocumentImpl *)castToNodeImpl(this)->getOwnerDocument())
        publicId = ((IDDocumentImpl *)castToNodeImpl(this)->getOwnerDocument())->getPooledString(value);
    else {
        XMLCh* temp = (XMLCh*) publicId; // cast off const
        delete [] temp;
        publicId = XMLString::replicate(value);
    }
}

void        IDDocumentTypeImpl::setSystemId(const XMLCh *value)
{
    if (value == 0)
        return;

    if ((IDDocumentImpl *)castToNodeImpl(this)->getOwnerDocument())
        systemId = ((IDDocumentImpl *)castToNodeImpl(this)->getOwnerDocument())->getPooledString(value);
    else {
        XMLCh* temp = (XMLCh*) systemId; // cast off const
        delete [] temp;
        systemId = XMLString::replicate(value);
    }
}

void        IDDocumentTypeImpl::setInternalSubset(const XMLCh *value)
{
    if (value == 0)
        return;

    if ((IDDocumentImpl *)castToNodeImpl(this)->getOwnerDocument())
        internalSubset = ((IDDocumentImpl *)castToNodeImpl(this)->getOwnerDocument())->getPooledString(value);
    else {
        XMLCh* temp = (XMLCh*) internalSubset; // cast off const
        delete [] temp;
        internalSubset = XMLString::replicate(value);
    }
}


//
// Delegation for functions inherited from Node
//

           IDOM_Node          *IDDocumentTypeImpl::appendChild(IDOM_Node *newChild)        {return fParent.appendChild (newChild); };
           IDOM_NamedNodeMap  *IDDocumentTypeImpl::getAttributes() const 			        {return fNode.getAttributes (); };
           IDOM_NodeList      *IDDocumentTypeImpl::getChildNodes() const 			        {return fParent.getChildNodes (); };
           IDOM_Node          *IDDocumentTypeImpl::getFirstChild() const 			        {return fParent.getFirstChild (); };
           IDOM_Node          *IDDocumentTypeImpl::getLastChild() const 		            {return fParent.getLastChild (); };
     const XMLCh              *IDDocumentTypeImpl::getLocalName() const                    {return fNode.getLocalName (); };
     const XMLCh              *IDDocumentTypeImpl::getNamespaceURI() const                 {return fNode.getNamespaceURI (); };
           IDOM_Node          *IDDocumentTypeImpl::getNextSibling() const                  {return fChild.getNextSibling (); };
     const XMLCh              *IDDocumentTypeImpl::getNodeValue() const                    {return fNode.getNodeValue (); };
           IDOM_Document      *IDDocumentTypeImpl::getOwnerDocument() const                {return fNode.getOwnerDocument (); };
     const XMLCh              *IDDocumentTypeImpl::getPrefix() const                       {return fNode.getPrefix (); };
           IDOM_Node          *IDDocumentTypeImpl::getParentNode() const                   {return fChild.getParentNode (this); };
           IDOM_Node          *IDDocumentTypeImpl::getPreviousSibling() const              {return fChild.getPreviousSibling (this); };
           bool                IDDocumentTypeImpl::hasChildNodes() const                   {return fParent.hasChildNodes (); };
           IDOM_Node          *IDDocumentTypeImpl::insertBefore(IDOM_Node *newChild, IDOM_Node *refChild)
                                                                            {return fParent.insertBefore (newChild, refChild); };
           void                IDDocumentTypeImpl::normalize()                             {fParent.normalize (); };
           IDOM_Node          *IDDocumentTypeImpl::removeChild(IDOM_Node *oldChild)        {return fParent.removeChild (oldChild); };
           IDOM_Node          *IDDocumentTypeImpl::replaceChild(IDOM_Node *newChild, IDOM_Node *oldChild)
                                                                            {return fParent.replaceChild (newChild, oldChild); };
           bool                IDDocumentTypeImpl::isSupported(const XMLCh *feature, const XMLCh *version) const
                                                                            {return fNode.isSupported (feature, version); };
           void                IDDocumentTypeImpl::setPrefix(const XMLCh  *prefix)         {fNode.setPrefix(prefix); };
           bool                IDDocumentTypeImpl::hasAttributes() const                   {return fNode.hasAttributes(); };

