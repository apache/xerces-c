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

#include "DOMDocumentImpl.hpp"
#include "DOMDocumentTypeImpl.hpp"
#include "DOMEntityImpl.hpp"
#include "DOMEntityReferenceImpl.hpp"
#include "DOMNamedNodeMapImpl.hpp"

#include <xercesc/dom/DOMException.hpp>
#include <xercesc/dom/DOMNode.hpp>

XERCES_CPP_NAMESPACE_BEGIN


DOMEntityReferenceImpl::DOMEntityReferenceImpl(DOMDocument *ownerDoc,
                                         const XMLCh *entityName)
    : fNode(ownerDoc), fParent(ownerDoc), fBaseURI(0)
{
    fName = ((DOMDocumentImpl *)getOwnerDocument())->getPooledString(entityName);
    // EntityReference behaves as a read-only node, since its contents
    // reflect the Entity it refers to -- but see setNodeName().
    //retrieve the corresponding entity content

    if (ownerDoc) {
        if (ownerDoc->getDoctype()) {
            if (ownerDoc->getDoctype()->getEntities()) {
                DOMEntityImpl* entity = (DOMEntityImpl*)ownerDoc->getDoctype()->getEntities()->getNamedItem(entityName);
                if (entity) {
                    fBaseURI = entity->getBaseURI();
                    DOMEntityReference* refEntity = entity->getEntityRef();
                    if (refEntity) {
                        fParent.cloneChildren(refEntity);
                    }
                }
            }
        }
    }

    fNode.setReadOnly(true, true);
}


DOMEntityReferenceImpl::DOMEntityReferenceImpl(DOMDocument *ownerDoc,
                                         const XMLCh *entityName,
                                         bool cloneChild)
    : fNode(ownerDoc), fParent(ownerDoc), fBaseURI(0)
{
    fName = ((DOMDocumentImpl *)getOwnerDocument())->getPooledString(entityName);
    // EntityReference behaves as a read-only node, since its contents
    // reflect the Entity it refers to -- but see setNodeName().
    //retrieve the corresponding entity content

    if (ownerDoc) {
        if (ownerDoc->getDoctype()) {
            if (ownerDoc->getDoctype()->getEntities()) {
                DOMEntityImpl* entity = (DOMEntityImpl*)ownerDoc->getDoctype()->getEntities()->getNamedItem(entityName);
                if (entity) {
                    fBaseURI = entity->getBaseURI();
                    if (cloneChild) {
                        DOMEntityReference* refEntity = entity->getEntityRef();
                        if (refEntity) {
                            fParent.cloneChildren(refEntity);
                        }
                    }
                }
            }
        }
    }

    fNode.setReadOnly(true, true);
}

DOMEntityReferenceImpl::DOMEntityReferenceImpl(const DOMEntityReferenceImpl &other,
                                         bool deep)
    : fNode(other.fNode), fParent(other.fParent), fChild(other.fChild), fName(other.fName), fBaseURI(other.fBaseURI)
{    
    if (deep)
        fParent.cloneChildren(&other);
    fNode.setReadOnly(true, true);
}



DOMEntityReferenceImpl::~DOMEntityReferenceImpl()
{
}

DOMNode *DOMEntityReferenceImpl::cloneNode(bool deep) const
{
    DOMNode* newNode = new (getOwnerDocument(), DOMDocumentImpl::ENTITY_REFERENCE_OBJECT) DOMEntityReferenceImpl(*this, deep);
    fNode.callUserDataHandlers(DOMUserDataHandler::NODE_CLONED, this, newNode);
    return newNode;
}


const XMLCh * DOMEntityReferenceImpl::getNodeName() const
{
    return fName;
}


short DOMEntityReferenceImpl::getNodeType() const {
    return DOMNode::ENTITY_REFERENCE_NODE;
}



/**
* EntityReferences never have a nodeValue.
* @throws DOMException(NO_MODIFICATION_ALLOWED_ERR)
*/
void DOMEntityReferenceImpl::setNodeValue(const XMLCh *x)
{
    fNode.setNodeValue(x);
}


/**
* EntityRef is already, and must be, a read-only node. Attempts to change
* that will throw a NO_MODIFICATION_ALLOWED_ERR DOMException.
* <P>
* If you want to alter its contents, edit the Entity definition.
*
* @param readOnly boolean
*/
void DOMEntityReferenceImpl::setReadOnly(bool readOnl,bool deep)
{
    if(((DOMDocumentImpl *)getOwnerDocument())->getErrorChecking() && readOnl==false)
        throw DOMException(DOMException::NO_MODIFICATION_ALLOWED_ERR, 0);
    fNode.setReadOnly(readOnl,deep);
}


