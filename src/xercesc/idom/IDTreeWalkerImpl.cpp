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
 * $Log$
 * Revision 1.1  2002/02/01 22:21:57  peiyongz
 * Initial revision
 *
 * Revision 1.3  2001/06/04 20:11:53  tng
 * IDOM: Complete IDNodeIterator, IDTreeWalker, IDNodeFilter.
 *
 * Revision 1.2  2001/05/11 13:26:00  tng
 * Copyright update.
 *
 * Revision 1.1.1.1  2001/04/03 00:14:35  andyh
 * IDOM
 *
 */

#include "IDTreeWalkerImpl.hpp"
#include "IDOM_Document.hpp"
#include "IDOM_DOMException.hpp"
#include "IDDocumentImpl.hpp"


/** constructor */
IDTreeWalkerImpl::IDTreeWalkerImpl (
                                IDOM_Node* root,
                                unsigned long whatToShow,
                                IDOM_NodeFilter* nodeFilter,
                                bool expandEntityRef)
:   fCurrentNode(root),
    fRoot(root),
    fWhatToShow(whatToShow),
    fNodeFilter(nodeFilter),
    fExpandEntityReferences(expandEntityRef)

{
}


IDTreeWalkerImpl::IDTreeWalkerImpl (const IDTreeWalkerImpl& twi)
: fCurrentNode(twi.fCurrentNode),
    fRoot(twi.fRoot),
    fWhatToShow(twi.fWhatToShow),
    fNodeFilter(twi.fNodeFilter),
    fExpandEntityReferences(twi.fExpandEntityReferences)
{
}


