/*
 * The Apache Software License, Version 1.1
 * 
 * Copyright (c) 1999 The Apache Software Foundation.  All rights 
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
 * Revision 1.4  2000/02/04 01:49:28  aruna1
 * TreeWalker and NodeIterator changes
 *
 * Revision 1.3  2000/01/22 01:38:29  andyh
 * Remove compiler warnings in DOM impl classes
 *
 * Revision 1.2  2000/01/05 01:16:07  andyh
 * DOM Level 2 core, namespace support added.
 *
 * Revision 1.1.1.1  1999/11/09 01:08:59  twl
 * Initial checkin
 *
 * Revision 1.3  1999/11/08 20:44:18  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */

#include "DOM_Node.hpp"
#include "DOM_NamedNodeMap.hpp"
#include "NamedNodeMapImpl.hpp"


DOM_NamedNodeMap::DOM_NamedNodeMap()
{
    fImpl = null;
};


DOM_NamedNodeMap::DOM_NamedNodeMap(const DOM_NamedNodeMap & other)
{
    this->fImpl = other.fImpl;
    NamedNodeMapImpl::addRef(fImpl);
};


DOM_NamedNodeMap::DOM_NamedNodeMap(NamedNodeMapImpl *impl)
{
    fImpl = impl;
    NamedNodeMapImpl::addRef(fImpl);
};



DOM_NamedNodeMap::~DOM_NamedNodeMap() 
{
    NamedNodeMapImpl::removeRef(fImpl);
};

bool DOM_NamedNodeMap::operator == (const DOM_NamedNodeMap &other) const
{
    return this->fImpl == other.fImpl;
};


bool DOM_NamedNodeMap::operator != (const DOM_NamedNodeMap &other) const
{
    return this->fImpl != other.fImpl;
};


bool DOM_NamedNodeMap::operator == (const DOM_NullPtr *p) const
{
    return this->fImpl == 0;
};


bool DOM_NamedNodeMap::operator != (const DOM_NullPtr *p) const
{
    return this->fImpl != 0;
};


DOM_NamedNodeMap & DOM_NamedNodeMap::operator = (const DOM_NamedNodeMap & other)
{
    if (this->fImpl != other.fImpl) 
    {
        NamedNodeMapImpl::removeRef(fImpl);
        this->fImpl = other.fImpl;
        NamedNodeMapImpl::addRef(fImpl);
    }
    return *this;
};


DOM_NamedNodeMap & DOM_NamedNodeMap::operator = (const DOM_NullPtr *other)
{
    NamedNodeMapImpl::removeRef(fImpl);
    this->fImpl = 0;
    return *this;
};


DOM_Node DOM_NamedNodeMap::getNamedItem(const DOMString &name)
{
    return DOM_Node(fImpl->getNamedItem(name));
};


DOM_Node DOM_NamedNodeMap::setNamedItem(DOM_Node arg)
{
    return DOM_Node(fImpl->setNamedItem(arg.fImpl));
};


DOM_Node DOM_NamedNodeMap::removeNamedItem(const DOMString &name)
{
    return DOM_Node(fImpl->removeNamedItem(name));
};


DOM_Node DOM_NamedNodeMap::item(unsigned int index)
{
    return DOM_Node(fImpl->item(index));
};


unsigned int DOM_NamedNodeMap::getLength() const
{
    return fImpl->getLength();
};


//Introduced in DOM Level 2

DOM_Node DOM_NamedNodeMap::getNamedItemNS(const DOMString &namespaceURI,
	const DOMString &localName)
{
    return DOM_Node(fImpl->getNamedItemNS(namespaceURI, localName));
}

DOM_Node DOM_NamedNodeMap::setNamedItemNS(DOM_Node arg)
{
    return DOM_Node(fImpl->setNamedItemNS(arg.fImpl));
}

DOM_Node DOM_NamedNodeMap::removeNamedItemNS(const DOMString &namespaceURI,
	const DOMString &localName)
{
    return DOM_Node(fImpl->removeNamedItemNS(namespaceURI, localName));
}
