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

#include "IDProcessingInstructionImpl.hpp"
#include "IDDocumentImpl.hpp"
#include "IDNodeImpl.hpp"
#include "IDOM_DOMException.hpp"
#include "IDOM_Node.hpp"

IDProcessingInstructionImpl::IDProcessingInstructionImpl(IDOM_Document *ownerDoc,
                                                     const XMLCh *targt,
                                                     const XMLCh *dat)
    : fNode(ownerDoc)
{
    fNode.setIsLeafNode(true);
    this->fTarget = ((IDDocumentImpl *)ownerDoc)->cloneString(targt);
    this->fData =   ((IDDocumentImpl *)ownerDoc)->cloneString(dat);
};


IDProcessingInstructionImpl::IDProcessingInstructionImpl(
                                        const IDProcessingInstructionImpl &other,
                                        bool deep)
    : fNode(other.fNode), fChild(other.fChild)
{
    fNode.setIsLeafNode(true);
    fTarget = other.fTarget;
    fData = other.fData;
};


IDProcessingInstructionImpl::~IDProcessingInstructionImpl()
{
};


IDOM_Node *IDProcessingInstructionImpl::cloneNode(bool deep) const
{
    return new (getOwnerDocument()) IDProcessingInstructionImpl(*this, deep);
};


const XMLCh * IDProcessingInstructionImpl::getNodeName() const
{
    return fTarget;
};


short IDProcessingInstructionImpl::getNodeType() const {
    return IDOM_Node::PROCESSING_INSTRUCTION_NODE;
};


const XMLCh * IDProcessingInstructionImpl::getNodeValue() const
{
    return fData;
};


void IDProcessingInstructionImpl::setNodeValue(const XMLCh *value)
{
    if (fNode.isReadOnly())
        throw IDOM_DOMException(IDOM_DOMException::NO_MODIFICATION_ALLOWED_ERR, 0);
    fData = ((IDDocumentImpl *)getOwnerDocument())->cloneString(value);
};


const XMLCh * IDProcessingInstructionImpl::getData() const
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
const XMLCh * IDProcessingInstructionImpl::getTarget() const
{
    return fTarget;
};


/**
* Change the data content of this PI.
* Note that setNodeValue is aliased to setData
* @see getData().
* @throws DOMException(NO_MODIFICATION_ALLOWED_ERR) if node is read-only.
*/
void IDProcessingInstructionImpl::setData(const XMLCh *arg)
{
    if (fNode.isReadOnly())
        throw IDOM_DOMException(IDOM_DOMException::NO_MODIFICATION_ALLOWED_ERR,
                               0);
    fData = ((IDDocumentImpl *)getOwnerDocument())->cloneString(arg);
};



//
//    Delegation stubs for inherited functions
//
           IDOM_Node          *IDProcessingInstructionImpl::appendChild(IDOM_Node *newChild)        {return fNode.appendChild (newChild); };
           IDOM_NamedNodeMap  *IDProcessingInstructionImpl::getAttributes() const 			        {return fNode.getAttributes (); };
           IDOM_NodeList      *IDProcessingInstructionImpl::getChildNodes() const 			        {return fNode.getChildNodes (); };
           IDOM_Node          *IDProcessingInstructionImpl::getFirstChild() const 			        {return fNode.getFirstChild (); };
           IDOM_Node          *IDProcessingInstructionImpl::getLastChild() const 		            {return fNode.getLastChild (); };
     const XMLCh              *IDProcessingInstructionImpl::getLocalName() const                    {return fNode.getLocalName (); };
     const XMLCh              *IDProcessingInstructionImpl::getNamespaceURI() const                 {return fNode.getNamespaceURI (); };
           IDOM_Node          *IDProcessingInstructionImpl::getNextSibling() const                  {return fChild.getNextSibling (); };
           IDOM_Document      *IDProcessingInstructionImpl::getOwnerDocument() const                {return fNode.getOwnerDocument (); };
     const XMLCh              *IDProcessingInstructionImpl::getPrefix() const                       {return fNode.getPrefix (); };
           IDOM_Node          *IDProcessingInstructionImpl::getParentNode() const                   {return fChild.getParentNode (this); };
           IDOM_Node          *IDProcessingInstructionImpl::getPreviousSibling() const              {return fChild.getPreviousSibling (this); };
           bool                IDProcessingInstructionImpl::hasChildNodes() const                   {return fNode.hasChildNodes (); };
           IDOM_Node          *IDProcessingInstructionImpl::insertBefore(IDOM_Node *newChild, IDOM_Node *refChild)
                                                                                                    {return fNode.insertBefore (newChild, refChild); };
           void                IDProcessingInstructionImpl::normalize()                             {fNode.normalize (); };
           IDOM_Node          *IDProcessingInstructionImpl::removeChild(IDOM_Node *oldChild)        {return fNode.removeChild (oldChild); };
           IDOM_Node          *IDProcessingInstructionImpl::replaceChild(IDOM_Node *newChild, IDOM_Node *oldChild)
                                                                                                    {return fNode.replaceChild (newChild, oldChild); };
           bool                IDProcessingInstructionImpl::supports(const XMLCh *feature, const XMLCh *version) const
                                                                                                    {return fNode.supports (feature, version); };
           void                IDProcessingInstructionImpl::setPrefix(const XMLCh  *prefix)         {fNode.setPrefix(prefix); };
