/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 2001 The Apache Software Foundation.  All rights
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

#include "IDXMLDeclImpl.hpp"
#include "IDOM_Node.hpp"
#include "IDDocumentImpl.hpp"
#include <util/XMLUni.hpp>
#include <util/XMLUniDefs.hpp>



IDXMLDeclImpl::IDXMLDeclImpl(IDOM_Document *ownerDoc)
    : fNode(ownerDoc),
    fVersion    (XMLUni::fgSupportedVersion),
    fEncoding   (XMLUni::fgUTF8EncodingString),
    fStandalone (XMLUni::fgNoString)
{
    fNode.setIsLeafNode(true);
};


//Introduced in DOM Level 2
IDXMLDeclImpl::IDXMLDeclImpl(IDOM_Document *ownerDoc, const XMLCh *ver,
                         const XMLCh *enc, const XMLCh *std)
	: fNode(ownerDoc)
{
    fNode.setIsLeafNode(true);
    IDDocumentImpl *doc = (IDDocumentImpl *)ownerDoc;
    fVersion    = doc->cloneString(ver);
    fEncoding   = doc->cloneString(enc);
    fStandalone = doc->cloneString(std);
};


IDXMLDeclImpl::IDXMLDeclImpl(const IDXMLDeclImpl &other, bool deep)
    : fNode(other.fNode), fChild(other.fChild)
{
    fNode.setIsLeafNode(true);
    fVersion     = other.fVersion;
    fEncoding    = other.fEncoding;
    fStandalone  = other.fStandalone;
};


IDXMLDeclImpl::~IDXMLDeclImpl()
{
};

IDOM_Node * IDXMLDeclImpl::cloneNode(bool deep) const
{
    return new (getOwnerDocument()) IDXMLDeclImpl(*this, deep);
}

const XMLCh * IDXMLDeclImpl::getNodeName() const
{
    static const XMLCh nam[] = // "#xmldecl"
      {chPound, chLatin_x, chLatin_m, chLatin_l, chLatin_d, chLatin_e,chLatin_c, chLatin_l, 0};

    return nam;
}

short IDXMLDeclImpl::getNodeType() const
{
    return IDOM_Node::XML_DECL_NODE;
}

const XMLCh * IDXMLDeclImpl::getVersion() const
{
    return fVersion;
}

const XMLCh * IDXMLDeclImpl::getEncoding() const
{
    return fEncoding;
}

const XMLCh * IDXMLDeclImpl::getStandalone() const
{
    return fStandalone;
}

void IDXMLDeclImpl::setVersion(const XMLCh *data)
{
    fVersion = ((IDDocumentImpl *)getOwnerDocument())->cloneString(data);
}

void IDXMLDeclImpl::setEncoding(const XMLCh *data)
{
    fEncoding = ((IDDocumentImpl *)getOwnerDocument())->cloneString(data);
}

void IDXMLDeclImpl::setStandalone(const XMLCh *data)
{
    fStandalone = ((IDDocumentImpl *)getOwnerDocument())->cloneString(data);
}


//
//  Delegation Functions
//
           IDOM_Node          *IDXMLDeclImpl::appendChild(IDOM_Node *newChild)        {return fNode.appendChild (newChild); };
           IDOM_NamedNodeMap  *IDXMLDeclImpl::getAttributes() const 			        {return fNode.getAttributes (); };
           IDOM_NodeList      *IDXMLDeclImpl::getChildNodes() const 			        {return fNode.getChildNodes (); };
           IDOM_Node          *IDXMLDeclImpl::getFirstChild() const 			        {return fNode.getFirstChild (); };
           IDOM_Node          *IDXMLDeclImpl::getLastChild() const 		                {return fNode.getLastChild (); };
     const XMLCh              *IDXMLDeclImpl::getLocalName() const                    {return fNode.getLocalName (); };
     const XMLCh              *IDXMLDeclImpl::getNamespaceURI() const                 {return fNode.getNamespaceURI (); };
           IDOM_Node          *IDXMLDeclImpl::getNextSibling() const                  {return fChild.getNextSibling (); };
     const XMLCh              *IDXMLDeclImpl::getNodeValue() const                    {return fNode.getNodeValue (); };
           IDOM_Document      *IDXMLDeclImpl::getOwnerDocument() const                {return fNode.getOwnerDocument (); };
     const XMLCh              *IDXMLDeclImpl::getPrefix() const                       {return fNode.getPrefix (); };
           IDOM_Node          *IDXMLDeclImpl::getParentNode() const                   {return fChild.getParentNode (this); };
           IDOM_Node          *IDXMLDeclImpl::getPreviousSibling() const              {return fChild.getPreviousSibling (this); };
           bool                IDXMLDeclImpl::hasChildNodes() const                   {return fNode.hasChildNodes (); };
           IDOM_Node          *IDXMLDeclImpl::insertBefore(IDOM_Node *newChild, IDOM_Node *refChild)
                                                                                        {return fNode.insertBefore (newChild, refChild); };
           void                IDXMLDeclImpl::normalize()                             {fNode.normalize (); };
           IDOM_Node          *IDXMLDeclImpl::removeChild(IDOM_Node *oldChild)        {return fNode.removeChild (oldChild); };
           IDOM_Node          *IDXMLDeclImpl::replaceChild(IDOM_Node *newChild, IDOM_Node *oldChild)
                                                                                       {return fNode.replaceChild (newChild, oldChild); };
           void                IDXMLDeclImpl::setNodeValue(const XMLCh  *nodeValue)   {fNode.setNodeValue (nodeValue); };
           bool                IDXMLDeclImpl::supports(const XMLCh *feature, const XMLCh *version) const
                                                                                        {return fNode.supports (feature, version); };
           void                IDXMLDeclImpl::setPrefix(const XMLCh  *prefix)         {fNode.setPrefix(prefix); };

