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

#include <xercesc/util/XMLUniDefs.hpp>
#include "DOMAttrNSImpl.hpp"
#include "DOMDocumentImpl.hpp"
#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/dom/DOMException.hpp>

#include "assert.h"

XERCES_CPP_NAMESPACE_BEGIN

DOMAttrNSImpl::DOMAttrNSImpl(DOMDocument *ownerDoc, const XMLCh *nam) :
DOMAttrImpl(ownerDoc, nam)
{
    this->fNamespaceURI=0;	//DOM Level 2
    this->fLocalName=0;       //DOM Level 2
    this->fPrefix=0;
}

//Introduced in DOM Level 2
DOMAttrNSImpl::DOMAttrNSImpl(DOMDocument *ownerDoc,
                           const XMLCh *namespaceURI,
                           const XMLCh *qualifiedName) :
DOMAttrImpl(ownerDoc, qualifiedName)
{
    setName(namespaceURI, qualifiedName);
}

DOMAttrNSImpl::DOMAttrNSImpl(const DOMAttrNSImpl &other, bool deep) :
DOMAttrImpl(other, deep)
{
    this->fNamespaceURI = other.fNamespaceURI;	//DOM Level 2
    this->fLocalName = other.fLocalName;          //DOM Level 2
    this->fPrefix = other.fPrefix;
}

DOMNode * DOMAttrNSImpl::cloneNode(bool deep) const
{
    DOMNode* newNode = new (getOwnerDocument(), DOMDocumentImpl::ATTR_NS_OBJECT) DOMAttrNSImpl(*this, deep);
    fNode.callUserDataHandlers(DOMUserDataHandler::NODE_CLONED, this, newNode);
    return newNode;
}

const XMLCh * DOMAttrNSImpl::getNamespaceURI() const
{
    return fNamespaceURI;
}

const XMLCh * DOMAttrNSImpl::getPrefix() const
{
    return fPrefix;
}

const XMLCh * DOMAttrNSImpl::getLocalName() const
{
    return fLocalName;
}

void DOMAttrNSImpl::setPrefix(const XMLCh *prefix)
{
    const XMLCh * xml = DOMNodeImpl::getXmlString();
    const XMLCh * xmlURI = DOMNodeImpl::getXmlURIString();
    const XMLCh * xmlns = DOMNodeImpl::getXmlnsString();
    const XMLCh * xmlnsURI = DOMNodeImpl::getXmlnsURIString();

    if (fNode.isReadOnly())
        throw DOMException(DOMException::NO_MODIFICATION_ALLOWED_ERR,
        0);
    if (fNamespaceURI == 0 || fNamespaceURI[0] == chNull || XMLString::equals(fLocalName, xmlns))
        throw DOMException(DOMException::NAMESPACE_ERR, 0);

    if (prefix != 0 && !((DOMDocumentImpl *)this->getOwnerDocument())->isXMLName(prefix))
        throw DOMException(DOMException::INVALID_CHARACTER_ERR,0);

    if (prefix == 0 || prefix[0] == chNull) {
        fName = fLocalName;
        fPrefix = 0;
        return;
    }

    if (XMLString::equals(prefix, xml)&&
        !XMLString::equals(fNamespaceURI, xmlURI)||
        XMLString::equals(prefix, xmlns)&&
        !XMLString::equals(fNamespaceURI, xmlnsURI))
        throw DOMException(DOMException::NAMESPACE_ERR, 0);

    if (XMLString::indexOf(prefix, chColon) != -1) {
        throw DOMException(DOMException::NAMESPACE_ERR, 0);
    }

    this-> fPrefix = ((DOMDocumentImpl *)this->getOwnerDocument())->getPooledString(prefix);

    int prefixLen = XMLString::stringLen(prefix);
    int newQualifiedNameLen = prefixLen+1+XMLString::stringLen(fLocalName);
    XMLCh* newName;
    XMLCh temp[4000];
    if (newQualifiedNameLen >= 3999)
        newName = (XMLCh*) ((DOMDocumentImpl *)this->getOwnerDocument())->getMemoryManager()->allocate
        (
            newQualifiedNameLen * sizeof(XMLCh)
        );//new XMLCh[newQualifiedNameLen];
    else
        newName = temp;

    // newName = prefix + chColon + fLocalName;
    XMLString::copyString(newName, prefix);
    newName[prefixLen] = chColon;
    XMLString::copyString(&newName[prefixLen+1], fLocalName);

    fName = ((DOMDocumentImpl *)this->getOwnerDocument())->
                                           getPooledString(newName);

    if (newQualifiedNameLen >= 3999)
        ((DOMDocumentImpl *)this->getOwnerDocument())->getMemoryManager()->deallocate(newName);//delete[] newName;

}

