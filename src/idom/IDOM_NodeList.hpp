#ifndef IDOM_NodeList_HEADER_GUARD_
#define IDOM_NodeList_HEADER_GUARD_

/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 2001 The Apache Software Foundation.  All rights
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
 * Revision 1.2  2001/05/11 13:25:54  tng
 * Copyright update.
 *
 * Revision 1.1.1.1  2001/04/03 00:14:31  andyh
 * IDOM
 *
 */

#include <util/XercesDefs.hpp>

class IDOM_Node;


/**
 * The <code>NodeList</code> interface provides the abstraction of an ordered
 * collection of nodes.  NodeLists are created by DOM_Document::getElementsByTagName(),
 * DOM_Node::getChildNodes(),
 *
 * <p>The items in the <code>NodeList</code> are accessible via an integral
 * index, starting from 0.
 *
 * NodeLists are "live", in that any changes to the document tree are immediately
 * reflected in any NodeLists that may have been created for that tree.
 */

class  CDOM_EXPORT IDOM_NodeList {
protected:
    IDOM_NodeList() {};
    IDOM_NodeList(const IDOM_NodeList &other) {};
    IDOM_NodeList & operator = (const IDOM_NodeList &other) {return *this;};

public:
    /** @name Destructor. */
    //@{
    virtual ~IDOM_NodeList()  {};
    //@}



    /** @name Get functions. */
    //@{
    /**
     * Returns the <code>index</code>th item in the collection.
     *
     * If <code>index</code> is greater than or equal to the number of nodes in
     * the list, this returns <code>null</code>.
     *
     * @param index Index into the collection.
     * @return The node at the <code>index</code>th position in the
     *   <code>NodeList</code>, or <code>null</code> if that is not a valid
     *   index.
     */
    virtual IDOM_Node  *item(unsigned int index) const = 0;

    /**
     * Returns the number of nodes in the list.
     *
     * The range of valid child node indices is 0 to <code>length-1</code> inclusive.
     */
    virtual unsigned int getLength() const = 0;
    //@}
};

#endif


