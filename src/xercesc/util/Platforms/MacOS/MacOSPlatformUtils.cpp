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
 * $Id$
 */


// ---------------------------------------------------------------------------
// Note:
//   Xerces is not officially supported on Macintosh. This file was sent
//   in by one of the Macintosh users and is included in the distribution
//   just for convenience. Please send any defects / modification
//   reports to xml4c@us.ibm.com
// ---------------------------------------------------------------------------


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/util/Janitor.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/RuntimeException.hpp>
#include <xercesc/util/XMLUniDefs.hpp>
#include <xercesc/util/XMLUni.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/Platforms/MacOS/MacOSDefs.hpp>
#include <xercesc/util/Platforms/MacOS/MacOSPlatformUtils.hpp>

#if (defined(XML_USE_INMEMORY_MSGLOADER) || defined(XML_USE_INMEM_MESSAGELOADER))
   #include <xercesc/util/MsgLoaders/InMemory/InMemMsgLoader.hpp>
#endif
#if (defined(XML_USE_MACOS_UNICODECONVERTER) || defined(XML_USE_NATIVE_TRANSCODER))
   #include <xercesc/util/Transcoders/MacOSUnicodeConverter/MacOSUnicodeConverter.hpp>
#endif
#if (defined(XML_USE_NETACCESSOR_URLACCESS) || defined(XML_USE_NETACCESSOR_NATIVE))
   #include <xercesc/util/NetAccessors/MacOSURLAccess/MacOSURLAccess.hpp>
#endif

#include <cstring>
#include <cstdlib>
#include <cctype>
#include <cstdio>
#include <memory>
#include <algorithm>

#if defined(XML_MACOSX)
    //	Include from Frameworks Headers under ProjectBuilder
    #include <Carbon/Carbon.h>
#else
    //	Classic include styles
    #include <Files.h>
    #include <Gestalt.h>
    #include <TextUtils.h>
    #include <TextEncodingConverter.h>
    #include <Multiprocessing.h>
    #include <DriverSynchronization.h>
    #include <DriverServices.h>
    #include <CFString.h>
    #include <URLAccess.h>
#endif



//----------------------------------------------------------------------------
//	Local Constants
//----------------------------------------------------------------------------
const std::size_t kMaxStaticPathChars = 512;		// Size of our statically allocated path buffers


//----------------------------------------------------------------------------
// Function Prototypes
//----------------------------------------------------------------------------
XMLCh*	ConvertColonToSlash(XMLCh* p, std::size_t charCount);
XMLCh*	ConvertSlashToColon(XMLCh* p, std::size_t charCount);
char*	ConvertSlashToColon(char* p, std::size_t charCount);

XMLCh*	XMLCreateFullPathFromFSRef_X(const FSRef& startingRef);
XMLCh*	XMLCreateFullPathFromFSRef_Classic(const FSRef& startingRef);
XMLCh*	XMLCreateFullPathFromFSSpec_Classic(const FSSpec& startingSpec);
bool	XMLParsePathToFSRef_X(const XMLCh* const pathName, FSRef& ref);
bool	XMLParsePathToFSRef_Classic(const XMLCh* const pathName, FSRef& ref);
bool	XMLParsePathToFSSpec_Classic(const XMLCh* const pathName, FSSpec& spec);

std::size_t TranscodeUniCharsToUTF8(UniChar* src, char* dst, std::size_t srcCnt, std::size_t maxChars);
std::size_t TranscodeUTF8ToUniChars(char* src, UniChar* dst, std::size_t maxChars);


//----------------------------------------------------------------------------
//  Local Data
//
//	gIsClassic
//	 True if we're in the "classic" environment, either under the blue blox
//	 or on a real classic OS.
//
//  gFileSystemCompatible
//   This flag indicates whether the file system APIs meet our minimum
//   requirements.
//
// gHasFSSpecAPIs
//   True if the FSSpecAPIs are available. These are required.
//
// gHasF2TBAPIs
//   True if the FS supports 2 terrabyte calls. These are required for
//   use under Carbon.
//
// gHasHFSPlusAPIs
//   True if the FS supports HFSPlus APIs. If this is true, then the
//   new Fork calls are used, and all file name and path handling
//   uses long unicode names. Note that once a file is opened with
//   the fork calls, only fork calls may be used to access it.
//
// gHasFSPathAPIs
//   True if the FS supports posix path creation APIs FSPathMakeRef and
//	 FSRefMakePath. If so, these routines are used to support path creation
//	 and  interpretation.
//
// gHasMPAPIs
//	 True if the Multiprocessing APIs are available.
//----------------------------------------------------------------------------
static bool gIsClassic				= false;
static bool gFileSystemCompatible	= false;
static bool gHasFSSpecAPIs			= false;
static bool gHasFS2TBAPIs			= false;
static bool gHasHFSPlusAPIs			= false;
static bool gHasFSPathAPIs			= false;
static bool gHasMPAPIs				= false;


//----------------------------------------------------------------------------
// XMLMacFile methods
//----------------------------------------------------------------------------

unsigned int
XMLMacFile::currPos()
{
    OSErr err = noErr;
    unsigned int pos = 0;
    
    if (!mFileValid)
        ThrowXML(XMLPlatformUtilsException, XMLExcepts::File_CouldNotGetCurPos);
    
    if (gHasHFSPlusAPIs)
    {
        SInt64 bigPos = 0;
        err = FSGetForkPosition(mFileRefNum, &bigPos);
        if (err == noErr)
            pos = bigPos;
    }
    else
    {
        long longPos;
        err = GetFPos(mFileRefNum, &longPos);
        if (err == noErr)
            pos = longPos;
    }
    
    if (err != noErr)
        ThrowXML(XMLPlatformUtilsException, XMLExcepts::File_CouldNotGetCurPos);
    
    return pos;
}


void
XMLMacFile::close()
{
    OSErr err = noErr;
    if (!mFileValid)
        ThrowXML(XMLPlatformUtilsException, XMLExcepts::File_CouldNotCloseFile);
    
    if (gHasHFSPlusAPIs)
        err = FSCloseFork(mFileRefNum);
    else
        err = FSClose(mFileRefNum);
    
    mFileValid = false;
    
    if (err != noErr)
        ThrowXML(XMLPlatformUtilsException, XMLExcepts::File_CouldNotCloseFile);
}


