/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 1999-2000 The Apache Software Foundation.  All rights
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

#include <util/RefVectorOf.hpp>
#include "NodeImpl.hpp"
#include "RangeImpl.hpp"
#include "DocumentImpl.hpp"
#include "DOM_DOMException.hpp"
#include "DOM_Document.hpp"
#include "DocumentFragmentImpl.hpp"
#include "DOM_Document.hpp"
#include "DOM_RangeException.hpp"
#include "DOM_DOMException.hpp"
#include "DOM_Text.hpp"


//---------------------
// C'tor and D'tor 
//---------------------

RangeImpl::RangeImpl(DOM_Document doc)
  
    :   fDocument(doc),
        fStartContainer(doc),
        fStartOffset(0),
        fEndContainer(doc),
        fEndOffset(0),
        fDetached(false),
        fCollapsed(true),
        fCommonAncestorContainer(0),
        fRemoveChild(0)
{
}        

RangeImpl::RangeImpl(const RangeImpl& other)
{
    fDocument = other.fDocument;
    fStartContainer = other.fStartContainer;
    fStartOffset = other.fStartOffset;
    fEndContainer = other.fEndContainer;
    fEndOffset = other.fEndOffset;
    fDetached = other.fDetached;
    fCollapsed = other.fCollapsed;
    fCommonAncestorContainer = other.fCommonAncestorContainer;
    fRemoveChild = other.fRemoveChild;
}

RangeImpl::~RangeImpl()
{
}

void RangeImpl::unreferenced()
{
    if (((DocumentImpl*)fDocument.fImpl)->ranges != 0L) {
        int sz = ((DocumentImpl*)fDocument.fImpl)->ranges->size();
        for (int i=0; i< sz; i++) {
            if (((DocumentImpl*)fDocument.fImpl)->ranges->elementAt(i) == this) {
                ((DocumentImpl*)fDocument.fImpl)->ranges->removeElementAt(i);
                break;
            }
        }
    }
    delete this;
};


//-------------------------------
// Public getter functions
//-------------------------------


DOM_Node& RangeImpl::getStartContainer()
{
    return fStartContainer;
}

unsigned int RangeImpl::getStartOffset()
{
    return fStartOffset;
}

DOM_Node& RangeImpl::getEndContainer()
{
    return fEndContainer;
}

unsigned int RangeImpl::getEndOffset()
{
    return fEndOffset;
}



bool RangeImpl::getCollapsed()
{
    if (fDetached) 
    {
        throw DOM_DOMException(
            DOM_DOMException::INVALID_STATE_ERR, null);
    }

    return ((fStartContainer == fEndContainer) 
             && (fStartOffset == fEndOffset));
}

//-------------------------------
// Public getter functions
//-------------------------------

void RangeImpl::setStartContainer(const DOM_Node& node) 
{
    fStartContainer = node;
}

void RangeImpl::setStartOffset(unsigned int offset) 
{
    fStartOffset = offset;
}

void RangeImpl::setEndContainer(const DOM_Node& node) 
{
    fEndContainer = node;
    
}

void RangeImpl::setEndOffset(unsigned int offset) 
{
    fEndOffset = offset;
}

void RangeImpl::setCommonAncestorContainer(const DOM_Node&  node) 
{
    fCommonAncestorContainer = node;
}

void RangeImpl::setStart(DOM_Node& refNode, unsigned int offset)
{
    checkIndex(refNode, offset);
    
    fStartContainer = refNode;
    fStartOffset    = offset;
    
    if ((fDocument != refNode.getOwnerDocument() ) 
        && (refNode.getOwnerDocument().fImpl != 0) )
    {
        fDocument = refNode.getOwnerDocument();
        collapse(true); 
    }
    
    if (fStartOffset >= fEndOffset)
        collapse(true); //collapse the range positions to start
    else
        fCollapsed = false;
}

void RangeImpl::setEnd(DOM_Node& refNode, unsigned int offset)
{
    checkIndex(refNode, offset);
            
    fEndContainer   = refNode;
    fEndOffset      = offset;
    
    if ((fDocument != refNode.getOwnerDocument() ) 
        && (refNode.getOwnerDocument().fImpl != 0) )
    {
        fDocument = refNode.getOwnerDocument();
        collapse(false);
    }
    
    if (fEndOffset <= fStartOffset) 
        collapse(false); //collapse the range positions to end
    else
        fCollapsed = false;
}

void RangeImpl::setStartBefore(DOM_Node& refNode)
{
    validateNode(refNode);
    
    fStartContainer = refNode.getParentNode();
   unsigned int i = 0;
    for (DOM_Node n = refNode; n!=null; n = n.getPreviousSibling()) {
        i++;
    }
    if (i == 0)
        fStartOffset = 0;
    else
        fStartOffset = i-1;

    if ((fDocument != refNode.getOwnerDocument()) 
        && (refNode.getOwnerDocument().fImpl != 0) )
    {
        fDocument = refNode.getOwnerDocument();
        collapse(true); 
    }
    
    if (fStartOffset > fEndOffset)
        collapse(true); //collapse the range positions to start
    else
        fCollapsed = false;
}

void RangeImpl::setStartAfter(DOM_Node& refNode)
{
    validateNode(refNode);

    fStartContainer = refNode.getParentNode();
    unsigned int i = 0;
    for (DOM_Node n = refNode; n!=null; n = n.getPreviousSibling()) {
        i++;
    }

    fStartOffset = i;
    
    if ((fDocument != refNode.getOwnerDocument() ) 
        && (refNode.getOwnerDocument().fImpl != 0) )
    {
        fDocument = refNode.getOwnerDocument();
        collapse(true); 
    }
    
    if (fStartOffset > fEndOffset)
        collapse(true); //collapse the range positions to start
    else
        fCollapsed = false;
}

