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

#include "IDOM_Document.hpp"
#include "IDDocumentImpl.hpp"
#include "IDOM_DocumentType.hpp"
#include "IDOM_DOMException.hpp"
#include "IDOM_DOMImplementation.hpp"
#include "IDOM_NamedNodeMap.hpp"
#include "IDOM_Node.hpp"

#include "IDCasts.hpp"
#include "IDDocumentTypeImpl.hpp"
#include "IDAttrImpl.hpp"
#include "IDAttrNSImpl.hpp"
#include "IDCDATASectionImpl.hpp"
#include "IDCommentImpl.hpp"
#include "IDDeepNodeListImpl.hpp"
#include "IDDocumentFragmentImpl.hpp"
#include "IDElementImpl.hpp"
#include "IDElementNSImpl.hpp"
#include "IDEntityImpl.hpp"
#include "IDEntityReferenceImpl.hpp"
#include "IDNamedNodeMapImpl.hpp"
#include "IDNotationImpl.hpp"
#include "IDProcessingInstructionImpl.hpp"
#include "IDTextImpl.hpp"

#include "IDStringPool.hpp"
#include "IDTreeWalkerImpl.hpp"
#include "IDNodeIteratorImpl.hpp"
#include "IDNodeIDMap.hpp"
#include "IDRangeImpl.hpp"
#include <xercesc/internal/XMLReader.hpp>
#include <xercesc/util/HashPtr.hpp>


//idom_revisit.  These can go away once all of the include files above are really there.
class IDOM_TreeWalker;
class IDOM_NodeFilter;


//
//   Constructors.   Warning - be very careful with the ordering of initialization
//                             of the heap.  Ordering depends on the order of declaration
//                             in the .hpp file, not on the order of initializers here
//                             in the constructor.  The heap declaration can not be
//                             first - fNode and fParent must be first for the casting
//                             functions in IDCasts to work correctly.  This means that
//                             fNode and fParent constructors used here can not
//                             allocate.
//
IDDocumentImpl::IDDocumentImpl()
    : fNode(this),
      fParent(this),
      fCurrentBlock(0),
      fFreePtr(0),
      fFreeBytesRemaining(0),
      fDocType(0),
      fDocElement(0),
      fNamePool(0),
      fIterators(0L),
      fTreeWalkers(0L),
      fNodeIDMap(0),
      fUserData(0),
      fRanges(0),
      fChanges(0),
      fNodeListPool(0)
{
    fNamePool    = new (this) IDStringPool(257, this);
};


//DOM Level 2
IDDocumentImpl::IDDocumentImpl(const XMLCh *fNamespaceURI,
                               const XMLCh *qualifiedName,
                               IDOM_DocumentType *doctype)
    : fNode(this),
      fParent(this),
      fCurrentBlock(0),
      fFreePtr(0),
      fFreeBytesRemaining(0),
      fDocType(0),
      fDocElement(0),
      fNamePool(0),
      fIterators(0L),
      fTreeWalkers(0L),
      fNodeIDMap(0),
      fUserData(0),
      fRanges(0),
      fChanges(0),
      fNodeListPool(0)
{
    fNamePool    = new (this) IDStringPool(257, this);
    try {
        setDocumentType(doctype);
        appendChild(createElementNS(fNamespaceURI, qualifiedName));  //root element
    }
    catch (...) {
        this->deleteHeap();
        throw;
    }
}

void IDDocumentImpl::setDocumentType(IDOM_DocumentType *doctype)
{
	if (!doctype)
		return;

    // New doctypes can be created either with the factory methods on DOMImplementation, in
    //   which case ownerDocument will be 0, or with methods on DocumentImpl, in which case
    //   ownerDocument will be set, but the DocType won't yet be a child of the document.
    if (doctype->getOwnerDocument() != 0 && doctype->getOwnerDocument() != this)
        throw IDOM_DOMException(	//one doctype can belong to only one IDDocumentImpl
        IDOM_DOMException::WRONG_DOCUMENT_ERR, 0);

    IDDocumentTypeImpl* doctypeImpl = (IDDocumentTypeImpl*) doctype;
    doctypeImpl->setOwnerDocument(this);

    // The doctype can not have any Entities or Notations yet, because they can not
    //   be created except through factory methods on a document.
				
    // idom_revisit.  What if this doctype is already a child of the document?
	appendChild(doctype);

}

