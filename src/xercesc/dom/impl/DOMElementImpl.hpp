#ifndef DOMElementImpl_HEADER_GUARD_
#define DOMElementImpl_HEADER_GUARD_

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
#include <xercesc/util/XMLString.hpp>
#include <xercesc/dom/DOMElement.hpp>

#include "DOMChildNode.hpp"
#include "DOMNodeImpl.hpp"
#include "DOMParentNode.hpp"

//#include "AttrImpl.hpp"
#include "DOMAttrMapImpl.hpp"

XERCES_CPP_NAMESPACE_BEGIN

class DOMTypeInfo;
class DOMNodeList;
class DOMAttrMapImpl;
class DOMDocument;




class CDOM_EXPORT DOMElementImpl: public DOMElement {
public:
    DOMNodeImpl       fNode;
    DOMParentNode     fParent;
    DOMChildNode      fChild;
    DOMAttrMapImpl    *fAttributes;
    DOMAttrMapImpl    *fDefaultAttributes;
    const XMLCh      *fName;

private:
    const DOMTypeInfo *fSchemaType;

public:
    DOMElementImpl(DOMDocument *ownerDoc, const XMLCh *name);

    DOMElementImpl(const DOMElementImpl &other, bool deep=false);
    virtual ~DOMElementImpl();

    // Declare functions from DOMNode.  They all must be implemented by this class
    DOMNODE_FUNCTIONS;

    // Functions introduced on Element...
    virtual const XMLCh*      getAttribute(const XMLCh *name) const;
    virtual DOMAttr*          getAttributeNode(const XMLCh *name) const;
    virtual DOMNodeList*      getElementsByTagName(const XMLCh *tagname) const;
    virtual const XMLCh*      getTagName() const;
    virtual void              removeAttribute(const XMLCh *name);
    virtual DOMAttr*          removeAttributeNode(DOMAttr * oldAttr);
    virtual void              setAttribute(const XMLCh *name, const XMLCh *value);
    virtual DOMAttr*          setAttributeNode(DOMAttr *newAttr);
    virtual void              setReadOnly(bool readOnly, bool deep);

    //Introduced in DOM Level 2
    virtual const XMLCh*      getAttributeNS(const XMLCh *namespaceURI,
                                             const XMLCh *localName) const;
    virtual void              setAttributeNS(const XMLCh *namespaceURI,
                                             const XMLCh *qualifiedName,
                                             const XMLCh *value);
    virtual void              removeAttributeNS(const XMLCh *namespaceURI,
                                                const XMLCh *localName);
    virtual DOMAttr*          getAttributeNodeNS(const XMLCh *namespaceURI,
                                                 const XMLCh *localName) const;
    virtual DOMAttr*          setAttributeNodeNS(DOMAttr *newAttr);
    virtual DOMNodeList*      getElementsByTagNameNS(const XMLCh *namespaceURI,
                                                     const XMLCh *localName) const;
    virtual bool              hasAttribute(const XMLCh *name) const;
    virtual bool              hasAttributeNS(const XMLCh *namespaceURI,
                                             const XMLCh *localName) const;

    //Introduced in DOM level 3
    virtual void setIdAttribute(const XMLCh* name);
    virtual void setIdAttributeNS(const XMLCh* namespaceURI, const XMLCh* localName);
    virtual void setIdAttributeNode(const DOMAttr *idAttr);

    // for handling of default attribute
    virtual DOMAttr*          setDefaultAttributeNode(DOMAttr *newAttr);
    virtual DOMAttr*          setDefaultAttributeNodeNS(DOMAttr *newAttr);
    virtual DOMAttrMapImpl*   getDefaultAttributes() const;

    // helper function for DOM Level 3 renameNode
    virtual DOMNode* rename(const XMLCh* namespaceURI, const XMLCh* name);

    virtual const DOMTypeInfo * getTypeInfo() const;

    //helper function for DOM Level 3 TypeInfo
    virtual void setTypeInfo(const XMLCh* typeName, const XMLCh* typeURI);

protected:
    // default attribute helper functions
    virtual void setupDefaultAttributes();

private:
    // -----------------------------------------------------------------------
    // Unimplemented constructors and operators
    // -----------------------------------------------------------------------    
    DOMElementImpl & operator = (const DOMElementImpl &);
};

XERCES_CPP_NAMESPACE_END

#endif