void RangeImpl::setEndBefore(DOM_Node& refNode)
{
    validateNode(refNode);
    
    fEndContainer = refNode.getParentNode();
    unsigned int i = 0;
    for (DOM_Node n = refNode; n!=null; n = n.getPreviousSibling(), i++);

    if (i< 1)
        fEndOffset = 0;
    else
        fEndOffset = i-1;

    if ((fDocument != refNode.getOwnerDocument() ) 
        && (refNode.getOwnerDocument().fImpl != 0) )
    {
        fDocument = refNode.getOwnerDocument();
        collapse(true); 
    }
    
    if (fEndOffset < fStartOffset) 
        collapse(false); //collapse the range positions to end
    else
        fCollapsed = false;
}

void RangeImpl::setEndAfter(DOM_Node& refNode)
{
    validateNode(refNode);
    
    fEndContainer = refNode.getParentNode();
    unsigned int i = 0;
    for (DOM_Node n = refNode; n!=null; n = n.getPreviousSibling(), i++);

    if (i ==0)
        fEndOffset = 0;
    else
        fEndOffset = i;

    if ((fDocument != refNode.getOwnerDocument() ) 
        && (refNode.getOwnerDocument().fImpl != 0) )
    {
        fDocument = refNode.getOwnerDocument();
        collapse(true); 
    }
    
    if (fEndOffset < fStartOffset) 
        collapse(false); //collapse the range positions to end
    else
        fCollapsed = false;
}
//-------------------------------
// Public Misc. functions
//-------------------------------
void RangeImpl::detach()
{
    fDetached = true;
    
    //nullify nodes
    fStartContainer = 0;
    fStartOffset    = 0;
    fEndContainer   = 0;
    fEndOffset      = 0;
    fCollapsed      = true;
    fCommonAncestorContainer = 0;

    fRemoveChild    = 0;
}

void RangeImpl::collapse(bool toStart)
{
    if( fDetached) {
        throw DOM_DOMException(
            DOM_DOMException::INVALID_STATE_ERR, null);
    }

    if (toStart) {
        fEndContainer = fStartContainer;
        fEndOffset = fStartOffset;
    } else {
        fStartContainer = fEndContainer;
        fStartOffset = fEndOffset;
    }
    fCollapsed = true;
}

void RangeImpl::selectNode(DOM_Node& refNode)
{
    validateNode(refNode);
    short type = refNode.getNodeType();
    if (type ==  DOM_Node::ATTRIBUTE_NODE
        || type == DOM_Node::ENTITY_NODE
        || type == DOM_Node::NOTATION_NODE
        || type == DOM_Node::DOCUMENT_TYPE_NODE
        || type == DOM_Node::DOCUMENT_FRAGMENT_NODE) {

        throw DOM_RangeException(
            DOM_RangeException::INVALID_NODE_TYPE_ERR, null);
    }
    //First check for the text type node
    if (type ==  DOM_Node::TEXT_NODE) 
    {
        //The node itself is the container.
        fStartContainer = refNode;
        fEndContainer   = refNode;
        
        //Select all the contents of the node
        fStartOffset = 0;
        fEndOffset = ((DOM_Text &)refNode).getLength();
        return;
    }

    DOM_Node parent = refNode.getParentNode();
    if (parent != null ) // REVIST: what to do if it IS null?
    {
        fStartContainer = parent;
        fEndContainer = parent;
      
        unsigned int i = 0;
        for (DOM_Node n = parent.getFirstChild(); n!=null, n!=refNode; n = n.getNextSibling()) {
            i++;
        }
       
        fStartOffset = i;
        fEndOffset = fStartOffset+1;
    }
}

void RangeImpl::selectNodeContents(DOM_Node& node)
{
    validateNode(node);
     short type = node.getNodeType();
    if (type == DOM_Node::ENTITY_NODE
        || type == DOM_Node::NOTATION_NODE
        || type == DOM_Node::DOCUMENT_TYPE_NODE) {

        throw DOM_RangeException(
            DOM_RangeException::INVALID_NODE_TYPE_ERR, null);
    }
    
    fStartContainer = node;
    fEndContainer = node;
    
    fStartOffset = 0;
    if (node.getNodeType() == DOM_Node::TEXT_NODE ) {
        fEndOffset = ((DOM_Text &)node).getLength();
        return;
    }

    DOM_Node first = node.getFirstChild();
    if (first == null) {
        fEndOffset = 0;
        return;
    }
    unsigned int i = 0;
    for (DOM_Node n = first; n!=null; n = n.getNextSibling()) {
        i++;
    }
    fEndOffset = i;
}

void RangeImpl::surroundContents(DOM_Node& newParent)
{
    if (newParent==null) return;
 
    //check for elimination criteria
    if( fDetached) {
        throw DOM_DOMException(
            DOM_DOMException::INVALID_STATE_ERR, null);
    }

    if (newParent.getOwnerDocument() !=fDocument) {
        throw DOM_DOMException(
            DOM_DOMException::WRONG_DOCUMENT_ERR, null);
    }
    
    int type = newParent.getNodeType();
    if (type == DOM_Node::ATTRIBUTE_NODE
        || type == DOM_Node::ENTITY_NODE
        || type == DOM_Node::NOTATION_NODE
        || type == DOM_Node::DOCUMENT_TYPE_NODE
        || type == DOM_Node::DOCUMENT_NODE
        || type == DOM_Node::DOCUMENT_FRAGMENT_NODE)
    {
        throw DOM_RangeException(
            DOM_RangeException::INVALID_NODE_TYPE_ERR, null);
    }
    
    DOM_Node root = getCommonAncestorContainer();
    
    DOM_Node realStart = fStartContainer;
    DOM_Node realEnd = fEndContainer;

    if (fStartContainer.getNodeType() == DOM_Node::TEXT_NODE) {
        realStart = fStartContainer.getParentNode();
    }
    if (fEndContainer.getNodeType() == DOM_Node::TEXT_NODE) {
        realEnd = fEndContainer.getParentNode();
    }
    
    if (realStart != realEnd) {
        throw DOM_RangeException(
            DOM_RangeException::BAD_BOUNDARYPOINTS_ERR, null);
    }
    
    DOM_DocumentFragment frag = extractContents();
    insertNode(newParent);
    newParent.appendChild(frag);
    selectNode(newParent);
}


