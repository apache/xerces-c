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
#include "DOMElementNSImpl.hpp"
#include "DOMDocumentImpl.hpp"
#include <xercesc/dom/DOMException.hpp>
#include <xercesc/util/XMLUri.hpp>
#include <xercesc/util/OutOfMemoryException.hpp>

XERCES_CPP_NAMESPACE_BEGIN


DOMElementNSImpl::DOMElementNSImpl(DOMDocument *ownerDoc, const XMLCh *nam) :
    DOMElementImpl(ownerDoc, nam)
{
    this->fNamespaceURI=0;	  //DOM Level 2
    this->fLocalName=0;       //DOM Level 2
    this->fPrefix=0;
}

//Introduced in DOM Level 2
DOMElementNSImpl::DOMElementNSImpl(DOMDocument *ownerDoc,
                             const XMLCh *namespaceURI,
                             const XMLCh *qualifiedName) :
    DOMElementImpl(ownerDoc, qualifiedName)
{
    setName(namespaceURI, qualifiedName);
}

DOMElementNSImpl::DOMElementNSImpl(const DOMElementNSImpl &other, bool deep) :
    DOMElementImpl(other, deep)
{
    this->fNamespaceURI = other.fNamespaceURI;	        //DOM Level 2
    this->fLocalName = other.fLocalName;                //DOM Level 2
    this->fPrefix = other.fPrefix;
}

DOMNode * DOMElementNSImpl::cloneNode(bool deep) const {
    DOMNode* newNode = new (getOwnerDocument(), DOMDocumentImpl::ELEMENT_NS_OBJECT) DOMElementNSImpl(*this, deep);
    fNode.callUserDataHandlers(DOMUserDataHandler::NODE_CLONED, this, newNode);
    return newNode;
}

const XMLCh * DOMElementNSImpl::getNamespaceURI() const
{
    return fNamespaceURI;
}

const XMLCh * DOMElementNSImpl::getPrefix() const
{
    return fPrefix;
}


const XMLCh * DOMElementNSImpl::getLocalName() const
{
    return fLocalName;
}

const XMLCh* DOMElementNSImpl::getBaseURI() const
{
    const XMLCh* baseURI = (fNode.fOwnerNode)->getBaseURI();
    if (fAttributes) {
        const XMLCh baseString[] =
        {
            chLatin_b, chLatin_a, chLatin_s, chLatin_e, chNull
        };
        DOMNode* attrNode = fAttributes->getNamedItemNS(DOMNodeImpl::getXmlURIString(), baseString);
        if (attrNode) {
            const XMLCh* uri =  attrNode->getNodeValue();
            if (uri && *uri) {// attribute value is always empty string
                try {
                    XMLUri temp(baseURI, ((DOMDocumentImpl *)this->getOwnerDocument())->getMemoryManager());
                    XMLUri temp2(&temp, uri, ((DOMDocumentImpl *)this->getOwnerDocument())->getMemoryManager());
                    uri = ((DOMDocumentImpl *)this->getOwnerDocument())->cloneString(temp2.getUriText());
                }
                catch(const OutOfMemoryException&)
                {
                    throw;
                }
                catch (...){
                    // REVISIT: what should happen in this case?
                    return 0;
                }
                return uri;
            }
        }
    }
    return baseURI;
}


void DOMElementNSImpl::setPrefix(const XMLCh *prefix)
{
    const XMLCh * xml      = DOMNodeImpl::getXmlString();
    const XMLCh * xmlURI   = DOMNodeImpl::getXmlURIString();

    if (fNode.isReadOnly())
        throw DOMException(DOMException::NO_MODIFICATION_ALLOWED_ERR,
                               0);
    if(prefix != 0 && !((DOMDocumentImpl *)this->getOwnerDocument())->isXMLName(prefix))
        throw DOMException(DOMException::INVALID_CHARACTER_ERR,0);

    if (fNamespaceURI == 0 || fNamespaceURI[0] == chNull)
        throw DOMException(DOMException::NAMESPACE_ERR, 0);

    if (prefix == 0 || *prefix == 0) {
        fName = fLocalName;
        return;
    }

    if (XMLString::equals(prefix, xml) &&
        !XMLString::equals(fNamespaceURI, xmlURI))
        throw DOMException(DOMException::NAMESPACE_ERR, 0);


    if (XMLString::indexOf(prefix, chColon) != -1) {
        throw DOMException(DOMException::NAMESPACE_ERR, 0);
    }

    this-> fPrefix = ((DOMDocumentImpl *)this->getOwnerDocument())->getPooledString(prefix);

    int prefixLen = XMLString::stringLen(prefix);
    int newQualifiedNameLen = prefixLen+1+XMLString::stringLen(fLocalName);

    XMLCh *newName;
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

void DOMElementNSImpl::release()
{
    if (fNode.isOwned() && !fNode.isToBeReleased())
        throw DOMException(DOMException::INVALID_ACCESS_ERR,0);

    DOMDocumentImpl* doc = (DOMDocumentImpl*) getOwnerDocument();
    if (doc) {
        fNode.callUserDataHandlers(DOMUserDataHandler::NODE_DELETED, 0, 0);
        fParent.release();
        doc->release(this, DOMDocumentImpl::ELEMENT_NS_OBJECT);
    }
    else {
        // shouldn't reach here
        throw DOMException(DOMException::INVALID_ACCESS_ERR,0);
    }
}

DOMNode* DOMElementNSImpl::rename(const XMLCh* namespaceURI, const XMLCh* name)
{
    setName(namespaceURI, name);
    fAttributes->reconcileDefaultAttributes(getDefaultAttributes());
    return this;
}

void DOMElementNSImpl::setName(const XMLCh *namespaceURI,
                               const XMLCh *qualifiedName)
{
    DOMDocumentImpl* ownerDoc = (DOMDocumentImpl *) getOwnerDocument();
    this->fName = ownerDoc->getPooledString(qualifiedName);

    int index = DOMDocumentImpl::indexofQualifiedName(qualifiedName);
    if (index < 0)
        throw DOMException(DOMException::NAMESPACE_ERR, 0);

    if (index == 0) {	//qualifiedName contains no ':'
        this -> fPrefix = 0;
        this -> fLocalName = this -> fName;
    } else {	//0 < index < this->name.length()-1
        XMLCh* newName;
        XMLCh temp[4000];
        if (index >= 3999)
            newName = (XMLCh*) ownerDoc->getMemoryManager()->allocate
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
            ownerDoc->getMemoryManager()->deallocate(newName);//delete[] newName;

        // Before we carry on, we should check if the prefix or localName are valid XMLName
        if (!((DOMDocumentImpl *)this->getOwnerDocument())->isXMLName(fPrefix) || !((DOMDocumentImpl *)this->getOwnerDocument())->isXMLName(fLocalName))
            throw DOMException(DOMException::NAMESPACE_ERR, 0);
    }

    // DOM Level 3: namespace URI is never empty string.
    const XMLCh * URI = DOMNodeImpl::mapPrefix
        (
            fPrefix,
            (!namespaceURI || !*namespaceURI) ? 0 : namespaceURI,
            DOMNode::ELEMENT_NODE
        );
    this -> fNamespaceURI = (URI == 0) ? 0 : ownerDoc->getPooledString(URI);
}

XERCES_CPP_NAMESPACE_END