void DOMAttrNSImpl::release()
{
    if (fNode.isOwned() && !fNode.isToBeReleased())
        throw DOMException(DOMException::INVALID_ACCESS_ERR,0);

    DOMDocumentImpl* doc = (DOMDocumentImpl*) getOwnerDocument();
    if (doc) {
        fNode.callUserDataHandlers(DOMUserDataHandler::NODE_DELETED, 0, 0);
        fParent.release();
        doc->release(this, DOMDocumentImpl::ATTR_NS_OBJECT);
    }
    else {
        // shouldn't reach here
        throw DOMException(DOMException::INVALID_ACCESS_ERR,0);
    }
}


DOMNode* DOMAttrNSImpl::rename(const XMLCh* namespaceURI, const XMLCh* name)
{
    DOMElement* el = getOwnerElement();
    if (el)
        el->removeAttributeNode(this);

    setName(namespaceURI, name);

    if (el)
        el->setAttributeNodeNS(this);

    return this;
}

void DOMAttrNSImpl::setName(const XMLCh* namespaceURI, const XMLCh* qualifiedName)
{
    DOMDocumentImpl* ownerDoc = (DOMDocumentImpl *) getOwnerDocument();
    const XMLCh * xmlns = DOMNodeImpl::getXmlnsString();
    const XMLCh * xmlnsURI = DOMNodeImpl::getXmlnsURIString();
    this->fName = ownerDoc->getPooledString(qualifiedName);

    int index = DOMDocumentImpl::indexofQualifiedName(qualifiedName);
    if (index < 0)
        throw DOMException(DOMException::NAMESPACE_ERR, 0);

    bool xmlnsAlone = false;	//true if attribute name is "xmlns"
    if (index == 0) {	//qualifiedName contains no ':'
        if (XMLString::equals(this->fName, xmlns)) {
            if (!XMLString::equals(namespaceURI, xmlnsURI))
                throw DOMException(DOMException::NAMESPACE_ERR, 0);
            xmlnsAlone = true;
        }
        this -> fPrefix = 0;
        this -> fLocalName = this -> fName;
    } else {	//0 < index < this->name.length()-1
        XMLCh* newName;
        XMLCh temp[4000];
        if (index >= 3999)
            newName = (XMLCh*) ((DOMDocumentImpl *)this->getOwnerDocument())->getMemoryManager()->allocate
            (
                (XMLString::stringLen(qualifiedName) + 1) * sizeof(XMLCh)
            );//new XMLCh[XMLString::stringLen(qualifiedName)+1];
        else
            newName = temp;

        XMLString::copyNString(newName, fName, index);
        newName[index] = chNull;
        this-> fPrefix = ownerDoc->getPooledString(newName);
        this -> fLocalName = ownerDoc->getPooledString(fName+index+1);

        if (index >= 3999)
            ((DOMDocumentImpl *)this->getOwnerDocument())->getMemoryManager()->deallocate(newName);//delete[] newName;

        // Before we carry on, we should check if the prefix or localName are valid XMLName
        if (!((DOMDocumentImpl *)this->getOwnerDocument())->isXMLName(fPrefix) || !((DOMDocumentImpl *)this->getOwnerDocument())->isXMLName(fLocalName))
            throw DOMException(DOMException::NAMESPACE_ERR, 0);
    }

    // DOM Level 3: namespace URI is never empty string.
    const XMLCh * URI = xmlnsAlone ? xmlnsURI
        : DOMNodeImpl::mapPrefix
          (
              fPrefix,
              (!namespaceURI || !*namespaceURI) ? 0 : namespaceURI,
              DOMNode::ATTRIBUTE_NODE
          );
    this -> fNamespaceURI = (URI == 0) ? 0 : ownerDoc->getPooledString(URI);
}

XERCES_CPP_NAMESPACE_END

