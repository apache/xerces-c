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

#include "IDElementImpl.hpp"

#include "IDOM_Attr.hpp"
#include "IDOM_Document.hpp"
#include "IDOM_DOMException.hpp"

#include "IDAttrMapImpl.hpp"
#include "IDDocumentImpl.hpp"
#include "IDParentNode.hpp"
#include "IDStringPool.hpp"


#include "IDDeepNodeListImpl.hpp"
//#include "IDDocumentImpl.hpp"
#include "IDDocumentTypeImpl.hpp"
//#include "IDOM_DOMException.hpp"
//#include "IDElementDefinitionImpl.hpp"
#include "IDNamedNodeMapImpl.hpp"
//#include "IDNodeVector.hpp"


class IDOM_Attr;

IDElementImpl::IDElementImpl(IDOM_Document *ownerDoc, const XMLCh *eName)
    : fNode(ownerDoc), fParent(ownerDoc), fAttributes(0)
{
    IDDocumentImpl *docImpl = (IDDocumentImpl *)ownerDoc;
    fName = docImpl->fNamePool->getPooledString(eName);
    setupDefaultAttributes();
    if (!fAttributes)
        fAttributes = new (getOwnerDocument()) IDAttrMapImpl(this);
};


IDElementImpl::IDElementImpl(const IDElementImpl &other, bool deep)
    : fNode(other.getOwnerDocument()),
      fParent(other.getOwnerDocument()),
      fAttributes(0)
{
    fName = other.fName;
    if (deep)
        fParent.cloneChildren(&other);

    if (other.getAttributes())
    {
        fAttributes = ((IDAttrMapImpl *)other.getAttributes())->cloneAttrMap(this);
    }

    if (!fAttributes) {
        setupDefaultAttributes();
        if (!fAttributes)
            fAttributes = new (getOwnerDocument()) IDAttrMapImpl(this);
    }
};


IDElementImpl::~IDElementImpl()
{
};


IDOM_Node *IDElementImpl::cloneNode(bool deep) const
{
    return new (getOwnerDocument()) IDElementImpl(*this, deep);
    return 0;
};




const XMLCh * IDElementImpl::getNodeName() const {
    return fName;
};


short IDElementImpl::getNodeType() const {
    return IDOM_Node::ELEMENT_NODE;
};


const XMLCh * IDElementImpl::getAttribute(const XMLCh *nam) const
{
    static const XMLCh emptyString[]  = {0};
    IDOM_Node * attr=0;

    attr=fAttributes->getNamedItem(nam);

    const XMLCh *retString = emptyString;
    if (attr != 0)
        retString = attr->getNodeValue();

    return retString;
};



IDOM_Attr *IDElementImpl::getAttributeNode(const XMLCh *nam) const
{
    return  (IDOM_Attr *)fAttributes->getNamedItem(nam);
};

#include "stdio.h"

IDOM_NamedNodeMap *IDElementImpl::getAttributes() const
{
    IDElementImpl *ncThis = (IDElementImpl *)this;   // cast off const
    return ncThis->fAttributes;
};



IDOM_NodeList *IDElementImpl::getElementsByTagName(const XMLCh *tagname) const
{
    IDDocumentImpl *docImpl = (IDDocumentImpl *)getOwnerDocument();
    return docImpl->getDeepNodeList(this,tagname);
};


const XMLCh * IDElementImpl::getTagName() const
{
    return fName;
}


void IDElementImpl::removeAttribute(const XMLCh *nam)
{
    if (fNode.isReadOnly())
        throw IDOM_DOMException(
             IDOM_DOMException::NO_MODIFICATION_ALLOWED_ERR, 0);

    IDOM_Node *att = fAttributes->getNamedItem(nam);
    if (att != 0)
    {
        fAttributes->removeNamedItem(nam);
    }
};



