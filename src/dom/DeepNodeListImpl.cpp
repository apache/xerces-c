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
 * Revision 1.8  2001/10/25 21:47:14  peiyongz
 * Replace XMLDeleterFor with XMLRegisterCleanup
 *
 * Revision 1.7  2001/10/18 18:01:29  tng
 * [Bug 1699] Redirect "delete this" to a temp ptr to bypass AIX xlC v5 optimization memory leak problem.
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

#include "DeepNodeListImpl.hpp"
#include "NodeVector.hpp"
#include "NodeImpl.hpp"
#include "ElementImpl.hpp"
#include "DStringPool.hpp"
#include <limits.h>

static DOMString *kAstr = 0;
static XMLRegisterCleanup kAstrCleanup;

DeepNodeListImpl::DeepNodeListImpl(NodeImpl *rootNod, const DOMString &tagNam)
{
    changes = 0;
    this->rootNode = rootNod;
    this->tagName = tagNam;
    nodes=new NodeVector();
    matchAll = tagName.equals(DStringPool::getStaticString("*"
                                                         , &kAstr
                                                         , reinitDeepNodeListImpl
                                                         , kAstrCleanup));
    this->namespaceURI = null;	//DOM Level 2
    this->matchAllURI = false;	//DOM Level 2
    this->matchURIandTagname = false;	//DOM Level 2
};


//DOM Level 2
DeepNodeListImpl::DeepNodeListImpl(NodeImpl *rootNod,
    const DOMString &fNamespaceURI, const DOMString &localName)
{
    changes = 0;
    this->rootNode = rootNod;
    this->tagName = localName;
    nodes=new NodeVector();
    matchAll = tagName.equals(DStringPool::getStaticString("*"
                                                         , &kAstr
                                                         , reinitDeepNodeListImpl
                                                         , kAstrCleanup));

    this->namespaceURI = fNamespaceURI;
    this->matchAllURI = fNamespaceURI.equals(DStringPool::getStaticString("*"
                                                                        , &kAstr
                                                                        , reinitDeepNodeListImpl
                                                                        , kAstrCleanup));
                                                                        
    this->matchURIandTagname = true;
};


DeepNodeListImpl::~DeepNodeListImpl()
{
    delete nodes;
};


unsigned int DeepNodeListImpl::getLength()
{
    // Preload all matching elements. (Stops when we run out of subtree!)
    item(INT_MAX);
    return nodes->size();
};



// Start from the first child and count forward, 0-based. index>length-1
// should return null.
//
// Attempts to do only work actually requested, cache work already
// done, and to flush that cache when the tree has changed.
//
// LIMITATION: ????? Unable to tell relevant tree-changes from
// irrelevant ones.  Doing so in a really useful manner would seem
// to involve a tree-walk in its own right, or maintaining our data
// in a parallel tree.
NodeImpl *DeepNodeListImpl::item(unsigned int index)
{
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

        while(thisNode!=null && index >= nodes->size() && thisNode!=null)
        {
            thisNode=nextMatchingElementAfter(thisNode);
            if(thisNode!=null)
                nodes->addElement(thisNode);
        }
        return thisNode;           // Either what we want, or null (not avail.)
    }
};



/* Iterative tree-walker. When you have a Parent link, there's often no
need to resort to recursion. NOTE THAT only Element nodes are matched
since we're specifically supporting getElementsByTagName().
*/

NodeImpl *DeepNodeListImpl::nextMatchingElementAfter(NodeImpl *current)
{
    NodeImpl *next;
    while (current != null)
    {
        // Look down to first child.
        if (current->hasChildNodes())
        {
            current = current->getFirstChild();
        }
        // Look right to sibling (but not from root!)
        else
        {
            if (current != rootNode && null != (next = current->getNextSibling()))
            {
                current = next;
            }
            // Look up and right (but not past root!)
            else
            {
                next = null;
                for (; current != rootNode; // Stop when we return to starting point
                current = current->getParentNode())
                {
                    next = current->getNextSibling();
                    if (next != null)
                        break;
                }
                current = next;
            }
        }

        // Have we found an Element with the right tagName?
        // ("*" matches anything.)
        if (current != null && current != rootNode && current->isElementImpl()) {
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
    return null;
};


//
//  unreferenced()      The RefCountedImpl base class calls back to this function
//                      when the ref count goes to zero.
//
//
void DeepNodeListImpl::unreferenced()
{
//    delete this;
      DeepNodeListImpl* ptr = this;
      delete ptr;
};

// -----------------------------------------------------------------------
//  Notification that lazy data has been deleted
// -----------------------------------------------------------------------
void DeepNodeListImpl::reinitDeepNodeListImpl() {
	delete kAstr;
	kAstr = 0;
}