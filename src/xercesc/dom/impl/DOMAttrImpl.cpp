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

#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMException.hpp>

#include "DOMAttrImpl.hpp"
#include "DOMStringPool.hpp"
#include "DOMDocumentImpl.hpp"
#include "DOMCasts.hpp"
#include "DOMTypeInfoImpl.hpp"

XERCES_CPP_NAMESPACE_BEGIN

DOMAttrImpl::DOMAttrImpl(DOMDocument *ownerDoc, const XMLCh *aName)
    : fNode(ownerDoc), fParent (ownerDoc), fSchemaType(0)
{
    DOMDocumentImpl *docImpl = (DOMDocumentImpl *)ownerDoc;
    fName = docImpl->getPooledString(aName);
    fNode.isSpecified(true);
}

DOMAttrImpl::DOMAttrImpl(const DOMAttrImpl &other, bool)
    : fNode(other.fNode), fParent (other.fParent), fName(other.fName), fSchemaType(other.fSchemaType)
{    
    if (other.fNode.isSpecified())
        fNode.isSpecified(true);
    else
        fNode.isSpecified(false);

    if (other.fNode.isIdAttr())
    {
        fNode.isIdAttr(true);
        DOMDocumentImpl *doc = (DOMDocumentImpl *)this->getOwnerDocument();
        doc->getNodeIDMap()->add(this);
    }

    fParent.cloneChildren(&other);
}


DOMAttrImpl::~DOMAttrImpl() {
}


DOMNode * DOMAttrImpl::cloneNode(bool deep) const
{
    DOMNode* newNode = new (this->getOwnerDocument(), DOMDocumentImpl::ATTR_OBJECT) DOMAttrImpl(*this, deep);
    fNode.callUserDataHandlers(DOMUserDataHandler::NODE_CLONED, this, newNode);
    return newNode;
}


const XMLCh * DOMAttrImpl::getNodeName()  const{
    return fName;
}


short DOMAttrImpl::getNodeType() const {
    return DOMNode::ATTRIBUTE_NODE;
}


const XMLCh * DOMAttrImpl::getName() const {
    return fName;
}


const XMLCh * DOMAttrImpl::getNodeValue() const
{
    return getValue();
}


bool DOMAttrImpl::getSpecified() const
{
    return fNode.isSpecified();
}




const XMLCh * DOMAttrImpl::getValue() const
{
    if (fParent.fFirstChild == 0) {
        return XMLUni::fgZeroLenString; // return "";
    }

    // Simple case where attribute value is just a single text node
    DOMNode *node = castToChildImpl(fParent.fFirstChild)->nextSibling;
    if (node == 0 && fParent.fFirstChild->getNodeType() == DOMNode::TEXT_NODE) {
        return fParent.fFirstChild->getNodeValue();
    }

    //
    // Complicated case whre attribute value is a DOM tree
    //
    // According to the spec, the child nodes of the Attr node may be either
    // Text or EntityReference nodes.
    //
    // The parser will not create such thing, this is for those created by users.
    //
    // In such case, we have to visit each child to retrieve the text
    //

    XMLBuffer buf(1023, ((DOMDocumentImpl *)this->getOwnerDocument())->getMemoryManager());
    getTextValue(fParent.fFirstChild, buf);

    return (XMLCh*) ((DOMDocumentImpl *)this->getOwnerDocument())->getPooledString(buf.getRawBuffer());
}

void DOMAttrImpl::getTextValue(DOMNode* node, XMLBuffer& buf) const
{
    if (node->getNodeType() == DOMNode::TEXT_NODE)
        buf.append(node->getNodeValue());
    else if (node->getNodeType() == DOMNode::ENTITY_REFERENCE_NODE)
    {
        for (node = node->getFirstChild(); node != 0; node = castToChildImpl(node)->nextSibling)
        {
            getTextValue(node, buf);
        }
    }

    return;
}


void DOMAttrImpl::setNodeValue(const XMLCh *val)
{
    setValue(val);
}



void DOMAttrImpl::setSpecified(bool arg)
{
    fNode.isSpecified(arg);
}