IDOM_Attr *IDElementImpl::removeAttributeNode(IDOM_Attr *oldAttr)
{
    if (fNode.isReadOnly())
        throw IDOM_DOMException(
        IDOM_DOMException::NO_MODIFICATION_ALLOWED_ERR, 0);

    IDOM_Node *found = fAttributes->getNamedItem(oldAttr->getName());

    // If it is in fact the right object, remove it.

    if (found == oldAttr)
        fAttributes->removeNamedItem(oldAttr->getName());
    else
        throw IDOM_DOMException(IDOM_DOMException::NOT_FOUND_ERR, 0);

    return (IDOM_Attr *)found;	

};



void IDElementImpl::setAttribute(const XMLCh *nam, const XMLCh *val)
{
    if (fNode.isReadOnly())
        throw IDOM_DOMException(
        IDOM_DOMException::NO_MODIFICATION_ALLOWED_ERR, 0);

    IDOM_Attr* newAttr = getAttributeNode(nam);
    if (!newAttr)
    {
        newAttr = this->fNode.getOwnerDocument()->createAttribute(nam);
        fAttributes->setNamedItem(newAttr);
    }

    newAttr->setNodeValue(val);
};



IDOM_Attr * IDElementImpl::setAttributeNode(IDOM_Attr *newAttr)
{
    if (fNode.isReadOnly())
        throw IDOM_DOMException(
        IDOM_DOMException::NO_MODIFICATION_ALLOWED_ERR, 0);

    if (newAttr->getNodeType() != IDOM_Node::ATTRIBUTE_NODE)
        throw IDOM_DOMException(IDOM_DOMException::WRONG_DOCUMENT_ERR, 0);
        // idom_revisit.  Exception doesn't match test.

    IDOM_Attr *oldAttr =
      (IDOM_Attr *) fAttributes->getNamedItem(newAttr->getName());
    // This will throw INUSE if necessary
    fAttributes->setNamedItem(newAttr);

    return oldAttr;
};


void IDElementImpl::setNodeValue(const XMLCh *x)
{
    fNode.setNodeValue(x);
};



void IDElementImpl::setReadOnly(bool readOnl, bool deep)
{
    fNode.setReadOnly(readOnl,deep);
    fAttributes->setReadOnly(readOnl,true);
};


//Introduced in DOM Level 2
const XMLCh * IDElementImpl::getAttributeNS(const XMLCh *fNamespaceURI,
	const XMLCh *fLocalName) const
{
    IDOM_Attr * attr=
      (IDOM_Attr *)(fAttributes->getNamedItemNS(fNamespaceURI, fLocalName));
    return (attr==0) ? 0 : attr->getValue();
}


void IDElementImpl::setAttributeNS(const XMLCh *fNamespaceURI,
	const XMLCh *qualifiedName, const XMLCh *fValue)
{
    if (fNode.isReadOnly())
        throw IDOM_DOMException(
	    IDOM_DOMException::NO_MODIFICATION_ALLOWED_ERR, 0);

    IDOM_Attr *newAttr =
        this->fNode.getOwnerDocument()->createAttributeNS(fNamespaceURI, qualifiedName);
    newAttr->setNodeValue(fValue);
    fAttributes->setNamedItem(newAttr);
}


void IDElementImpl::removeAttributeNS(const XMLCh *fNamespaceURI,
	const XMLCh *fLocalName)
{
    if (fNode.isReadOnly())
        throw IDOM_DOMException(
        IDOM_DOMException::NO_MODIFICATION_ALLOWED_ERR, 0);

    IDOM_Attr *att =
        (IDOM_Attr *)fAttributes->getNamedItemNS(fNamespaceURI, fLocalName);
    // Remove it
    if (att != 0) {
        fAttributes->removeNamedItemNS(fNamespaceURI, fLocalName);
    }
}


IDOM_Attr *IDElementImpl::getAttributeNodeNS(const XMLCh *fNamespaceURI,
	const XMLCh *fLocalName) const
{
    return (IDOM_Attr *)fAttributes->getNamedItemNS(fNamespaceURI, fLocalName);
}


