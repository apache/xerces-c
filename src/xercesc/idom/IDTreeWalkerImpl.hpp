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
 * Revision 1.2  2002/02/04 21:20:59  tng
 * Add DOM 2 Level missing functions:
 *   1. NodeIterator::getRoot
 *   2. TreeWalker::getRoot
 *   3. Element::hasAttribute
 *   4. Element::hasAttributeNS
 *   5. Node::hasAttributes
 *   6. Node::isSupported
 *
 * Revision 1.1.1.1  2002/02/01 22:21:57  peiyongz
 * sane_include
 *
 * Revision 1.3  2001/06/04 20:11:53  tng
 * IDOM: Complete IDNodeIterator, IDTreeWalker, IDNodeFilter.
 *
 * Revision 1.2  2001/05/11 13:26:00  tng
 * Copyright update.
 *
 * Revision 1.1.1.1  2001/04/03 00:14:35  andyh
 * IDOM
 *
 */

#ifndef IDTreeWalkerImpl_HEADER_GUARD_
#define IDTreeWalkerImpl_HEADER_GUARD_


#include "IDOM_TreeWalker.hpp"


class CDOM_EXPORT IDTreeWalkerImpl : public IDOM_TreeWalker {
    private:
        // The whatToShow mask.
        unsigned long fWhatToShow;

        // The NodeFilter reference.
        IDOM_NodeFilter* fNodeFilter;

        // The current Node.
        IDOM_Node* fCurrentNode;

        // The root Node.
        IDOM_Node* fRoot;

        // The expandEntity reference flag.
        bool fExpandEntityReferences;

	public:
    // Implementation Note: No state is kept except the data above
    // (fWhatToShow, fNodeFilter, fCurrentNode, fRoot) such that
    // setters could be created for these data values and the
    // implementation will still work.

    /** Public constructor */
    IDTreeWalkerImpl (
        IDOM_Node* root,
        unsigned long whatToShow,
        IDOM_NodeFilter* nodeFilter,
        bool expandEntityRef);
    IDTreeWalkerImpl (const IDTreeWalkerImpl& twi);
    IDTreeWalkerImpl& operator= (const IDTreeWalkerImpl& twi);

    // Return the root node.
    virtual IDOM_Node* getRoot ();

    // Return the whatToShow value.
    virtual unsigned long  getWhatToShow ();

    // Return the NodeFilter.
    virtual IDOM_NodeFilter* getFilter ();

	
    // Return the current IDOM_Node.
    virtual IDOM_Node* getCurrentNode ();

    // Return the current Node.
    virtual void setCurrentNode (IDOM_Node* node);

    // Return the parent Node from the current node,
    //  after applying filter, whatToshow.
    //  If result is not null, set the current Node.
    virtual IDOM_Node* parentNode ();

    // Return the first child Node from the current node,
    //  after applying filter, whatToshow.
    //  If result is not null, set the current Node.
    virtual IDOM_Node* firstChild ();

    // Return the last child Node from the current node,
    //  after applying filter, whatToshow.
    //  If result is not null, set the current Node.
    virtual IDOM_Node* lastChild ();

    // Return the previous sibling Node from the current node,
    //  after applying filter, whatToshow.
    //  If result is not null, set the current Node.
    virtual IDOM_Node* previousSibling ();

    // Return the next sibling Node from the current node,
    //  after applying filter, whatToshow.
    //  If result is not null, set the current Node.

    virtual IDOM_Node* nextSibling ();
    // Return the previous Node from the current node,
    //  after applying filter, whatToshow.
    //  If result is not null, set the current Node.
    virtual IDOM_Node* previousNode ();

    // Return the next Node from the current node,
    //  after applying filter, whatToshow.
    //  If result is not null, set the current Node.
    virtual IDOM_Node* nextNode ();

    // Get the expandEntity reference flag.
    virtual bool getExpandEntityReferences();

protected:

    // Internal function.
    //  Return the parent Node, from the input node
    //  after applying filter, whatToshow.
    //  The current node is not consulted or set.
    IDOM_Node* getParentNode (IDOM_Node* node);

    // Internal function.
    //  Return the nextSibling Node, from the input node
    //  after applying filter, whatToshow.
    //  The current node is not consulted or set.
    IDOM_Node* getNextSibling (IDOM_Node* node);

    // Internal function.
    //  Return the previous sibling Node, from the input node
    //  after applying filter, whatToshow.
    //  The current node is not consulted or set.
    IDOM_Node* getPreviousSibling (IDOM_Node* node);

    // Internal function.
    //  Return the first child Node, from the input node
    //  after applying filter, whatToshow.
    //  The current node is not consulted or set.
    IDOM_Node* getFirstChild (IDOM_Node* node);

    // Internal function.
    //  Return the last child Node, from the input node
    //  after applying filter, whatToshow.
    //  The current node is not consulted or set.
    IDOM_Node* getLastChild (IDOM_Node* node);

    // The node is accepted if it passes the whatToShow and the filter.
    short acceptNode (IDOM_Node* node);

    		
};

#endif
