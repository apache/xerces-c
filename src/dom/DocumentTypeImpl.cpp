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
 * Revision 1.3  2000/01/08 00:09:28  andyh
 * Correcf failures in DOMTest with entity references and read-only nodes.
 * Correct reference counting problem NamedNodeMap.
 * Add export methods to NamedNodeMap and DocumentTypeImpl.
 * Redo DocumentImpl::cloneNode
 *
 * (Changes by Chih-Hsiang Chou)
 *
 * Revision 1.2  1999/11/30 21:16:25  roddey
 * Changes to add the transcode() method to DOMString, which returns a transcoded
 * version (to local code page) of the DOM string contents. And I changed all of the
 * exception 'throw by pointer' to 'throw by value' style.
 *
 * Revision 1.1.1.1  1999/11/09 01:08:44  twl
 * Initial checkin
 *
 * Revision 1.3  1999/11/08 20:44:24  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */

#include "DocumentTypeImpl.hpp"
#include "NodeImpl.hpp"
#include "DOM_Node.hpp"
#include "NamedNodeMapImpl.hpp"
#include "DOM_DOMException.hpp"


DocumentTypeImpl::DocumentTypeImpl(DocumentImpl *ownerDoc, const DOMString &dtName) 
: NodeImpl(ownerDoc,dtName,DOM_Node::DOCUMENT_TYPE_NODE,false,null),
    publicID(null), systemID(null), internalSubset(null)	//DOM Level 2
{
    entities = new NamedNodeMapImpl(ownerDoc,null);
    notations= new NamedNodeMapImpl(ownerDoc,null);
    
    // NON-DOM
    elements = new NamedNodeMapImpl(ownerDoc,null);
};


//Introduced in DOM Level 2
DocumentTypeImpl::DocumentTypeImpl(const DOMString &qualifiedName,
    const DOMString &publicID, const DOMString &systemID,
    const DOMString &internalSubset)
: NodeImpl(null, qualifiedName, DOM_Node::DOCUMENT_TYPE_NODE, false, null),
    publicID(publicID), systemID(systemID), internalSubset(internalSubset)
{
    entities = new NamedNodeMapImpl(null,null);
    notations= new NamedNodeMapImpl(null,null);
    
    // NON-DOM
    elements = new NamedNodeMapImpl(null,null);
};


DocumentTypeImpl::DocumentTypeImpl(const DocumentTypeImpl &other, bool deep)
: NodeImpl(other, deep)
{
    entities = other.entities->cloneMap();
    notations= other.notations->cloneMap();
    
    // NON-DOM
    elements = other.elements->cloneMap();

    //DOM Level 2
    publicID = other.publicID.clone();
    systemID = other.systemID.clone();
    internalSubset = other.internalSubset.clone();
};


DocumentTypeImpl::~DocumentTypeImpl()
{
    if (entities != null)
    {
        entities->removeAll();
        NamedNodeMapImpl::removeRef(entities);
    }

    if (notations != null)
    {
        notations->removeAll();
        NamedNodeMapImpl::removeRef(notations);
    }

    if (elements != null)
    {
        elements->removeAll();
        NamedNodeMapImpl::removeRef(elements);
    }

};


NodeImpl *DocumentTypeImpl::cloneNode(bool deep)
{
    return new DocumentTypeImpl(*this, deep);
};


NamedNodeMapImpl *DocumentTypeImpl::getElements()
{
    return elements;
};


NamedNodeMapImpl *DocumentTypeImpl::getEntities()
{
    return entities;
};


DOMString DocumentTypeImpl::getName()
{
    return name;
};


NamedNodeMapImpl *DocumentTypeImpl::getNotations()
{
    return notations;
};


bool DocumentTypeImpl::isDocumentTypeImpl()
{
    return true;
};


void DocumentTypeImpl::setNodeValue(const DOMString &val)
{
    throw DOM_DOMException(
        DOM_DOMException::NO_MODIFICATION_ALLOWED_ERR, null);
};


void DocumentTypeImpl::setReadOnly(bool readOnl, bool deep)
{
    NodeImpl::setReadOnly(readOnl,deep);
    entities->setReadOnly(readOnl,true);
    notations->setReadOnly(readOnl,true);
};


//Introduced in DOM Level 2

DOMString DocumentTypeImpl::getPublicID()
{
    return publicID;
}


DOMString DocumentTypeImpl::getSystemID()
{
    return systemID;
}


DOMString DocumentTypeImpl::getInternalSubset()
{
    return internalSubset;
}


void DocumentTypeImpl::setOwnerDocument(DocumentImpl *docImpl)
{
    ownerDocument = docImpl;
    //Note: ownerDoc of entities, notations and elements remain unchanged
    //The DOM APIs does not require a NamedNodeMap to have an owner document
}


/** Export this node to a different document docImpl.
 */
DocumentTypeImpl *DocumentTypeImpl::export(DocumentImpl *docImpl, bool deep)
{
    DocumentTypeImpl *doctype;
    if (localName != null) {	//true if namespace involved, i.e. DOM Level 2 and after
	doctype = new DocumentTypeImpl(name, publicID, systemID, internalSubset);
	doctype -> setOwnerDocument(docImpl);
    } else
	doctype = new DocumentTypeImpl(docImpl, name);
    if (deep) {
	delete doctype -> entities;
	delete doctype -> notations;
	delete doctype -> elements;
	doctype -> entities = entities -> export(docImpl);
	doctype -> notations = notations -> export(docImpl);
	doctype -> elements = elements -> export(docImpl);
    }
    return doctype;
}

