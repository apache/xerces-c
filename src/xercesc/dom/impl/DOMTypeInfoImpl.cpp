/*
 * Copyright 2003,2004 The Apache Software Foundation.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *      http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "DOMTypeInfoImpl.hpp"
#include "DOMDocumentImpl.hpp"

XERCES_CPP_NAMESPACE_BEGIN

DOMTypeInfoImpl::DOMTypeInfoImpl(const XMLCh* n, const XMLCh* nURI, DOMDocumentImpl *impl) {
    name = n?impl->getPooledString(n):n;
    namespaceURI = nURI?impl->getPooledString(nURI):nURI;
}

DOMTypeInfoImpl::DOMTypeInfoImpl() : name(0), namespaceURI(0) {}

const XMLCh* DOMTypeInfoImpl::getName() const {
    return name;
}

const XMLCh* DOMTypeInfoImpl::getNamespace() const {
  return namespaceURI;
}

XERCES_CPP_NAMESPACE_END
/**
 * End of file DOMTypeInfo.cpp
 */
