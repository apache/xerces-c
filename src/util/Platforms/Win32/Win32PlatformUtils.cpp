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
 * Revision 1.21  2000/05/09 00:22:41  andyh
 * Memory Cleanup.  XMLPlatformUtils::Terminate() deletes all lazily
 * allocated memory; memory leak checking tools will no longer report
 * that leaks exist.  (DOM GetElementsByTagID temporarily removed
 * as part of this.)
 *
 * Revision 1.20  2000/04/18 23:26:01  andyh
 * Fix problem on NT with conflict between Korean Won Sign (0x20A9)
 * and backslash in file path names.
 *
 * Revision 1.19  2000/04/18 17:57:29  roddey
 * Fix signature of ::fileSize() method. Bug #119
 *
 * Revision 1.18  2000/04/11 19:11:55  roddey
 * Replace Yen signs with backslash in wide char path names
 * under NT. This avoids ambiguous code point in some asian
 * encodings.
 *
 * Revision 1.17  2000/04/05 18:55:36  roddey
 * Delete the critical section handle in closeMutex().
 *
 * Revision 1.16  2000/03/20 23:43:03  rahulj
 * Expanded tabs to spaces.
 *
 * Revision 1.15  2000/03/18 00:00:02  roddey
 * Initial updates for two way transcoding support
 *
 * Revision 1.14  2000/03/17 02:37:55  rahulj
 * First cut at adding HTTP capability via native sockets.
 * Still need to add:
 *   error handling capability, ports other than 80,
 *   escaped URL's
 * Will add options in project file only when I am done with these
 * above changes.
 *
 * Revision 1.13  2000/03/02 19:55:34  roddey
 * This checkin includes many changes done while waiting for the
 * 1.1.0 code to be finished. I can't list them all here, but a list is
 * available elsewhere.
 *
 * Revision 1.12  2000/02/06 07:48:32  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.11  2000/01/31 23:53:14  roddey
 * Since all synchronization inside the parser is intraprocess, the Win32 mutex
 * implementation was changed to use critical sections for speed.
 *
 * Revision 1.10  2000/01/25 21:34:45  roddey
 * Added support for the two new panic errors.
 *
 * Revision 1.9  2000/01/22 00:03:47  roddey
 * Added a check for a broken pipe error on file read. This allows folks to support
 * a pipe as an input source and not die when the other side drops the sending
 * end of the pipe.
 *
 * Revision 1.8  2000/01/20 20:37:25  andyh
 * Remove DEVENV_VCPP preprocessor variable everywhere.
 * It was obsolete, left over from an earlier configuration system.
 * And it was not set correctly in all projects.
 *
 * Should fix build problem reported by some with use of
 * InterlockedCompareExchange() on Windows with VC6.
 *
 * Revision 1.7  2000/01/19 00:57:26  roddey
 * Changes to get rid of dependence on old utils standard streams and to
 * get rid of the fgLibLocation stuff.
 *
 * Revision 1.6  2000/01/15 01:26:18  rahulj
 * Added support for HTTP to the parser using libWWW 5.2.8.
 * Renamed URL.[ch]pp to XMLURL.[ch]pp and like wise for the class name.
 * Only tested under NT 4.0 SP 5.
 * Removed URL.hpp from files where it was not used.
 *
 * Revision 1.5  2000/01/14 19:48:34  andyh
 * Update MSVC project files to reflect change in DLL version to 1_1
 * Update dll name in panic message text from XML4C to Xerces
 *
 * Revision 1.4  2000/01/13 20:07:10  roddey
 * Just a trivial fix of a misspelling in one of the panic messages.
 *
 * Revision 1.3  2000/01/12 00:16:47  roddey
 * Changes to deal with multiply nested, relative pathed, entities and to deal
 * with the new URL class changes.
 *
 * Revision 1.2  1999/11/22 20:41:26  abagchi
 * Changed 'intlFiles/Locales' to 'icu/data'
 *
 * Revision 1.1.1.1  1999/11/09 01:06:21  twl
 * Initial checkin
 *
 * Revision 1.4  1999/11/08 20:45:33  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <util/Janitor.hpp>
#include <util/PlatformUtils.hpp>
#include <util/RuntimeException.hpp>
#include <util/XMLExceptMsgs.hpp>
#include <util/XMLString.hpp>
#include <util/XMLUni.hpp>
#include <windows.h>

#ifdef _DEBUG
#include <crtdbg.h>
#endif

//
//  These control which transcoding service is used by the Win32 version.
//  They allow this to be controlled from the build process by just defining
//  one of these values.
//
#if defined (XML_USE_ICU_TRANSCODER)
    #include <util/Transcoders/ICU/ICUTransService.hpp>
