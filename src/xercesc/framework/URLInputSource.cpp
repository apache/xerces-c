/*
 * Copyright 1999-2000,2004 The Apache Software Foundation.
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

/**
 * $Log$
 * Revision 1.5  2004/09/08 13:55:58  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.4  2003/05/16 21:36:55  knoaman
 * Memory manager implementation: Modify constructors to pass in the memory manager.
 *
 * Revision 1.3  2002/11/04 15:00:21  tng
 * C++ Namespace Support.
 *
 * Revision 1.2  2002/04/25 00:12:34  jasons
 * fairly serious error in the two XMLCh* constructor, the fURL member
 * was being improperly initialized with the (systemID,publicID) instead
 * of (baseId,systemId)
 *
 * Revision 1.1.1.1  2002/02/01 22:21:50  peiyongz
 * sane_include
 *
 * Revision 1.3  2000/02/06 07:47:46  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.2  2000/01/15 01:26:16  rahulj
 * Added support for HTTP to the parser using libWWW 5.2.8.
 * Renamed URL.[ch]pp to XMLURL.[ch]pp and like wise for the class name.
 * Only tested under NT 4.0 SP 5.
 * Removed URL.hpp from files where it was not used.
 *
 * Revision 1.1  2000/01/12 00:13:26  roddey
 * These were moved from internal/ to framework/, which was something that should have
 * happened long ago. They are really framework type of classes.
 *
 * Revision 1.1.1.1  1999/11/09 01:08:18  twl
 * Initial checkin
 *
 * Revision 1.3  1999/11/08 20:44:44  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/util/BinFileInputStream.hpp>
#include <xercesc/util/Janitor.hpp>
#include <xercesc/util/XMLURL.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/framework/URLInputSource.hpp>

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  URLInputSource: Constructors and Destructor
// ---------------------------------------------------------------------------
URLInputSource::URLInputSource( const XMLURL&         urlId
                              , MemoryManager* const  manager) :

    InputSource(manager)
    , fURL(urlId)
{
    setSystemId(fURL.getURLText());
}

URLInputSource::URLInputSource( const XMLCh* const    baseId
                              , const XMLCh* const    systemId
                              , MemoryManager* const  manager) :
    InputSource(manager)
    , fURL(baseId, systemId)
{
    // Create a URL that will build up the full URL and store as the system id
    setSystemId(fURL.getURLText());
}

URLInputSource::URLInputSource( const XMLCh* const    baseId
                              , const XMLCh* const    systemId
                              , const XMLCh* const    publicId
                              , MemoryManager* const  manager) :
    InputSource(0, publicId, manager)
    , fURL(baseId, systemId)
{
    setSystemId(fURL.getURLText());
}

URLInputSource::URLInputSource( const XMLCh* const    baseId
                              , const char* const     systemId
                              , MemoryManager* const  manager) :
    InputSource(manager)
    , fURL(baseId, systemId)
{
    setSystemId(fURL.getURLText());
}

URLInputSource::URLInputSource( const   XMLCh* const   baseId
                                , const char* const    systemId
                                , const char* const    publicId
                                , MemoryManager* const  manager) :
    InputSource(0, publicId, manager)
    , fURL(baseId, systemId)
{
    setSystemId(fURL.getURLText());
}

URLInputSource::~URLInputSource()
{
}


// ---------------------------------------------------------------------------
//  URLInputSource: Implementation of the input source interface
// ---------------------------------------------------------------------------
BinInputStream* URLInputSource::makeStream() const
{
    // Ask the URL to create us an appropriate input stream
    return fURL.makeNewStream();
}

XERCES_CPP_NAMESPACE_END

