#ifndef DOMTreeWalker_HEADER_GUARD_
#define DOMTreeWalker_HEADER_GUARD_

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
 * $Id$
 */

#include "DOMNode.hpp"
#include "DOMNodeFilter.hpp"


/**
 * <code>DOMTreeWalker</code> objects are used to navigate a document tree or
 * subtree using the view of the document defined by its <code>whatToShow</code>
 * flags and any filters that are defined for the <code>DOMTreeWalker</code>. Any
 * function which performs navigation using a <code>DOMTreeWalker</code> will
 * automatically support any view defined by a <code>DOMTreeWalker</code>.
 *
 * Omitting nodes from the logical view of a subtree can result in a structure that is
 * substantially different from the same subtree in the complete, unfiltered document. Nodes
 * that are siblings in the DOMTreeWalker view may be children of different, widely separated
 * nodes in the original view. For instance, consider a Filter that skips all nodes except for
 * Text nodes and the root node of a document. In the logical view that results, all text
 * nodes will be siblings and appear as direct children of the root node, no matter how
 * deeply nested the structure of the original document.
 *
 */
class CDOM_EXPORT DOMTreeWalker {
    protected:
        DOMTreeWalker() {};
        DOMTreeWalker(const DOMTreeWalker &other) {};
        DOMTreeWalker & operator = (const DOMTreeWalker &other) {return *this;};

    public:
        /** @name Get functions. */
        //@{
        /**
         * The <code>root</code> node of the <code>TreeWalker</code>, as specified
         * when it was created.
         */
        virtual DOMNode*          getRoot() = 0;
        /**
          * Return which node types are presented via the DOMTreeWalker.
          * These constants are defined in the DOMNodeFilter interface.
          *
          */
        virtual unsigned long   	getWhatToShow()= 0;

        /**
          * Return The filter used to screen nodes.
          *
          */
        virtual DOMNodeFilter*		getFilter()= 0;

        /**
          * Return the expandEntityReferences flag.
          * The value of this flag determines whether the children of entity reference
          * nodes are visible to the DOMTreeWalker. If false, they will be skipped over.
          *
          */
        virtual bool getExpandEntityReferences()= 0;

        /**
          * Return the node at which the DOMTreeWalker is currently positioned.
          *
          */
        virtual DOMNode*		getCurrentNode()= 0;

        /**
          * Moves to and returns the closest visible ancestor node of the current node.
          * If the search for parentNode attempts to step upward from the DOMTreeWalker's root
          * node, or if it fails to find a visible ancestor node, this method retains the
          * current position and returns null.
          *
          */
        virtual DOMNode*		parentNode()= 0;

        /**
          * Moves the <code>DOMTreeWalker</code> to the first child of the current node,
          * and returns the new node. If the current node has no children, returns
          * <code>null</code>, and retains the current node.
          *
          */
        virtual DOMNode*		firstChild()= 0;

        /**
          * Moves the <code>DOMTreeWalker</code> to the last child of the current node, and
          * returns the new node. If the current node has no children, returns
          * <code>null</code>, and retains the current node.
          *
          */
        virtual DOMNode*		lastChild()= 0;

        /**
          * Moves the <code>DOMTreeWalker</code> to the previous sibling of the current
          * node, and returns the new node. If the current node has no previous sibling,
          * returns <code>null</code>, and retains the current node.
          *
          */
        virtual DOMNode*		previousSibling()= 0;

        /**
          * Moves the <code>DOMTreeWalker</code> to the next sibling of the current node,
          * and returns the new node. If the current node has no next sibling, returns
          * <code>null</code>, and retains the current node.
          *
          */
        virtual DOMNode*		nextSibling()= 0;

        /**
          * Moves the <code>DOMTreeWalker</code> to the previous visible node in document
          * order relative to the current node, and returns the new node. If the current
          * node has no previous node,
          * or if the search for previousNode attempts to step upward from the DOMTreeWalker's
          * root node, returns <code>null</code>, and retains the current node.
          *
          */
        virtual DOMNode*		previousNode()= 0;

        /**
          * Moves the <code>DOMTreeWalker</code> to the next visible node in document order
          * relative to the current node, and returns the new node. If the current node has
          * no next node,
          * or if the search for nextNode attempts to step upward from the DOMTreeWalker's
          * root node, returns <code>null</code>, and retains the current node.
          *
          */
        virtual DOMNode*		nextNode()= 0;
        //@}

        /** @name Set functions. */
        //@{
        /**
          * Set the node at which the DOMTreeWalker is currently positioned.
          *
          */
        virtual void			setCurrentNode(DOMNode* currentNode)= 0;
        //@}
};

#endif
