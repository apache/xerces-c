/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 2001-2002 The Apache Software Foundation.  All rights
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
 * Revision 1.1  2002/05/21 20:26:44  tng
 * DOM Reorganization: move IDOM from src/xercesc/idom to src/xercesc/dom and src/xercesc/dom/impl.  And rename IDOM_XXXX to DOMXXX.
 *
 * Revision 1.2  2002/02/04 21:20:59  tng
 * Add DOM 2 Level missing functions:
 *   1. NodeIterator::getRoot
 *   2. TreeWalker::getRoot
 *   3. Element::hasAttribute
 *   4. Element::hasAttributeNS
 *   5. Node::hasAttributes
 *   6. Node::isSupported
 *
 * Revision 1.1.1.1  2002/02/01 22:21:56  peiyongz
 * sane_include
 *
 * Revision 1.3  2001/06/04 20:11:53  tng
 * IDOM: Complete IDNodeIterator, IDTreeWalker, IDNodeFilter.
 *
 * Revision 1.2  2001/05/11 13:25:56  tng
 * Copyright update.
 *
 * Revision 1.1.1.1  2001/04/03 00:14:33  andyh
 * IDOM
 *
 */

#ifndef IDOM_TreeWalker_HEADER_GUARD_
#define IDOM_TreeWalker_HEADER_GUARD_

#include "IDOM_Node.hpp"
#include "IDOM_NodeFilter.hpp"


/**
 * <code>IDOM_TreeWalker</code> objects are used to navigate a document tree or
 * subtree using the view of the document defined by its <code>whatToShow</code>
 * flags and any filters that are defined for the <code>IDOM_TreeWalker</code>. Any
 * function which performs navigation using a <code>IDOM_TreeWalker</code> will
 * automatically support any view defined by a <code>IDOM_TreeWalker</code>.
 *
 * Omitting nodes from the logical view of a subtree can result in a structure that is
 * substantially different from the same subtree in the complete, unfiltered document. Nodes
 * that are siblings in the IDOM_TreeWalker view may be children of different, widely separated
 * nodes in the original view. For instance, consider a Filter that skips all nodes except for
 * Text nodes and the root node of a document. In the logical view that results, all text
 * nodes will be siblings and appear as direct children of the root node, no matter how
 * deeply nested the structure of the original document.
 *
 */
class CDOM_EXPORT IDOM_TreeWalker {
    protected:
        IDOM_TreeWalker() {};
        IDOM_TreeWalker(const IDOM_TreeWalker &other) {};
        IDOM_TreeWalker & operator = (const IDOM_TreeWalker &other) {return *this;};

    public:
        /** @name Get functions. */
        //@{
        /**
         * The <code>root</code> node of the <code>TreeWalker</code>, as specified
         * when it was created.
         */
        virtual IDOM_Node*          getRoot() = 0;
        /**
          * Return which node types are presented via the IDOM_TreeWalker.
          * These constants are defined in the IDOM_NodeFilter interface.
          *
          */
        virtual unsigned long   	getWhatToShow()= 0;

        /**
          * Return The filter used to screen nodes.
          *
          */
        virtual IDOM_NodeFilter*		getFilter()= 0;

        /**
          * Return the expandEntityReferences flag.
          * The value of this flag determines whether the children of entity reference
          * nodes are visible to the IDOM_TreeWalker. If false, they will be skipped over.
          *
          */
        virtual bool getExpandEntityReferences()= 0;

        /**
          * Return the node at which the IDOM_TreeWalker is currently positioned.
          *
          */
        virtual IDOM_Node*		getCurrentNode()= 0;

        /**
          * Moves to and returns the closest visible ancestor node of the current node.
          * If the search for parentNode attempts to step upward from the IDOM_TreeWalker's root
          * node, or if it fails to find a visible ancestor node, this method retains the
          * current position and returns null.
          *
          */
        virtual IDOM_Node*		parentNode()= 0;

        /**
          * Moves the <code>IDOM_TreeWalker</code> to the first child of the current node,
          * and returns the new node. If the current node has no children, returns
          * <code>null</code>, and retains the current node.
          *
          */
        virtual IDOM_Node*		firstChild()= 0;

        /**
          * Moves the <code>IDOM_TreeWalker</code> to the last child of the current node, and
          * returns the new node. If the current node has no children, returns
          * <code>null</code>, and retains the current node.
          *
          */
        virtual IDOM_Node*		lastChild()= 0;

        /**
          * Moves the <code>IDOM_TreeWalker</code> to the previous sibling of the current
          * node, and returns the new node. If the current node has no previous sibling,
          * returns <code>null</code>, and retains the current node.
          *
          */
        virtual IDOM_Node*		previousSibling()= 0;

        /**
          * Moves the <code>IDOM_TreeWalker</code> to the next sibling of the current node,
          * and returns the new node. If the current node has no next sibling, returns
          * <code>null</code>, and retains the current node.
          *
          */
        virtual IDOM_Node*		nextSibling()= 0;

        /**
          * Moves the <code>IDOM_TreeWalker</code> to the previous visible node in document
          * order relative to the current node, and returns the new node. If the current
          * node has no previous node,
          * or if the search for previousNode attempts to step upward from the IDOM_TreeWalker's
          * root node, returns <code>null</code>, and retains the current node.
          *
          */
        virtual IDOM_Node*		previousNode()= 0;

        /**
          * Moves the <code>IDOM_TreeWalker</code> to the next visible node in document order
          * relative to the current node, and returns the new node. If the current node has
          * no next node,
          * or if the search for nextNode attempts to step upward from the IDOM_TreeWalker's
          * root node, returns <code>null</code>, and retains the current node.
          *
          */
        virtual IDOM_Node*		nextNode()= 0;
        //@}

        /** @name Set functions. */
        //@{
        /**
          * Set the node at which the IDOM_TreeWalker is currently positioned.
          *
          */
        virtual void			setCurrentNode(IDOM_Node* currentNode)= 0;
        //@}
};

#endif