unsigned int
XMLMacFile::size()
{
    OSErr err = noErr;
    unsigned int len = 0;
    
    if (!mFileValid)
        ThrowXML(XMLPlatformUtilsException, XMLExcepts::File_CouldNotGetSize);
    
    if (gHasHFSPlusAPIs)
    {
        SInt64 bigLen = 0;
        err = FSGetForkSize(mFileRefNum, &bigLen);
        if (err == noErr)
            len = bigLen;
    }
    else
    {
        long longLen;
        err = GetEOF(mFileRefNum, &longLen);
        if (err == noErr)
            len = longLen;
    }
    
    if (err != noErr)
        ThrowXML(XMLPlatformUtilsException, XMLExcepts::File_CouldNotGetSize);
    
    return len;
}


void
XMLMacFile::open(const XMLCh* const fileName)
{
    OSErr err = noErr;
    
    if (mFileValid)
        ThrowXML(XMLPlatformUtilsException, XMLExcepts::File_CouldNotOpenFile);
    
    if (gHasHFSPlusAPIs)
    {
        FSRef ref;
        if (!XMLParsePathToFSRef(fileName, ref))
            err = fnfErr;
        
        HFSUniStr255 forkName;
        if (err == noErr)
            err = FSGetDataForkName(&forkName);
        
        if (err == noErr)
            err = FSOpenFork(&ref, forkName.length, forkName.unicode, fsRdPerm, &mFileRefNum);
    }
    else
    {
        FSSpec spec;
        if (!XMLParsePathToFSSpec(fileName, spec))
            err = fnfErr;
        
        if (err == noErr)
            err = FSpOpenDF(&spec, fsRdPerm, &mFileRefNum);
    }
    
    if (err != noErr)
        ThrowXML1(XMLPlatformUtilsException, XMLExcepts::File_CouldNotOpenFile, fileName);
    
    mFileValid = true;
}


unsigned int
XMLMacFile::read(const unsigned int toRead, XMLByte* const toFill)
{
    unsigned int bytesRead = 0;
    OSErr err = noErr;
    
    if (!mFileValid)
        ThrowXML(XMLPlatformUtilsException, XMLExcepts::File_CouldNotReadFromFile);
    
    if (gHasHFSPlusAPIs)
    {
        ByteCount actualCount;
        err = FSReadFork(mFileRefNum, fsFromMark, 0, toRead, toFill, &actualCount);
        bytesRead = actualCount;
    }
    else
    {
        long byteCount = toRead;
        err = FSRead(mFileRefNum, &byteCount, toFill);
        bytesRead = byteCount;
    }
    
    if (err != noErr && err != eofErr)
        ThrowXML(XMLPlatformUtilsException, XMLExcepts::File_CouldNotReadFromFile);
    
    return bytesRead;
}


void
XMLMacFile::reset()
{
    OSErr err = noErr;
    
    if (!mFileValid)
        ThrowXML(XMLPlatformUtilsException, XMLExcepts::File_CouldNotResetFile);
    
    if (gHasHFSPlusAPIs)
        err = FSSetForkPosition(mFileRefNum, fsFromStart, 0);
    else
        err = SetFPos(mFileRefNum, fsFromStart, 0);
    
    if (err != noErr)
        ThrowXML(XMLPlatformUtilsException, XMLExcepts::File_CouldNotResetFile);
}


XMLMacFile::~XMLMacFile()
{
    if (mFileValid)
        close();
}


// ---------------------------------------------------------------------------
//  XMLPlatformUtils: The panic method
// ---------------------------------------------------------------------------
void
XMLPlatformUtils::panic(const PanicReasons reason)
{
    const char* reasonStr = "Unknown reason";
    if (reason == Panic_NoTransService)
        reasonStr = "Could not load a transcoding service";
    else if (reason == Panic_NoDefTranscoder)
        reasonStr = "Could not load a local code page transcoder";
    else if (reason == Panic_CantFindLib)
        reasonStr = "Could not find the xerces-c DLL";
    else if (reason == Panic_UnknownMsgDomain)
        reasonStr = "Unknown message domain";
    else if (reason == Panic_CantLoadMsgDomain)
        reasonStr = "Cannot load message domain";
    else if (reason == Panic_SynchronizationErr)
        reasonStr = "A system synchronization error occurred";
    else if (reason == Panic_SystemInit)
        reasonStr = "Failed to complete platform dependent initialization";
    else
        reasonStr = "Unknown error source";
    
    char text[256];
    std::snprintf(text, sizeof(text), "Xerces Panic Error: %s", reasonStr);
    
    //
    //  The default handling of panics is not very friendly.
    //	To replace it with something more friendly, you'll need to:
    //	- #define XML_USE_CUSTOM_PANIC_PROC
    //	- Write, and link with, XMLCustomPanicProc
    //	- Implement your panic handling within XMLCustomPanicProc.
    //
#if defined(XML_USE_CUSTOM_PANIC_PROC)
    XMLCustomPanicProc(reason, reasonStr);
#else
    Str255 pasText;
    CopyCStringToPascal(text, pasText);
    DebugStr(pasText);
#endif
    
    //	Life's got us down. Good-bye world.
    std::exit(-1);
}


// ---------------------------------------------------------------------------
//  XMLPlatformUtils: File Methods
// ---------------------------------------------------------------------------
unsigned int
XMLPlatformUtils::curFilePos(const FileHandle theFile)
{
    return theFile->currPos();
}

void
XMLPlatformUtils::closeFile(const FileHandle theFile)
{
    theFile->close();
}

unsigned int
XMLPlatformUtils::fileSize(const FileHandle theFile)
{
    return theFile->size();
}


FileHandle
XMLPlatformUtils::openFile(const char* const fileName)
{
    const XMLCh* xmlPath = XMLString::transcode(fileName);
    ArrayJanitor<const XMLCh> jan(xmlPath);
    return openFile(xmlPath);
}


FileHandle
XMLPlatformUtils::openFile(const XMLCh* const fileName)
{
    // Check to make sure the file system is in a state where we can use it
    if (!gFileSystemCompatible)
        ThrowXML(XMLPlatformUtilsException, XMLExcepts::File_CouldNotOpenFile);
    
    XMLMacFile* file = new XMLMacFile();
    Janitor<XMLMacAbstractFile> janFile(file);
    file->open(fileName);
    janFile.orphan();
    
    return file;
}


FileHandle
XMLPlatformUtils::openStdInHandle()
{
    ThrowXML(XMLPlatformUtilsException, XMLExcepts::File_CouldNotOpenFile);
    return NULL;
}


