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

#include <util/XercesDefs.hpp>
#include "IDOM_DOMException.hpp"
#include "IDOM_Node.hpp"

#include "IDDocumentImpl.hpp"
#include "IDNodeListImpl.hpp"
#include "IDParentNode.hpp"
#include "IDCasts.hpp"


IDParentNode::IDParentNode(IDOM_Document *ownerDoc)
    : fOwnerDocument(ownerDoc), fChildNodeList(castToNode(this))
{
    fFirstChild = 0;
};  

// This only makes a shallow copy, cloneChildren must also be called for a
// deep clone
IDParentNode::IDParentNode(const IDParentNode &other)  :
    fChildNodeList(castToNode(this))
{
    this->fOwnerDocument = other.fOwnerDocument;

    // Need to break the association w/ original kids
    this->fFirstChild = 0;
};


IDOM_Node * IDParentNode::appendChild(IDOM_Node *newChild)      
{
    return insertBefore(newChild, 0);
};


void IDParentNode::cloneChildren(const IDOM_Node *other) {    
  //    for (IDOM_Node *mykid = other.getFirstChild(); 
    for (IDOM_Node *mykid = other->getFirstChild(); 
         mykid != 0; 
         mykid = mykid->getNextSibling()) {
            castToNodeImpl(this)->appendChild(mykid->cloneNode(true));
    }
}

IDOM_Document * IDParentNode::getOwnerDocument() const {
    return fOwnerDocument;
}

// unlike getOwnerDocument this is not overriden by DocumentImpl to return 0
IDOM_Document * IDParentNode::getDocument() const {
    return fOwnerDocument;
}


IDOM_NodeList *IDParentNode::getChildNodes() const {
    const IDOM_NodeList *ret = &fChildNodeList;
    return (IDOM_NodeList *)ret;   // cast off const.
};


IDOM_Node * IDParentNode::getFirstChild() const {
    return fFirstChild;
}; 


IDOM_Node * IDParentNode::getLastChild() const
{
    return lastChild();
}; 

IDOM_Node * IDParentNode::lastChild() const
{
    // last child is stored as the previous sibling of first child
    if (fFirstChild == 0) {
        return 0;
    }

    IDChildNode *firstChild = castToChildImpl(fFirstChild);
    IDOM_Node *ret = firstChild->previousSibling;
    return ret;
}; 


//
//  idom_revisit.  Is this function used anywhere?  I don't see it.
//
void IDParentNode::lastChild(IDOM_Node *node) {
    // store lastChild as previous sibling of first child
    if (fFirstChild != 0) {
        IDChildNode *firstChild = castToChildImpl(fFirstChild);
        firstChild->previousSibling = node;
    }
}


bool IDParentNode::hasChildNodes() const
{ 
    return fFirstChild!=0;
}; 



IDOM_Node *IDParentNode::insertBefore(IDOM_Node *newChild, IDOM_Node *refChild) {
    IDNodeImpl *thisNodeImpl = castToNodeImpl(this);
    if (thisNodeImpl->isReadOnly())
        throw IDOM_DOMException(IDOM_DOMException::NO_MODIFICATION_ALLOWED_ERR, 0);
    
    if (newChild->getOwnerDocument() != fOwnerDocument)
        throw IDOM_DOMException(IDOM_DOMException::WRONG_DOCUMENT_ERR, 0);
    
    
    // Prevent cycles in the tree
    bool treeSafe=true;
    for(IDOM_Node *a=castToNode(this)->getParentNode();
        treeSafe && a!=0;
        a=a->getParentNode())
        treeSafe=(newChild!=a);
    if(!treeSafe)
        throw IDOM_DOMException(IDOM_DOMException::HIERARCHY_REQUEST_ERR,0);
    
    // refChild must in fact be a child of this node (or 0)
    if (refChild!=0 && refChild->getParentNode() != castToNode(this))
        throw IDOM_DOMException(IDOM_DOMException::NOT_FOUND_ERR,0);
    
    if (newChild->getNodeType() == IDOM_Node::DOCUMENT_FRAGMENT_NODE)
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
        for(IDOM_Node *kid=newChild->getFirstChild(); // Prescan
              kid!=0;
              kid=kid->getNextSibling())
        {
            if (!IDDocumentImpl::isKidOK(castToNode(this), kid))
              throw IDOM_DOMException(IDOM_DOMException::HIERARCHY_REQUEST_ERR,0);
        }                       
        while(newChild->hasChildNodes())     // Move
            insertBefore(newChild->getFirstChild(),refChild);
    }
    
    else if (!IDDocumentImpl::isKidOK(castToNode(this), newChild))
        throw IDOM_DOMException(IDOM_DOMException::HIERARCHY_REQUEST_ERR,0);
    
    else
    {
        IDOM_Node *oldparent=newChild->getParentNode();
        if(oldparent!=0) {
            oldparent->removeChild(newChild);
        }

        // Attach up
        castToNodeImpl(newChild)->fOwnerNode = castToNode(this);
        castToNodeImpl(newChild)->isOwned(true);
        
        // Attach before and after
        // Note: fFirstChild.previousSibling == lastChild!!
        if (fFirstChild == 0) {
            // this our first and only child
            fFirstChild = newChild;
            castToNodeImpl(newChild)->isFirstChild(true);
            // castToChildImpl(newChild)->previousSibling = newChild;
            IDChildNode *newChild_ci = castToChildImpl(newChild);
            newChild_ci->previousSibling = newChild;
        } else {
            if (refChild == 0) {
                // this is an append
                IDOM_Node *lastChild = castToChildImpl(fFirstChild)->previousSibling;
                castToChildImpl(lastChild)->nextSibling = newChild;
                castToChildImpl(newChild)->previousSibling = lastChild;
                castToChildImpl(fFirstChild)->previousSibling = newChild;
            } else {
                // this is an insert
                if (refChild == fFirstChild) {
                    // at the head of the list
                    castToNodeImpl(fFirstChild)->isFirstChild(false);
                    castToChildImpl(newChild)->nextSibling = fFirstChild;
                    castToChildImpl(newChild)->previousSibling = castToChildImpl(fFirstChild)->previousSibling;
                    castToChildImpl(fFirstChild)->previousSibling = newChild;
                    fFirstChild = newChild;
                    castToNodeImpl(newChild)->isFirstChild(true);
                } else {
                    // somewhere in the middle
                    IDOM_Node *prev = castToChildImpl(refChild)->previousSibling;
                    castToChildImpl(newChild)->nextSibling = refChild;
                    castToChildImpl(prev)->nextSibling = newChild;
                    castToChildImpl(refChild)->previousSibling = newChild;
                    castToChildImpl(newChild)->previousSibling = prev;
                }
            }
        }
    }

    changed();
    
