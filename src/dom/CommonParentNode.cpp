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



THIS_CLASS::THIS_CLASS(DocumentImpl *ownerDoc)
    : PARENT_CLASS(ownerDoc)
{
    this->ownerDocument = ownerDoc;
    this->firstChild = null;

    fChanges = 0;
};  

// This only makes a shallow copy, cloneChildren must also be called for a
// deep clone
THIS_CLASS::THIS_CLASS(const THIS_CLASS &other)
    : PARENT_CLASS(other)
{
    this->ownerDocument = other.ownerDocument;

    // Need to break the association w/ original kids
    this->firstChild = null;

    fChanges = 0;
};


void THIS_CLASS::cloneChildren(const NodeImpl &other) {    
  //    for (NodeImpl *mykid = other.getFirstChild(); 
    for (NodeImpl *mykid = ((NodeImpl&)other).getFirstChild(); 
         mykid != null; 
         mykid = mykid->getNextSibling()) {
        this->appendChild(mykid->cloneNode(true));
    }
}

DocumentImpl * THIS_CLASS::getOwnerDocument() {
    return ownerDocument;
}

// unlike getOwnerDocument this is not overriden by DocumentImpl to return null
DocumentImpl * THIS_CLASS::getDocument() {
    return ownerDocument;
}

void THIS_CLASS::setOwnerDocument(DocumentImpl *doc) {
    ownerDocument = doc;
    for (NodeImpl *child = firstChild;
         child != null; child = child->getNextSibling()) {
        child->setOwnerDocument(doc);
    }
}


void THIS_CLASS::changed()
{
    ++fChanges;
    NodeImpl *parentNode = getParentNode();
    if (parentNode != null) {
        parentNode->changed();
    }
};  

int THIS_CLASS::changes()
{
    return fChanges;
};  


NodeListImpl *THIS_CLASS::getChildNodes() {
    return this;
};


NodeImpl * THIS_CLASS::getFirstChild() {
    return firstChild;
}; 


NodeImpl * THIS_CLASS::getLastChild()
{
    return lastChild();
}; 

ChildNode * THIS_CLASS::lastChild()
{
    // last child is stored as the previous sibling of first child
    return firstChild != null ? firstChild->previousSibling : null;
}; 

void THIS_CLASS::lastChild(ChildNode *node) {
        // store lastChild as previous sibling of first child
        if (firstChild != null) {
            firstChild->previousSibling = node;
        }
    }


unsigned int THIS_CLASS::getLength() {
    unsigned int count = 0;
    ChildNode *node = firstChild;
    while(node != null)
    {
        ++count;
        node = node->nextSibling;
    }
    return count;
};


bool THIS_CLASS::hasChildNodes()
{ 
    return firstChild!=null;
}; 