unsigned int
XMLPlatformUtils::readFileBuffer(   const   FileHandle      theFile
                                 , const unsigned int    toRead
                                 ,       XMLByte* const  toFill)
{
    return theFile->read(toRead, toFill);
}


void
XMLPlatformUtils::resetFile(FileHandle theFile)
{
    theFile->reset();
}


// ---------------------------------------------------------------------------
//  XMLPlatformUtils: File system methods
// ---------------------------------------------------------------------------
XMLCh*
XMLPlatformUtils::getFullPath(const XMLCh* const srcPath)
{
    XMLCh* path = NULL;
    
    if (gHasHFSPlusAPIs)
    {
        FSRef ref;
        if (!XMLParsePathToFSRef(srcPath, ref) || (path = XMLCreateFullPathFromFSRef(ref)) == NULL)
            path = XMLString::replicate(srcPath);
    }
    else
    {
        FSSpec spec;
        if (!XMLParsePathToFSSpec(srcPath, spec) || (path = XMLCreateFullPathFromFSSpec(spec)) == NULL)
            path = XMLString::replicate(srcPath);
    }
    
    return path;
}


bool
XMLPlatformUtils::isRelative(const XMLCh* const toCheck)
{
    return (toCheck[0] != L'/');
}


XMLCh*
XMLPlatformUtils::weavePaths(const   XMLCh* const    basePath
                                    , const XMLCh* const    relativePath)
                                    
{
    // Code from Windows largely unmodified for the Macintosh,
    // with the exception of removing support for '\' path
    // separator.
    //
    // Note that there is no support currently for Macintosh
    // path separators ':'.
    
    // Create a buffer as large as both parts and empty it
    ArrayJanitor<XMLCh> tmpBuf(new XMLCh[XMLString::stringLen(basePath)
        + XMLString::stringLen(relativePath)
        + 2]);
    tmpBuf[0] = 0;
    
    //
    //  If we have no base path, then just take the relative path as
    //  is.
    //
    if (!basePath)
    {
        XMLString::copyString(tmpBuf.get(), relativePath);
        return tmpBuf.release();
    }
    
    if (!*basePath)
    {
        XMLString::copyString(tmpBuf.get(), relativePath);
        return tmpBuf.release();
    }
    
    const XMLCh* basePtr = basePath + (XMLString::stringLen(basePath) - 1);
    if (*basePtr != chForwardSlash)
    {
        while ((basePtr >= basePath) && (*basePtr != chForwardSlash))
            basePtr--;
    }
    
    // There is no relevant base path, so just take the relative part
    if (basePtr < basePath)
    {
        XMLString::copyString(tmpBuf.get(), relativePath);
        return tmpBuf.release();
    }
    
    //  We have some path part, so we need to check to see if we have to
    //  weave any of the parts together.
    const XMLCh* pathPtr = relativePath;
    
    while (true)
    {
        // If it does not start with some period, then we are done
        if (*pathPtr != chPeriod)
            break;
        
        unsigned int periodCount = 1;
        pathPtr++;
        if (*pathPtr == chPeriod)
        {
            pathPtr++;
            periodCount++;
        }
        
        // Has to be followed by a / or the null to mean anything
        if ((*pathPtr != chForwardSlash) &&  *pathPtr)
            break;
        
        if (*pathPtr)
            pathPtr++;
        
        // If it's one period, just eat it, else move backwards in the base
        if (periodCount == 2)
        {
            basePtr--;
            while ((basePtr >= basePath) && (*basePtr != chForwardSlash))
                basePtr--;
            
            // The base cannot provide enough levels, so it's in error/
            if (basePtr < basePath)
                ThrowXML(XMLPlatformUtilsException, XMLExcepts::File_BasePathUnderflow);
        }
    }
    
    // Copy the base part up to the base pointer
    XMLCh* bufPtr = tmpBuf.get();
    const XMLCh* tmpPtr = basePath;
    while (tmpPtr <= basePtr)
        *bufPtr++ = *tmpPtr++;
    
    // And then copy on the rest of our path
    XMLString::copyString(bufPtr, pathPtr);
    
    // Orphan the buffer and return it
    return tmpBuf.release();
}


// ---------------------------------------------------------------------------
//  XMLPlatformUtils: Timing Methods
// ---------------------------------------------------------------------------
unsigned long
XMLPlatformUtils::getCurrentMillis()
{
	if ((void*)kUnresolvedCFragSymbolAddress != UpTime)
	{
		// Use Driver services routines, now in Carbon,
		// to get the elapsed milliseconds.
		AbsoluteTime time = UpTime();
		return AbsoluteToDuration(time);
	}
	else
		return TickCount() * 100 / 6;
}


// ---------------------------------------------------------------------------
//  Mutex methods
//
// There are a number of choices for multi-threading on Mac OS. Traditionally
// there was the Thread Manager, which provided cooperative multitasking on
// 68K and PPC platforms, and preemptive multitasking on 68K platforms only.
// The primary threading model supported under Carbon is the Multiprocessing
// library, which as of version 2.0 provides a nice set of primitives. Under
// Mac OS X, the Multiprocessing library is a thin veneer over pthreads.
//
// For lack of any really good solutions, I've implemented these mutexes
// atop the Multiprocessing library. The critical regions employed here
// support recursive behavior, which is required by Xerces.
//
// Please note that, despite this implementation, there are probably other
// MacOS barriers to actually using Xerces in a multi-threaded environment.
// Many other parts of your system and/or development environment may not
// support pre-emption, even under Mac OS X. Examples may include the memory
// allocator, the Unicode Converter or Utilities, and perhaps the file
// system (though the FS is better with Multiprocessing as of System 9.0).
//
// These routines are provided somewhat speculatively, and with the philosphy
// that this code, at least, shouldn't be the reason why multithreading
// doesn't work. Compatibility of this library wrt the other areas described
// above will be resolved in time.
// ---------------------------------------------------------------------------

void*
XMLPlatformUtils::makeMutex()
{
	if (gHasMPAPIs)
	{
		MPCriticalRegionID criticalRegion = NULL;
		OSStatus status = MPCreateCriticalRegion(&criticalRegion);
		return (status == noErr) ? (void*)(criticalRegion) : NULL;
	}
	else
		return (void*)1;
}


