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

#include "DOMProcessingInstructionImpl.hpp"
#include "DOMDocumentImpl.hpp"
#include "DOMNodeImpl.hpp"

#include <xercesc/dom/DOMException.hpp>
#include <xercesc/dom/DOMNode.hpp>

DOMProcessingInstructionImpl::DOMProcessingInstructionImpl(DOMDocument *ownerDoc,
                                                     const XMLCh *targt,
                                                     const XMLCh *dat)
    : fNode(ownerDoc)
{
    fNode.setIsLeafNode(true);
    this->fTarget = ((DOMDocumentImpl *)ownerDoc)->cloneString(targt);
    this->fData =   ((DOMDocumentImpl *)ownerDoc)->cloneString(dat);
};


DOMProcessingInstructionImpl::DOMProcessingInstructionImpl(
                                        const DOMProcessingInstructionImpl &other,
                                        bool deep)
    : fNode(other.fNode), fChild(other.fChild)
{
    fNode.setIsLeafNode(true);
    fTarget = other.fTarget;
    fData = other.fData;
};


DOMProcessingInstructionImpl::~DOMProcessingInstructionImpl()
{
};


DOMNode *DOMProcessingInstructionImpl::cloneNode(bool deep) const
{
    return new (getOwnerDocument()) DOMProcessingInstructionImpl(*this, deep);
};


const XMLCh * DOMProcessingInstructionImpl::getNodeName() const
{
    return fTarget;
};


short DOMProcessingInstructionImpl::getNodeType() const {
    return DOMNode::PROCESSING_INSTRUCTION_NODE;
};


const XMLCh * DOMProcessingInstructionImpl::getNodeValue() const
{
    return fData;
};


void DOMProcessingInstructionImpl::setNodeValue(const XMLCh *value)
{
    if (fNode.isReadOnly())
        throw DOMException(DOMException::NO_MODIFICATION_ALLOWED_ERR, 0);
    fData = ((DOMDocumentImpl *)getOwnerDocument())->cloneString(value);
};


const XMLCh * DOMProcessingInstructionImpl::getData() const
{
    return fData;
};


/** A PI's "target" states what processor channel the PI's data
should be directed to. It is defined differently in HTML and XML.

  In XML, a PI's "target" is the first (whitespace-delimited) token
  following the "<?" token that begins the PI.

    In HTML, target is always 0.

      Note that getNodeName is aliased to getTarget.
*/
const XMLCh * DOMProcessingInstructionImpl::getTarget() const
{
    return fTarget;
};


/**
* Change the data content of this PI.
* Note that setNodeValue is aliased to setData
* @see getData().
* @throws DOMException(NO_MODIFICATION_ALLOWED_ERR) if node is read-only.
*/
void DOMProcessingInstructionImpl::setData(const XMLCh *arg)
{
    if (fNode.isReadOnly())
        throw DOMException(DOMException::NO_MODIFICATION_ALLOWED_ERR,
                               0);
    fData = ((DOMDocumentImpl *)getOwnerDocument())->cloneString(arg);
};



//
//    Delegation stubs for inherited functions
//
           DOMNode          *DOMProcessingInstructionImpl::appendChild(DOMNode *newChild)        {return fNode.appendChild (newChild); };
           DOMNamedNodeMap  *DOMProcessingInstructionImpl::getAttributes() const 			        {return fNode.getAttributes (); };
           DOMNodeList      *DOMProcessingInstructionImpl::getChildNodes() const 			        {return fNode.getChildNodes (); };
           DOMNode          *DOMProcessingInstructionImpl::getFirstChild() const 			        {return fNode.getFirstChild (); };
           DOMNode          *DOMProcessingInstructionImpl::getLastChild() const 		            {return fNode.getLastChild (); };
     const XMLCh              *DOMProcessingInstructionImpl::getLocalName() const                    {return fNode.getLocalName (); };
     const XMLCh              *DOMProcessingInstructionImpl::getNamespaceURI() const                 {return fNode.getNamespaceURI (); };
           DOMNode          *DOMProcessingInstructionImpl::getNextSibling() const                  {return fChild.getNextSibling (); };
           DOMDocument      *DOMProcessingInstructionImpl::getOwnerDocument() const                {return fNode.getOwnerDocument (); };
     const XMLCh              *DOMProcessingInstructionImpl::getPrefix() const                       {return fNode.getPrefix (); };
           DOMNode          *DOMProcessingInstructionImpl::getParentNode() const                   {return fChild.getParentNode (this); };
           DOMNode          *DOMProcessingInstructionImpl::getPreviousSibling() const              {return fChild.getPreviousSibling (this); };
           bool                DOMProcessingInstructionImpl::hasChildNodes() const                   {return fNode.hasChildNodes (); };
           DOMNode          *DOMProcessingInstructionImpl::insertBefore(DOMNode *newChild, DOMNode *refChild)
                                                                                                    {return fNode.insertBefore (newChild, refChild); };
           void                DOMProcessingInstructionImpl::normalize()                             {fNode.normalize (); };
           DOMNode          *DOMProcessingInstructionImpl::removeChild(DOMNode *oldChild)        {return fNode.removeChild (oldChild); };
           DOMNode          *DOMProcessingInstructionImpl::replaceChild(DOMNode *newChild, DOMNode *oldChild)
                                                                                                    {return fNode.replaceChild (newChild, oldChild); };
           bool                DOMProcessingInstructionImpl::isSupported(const XMLCh *feature, const XMLCh *version) const
                                                                                                    {return fNode.isSupported (feature, version); };
           void                DOMProcessingInstructionImpl::setPrefix(const XMLCh  *prefix)         {fNode.setPrefix(prefix); };
           bool                DOMProcessingInstructionImpl::hasAttributes() const                   {return fNode.hasAttributes(); };
