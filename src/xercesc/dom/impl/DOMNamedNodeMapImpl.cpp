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
    this->fOwnerNode=ownerNod;
    this->fNodes = 0;
}



DOMNamedNodeMapImpl::~DOMNamedNodeMapImpl()
{
}

bool DOMNamedNodeMapImpl::readOnly() {
    return castToNodeImpl(fOwnerNode)->isReadOnly();
}

DOMNamedNodeMapImpl *DOMNamedNodeMapImpl::cloneMap(DOMNode *ownerNod)
{
    DOMDocumentImpl *doc = (DOMDocumentImpl *)(castToNodeImpl(ownerNod)->getOwnerDocument());
    DOMNamedNodeMapImpl *newmap = new (doc) DOMNamedNodeMapImpl(ownerNod);
	
    if (fNodes != 0)
    {
        newmap->fNodes = new (doc) DOMNodeVector(doc, fNodes->size());
        for (XMLSize_t i = 0; i < fNodes->size(); ++i)
        {
            DOMNode *n = fNodes->elementAt(i)->cloneNode(true);
			castToNodeImpl(n)->isSpecified(castToNodeImpl(fNodes->elementAt(i))->isSpecified());
            castToNodeImpl(n)->fOwnerNode = ownerNod;
            castToNodeImpl(n)->isOwned(true);
            newmap->fNodes->addElement(n);
        }
    }

    return newmap;
}


//
//  removeAll - This function removes all elements from a named node map.
//              It is called from the destructors for Elements and DocumentTypes,
//              to remove the contents when the owning Element or DocType goes
//              away.  The empty NamedNodeMap may persist if the user code
//              has a reference to it.
//
//              AH Revist - the empty map should be made read-only, since
//              adding it was logically part of the [Element, DocumentType]
//              that has been deleted, and adding anything new to it would
//              be meaningless, and almost certainly an error.
//
void DOMNamedNodeMapImpl::removeAll()
{
    if (fNodes)
    {

        for (int i=fNodes->size()-1; i>=0; i--)
        {
            DOMNode *n = fNodes->elementAt(i);
            castToNodeImpl(n)->fOwnerNode = fOwnerNode->getOwnerDocument();
            castToNodeImpl(n)->isOwned(false);
        }
        // We have no way to delete fNodes.  Leave it around; we can re-use
        //  it if the owner node ever adds new attributes (or whatevers)
    }
}



int DOMNamedNodeMapImpl::findNamePoint(const XMLCh *name) const
{

    // Binary search
    int i=0;
    if(fNodes!=0)
    {
        int first=0,last=fNodes->size()-1;

        while(first<=last)
        {
            i=(first+last)/2;
            int test = XMLString::compareString(name, fNodes->elementAt(i)->getNodeName());
            if(test==0)
                return i; // Name found
            else if(test<0)
                last=i-1;
            else
                first=i+1;
        }
        if(first>i) i=first;
    }
    /********************
    // Linear search
    int i = 0;
    if (fNodes != 0)
    for (i = 0; i < fNodes.size(); ++i)
    {
    int test = name.compareTo(((NodeImpl *) (fNodes.elementAt(i))).getNodeName());
    if (test == 0)
    return i;
    else
    if (test < 0)
    {
    break; // Found insertpoint
    }
    }

    *******************/
    return -1 - i; // not-found has to be encoded.
}





XMLSize_t DOMNamedNodeMapImpl::getLength() const
{
    return (fNodes != 0) ? fNodes->size() : 0;
}



DOMNode * DOMNamedNodeMapImpl::getNamedItem(const XMLCh *name) const
{
    int i=findNamePoint(name);
    return (i<0) ? 0 : fNodes->elementAt(i);
}



DOMNode * DOMNamedNodeMapImpl::item(XMLSize_t index) const
{
    return (fNodes != 0 && index < fNodes->size()) ?
        fNodes->elementAt(index) : 0;
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
            DOMException::NO_MODIFICATION_ALLOWED_ERR, 0);
    int i=findNamePoint(name);
    DOMNode *n = 0;

    if(i<0)
        throw DOMException(DOMException::NOT_FOUND_ERR, 0);

    n = fNodes->elementAt(i);
    fNodes->removeElementAt(i);
    castToNodeImpl(n)->fOwnerNode = fOwnerNode->getOwnerDocument();
    castToNodeImpl(n)->isOwned(false);
    return n;
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
        throw DOMException(DOMException::WRONG_DOCUMENT_ERR,0);
    if (this->readOnly())
        throw DOMException(DOMException::NO_MODIFICATION_ALLOWED_ERR, 0);
    if ((arg->getNodeType() == DOMNode::ATTRIBUTE_NODE) && argImpl->isOwned() && (argImpl->fOwnerNode != fOwnerNode))
        throw DOMException(DOMException::INUSE_ATTRIBUTE_ERR,0);

    argImpl->fOwnerNode = fOwnerNode;
    argImpl->isOwned(true);
    int i=findNamePoint(arg->getNodeName());
    DOMNode * previous=0;
    if(i>=0)
    {
        previous = fNodes->elementAt(i);
        fNodes->setElementAt(arg,i);
    }
    else
    {
        i=-1-i; // Insert point (may be end of list)
        if(0==fNodes)
        {
            fNodes=new (doc) DOMNodeVector(doc);
        }
        fNodes->insertElementAt(arg,i);
    }
    if (previous != 0) {
        castToNodeImpl(previous)->fOwnerNode = fOwnerNode->getOwnerDocument();
        castToNodeImpl(previous)->isOwned(false);
    }

    return previous;
}