void
XMLPlatformUtils::closeMutex(void* const mtxHandle)
{
	if (gHasMPAPIs)
	{
		MPCriticalRegionID criticalRegion = reinterpret_cast<MPCriticalRegionID>(mtxHandle);
		OSStatus status = MPDeleteCriticalRegion(criticalRegion);
		status = noErr;	// ignore any error and zap compiler warning
	}
	else
		;
}


void
XMLPlatformUtils::lockMutex(void* const mtxHandle)
{
	if (gHasMPAPIs)
	{
		MPCriticalRegionID criticalRegion = reinterpret_cast<MPCriticalRegionID>(mtxHandle);
		OSStatus status = MPEnterCriticalRegion(criticalRegion, kDurationForever);
		status = noErr;	// ignore any error and zap compiler warning
	}
	else
		;
}


void
XMLPlatformUtils::unlockMutex(void* const mtxHandle)
{
	if (gHasMPAPIs)
	{
		MPCriticalRegionID criticalRegion = reinterpret_cast<MPCriticalRegionID>(mtxHandle);
		OSStatus status = MPExitCriticalRegion(criticalRegion);
		status = noErr;	// ignore any error and zap compiler warning
	}
	else
		;
}


// ---------------------------------------------------------------------------
//  Miscellaneous synchronization methods
//
// Atomic manipulation is implemented atop routines that were traditionally
// part of DriverServices, but are now apparently a part of Carbon. If this
// selection proves to be invalid, similar routines in OpenTransport could
// be used instead.
// ---------------------------------------------------------------------------

void*
XMLPlatformUtils::compareAndSwap(       void**      toFill
                                 , const void* const newValue
                                 , const void* const toCompare)
{
    // Replace *toFill with newValue iff *toFill == toCompare,
    // returning previous value of *toFill
    
    Boolean success = CompareAndSwap(
        reinterpret_cast<UInt32>(toCompare),
        reinterpret_cast<UInt32>(newValue),
        reinterpret_cast<UInt32*>(toFill));
    
    return (success) ? const_cast<void*>(toCompare) : *toFill;
}


//
//	Atomic Increment and Decrement
//
//	Apple's routines return the value as it was before the
//	operation, while these routines want to return it as it
//	is after. So we perform the translation before returning
//	the value.
//
int
XMLPlatformUtils::atomicIncrement(int &location)
{
    return IncrementAtomic(reinterpret_cast<long*>(&location)) + 1;
}


int
XMLPlatformUtils::atomicDecrement(int &location)
{
    return DecrementAtomic(reinterpret_cast<long*>(&location)) - 1;
}


// ---------------------------------------------------------------------------
//  XMLPlatformUtils: Private Static Methods
// ---------------------------------------------------------------------------

//
//  This method handles the MacOS basic init functions.
//
void
XMLPlatformUtils::platformInit()
{
	//	Detect available functions by seeing if the symbols resolve
	
	long value = 0;
	gIsClassic				= ((void*)kUnresolvedCFragSymbolAddress != Gestalt)
								&& (noErr == Gestalt(gestaltSystemVersion, &value))
								&& (value < 0x01000)
								;
	
	gHasFSSpecAPIs			= ((void*)kUnresolvedCFragSymbolAddress != FSMakeFSSpec);
	gHasFS2TBAPIs			= ((void*)kUnresolvedCFragSymbolAddress != PBXGetVolInfoSync);
	gHasHFSPlusAPIs			= ((void*)kUnresolvedCFragSymbolAddress != FSOpenFork);
#if TARGET_API_MAC_CARBON
	gHasFSPathAPIs			= ((void*)kUnresolvedCFragSymbolAddress != FSPathMakeRef);
#else
	gHasFSPathAPIs			= false;
#endif
	gHasMPAPIs				= MPLibraryIsLoaded();
    
	//	We require FSSpecs at a minimum
    gFileSystemCompatible = gHasFSSpecAPIs;
}


//
//  This method handles the MacOS basic termination functions.
//
void
XMLPlatformUtils::platformTerm()
{
}


// ---------------------------------------------------------------------------
//  XMLPlatformUtils: Private Static Methods
// ---------------------------------------------------------------------------

//
//  This method is called by the platform independent part of this class
//  during initialization. We have to create the type of net accessor that
//  we want to use. If none, then just return zero.
//
XMLNetAccessor*
XMLPlatformUtils::makeNetAccessor()
{
#if (defined(XML_USE_NETACCESSOR_URLACCESS) || defined(XML_USE_NETACCESSOR_NATIVE))
	//	Only try to use URLAccess if it's actually available
	if (URLAccessAvailable())
		return new MacOSURLAccess;
#endif

	//	No netaccessor available--we can live with it, but you won't
	//	get net access.
	return 0;
}


//
//  This method is called by the platform independent part of this class
//  when client code asks to have one of the supported message sets loaded.
//
XMLMsgLoader*
XMLPlatformUtils::loadAMsgSet(const XMLCh* const msgDomain)
{
#if (defined(XML_USE_INMEMORY_MSGLOADER) || defined(XML_USE_INMEM_MESSAGELOADER))
    return new InMemMsgLoader(msgDomain);
#else
    #error You must provide a message loader
    return 0;
#endif
}


//
//  This method is called very early in the bootstrapping process. This guy
//  must create a transcoding service and return it. It cannot use any string
//  methods, any transcoding services, throw any exceptions, etc... It just
//  makes a transcoding service and returns it, or returns zero on failure.
//
XMLTransService*
XMLPlatformUtils::makeTransService()
{
#if (defined(XML_USE_MACOS_UNICODECONVERTER) || defined(XML_USE_NATIVE_TRANSCODER))
    if (MacOSUnicodeConverter::IsMacOSUnicodeConverterSupported())
        return new MacOSUnicodeConverter;
#else
    #error You must provide a transcoding service implementation
#endif

    //	If we got here it's because we didn't detect the Mac OS
    //	Unicode Converter or Text Encoding Converter routines
    //	that we require to function properly. Xerces will not
    //	survive this condition.
    return NULL;
}


// ---------------------------------------------------------------------------
//	Utility Functions
// ---------------------------------------------------------------------------

XMLCh*
CopyUniCharsToXMLChs(const UniChar* src, XMLCh* dst, std::size_t charCount, std::size_t maxChars)
{
	//	Ensure we don't step on anybody's toes
	std::size_t cnt = std::min(charCount, maxChars);
	
	//	Copy the characters. UniChar is unsigned, so we shouldn't have
	//	any sign extension problems.
	//	To allow copy within a identical range, we copy backwards,
	//	since XMLCh (may be) larger than UniChar.
	dst += cnt;
	src += cnt;
	for (; cnt > 0; --cnt)
		*--dst = *--src;
		
	return dst;
}


