#ifndef ElementImpl_HEADER_GUARD_
#define ElementImpl_HEADER_GUARD_

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
#include "AttrImpl.hpp"
#include "AttrMapImpl.hpp"
#include "ParentNode.hpp"

XERCES_CPP_NAMESPACE_BEGIN


class DeepNodeListImpl;

class CDOM_EXPORT ElementImpl: public ParentNode {
protected:
    DOMString name;
    AttrMapImpl *attributes;

public:
    ElementImpl(DocumentImpl *ownerDoc, const DOMString &name);
    ElementImpl(const ElementImpl &other, bool deep=false);
    virtual ~ElementImpl();

    virtual bool isElementImpl();
    virtual NodeImpl * cloneNode(bool deep);
    virtual DOMString getNodeName();
    virtual short getNodeType();
    virtual DOMString getAttribute(const DOMString &name);
    virtual AttrImpl *getAttributeNode(const DOMString &name);
    virtual NamedNodeMapImpl * getAttributes();
    virtual DeepNodeListImpl * getElementsByTagName(const DOMString &tagname);
    virtual DOMString getTagName();
    virtual void removeAttribute(const DOMString &name);
    virtual AttrImpl * removeAttributeNode(AttrImpl * oldAttr);
    virtual AttrImpl *setAttribute(const DOMString &name, const DOMString &value);
    virtual AttrImpl *setAttributeNode(AttrImpl *newAttr);
    virtual void setReadOnly(bool readOnly, bool deep);

    //Introduced in DOM Level 2
    virtual DOMString getAttributeNS(const DOMString &namespaceURI,
	const DOMString &localName);
    virtual AttrImpl *setAttributeNS(const DOMString &namespaceURI,
	const DOMString &qualifiedName, const DOMString &value);
    virtual void removeAttributeNS(const DOMString &namespaceURI,
	const DOMString &localName);
    virtual AttrImpl *getAttributeNodeNS(const DOMString &namespaceURI,
	const DOMString &localName);
    virtual AttrImpl *setAttributeNodeNS(AttrImpl *newAttr);
    virtual DeepNodeListImpl *getElementsByTagNameNS(const DOMString &namespaceURI,
	const DOMString &localName);

    virtual void setOwnerDocument(DocumentImpl *doc);
    virtual bool hasAttributes();
    virtual bool hasAttribute(const DOMString &name);
    virtual bool hasAttributeNS(const DOMString &namespaceURI, const DOMString &localName);

	//Utils for the DOM_NamedNodeMap wrapper class
	//All NamedNodeMap utils begin with NNM
    virtual NamedNodeMapImpl *NNM_cloneMap(NodeImpl *nnm_ownerNode);
    virtual int             NNM_findNamePoint(const DOMString &nnm_name);
    virtual unsigned int    NNM_getLength();
    virtual NodeImpl       *NNM_getNamedItem(const DOMString &nnm_name);
    virtual NodeImpl       *NNM_item(unsigned int nnm_index);
    virtual void            NNM_removeAll();
    virtual NodeImpl       *NNM_removeNamedItem(const DOMString &nnm_name);
    virtual NodeImpl       *NNM_setNamedItem(NodeImpl *nnm_arg);
    virtual void            NNM_setReadOnly(bool nnm_readOnly, bool nnm_deep);
    //Introduced in DOM Level 2
    virtual int             NNM_findNamePoint(const DOMString &nnm_namespaceURI, const DOMString &nnm_localName);
    virtual NodeImpl       *NNM_getNamedItemNS(const DOMString &nnm_namespaceURI, const DOMString &nnm_localName);
    virtual NodeImpl       *NNM_setNamedItemNS(NodeImpl *nnm_arg);
    virtual NodeImpl       *NNM_removeNamedItemNS(const DOMString &nnm_namespaceURI, const DOMString &nnm_localName);
    virtual void            NNM_setOwnerDocument(DocumentImpl *nnm_doc);

	// default attribute helper functions
	virtual AttrMapImpl *getDefaultAttributes();
	virtual void setupDefaultAttributes();

    // -----------------------------------------------------------------------
    //  Notification that lazy data has been deleted
    // -----------------------------------------------------------------------
	static void reinitElementImpl();
};

XERCES_CPP_NAMESPACE_END

#endif
