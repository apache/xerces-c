/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 1999-2002 The Apache Software Foundation.  All rights
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

//
//	file:   NodeVector.cpp
//			Implementation of class NodeVector.
//			(Use of STL vector, or equivalent, would have been nice,
//			but is not available.  'NodeImpl *' is the only type
//			kept in Vectors in this DOM implementation, so this is
//			a hardwired implementation for that type.
//

#include "NodeVector.hpp"
#include <xercesc/framework/MemoryManager.hpp>
#include <assert.h>

XERCES_CPP_NAMESPACE_BEGIN


NodeVector::NodeVector(MemoryManager* const manager)
: fMemoryManager(manager)
{
	init(10);
};

NodeVector::NodeVector(unsigned int size,
                       MemoryManager* const manager)
: fMemoryManager(manager)
{
	init(size);
};


void NodeVector::init(unsigned int size) {
	assert(size > 0);
	data = (NodeImpl**) fMemoryManager->allocate(size * sizeof(NodeImpl*));//new NodeImpl *[size];
	assert(data != 0);
	allocatedSize = size;
	nextFreeSlot = 0;
};


NodeVector::~NodeVector() {
	fMemoryManager->deallocate(data);//delete [] data;
};


void NodeVector::addElement(NodeImpl *elem) {
	checkSpace();
	data[nextFreeSlot] = elem;
	++nextFreeSlot;
};


void NodeVector::checkSpace() {
	if (nextFreeSlot == allocatedSize) {
                unsigned int grow = allocatedSize/2;
                if (grow < 50) grow = 50;
		unsigned int newAllocatedSize = allocatedSize + grow;
		NodeImpl **newData = (NodeImpl**) fMemoryManager->allocate
        (
            newAllocatedSize * sizeof(NodeImpl*)
        );//new NodeImpl *[newAllocatedSize];
		assert(newData != 0);
		for (unsigned int i=0; i<allocatedSize; i++) {
			newData[i] = data[i];
		};
		fMemoryManager->deallocate(data);//delete [] data;
		allocatedSize = newAllocatedSize;
		data = newData;
	};
};

	
NodeImpl *NodeVector::elementAt(unsigned int index) {
    if (index >= nextFreeSlot)
        return 0;
	return data[index];
};

NodeImpl *NodeVector::lastElement() {
	if (nextFreeSlot == 0)
		return 0;
	return data[nextFreeSlot-1];
};


void NodeVector::insertElementAt(NodeImpl *elem, unsigned int index) {
	unsigned int i;

	assert(index <= nextFreeSlot);

	checkSpace();
	for (i=nextFreeSlot; i>index; --i) {
		data[i] = data[i-1];
	}
	data[index] = elem;
	++nextFreeSlot;

};


void NodeVector::removeElementAt(unsigned int index) {
	assert(index < nextFreeSlot);
	for (unsigned int i=index; i<nextFreeSlot-1; ++i) {
		data[i] = data[i+1];
	}
	--nextFreeSlot;
};

void NodeVector::reset() {
	nextFreeSlot = 0;
};

void NodeVector::setElementAt(NodeImpl *elem, unsigned int index) {
	assert(index < nextFreeSlot);
	data[index] = elem;
};


unsigned int NodeVector::size() {
	return nextFreeSlot;
};
		

XERCES_CPP_NAMESPACE_END

