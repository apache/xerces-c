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
 * Revision 1.5  2001/08/07 17:01:09  tng
 * [Bug 2676] IDOM: pure virtual called in IDDeepNodeListImpl::item() .
 *
 * Revision 1.4  2001/06/04 20:11:52  tng
 * IDOM: Complete IDNodeIterator, IDTreeWalker, IDNodeFilter.
 *
 * Revision 1.3  2001/06/04 14:55:32  tng
 * IDOM: Add IRange and IDeepNodeList Support.
 *
 * Revision 1.2  2001/05/11 13:25:40  tng
 * Copyright update.
 *
 * Revision 1.1.1.1  2001/04/03 00:14:20  andyh
 * IDOM
 *
 */

#include "IDDeepNodeListImpl.hpp"
#include "IDElementImpl.hpp"
#include "IDDocumentImpl.hpp"
#include "IDCasts.hpp"
#include "IDNodeImpl.hpp"
#include <util/XMLUniDefs.hpp>
#include <limits.h>

static const XMLCh kAstr[] = {chAsterisk, chNull};
IDDeepNodeListPool<IDDeepNodeListImpl>* IDDeepNodeListImpl::fNodeListPool = 0;

IDDeepNodeListImpl::IDDeepNodeListImpl(const IDOM_Node *rootNode,
                                       const XMLCh *tagName)
    : fRootNode(rootNode)
    , fChanges(0)
    , fCurrentNode(0)
    , fCurrentIndexPlus1(0)
    , fNamespaceURI(0)
    , fMatchAllURI(false)
    , fMatchURIandTagname(false)
{
    fTagName = ((IDDocumentImpl*)rootNode->getOwnerDocument())->getPooledString(tagName);
    fMatchAll = (XMLString::compareString(fTagName, kAstr) == 0);
}


//DOM Level 2
IDDeepNodeListImpl::IDDeepNodeListImpl(const IDOM_Node *rootNode,
                                       const XMLCh *namespaceURI,
                                       const XMLCh *localName)
    : fRootNode(rootNode)
    , fChanges(0)
    , fCurrentNode(0)
    , fCurrentIndexPlus1(0)
    , fMatchAllURI(false)
    , fMatchURIandTagname(true)
{
    fTagName = ((IDDocumentImpl*)rootNode->getOwnerDocument())->getPooledString(localName);
    fMatchAll = (XMLString::compareString(fTagName, kAstr) == 0);
    fMatchAllURI = (XMLString::compareString(namespaceURI, kAstr) == 0);
    fNamespaceURI = ((IDDocumentImpl*)rootNode->getOwnerDocument())->getPooledString(namespaceURI);
}


IDDeepNodeListImpl::~IDDeepNodeListImpl()
{
}


IDOM_NodeList *IDDeepNodeListImpl::getDeepNodeList(const IDOM_Node *rootNode, const XMLCh *tagName)
{
    if(!fNodeListPool) {
        fNodeListPool = new ((IDDocumentImpl*)rootNode->getOwnerDocument()) IDDeepNodeListPool<IDDeepNodeListImpl>(109);
    }

    IDDeepNodeListImpl* retList = fNodeListPool->getByKey(rootNode, tagName, 0);
    if (!retList) {
        // the pool will adopt the IDDeepNodeListImpl
        int id = fNodeListPool->put((void*) rootNode, (XMLCh*) tagName, 0, new IDDeepNodeListImpl(rootNode, tagName));
        retList = fNodeListPool->getById(id);
    }

    return retList;
}


IDOM_NodeList *IDDeepNodeListImpl::getDeepNodeList(const IDOM_Node *rootNode,     //DOM Level 2
                                                   const XMLCh *namespaceURI,
                                                   const XMLCh *localName)
{
    if(!fNodeListPool) {
        fNodeListPool = new ((IDDocumentImpl*)rootNode->getOwnerDocument()) IDDeepNodeListPool<IDDeepNodeListImpl>(109);
    }

    IDDeepNodeListImpl* retList = fNodeListPool->getByKey(rootNode, localName, namespaceURI);
    if (!retList) {
        // the pool will adopt the IDDeepNodeListImpl
        int id = fNodeListPool->put((void*) rootNode, (XMLCh*) localName, (XMLCh*) namespaceURI, new IDDeepNodeListImpl(rootNode, namespaceURI, localName));
        retList = fNodeListPool->getById(id);
    }

    return retList;
}


