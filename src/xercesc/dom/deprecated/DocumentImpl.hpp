#ifndef DocumentImpl_HEADER_GUARD_
#define DocumentImpl_HEADER_GUARD_

/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 1999-2002 The Apache Software Foundation.  All rights
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

/*
 * $Id$
 */

//
//  This file is part of the internal implementation of the C++ XML DOM.
//  It should NOT be included or used directly by application programs.
//
//  Applications should include the file <xercesc/dom/deprecated/DOM.hpp> for the entire
//  DOM API, or DOM_*.hpp for individual DOM classes, where the class
//  name is substituded for the *.
//

#include <xercesc/util/XercesDefs.hpp>
#include "ParentNode.hpp"
#include "DOM_Node.hpp"
#include "DOM_Element.hpp"
#include "xercesc/util/RefVectorOf.hpp"
#include "xercesc/util/RefHashTableOf.hpp"
#include "XMLDeclImpl.hpp"

XERCES_CPP_NAMESPACE_BEGIN


class DocumentTypeImpl;
class ElementImpl;
class AttrImpl;
class CDATASectionImpl;
class CommentImpl;
class DeepNodeListImpl;
class DocumentFragmentImpl;
class DocumentTypeImpl;
class DStringPool;
class EntityImpl;
class EntityReferenceImpl;
class NotationImpl;
class ProcessingInstructionImpl;
class TextImpl;
class NodeIteratorImpl;
class TreeWalkerImpl;
class DOM_NodeFilter;
class NodeFilterImpl;
class DOM_DOMImplementation;
class DOMString;
class NodeIDMap;
class RangeImpl;

typedef RefVectorOf<NodeIteratorImpl> NodeIterators;
typedef RefVectorOf<TreeWalkerImpl> TreeWalkers;
typedef RefVectorOf<RangeImpl> RangeImpls;


class CDOM_EXPORT DocumentImpl: public ParentNode {
private:
    // -----------------------------------------------------------------------
    //  Private data types
    // -----------------------------------------------------------------------

   	void setDocumentType(DocumentTypeImpl *doctype);

    DocumentTypeImpl            *docType;
    ElementImpl                 *docElement;
    DStringPool                 *namePool;
    NodeIDMap                   *fNodeIDMap;      // for use by GetElementsById().

    NodeIterators               *iterators;
    TreeWalkers                 *treeWalkers;
	RefHashTableOf<void>		*userData;
    RangeImpls                  *ranges;

    /**
     * Number of alterations made to this document since its creation.
     * Serves as a "dirty bit" so that live objects such as NodeList can
     * recognize when an alteration has been made and discard its cached
     * state information.
     * <p>
     * Any method that alters the tree structure MUST cause or be
     * accompanied by a call to changed(), to inform it that any outstanding
     * NodeLists may have to be updated.
     * <p>
     * (Required because NodeList is simultaneously "live" and integer-
     * indexed -- a bad decision in the DOM's design.)
     * <p>
     * Note that changes which do not affect the tree's structure -- changing
     * the node's name, for example -- do _not_ have to call changed().
     * <p>
     * Alternative implementation would be to use a cryptographic
     * Digest value rather than a count. This would have the advantage that
     * "harmless" changes (those producing equal() trees) would not force
     * NodeList to resynchronize. Disadvantage is that it's slightly more prone
     * to "false negatives", though that's the difference between "wildly
     * unlikely" and "absurdly unlikely". IF we start maintaining digests,
     * we should consider taking advantage of them.
     *
     * Note: This used to be done a node basis, so that we knew what
     * subtree changed. But since only DeepNodeList really use this today,
     * the gain appears to be really small compared to the cost of having
     * an int on every (parent) node plus having to walk up the tree all the
     * way to the root to mark the branch as changed everytime a node is
     * changed.
     * So we now have a single counter global to the document. It means that
     * some objects may flush their cache more often than necessary, but this
     * makes nodes smaller and only the document needs to be marked as changed.
     */
    int fChanges;

    /** Bypass error checking. */
    bool errorChecking;

    MemoryManager               *fMemoryManager;

    friend class NodeIteratorImpl;
    friend class TreeWalkerImpl;
    friend class RangeImpl;
   	friend class DOMParser;

public:
    DocumentImpl(MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);
    DocumentImpl(const DOMString &namespaceURI,	    //DOM Level 2
	             const DOMString &qualifiedName,
                 DocumentTypeImpl *doctype,
                 MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);
    virtual ~DocumentImpl();
    virtual bool isDocumentImpl();   // RTTI replacement function

