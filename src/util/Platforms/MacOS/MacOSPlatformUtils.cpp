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
 * Revision 1.5  2000/07/18 18:26:09  andyh
 * Mac OS update.
 * Contributed by James Berry <jberry@criticalpath.com>
 *
 * Revision 1.4  2000/03/02 21:10:37  abagchi
 * Added empty function platformTerm()
 *
 * Revision 1.3  2000/03/02 19:55:26  roddey
 * This checkin includes many changes done while waiting for the
 * 1.1.0 code to be finished. I can't list them all here, but a list is
 * available elsewhere.
 *
 * Revision 1.2  2000/02/06 07:48:28  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.1.1.1  1999/11/09 01:06:55  twl
 * Initial checkin
 *
 * Revision 1.2  1999/11/08 20:45:30  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
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
#include <util/Janitor.hpp>
#include <util/PlatformUtils.hpp>
#include <util/RuntimeException.hpp>
#include <util/XMLUni.hpp>
#include <util/XMLString.hpp>
#include <util/Platforms/MacOS/MacOSDefs.hpp>
#include <util/Platforms/MacOS/MacOSPlatformUtils.hpp>
#include <util/MsgLoaders/InMemory/InMemMsgLoader.hpp>
#include <util/Transcoders/MacOSUnicodeConverter/MacOSUnicodeConverter.hpp>

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <memory>

#include <Files.h>
#include <Gestalt.h>
#include <Traps.h>
#include <TextUtils.h>
#include <Multiprocessing.h>
#include <DriverSynchronization.h>


//----------------------------------------------------------------------------
// Function Prototypes
//----------------------------------------------------------------------------
#if !TARGET_API_MAC_CARBON
static bool  TrapAvailable(UInt16 trapWord);
#endif

//----------------------------------------------------------------------------
//  Local Data
//
//  gGestaltAvail
//    This flag indicates the presence of the gestalt manager.  This version
//    of the MacOS port can not run without it.
//
//  gFileSystemCompatible
//    This flag indicates whether the file system APIs meet our minimum
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
//----------------------------------------------------------------------------
static bool gGestaltAvail   = false;
static bool gFileSystemCompatible = false;
static bool gHasFSSpecAPIs   = false;
static bool gHasFS2TBAPIs   = false;
static bool gHasHFSPlusAPIs   = false;


//----------------------------------------------------------------------------
// XMLMacFile methods
//----------------------------------------------------------------------------

unsigned int XMLMacFile::currPos()
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


void XMLMacFile::close()
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


void XMLMacFile::open(const XMLCh* const fileName)
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
            err = FSOpenFork(&ref, forkName.length, forkName.unicode, fsRdWrPerm, &mFileRefNum);
    }
    else
    {
        FSSpec spec;
        if (!XMLParsePathToFSSpec(fileName, spec))
            err = fnfErr;
        
        if (err == noErr)
            err = FSpOpenDF(&spec, fsRdWrPerm, &mFileRefNum);
    }
    
    if (err != noErr)
        ThrowXML(XMLPlatformUtilsException, XMLExcepts::File_CouldNotOpenFile);
    
    mFileValid = true;
}

unsigned int XMLMacFile::read(const unsigned int toRead, XMLByte* const toFill)
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
void XMLPlatformUtils::panic(const PanicReasons reason)
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
        reasonStr = "A system synchronization error occured";
    else if (reason == Panic_SystemInit)
        reasonStr = "Failed to complete platform dependent initialization";
    else
        reasonStr = "Unknown error source";
    
    //
    //  This isn't real friendly and should be cleaned up.
    // Replace this code to do whatever you need to do.
    //
    char text[200];
    sprintf(text, "Xerces Panic Error: %s", reasonStr);
    
    Str255 pasText;
    CopyCStringToPascal(text, pasText);
    DebugStr(pasText);
    
    exit(-1);
}


// ---------------------------------------------------------------------------
//  XMLPlatformUtils: File Methods
// ---------------------------------------------------------------------------
unsigned int XMLPlatformUtils::curFilePos(const FileHandle theFile)
{
    return theFile->currPos();
}

void XMLPlatformUtils::closeFile(const FileHandle theFile)
{
    theFile->close();
}

unsigned int XMLPlatformUtils::fileSize(const FileHandle theFile)
{
    return theFile->size();
}


FileHandle XMLPlatformUtils::openFile(const char* const fileName)
{
    const XMLCh* xmlPath = XMLString::transcode(fileName);
    ArrayJanitor<const XMLCh> jan(xmlPath);
    return openFile(xmlPath);
}


