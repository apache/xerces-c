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

#include "DOMCDATASectionImpl.hpp"
#include "DOMNodeImpl.hpp"
#include "DOMRangeImpl.hpp"
#include "DOMDocumentImpl.hpp"
#include "DOMCasts.hpp"
#include "DOMStringPool.hpp"
#include <xercesc/dom/DOMException.hpp>
#include <xercesc/util/XMLUniDefs.hpp>

XERCES_CPP_NAMESPACE_BEGIN


DOMCDATASectionImpl::DOMCDATASectionImpl(DOMDocument *ownerDoc,
                                   const XMLCh *dat)
    : fNode(ownerDoc), fParent(ownerDoc), fCharacterData(ownerDoc, dat)
{
}


DOMCDATASectionImpl::DOMCDATASectionImpl(const DOMCDATASectionImpl &other, bool)
    : fNode(*castToNodeImpl(&other)),
    fParent(*castToParentImpl(&other)),
    fChild(*castToChildImpl(&other)),
    fCharacterData(other.fCharacterData)
{
    // revisit.  SOmething nees to make "deep" work.
}


DOMCDATASectionImpl::~DOMCDATASectionImpl()
{
}


DOMNode  *DOMCDATASectionImpl::cloneNode(bool deep) const
{
    DOMNode* newNode = new (this->getOwnerDocument(), DOMDocumentImpl::CDATA_SECTION_OBJECT) DOMCDATASectionImpl(*this, deep);
    fNode.callUserDataHandlers(DOMUserDataHandler::NODE_CLONED, this, newNode);
    return newNode;
}


const XMLCh * DOMCDATASectionImpl::getNodeName() const {
    static const XMLCh gcdata_section[] = {chPound, chLatin_c, chLatin_d, chLatin_a, chLatin_t, chLatin_a,
        chDash, chLatin_s, chLatin_e, chLatin_c, chLatin_t, chLatin_i, chLatin_o, chLatin_n, 0};
    return gcdata_section;
}


short DOMCDATASectionImpl::getNodeType() const {
    return DOMNode::CDATA_SECTION_NODE;
}


bool DOMCDATASectionImpl::isIgnorableWhitespace() const
{
    return fNode.ignorableWhitespace();
}


//
//  splitText.   revist - factor into a common function for use
//                             here and in DOMTextImpl
//
DOMText *DOMCDATASectionImpl::splitText(XMLSize_t offset)
{
    if (fNode.isReadOnly())
    {
        throw DOMException(
            DOMException::NO_MODIFICATION_ALLOWED_ERR, 0);
    }
    XMLSize_t len = fCharacterData.fDataBuf->getLen();
    if (offset > len || offset < 0)
        throw DOMException(DOMException::INDEX_SIZE_ERR, 0);

    DOMText *newText =
                getOwnerDocument()->createCDATASection(
                        this->substringData(offset, len - offset));

    DOMNode *parent = getParentNode();
    if (parent != 0)
        parent->insertBefore(newText, getNextSibling());

    fCharacterData.fDataBuf->chop(offset);

    if (this->getOwnerDocument() != 0) {
        Ranges* ranges = ((DOMDocumentImpl *)this->getOwnerDocument())->getRanges();
        if (ranges != 0) {
            XMLSize_t sz = ranges->size();
            if (sz != 0) {
                for (XMLSize_t i =0; i<sz; i++) {
                    ranges->elementAt(i)->updateSplitInfo( this, newText, offset);
                }
            }
        }
    }

    return newText;
}


bool DOMCDATASectionImpl::getIsWhitespaceInElementContent() const
{
    return isIgnorableWhitespace();
}

const XMLCh* DOMCDATASectionImpl::getWholeText() {
    throw DOMException(DOMException::NOT_SUPPORTED_ERR, 0);
    return 0;
}

DOMText* DOMCDATASectionImpl::replaceWholeText(const XMLCh*){
    throw DOMException(DOMException::NOT_SUPPORTED_ERR, 0);
    return 0;
}


void DOMCDATASectionImpl::release()
{
    if (fNode.isOwned() && !fNode.isToBeReleased())
        throw DOMException(DOMException::INVALID_ACCESS_ERR,0);

    DOMDocumentImpl* doc = (DOMDocumentImpl*) getOwnerDocument();

    if (doc) {
        fNode.callUserDataHandlers(DOMUserDataHandler::NODE_DELETED, 0, 0);
        fParent.release();
        fCharacterData.releaseBuffer();
        doc->release(this, DOMDocumentImpl::CDATA_SECTION_OBJECT);
    }
    else {
        // shouldn't reach here
        throw DOMException(DOMException::INVALID_ACCESS_ERR,0);
    }
}


