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


#include <util/XMLUniDefs.hpp>

#include "IDDocumentImpl.hpp"
#include "IDOM_DOMException.hpp"
#include "IDOM_Node.hpp"
#include "IDTextImpl.hpp"
#include "IDCharacterDataImpl.hpp"
#include "IDChildNode.hpp"


#include <assert.h>
//#include "IDRangeImpl.hpp"

class IDOM_Document;

IDTextImpl::IDTextImpl(IDOM_Document *ownerDoc, const XMLCh *dat)
    : fNode(ownerDoc), fCharacterData(ownerDoc, dat)
{
    fNode.setIsLeafNode(true);
};

IDTextImpl::IDTextImpl(const IDTextImpl &other, bool deep)
    : fNode(other.fNode), fCharacterData(other.fCharacterData)
{
    fNode.setIsLeafNode(true);
};

IDTextImpl::~IDTextImpl()
{
};


IDOM_Node *IDTextImpl::cloneNode(bool deep) const
{
    return new (getOwnerDocument()) IDTextImpl(*this, deep);
};


const XMLCh * IDTextImpl::getNodeName() const {
    static const XMLCh gtext[] = {chPound, chLatin_t, chLatin_e, chLatin_x, chLatin_t, chNull};
    return gtext;
}

short IDTextImpl::getNodeType() const {
    return IDOM_Node::TEXT_NODE;
};


IDOM_Text *IDTextImpl::splitText(unsigned int offset)
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
                getOwnerDocument()->createTextNode(
                        this->substringData(offset, len - offset));

    IDOM_Node *parent = getParentNode();
    if (parent != 0)
        parent->insertBefore(newText, getNextSibling());

    XMLCh *wData = (XMLCh *)(fCharacterData.fData);  // Cast off const.
    wData[offset] = 0;                               //  idom_revisit - could change a string that
                                                     //     application code has.  Do we want to do this?


#ifdef idom_revisit
    if (this->getOwnerDocument() != null) {
        typedef RefVectorOf<RangeImpl> RangeImpls;
        RangeImpls* ranges = this->getOwnerDocument()->getRanges();
        if (ranges != null) {
            unsigned int sz = ranges->size();
            if (sz != 0) {
                for (unsigned int i =0; i<sz; i++) {
                    ranges->elementAt(i)->updateSplitInfo( this, newText, offset);
                }
            }
        }
    }
#endif

    return newText;
};


bool IDTextImpl::isIgnorableWhitespace() const
{
    return fNode.ignorableWhitespace();
}



void IDTextImpl::setIgnorableWhitespace(bool ignorable)
{
    fNode.ignorableWhitespace(ignorable);
}


//
//  Delegation functions
//
           IDOM_Node          *IDTextImpl::appendChild(IDOM_Node *newChild)        {return fNode.appendChild (newChild); };
           IDOM_NamedNodeMap  *IDTextImpl::getAttributes() const 			        {return fNode.getAttributes (); };
           IDOM_NodeList      *IDTextImpl::getChildNodes() const 			        {return fNode.getChildNodes (); };
           IDOM_Node          *IDTextImpl::getFirstChild() const 			        {return fNode.getFirstChild (); };
           IDOM_Node          *IDTextImpl::getLastChild() const 		            {return fNode.getLastChild (); };
     const XMLCh              *IDTextImpl::getLocalName() const                    {return fNode.getLocalName (); };
     const XMLCh              *IDTextImpl::getNamespaceURI() const                 {return fNode.getNamespaceURI (); };
           IDOM_Node          *IDTextImpl::getNextSibling() const                  {return fChild.getNextSibling (); };
     const XMLCh              *IDTextImpl::getNodeValue() const                    {return fCharacterData.getNodeValue (); };
           IDOM_Document      *IDTextImpl::getOwnerDocument() const                {return fNode.getOwnerDocument (); };
     const XMLCh              *IDTextImpl::getPrefix() const                       {return fNode.getPrefix (); };
           IDOM_Node          *IDTextImpl::getParentNode() const                   {return fChild.getParentNode (this); };
           IDOM_Node          *IDTextImpl::getPreviousSibling() const              {return fChild.getPreviousSibling (this); };
           bool                IDTextImpl::hasChildNodes() const                   {return fNode.hasChildNodes (); };
           IDOM_Node          *IDTextImpl::insertBefore(IDOM_Node *newChild, IDOM_Node *refChild)
                                                                                    {return fNode.insertBefore (newChild, refChild); };
           void                IDTextImpl::normalize()                             {fNode.normalize (); };
           IDOM_Node          *IDTextImpl::removeChild(IDOM_Node *oldChild)        {return fNode.removeChild (oldChild); };
           IDOM_Node          *IDTextImpl::replaceChild(IDOM_Node *newChild, IDOM_Node *oldChild)
                                                                                    {return fNode.replaceChild (newChild, oldChild); };
           void                IDTextImpl::setNodeValue(const XMLCh  *nodeValue)   {fNode.setNodeValue (nodeValue); };
           bool                IDTextImpl::supports(const XMLCh *feature, const XMLCh *version) const
                                                                                    {return fNode.supports (feature, version); };
           void                IDTextImpl::setPrefix(const XMLCh  *prefix)         {fNode.setPrefix(prefix); };


//
//   Delegation of CharacerData functions.
//


     const XMLCh * IDTextImpl::getData() const                  {return fCharacterData.getData();};
     unsigned int  IDTextImpl::getLength() const                {return fCharacterData.getLength();};
     const XMLCh * IDTextImpl::substringData(unsigned int offset, unsigned int count) const
                                                                {return fCharacterData.substringData(offset, count);};
     void          IDTextImpl::appendData(const XMLCh *arg)     {fCharacterData.appendData(this, arg);};
     void          IDTextImpl::insertData(unsigned int offset, const  XMLCh *arg)
                                                                {fCharacterData.insertData(offset, arg);};
     void          IDTextImpl::deleteData(unsigned int offset, unsigned int count)
                                                                {fCharacterData.deleteData(this, offset, count);};
     void          IDTextImpl::replaceData(unsigned int offset, unsigned int count, const XMLCh *arg)
                                                                {fCharacterData.replaceData(offset, count, arg);};
     void          IDTextImpl::setData(const XMLCh *data)       {fCharacterData.setData(data);};