FileHandle XMLPlatformUtils::openFile(const XMLCh* const fileName)
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


FileHandle XMLPlatformUtils::openStdInHandle()
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


void XMLPlatformUtils::resetFile(FileHandle theFile)
{
    theFile->reset();
}


// ---------------------------------------------------------------------------
//  XMLPlatformUtils: File system methods
// ---------------------------------------------------------------------------
XMLCh* XMLPlatformUtils::getFullPath(const XMLCh* const srcPath)
{
    XMLCh* path = NULL;
    
    if (gHasHFSPlusAPIs)
    {
        FSRef ref;
        if (!XMLParsePathToFSRef(srcPath, ref) || (path = XMLCreateFullPathFromRef(ref)) == NULL)
            path = XMLString::replicate(srcPath);
    }
    else
    {
        FSSpec spec;
        if (!XMLParsePathToFSSpec(srcPath, spec) || (path = XMLCreateFullPathFromSpec(spec)) == NULL)
            path = XMLString::replicate(srcPath);
    }
    
    return path;
}


bool XMLPlatformUtils::isRelative(const XMLCh* const toCheck)
{
    return (toCheck[0] != L'/');
}



XMLCh* XMLPlatformUtils::weavePaths(const   XMLCh* const    basePath
                                    , const XMLCh* const    relativePath)
                                    
