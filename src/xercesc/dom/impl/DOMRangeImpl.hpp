#ifndef DOMRangeImpl_HEADER_GUARD_
#define DOMRangeImpl_HEADER_GUARD_

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

#include <xercesc/util/XercesDefs.hpp>
#include <xercesc/dom/DOMRange.hpp>
#include <xercesc/util/PlatformUtils.hpp>

XERCES_CPP_NAMESPACE_BEGIN



class       DOMNode;
class       DOMDocumentFragment;
class       DOMDocument;
class       DOMText;
class       MemoryManager;

class CDOM_EXPORT DOMRangeImpl: public DOMRange {
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

    DOMNode*     fStartContainer;
    XMLSize_t    fStartOffset;
    DOMNode*     fEndContainer;
    XMLSize_t    fEndOffset;
    bool         fCollapsed;
    DOMDocument* fDocument;
    bool         fDetached;

    DOMNode*     fRemoveChild;
    MemoryManager* fMemoryManager;

public:
    //c'tor
    DOMRangeImpl(DOMDocument* doc, MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);
    DOMRangeImpl(const DOMRangeImpl& other);

    //d'tor
    ~DOMRangeImpl();

    //getter functions
    virtual DOMNode* getStartContainer() const;
    virtual XMLSize_t getStartOffset() const;
    virtual DOMNode* getEndContainer() const;
    virtual XMLSize_t getEndOffset() const;
    virtual bool getCollapsed() const;
    virtual const DOMNode* getCommonAncestorContainer() const;

    //setter functions
    virtual void setStart(const DOMNode *parent, XMLSize_t offset);
    virtual void setEnd(const DOMNode *parent, XMLSize_t offset);

    virtual void setStartBefore(const DOMNode *refNode);
    virtual void setStartAfter(const DOMNode *refNode);
    virtual void setEndBefore(const DOMNode *refNode);
    virtual void setEndAfter(const DOMNode *refNode);

    //misc functions
    virtual void collapse(bool toStart);
    virtual void selectNode(const DOMNode *node);
    virtual void selectNodeContents(const DOMNode *node);

    //Functions related to comparing range Boundrary-Points
    virtual short compareBoundaryPoints(CompareHow how, const DOMRange* range) const;
    virtual void deleteContents();
    virtual DOMDocumentFragment* extractContents();
    virtual DOMDocumentFragment* cloneContents() const;
    virtual void insertNode(DOMNode* node);

    //Misc functions
    virtual void surroundContents(DOMNode *node);
    virtual DOMRange* cloneRange() const;
    virtual const XMLCh* toString() const;
    virtual void detach();
    virtual void release();

    //getter functions
    DOMDocument*         getDocument();

    // functions to inform all existing valid ranges about a change
    void updateSplitInfo(DOMNode* oldNode, DOMNode* startNode, XMLSize_t offset);
    void updateRangeForInsertedNode(DOMNode* node);
    void receiveReplacedText(DOMNode* node);
    void updateRangeForDeletedText(DOMNode* node, XMLSize_t offset, int count);
    void updateRangeForInsertedText(DOMNode* node, XMLSize_t offset, int count);
    void updateRangeForDeletedNode(DOMNode* node);

private:
    //setter functions
    void        setStartContainer(const DOMNode* node);
    void        setStartOffset(XMLSize_t offset) ;
    void        setEndContainer(const DOMNode* node);
    void        setEndOffset(XMLSize_t offset) ;

    //misc functions
    void        validateNode(const DOMNode* node) const;
    bool        isValidAncestorType(const DOMNode* node) const;
    bool        hasLegalRootContainer(const DOMNode* node) const;
    bool        isLegalContainedNode(const DOMNode* node ) const;
    void        checkIndex(const DOMNode* node, XMLSize_t offset) const;
    static bool isAncestorOf(const DOMNode* a, const DOMNode* b);

    XMLSize_t   indexOf(const DOMNode* child, const DOMNode* parent) const;

    const DOMNode*       commonAncestorOf(const DOMNode* pointA, const DOMNode* pointB) const;
    DOMNode*             nextNode(const DOMNode* node, bool visitChildren) const;
    DOMDocumentFragment* traverseContents(TraversalType type);
    void                  checkReadOnly(DOMNode* start, DOMNode* end,
                                  XMLSize_t starOffset, XMLSize_t endOffset);
    void                  recurseTreeAndCheck(DOMNode* start, DOMNode* end);
    DOMNode*             removeChild(DOMNode* parent, DOMNode* child);

    DOMDocumentFragment* traverseSameContainer( int how );
    DOMDocumentFragment* traverseCommonStartContainer( DOMNode *endAncestor, int how );
    DOMDocumentFragment* traverseCommonEndContainer( DOMNode *startAncestor, int how );
    DOMDocumentFragment* traverseCommonAncestors( DOMNode *startAncestor, DOMNode *endAncestor, int how );
    DOMNode*    traverseRightBoundary( DOMNode *root, int how );
    DOMNode*    traverseLeftBoundary( DOMNode *root, int how );
    DOMNode*    traverseNode( DOMNode *n, bool isFullySelected, bool isLeft, int how );
    DOMNode*    traverseFullySelected( DOMNode *n, int how );
    DOMNode*    traversePartiallySelected( DOMNode *n, int how );
    DOMNode*    traverseTextNode( DOMNode *n, bool isLeft, int how );
    DOMNode*    getSelectedNode( DOMNode *container, int offset );

    // -----------------------------------------------------------------------
    // Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    DOMRangeImpl & operator = (const DOMRangeImpl &);
};

XERCES_CPP_NAMESPACE_END

#endif