IDDocumentImpl::~IDDocumentImpl()
{
    //  Clean up the fNodeListPool
    if (fNodeListPool)
        fNodeListPool->cleanup();

    //  Clean up the RefVector
    if (fIterators)
        fIterators->cleanup();

    if (fTreeWalkers)
        fTreeWalkers->cleanup();

    if (fRanges)
        fRanges->cleanup();

    //  Delete the heap for this document.  This uncerimoniously yanks the storage
    //      out from under all of the nodes in the document.  Destructors are NOT called.
    this->deleteHeap();
};


IDOM_Node *IDDocumentImpl::cloneNode(bool deep) const {

    // clone the node itself
    // idom_revisit  -- this doesn't look right.  What about entities, doctype?

    // Note:  the cloned document node goes on the system heap.  All other
    //   nodes added to the new document will go on that document's heap,
    //   but we need to construct the document first, before its heap exists.
    IDDocumentImpl *newdoc = new IDDocumentImpl();

    // then the children by _importing_ them
    if (deep)
        for (IDOM_Node *n = this->getFirstChild(); n != 0; n = n->getNextSibling()) {
            newdoc->appendChild(newdoc->importNode(n, true));
	}
    return newdoc;
};


const XMLCh * IDDocumentImpl::getNodeName() const {
    static const XMLCh nam[] =  // "#document"
        {chPound, chLatin_d, chLatin_o, chLatin_c, chLatin_u, chLatin_m, chLatin_e, chLatin_n, chLatin_t, 0};
    return nam;
}


short IDDocumentImpl::getNodeType() const {
    return IDOM_Node::DOCUMENT_NODE;
};


// even though ownerDocument refers to this in this implementation
// the DOM Level 2 spec says it must be 0, so make it appear so
IDOM_Document * IDDocumentImpl::getOwnerDocument() const {
    return 0;
}


IDOM_Attr *IDDocumentImpl::createAttribute(const XMLCh *nam)
{
    if(!isXMLName(nam))
        throw IDOM_DOMException(IDOM_DOMException::INVALID_CHARACTER_ERR,0);
    return new (this) IDAttrImpl(this,nam);
};



IDOM_CDATASection *IDDocumentImpl::createCDATASection(const XMLCh *data) {
    return new (this) IDCDATASectionImpl(this,data);
};



IDOM_Comment *IDDocumentImpl::createComment(const XMLCh *data)
{
    return new (this) IDCommentImpl(this, data);
};



IDOM_DocumentFragment *IDDocumentImpl::createDocumentFragment()
{
    return new (this) IDDocumentFragmentImpl(this);
};



IDOM_DocumentType *IDDocumentImpl::createDocumentType(const XMLCh *nam)
{
    if (!isXMLName(nam))
        throw IDOM_DOMException(
        IDOM_DOMException::INVALID_CHARACTER_ERR, 0);

    return new (this) IDDocumentTypeImpl(this, nam);
};



IDOM_DocumentType *
    IDDocumentImpl::createDocumentType(const XMLCh *qualifiedName,
                                     const XMLCh *publicId,
                                     const XMLCh *systemId)
{
    if (!isXMLName(qualifiedName))
        throw IDOM_DOMException(
        IDOM_DOMException::INVALID_CHARACTER_ERR, 0);

    return new (this) IDDocumentTypeImpl(this, qualifiedName, publicId, systemId);
};



IDOM_Element *IDDocumentImpl::createElement(const XMLCh *tagName)
{
    if(!isXMLName(tagName))
        throw IDOM_DOMException(IDOM_DOMException::INVALID_CHARACTER_ERR,0);

    return new (this) IDElementImpl(this,tagName);
};


IDOM_Element *IDDocumentImpl::createElementNoCheck(const XMLCh *tagName)
{
    return new (this) IDElementImpl(this, tagName);
};




