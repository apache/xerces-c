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

#include "TreeWalkerImpl.hpp"
#include "DOM_Document.hpp"
#include "DOM_DOMException.hpp"
#include "DocumentImpl.hpp"

XERCES_CPP_NAMESPACE_BEGIN


/** constructor */
TreeWalkerImpl::TreeWalkerImpl (
                                DOM_Node root,
                                unsigned long whatToShow,
                                DOM_NodeFilter* nodeFilter,
                                bool expandEntityRef)
:   fCurrentNode(root),
    fRoot(root),
    fWhatToShow(whatToShow),
    fNodeFilter(nodeFilter),
    fExpandEntityReferences(expandEntityRef)

{
}


TreeWalkerImpl::TreeWalkerImpl (const TreeWalkerImpl& twi)
: fCurrentNode(twi.fCurrentNode),
    fRoot(twi.fRoot),
    fWhatToShow(twi.fWhatToShow),
    fNodeFilter(twi.fNodeFilter),
    fExpandEntityReferences(twi.fExpandEntityReferences)
{
}


TreeWalkerImpl& TreeWalkerImpl::operator= (const TreeWalkerImpl& twi) {
    if (this != &twi)
    {
        fCurrentNode            = twi.fCurrentNode;
        fRoot                   = twi.fRoot;
        fWhatToShow             = twi.fWhatToShow;
        fNodeFilter             = twi.fNodeFilter;
		fExpandEntityReferences = twi.fExpandEntityReferences;
    }

    return *this;
}



void TreeWalkerImpl::unreferenced()
{
    DOM_Document doc = fRoot.getOwnerDocument();
    DocumentImpl* impl;

    if (! doc.isNull()) {
        impl = (DocumentImpl *) doc.fImpl;
    }
    else
        impl = (DocumentImpl *) fRoot.fImpl;

    if (impl->treeWalkers != 0L) {
        int i;
        int sz = impl->treeWalkers->size();
        for (i = 0; i < sz; i++)
            if (impl->treeWalkers->elementAt(i) == this) {
                impl->treeWalkers->removeElementAt(i);
                break;
            }
    }

//    delete this;
    TreeWalkerImpl* ptr = this;
    delete ptr;
}


/** Return the Root Node. */
DOM_Node TreeWalkerImpl::getRoot () {

    return fRoot;
}

/** Return the whatToShow value */
unsigned long TreeWalkerImpl::getWhatToShow () {
    return fWhatToShow;
}


/** Return the NodeFilter */
DOM_NodeFilter* TreeWalkerImpl::getFilter () {
    return fNodeFilter;
}

/** Get the expandEntity reference flag. */
bool TreeWalkerImpl::getExpandEntityReferences() {
    return fExpandEntityReferences;
}



/** Return the current Node. */
DOM_Node TreeWalkerImpl::getCurrentNode () {

    return fCurrentNode;
}


/** Return the current Node. */
void TreeWalkerImpl::setCurrentNode (DOM_Node node) {

    fCurrentNode = node;
}


/** Return the parent Node from the current node,
 *  after applying filter, whatToshow.
 *  If result is not null, set the current Node.
 */
DOM_Node TreeWalkerImpl::parentNode () {

	DOM_Node result;

    if (fCurrentNode.isNull()) return result;

    DOM_Node node = getParentNode(fCurrentNode);
    if (node != 0) {
        fCurrentNode = node;
    }
    return node;

}


/** Return the first child Node from the current node,
 *  after applying filter, whatToshow.
 *  If result is not null, set the current Node.
 */
DOM_Node TreeWalkerImpl::firstChild () {

	DOM_Node result;

    if (fCurrentNode.isNull()) return result;

    DOM_Node node = getFirstChild(fCurrentNode);
    if (! node.isNull()) {
        fCurrentNode = node;
    }
    return node;
}


/** Return the last child Node from the current node,
 *  after applying filter, whatToshow.
 *  If result is not null, set the current Node.
 */