UniChar*
CopyXMLChsToUniChars(const XMLCh* src, UniChar* dst, std::size_t charCount, std::size_t maxChars)
{
	UniChar* dstBegin = dst;
	
	//	Ensure we don't step on anybody's toes
	std::size_t cnt = std::min(charCount, maxChars);
	
	//	Copy the characters. XMLCh's will be truncated on copy to UniChar's.
	//	To allow copy within a identical range, we copy forwards,
	//	since XMLCh (may be) larger than UniChar.
	for (; cnt > 0; --cnt)
		*dst++ = *src++;
		
	return dstBegin;
}


XMLCh*
ConvertColonToSlash(XMLCh* p, std::size_t charCount)
{
	XMLCh* start = p;
	for (; charCount > 0; --charCount)
	{
		XMLCh c = *p;
		if (c == ':')
			*p++ = '/';
		else
			p++;
	}
	return start;
}


XMLCh*
ConvertSlashToColon(XMLCh* p, std::size_t charCount)
{
	XMLCh* start = p;
	for (; charCount > 0; --charCount)
	{
		XMLCh c = *p;
		if (c == '/')
			*p++ = ':';
		else
			p++;
	}
	return start;
}


char*
ConvertSlashToColon(char* p, std::size_t charCount)
{
	char* start = p;
	for (; charCount > 0; --charCount)
	{
		char c = *p;
		if (c == '/')
			*p++ = ':';
		else
			p++;
	}
	return start;
}


bool
XMLParsePathToFSRef(const XMLCh* const pathName, FSRef& ref)
{
	bool result;
	
	//	If FSPathMakeRef is available, we use it to parse the
	//	path: this gives us "standard" path support under MacOS X.
	//	Without this, our paths in that environment would always
	//	have a volume name at their root...which would look
	//	"normal" to Mac users, but not normal to unix users. Since
	//	we're making "unix" paths, we'll stick with the unix
	//	style paths. This also allows us to easilly take paths
	//	off the command line.
	//
	//	FSPathMakeRef is available on Mac OS X and in CarbonLib 1.1
	//	and greater. But on classic under CarbonLib, it expects paths
	//	to contain ':' separators, for which we're not prepared. Since
	//	this isn't a case where we need to use it, we drop back to the
	//	classic case for this.
		
	if (TARGET_API_MAC_CARBON && !gIsClassic && gHasFSPathAPIs)
		result = XMLParsePathToFSRef_X(pathName, ref);
	else
		result = XMLParsePathToFSRef_Classic(pathName, ref);
		
	return result;
}


bool
XMLParsePathToFSRef_X(const XMLCh* const pathName, FSRef& ref)
{
	//	Parse Path to FSRef using FSPathMakeRef as available under
	//	Mac OS X and CarbonLib 1.1 and greater.
	
	OSStatus err = noErr;	
	std::size_t pathLen = XMLString::stringLen(pathName);

    //	Transcode XMLCh into UniChar
	UniChar uniBuf[kMaxStaticPathChars];
	CopyXMLChsToUniChars(pathName, uniBuf, pathLen, kMaxStaticPathChars);
	
	//	Transcode Unicode to UTF-8
	char utf8Buf[kMaxStaticPathChars];
	pathLen = TranscodeUniCharsToUTF8(uniBuf, utf8Buf, pathLen, kMaxStaticPathChars-1);
	
	//	Terminate the path
	char* p = utf8Buf;
	p[pathLen++] = '\0';
	
	//	If it's a relative path, pre-pend the current directory to the path.
	//	FSPathMakeRef doesn't deal with relativity on the front of the path
	if (*p == '.')
	{
		//	Right justify the user path to make room for the pre-pended path
		std::memmove(p + kMaxStaticPathChars - pathLen, p, pathLen);
				
		//	Get the current directory
        FSSpec spec;
		if (err == noErr)
			err = FSMakeFSSpec(0, 0, NULL, &spec);
        if (err == noErr)
            err = FSpMakeFSRef(&spec, &ref);
		
		//	Get pathname to the current directory
		if (err == noErr)
			err = FSRefMakePath(&ref, reinterpret_cast<UInt8*>(p), kMaxStaticPathChars - pathLen - 1);	// leave room for one '/'
		std::size_t prefixLen = std::strlen(p);
			
		//	Now munge the two paths back together
		if (err == noErr)
		{
			p[prefixLen++] = '/';
			std::memmove(p + prefixLen, p + kMaxStaticPathChars - pathLen, pathLen);
		}
		
		//	We now have a path from an absolute starting point
	}
	
	//	Let the OS discover the location
	Boolean isDirectory = false;
	if (err == noErr)
		err = FSPathMakeRef(reinterpret_cast<UInt8*>(p), &ref, &isDirectory);
		
	//	Return true on success
	return (err == noErr);
}