short RangeImpl::compareBoundaryPoints(DOM_Range::CompareHow how, RangeImpl* srcRange)
{
    if (fDocument != srcRange->fDocument) {
        throw DOM_DOMException(
            DOM_DOMException::WRONG_DOCUMENT_ERR, null);
    }
    if( fDetached) {
        throw DOM_DOMException(
            DOM_DOMException::INVALID_STATE_ERR, null);
    }

    DOM_Node pointA, pointB;
    int offsetA, offsetB;
        
    switch (how)
    {
    case (DOM_Range::START_TO_START) :
        pointA = srcRange->getStartContainer();
        pointB = fStartContainer;
        offsetA = srcRange->getStartOffset();
        offsetB = fStartOffset;
        break;
    case (DOM_Range::START_TO_END) :
        pointA = srcRange->getStartContainer();
        pointB = fEndContainer;
        offsetA = srcRange->getStartOffset();
        offsetB = fEndOffset;
        break;
    case (DOM_Range::END_TO_START) :
        pointA = srcRange->getEndContainer();
        pointB = fStartContainer;
        offsetA = srcRange->getEndOffset();
        offsetB = fStartOffset;
        break;
    case (DOM_Range::END_TO_END) :
        pointA = srcRange->getEndContainer();
        pointB = fEndContainer;
        offsetA = srcRange->getEndOffset();
        offsetB = fEndOffset;
        break;
    }
    
    // case 1: same container
    if (pointA == pointB) {
        if (offsetA < offsetB) return -1; //A before B
        if (offsetA == offsetB) return 0; //A equal to B
        return 1; // A after B
    }
    // case 2: Child C of container A is ancestor of B
    for (DOM_Node node = pointA.getFirstChild(); node != null; node=node.getNextSibling()) {
        if (isAncestorOf(node, pointB)) {
            int index = indexOf(node, pointA);
            if (offsetA <=  index) return -1;
            return 1;
        }
    }
    // case 3: Child C of container B is ancestor of A
    for (DOM_Node nd = pointB.getFirstChild(); nd != null; nd=nd.getNextSibling()) {
        if (isAncestorOf(nd, pointA)) {
            int index = indexOf(nd, pointB);
            if (index < offsetB ) return -1; 
            return 1; //B strictly before A
        }
    }
    
    // case 4: preorder traversal of context tree.
    DOM_Node ancestor = commonAncestorOf(pointA, pointB);
    DOM_Node current = ancestor;
    
    do {
        if (current == pointA) return -1;
        if (current == pointB) return 1;
        current = nextNode(current, true); 
    }
    while (current!=null && current!=ancestor);
    
    return -2; // this should never happen
}