IDOM_Attr *IDElementImpl::setAttributeNodeNS(IDOM_Attr *newAttr)
{
    if (fNode.isReadOnly())
        throw IDOM_DOMException(
	        IDOM_DOMException::NO_MODIFICATION_ALLOWED_ERR, 0);

    if (newAttr -> getOwnerDocument() != this -> getOwnerDocument())
        throw IDOM_DOMException(IDOM_DOMException::WRONG_DOCUMENT_ERR, 0);

    IDOM_Attr *oldAttr = (IDOM_Attr *) fAttributes->getNamedItemNS(newAttr->getNamespaceURI(), newAttr->getLocalName());

    // This will throw INUSE if necessary
    fAttributes->setNamedItemNS(newAttr);

    return oldAttr;
}


IDOM_NodeList *IDElementImpl::getElementsByTagNameNS(const XMLCh *namespaceURI,
	const XMLCh *localName) const
{
    IDDocumentImpl *docImpl = (IDDocumentImpl *)getOwnerDocument();;
    return docImpl->getDeepNodeList(this, namespaceURI, localName);
}

bool IDElementImpl::hasAttributes() const
{
    return (fAttributes != 0 && fAttributes->getLength() != 0);
};


bool IDElementImpl::hasAttribute(const XMLCh *name) const
{
    return (getAttributeNode(name) != 0);
};


bool IDElementImpl::hasAttributeNS(const XMLCh *namespaceURI,
	const XMLCh *localName) const
{
    return (getAttributeNodeNS(namespaceURI, localName) != 0);
};


// DOM_NamedNodeMap UTILITIES
#ifdef idom_revisit
NamedNodeMapImpl *IDElementImpl::NNM_cloneMap(IDOM_Node *nnm_ownerNode)
{
	return (getAttributes() == 0) ? 0 : nnm_ownerNode->getAttributes()->cloneMap(nnm_ownerNode);
}

int IDElementImpl::NNM_findNamePoint(const XMLCh *nnm_name)
{
	return (getAttributes() == 0) ? -1 : getAttributes()->findNamePoint(nnm_name);
}

unsigned int IDElementImpl::NNM_getLength()
{
	return (getAttributes() == 0) ? 0 : getAttributes()->getLength();
}

IDOM_Node *IDElementImpl::NNM_getNamedItem(const XMLCh *nnm_name)
{
	return (getAttributes() == 0) ? 0 : getAttributes()->getNamedItem(nnm_name);
}

IDOM_Node *IDElementImpl::NNM_item(unsigned int nnm_index)
{
	return (getAttributes() == 0) ? 0 : getAttributes()->item(nnm_index);
}

void IDElementImpl::NNM_removeAll()
{
	if (getAttributes() != 0)
		getAttributes()->removeAll();
}

IDOM_Node *IDElementImpl::NNM_removeNamedItem(const XMLCh *nnm_name)
{
	if (getAttributes() == 0)
		throw IDOM_DOMException(IDOM_DOMException::NOT_FOUND_ERR, 0);
	else
		return getAttributes()->removeNamedItem(nnm_name);
	return 0;
}

IDOM_Node *IDElementImpl::NNM_setNamedItem(IDOM_Node *nnm_arg)
{
	return fAttributes->setNamedItem(nnm_arg);
}

void IDElementImpl::NNM_setReadOnly(bool nnm_readOnly, bool nnm_deep)
{
	if (getAttributes() != 0)
		getAttributes()->setReadOnly(nnm_readOnly, nnm_deep);
}

int IDElementImpl::NNM_findNamePoint(const XMLCh *nnm_namespaceURI, const XMLCh *nnm_localName)
{
	return (getAttributes() == 0) ? -1 : getAttributes()->findNamePoint(nnm_namespaceURI, nnm_localName);
}

IDOM_Node *IDElementImpl::NNM_getNamedItemNS(const XMLCh *nnm_namespaceURI, const XMLCh *nnm_localName)
{
	return (getAttributes() == 0) ? 0 : getAttributes()->getNamedItemNS(nnm_namespaceURI, nnm_localName);
}

IDOM_Node *IDElementImpl::NNM_setNamedItemNS(IDOM_Node *nnm_arg)
{
	return getAttributes()->setNamedItemNS(nnm_arg);
}