NodeImpl *THIS_CLASS::insertBefore(NodeImpl *newChild, NodeImpl *refChild) {
    if (readOnly())
        throw DOM_DOMException(
        DOM_DOMException::NO_MODIFICATION_ALLOWED_ERR, null);
    
    if (newChild->getOwnerDocument() != ownerDocument)
        throw DOM_DOMException(DOM_DOMException::WRONG_DOCUMENT_ERR, null);
    
    // Convert to internal type, to avoid repeated casting  
    ChildNode * newInternal= (ChildNode *)newChild;
    
    // Prevent cycles in the tree
    bool treeSafe=true;
    for(NodeImpl *a=this->getParentNode();
        treeSafe && a!=null;
        a=a->getParentNode())
        treeSafe=(newInternal!=a);
    if(!treeSafe)
        throw DOM_DOMException(DOM_DOMException::HIERARCHY_REQUEST_ERR,null);
    
    // refChild must in fact be a child of this node (or null)
    if (refChild!=null && refChild->getParentNode() != this)
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
            if (!DocumentImpl::isKidOK(this, kid))
              throw DOM_DOMException(DOM_DOMException::HIERARCHY_REQUEST_ERR,null);
        }                       
        while(newInternal->hasChildNodes())     // Move
            insertBefore(newInternal->getFirstChild(),refChild);
    }
    
    else if (!DocumentImpl::isKidOK(this, newInternal))
        throw DOM_DOMException(DOM_DOMException::HIERARCHY_REQUEST_ERR,null);
    
    else
    {
        // Convert to internal type, to avoid repeated casting
        ChildNode *refInternal = (ChildNode *)refChild;

        NodeImpl *oldparent=newInternal->getParentNode();
        if(oldparent!=null)
            oldparent->removeChild(newInternal);
        
        ChildNode *prev;
        // Find the node we're inserting after, if any (null if
        // inserting to head of list)
        prev = (refInternal == null)
            ? lastChild() : refInternal->previousSibling;
        
        // Attach up
        newInternal->ownerNode = this;
        newInternal->owned(true);
        
        // Attach after
        newInternal->previousSibling=prev;
        if (refInternal == firstChild) {
            firstChild = newInternal;
            if (refInternal != null)
                 refInternal->firstChild(false);
            newInternal->firstChild(true);
        } else {
            prev->nextSibling = newInternal;
        }

        // Attach before
        newInternal->nextSibling = refInternal;
        if(refInternal == null) {
            // store lastChild as previous sibling of first child
            firstChild->previousSibling = newInternal;
        } else {
            refInternal->previousSibling = newInternal;
        }
    }
    changed();
    
    if (this->getOwnerDocument() != null) {
        typedef RefVectorOf<RangeImpl> RangeImpls;
        RangeImpls* ranges = this->getOwnerDocument()->getRanges();
        if ( ranges != null) {
            unsigned int sz = ranges->size();
            if (sz != 0) {
                for (unsigned int i =0; i<sz; i++) {
                    ranges->elementAt(i)->updateRangeForInsertedNode(newInternal);
                }
            }
        }
    }

    return newInternal;
};
  
  
NodeImpl *THIS_CLASS::item(unsigned int index) {
    ChildNode *node = firstChild;
    for(unsigned int i=0; i<index && node!=null; ++i)
        node = node->nextSibling;
    return node;
};
  
  
NodeImpl *THIS_CLASS::removeChild(NodeImpl *oldChild) 
{
    if (readOnly())
        throw DOM_DOMException(
        DOM_DOMException::NO_MODIFICATION_ALLOWED_ERR, null);
    
    if (oldChild != null && oldChild->getParentNode() != this)
        throw DOM_DOMException(DOM_DOMException::NOT_FOUND_ERR, null);
    
    //fix other ranges for change before deleting the node
    if (this->getOwnerDocument() !=  null  ) {
        typedef RefVectorOf<RangeImpl> RangeImpls;
        RangeImpls* ranges = this->getOwnerDocument()->getRanges();
        if (ranges != null) {
            unsigned int sz = ranges->size();
            if (sz != 0) {
                for (unsigned int i =0; i<sz; i++) {
                    if (ranges->elementAt(i) != null) 
                        ranges->elementAt(i)->updateRangeForDeletedNode(oldChild);
                }
            }
        }
    }
    
    ChildNode * oldInternal = (ChildNode *) oldChild;
    
    // Patch tree past oldChild
    ChildNode *prev = oldInternal->previousSibling;
    ChildNode *next = oldInternal->nextSibling;
    
    if (oldInternal != firstChild)
        prev->nextSibling = next;
    else {
        oldInternal->firstChild(false);
        firstChild = next;
        if (next != null) {
            next->firstChild(true);
        }
    }
    
    if (next != null)         // oldInternal != lastChild
        next->previousSibling = prev;
    else {
        if (firstChild != null) {
            // store lastChild as previous sibling of first child
            firstChild->previousSibling = prev;
        }
    }
    
    // Remove oldChild's references to tree
    oldInternal->ownerNode = ownerDocument;
    oldInternal->owned(false);
    oldInternal->nextSibling = null;
    oldInternal->previousSibling = null;
    
    changed();
    
    return oldInternal;
};


NodeImpl *THIS_CLASS::replaceChild(NodeImpl *newChild, NodeImpl *oldChild)
{
    insertBefore(newChild, oldChild);
    // changed() already done.
    return removeChild(oldChild);
};
  

void THIS_CLASS::setReadOnly(bool readOnl, bool deep)
{
    NodeImpl::setReadOnly(readOnl, deep);
      
    if (deep)
        // Recursively set kids
        for (ChildNode *mykid = firstChild; 
             mykid != null; 
             mykid = mykid->nextSibling)
            if(! (mykid->isEntityReference()))
                mykid->setReadOnly(readOnl,true);
};
  
  
//Introduced in DOM Level 2
  
void THIS_CLASS::normalize()
{
    ChildNode *kid, *next;
    for (kid = firstChild; kid != null; kid = next)
    {
        next = kid->nextSibling;
        
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

