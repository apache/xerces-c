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

// This class doesn't support having any children, and implements the behavior
// of an empty NodeList as far getChildNodes is concerned.
// The ParentNode subclass overrides this behavior.


#include "IDNodeImpl.hpp"
#include "IDOM_DOMException.hpp"
#include "IDCasts.hpp"
#include "IDDocumentImpl.hpp"
#include "IDDOMImplementation.hpp"

#include <xercesc/util/XMLUniDefs.hpp>
#include <xercesc/util/XMLRegisterCleanup.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <stdio.h>
#include <assert.h>

const unsigned short IDNodeImpl::READONLY     = 0x1<<0;
const unsigned short IDNodeImpl::SYNCDATA     = 0x1<<1;
const unsigned short IDNodeImpl::SYNCCHILDREN = 0x1<<2;
const unsigned short IDNodeImpl::OWNED        = 0x1<<3;
const unsigned short IDNodeImpl::FIRSTCHILD   = 0x1<<4;
const unsigned short IDNodeImpl::SPECIFIED    = 0x1<<5;
const unsigned short IDNodeImpl::IGNORABLEWS  = 0x1<<6;
const unsigned short IDNodeImpl::SETVALUE     = 0x1<<7;
const unsigned short IDNodeImpl::ID_ATTR      = 0x1<<8;
const unsigned short IDNodeImpl::USERDATA     = 0x1<<9;
const unsigned short IDNodeImpl::LEAFNODETYPE = 0x1<<10;
const unsigned short IDNodeImpl::CHILDNODE    = 0x1<<11;

// -----------------------------------------------------------------------
//  Reset the singleton gEmptyNodeList
// -----------------------------------------------------------------------
static IDNodeListImpl *gEmptyNodeList;  // make a singleton empty node list
static void reinitEmptyNodeList()
{
	delete gEmptyNodeList;
	gEmptyNodeList = 0;
}

// -----------------------------------------------------------------------
//  IDNodeImpl Functions
// -----------------------------------------------------------------------
IDNodeImpl::IDNodeImpl(IDOM_Node *ownerNode)
{
    this->flags = 0;
    // as long as we do not have any owner, fOwnerNode is our ownerDocument
    fOwnerNode  = ownerNode;
};

// This only makes a shallow copy, cloneChildren must also be called for a
// deep clone
IDNodeImpl::IDNodeImpl(const IDNodeImpl &other) {
    this->flags = other.flags;
    this->isReadOnly(false);

    // Need to break the association w/ original parent
    this->fOwnerNode = other.getOwnerDocument();
    this->isOwned(false);
};



IDNodeImpl::~IDNodeImpl() {
    //
    //   Note:  With IDOM memory mgmt, destructors are not called for nodes.

    //	if (hasUserData())
    //	{
    //		setUserData(0);
    //	}
};


IDOM_Node * IDNodeImpl::appendChild(IDOM_Node *newChild)
{
    // Only node types that don't allow children will use this default function.
    //   Others will go to IDParentNode::appendChild.
    throw IDOM_DOMException(IDOM_DOMException::HIERARCHY_REQUEST_ERR,0);
    return 0;
    //  return insertBefore(newChild, 0);
};


IDOM_NamedNodeMap * IDNodeImpl::getAttributes() const {
    return 0;                   // overridden in ElementImpl
};


IDOM_NodeList *IDNodeImpl::getChildNodes() const {
	static XMLRegisterCleanup emptyNodeListCleanup;

    if (gEmptyNodeList == 0)
    {
        IDOM_NodeList *t = new IDNodeListImpl(0);
        if (XMLPlatformUtils::compareAndSwap((void **)&gEmptyNodeList, t, 0) != 0)
        {
            delete t;
        }
        else
        {
            emptyNodeListCleanup.registerCleanup(reinitEmptyNodeList);
        }

    }
    return (IDOM_NodeList *)gEmptyNodeList;
};



IDOM_Node * IDNodeImpl::getFirstChild() const {
    return 0;                   // overridden in ParentNode
};


IDOM_Node * IDNodeImpl::getLastChild() const
{
    return 0;                   // overridden in ParentNode
};


IDOM_Node * IDNodeImpl::getNextSibling() const {
    return 0;                // overridden in ChildNode
};


const XMLCh * IDNodeImpl::getNodeValue() const {
    return 0;                    // Overridden by anything that has a value
}