IDOM_Entity *IDDocumentImpl::createEntity(const XMLCh *nam)
{
    if (!isXMLName(nam))
        throw IDOM_DOMException(
        IDOM_DOMException::INVALID_CHARACTER_ERR, 0);

    return new (this) IDEntityImpl(this, nam);
};



IDOM_EntityReference *IDDocumentImpl::createEntityReference(const XMLCh *nam)
{
    if (!isXMLName(nam))
        throw IDOM_DOMException(
        IDOM_DOMException::INVALID_CHARACTER_ERR, 0);

    return new (this) IDEntityReferenceImpl(this, nam);
};



IDOM_Notation *IDDocumentImpl::createNotation(const XMLCh *nam)
{
    if (!isXMLName(nam))
        throw IDOM_DOMException(
        IDOM_DOMException::INVALID_CHARACTER_ERR, 0);

    return new (this) IDNotationImpl(this, nam);
};



IDOM_ProcessingInstruction *IDDocumentImpl::createProcessingInstruction(
                                          const XMLCh *target, const XMLCh *data)
{
    if(!isXMLName(target))
        throw IDOM_DOMException(IDOM_DOMException::INVALID_CHARACTER_ERR,0);
    return new (this) IDProcessingInstructionImpl(this,target,data);
};




IDOM_Text *IDDocumentImpl::createTextNode(const XMLCh *data)
{
    return new (this) IDTextImpl(this,data);
};


IDOM_NodeIterator* IDDocumentImpl::createNodeIterator (
          IDOM_Node *root, unsigned long whatToShow, IDOM_NodeFilter* filter, bool entityReferenceExpansion)
{
		// Create the node iterator implementation object.
		//	Add it to the vector of fIterators that must be synchronized when a node is deleted.
		//	The vector of fIterators is kept in the "owner document" if there is one. If there isn't one, I assume that root is the
		//	owner document.

    IDNodeIteratorImpl* iter = new (this) IDNodeIteratorImpl(root, whatToShow, filter, entityReferenceExpansion);
    IDOM_Document* doc = root->getOwnerDocument();
    IDDocumentImpl* impl;

    if (doc != 0) {
        impl = (IDDocumentImpl *) doc;
    }
    else
        impl = (IDDocumentImpl *) root;

    if (impl->fIterators == 0L) {
        impl->fIterators = new (this) NodeIterators(1, false);
        impl->fIterators->addElement(iter);
    }

    return iter;
}


IDOM_TreeWalker* IDDocumentImpl::createTreeWalker (IDOM_Node *root, unsigned long whatToShow, IDOM_NodeFilter* filter, bool entityReferenceExpansion)
{
    // See notes for createNodeIterator...

    IDTreeWalkerImpl* twi = new (this) IDTreeWalkerImpl(root, whatToShow, filter, entityReferenceExpansion);
    IDOM_Document* doc = root->getOwnerDocument();
    IDDocumentImpl* impl;

    if ( doc != 0) {
        impl = (IDDocumentImpl *) doc;
    }
    else
        impl = (IDDocumentImpl *) root;

    if (impl->fTreeWalkers == 0L) {
        impl->fTreeWalkers = new (this) TreeWalkers(1, false);
        impl->fTreeWalkers->addElement(twi);
    }

    return twi;
}




IDOM_DocumentType *IDDocumentImpl::getDoctype() const
{
    return fDocType;
};



IDOM_Element *IDDocumentImpl::getDocumentElement() const
{
    return fDocElement;
};



IDOM_NodeList *IDDocumentImpl::getElementsByTagName(const XMLCh *tagname) const
{
    // cast off the const of this because we will update the fNodeListPool
    return ((IDDocumentImpl*)this)->getDeepNodeList(this,tagname);
};


IDOM_DOMImplementation   *IDDocumentImpl::getImplementation() const {
    return IDOM_DOMImplementation::getImplementation();
}


