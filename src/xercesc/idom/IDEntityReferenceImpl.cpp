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

/**
* EntityReference models the XML &entityname; syntax, when used for
* entities defined by the DOM. Entities hardcoded into XML, such as
* character entities, should instead have been translated into text
* by the code which generated the DOM tree.
* <P>
* An XML processor has the alternative of fully expanding Entities
* into the normal document tree. If it does so, no EntityReference nodes
* will appear.
* <P>
* Similarly, non-validating XML processors are not required to read
* or process entity declarations made in the external subset or
* declared in external parameter entities. Hence, some applications
* may not make the replacement value available for Parsed Entities
* of these types.
* <P>
* EntityReference behaves as a read-only node, and the children of
* the EntityReference (which reflect those of the Entity, and should
* also be read-only) give its replacement value, if any. They are
* supposed to automagically stay in synch if the DocumentType is
* updated with new values for the Entity.
* <P>
* The defined behavior makes efficient storage difficult for the DOM
* implementor. We can't just look aside to the Entity's definition
* in the DocumentType since those nodes have the wrong parent (unless
* we can come up with a clever "imaginary parent" mechanism). We
* must at least appear to clone those children... which raises the
* issue of keeping the reference synchronized with its parent.
* This leads me back to the "cached image of centrally defined data"
* solution, much as I dislike it.
* <P>
* For now I have decided, since REC-DOM-Level-1-19980818 doesn't
* cover this in much detail, that synchronization doesn't have to be
* considered while the user is deep in the tree. That is, if you're
* looking within one of the EntityReferennce's children and the Entity
* changes, you won't be informed; instead, you will continue to access
* the same object -- which may or may not still be part of the tree.
* This is the same behavior that obtains elsewhere in the DOM if the
* subtree you're looking at is deleted from its parent, so it's
* acceptable here. (If it really bothers folks, we could set things
* up so deleted subtrees are walked and marked invalid, but that's
* not part of the DOM's defined behavior.)
* <P>
* As a result, only the EntityReference itself has to be aware of
* changes in the Entity. And it can take advantage of the same
* structure-change-monitoring code I implemented to support
* DeepNodeList.
*
* @author Rania Y. Khalaf
* @author Joseph Kesselman
* @since  PR-DOM-Level-1-19980818.
*/

#include "IDDocumentImpl.hpp"
#include "IDDocumentTypeImpl.hpp"
#include "IDEntityImpl.hpp"
#include "IDEntityReferenceImpl.hpp"
#include "IDOM_DOMException.hpp"
#include "IDOM_Node.hpp"
#include "IDNamedNodeMapImpl.hpp"


IDEntityReferenceImpl::IDEntityReferenceImpl(IDOM_Document *ownerDoc,
                                         const XMLCh *entityName)
    : fNode(ownerDoc), fParent(ownerDoc)
{
    fName = ((IDDocumentImpl *)getOwnerDocument())->getPooledString(entityName);
    // EntityReference behaves as a read-only node, since its contents
    // reflect the Entity it refers to -- but see setNodeName().
    fNode.isReadOnly(true);
}



IDEntityReferenceImpl::IDEntityReferenceImpl(const IDEntityReferenceImpl &other,
                                         bool deep)
    : fNode(other.fNode), fParent(other.fParent), fChild(other.fChild)
{
    fName = other.fName;
    if (deep)
        fParent.cloneChildren(&other);
    fNode.isReadOnly(true);
}



IDEntityReferenceImpl::~IDEntityReferenceImpl()
{
}


IDOM_Node *IDEntityReferenceImpl::cloneNode(bool deep) const
{
    return new (getOwnerDocument()) IDEntityReferenceImpl(*this, deep);
}


const XMLCh * IDEntityReferenceImpl::getNodeName() const
{
    return fName;
};


short IDEntityReferenceImpl::getNodeType() const {
    return IDOM_Node::ENTITY_REFERENCE_NODE;
};



/**
* EntityReferences never have a nodeValue.
* @throws DOMException(NO_MODIFICATION_ALLOWED_ERR)
*/
void IDEntityReferenceImpl::setNodeValue(const XMLCh *x)
{
    fNode.setNodeValue(x);
}


/**
* EntityRef is already, and must be, a read-only node. Attempts to change
* that will throw a NO_MODIFICATION_ALLOWED_ERR DOMException.
* <P>
* If you want to alter its contents, edit the Entity definition.
*
* @param readOnly boolean
*/
void IDEntityReferenceImpl::setReadOnly(bool readOnl,bool deep)
{
    if(readOnl==false)
        throw IDOM_DOMException(IDOM_DOMException::NO_MODIFICATION_ALLOWED_ERR, 0);
    fNode.setReadOnly(readOnl,deep);
}

//
//   Delegate functions from Node to the appropriate implementation.
//


           IDOM_Node          *IDEntityReferenceImpl::appendChild(IDOM_Node *newChild)        {return fParent.appendChild (newChild); };
           IDOM_NamedNodeMap  *IDEntityReferenceImpl::getAttributes() const 			        {return fNode.getAttributes (); };
           IDOM_NodeList      *IDEntityReferenceImpl::getChildNodes() const 			        {return fParent.getChildNodes (); };
           IDOM_Node          *IDEntityReferenceImpl::getFirstChild() const 			        {return fParent.getFirstChild (); };
           IDOM_Node          *IDEntityReferenceImpl::getLastChild() const 		            {return fParent.getLastChild (); };
     const XMLCh              *IDEntityReferenceImpl::getLocalName() const                    {return fNode.getLocalName (); };
     const XMLCh              *IDEntityReferenceImpl::getNamespaceURI() const                 {return fNode.getNamespaceURI (); };
           IDOM_Node          *IDEntityReferenceImpl::getNextSibling() const                  {return fChild.getNextSibling (); };
     const XMLCh              *IDEntityReferenceImpl::getNodeValue() const                    {return fNode.getNodeValue (); };
           IDOM_Document      *IDEntityReferenceImpl::getOwnerDocument() const                {return fNode.getOwnerDocument (); };
     const XMLCh              *IDEntityReferenceImpl::getPrefix() const                       {return fNode.getPrefix (); };
           IDOM_Node          *IDEntityReferenceImpl::getParentNode() const                   {return fChild.getParentNode (this); };
           IDOM_Node          *IDEntityReferenceImpl::getPreviousSibling() const              {return fChild.getPreviousSibling (this); };
           bool                IDEntityReferenceImpl::hasChildNodes() const                   {return fParent.hasChildNodes (); };
           IDOM_Node          *IDEntityReferenceImpl::insertBefore(IDOM_Node *newChild, IDOM_Node *refChild)
                                                                            {return fParent.insertBefore (newChild, refChild); };
           void                IDEntityReferenceImpl::normalize()                             {fParent.normalize (); };
           IDOM_Node          *IDEntityReferenceImpl::removeChild(IDOM_Node *oldChild)        {return fParent.removeChild (oldChild); };
           IDOM_Node          *IDEntityReferenceImpl::replaceChild(IDOM_Node *newChild, IDOM_Node *oldChild)
                                                                            {return fParent.replaceChild (newChild, oldChild); };
           bool                IDEntityReferenceImpl::isSupported(const XMLCh *feature, const XMLCh *version) const
                                                                            {return fNode.isSupported (feature, version); };
           void                IDEntityReferenceImpl::setPrefix(const XMLCh  *prefix)         {fNode.setPrefix(prefix); };
           bool                IDEntityReferenceImpl::hasAttributes() const                   {return fNode.hasAttributes(); };
