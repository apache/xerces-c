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

#include "IDCDATASectionImpl.hpp"
#include "IDNodeImpl.hpp"
#include "IDDocumentImpl.hpp"
#include "IDRangeImpl.hpp"
#include "IDCasts.hpp"
#include "IDOM_DOMException.hpp"
#include <xercesc/util/XMLUniDefs.hpp>

IDCDATASectionImpl::IDCDATASectionImpl(IDOM_Document *ownerDoc,
                                   const XMLCh *dat)
    : fNode(ownerDoc), fParent(ownerDoc), fCharacterData(ownerDoc, dat)
{
};


IDCDATASectionImpl::IDCDATASectionImpl(const IDCDATASectionImpl &other, bool deep)
    : fNode(*castToNodeImpl(&other)),
    fParent(*castToParentImpl(&other)),
    fChild(*castToChildImpl(&other)),
    fCharacterData(other.fCharacterData)
{
    // idom_revisit.  SOmething nees to make "deep" work.
};


IDCDATASectionImpl::~IDCDATASectionImpl()
{
};


IDOM_Node  *IDCDATASectionImpl::cloneNode(bool deep) const
{
    return new (this->getOwnerDocument()) IDCDATASectionImpl(*this, deep);
};


const XMLCh * IDCDATASectionImpl::getNodeName() const {
    static const XMLCh gcdata_section[] = {chPound, chLatin_c, chLatin_d, chLatin_a, chLatin_t, chLatin_a,
        chDash, chLatin_s, chLatin_e, chLatin_c, chLatin_t, chLatin_i, chLatin_o, chLatin_n, 0};
    return gcdata_section;
};


short IDCDATASectionImpl::getNodeType() const {
    return IDOM_Node::CDATA_SECTION_NODE;
};


bool IDCDATASectionImpl::isIgnorableWhitespace() const
{
    return fNode.ignorableWhitespace();
}


//
//  splitText.   idom_revist - factor into a common function for use
//                             here and in IDTextImpl
//
IDOM_Text *IDCDATASectionImpl::splitText(unsigned int offset)
{
    if (fNode.isReadOnly())
    {
        throw IDOM_DOMException(
            IDOM_DOMException::NO_MODIFICATION_ALLOWED_ERR, 0);
    }
    unsigned int len = XMLString::stringLen(fCharacterData.fData);
    if (offset > len)
        throw IDOM_DOMException(IDOM_DOMException::INDEX_SIZE_ERR, 0);

    IDOM_Text *newText =
                getOwnerDocument()->createCDATASection(
                        this->substringData(offset, len - offset));

    IDOM_Node *parent = getParentNode();
    if (parent != 0)
        parent->insertBefore(newText, getNextSibling());

    XMLCh *wData = (XMLCh *)(fCharacterData.fData);  // Cast off const.
    wData[offset] = 0;                               //  idom_revisit - could change a string that
                                                     //     application code has.  Do we want to do this?


    if (this->getOwnerDocument() != 0) {
        Ranges* ranges = ((IDDocumentImpl *)this->getOwnerDocument())->getRanges();
        if (ranges != 0) {
            unsigned int sz = ranges->size();
            if (sz != 0) {
                for (unsigned int i =0; i<sz; i++) {
                    ranges->elementAt(i)->updateSplitInfo( this, newText, offset);
                }
            }
        }
    }

    return newText;
};





//
//  Delegation stubs for other DOM_Node inherited functions.
//
           IDOM_Node          *IDCDATASectionImpl::appendChild(IDOM_Node *newChild)        {return fParent.appendChild (newChild); };
           IDOM_NamedNodeMap  *IDCDATASectionImpl::getAttributes() const 			        {return fNode.getAttributes (); };
           IDOM_NodeList      *IDCDATASectionImpl::getChildNodes() const 			        {return fParent.getChildNodes (); };
           IDOM_Node          *IDCDATASectionImpl::getFirstChild() const 			        {return fParent.getFirstChild (); };
           IDOM_Node          *IDCDATASectionImpl::getLastChild() const 		            {return fParent.getLastChild (); };
     const XMLCh              *IDCDATASectionImpl::getLocalName() const                    {return fNode.getLocalName (); };
     const XMLCh              *IDCDATASectionImpl::getNamespaceURI() const                 {return fNode.getNamespaceURI (); };
           IDOM_Node          *IDCDATASectionImpl::getNextSibling() const                  {return fChild.getNextSibling (); };
     const XMLCh              *IDCDATASectionImpl::getNodeValue() const                    {return fCharacterData.getNodeValue (); };
           IDOM_Document      *IDCDATASectionImpl::getOwnerDocument() const                {return fNode.getOwnerDocument (); };
     const XMLCh              *IDCDATASectionImpl::getPrefix() const                       {return fNode.getPrefix (); };
           IDOM_Node          *IDCDATASectionImpl::getParentNode() const                   {return fChild.getParentNode (this); };
           IDOM_Node          *IDCDATASectionImpl::getPreviousSibling() const              {return fChild.getPreviousSibling (this); };
           bool                IDCDATASectionImpl::hasChildNodes() const                   {return fParent.hasChildNodes (); };
           IDOM_Node          *IDCDATASectionImpl::insertBefore(IDOM_Node *newChild, IDOM_Node *refChild)
                                                                            {return fParent.insertBefore (newChild, refChild); };
           void                IDCDATASectionImpl::normalize()                             {fNode.normalize (); };
           IDOM_Node          *IDCDATASectionImpl::removeChild(IDOM_Node *oldChild)        {return fParent.removeChild (oldChild); };
           IDOM_Node          *IDCDATASectionImpl::replaceChild(IDOM_Node *newChild, IDOM_Node *oldChild)
                                                                            {return fParent.replaceChild (newChild, oldChild); };
           bool                IDCDATASectionImpl::supports(const XMLCh *feature, const XMLCh *version) const
                                                                            {return fNode.supports (feature, version); };
           void                IDCDATASectionImpl::setPrefix(const XMLCh  *prefix)         {fNode.setPrefix(prefix); };


//
//   Delegation of CharacerData functions.
//


     const XMLCh * IDCDATASectionImpl::getData() const                  {return fCharacterData.getData();};
     unsigned int  IDCDATASectionImpl::getLength() const                {return fCharacterData.getLength();};
     const XMLCh * IDCDATASectionImpl::substringData(unsigned int offset, unsigned int count) const
                                                                {return fCharacterData.substringData(this, offset, count);};
     void          IDCDATASectionImpl::appendData(const XMLCh *arg)     {fCharacterData.appendData(this, arg);};
     void          IDCDATASectionImpl::insertData(unsigned int offset, const  XMLCh *arg)
                                                                {fCharacterData.insertData(this, offset, arg);};
     void          IDCDATASectionImpl::deleteData(unsigned int offset, unsigned int count)
                                                                {fCharacterData.deleteData(this, offset, count);};
     void          IDCDATASectionImpl::replaceData(unsigned int offset, unsigned int count, const XMLCh *arg)
                                                                {fCharacterData.replaceData(this, offset, count, arg);};
     void          IDCDATASectionImpl::setData(const XMLCh *data)       {fCharacterData.setData(this, data);};
     void          IDCDATASectionImpl::setNodeValue(const XMLCh  *nodeValue)   {fCharacterData.setNodeValue (this, nodeValue); };


