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

//
//	file:   DOMNodeVector.cpp
//			Implementation of class DOMNodeVector.
//			(Use of STL vector, or equivalent, would have been nice,
//			but is not available.  'DOMNode *' is the only type
//			kept in Vectors in this DOM implementation, so this is
//			a hardwired implementation for that type.
//

#include "DOMNodeVector.hpp"
#include "DOMDocumentImpl.hpp"
#include <assert.h>

XERCES_CPP_NAMESPACE_BEGIN


DOMNodeVector::DOMNodeVector(DOMDocument *doc)
{
	init(doc, 10);
}

DOMNodeVector::DOMNodeVector(DOMDocument *doc, XMLSize_t size) {
	init(doc, size);
}


void DOMNodeVector::init(DOMDocument *doc, XMLSize_t size) {
    assert(size > 0);
    //data = new (doc) DOMNode *[size];
    data = (DOMNode**) ((DOMDocumentImpl *)doc)->allocate(sizeof(DOMNode*) * size);
    assert(data != 0);
    XMLSize_t i;
    for (i=0; i<size; i++)
        data[i] = 0;
    allocatedSize = size;
    nextFreeSlot = 0;
}


DOMNodeVector::~DOMNodeVector() {
}


void DOMNodeVector::addElement(DOMNode *elem) {
	checkSpace();
	data[nextFreeSlot] = elem;
	++nextFreeSlot;
}


void DOMNodeVector::checkSpace() {
    if (nextFreeSlot == allocatedSize) {
        XMLSize_t grow = allocatedSize/2;
        if (grow < 50) grow = 50;
        XMLSize_t newAllocatedSize = allocatedSize + grow;
        DOMDocument *doc = data[0]->getOwnerDocument();

        //DOMNode **newData = new (doc) DOMNode *[newAllocatedSize];
        DOMNode **newData = (DOMNode**) ((DOMDocumentImpl *)doc)->allocate(sizeof(DOMNode*) * newAllocatedSize);

        assert(newData != 0);
        for (XMLSize_t i=0; i<allocatedSize; i++) {
            newData[i] = data[i];
        }
        // delete [] data;  // revisit.  Can't delete!  Recycle?
        allocatedSize = newAllocatedSize;
        data = newData;
    }
}

	
DOMNode *DOMNodeVector::elementAt(XMLSize_t index) {
    if (index >= nextFreeSlot)
        return 0;
	return data[index];
}

DOMNode *DOMNodeVector::lastElement() {
	if (nextFreeSlot == 0)
		return 0;
	return data[nextFreeSlot-1];
}


void DOMNodeVector::insertElementAt(DOMNode *elem, XMLSize_t index) {
	XMLSize_t i;

	assert(index <= nextFreeSlot);

	checkSpace();
	for (i=nextFreeSlot; i>index; --i) {
		data[i] = data[i-1];
	}
	data[index] = elem;
	++nextFreeSlot;

}


void DOMNodeVector::removeElementAt(XMLSize_t index) {
	assert(index < nextFreeSlot);
	for (XMLSize_t i=index; i<nextFreeSlot-1; ++i) {
		data[i] = data[i+1];
	}
	--nextFreeSlot;
}

void DOMNodeVector::reset() {
	nextFreeSlot = 0;
}

void DOMNodeVector::setElementAt(DOMNode *elem, XMLSize_t index) {
	assert(index < nextFreeSlot);
	data[index] = elem;
}


XMLSize_t DOMNodeVector::size() {
	return nextFreeSlot;
}
		

XERCES_CPP_NAMESPACE_END