DOM_Node TreeWalkerImpl::lastChild () {

    DOM_Node result;

    if (fCurrentNode.isNull()) return result;

    DOM_Node node = getLastChild(fCurrentNode);
    if (! node.isNull()) {
        fCurrentNode = node;
    }
    return node;
}


/** Return the previous sibling Node from the current node,
 *  after applying filter, whatToshow.
 *  If result is not null, set the current Node.
 */

DOM_Node TreeWalkerImpl::previousSibling () {
	
	DOM_Node result;

    if (fCurrentNode.isNull()) return result;

    DOM_Node node = getPreviousSibling(fCurrentNode);
    if (! node.isNull()) {
        fCurrentNode = node;
    }
    return node;
}


/** Return the next sibling Node from the current node,
 *  after applying filter, whatToshow.
 *  If result is not null, set the current Node.
 */

DOM_Node TreeWalkerImpl::nextSibling () {
		
	DOM_Node result;

    if (fCurrentNode.isNull()) return result;

    DOM_Node node = getNextSibling(fCurrentNode);
    if (! node.isNull()) {
        fCurrentNode = node;
    }
    return node;
}


/** Return the previous Node from the current node,
 *  after applying filter, whatToshow.
 *  If result is not null, set the current Node.
 */

DOM_Node TreeWalkerImpl::previousNode () {
	
    DOM_Node result;

    if (fCurrentNode.isNull()) return result;

    // get sibling
    result = getPreviousSibling(fCurrentNode);
    if (result.isNull()) {
        result = getParentNode(fCurrentNode);
        if (! result.isNull()) {
            fCurrentNode = result;
            return fCurrentNode;
        }
        return result;
    }

    // get the lastChild of result.
    DOM_Node lastChild  = getLastChild(result);

    // if there is a lastChild which passes filters return it.
    if (! lastChild.isNull()) {
        fCurrentNode = lastChild;
        return fCurrentNode;
    }

    // otherwise return the previous sibling.
    if (! result.isNull()) {
        fCurrentNode = result;
        return fCurrentNode;
    }

    // otherwise return null.
    return result;
}


/** Return the next Node from the current node,
 *  after applying filter, whatToshow.
 *  If result is not null, set the current Node.
 */

DOM_Node TreeWalkerImpl::nextNode () {
	
	DOM_Node result;

    if (fCurrentNode.isNull()) return result;

    result = getFirstChild(fCurrentNode);

    if (! result.isNull()) {
        fCurrentNode = result;
        return result;
    }

    result = getNextSibling(fCurrentNode);

    if (! result.isNull()) {
        fCurrentNode = result;
        return result;
    }

    // return parent's 1st sibling.
    DOM_Node parent = getParentNode(fCurrentNode);
    while (! parent.isNull()) {
        result = getNextSibling(parent);
        if (! result.isNull()) {
            fCurrentNode = result;
            return result;
        } else {
            parent = getParentNode(parent);
        }
    }

    // end , return null
    return result;
}


/** Internal function.
 *  Return the parent Node, from the input node
 *  after applying filter, whatToshow.
 *  The current node is not consulted or set.
 */

DOM_Node TreeWalkerImpl::getParentNode (DOM_Node node) {
	
	DOM_Node result;

    if (node.isNull() || node == fRoot) return result;

    DOM_Node newNode = node.getParentNode();
    if (newNode.isNull())  return result;

    short accept = acceptNode(newNode);

    if (accept == DOM_NodeFilter::FILTER_ACCEPT)
        return newNode;

    return getParentNode(newNode);

}


/** Internal function.
 *  Return the nextSibling Node, from the input node
 *  after applying filter, whatToshow.
 *  The current node is not consulted or set.
 */

DOM_Node TreeWalkerImpl::getNextSibling (DOM_Node node) {
	
	DOM_Node result;

    if (node.isNull() || node == fRoot) return result;

    DOM_Node newNode = node.getNextSibling();
    if (newNode.isNull()) {

        newNode = node.getParentNode();

        if (newNode.isNull() || node == fRoot)  return result;

        short parentAccept = acceptNode(newNode);

        if (parentAccept == DOM_NodeFilter::FILTER_SKIP) {
            return getNextSibling(newNode);
        }

        return result;
    }

    short accept = acceptNode(newNode);

    if (accept == DOM_NodeFilter::FILTER_ACCEPT)
        return newNode;
    else
    if (accept == DOM_NodeFilter::FILTER_SKIP) {
        DOM_Node fChild =  getFirstChild(newNode);
        if (fChild.isNull()) {
            return getNextSibling(newNode);
        }
        return fChild;
    }
    return getNextSibling(newNode);

}