void DOMAttrImpl::setValue(const XMLCh *val)
{
    if (fNode.isReadOnly())
    {
        throw DOMException (
            DOMException::NO_MODIFICATION_ALLOWED_ERR, 0);
    }

    //  If this attribute was of type ID and in the map, take it out,
    //    then put it back in with the new name.  For now, we don't worry
    //    about what happens if the new name conflicts
    //
    DOMDocumentImpl *doc = (DOMDocumentImpl *)getOwnerDocument();
    if (fNode.isIdAttr())
        doc->getNodeIDMap()->remove(this);

    DOMNode *kid;
    while ((kid = fParent.fFirstChild) != 0)         // Remove existing kids
    {
        DOMNode* node = removeChild(kid);
        if (node)
            node->release();
    }

    if (val != 0)              // Create and add the new one
        appendChild(doc->createTextNode(val));
    fNode.isSpecified(true);
    fParent.changed();

    if (fNode.isIdAttr())
        doc->getNodeIDMap()->add(this);

}




//Introduced in DOM Level 2

DOMElement *DOMAttrImpl::getOwnerElement() const
{
    // if we have an owner, ownerNode is our ownerElement, otherwise it's
    // our ownerDocument and we don't have an ownerElement
    return (DOMElement *) (fNode.isOwned() ? fNode.fOwnerNode : 0);
}


//internal use by parser only
void DOMAttrImpl::setOwnerElement(DOMElement *ownerElem)
{
    fNode.fOwnerNode = ownerElem;
    // revisit.  Is this backwards?  isOwned(true)?
    fNode.isOwned(false);
}


//For DOM Level 3

void DOMAttrImpl::release()
{
    if (fNode.isOwned() && !fNode.isToBeReleased())
        throw DOMException(DOMException::INVALID_ACCESS_ERR,0);

    DOMDocumentImpl* doc = (DOMDocumentImpl*) getOwnerDocument();
    if (doc) {
        fNode.callUserDataHandlers(DOMUserDataHandler::NODE_DELETED, 0, 0);
        fParent.release();
        doc->release(this, DOMDocumentImpl::ATTR_OBJECT);
    }
    else {
        // shouldn't reach here
        throw DOMException(DOMException::INVALID_ACCESS_ERR,0);
    }
}


bool DOMAttrImpl::isId() const {
    return fNode.isIdAttr();
}


DOMNode* DOMAttrImpl::rename(const XMLCh* namespaceURI, const XMLCh* name)
{
    DOMElement* el = getOwnerElement();
    DOMDocumentImpl* doc = (DOMDocumentImpl*) getOwnerDocument();

    if (el)
        el->removeAttributeNode(this);

    if (!namespaceURI || !*namespaceURI) {
        fName = doc->getPooledString(name);

        if (el)
            el->setAttributeNode(this);

        return this;
    }
    else {

        // create a new AttrNS
        DOMAttr* newAttr = doc->createAttributeNS(namespaceURI, name);

        // transfer the userData
        doc->transferUserData(castToNodeImpl(this), castToNodeImpl(newAttr));

        // move children to new node
        DOMNode* child = getFirstChild();
        while (child) {
            removeChild(child);
            newAttr->appendChild(child);
            child = getFirstChild();
        }

        // and fire user data NODE_RENAMED event
        castToNodeImpl(newAttr)->callUserDataHandlers(DOMUserDataHandler::NODE_RENAMED, this, newAttr);

        // reattach attr to element
        if (el)
            el->setAttributeNodeNS(newAttr);

        return newAttr;
    }
}

const DOMTypeInfo *DOMAttrImpl::getTypeInfo() const
{
    if(!fSchemaType)
        ((DOMAttrImpl *)(this))->fSchemaType = new (getOwnerDocument()) DOMTypeInfoImpl(0, 0, (DOMDocumentImpl *)getOwnerDocument());

    return fSchemaType;
}