#elif defined (XML_USE_WIN32_TRANSCODER)
    #include <util/Transcoders/Win32/Win32TransService.hpp>
#else
    #error A transcoding service must be chosen
#endif

//
//  These control which message loading service is used by the Win32 version.
//  They allow this to be controlled from the build process by just defining
//  one of these values.
//
#if defined (XML_USE_INMEMORY_MSGLOADER)
    #include <util/MsgLoaders/InMemory/InMemMsgLoader.hpp>
#elif defined (XML_USE_WIN32_MSGLOADER)
    #include <util/MsgLoaders/Win32/Win32MsgLoader.hpp>
#else
    #error A message loading service must be chosen
#endif

//
//  These control which network access service is used by the Win32 version.
//  They allow this to be controlled from the build process by just defining
//  one of these values.
//
#if defined (XML_USE_NETACCESSOR_LIBWWW)
    #include <util/NetAccessors/libWWW/LibWWWNetAccessor.hpp>
#elif defined (XML_USE_NETACCESSOR_WINSOCK)
    #include <util/NetAccessors/WinSock/WinSockNetAccessor.hpp>
#endif



// ---------------------------------------------------------------------------
//  Local data
//
//  gOnNT
//      We figure out during init if we are on NT or not. If we are, then
//      we can avoid a lot of transcoding in our system services stuff.
// ---------------------------------------------------------------------------
static bool     gOnNT;


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
        reasonStr = "Failed to complete platfrom dependent initialization";

    //
    //  We just do a popup and exit. Replace this code to do whatever
    //  you need to do.
    //
    MessageBoxA
    (
        0
        , "Xerces Panic Error"
        , reasonStr
        , MB_OK | MB_ICONSTOP
    );
    exit(-1);
}



// ---------------------------------------------------------------------------
//  XMLPlatformUtils: File Methods
// ---------------------------------------------------------------------------
unsigned int XMLPlatformUtils::curFilePos(FileHandle theFile)
{
    // Get the current position
    const unsigned int curPos = ::SetFilePointer(theFile, 0, 0, FILE_CURRENT);
    if (curPos == 0xFFFFFFFF)
        ThrowXML(XMLPlatformUtilsException, XMLExcepts::File_CouldNotGetCurPos);

    return curPos;
}

void XMLPlatformUtils::closeFile(FileHandle theFile)
{
    if (!::CloseHandle(theFile))
        ThrowXML(XMLPlatformUtilsException, XMLExcepts::File_CouldNotCloseFile);
}

unsigned int XMLPlatformUtils::fileSize(FileHandle theFile)
{
    // Get the current position
    const unsigned int curPos = ::SetFilePointer(theFile, 0, 0, FILE_CURRENT);
    if (curPos == 0xFFFFFFFF)
        ThrowXML(XMLPlatformUtilsException, XMLExcepts::File_CouldNotGetCurPos);

    // Seek to the end and save that value for return
    const unsigned int retVal = ::SetFilePointer(theFile, 0, 0, FILE_END);
    if (curPos == 0xFFFFFFFF)
        ThrowXML(XMLPlatformUtilsException, XMLExcepts::File_CouldNotSeekToEnd);

    // And put the pointer back
    if (::SetFilePointer(theFile, curPos, 0, FILE_BEGIN) == 0xFFFFFFFF)
        ThrowXML(XMLPlatformUtilsException, XMLExcepts::File_CouldNotSeekToPos);

    return retVal;
}

FileHandle XMLPlatformUtils::openFile(const char* const fileName)
{
    FileHandle retVal = ::CreateFileA
    (
        fileName
        , GENERIC_READ
        , FILE_SHARE_READ
        , 0
        , OPEN_EXISTING
        , FILE_FLAG_SEQUENTIAL_SCAN
        , 0
    );

    if (retVal == INVALID_HANDLE_VALUE)
        return 0;

    return retVal;
}

