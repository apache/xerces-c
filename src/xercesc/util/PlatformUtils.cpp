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
 * Revision 1.19  2004/10/20 15:18:35  knoaman
 * Allow option of initializing static data in XMLPlatformUtils::Initialize
 *
 * Revision 1.18  2004/09/08 13:56:22  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.17  2004/01/09 04:39:56  knoaman
 * Use a global static mutex for locking when creating local static mutexes instead of compareAndSwap.
 *
 * Revision 1.16  2003/12/17 00:18:35  cargilld
 * Update to memory management so that the static memory manager (one used to call Initialize) is only for static data.
 *
 * Revision 1.15  2003/12/03 17:30:19  neilg
 * uninitialize panic handlers so they will be ready for subsequent initalizations
 *
 * Revision 1.14  2003/11/24 18:07:41  knoaman
 * Add missing ';'
 *
 * Revision 1.13  2003/11/24 17:07:53  knoaman
 * Reset memory manager in Terminate
 *
 * Revision 1.12  2003/11/17 14:50:29  knoaman
 * Fix for bug 23930 by Ronald  Landheer-Cieslak.
 *
 * Revision 1.11  2003/08/27 16:41:56  jberry
 * Add new static global that always points to array-allocating memory manager
 *
 * Revision 1.10  2003/05/18 14:02:05  knoaman
 * Memory manager implementation: pass per instance manager.
 *
 * Revision 1.9  2003/04/30 16:54:38  knoaman
 * Initialize xerces with a default memory manager if none is provided.
 *
 * Revision 1.8  2003/03/09 16:40:47  peiyongz
 * PanicHandler
 *
 * Revision 1.7  2003/02/17 19:54:47  peiyongz
 * Allow set user specified error message file location in PlatformUtils::Initialize().
 *
 * Revision 1.6  2002/12/20 22:10:20  tng
 * XML 1.1
 *
 * Revision 1.5  2002/11/08 21:06:16  peiyongz
 * move comment to *hpp for API doc generation
 *
 * Revision 1.4  2002/11/04 22:24:21  peiyongz
 * Locale setting for message loader
 *
 * Revision 1.3  2002/11/04 15:22:04  tng
 * C++ Namespace Support.
 *
 * Revision 1.2  2002/07/18 20:05:31  knoaman
 * Add a new feature to control strict IANA encoding name.
 *
 * Revision 1.1.1.1  2002/02/01 22:22:11  peiyongz
 * sane_include
 *
 * Revision 1.16  2001/10/25 21:52:28  peiyongz
 * XMLDeleterFor related functions and data are removed.
 *
 * Revision 1.15  2001/10/24 19:59:49  peiyongz
 * [Bug 3975] gInitFlag problem
 *
 * Revision 1.14  2001/10/23 23:09:32  peiyongz
 * [Bug#880] patch to PlatformUtils:init()/term() and related. from Mark Weaver
 *
 * Revision 1.13  2001/05/11 13:26:27  tng
 * Copyright update.
 *
 * Revision 1.12  2001/05/03 18:43:01  knoaman
 * Added new option to the parsers so that the NEL (0x85) char can be treated as a newline character.
 *
 * Revision 1.11  2001/02/07 17:46:34  billsch
 * Rearranged statements in Initialize() so that platformInit() is called
 * before an XMLMutex is created.
 *
 * Revision 1.10  2000/07/25 20:55:23  jpolast
 * use gInitFlag as a reference to the number of times
 * Initialized was called.  this way, the terminate routines are
 * not invoked until all processes call Terminate() on the parser.
 *
 * Revision 1.9  2000/06/26 20:30:04  jpolast
 * check if initialized in Terminate() to stop access violations
 * submitted by John_Roper@iOra.com
 *
 * Revision 1.8  2000/05/09 00:22:40  andyh
 * Memory Cleanup.  XMLPlatformUtils::Terminate() deletes all lazily
 * allocated memory; memory leak checking tools will no longer report
 * that leaks exist.  (DOM GetElementsByTagID temporarily removed
 * as part of this.)
 *
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
#include <xercesc/util/XMLMsgLoader.hpp>
#include <xercesc/util/Mutexes.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/RefVectorOf.hpp>
#include <xercesc/util/TransService.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/XMLNetAccessor.hpp>
#include <xercesc/util/XMLUni.hpp>
#include <xercesc/internal/XMLReader.hpp>
#include <xercesc/util/RuntimeException.hpp>
#include <xercesc/util/XMLRegisterCleanup.hpp>
#include <xercesc/util/DefaultPanicHandler.hpp>
#include <xercesc/util/XMLInitializer.hpp>
#include <xercesc/internal/MemoryManagerImpl.hpp>
#include <xercesc/internal/MemoryManagerArrayImpl.hpp>

#include <limits.h>

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  Local data members
//
//  gSyncMutex
//      This is a mutex that will be used to synchronize access to some of
//      the static data of the platform utilities class and here locally.
// ---------------------------------------------------------------------------
static XMLMutex*                gSyncMutex = 0;
static long                     gInitFlag = 0;

// ---------------------------------------------------------------------------
//  Global data
//
//	gXMLCleanupList
//		This is a list of cleanup functions to be called on
//		XMLPlatformUtils::Terminate.  Their function is to reset static
//		data in classes that use it.
//
//	gXMLCleanupListMutex
//		This is a mutex that will be used to synchronise access to the global
//		static data cleanup list
// ---------------------------------------------------------------------------
XMLRegisterCleanup*	gXMLCleanupList = 0;
XMLMutex*           gXMLCleanupListMutex = 0;


// ---------------------------------------------------------------------------
//  XMLPlatformUtils: Static Data Members
// ---------------------------------------------------------------------------
XMLNetAccessor*         XMLPlatformUtils::fgNetAccessor = 0;
XMLTransService*        XMLPlatformUtils::fgTransService = 0;
PanicHandler*           XMLPlatformUtils::fgUserPanicHandler = 0;
PanicHandler*           XMLPlatformUtils::fgDefaultPanicHandler = 0;
MemoryManager*          XMLPlatformUtils::fgMemoryManager = 0;
MemoryManagerArrayImpl  gArrayMemoryManager;
MemoryManager*          XMLPlatformUtils::fgArrayMemoryManager = &gArrayMemoryManager;
bool                    XMLPlatformUtils::fgMemMgrAdopted = true;
XMLMutex*               XMLPlatformUtils::fgAtomicMutex = 0;

// ---------------------------------------------------------------------------
//  XMLPlatformUtils: Init/term methods
// ---------------------------------------------------------------------------
void XMLPlatformUtils::Initialize(const char*          const locale 
                                , const char*          const nlsHome
                                ,       PanicHandler*  const panicHandler
                                ,       MemoryManager* const memoryManager
                                ,       bool                 toInitStatics)
{
    //
    //  Effects of overflow:
    //  . resouce re-allocations
    //  . consequently resource leaks
    //  . potentially terminate() may never get executed
    //
    //  We got to prevent overflow from happening.
    //  no error or exception
    //
    if (gInitFlag == LONG_MAX)
        return;
	
    //
    //  Make sure we haven't already been initialized. Note that this is not
    //  thread safe and is not intended for that. Its more for those COM
    //  like processes that cannot keep up with whether they have initialized
    //  us yet or not.
    //
    gInitFlag++;

    if (gInitFlag > 1)
      return;

    // Set pluggable memory manager
    if (!fgMemoryManager)
    {
        if (memoryManager)
        {
            fgMemoryManager = memoryManager;
            fgMemMgrAdopted = false;
        }
        else
        {
            fgMemoryManager = new MemoryManagerImpl();
        }
    }

    /***
     * Panic Handler:
     *
     ***/
    if (!panicHandler)
    {
        fgDefaultPanicHandler = new DefaultPanicHandler();
    }
    else
    {
        fgUserPanicHandler = panicHandler;
    }
    
    //
    //  Call the platform init method, which is implemented in each of the
    //  per-platform implementation cpp files. This one does the very low
    //  level per-platform setup. It cannot use any XML util services at all,
    //  i.e. only native services.
    //
    platformInit();

    // Create the local sync mutex
    gSyncMutex = new XMLMutex(fgMemoryManager);

	// Create the mutex for the static data cleanup list
    gXMLCleanupListMutex = new XMLMutex(fgMemoryManager);
    fgAtomicMutex = new XMLMutex(fgMemoryManager);

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
        panic(PanicHandler::Panic_NoTransService);

    // Initialize the transcoder service
    fgTransService->initTransService();

    //
    //  Try to create a default local code page transcoder. This is the one
    //  that will be used internally by the XMLString class. If we cannot
    //  create one, then call the panic method.
    //
    XMLLCPTranscoder* defXCode = XMLPlatformUtils::fgTransService->makeNewLCPTranscoder();
    if (!defXCode)
        panic(PanicHandler::Panic_NoDefTranscoder);
    XMLString::initString(defXCode, fgMemoryManager);

    //
    //  Now lets ask the per-platform code to give us an instance of the type
    //  of network access implementation he wants to use. This can return
    //  a zero pointer if this platform doesn't want to support this.
    //
    fgNetAccessor = makeNetAccessor();

    /***
     * Message Loader:
     *
     *     Locale setting 
     *     nlsHome setting 
     ***/
    XMLMsgLoader::setLocale(locale);
    XMLMsgLoader::setNLSHome(nlsHome);

    if (toInitStatics) {
        XMLInitializer::InitializeAllStaticData();
    }
}


void XMLPlatformUtils::Terminate()
{
    //
    // To prevent it from running underflow.
    // otherwise we come to delete non-existing resources.
    //
    //  no error or exception
    //
    if (gInitFlag == 0)
        return;

	gInitFlag--;
	
	if (gInitFlag > 0)
		return;

    // Delete any net accessor that got installed
    delete fgNetAccessor;
    fgNetAccessor = 0;

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

    // Clean up mutex
    delete fgAtomicMutex;
    fgAtomicMutex = 0;

	// Clean up statically allocated, lazily cleaned data in each class
	// that has registered for it.
	// Note that calling doCleanup() also unregisters the cleanup
	// function, so that we are chewing the list down to nothing here
	while (gXMLCleanupList)
		gXMLCleanupList->doCleanup();

	// Clean up the mutex for accessing gXMLCleanupList
	delete gXMLCleanupListMutex;
	gXMLCleanupListMutex = 0;

    //
    //  And do platform termination. This cannot do use any XML services
    //  at all, it can only clean up local stuff. It it reports an error,
    //  it cannot use any XML exception or error reporting services.
    //
    platformTerm();

    /***
     *  de-allocate resource
     *
     *  refer to discussion in the Initialize()
     ***/
    XMLMsgLoader::setLocale(0);
    XMLMsgLoader::setNLSHome(0);

    if (fgDefaultPanicHandler)
    {
        delete fgDefaultPanicHandler;
    }
    fgDefaultPanicHandler = 0;
    fgUserPanicHandler = 0;

    // de-allocate default memory manager
    if (fgMemMgrAdopted)
        delete fgMemoryManager;
    else
        fgMemMgrAdopted = true;

    // set memory manager to 0
    fgMemoryManager = 0;

    // And say we are no longer initialized
    gInitFlag = 0;
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
//  XMLPlatformUtils: NEL Character Handling
// ---------------------------------------------------------------------------
void XMLPlatformUtils::recognizeNEL(bool state, MemoryManager* const manager) {

    //Make sure initialize has been called
    if (gInitFlag == 0) {
        return;
    }

    if (state) {

        if (!XMLChar1_0::isNELRecognized()) {
            XMLChar1_0::enableNELWS();
        }
    }
    else {

        if (XMLChar1_0::isNELRecognized()) {
            ThrowXMLwithMemMgr(RuntimeException, XMLExcepts::NEL_RepeatedCalls, manager);
        }
    }
}


bool XMLPlatformUtils::isNELRecognized() {

    return XMLChar1_0::isNELRecognized();
}

// ---------------------------------------------------------------------------
//  XMLPlatformUtils: IANA Encoding checking setting
// ---------------------------------------------------------------------------
void XMLPlatformUtils::strictIANAEncoding(const bool state) {

    //Make sure initialize has been called
    if (gInitFlag == 0) {
        return;
    }

    fgTransService->strictIANAEncoding(state);
}


bool XMLPlatformUtils::isStrictIANAEncoding() {

    if (gInitFlag)
        return fgTransService->isStrictIANAEncoding();

    return false;
}

XERCES_CPP_NAMESPACE_END
