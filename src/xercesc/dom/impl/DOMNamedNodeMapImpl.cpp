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


#include <xercesc/dom/DOMAttr.hpp>
#include <xercesc/dom/DOMException.hpp>
#include <xercesc/framework/XMLBuffer.hpp>
#include <xercesc/util/XMLUniDefs.hpp>

#include "DOMNodeVector.hpp"
#include "DOMNamedNodeMapImpl.hpp"
#include "DOMCasts.hpp"
#include "DOMDocumentImpl.hpp"
#include "DOMNodeImpl.hpp"

XERCES_CPP_NAMESPACE_BEGIN

DOMNamedNodeMapImpl::DOMNamedNodeMapImpl(DOMNode *ownerNod)
{
    fOwnerNode=ownerNod;
    memset(fBuckets,0,MAP_SIZE*sizeof(DOMNodeVector*));
}

DOMNamedNodeMapImpl::~DOMNamedNodeMapImpl()
{
}

bool DOMNamedNodeMapImpl::readOnly() 
{
    return castToNodeImpl(fOwnerNode)->isReadOnly();
}

DOMNamedNodeMapImpl *DOMNamedNodeMapImpl::cloneMap(DOMNode *ownerNod)
{
    DOMDocumentImpl *doc = (DOMDocumentImpl *)(castToNodeImpl(ownerNod)->getOwnerDocument());
    DOMNamedNodeMapImpl *newmap = new (doc) DOMNamedNodeMapImpl(ownerNod);
	
    for(int index=0;index<MAP_SIZE;index++)
        if (fBuckets[index] != 0) {
            XMLSize_t size=fBuckets[index]->size();
            newmap->fBuckets[index] = new (doc) DOMNodeVector(doc, size);
            for (XMLSize_t i = 0; i < size; ++i) {
                DOMNode *s = fBuckets[index]->elementAt(i);
                DOMNode *n = s->cloneNode(true);
			    castToNodeImpl(n)->isSpecified(castToNodeImpl(s)->isSpecified());
                castToNodeImpl(n)->fOwnerNode = ownerNod;
                castToNodeImpl(n)->isOwned(true);
                newmap->fBuckets[index]->addElement(n);
            }
        }

    return newmap;
}


XMLSize_t DOMNamedNodeMapImpl::getLength() const
{
    XMLSize_t count=0;
    for(int index=0;index<MAP_SIZE;index++)
        count+=(fBuckets[index]==0?0:fBuckets[index]->size());
    return count;
}

DOMNode * DOMNamedNodeMapImpl::item(XMLSize_t index) const
{
    XMLSize_t count=0;
    for(XMLSize_t i=0;i<MAP_SIZE;i++) {
        if(fBuckets[i]==0)
            continue;
        XMLSize_t thisBucket=fBuckets[i]->size();
        if(index>=count && index<(count+thisBucket))
            return fBuckets[i]->elementAt(index-count);
        count+=thisBucket;
    }
    return NULL;
}


DOMNode * DOMNamedNodeMapImpl::getNamedItem(const XMLCh *name) const
{
    unsigned int hash=XMLString::hash(name,MAP_SIZE);
    if(fBuckets[hash]==0)
        return 0;

    int i = 0;
    int size = fBuckets[hash]->size();
    for (i = 0; i < size; ++i) {
        DOMNode *n=fBuckets[hash]->elementAt(i);
        if(XMLString::equals(name,n->getNodeName()))
            return n;
    }

    return 0;
}


//
// removeNamedItem() - Remove the named item, and return it.
//                      The caller can release the
//                      returned item if it's not used
//                      we can't do it here because the caller would
//                      never see the returned node.
//
DOMNode * DOMNamedNodeMapImpl::removeNamedItem(const XMLCh *name)
{
    if (this->readOnly())
        throw DOMException(
            DOMException::NO_MODIFICATION_ALLOWED_ERR, 0, GetDOMNamedNodeMapMemoryManager);
    
    unsigned int hash=XMLString::hash(name,MAP_SIZE);
    if(fBuckets[hash]==0)
        throw DOMException(DOMException::NOT_FOUND_ERR, 0, GetDOMNamedNodeMapMemoryManager);

    int i = 0;
    int size = fBuckets[hash]->size();
    for (i = 0; i < size; ++i) {
        DOMNode *n=fBuckets[hash]->elementAt(i);
        if(XMLString::equals(name,n->getNodeName())) {
            fBuckets[hash]->removeElementAt(i);
            castToNodeImpl(n)->fOwnerNode = fOwnerNode->getOwnerDocument();
            castToNodeImpl(n)->isOwned(false);
            return n;
        }
    }

    throw DOMException(DOMException::NOT_FOUND_ERR, 0, GetDOMNamedNodeMapMemoryManager);
    return 0;
}



