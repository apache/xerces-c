#ifndef IDRangeImpl_HEADER_GUARD_
#define IDRangeImpl_HEADER_GUARD_
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
 * $Id$
 */

#include <util/XercesDefs.hpp>

#include "IDOM_Range.hpp"

class       IDOM_Node;
class       IDOM_DocumentFragment;
class       IDOM_Text;

class CDOM_EXPORT IDRangeImpl: public IDOM_Range {
private:
    enum TraversalType {
        EXTRACT_CONTENTS = 1,
        CLONE_CONTENTS   = 2,
        DELETE_CONTENTS  = 3
    };

    enum TraversePoint {
        BEFORE  = -1,
        START   = 0,
        AFTER   = 1
    };

    //private data

    IDOM_Node       *fStartContainer;
    unsigned int    fStartOffset;
    IDOM_Node       *fEndContainer;
    unsigned int    fEndOffset;
    bool            fCollapsed;
    IDOM_Document   *fDocument;
    bool            fDetached;

    IDOM_Node       *fRemoveChild;

public:
    //c'tor
    IDRangeImpl(IDOM_Document* doc);
    IDRangeImpl(const IDRangeImpl& other);

    //d'tor
    ~IDRangeImpl();

    //getter functions
    virtual IDOM_Node* getStartContainer() const;
    virtual unsigned int getStartOffset() const;
    virtual IDOM_Node* getEndContainer() const;
    virtual unsigned int getEndOffset() const;
    virtual bool getCollapsed() const;
    virtual const IDOM_Node* getCommonAncestorContainer() const;

    //setter functions
    virtual void setStart(const IDOM_Node *parent, unsigned int offset);
    virtual void setEnd(const IDOM_Node *parent, unsigned int offset);

    virtual void setStartBefore(const IDOM_Node *refNode);
    virtual void setStartAfter(const IDOM_Node *refNode);
    virtual void setEndBefore(const IDOM_Node *refNode);
    virtual void setEndAfter(const IDOM_Node *refNode);

    //misc functions
    virtual void collapse(bool toStart);
    virtual void selectNode(const IDOM_Node *node);
    virtual void selectNodeContents(const IDOM_Node *node);

    //Functions related to comparing range Boundrary-Points
    virtual short compareBoundaryPoints(CompareHow how, const IDOM_Range* range) const;
    virtual void deleteContents();
    virtual IDOM_DocumentFragment* extractContents();
    virtual IDOM_DocumentFragment* cloneContents() const;
    virtual void insertNode(IDOM_Node* node);

    //Misc functions
    virtual void surroundContents(IDOM_Node *node);
    virtual IDOM_Range* cloneRange() const;
    virtual const XMLCh* toString() const;
    virtual void detach();

    //getter functions
    IDOM_Document*         getDocument();

    // functions to inform all existing valid ranges about a change
    void updateSplitInfo(IDOM_Text* oldNode, IDOM_Text* startNode, unsigned int offset);
    void updateRangeForInsertedNode(IDOM_Node* node);
    void receiveReplacedText(IDOM_Node* node);
    void updateRangeForDeletedText(IDOM_Node* node, unsigned int offset, int count);
    void updateRangeForDeletedNode(IDOM_Node* node);

private:
    //setter functions
    void        setStartContainer(const IDOM_Node* node);
    void        setStartOffset(unsigned int offset) ;
    void        setEndContainer(const IDOM_Node* node);
    void        setEndOffset(unsigned int offset) ;

    //misc functions
    void        validateNode(const IDOM_Node* node) const;
    bool        isValidAncestorType(const IDOM_Node* node) const;
    bool        hasLegalRootContainer(const IDOM_Node* node) const;
    bool        isLegalContainedNode(const IDOM_Node* node ) const;
    void        checkIndex(const IDOM_Node* node, unsigned int offset) const;
    static bool isAncestorOf(const IDOM_Node* a, const IDOM_Node* b);

    unsigned short        indexOf(const IDOM_Node* child, const IDOM_Node* parent) const;

    const IDOM_Node*       commonAncestorOf(const IDOM_Node* pointA, const IDOM_Node* pointB) const;
    IDOM_Node*             nextNode(const IDOM_Node* node, bool visitChildren) const;
    IDOM_DocumentFragment* traverseContents(TraversalType type);
    void                  checkReadOnly(IDOM_Node* start, IDOM_Node* end,
                                  unsigned int starOffset, unsigned int endOffset);
    void                  recurseTreeAndCheck(IDOM_Node* start, IDOM_Node* end);
    IDOM_Node*             removeChild(IDOM_Node* parent, IDOM_Node* child);

    IDOM_DocumentFragment* traverseSameContainer( int how );
    IDOM_DocumentFragment* traverseCommonStartContainer( IDOM_Node *endAncestor, int how );
    IDOM_DocumentFragment* traverseCommonEndContainer( IDOM_Node *startAncestor, int how );
    IDOM_DocumentFragment* traverseCommonAncestors( IDOM_Node *startAncestor, IDOM_Node *endAncestor, int how );
    IDOM_Node*    traverseRightBoundary( IDOM_Node *root, int how );
    IDOM_Node*    traverseLeftBoundary( IDOM_Node *root, int how );
    IDOM_Node*    traverseNode( IDOM_Node *n, bool isFullySelected, bool isLeft, int how );
    IDOM_Node*    traverseFullySelected( IDOM_Node *n, int how );
    IDOM_Node*    traversePartiallySelected( IDOM_Node *n, int how );
    IDOM_Node*    traverseTextNode( IDOM_Node *n, bool isLeft, int how );
    IDOM_Node*    getSelectedNode( IDOM_Node *container, int offset );

};

#endif