void DOMEntityReferenceImpl::release()
{
    if (fNode.isOwned() && !fNode.isToBeReleased())
        throw DOMException(DOMException::INVALID_ACCESS_ERR,0);

    DOMDocumentImpl* doc = (DOMDocumentImpl*) getOwnerDocument();
    if (doc) {
        fNode.callUserDataHandlers(DOMUserDataHandler::NODE_DELETED, 0, 0);
        fParent.release();
        doc->release(this, DOMDocumentImpl::ENTITY_REFERENCE_OBJECT);
    }
    else {
        // shouldn't reach here
        throw DOMException(DOMException::INVALID_ACCESS_ERR,0);
    }
}

const XMLCh* DOMEntityReferenceImpl::getBaseURI() const
{
    return fBaseURI;
}



//
//   Delegate functions from Node to the appropriate implementation.
//


           DOMNode*         DOMEntityReferenceImpl::appendChild(DOMNode *newChild)          {return fParent.appendChild (newChild); }
           DOMNamedNodeMap* DOMEntityReferenceImpl::getAttributes() const                   {return fNode.getAttributes (); }
           DOMNodeList*     DOMEntityReferenceImpl::getChildNodes() const                   {return fParent.getChildNodes (); }
           DOMNode*         DOMEntityReferenceImpl::getFirstChild() const                   {return fParent.getFirstChild (); }
           DOMNode*         DOMEntityReferenceImpl::getLastChild() const                    {return fParent.getLastChild (); }
     const XMLCh*           DOMEntityReferenceImpl::getLocalName() const                    {return fNode.getLocalName (); }
     const XMLCh*           DOMEntityReferenceImpl::getNamespaceURI() const                 {return fNode.getNamespaceURI (); }
           DOMNode*         DOMEntityReferenceImpl::getNextSibling() const                  {return fChild.getNextSibling (); }
     const XMLCh*           DOMEntityReferenceImpl::getNodeValue() const                    {return fNode.getNodeValue (); }
           DOMDocument*     DOMEntityReferenceImpl::getOwnerDocument() const                {return fParent.fOwnerDocument; }
     const XMLCh*           DOMEntityReferenceImpl::getPrefix() const                       {return fNode.getPrefix (); }
           DOMNode*         DOMEntityReferenceImpl::getParentNode() const                   {return fChild.getParentNode (this); }
           DOMNode*         DOMEntityReferenceImpl::getPreviousSibling() const              {return fChild.getPreviousSibling (this); }
           bool             DOMEntityReferenceImpl::hasChildNodes() const                   {return fParent.hasChildNodes (); }
           DOMNode*         DOMEntityReferenceImpl::insertBefore(DOMNode *newChild, DOMNode *refChild)
                                                                                            {return fParent.insertBefore (newChild, refChild); }
           void             DOMEntityReferenceImpl::normalize()                             {fParent.normalize (); }
           DOMNode*         DOMEntityReferenceImpl::removeChild(DOMNode *oldChild)          {return fParent.removeChild (oldChild); }
           DOMNode*         DOMEntityReferenceImpl::replaceChild(DOMNode *newChild, DOMNode *oldChild)
                                                                                            {return fParent.replaceChild (newChild, oldChild); }
           bool             DOMEntityReferenceImpl::isSupported(const XMLCh *feature, const XMLCh *version) const
                                                                                            {return fNode.isSupported (feature, version); }
           void             DOMEntityReferenceImpl::setPrefix(const XMLCh  *prefix)         {fNode.setPrefix(prefix); }
           bool             DOMEntityReferenceImpl::hasAttributes() const                   {return fNode.hasAttributes(); }
           bool             DOMEntityReferenceImpl::isSameNode(const DOMNode* other) const  {return fNode.isSameNode(other); }
           bool             DOMEntityReferenceImpl::isEqualNode(const DOMNode* arg) const   {return fParent.isEqualNode(arg); }
           void*            DOMEntityReferenceImpl::setUserData(const XMLCh* key, void* data, DOMUserDataHandler* handler)
                                                                                            {return fNode.setUserData(key, data, handler); }
           void*            DOMEntityReferenceImpl::getUserData(const XMLCh* key) const     {return fNode.getUserData(key); }
           short            DOMEntityReferenceImpl::compareTreePosition(const DOMNode* other) const {return fNode.compareTreePosition(other); }
           const XMLCh*     DOMEntityReferenceImpl::getTextContent() const                  {return fNode.getTextContent(); }
           void             DOMEntityReferenceImpl::setTextContent(const XMLCh* textContent){fNode.setTextContent(textContent); }
           const XMLCh*     DOMEntityReferenceImpl::lookupNamespacePrefix(const XMLCh* namespaceURI, bool useDefault) const  {return fNode.lookupNamespacePrefix(namespaceURI, useDefault); }
           bool             DOMEntityReferenceImpl::isDefaultNamespace(const XMLCh* namespaceURI) const {return fNode.isDefaultNamespace(namespaceURI); }
           const XMLCh*     DOMEntityReferenceImpl::lookupNamespaceURI(const XMLCh* prefix) const  {return fNode.lookupNamespaceURI(prefix); }
           DOMNode*         DOMEntityReferenceImpl::getInterface(const XMLCh* feature)      {return fNode.getInterface(feature); }

XERCES_CPP_NAMESPACE_END

