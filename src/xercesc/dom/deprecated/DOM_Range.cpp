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



#include "DOM_Range.hpp"
#include "DocumentImpl.hpp"
#include "RangeImpl.hpp"

XERCES_CPP_NAMESPACE_BEGIN


DOM_Range::DOM_Range()
:fImpl(0)
{
}

DOM_Range::DOM_Range(RangeImpl* impl)
{
    fImpl = impl;
    RefCountedImpl::addRef(fImpl);
}

DOM_Range::DOM_Range(const DOM_Range& other)
{
    fImpl = other.fImpl;
    RefCountedImpl::addRef(fImpl);
}

DOM_Range::~DOM_Range()
{
    RefCountedImpl::removeRef (this->fImpl);
    fImpl = 0;
};

DOM_Range & DOM_Range::operator = (const DOM_Range &other)
{
    if (this->fImpl != other.fImpl)
    {
        RefCountedImpl::removeRef(this->fImpl);
        this->fImpl = other.fImpl;
        RefCountedImpl::addRef(this->fImpl);
    }
    return *this;
};


DOM_Range & DOM_Range::operator = (const DOM_NullPtr *other)
{
    RefCountedImpl::removeRef(this->fImpl);
    this->fImpl = 0;
    return *this;
};

bool       DOM_Range::operator != (const DOM_Range & other) const
{
    return this->fImpl != other.fImpl;
};


bool       DOM_Range::operator == (const DOM_Range & other) const
{
    return this->fImpl == other.fImpl;
};

bool       DOM_Range::operator != (const DOM_NullPtr * other) const
{
    return this->fImpl != 0;
};


bool       DOM_Range::operator == (const DOM_NullPtr * other) const
{
    return this->fImpl == 0;
}

//getter functions

DOM_Node DOM_Range::getStartContainer() const
{
    return ((RangeImpl *)fImpl)->getStartContainer();
}
unsigned int DOM_Range::getStartOffset() const
{
    return ((RangeImpl *)fImpl)->getStartOffset();
}
DOM_Node DOM_Range::getEndContainer() const
{
        return ((RangeImpl *)fImpl)->getEndContainer();
}
unsigned int DOM_Range::getEndOffset() const
{
        return ((RangeImpl *)fImpl)->getEndOffset();
}
const DOM_Node DOM_Range::getCommonAncestorContainer() const
{
        return ((RangeImpl *)fImpl)->getCommonAncestorContainer();
}

//setter functions

void DOM_Range::setStart(const DOM_Node& parent, unsigned int offset)
{
    this->fImpl->setStart(parent, offset);
}

void DOM_Range::setEnd(const DOM_Node& parent, unsigned int offset)
{
    this->fImpl->setEnd(parent, offset);
}

void DOM_Range::setStartBefore(const DOM_Node& refNode)
{
    this->fImpl->setStartBefore(refNode);
}

void DOM_Range::setStartAfter(const DOM_Node& refNode)
{
    this->fImpl->setStartAfter(refNode);
}

void DOM_Range::setEndBefore(const DOM_Node& refNode)
{
    this->fImpl->setEndBefore(refNode);
}

void DOM_Range::setEndAfter(const DOM_Node& refNode)
{
    this->fImpl->setEndAfter(refNode);
}

//misc functions
void DOM_Range::collapse(bool toStart)
{
    this->fImpl->collapse(toStart);
}

bool DOM_Range::getCollapsed() const
{
    return ((RangeImpl *)fImpl)->getCollapsed();
}

void DOM_Range::selectNode(const DOM_Node& node)
{
  ((RangeImpl *)fImpl)->selectNode(node);
}
void DOM_Range::selectNodeContents(const DOM_Node& node)
{
    ((RangeImpl *)fImpl)->selectNodeContents(node);
}

//Functions related to comparing ange Boundrary-Points
short DOM_Range::compareBoundaryPoints(CompareHow how, const DOM_Range& range) const
{
    return ((RangeImpl *)fImpl)->compareBoundaryPoints(how, range.fImpl);
}

void DOM_Range::deleteContents()
{
    ((RangeImpl *)fImpl)->deleteContents();
}

DOM_DocumentFragment DOM_Range::extractContents()
{
    return ((RangeImpl *)fImpl)->extractContents();
}

DOM_DocumentFragment DOM_Range::cloneContents() const
{
    return ((RangeImpl *)fImpl)->cloneContents();
}

void DOM_Range::insertNode(DOM_Node& node)
{
    ((RangeImpl *)fImpl)->insertNode(node);
}

//Misc functions
void DOM_Range::surroundContents(DOM_Node& node)
{
    ((RangeImpl *)fImpl)->surroundContents(node);
}

DOM_Range DOM_Range::cloneRange() const
{
    return DOM_Range( ((RangeImpl *)fImpl)->cloneRange() );
}

DOMString DOM_Range::toString() const
{
    return ((RangeImpl *)fImpl)->toString();
}

void DOM_Range::detach()
{
    ((RangeImpl *)fImpl)->detach();
}

XERCES_CPP_NAMESPACE_END