    virtual NodeImpl            *cloneNode(bool deep);
    virtual DOMString getNodeName();
    virtual short getNodeType();
    virtual DocumentImpl * getOwnerDocument();
    virtual AttrImpl            *createAttribute(const DOMString &name);
    virtual CDATASectionImpl    *createCDATASection(const DOMString &data);
    virtual CommentImpl         *createComment(const DOMString &data);
    virtual DocumentFragmentImpl *createDocumentFragment();
    virtual DocumentTypeImpl    *createDocumentType(const DOMString &name);
    virtual DocumentTypeImpl    *createDocumentType(const DOMString &qName,
                                                    const DOMString &publicId,
                                                    const DOMString &systemId);
    virtual ElementImpl         *createElement(const DOMString & tagName);
    virtual ElementImpl         *createElement(const XMLCh *tagName);
    virtual EntityImpl          *createEntity(const DOMString & name);
    virtual EntityReferenceImpl *createEntityReference(const DOMString & name);
    virtual NotationImpl        *createNotation(const DOMString & name);
    virtual ProcessingInstructionImpl *createProcessingInstruction(const DOMString & target, const DOMString & data);
    virtual TextImpl            *createTextNode(const DOMString & data);
    virtual DocumentTypeImpl    *getDoctype();
    virtual ElementImpl         *getDocumentElement();
    virtual DeepNodeListImpl    *getElementsByTagName(const DOMString & tagname);
    virtual NodeImpl            *insertBefore(NodeImpl *newChild, NodeImpl *refChild);
    bool                        isXMLName(const DOMString & s);
    virtual void                referenced();
    virtual NodeImpl            *removeChild(NodeImpl *oldChild);
    virtual void                unreferenced();
    static  NodeIteratorImpl*   createNodeIterator(DOM_Node root, unsigned long whatToShow, DOM_NodeFilter* filter, bool entityReferenceExpansion,
                                                   MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);
    static  TreeWalkerImpl*     createTreeWalker(DOM_Node root, unsigned long whatToShow, DOM_NodeFilter* filter, bool entityReferenceExpansion,
                                                 MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);
    virtual XMLDeclImpl*        createXMLDecl(const DOMString& version, const DOMString& encoding, const DOMString& standalone);
    virtual void*				getUserData();
    virtual void				setUserData(void* value);
    virtual RangeImpl*          createRange();
    virtual RangeImpls*         getRanges();  //non-standard api
    virtual void                removeRange(RangeImpl* range); //non-standard api


	// helper functions to prevent storing userdata pointers on every node.
    virtual void setUserData(NodeImpl* n, void* data);
    virtual void* getUserData(NodeImpl* n);

    //Introduced in DOM Level 2
    virtual NodeImpl            *importNode(NodeImpl *source, bool deep);
    virtual ElementImpl         *createElementNS(const DOMString &namespaceURI,
	                                             const DOMString &qualifiedName);
    virtual AttrImpl            *createAttributeNS(const DOMString &namespaceURI,
	                                             const DOMString &qualifiedName);
    virtual DeepNodeListImpl    *getElementsByTagNameNS(const DOMString &namespaceURI,
	                                              const DOMString &localName);
    virtual ElementImpl         *getElementById(const DOMString &elementId);

    //Return the index > 0 of ':' in the given qualified name qName="prefix:localName".
    //Return 0 if there is no ':', or -1 if qName is malformed such as ":abcd".
    static  int                 indexofQualifiedName(const DOMString & qName);
    static  bool                isKidOK(NodeImpl *parent, NodeImpl *child);

    inline NodeIDMap *          getNodeIDMap() {return fNodeIDMap;};

    virtual void changed();
    virtual int changes();

    /**
     * Sets whether the DOM implementation performs error checking
     * upon operations. Turning off error checking only affects
     * the following DOM checks:
     * <ul>
     * <li>Checking strings to make sure that all characters are
     *     legal XML characters
     * <li>Hierarchy checking such as allowed children, checks for
     *     cycles, etc.
     * </ul>
     * <p>
     * Turning off error checking does <em>not</em> turn off the
     * following checks:
     * <ul>
     * <li>Read only checks
     * <li>Checks related to DOM events
     * </ul>
     */
    inline void setErrorChecking(bool check) {
        errorChecking = check;
    }

    /**
     * Returns true if the DOM implementation performs error checking.
     */
    inline bool getErrorChecking() {
        return errorChecking;
    }

    inline MemoryManager* getMemoryManager() const {
        return fMemoryManager;
    }

    // -----------------------------------------------------------------------
    //  Notification that lazy data has been deleted
    // -----------------------------------------------------------------------
	static void reinitDocumentImpl();

};

XERCES_CPP_NAMESPACE_END

#endif
