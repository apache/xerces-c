/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 1999 The Apache Software Foundation.  All rights
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
 * originally based on software copyright (c) 1999, International
 * Business Machines, Inc., http://www.ibm.com .  For more information
 * on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 */

/**
 * $Log$
 * Revision 1.5  2000/01/19 21:39:19  andyh
 * DOM L2, fix problems with new style createDocument.
 *
 * Revision 1.4  2000/01/08 00:09:27  andyh
 * Correcf failures in DOMTest with entity references and read-only nodes.
 * Correct reference counting problem NamedNodeMap.
 * Add export methods to NamedNodeMap and DocumentTypeImpl.
 * Redo DocumentImpl::cloneNode
 *
 * (Changes by Chih-Hsiang Chou)
 *
 * Revision 1.3  2000/01/05 01:16:08  andyh
 * DOM Level 2 core, namespace support added.
 *
 * Revision 1.2  1999/11/30 21:16:25  roddey
 * Changes to add the transcode() method to DOMString, which returns a transcoded
 * version (to local code page) of the DOM string contents. And I changed all of the
 * exception 'throw by pointer' to 'throw by value' style.
 *
 * Revision 1.1.1.1  1999/11/09 01:08:43  twl
 * Initial checkin
 *
 * Revision 1.5  1999/11/08 20:44:24  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */

//
// file DocumentImpl.cpp
//

#include "DocumentImpl.hpp"
#include "DOM_DOMException.hpp"
#include "DOM_Node.hpp"

#include "DocumentTypeImpl.hpp"
#include "ElementImpl.hpp"
#include "AttrImpl.hpp"
#include "CDATASectionImpl.hpp"
#include "CommentImpl.hpp"
#include "DocumentFragmentImpl.hpp"
#include "EntityImpl.hpp"
#include "EntityReferenceImpl.hpp"
#include "NotationImpl.hpp"
#include "ProcessingInstructionImpl.hpp"
#include "TextImpl.hpp"
#include "DOM_DOMImplementation.hpp"
#include "DeepNodeListImpl.hpp"
#include "NamedNodeMapImpl.hpp"
#include "DStringPool.hpp"
#include <internal/XMLReader.hpp>
#include "TreeWalkerImpl.hpp"
#include "NodeIteratorImpl.hpp"
#include "DOM_Document.hpp"

static DOMString *nam = 0;  // Will be lazily initialized to "#document"

DocumentImpl::DocumentImpl(): NodeImpl(null,
                                       DStringPool::getStaticString("#document", &nam),
                                       DOM_Node::DOCUMENT_NODE,
                                       false,
                                       null)
{
    docType=null;
    docElement=null;
    namePool = new DStringPool(257);
    iterators = 0L;
    treeWalkers = 0L;
};


//DOM Level 2
DocumentImpl::DocumentImpl(const DOMString &namespaceURI,
	const DOMString &qualifiedName, DocumentTypeImpl *doctype)
: NodeImpl(null, null, DStringPool::getStaticString("#document", &nam), DOM_Node::DOCUMENT_NODE, false, null)
{
    if (doctype != null && doctype->getOwnerDocument() != null)
        throw DOM_DOMException(	//one doctype can belong to only one DocumentImpl
	    DOM_DOMException::WRONG_DOCUMENT_ERR, null);
    docType=null;
    if (doctype != null) {
	doctype -> setOwnerDocument(this);
	appendChild(doctype);
    }
    docElement=null;
    appendChild(createElementNS(namespaceURI, qualifiedName));  //root element
    namePool = new DStringPool(257);
    iterators = 0L;
    treeWalkers = 0L;
}


DocumentImpl::~DocumentImpl()
{
    if (iterators != 0L) {
        // The data in the vector is pointers owned by smart pointers, and will be cleaned up when they go away.
        delete iterators;
    }

    if (treeWalkers != 0L) {
        // The data in the vector is pointers owned by smart pointers, and will be cleaned up when they go away.
        delete treeWalkers;
    }

    delete namePool;
    // Do not delete docType and docElement pointers here.
    //  These are also normal child nodes of the document,
    //  and refcounting will take them out in the usual way.
};