//
// setNamedItem()  Put the item into the NamedNodeList by name.
//                  If an item with the same name already was
//                  in the list, replace it.  Return the old
//                  item, if there was one.
//                  Caller is responsible for arranging for
//                  deletion of the old item if its ref count is
//                  zero.
//
DOMNode * DOMNamedNodeMapImpl::setNamedItem(DOMNode * arg)
{
    DOMDocument *doc = fOwnerNode->getOwnerDocument();
    DOMNodeImpl *argImpl = castToNodeImpl(arg);
    if(argImpl->getOwnerDocument() != doc)
        throw DOMException(DOMException::WRONG_DOCUMENT_ERR,0, GetDOMNamedNodeMapMemoryManager);
    if (this->readOnly())
        throw DOMException(DOMException::NO_MODIFICATION_ALLOWED_ERR, 0, GetDOMNamedNodeMapMemoryManager);
    if ((arg->getNodeType() == DOMNode::ATTRIBUTE_NODE) && argImpl->isOwned() && (argImpl->fOwnerNode != fOwnerNode))
        throw DOMException(DOMException::INUSE_ATTRIBUTE_ERR,0, GetDOMNamedNodeMapMemoryManager);

    argImpl->fOwnerNode = fOwnerNode;
    argImpl->isOwned(true);

    const XMLCh* name=arg->getNodeName();
    unsigned int hash=XMLString::hash(name,MAP_SIZE);
    if(fBuckets[hash]==0)
        fBuckets[hash] = new (doc) DOMNodeVector(doc, 3);

    int i = 0;
    int size = fBuckets[hash]->size();
    for (i = 0; i < size; ++i) {
        DOMNode *n=fBuckets[hash]->elementAt(i);
        if(XMLString::equals(name,n->getNodeName())) {
            fBuckets[hash]->setElementAt(arg,i);
            castToNodeImpl(n)->fOwnerNode = fOwnerNode->getOwnerDocument();
            castToNodeImpl(n)->isOwned(false);
            return n;
        }
    }
    fBuckets[hash]->addElement(arg);
    return 0;
}


void DOMNamedNodeMapImpl::setReadOnly(bool readOnl, bool deep)
{
    // this->fReadOnly=readOnl;
    if(deep) {
        for (int index = 0; index < MAP_SIZE; index++) {
            if(fBuckets[index]==0)
                continue;
            int sz = fBuckets[index]->size();
            for (int i=0; i<sz; ++i)
                castToNodeImpl(fBuckets[index]->elementAt(i))->setReadOnly(readOnl, deep);
        }
    }
}


//Introduced in DOM Level 2

DOMNode *DOMNamedNodeMapImpl::getNamedItemNS(const XMLCh *namespaceURI, const XMLCh *localName) const
{
    // the map is indexed using the full name of nodes; to search given a namespace and a local name
    // we have to do a linear search
    for (int index = 0; index < MAP_SIZE; index++) {
        if(fBuckets[index]==0)
            continue;

        int i = 0;
        int size = fBuckets[index]->size();
        for (i = 0; i < size; ++i) {
            DOMNode *n=fBuckets[index]->elementAt(i);
            const XMLCh * nNamespaceURI = n->getNamespaceURI();
            const XMLCh * nLocalName = n->getLocalName();
            if (!XMLString::equals(nNamespaceURI, namespaceURI))    //URI not match
                continue;
            else {
                if (XMLString::equals(localName, nLocalName)
                    ||
                    (nLocalName == 0 && XMLString::equals(localName, n->getNodeName())))
                    return n;
            }
        }
    }
    return 0;
}


