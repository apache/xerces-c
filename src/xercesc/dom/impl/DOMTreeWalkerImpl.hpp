#ifndef DOMTreeWalkerImpl_HEADER_GUARD_
#define DOMTreeWalkerImpl_HEADER_GUARD_

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

//
//  This file is part of the internal implementation of the C++ XML DOM.
//  It should NOT be included or used directly by application programs.
//
//  Applications should include the file <xercesc/dom/DOM.hpp> for the entire
//  DOM API, or xercesc/dom/DOM*.hpp for individual DOM classes, where the class
//  name is substituded for the *.
//

#include <xercesc/dom/DOMTreeWalker.hpp>

XERCES_CPP_NAMESPACE_BEGIN


class CDOM_EXPORT DOMTreeWalkerImpl : public DOMTreeWalker {
    private:
        // The whatToShow mask.
        unsigned long fWhatToShow;

        // The NodeFilter reference.
        DOMNodeFilter* fNodeFilter;

        // The current Node.
        DOMNode* fCurrentNode;

        // The root Node.
        DOMNode* fRoot;

        // The expandEntity reference flag.
        bool fExpandEntityReferences;

	public:
    // Implementation Note: No state is kept except the data above
    // (fWhatToShow, fNodeFilter, fCurrentNode, fRoot) such that
    // setters could be created for these data values and the
    // implementation will still work.

    /** Public constructor */
    DOMTreeWalkerImpl (
        DOMNode* root,
        unsigned long whatToShow,
        DOMNodeFilter* nodeFilter,
        bool expandEntityRef);
    DOMTreeWalkerImpl (const DOMTreeWalkerImpl& twi);
    DOMTreeWalkerImpl& operator= (const DOMTreeWalkerImpl& twi);

    // Return the root node.
    virtual DOMNode* getRoot ();

    // Return the whatToShow value.
    virtual unsigned long  getWhatToShow ();

    // Return the NodeFilter.
    virtual DOMNodeFilter* getFilter ();

	
    // Return the current DOMNode.
    virtual DOMNode* getCurrentNode ();

    // Return the current Node.
    virtual void setCurrentNode (DOMNode* node);

    // Return the parent Node from the current node,
    //  after applying filter, whatToshow.
    //  If result is not null, set the current Node.
    virtual DOMNode* parentNode ();

    // Return the first child Node from the current node,
    //  after applying filter, whatToshow.
    //  If result is not null, set the current Node.
    virtual DOMNode* firstChild ();

    // Return the last child Node from the current node,
    //  after applying filter, whatToshow.
    //  If result is not null, set the current Node.
    virtual DOMNode* lastChild ();

    // Return the previous sibling Node from the current node,
    //  after applying filter, whatToshow.
    //  If result is not null, set the current Node.
    virtual DOMNode* previousSibling ();

    // Return the next sibling Node from the current node,
    //  after applying filter, whatToshow.
    //  If result is not null, set the current Node.

    virtual DOMNode* nextSibling ();
    // Return the previous Node from the current node,
    //  after applying filter, whatToshow.
    //  If result is not null, set the current Node.
    virtual DOMNode* previousNode ();

    // Return the next Node from the current node,
    //  after applying filter, whatToshow.
    //  If result is not null, set the current Node.
    virtual DOMNode* nextNode ();

    // Get the expandEntity reference flag.
    virtual bool getExpandEntityReferences();

    // release the resource
    virtual void release();

protected:

    // Internal function.
    //  Return the parent Node, from the input node
    //  after applying filter, whatToshow.
    //  The current node is not consulted or set.
    DOMNode* getParentNode (DOMNode* node);

    // Internal function.
    //  Return the nextSibling Node, from the input node
    //  after applying filter, whatToshow.
    //  The current node is not consulted or set.
    DOMNode* getNextSibling (DOMNode* node);

    // Internal function.
    //  Return the previous sibling Node, from the input node
    //  after applying filter, whatToshow.
    //  The current node is not consulted or set.
    DOMNode* getPreviousSibling (DOMNode* node);

    // Internal function.
    //  Return the first child Node, from the input node
    //  after applying filter, whatToshow.
    //  The current node is not consulted or set.
    DOMNode* getFirstChild (DOMNode* node);

    // Internal function.
    //  Return the last child Node, from the input node
    //  after applying filter, whatToshow.
    //  The current node is not consulted or set.
    DOMNode* getLastChild (DOMNode* node);

    // The node is accepted if it passes the whatToShow and the filter.
    short acceptNode (DOMNode* node);

    		
};

XERCES_CPP_NAMESPACE_END

#endif