NodeImpl *DocumentImpl::cloneNode(bool deep) {
    bool ns = localName != null;    //true if namespace involved, i.e. DOM Level 2 and after
    DocumentImpl *newdoc = ns ? new DocumentImpl(namespaceURI, name, null) : new DocumentImpl();
    if (deep)
        for (NodeImpl *n=getFirstChild(); n!=null; n=n->getNextSibling()) {
	    if (n -> isDocumentTypeImpl()) {
		DocumentTypeImpl *doctype = ((DocumentTypeImpl *)n) -> export(newdoc, true);
		newdoc -> appendChild(doctype);
		newdoc -> docType = doctype;
	    } else if (n -> isElementImpl()) {
		ElementImpl *docelem = (ElementImpl *) newdoc -> importNode(n, true);
		newdoc -> appendChild(docelem);
		newdoc -> docElement = docelem;
	    } else
		newdoc -> appendChild(newdoc->importNode(n,true));
	}
    return newdoc;
};

bool DocumentImpl::isDocumentImpl() {
    return true;
};


AttrImpl *DocumentImpl::createAttribute(const DOMString &nam)
{
    if(!isXMLName(nam))
        throw DOM_DOMException(DOM_DOMException::INVALID_CHARACTER_ERR,null);
    return new AttrImpl(this,nam);
};



CDATASectionImpl *DocumentImpl::createCDATASection(const DOMString &data) {
    return new CDATASectionImpl(this,data);
};



CommentImpl *DocumentImpl::createComment(const DOMString &data)
{
    return new CommentImpl(this,data);
};



DocumentFragmentImpl *DocumentImpl::createDocumentFragment()
{
    return new DocumentFragmentImpl(this);
};



DocumentTypeImpl *DocumentImpl::createDocumentType(const DOMString &nam)
{
    if (!isXMLName(nam))
        throw DOM_DOMException(
        DOM_DOMException::INVALID_CHARACTER_ERR, null);

    return new DocumentTypeImpl(this, nam);
};



ElementImpl *DocumentImpl::createElement(const DOMString &tagName)
{
    if(!isXMLName(tagName))
        throw DOM_DOMException(DOM_DOMException::INVALID_CHARACTER_ERR,null);
    DOMString pooledTagName = this->namePool->getPooledString(tagName);
    return new ElementImpl(this,pooledTagName);
};


ElementImpl *DocumentImpl::createElement(const XMLCh *tagName)
{
    DOMString pooledTagName = this->namePool->getPooledString(tagName);
    return new ElementImpl(this,pooledTagName);
};




EntityImpl *DocumentImpl::createEntity(const DOMString &nam)
{
    if (!isXMLName(nam))
        throw DOM_DOMException(
        DOM_DOMException::INVALID_CHARACTER_ERR, null);

    return new EntityImpl(this, nam);
};



EntityReferenceImpl *DocumentImpl::createEntityReference(const DOMString &nam)
{
    if (!isXMLName(nam))
        throw DOM_DOMException(
        DOM_DOMException::INVALID_CHARACTER_ERR, null);

    return new EntityReferenceImpl(this, nam);
};



NotationImpl *DocumentImpl::createNotation(const DOMString &nam)
{
    if (!isXMLName(nam))
        throw DOM_DOMException(
        DOM_DOMException::INVALID_CHARACTER_ERR, null);

    return new NotationImpl(this, nam);
};



ProcessingInstructionImpl *DocumentImpl::createProcessingInstruction(
                                          const DOMString &target, const DOMString &data)
{
    if(!isXMLName(target))
        throw DOM_DOMException(DOM_DOMException::INVALID_CHARACTER_ERR,null);
    return new ProcessingInstructionImpl(this,target,data);
};




TextImpl *DocumentImpl::createTextNode(const DOMString &data)
{
    return new TextImpl(this,data);
};


NodeIteratorImpl* DocumentImpl::createNodeIterator (DOM_Node root, short whatToShow, DOM_NodeFilter filter, NodeFilterImpl* fi)
{
		// Create the node iterator implementation object.
		//	Add it to the vector of iterators that must be synchronized when a node is deleted.
		//	The vector of iterators is kept in the "owner document" if there is one. If there isn't one, I assume that root is the
		//	owner document.

    NodeIteratorImpl* iter = new NodeIteratorImpl(root, whatToShow, filter, fi);
    DOM_Document doc = root.getOwnerDocument();
    DocumentImpl* impl;

    if (! doc.isNull()) {
        impl = (DocumentImpl *) doc.fImpl;
    }
    else
        impl = (DocumentImpl *) root.fImpl;

    if (impl->iterators == 0L) {
        impl->iterators = new NodeIterators(1, false);
        impl->iterators->addElement(iter);
    }

    return iter;
}