void DOMAttrImpl::setTypeInfo(const XMLCh* typeName, const XMLCh* typeURI) 
{
    fSchemaType = new (getOwnerDocument()) DOMTypeInfoImpl(typeName, typeURI, (DOMDocumentImpl *)getOwnerDocument());
}


           DOMNode*         DOMAttrImpl::appendChild(DOMNode *newChild)          {return fParent.appendChild (newChild); }
           DOMNamedNodeMap* DOMAttrImpl::getAttributes() const                   {return fNode.getAttributes (); }
           DOMNodeList*     DOMAttrImpl::getChildNodes() const                   {return fParent.getChildNodes (); }
           DOMNode*         DOMAttrImpl::getFirstChild() const                   {return fParent.getFirstChild (); }
           DOMNode*         DOMAttrImpl::getLastChild() const                    {return fParent.getLastChild (); }
     const XMLCh*           DOMAttrImpl::getLocalName() const                    {return fNode.getLocalName (); }
     const XMLCh*           DOMAttrImpl::getNamespaceURI() const                 {return fNode.getNamespaceURI (); }
           DOMNode*         DOMAttrImpl::getNextSibling() const                  {return fNode.getNextSibling (); }
           DOMDocument*     DOMAttrImpl::getOwnerDocument() const                {return fParent.fOwnerDocument; }
     const XMLCh*           DOMAttrImpl::getPrefix() const                       {return fNode.getPrefix (); }
           DOMNode*         DOMAttrImpl::getParentNode() const                   {return fNode.getParentNode (); }
           DOMNode*         DOMAttrImpl::getPreviousSibling() const              {return fNode.getPreviousSibling (); }
           bool             DOMAttrImpl::hasChildNodes() const                   {return fParent.hasChildNodes (); }
           DOMNode*         DOMAttrImpl::insertBefore(DOMNode *newChild, DOMNode *refChild)
                                                                                 {return fParent.insertBefore (newChild, refChild); }
           void             DOMAttrImpl::normalize()                             {fParent.normalize (); }
           DOMNode*         DOMAttrImpl::removeChild(DOMNode *oldChild)          {return fParent.removeChild (oldChild); }
           DOMNode*         DOMAttrImpl::replaceChild(DOMNode *newChild, DOMNode *oldChild)
                                                                                 {return fParent.replaceChild (newChild, oldChild); }
           bool             DOMAttrImpl::isSupported(const XMLCh *feature, const XMLCh *version) const
                                                                                 {return fNode.isSupported (feature, version); }
           void             DOMAttrImpl::setPrefix(const XMLCh  *prefix)         {fNode.setPrefix(prefix); }
           bool             DOMAttrImpl::hasAttributes() const                   {return fNode.hasAttributes(); }
           bool             DOMAttrImpl::isSameNode(const DOMNode* other) const  {return fNode.isSameNode(other); }
           bool             DOMAttrImpl::isEqualNode(const DOMNode* arg) const   {return fParent.isEqualNode(arg); }
           void*            DOMAttrImpl::setUserData(const XMLCh* key, void* data, DOMUserDataHandler* handler)
                                                                                 {return fNode.setUserData(key, data, handler); }
           void*            DOMAttrImpl::getUserData(const XMLCh* key) const     {return fNode.getUserData(key); }
           const XMLCh*     DOMAttrImpl::getBaseURI() const                      {return fNode.getBaseURI(); }
           short            DOMAttrImpl::compareTreePosition(const DOMNode* other) const {return fNode.compareTreePosition(other); }
           const XMLCh*     DOMAttrImpl::getTextContent() const                  {return fNode.getTextContent(); }
           void             DOMAttrImpl::setTextContent(const XMLCh* textContent){fNode.setTextContent(textContent); }
           const XMLCh*     DOMAttrImpl::lookupNamespacePrefix(const XMLCh* namespaceURI, bool useDefault) const  {return fNode.lookupNamespacePrefix(namespaceURI, useDefault); }
           bool             DOMAttrImpl::isDefaultNamespace(const XMLCh* namespaceURI) const {return fNode.isDefaultNamespace(namespaceURI); }
           const XMLCh*     DOMAttrImpl::lookupNamespaceURI(const XMLCh* prefix) const  {return fNode.lookupNamespaceURI(prefix); }
           DOMNode*         DOMAttrImpl::getInterface(const XMLCh* feature)      {return fNode.getInterface(feature); }

XERCES_CPP_NAMESPACE_END

