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

#include "NodeContainer.hpp"
#include "DOM_DOMException.hpp"
#include "DOM_Node.hpp"
#include "DOM_DOMImplementation.hpp"
#include "DOMString.hpp"
#include "DStringPool.hpp"
#include "DocumentImpl.hpp"
#include "stdio.h"
#include "TextImpl.hpp"

static DOMString *s_xml = null;
static DOMString *s_xmlURI = null;
static DOMString *s_xmlns = null;
static DOMString *s_xmlnsURI = null;

NodeContainer::NodeContainer(DocumentImpl *ownerDoc)
    : NodeImpl(ownerDoc)
{
    this->firstChild = null;
    this->lastChild  = null;
};  

// This only makes a shallow copy, cloneChildren must also be called for a
// deep clone
NodeContainer::NodeContainer(const NodeContainer &other)
    : NodeImpl(other)
{
    // Need to break the association w/ original kids
    this->firstChild = null;
    this->lastChild = null;
};


void NodeContainer::cloneChildren(const NodeImpl &other) {    
    for (NodeImpl *mykid = ((NodeImpl&)other).getFirstChild(); 
         mykid != null; 
         mykid = mykid->nextSibling) {
        this->appendChild(mykid->cloneNode(true));
    }
}

NodeImpl * NodeContainer::appendChild(NodeImpl *newChild)      
{
    return insertBefore(newChild, null);
};


NodeListImpl *NodeContainer::getChildNodes() {
    return this;
};


NodeImpl * NodeContainer::getFirstChild() {
    return firstChild;
}; 


NodeImpl * NodeContainer::getLastChild()
{
    return lastChild;
}; 


unsigned int NodeContainer::getLength() {
    unsigned int count = 0;
    NodeImpl *node = firstChild;
    while(node != null)
    {
        ++count;
        node = node->nextSibling;
    }
    return count;
};


bool NodeContainer::hasChildNodes()
{ 
    return firstChild!=null;
}; 



NodeImpl *NodeContainer::insertBefore(NodeImpl *newChild, NodeImpl *refChild) {
    if (readOnly)
        throw DOM_DOMException(
        DOM_DOMException::NO_MODIFICATION_ALLOWED_ERR, null);
    
    if(  !(newChild->getOwnerDocument() == ownerDocument   ||
        // SPECIAL-CASE: Document has no owner, but may be the owner.
        ( this->isDocumentImpl() &&
        newChild->getOwnerDocument() == (DocumentImpl *)this ) 
        ) )
        throw DOM_DOMException(DOM_DOMException::WRONG_DOCUMENT_ERR, null);
    
    // Convert to internal type, to avoid repeated casting  
    //   (left over from the original Java.  Meaningless in this version.)
    NodeImpl * newInternal= newChild;
    
    // Prevent cycles in the tree
    bool treeSafe=true;
    for(NodeImpl *a=this->getParentNode();
        treeSafe && a!=null;
        a=a->getParentNode())
        treeSafe=(newInternal!=a);
    if(!treeSafe)
        throw DOM_DOMException(DOM_DOMException::HIERARCHY_REQUEST_ERR,null);
    
    // refChild must in fact be a child of this node (or null)
    if(refChild!=null && refChild->ownerNode != this)
        throw DOM_DOMException(DOM_DOMException::NOT_FOUND_ERR,null);
    
    if (newInternal->isDocumentFragmentImpl())
    {
        // SLOW BUT SAFE: We could insert the whole subtree without
        // juggling so many next/previous pointers. (Wipe out the
        // parent's child-list, patch the parent pointers, set the
        // ends of the list.) But we know some subclasses have special-
        // case behavior they add to insertBefore(), so we don't risk it.
        // This approch also takes fewer bytecodes.
        
        // NOTE: If one of the children is not a legal child of this
        // node, throw HIERARCHY_REQUEST_ERR before _any_ of the children
        // have been transferred. (Alternative behaviors would be to
        // reparent up to the first failure point or reparent all those
        // which are acceptable to the target node, neither of which is
        // as robust. PR-DOM-0818 isn't entirely clear on which it
        // recommends?????
        
        // No need to check kids for right-document; if they weren't,
        // they wouldn't be kids of that DocFrag.
        for(NodeImpl *kid=newInternal->getFirstChild(); // Prescan
        kid!=null;
        kid=kid->getNextSibling())
        {
            if(!isKidOK(this,kid))
                throw DOM_DOMException(DOM_DOMException::HIERARCHY_REQUEST_ERR,null);
        }                       
        while(newInternal->hasChildNodes())     // Move
            insertBefore(newInternal->getFirstChild(),refChild);
    }
    
    else if(!isKidOK(this, newInternal))
        throw DOM_DOMException(DOM_DOMException::HIERARCHY_REQUEST_ERR,null);
    
    else
    {
        NodeImpl *oldparent=newInternal->ownerNode;
        if(oldparent!=null)
            oldparent->removeChild(newInternal);
        
        NodeImpl *prev;
        // Find the node we're inserting after, if any (null if
        // inserting to head of list)
        prev= (refChild==null)
            ? lastChild
            : refChild->previousSibling;
        
        // Attach up
        newInternal->ownerNode=this;
        
        // Attach after
        newInternal->previousSibling=prev;
        if(prev==null)
            firstChild=newInternal;
        else
            prev->nextSibling=newInternal;
        
        // Attach before
        newInternal->nextSibling = refChild;
        if(refChild==null)
            lastChild=newInternal;
        else
            refChild->previousSibling=newInternal;
    }
    changed();
    
    return newInternal;
};