//
//  Unlike the external getOwnerDocument, this one returns the owner document
//     for document nodes as well as all of the other node types.
//
IDOM_Document *IDNodeImpl::getOwnerDocument() const
{
    if (!this->isLeafNode())
    {
        IDElementImpl *ep = (IDElementImpl *)castToNode(this);
        return ep->fParent.fOwnerDocument;
    }

    //  Leaf node types - those that cannot have children, like Text.
    if (isOwned()) {
        return fOwnerNode->getOwnerDocument();
    } else {
        assert (fOwnerNode->getNodeType() == IDOM_Node::DOCUMENT_NODE);
        return  (IDOM_Document *)fOwnerNode;
    }
};


void IDNodeImpl::setOwnerDocument(IDOM_Document *doc) {
    // if we have an owner we rely on it to have it right
    // otherwise fOwnerNode is our ownerDocument
    if (!isOwned()) {
        // idom_revisit.  Problem with storage for doctype nodes that were created
        //                on the system heap in advance of having a document.
        fOwnerNode = doc;
    }
}

IDOM_Node * IDNodeImpl::getParentNode() const
{
    return 0;                // overridden in ChildNode
};


IDOM_Node*  IDNodeImpl::getPreviousSibling() const
{
    return 0;                // overridden in ChildNode
};


void *IDNodeImpl::getUserData() const
{
    void *userData = 0;
#ifdef idom_revisit
    if (hasUserData()) {
        userData =  (IDDocumentImpl *)getOwnerDocument()->getUserData(this);
    }
#endif
  return userData;
};

bool IDNodeImpl::hasChildNodes() const
{
    return false;
};



IDOM_Node *IDNodeImpl::insertBefore(IDOM_Node *newChild, IDOM_Node *refChild) {
    throw IDOM_DOMException(IDOM_DOMException::HIERARCHY_REQUEST_ERR, 0);
    return 0;
};


IDOM_Node *IDNodeImpl::removeChild(IDOM_Node *oldChild)
{
    throw IDOM_DOMException(IDOM_DOMException::NOT_FOUND_ERR, 0);
    return 0;
};


IDOM_Node *IDNodeImpl::replaceChild(IDOM_Node *newChild, IDOM_Node *oldChild)
{
    throw IDOM_DOMException(IDOM_DOMException::HIERARCHY_REQUEST_ERR,0);
    return 0;
};



void IDNodeImpl::setNodeValue(const XMLCh *val)
{
    if (isReadOnly())
        throw IDOM_DOMException(IDOM_DOMException::NO_MODIFICATION_ALLOWED_ERR, 0);
    // Default behavior is to do nothing, overridden in some subclasses
};



void IDNodeImpl::setReadOnly(bool readOnl, bool deep)
{
    this->isReadOnly(readOnl);

    if (deep) {
        for (IDOM_Node *mykid = castToNode(this)->getFirstChild();
            mykid != 0;
            mykid = mykid->getNextSibling())
            if(mykid->getNodeType() != IDOM_Node::ENTITY_REFERENCE_NODE)
                castToNodeImpl(mykid)->setReadOnly(readOnl,true);
    }
}


void IDNodeImpl::setUserData(void * val)
{
	IDDocumentImpl *doc = (IDDocumentImpl *)this->getOwnerDocument();

    doc->setUserData(val);
	if (val)
		hasUserData(true);
	else
		hasUserData(false);
};



//Introduced in DOM Level 2

void IDNodeImpl::normalize()
{
    // does nothing by default, overridden by subclasses
};


bool IDNodeImpl::isSupported(const XMLCh *feature, const XMLCh *version) const
{
    return IDOM_DOMImplementation::getImplementation()->hasFeature(feature, version);
}

const XMLCh *IDNodeImpl::getNamespaceURI() const
{
    return 0;
}

const XMLCh *IDNodeImpl::getPrefix() const
{
    return 0;
}

const XMLCh *IDNodeImpl::getLocalName() const
{
    return 0;
}


void IDNodeImpl::setPrefix(const XMLCh *fPrefix)
{
    throw IDOM_DOMException(IDOM_DOMException::NAMESPACE_ERR, 0);
}


bool IDNodeImpl::hasAttributes() const {
    return 0;                   // overridden in ElementImpl
};





static const XMLCh s_xml[] = {chLatin_x, chLatin_m, chLatin_l, chNull};
static const XMLCh s_xmlURI[] =    // "http://www.w3.org/XML/1998/namespace"
    { chLatin_h, chLatin_t, chLatin_t, chLatin_p, chColon, chForwardSlash, chForwardSlash,
      chLatin_w, chLatin_w, chLatin_w, chPeriod, chLatin_w, chDigit_3, chPeriod,
      chLatin_o, chLatin_r, chLatin_g, chForwardSlash, chLatin_X, chLatin_M, chLatin_L, chForwardSlash,
      chDigit_1, chDigit_9, chDigit_9, chDigit_8, chForwardSlash,
      chLatin_n, chLatin_a, chLatin_m, chLatin_e, chLatin_s, chLatin_p, chLatin_a, chLatin_c, chLatin_e,
      chNull};
