/*
 * The Apache Software License, Version 1.1
 * 
 * Copyright (c) 1999-2000 The Apache Software Foundation.  All rights
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
 * originally based on software copyright (c) 1999, International
 * Business Machines, Inc., http://www.ibm.com .  For more information
 * on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 */

/*
 * $Log$
 * Revision 1.7  2000/03/24 19:50:29  roddey
 * Clear the 'initialized' flag when the termination call is made. Probably
 * not required technically, but...
 *
 * Revision 1.6  2000/03/02 19:54:44  roddey
 * This checkin includes many changes done while waiting for the
 * 1.1.0 code to be finished. I can't list them all here, but a list is
 * available elsewhere.
 *
 * Revision 1.5  2000/02/06 07:48:03  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.4  2000/01/19 00:56:59  roddey
 * Changes to get rid of dependence on old utils standard streams and to
 * get rid of the fgLibLocation stuff.
 *
 * Revision 1.3  1999/12/18 00:18:10  roddey
 * More changes to support the new, completely orthagonal support for
 * intrinsic encodings.
 *
 * Revision 1.2  1999/12/15 19:41:28  roddey
 * Support for the new transcoder system, where even intrinsic encodings are
 * done via the same transcoder abstraction as external ones.
 *
 * Revision 1.1.1.1  1999/11/09 01:04:53  twl
 * Initial checkin
 *
 * Revision 1.2  1999/11/08 20:45:11  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <util/XMLMsgLoader.hpp>
#include <util/Mutexes.hpp>
#include <util/PlatformUtils.hpp>
#include <util/RefVectorOf.hpp>
#include <util/TransService.hpp>
#include <util/XMLString.hpp>
#include <util/XMLNetAccessor.hpp>
#include <util/XMLUni.hpp>


// ---------------------------------------------------------------------------
//  Local data members
//
//  gSyncMutex
//      This is a mutex that will be used to synchronize access to some of
//      the static data of the platform utilities class and here locally.
// ---------------------------------------------------------------------------
static XMLMutex*                gSyncMutex = 0;
static RefVectorOf<XMLDeleter>  gLazyData(512);
static bool                     gInitFlag = false;


// ---------------------------------------------------------------------------
//  XMLPlatformUtils: Static Data Members
// ---------------------------------------------------------------------------
XMLNetAccessor*     XMLPlatformUtils::fgNetAccessor = 0;
XMLTransService*    XMLPlatformUtils::fgTransService = 0;



// ---------------------------------------------------------------------------
//  XMLPlatformUtils: Init/term methods
// ---------------------------------------------------------------------------
void XMLPlatformUtils::Initialize()
{
    //
    //  Make sure we haven't already been initialized. Note that this is not
    //  thread safe and is not intended for that. Its more for those COM
    //  like processes that cannot keep up with whether they have initialized
    //  us yet or not.
    //
    if (gInitFlag)
        return;
    gInitFlag = true;

    // Create the local sync mutex
    gSyncMutex = new XMLMutex;

    //
    //  Call the platform init method, which is implemented in each of the
    //  per-platform implementation cpp files. This one does the very low
    //  level per-platform setup. It cannot use any XML util services at all,
    //  i.e. only native services.
    //
    platformInit();

    //
    //  Ask the per-platform code to make the desired transcoding service for
    //  us to use. This call cannot throw any exceptions or do anything that
    //  cause any transcoding to happen. It should create the service and
    //  return it or zero if it cannot.
    //
    //  This one also cannot use any utility services. It can only create a
    //  transcoding service object and return it.
    //
    //  If we cannot make one, then we call panic to end the process.
    //
    fgTransService = makeTransService();
    if (!fgTransService)
        panic(Panic_NoTransService);

    // Initialize the transcoder service
    fgTransService->initTransService();

    //
    //  Try to create a default local code page transcoder. This is the one
    //  that will be used internally by the XMLString class. If we cannot
    //  create one, then call the panic method.
    //
    XMLLCPTranscoder* defXCode = XMLPlatformUtils::fgTransService->makeNewLCPTranscoder();
    if (!defXCode)
        panic(Panic_NoDefTranscoder);
    XMLString::initString(defXCode);

    //
    //  Now lets ask the per-platform code to give us an instance of the type
    //  of network access implementation he wants to use. This can return
    //  a zero pointer if this platform doesn't want to support this.
    //
    fgNetAccessor = makeNetAccessor();
}


void XMLPlatformUtils::Terminate()
{
    // Delete any net accessor that got installed
    delete fgNetAccessor;
    fgNetAccessor = 0;

    //
    //  Call the method that cleans up the registered lazy eval objects.
    //  This is all higher level code which might try to report errors if
    //  they have problems, so we want to do this before we hose our
    //  fundamental bits and pieces.
    //
    cleanupLazyData();

    //
    //  Call some other internal modules to give them a chance to clean up.
    //  Do the string class last in case something tries to use it during
    //  cleanup.
    //
    XMLString::termString();

    // Clean up the the transcoding service
    delete fgTransService;
    fgTransService = 0;

    // Clean up the sync mutex
    delete gSyncMutex;
    gSyncMutex = 0;

    //
    //  And do platform termination. This cannot do use any XML services
    //  at all, it can only clean up local stuff. It it reports an error,
    //  it cannot use any XML exception or error reporting services.
    //
    platformTerm();

    // And say we are no longer initialized
    gInitFlag = false;
}


// ---------------------------------------------------------------------------
//  XMLPlatformUtils: Msg support methods
// ---------------------------------------------------------------------------
XMLMsgLoader* XMLPlatformUtils::loadMsgSet(const XMLCh* const msgDomain)
{
    //
    //  Ask the platform support to load up the correct type of message
    //  loader for the indicated message set. We don't check here whether it
    //  works or not. That's their decision.
    //
    return loadAMsgSet(msgDomain);
}


// ---------------------------------------------------------------------------
//  XMLPlatformUtils: Lazy eval methods
// ---------------------------------------------------------------------------
void XMLPlatformUtils::registerLazyData(XMLDeleter* const deleter)
{
    // Just add a copy of this object to the vector. MUST be synchronized
    XMLMutexLock lock(gSyncMutex);
    gLazyData.addElement(deleter);
}


void XMLPlatformUtils::cleanupLazyData()
{
    //
    //  Loop through the vector and try to delete each object. Use a try
    //  block so that we don't fall out prematurely if one of them hoses.
    //  Note that we effectively do them in reverse order of their having
    //  been registered.
    //
    //  Also, note that we don't synchronize here because this is happening
    //  during shutdown.
    //
    while (gLazyData.size())
    {
        try
        {
            gLazyData.removeLastElement();
        }

        catch(...)
        {
            // We don't try to report errors here, just fall through
        }
    }
}
