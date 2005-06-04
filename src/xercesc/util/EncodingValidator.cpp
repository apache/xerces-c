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
 * Revision 1.8  2004/10/20 15:18:35  knoaman
 * Allow option of initializing static data in XMLPlatformUtils::Initialize
 *
 * Revision 1.7  2004/09/08 13:56:21  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.6  2004/07/22 15:37:18  knoaman
 * Use file static instance instead of local static instance
 *
 * Revision 1.5  2004/01/13 16:17:10  knoaman
 * Fo sanity, use class name to qualify method
 *
 * Revision 1.4  2004/01/09 22:41:58  knoaman
 * Use a global static mutex for locking when creating local static mutexes instead of compareAndSwap
 *
 * Revision 1.3  2003/03/04 21:11:12  knoaman
 * [Bug 17516] Thread safety problems in ../util/ and ../util/regx.
 *
 * Revision 1.2  2002/11/04 15:22:03  tng
 * C++ Namespace Support.
 *
 * Revision 1.1  2002/07/18 20:08:16  knoaman
 * Initial checkin: feature to control strict IANA encoding name.
 *
 */

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/util/EncodingValidator.hpp>
#include <xercesc/internal/IANAEncodings.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLRegisterCleanup.hpp>
#include <xercesc/util/XMLInitializer.hpp>

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  Local static data
// ---------------------------------------------------------------------------
static XMLMutex* sEncValMutex = 0;
static XMLRegisterCleanup encValRegistryCleanup;
static XMLRegisterCleanup instanceCleanup;

// ---------------------------------------------------------------------------
//  Local, static functions
// ---------------------------------------------------------------------------
static void reinitEncValMutex()
{
    delete sEncValMutex;
    sEncValMutex = 0;
}

static XMLMutex& getEncValMutex()
{
    if (!sEncValMutex)
    {
        XMLMutexLock lock(XMLPlatformUtils::fgAtomicMutex);

        // If we got here first, then register it and set the registered flag
        if (!sEncValMutex)
        {
            sEncValMutex = new XMLMutex(XMLPlatformUtils::fgMemoryManager);
            encValRegistryCleanup.registerCleanup(reinitEncValMutex);
        }
    }
    return *sEncValMutex;
}

void XMLInitializer::initializeEncodingValidator()
{
    EncodingValidator::fInstance = new EncodingValidator();
    if (EncodingValidator::fInstance) {
        instanceCleanup.registerCleanup(EncodingValidator::reinitInstance);
    }
}

// ---------------------------------------------------------------------------
//  Static member data initialization
// ---------------------------------------------------------------------------
EncodingValidator* EncodingValidator::fInstance = 0;


// ---------------------------------------------------------------------------
//  EncodingValidator: Constructors and Destructor
// ---------------------------------------------------------------------------
EncodingValidator::EncodingValidator() :
    fEncodingRegistry(0)
{
    initializeRegistry();
}

EncodingValidator::~EncodingValidator() {

    delete fEncodingRegistry;
    fEncodingRegistry = 0;
}

// ---------------------------------------------------------------------------
//  EncodingValidator: Validation methods
// ---------------------------------------------------------------------------
bool EncodingValidator::isValidEncoding(const XMLCh* const encName) {

    if (fEncodingRegistry->containsKey(encName))
		return true;

	return false;
}


// ---------------------------------------------------------------------------
//  EncodingValidator: Initialization methods
// ---------------------------------------------------------------------------
void EncodingValidator::initializeRegistry() {

    fEncodingRegistry = new ValueHashTableOf<bool>(109);

    for (unsigned int i=0; i < gEncodingArraySize; i++) {
        fEncodingRegistry->put((void*) gEncodingArray[i], true);
    }
}


// ---------------------------------------------------------------------------
//  EncodingValidator: Instance methods
// ---------------------------------------------------------------------------
EncodingValidator* EncodingValidator::instance()
{
    if (!fInstance)
    {
        XMLMutexLock lock(&getEncValMutex());

        if (!fInstance)
        { 
            fInstance = new EncodingValidator();
            instanceCleanup.registerCleanup(EncodingValidator::reinitInstance);
        }
    }

    return (fInstance);
}

// -----------------------------------------------------------------------
//  Notification that lazy data has been deleted
// -----------------------------------------------------------------------
void EncodingValidator::reinitInstance() {

	delete fInstance;
	fInstance = 0;
}

XERCES_CPP_NAMESPACE_END

/**
  * End of file EncodingValidator.cpp
  */