void RangeImpl:: deleteContents()
{
    if ((fStartContainer == null) || (fEndContainer == null) ) return;
    checkReadOnly(fStartContainer, fEndContainer, fStartOffset, fEndOffset);
    
    if( fDetached) {
        throw DOM_DOMException(
            DOM_DOMException::INVALID_STATE_ERR, null);
    }
    
    DOM_Node current = fStartContainer;
    
    // if same container, simplify case
    if (fStartContainer == fEndContainer) {
        if (fStartOffset == fEndOffset) { // already  collapsed
            return; 
        } 
        if (fStartOffset > fEndOffset)
            throw DOM_RangeException(DOM_RangeException::BAD_BOUNDARYPOINTS_ERR, null);
        
        if (fStartContainer.getNodeType() == DOM_Node::TEXT_NODE) {
            DOMString value = fStartContainer.getNodeValue();
            unsigned int realStart = fStartOffset;
            unsigned int realEnd = fEndOffset;
            
            if (fStartOffset > value.length()) realStart = value.length()-1;
            if (fEndOffset > value.length()) realEnd = value.length()-1;
            
            ((DOM_Text &)fStartContainer).deleteData(realStart, realEnd-realStart);
            
        } else {
            current = fStartContainer.getFirstChild();  
            unsigned int i = 0;
            //move till the start offset 
            for(i = 0; i < fStartOffset && current != null; i++) {
                current=current.getNextSibling();
            }
                        
            //delete all children after the start offset to end offset
            for(i = 0; i < (fEndOffset-fStartOffset) && (current != null); i++) {
                DOM_Node newCurrent = current.getNextSibling();
                removeChild(fStartContainer, current);
                current = newCurrent;
            }
        }
        collapse(true);
        return;
    }
    
    //The case of partial selections and when start container is not the same as end one
    bool deleteCurrent  = false;
    
     // initialize current for startContainer.
    if (current.getNodeType() == DOM_Node::TEXT_NODE) {
        ((DOM_Text &)current).deleteData(fStartOffset, current.getNodeValue().length()-fStartOffset);
    } else {
        current = current.getFirstChild();
        for (unsigned int i = 0 ; i < fStartOffset && current != null; i++){
            current = current.getNextSibling();
        }
        if (current == null) {
            current = fStartContainer;
        } else if (current != fStartContainer)
            deleteCurrent = true;
    }
    
    DOM_Node parent         = null;
    DOM_Node next;
    DOM_Node partialNode    = null;
    int partialInt          = START;
    DOM_Node startRoot      = null;
    
    DOM_Node root = getCommonAncestorContainer();
    // traverse up from the startContainer...
    // current starts as the node to delete;
    while (current != root && current != null) {
        
        parent = current.getParentNode();
        if (parent == root) {
            if (startRoot == null)
                startRoot = current;
        } else {
            if (partialNode == null) {
                partialNode = parent;
                partialInt = AFTER;
            }
        }
        
        if (parent != root) {
            next = current.getNextSibling();
            DOM_Node nextnext;
            while (next != null)  {
                nextnext = next.getNextSibling();
                removeChild(parent, next);
                next = nextnext;
            }
        }
        
        if (deleteCurrent) {
            removeChild(parent, current);
            deleteCurrent = false;
        }
        current = parent;
    }
    
    DOM_Node endRoot = null;
    // initialize current for endContainer.
    current = fEndContainer;
    if (current.getNodeType() == DOM_Node::TEXT_NODE) {
        ((DOM_Text &)current).deleteData(0, fEndOffset); 
    } else {
        
        if (fEndOffset == 0) { // "before"
            current = fEndContainer;
        }
        else {
            current = current.getFirstChild();
            for(unsigned int i = 1; i < fEndOffset && current != null; i++) {
                current=current.getNextSibling();
            }
            if (current==null) { // REVIST: index-out-of-range what to do?
                current = fEndContainer.getLastChild();
            } else 
                if (current != fStartContainer) {
                    deleteCurrent = true;
                }
                
        }
    }
    
    // traverse up from the endContainer...
    while (current != root && current != null) {
        
        parent = current.getParentNode();
        if (parent == root) {
            if (endRoot == null)
                endRoot = current;
        } else {
            if (partialNode==null) {
                partialNode = parent;
                partialInt = BEFORE;
            }
        }
        
        if (parent != root && parent != null) {
            next = current.getPreviousSibling();
            DOM_Node nextnext;
            while (next != null) {
                nextnext = next.getPreviousSibling();
                removeChild(parent, next);
                next = nextnext;
            }
        }
        
        if (deleteCurrent) {
            removeChild(parent, current);
            deleteCurrent = false;
        }
        current = parent;
    }
    
    //if (endRoot == null || startRoot == null) return; //REVIST
    current = endRoot.getPreviousSibling();
    DOM_Node prev = null;
    while (current != null && current != startRoot ) {
        prev = current.getPreviousSibling();
        parent = current.getParentNode();
        if (parent != null) {
            removeChild(parent, current);
        }
        current = prev;
    }
    
    if (partialNode == null) {
        collapse(true);
    } else if (partialInt == AFTER) {
        setStartAfter(partialNode);
        setEndAfter(partialNode);
    } else if (partialInt == BEFORE) {
        setStartBefore(partialNode);
        setEndBefore(partialNode);
    }
}

DOM_DocumentFragment RangeImpl::extractContents()
{
    return traverseContents(EXTRACT_CONTENTS);
}

DOM_DocumentFragment RangeImpl::cloneContents()
{
    return traverseContents(CLONE_CONTENTS);
}


void RangeImpl::insertNode(DOM_Node& newNode)
{
    if (newNode == null) return; //don't have to do anything

    if (fStartContainer.getParentNode().fImpl->readOnly()) {
        throw DOM_DOMException(
            DOM_DOMException::NO_MODIFICATION_ALLOWED_ERR, null);
    }
    
    if (fDocument != newNode.getOwnerDocument()) {
        throw DOM_DOMException(
            DOM_DOMException::WRONG_DOCUMENT_ERR, null);
    }
    
    // Prevent cycles in the tree.
    //isKidOK() is not checked here as its taken care by insertBefore() function
    if (isAncestorOf( newNode, fStartContainer)) {
        throw DOM_DOMException(
            DOM_DOMException::HIERARCHY_REQUEST_ERR, null);
    }
    
    if( fDetached) {
        throw DOM_DOMException(
            DOM_DOMException::INVALID_STATE_ERR, null);
    }
    
    int type = newNode.getNodeType();
    if (type == DOM_Node::ATTRIBUTE_NODE
        || type == DOM_Node::ENTITY_NODE
        || type == DOM_Node::NOTATION_NODE
        || type == DOM_Node::DOCUMENT_NODE
        || type == DOM_Node::DOCUMENT_FRAGMENT_NODE)
    {
        throw DOM_RangeException(
            DOM_RangeException::INVALID_NODE_TYPE_ERR, null);
    }
   

    DOM_Node parent;
    DOM_Node next;

    if (fStartContainer.getNodeType() == DOM_Node::TEXT_NODE) {

        //set 'parent' and 'next' here
        parent = fStartContainer.getParentNode();
        //split the text nodes
       if (fStartOffset > 0) 
            ((DOM_Text &)fStartContainer).splitText(fStartOffset);
        
        //update the new start information later. After inserting the first newNode
        if (fStartOffset == 0) 
            next = fStartContainer;
        else
            next = fStartContainer.getNextSibling();
       
    } // end of text handling
    else {
        parent = fStartContainer;
        
        next = fStartContainer.getFirstChild();
        for(unsigned int i = 0; (i < fStartOffset) && (next != null); i++) {
            next=next.getNextSibling();
        }
    }

    if (parent != null) {
        if (next != null) 
            parent.insertBefore(newNode, next);
        else 
            parent.appendChild(newNode);
    }

}