#ifdef idom_revisit
    if (this->getOwnerDocument() != 0) {
        typedef RefVectorOf<RangeImpl> RangeImpls;
        RangeImpls* ranges = this->getOwnerDocument()->getRanges();
        if ( ranges != 0) {
            unsigned int sz = ranges->size();
            if (sz != 0) {
                for (unsigned int i =0; i<sz; i++) {
                    ranges->elementAt(i)->updateRangeForInsertedNode(newChild);
                }
            }
        }
    }
#endif

    return newChild;
};
  
  
  
IDOM_Node *IDParentNode::removeChild(IDOM_Node *oldChild) 
{
    if (castToNodeImpl(this)->isReadOnly())
        throw IDOM_DOMException(
        IDOM_DOMException::NO_MODIFICATION_ALLOWED_ERR, 0);
    
    if (oldChild != 0 && oldChild->getParentNode() != castToNode(this))
        throw IDOM_DOMException(IDOM_DOMException::NOT_FOUND_ERR, 0);
    
    //fix other ranges for change before deleting the node
#ifdef idom_revisit
    if (this->getOwnerDocument() !=  0  ) {
        typedef RefVectorOf<RangeImpl> RangeImpls;
        RangeImpls* ranges = this->getOwnerDocument()->getRanges();
        if (ranges != 0) {
            unsigned int sz = ranges->size();
            if (sz != 0) {
                for (unsigned int i =0; i<sz; i++) {
                    if (ranges->elementAt(i) != 0) 
                        ranges->elementAt(i)->updateRangeForDeletedNode(oldChild);
                }
            }
        }
    }
#endif
    
   
    // Patch linked list around oldChild
    // Note: lastChild == fFirstChild->previousSibling
    if (oldChild == fFirstChild) {
        // removing first child
        castToNodeImpl(oldChild)->isFirstChild(false);
        fFirstChild = castToChildImpl(oldChild)->nextSibling;
        if (fFirstChild != 0) {
            castToNodeImpl(fFirstChild)->isFirstChild(true);
            castToChildImpl(fFirstChild)->previousSibling = castToChildImpl(oldChild)->previousSibling;
        }
    } else {
        IDOM_Node *prev = castToChildImpl(oldChild)->previousSibling;
        IDOM_Node *next = castToChildImpl(oldChild)->nextSibling;
        castToChildImpl(prev)->nextSibling = next;
        if (next == 0) {
            // removing last child
            castToChildImpl(fFirstChild)->previousSibling = prev;
        } else {
            // removing some other child in the middle
            castToChildImpl(next)->previousSibling = prev;
        }
    }

    // Remove oldChild's references to tree
    castToNodeImpl(oldChild)->fOwnerNode = fOwnerDocument;
    castToNodeImpl(oldChild)->isOwned(false);
    castToChildImpl(oldChild)->nextSibling = 0;
    castToChildImpl(oldChild)->previousSibling = 0;

    changed();

    return oldChild;
};


IDOM_Node *IDParentNode::replaceChild(IDOM_Node *newChild, IDOM_Node *oldChild)
{
    insertBefore(newChild, oldChild);
    // changed() already done.
    return removeChild(oldChild);
};
  

  
//Introduced in DOM Level 2
  
void IDParentNode::normalize()
{
    IDOM_Node *kid, *next;
    for (kid = fFirstChild; kid != 0; kid = next)
    {
        next = castToChildImpl(kid)->nextSibling;
        
        // If kid and next are both Text nodes (but _not_ CDATASection,
        // which is a subclass of Text), they can be merged.
        if (next != 0 && 
            kid->getNodeType() == IDOM_Node::TEXT_NODE   && 
            next->getNodeType() == IDOM_Node::TEXT_NODE )
        {
            ((IDTextImpl *) kid)->appendData(((IDTextImpl *) next)->getData());
            removeChild(next);
            next = kid; // Don't advance; there might be another.
        }
        
        // Otherwise it might be an Element, which is handled recursively  
        else
            if (kid->getNodeType() == IDOM_Node::ELEMENT_NODE)
                kid->normalize();
    };
    
    // changed() will have occurred when the removeChild() was done,
    // so does not have to be reissued.
};

