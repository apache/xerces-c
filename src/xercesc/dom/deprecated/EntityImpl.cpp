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

#include "DOM_DOMException.hpp"
#include "DOM_Node.hpp"
#include "EntityImpl.hpp"
#include "DocumentImpl.hpp"

XERCES_CPP_NAMESPACE_BEGIN


EntityImpl::EntityImpl(DocumentImpl *ownerDoc, const DOMString &eName)
   : ParentNode(ownerDoc),
	refEntity(0)

{
    name        = eName.clone();
    setReadOnly(true, true);
};


EntityImpl::EntityImpl(const EntityImpl &other, bool deep)
    : ParentNode(other)
{
    name            = other.name.clone();
    if (deep)
        cloneChildren(other);
    publicId        = other.publicId.clone();
    systemId        = other.systemId.clone();
    notationName    = other.notationName.clone();

    RefCountedImpl::removeRef(refEntity);
    refEntity       = other.refEntity;	
    RefCountedImpl::addRef(other.refEntity);

    setReadOnly(true, true);
};


EntityImpl::~EntityImpl() {
};


NodeImpl *EntityImpl::cloneNode(bool deep)
{
    return new (getOwnerDocument()->getMemoryManager()) EntityImpl(*this, deep);
};


DOMString EntityImpl::getNodeName() {
    return name;
};


short EntityImpl::getNodeType() {
    return DOM_Node::ENTITY_NODE;
};


DOMString EntityImpl::getNotationName()
{
    return notationName;
};


DOMString EntityImpl::getPublicId() {
    return publicId;
};


DOMString EntityImpl::getSystemId()
{
    return systemId;
};


void EntityImpl::setNotationName(const DOMString &arg)
{
    notationName = arg;
};


void EntityImpl::setPublicId(const DOMString &arg)
{
    publicId = arg;
};


void EntityImpl::setSystemId(const DOMString &arg)
{
    systemId = arg;
};

void		EntityImpl::setEntityRef(EntityReferenceImpl* other)
{
   RefCountedImpl::removeRef(refEntity);
	refEntity = other;
   RefCountedImpl::addRef(other);
}

EntityReferenceImpl*		EntityImpl::getEntityRef() const
{
	return refEntity;
}

void	EntityImpl::cloneEntityRefTree()
{
	//lazily clone the entityRef tree to this entity
	if (firstChild != 0)
		return;

	if (!refEntity)
		return;

   setReadOnly(false, true);
	this->cloneChildren(*refEntity);
   setReadOnly(true, true);
}

NodeImpl * EntityImpl::getFirstChild()
{
    cloneEntityRefTree();
	return firstChild;
};

NodeImpl*   EntityImpl::getLastChild()
{
	cloneEntityRefTree();
	return lastChild();
}

NodeListImpl* EntityImpl::getChildNodes()
{
	cloneEntityRefTree();
	return this;

}

bool EntityImpl::hasChildNodes()
{
	cloneEntityRefTree();
	return firstChild!=null;
}

NodeImpl* EntityImpl::item(unsigned int index)
{
	cloneEntityRefTree();
    ChildNode *node = firstChild;
    for(unsigned int i=0; i<index && node!=null; ++i)
        node = node->nextSibling;
    return node;
}

XERCES_CPP_NAMESPACE_END

