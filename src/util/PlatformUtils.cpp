/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 1999-2001 The Apache Software Foundation.  All rights
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
#include <util/XMLMsgLoader.hpp>
#include <util/Mutexes.hpp>
#include <util/PlatformUtils.hpp>
#include <util/RefVectorOf.hpp>
#include <util/TransService.hpp>
#include <util/XMLString.hpp>
#include <util/XMLNetAccessor.hpp>
#include <util/XMLUni.hpp>
#include <internal/XMLReader.hpp>
#include <util/RuntimeException.hpp>
#include <util/XMLRegisterCleanup.hpp>
#include <limits.h>


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
XMLMutex*			gXMLCleanupListMutex = 0;


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

    //
    //  Call the platform init method, which is implemented in each of the
    //  per-platform implementation cpp files. This one does the very low
    //  level per-platform setup. It cannot use any XML util services at all,
    //  i.e. only native services.
    //
    platformInit();

    // Create the local sync mutex
    gSyncMutex = new XMLMutex;

	// Create the mutex for the static data cleanup list
    gXMLCleanupListMutex = new XMLMutex;

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
void XMLPlatformUtils::recognizeNEL(bool state) {

    //Make sure initialize has been called
    if (gInitFlag == 0) {
        return;
    }

    if (state) {

        if (!XMLReader::isNELRecognized()) {
            XMLReader::enableNELWS();
        }
    }
    else {

        if (XMLReader::isNELRecognized()) {
            ThrowXML(RuntimeException, XMLExcepts::NEL_RepeatedCalls);
        }
    }
}


bool XMLPlatformUtils::isNELRecognized() {

    return XMLReader::isNELRecognized();
}