bool
XMLParsePathToFSRef_Classic(const XMLCh* const pathName, FSRef& ref)
{
	//	Parse Path to FSRef by stepping manually through path components.
	
	//	Path's parsed in this way must always begin with a volume name.
	//	This assumption would fail for standard unix paths under Mac OS X,
	//	so for those cases we use the routine XMLParsePathToFSRef_Carbon
	//	above.
	
    const XMLCh* p = pathName;
    const XMLCh* pEnd;
    std::size_t segLen;
	
	const std::size_t kXMLBufCount = 256;
	XMLCh xmlBuf[kXMLBufCount];
    
    OSErr err = noErr;
    
    if (*p == L'/')
    {
        // Absolute name: grab the first component as volume name
        
        // Find the end of the path segment
        for (pEnd = ++p; *pEnd && *pEnd != L'/'; ++pEnd) ;
        segLen = pEnd - p;
        
        // Try to find a volume that matches this name
        for (ItemCount volIndex = 1; err == noErr; ++volIndex)
        {
            HFSUniStr255 hfsStr;
            hfsStr.length = 0;
            
            // Get the volume name
            err = FSGetVolumeInfo(
                0,
                volIndex,
                static_cast<FSVolumeRefNum*>(NULL),
                0,
                static_cast<FSVolumeInfo*>(NULL),
                &hfsStr,
                &ref
                );
            
            // Compare against our path segment
            if (err == noErr && segLen == hfsStr.length)
            {
            	//	Case-insensitive compare
            	if (XMLString::compareNIString(
									ConvertSlashToColon(
										CopyUniCharsToXMLChs(hfsStr.unicode, xmlBuf, segLen, kXMLBufCount),
										segLen),
									p, segLen) == 0)
                    break;  // we found our volume
            }
        }
        
        p = pEnd;
    }
    else
    {
        // Relative name, so get the default directory as parent ref
        FSSpec spec;
        err = FSMakeFSSpec(0, 0, NULL, &spec);
        if (err == noErr)
            err = FSpMakeFSRef(&spec, &ref);
    }
    
    // ref now refers to the a parent directory: parse the rest of the path
    while (err == noErr && *p)
    {
        switch (*p)
        {
        case L'/':   // Just skip any number of path separators
            ++p;
            break;
            
        case L'.':   // Potentially "current directory" or "parent directory"
            if (p[1] == L'/' || p[1] == 0)       // "current directory"
            {
                ++p;
                break;
            }
            else if (p[1] == L'.' && (p[2] == L'/' || p[2] == 0)) // "parent directory"
            {
                p += 2;  // Get the parent of our parent
                
                FSCatalogInfo catalogInfo;
                err = FSGetCatalogInfo(
                    &ref,
                    kFSCatInfoParentDirID,
                    &catalogInfo,
                    static_cast<HFSUniStr255*>(NULL),
                    static_cast<FSSpec*>(NULL),
                    &ref
                    );
                
                // Check that we didn't go too far
                if (err != noErr || catalogInfo.parentDirID == fsRtParID)
                    return false;
                
                break;
            }
            else // some other sequence of periods...fall through and treat as segment
                ;
            
        default:
            // Find the end of the path segment
            for (pEnd = p; *pEnd && *pEnd != L'/'; ++pEnd) ;
            segLen = pEnd - p;
			
            // pEnd now points either to '/' or NUL
            // Create a new ref using this path segment
            err = FSMakeFSRefUnicode(
                &ref,
                segLen,
                ConvertColonToSlash(
                	CopyXMLChsToUniChars(p, reinterpret_cast<UniChar*>(xmlBuf), segLen, kXMLBufCount),
                	segLen),
                kTextEncodingUnknown,
                &ref
                );
            
            p = pEnd;
            break;
        }
    }
    
    return err == noErr;
}


bool
XMLParsePathToFSSpec(const XMLCh* const pathName, FSSpec& spec)
{
	//	Parse Path to an FSSpec

	//	If we've got HFS+ APIs, do this in terms of refs so that
	//	we can grandfather in the use of FSPathMakeRef under Mac OS X 
	//	and CarbonLib 1.1. Otherwise, do it the hard way.
	
	bool result = false;
	
	if (gHasHFSPlusAPIs)
	{
		//	Parse to a ref
		FSRef ref;
		result = XMLParsePathToFSRef(pathName, ref);
		
		//	Down convert to a spec
		if (result)
			result = (noErr == FSGetCatalogInfo(&ref,
								kFSCatInfoNone,
								static_cast<FSCatalogInfo*>(NULL),	// catalogInfo
								static_cast<HFSUniStr255*>(NULL),	// outName
								&spec,
								static_cast<FSRef*>(NULL)			// parentRef
								));
	}
	else
		result = XMLParsePathToFSSpec_Classic(pathName, spec);
		
	//	Return true on success
	return result;
}


bool
XMLParsePathToFSSpec_Classic(const XMLCh* const pathName, FSSpec& spec)
{
	//	Manually parse the path using FSSpec APIs.
	
    //	Transcode the path into ascii
    const char* p = XMLString::transcode(pathName);
    ArrayJanitor<const char> janPath(p);
    const char* pEnd;
    std::size_t segLen;
    
    OSStatus err = noErr;
    Str255 name;  // Must be long enough for a partial pathname consisting of two segments (64 bytes)
    
    if (*p == '/')
    {
        // Absolute name: grab the first component as volume name
        
        // Find the end of the path segment
        for (pEnd = ++p; *pEnd && *pEnd != '/'; ++pEnd) ;
        segLen = pEnd - p;
        
        // Try to find a volume that matches this name
        for (ItemCount volIndex = 1; err == noErr; ++volIndex)
        {
            FSVolumeRefNum volRefNum;
            
            if (gHasFS2TBAPIs)
            {
                XVolumeParam xVolParam;
                name[0] = 0;
                xVolParam.ioNamePtr  = name;
                xVolParam.ioVRefNum  = 0;
                xVolParam.ioXVersion = 0;
                xVolParam.ioVolIndex = volIndex;
                err = PBXGetVolInfoSync(&xVolParam);
                volRefNum = xVolParam.ioVRefNum;
            }
            else
            {
#if !TARGET_API_MAC_CARBON
                HParamBlockRec hfsParams;
                name[0] = 0;
                hfsParams.volumeParam.ioNamePtr  = name;
                hfsParams.volumeParam.ioVRefNum  = 0;
                hfsParams.volumeParam.ioVolIndex = volIndex;
                err = PBHGetVInfoSync(&hfsParams);
                volRefNum = hfsParams.volumeParam.ioVRefNum;
#else
                err = nsvErr;
#endif
            }
            
            // Compare against our path segment
            if (err == noErr && segLen == StrLength(name))
            {
            	//	Case-insensitive compare
            	if (XMLString::compareNIString(
	            		ConvertSlashToColon(reinterpret_cast<char*>(&name[1]), segLen),
	            		p, segLen) == 0)
                {
                    // we found our volume: fill in the spec
                    err = FSMakeFSSpec(volRefNum, fsRtDirID, NULL, &spec);
                    break;
                }
            }
        }
        
        p = pEnd;
    }
    else
    {
        // Relative name, so get the default directory as parent spec
        err = FSMakeFSSpec(0, 0, NULL, &spec);
    }
    
    // We now have a parent directory in the spec.
    while (err == noErr && *p)
    {
        switch (*p)
        {
        case '/': 	// Just skip any number of path separators
            ++p;
            break;
            
        case L'.': 	// Potentially "current directory" or "parent directory"
            if (p[1] == '/' || p[1] == 0)      // "current directory"
            {
                ++p;
                break;
            }
            else if (p[1] == '.' && (p[2] == '/' || p[2] == 0)) // "parent directory"
            {
                p += 2;  // Get the parent of our parent
                
                CInfoPBRec catInfo;
                catInfo.dirInfo.ioNamePtr = NULL;
                catInfo.dirInfo.ioVRefNum = spec.vRefNum;
                catInfo.dirInfo.ioFDirIndex = -1;
                catInfo.dirInfo.ioDrDirID = spec.parID;
                err = PBGetCatInfoSync(&catInfo);
                
                // Check that we didn't go too far
                if (err != noErr || catInfo.dirInfo.ioDrParID == fsRtParID)
                    return false;
                
                // Update our spec
                if (err == noErr)
                    err = FSMakeFSSpec(spec.vRefNum, catInfo.dirInfo.ioDrDirID, NULL, &spec);
                
                break;
            }
            else // some other sequence of periods...fall through and treat as segment
                ;
            
        default:
            {
                // Find the end of the path segment
                for (pEnd = p; *pEnd && *pEnd != '/'; ++pEnd) ;
                segLen = pEnd - p;
                
                // Check for name length overflow
                if (segLen > 31)
                    return false;
                
                // Make a partial pathname from our current spec to the new object
                unsigned char* partial = &name[1];
                
                *partial++ = ':';       // Partial leads with :
                const unsigned char* specName = spec.name; // Copy in spec name
                for (int specCnt = *specName++; specCnt > 0; --specCnt)
                    *partial++ = *specName++;
                
                *partial++ = ':';       // Separator
                while (p != pEnd)       // Copy in new element
               	{
                	if (*p == ':')				// Convert : to /
                	{
                		*partial++ = '/';
                		p++;
                	}
                	else
                		*partial++ = *p++;
				}
				                
                name[0] = partial - &name[1];   // Set the name length
                
                // Update the spec
                err = FSMakeFSSpec(spec.vRefNum, spec.parID, name, &spec);
            }
            break;
        }
    }
    
    return err == noErr;
}