IDOM_Node *IDElementImpl::NNM_removeNamedItemNS(const XMLCh *nnm_namespaceURI, const XMLCh *nnm_localName)
{
	if (getAttributes() == 0)
        throw IDOM_DOMException(IDOM_DOMException::NOT_FOUND_ERR, 0);
	else
		return getAttributes()->removeNamedItemNS(nnm_namespaceURI, nnm_localName);
	return 0;
}

void IDElementImpl::NNM_setOwnerDocument(DocumentImpl *nnm_doc)
{
	if (getAttributes() != 0)
		getAttributes()->setOwnerDocument(nnm_doc);
}
#endif


// util functions for default attributes
// returns the default attribute map for this node from the owner document
IDAttrMapImpl *IDElementImpl::getDefaultAttributes()
{
	if ((fNode.fOwnerNode == 0) || (getOwnerDocument() == 0))
		return 0;

	IDOM_Document *tmpdoc = getOwnerDocument();
	if (tmpdoc->getDoctype() == 0)
		return 0;
	
	IDOM_Node *eldef = ((IDDocumentTypeImpl*)tmpdoc->getDoctype())->getElements()->getNamedItem(getNodeName());
	return (eldef == 0) ? 0 : (IDAttrMapImpl *)(eldef->getAttributes());
}

// resets all attributes for this node to their default values
void IDElementImpl::setupDefaultAttributes()
{
    if ((fNode.fOwnerNode == 0) || (getOwnerDocument() == 0) || (getOwnerDocument()->getDoctype() == 0))
        return;

    IDAttrMapImpl* defAttrs = getDefaultAttributes();

    if (defAttrs)
        fAttributes = new (getOwnerDocument()) IDAttrMapImpl(this, defAttrs);
}




//
//   Functions inherited from Node
//
           IDOM_Node          *IDElementImpl::appendChild(IDOM_Node *newChild)        {return fParent.appendChild (newChild); };
           IDOM_NodeList      *IDElementImpl::getChildNodes() const 			        {return fParent.getChildNodes (); };
           IDOM_Node          *IDElementImpl::getFirstChild() const 			        {return fParent.getFirstChild (); };
           IDOM_Node          *IDElementImpl::getLastChild() const 		            {return fParent.getLastChild (); };
     const XMLCh              *IDElementImpl::getLocalName() const                    {return fNode.getLocalName (); };
     const XMLCh              *IDElementImpl::getNamespaceURI() const                 {return fNode.getNamespaceURI (); };
           IDOM_Node          *IDElementImpl::getNextSibling() const                  {return fChild.getNextSibling (); };
     const XMLCh              *IDElementImpl::getNodeValue() const                    {return fNode.getNodeValue (); };
           IDOM_Document      *IDElementImpl::getOwnerDocument() const                {return fNode.getOwnerDocument (); };
     const XMLCh              *IDElementImpl::getPrefix() const                       {return fNode.getPrefix (); };
           IDOM_Node          *IDElementImpl::getParentNode() const                   {return fChild.getParentNode (this); };
           IDOM_Node          *IDElementImpl::getPreviousSibling() const              {return fChild.getPreviousSibling (this); };
           bool                IDElementImpl::hasChildNodes() const                   {return fParent.hasChildNodes (); };
           IDOM_Node          *IDElementImpl::insertBefore(IDOM_Node *newChild, IDOM_Node *refChild)
                                                                            {return fParent.insertBefore (newChild, refChild); };
           void                IDElementImpl::normalize()                             {fParent.normalize (); };
           IDOM_Node          *IDElementImpl::removeChild(IDOM_Node *oldChild)        {return fParent.removeChild (oldChild); };
           IDOM_Node          *IDElementImpl::replaceChild(IDOM_Node *newChild, IDOM_Node *oldChild)
                                                                            {return fParent.replaceChild (newChild, oldChild); };
           bool                IDElementImpl::isSupported(const XMLCh *feature, const XMLCh *version) const
                                                                            {return fNode.isSupported (feature, version); };
           void                IDElementImpl::setPrefix(const XMLCh  *prefix)         {fNode.setPrefix(prefix); };