RangeImpl* RangeImpl::cloneRange()
{
    if( fDetached) {
        throw DOM_DOMException(
            DOM_DOMException::INVALID_STATE_ERR, null);
    }
    
    RangeImpl* range = ((DocumentImpl*)fDocument.fImpl)->createRange();
    range->setStart(fStartContainer, fStartOffset);
    range->setEnd(fEndContainer, fEndOffset);
    
    return range;
}

DOMString RangeImpl::toString()
{
    if( fDetached) {
        throw DOM_DOMException(
            DOM_DOMException::INVALID_STATE_ERR, null);
    }
    
    DOM_Node node = fStartContainer;
    
    DOMString tempString;
    if ( (fStartContainer.getNodeType() == DOM_Node::TEXT_NODE)
        || (fStartContainer.getNodeType() == DOM_Node::CDATA_SECTION_NODE) ) {
        if (fStartContainer == fEndContainer) {
            tempString.appendData(fStartContainer.getNodeValue().substringData(fStartOffset, fEndOffset));
            return tempString;
        } else {
            int length = fStartContainer.getNodeValue().length();
            tempString.appendData(fStartContainer.getNodeValue().substringData(fStartOffset, length - fStartOffset));
        }
    }else if (node == fEndContainer){
        DOM_Node anode = node.getFirstChild();
        unsigned int i = 0;
        for ( ;i<fStartOffset; i++)
            anode = anode.getNextSibling();
        for( ; ( i<fEndOffset || anode!=null); anode = anode.getNextSibling(), i++) {
            if( (anode.getNodeType() == DOM_Node::TEXT_NODE)
                || (anode.getNodeType() == DOM_Node::CDATA_SECTION_NODE)) {
                tempString.appendData(anode.getNodeValue());
            }
        }
        return tempString;
    }
    DOM_Node root = getCommonAncestorContainer();
    
    while (node != fEndContainer) {
         node = nextNode(node, true);
        if ((node == null) || (node == fEndContainer)) break;
        
        if (node.getNodeType() == DOM_Node::TEXT_NODE
            ||  node.getNodeType() == DOM_Node::CDATA_SECTION_NODE
            ) {
            tempString.appendData(node.getNodeValue());
        }
    }
    
    if (fEndContainer.getNodeType() == DOM_Node::TEXT_NODE
        || fEndContainer.getNodeType() == DOM_Node::CDATA_SECTION_NODE) {
        tempString.appendData(fEndContainer.getNodeValue().substringData(0,fEndOffset));
    }
    return tempString;
}

DOM_Document RangeImpl::getDocument()
{
    return fDocument;
}

const DOM_Node& RangeImpl::getCommonAncestorContainer()
{
     return fCommonAncestorContainer = commonAncestorOf(fStartContainer, fEndContainer);    
  
}

//---------------------
//private functions
//---------------------

bool RangeImpl::isValidAncestorType(DOM_Node& node)
{
    for (DOM_Node aNode = node; aNode!=null; aNode = aNode.getParentNode()) {
        short type = aNode.getNodeType();
        if ( type == DOM_Node::ENTITY_NODE
            || type == DOM_Node::NOTATION_NODE
            || type == DOM_Node::DOCUMENT_TYPE_NODE)
            return false;
    }
    return true;
}

bool RangeImpl::isAncestorOf(const DOM_Node& a, const DOM_Node& b) {
    for (DOM_Node node=b; node != null; node=node.getParentNode()) {
        if  (node == a) return true;
    }
    return false;
}

unsigned short RangeImpl::indexOf(const DOM_Node& child, const DOM_Node& parent)
{
    unsigned short i = 0;
    if (child.getParentNode() != parent) return -1;
    for(DOM_Node node = child; node!= null; node=node.getPreviousSibling()) {
        i++;
    }
    return i;
}

void RangeImpl::validateNode(DOM_Node& node)
{
    if( fDetached) {
        throw DOM_DOMException(
            DOM_DOMException::INVALID_STATE_ERR, null);
    }
    
    if ( !isValidAncestorType(node)) {
        throw DOM_RangeException(
            DOM_RangeException::INVALID_NODE_TYPE_ERR, null);
    }
}


DOM_Node RangeImpl::commonAncestorOf(DOM_Node& pointA, DOM_Node& pointB) 
{
    if (fDetached) 
            throw DOM_DOMException(DOM_DOMException::INVALID_STATE_ERR, null);

    if (pointA.getOwnerDocument() != pointB.getOwnerDocument())
        throw DOM_DOMException( DOM_DOMException::WRONG_DOCUMENT_ERR, null );

    //if the containers are same then it itself is its common ancestor.
    if (pointA == pointB)
        return pointA; 

    typedef RefVectorOf<NodeImpl> VectorNodes;
    VectorNodes* startV= new VectorNodes(1, false);
    DOM_Node node;

    for (node=fStartContainer; node != null; node=node.getParentNode()) 
    {
        startV->addElement(node.fImpl);
    }
    VectorNodes* endV = new VectorNodes(1, false);
    for (node=fEndContainer; node != null; node=node.getParentNode()) 
    {
        endV->addElement(node.fImpl);
    }

    int s = startV->size()-1;
    int e = endV->size()-1;
 
    NodeImpl* commonAncestor;
    
    while (s>=0 && e>=0) {
        if (startV->elementAt(s) == endV->elementAt(e)) {
            commonAncestor = startV->elementAt(s);
        }
        else  break;
        --s;
        --e;
    }
    
    delete startV;
    delete endV;
     
    return DOM_Node(commonAncestor);
}

