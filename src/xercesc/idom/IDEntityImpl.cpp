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

#include "IDOM_DOMException.hpp"
#include "IDOM_Node.hpp"
#include "IDEntityImpl.hpp"
#include "IDDocumentImpl.hpp"
#include "IDOM_EntityReference.hpp"


IDEntityImpl::IDEntityImpl(IDOM_Document *ownerDoc, const XMLCh *eName)
   :  fNode(ownerDoc), fParent(ownerDoc)
{
    fRefEntity  = 0;
    fName        = ((IDDocumentImpl *)ownerDoc)->getPooledString(eName);
    fNode.isReadOnly(true);
};


IDEntityImpl::IDEntityImpl(const IDEntityImpl &other, bool deep)
    : fNode(other.fNode), fParent(other.fParent)
{
    fName            = other.fName;
    if (deep)
        fParent.cloneChildren(&other);
    fPublicId        = other.fPublicId;
    fSystemId        = other.fSystemId;
    fNotationName    = other.fNotationName;

    fRefEntity       = other.fRefEntity;	
    fNode.isReadOnly(true);
};


IDEntityImpl::~IDEntityImpl() {
};


IDOM_Node *IDEntityImpl::cloneNode(bool deep) const
{
    return new (getOwnerDocument()) IDEntityImpl(*this, deep);
};


const XMLCh * IDEntityImpl::getNodeName() const {
    return fName;
};


short IDEntityImpl::getNodeType() const {
    return IDOM_Node::ENTITY_NODE;
};


const XMLCh * IDEntityImpl::getNotationName() const
{
    return fNotationName;
};


const XMLCh * IDEntityImpl::getPublicId() const {
    return fPublicId;
};


const XMLCh * IDEntityImpl::getSystemId() const
{
    return fSystemId;
};


void IDEntityImpl::setNodeValue(const XMLCh *arg)
{
    fNode.setNodeValue(arg);
};


void IDEntityImpl::setNotationName(const XMLCh *arg)
{
    IDDocumentImpl *doc = (IDDocumentImpl *)this->getOwnerDocument();
    fNotationName = doc->cloneString(arg);
}


void IDEntityImpl::setPublicId(const XMLCh *arg)
{
    IDDocumentImpl *doc = (IDDocumentImpl *)this->getOwnerDocument();
    fPublicId = doc->cloneString(arg);
}


void IDEntityImpl::setSystemId(const XMLCh *arg)
{
    IDDocumentImpl *doc = (IDDocumentImpl *)this->getOwnerDocument();
    fSystemId = doc->cloneString(arg);
}


void   IDEntityImpl::setEntityRef(IDOM_EntityReference* other)
{
	fRefEntity = other;
}


IDOM_EntityReference*		IDEntityImpl::getEntityRef() const
{
	return fRefEntity;
}

void	IDEntityImpl::cloneEntityRefTree() const
{
    // cast off const.  This method is const because it is
    //   called from a bunch of logically const methods, like
    //   getFirstChild().
    IDEntityImpl *ncThis = (IDEntityImpl *)this;
    //lazily clone the entityRef tree to this entity
    if (fParent.fFirstChild != 0)
        return;

    if (!fRefEntity)
        return;

    ncThis->fNode.isReadOnly(false);
    ncThis->fParent.cloneChildren(fRefEntity);
    ncThis->fNode.isReadOnly(true);
}

IDOM_Node * IDEntityImpl::getFirstChild() const
{
    cloneEntityRefTree();
	return fParent.fFirstChild;
};

IDOM_Node *   IDEntityImpl::getLastChild() const
{
	cloneEntityRefTree();
	return fParent.getLastChild();
}

IDOM_NodeList* IDEntityImpl::getChildNodes() const
{
	cloneEntityRefTree();
	return this->fParent.getChildNodes();

}

bool IDEntityImpl::hasChildNodes() const
{
	cloneEntityRefTree();
	return fParent.fFirstChild!=0;
}


//
//  Functions inherited from Node
//

           IDOM_Node          *IDEntityImpl::appendChild(IDOM_Node *newChild)        {return fParent.appendChild (newChild); };
           IDOM_NamedNodeMap  *IDEntityImpl::getAttributes() const 			        {return fNode.getAttributes (); };
     const XMLCh              *IDEntityImpl::getLocalName() const                    {return fNode.getLocalName (); };
     const XMLCh              *IDEntityImpl::getNamespaceURI() const                 {return fNode.getNamespaceURI (); };
           IDOM_Node          *IDEntityImpl::getNextSibling() const                  {return fNode.getNextSibling (); };
     const XMLCh              *IDEntityImpl::getNodeValue() const                    {return fNode.getNodeValue (); };
           IDOM_Document      *IDEntityImpl::getOwnerDocument() const                {return fNode.getOwnerDocument (); };
     const XMLCh              *IDEntityImpl::getPrefix() const                       {return fNode.getPrefix (); };
           IDOM_Node          *IDEntityImpl::getParentNode() const                   {return fNode.getParentNode (); };
           IDOM_Node          *IDEntityImpl::getPreviousSibling() const              {return fNode.getPreviousSibling (); };
           IDOM_Node          *IDEntityImpl::insertBefore(IDOM_Node *newChild, IDOM_Node *refChild)
                                                                            {return fParent.insertBefore (newChild, refChild); };
           void                IDEntityImpl::normalize()                             {fNode.normalize (); };
           IDOM_Node          *IDEntityImpl::removeChild(IDOM_Node *oldChild)        {return fParent.removeChild (oldChild); };
           IDOM_Node          *IDEntityImpl::replaceChild(IDOM_Node *newChild, IDOM_Node *oldChild)
                                                                            {return fParent.replaceChild (newChild, oldChild); };
           bool                IDEntityImpl::supports(const XMLCh *feature, const XMLCh *version) const
                                                                            {return fNode.supports (feature, version); };
           void                IDEntityImpl::setPrefix(const XMLCh  *prefix)         {fNode.setPrefix(prefix); };
