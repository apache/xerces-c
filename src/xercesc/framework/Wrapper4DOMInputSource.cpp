/*
 * Copyright 2002,2004 The Apache Software Foundation.
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

/*
 * $Log$
 * Revision 1.7  2004/09/08 13:55:58  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.6  2003/12/17 00:18:33  cargilld
 * Update to memory management so that the static memory manager (one used to call Initialize) is only for static data.
 *
 * Revision 1.5  2003/05/30 16:11:43  gareth
 * Fixes so we compile under VC7.1. Patch by Alberto Massari.
 *
 * Revision 1.4  2003/05/16 21:36:55  knoaman
 * Memory manager implementation: Modify constructors to pass in the memory manager.
 *
 * Revision 1.3  2002/11/04 15:00:21  tng
 * C++ Namespace Support.
 *
 * Revision 1.2  2002/07/19 14:57:28  knoaman
 * Add an adoptFlag parameter to the constructor and remove the setter method.
 * Documentation update.
 *
 * Revision 1.1  2002/06/18 20:00:22  knoaman
 * Initial checkin.
 *
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/framework/Wrapper4DOMInputSource.hpp>
#include <xercesc/dom/DOMInputSource.hpp>
#include <xercesc/util/NullPointerException.hpp>

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  Wrapper4DOMInputSource: Constructor and Destructor
// ---------------------------------------------------------------------------
Wrapper4DOMInputSource::Wrapper4DOMInputSource(DOMInputSource* const inputSource,
                                               const bool adoptFlag,
                                               MemoryManager* const  manager) :
    InputSource(manager)
    , fAdoptInputSource(adoptFlag)
    ,  fInputSource(inputSource)
{
    if (!inputSource)
        ThrowXMLwithMemMgr(NullPointerException, XMLExcepts::CPtr_PointerIsZero, getMemoryManager());
}

Wrapper4DOMInputSource::~Wrapper4DOMInputSource()
{
    if (fAdoptInputSource)
        delete fInputSource;
}


// ---------------------------------------------------------------------------
//  Wrapper4DOMInputSource: Getter methods
// ---------------------------------------------------------------------------
bool Wrapper4DOMInputSource::getIssueFatalErrorIfNotFound() const
{
    return fInputSource->getIssueFatalErrorIfNotFound();
}

const XMLCh* Wrapper4DOMInputSource::getEncoding() const
{
    return fInputSource->getEncoding();
}

const XMLCh* Wrapper4DOMInputSource::getSystemId() const
{
    return fInputSource->getSystemId();
}

const XMLCh* Wrapper4DOMInputSource::getPublicId() const
{
    return fInputSource->getPublicId();
}


// ---------------------------------------------------------------------------
//  Wrapper4DOMInputSource: Setter methods
// ---------------------------------------------------------------------------
void Wrapper4DOMInputSource::setIssueFatalErrorIfNotFound(const bool flag)
{
    fInputSource->setIssueFatalErrorIfNotFound(flag);
}


void Wrapper4DOMInputSource::setEncoding(const XMLCh* const encodingStr)
{
    fInputSource->setEncoding(encodingStr);
}


void Wrapper4DOMInputSource::setPublicId(const XMLCh* const publicId)
{
    fInputSource->setPublicId(publicId);
}


void Wrapper4DOMInputSource::setSystemId(const XMLCh* const systemId)
{
    fInputSource->setSystemId(systemId);
}


// ---------------------------------------------------------------------------
//  Wrapper4DOMInputSource: Stream methods
// ---------------------------------------------------------------------------
BinInputStream* Wrapper4DOMInputSource::makeStream() const
{
    return fInputSource->makeStream();
}

XERCES_CPP_NAMESPACE_END