/** Internal function.
 *  Return the previous sibling Node, from the input node
 *  after applying filter, whatToshow.
 *  The current node is not consulted or set.
 */

DOM_Node TreeWalkerImpl::getPreviousSibling (DOM_Node node) {
		
	DOM_Node result;

    if (node.isNull() || node == fRoot) return result;

    DOM_Node newNode = node.getPreviousSibling();
    if (newNode.isNull()) {

        newNode = node.getParentNode();
        if (newNode.isNull() || node == fRoot)  return result;

        short parentAccept = acceptNode(newNode);

        if (parentAccept == DOM_NodeFilter::FILTER_SKIP) {
            return getPreviousSibling(newNode);
        }

        return result;
    }

    short accept = acceptNode(newNode);

    if (accept == DOM_NodeFilter::FILTER_ACCEPT)
        return newNode;
    else
    if (accept == DOM_NodeFilter::FILTER_SKIP) {
        DOM_Node fChild =  getLastChild(newNode);
        if (fChild.isNull()) {
            return getPreviousSibling(newNode);
        }
        return fChild;
    }
    return getPreviousSibling(newNode);

}


/** Internal function.
 *  Return the first child Node, from the input node
 *  after applying filter, whatToshow.
 *  The current node is not consulted or set.
 */

DOM_Node TreeWalkerImpl::getFirstChild (DOM_Node node) {
		
	DOM_Node result;

    if (node.isNull()) return result;

    DOM_Node newNode = node.getFirstChild();
    if (newNode.isNull())  return result;

    short accept = acceptNode(newNode);

    if (accept == DOM_NodeFilter::FILTER_ACCEPT)
        return newNode;
    else
    if (accept == DOM_NodeFilter::FILTER_SKIP
        && newNode.hasChildNodes())
    {
        return getFirstChild(newNode);
    }
    return getNextSibling(newNode);
}


/** Internal function.
 *  Return the last child Node, from the input node
 *  after applying filter, whatToshow.
 *  The current node is not consulted or set.
 */

DOM_Node TreeWalkerImpl::getLastChild (DOM_Node node) {
	
	DOM_Node result;

    if (node.isNull()) return result;

    DOM_Node newNode = node.getLastChild();
    if (newNode.isNull())  return result;

    short accept = acceptNode(newNode);

    if (accept == DOM_NodeFilter::FILTER_ACCEPT)
        return newNode;
    else
    if (accept == DOM_NodeFilter::FILTER_SKIP
        && newNode.hasChildNodes())
    {
        return getLastChild(newNode);
    }
    return getPreviousSibling(newNode);


}


/** The node is accepted if it passes the whatToShow and the filter. */

short TreeWalkerImpl::acceptNode (DOM_Node node) {
	
    if (fNodeFilter == 0) {
        if ( ( fWhatToShow & (1 << (node.getNodeType() - 1))) != 0)
        {
            return DOM_NodeFilter::FILTER_ACCEPT;
        }
        else
        {
            return DOM_NodeFilter::FILTER_SKIP;
        }
    } else {
        // REVISIT: This logic is unclear from the spec!
        if ((fWhatToShow & (1 << (node.getNodeType() - 1))) != 0 ) {
            return fNodeFilter->acceptNode(node);
        } else {
            // what to show has failed!
            if (fNodeFilter->acceptNode(node) == DOM_NodeFilter::FILTER_REJECT) {
                return DOM_NodeFilter::FILTER_REJECT;
            } else {
                return DOM_NodeFilter::FILTER_SKIP;
            }
        }
    }
}

XERCES_CPP_NAMESPACE_END