TreeWalkerImpl* DocumentImpl::createTreeWalker (DOM_Node root, short whatToShow, DOM_NodeFilter filter, NodeFilterImpl* fi)
{
		// See notes for createNodeIterator...

    TreeWalkerImpl* twi = new TreeWalkerImpl(root, whatToShow, filter, fi);
    DOM_Document doc = root.getOwnerDocument();
    DocumentImpl* impl;

    if (! doc.isNull()) {
        impl = (DocumentImpl *) doc.fImpl;
    }
    else
        impl = (DocumentImpl *) root.fImpl;

    if (impl->treeWalkers == 0L) {
        impl->treeWalkers = new TreeWalkers(1, false);
        impl->treeWalkers->addElement(twi);
    }

    return twi;
}




DocumentTypeImpl *DocumentImpl::getDoctype()
{
    return docType;
};



ElementImpl *DocumentImpl::getDocumentElement()
{
    return docElement;
};



DeepNodeListImpl *DocumentImpl::getElementsByTagName(const DOMString &tagname)
{
    return new DeepNodeListImpl(this,tagname);
};



NodeImpl *DocumentImpl::insertBefore(NodeImpl *newChild, NodeImpl *refChild)
{
    // Only one such child permitted
    if(
        (newChild->isElementImpl() && docElement!=null)
        ||
        (newChild->isDocumentTypeImpl() && docType!=null)
        )
        throw DOM_DOMException(DOM_DOMException::HIERARCHY_REQUEST_ERR,null);

    NodeImpl::insertBefore(newChild,refChild);

    // If insert succeeded, cache the kid appropriately
    if(newChild->isElementImpl())
        docElement=(ElementImpl *)newChild;
    else if(newChild->isDocumentTypeImpl())
        docType=(DocumentTypeImpl *)newChild;

    return newChild;
};



bool DocumentImpl::isXMLName(const DOMString &s)
{
    XMLCh       *nam;
    int         length;
    int         i;

    length = s.length();
    if (length == 0)
        return false;

    nam = s.rawBuffer();
    if (!XMLReader::isFirstNameChar(nam[0]))
        return false;

    for (i=1; i<length; i++)
    {
        if (!XMLReader::isNameChar(nam[i]))
            return false;
    }
    return true;
};


//  referenced().   Override this function here in class DocumentImpl because
//                  we don't want the action taken in NodeImpl, which is
//                  to add a reference to the node's owning document.
//
void DocumentImpl::referenced()
{
    // Intentionally empty.
};



NodeImpl *DocumentImpl::removeChild(NodeImpl *oldChild)
{
    NodeImpl::removeChild(oldChild);

    // If remove succeeded, un-cache the kid appropriately
    if(oldChild->isElementImpl())
        docElement=null;
    else if(oldChild->isDocumentTypeImpl())
        docType=null;

    return oldChild;
};



void DocumentImpl::setNodeValue(const DOMString &x)
{
    throw DOM_DOMException(DOM_DOMException::NO_MODIFICATION_ALLOWED_ERR, null);
};



//
//    unreferenced()  will be called whenever the refernce count on
//          this document goes from 1 to 0.  In all cases, when this
//          happens to a document node (which is the case here), it
//          is time to actually delete the document.
//
//          See also NodeImpl::referenced() and unreferenced(), which
//          update document node ref counts based on references coming
//          or going to nodes owned by the document.
//
void DocumentImpl::unreferenced()
{
    deleteIf(this);
};


//Introduced in DOM Level 2

