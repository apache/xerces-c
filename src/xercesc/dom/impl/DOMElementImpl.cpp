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

#include "DOMElementImpl.hpp"

#include <xercesc/dom/DOMAttr.hpp>
#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMException.hpp>

#include "DOMAttrMapImpl.hpp"
#include "DOMDocumentImpl.hpp"
#include "DOMParentNode.hpp"
#include "DOMStringPool.hpp"


#include "DOMDeepNodeListImpl.hpp"
#include "DOMDocumentTypeImpl.hpp"
#include "DOMNamedNodeMapImpl.hpp"


class DOMAttr;

DOMElementImpl::DOMElementImpl(DOMDocument *ownerDoc, const XMLCh *eName)
    : fNode(ownerDoc), fParent(ownerDoc), fAttributes(0)
{
    DOMDocumentImpl *docImpl = (DOMDocumentImpl *)ownerDoc;
    fName = docImpl->fNamePool->getPooledString(eName);
    setupDefaultAttributes();
    if (!fAttributes)
        fAttributes = new (getOwnerDocument()) DOMAttrMapImpl(this);
};


DOMElementImpl::DOMElementImpl(const DOMElementImpl &other, bool deep)
    : fNode(other.getOwnerDocument()),
      fParent(other.getOwnerDocument()),
      fAttributes(0)
{
    fName = other.fName;
    if (deep)
        fParent.cloneChildren(&other);

    if (other.getAttributes())
    {
        fAttributes = ((DOMAttrMapImpl *)other.getAttributes())->cloneAttrMap(this);
    }

    if (!fAttributes) {
        setupDefaultAttributes();
        if (!fAttributes)
            fAttributes = new (getOwnerDocument()) DOMAttrMapImpl(this);
    }
};


DOMElementImpl::~DOMElementImpl()
{
};


DOMNode *DOMElementImpl::cloneNode(bool deep) const
{
    DOMNode* newNode = new (getOwnerDocument()) DOMElementImpl(*this, deep);
    fNode.callUserDataHandlers(DOMUserDataHandler::NODE_CLONED, this, newNode);
    return newNode;
};




const XMLCh * DOMElementImpl::getNodeName() const {
    return fName;
};


short DOMElementImpl::getNodeType() const {
    return DOMNode::ELEMENT_NODE;
};


const XMLCh * DOMElementImpl::getAttribute(const XMLCh *nam) const
{
    static const XMLCh emptyString[]  = {0};
    DOMNode * attr=0;

    attr=fAttributes->getNamedItem(nam);

    const XMLCh *retString = emptyString;
    if (attr != 0)
        retString = attr->getNodeValue();

    return retString;
};



DOMAttr *DOMElementImpl::getAttributeNode(const XMLCh *nam) const
{
    return  (DOMAttr *)fAttributes->getNamedItem(nam);
};

#include "stdio.h"

DOMNamedNodeMap *DOMElementImpl::getAttributes() const
{
    DOMElementImpl *ncThis = (DOMElementImpl *)this;   // cast off const
    return ncThis->fAttributes;
};



DOMNodeList *DOMElementImpl::getElementsByTagName(const XMLCh *tagname) const
{
    DOMDocumentImpl *docImpl = (DOMDocumentImpl *)getOwnerDocument();
    return docImpl->getDeepNodeList(this,tagname);
};


const XMLCh * DOMElementImpl::getTagName() const
{
    return fName;
}


void DOMElementImpl::removeAttribute(const XMLCh *nam)
{
    if (fNode.isReadOnly())
        throw DOMException(
             DOMException::NO_MODIFICATION_ALLOWED_ERR, 0);

    DOMNode *att = fAttributes->getNamedItem(nam);
    if (att != 0)
    {
        fAttributes->removeNamedItem(nam);
    }
};



DOMAttr *DOMElementImpl::removeAttributeNode(DOMAttr *oldAttr)
{
    if (fNode.isReadOnly())
        throw DOMException(
        DOMException::NO_MODIFICATION_ALLOWED_ERR, 0);

    DOMNode *found = fAttributes->getNamedItem(oldAttr->getName());

    // If it is in fact the right object, remove it.

    if (found == oldAttr)
        fAttributes->removeNamedItem(oldAttr->getName());
    else
        throw DOMException(DOMException::NOT_FOUND_ERR, 0);

    return (DOMAttr *)found;

};



