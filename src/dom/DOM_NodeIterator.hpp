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
 * Revision 1.4  2000/02/10 23:38:05  abagchi
 * Added API docs
 *
 * Revision 1.3  2000/02/06 07:47:30  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.2  2000/02/04 01:49:28  aruna1
 * TreeWalker and NodeIterator changes
 *
 * Revision 1.3  1999/11/08 20:44:20  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 * Revision 1.1.1.1  1999/11/09 01:09:01  twl
 * Initial checkin
 *
 */

#ifndef DOM_NodeIterator_HEADER_GUARD_
#define DOM_NodeIterator_HEADER_GUARD_

#include "DOM_NodeFilter.hpp"
#include "DOM_Node.hpp"

class NodeIteratorImpl;

/**
  *	NodeIterators are used to step through a set of nodes, e.g. the set of nodes in a
  * NodeList, the document subtree governed by a particular node, the results of a
  * query, or any other set of nodes. The set of nodes to be iterated is determined by
  * the implementation of the NodeIterator. DOM Level 2 specifies a single
  * NodeIterator implementation for document-order traversal of a document subtree.
  * Instances of these iterators are created by calling
  * <code>DocumentTraversal.createNodeIterator()</code>.
  */
class CDOM_EXPORT DOM_NodeIterator
{
	public:
    /** @name Constructors */
    //@{

	/**
	  * Default constructor
	  */
    DOM_NodeIterator ();

	/**
	  * Copy constructor
	  * @param other The object to be copied
	  */
    DOM_NodeIterator(const DOM_NodeIterator &other);
    //@}

    /** @name Assignment operators */
	/**
	  * Assignment operator
	  * @param other The object to be copied through assignment
	  */
    DOM_NodeIterator & operator = (const DOM_NodeIterator &other);

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
    DOM_NodeIterator & operator = (const DOM_NullPtr *val);
    //@}

    /** @name Destructor */
	/**
	  * Destructor
	  */
    //@{
    ~DOM_NodeIterator();
	//@}

    /** @name Equality operators */
    //@{
    /**
     *  Test whether this NodeIterator reference refers to the same underlying
     *  node iterator as the other reference object.  This does not
     *  compare the contents of two different objects.
     *
     *  @param other The value to be compared
     *  @return Returns true if the underlying node iterator is same
     */
    bool operator == (const DOM_NodeIterator & other)const;

    /**
     *  Use this comparison operator to test whether a Node Iterator reference
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
     *  Test whether this NodeIterator reference refers to the same underlying
     *  node iterator as the other reference object.  This does not
     *  compare the contents of two different objects.
     *
     *  @param other The value to be compared
     *  @return Returns true if the underlying node iterator is different
     */
    bool operator != (const DOM_NodeIterator & other) const;
    /**
     *  Use this comparison operator to test whether a Node Iterator reference
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
      *  iterator. The available set of constants is defined in the Filters
      *  interface.
	  */
    unsigned long       getWhatToShow();

    /**
      *  Get the <code>expandEntity</code> reference flag.
	  *  The value of this flag determines whether the children of entity
      *  reference nodes are visible to the iterator. If false, they will be skipped
      *  over.
	  *
      *  To produce a view of the document that has entity references
      *  expanded and does not expose the entity reference node itself, use
      *  the <code>whatToShow</code> flags to hide the entity reference node and set
      *  <code>expandEntityReferences</code> to true when creating the iterator. To
      *  produce a view of the document that has entity reference nodes but
      *  no entity expansion, use the <code>whatToShow</code> flags to show the entity
      *  reference node and set <code>expandEntityReferences</code> to false.
      */
    bool getExpandEntityReferences();

	/**
	  *	Get the filter used to screen nodes.
	  */
    DOM_NodeFilter*     getFilter();
	//@}

    /** @name Iterator Methods */
    //@{
	/**
	  *	 Returns the next node in the set and advances the position of the
      *  iterator in the set. After a NodeIterator is created, the first call to
      *  <code>nextNode()</code> returns the first node in the set.
	  *  @return Returns the next node in the set
	  */
    DOM_Node            nextNode();

    /**
	  *	 Returns the previous node in the set and moves the position of the
      *  iterator backwards in the set.
	  *  @return Returns the previous node in the set
	  */
    DOM_Node            previousNode();

    /**
	  *	 Detaches the iterator from the set which it iterated over, releasing any
      *  computational resources and placing the iterator in the INVALID state.
      *  After detach has been invoked, calls to nextNode or previousNode
      *  will raise the exception <code>INVALID_STATE_ERR</code>.
	  */
	void				detach();
	//@}

    private:
      NodeIteratorImpl*                 fImpl;

	protected:
      DOM_NodeIterator (NodeIteratorImpl* impl);
};

#endif
