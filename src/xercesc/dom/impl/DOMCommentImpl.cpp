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

#include "DOMCommentImpl.hpp"
#include "DOMCharacterDataImpl.hpp"
#include "DOMStringPool.hpp"
#include "DOMCasts.hpp"
#include "DOMDocumentImpl.hpp"
#include "DOMRangeImpl.hpp"
#include <xercesc/dom/DOMNode.hpp>
#include <xercesc/dom/DOMException.hpp>
#include <xercesc/util/XMLUniDefs.hpp>

XERCES_CPP_NAMESPACE_BEGIN


DOMCommentImpl::DOMCommentImpl(DOMDocument *ownerDoc, const XMLCh *dat)
    : fNode(ownerDoc),  fCharacterData(ownerDoc, dat)
{
    fNode.setIsLeafNode(true);
}


DOMCommentImpl::DOMCommentImpl(const DOMCommentImpl &other, bool)

    : fNode(other.fNode),
    fChild(other.fChild),
    fCharacterData(other.fCharacterData)
{
    fNode.setIsLeafNode(true);
}


DOMCommentImpl::~DOMCommentImpl() {
}



DOMNode * DOMCommentImpl::cloneNode(bool deep) const
{
    DOMNode* newNode = new (getOwnerDocument(), DOMDocumentImpl::COMMENT_OBJECT) DOMCommentImpl(*this, deep);
    fNode.callUserDataHandlers(DOMUserDataHandler::NODE_CLONED, this, newNode);
    return newNode;
}


const XMLCh * DOMCommentImpl::getNodeName() const {
    static const XMLCh gComment[] =
        {chPound, chLatin_c, chLatin_o, chLatin_m, chLatin_m, chLatin_e,chLatin_n, chLatin_t, 0};
    return gComment;
}

short DOMCommentImpl::getNodeType() const {
    return DOMNode::COMMENT_NODE;
}

void DOMCommentImpl::release()
{
    if (fNode.isOwned() && !fNode.isToBeReleased())
        throw DOMException(DOMException::INVALID_ACCESS_ERR,0);

    DOMDocumentImpl* doc = (DOMDocumentImpl*) getOwnerDocument();
    if (doc) {
        fNode.callUserDataHandlers(DOMUserDataHandler::NODE_DELETED, 0, 0);
        fCharacterData.releaseBuffer();
        doc->release(this, DOMDocumentImpl::COMMENT_OBJECT);
    }
    else {
        // shouldn't reach here
        throw DOMException(DOMException::INVALID_ACCESS_ERR,0);
    }
}