XMLCh*
XMLCreateFullPathFromFSRef(const FSRef& startingRef)
{
	XMLCh* result = NULL;
	
	//	If FSRefMakePath is available, we use it to create the
	//	path: this gives us "standard" path support under MacOS X.
	//	Without this, our paths in that environment would always
	//	have a volume name at their root...which would look
	//	"normal" to Mac users, but not normal to unix users. Since
	//	we're making "unix" paths, we'll stick with the unix
	//	style paths. This also allows us to easilly take paths
	//	off the command line.
	//
	//	FSRefMakePath is available on Mac OS X and in CarbonLib 1.1
	//	and greater. But we use it only on X since on Classic it
	//	makes paths with ':' separators, which really confuses us!
	
	if (TARGET_API_MAC_CARBON && !gIsClassic && gHasFSPathAPIs)
		result = XMLCreateFullPathFromFSRef_X(startingRef);
	else
		result = XMLCreateFullPathFromFSRef_Classic(startingRef);
		
	return result;
}


XMLCh*
XMLCreateFullPathFromFSRef_X(const FSRef& startingRef)
{
	//	Create the path using FSRefMakePath as available on Mac OS X
	//	and under CarbonLib 1.1 and greater.
	OSStatus err = noErr;
	
	//	Make the path in utf8 form
	char utf8Buf[kMaxStaticPathChars];
	utf8Buf[0] = '\0';
	
	if (err == noErr)
		err = FSRefMakePath(&startingRef, reinterpret_cast<UInt8*>(utf8Buf), kMaxStaticPathChars);
		
	//	Bail if path conversion failed
	if (err != noErr)
		return NULL;
	
	//	Transcode into UniChars
	UniChar uniBuf[kMaxStaticPathChars];
	std::size_t pathLen = TranscodeUTF8ToUniChars(utf8Buf, uniBuf, kMaxStaticPathChars-1);
	uniBuf[pathLen++] = 0;
	
	//	Transcode into a dynamically allocated buffer of XMLChs
	ArrayJanitor<XMLCh> result(new XMLCh[pathLen]);
	if (result.get() != NULL)
		CopyUniCharsToXMLChs(uniBuf, result.get(), pathLen, pathLen);
		
	return result.release();
}


XMLCh*
XMLCreateFullPathFromFSRef_Classic(const FSRef& startingRef)
{
	//	Manually create the path using FSRef APIs.
    OSStatus err = noErr;
    FSCatalogInfo catalogInfo;
    HFSUniStr255 name;
    FSRef ref = startingRef;
    
    XMLCh buf[kMaxStaticPathChars];
    std::size_t bufPos   = kMaxStaticPathChars;
    std::size_t bufCnt   = 0;
    
    ArrayJanitor<XMLCh> result(NULL);
    std::size_t resultLen = 0;
    
    buf[--bufPos] = L'\0';
    ++bufCnt;
    
	do
	{
		err = FSGetCatalogInfo(
			&ref,
			kFSCatInfoParentDirID,
			&catalogInfo,
			&name,
			static_cast<FSSpec*>(NULL),
			&ref
			);
		
		if (err == noErr)
		{
			// If there's not room in our static buffer for the new
			// name plus separator, dump it to the dynamic result buffer.
			if (bufPos < (std::size_t)name.length + 1)
			{
				ArrayJanitor<XMLCh> temp(new XMLCh[bufCnt + resultLen]);
				
				// Copy in the static buffer
				std::memcpy(temp.get(), &buf[bufPos], bufCnt * sizeof(XMLCh));
				
				// Copy in the old buffer
				if (resultLen > 0)
					std::memcpy(temp.get() + bufCnt, result.get(), resultLen * sizeof(XMLCh));
				
				result.reset(temp.release());
				resultLen += bufCnt;
				
				bufPos = kMaxStaticPathChars;
				bufCnt = 0;
			}
			
			// Prepend our new name and a '/'
			bufPos -= name.length;
			ConvertSlashToColon(CopyUniCharsToXMLChs(name.unicode, &buf[bufPos], name.length, name.length), name.length);
			buf[--bufPos] = L'/';
			bufCnt += (name.length + 1);
		}
	}
	while (err == noErr && catalogInfo.parentDirID != fsRtParID);
	
	// Composite existing buffer with any previous result buffer
	ArrayJanitor<XMLCh> final(new XMLCh[bufCnt + resultLen]);
	
	// Copy in the static buffer
	std::memcpy(final.get(), &buf[bufPos], bufCnt * sizeof(XMLCh));
	
	// Copy in the old buffer
	if (resultLen > 0)
		std::memcpy(final.get() + bufCnt, result.get(), resultLen * sizeof(XMLCh));
	
    return final.release();
}


