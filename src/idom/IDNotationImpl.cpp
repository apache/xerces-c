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

/**
* Notations are how the Document Type Description (DTD) records hints
* about the format of an XML "unparsed entity" -- in other words,
* non-XML data bound to this document type, which some applications
* may wish to consult when manipulating the document. A Notation
* represents a name-value pair, with its nodeName being set to the
* declared name of the notation.
* <P>
* Notations are also used to formally declare the "targets" of
* Processing Instructions.
* <P>
* Note that the Notation's data is non-DOM information; the DOM only
* records what and where it is.
* <P>
* See the XML 1.0 spec, sections 4.7 and 2.6, for more info.
* <P>
* Level 1 of the DOM does not support editing Notation contents.
*
* @author Rania Y. Khalaf 
* @author Joseph Kesselman
* @since  PR-DOM-Level-1-19980818.
*/

#include "IDNotationImpl.hpp"
#include "IDOM_DOMException.hpp"
#include "IDOM_Node.hpp"
#include "IDDocumentImpl.hpp"



IDNotationImpl::IDNotationImpl(IDOM_Document *ownerDoc, const XMLCh *nName)
    : fNode(ownerDoc)
{
    fNode.setIsLeafNode(true);
    fName = ((IDDocumentImpl *)ownerDoc)->cloneString(nName);
};

IDNotationImpl::IDNotationImpl(const IDNotationImpl &other, bool deep)
    : fNode(other.fNode)
{
    fNode.setIsLeafNode(true);
    fName = other.fName;
    fPublicId = other.fPublicId;
    fPublicId = other.fSystemId;
};


IDNotationImpl::~IDNotationImpl()
{
};


IDOM_Node *IDNotationImpl::cloneNode(bool deep) const
{
    return new (getOwnerDocument()) IDNotationImpl(*this, deep);
};


const XMLCh * IDNotationImpl::getNodeName() const {
    return fName;
};


short IDNotationImpl::getNodeType() const {
    return IDOM_Node::NOTATION_NODE;
};



const XMLCh * IDNotationImpl::getPublicId() const
{
    return fPublicId;
};


const XMLCh * IDNotationImpl::getSystemId() const
{
    return fSystemId;
};


void IDNotationImpl::setNodeValue(const XMLCh *arg)
{
    throw IDOM_DOMException(IDOM_DOMException::NO_MODIFICATION_ALLOWED_ERR, 0);
};


void IDNotationImpl::setPublicId(const XMLCh *arg)
{
    if(fNode.isReadOnly())
        throw IDOM_DOMException(
        IDOM_DOMException::NO_MODIFICATION_ALLOWED_ERR,0);
    
    fPublicId = ((IDDocumentImpl *)getOwnerDocument())->cloneString(arg);
};


void IDNotationImpl::setSystemId(const XMLCh *arg)
{
    if(fNode.isReadOnly())
        throw IDOM_DOMException(
        IDOM_DOMException::NO_MODIFICATION_ALLOWED_ERR,0);
    
    fSystemId = ((IDDocumentImpl *)getOwnerDocument())->cloneString(arg);
};




           IDOM_Node          *IDNotationImpl::appendChild(IDOM_Node *newChild)        {return fNode.appendChild (newChild); };
           IDOM_NamedNodeMap  *IDNotationImpl::getAttributes() const 			        {return fNode.getAttributes (); };
           IDOM_NodeList      *IDNotationImpl::getChildNodes() const 			        {return fNode.getChildNodes (); };
           IDOM_Node          *IDNotationImpl::getFirstChild() const 			        {return fNode.getFirstChild (); };
           IDOM_Node          *IDNotationImpl::getLastChild() const 		            {return fNode.getLastChild (); };
     const XMLCh              *IDNotationImpl::getLocalName() const                    {return fNode.getLocalName (); };
     const XMLCh              *IDNotationImpl::getNamespaceURI() const                 {return fNode.getNamespaceURI (); };
           IDOM_Node          *IDNotationImpl::getNextSibling() const                  {return fNode.getNextSibling (); };
     const XMLCh              *IDNotationImpl::getNodeValue() const                    {return fNode.getNodeValue (); };
           IDOM_Document      *IDNotationImpl::getOwnerDocument() const                {return fNode.getOwnerDocument (); };
     const XMLCh              *IDNotationImpl::getPrefix() const                       {return fNode.getPrefix (); };
           IDOM_Node          *IDNotationImpl::getParentNode() const                   {return fNode.getParentNode (); };
           IDOM_Node          *IDNotationImpl::getPreviousSibling() const              {return fNode.getPreviousSibling (); };
           bool                IDNotationImpl::hasChildNodes() const                   {return fNode.hasChildNodes (); };
           IDOM_Node          *IDNotationImpl::insertBefore(IDOM_Node *newChild, IDOM_Node *refChild)
                                                                            {return fNode.insertBefore (newChild, refChild); };
           void                IDNotationImpl::normalize()                             {fNode.normalize (); };
           IDOM_Node          *IDNotationImpl::removeChild(IDOM_Node *oldChild)        {return fNode.removeChild (oldChild); };
           IDOM_Node          *IDNotationImpl::replaceChild(IDOM_Node *newChild, IDOM_Node *oldChild)
                                                                            {return fNode.replaceChild (newChild, oldChild); };
           bool                IDNotationImpl::supports(const XMLCh *feature, const XMLCh *version) const
                                                                            {return fNode.supports (feature, version); };
           void                IDNotationImpl::setPrefix(const XMLCh  *prefix)         {fNode.setPrefix(prefix); };


