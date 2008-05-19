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

#include "DOMXPathNSResolverImpl.hpp"
#include <xercesc/dom/DOMNode.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/Janitor.hpp>
#include <xercesc/util/XMLString.hpp>

XERCES_CPP_NAMESPACE_BEGIN

DOMXPathNSResolverImpl::DOMXPathNSResolverImpl(const DOMNode *nodeResolver, MemoryManager* const manager) :
    fNamespaceBindings(0),
    fResolverNode(nodeResolver),
    fManager(manager)
{
    fNamespaceBindings = new (fManager) RefHashTableOf<KVStringPair>(7, true, fManager);
}

DOMXPathNSResolverImpl::~DOMXPathNSResolverImpl()
{
    delete fNamespaceBindings;
}

const XMLCh* DOMXPathNSResolverImpl::lookupNamespaceURI(const XMLCh* prefix) const
{
    if(XMLString::equals(prefix, XMLUni::fgXMLString))
        return XMLUni::fgXMLURIName;

    const KVStringPair *pair = fNamespaceBindings->get((void*)prefix);
    if(pair) {
        if(*pair->getValue() == 0) return NULL;
        return pair->getValue();
    }

    if(fResolverNode)
        return fResolverNode->lookupNamespaceURI(prefix);

    return NULL;
}

const XMLCh* DOMXPathNSResolverImpl::lookupPrefix(const XMLCh* uri) const
{
    if(XMLString::equals(uri, XMLUni::fgXMLURIName))
        return XMLUni::fgXMLString;

    RefHashTableOfEnumerator<KVStringPair> enumerator((RefHashTableOf<KVStringPair>*)fNamespaceBindings);
    while(enumerator.hasMoreElements()) {
        KVStringPair &pair = enumerator.nextElement();
        if(XMLString::equals(pair.getValue(), uri)) {
            return pair.getKey();
        }
    }

    if(fResolverNode)
        return fResolverNode->lookupPrefix(uri);

    return NULL;
}

void DOMXPathNSResolverImpl::addNamespaceBinding(const XMLCh* prefix, const XMLCh* uri)
{
    if(prefix == 0) prefix = XMLUni::fgZeroLenString;
    if(uri == 0) uri = XMLUni::fgZeroLenString;

    KVStringPair* pair = new (fManager) KVStringPair(prefix, uri, fManager);

    fNamespaceBindings->put((void*)pair->getKey(), pair);
}

void DOMXPathNSResolverImpl::release()
{
    DOMXPathNSResolverImpl* me=(DOMXPathNSResolverImpl*)this;
    delete me;
}

XERCES_CPP_NAMESPACE_END