void DOMElementImpl::setAttribute(const XMLCh *nam, const XMLCh *val)
{
    if (fNode.isReadOnly())
        throw DOMException(
        DOMException::NO_MODIFICATION_ALLOWED_ERR, 0);

    DOMAttr* newAttr = getAttributeNode(nam);
    if (!newAttr)
    {
        newAttr = this->fNode.getOwnerDocument()->createAttribute(nam);
        fAttributes->setNamedItem(newAttr);
    }

    newAttr->setNodeValue(val);
};



DOMAttr * DOMElementImpl::setAttributeNode(DOMAttr *newAttr)
{
    if (fNode.isReadOnly())
        throw DOMException(
        DOMException::NO_MODIFICATION_ALLOWED_ERR, 0);

    if (newAttr->getNodeType() != DOMNode::ATTRIBUTE_NODE)
        throw DOMException(DOMException::WRONG_DOCUMENT_ERR, 0);
        // revisit.  Exception doesn't match test.

    DOMAttr *oldAttr =
      (DOMAttr *) fAttributes->getNamedItem(newAttr->getName());
    // This will throw INUSE if necessary
    fAttributes->setNamedItem(newAttr);

    return oldAttr;
};


void DOMElementImpl::setNodeValue(const XMLCh *x)
{
    fNode.setNodeValue(x);
};



void DOMElementImpl::setReadOnly(bool readOnl, bool deep)
{
    fNode.setReadOnly(readOnl,deep);
    fAttributes->setReadOnly(readOnl,true);
};


//Introduced in DOM Level 2
const XMLCh * DOMElementImpl::getAttributeNS(const XMLCh *fNamespaceURI,
    const XMLCh *fLocalName) const
{
    DOMAttr * attr=
      (DOMAttr *)(fAttributes->getNamedItemNS(fNamespaceURI, fLocalName));
    return (attr==0) ? 0 : attr->getValue();
}


void DOMElementImpl::setAttributeNS(const XMLCh *fNamespaceURI,
    const XMLCh *qualifiedName, const XMLCh *fValue)
{
    if (fNode.isReadOnly())
        throw DOMException(
        DOMException::NO_MODIFICATION_ALLOWED_ERR, 0);

    DOMAttr *newAttr =
        this->fNode.getOwnerDocument()->createAttributeNS(fNamespaceURI, qualifiedName);
    newAttr->setNodeValue(fValue);
    fAttributes->setNamedItem(newAttr);
}


void DOMElementImpl::removeAttributeNS(const XMLCh *fNamespaceURI,
    const XMLCh *fLocalName)
{
    if (fNode.isReadOnly())
        throw DOMException(
        DOMException::NO_MODIFICATION_ALLOWED_ERR, 0);

    DOMAttr *att =
        (DOMAttr *)fAttributes->getNamedItemNS(fNamespaceURI, fLocalName);
    // Remove it
    if (att != 0) {
        fAttributes->removeNamedItemNS(fNamespaceURI, fLocalName);
    }
}


DOMAttr *DOMElementImpl::getAttributeNodeNS(const XMLCh *fNamespaceURI,
    const XMLCh *fLocalName) const
{
    return (DOMAttr *)fAttributes->getNamedItemNS(fNamespaceURI, fLocalName);
}


DOMAttr *DOMElementImpl::setAttributeNodeNS(DOMAttr *newAttr)
{
    if (fNode.isReadOnly())
        throw DOMException(
            DOMException::NO_MODIFICATION_ALLOWED_ERR, 0);

    if (newAttr -> getOwnerDocument() != this -> getOwnerDocument())
        throw DOMException(DOMException::WRONG_DOCUMENT_ERR, 0);

    DOMAttr *oldAttr = (DOMAttr *) fAttributes->getNamedItemNS(newAttr->getNamespaceURI(), newAttr->getLocalName());

    // This will throw INUSE if necessary
    fAttributes->setNamedItemNS(newAttr);

    return oldAttr;
}


DOMNodeList *DOMElementImpl::getElementsByTagNameNS(const XMLCh *namespaceURI,
    const XMLCh *localName) const
{
    DOMDocumentImpl *docImpl = (DOMDocumentImpl *)getOwnerDocument();;
    return docImpl->getDeepNodeList(this, namespaceURI, localName);
}

