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

#include "DOM_Node.hpp"
#include "DOM_NodeList.hpp"
#include "DOM_NamedNodeMap.hpp"
#include "DOM_Document.hpp"

#include "NodeImpl.hpp"
#include <assert.h>

XERCES_CPP_NAMESPACE_BEGIN


DOM_Node::DOM_Node()
{
    fImpl = null;
};


DOM_Node::DOM_Node(NodeImpl *impl)
{
    fImpl = impl;
    RefCountedImpl::addRef(fImpl);
};


DOM_Node::DOM_Node(const DOM_Node &other)
{
	this->fImpl = other.fImpl;
    RefCountedImpl::addRef(fImpl);
};


DOM_Node & DOM_Node::operator = (const DOM_Node &other)
{
    if (this->fImpl != other.fImpl)
    {
        RefCountedImpl::removeRef(this->fImpl);
        this->fImpl = other.fImpl;
        RefCountedImpl::addRef(this->fImpl);
    }
    return *this;
};


DOM_Node & DOM_Node::operator = (const DOM_NullPtr *other)
{
    RefCountedImpl::removeRef(this->fImpl);
    this->fImpl = 0;
    return *this;
};



DOM_Node::~DOM_Node()
{
    RefCountedImpl::removeRef (this->fImpl);
    fImpl = 0;
};

//
//      Comparison operators.  Equivalent of Java object reference ==
//                                         Null references compare ==.
//
bool       DOM_Node::operator != (const DOM_Node & other) const
{
    return this->fImpl != other.fImpl;
};


bool       DOM_Node::operator == (const DOM_Node & other) const
{
    return this->fImpl == other.fImpl;
};

bool       DOM_Node::operator != (const DOM_NullPtr * other) const
{
    return this->fImpl != 0;
};


bool       DOM_Node::operator == (const DOM_NullPtr * other) const
{
    return this->fImpl == 0;
};




DOM_Node   DOM_Node::appendChild(const DOM_Node &newChild)
{
    return DOM_Node(fImpl->appendChild(newChild.fImpl));
};


DOM_Node      DOM_Node::cloneNode(bool deep) const
{
    return DOM_Node(fImpl->cloneNode(deep));
};


DOMString  DOM_Node::getNodeName()  const
{
    return fImpl->getNodeName().clone();
};


DOMString  DOM_Node::getNodeValue() const
{
    return fImpl->getNodeValue().clone();
};


short   DOM_Node::getNodeType() const
{
    return fImpl->getNodeType();
};


DOM_Node      DOM_Node::getParentNode() const
{
    return DOM_Node(fImpl->getParentNode());
};


DOM_NodeList      DOM_Node::getChildNodes() const
{
    return DOM_NodeList(fImpl);
};


DOM_Node      DOM_Node::getFirstChild() const
{
    return DOM_Node(fImpl->getFirstChild());
};


DOM_Node      DOM_Node::getLastChild() const
{
    return DOM_Node(fImpl->getLastChild());
};


DOM_Node      DOM_Node::getPreviousSibling() const
{
    return DOM_Node(fImpl->getPreviousSibling());
};


DOM_Node       DOM_Node::getNextSibling() const
{
    return DOM_Node(fImpl->getNextSibling());
};


void          *DOM_Node::getUserData() const
{
    return fImpl->getUserData ();
}

DOM_NamedNodeMap DOM_Node::getAttributes() const
{
	if (getNodeType() == ELEMENT_NODE)
		return (fImpl->getAttributes() == null) ? DOM_NamedNodeMap(fImpl) : DOM_NamedNodeMap(fImpl->getAttributes());
	else
		return DOM_NamedNodeMap();
};


DOM_Document   DOM_Node::getOwnerDocument() const
{
    return fImpl->getOwnerDocument();
};


bool           DOM_Node::hasChildNodes() const
{
    return fImpl->hasChildNodes();
};


DOM_Node       DOM_Node::insertBefore(const DOM_Node &newChild, const DOM_Node &refChild){
    return DOM_Node(fImpl->insertBefore(newChild.fImpl, refChild.fImpl));
};


bool               DOM_Node::isNull() const
{
    return fImpl == null;
};


DOM_Node       DOM_Node::replaceChild(const DOM_Node &newChild, const DOM_Node &oldChild){
    return DOM_Node(fImpl->replaceChild(newChild.fImpl, oldChild.fImpl));
};


DOM_Node       DOM_Node::removeChild(const DOM_Node &oldChild){
    return DOM_Node(fImpl->removeChild(oldChild.fImpl));
};


void           DOM_Node::setNodeValue(const DOMString &nodeValue)
{
    fImpl->setNodeValue(nodeValue);
};


void            DOM_Node::setUserData(void *p)
{
    fImpl->setUserData(p);
}


//Introduced in DOM Level 2

void              DOM_Node::normalize()
{
    fImpl->normalize();
};


bool              DOM_Node::isSupported(const DOMString &feature,
	                       const DOMString &version) const
{
    return fImpl->isSupported(feature, version);
}

DOMString         DOM_Node::getNamespaceURI() const
{
    return fImpl->getNamespaceURI().clone();
}

DOMString         DOM_Node::getPrefix() const
{
    return fImpl->getPrefix().clone();
}

DOMString         DOM_Node::getLocalName() const
{
    return fImpl->getLocalName().clone();
}

void              DOM_Node::setPrefix(const DOMString &prefix)
{
    fImpl->setPrefix(prefix);
}

bool              DOM_Node::hasAttributes() const
{
    return fImpl->hasAttributes();
}

XERCES_CPP_NAMESPACE_END