void RangeImpl::checkIndex(DOM_Node& node, unsigned int offset)
{
    validateNode(node);

    if (offset < 0) {
        throw DOM_DOMException( DOM_DOMException::INDEX_SIZE_ERR, null );
    }
    
    short type = node.getNodeType();
    
    if((type == DOM_Node::TEXT_NODE
        || type == DOM_Node::CDATA_SECTION_NODE
        || type == DOM_Node::COMMENT_NODE
        || type == DOM_Node::PROCESSING_INSTRUCTION_NODE)) {
        if (offset > node.getNodeValue().length())
            throw DOM_DOMException( DOM_DOMException::INDEX_SIZE_ERR, null );
        else  return;
    }
    
    DOM_Node child = node.getFirstChild();
    unsigned int i = 0;
    for (; child != null; i++) {
        child = child.getNextSibling();
    }
    if (i < offset) {
        throw DOM_DOMException( DOM_DOMException::INDEX_SIZE_ERR, null );
    }
    
}

DOM_Node RangeImpl::nextNode(const DOM_Node& node, bool visitChildren) {
    
    if (node == null) return null;
    
    DOM_Node result;
    if (visitChildren) {
        result = node.getFirstChild();
        if (result != null) {
            return result;
        }
    }
    
    // if hasSibling, return sibling
    result = node.getNextSibling();
    if (result != null) {
        return result;
    }
    
    
    // return parent's 1st sibling.
    DOM_Node parent = node.getParentNode();
    

    while ( (parent != null) && (parent != fDocument) )
    {
        result = parent.getNextSibling();
        if (result != null) {
            return result;
        } else {
            parent = parent.getParentNode();
            if (parent == fEndContainer) return parent;

        }
        
    }
    // end of list, return null
    return null;            
}


