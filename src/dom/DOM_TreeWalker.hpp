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

/**
 * $Log$
 * Revision 1.5  2000/02/11 03:01:04  abagchi
 * Made protected method public to compile, must be fixed later
 *
 * Revision 1.4  2000/02/11 01:05:11  abagchi
 * Added API docs
 *
 * Revision 1.3  2000/02/06 07:47:31  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.2  2000/02/04 01:49:27  aruna1
 * TreeWalker and NodeIterator changes
 *
 * Revision 1.1.1.1  1999/11/09 01:09:05  twl
 * Initial checkin
 *
 * Revision 1.3  1999/11/08 20:44:22  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */

#ifndef DOM_TreeWalker_HEADER_GUARD_
#define DOM_TreeWalker_HEADER_GUARD_

#include "DOM_Node.hpp"
#include "DOM_NodeFilter.hpp"

class TreeWalkerImpl;

   /**
     * TreeWalker objects are used to navigate a document tree or subtree using the view of
     * the document defined by its <code>whatToShow</code> flags and any filters that are defined for the
     * TreeWalker. Any function which performs navigation using a TreeWalker will
     * automatically support any view defined by a TreeWalker.
     *
     * Omitting nodes from the logical view of a subtree can result in a structure that is
     * substantially different from the same subtree in the complete, unfiltered document. Nodes
     * that are siblings in the TreeWalker view may be children of different, widely separated
     * nodes in the original view. For instance, consider a Filter that skips all nodes except for
     * Text nodes and the root node of a document. In the logical view that results, all text
     * nodes will be siblings and appear as direct children of the root node, no matter how
     * deeply nested the structure of the original document.
     */

class CDOM_EXPORT DOM_TreeWalker {

    public:
			
        /** @name Constructors */
        //@{

        /**
          * Default constructor
          */
        DOM_TreeWalker ();

        /**
          * Copy constructor
          * @param other The object to be copied
          */
        DOM_TreeWalker(const DOM_TreeWalker &other);
        //@}

        /** @name Destructor */
        //@{
        /**
          * Destructor
          */
        ~DOM_TreeWalker();
        //@}

        /** @name Assignment operators */
        //@{
        /**
          * Assignment operator
          * @param other The object to be copied through assignment
          */
        DOM_TreeWalker & operator = (const DOM_TreeWalker &other);

        /**
          * Assignment operator
          *
          *   This overloaded variant is provided for
          *   the sole purpose of setting a DOM_Node reference variable to
          *   zero.  Nulling out a reference variable in this way will decrement
          *   the reference count on the underlying Node object that the variable
          *   formerly referenced.  This effect is normally obtained when reference
          *   variable goes out of scope, but zeroing them can be useful for
          *   global instances, or for local instances that will remain in scope
          *   for an extended time,  when the storage belonging to the underlying
          *   node needs to be reclaimed.
          * @param val Only a value of 0, or null, is allowed.
          */
        DOM_TreeWalker & operator = (const DOM_NullPtr *val);
        //@}

        /** @name Equality operators */
        //@{
        /**
         *  Test whether this TreeWalker reference refers to the same underlying
         *  tree walker as the other reference object.  This does not
         *  compare the contents of two different objects.
         *
         *  @param other The value to be compared
         *  @return Returns true if the underlying tree walker object is same
         */
        bool operator == (const DOM_TreeWalker & other)const;

        /**
         *  Use this comparison operator to test whether a tree walker reference
         *  is null.
         *
         *  @param other The value to be compared, which must be 0 or null.
         *  @return Returns true if the node iterator is null
         */
        bool operator == (const DOM_NullPtr *other) const;
        //@}

        /** @name Inequality operators */
        //@{

        /**
         *  Test whether this TreeWalker reference refers to the same underlying
         *  tree walker as the other reference object.  This does not
         *  compare the contents of two different objects.
         *
         *  @param other The value to be compared
         *  @return Returns true if the underlying tree walker is different
         */
        bool operator != (const DOM_TreeWalker & other) const;

        /**
         *  Use this comparison operator to test whether a tree walker reference
         *  is null.
         *
         *  @param other The value to be compared, which must be 0 or null.
         *  @return Returns true if the node iterator is NOT null
         */
        bool operator != (const DOM_NullPtr * other) const;
        //@}

