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

#include <xercesc/dom/DOMException.hpp>
#include <xercesc/dom/DOMNode.hpp>
#include <xercesc/dom/DOMEntityReference.hpp>
#include "DOMEntityImpl.hpp"
#include "DOMDocumentImpl.hpp"

XERCES_CPP_NAMESPACE_BEGIN


DOMEntityImpl::DOMEntityImpl(DOMDocument *ownerDoc, const XMLCh *eName)
   : fNode(ownerDoc),
     fParent(ownerDoc),
     fPublicId(0),
     fSystemId(0),
     fNotationName(0),
     fRefEntity(0),
     fActualEncoding(0),
     fEncoding(0),
     fVersion(0),     
     fBaseURI(0),
     fEntityRefNodeCloned(false)
{   
    fName        = ((DOMDocumentImpl *)ownerDoc)->getPooledString(eName);
    fNode.setReadOnly(true, true);
}


DOMEntityImpl::DOMEntityImpl(const DOMEntityImpl &other, bool deep)
    : fNode(other.fNode),
      fParent(other.fParent),
      fName(other.fName),
      fPublicId(other.fPublicId),
      fSystemId(other.fSystemId),
      fNotationName(other.fNotationName),
      fRefEntity(other.fRefEntity),
      fActualEncoding(other.fActualEncoding),
      fEncoding(other.fEncoding),
      fVersion(other.fVersion),
      fBaseURI(other.fBaseURI),
      fEntityRefNodeCloned(false)
{    
    if (deep)
        fParent.cloneChildren(&other);   
    fNode.setReadOnly(true, true);
}


DOMEntityImpl::~DOMEntityImpl() {
}


DOMNode *DOMEntityImpl::cloneNode(bool deep) const
{
    DOMNode* newNode = new (getOwnerDocument(), DOMDocumentImpl::ENTITY_OBJECT) DOMEntityImpl(*this, deep);
    fNode.callUserDataHandlers(DOMUserDataHandler::NODE_CLONED, this, newNode);
    return newNode;
}


const XMLCh * DOMEntityImpl::getNodeName() const {
    return fName;
}


short DOMEntityImpl::getNodeType() const {
    return DOMNode::ENTITY_NODE;
}


const XMLCh * DOMEntityImpl::getNotationName() const
{
    return fNotationName;
}


const XMLCh * DOMEntityImpl::getPublicId() const {
    return fPublicId;
}


const XMLCh * DOMEntityImpl::getSystemId() const
{
    return fSystemId;
}


const XMLCh* DOMEntityImpl::getBaseURI() const
{
    return fBaseURI;
}


void DOMEntityImpl::setNodeValue(const XMLCh *arg)
{
    fNode.setNodeValue(arg);
}


void DOMEntityImpl::setNotationName(const XMLCh *arg)
{
    DOMDocumentImpl *doc = (DOMDocumentImpl *)this->getOwnerDocument();
    fNotationName = doc->cloneString(arg);
}


void DOMEntityImpl::setPublicId(const XMLCh *arg)
{
    DOMDocumentImpl *doc = (DOMDocumentImpl *)this->getOwnerDocument();
    fPublicId = doc->cloneString(arg);
}


void DOMEntityImpl::setSystemId(const XMLCh *arg)
{
    DOMDocumentImpl *doc = (DOMDocumentImpl *)this->getOwnerDocument();
    fSystemId = doc->cloneString(arg);
}


void DOMEntityImpl::setBaseURI(const XMLCh* baseURI) {
    if (baseURI && *baseURI) {
        XMLCh* temp = (XMLCh*) ((DOMDocumentImpl *)getOwnerDocument())->allocate((XMLString::stringLen(baseURI) + 9)*sizeof(XMLCh));
        XMLString::fixURI(baseURI, temp);
        fBaseURI = temp;
    }
    else
        fBaseURI = 0;
}


void   DOMEntityImpl::setEntityRef(DOMEntityReference* other)
{
    fRefEntity = other;
}


DOMEntityReference*  DOMEntityImpl::getEntityRef() const
{
    return fRefEntity;
}

void  DOMEntityImpl::cloneEntityRefTree() const
{
    if (fEntityRefNodeCloned)
        return;

    // cast off const.  This method is const because it is
    //   called from a bunch of logically const methods, like
    //   getFirstChild().
    DOMEntityImpl *ncThis = (DOMEntityImpl *)this;

    //lazily clone the entityRef tree to this entity
    if (fParent.fFirstChild != 0)
        return;

    if (!fRefEntity)
        return;

    ncThis->fEntityRefNodeCloned = true;
    ncThis->fNode.setReadOnly(false, true);
    ncThis->fParent.cloneChildren(fRefEntity);
    ncThis->fNode.setReadOnly(true, true);
}

DOMNode * DOMEntityImpl::getFirstChild() const
{
    cloneEntityRefTree();
    return fParent.fFirstChild;
}

DOMNode *   DOMEntityImpl::getLastChild() const
{
    cloneEntityRefTree();
    return fParent.getLastChild();
}

DOMNodeList* DOMEntityImpl::getChildNodes() const
{
    cloneEntityRefTree();
    return this->fParent.getChildNodes();

}

bool DOMEntityImpl::hasChildNodes() const
{
    cloneEntityRefTree();
    return fParent.fFirstChild!=0;
}


