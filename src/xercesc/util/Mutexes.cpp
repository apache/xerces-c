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

/*
 * $Log$
 * Revision 1.3  2004/09/08 13:56:22  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.2  2002/11/04 15:22:04  tng
 * C++ Namespace Support.
 *
 * Revision 1.1.1.1  2002/02/01 22:22:11  peiyongz
 * sane_include
 *
 * Revision 1.4  2000/03/02 19:54:41  roddey
 * This checkin includes many changes done while waiting for the
 * 1.1.0 code to be finished. I can't list them all here, but a list is
 * available elsewhere.
 *
 * Revision 1.3  2000/02/06 07:48:02  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.2  1999/12/02 19:42:35  roddey
 * Got rid of attempts to fancy/schmanzy lazy eval mutex implementation and just
 * force anyone who needs a global/static mutex to lazy eval that themselves.
 *
 * Revision 1.1.1.1  1999/11/09 01:04:45  twl
 * Initial checkin
 *
 * Revision 1.2  1999/11/08 20:45:09  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */



// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/util/XercesDefs.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/Mutexes.hpp>

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  XMLMutex: Constructors and Destructor
// ---------------------------------------------------------------------------
XMLMutex::XMLMutex(MemoryManager* const manager) :

    fHandle(0)
{
    // Ask the per-platform driver to make us a mutex
    fHandle = XMLPlatformUtils::makeMutex(manager);
}


XMLMutex::~XMLMutex()
{
    if (fHandle)
    {
        XMLPlatformUtils::closeMutex(fHandle);
        fHandle = 0;
    }
}


// ---------------------------------------------------------------------------
//  XMLMutex: Lock control methods
// ---------------------------------------------------------------------------
void XMLMutex::lock()
{
    XMLPlatformUtils::lockMutex(fHandle);
}

void XMLMutex::unlock()
{
    XMLPlatformUtils::unlockMutex(fHandle);
}



// ---------------------------------------------------------------------------
//  XMLMutexLock: Constructors and Destructor
// ---------------------------------------------------------------------------
XMLMutexLock::XMLMutexLock(XMLMutex* const toLock) :

    fToLock(toLock)
{
    fToLock->lock();
}


XMLMutexLock::~XMLMutexLock()
{
    fToLock->unlock();
}

XERCES_CPP_NAMESPACE_END