FileHandle XMLPlatformUtils::openFile(const XMLCh* const fileName)
{
    // Watch for obvious wierdness
    if (!fileName)
        return 0;

    FileHandle retVal = 0;
    if (gOnNT)
    {
        //
        //  Ok, this might look stupid but its a semi-expedient way to deal
        //  with a thorny problem. Shift-JIS and some other Asian encodings
        //  are fundamentally broken and map both the backslash and the Yen
        //  sign to the same code point. Transcoders have to pick one or the
        //  other to map '\' to Unicode and tend to choose the Yen sign. Since we
        //  never transcode back to the local code page, the Unicode Yen signs
        //  will still be in the path and will fail.
        //
        //  So, we will check this path name for Yen signs and, if they are
        //  there, we'll replace them with back slashes.  The Korean Won
        //  currency symbol has the same problem.
        //
        const XMLCh* srcPtr = fileName;
        while (*srcPtr)
        {
            if (*srcPtr == chYenSign ||
				*srcPtr == chWonSign)
                    break;
            srcPtr++;
        }

        //
        //  If we found a yen, then we have to create a temp file name. Else
        //  go with the file name as is and save the overhead.
        //
        if (*srcPtr)
        {
            XMLCh* tmpName = XMLString::replicate(fileName);

            XMLCh* srcPtr = tmpName;
            while (*srcPtr)
            {
                if (*srcPtr == chYenSign ||
					*srcPtr == chWonSign)
                    *srcPtr = chBackSlash;
                srcPtr++;
            }

            retVal = ::CreateFileW
            (
                tmpName
                , GENERIC_READ
                , FILE_SHARE_READ
                , 0
                , OPEN_EXISTING
                , FILE_FLAG_SEQUENTIAL_SCAN
                , 0
            );

            delete [] tmpName;
        }
         else
        {
            retVal = ::CreateFileW
            (
                fileName
                , GENERIC_READ
                , FILE_SHARE_READ
                , 0
                , OPEN_EXISTING
                , FILE_FLAG_SEQUENTIAL_SCAN
                , 0
            );
        }
    }
     else
    {
        char* tmpName = XMLString::transcode(fileName);
        retVal = ::CreateFileA
        (
            tmpName
            , GENERIC_READ
            , FILE_SHARE_READ
            , 0
            , OPEN_EXISTING
            , FILE_FLAG_SEQUENTIAL_SCAN
            , 0
        );
        delete [] tmpName;
    }

    if (retVal == INVALID_HANDLE_VALUE)
        return 0;

    return retVal;
}


FileHandle XMLPlatformUtils::openStdInHandle()
{
    //
    //  Get the standard input handle. Duplicate it and return that copy
    //  since the outside world cannot tell the difference and will shut
    //  down this handle when its done with it. If we gave out the orignal,
    //  shutting it would prevent any further output.
    //
    HANDLE stdInOrg = ::GetStdHandle(STD_INPUT_HANDLE);
    if (stdInOrg == INVALID_HANDLE_VALUE)
        ThrowXML(XMLPlatformUtilsException, XMLExcepts::File_CouldNotOpenFile);

    HANDLE retHandle;
    if (!::DuplicateHandle
    (
        ::GetCurrentProcess()
        , stdInOrg
        , ::GetCurrentProcess()
        , &retHandle
        , 0
        , FALSE
        , DUPLICATE_SAME_ACCESS))
    {
        ThrowXML(XMLPlatformUtilsException, XMLExcepts::File_CouldNotDupHandle);
    }
    return retHandle;
}


unsigned int
XMLPlatformUtils::readFileBuffer(       FileHandle      theFile
                                , const unsigned int    toRead
                                ,       XMLByte* const  toFill)
{
    unsigned long bytesRead = 0;
    if (!::ReadFile(theFile, toFill, toRead, &bytesRead, 0))
    {
        //
        //  Check specially for a broken pipe error. If we get this, it just
        //  means no more data from the pipe, so return zero.
        //
        if (::GetLastError() != ERROR_BROKEN_PIPE)
            ThrowXML(XMLPlatformUtilsException, XMLExcepts::File_CouldNotReadFromFile);
    }
    return (unsigned int)bytesRead;
}


void XMLPlatformUtils::resetFile(FileHandle theFile)
{
    // Seek to the start of the file
    if (::SetFilePointer(theFile, 0, 0, FILE_BEGIN) == 0xFFFFFFFF)
        ThrowXML(XMLPlatformUtilsException, XMLExcepts::File_CouldNotResetFile);
}