{
    // Code from Windows largely unmodified for the Macintosh,
    // with the exception of removing support for '\' path
    // separator.
    //
    // Note that there is no support currently for Macintosh
    // path separators ':'.
    
    // Create a buffer as large as both parts and empty it
    XMLCh* tmpBuf = new XMLCh[XMLString::stringLen(basePath)
        + XMLString::stringLen(relativePath)
        + 2];
    ArrayJanitor<XMLCh> janBuf(tmpBuf);
    *tmpBuf = 0;
    
    //
    //  If we have no base path, then just take the relative path as
    //  is.
    //
    if (!basePath)
    {
        XMLString::copyString(tmpBuf, relativePath);
        janBuf.orphan();
        return tmpBuf;
    }
    
    if (!*basePath)
    {
        XMLString::copyString(tmpBuf, relativePath);
        janBuf.orphan();
        return tmpBuf;
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
        XMLString::copyString(tmpBuf, relativePath);
        janBuf.orphan();
        return tmpBuf;
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
    XMLCh* bufPtr = tmpBuf;
    const XMLCh* tmpPtr = basePath;
    while (tmpPtr <= basePtr)
        *bufPtr++ = *tmpPtr++;
    
    // And then copy on the rest of our path
    XMLString::copyString(bufPtr, pathPtr);
    
    // Orphan the buffer and return it
    janBuf.orphan();
    return tmpBuf;
}


// ---------------------------------------------------------------------------
//  XMLPlatformUtils: Timing Methods
// ---------------------------------------------------------------------------
unsigned long XMLPlatformUtils::getCurrentMillis()
{
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

void* XMLPlatformUtils::makeMutex()
{
    MPCriticalRegionID criticalRegion = NULL;
    OSStatus status = MPCreateCriticalRegion(&criticalRegion);
    return (status == noErr) ? static_cast<void*>(criticalRegion) : NULL;
}


void XMLPlatformUtils::closeMutex(void* const mtxHandle)
{
    MPCriticalRegionID criticalRegion = reinterpret_cast<MPCriticalRegionID>(mtxHandle);
    OSStatus status = MPDeleteCriticalRegion(criticalRegion);
}


void XMLPlatformUtils::lockMutex(void* const mtxHandle)
{
    MPCriticalRegionID criticalRegion = reinterpret_cast<MPCriticalRegionID>(mtxHandle);
    OSStatus status = MPEnterCriticalRegion(criticalRegion, kDurationForever);
}


void XMLPlatformUtils::unlockMutex(void* const mtxHandle)
{
    MPCriticalRegionID criticalRegion = reinterpret_cast<MPCriticalRegionID>(mtxHandle);
    OSStatus status = MPExitCriticalRegion(criticalRegion);
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


int XMLPlatformUtils::atomicIncrement(int &location)
{
    return IncrementAtomic(reinterpret_cast<long*>(&location));
}


int XMLPlatformUtils::atomicDecrement(int &location)
{
    return DecrementAtomic(reinterpret_cast<long*>(&location));
}


// ---------------------------------------------------------------------------
//  XMLPlatformUtils: Private Static Methods
// ---------------------------------------------------------------------------

//
//  This method handles the MacOS basic init functions.
//
void XMLPlatformUtils::platformInit()
{
    long value;
    
    // Figure out if we have the gestalt manager
    // --we better have by now...this is a pre-system 7 feature!
    gGestaltAvail =
#if TARGET_API_MAC_CARBON
        true;
#else
    TrapAvailable(_Gestalt);
#endif
    
    // Figure out which functions we have available
    if (gGestaltAvail)
    {
        if (Gestalt(gestaltFSAttr, &value) == noErr)
        {
            gHasFSSpecAPIs = (value & gestaltHasFSSpecCalls) != 0;
            gHasFS2TBAPIs = (value & gestaltFSSupports2TBVols) != 0;
            gHasHFSPlusAPIs = (value & gestaltHasHFSPlusAPIs) != 0;
        }
    }
    
    gFileSystemCompatible = gHasFSSpecAPIs;
}


//
//  This method handles the MacOS basic termination functions.
//
void XMLPlatformUtils::platformTerm()
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
XMLNetAccessor* XMLPlatformUtils::makeNetAccessor()
{
    // No net accessor on the Mac for now.
    //
    // We could:
    //  Use URLAccess under Carbon and >= 8.6
    //  Use libwww on Mac OS X
    return 0;
}


//
//  This method is called by the platform independent part of this class
//  when client code asks to have one of the supported message sets loaded.
//
XMLMsgLoader* XMLPlatformUtils::loadAMsgSet(const XMLCh* const msgDomain)
{
    return new InMemMsgLoader(msgDomain);
}


//
//  This method is called very early in the bootstrapping process. This guy
//  must create a transcoding service and return it. It cannot use any string
//  methods, any transcoding services, throw any exceptions, etc... It just
//  makes a transcoding service and returns it, or returns zero on failure.
//
XMLTransService* XMLPlatformUtils::makeTransService()
{
    return new MacOSUnicodeConverter;
}


//---------------------------------------------------
// Utility Functions
//---------------------------------------------------

// Routine to check to see if a system function is available
#if !TARGET_API_MAC_CARBON
static bool
TrapAvailable (UInt16 trapWord)
{
    TrapType trType;
    
    // first determine whether it is an Operating System or Toolbox routine
    if ((trapWord & 0x0800) == 0)
        trType = OSTrap;
    else
        trType = ToolTrap;
    
    // filter cases where older systems mask with 0x1FF rather than 0x3FF
    if (trType == ToolTrap && ((trapWord & 0x03FF) >= 0x200) &&
        (GetToolboxTrapAddress(0xA86E) == GetToolboxTrapAddress(0xAA6E)))
        return false;
    else
        return (NGetTrapAddress(trapWord, trType) !=
        GetToolboxTrapAddress(_Unimplemented));
}
#endif


bool
XMLParsePathToFSRef(const XMLCh* const pathName, FSRef& ref)
{
    const XMLCh* p = pathName;
    const XMLCh* pEnd;
    
    OSErr err = noErr;
    
    if (*p == L'/')
    {
        // Absolute name: grab the first component as volume name
        
        // Find the end of the path segment
        for (pEnd = ++p; *pEnd && *pEnd != L'/'; ++pEnd) ;
        size_t segLen = pEnd - p;
        
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
                const UniChar* a = hfsStr.unicode;
                const XMLCh* b = p;
                while (b != pEnd && *a == *b)
                {
                    ++a;
                    ++b;
                }
                
                if (b == pEnd)
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
            
            // pEnd now points either to '/' or NUL
            // Create a new ref using this path segment
            err = FSMakeFSRefUnicode(
                &ref,
                pEnd - p,
                reinterpret_cast<UniChar*>(const_cast<XMLCh*>(p)),
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
    // Transcode the path into ascii
    const char* p = XMLString::transcode(pathName);
    ArrayJanitor<const char> janPath(p);
    const char* pEnd;
    
    OSErr err = noErr;
    Str255 name;  // Must be long enough for a partial pathname consisting of two segments (64 bytes)
    
    if (*p == '/')
    {
        // Absolute name: grab the first component as volume name
        
        // Find the end of the path segment
        for (pEnd = ++p; *pEnd && *pEnd != '/'; ++pEnd) ;
        size_t segLen = pEnd - p;
        
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
                ConstStringPtr a = name + 1;
                const char* b = p;
                while (b != pEnd && (*a == *b))
                {
                    ++a;
                    ++b;
                }
                
                if (b == pEnd)
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
        case '/':   // Just skip any number of path separators
            ++p;
            break;
            
        case L'.':   // Potentially "current directory" or "parent directory"
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
                    err = FSMakeFSSpec(spec.vRefNum, catInfo.dirInfo.ioDrParID, NULL, &spec);
                
                break;
            }
            else // some other sequence of periods...fall through and treat as segment
                ;
            
        default:
            {
                // Find the end of the path segment
                for (pEnd = p; *pEnd && *pEnd != '/'; ++pEnd) ;
                
                // Check for name length overflow
                if (pEnd - p > 31)
                    return false;
                
                // Make a partial pathname from our current spec to the new object
                unsigned char* partial = &name[1];
                
                *partial++ = ':';       // Partial leads with :
                const unsigned char* specName = spec.name; // Copy in spec name
                for (int specCnt = *specName++; specCnt > 0; --specCnt)
                    *partial++ = *specName++;
                
                *partial++ = ':';       // Separator
                while (p != pEnd)       // Copy in new element
                    *partial++ = *p++;
                
                name[0] = partial - &name[1];    // Set the name length
                
                // Update the spec
                err = FSMakeFSSpec(spec.vRefNum, spec.parID, name, &spec);
            }
            break;
        }
    }
    
    return err == noErr;
}


XMLCh*
XMLCreateFullPathFromRef(const FSRef& startingRef)
{
    OSErr err = noErr;
    FSCatalogInfo catalogInfo;
    HFSUniStr255 name;
    FSRef ref = startingRef;
    
    const size_t kBufSize = 512;
    XMLCh buf[kBufSize];
    size_t bufPos   = kBufSize;
    size_t bufCnt   = 0;
    
    XMLCh* result = NULL;
    size_t resultLen = 0;
    
    buf[--bufPos] = L'\0';
    ++bufCnt;
    
    try  // help in cleaning up since ArrayJanitor doesn't handle assignment ;(
    {
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
                if (bufPos < name.length + 1)
                {
                    XMLCh* temp = new XMLCh[bufCnt + resultLen];
                    
                    // Copy in the static buffer
                    memcpy(temp, &buf[bufPos], bufCnt * sizeof(XMLCh));
                    
                    // Copy in the old buffer
                    if (resultLen > 0)
                        memcpy(temp + bufCnt, result, resultLen);
                    
                    delete [] result;
                    result = temp;
                    resultLen += bufCnt;
                    
                    bufPos = kBufSize;
                    bufCnt = 0;
                }
                
                // Prepend our new name and a '/'
                bufPos -= name.length;
                memcpy(&buf[bufPos], name.unicode, name.length * sizeof(UniChar));
                buf[--bufPos] = L'/';
                bufCnt += (name.length + 1);
            }
        }
        while (err == noErr && catalogInfo.parentDirID != fsRtParID);
        
        // Composite existing buffer with any previous result buffer
        XMLCh* temp = new XMLCh[bufCnt + resultLen];
        
        // Copy in the static buffer
        memcpy(temp, &buf[bufPos], bufCnt * sizeof(XMLCh));
        
        // Copy in the old buffer
        if (resultLen > 0)
            memcpy(temp + bufCnt, result, resultLen * sizeof(XMLCh));
        
        delete [] result;
        result = temp;
    }
    catch (...)
    {
        delete [] result;
        throw;
    }
    
    return result;
}


XMLCh*
XMLCreateFullPathFromSpec(const FSSpec& startingSpec)
{
    OSErr err = noErr;
    FSSpec spec = startingSpec;
    
    const size_t kBufSize = 512;
    char buf[kBufSize];
    size_t bufPos   = kBufSize;
    size_t bufCnt   = 0;
    
    char* result = NULL;
    size_t resultLen = 0;
    
    buf[--bufPos] = '\0';
    ++bufCnt;
    
    try  // help in cleanup since array janitor can't handle assignment ;(
    {
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
                size_t nameLen = StrLength(spec.name);
                
                // If there's not room in our static buffer for the new
                // name plus separator, dump it to the dynamic result buffer.
                if (bufPos < nameLen + 1)
                {
                    char* temp = new char[bufCnt + resultLen];
                    
                    // Copy in the static buffer
                    memcpy(temp, &buf[bufPos], bufCnt);
                    
                    // Copy in the old buffer
                    if (resultLen > 0)
                        memcpy(temp + bufCnt, result, resultLen);
                    
                    delete [] result;
                    result = temp;
                    resultLen += bufCnt;
                    
                    bufPos = kBufSize;
                    bufCnt = 0;
                }
                
                // Prepend our new name and a '/'
                bufPos -= nameLen;
                memcpy(&buf[bufPos], &spec.name[1], nameLen);
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
        char* temp = new char[bufCnt + resultLen];
        
        // Copy in the static buffer
        memcpy(temp, &buf[bufPos], bufCnt);
        
        // Copy in the old buffer
        if (resultLen > 0)
            memcpy(temp + bufCnt, result, resultLen);
        
        delete [] result;
        result = temp;
        resultLen += bufCnt;
    }
    catch (...)
    {
        delete [] result;
        throw;
    }
    
    // Cleanup and transcode to unicode
    ArrayJanitor<char> jan(result);
    return XMLString::transcode(result);
}


/*
XMLResFile is pretty twisted and not currently used.

static const char *resBaseStr = "/Access The Resource Fork Instead of the data fork?";


FileHandle XMLPlatformUtils::openFile(const char* const fileName)
{
    FileHandle file = 0;
    int isRes = 0;
    
    // Check to make sure the file system is in a state where we can use it
    if (!gFileSystemReady)
        ThrowXML(XMLPlatformUtilsException, XML4CExcepts::File_CouldNotOpenFile);
    //throw XMLPlatformUtilsException("XMLPlatformUtils::openFile(const char* const) -- File system not ready."
    //          "  Maybe missing gestalt or no support for FSSpec's.");
    
    if (strlen(fileName) >= strlen(resBaseStr))
        if (strstr(fileName, resBaseStr) == fileName)
            isRes = 1;
        
        if (isRes == 0)
        {
            file = new XMLMacFile();
            file->open(fileName);
        }
        else
        {
            file = new XMLResFile();
            file->open(&fileName[strlen(resBaseStr)]);
        }
        if (file == 0)
            ThrowXML(XMLPlatformUtilsException, XML4CExcepts::File_CouldNotOpenFile);
        //throw XMLPlatformUtilsException("XMLPlatformUtils::openFile(const char* const) -- Failed to allocate file object.");
        return file;
}


//----------------------------------------------------------------------------
// XMLResFile methods
//----------------------------------------------------------------------------
XMLResFile::XMLResFile();


void XMLResFile::open(const char* const resInfo)
{
    char option[32], value[32], command[70];
    int cmdEnd = 0, cmdStart = 0;
    int optEnd = 0, sep;
    int mode = -1;
    int typeValid = 0;
    
    if (!strchr(&resInfo[cmdStart], '/'))
        ThrowXML(XMLPlatformUtilsException, XML4CExcepts::URL_MalformedURL);
    
    while(resInfo[cmdEnd] != '/')
    {
        if (strchr(&resInfo[cmdStart], '&') < strchr(&resInfo[cmdStart], '/') && strchr(&resInfo[cmdStart], '&') != 0)
            cmdEnd = strchr(&resInfo[cmdStart], '&') - resInfo - 1;
        else
            cmdEnd = strchr(&resInfo[cmdStart], '/') - resInfo - 1;
        
        if (cmdEnd - cmdStart > 68)
            ThrowXML(XMLPlatformUtilsException, XML4CExcepts::File_CouldNotOpenFile);
        //throw XMLPlatformUtilsException("XMLPlatformUtils::openFile -- resource option too long (>68 chars)");
        
        memcpy(command, &resInfo[cmdStart], cmdEnd - cmdStart + 1);
        command[cmdEnd - cmdStart + 1] = 0;
        if (!strchr(command, '='))
            ThrowXML(XMLPlatformUtilsException, XML4CExcepts::URL_MalformedURL);
        //throw XMLPlatformUtilsException("XMLPlatformUtils::openFile -- Malformed resource locater");
        
        sep = strchr(command, '=') - command;
        memcpy(option, command, sep);
        option[sep] = 0;
        memcpy(value, &command[sep+1], strlen(command) - sep);
        
        if (!strcmp(option, "mode"))
        {
            if (!strcmp(value, "by_id"))
                mode = 1;
            else if (!strcmp(value, "by_id1"))
                mode = 2;
            else if (!strcmp(value, "by_name"))
                mode = 3;
            else if (!strcmp(value, "by_name1"))
                mode = 4;
            else
                ThrowXML(XMLPlatformUtilsException, XML4CExcepts::File_CouldNotOpenFile);
            //throw XMLPlatformUtilsException("XMLPlatformUtils::openFile -- 'mode' has to be 'by_id' or 'by_id1' or 'by_name' or 'by_name1'");
            
        }
        if (!strcmp(option, "type"))
        {
            if (strlen(value) != 4)
                ThrowXML(XMLPlatformUtilsException, XML4CExcepts::File_CouldNotOpenFile);
            //throw XMLPlatformUtilsException("XMLPlatformUtils::openFile -- 'type' has to be four characters long");
            typeValid = 1;
            type = 0;
            type += value[0] << 24;
            type += value[1] << 16;
            type += value[2] << 8;
            type += value[3];
        }
        
        cmdStart = cmdEnd + 2;
        cmdEnd++;
    }
    
    if (mode == 0)
        ThrowXML(XMLPlatformUtilsException, XML4CExcepts::File_CouldNotOpenFile);
    //throw XMLPlatformUtilsException("XMLPlatformUtils::openFile -- Malformed resource locater requires a 'mode'");
    if (typeValid == 0)
        ThrowXML(XMLPlatformUtilsException, XML4CExcepts::File_CouldNotOpenFile);
    //throw XMLPlatformUtilsException("XMLPlatformUtils::openFile -- Malformed resource locater requires a 'type'");
    
    switch(mode)
    {
    case 1: case 2:
        id = atol(&resInfo[cmdEnd+1]);
        if (mode == 1)
            data = GetResource(type, id);
        else
            data = Get1Resource(type, id);
        break;
        
    case 3: case 4:
        if (strlen(&resInfo[cmdEnd]) >= 255)
            ThrowXML(XMLPlatformUtilsException, XML4CExcepts::File_CouldNotOpenFile);
        //throw XMLPlatformUtilsException("XMLPlatformUtils::openFile -- Resource names have to be 255 characters or less");
        strcpy((char*)name, &resInfo[cmdEnd]);
        name[0] = strlen((char*)&name[1]);
        if (mode == 3)
            data = GetNamedResource(type, name);
        else
            data = Get1NamedResource(type, name);
        break;
    }
    
    if (ResError() != noErr)
        ThrowXML(XMLPlatformUtilsException, XML4CExcepts::File_CouldNotOpenFile);
    //throw XMLPlatformUtilsException("XMLPlatformUtils::openFile -- Error opening resource");
    
    GetResInfo(data, &id, &type, name);
    len = GetResourceSizeOnDisk(data);
    if (ResError() != noErr)
        ThrowXML(XMLPlatformUtilsException, XML4CExcepts::File_CouldNotOpenFile);
    //throw XMLPlatformUtilsException("XMLPlatformUtils::openFile -- Error loading resource info");
    
    valid = 1;
 }
                                                
 unsigned int XMLResFile::read(const unsigned int buffLen, XMLByte* const buff)
 {
     unsigned int totAvail = len - pos;
     unsigned int numRead = (buffLen >= totAvail) ? totAvail : buffLen;
     
     HLock(data);
     memcpy(buff, *data, numRead);
     HUnlock(data);
     pos += numRead;
     return numRead;
 }
 
 void XMLResFile::close()
 {
     if (!valid)
         ThrowXML(XMLPlatformUtilsException, XML4CExcepts::File_CouldNotCloseFile);
     //throw XMLPlatformUtilsException("XMLPlatformUtils::curFilePos -- Not a valid file");
     ReleaseResource(data);
     valid = 0;
 }
 
 unsigned int XMLResFile::currPos()
 {
     if (!valid)
         ThrowXML(XMLPlatformUtilsException, XML4CExcepts::File_CouldNotGetCurPos);
     //throw XMLPlatformUtilsException("XMLPlatformUtils::curFilePos -- Not a valid file");
     return pos;
 }
 
 void XMLResFile::reset()
 {
     if (!valid)
         ThrowXML(XMLPlatformUtilsException, XML4CExcepts::File_CouldNotResetFile);
     //throw XMLPlatformUtilsException("XMLPlatformUtils::resetFile -- Not a valid file");
     pos = 0;
 }
 
 unsigned int XMLResFile::size()
 {
     if (!valid)
         ThrowXML(XMLPlatformUtilsException, XML4CExcepts::File_CouldNotGetSize);
     //throw XMLPlatformUtilsException("XMLPlatformUtils::fileSize -- Not a valid file");
     return len;
 }
 
 XMLResFile::~XMLResFile()
 {
     if (valid)
         close();
 }
*/



