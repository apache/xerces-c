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
    : fNode(ownerDoc), fParent(ownerDoc),
    publicId(0), systemId(0), internalSubset(0) //DOM Level 2
	, intSubsetReading(false)	
{
    name = ((IDDocumentImpl *)ownerDoc)->getPooledString(dtName);
    entities = new (ownerDoc) IDNamedNodeMapImpl(this);
    notations = new (ownerDoc) IDNamedNodeMapImpl(this);

};


//Introduced in DOM Level 2
IDDocumentTypeImpl::IDDocumentTypeImpl(IDOM_Document *ownerDoc,
                                   const XMLCh *qualifiedName,
                                   const XMLCh *pubId,
                                   const XMLCh *sysId)
	: fNode(ownerDoc),
    fParent(ownerDoc)
{
    IDDocumentImpl *docImpl = (IDDocumentImpl *)ownerDoc;
    publicId = docImpl->cloneString(pubId);
    systemId = docImpl->cloneString(sysId);
    internalSubset = 0;
    intSubsetReading = false;

    name = ((IDDocumentImpl *)ownerDoc)->getPooledString(qualifiedName);
    if (IDDocumentImpl::indexofQualifiedName(qualifiedName) < 0)
        throw IDOM_DOMException(IDOM_DOMException::NAMESPACE_ERR, 0);

    entities = new (ownerDoc) IDNamedNodeMapImpl(this);
    notations= new (ownerDoc) IDNamedNodeMapImpl(this);
};


IDDocumentTypeImpl::IDDocumentTypeImpl(const IDDocumentTypeImpl &other, bool deep)
    : fNode(other.fNode), fParent(other.fParent), fChild(other.fChild)
{
    name = other.name;
    if (deep)
        fParent.cloneChildren(&other);
    entities = ((IDNamedNodeMapImpl *)other.entities)->cloneMap(this);
    notations= ((IDNamedNodeMapImpl *)other.notations)->cloneMap(this);

    //DOM Level 2
    publicId		= other.publicId;
    systemId		= other.systemId;
	internalSubset	= other.internalSubset;
	intSubsetReading = other.intSubsetReading;
}


IDDocumentTypeImpl::~IDDocumentTypeImpl()
{
}


IDOM_Node *IDDocumentTypeImpl::cloneNode(bool deep) const
{
    return new (castToNodeImpl(this)->getOwnerDocument()) IDDocumentTypeImpl(*this, deep);
}

/**
 * NON-DOM
 * set the ownerDocument of this node and its children
 */
void IDDocumentTypeImpl::setOwnerDocument(IDOM_Document *doc) {
    fNode.setOwnerDocument(doc);
    // idom_revisit.  Who deletes this guy, if he's not on the document heap?
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


void IDDocumentTypeImpl::setNodeValue(const XMLCh *val)
{
    throw IDOM_DOMException(
        IDOM_DOMException::NO_MODIFICATION_ALLOWED_ERR, 0);
};


void IDDocumentTypeImpl::setReadOnly(bool readOnl, bool deep)
{
    fNode.setReadOnly(readOnl,deep);
    ((IDNamedNodeMapImpl *)entities)->setReadOnly(readOnl,true);
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
    publicId = ((IDDocumentImpl *)this->fNode.getOwnerDocument())->getPooledString(value);
}

void        IDDocumentTypeImpl::setSystemId(const XMLCh *value)
{
    if (value == 0)
        return;
    systemId = ((IDDocumentImpl *)this->fNode.getOwnerDocument())->getPooledString(value);
}

void        IDDocumentTypeImpl::setInternalSubset(const XMLCh *value)
{
    if (value == 0)
        return;
    internalSubset = ((IDDocumentImpl *)this->fNode.getOwnerDocument())->getPooledString(value);
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
           void                IDDocumentTypeImpl::normalize()                             {fNode.normalize (); };
           IDOM_Node          *IDDocumentTypeImpl::removeChild(IDOM_Node *oldChild)        {return fParent.removeChild (oldChild); };
           IDOM_Node          *IDDocumentTypeImpl::replaceChild(IDOM_Node *newChild, IDOM_Node *oldChild)
                                                                            {return fParent.replaceChild (newChild, oldChild); };
           bool                IDDocumentTypeImpl::supports(const XMLCh *feature, const XMLCh *version) const
                                                                            {return fNode.supports (feature, version); };
           void                IDDocumentTypeImpl::setPrefix(const XMLCh  *prefix)         {fNode.setPrefix(prefix); };