void DOMNamedNodeMapImpl::setReadOnly(bool readOnl, bool deep)
{
    // this->fReadOnly=readOnl;
    if(deep && fNodes!=0)
    {
        int sz = fNodes->size();
        for (int i=0; i<sz; ++i) {
            castToNodeImpl(fNodes->elementAt(i))->setReadOnly(readOnl, deep);
        }
    }
}


//Introduced in DOM Level 2

int DOMNamedNodeMapImpl::findNamePoint(const XMLCh *namespaceURI,
	const XMLCh *localName) const
{
    if (fNodes == 0)
	return -1;
    // This is a linear search through the same fNodes Vector.
    // The Vector is sorted on the DOM Level 1 nodename.
    // The DOM Level 2 NS keys are namespaceURI and Localname,
    // so we must linear search thru it.
    // In addition, to get this to work with fNodes without any namespace
    // (namespaceURI and localNames are both 0) we then use the nodeName
    // as a secondary key.
    int i, len = fNodes -> size();
    for (i = 0; i < len; ++i) {
        DOMNode *node = fNodes -> elementAt(i);
        const XMLCh * nNamespaceURI = node->getNamespaceURI();
        const XMLCh * nLocalName = node->getLocalName();
        if (!XMLString::equals(nNamespaceURI, namespaceURI))    //URI not match
            continue;
        else {
            if (XMLString::equals(localName, nLocalName)
                ||
                (nLocalName == 0 && XMLString::equals(localName, node->getNodeName())))
                return i;
        }
    }
    return -1;	//not found
}


DOMNode *DOMNamedNodeMapImpl::getNamedItemNS(const XMLCh *namespaceURI,
	const XMLCh *localName) const
{
    int i = findNamePoint(namespaceURI, localName);
    return i < 0 ? 0 : fNodes -> elementAt(i);
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
        throw DOMException(DOMException::WRONG_DOCUMENT_ERR,0);
    if (this->readOnly())
        throw DOMException(DOMException::NO_MODIFICATION_ALLOWED_ERR, 0);
    if (argImpl->isOwned())
        throw DOMException(DOMException::INUSE_ATTRIBUTE_ERR,0);

    argImpl->fOwnerNode = fOwnerNode;
    argImpl->isOwned(true);
    int i=findNamePoint(arg->getNamespaceURI(), arg->getLocalName());
    DOMNode *previous=0;
    if(i>=0) {
        previous = fNodes->elementAt(i);
        fNodes->setElementAt(arg,i);
    } else {
        i=findNamePoint(arg->getNodeName()); // Insert point (may be end of list)
        if (i<0)
          i = -1 - i;
        if(0==fNodes)
            fNodes=new (doc) DOMNodeVector(doc);
        fNodes->insertElementAt(arg,i);
    }
    if (previous != 0) {
        castToNodeImpl(previous)->fOwnerNode = fOwnerNode->getOwnerDocument();
        castToNodeImpl(previous)->isOwned(false);
    }

    return previous;
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
        DOMException::NO_MODIFICATION_ALLOWED_ERR, 0);
    int i = findNamePoint(namespaceURI, localName);
    if (i < 0)
        throw DOMException(DOMException::NOT_FOUND_ERR, 0);

    DOMNode * n = fNodes -> elementAt(i);
    fNodes -> removeElementAt(i);	//remove n from nodes
    castToNodeImpl(n)->fOwnerNode = fOwnerNode->getOwnerDocument();
    castToNodeImpl(n)->isOwned(false);
    return n;
}



void DOMNamedNodeMapImpl::cloneContent(const DOMNamedNodeMapImpl *srcmap)
{
    if ((srcmap != 0) && (srcmap->fNodes != 0))
    {
        if (fNodes != 0)
            fNodes->reset();
        else
        {
            XMLSize_t size = srcmap->fNodes->size();
            if(size > 0) {
                DOMDocument *doc = fOwnerNode->getOwnerDocument();
                fNodes = new (doc) DOMNodeVector(doc, size);
            }
        }

        for (XMLSize_t i = 0; i < srcmap->fNodes->size(); i++)
        {
            DOMNode *n = srcmap->fNodes->elementAt(i);
            DOMNode *clone = n->cloneNode(true);
            castToNodeImpl(clone)->isSpecified(castToNodeImpl(n)->isSpecified());
            castToNodeImpl(clone)->fOwnerNode = fOwnerNode;
            castToNodeImpl(clone)->isOwned(true);
            fNodes->addElement(clone);
        }
    }
}


// remove the name using index
// avoid calling findNamePoint again if the index is already known
DOMNode * DOMNamedNodeMapImpl::removeNamedItemAt(XMLSize_t index)
{
    if (this->readOnly())
        throw DOMException(
            DOMException::NO_MODIFICATION_ALLOWED_ERR, 0);

    DOMNode *n = item(index);
    if(!n)
        throw DOMException(DOMException::NOT_FOUND_ERR, 0);

    fNodes->removeElementAt(index);
    castToNodeImpl(n)->fOwnerNode = fOwnerNode->getOwnerDocument();
    castToNodeImpl(n)->isOwned(false);
    return n;
}

XERCES_CPP_NAMESPACE_END