IDOM_Node *IDDocumentImpl::insertBefore(IDOM_Node *newChild, IDOM_Node *refChild)
{
    // Only one such child permitted
    if(
        (newChild->getNodeType() == IDOM_Node::ELEMENT_NODE  && fDocElement!=0)
        ||
        (newChild->getNodeType() == IDOM_Node::DOCUMENT_TYPE_NODE  && fDocType!=0)
        )
        throw IDOM_DOMException(IDOM_DOMException::HIERARCHY_REQUEST_ERR,0);

    fParent.insertBefore(newChild,refChild);

    // If insert succeeded, cache the kid appropriately
    if(newChild->getNodeType() == IDOM_Node::ELEMENT_NODE)
        fDocElement=(IDOM_Element *)newChild;
    else if(newChild->getNodeType() == IDOM_Node::DOCUMENT_TYPE_NODE)
        fDocType=(IDOM_DocumentType *)newChild;

    return newChild;
};



bool IDDocumentImpl::isXMLName(const XMLCh *s)
{
    // idom_revist.  This function probably already exists in the scanner.
    if (!XMLReader::isFirstNameChar(s[0]))
        return false;

    const XMLCh *p;
    for (p=s+1; *p!=0; p++)
    {
        if (!XMLReader::isNameChar(*p))
            return false;
    }
    return true;
};




IDOM_Node *IDDocumentImpl::removeChild(IDOM_Node *oldChild)
{
    fParent.removeChild(oldChild);

    // If remove succeeded, un-cache the kid appropriately
    if(oldChild->getNodeType() == IDOM_Node::ELEMENT_NODE)
        fDocElement=0;
    else if(oldChild->getNodeType() == IDOM_Node::DOCUMENT_TYPE_NODE)
        fDocType=0;

    return oldChild;
};



void IDDocumentImpl::setNodeValue(const XMLCh *x)
{
    fNode.setNodeValue(x);
};




//Introduced in DOM Level 2