        /** @name Get Functions */
        //@{
        /**
          *	 Returns the value of <code>whatToShow</code>
          *  This attribute determines which node types are presented via the
          *  <code>TreeWalker</code>. The available set of constants is defined in the Filters
          *  interface.
          */
        unsigned long   				getWhatToShow();
        /**
          *	Get the filter used to screen nodes.
          */
        DOM_NodeFilter*					getFilter();
        /**
          * Gets the node at which the tree walker is currently positioned.
          *
          * The value must not be null. Alterations to the DOM tree may cause the
          * current node to no longer be accepted by the TreeWalker's associated
          * filter. <code>currentNode</code> may also be explicitly set to any node, whether or not it
          * is within the subtree specified by the root node or would be accepted by
          * the filter and <code>whatToShow</code> flags. Further traversal occurs relative to
          * currentNode even if it is not part of the current view by applying the filters
          * in the requested direction (not changing <code>currentNode</code> where no traversal is
          * possible).
          */
        DOM_Node						getCurrentNode();
        //@}

        /** @name Set Functions */
        //@{
        /**
          *  Sets the current node of the DOM tree in tree walker
          *
          * To know more about the current node see <code>getCurrentNode</code> description.
          * @see #getCurrentNode
          */
        void							setCurrentNode(DOM_Node currentNode);
        //@}

        /** @name Iterator Methods */
        //@{

        /**
          * Moves to and returns the closest visible ancestor node of the current node.
          * If the search for parentNode attempts to step upward from the
          * TreeWalker's root node, or if it fails to find a visible ancestor node, this
          * method retains the current position and returns null.
          *
          * @return The new parent node, or null if the current node has no parent in
          *         the TreeWalker's logical view.
          */
        DOM_Node						parentNode();

        /**
          * Moves the TreeWalker to the first child of the current node, and returns the new node.
          * If the current node has no children, returns null, and retains
          * the current node
          * @return       The new node, or null if the current node has no children.
          */
        DOM_Node						firstChild();

        /**
          * Moves the TreeWalker to the last child of the current node, and returns the new node.
          * If the current node has no children, returns null, and retains
          * the current node.
          *
          * @return The new node, or null if the current node has no children.
          */
        DOM_Node						lastChild();

        /**
          * Moves the TreeWalker to the previous sibling of the current node, and returns the new node.
          * If the current node has no previous sibling, returns
          * <code>null</code>, and retains the current node.
          *
          * @return  The new node, or null if the current node has no previous sibling.
          */
        DOM_Node						previousSibling();

        /**
          * Moves the TreeWalker to the next sibling of the current node, and returns the new node.
          * If the current node has no next sibling, returns <code>null</code>, and
          * retains the current node.
          *
          * @return The new node, or null if the current node has no next sibling.
          */
        DOM_Node						nextSibling();

        /**
          * Moves the TreeWalker to the previous visible node in document order relative to the current node, and returns the new node.
          * If the current node
          * has no previous node, or if the search for previousNode attempts to step
          * upward from the TreeWalker's root node, returns <code>null</code>, and retains the
          * current node.
          *
          * @return  The new node, or null if the current <code>node</code> has no previous node.
          */
        DOM_Node						previousNode();

        /**
          * Moves the TreeWalker to the next visible node in document order relative to the current node, and returns the new node.
          * If the current node has no
          * next node, or if the search for nextNode attempts to step upward from the
          * TreeWalker's root node, returns null, and retains the current node.
          * @return The new node, or <code>null</code> if the current node has no next node.
          */
        DOM_Node						nextNode();

        /**
          *	 Detaches the iterator from the set which it iterated over, releasing any computational resources and placing the iterator in the INVALID state.
          *  After detach has been invoked, calls to <code>nextNode</code> or <code>previousNode</code>
          *  will raise the exception <code>INVALID_STATE_ERR</code>.
          */
        void							detach();
        //@}

        DOM_TreeWalker (TreeWalkerImpl* impl);
    private:
        TreeWalkerImpl*                 fImpl;

};


#endif