/** Uses the kidOK lookup table to check whether the proposed
    tree structure is legal.

    ????? It feels like there must be a more efficient solution,
    but for the life of me I can't think what it would be.
*/
bool NodeContainer::isKidOK(NodeImpl *parent, NodeImpl *child)
{
      static int kidOK[13];
      
      if (kidOK[DOM_Node::DOCUMENT_NODE] == 0)
      {
          kidOK[DOM_Node::DOCUMENT_NODE] = 
              1 << DOM_Node::ELEMENT_NODE |
              1 << DOM_Node::PROCESSING_INSTRUCTION_NODE | 
              1 << DOM_Node::COMMENT_NODE | 
              1 << DOM_Node::DOCUMENT_TYPE_NODE;
          
          kidOK[DOM_Node::DOCUMENT_FRAGMENT_NODE] = 
              kidOK[DOM_Node::ENTITY_NODE] = 
              kidOK[DOM_Node::ENTITY_REFERENCE_NODE] = 
              kidOK[DOM_Node::ELEMENT_NODE] = 
              1 << DOM_Node::ELEMENT_NODE |
              1 << DOM_Node::PROCESSING_INSTRUCTION_NODE | 
              1 << DOM_Node::COMMENT_NODE |
              1 << DOM_Node::TEXT_NODE |
              1 << DOM_Node::CDATA_SECTION_NODE |
              1 << DOM_Node::ENTITY_REFERENCE_NODE;
          
          kidOK[DOM_Node::DOCUMENT_TYPE_NODE] = 
              1 << DOM_Node::NOTATION_NODE |
              1 << DOM_Node::ENTITY_NODE;
          
          kidOK[DOM_Node::ATTRIBUTE_NODE] = 
              1 << DOM_Node::TEXT_NODE |
              1 << DOM_Node::ENTITY_REFERENCE_NODE;
          
          kidOK[DOM_Node::PROCESSING_INSTRUCTION_NODE] = 
              kidOK[DOM_Node::COMMENT_NODE] = 
              kidOK[DOM_Node::TEXT_NODE] = 
              kidOK[DOM_Node::CDATA_SECTION_NODE] = 
              kidOK[DOM_Node::NOTATION_NODE] = 
              0;
      };
      int p=parent->getNodeType();
      int ch = child->getNodeType();
      return (kidOK[p] & 1<<ch) != 0;
};
  
  
  
NodeImpl *NodeContainer::item(unsigned int index) {
    NodeImpl *node = firstChild;
    for(unsigned int i=0; i<index && node!=null; ++i)
        node = node->nextSibling;
    return node;
};
  
  
NodeImpl *NodeContainer::removeChild(NodeImpl *oldChild) 
{
    if (readOnly)
        throw DOM_DOMException(
          DOM_DOMException::NO_MODIFICATION_ALLOWED_ERR, null);
      
      if (oldChild != null && oldChild->ownerNode != this)
          throw DOM_DOMException(DOM_DOMException::NOT_FOUND_ERR, null);
      
      // Patch tree past oldChild
      NodeImpl *prev = oldChild->previousSibling;
      NodeImpl *next = oldChild->nextSibling;
      if (prev != null)
          prev->nextSibling = next;
      else
          firstChild = next;
      if (next != null)
          next->previousSibling = prev;
      else
          lastChild = prev;
      
      // Remove oldChild's references to tree
      oldChild->ownerNode = null;
      oldChild->nextSibling = null;
      oldChild->previousSibling = null;
      changed();
      return oldChild;
};
  
  
NodeImpl *NodeContainer::replaceChild(NodeImpl *newChild, NodeImpl *oldChild)
{
    insertBefore(newChild, oldChild);
    // changed() already done.
    return removeChild(oldChild);
};
  

void NodeContainer::setReadOnly(bool readOnl, bool deep)
{
    NodeImpl::setReadOnly(readOnl, deep);
      
    if (deep)
        // Recursively set kids
        for (NodeImpl *mykid = firstChild; 
             mykid != null; 
             mykid = mykid->nextSibling)
            if(! (mykid->isEntityReference()))
                mykid->setReadOnly(readOnl,true);
};
  
  
//Introduced in DOM Level 2
  
void NodeContainer::normalize()
{
    NodeImpl *kid, *next;
    for (kid = getFirstChild(); kid != null; kid = next)
    {
        next = kid->getNextSibling();
        
        // If kid and next are both Text nodes (but _not_ CDATASection,
        // which is a subclass of Text), they can be merged.
        if (next != null && 
            kid->isTextImpl()   && !(kid->isCDATASectionImpl())  && 
            next->isTextImpl()  && !(next->isCDATASectionImpl()) )
        {
            ((TextImpl *) kid)->appendData(((TextImpl *) next)->getData());
            removeChild(next);
            if (next->nodeRefCount == 0)
                deleteIf(next);
            next = kid; // Don't advance; there might be another.
        }
        
        // Otherwise it might be an Element, which is handled recursively  
        else
            if (kid->isElementImpl())
                kid->normalize();
    };
    
    // changed() will have occurred when the removeChild() was done,
    // so does not have to be reissued.
};
