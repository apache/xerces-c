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

// NodeIteratorImpl.cpp: implementation of the NodeIteratorImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "NodeIteratorImpl.hpp"
#include "DOM_Document.hpp"
#include "DOM_DOMException.hpp"
#include "DocumentImpl.hpp"

XERCES_CPP_NAMESPACE_BEGIN


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NodeIteratorImpl::NodeIteratorImpl ()
: fDetached(false),
    fNodeFilter(0)
{
}	

NodeIteratorImpl::~NodeIteratorImpl ()
{
	fDetached = false;
}


void NodeIteratorImpl::detach ()
{
	fDetached = true;
}


NodeIteratorImpl::NodeIteratorImpl (
                                    DOM_Node root,
                                    unsigned long whatToShow,
                                    DOM_NodeFilter* nodeFilter,
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


NodeIteratorImpl::NodeIteratorImpl ( const NodeIteratorImpl& toCopy)
    :   fDetached(toCopy.fDetached),
    fRoot(toCopy.fRoot),
    fCurrentNode(toCopy.fCurrentNode),
    fWhatToShow(toCopy.fWhatToShow),
    fNodeFilter(toCopy.fNodeFilter),
    fForward(toCopy.fForward),
    fExpandEntityReferences(toCopy.fExpandEntityReferences)
{
}


NodeIteratorImpl& NodeIteratorImpl::operator= (const NodeIteratorImpl& other) {
    fRoot                   = other.fRoot;
    fCurrentNode            = other.fRoot;
    fWhatToShow             = other.fWhatToShow;
    fNodeFilter             = other.fNodeFilter;
    fForward                = other.fForward;
	fDetached               = other.fDetached;
    fExpandEntityReferences = other.fExpandEntityReferences;
    return *this;
}

/** Return the Root Node. */
DOM_Node NodeIteratorImpl::getRoot () {
    return fRoot;
}

// Implementation Note: Note that the iterator looks at whatToShow
// and filter values at each call, and therefore one _could_ add
// setters for these values and alter them while iterating!

/** Return the whatToShow value */

unsigned long NodeIteratorImpl::getWhatToShow () {
    return fWhatToShow;
}


/** Return the filter */

DOM_NodeFilter* NodeIteratorImpl::getFilter () {
    return fNodeFilter;
}

/** Get the expandEntity reference flag. */
bool NodeIteratorImpl::getExpandEntityReferences()
{
    return fExpandEntityReferences;
}

/** Return the next DOM_Node in the Iterator. The node is the next node in
 *  depth-first order which also passes the filter, and whatToShow.
 *  A null return means either that
 */

DOM_Node NodeIteratorImpl::nextNode () {
	if (fDetached)
		throw DOM_DOMException(DOM_DOMException::INVALID_STATE_ERR, null);

	DOM_Node result;

    // if root is null there is no next node.
    if (fRoot.isNull())
			return result;

    DOM_Node aNextNode = fCurrentNode;
    bool accepted = false; // the next node has not been accepted.

    while (!accepted) {

        // if last direction is not forward, repeat node.
        if (!fForward && !aNextNode.isNull()) {
            //System.out.println("nextNode():!fForward:"+fCurrentNode.getNodeName());
            aNextNode = fCurrentNode;
        } else {
        // else get the next node via depth-first
            aNextNode = nextNode(aNextNode, true);
        }

        fForward = true; //REVIST: should direction be set forward before null check?

        // nothing in the list. return null.
        if (aNextNode.isNull())
					return result;

        // does node pass the filters and whatToShow?
        accepted = acceptNode(aNextNode);
        if (accepted) {
            // if so, then the node is the current node.
            fCurrentNode = aNextNode;
            return fCurrentNode;
				}

    }

    // no nodes, or no accepted nodes.
    return result;
}


/** Return the previous Node in the Iterator. The node is the next node in
 *  _backwards_ depth-first order which also passes the filter, and whatToShow.
 */

DOM_Node NodeIteratorImpl::previousNode () {
	if (fDetached)
		throw DOM_DOMException(DOM_DOMException::INVALID_STATE_ERR, null);
		
	DOM_Node result;

    // if the root is null, or the current node is null, return null.
    if (fRoot.isNull() || fCurrentNode.isNull())
			return result;

    DOM_Node aPreviousNode = fCurrentNode;
    bool accepted = false;

    while (!accepted) {


        if (fForward && ! aPreviousNode.isNull()) {
            //repeat last node.
            aPreviousNode = fCurrentNode;
        } else {
            // get previous node in backwards depth first order.
            aPreviousNode = previousNode(aPreviousNode);
        }

        // we are going backwards
        fForward = false;

        // if the new previous node is null, we're at head or past the root,
        // so return null.
        if (aPreviousNode.isNull())
					return result;

        // check if node passes filters and whatToShow.
        accepted = acceptNode(aPreviousNode);
        if (accepted) {
            // if accepted, update the current node, and return it.
            fCurrentNode = aPreviousNode;
            return fCurrentNode;
        }
    }
    // there are no nodes?
    return result;
}


/** The node is accepted if it passes the whatToShow and the filter. */
bool NodeIteratorImpl::acceptNode (DOM_Node node) {
	if (fDetached)
		throw DOM_DOMException(DOM_DOMException::INVALID_STATE_ERR, null);

    if (fNodeFilter == 0) {
        return ((fWhatToShow & (1 << (node.getNodeType() - 1))) != 0);
    } else {
        return ((fWhatToShow & (1 << (node.getNodeType() - 1))) != 0)
            && fNodeFilter->acceptNode(node) == DOM_NodeFilter::FILTER_ACCEPT;
    }
}


/** Return node, if matches or any parent if matches. */
DOM_Node NodeIteratorImpl::matchNodeOrParent (DOM_Node node) {
		DOM_Node result;

    for (DOM_Node n = fCurrentNode; n != fRoot; n = n.getParentNode()) {
        if (node == n) return n;
    }

    return result;
}


/** The method nextNode(DOM_Node, bool) returns the next node
 *  from the actual DOM tree.
 *
 *  The bool visitChildren determines whether to visit the children.
 *  The result is the nextNode.
 */

DOM_Node NodeIteratorImpl::nextNode (DOM_Node node, bool visitChildren) {
	if (fDetached)
		throw DOM_DOMException(DOM_DOMException::INVALID_STATE_ERR, null);

    if (node.isNull()) return fRoot;

    DOM_Node result;
    // only check children if we visit children.
    if (visitChildren) {
        //if hasChildren, return 1st child.
        if (node.hasChildNodes()) {
            result = node.getFirstChild();
            return result;
        }
    }

    // if hasSibling, return sibling
    if (node != fRoot) {
        result = node.getNextSibling();
        if (! result.isNull()) return result;


        // return parent's 1st sibling.
        DOM_Node parent = node.getParentNode();
        while (!parent.isNull() && parent != fRoot) {
            result = parent.getNextSibling();
            if (!result.isNull()) {
                return result;
            } else {
                parent = parent.getParentNode();
            }

        } // while (parent != null && parent != fRoot) {
    }
    // end of list, return null
    DOM_Node aNull;
    return aNull;
}


/** The method previousNode(DOM_Node) returns the previous node
 *  from the actual DOM tree.
 */

DOM_Node NodeIteratorImpl::previousNode (DOM_Node node) {
	if (fDetached)
		throw DOM_DOMException(DOM_DOMException::INVALID_STATE_ERR, null);

    DOM_Node result;

    // if we're at the root, return null.
    if (node == fRoot)
			return result;

    // get sibling
    result = node.getPreviousSibling();
    if (result.isNull()) {
        //if 1st sibling, return parent
        result = node.getParentNode();
        return result;
    }

    // if sibling has children, keep getting last child of child.
    if (result.hasChildNodes()) {
        while (result.hasChildNodes()) {
            result = result.getLastChild();
        }
    }

    return result;
}


/** Fix-up the iterator on a remove. Called by DOM or otherwise,
 *  before an actual DOM remove.
 */

void NodeIteratorImpl::removeNode (DOM_Node node) {
	if (fDetached)
		throw DOM_DOMException(DOM_DOMException::INVALID_STATE_ERR, null);

    // Implementation note: Fix-up means setting the current node properly
    // after a remove.

    if (node.isNull())
				return;

    DOM_Node deleted = matchNodeOrParent(node);

    if (deleted.isNull()) return;

    if (fForward) {
        fCurrentNode = previousNode(deleted);
    } else
    // if (!fForward)
    {
        DOM_Node next = nextNode(deleted, false);
        if (! next.isNull()) {
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


void NodeIteratorImpl::unreferenced()
{
    DOM_Document doc = fRoot.getOwnerDocument();
    DocumentImpl* impl;

    if (! doc.isNull()) {
        impl = (DocumentImpl *) doc.fImpl;
    }
    else
        impl = (DocumentImpl *) fRoot.fImpl;

    if (impl->iterators != 0L) {
        int i;
        int sz = impl->iterators->size();
        for (i = 0; i < sz; i++)
            if (impl->iterators->elementAt(i) == this) {
                impl->iterators->removeElementAt(i);
                break;
            }
    }

//    delete this;
    NodeIteratorImpl* ptr = this;
    delete ptr;
}

XERCES_CPP_NAMESPACE_END

