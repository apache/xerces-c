/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 2001-2004 The Apache Software Foundation.  All rights
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

#include "DOMDocumentFragmentImpl.hpp"
#include "DOMDocumentImpl.hpp"
#include "DOMCasts.hpp"
#include <xercesc/dom/DOMNode.hpp>
#include <xercesc/dom/DOMException.hpp>
#include <xercesc/util/XMLUniDefs.hpp>

XERCES_CPP_NAMESPACE_BEGIN


DOMDocumentFragmentImpl::DOMDocumentFragmentImpl(DOMDocument *masterDoc)
    : fNode(masterDoc), fParent(masterDoc)
{
}


DOMDocumentFragmentImpl::DOMDocumentFragmentImpl(const DOMDocumentFragmentImpl &other,
                                           bool deep)
    : fNode(other.fNode), fParent(other.fParent)
{
    if (deep)
        castToParentImpl(this)->cloneChildren(&other);
}


DOMDocumentFragmentImpl::~DOMDocumentFragmentImpl()
{
}



DOMNode *DOMDocumentFragmentImpl::cloneNode(bool deep) const
{
    DOMNode* newNode = new (castToNodeImpl(this)->getOwnerDocument(), DOMDocumentImpl::DOCUMENT_FRAGMENT_OBJECT) DOMDocumentFragmentImpl(*this, deep);
    fNode.callUserDataHandlers(DOMUserDataHandler::NODE_CLONED, this, newNode);
    return newNode;
}


const XMLCh * DOMDocumentFragmentImpl::getNodeName() const {
    static const XMLCh name[] = {chPound, chLatin_d, chLatin_o, chLatin_c, chLatin_u, chLatin_m,
        chLatin_e, chLatin_n, chLatin_t, chDash,
        chLatin_f, chLatin_r, chLatin_a, chLatin_g, chLatin_m, chLatin_e, chLatin_n, chLatin_t, 0};
    return name;
}


short DOMDocumentFragmentImpl::getNodeType() const {
    return DOMNode::DOCUMENT_FRAGMENT_NODE;
}


void DOMDocumentFragmentImpl::setNodeValue(const XMLCh *x)
{
    fNode.setNodeValue(x);
}


void DOMDocumentFragmentImpl::release()
{
    if (fNode.isOwned() && !fNode.isToBeReleased())
        throw DOMException(DOMException::INVALID_ACCESS_ERR,0);

    DOMDocumentImpl* doc = (DOMDocumentImpl*) getOwnerDocument();
    if (doc) {
        fNode.callUserDataHandlers(DOMUserDataHandler::NODE_DELETED, 0, 0);
        fParent.release();
        doc->release(this, DOMDocumentImpl::DOCUMENT_FRAGMENT_OBJECT);
    }
    else {
        // shouldn't reach here
        throw DOMException(DOMException::INVALID_ACCESS_ERR,0);
    }
}

//
//  Delegation stubs for inherited functions.
//
           DOMNode*         DOMDocumentFragmentImpl::appendChild(DOMNode *newChild)          {return fParent.appendChild (newChild); }
           DOMNamedNodeMap* DOMDocumentFragmentImpl::getAttributes() const                   {return fNode.getAttributes (); }
           DOMNodeList*     DOMDocumentFragmentImpl::getChildNodes() const                   {return fParent.getChildNodes (); }
           DOMNode*         DOMDocumentFragmentImpl::getFirstChild() const                   {return fParent.getFirstChild (); }
           DOMNode*         DOMDocumentFragmentImpl::getLastChild() const                    {return fParent.getLastChild (); }
     const XMLCh*           DOMDocumentFragmentImpl::getLocalName() const                    {return fNode.getLocalName (); }
     const XMLCh*           DOMDocumentFragmentImpl::getNamespaceURI() const                 {return fNode.getNamespaceURI (); }
           DOMNode*         DOMDocumentFragmentImpl::getNextSibling() const                  {return fNode.getNextSibling (); }
     const XMLCh*           DOMDocumentFragmentImpl::getNodeValue() const                    {return fNode.getNodeValue (); }
           DOMDocument*     DOMDocumentFragmentImpl::getOwnerDocument() const                {return fParent.fOwnerDocument; }
     const XMLCh*           DOMDocumentFragmentImpl::getPrefix() const                       {return fNode.getPrefix (); }
           DOMNode*         DOMDocumentFragmentImpl::getParentNode() const                   {return fNode.getParentNode (); }
           DOMNode*         DOMDocumentFragmentImpl::getPreviousSibling() const              {return fNode.getPreviousSibling (); }
           bool             DOMDocumentFragmentImpl::hasChildNodes() const                   {return fParent.hasChildNodes (); }
           DOMNode*         DOMDocumentFragmentImpl::insertBefore(DOMNode *newChild, DOMNode *refChild)
                                                                                             {return fParent.insertBefore (newChild, refChild); }
           void             DOMDocumentFragmentImpl::normalize()                             {fParent.normalize (); }
           DOMNode*         DOMDocumentFragmentImpl::removeChild(DOMNode *oldChild)          {return fParent.removeChild (oldChild); }
           DOMNode*         DOMDocumentFragmentImpl::replaceChild(DOMNode *newChild, DOMNode *oldChild)
                                                                                             {return fParent.replaceChild (newChild, oldChild); }
           bool             DOMDocumentFragmentImpl::isSupported(const XMLCh *feature, const XMLCh *version) const
                                                                                             {return fNode.isSupported (feature, version); }
           void             DOMDocumentFragmentImpl::setPrefix(const XMLCh  *prefix)         {fNode.setPrefix(prefix); }
           bool             DOMDocumentFragmentImpl::hasAttributes() const                   {return fNode.hasAttributes(); }
           bool             DOMDocumentFragmentImpl::isSameNode(const DOMNode* other) const  {return fNode.isSameNode(other); }
           bool             DOMDocumentFragmentImpl::isEqualNode(const DOMNode* arg) const   {return fParent.isEqualNode(arg); }
           void*            DOMDocumentFragmentImpl::setUserData(const XMLCh* key, void* data, DOMUserDataHandler* handler)
                                                                                             {return fNode.setUserData(key, data, handler); }
           void*            DOMDocumentFragmentImpl::getUserData(const XMLCh* key) const     {return fNode.getUserData(key); }
           const XMLCh*     DOMDocumentFragmentImpl::getBaseURI() const                      {return fNode.getBaseURI(); }
           short            DOMDocumentFragmentImpl::compareTreePosition(const DOMNode* other) const {return fNode.compareTreePosition(other); }
           const XMLCh*     DOMDocumentFragmentImpl::getTextContent() const                  {return fNode.getTextContent(); }
           void             DOMDocumentFragmentImpl::setTextContent(const XMLCh* textContent){fNode.setTextContent(textContent); }
           const XMLCh*     DOMDocumentFragmentImpl::lookupNamespacePrefix(const XMLCh* namespaceURI, bool useDefault) const  {return fNode.lookupNamespacePrefix(namespaceURI, useDefault); }
           bool             DOMDocumentFragmentImpl::isDefaultNamespace(const XMLCh* namespaceURI) const {return fNode.isDefaultNamespace(namespaceURI); }
           const XMLCh*     DOMDocumentFragmentImpl::lookupNamespaceURI(const XMLCh* prefix) const  {return fNode.lookupNamespaceURI(prefix); }
           DOMNode*         DOMDocumentFragmentImpl::getInterface(const XMLCh* feature)      {return fNode.getInterface(feature); }

XERCES_CPP_NAMESPACE_END