static const XMLCh s_xmlns[] = {chLatin_x, chLatin_m, chLatin_l, chLatin_n, chLatin_s, chNull};
static const XMLCh s_xmlnsURI[] = // "http://www.w3.org/2000/xmlns/"
    {  chLatin_h, chLatin_t, chLatin_t, chLatin_p, chColon, chForwardSlash, chForwardSlash,
       chLatin_w, chLatin_w, chLatin_w, chPeriod, chLatin_w, chDigit_3, chPeriod,
       chLatin_o, chLatin_r, chLatin_g, chForwardSlash,
       chDigit_2, chDigit_0, chDigit_0, chDigit_0, chForwardSlash,
       chLatin_x, chLatin_m, chLatin_l, chLatin_n, chLatin_s, chForwardSlash, chNull};


const XMLCh *IDNodeImpl::getXmlString()      {return s_xml;};
const XMLCh *IDNodeImpl::getXmlURIString()   {return s_xmlURI;};
const XMLCh *IDNodeImpl::getXmlnsString()    {return s_xmlns;};
const XMLCh *IDNodeImpl::getXmlnsURIString() {return s_xmlnsURI;};

//Return a URI mapped from the given prefix and namespaceURI as below
//	prefix   namespaceURI		output
//---------------------------------------------------
//	"xml"      xmlURI            xmlURI
//	"xml"	   otherwise         NAMESPACE_ERR
//	"xmlns"	   xmlnsURI	         xmlnsURI (nType = ATTRIBUTE_NODE only)
//	"xmlns"	   otherwise         NAMESPACE_ERR (nType = ATTRIBUTE_NODE only)
//   != null   null or ""        NAMESPACE_ERR
//	else       any			     namesapceURI
const XMLCh* IDNodeImpl::mapPrefix(const XMLCh *prefix,
                                     const XMLCh *namespaceURI, short nType)
{

    static const XMLCh s_xml[] = {chLatin_x, chLatin_m, chLatin_l, chNull};
    static const XMLCh s_xmlURI[] =    // "http://www.w3.org/XML/1998/namespace"
    { chLatin_h, chLatin_t, chLatin_t, chLatin_p, chColon, chForwardSlash, chForwardSlash,
      chLatin_w, chLatin_w, chLatin_w, chPeriod, chLatin_w, chDigit_3, chPeriod,
      chLatin_o, chLatin_r, chLatin_g, chForwardSlash, chLatin_X, chLatin_M, chLatin_L, chForwardSlash,
      chDigit_1, chDigit_9, chDigit_9, chDigit_8, chForwardSlash,
      chLatin_n, chLatin_a, chLatin_m, chLatin_e, chLatin_s, chLatin_p, chLatin_a, chLatin_c, chLatin_e,
      chNull};
    static const XMLCh s_xmlns[] = {chLatin_x, chLatin_m, chLatin_l, chLatin_n, chLatin_s, chNull};
    static const XMLCh s_xmlnsURI[] = // "http://www.w3.org/2000/xmlns/"
    {  chLatin_h, chLatin_t, chLatin_t, chLatin_p, chColon, chForwardSlash, chForwardSlash,
       chLatin_w, chLatin_w, chLatin_w, chPeriod, chLatin_w, chDigit_3, chPeriod,
       chLatin_o, chLatin_r, chLatin_g, chForwardSlash,
       chDigit_2, chDigit_0, chDigit_0, chDigit_0, chForwardSlash,
       chLatin_x, chLatin_m, chLatin_l, chLatin_n, chLatin_s, chForwardSlash, chNull};


    if (prefix == 0)
        return namespaceURI;

    if (XMLString::compareString(prefix, s_xml) == 0)  {
        if (XMLString::compareString(namespaceURI, s_xmlURI) == 0)
            return s_xmlURI;
        throw IDOM_DOMException(IDOM_DOMException::NAMESPACE_ERR, 0);
    } else if (nType == IDOM_Node::ATTRIBUTE_NODE && XMLString::compareString(prefix, s_xmlns) == 0) {
        if (XMLString::compareString(namespaceURI, s_xmlnsURI) == 0)
            return s_xmlnsURI;
        throw IDOM_DOMException(IDOM_DOMException::NAMESPACE_ERR, 0);
    } else if (namespaceURI == 0 || *namespaceURI == 0) {
        throw IDOM_DOMException(IDOM_DOMException::NAMESPACE_ERR, 0);
    } else
        return namespaceURI;
    return namespaceURI;
}