/** This is the master traversal function which is used by
*  both extractContents and cloneContents().
*/
DOM_DocumentFragment RangeImpl::traverseContents(TraversalType trvType)
{

    if (fDetached) 
            throw DOM_DOMException(DOM_DOMException::INVALID_STATE_ERR, null);
    
    if (fStartContainer == null || fEndContainer == null) {
        return DOM_DocumentFragment(); // REVIST: Throw exception?
    }
    
    DOM_DocumentFragment frag = fDocument.createDocumentFragment() ;
    
    DOM_Node current = fStartContainer;
    DOM_Node cloneCurrent = null;
    DOM_Node cloneParent = null;
    DOM_Node partialNode = null;
    int partialInt = START;
    
    // if same container, simplify case
    if (fStartContainer == fEndContainer) {
        if (fStartOffset == fEndOffset) { // eg collapsed
            return frag; // REVIST: what is correct re spec?
        }
        if (fStartContainer.getNodeType() == DOM_Node::TEXT_NODE) {
            cloneCurrent = fStartContainer.cloneNode(false);
            cloneCurrent.setNodeValue(
                cloneCurrent.getNodeValue().substringData(fStartOffset, fEndOffset));
            if (trvType == EXTRACT_CONTENTS) {
                ((DOM_Text &)current).deleteData(fStartOffset, fEndOffset-fStartOffset);
            }
            frag.appendChild(cloneCurrent);
        } else {
            current = current.getFirstChild();
            unsigned int i = 0;
            for(i = 0; i < fStartOffset && current != null; i++) {
                current=current.getNextSibling();
            }
            unsigned int n = fEndOffset-fStartOffset;
            for(i = 0; i < n && current != null ;i++) {
                DOM_Node newCurrent=current.getNextSibling();
                
                if (trvType == CLONE_CONTENTS) {
                    cloneCurrent = current.cloneNode(true);
                    frag.appendChild(cloneCurrent);
                } else
                    if (trvType == EXTRACT_CONTENTS) {
                        frag.appendChild(current);
                    }
                    current = newCurrent;
            }
        }
        if (trvType == EXTRACT_CONTENTS ) {
            collapse(true);
        }
        return frag;
    }
    
    //***** END SIMPLE CASE ****
   

    DOM_Node root = getCommonAncestorContainer();
    DOM_Node parent = null;
    // go up the start tree...
    current = fStartContainer;

    bool endAtRoot = false;
    
    //REVIST: Always clone TEXT_NODE's?
    if (current.getNodeType() == DOM_Node::TEXT_NODE) {
        cloneCurrent = current.cloneNode(false);
        cloneCurrent.setNodeValue(
            ((DOM_Text&)cloneCurrent).getNodeValue().substringData(fStartOffset, current.getNodeValue().length() - fStartOffset));
        if (trvType == EXTRACT_CONTENTS) {
            ((DOM_Text&)current).deleteData(fStartOffset, current.getNodeValue().length()-fStartOffset);
        }
    } else {
        current = current.getFirstChild();
        for(unsigned int i = 0; i < fStartOffset && current != null; i++) {
            current=current.getNextSibling();
        }
        // current is now at the offset.
        if (current==null) { //"after"
            current = fStartContainer;
        }
        
        if (trvType == CLONE_CONTENTS) {
            cloneCurrent = current.cloneNode(true);
        } else if (trvType == EXTRACT_CONTENTS ) {
                cloneCurrent = current;
        }
    }
    
    DOM_Node startRoot = null;
    DOM_Node endRoot = null;
    parent = null;
    

    if (root == fEndContainer) {
        if (fStartContainer.getParentNode() == fEndContainer) { 
            //a unique situation when start and end are partial under the same pass
            DOM_Node endNode = fEndContainer.getFirstChild();
            for (unsigned int i = 0; 
                i <= fEndOffset-2; 
                i++, endNode = endNode.getNextSibling());
            
            if (cloneParent == null)
                cloneParent = root.cloneNode(false);
            
            cloneParent.appendChild(cloneCurrent); //clone the node from above
            
            for (current= current.getNextSibling(); 
                current != null, current != endNode.getNextSibling(); 
                current=current.getNextSibling()) {
                if (trvType == CLONE_CONTENTS) {
                    cloneCurrent = current.cloneNode(true);
                    cloneParent.appendChild(cloneCurrent);
                } else if (trvType == EXTRACT_CONTENTS) {
                        cloneParent.appendChild(current);
                }
            }
            if (trvType == EXTRACT_CONTENTS) {
                collapse(true);
            }
            frag.appendChild(cloneParent);
            return frag;
        }
    }

    // going up in a direct line from boundary point
    // through parents to the common ancestor,
    // all these nodes are partially selected, and must
    // be cloned.
    while (current != root) {
        parent = current.getParentNode();
        
        if (parent == root) {
            cloneParent = frag;
            startRoot = current;
        } else {
            //check if (parent == null) case too
            cloneParent = parent.cloneNode(false);
            if (partialNode==null && parent != root) {
                partialNode = parent;
                partialInt = AFTER;
            }
            
        }
        
        // The children to the "right" of the "ancestor hierarchy"
        // are "fully-selected".
        DOM_Node next = null;
        
        //increment to the next sibling BEFORE doing the appendChild
        current = current.getNextSibling();
        //do this appendChild after the increment above.
        cloneParent.appendChild(cloneCurrent);
        
        while (current != null) {
            next = current.getNextSibling();
            
            if (current != null && parent != root) {
                if (trvType == CLONE_CONTENTS) {
                    cloneCurrent = current.cloneNode(true);
                    cloneParent.appendChild(cloneCurrent);
                } else
                    if (trvType == EXTRACT_CONTENTS) {
                        cloneParent.appendChild(current);
                    }
            }
            current = next;
        }
        
        current = parent;
        cloneCurrent = cloneParent;
    }
    
    // go up the end tree...
    current = fEndContainer;
    
    if (current.getNodeType() == DOM_Node::TEXT_NODE) {
        cloneCurrent = current.cloneNode(false);
        cloneCurrent.setNodeValue(
            (cloneCurrent.getNodeValue()).substringData(0,fEndOffset));
        if (trvType == EXTRACT_CONTENTS) {
            ((DOM_Text&)current).deleteData(0, fEndOffset);
        }
    } else {
        if (fEndOffset == 0) { // "before"
            current = fEndContainer;
        }
        else {
            current = current.getFirstChild();
            for(unsigned int i = 1; i < fEndOffset && current != null; i++) {
                current=current.getNextSibling();
            }
            if (current==null) { // REVIST: index-out-of-range what to do?
                current = fEndContainer.getLastChild();
            }
        }
        if (trvType == CLONE_CONTENTS) {
            cloneCurrent = current.cloneNode(true);
        } else
            if (trvType == EXTRACT_CONTENTS ) {
                cloneCurrent = current;
            }
    }
    
    while (current != root && current != null) {
        parent = current.getParentNode();
        if (parent == root) {
            cloneParent = frag;
            endRoot = current;
        } else {
            cloneParent = parent.cloneNode(false);
            if (partialNode==null && parent != root) {
                partialNode = parent;
                partialInt = BEFORE;
            }
        }
        
        DOM_Node holdCurrent = current;
        
        current = parent.getFirstChild();
        
        cloneParent.appendChild(cloneCurrent);
        
        DOM_Node next = null;
        while (current != holdCurrent && current != null) {
            next = current.getNextSibling();
            // The leftmost children are fully-selected
            // and are removed, and appended, not cloned.
            if (current != null && parent != root) {
                if (trvType == CLONE_CONTENTS) {
                    cloneCurrent = current.cloneNode(true);
                    cloneParent.appendChild(cloneCurrent);
                } else
                    if (trvType == EXTRACT_CONTENTS) {
                        //cloneCurrent = current;
                        cloneParent.appendChild(current);
                    }
            }
            current = next;
        }
        
        current = parent;
        cloneCurrent = cloneParent;
        
    }
    
    // traverse the "fully-selected" middle...
    DOM_Node clonedPrevious = frag.getLastChild();
    current = endRoot.getPreviousSibling();
    DOM_Node prev = null;
    while (current != startRoot && current != null) {
        prev = current.getPreviousSibling();
        
        if (trvType == CLONE_CONTENTS) {
            cloneCurrent = current.cloneNode(true);
        } else
            if (trvType == EXTRACT_CONTENTS) {
                cloneCurrent = current;
            }
            
            frag.insertBefore(cloneCurrent, clonedPrevious);
            
            current = prev;
            clonedPrevious = cloneCurrent;
    }
    
    // collapse the range...
    if (trvType == EXTRACT_CONTENTS ) {
        if (partialNode == null) {
            collapse(true);
        } else
            if (partialInt == AFTER) {
                setStartAfter(partialNode);
                setEndAfter(partialNode);
            }
            else if (partialInt == BEFORE) {
                setStartBefore(partialNode);
                setEndBefore(partialNode);
            }
    }

    
    return frag;
}

void RangeImpl::checkReadOnly(DOM_Node& start, DOM_Node& end, 
                              unsigned int startOffset, unsigned int endOffset)
{
    if ((start == null) || (end == null) ) return;
    //if both start and end are text check and return
    if (start.getNodeType() == DOM_Node::TEXT_NODE) {
        if (start.fImpl->readOnly()) {
            throw DOM_DOMException(
                DOM_DOMException::NO_MODIFICATION_ALLOWED_ERR, null);
        }
        if (start == end)
            return;
    }
    //set the start and end nodes to check
    DOM_Node sNode = start.getFirstChild();
    for(unsigned int i = 0; i<startOffset; i++)
        sNode = sNode.getNextSibling();
    
    DOM_Node eNode;
    if (end.getNodeType() == DOM_Node::TEXT_NODE) {
        eNode = end; //need to check only till this node
    }
    else { //need to check all the kids that fall before the end offset value
        eNode = end.getFirstChild();
        for (unsigned int i = 0; i<endOffset-1; i++)
            eNode = eNode.getNextSibling();
    }
    //recursivly search if any node is readonly
    recurseTreeAndCheck(sNode, eNode);
}

