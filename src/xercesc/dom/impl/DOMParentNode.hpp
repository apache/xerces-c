#ifndef DOMParentNode_HEADER_GUARD_
#define DOMParentNode_HEADER_GUARD_


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

/**
 * ParentNode provides the capability of having child
 * nodes. Not every node in the DOM can have children, so only nodes that can
 * should include this class and pay the price for it.
 * <P>
 * While we have a direct reference to the first child, the last child is
 * stored as the previous sibling of the first child. First child nodes are
 * marked as being so, and getNextSibling hides this fact.
 *
 **/

#include <xercesc/util/XercesDefs.hpp>
#include "DOMNodeListImpl.hpp"

XERCES_CPP_NAMESPACE_BEGIN


class DOMChildNode;
class DOMDocument;
class DOMNode;
class DOMNodeList;

class CDOM_EXPORT DOMParentNode  {
public:
    DOMDocument            *fOwnerDocument; // Document this node belongs to
    DOMNode                *fFirstChild;
    DOMNodeListImpl            fChildNodeList;      // for GetChildNodes()

public:
    DOMParentNode(DOMDocument *ownerDocument);
    DOMParentNode(const DOMParentNode &other);

    DOMDocument * getOwnerDocument() const;
    void setOwnerDocument(DOMDocument* doc);

    // Track changes to the node tree structure under this node.  An optimization
    //   for NodeLists.
    int changes() const;
    void changed();

    DOMNode*     appendChild(DOMNode *newChild);
    DOMNodeList* getChildNodes() const;
    DOMNode*     getFirstChild() const;
    DOMNode*     getLastChild() const;
    XMLSize_t    getLength() const;
    bool         hasChildNodes() const;
    DOMNode*     insertBefore(DOMNode *newChild, DOMNode *refChild);
    DOMNode*     item(XMLSize_t index) const;
    DOMNode*     removeChild(DOMNode *oldChild);
    DOMNode*     replaceChild(DOMNode *newChild, DOMNode *oldChild);

    //Introduced in DOM Level 2
    void	normalize();

    //Introduced in DOM Level 3
    bool isEqualNode(const DOMNode* arg) const;

    // NON-DOM
    // unlike getOwnerDocument this never returns null, even for Document nodes
    DOMDocument * getDocument() const;
    void          release();


public:
    void cloneChildren(const DOMNode *other);
    DOMNode * lastChild() const;
    void lastChild(DOMNode *);

private:
    // unimplemented    
    DOMParentNode& operator= (const DOMParentNode& other);
};

XERCES_CPP_NAMESPACE_END

#endif