IDOM_Node *IDDocumentImpl::importNode(IDOM_Node *source, bool deep)
{
    IDOM_Node *newnode=0;

    switch (source->getNodeType())
    {
    case IDOM_Node::ELEMENT_NODE :
        {
            IDOM_Element *newelement;
            if (source->getLocalName() == 0)
                newelement = createElement(source->getNodeName());
            else
                newelement = createElementNS(source->getNamespaceURI(),
                source->getNodeName());
            IDOM_NamedNodeMap *srcattr=source->getAttributes();
            if(srcattr!=0)
                for(unsigned int i=0;i<srcattr->getLength();++i)
                {
                    IDOM_Attr *attr = (IDOM_Attr *) srcattr->item(i);
                    if (attr -> getSpecified())	{ // not a default attribute
                        IDOM_Attr *nattr = (IDOM_Attr *) importNode(attr, true);
                        if (attr -> getLocalName() == 0)
                            newelement->setAttributeNode(nattr);
                        else
                            newelement->setAttributeNodeNS(nattr);
                    }
                }
                newnode=newelement;
        }
        break;
    case IDOM_Node::ATTRIBUTE_NODE :
        if (source->getLocalName() == 0)
            newnode = createAttribute(source->getNodeName());
        else
            newnode = createAttributeNS(source->getNamespaceURI(),
            source->getNodeName());
        deep = true;
        // Kids carry value
        break;
    case IDOM_Node::TEXT_NODE :
        newnode = createTextNode(source->getNodeValue());
        break;
    case IDOM_Node::CDATA_SECTION_NODE :
        newnode = createCDATASection(source->getNodeValue());
        break;
    case IDOM_Node::ENTITY_REFERENCE_NODE :
        newnode = createEntityReference(source->getNodeName());
        castToNodeImpl(newnode) -> isReadOnly(false); //allow deep import temporarily
        break;
    case IDOM_Node::ENTITY_NODE :
        {
            IDOM_Entity *srcentity=(IDOM_Entity *)source;
            IDEntityImpl *newentity = (IDEntityImpl *)createEntity(source->getNodeName());
            newentity->setPublicId(srcentity->getPublicId());
            newentity->setSystemId(srcentity->getSystemId());
            newentity->setNotationName(srcentity->getNotationName());
            // Kids carry additional value
            newnode=newentity;
            castToNodeImpl(newentity)->isReadOnly(false);// allow deep import temporarily
        }
        break;
    case IDOM_Node::PROCESSING_INSTRUCTION_NODE :
        newnode = createProcessingInstruction(source->getNodeName(),
            source->getNodeValue());
        break;
    case IDOM_Node::COMMENT_NODE :
        newnode = createComment(source->getNodeValue());
        break;
    case IDOM_Node::DOCUMENT_TYPE_NODE :
        {
            IDOM_DocumentType *srcdoctype = (IDOM_DocumentType *)source;
            IDOM_DocumentType *newdoctype = (IDOM_DocumentType *)
                createDocumentType(srcdoctype->getNodeName(),
                srcdoctype->getPublicId(),
                srcdoctype->getSystemId());
            // Values are on NamedNodeMaps
            IDOM_NamedNodeMap *smap = srcdoctype->getEntities();
            IDOM_NamedNodeMap *tmap = newdoctype->getEntities();
            if(smap != 0) {
                for(unsigned int i = 0; i < smap->getLength(); i++) {
                    tmap->setNamedItem(importNode(smap->item(i), true));
                }
            }
            smap = srcdoctype->getNotations();
            tmap = newdoctype->getNotations();
            if (smap != 0) {
                for(unsigned int i = 0; i < smap->getLength(); i++) {
                    tmap->setNamedItem(importNode(smap->item(i), true));
                }
            }
            // NOTE: At this time, the DOM definition of DocumentType
            // doesn't cover Elements and their Attributes. domimpl's
            // extentions in that area will not be preserved, even if
            // copying from domimpl to domimpl. We could special-case
            // that here. Arguably we should. Consider. ?????
            newnode = newdoctype;
        }
        break;
    case IDOM_Node::DOCUMENT_FRAGMENT_NODE :
        newnode = createDocumentFragment();
        // No name, kids carry value
        break;
    case IDOM_Node::NOTATION_NODE :
        {
            IDOM_Notation *srcnotation=(IDOM_Notation *)source;
            IDNotationImpl *newnotation = (IDNotationImpl *)createNotation(source->getNodeName());
            newnotation->setPublicId(srcnotation->getPublicId());
            newnotation->setSystemId(srcnotation->getSystemId());
            // Kids carry additional value
            newnode=newnotation;
            // No name, no value
            break;
        }

    case IDOM_Node::DOCUMENT_NODE : // Document can't be child of Document
    default:                       // Unknown node type
        throw IDOM_DOMException(IDOM_DOMException::NOT_SUPPORTED_ERR, 0);
    }

    // If deep, replicate and attach the kids.
    if (deep)
        for (IDOM_Node *srckid = source->getFirstChild();
        srckid != 0;
        srckid = srckid->getNextSibling()) {
            newnode->appendChild(importNode(srckid, true));
        }
        if (newnode->getNodeType() == IDOM_Node::ENTITY_REFERENCE_NODE
            || newnode->getNodeType() == IDOM_Node::ENTITY_REFERENCE_NODE)
            castToNodeImpl(newnode)->isReadOnly(true);

        return newnode;
}


IDOM_Element *IDDocumentImpl::createElementNS(const XMLCh *fNamespaceURI,
	const XMLCh *qualifiedName)
{
    if(!isXMLName(qualifiedName))
        throw IDOM_DOMException(IDOM_DOMException::INVALID_CHARACTER_ERR,0);
    //XMLCh * pooledTagName = this->fNamePool->getPooledString(qualifiedName);
    return new (this) IDElementNSImpl(this, fNamespaceURI, qualifiedName);
}


IDOM_Attr *IDDocumentImpl::createAttributeNS(const XMLCh *fNamespaceURI,
	const XMLCh *qualifiedName)
{
    if(!isXMLName(qualifiedName))
        throw IDOM_DOMException(IDOM_DOMException::INVALID_CHARACTER_ERR,0);
    return new (this) IDAttrNSImpl(this, fNamespaceURI, qualifiedName);
}


IDOM_NodeList *IDDocumentImpl::getElementsByTagNameNS(const XMLCh *fNamespaceURI,
	const XMLCh *fLocalName)  const
{
    // cast off the const of this because we will update the fNodeListPool
    return ((IDDocumentImpl*)this)->getDeepNodeList(this, fNamespaceURI, fLocalName);
}


