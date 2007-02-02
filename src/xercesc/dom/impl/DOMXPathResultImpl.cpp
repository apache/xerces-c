/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 * 
 *      http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "DOMXPathResultImpl.hpp"
#include <xercesc/dom/DOMNode.hpp>
#include <xercesc/dom/DOMXPathException.hpp>

XERCES_CPP_NAMESPACE_BEGIN

DOMXPathResultImpl::DOMXPathResultImpl(unsigned short type, MemoryManager* const manager) :
  fType(type),
  fMemoryManager(manager)
{
    fSnapshot=new (fMemoryManager) RefVectorOf<DOMNode>(13, false, fMemoryManager);
}

DOMXPathResultImpl::~DOMXPathResultImpl()
{
    delete fSnapshot;
}

void DOMXPathResultImpl::reset(unsigned short type)
{
    fType=type;
    fSnapshot->removeAllElements();
}

void DOMXPathResultImpl::addResult(DOMNode* node)
{
    fSnapshot->addElement(node);
}

bool DOMXPathResultImpl::getBooleanValue() const
{
    throw DOMXPathException(DOMXPathException::TYPE_ERR, 0, fMemoryManager);
}

double DOMXPathResultImpl::getNumberValue() const
{
    throw DOMXPathException(DOMXPathException::TYPE_ERR, 0, fMemoryManager);
}

const XMLCh* DOMXPathResultImpl::getStringValue() const
{
    throw DOMXPathException(DOMXPathException::TYPE_ERR, 0, fMemoryManager);
}

DOMNode* DOMXPathResultImpl::iterateNext() const
{
    throw DOMXPathException(DOMXPathException::TYPE_ERR, 0, fMemoryManager);
}

bool DOMXPathResultImpl::getInvalidIteratorState() const
{
    return false;
}

short DOMXPathResultImpl::getResultType() const
{
    return fType;
}

DOMNode *DOMXPathResultImpl::getSingleNodeValue() const
{
    if(fType==ANY_UNORDERED_NODE_TYPE || fType==FIRST_ORDERED_NODE_TYPE)
        if(fSnapshot->size()>0)
            return fSnapshot->elementAt(0);
        else
            return NULL;
    throw DOMXPathException(DOMXPathException::TYPE_ERR, 0, fMemoryManager);
}

unsigned long DOMXPathResultImpl::getSnapshotLength() const
{
    if(fType==UNORDERED_NODE_SNAPSHOT_TYPE || fType==ORDERED_NODE_SNAPSHOT_TYPE)
        return fSnapshot->size();
    throw DOMXPathException(DOMXPathException::TYPE_ERR, 0, fMemoryManager);
}

DOMNode* DOMXPathResultImpl::snapshotItem(unsigned long index) const
{
    if(fType==UNORDERED_NODE_SNAPSHOT_TYPE || fType==ORDERED_NODE_SNAPSHOT_TYPE)
        if(fSnapshot->size()>index)
            return fSnapshot->elementAt(index);
        else
            return NULL;
    throw DOMXPathException(DOMXPathException::TYPE_ERR, 0, fMemoryManager);
}

void DOMXPathResultImpl::release() const
{
    DOMXPathResultImpl* me=(DOMXPathResultImpl*)this;
    delete me;
}

XERCES_CPP_NAMESPACE_END