unsigned int IDDeepNodeListImpl::getLength()
{
    // After getting the length of the list, the most likely operation is
    // to iterate through the list.  Therefore, it's best to cache the
    // the first element, rather than forcing a search for it the second time.
    //
    // idom_revisit:  This assumes the user writes:
    //
    //    int len = nodeList->getLength();
    //    for (i = 0; i < len; i++)
    //        nodeList->item(i);
    //
    // If a foolish user writes the following, the cached node will be reset
    // to zero on every iteration!  Should we account for this sloppy style?
    //
    //    for (i = 0; i < nodeList->getLength(); i++)
    //        nodeList->item(i);
    // end idom_revisit
    item(0);
    IDOM_Node *cacheFirstNode = fCurrentNode;

    // item(int) stops when we run out of subtree, at which point
    // fCurrentIndexPlus1 will point past end of list!
    item(INT_MAX);
    unsigned int length = fCurrentIndexPlus1;

    // Restore cache to beginning of list
    if (cacheFirstNode != 0)
    {
        fCurrentIndexPlus1 = 1;
        fCurrentNode = cacheFirstNode;
    }

    return length;
}



// Start from the first child and count forward, 0-based. index>length-1
// should return 0.
//
// Attempts to do only work actually requested, cache work already
// done, and to flush that cache when the tree has changed.
//
// LIMITATION: ????? Unable to tell relevant tree-changes from
// irrelevant ones.  Doing so in a really useful manner would seem
// to involve a tree-walk in its own right, or maintaining our data
// in a parallel tree.
IDOM_Node *IDDeepNodeListImpl::item(unsigned int index)
{
    unsigned int currentIndexPlus1 = fCurrentIndexPlus1;
    IDOM_Node *currentNode = fCurrentNode;

    if (castToParentImpl(fRootNode)->changes() != fChanges)
    {
        // Tree changed. Do it all from scratch!
        currentIndexPlus1 = 0;
        currentNode = (IDOM_Node *)fRootNode;
        fChanges = castToParentImpl(fRootNode)->changes();
    }
    else if (currentIndexPlus1 > index+1)
    {
        // Interested in something before cached node.  Do it all from scratch!
        currentIndexPlus1 = 0;
        currentNode = (IDOM_Node *)fRootNode;
    }
    else if (index+1 == currentIndexPlus1)
    {
        // What luck!  User is interested in cached node.
        return currentNode;
    }

    IDOM_Node *nextNode = 0;

// idom_revisit - ???? How efficient is this loop? ????

    // Start at the place in the tree at which we're
    // currently pointing and count off nodes until we
    // reach the node of interest or the end of the tree.
    while (currentIndexPlus1 < index+1 && currentNode != 0)
    {
        nextNode = nextMatchingElementAfter(currentNode);
        if (nextNode == 0)
            break;
        currentNode = nextNode;
        currentIndexPlus1++;
    }

    fCurrentNode = currentNode;
    fCurrentIndexPlus1 = currentIndexPlus1;

    // If we found a node at the requested index, make that the current node
    if (nextNode != 0)
    {
        return currentNode;
    }

    // If we didn't find a node at the requested index, return 0
    return 0;
}



/* Iterative tree-walker. When you have a Parent link, there's often no
need to resort to recursion. NOTE THAT only Element nodes are matched
since we're specifically supporting getElementsByTagName().
*/
IDOM_Node *IDDeepNodeListImpl::nextMatchingElementAfter(IDOM_Node *current)
{
    IDOM_Node *next;
    while (current != 0)
    {
        // Look down to first child.
        if (current->hasChildNodes())
        {
            current = current->getFirstChild();
        }
        // Look right to sibling (but not from root!)
        else
        {
            if (current != fRootNode && 0 != (next = current->getNextSibling()))
            {
                current = next;
            }
            // Look up and right (but not past root!)
            else
            {
                next = 0;
                for (;
                     current != fRootNode; // Stop on return to starting point
                     current = current->getParentNode())
                {
                    next = current->getNextSibling();
                    if (next != 0)
                        break;
                }
                current = next;
            }
        }

        // Have we found an Element with the right tagName?
        // ("*" matches anything.)
        if (current != 0 && current != fRootNode &&
            current->getNodeType() == IDOM_Node::ELEMENT_NODE) {
            IDOM_Element *currElement = (IDOM_Element *)current;

            if (!fMatchURIandTagname) {        //DOM Level 1
                if (fMatchAll ||
                    (XMLString::compareString(currElement->getTagName(),
                                              fTagName) == 0))
                    return current;
            } else {        //DOM Level 2
                if (!fMatchAllURI &&
                    (XMLString::compareString(current -> getNamespaceURI(),
                                              fNamespaceURI) != 0))
                    continue;

                if (fMatchAll ||
                    (XMLString::compareString(current -> getLocalName(),
                                              fTagName) == 0))
                    return current;
            }
        }

        // Otherwise continue walking the tree
    }
    // Fell out of tree-walk; no more instances found
    return 0;
}

