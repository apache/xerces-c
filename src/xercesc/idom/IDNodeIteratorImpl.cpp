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
 * $Log$
 * Revision 1.3  2002/03/15 16:30:04  tng
 * [Bug 6888] NodeIterator.  Retrofit this typo that was applied to Xerces-J, although this function "matchNodeOrParent" is not used in Xerces-C++....
 *
 * Revision 1.2  2002/02/04 21:20:59  tng
 * Add DOM 2 Level missing functions:
 *   1. NodeIterator::getRoot
 *   2. TreeWalker::getRoot
 *   3. Element::hasAttribute
 *   4. Element::hasAttributeNS
 *   5. Node::hasAttributes
 *   6. Node::isSupported
 *
 * Revision 1.1.1.1  2002/02/01 22:21:55  peiyongz
 * sane_include
 *
 * Revision 1.3  2001/06/04 20:11:52  tng
 * IDOM: Complete IDNodeIterator, IDTreeWalker, IDNodeFilter.
 *
 * Revision 1.2  2001/05/11 13:25:45  tng
 * Copyright update.
 *
 * Revision 1.1.1.1  2001/04/03 00:14:25  andyh
 * IDOM
 *
 */

// IDNodeIteratorImpl.cpp: implementation of the IDNodeIteratorImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "IDNodeIteratorImpl.hpp"
#include "IDOM_Document.hpp"
#include "IDOM_DOMException.hpp"
#include "IDDocumentImpl.hpp"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IDNodeIteratorImpl::IDNodeIteratorImpl ()
: fDetached(false),
    fNodeFilter(0)
{
}	

IDNodeIteratorImpl::~IDNodeIteratorImpl ()
{
	fDetached = false;
}


void IDNodeIteratorImpl::detach ()
{
	fDetached = true;
}


IDNodeIteratorImpl::IDNodeIteratorImpl (
                                    IDOM_Node* root,
                                    unsigned long whatToShow,
                                    IDOM_NodeFilter* nodeFilter,
                                    bool expandEntityRef)
:   fDetached(false),
    fRoot(root),
    fCurrentNode(0),
    fWhatToShow(whatToShow),
    fNodeFilter(nodeFilter),
    fForward(true),
    fExpandEntityReferences(expandEntityRef)
{
	
}


IDNodeIteratorImpl::IDNodeIteratorImpl ( const IDNodeIteratorImpl& toCopy)
    :   fDetached(toCopy.fDetached),
    fRoot(toCopy.fRoot),
    fCurrentNode(toCopy.fCurrentNode),
    fWhatToShow(toCopy.fWhatToShow),
    fNodeFilter(toCopy.fNodeFilter),
    fForward(toCopy.fForward),
    fExpandEntityReferences(toCopy.fExpandEntityReferences)
{
}


IDNodeIteratorImpl& IDNodeIteratorImpl::operator= (const IDNodeIteratorImpl& other) {
    fRoot                   = other.fRoot;
    fCurrentNode            = other.fRoot;
    fWhatToShow             = other.fWhatToShow;
    fNodeFilter             = other.fNodeFilter;
    fForward                = other.fForward;
    fDetached               = other.fDetached;
    fExpandEntityReferences = other.fExpandEntityReferences;
    return *this;
}

IDOM_Node* IDNodeIteratorImpl::getRoot() {
    return fRoot;
}


// Implementation Note: Note that the iterator looks at whatToShow
// and filter values at each call, and therefore one _could_ add
// setters for these values and alter them while iterating!

/** Return the whatToShow value */

unsigned long IDNodeIteratorImpl::getWhatToShow () {
    return fWhatToShow;
}


/** Return the filter */

IDOM_NodeFilter* IDNodeIteratorImpl::getFilter () {
    return fNodeFilter;
}

/** Get the expandEntity reference flag. */
bool IDNodeIteratorImpl::getExpandEntityReferences()
{
    return fExpandEntityReferences;
}

/** Return the next IDOM_Node* in the Iterator. The node is the next node in
 *  depth-first order which also passes the filter, and whatToShow.
 *  A 0 return means either that
 */

IDOM_Node* IDNodeIteratorImpl::nextNode () {
	if (fDetached)
		throw IDOM_DOMException(IDOM_DOMException::INVALID_STATE_ERR, 0);

    // if root is 0 there is no next node->
    if (!fRoot)
			return 0;

    IDOM_Node* aNextNode = fCurrentNode;
    bool accepted = false; // the next node has not been accepted.

    while (!accepted) {

        // if last direction is not forward, repeat node->
        if (!fForward && (aNextNode != 0)) {
            //System.out.println("nextNode():!fForward:"+fCurrentNode.getNodeName());
            aNextNode = fCurrentNode;
        } else {
        // else get the next node via depth-first
            aNextNode = nextNode(aNextNode, true);
        }

        fForward = true; //REVIST: should direction be set forward before 0 check?

        // nothing in the list. return 0.
        if (!aNextNode) return 0;

        // does node pass the filters and whatToShow?
        accepted = acceptNode(aNextNode);
        if (accepted) {
            // if so, then the node is the current node->
            fCurrentNode = aNextNode;
            return fCurrentNode;
        }
    }

    // no nodes, or no accepted nodes.
    return 0;
}


