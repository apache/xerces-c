#ifndef DocumentImpl_HEADER_GUARD_
#define DocumentImpl_HEADER_GUARD_

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

/*
 * $Id$
 */

//
//  This file is part of the internal implementation of the C++ XML DOM.
//  It should NOT be included or used directly by application programs.
//
//  Applications should include the file <dom/DOM.hpp> for the entire
//  DOM API, or DOM_*.hpp for individual DOM classes, where the class
//  name is substituded for the *.
//

#include <util/XercesDefs.hpp>
#include "NodeContainer.hpp"
#include "DOM_Node.hpp"
#include "DOM_Element.hpp"
#include "util/RefVectorOf.hpp"
#include "util/RefHashTableOf.hpp"

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

typedef RefVectorOf<NodeIteratorImpl> NodeIterators;
typedef RefVectorOf<TreeWalkerImpl> TreeWalkers;


class CDOM_EXPORT DocumentImpl: public NodeContainer {
private:
private:
    // -----------------------------------------------------------------------
    //  Private data types
    // -----------------------------------------------------------------------
    class PoolElem
    {
        public :
            PoolElem(const DOMString string, const DOM_Element &elem);
            ~PoolElem();

            const XMLCh* getKey() const;

            DOM_Element		 fElem;
            DOMString        fString;
    };

   	void setDocumentType(DocumentTypeImpl *doctype);

    DocumentTypeImpl            *docType;
    ElementImpl                 *docElement;
    DStringPool                 *namePool;

    RefHashTableOf<PoolElem>	*identifiers;
    NodeIterators               *iterators;
    TreeWalkers                 *treeWalkers;
    friend class NodeIteratorImpl;
    friend class TreeWalkerImpl;
   	friend class DOMParser;

public:
    DocumentImpl();
    DocumentImpl(const DOMString &namespaceURI,	    //DOM Level 2
	const DOMString &qualifiedName, DocumentTypeImpl *doctype);
    virtual ~DocumentImpl();
    virtual bool isDocumentImpl();   // RTTI replacement function

    virtual NodeImpl            *cloneNode(bool deep);
    virtual DOMString getNodeName();
    virtual short getNodeType();
    virtual AttrImpl            *createAttribute(const DOMString &name);
    virtual CDATASectionImpl    *createCDATASection(const DOMString &data);
    virtual CommentImpl         *createComment(const DOMString &data);
    virtual DocumentFragmentImpl *createDocumentFragment();
    virtual DocumentTypeImpl    *createDocumentType(const DOMString &name);
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
    static  bool                isXMLName(const DOMString & s);
    virtual void                referenced();
    virtual NodeImpl            *removeChild(NodeImpl *oldChild);
    virtual void                setNodeValue(const DOMString & x);
    virtual void                unreferenced();
    static  NodeIteratorImpl*   createNodeIterator(DOM_Node root, unsigned long whatToShow, DOM_NodeFilter* filter, bool entityReferenceExpansion);
    static  TreeWalkerImpl*     createTreeWalker(DOM_Node root, unsigned long whatToShow, DOM_NodeFilter* filter, bool entityReferenceExpansion);

    //Introduced in DOM Level 2
    virtual NodeImpl            *importNode(NodeImpl *source, bool deep);
    virtual ElementImpl         *createElementNS(const DOMString &namespaceURI,
	const DOMString &qualifiedName);
    virtual AttrImpl            *createAttributeNS(const DOMString &namespaceURI,
	const DOMString &qualifiedName);
    virtual DeepNodeListImpl    *getElementsByTagNameNS(const DOMString &namespaceURI,
	const DOMString &localName);
    virtual ElementImpl         *getElementById(const DOMString &elementId);

	// Non-standard accessory functions
    virtual void		         putIdentifier(const DOMString &elementId, const DOM_Element &ele);

    //Return the index > 0 of ':' in the given qualified name qName="prefix:localName".
    //Return 0 if there is no ':', or -1 if qName is malformed such as ":abcd".
    static  int                 indexofQualifiedName(const DOMString & qName);
};

#endif
