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

#include "IDCharacterDataImpl.hpp"
#include "IDOM_DOMException.hpp"
#include "IDOM_Node.hpp"
//#include "RangeImpl.hpp"
#include "IDDocumentImpl.hpp"
#include "IDCasts.hpp"


IDCharacterDataImpl::IDCharacterDataImpl(IDOM_Document *doc, const XMLCh *dat)
{
    this->fData = ((IDDocumentImpl *)doc)->cloneString(dat);
};


IDCharacterDataImpl::IDCharacterDataImpl(const IDCharacterDataImpl &other)
{
    fData = other.fData;
};


IDCharacterDataImpl::~IDCharacterDataImpl() {
};


const XMLCh * IDCharacterDataImpl::getNodeValue() const
{
    return fData;
};


void IDCharacterDataImpl::setNodeValue(const IDOM_Node *node, const XMLCh *value)
{
    if (castToNodeImpl(node)->isReadOnly())
        throw IDOM_DOMException(IDOM_DOMException::NO_MODIFICATION_ALLOWED_ERR,
                               0);
    fData = ((IDDocumentImpl *)node->getOwnerDocument())->cloneString(value);

#ifdef idom_revist
    if (this->getOwnerDocument() != null) {
        typedef RefVectorOf<RangeImpl> RangeImpls;
        RangeImpls* ranges = this->getOwnerDocument()->getRanges();
        if (ranges != null) {
            unsigned int sz = ranges->size();
            if (sz != 0) {
                for (unsigned int i =0; i<sz; i++) {
                    ranges->elementAt(i)->receiveReplacedText( this);
                }
            }
        }
    }
#endif
};


void IDCharacterDataImpl::appendData(const IDOM_Node *node, const XMLCh *dat)
{
    if(castToNodeImpl(node)->isReadOnly())
        throw IDOM_DOMException(
        IDOM_DOMException::NO_MODIFICATION_ALLOWED_ERR, 0);
 
#ifdef idom_revist
    this->data.appendData(dat);
#endif
};


void IDCharacterDataImpl::deleteData(const IDOM_Node *node, unsigned int offset, unsigned int count)
{
    if (castToNodeImpl(node)->isReadOnly())
        throw IDOM_DOMException(IDOM_DOMException::NO_MODIFICATION_ALLOWED_ERR, 0);

    // Note: the C++ XMLCh * operation throws the correct DOMExceptions
    //       when parameter values are bad.
    //  

    unsigned int len = XMLString::stringLen(this->fData);
    if (offset >= len)
        throw IDOM_DOMException(IDOM_DOMException::INDEX_SIZE_ERR, 0);



    // Cap the value of delLength to avoid trouble with overflows
    //  in the following length computations.
    if (count > len)
        count = len;

    // If the length of data to be deleted would extend off the end
    //   of the string, cut it back to stop at the end of string.
    if (offset + count >= len)
        count = len - offset;


    unsigned int newLen = len - count;
    XMLCh *newString = new (node->getOwnerDocument()) XMLCh(newLen + 1);

    XMLString::copyNString(newString, fData, offset);
    XMLString::copyString(newString+offset, fData+offset+len);

    fData = newString;
        
    // We don't delete the old string (doesn't work), or alter
    //   the old string (may be shared)
    //   It just hangs around, possibly orphaned.

#ifdef idom_revist
    if (node->getOwnerDocument() != null) {
        typedef RefVectorOf<RangeImpl> RangeImpls;
        RangeImpls* ranges = node->getOwnerDocument()->getRanges();
        if (ranges != null) {
            unsigned int sz = ranges->size();
            if (sz != 0) {
                for (unsigned int i =0; i<sz; i++) {
                    ranges->elementAt(i)->updateRangeForDeletedText( node, offset, count);
                }
            }
        }
    }
#endif //idom_revisit
};



const XMLCh *IDCharacterDataImpl::getData() const
{
    return fData; 
};


//
//  getCharDataLength - return the length of the character data string.
//
unsigned int IDCharacterDataImpl::getLength() const
{
    return XMLString::stringLen(fData);  
};



void IDCharacterDataImpl::insertData(unsigned int offset, const XMLCh *dat) 
{
    #ifdef idom_revist
    if (isReadOnly())
        throw DOM_DOMException(
        DOM_DOMException::NO_MODIFICATION_ALLOWED_ERR, null);
    
    // Note: the C++ XMLCh * operation throws the correct DOMExceptions
    //       when parameter values are bad.
    //  
    this->data.insertData(offset, dat);
#endif
}



void IDCharacterDataImpl::replaceData(unsigned int offset, unsigned int count,
                                    const XMLCh *dat)
{
#ifdef idom_revist
    if (isReadOnly())
        throw DOM_DOMException(
        DOM_DOMException::NO_MODIFICATION_ALLOWED_ERR, null);
    deleteData(offset, count);
    insertData(offset, dat);
#endif
};




void IDCharacterDataImpl::setData(const XMLCh *arg)
{
#ifdef idom_revist
    if (isReadOnly())
        throw DOM_DOMException(DOM_DOMException::NO_MODIFICATION_ALLOWED_ERR,
                               null);
    data = arg.clone();
#endif
};





const XMLCh * IDCharacterDataImpl::substringData(unsigned int offset,
                                           unsigned int count) const
{
#ifdef idom_revist

    // Note: the C++ XMLCh * operation throws the correct DOMExceptions
    //       when parameter values are bad.
    //  
    return data.substringData(offset, count);
#endif
    return 0;
};


