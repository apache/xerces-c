/*
 * The Apache Software License, Version 1.1
 * 
 * Copyright (c) 1999-2000 The Apache Software Foundation.  All rights
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

#include "IDDocumentFragmentImpl.hpp"
#include "IDOM_Node.hpp"
#include "IDDocumentImpl.hpp"
#include "IDOM_DOMException.hpp"
#include "IDCasts.hpp"
#include <util/XMLUniDefs.hpp>


static const XMLCh * nam;   // Will be lazily initialized to "#document-fragment"

IDDocumentFragmentImpl::IDDocumentFragmentImpl(IDOM_Document *masterDoc)
    : fNode(masterDoc), fParent(masterDoc)
{
};
        

IDDocumentFragmentImpl::IDDocumentFragmentImpl(const IDDocumentFragmentImpl &other,
                                           bool deep)
    : fNode(other.fNode), fParent(other.fParent)
{
    if (deep)
        castToParentImpl(this)->cloneChildren(&other);
};


IDDocumentFragmentImpl::~IDDocumentFragmentImpl()
{
};


        
IDOM_Node *IDDocumentFragmentImpl::cloneNode(bool deep) const
{
    return new (castToNodeImpl(this)->getOwnerDocument()) IDDocumentFragmentImpl(*this, deep);
};


const XMLCh * IDDocumentFragmentImpl::getNodeName() const {
    static const XMLCh name[] = {chPound, chLatin_d, chLatin_o, chLatin_c, chLatin_u, chLatin_m,
        chLatin_e, chLatin_n, chLatin_t, chDash,
        chLatin_f, chLatin_r, chLatin_a, chLatin_g, chLatin_m, chLatin_e, chLatin_n, chLatin_t, 0};  
    return name;
}


short IDDocumentFragmentImpl::getNodeType() const {
    return IDOM_Node::DOCUMENT_FRAGMENT_NODE;
};


void IDDocumentFragmentImpl::setNodeValue(const XMLCh *x)
{
        throw IDOM_DOMException(IDOM_DOMException::NO_MODIFICATION_ALLOWED_ERR, 0);
};


//
//  Delegation stubs for inherited functions.
//
           IDOM_Node          *IDDocumentFragmentImpl::appendChild(IDOM_Node *newChild)        {return fParent.appendChild (newChild); };
           IDOM_NamedNodeMap  *IDDocumentFragmentImpl::getAttributes() const 			        {return fNode.getAttributes (); };
           IDOM_NodeList      *IDDocumentFragmentImpl::getChildNodes() const 			        {return fParent.getChildNodes (); };
           IDOM_Node          *IDDocumentFragmentImpl::getFirstChild() const 			        {return fParent.getFirstChild (); };
           IDOM_Node          *IDDocumentFragmentImpl::getLastChild() const 		            {return fParent.getLastChild (); };
     const XMLCh              *IDDocumentFragmentImpl::getLocalName() const                    {return fNode.getLocalName (); };
     const XMLCh              *IDDocumentFragmentImpl::getNamespaceURI() const                 {return fNode.getNamespaceURI (); };
           IDOM_Node          *IDDocumentFragmentImpl::getNextSibling() const                  {return fNode.getNextSibling (); };
     const XMLCh              *IDDocumentFragmentImpl::getNodeValue() const                    {return fNode.getNodeValue (); };
           IDOM_Document      *IDDocumentFragmentImpl::getOwnerDocument() const                {return fNode.getOwnerDocument (); };
     const XMLCh              *IDDocumentFragmentImpl::getPrefix() const                       {return fNode.getPrefix (); };
           IDOM_Node          *IDDocumentFragmentImpl::getParentNode() const                   {return fNode.getParentNode (); };
           IDOM_Node          *IDDocumentFragmentImpl::getPreviousSibling() const              {return fNode.getPreviousSibling (); };
           bool                IDDocumentFragmentImpl::hasChildNodes() const                   {return fParent.hasChildNodes (); };
           IDOM_Node          *IDDocumentFragmentImpl::insertBefore(IDOM_Node *newChild, IDOM_Node *refChild)
                                                                            {return fParent.insertBefore (newChild, refChild); };
           void                IDDocumentFragmentImpl::normalize()                             {fNode.normalize (); };
           IDOM_Node          *IDDocumentFragmentImpl::removeChild(IDOM_Node *oldChild)        {return fParent.removeChild (oldChild); };
           IDOM_Node          *IDDocumentFragmentImpl::replaceChild(IDOM_Node *newChild, IDOM_Node *oldChild)
                                                                            {return fParent.replaceChild (newChild, oldChild); };
           bool                IDDocumentFragmentImpl::supports(const XMLCh *feature, const XMLCh *version) const
                                                                            {return fNode.supports (feature, version); };
           void                IDDocumentFragmentImpl::setPrefix(const XMLCh  *prefix)         {fNode.setPrefix(prefix); };

