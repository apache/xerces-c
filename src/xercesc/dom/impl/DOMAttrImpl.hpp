#ifndef DOMAttrImpl_HEADER_GUARD_
#define DOMAttrImpl_HEADER_GUARD_

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
#include "DOMParentNode.hpp"
#include "DOMNodeImpl.hpp"
#include "DOMDocumentImpl.hpp"
#include <xercesc/dom/DOMAttr.hpp>
#include <xercesc/dom/DOMTypeInfo.hpp>
#include <xercesc/framework/XMLBuffer.hpp>
#include "DOMNodeIDMap.hpp"

XERCES_CPP_NAMESPACE_BEGIN

class DOMElementImpl;

class CDOM_EXPORT DOMAttrImpl: public DOMAttr {

public:
    DOMNodeImpl        fNode;
    DOMParentNode      fParent;
    const XMLCh       *fName;

private:
    const DOMTypeInfo *fSchemaType;

public:
    DOMAttrImpl(DOMDocument *ownerDocument, const XMLCh *aName);
    DOMAttrImpl(const DOMAttrImpl &other, bool deep=false);
    virtual ~DOMAttrImpl();

     // Add all functions that are pure virtual in DOMNODE
    DOMNODE_FUNCTIONS;

    virtual const XMLCh *       getName() const;
    virtual bool getSpecified() const;
    virtual const XMLCh * getValue() const;
    virtual void setSpecified(bool arg);
    virtual void setValue(const XMLCh * value);
    virtual bool isId() const;

    //Introduced in DOM Level 2
    DOMElement *getOwnerElement() const;
    void setOwnerElement(DOMElement *ownerElem);    //internal use only

    // helper function for DOM Level 3 renameNode
    virtual DOMNode* rename(const XMLCh* namespaceURI, const XMLCh* name);

    virtual const DOMTypeInfo* getTypeInfo() const;

    //helper function for DOM Level 3 TypeInfo
    virtual void setTypeInfo(const XMLCh* typeName, const XMLCh* typeURI);

   // helper method that sets this attr to an idnode and places it into the document map
   virtual void addAttrToIDNodeMap();

   // helper to remove this attr from from the id map if it is in there
   virtual void removeAttrFromIDNodeMap();
private:
    void getTextValue(DOMNode* node, XMLBuffer& buf) const;

    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------    
    DOMAttrImpl& operator=(const DOMAttrImpl&);
};

inline void DOMAttrImpl::removeAttrFromIDNodeMap()
{
    if (fNode.isIdAttr()) {
        ((DOMDocumentImpl *)getOwnerDocument())->getNodeIDMap()->remove(this);
        fNode.isIdAttr(false);
    }
}

inline void DOMAttrImpl::addAttrToIDNodeMap()
{
    if (fNode.isIdAttr()) 
        return;

    fNode.isIdAttr(true);

    // REVIST For now, we don't worry about what happens if the new
    // name conflicts as per setValue
    DOMDocumentImpl *doc = (DOMDocumentImpl *)(fParent.fOwnerDocument);

    if (doc->fNodeIDMap == 0)
        doc->fNodeIDMap = new (doc) DOMNodeIDMap(500, doc);

    doc->getNodeIDMap()->add(this);
}

XERCES_CPP_NAMESPACE_END

#endif