/** Return the previous Node in the Iterator. The node is the next node in
 *  _backwards_ depth-first order which also passes the filter, and whatToShow.
 */

IDOM_Node* IDNodeIteratorImpl::previousNode () {
	if (fDetached)
		throw IDOM_DOMException(IDOM_DOMException::INVALID_STATE_ERR, 0);
		
    // if the root is 0, or the current node is 0, return 0.
    if (!fRoot || !fCurrentNode) return 0;

    IDOM_Node* aPreviousNode = fCurrentNode;
    bool accepted = false;

    while (!accepted) {

        if (fForward && (aPreviousNode != 0)) {
            //repeat last node->
            aPreviousNode = fCurrentNode;
        } else {
            // get previous node in backwards depth first order.
            aPreviousNode = previousNode(aPreviousNode);
        }

        // we are going backwards
        fForward = false;

        // if the new previous node is 0, we're at head or past the root,
        // so return 0.
        if (!aPreviousNode) return 0;

        // check if node passes filters and whatToShow.
        accepted = acceptNode(aPreviousNode);
        if (accepted) {
            // if accepted, update the current node, and return it.
            fCurrentNode = aPreviousNode;
            return fCurrentNode;
        }
    }
    // there are no nodes?
    return 0;
}


/** The node is accepted if it passes the whatToShow and the filter. */
bool IDNodeIteratorImpl::acceptNode (IDOM_Node* node) {
	if (fDetached)
		throw IDOM_DOMException(IDOM_DOMException::INVALID_STATE_ERR, 0);

    if (fNodeFilter == 0) {
        return ((fWhatToShow & (1 << (node->getNodeType() - 1))) != 0);
    } else {
        return ((fWhatToShow & (1 << (node->getNodeType() - 1))) != 0)
            && fNodeFilter->acceptNode(node) == IDOM_NodeFilter::FILTER_ACCEPT;
    }
}


/** Return node, if matches or any parent if matches. */
IDOM_Node* IDNodeIteratorImpl::matchNodeOrParent (IDOM_Node* node) {

    for (IDOM_Node* n = fCurrentNode; n != fRoot; n = n->getParentNode()) {
        if (node == n) return n;
    }

    return 0;
}


/** The method nextNode(IDOM_Node, bool) returns the next node
 *  from the actual DOM tree.
 *
 *  The bool visitChildren determines whether to visit the children.
 *  The result is the nextNode.
 */

IDOM_Node* IDNodeIteratorImpl::nextNode (IDOM_Node* node, bool visitChildren) {
	if (fDetached)
		throw IDOM_DOMException(IDOM_DOMException::INVALID_STATE_ERR, 0);

    if (!node) return fRoot;

    IDOM_Node* result = 0;
    // only check children if we visit children.
    if (visitChildren) {
        //if hasChildren, return 1st child.
        if (node->hasChildNodes()) {
            result = node->getFirstChild();
            return result;
        }
    }

    // if hasSibling, return sibling
    if (node != fRoot) {
        result = node->getNextSibling();
        if (result != 0) return result;


        // return parent's 1st sibling.
        IDOM_Node* parent = node->getParentNode();
        while ((parent != 0) && parent != fRoot) {
            result = parent->getNextSibling();
            if (result != 0) {
                return result;
            } else {
                parent = parent->getParentNode();
            }

        } // while (parent != 0 && parent != fRoot) {
    }
    // end of list, return 0
    return 0;
}


/** The method previousNode(IDOM_Node) returns the previous node
 *  from the actual DOM tree.
 */

IDOM_Node* IDNodeIteratorImpl::previousNode (IDOM_Node* node) {
	if (fDetached)
		throw IDOM_DOMException(IDOM_DOMException::INVALID_STATE_ERR, 0);

    IDOM_Node* result = 0;

    // if we're at the root, return 0.
    if (node == fRoot)
			return 0;

    // get sibling
    result = node->getPreviousSibling();
    if (!result) {
        //if 1st sibling, return parent
        result = node->getParentNode();
        return result;
    }

    // if sibling has children, keep getting last child of child.
    if (result->hasChildNodes()) {
        while (result->hasChildNodes()) {
            result = result->getLastChild();
        }
    }

    return result;
}


/** Fix-up the iterator on a remove. Called by DOM or otherwise,
 *  before an actual DOM remove.
 */

void IDNodeIteratorImpl::removeNode (IDOM_Node* node) {
	if (fDetached)
		throw IDOM_DOMException(IDOM_DOMException::INVALID_STATE_ERR, 0);

    // Implementation note: Fix-up means setting the current node properly
    // after a remove.

    if (!node) return;

    IDOM_Node* deleted = matchNodeOrParent(node);

    if (!deleted) return;

    if (fForward) {
        fCurrentNode = previousNode(deleted);
    } else
    // if (!fForward)
    {
        IDOM_Node* next = nextNode(deleted, false);
        if (next != 0) {
            // normal case: there _are_ nodes following this in the iterator.
            fCurrentNode = next;
        } else {
            // the last node in the iterator is to be removed,
            // so we set the current node to be the previous one.
            fCurrentNode = previousNode(deleted);
            fForward = true;
        }

    }

}

