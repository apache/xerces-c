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
 * Represents an XML (or HTML) comment.
 *
 * @author Rania Y. Khalaf
 * @author Joseph Kesselman
 * @since  PR-DOM-Level-1-19980818.
 */

#include "IDCommentImpl.hpp"
#include "IDCharacterDataImpl.hpp"
#include "IDOM_Node.hpp"
#include "IDCasts.hpp"
#include "IDDocumentImpl.hpp"
#include <xercesc/util/XMLUniDefs.hpp>

IDCommentImpl::IDCommentImpl(IDOM_Document *ownerDoc, const XMLCh *dat)
    : fNode(ownerDoc),  fCharacterData(ownerDoc, dat)
{
    fNode.setIsLeafNode(true);
};


IDCommentImpl::IDCommentImpl(const IDCommentImpl &other, bool deep)

    : fNode(other.fNode),
    fChild(other.fChild),
    fCharacterData(other.fCharacterData)
{
    fNode.setIsLeafNode(true);
};


IDCommentImpl::~IDCommentImpl() {
};



IDOM_Node * IDCommentImpl::cloneNode(bool deep) const
{
    return new (getOwnerDocument()) IDCommentImpl(*this, deep);
};


const XMLCh * IDCommentImpl::getNodeName() const {
    static const XMLCh gComment[] =
        {chPound, chLatin_c, chLatin_o, chLatin_m, chLatin_m, chLatin_e,chLatin_n, chLatin_t, 0};
    return gComment;
}

short IDCommentImpl::getNodeType() const {
    return IDOM_Node::COMMENT_NODE;
};




           IDOM_Node          *IDCommentImpl::appendChild(IDOM_Node *newChild)        {return fNode.appendChild (newChild); };
           IDOM_NamedNodeMap  *IDCommentImpl::getAttributes() const 			        {return fNode.getAttributes (); };
           IDOM_NodeList      *IDCommentImpl::getChildNodes() const 			        {return fNode.getChildNodes (); };
           IDOM_Node          *IDCommentImpl::getFirstChild() const 			        {return fNode.getFirstChild (); };
           IDOM_Node          *IDCommentImpl::getLastChild() const 		            {return fNode.getLastChild (); };
     const XMLCh              *IDCommentImpl::getLocalName() const                    {return fNode.getLocalName (); };
     const XMLCh              *IDCommentImpl::getNamespaceURI() const                 {return fNode.getNamespaceURI (); };
           IDOM_Node          *IDCommentImpl::getNextSibling() const                  {return fChild.getNextSibling (); };
     const XMLCh              *IDCommentImpl::getNodeValue() const                    {return fCharacterData.getNodeValue (); };
           IDOM_Document      *IDCommentImpl::getOwnerDocument() const                {return fNode.getOwnerDocument (); };
     const XMLCh              *IDCommentImpl::getPrefix() const                       {return fNode.getPrefix (); };
           IDOM_Node          *IDCommentImpl::getParentNode() const                   {return fChild.getParentNode (this); };
           IDOM_Node          *IDCommentImpl::getPreviousSibling() const              {return fChild.getPreviousSibling (this); };
           bool                IDCommentImpl::hasChildNodes() const                   {return fNode.hasChildNodes (); };
           IDOM_Node          *IDCommentImpl::insertBefore(IDOM_Node *newChild, IDOM_Node *refChild)
                                                                            {return fNode.insertBefore (newChild, refChild); };
           void                IDCommentImpl::normalize()                             {fNode.normalize (); };
           IDOM_Node          *IDCommentImpl::removeChild(IDOM_Node *oldChild)        {return fNode.removeChild (oldChild); };
           IDOM_Node          *IDCommentImpl::replaceChild(IDOM_Node *newChild, IDOM_Node *oldChild)
                                                                            {return fNode.replaceChild (newChild, oldChild); };
           bool                IDCommentImpl::isSupported(const XMLCh *feature, const XMLCh *version) const
                                                                            {return fNode.isSupported (feature, version); };
           void                IDCommentImpl::setPrefix(const XMLCh  *prefix)         {fNode.setPrefix(prefix); };
           bool                IDCommentImpl::hasAttributes() const                   {return fNode.hasAttributes(); };


//
//   Delegation of CharacerData functions.
//


     const XMLCh * IDCommentImpl::getData() const                  {return fCharacterData.getData();};
     unsigned int  IDCommentImpl::getLength() const                {return fCharacterData.getLength();};
     const XMLCh * IDCommentImpl::substringData(unsigned int offset, unsigned int count) const
                                                                {return fCharacterData.substringData(this, offset, count);};
     void          IDCommentImpl::appendData(const XMLCh *arg)     {fCharacterData.appendData(this, arg);};
     void          IDCommentImpl::insertData(unsigned int offset, const  XMLCh *arg)
                                                                {fCharacterData.insertData(this, offset, arg);};
     void          IDCommentImpl::deleteData(unsigned int offset, unsigned int count)
                                                                {fCharacterData.deleteData(this, offset, count);};
     void          IDCommentImpl::replaceData(unsigned int offset, unsigned int count, const XMLCh *arg)
                                                                {fCharacterData.replaceData(this, offset, count, arg);};
     void          IDCommentImpl::setData(const XMLCh *data)       {fCharacterData.setData(this, data);};
     void          IDCommentImpl::setNodeValue(const XMLCh  *nodeValue)   {fCharacterData.setNodeValue (this, nodeValue); };

