/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 1999-2002 The Apache Software Foundation.  All rights
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

/*
 * $Id$
 */


#include "DOM_Document.hpp"
#include "DeepNodeListImpl.hpp"
#include "DocumentImpl.hpp"
#include "NodeIteratorImpl.hpp"
#include "TreeWalkerImpl.hpp"

XERCES_CPP_NAMESPACE_BEGIN


DOM_Document::DOM_Document (DocumentImpl *impl) :
    DOM_Node(impl)
{
};


DOM_Document::DOM_Document() :
        DOM_Node(null)
{
};


DOM_Document::DOM_Document(const DOM_Document &other)
: DOM_Node(other)
{
};


DOM_Document::~DOM_Document()
{
};

DOM_Document & DOM_Document::operator = (const DOM_Document &other)
{
    return (DOM_Document &) DOM_Node::operator = (other);
};


DOM_Document & DOM_Document::operator = (const DOM_NullPtr *other)
{
    return (DOM_Document &) DOM_Node::operator = (other);
};


DOM_Document    DOM_Document::createDocument(MemoryManager* const manager)
{
        return DOM_Document(new (manager) DocumentImpl(manager));
};


DOM_Notation    DOM_Document::createNotation(const DOMString &name)
{
        return DOM_Notation(((DocumentImpl *)fImpl)->createNotation(name));
};


DOM_DocumentType       DOM_Document::getDoctype() const {
        return DOM_DocumentType(((DocumentImpl *)fImpl)->getDoctype());
};


DOM_DOMImplementation  &DOM_Document::getImplementation() const {
        return DOM_DOMImplementation::getImplementation();
};

DOM_Element            DOM_Document::getDocumentElement() const {
        return DOM_Element(((DocumentImpl *)fImpl)->getDocumentElement());
};


DOM_Element            DOM_Document::createElement(const DOMString &tagName)
{
        return DOM_Element(((DocumentImpl *)fImpl)->createElement(tagName));
};



DOM_Element            DOM_Document::createElement(const XMLCh *tagName)
{
        return DOM_Element(((DocumentImpl *)fImpl)->createElement(tagName));
};



DOM_Entity            DOM_Document::createEntity(const DOMString &name)
{
        return DOM_Entity(((DocumentImpl *)fImpl)->createEntity(name));
};



DOM_DocumentFragment   DOM_Document::createDocumentFragment()
{
        return DOM_DocumentFragment(((DocumentImpl *)fImpl)->createDocumentFragment());
};


DOM_DocumentType DOM_Document::createDocumentType(const DOMString &name)
{
        return DOM_DocumentType(((DocumentImpl *)fImpl)->createDocumentType(name));
};



DOM_Text               DOM_Document::createTextNode(const DOMString &data) {
        return DOM_Text(((DocumentImpl *)fImpl)->createTextNode(data));
};


DOM_Comment            DOM_Document::createComment(const DOMString &data) {
        return DOM_Comment(((DocumentImpl *)fImpl)->createComment(data));
};


DOM_CDATASection       DOM_Document::createCDATASection(const DOMString &data) {
        return DOM_CDATASection(((DocumentImpl *)fImpl)->createCDATASection(data));
};


DOM_ProcessingInstruction DOM_Document::createProcessingInstruction(const DOMString &target,
                                                     const DOMString &data) {
        return DOM_ProcessingInstruction(((DocumentImpl *)fImpl)->createProcessingInstruction(target, data));
};


DOM_Attr               DOM_Document::createAttribute(const DOMString &name) {
        return DOM_Attr(((DocumentImpl *)fImpl)->createAttribute(name));
};


DOM_EntityReference    DOM_Document::createEntityReference(const DOMString &name) {
        return DOM_EntityReference(((DocumentImpl *)fImpl)->createEntityReference(name));
};


DOM_NodeIterator       DOM_Document::createNodeIterator(DOM_Node root, unsigned long whatToShow, DOM_NodeFilter* filter, bool entityReferenceExpansion) {
        return DOM_NodeIterator(DocumentImpl::createNodeIterator(root, whatToShow, filter, entityReferenceExpansion));
};


DOM_TreeWalker DOM_Document::createTreeWalker(DOM_Node root, unsigned long whatToShow, DOM_NodeFilter* filter, bool entityReferenceExpansion) {
    return DOM_TreeWalker(DocumentImpl::createTreeWalker(root, whatToShow, filter, entityReferenceExpansion));
};


DOM_NodeList DOM_Document::getElementsByTagName(const DOMString &tagname) const  {
	return DOM_NodeList(((DocumentImpl *)fImpl)->getElementsByTagName(tagname));
};


//Introduced in DOM Level 2

DOM_Node DOM_Document::importNode(const DOM_Node &importedNode, bool deep)
{
	return DOM_Node(((DocumentImpl *)fImpl)->importNode(importedNode.fImpl, deep));
};


DOM_Element         DOM_Document::createElementNS(const DOMString &namespaceURI,
	const DOMString &qualifiedName)
{
        return DOM_Element(((DocumentImpl *)fImpl)->createElementNS(namespaceURI, qualifiedName));
}


DOM_Attr            DOM_Document::createAttributeNS(const DOMString &namespaceURI,
	const DOMString &qualifiedName)
{
        return DOM_Attr(((DocumentImpl *)fImpl)->createAttributeNS(namespaceURI, qualifiedName));
}


DOM_NodeList        DOM_Document::getElementsByTagNameNS(const DOMString &namespaceURI,
	const DOMString &localName) const
{
        return DOM_NodeList(((DocumentImpl *)fImpl)->getElementsByTagNameNS(namespaceURI, localName));
}


DOM_Element         DOM_Document::getElementById(const DOMString &elementId)
{
        return DOM_Element(((DocumentImpl *)fImpl)->getElementById(elementId));
}


DOM_XMLDecl DOM_Document::createXMLDecl(const DOMString& version, const DOMString& encoding, const DOMString& standalone)
{
    return DOM_XMLDecl( ((DocumentImpl *)fImpl)->createXMLDecl(version, encoding, standalone));
}

DOM_Range    DOM_Document::createRange()
{
    return DOM_Range( ((DocumentImpl *)fImpl)->createRange() );
}


void DOM_Document::setErrorChecking(bool check) {
    ((DocumentImpl *)fImpl)->setErrorChecking(check);
}

bool DOM_Document::getErrorChecking() {
    return ((DocumentImpl *)fImpl)->getErrorChecking();
}

XERCES_CPP_NAMESPACE_END