// Non standard extension for the range to work
DOMComment *DOMCommentImpl::splitText(XMLSize_t offset)
{
    if (fNode.isReadOnly())
    {
        throw DOMException(
            DOMException::NO_MODIFICATION_ALLOWED_ERR, 0);
    }
    XMLSize_t len = fCharacterData.fDataBuf->getLen();
    if (offset > len || offset < 0)
        throw DOMException(DOMException::INDEX_SIZE_ERR, 0);

    DOMComment *newText =
                getOwnerDocument()->createComment(
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


           DOMNode*         DOMCommentImpl::appendChild(DOMNode *newChild)          {return fNode.appendChild (newChild); }
           DOMNamedNodeMap* DOMCommentImpl::getAttributes() const                   {return fNode.getAttributes (); }
           DOMNodeList*     DOMCommentImpl::getChildNodes() const                   {return fNode.getChildNodes (); }
           DOMNode*         DOMCommentImpl::getFirstChild() const                   {return fNode.getFirstChild (); }
           DOMNode*         DOMCommentImpl::getLastChild() const                    {return fNode.getLastChild (); }
     const XMLCh*           DOMCommentImpl::getLocalName() const                    {return fNode.getLocalName (); }
     const XMLCh*           DOMCommentImpl::getNamespaceURI() const                 {return fNode.getNamespaceURI (); }
           DOMNode*         DOMCommentImpl::getNextSibling() const                  {return fChild.getNextSibling (); }
     const XMLCh*           DOMCommentImpl::getNodeValue() const                    {return fCharacterData.getNodeValue (); }
           DOMDocument*     DOMCommentImpl::getOwnerDocument() const                {return fNode.getOwnerDocument (); }
     const XMLCh*           DOMCommentImpl::getPrefix() const                       {return fNode.getPrefix (); }
           DOMNode*         DOMCommentImpl::getParentNode() const                   {return fChild.getParentNode (this); }
           DOMNode*         DOMCommentImpl::getPreviousSibling() const              {return fChild.getPreviousSibling (this); }
           bool             DOMCommentImpl::hasChildNodes() const                   {return fNode.hasChildNodes (); }
           DOMNode*         DOMCommentImpl::insertBefore(DOMNode *newChild, DOMNode *refChild)
                                                                                    {return fNode.insertBefore (newChild, refChild); }
           void             DOMCommentImpl::normalize()                             {fNode.normalize (); }
           DOMNode*         DOMCommentImpl::removeChild(DOMNode *oldChild)          {return fNode.removeChild (oldChild); }
           DOMNode*         DOMCommentImpl::replaceChild(DOMNode *newChild, DOMNode *oldChild)
                                                                                    {return fNode.replaceChild (newChild, oldChild); }
           bool             DOMCommentImpl::isSupported(const XMLCh *feature, const XMLCh *version) const
                                                                                    {return fNode.isSupported (feature, version); }
           void             DOMCommentImpl::setPrefix(const XMLCh  *prefix)         {fNode.setPrefix(prefix); }
           bool             DOMCommentImpl::hasAttributes() const                   {return fNode.hasAttributes(); }
           bool             DOMCommentImpl::isSameNode(const DOMNode* other) const  {return fNode.isSameNode(other); }
           bool             DOMCommentImpl::isEqualNode(const DOMNode* arg) const   {return fNode.isEqualNode(arg); }
           void*            DOMCommentImpl::setUserData(const XMLCh* key, void* data, DOMUserDataHandler* handler)
                                                                                    {return fNode.setUserData(key, data, handler); }
           void*            DOMCommentImpl::getUserData(const XMLCh* key) const     {return fNode.getUserData(key); }
           const XMLCh*     DOMCommentImpl::getBaseURI() const                      {return fNode.getBaseURI(); }
           short            DOMCommentImpl::compareTreePosition(const DOMNode* other) const {return fNode.compareTreePosition(other); }
           const XMLCh*     DOMCommentImpl::getTextContent() const                  {return fNode.getTextContent(); }
           void             DOMCommentImpl::setTextContent(const XMLCh* textContent){fNode.setTextContent(textContent); }
           const XMLCh*     DOMCommentImpl::lookupNamespacePrefix(const XMLCh* namespaceURI, bool useDefault) const  {return fNode.lookupNamespacePrefix(namespaceURI, useDefault); }
           bool             DOMCommentImpl::isDefaultNamespace(const XMLCh* namespaceURI) const {return fNode.isDefaultNamespace(namespaceURI); }
           const XMLCh*     DOMCommentImpl::lookupNamespaceURI(const XMLCh* prefix) const  {return fNode.lookupNamespaceURI(prefix); }
           DOMNode*         DOMCommentImpl::getInterface(const XMLCh* feature)      {return fNode.getInterface(feature); }



//
//   Delegation of CharacerData functions.
//


           const XMLCh*     DOMCommentImpl::getData() const                         {return fCharacterData.getData();}
           XMLSize_t        DOMCommentImpl::getLength() const                       {return fCharacterData.getLength();}
           const XMLCh*     DOMCommentImpl::substringData(XMLSize_t offset, XMLSize_t count) const
                                                                                    {return fCharacterData.substringData(this, offset, count);}
           void             DOMCommentImpl::appendData(const XMLCh *arg)            {fCharacterData.appendData(this, arg);}
           void             DOMCommentImpl::insertData(XMLSize_t offset, const  XMLCh *arg)
                                                                                    {fCharacterData.insertData(this, offset, arg);}
           void             DOMCommentImpl::deleteData(XMLSize_t offset, XMLSize_t count)
                                                                                    {fCharacterData.deleteData(this, offset, count);}
           void             DOMCommentImpl::replaceData(XMLSize_t offset, XMLSize_t count, const XMLCh *arg)
                                                                                    {fCharacterData.replaceData(this, offset, count, arg);}
           void             DOMCommentImpl::setData(const XMLCh *data)              {fCharacterData.setData(this, data);}
           void             DOMCommentImpl::setNodeValue(const XMLCh  *nodeValue)   {fCharacterData.setNodeValue (this, nodeValue); }

XERCES_CPP_NAMESPACE_END