IDOM_Element *IDDocumentImpl::getElementById(const XMLCh *elementId) const
{
    if (fNodeIDMap == 0)
        return 0;

    IDOM_Attr *theAttr = fNodeIDMap->find(elementId);
    if (theAttr == 0)
	    return 0;

    return theAttr->getOwnerElement();
}


//Return the index > 0 of ':' in the given qualified name qName="prefix:localName".
//Return 0 if there is no ':', or -1 if qName is malformed such as ":abcd" or "abcd:".
int IDDocumentImpl::indexofQualifiedName(const XMLCh * qName)
{
    int qNameLen = XMLString::stringLen(qName);
    int index = -1, count = 0;
    for (int i = 0; i < qNameLen; ++i) {
        if (qName[i] == chColon) {
            index = i;
            ++count;	//number of ':' found
        }
    }

    if (qNameLen == 0 || count > 1 || index == 0 || index == qNameLen-1)
        return -1;
    return count == 0 ? 0 : index;
}



IDOM_Range* IDDocumentImpl::createRange()
{

    IDRangeImpl* range = new (this) IDRangeImpl(this);

    if (fRanges == 0L) {
        fRanges = new (this) Ranges(1, false);
    }
    fRanges->addElement(range);
    return range;
}

Ranges* IDDocumentImpl::getRanges() const
{
    return fRanges;
}

void IDDocumentImpl::removeRange(IDRangeImpl* range)
{
    if (fRanges != 0) {
        unsigned int sz = fRanges->size();
        if (sz !=0) {
            for (unsigned int i =0; i<sz; i++) {
                if (fRanges->elementAt(i) == range) {
                    fRanges->removeElementAt(i);
                    break;
                }
            }
        }
    }
}

/** Uses the kidOK lookup table to check whether the proposed
    tree structure is legal.

    ????? It feels like there must be a more efficient solution,
    but for the life of me I can't think what it would be.
*/
bool IDDocumentImpl::isKidOK(IDOM_Node *parent, IDOM_Node *child)
{
      static int kidOK[14];

      if (kidOK[IDOM_Node::ATTRIBUTE_NODE] == 0)
      {
          kidOK[IDOM_Node::DOCUMENT_NODE] =
              1 << IDOM_Node::ELEMENT_NODE |
              1 << IDOM_Node::PROCESSING_INSTRUCTION_NODE |
              1 << IDOM_Node::COMMENT_NODE |
              1 << IDOM_Node::DOCUMENT_TYPE_NODE |
              1 << IDOM_Node::XML_DECL_NODE;

          kidOK[IDOM_Node::DOCUMENT_FRAGMENT_NODE] =
              kidOK[IDOM_Node::ENTITY_NODE] =
              kidOK[IDOM_Node::ENTITY_REFERENCE_NODE] =
              kidOK[IDOM_Node::ELEMENT_NODE] =
              1 << IDOM_Node::ELEMENT_NODE |
              1 << IDOM_Node::PROCESSING_INSTRUCTION_NODE |
              1 << IDOM_Node::COMMENT_NODE |
              1 << IDOM_Node::TEXT_NODE |
              1 << IDOM_Node::CDATA_SECTION_NODE |
              1 << IDOM_Node::ENTITY_REFERENCE_NODE |
              1 << IDOM_Node::XML_DECL_NODE;

          kidOK[IDOM_Node::ATTRIBUTE_NODE] =
              1 << IDOM_Node::TEXT_NODE |
              1 << IDOM_Node::ENTITY_REFERENCE_NODE;

          kidOK[IDOM_Node::PROCESSING_INSTRUCTION_NODE] =
              kidOK[IDOM_Node::COMMENT_NODE] =
              kidOK[IDOM_Node::TEXT_NODE] =
              kidOK[IDOM_Node::CDATA_SECTION_NODE] =
              kidOK[IDOM_Node::NOTATION_NODE] =
              0;
      };
      int p=parent->getNodeType();
      int ch = child->getNodeType();
      return (kidOK[p] & 1<<ch) != 0;
}