bool DOMElementImpl::hasAttributes() const
{
    return (fAttributes != 0 && fAttributes->getLength() != 0);
};


bool DOMElementImpl::hasAttribute(const XMLCh *name) const
{
    return (getAttributeNode(name) != 0);
};


bool DOMElementImpl::hasAttributeNS(const XMLCh *namespaceURI,
    const XMLCh *localName) const
{
    return (getAttributeNodeNS(namespaceURI, localName) != 0);
};


// util functions for default attributes
// returns the default attribute map for this node from the owner document
DOMAttrMapImpl *DOMElementImpl::getDefaultAttributes()
{
    if ((fNode.fOwnerNode == 0) || (getOwnerDocument() == 0))
        return 0;

    DOMDocument *tmpdoc = getOwnerDocument();
    if (tmpdoc->getDoctype() == 0)
        return 0;

    DOMNode *eldef = ((DOMDocumentTypeImpl*)tmpdoc->getDoctype())->getElements()->getNamedItem(getNodeName());
    return (eldef == 0) ? 0 : (DOMAttrMapImpl *)(eldef->getAttributes());
}

// resets all attributes for this node to their default values
void DOMElementImpl::setupDefaultAttributes()
{
    if ((fNode.fOwnerNode == 0) || (getOwnerDocument() == 0) || (getOwnerDocument()->getDoctype() == 0))
        return;

    DOMAttrMapImpl* defAttrs = getDefaultAttributes();

    if (defAttrs)
        fAttributes = new (getOwnerDocument()) DOMAttrMapImpl(this, defAttrs);
}




//
//   Functions inherited from Node
//
           DOMNode*         DOMElementImpl::appendChild(DOMNode *newChild)          {return fParent.appendChild (newChild); };
           DOMNodeList*     DOMElementImpl::getChildNodes() const                   {return fParent.getChildNodes (); };
           DOMNode*         DOMElementImpl::getFirstChild() const                   {return fParent.getFirstChild (); };
           DOMNode*         DOMElementImpl::getLastChild() const                    {return fParent.getLastChild (); };
     const XMLCh*           DOMElementImpl::getLocalName() const                    {return fNode.getLocalName (); };
     const XMLCh*           DOMElementImpl::getNamespaceURI() const                 {return fNode.getNamespaceURI (); };
           DOMNode*         DOMElementImpl::getNextSibling() const                  {return fChild.getNextSibling (); };
     const XMLCh*           DOMElementImpl::getNodeValue() const                    {return fNode.getNodeValue (); };
           DOMDocument*     DOMElementImpl::getOwnerDocument() const                {return fNode.getOwnerDocument (); };
     const XMLCh*           DOMElementImpl::getPrefix() const                       {return fNode.getPrefix (); };
           DOMNode*         DOMElementImpl::getParentNode() const                   {return fChild.getParentNode (this); };
           DOMNode*         DOMElementImpl::getPreviousSibling() const              {return fChild.getPreviousSibling (this); };
           bool             DOMElementImpl::hasChildNodes() const                   {return fParent.hasChildNodes (); };
           DOMNode*         DOMElementImpl::insertBefore(DOMNode *newChild, DOMNode *refChild)
                                                                                    {return fParent.insertBefore (newChild, refChild); };
           void             DOMElementImpl::normalize()                             {fParent.normalize (); };
           DOMNode*         DOMElementImpl::removeChild(DOMNode *oldChild)          {return fParent.removeChild (oldChild); };
           DOMNode*         DOMElementImpl::replaceChild(DOMNode *newChild, DOMNode *oldChild)
                                                                                    {return fParent.replaceChild (newChild, oldChild); };
           bool             DOMElementImpl::isSupported(const XMLCh *feature, const XMLCh *version) const
                                                                                    {return fNode.isSupported (feature, version); };
           void             DOMElementImpl::setPrefix(const XMLCh  *prefix)         {fNode.setPrefix(prefix); };
           void*            DOMElementImpl::setUserData(const XMLCh* key, void* data, DOMUserDataHandler* handler)
                                                                                    {return fNode.setUserData(key, data, handler); };
           void*            DOMElementImpl::getUserData(const XMLCh* key) const     {return fNode.getUserData(key); };


