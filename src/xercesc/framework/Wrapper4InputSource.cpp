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
 * Revision 1.4  2002/11/04 15:00:21  tng
 * C++ Namespace Support.
 *
 * Revision 1.3  2002/07/19 14:57:28  knoaman
 * Add an adoptFlag parameter to the constructor and remove the setter method.
 * Documentation update.
 *
 * Revision 1.2  2002/07/15 18:26:46  tng
 * DOM L3: For consistency, add release() to DOMInputSource and thus Wrapper4InputSource.
 *
 * Revision 1.1  2002/06/18 19:07:07  knoaman
 * Initial checkin.
 *
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/framework/Wrapper4InputSource.hpp>
#include <xercesc/sax/InputSource.hpp>
#include <xercesc/util/NullPointerException.hpp>

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  Wrapper4InputSource: Constructor and Destructor
// ---------------------------------------------------------------------------
Wrapper4InputSource::Wrapper4InputSource(InputSource* const inputSource,
                                         const bool adoptFlag
                                         , MemoryManager* const manager) :
    fAdoptInputSource(adoptFlag)
    ,  fInputSource(inputSource)
{
    if (!inputSource)
        ThrowXMLwithMemMgr(NullPointerException, XMLExcepts::CPtr_PointerIsZero, manager);
}

Wrapper4InputSource::~Wrapper4InputSource()
{
    if (fAdoptInputSource)
        delete fInputSource;
}


// ---------------------------------------------------------------------------
//  Wrapper4InputSource: Getter methods
// ---------------------------------------------------------------------------
bool Wrapper4InputSource::getIssueFatalErrorIfNotFound() const
{
    return fInputSource->getIssueFatalErrorIfNotFound();
}

const XMLCh* Wrapper4InputSource::getEncoding() const
{
    return fInputSource->getEncoding();
}

const XMLCh* Wrapper4InputSource::getSystemId() const
{
    return fInputSource->getSystemId();
}

const XMLCh* Wrapper4InputSource::getPublicId() const
{
    return fInputSource->getPublicId();
}


// ---------------------------------------------------------------------------
//  Wrapper4InputSource: Setter methods
// ---------------------------------------------------------------------------
void Wrapper4InputSource::setIssueFatalErrorIfNotFound(const bool flag)
{
    fInputSource->setIssueFatalErrorIfNotFound(flag);
}


void Wrapper4InputSource::setEncoding(const XMLCh* const encodingStr)
{
    fInputSource->setEncoding(encodingStr);
}


void Wrapper4InputSource::setPublicId(const XMLCh* const publicId)
{
    fInputSource->setPublicId(publicId);
}


void Wrapper4InputSource::setSystemId(const XMLCh* const systemId)
{
    fInputSource->setSystemId(systemId);
}


// ---------------------------------------------------------------------------
//  Wrapper4InputSource: Stream methods
// ---------------------------------------------------------------------------
BinInputStream* Wrapper4InputSource::makeStream() const
{
    return fInputSource->makeStream();
}

// ---------------------------------------------------------------------------
//  Wrapper4InputSource: Memory methods
// ---------------------------------------------------------------------------
void Wrapper4InputSource::release()
{
    Wrapper4InputSource* src = (Wrapper4InputSource*) this;
    delete src;
}

XERCES_CPP_NAMESPACE_END