IDTreeWalkerImpl& IDTreeWalkerImpl::operator= (const IDTreeWalkerImpl& twi) {
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



/** Return the whatToShow value */
unsigned long IDTreeWalkerImpl::getWhatToShow () {
    return fWhatToShow;
}


/** Return the NodeFilter */
IDOM_NodeFilter* IDTreeWalkerImpl::getFilter () {
    return fNodeFilter;
}

/** Get the expandEntity reference flag. */
bool IDTreeWalkerImpl::getExpandEntityReferences() {
    return fExpandEntityReferences;
}



/** Return the current Node. */
IDOM_Node* IDTreeWalkerImpl::getCurrentNode () {

    return fCurrentNode;
}


/** Return the current Node. */
void IDTreeWalkerImpl::setCurrentNode (IDOM_Node* node) {

    fCurrentNode = node;
}


/** Return the parent Node from the current node,
 *  after applying filter, whatToshow.
 *  If result is not null, set the current Node.
 */
IDOM_Node* IDTreeWalkerImpl::parentNode () {

    if (!fCurrentNode) return 0;

    IDOM_Node* node = getParentNode(fCurrentNode);
    if (node != 0) {
        fCurrentNode = node;
    }
    return node;

}


/** Return the first child Node from the current node,
 *  after applying filter, whatToshow.
 *  If result is not null, set the current Node.
 */
IDOM_Node* IDTreeWalkerImpl::firstChild () {

    if (!fCurrentNode) return 0;

    IDOM_Node* node = getFirstChild(fCurrentNode);

    if (node != 0) {
        fCurrentNode = node;
    }
    return node;
}


/** Return the last child Node from the current node,
 *  after applying filter, whatToshow.
 *  If result is not null, set the current Node.
 */
IDOM_Node* IDTreeWalkerImpl::lastChild () {

    if (!fCurrentNode) return 0;

    IDOM_Node* node = getLastChild(fCurrentNode);
    if (node != 0) {
        fCurrentNode = node;
    }
    return node;
}


/** Return the previous sibling Node from the current node,
 *  after applying filter, whatToshow.
 *  If result is not null, set the current Node.
 */

IDOM_Node* IDTreeWalkerImpl::previousSibling () {
	
    if (!fCurrentNode) return 0;

    IDOM_Node* node = getPreviousSibling(fCurrentNode);
    if (node != 0) {
        fCurrentNode = node;
    }
    return node;
}


/** Return the next sibling Node from the current node,
 *  after applying filter, whatToshow.
 *  If result is not null, set the current Node.
 */

IDOM_Node* IDTreeWalkerImpl::nextSibling () {
		
    if (!fCurrentNode) return 0;

    IDOM_Node* node = getNextSibling(fCurrentNode);
    if (node != 0) {
        fCurrentNode = node;
    }
    return node;
}


/** Return the previous Node from the current node,
 *  after applying filter, whatToshow.
 *  If result is not null, set the current Node.
 */

IDOM_Node* IDTreeWalkerImpl::previousNode () {
	
    if (!fCurrentNode) return 0;

    // get sibling
    IDOM_Node* node = getPreviousSibling(fCurrentNode);
    if (node == 0) {
        node = getParentNode(fCurrentNode);
        if ( node != 0) {
            fCurrentNode = node;
        }
        return node;
    }
    else {

        // get the lastChild of result.
        IDOM_Node* lastChild  = getLastChild(node);

        // if there is a lastChild which passes filters return it.
        if (lastChild != 0) {
            fCurrentNode = lastChild;
        }
        else {
            fCurrentNode = node;
        }
        return fCurrentNode;
    }
}


/** Return the next Node from the current node,
 *  after applying filter, whatToshow.
 *  If result is not null, set the current Node.
 */

IDOM_Node* IDTreeWalkerImpl::nextNode () {
	
    if (!fCurrentNode) return 0;

    IDOM_Node* node = getFirstChild(fCurrentNode);

    if (node != 0) {
        fCurrentNode = node;
        return node;
    }
    else {

        node = getNextSibling(fCurrentNode);

        if (node != 0) {
            fCurrentNode = node;
            return node;
        }
        else {

            // return parent's 1st sibling.
            IDOM_Node* parent = getParentNode(fCurrentNode);
            while ( parent != 0) {
                node = getNextSibling(parent);
                if (node != 0) {
                    fCurrentNode = node;
                    return node;
                } else {
                    parent = getParentNode(parent);
                }
            }
            return node;
        }
    }
}


/** Internal function.
 *  Return the parent Node, from the input node
 *  after applying filter, whatToshow.
 *  The current node is not consulted or set.
 */

IDOM_Node* IDTreeWalkerImpl::getParentNode (IDOM_Node* node) {
	
    if (!node || node == fRoot) return 0;

    IDOM_Node* newNode = node->getParentNode();
    if (!newNode)  return 0;

    short accept = acceptNode(newNode);

    if (accept == IDOM_NodeFilter::FILTER_ACCEPT)
        return newNode;

    return getParentNode(newNode);

}


/** Internal function.
 *  Return the nextSibling Node, from the input node
 *  after applying filter, whatToshow.
 *  The current node is not consulted or set.
 */

IDOM_Node* IDTreeWalkerImpl::getNextSibling (IDOM_Node* node) {
	
    if (!node || node == fRoot) return 0;

    IDOM_Node* newNode = node->getNextSibling();
    if (!newNode) {

        newNode = node->getParentNode();

        if (!newNode || node == fRoot)  return 0;

        short parentAccept = acceptNode(newNode);

        if (parentAccept == IDOM_NodeFilter::FILTER_SKIP) {
            return getNextSibling(newNode);
        }

        return 0;
    }

    short accept = acceptNode(newNode);

    if (accept == IDOM_NodeFilter::FILTER_ACCEPT)
        return newNode;
    else
    if (accept == IDOM_NodeFilter::FILTER_SKIP) {
        IDOM_Node* fChild =  getFirstChild(newNode);
        if (!fChild) {
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

IDOM_Node* IDTreeWalkerImpl::getPreviousSibling (IDOM_Node* node) {
		
    if (!node || node == fRoot) return 0;

    IDOM_Node* newNode = node->getPreviousSibling();
    if (!newNode) {

        newNode = node->getParentNode();
        if (!newNode || node == fRoot)  return 0;

        short parentAccept = acceptNode(newNode);

        if (parentAccept == IDOM_NodeFilter::FILTER_SKIP) {
            return getPreviousSibling(newNode);
        }

        return 0;
    }

    short accept = acceptNode(newNode);

    if (accept == IDOM_NodeFilter::FILTER_ACCEPT)
        return newNode;
    else
    if (accept == IDOM_NodeFilter::FILTER_SKIP) {
        IDOM_Node* fChild =  getLastChild(newNode);
        if (!fChild) {
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

IDOM_Node* IDTreeWalkerImpl::getFirstChild (IDOM_Node* node) {
		
    if (!node) return 0;

    IDOM_Node* newNode = node->getFirstChild();
    if (!newNode)  return 0;

    short accept = acceptNode(newNode);

    if (accept == IDOM_NodeFilter::FILTER_ACCEPT)
        return newNode;
    else
    if (accept == IDOM_NodeFilter::FILTER_SKIP
        && newNode->hasChildNodes())
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

IDOM_Node* IDTreeWalkerImpl::getLastChild (IDOM_Node* node) {
	
    if (!node) return 0;

    IDOM_Node* newNode = node->getLastChild();
    if (!newNode)  return 0;

    short accept = acceptNode(newNode);

    if (accept == IDOM_NodeFilter::FILTER_ACCEPT)
        return newNode;
    else
    if (accept == IDOM_NodeFilter::FILTER_SKIP
        && newNode->hasChildNodes())
    {
        return getLastChild(newNode);
    }
    return getPreviousSibling(newNode);


}


/** The node is accepted if it passes the whatToShow and the filter. */

short IDTreeWalkerImpl::acceptNode (IDOM_Node* node) {
	
    if (fNodeFilter == 0) {
        if ( ( fWhatToShow & (1 << (node->getNodeType() - 1))) != 0)
        {
            return IDOM_NodeFilter::FILTER_ACCEPT;
        }
        else
        {
            return IDOM_NodeFilter::FILTER_SKIP;
        }
    } else {
        // REVISIT: This logic is unclear from the spec!
        if ((fWhatToShow & (1 << (node->getNodeType() - 1))) != 0 ) {
            return fNodeFilter->acceptNode(node);
        } else {
            // what to show has failed!
            if (fNodeFilter->acceptNode(node) == IDOM_NodeFilter::FILTER_REJECT) {
                return IDOM_NodeFilter::FILTER_REJECT;
            } else {
                return IDOM_NodeFilter::FILTER_SKIP;
            }
        }
    }
}

