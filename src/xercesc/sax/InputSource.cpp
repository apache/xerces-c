/*
 * Copyright 1999-2004 The Apache Software Foundation.
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
 * Revision 1.6  2004/09/08 13:56:19  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.5  2003/05/16 21:36:59  knoaman
 * Memory manager implementation: Modify constructors to pass in the memory manager.
 *
 * Revision 1.4  2003/05/15 18:27:05  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.3  2003/04/21 21:07:38  knoaman
 * Use XMLString::release to prepare for configurable memory manager.
 *
 * Revision 1.2  2002/11/04 14:56:26  tng
 * C++ Namespace Support.
 *
 * Revision 1.1.1.1  2002/02/01 22:22:08  peiyongz
 * sane_include
 *
 * Revision 1.5  2001/11/21 16:14:32  tng
 * Schema: New method InputSource::get/setIssueFatalErrorIfNotFound to tell the parser whether to issue fatal error or not if cannot find it (the InputSource).  This is required for schema processing as it shouldn't be a fatal error if the schema is not found.
 *
 * Revision 1.4  2000/03/02 19:54:35  roddey
 * This checkin includes many changes done while waiting for the
 * 1.1.0 code to be finished. I can't list them all here, but a list is
 * available elsewhere.
 *
 * Revision 1.3  2000/02/06 07:47:58  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.2  2000/01/12 00:15:39  roddey
 * Changes to deal with multiply nested, relative pathed, entities and to deal
 * with the new URL class changes.
 *
 * Revision 1.1.1.1  1999/11/09 01:07:45  twl
 * Initial checkin
 *
 * Revision 1.2  1999/11/08 20:45:01  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include    <xercesc/sax/InputSource.hpp>
#include    <xercesc/util/XMLString.hpp>

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  InputSource: Destructor
// ---------------------------------------------------------------------------
InputSource::~InputSource()
{
    fMemoryManager->deallocate(fEncoding);
    fMemoryManager->deallocate(fPublicId);
    fMemoryManager->deallocate(fSystemId);
}


// ---------------------------------------------------------------------------
//  InputSource: Setter methods
// ---------------------------------------------------------------------------
void InputSource::setEncoding(const XMLCh* const encodingStr)
{
    fMemoryManager->deallocate(fEncoding);
    fEncoding = XMLString::replicate(encodingStr, fMemoryManager);
}


void InputSource::setPublicId(const XMLCh* const publicId)
{
    fMemoryManager->deallocate(fPublicId);
    fPublicId = XMLString::replicate(publicId, fMemoryManager);
}


void InputSource::setSystemId(const XMLCh* const systemId)
{
    fMemoryManager->deallocate(fSystemId);
    fSystemId = XMLString::replicate(systemId, fMemoryManager);
}



// ---------------------------------------------------------------------------
//  InputSource: Hidden Constructors
// ---------------------------------------------------------------------------
InputSource::InputSource(MemoryManager* const manager) :

    fMemoryManager(manager)
    , fEncoding(0)
    , fPublicId(0)
    , fSystemId(0)
    , fFatalErrorIfNotFound(true)
{
}

InputSource::InputSource(const XMLCh* const systemId,
                         MemoryManager* const manager) :

    fMemoryManager(manager)
    , fEncoding(0)
    , fPublicId(0)
    , fSystemId(0)
    , fFatalErrorIfNotFound(true)
{
    fSystemId = XMLString::replicate(systemId, fMemoryManager);
}

InputSource::InputSource(const  XMLCh* const   systemId
                        , const XMLCh* const   publicId
                        , MemoryManager* const manager) :

    fMemoryManager(manager)
    , fEncoding(0)
    , fPublicId(0)
    , fSystemId(0)
    , fFatalErrorIfNotFound(true)
{
    fPublicId = XMLString::replicate(publicId, fMemoryManager);
    fSystemId = XMLString::replicate(systemId, fMemoryManager);
}

InputSource::InputSource(const char* const systemId,
                         MemoryManager* const manager) :

    fMemoryManager(manager)
    , fEncoding(0)
    , fPublicId(0)
    , fSystemId(0)
    , fFatalErrorIfNotFound(true)
{
    fSystemId = XMLString::transcode(systemId, fMemoryManager);
}

InputSource::InputSource(const  char* const systemId
                        , const char* const publicId
                        , MemoryManager* const manager) :

    fMemoryManager(manager)
    , fEncoding(0)
    , fPublicId(0)
    , fSystemId(0)
    , fFatalErrorIfNotFound(true)
{
    fPublicId = XMLString::transcode(publicId, fMemoryManager);
    fSystemId = XMLString::transcode(systemId, fMemoryManager);
}

XERCES_CPP_NAMESPACE_END

