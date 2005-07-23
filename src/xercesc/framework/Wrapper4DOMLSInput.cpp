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
 * $Id$
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/framework/Wrapper4DOMLSInput.hpp>
#include <xercesc/dom/DOMLSInput.hpp>
#include <xercesc/util/NullPointerException.hpp>

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  Wrapper4DOMLSInput: Constructor and Destructor
// ---------------------------------------------------------------------------
Wrapper4DOMLSInput::Wrapper4DOMLSInput(DOMLSInput* const inputSource,
                                               const bool adoptFlag,
                                               MemoryManager* const  manager) :
    InputSource(manager)
    , fAdoptInputSource(adoptFlag)
    ,  fInputSource(inputSource)
{
    if (!inputSource)
        ThrowXMLwithMemMgr(NullPointerException, XMLExcepts::CPtr_PointerIsZero, getMemoryManager());
}

Wrapper4DOMLSInput::~Wrapper4DOMLSInput()
{
    if (fAdoptInputSource)
        delete fInputSource;
}


// ---------------------------------------------------------------------------
//  Wrapper4DOMLSInput: Getter methods
// ---------------------------------------------------------------------------
bool Wrapper4DOMLSInput::getIssueFatalErrorIfNotFound() const
{
    return fInputSource->getIssueFatalErrorIfNotFound();
}

const XMLCh* Wrapper4DOMLSInput::getEncoding() const
{
    return fInputSource->getEncoding();
}

const XMLCh* Wrapper4DOMLSInput::getSystemId() const
{
    return fInputSource->getSystemId();
}

const XMLCh* Wrapper4DOMLSInput::getPublicId() const
{
    return fInputSource->getPublicId();
}


// ---------------------------------------------------------------------------
//  Wrapper4DOMLSInput: Setter methods
// ---------------------------------------------------------------------------
void Wrapper4DOMLSInput::setIssueFatalErrorIfNotFound(const bool flag)
{
    fInputSource->setIssueFatalErrorIfNotFound(flag);
}


void Wrapper4DOMLSInput::setEncoding(const XMLCh* const encodingStr)
{
    fInputSource->setEncoding(encodingStr);
}


void Wrapper4DOMLSInput::setPublicId(const XMLCh* const publicId)
{
    fInputSource->setPublicId(publicId);
}


void Wrapper4DOMLSInput::setSystemId(const XMLCh* const systemId)
{
    fInputSource->setSystemId(systemId);
}


// ---------------------------------------------------------------------------
//  Wrapper4DOMLSInput: Stream methods
// ---------------------------------------------------------------------------
BinInputStream* Wrapper4DOMLSInput::makeStream() const
{
    return fInputSource->getByteStream();
}

XERCES_CPP_NAMESPACE_END

