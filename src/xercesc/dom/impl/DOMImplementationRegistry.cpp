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
 * originally based on software copyright (c) 2002, International
 * Business Machines, Inc., http://www.ibm.com .  For more information
 * on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 */

/*
 * $Id$
 */

#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/RefVectorOf.hpp>
#include <xercesc/util/XMLRegisterCleanup.hpp>
#include <xercesc/dom/DOMImplementationRegistry.hpp>
#include <xercesc/dom/DOMImplementationSource.hpp>
#include <xercesc/dom/DOMImplementation.hpp>
#include "DOMImplementationImpl.hpp"

XERCES_CPP_NAMESPACE_BEGIN


// -----------------------------------------------------------------------
//  Static constants.  These are lazily initialized on first usage.
//                     (Static constructors can not be safely used because
//                      of order of initialization dependencies.)
// -----------------------------------------------------------------------
// Points to the singleton instance of a registry of DOMImplementationSource
static RefVectorOf<DOMImplementationSource>* gDOMImplSrcVector = 0;

//  Global mutex that is used to synchronize access to the vector
static XMLMutex* gDOMImplSrcVectorMutex = 0;


// -----------------------------------------------------------------------
//  Function prototypes for internally used functions.
// -----------------------------------------------------------------------
RefVectorOf<DOMImplementationSource>* getDOMImplSrcVector();
XMLMutex& getDOMImplSrcVectorMutex();


// -----------------------------------------------------------------------
//  Reset the static data
// -----------------------------------------------------------------------
static void reinitDOMImplSrcVector()
{
	delete gDOMImplSrcVector;
	gDOMImplSrcVector = 0;
}

static void reinitDOMImplSrcVectorMutex()
{
    delete gDOMImplSrcVectorMutex;
    gDOMImplSrcVectorMutex = 0;
}

// -----------------------------------------------------------------------
//  Get the static data
// -----------------------------------------------------------------------
RefVectorOf<DOMImplementationSource>* getDOMImplSrcVector()
{
    // Note: we are not synchronizing on creation since that caller is doing
    //       it (i.e. caller is locking a mutex before calling us)
    static XMLRegisterCleanup cleanupDOMImplSrcVector;
    if (!gDOMImplSrcVector)
    {
        gDOMImplSrcVector = new RefVectorOf<DOMImplementationSource>(3, false);
        cleanupDOMImplSrcVector.registerCleanup(reinitDOMImplSrcVector);
    }

    return gDOMImplSrcVector;
}

XMLMutex& getDOMImplSrcVectorMutex()
{
    static XMLRegisterCleanup cleanupDOMImplSrcVectorMutex;
    if (!gDOMImplSrcVectorMutex)
    {
        XMLMutexLock lock(XMLPlatformUtils::fgAtomicMutex);

        if (!gDOMImplSrcVectorMutex)
        {
            gDOMImplSrcVectorMutex = new XMLMutex;
            cleanupDOMImplSrcVectorMutex.registerCleanup(reinitDOMImplSrcVectorMutex);
        }
    }

    return *gDOMImplSrcVectorMutex;
}


// -----------------------------------------------------------------------
//  DOMImplementationRegistry Functions
// -----------------------------------------------------------------------
DOMImplementation *DOMImplementationRegistry::getDOMImplementation(const XMLCh* features) {

    XMLMutexLock lock(&getDOMImplSrcVectorMutex());

    unsigned int len = getDOMImplSrcVector()->size();

    // Put our defined source there
    if (len == 0)
        getDOMImplSrcVector()->addElement((DOMImplementationSource*)DOMImplementationImpl::getDOMImplementationImpl());

    len = getDOMImplSrcVector()->size();

    for (unsigned int i = len; i > 0; i--) {
        DOMImplementationSource* source = getDOMImplSrcVector()->elementAt(i-1);
        DOMImplementation* impl = source->getDOMImplementation(features);
        if (impl)
            return impl;
    }

    return 0;
}

void DOMImplementationRegistry::addSource (DOMImplementationSource* source) {
    XMLMutexLock lock(&getDOMImplSrcVectorMutex());
    getDOMImplSrcVector()->addElement(source);
}


XERCES_CPP_NAMESPACE_END

