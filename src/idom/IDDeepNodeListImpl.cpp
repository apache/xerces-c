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
 * $Log$
 * Revision 1.1  2001/04/03 00:14:20  andyh
 * Initial revision
 *
 * Revision 1.6  2000/04/27 02:52:42  lehors
 * global reorganization similar to what I've done in Java,
 * nodes now are much smaller.
 * The main changes are:
 * renamed NodeContainer to ParentNode,
 * introduced ChildNode and ChildAndParentNode,
 * all the boolean attributes have been changed to bit flags,
 * ownerDocument is no longer an attribute of NodeImpl, only Parent nodes have
 * it, leave nodes rely on their parent to get it, or get it from ownerNode when
 * they do not have a parent,
 * parent Nodes no longer have a direct pointer to the last child
 * instead the last child is stored as the previous sibling of
 * the first child.
 * I also added support for importing a DocumentType as it's done in Java,
 * and got the importNode mechanism back in sync with Java as well.
 *
 * Here are the most significant changes in size:
 * ElementImpl 52 -> 48
 * TextImpl    44 -> 32
 * AttrImpl    52 -> 36
 *
 * Revision 1.5  2000/03/02 19:53:59  roddey
 * This checkin includes many changes done while waiting for the
 * 1.1.0 code to be finished. I can't list them all here, but a list is
 * available elsewhere.
 *
 * Revision 1.4  2000/02/06 07:47:31  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.3  2000/02/04 01:49:30  aruna1
 * TreeWalker and NodeIterator changes
 *
 * Revision 1.2  2000/01/22 01:38:29  andyh
 * Remove compiler warnings in DOM impl classes
 *
 * Revision 1.1.1.1  1999/11/09 01:08:42  twl
 * Initial checkin
 *
 * Revision 1.3  1999/11/08 20:44:23  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */

#include "IDDeepNodeListImpl.hpp"
#include "IDNodeImpl.hpp"
#include "IDElementImpl.hpp"
#include <limits.h>

static const XMLCh *kAstr;

IDDeepNodeListImpl::IDDeepNodeListImpl(const IDOM_Node *rootNod, const XMLCh *tagNam)
{
#ifdef idom_revisit
    changes = 0;
    this->rootNode = rootNod;
    this->tagName = tagNam;
    nodes=new (castToNodeImpl(rootNod)->getOwnerDocument()) NodeVector();
    matchAll = tagName.equals(DStringPool::getStaticString("*", &kAstr));
    this->namespaceURI = 0;	//DOM Level 2
    this->matchAllURI = false;	//DOM Level 2
    this->matchURIandTagname = false;	//DOM Level 2
#endif
};


//DOM Level 2
IDDeepNodeListImpl::IDDeepNodeListImpl(const IDOM_Node *rootNod,
    const XMLCh *fNamespaceURI, const XMLCh *localName)
{
#ifdef idom_revisit

    changes = 0;
    this->rootNode = rootNod;
    this->tagName = localName;
    nodes=new (castToNodeImpl(rootNod)->getOwnerDocument()) NodeVector();
    matchAll = tagName.equals(DStringPool::getStaticString("*", &kAstr));
    this->namespaceURI = fNamespaceURI;
    this->matchAllURI = fNamespaceURI.equals(DStringPool::getStaticString("*", &kAstr));
    this->matchURIandTagname = true;
#endif
};


IDDeepNodeListImpl::~IDDeepNodeListImpl()
{
#ifdef idom_revisit
    delete nodes;
#endif
};


IDOM_NodeList *IDDeepNodeListImpl::getDeepNodeList(const IDOM_Node *rootNode, const XMLCh *tagName)
{
    // idom_revisit
    return 0;
};


IDOM_NodeList *IDDeepNodeListImpl::getDeepNodeList(const IDOM_Node *rootNode,	//DOM Level 2
			                            const XMLCh *namespaceURI,
                                        const XMLCh *localName)
{
    // idom_revisit
    return 0;
}


unsigned int IDDeepNodeListImpl::getLength() const
{
#ifdef idom_revisit
    // Preload all matching elements. (Stops when we run out of subtree!)
    item(INT_MAX);
    return nodes->size();
#endif
    return 0;
};



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
IDOM_Node *IDDeepNodeListImpl::item(unsigned int index) const
{
#ifdef idom_revisit
    NodeImpl *thisNode;
    
    if(rootNode->changes() != changes)
    {
        nodes->reset();     // Tree changed. Do it all from scratch!
        changes = rootNode->changes();
    }
    
    if(index< nodes->size())      // In the cache
        return nodes->elementAt((int) index);
    else                        // Not yet seen
    {
        if(nodes->size()==0)     // Pick up where we left off
            thisNode=rootNode; // (Which may be the beginning)
        else
            thisNode=nodes->lastElement();

        while(thisNode!=0 && index >= nodes->size() && thisNode!=0)
        {
            thisNode=nextMatchingElementAfter(thisNode);
            if(thisNode!=0)
                nodes->addElement(thisNode);
        }
        return thisNode;           // Either what we want, or 0 (not avail.)
    }
#endif
    return 0;
};



/* Iterative tree-walker. When you have a Parent link, there's often no
need to resort to recursion. NOTE THAT only Element nodes are matched
since we're specifically supporting getElementsByTagName().
*/
#ifdef idom_revisit

IDOM_Node *IDDeepNodeListImpl::nextMatchingElementAfter(IDOM_Node *current)
{
    NodeImpl *next;
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
            if (current != rootNode && 0 != (next = current->getNextSibling()))
            {
                current = next;
            }
            // Look up and right (but not past root!)
            else
            {
                next = 0;
                for (; current != rootNode; // Stop when we return to starting point
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
        if (current != 0 && current != rootNode && current->isElementImpl()) {
	    if (!matchURIandTagname) {	//DOM Level 1
		if (matchAll || ((ElementImpl *)current)->getTagName().equals(tagName))
		    return current;
	    } else {	//DOM Level 2
		if (!matchAllURI && !(current -> getNamespaceURI().equals(namespaceURI)))
		    continue;
		if (matchAll || current -> getLocalName().equals(tagName))
		    return current;
	    }
	}
        
        // Otherwise continue walking the tree
    }
    // Fell out of tree-walk; no more instances found
    return 0;
};
#endif

