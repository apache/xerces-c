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

// This class only adds the ability to have siblings

#include <xercesc/util/XercesDefs.hpp>
#include "DOMNodeImpl.hpp"
#include "DOMChildNode.hpp"
#include "DOMCasts.hpp"

XERCES_CPP_NAMESPACE_BEGIN


DOMChildNode::DOMChildNode()
{
    this->previousSibling  = 0;
    this->nextSibling  = 0;
}

// This only makes a shallow copy, cloneChildren must also be called for a
// deep clone
DOMChildNode::DOMChildNode(const DOMChildNode &)
{
    // Need to break the association w/ original siblings and parent
    this->previousSibling = 0;
    this->nextSibling = 0;
}

DOMChildNode::~DOMChildNode() {
}


DOMNode * DOMChildNode::getNextSibling() const {
    return nextSibling;
}

//
//  Note:  for getParentNode and getPreviousSibling(), below, an
//         extra paramter "thisNode" is required.  This is because there
//         is no way to cast from a DOMChildNode pointer back to the
//         DOMNodeImpl that it is part of.  Our "this" may or may not
//         be preceded by a fParent in the object layout, and there's no
//         practical way to tell, so we just take an extra parameter instead.

DOMNode * DOMChildNode::getParentNode(const DOMNode *thisNode) const
{
    // if we have an owner, ownerNode is our parent, otherwise it's
    // our ownerDocument and we don't have a parent
    DOMNodeImpl *thisNodeImpl = castToNodeImpl(thisNode);
    return thisNodeImpl->isOwned() ? thisNodeImpl->fOwnerNode : 0;
}

DOMNode * DOMChildNode::getPreviousSibling(const DOMNode *thisNode) const {
    // if we are the firstChild, previousSibling actually refers to our
    // parent's lastChild, but we hide that
    return castToNodeImpl(thisNode)->isFirstChild() ? 0 : previousSibling;
}

XERCES_CPP_NAMESPACE_END