NodeImpl *DocumentImpl::importNode(NodeImpl *source, bool deep)
{
    NodeImpl *newnode=null;

    switch (source->getNodeType())
    {
    case DOM_Node::ELEMENT_NODE :
        {
            ElementImpl *newelement;
	    if (source->getLocalName() == null)
		newelement = createElement(source->getNodeName());
	    else
		newelement = createElementNS(source->getNamespaceURI(), source->getNodeName());
            NamedNodeMapImpl *srcattr=source->getAttributes();
            if(srcattr!=null)
                for(int i=0;i<srcattr->getLength();++i)
		{
		    AttrImpl *attr = (AttrImpl *) srcattr->item(i);
		    if (attr -> getSpecified())	//not a default attribute
			if (attr -> getLocalName() == null)
			    newelement->setAttributeNode((AttrImpl *)importNode(attr, true));
			else
			    newelement->setAttributeNodeNS((AttrImpl *)importNode(attr, true));
		}
            newnode=newelement;
        }
        break;
    case DOM_Node::ATTRIBUTE_NODE :
	if (source->getLocalName() == null)
	    newnode = createAttribute(source->getNodeName());
	else
	    newnode = createAttributeNS(source->getNamespaceURI(), source->getNodeName());
	deep = true;
        // Kids carry value
        break;
    case DOM_Node::TEXT_NODE :
        newnode = createTextNode(source->getNodeValue());
        break;
    case DOM_Node::CDATA_SECTION_NODE :
        newnode = createCDATASection(source->getNodeValue());
        break;
    case DOM_Node::COMMENT_NODE :
        newnode = createComment(source->getNodeValue());
        break;
    case DOM_Node::ENTITY_REFERENCE_NODE :
        newnode = createEntityReference(source->getNodeName());
	newnode -> readOnly = false; //allow deep copy temporarily
//        deep=false; // ????? Right Thing?
        // Value implied by doctype, so we should not copy it
        // -- instead, refer to local doctype, if any.
        break;
    case DOM_Node::ENTITY_NODE :
        {
            EntityImpl *srcentity=(EntityImpl *)source;
            EntityImpl *newentity = createEntity(source->getNodeName());
            newentity->setPublicId(srcentity->getPublicId());
            newentity->setSystemId(srcentity->getSystemId());
            newentity->setNotationName(srcentity->getNotationName());
            // Kids carry additional value
            newnode=newentity;
        }
        break;
    case DOM_Node::PROCESSING_INSTRUCTION_NODE :
        newnode = createProcessingInstruction(source->getNodeName(), source->getNodeValue());
        break;
    case DOM_Node::DOCUMENT_FRAGMENT_NODE :
        newnode = createDocumentFragment();
        // No name, kids carry value
        break;
    case DOM_Node::NOTATION_NODE :
        {
            NotationImpl *srcnotation=(NotationImpl *)source;
            NotationImpl *newnotation = createNotation(source->getNodeName());
            newnotation->setPublicId(srcnotation->getPublicId());
            newnotation->setSystemId(srcnotation->getSystemId());
            // Kids carry additional value
            newnode=newnotation;
            // No name, no value
            break;
        }

    case DOM_Node::DOCUMENT_NODE : // Document can't be child of Document
    case DOM_Node::DOCUMENT_TYPE_NODE :
    default:                       // Unknown node type
        throw DOM_DOMException(DOM_DOMException::NOT_SUPPORTED_ERR, null);
    }

    // If deep, replicate and attach the kids.
    if (deep)
        for (NodeImpl *srckid = source->getFirstChild(); srckid != null; srckid = srckid->getNextSibling())
        {
            newnode->appendChild(importNode(srckid, true));
        }
    if (newnode -> getNodeType() == DOM_Node::ENTITY_REFERENCE_NODE)
	newnode -> readOnly = true;

    return newnode;
};


ElementImpl *DocumentImpl::createElementNS(const DOMString &namespaceURI,
	const DOMString &qualifiedName)
{
    if(!isXMLName(qualifiedName))
        throw DOM_DOMException(DOM_DOMException::INVALID_CHARACTER_ERR,null);
    //DOMString pooledTagName = this->namePool->getPooledString(qualifiedName);
    return new ElementImpl(this, namespaceURI, qualifiedName);
}


AttrImpl *DocumentImpl::createAttributeNS(const DOMString &namespaceURI,
	const DOMString &qualifiedName)
{
    if(!isXMLName(qualifiedName))
        throw DOM_DOMException(DOM_DOMException::INVALID_CHARACTER_ERR,null);
    return new AttrImpl(this, namespaceURI, qualifiedName); 
}


DeepNodeListImpl *DocumentImpl::getElementsByTagNameNS(const DOMString &namespaceURI,
	const DOMString &localName)
{
    return new DeepNodeListImpl(this, namespaceURI, localName);
}


ElementImpl *DocumentImpl::getElementById(const DOMString &elementId)
{
    return null;
}