//
// setNamedItemNS()  Put the item into the NamedNodeList by name.
//                  If an item with the same name already was
//                  in the list, replace it.  Return the old
//                  item, if there was one.
//                  Caller is responsible for arranging for
//                  deletion of the old item if its ref count is
//                  zero.
//
DOMNode * DOMNamedNodeMapImpl::setNamedItemNS(DOMNode *arg)
{
    DOMDocument *doc = fOwnerNode->getOwnerDocument();
    DOMNodeImpl *argImpl = castToNodeImpl(arg);
    if (argImpl->getOwnerDocument() != doc)
        throw DOMException(DOMException::WRONG_DOCUMENT_ERR,0, GetDOMNamedNodeMapMemoryManager);
    if (this->readOnly())
        throw DOMException(DOMException::NO_MODIFICATION_ALLOWED_ERR, 0, GetDOMNamedNodeMapMemoryManager);
    if (argImpl->isOwned())
        throw DOMException(DOMException::INUSE_ATTRIBUTE_ERR,0, GetDOMNamedNodeMapMemoryManager);

    argImpl->fOwnerNode = fOwnerNode;
    argImpl->isOwned(true);

    const XMLCh* namespaceURI=arg->getNamespaceURI();
    const XMLCh* localName=arg->getLocalName();
    // the map is indexed using the full name of nodes; to search given a namespace and a local name
    // we have to do a linear search
    for (int index = 0; index < MAP_SIZE; index++) {
        if(fBuckets[index]==0)
            continue;

        int i = 0;
        int size = fBuckets[index]->size();
        for (i = 0; i < size; ++i) {
            DOMNode *n=fBuckets[index]->elementAt(i);
            const XMLCh * nNamespaceURI = n->getNamespaceURI();
            const XMLCh * nLocalName = n->getLocalName();
            if (!XMLString::equals(nNamespaceURI, namespaceURI))    //URI not match
                continue;
            else {
                if (XMLString::equals(localName, nLocalName)
                    ||
                    (nLocalName == 0 && XMLString::equals(localName, n->getNodeName()))) {
                    fBuckets[index]->setElementAt(arg,i);
                    castToNodeImpl(n)->fOwnerNode = fOwnerNode->getOwnerDocument();
                    castToNodeImpl(n)->isOwned(false);
                    return n;
                }
            }
        }
    }
    // if not found, add it using the full name as key
    return setNamedItem(arg);
}


// removeNamedItemNS() - Remove the named item, and return it.
//                      The caller can release the
//                      returned item if it's not used
//                      we can't do it here because the caller would
//                      never see the returned node.
DOMNode *DOMNamedNodeMapImpl::removeNamedItemNS(const XMLCh *namespaceURI,
                                                 const XMLCh *localName)
{
    if (this->readOnly())
        throw DOMException(
        DOMException::NO_MODIFICATION_ALLOWED_ERR, 0, GetDOMNamedNodeMapMemoryManager);

    // the map is indexed using the full name of nodes; to search given a namespace and a local name
    // we have to do a linear search
    for (int index = 0; index < MAP_SIZE; index++) {
        if(fBuckets[index]==0)
            continue;

        int i = 0;
        int size = fBuckets[index]->size();
        for (i = 0; i < size; ++i) {
            DOMNode *n=fBuckets[index]->elementAt(i);
            const XMLCh * nNamespaceURI = n->getNamespaceURI();
            const XMLCh * nLocalName = n->getLocalName();
            if (!XMLString::equals(nNamespaceURI, namespaceURI))    //URI not match
                continue;
            else {
                if (XMLString::equals(localName, nLocalName)
                    ||
                    (nLocalName == 0 && XMLString::equals(localName, n->getNodeName()))) {
                    fBuckets[index]->removeElementAt(i);
                    castToNodeImpl(n)->fOwnerNode = fOwnerNode->getOwnerDocument();
                    castToNodeImpl(n)->isOwned(false);
                    return n;
                }
            }
        }
    }
    throw DOMException(DOMException::NOT_FOUND_ERR, 0, GetDOMNamedNodeMapMemoryManager);
    return 0;
}

XERCES_CPP_NAMESPACE_END