void IDDocumentImpl::setUserData(IDOM_Node* n, void* data)
{
	if (!fUserData && data)
		fUserData = new (this) RefHashTableOf<void>(29, false, new (this) HashPtr());
	if (!data && fUserData)
		fUserData->removeKey((void*)n);
	else
		fUserData->put((void*)n,data);
}

void* IDDocumentImpl::getUserData(const IDOM_Node* n) const
{
	if (fUserData)
		return fUserData->get((void*)n);
	else
		return 0;
}

void* IDDocumentImpl::getUserData() const
{
	return (fNode.hasUserData()) ? getUserData(this) : 0;
}

void IDDocumentImpl::setUserData(void* val)
{
	setUserData(this, val);
	if (val)
		fNode.hasUserData(true);
	else
		fNode.hasUserData(false);
};




void            IDDocumentImpl::changed()
{
    fChanges++;
}


int             IDDocumentImpl::changes() const{
    return fChanges;
};



//
//    Delegation for functions inherited from IDOM_Node
//
           IDOM_Node          *IDDocumentImpl::appendChild(IDOM_Node *newChild)        {return insertBefore(newChild, 0); };
           IDOM_NamedNodeMap  *IDDocumentImpl::getAttributes() const 			        {return fNode.getAttributes (); };
           IDOM_NodeList      *IDDocumentImpl::getChildNodes() const 			        {return fParent.getChildNodes (); };
           IDOM_Node          *IDDocumentImpl::getFirstChild() const 			        {return fParent.getFirstChild (); };
           IDOM_Node          *IDDocumentImpl::getLastChild() const 		            {return fParent.getLastChild (); };
     const XMLCh              *IDDocumentImpl::getLocalName() const                    {return fNode.getLocalName (); };
     const XMLCh              *IDDocumentImpl::getNamespaceURI() const                 {return fNode.getNamespaceURI (); };
           IDOM_Node          *IDDocumentImpl::getNextSibling() const                  {return fNode.getNextSibling (); };
     const XMLCh              *IDDocumentImpl::getNodeValue() const                    {return fNode.getNodeValue (); };
     const XMLCh              *IDDocumentImpl::getPrefix() const                       {return fNode.getPrefix (); };
           IDOM_Node          *IDDocumentImpl::getParentNode() const                   {return fNode.getParentNode (); };
           IDOM_Node          *IDDocumentImpl::getPreviousSibling() const              {return fNode.getPreviousSibling (); };
           bool                IDDocumentImpl::hasChildNodes() const                   {return fParent.hasChildNodes (); };
           void                IDDocumentImpl::normalize()                             {fNode.normalize (); };
           IDOM_Node          *IDDocumentImpl::replaceChild(IDOM_Node *newChild, IDOM_Node *oldChild)
                                                                            {return fParent.replaceChild (newChild, oldChild); };
           bool                IDDocumentImpl::isSupported(const XMLCh *feature, const XMLCh *version) const
                                                                            {return fNode.isSupported (feature, version); };
           void                IDDocumentImpl::setPrefix(const XMLCh  *prefix)         {fNode.setPrefix(prefix); };
           bool                IDDocumentImpl::hasAttributes() const                   {return fNode.hasAttributes(); };


//-----------------------------------------------------------------------
//
//  Per Document Heap and Heap Helper functions
//
//        idom_revisit - this stuff should be a class of its own, rather than
//                       just lying around naked in DocumentImpl.
//
//-----------------------------------------------------------------------

XMLCh * IDDocumentImpl::cloneString(const XMLCh *src)
{
    size_t   len = XMLString::stringLen(src);
    len = (len + 1) * sizeof(XMLCh);
    len = (len % 4) + len;
    XMLCh *newStr = (XMLCh *)this->allocate(len);
    XMLString::copyString(newStr, src);
    return newStr;
}


const XMLCh *  IDDocumentImpl::getPooledString(const XMLCh *src)
{
    return this->fNamePool->getPooledString(src);
}

