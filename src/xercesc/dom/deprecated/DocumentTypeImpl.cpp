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

#include "DocumentTypeImpl.hpp"
#include "DOM_Node.hpp"
#include "NamedNodeMapImpl.hpp"
#include "DOM_DOMException.hpp"
#include "DocumentImpl.hpp"

XERCES_CPP_NAMESPACE_BEGIN


DocumentTypeImpl::DocumentTypeImpl(DocumentImpl *ownerDoc,
                                   const DOMString &dtName)
    : ParentNode(ownerDoc),
    publicId(null), systemId(null), internalSubset(null) //DOM Level 2
	, intSubsetReading(false)	
{
    name = dtName.clone();
    entities = new NamedNodeMapImpl(this);
    notations = new NamedNodeMapImpl(this);
	elements = new NamedNodeMapImpl(this);

};


//Introduced in DOM Level 2
DocumentTypeImpl::DocumentTypeImpl(DocumentImpl *ownerDoc,
                                   const DOMString &qualifiedName,
                                   const DOMString &pubId,
                                   const DOMString &sysId)
	: ParentNode(ownerDoc),
    publicId(pubId), systemId(sysId), internalSubset(null)
	, intSubsetReading(false)
{
    name = qualifiedName.clone();
    if (DocumentImpl::indexofQualifiedName(qualifiedName) < 0)
        throw DOM_DOMException(DOM_DOMException::NAMESPACE_ERR, null);

    entities = new NamedNodeMapImpl(this);
    notations= new NamedNodeMapImpl(this);
	elements = new NamedNodeMapImpl(this);
};


DocumentTypeImpl::DocumentTypeImpl(const DocumentTypeImpl &other, bool deep)
    : ParentNode(other)
{
    name = other.name.clone();
    if (deep)
        cloneChildren(other);
    entities = other.entities->cloneMap(this);
    notations= other.notations->cloneMap(this);
	elements = other.elements->cloneMap(this);

    //DOM Level 2
    publicId		= other.publicId.clone();
    systemId		= other.systemId.clone();
	internalSubset	= other.internalSubset.clone();
	intSubsetReading = other.intSubsetReading;
};


DocumentTypeImpl::~DocumentTypeImpl()
{
    if (entities != null)
    {
        entities->removeAll();
        NamedNodeMapImpl::removeRef(entities);
    }

    if (notations != null)
    {
        notations->removeAll();
        NamedNodeMapImpl::removeRef(notations);
    }
	if (elements != null)
	{
		elements->removeAll();
		NamedNodeMapImpl::removeRef(elements);
	}
};


NodeImpl *DocumentTypeImpl::cloneNode(bool deep)
{
    return new DocumentTypeImpl(*this, deep);
};

/**
 * NON-DOM
 * set the ownerDocument of this node and its children
 */
void DocumentTypeImpl::setOwnerDocument(DocumentImpl *doc) {
    ParentNode::setOwnerDocument(doc);
    entities->setOwnerDocument(doc);
    notations->setOwnerDocument(doc);
    //    elements->setOwnerDocument(doc);
}

DOMString DocumentTypeImpl::getNodeName()
{
    return name;
};


short DocumentTypeImpl::getNodeType() {
    return DOM_Node::DOCUMENT_TYPE_NODE;
};


NamedNodeMapImpl *DocumentTypeImpl::getEntities()
{
    return entities;
};

NamedNodeMapImpl *DocumentTypeImpl::getElements()
{
    return elements;
};

DOMString DocumentTypeImpl::getName()
{
    return name;
};


NamedNodeMapImpl *DocumentTypeImpl::getNotations()
{
    return notations;
};


bool DocumentTypeImpl::isDocumentTypeImpl()
{
    return true;
};


void DocumentTypeImpl::setReadOnly(bool readOnl, bool deep)
{
    ParentNode::setReadOnly(readOnl,deep);
    entities->setReadOnly(readOnl,true);
    notations->setReadOnly(readOnl,true);
};


//Introduced in DOM Level 2

DOMString DocumentTypeImpl::getPublicId()
{
    return publicId;
}


DOMString DocumentTypeImpl::getSystemId()
{
    return systemId;
}


DOMString DocumentTypeImpl::getInternalSubset()
{
    return internalSubset;
}

bool DocumentTypeImpl::isIntSubsetReading()
{
    return intSubsetReading;
}


//set functions

void        DocumentTypeImpl::setPublicId(const DOMString& value)
{
    if (value == 0)
        return;
    publicId = value.clone();
}

void        DocumentTypeImpl::setSystemId(const DOMString& value)
{
    if (value == 0)
        return;
    systemId = value.clone();
}

void        DocumentTypeImpl::setInternalSubset(const DOMString &value)
{
    if (value == 0)
        return;
    internalSubset = value.clone();
}

XERCES_CPP_NAMESPACE_END