void DOMEntityImpl::release()
{
    if (fNode.isOwned() && !fNode.isToBeReleased())
        throw DOMException(DOMException::INVALID_ACCESS_ERR,0);

    DOMDocumentImpl* doc = (DOMDocumentImpl*) getOwnerDocument();
    if (doc) {
        fNode.callUserDataHandlers(DOMUserDataHandler::NODE_DELETED, 0, 0);
        fParent.release();
        doc->release(this, DOMDocumentImpl::ENTITY_OBJECT);
    }
    else {
        // shouldn't reach here
        throw DOMException(DOMException::INVALID_ACCESS_ERR,0);
    }
}

//
//  Functions inherited from Node
//

           DOMNode*         DOMEntityImpl::appendChild(DOMNode *newChild)          {cloneEntityRefTree(); return fParent.appendChild (newChild); }
           DOMNamedNodeMap* DOMEntityImpl::getAttributes() const                   {return fNode.getAttributes (); }
     const XMLCh*           DOMEntityImpl::getLocalName() const                    {return fNode.getLocalName (); }
     const XMLCh*           DOMEntityImpl::getNamespaceURI() const                 {return fNode.getNamespaceURI (); }
           DOMNode*         DOMEntityImpl::getNextSibling() const                  {return fNode.getNextSibling (); }
     const XMLCh*           DOMEntityImpl::getNodeValue() const                    {return fNode.getNodeValue (); }
           DOMDocument*     DOMEntityImpl::getOwnerDocument() const                {return fParent.fOwnerDocument; }
     const XMLCh*           DOMEntityImpl::getPrefix() const                       {return fNode.getPrefix (); }
           DOMNode*         DOMEntityImpl::getParentNode() const                   {return fNode.getParentNode (); }
           DOMNode*         DOMEntityImpl::getPreviousSibling() const              {return fNode.getPreviousSibling (); }
           DOMNode*         DOMEntityImpl::insertBefore(DOMNode *newChild, DOMNode *refChild)
                                                                                   {cloneEntityRefTree(); return fParent.insertBefore (newChild, refChild); }
           void             DOMEntityImpl::normalize()                             {cloneEntityRefTree(); fParent.normalize (); }
           DOMNode*         DOMEntityImpl::removeChild(DOMNode *oldChild)          {cloneEntityRefTree(); return fParent.removeChild (oldChild); }
           DOMNode*         DOMEntityImpl::replaceChild(DOMNode *newChild, DOMNode *oldChild)
                                                                                   {cloneEntityRefTree(); return fParent.replaceChild (newChild, oldChild); }
           bool             DOMEntityImpl::isSupported(const XMLCh *feature, const XMLCh *version) const
                                                                                   {return fNode.isSupported (feature, version); }
           void             DOMEntityImpl::setPrefix(const XMLCh  *prefix)         {fNode.setPrefix(prefix); }
           bool             DOMEntityImpl::hasAttributes() const                   {return fNode.hasAttributes(); }
           bool             DOMEntityImpl::isSameNode(const DOMNode* other) const  {return fNode.isSameNode(other); }
           bool             DOMEntityImpl::isEqualNode(const DOMNode* arg) const   {cloneEntityRefTree(); return fParent.isEqualNode(arg); }
           void*            DOMEntityImpl::setUserData(const XMLCh* key, void* data, DOMUserDataHandler* handler)
                                                                                   {return fNode.setUserData(key, data, handler); }
           void*            DOMEntityImpl::getUserData(const XMLCh* key) const     {return fNode.getUserData(key); }
           short            DOMEntityImpl::compareTreePosition(const DOMNode* other) const {return fNode.compareTreePosition(other); }
           const XMLCh*     DOMEntityImpl::getTextContent() const                  {return fNode.getTextContent(); }
           void             DOMEntityImpl::setTextContent(const XMLCh* textContent){fNode.setTextContent(textContent); }
           const XMLCh*     DOMEntityImpl::lookupNamespacePrefix(const XMLCh* namespaceURI, bool useDefault) const  {return fNode.lookupNamespacePrefix(namespaceURI, useDefault); }
           bool             DOMEntityImpl::isDefaultNamespace(const XMLCh* namespaceURI) const {return fNode.isDefaultNamespace(namespaceURI); }
           const XMLCh*     DOMEntityImpl::lookupNamespaceURI(const XMLCh* prefix) const  {return fNode.lookupNamespaceURI(prefix); }
           DOMNode*         DOMEntityImpl::getInterface(const XMLCh* feature)      {return fNode.getInterface(feature); }


//Introduced in DOM Level 3
const XMLCh* DOMEntityImpl::getActualEncoding() const {
    return fActualEncoding;
}

void DOMEntityImpl::setActualEncoding(const XMLCh* actualEncoding){
    DOMDocumentImpl *doc = (DOMDocumentImpl *)this->getOwnerDocument();
    fActualEncoding = doc->cloneString(actualEncoding);
}

const XMLCh* DOMEntityImpl::getEncoding() const {
    return fEncoding;
}

void DOMEntityImpl::setEncoding(const XMLCh* encoding){
    DOMDocumentImpl *doc = (DOMDocumentImpl *)this->getOwnerDocument();
    fEncoding = doc->cloneString(encoding);
}

const XMLCh* DOMEntityImpl::getVersion() const {
    return fVersion;
}

void DOMEntityImpl::setVersion(const XMLCh* version){
    DOMDocumentImpl *doc = (DOMDocumentImpl *)this->getOwnerDocument();
    fVersion = doc->cloneString(version);
}

XERCES_CPP_NAMESPACE_END

