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

#include "IDCharacterDataImpl.hpp"
#include "IDOM_DOMException.hpp"
#include "IDOM_Node.hpp"
#include "IDRangeImpl.hpp"
#include "IDDocumentImpl.hpp"
#include "IDCasts.hpp"
#include <xercesc/framework/XMLBuffer.hpp>
#include <xercesc/util/XMLUniDefs.hpp>


IDCharacterDataImpl::IDCharacterDataImpl(IDOM_Document *doc, const XMLCh *dat)
{
    this->fData = ((IDDocumentImpl *)doc)->getPooledString(dat);
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
    fData = ((IDDocumentImpl *)node->getOwnerDocument())->getPooledString(value);

    if (node->getOwnerDocument() != 0) {
        Ranges* ranges = ((IDDocumentImpl *)node->getOwnerDocument())->getRanges();
        if (ranges != 0) {
            unsigned int sz = ranges->size();
            if (sz != 0) {
                for (unsigned int i =0; i<sz; i++) {
                    ranges->elementAt(i)->receiveReplacedText((IDOM_Node*)node);
                }
            }
        }
    }
};


void IDCharacterDataImpl::appendData(const IDOM_Node *node, const XMLCh *dat)
{
    if(castToNodeImpl(node)->isReadOnly())
        throw IDOM_DOMException(
        IDOM_DOMException::NO_MODIFICATION_ALLOWED_ERR, 0);


    XMLBuffer temp;
    temp.set(fData);
    temp.append(dat);
    fData = ((IDDocumentImpl *)node->getOwnerDocument())->getPooledString(temp.getRawBuffer());
};


void IDCharacterDataImpl::deleteData(const IDOM_Node *node, unsigned int offset, unsigned int count)
{
    if (castToNodeImpl(node)->isReadOnly())
        throw IDOM_DOMException(IDOM_DOMException::NO_MODIFICATION_ALLOWED_ERR, 0);

    // Note: the C++ XMLCh * operation throws the correct DOMExceptions
    //       when parameter values are bad.
    //

    unsigned int len = XMLString::stringLen(this->fData);
    if (offset >= len || offset < 0 || count < 0)
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

    XMLCh* newString;
    XMLCh temp[4000];
    if (newLen >= 3999)
        newString = new XMLCh[newLen+1];
    else
        newString = temp;

    XMLString::copyNString(newString, fData, offset);
    XMLString::copyString(newString+offset, fData+offset+count);

    fData = ((IDDocumentImpl *)node->getOwnerDocument())->getPooledString(newString);

    if (newLen >= 3999)
        delete[] newString;

    // We don't delete the old string (doesn't work), or alter
    //   the old string (may be shared)
    //   It just hangs around, possibly orphaned.

    if (node->getOwnerDocument() != 0) {
        Ranges* ranges = ((IDDocumentImpl *)node->getOwnerDocument())->getRanges();
        if (ranges != 0) {
            unsigned int sz = ranges->size();
            if (sz != 0) {
                for (unsigned int i =0; i<sz; i++) {
                    ranges->elementAt(i)->updateRangeForDeletedText( (IDOM_Node*)node, offset, count);
                }
            }
        }
    }
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



void IDCharacterDataImpl::insertData(const IDOM_Node *node, unsigned int offset, const XMLCh *dat)
{
    if (castToNodeImpl(node)->isReadOnly())
        throw IDOM_DOMException(
        IDOM_DOMException::NO_MODIFICATION_ALLOWED_ERR, 0);

    // Note: the C++ XMLCh * operation throws the correct DOMExceptions
    //       when parameter values are bad.
    //

    unsigned int len = XMLString::stringLen(this->fData);
    if (offset >= len || offset < 0)
        throw IDOM_DOMException(IDOM_DOMException::INDEX_SIZE_ERR, 0);

    unsigned int datLen = XMLString::stringLen(dat);

    unsigned int newLen = len + datLen;

    XMLCh* newString;
    XMLCh temp[4000];
    if (newLen >= 3999)
        newString = new XMLCh[newLen+1];
    else
        newString = temp;

    XMLString::copyNString(newString, fData, offset);
    XMLString::copyNString(newString+offset, dat, datLen);
    XMLString::copyString(newString+offset+datLen, fData+offset);

    fData = ((IDDocumentImpl *)node->getOwnerDocument())->getPooledString(newString);

    if (newLen >= 3999)
        delete[] newString;
}



void IDCharacterDataImpl::replaceData(const IDOM_Node *node, unsigned int offset, unsigned int count,
                                    const XMLCh *dat)
{
    if (castToNodeImpl(node)->isReadOnly())
        throw IDOM_DOMException(
        IDOM_DOMException::NO_MODIFICATION_ALLOWED_ERR, 0);

    deleteData(node, offset, count);
    insertData(node, offset, dat);
};




void IDCharacterDataImpl::setData(const IDOM_Node *node, const XMLCh *arg)
{
    if (castToNodeImpl(node)->isReadOnly())
        throw IDOM_DOMException(IDOM_DOMException::NO_MODIFICATION_ALLOWED_ERR, 0);
    fData = ((IDDocumentImpl *)node->getOwnerDocument())->getPooledString(arg);
};





const XMLCh * IDCharacterDataImpl::substringData(const IDOM_Node *node, unsigned int offset,
                                           unsigned int count) const
{

    // Note: the C++ XMLCh * operation throws the correct DOMExceptions
    //       when parameter values are bad.
    //


    unsigned int len = XMLString::stringLen(fData);

    if (offset >= len || offset < 0 || count < 0)
        throw IDOM_DOMException(IDOM_DOMException::INDEX_SIZE_ERR, 0);


    XMLCh* newString;
    XMLCh temp[4000];
    if (len >= 3999)
        newString = new XMLCh[len+1];
    else
        newString = temp;

    XMLString::copyNString(newString, fData+offset, count);
    newString[count] = chNull;

    const XMLCh* retString = ((IDDocumentImpl *)node->getOwnerDocument())->getPooledString(newString);

    if (len >= 3999)
        delete[] newString;

    return retString;
};