XMLCh*
XMLCreateFullPathFromFSSpec(const FSSpec& startingSpec)
{
	XMLCh* result = NULL;
	
	//	If FSRefs are available, do this operation in terms of refs...this
	//	allows us to grandfather in the use of FSPathMakeRef and FSRefMakePath
	//	if possible.
	if (gHasHFSPlusAPIs)
	{
		OSStatus err = noErr;
		FSRef ref;
		
		//	Up convert to FSRef
		if (err == noErr)
			err = FSpMakeFSRef(&startingSpec, &ref);
			
		//	Create the path
		if (err == noErr)
			result = XMLCreateFullPathFromFSRef(ref);
	}
	else
	{
		//	Create using FSSpecs only
		result = XMLCreateFullPathFromFSSpec_Classic(startingSpec);
	}
		
	return result;
}


XMLCh*
XMLCreateFullPathFromFSSpec_Classic(const FSSpec& startingSpec)
{
	//	Manually create the path using FSSpec APIs.
    OSStatus err = noErr;
    FSSpec spec = startingSpec;
    
    char buf[kMaxStaticPathChars];
    std::size_t bufPos   = kMaxStaticPathChars;
    std::size_t bufCnt   = 0;
    
    ArrayJanitor<char> result(NULL);
    std::size_t resultLen = 0;
    
    buf[--bufPos] = '\0';
    ++bufCnt;
    
	short index = 0;
	do
	{
		CInfoPBRec catInfo;
		catInfo.dirInfo.ioNamePtr = spec.name;
		catInfo.dirInfo.ioVRefNum = spec.vRefNum;
		catInfo.dirInfo.ioFDirIndex = index;
		catInfo.dirInfo.ioDrDirID = spec.parID;
		err = PBGetCatInfoSync(&catInfo);
		
		if (err == noErr)
		{
			std::size_t nameLen = StrLength(spec.name);
			
			// If there's not room in our static buffer for the new
			// name plus separator, dump it to the dynamic result buffer.
			if (bufPos < nameLen + 1)
			{
				ArrayJanitor<char> temp(new char[bufCnt + resultLen]);
				
				// Copy in the static buffer
				std::memcpy(temp.get(), &buf[bufPos], bufCnt);
				
				// Copy in the old buffer
				if (resultLen > 0)
					std::memcpy(temp.get() + bufCnt, result.get(), resultLen);
				
				result.reset(temp.release());
				resultLen += bufCnt;
				
				bufPos = kMaxStaticPathChars;
				bufCnt = 0;
			}
			
			// Prepend our new name and a '/'
			bufPos -= nameLen;
			ConvertSlashToColon((char*)std::memcpy(&buf[bufPos], &spec.name[1], nameLen), nameLen);
			buf[--bufPos] = '/';
			bufCnt += (nameLen + 1);
			
			// From here on out, ignore the input file name
			index = -1;
			
			// Move up to the parent
			spec.parID = catInfo.dirInfo.ioDrParID;
		}
	}
	while (err == noErr && spec.parID != fsRtParID);
	
	// Composite existing buffer with any previous result buffer
	ArrayJanitor<char> final(new char[bufCnt + resultLen]);
	
	// Copy in the static buffer
	std::memcpy(final.get(), &buf[bufPos], bufCnt);
	
	// Copy in the old buffer
	if (resultLen > 0)
		std::memcpy(final.get() + bufCnt, result.get(), resultLen);
    
    // Cleanup and transcode to unicode
    return XMLString::transcode(final.get());
}


std::size_t
TranscodeUniCharsToUTF8(UniChar* src, char* dst, std::size_t srcCnt, std::size_t maxChars)
{
	std::size_t result = 0;
	
    //	Use the text encoding converter to perform the format conversion.
    //	Note that this is slightly heavyweight, but we're not in a performance
    //	sensitive code-path.
    
    OSStatus err = noErr;
    TECObjectRef tec = 0;
    ByteCount bytesConsumed = 0;
    ByteCount bytesProduced = 0;
    
    TextEncoding inputEncoding	= CreateTextEncoding(kTextEncodingUnicodeDefault,
                                        kTextEncodingDefaultVariant,
                                        kUnicode16BitFormat);
                                        
    TextEncoding outputEncoding = CreateTextEncoding(kTextEncodingUnicodeDefault,
                                        kTextEncodingDefaultVariant,
                                        kUnicodeUTF8Format);
    
    if (err == noErr)
        err = TECCreateConverter(&tec, inputEncoding, outputEncoding);
        
    if (err == noErr)
        err = TECConvertText(tec,
                    (ConstTextPtr) src,
                    srcCnt * sizeof(UniChar),	// inputBufferLength
                    &bytesConsumed,				// actualInputLength
                    (TextPtr) dst,				// outputBuffer
                    maxChars * sizeof(char),	// outputBufferLength
                    &bytesProduced);			// actualOutputLength
                    
    TECDisposeConverter(tec);
    
    result = bytesProduced;
	
    //	Return number of chars in dst
	return result;
}


std::size_t
TranscodeUTF8ToUniChars(char* src, UniChar* dst, std::size_t maxChars)
{
	std::size_t result = 0;
	
    //	Use the text encoding converter to perform the format conversion.
    //	Note that this is slightly heavyweight, but we're not in a performance
    //	sensitive code-path.
    
    OSStatus err = noErr;
    TECObjectRef tec = 0;
    ByteCount bytesConsumed = 0;
    ByteCount bytesProduced = 0;
    
    
    TextEncoding inputEncoding	= CreateTextEncoding(kTextEncodingUnicodeDefault,
                                        kTextEncodingDefaultVariant,
                                        kUnicodeUTF8Format);
                                        
    TextEncoding outputEncoding = CreateTextEncoding(kTextEncodingUnicodeDefault,
                                        kTextEncodingDefaultVariant,
                                        kUnicode16BitFormat);
    
    if (err == noErr)
        err = TECCreateConverter(&tec, inputEncoding, outputEncoding);
        
    if (err == noErr)
        err = TECConvertText(tec,
                    (ConstTextPtr) src,
                    std::strlen(src),			// inputBufferLength
                    &bytesConsumed,				// actualInputLength
                    (TextPtr) dst,				// outputBuffer
                    maxChars * sizeof(UniChar),	// outputBufferLength
                    &bytesProduced);			// actualOutputLength
                    
    TECDisposeConverter(tec);
    
    result = bytesProduced / sizeof(UniChar);
	
    //	Return number of unicode characters in dst
	return result;
}