//
//  Delegation stubs for other DOM_Node inherited functions.
//
           DOMNode*         DOMCDATASectionImpl::appendChild(DOMNode *newChild)          {return fParent.appendChild (newChild); }
           DOMNamedNodeMap* DOMCDATASectionImpl::getAttributes() const                   {return fNode.getAttributes (); }
           DOMNodeList*     DOMCDATASectionImpl::getChildNodes() const                   {return fParent.getChildNodes (); }
           DOMNode*         DOMCDATASectionImpl::getFirstChild() const                   {return fParent.getFirstChild (); }
           DOMNode*         DOMCDATASectionImpl::getLastChild() const                    {return fParent.getLastChild (); }
     const XMLCh*           DOMCDATASectionImpl::getLocalName() const                    {return fNode.getLocalName (); }
     const XMLCh*           DOMCDATASectionImpl::getNamespaceURI() const                 {return fNode.getNamespaceURI (); }
           DOMNode*         DOMCDATASectionImpl::getNextSibling() const                  {return fChild.getNextSibling (); }
     const XMLCh*           DOMCDATASectionImpl::getNodeValue() const                    {return fCharacterData.getNodeValue (); }
           DOMDocument*     DOMCDATASectionImpl::getOwnerDocument() const                {return fParent.fOwnerDocument; }
     const XMLCh*           DOMCDATASectionImpl::getPrefix() const                       {return fNode.getPrefix (); }
           DOMNode*         DOMCDATASectionImpl::getParentNode() const                   {return fChild.getParentNode (this); }
           DOMNode*         DOMCDATASectionImpl::getPreviousSibling() const              {return fChild.getPreviousSibling (this); }
           bool             DOMCDATASectionImpl::hasChildNodes() const                   {return fParent.hasChildNodes (); }
           DOMNode*         DOMCDATASectionImpl::insertBefore(DOMNode *newChild, DOMNode *refChild)
                                                                                         {return fParent.insertBefore (newChild, refChild); }
           void             DOMCDATASectionImpl::normalize()                             {fNode.normalize (); }
           DOMNode*         DOMCDATASectionImpl::removeChild(DOMNode *oldChild)          {return fParent.removeChild (oldChild); }
           DOMNode*         DOMCDATASectionImpl::replaceChild(DOMNode *newChild, DOMNode *oldChild)
                                                                                         {return fParent.replaceChild (newChild, oldChild); }
           bool             DOMCDATASectionImpl::isSupported(const XMLCh *feature, const XMLCh *version) const
                                                                                         {return fNode.isSupported (feature, version); }
           void             DOMCDATASectionImpl::setPrefix(const XMLCh  *prefix)         {fNode.setPrefix(prefix); }
           bool             DOMCDATASectionImpl::hasAttributes() const                   {return fNode.hasAttributes(); }
           bool             DOMCDATASectionImpl::isSameNode(const DOMNode* other) const  {return fNode.isSameNode(other); }
           bool             DOMCDATASectionImpl::isEqualNode(const DOMNode* arg) const   {return fParent.isEqualNode(arg); }
           void*            DOMCDATASectionImpl::setUserData(const XMLCh* key, void* data, DOMUserDataHandler* handler)
                                                                                         {return fNode.setUserData(key, data, handler); }
           void*            DOMCDATASectionImpl::getUserData(const XMLCh* key) const     {return fNode.getUserData(key); }
           const XMLCh*     DOMCDATASectionImpl::getBaseURI() const                      {return fNode.getBaseURI(); }
           short            DOMCDATASectionImpl::compareTreePosition(const DOMNode* other) const {return fNode.compareTreePosition(other); }
           const XMLCh*     DOMCDATASectionImpl::getTextContent() const                  {return fNode.getTextContent(); }
           void             DOMCDATASectionImpl::setTextContent(const XMLCh* textContent){fNode.setTextContent(textContent); }
           const XMLCh*     DOMCDATASectionImpl::lookupNamespacePrefix(const XMLCh* namespaceURI, bool useDefault) const  {return fNode.lookupNamespacePrefix(namespaceURI, useDefault); }
           bool             DOMCDATASectionImpl::isDefaultNamespace(const XMLCh* namespaceURI) const {return fNode.isDefaultNamespace(namespaceURI); }
           const XMLCh*     DOMCDATASectionImpl::lookupNamespaceURI(const XMLCh* prefix) const  {return fNode.lookupNamespaceURI(prefix); }
           DOMNode*         DOMCDATASectionImpl::getInterface(const XMLCh* feature)      {return fNode.getInterface(feature); }



//
//   Delegation of CharacerData functions.
//


           const XMLCh*     DOMCDATASectionImpl::getData() const                         {return fCharacterData.getData();}
           XMLSize_t        DOMCDATASectionImpl::getLength() const                       {return fCharacterData.getLength();}
           const XMLCh*     DOMCDATASectionImpl::substringData(XMLSize_t offset, XMLSize_t count) const
                                                                                         {return fCharacterData.substringData(this, offset, count);}
           void             DOMCDATASectionImpl::appendData(const XMLCh *arg)            {fCharacterData.appendData(this, arg);}
           void             DOMCDATASectionImpl::insertData(XMLSize_t offset, const  XMLCh *arg)
                                                                                         {fCharacterData.insertData(this, offset, arg);}
           void             DOMCDATASectionImpl::deleteData(XMLSize_t offset, XMLSize_t count)
                                                                                         {fCharacterData.deleteData(this, offset, count);}
           void             DOMCDATASectionImpl::replaceData(XMLSize_t offset, XMLSize_t count, const XMLCh *arg)
                                                                                         {fCharacterData.replaceData(this, offset, count, arg);}
           void             DOMCDATASectionImpl::setData(const XMLCh *data)              {fCharacterData.setData(this, data);}
           void             DOMCDATASectionImpl::setNodeValue(const XMLCh  *nodeValue)   {fCharacterData.setNodeValue (this, nodeValue); }

XERCES_CPP_NAMESPACE_END