static const int kHeapAllocSize = 0x10000;    // The chunk size to allocate from the
                                              //   system allocator.

static const int kMaxSubAllocationSize = 4096;   // Any request for more bytes
                                                 //  than this will be handled by
                                                 //  allocating directly with system.

void *         IDDocumentImpl::allocate(size_t amount)
{

     size_t sizeOfPointer = sizeof(void *);
     if (amount%sizeOfPointer!=0)
       amount = amount + (sizeOfPointer - (amount % sizeOfPointer));

    // If the request is for a largish block, hand it off to the system
    //   allocator.  The block still must be linked into the list of
    //   allocated blocks so that it will be deleted when the time comes.
    if (amount > kMaxSubAllocationSize)
    {
        void* newBlock = 0;
        try {
            newBlock = new char[amount + sizeof(void *)];
        }
        catch (...) {
            ThrowXML(RuntimeException, XMLExcepts::Out_Of_Memory);
        }
        if (!newBlock)
           ThrowXML(RuntimeException, XMLExcepts::Out_Of_Memory);

        if (fCurrentBlock)
        {
            *(void **)newBlock = *(void **)fCurrentBlock;
            *(void **)fCurrentBlock = newBlock;
        }
        else
        {
            fCurrentBlock = newBlock;
            fFreePtr = 0;
            fFreeBytesRemaining = 0;
        }
        void *retPtr = (char *)newBlock + sizeOfPointer;
        return retPtr;
    }


    // It's a normal (sub-allocatable) request.
    if (amount > fFreeBytesRemaining)
    {
        // Request doesn't fit in the current block.
        //   Get a new one from the system allocator.
        void* newBlock = 0;
        try {
            newBlock = new char[kHeapAllocSize];
        }
        catch (...) {
            ThrowXML(RuntimeException, XMLExcepts::Out_Of_Memory);
        }
        if (!newBlock)
           ThrowXML(RuntimeException, XMLExcepts::Out_Of_Memory);

        *(void **)newBlock = fCurrentBlock;
        fCurrentBlock = newBlock;
        fFreePtr = (char *)newBlock + sizeof(void *);
        fFreeBytesRemaining = kHeapAllocSize - sizeof(void *);
    }

    void *retPtr = fFreePtr;
    fFreePtr += amount;
    fFreeBytesRemaining -= amount;
    return retPtr;
}


void    IDDocumentImpl::deleteHeap()
{
    void *block = fCurrentBlock;
    while (fCurrentBlock != 0)
    {
        void *nextBlock = *(void **)fCurrentBlock;
        delete [] fCurrentBlock;
        fCurrentBlock = nextBlock;
    }

}

IDOM_NodeList *IDDocumentImpl::getDeepNodeList(const IDOM_Node *rootNode, const XMLCh *tagName)
{
    if(!fNodeListPool) {
        fNodeListPool = new (this) IDDeepNodeListPool<IDDeepNodeListImpl>(109, false);
    }

    IDDeepNodeListImpl* retList = fNodeListPool->getByKey(rootNode, tagName, 0);
    if (!retList) {
        int id = fNodeListPool->put((void*) rootNode, (XMLCh*) tagName, 0, new (this) IDDeepNodeListImpl(rootNode, tagName));
        retList = fNodeListPool->getById(id);
    }

    return retList;
}


IDOM_NodeList *IDDocumentImpl::getDeepNodeList(const IDOM_Node *rootNode,     //DOM Level 2
                                                   const XMLCh *namespaceURI,
                                                   const XMLCh *localName)
{
    if(!fNodeListPool) {
        fNodeListPool = new (this) IDDeepNodeListPool<IDDeepNodeListImpl>(109, false);
    }

    IDDeepNodeListImpl* retList = fNodeListPool->getByKey(rootNode, localName, namespaceURI);
    if (!retList) {
        // the pool will adopt the IDDeepNodeListImpl
        int id = fNodeListPool->put((void*) rootNode, (XMLCh*) localName, (XMLCh*) namespaceURI, new (this) IDDeepNodeListImpl(rootNode, namespaceURI, localName));
        retList = fNodeListPool->getById(id);
    }

    return retList;
}

