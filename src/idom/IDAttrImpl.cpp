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

#include "IDAttrImpl.hpp"
#include "IDOM_DOMException.hpp"
#include "IDOM_Document.hpp"
#include "IDStringPool.hpp"
#include "IDNodeIDMap.hpp"
#include "IDDocumentImpl.hpp"
#include "IDCasts.hpp"


IDAttrImpl::IDAttrImpl(IDOM_Document *ownerDoc, const XMLCh *aName) 
    : fNode(ownerDoc), fParent (ownerDoc)
{
    IDDocumentImpl *docImpl = (IDDocumentImpl *)ownerDoc;
    fName = docImpl->getPooledString(aName);
    fNode.isSpecified(true);
};

IDAttrImpl::IDAttrImpl(const IDAttrImpl &other, bool deep)
    : fNode(other.fNode), fParent (other.fParent)
{
    fName = other.fName;
	
	if (other.fNode.isSpecified())
		fNode.isSpecified(true);
	else
		fNode.isSpecified(false);

    if (other.fNode.isIdAttr())
    {
        fNode.isIdAttr(true);
        IDDocumentImpl *doc = (IDDocumentImpl *)this->getOwnerDocument();
        doc->getNodeIDMap()->add(this);
    }
    
	fParent.cloneChildren(&other);
};


IDAttrImpl::~IDAttrImpl() {
};


IDOM_Node * IDAttrImpl::cloneNode(bool deep) const
{
    return new (this->getOwnerDocument()) IDAttrImpl(*this, deep);
};


const XMLCh * IDAttrImpl::getNodeName()  const{
    return fName;
};


short IDAttrImpl::getNodeType() const {
    return IDOM_Node::ATTRIBUTE_NODE;
};


const XMLCh * IDAttrImpl::getName() const {
    return fName;
};


const XMLCh * IDAttrImpl::getNodeValue() const
{
    return getValue();
};


bool IDAttrImpl::getSpecified() const
{
    return fNode.isSpecified();
};




const XMLCh * IDAttrImpl::getValue() const
{
    static const XMLCh emptyString[] = {0};
    if (fParent.fFirstChild == 0) {
        return emptyString; // return "";
    }
    IDOM_Node *node = castToChildImpl(fParent.fFirstChild)->nextSibling;
    if (node == 0) {
        return fParent.fFirstChild->getNodeValue();
    }
    int             length = 0;
    for (node = fParent.fFirstChild; node != 0; node = castToChildImpl(node)->nextSibling)
        length += XMLString::stringLen(node->getNodeValue());
    
    // idom_revisit - Come up with some way of not allocating a new string each
    //                time we do this.  But it's not an immediate pressing problem,
    //                becuase we only allocate a new string when we have attribute
    //                values that contain entity reference nodes.  And the parser
    //                does not ever produce such a thing.
    XMLCh * retString = new (this->getOwnerDocument()) XMLCh[length+1];
    retString[0] = 0;
    for (node = fParent.fFirstChild; node != 0; node = castToChildImpl(node)->nextSibling)
    {
        XMLString::catString(retString, node->getNodeValue());
    };
    
    return retString;
};



void IDAttrImpl::setNodeValue(const XMLCh *val)
{
    setValue(val);
};



void IDAttrImpl::setSpecified(bool arg)
{
    fNode.isSpecified(arg);
};



void IDAttrImpl::setValue(const XMLCh *val)
{
    if (fNode.isReadOnly())
    {
        throw IDOM_DOMException (
            IDOM_DOMException::NO_MODIFICATION_ALLOWED_ERR, 0);
    }
    
    //  If this attribute was of type ID and in the map, take it out,
    //    then put it back in with the new name.  For now, we don't worry
    //    about what happens if the new name conflicts
    //
    IDDocumentImpl *doc = (IDDocumentImpl *)getOwnerDocument();
    if (fNode.isIdAttr())
        doc->getNodeIDMap()->remove(this);

    IDOM_Node *kid;
    while ((kid = fParent.fFirstChild) != 0)         // Remove existing kids
    {
        removeChild(kid);
    }

    if (val != 0)              // Create and add the new one
        appendChild(doc->createTextNode(val));
    fNode.isSpecified(true);
    fParent.changed();
    
    if (fNode.isIdAttr())
        doc->getNodeIDMap()->add(this);

};




//Introduced in DOM Level 2

IDOM_Element *IDAttrImpl::getOwnerElement() const
{
    // if we have an owner, ownerNode is our ownerElement, otherwise it's
    // our ownerDocument and we don't have an ownerElement
    return (IDOM_Element *) (fNode.isOwned() ? fNode.fOwnerNode : 0);
}


//internal use by parser only
void IDAttrImpl::setOwnerElement(IDOM_Element *ownerElem)
{
    fNode.fOwnerNode = ownerElem;
    // idom_revisit.  Is this backwards?  isOwned(true)?
    fNode.isOwned(false);
}




           IDOM_Node          *IDAttrImpl::appendChild(IDOM_Node *newChild)        {return fParent.appendChild (newChild); };
           IDOM_NamedNodeMap  *IDAttrImpl::getAttributes() const 			       {return fNode.getAttributes (); };
           IDOM_NodeList      *IDAttrImpl::getChildNodes() const 			       {return fParent.getChildNodes (); };
           IDOM_Node          *IDAttrImpl::getFirstChild() const 			       {return fParent.getFirstChild (); };
           IDOM_Node          *IDAttrImpl::getLastChild() const 		           {return fParent.getLastChild (); };
     const XMLCh              *IDAttrImpl::getLocalName() const                    {return fNode.getLocalName (); };
     const XMLCh              *IDAttrImpl::getNamespaceURI() const                 {return fNode.getNamespaceURI (); };
           IDOM_Node          *IDAttrImpl::getNextSibling() const                  {return fNode.getNextSibling (); };
           IDOM_Document      *IDAttrImpl::getOwnerDocument() const                {return fNode.getOwnerDocument (); };
     const XMLCh              *IDAttrImpl::getPrefix() const                       {return fNode.getPrefix (); };
           IDOM_Node          *IDAttrImpl::getParentNode() const                   {return fNode.getParentNode (); };
           IDOM_Node          *IDAttrImpl::getPreviousSibling() const              {return fNode.getPreviousSibling (); };
           bool                IDAttrImpl::hasChildNodes() const                   {return fParent.hasChildNodes (); };
           IDOM_Node          *IDAttrImpl::insertBefore(IDOM_Node *newChild, IDOM_Node *refChild)
                                                                                   {return fParent.insertBefore (newChild, refChild); };
           void                IDAttrImpl::normalize()                             {fNode.normalize (); };
           IDOM_Node          *IDAttrImpl::removeChild(IDOM_Node *oldChild)        {return fParent.removeChild (oldChild); };
           IDOM_Node          *IDAttrImpl::replaceChild(IDOM_Node *newChild, IDOM_Node *oldChild)
                                                                                   {return fParent.replaceChild (newChild, oldChild); };
           bool                IDAttrImpl::supports(const XMLCh *feature, const XMLCh *version) const
                                                                                   {return fNode.supports (feature, version); };
           void                IDAttrImpl::setPrefix(const XMLCh  *prefix)         {fNode.setPrefix(prefix); };



