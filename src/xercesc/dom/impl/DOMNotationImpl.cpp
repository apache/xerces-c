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

#include "DOMDocumentImpl.hpp"
#include "DOMNotationImpl.hpp"
#include <xercesc/dom/DOMException.hpp>
#include <xercesc/dom/DOMNode.hpp>



DOMNotationImpl::DOMNotationImpl(DOMDocument *ownerDoc, const XMLCh *nName)
    : fNode(ownerDoc), fPublicId(0), fSystemId(0)
{
    fNode.setIsLeafNode(true);
    fName = ((DOMDocumentImpl *)ownerDoc)->cloneString(nName);
};

DOMNotationImpl::DOMNotationImpl(const DOMNotationImpl &other, bool deep)
    : fNode(other.fNode)
{
    fNode.setIsLeafNode(true);
    fName = other.fName;
    fPublicId = other.fPublicId;
    fSystemId = other.fSystemId;
};


DOMNotationImpl::~DOMNotationImpl()
{
};


DOMNode *DOMNotationImpl::cloneNode(bool deep) const
{
    return new (getOwnerDocument()) DOMNotationImpl(*this, deep);
};


const XMLCh * DOMNotationImpl::getNodeName() const {
    return fName;
};


short DOMNotationImpl::getNodeType() const {
    return DOMNode::NOTATION_NODE;
};



const XMLCh * DOMNotationImpl::getPublicId() const
{
    return fPublicId;
};


const XMLCh * DOMNotationImpl::getSystemId() const
{
    return fSystemId;
};


void DOMNotationImpl::setNodeValue(const XMLCh *arg)
{
    fNode.setNodeValue(arg);
};


void DOMNotationImpl::setPublicId(const XMLCh *arg)
{
    if(fNode.isReadOnly())
        throw DOMException(
        DOMException::NO_MODIFICATION_ALLOWED_ERR,0);

    fPublicId = ((DOMDocumentImpl *)getOwnerDocument())->cloneString(arg);
};


void DOMNotationImpl::setSystemId(const XMLCh *arg)
{
    if(fNode.isReadOnly())
        throw DOMException(
        DOMException::NO_MODIFICATION_ALLOWED_ERR,0);

    fSystemId = ((DOMDocumentImpl *)getOwnerDocument())->cloneString(arg);
};




           DOMNode          *DOMNotationImpl::appendChild(DOMNode *newChild)        {return fNode.appendChild (newChild); };
           DOMNamedNodeMap  *DOMNotationImpl::getAttributes() const 			        {return fNode.getAttributes (); };
           DOMNodeList      *DOMNotationImpl::getChildNodes() const 			        {return fNode.getChildNodes (); };
           DOMNode          *DOMNotationImpl::getFirstChild() const 			        {return fNode.getFirstChild (); };
           DOMNode          *DOMNotationImpl::getLastChild() const 		            {return fNode.getLastChild (); };
     const XMLCh              *DOMNotationImpl::getLocalName() const                    {return fNode.getLocalName (); };
     const XMLCh              *DOMNotationImpl::getNamespaceURI() const                 {return fNode.getNamespaceURI (); };
           DOMNode          *DOMNotationImpl::getNextSibling() const                  {return fNode.getNextSibling (); };
     const XMLCh              *DOMNotationImpl::getNodeValue() const                    {return fNode.getNodeValue (); };
           DOMDocument      *DOMNotationImpl::getOwnerDocument() const                {return fNode.getOwnerDocument (); };
     const XMLCh              *DOMNotationImpl::getPrefix() const                       {return fNode.getPrefix (); };
           DOMNode          *DOMNotationImpl::getParentNode() const                   {return fNode.getParentNode (); };
           DOMNode          *DOMNotationImpl::getPreviousSibling() const              {return fNode.getPreviousSibling (); };
           bool                DOMNotationImpl::hasChildNodes() const                   {return fNode.hasChildNodes (); };
           DOMNode          *DOMNotationImpl::insertBefore(DOMNode *newChild, DOMNode *refChild)
                                                                            {return fNode.insertBefore (newChild, refChild); };
           void                DOMNotationImpl::normalize()                             {fNode.normalize (); };
           DOMNode          *DOMNotationImpl::removeChild(DOMNode *oldChild)        {return fNode.removeChild (oldChild); };
           DOMNode          *DOMNotationImpl::replaceChild(DOMNode *newChild, DOMNode *oldChild)
                                                                            {return fNode.replaceChild (newChild, oldChild); };
           bool                DOMNotationImpl::isSupported(const XMLCh *feature, const XMLCh *version) const
                                                                            {return fNode.isSupported (feature, version); };
           void                DOMNotationImpl::setPrefix(const XMLCh  *prefix)         {fNode.setPrefix(prefix); };
           bool                DOMNotationImpl::hasAttributes() const                   {return fNode.hasAttributes(); };