// ---------------------------------------------------------------------------
//  XMLPlatformUtils: File system methods
// ---------------------------------------------------------------------------
XMLCh* XMLPlatformUtils::getFullPath(const XMLCh* const srcPath)
{
    //
    //  If we are on NT, then use wide character APIs, else use ASCII APIs.
    //  We have to do it manually since we are only built in ASCII mode from
    //  the standpoint of the APIs.
    //
    if (gOnNT)
    {
        // Use a local buffer that is big enough for the largest legal path
        const unsigned int bufSize = 1024;
        XMLCh tmpPath[bufSize + 1];

        XMLCh* namePart = 0;
        if (!::GetFullPathNameW(srcPath, bufSize, tmpPath, &namePart))
            return 0;

        // Return a copy of the path
        return XMLString::replicate(tmpPath);
    }
     else
    {
        // Transcode the incoming string
        char* tmpSrcPath = XMLString::transcode(srcPath);
        ArrayJanitor<char> janSrcPath(tmpSrcPath);

        // Use a local buffer that is big enough for the largest legal path
        const unsigned int bufSize = 511;
        char tmpPath[511 + 1];

        char* namePart = 0;
        if (!::GetFullPathNameA(tmpSrcPath, bufSize, tmpPath, &namePart))
            return 0;

        // Return a transcoded copy of the path
        return XMLString::transcode(tmpPath);
    }
}

bool XMLPlatformUtils::isRelative(const XMLCh* const toCheck)
{
    // Check for pathological case of empty path
    if (!toCheck[0])
        return false;

    //
    //  If its starts with a drive, then it cannot be relative. Note that
    //  we checked the drive not being empty above, so worst case its one
    //  char long and the check of the 1st char will fail because its really
    //  a null character.
    //
    if (toCheck[1] == chColon)
    {
        if (((toCheck[0] >= chLatin_A) && (toCheck[0] <= chLatin_Z))
        ||  ((toCheck[0] >= chLatin_a) && (toCheck[0] <= chLatin_z)))
        {
            return false;
        }
    }

    //
    //  If it starts with a double slash, then it cannot be relative since
    //  its a remote file.
    //
    if ((toCheck[0] == chBackSlash) && (toCheck[1] == chBackSlash))
        return false;

    // Else assume its a relative path
    return true;
}


XMLCh* XMLPlatformUtils::weavePaths(const   XMLCh* const    basePath
                                    , const XMLCh* const    relativePath)