void RangeImpl::recurseTreeAndCheck(DOM_Node& start, DOM_Node& end)
{
    for(DOM_Node node=start; node != null, node !=end; node=node.getNextSibling()) 
    {
        if (node.fImpl->readOnly()) {
            throw DOM_DOMException(
                DOM_DOMException::NO_MODIFICATION_ALLOWED_ERR, null);
        }
        
        if (node.hasChildNodes()) {
            node = node.getFirstChild();
            recurseTreeAndCheck(node, end);
        }
    }
}


DOM_Node RangeImpl::removeChild(DOM_Node& parent, DOM_Node& child) 
{
    fRemoveChild = child; //only a precaution measure not to update this range data before removal
    DOM_Node n = parent.removeChild(child);
    fRemoveChild = null;
    return n;
}


//
// Mutation functions
//


/* This function is called from DOM.
*  The  text has already beeen replaced.
*  Fix-up any offsets.
*/
void RangeImpl::receiveReplacedText(NodeImpl* node) 
{
    if (node == null) return;
    DOM_Node anode(node);
    if (anode == fStartContainer
        && fStartContainer.getNodeType() == DOM_Node::TEXT_NODE) {
        fStartOffset = 0;
    }
    if (anode == fEndContainer
        && fEndContainer.getNodeType() == DOM_Node::TEXT_NODE) {
        fEndOffset = 0;
    }
}

 
/** This function is called from DOM.
*  The  text has already beeen inserted.
*  Fix-up any offsets.
*/
void RangeImpl::updateRangeForDeletedText(DOM_Node& node, unsigned int offset, int count) 
{
    if (node == null) return;
    
    if (node == fStartContainer
        && fStartContainer.getNodeType() == DOM_Node::TEXT_NODE) {
        if (fStartOffset > offset+count) {
            fStartOffset = fStartOffset-count;
        } else if (fStartOffset > offset) {
            fStartOffset = offset;
        }
    }
    if (node == fEndContainer
        && fEndContainer.getNodeType() == DOM_Node::TEXT_NODE) {
        if (fEndOffset > offset+count) {
            fEndOffset = fEndOffset-count;
        } else if (fEndOffset > offset) { 
            fEndOffset = offset;
        }
    }
}



/** This function must be called by the DOM _BEFORE_
*  a node is deleted, because at that time it is
*  connected in the DOM tree, which we depend on.
*/
void RangeImpl::updateRangeForDeletedNode(NodeImpl* node) 
{
    
    if (node == null) return;
    if (fRemoveChild == node) return; 
    
    DOM_Node tNode(node);
    
    if (node->getParentNode() == fStartContainer.fImpl) {
        unsigned short index = indexOf(tNode, fStartContainer);
        if ( fStartOffset > index) {
            fStartOffset--;
        }
    }
    
    if (node->getParentNode() == fEndContainer.fImpl) {
        unsigned short index = indexOf(tNode, fEndContainer);
        if ( fEndOffset < index) {
            fEndOffset--;
        }
    }
    
    if (node->getParentNode() != fStartContainer.fImpl
        &&  node->getParentNode() != fEndContainer.fImpl) {
        if (isAncestorOf(node, fStartContainer)) {
            if (( node->getParentNode()->getNodeType() == DOM_Node::DOCUMENT_FRAGMENT_NODE) )
                return; //if the node's up in the heirarchy and its parent is doc-frag ignore
            DOM_Node tpNode(node->getParentNode());
            setStartContainer( tpNode );
            fStartOffset = indexOf( tNode, tpNode)-1;
        }
        if (isAncestorOf(node, fEndContainer)) {
            if (( node->getParentNode()->getNodeType() == DOM_Node::DOCUMENT_FRAGMENT_NODE))
                return;
            DOM_Node tpNode(node->getParentNode());
            setEndContainer( tpNode );
            fEndOffset = indexOf( tNode, tpNode)-1;
        }
    }
    
}

void RangeImpl::updateRangeForInsertedNode(NodeImpl* node) {
    if (node == null) return;
    
    if (node->getParentNode() == fStartContainer.fImpl) {
        unsigned int index = indexOf(DOM_Node(node), fStartContainer) -1;
        if (index < fStartOffset) {
            fStartOffset++;
        }
    }
    
    if (node->getParentNode() == fEndContainer.fImpl) {
        unsigned int index = indexOf(DOM_Node(node), fEndContainer);
        //if index is equal then the text is inserted before the end of 
        //range so should get included in the range
        
        if (index <= fEndOffset) {
            fEndOffset++;
        }
    }
}


void RangeImpl::updateSplitInfo(TextImpl* oldNode, TextImpl* startNode)
{
    if (startNode == null) return;
    
    DOM_Text oldText(oldNode);
    DOM_Text newText(startNode);
    unsigned int oldStartOffset;

    if (fStartContainer == oldText) {
        oldStartOffset = fStartOffset;
        fStartContainer = newText;
        fStartOffset = 0;
        
        if (fEndContainer == oldText) {
            fEndContainer = newText;
            fEndOffset = fEndOffset - oldStartOffset;
        }
    }
}



