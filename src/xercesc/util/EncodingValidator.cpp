/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 2002 The Apache Software Foundation.  All rights
 * reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. The end-user documentation included with the redistribution,
 *    if any, must include the following acknowledgment:
 *       "This product includes software developed by the
 *        Apache Software Foundation (http://www.apache.org/)."
 *    Alternately, this acknowledgment may appear in the software itself,
 *    if and wherever such third-party acknowledgments normally appear.
 *
 * 4. The names "Xerces" and "Apache Software Foundation" must
 *    not be used to endorse or promote products derived from this
 *    software without prior written permission. For written
 *    permission, please contact apache\@apache.org.
 *
 * 5. Products derived from this software may not be called "Apache",
 *    nor may "Apache" appear in their name, without prior written
 *    permission of the Apache Software Foundation.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE APACHE SOFTWARE FOUNDATION OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * ====================================================================
 *
 * This software consists of voluntary contributions made by many
 * individuals on behalf of the Apache Software Foundation, and was
 * originally based on software copyright (c) 2001, International
 * Business Machines, Inc., http://www.ibm.com .  For more information
 * on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 */

/*
 * $Log$
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
            sEncValMutex = new XMLMutex;
            encValRegistryCleanup.registerCleanup(reinitEncValMutex);
        }
    }
    return *sEncValMutex;
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
