#ifndef IDElementImpl_HEADER_GUARD_
#define IDElementImpl_HEADER_GUARD_

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

//
//  This file is part of the internal implementation of the C++ XML DOM.
//  It should NOT be included or used directly by application programs.
//
//  Applications should include the file <dom/DOM.hpp> for the entire
//  DOM API, or DOM_*.hpp for individual DOM classes, where the class
//  name is substituded for the *.
//


#include <util/XercesDefs.hpp>
#include <util/XMLString.hpp>
#include "IDOM_Element.hpp"
#include "IDChildNode.hpp"
#include "IDNodeImpl.hpp"
#include "IDParentNode.hpp"

//#include "AttrImpl.hpp"
#include "IDAttrMapImpl.hpp"

class IDOMNodeList;
class IDAttrMapImpl;
class IDOM_Document;




class CDOM_EXPORT IDElementImpl: public IDOM_Element {
public:
    IDNodeImpl       fNode;
    IDParentNode     fParent;
    IDChildNode      fChild;
    IDAttrMapImpl    fAttributes;
    const XMLCh      *fName;

public:
    IDElementImpl(IDOM_Document *ownerDoc, const XMLCh *name);

    IDElementImpl(const IDElementImpl &other, bool deep=false);
    virtual ~IDElementImpl();

    // Declare functions from IDOM_Node.  They all must be implemented by this class
    IDOM_NODE_FUNCTIONS;

    // Functions introduced on Element...
    virtual const XMLCh     * getAttribute(const XMLCh *name) const;
    virtual IDOM_Attr       * getAttributeNode(const XMLCh *name) const;
    virtual IDOM_NodeList   * getElementsByTagName(const XMLCh *tagname) const;
    virtual const XMLCh     * getTagName() const;
    virtual void              removeAttribute(const XMLCh *name);
    virtual IDOM_Attr       * removeAttributeNode(IDOM_Attr * oldAttr);
    virtual void              setAttribute(const XMLCh *name, const XMLCh *value);
    virtual IDOM_Attr       * setAttributeNode(IDOM_Attr *newAttr);
    virtual void              setReadOnly(bool readOnly, bool deep);

    //Introduced in DOM Level 2
    virtual const XMLCh *     getAttributeNS(const XMLCh *namespaceURI,
                                         const XMLCh *localName) const;
    virtual void              setAttributeNS(const XMLCh *namespaceURI,
                                   const XMLCh *qualifiedName,
                                   const XMLCh *value);
    virtual void              removeAttributeNS(const XMLCh *namespaceURI,
                                   const XMLCh *localName);
    virtual IDOM_Attr        *getAttributeNodeNS(const XMLCh *namespaceURI,
                                         const XMLCh *localName) const;
    virtual IDOM_Attr        *setAttributeNodeNS(IDOM_Attr *newAttr);
    virtual IDOM_NodeList    *getElementsByTagNameNS(const XMLCh *namespaceURI,
                                                  const XMLCh *localName) const;



#ifdef idom_revisit
	//Utils for the DOM_NamedNodeMap wrapper class
	//All NamedNodeMap utils begin with NNM
    virtual NamedNodeMapImpl *NNM_cloneMap(NodeImpl *nnm_ownerNode);
    virtual int             NNM_findNamePoint(const XMLCh *nnm_name);
    virtual unsigned int    NNM_getLength();
    virtual NodeImpl       *NNM_getNamedItem(const XMLCh *nnm_name);
    virtual NodeImpl       *NNM_item(unsigned int nnm_index);
    virtual void            NNM_removeAll();
    virtual NodeImpl       *NNM_removeNamedItem(const XMLCh *nnm_name);
    virtual NodeImpl       *NNM_setNamedItem(NodeImpl *nnm_arg);
    virtual void            NNM_setReadOnly(bool nnm_readOnly, bool nnm_deep);
    //Introduced in DOM Level 2
    virtual int             NNM_findNamePoint(const XMLCh *nnm_namespaceURI, const XMLCh *nnm_localName);
    virtual NodeImpl       *NNM_getNamedItemNS(const XMLCh *nnm_namespaceURI, const XMLCh *nnm_localName);
    virtual NodeImpl       *NNM_setNamedItemNS(NodeImpl *nnm_arg);
    virtual NodeImpl       *NNM_removeNamedItemNS(const XMLCh *nnm_namespaceURI, const XMLCh *nnm_localName);
    virtual void            NNM_setOwnerDocument(DocumentImpl *nnm_doc);
#endif //idom_revisit


	// default attribute helper functions
	virtual IDAttrMapImpl *getDefaultAttributes();
	virtual void setupDefaultAttributes();


};

#endif
