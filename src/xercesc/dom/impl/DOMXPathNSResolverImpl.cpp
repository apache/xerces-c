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

XERCES_CPP_NAMESPACE_BEGIN

DOMXPathNSResolverImpl::DOMXPathNSResolverImpl(const DOMNode *nodeResolver):
 fResolverNode(nodeResolver)
{
}

const XMLCh* DOMXPathNSResolverImpl::lookupNamespaceURI(const XMLCh* prefix) const
{
    if (XMLString::equals(prefix, XMLUni::fgXMLString))
        return XMLUni::fgXMLURIName;
    if(fResolverNode)
        return fResolverNode->lookupNamespaceURI(prefix);
    return NULL;
}

const XMLCh* DOMXPathNSResolverImpl::lookupPrefix(const XMLCh* URI) const
{
    if (XMLString::equals(URI, XMLUni::fgXMLURIName))
        return XMLUni::fgXMLString;
    if(fResolverNode)
        return fResolverNode->lookupPrefix(URI);
    return NULL;
}

XERCES_CPP_NAMESPACE_END