{
    // Create a buffer as large as both parts and empty it
    XMLCh* tmpBuf = new XMLCh[XMLString::stringLen(basePath)
                              + XMLString::stringLen(relativePath)
                              + 2];
    *tmpBuf = 0;

    //
    //  If we have no base path, then just take the relative path as
    //  is.
    //
    if (!basePath)
    {
        XMLString::copyString(tmpBuf, relativePath);
        return tmpBuf;
    }

    if (!*basePath)
    {
        XMLString::copyString(tmpBuf, relativePath);
        return tmpBuf;
    }

    const XMLCh* basePtr = basePath + (XMLString::stringLen(basePath) - 1);
    if ((*basePtr != chForwardSlash)
    &&  (*basePtr != chBackSlash))
    {
        while ((basePtr >= basePath)
        &&     ((*basePtr != chForwardSlash) && (*basePtr != chBackSlash)))
        {
            basePtr--;
        }
    }

    // There is no relevant base path, so just take the relative part
    if (basePtr < basePath)
    {
        XMLString::copyString(tmpBuf, relativePath);
        return tmpBuf;
    }

    // After this, make sure the buffer gets handled if we exit early
    ArrayJanitor<XMLCh> janBuf(tmpBuf);

    //
    //  We have some path part, so we need to check to see if we ahve to
    //  weave any of the parts together.
    //
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

        // Has to be followed by a \ or / or the null to mean anything
        if ((*pathPtr != chForwardSlash) && (*pathPtr != chBackSlash)
        &&  *pathPtr)
        {
            break;
        }
        if (*pathPtr)
            pathPtr++;

        // If its one period, just eat it, else move backwards in the base
        if (periodCount == 2)
        {
            basePtr--;
            while ((basePtr >= basePath)
            &&     ((*basePtr != chForwardSlash) && (*basePtr != chBackSlash)))
            {
                basePtr--;
            }

            // The base cannot provide enough levels, so its in error/
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
    return (unsigned long)::GetTickCount();
}



// ---------------------------------------------------------------------------
//  Mutex methods
// ---------------------------------------------------------------------------
void XMLPlatformUtils::closeMutex(void* const mtxHandle)
{
    ::DeleteCriticalSection((LPCRITICAL_SECTION)mtxHandle);
    delete mtxHandle;
}


void XMLPlatformUtils::lockMutex(void* const mtxHandle)
{
    ::EnterCriticalSection((LPCRITICAL_SECTION)mtxHandle);
}


void* XMLPlatformUtils::makeMutex()
{
    CRITICAL_SECTION* newCS = new CRITICAL_SECTION;
    InitializeCriticalSection(newCS);
    return newCS;
}


void XMLPlatformUtils::unlockMutex(void* const mtxHandle)
{
    ::LeaveCriticalSection((LPCRITICAL_SECTION)mtxHandle);
}


// ---------------------------------------------------------------------------
//  Miscellaneous synchronization methods
// ---------------------------------------------------------------------------
void*
XMLPlatformUtils::compareAndSwap(       void**      toFill
                                , const void* const newValue
                                , const void* const toCompare)
{
    //
    //  Windows supports InterlockedCompareExchange only on Windows 98,
    //  NT 4.0, and newer. Not on Win 95. So we are back to using assembler.
    //  (But only if building with MSVC.)
    //
    #if defined(_MSC_VER)

    void*   result;
    __asm
    {
        mov             eax, toCompare;
        mov             ebx, newValue;
        mov             ecx, toFill
        lock cmpxchg    [ecx], ebx;
        mov             result, eax;
    }
    return result;

    #else

    //    
    //  Note we have to cast off the constness of some of these because
    //  the system APIs are not C++ aware in all cases.
    //
    return ::InterlockedCompareExchange
    (
        toFill
        , (void*)newValue
        , (void*)toCompare
    );

    #endif
}


// ---------------------------------------------------------------------------
//  Atomic increment and decrement methods
// ---------------------------------------------------------------------------
int XMLPlatformUtils::atomicIncrement(int &location)
{
    return ::InterlockedIncrement(&(long &)location);
}


int XMLPlatformUtils::atomicDecrement(int &location)
{
    return ::InterlockedDecrement(&(long &)location);
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
#if defined (XML_USE_NETACCESSOR_LIBWWW)
    return new LibWWWNetAccessor();
#elif defined (XML_USE_NETACCESSOR_WINSOCK)
    return new WinSockNetAccessor();
#else
    return 0;
#endif
}


//
//  This method is called by the platform independent part of this class
//  when client code asks to have one of the supported message sets loaded.
//  In our case, we use the ICU based message loader mechanism.
//
XMLMsgLoader* XMLPlatformUtils::loadAMsgSet(const XMLCh* const msgDomain)
{
#if defined (XML_USE_INMEMORY_MSGLOADER)
    return new InMemMsgLoader(msgDomain);
#elif defined (XML_USE_WIN32_MSGLOADER)
    return new Win32MsgLoader(msgDomain);
#else
    #error You must provide a message loader
#endif
}


//
//  This method is called very early in the bootstrapping process. This guy
//  must create a transcoding service and return it. It cannot use any string
//  methods, any transcoding services, throw any exceptions, etc... It just
//  makes a transcoding service and returns it, or returns zero on failure.
//
XMLTransService* XMLPlatformUtils::makeTransService()
{
    //
    //  Since we are going to use the ICU service, we have to tell it where
    //  its converter files are. If the ICU_DATA environment variable is set,
    //  then its been told. Otherwise, we tell it our default value relative
    //  to our DLL.
    //
#if defined (XML_USE_ICU_TRANSCODER)
    return new ICUTransService;
#elif defined (XML_USE_WIN32_TRANSCODER)
    return new Win32TransService;
#else
    #error You must provide a transcoding service implementation
#endif
}


//
//  This method handles the Win32 per-platform basic init functions. The
//  primary jobs here are getting the path to our DLL and to get the
//  stdout and stderr file handles setup.
//
void XMLPlatformUtils::platformInit()
{

#if 1 && _DEBUG
    //  Enable this code for memeory leak testing
    
   // Send all reports to STDOUT
   _CrtSetReportMode( _CRT_WARN, _CRTDBG_MODE_FILE );
   _CrtSetReportFile( _CRT_WARN, _CRTDBG_FILE_STDOUT );
   _CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_FILE );
   _CrtSetReportFile( _CRT_ERROR, _CRTDBG_FILE_STDOUT );
   _CrtSetReportMode( _CRT_ASSERT, _CRTDBG_MODE_FILE );
   _CrtSetReportFile( _CRT_ASSERT, _CRTDBG_FILE_STDOUT );

    int tmpDbgFlag;
    tmpDbgFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
    tmpDbgFlag |= _CRTDBG_LEAK_CHECK_DF;
    _CrtSetDbgFlag(tmpDbgFlag);
#endif

    // Figure out if we are on NT and save that flag for later use
    OSVERSIONINFO   OSVer;
    OSVer.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    ::GetVersionEx(&OSVer);
    gOnNT = (OSVer.dwPlatformId == VER_PLATFORM_WIN32_NT);
}


void XMLPlatformUtils::platformTerm()
{
    // We don't have any temrination requirements for win32 at this time
}
