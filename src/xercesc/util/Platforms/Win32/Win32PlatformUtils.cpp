/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 1999-2002 The Apache Software Foundation.  All rights
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
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/util/Janitor.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/RuntimeException.hpp>
#include <xercesc/util/XMLExceptMsgs.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/XMLUniDefs.hpp>
#include <xercesc/util/XMLUni.hpp>
#include <xercesc/util/PanicHandler.hpp>

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef _DEBUG
 #ifdef _MSC_VER
    #include <crtdbg.h>
 #else
    #include <assert.h>
 #endif
#endif

//
//  These control which transcoding service is used by the Win32 version.
//  They allow this to be controlled from the build process by just defining
//  one of these values.
//
#if defined (XML_USE_ICU_TRANSCODER)
    #include <xercesc/util/Transcoders/ICU/ICUTransService.hpp>
#elif defined (XML_USE_WIN32_TRANSCODER)
    #include <xercesc/util/Transcoders/Win32/Win32TransService.hpp>
#elif defined (XML_USE_CYGWIN_TRANSCODER)
    #include <xercesc/util/Transcoders/Cygwin/CygwinTransService.hpp>
#else
    #error A transcoding service must be chosen
#endif

//
//  These control which message loading service is used by the Win32 version.
//  They allow this to be controlled from the build process by just defining
//  one of these values.
//
#if defined (XML_USE_INMEM_MESSAGELOADER)
    #include <xercesc/util/MsgLoaders/InMemory/InMemMsgLoader.hpp>
#elif defined (XML_USE_WIN32_MSGLOADER)
    #include <xercesc/util/MsgLoaders/Win32/Win32MsgLoader.hpp>
#elif defined(XML_USE_ICU_MESSAGELOADER)
    #include <xercesc/util/MsgLoaders/ICU/ICUMsgLoader.hpp>
#else
    #error A message loading service must be chosen
#endif

//
//  These control which network access service is used by the Win32 version.
//  They allow this to be controlled from the build process by just defining
//  one of these values.
//
#if defined (XML_USE_NETACCESSOR_LIBWWW)
    #include <xercesc/util/NetAccessors/libWWW/LibWWWNetAccessor.hpp>
#elif defined (XML_USE_NETACCESSOR_WINSOCK)
    #include <xercesc/util/NetAccessors/WinSock/WinSockNetAccessor.hpp>
#endif

XERCES_CPP_NAMESPACE_BEGIN

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
void XMLPlatformUtils::panic(const PanicHandler::PanicReasons reason)
{
    fgUserPanicHandler? fgUserPanicHandler->panic(reason) : fgDefaultPanicHandler->panic(reason);	
}



// ---------------------------------------------------------------------------
//  XMLPlatformUtils: File Methods
// ---------------------------------------------------------------------------

//
//  Functions to look for Unicode forward and back slashes.
//  This operation is complicated by the fact that some Japanese and Korean
//    encodings use the same encoding for both '\' and their currency symbol
//    (Yen or Won).  In these encodings, which is meant is context dependent.
//    Unicode converters choose the currency symbols.  But in the context
//    of a Windows file name, '\' is generally what was intended.
//
//    So we make a leap of faith, and assume that if we get a Yen or Won
//    here, in the context of a file name, that it originated in one of
//    these encodings, and is really supposed to be a '\'.
//
static bool isBackSlash(XMLCh c) {
    return c == chBackSlash ||
           c == chYenSign   ||
           c == chWonSign;
}

unsigned int XMLPlatformUtils::curFilePos(FileHandle theFile
                                          , MemoryManager* const manager)
{
    // Get the current position
    const unsigned int curPos = ::SetFilePointer(theFile, 0, 0, FILE_CURRENT);
    if (curPos == 0xFFFFFFFF)
        ThrowXMLwithMemMgr(XMLPlatformUtilsException, XMLExcepts::File_CouldNotGetCurPos, manager);

    return curPos;
}

void XMLPlatformUtils::closeFile(FileHandle theFile
                                 , MemoryManager* const manager)
{
    if (!::CloseHandle(theFile))
        ThrowXMLwithMemMgr(XMLPlatformUtilsException, XMLExcepts::File_CouldNotCloseFile, manager);
}

unsigned int XMLPlatformUtils::fileSize(FileHandle theFile
                                        , MemoryManager* const manager)
{
    // Get the current position
    const unsigned int curPos = ::SetFilePointer(theFile, 0, 0, FILE_CURRENT);
    if (curPos == 0xFFFFFFFF)
        ThrowXMLwithMemMgr(XMLPlatformUtilsException, XMLExcepts::File_CouldNotGetCurPos, manager);

    // Seek to the end and save that value for return
    const unsigned int retVal = ::SetFilePointer(theFile, 0, 0, FILE_END);
    if (retVal == 0xFFFFFFFF)
        ThrowXMLwithMemMgr(XMLPlatformUtilsException, XMLExcepts::File_CouldNotSeekToEnd, manager);

    // And put the pointer back
    if (::SetFilePointer(theFile, curPos, 0, FILE_BEGIN) == 0xFFFFFFFF)
        ThrowXMLwithMemMgr(XMLPlatformUtilsException, XMLExcepts::File_CouldNotSeekToPos, manager);

    return retVal;
}

FileHandle XMLPlatformUtils::openFile(const char* const fileName
                                      , MemoryManager* const manager)
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

FileHandle XMLPlatformUtils::openFile(const XMLCh* const fileName
                                      , MemoryManager* const manager)
{
    // Watch for obvious wierdness
    if (!fileName)
        return 0;

    //
    //  We have to play a little trick here. If its /x:.....
    //  style fully qualified path, we have to toss the leading /
    //  character.
    //
    const XMLCh* nameToOpen = fileName;
    if (*fileName == chForwardSlash)
    {
        if (XMLString::stringLen(fileName) > 3)
        {
            if (*(fileName + 2) == chColon)
            {
                const XMLCh chDrive = *(fileName + 1);
                if (((chDrive >= chLatin_A) && (chDrive <= chLatin_Z))
                ||  ((chDrive >= chLatin_a) && (chDrive <= chLatin_z)))
                {
                    nameToOpen = fileName + 1;
                }
            }

            // Similarly for UNC paths
            if ( *(fileName + 1) == *(fileName + 2) &&
                 (*(fileName + 1) == chForwardSlash ||
                  *(fileName + 1) == chBackSlash) )
            {
                nameToOpen = fileName + 1;
            }
        }
    }

    //  Ok, this might look stupid but its a semi-expedient way to deal
    //  with a thorny problem. Shift-JIS and some other Asian encodings
    //  are fundamentally broken and map both the backslash and the Yen
    //  sign to the same code point. Transcoders have to pick one or the
    //  other to map '\' to Unicode and tend to choose the Yen sign.
    //
    //  Unicode Yen or Won signs as directory separators will fail.
    //
    //  So, we will check this path name for Yen or won signs and, if they are
    //  there, we'll replace them with slashes.
    //
    //  A further twist:  we replace Yen and Won with forward slashes rather
    //   than back slashes.  Either form of slash will work as a directory
    //   separator.  On Win 95 and 98, though, Unicode back-slashes may
    //   fail to transode back to 8-bit 0x5C with some Unicode converters
    //   to  some of the problematic code pages.  Forward slashes always
    //   transcode correctly back to 8 bit char * form.
    //
    XMLCh *tmpUName = 0;

    const XMLCh* srcPtr = nameToOpen;
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
        tmpUName = XMLString::replicate(nameToOpen, manager);

        XMLCh* tmpPtr = tmpUName;
        while (*tmpPtr)
        {
            if (*tmpPtr == chYenSign ||
                *tmpPtr == chWonSign)
                *tmpPtr = chForwardSlash;
            tmpPtr++;
        }
        nameToOpen = tmpUName;
    }
    FileHandle retVal = 0;
    if (gOnNT)
    {
        retVal = ::CreateFileW
            (
            (LPCWSTR) nameToOpen
            , GENERIC_READ
            , FILE_SHARE_READ
            , 0
            , OPEN_EXISTING
            , FILE_FLAG_SEQUENTIAL_SCAN
            , 0
            );
    }
    else
    {
        //
        //  We are Win 95 / 98.  Take the Unicode file name back to (char *)
        //    so that we can open it.
        //
        char* tmpName = XMLString::transcode(nameToOpen, manager);
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
        manager->deallocate(tmpName);//delete [] tmpName;
    }

    if (tmpUName)
        manager->deallocate(tmpUName);//delete [] tmpUName;

    if (retVal == INVALID_HANDLE_VALUE)
        return 0;

    return retVal;
}

FileHandle XMLPlatformUtils::openFileToWrite(const char* const fileName
                                             , MemoryManager* const manager)
{
    FileHandle retVal = ::CreateFileA
    (
        fileName
        , GENERIC_WRITE
        , 0              // no shared write
        , 0
        , CREATE_ALWAYS
        , FILE_ATTRIBUTE_NORMAL
        , 0
    );

    if (retVal == INVALID_HANDLE_VALUE)
        return 0;

    return retVal;
}

FileHandle XMLPlatformUtils::openFileToWrite(const XMLCh* const fileName
                                             , MemoryManager* const manager)
{
    // Watch for obvious wierdness
    if (!fileName)
        return 0;

    //  Ok, this might look stupid but its a semi-expedient way to deal
    //  with a thorny problem. Shift-JIS and some other Asian encodings
    //  are fundamentally broken and map both the backslash and the Yen
    //  sign to the same code point. Transcoders have to pick one or the
    //  other to map '\' to Unicode and tend to choose the Yen sign.
    //
    //  Unicode Yen or Won signs as directory separators will fail.
    //
    //  So, we will check this path name for Yen or won signs and, if they are
    //  there, we'll replace them with slashes.
    //
    //  A further twist:  we replace Yen and Won with forward slashes rather
    //   than back slashes.  Either form of slash will work as a directory
    //   separator.  On Win 95 and 98, though, Unicode back-slashes may
    //   fail to transode back to 8-bit 0x5C with some Unicode converters
    //   to  some of the problematic code pages.  Forward slashes always
    //   transcode correctly back to 8 bit char * form.
    //
    XMLCh *tmpUName = 0;
    const XMLCh *nameToOpen = fileName;

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
        tmpUName = XMLString::replicate(fileName, manager);

        XMLCh* tmpPtr = tmpUName;
        while (*tmpPtr)
        {
            if (*tmpPtr == chYenSign ||
                *tmpPtr == chWonSign)
                *tmpPtr = chForwardSlash;
            tmpPtr++;
        }
        nameToOpen = tmpUName;
    }
    FileHandle retVal = 0;
    if (gOnNT)
    {
        retVal = ::CreateFileW
            (
            (LPCWSTR) nameToOpen
            , GENERIC_WRITE
            , 0              // no shared write
            , 0
            , CREATE_ALWAYS
            , FILE_ATTRIBUTE_NORMAL
            , 0
            );
    }
    else
    {
        //
        //  We are Win 95 / 98.  Take the Unicode file name back to (char *)
        //    so that we can open it.
        //
        char* tmpName = XMLString::transcode(nameToOpen, manager);
        retVal = ::CreateFileA
            (
            tmpName
            , GENERIC_WRITE
            , 0              // no shared write
            , 0
            , CREATE_ALWAYS
            , FILE_ATTRIBUTE_NORMAL
            , 0
            );
        manager->deallocate(tmpName);//delete [] tmpName;
    }

    if (tmpUName)
        manager->deallocate(tmpUName);//delete [] tmpUName;

    if (retVal == INVALID_HANDLE_VALUE)
        return 0;

    return retVal;
}

FileHandle XMLPlatformUtils::openStdInHandle(MemoryManager* const manager)
{
    //
    //  Get the standard input handle. Duplicate it and return that copy
    //  since the outside world cannot tell the difference and will shut
    //  down this handle when its done with it. If we gave out the orignal,
    //  shutting it would prevent any further output.
    //
    HANDLE stdInOrg = ::GetStdHandle(STD_INPUT_HANDLE);
    if (stdInOrg == INVALID_HANDLE_VALUE) {
        XMLCh stdinStr[] = {chLatin_s, chLatin_t, chLatin_d, chLatin_i, chLatin_n, chNull};
        ThrowXMLwithMemMgr1(XMLPlatformUtilsException, XMLExcepts::File_CouldNotOpenFile, stdinStr, manager);
    }

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
        ThrowXMLwithMemMgr(XMLPlatformUtilsException, XMLExcepts::File_CouldNotDupHandle, manager);
    }
    return retHandle;
}


unsigned int
XMLPlatformUtils::readFileBuffer(       FileHandle      theFile
                                , const unsigned int    toRead
                                ,       XMLByte* const  toFill
                                , MemoryManager* const  manager)
{
    unsigned long bytesRead = 0;
    if (!::ReadFile(theFile, toFill, toRead, &bytesRead, 0))
    {
        //
        //  Check specially for a broken pipe error. If we get this, it just
        //  means no more data from the pipe, so return zero.
        //
        if (::GetLastError() != ERROR_BROKEN_PIPE)
            ThrowXMLwithMemMgr(XMLPlatformUtilsException, XMLExcepts::File_CouldNotReadFromFile, manager);
    }
    return (unsigned int)bytesRead;
}

void
XMLPlatformUtils::writeBufferToFile( FileHandle     const  theFile
                                   , long                  toWrite
                                   , const XMLByte* const  toFlush
                                   , MemoryManager* const  manager)
{
    if (!theFile        ||
        (toWrite <= 0 ) ||
        !toFlush         )
        return;

    const XMLByte* tmpFlush = (const XMLByte*) toFlush;
    unsigned long  bytesWritten = 0;

    while (true)
    {
        if (!::WriteFile(theFile, tmpFlush, toWrite, &bytesWritten, 0))
            ThrowXMLwithMemMgr(XMLPlatformUtilsException, XMLExcepts::File_CouldNotWriteToFile, manager);

        if (bytesWritten < (unsigned long) toWrite) //incomplete write
        {
            tmpFlush+=bytesWritten;
            toWrite-=bytesWritten;
            bytesWritten=0;
        }
        else
            return;
    }

    return;
}

void XMLPlatformUtils::resetFile(FileHandle theFile
                                 , MemoryManager* const manager)
{
    // Seek to the start of the file
    if (::SetFilePointer(theFile, 0, 0, FILE_BEGIN) == 0xFFFFFFFF)
        ThrowXMLwithMemMgr(XMLPlatformUtilsException, XMLExcepts::File_CouldNotResetFile, manager);

}


// ---------------------------------------------------------------------------
//  XMLPlatformUtils: File system methods
// ---------------------------------------------------------------------------
XMLCh* XMLPlatformUtils::getFullPath(const XMLCh* const srcPath,
                                     MemoryManager* const manager)
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
        if (!::GetFullPathNameW((LPCWSTR)srcPath, bufSize, (LPWSTR)tmpPath, (LPWSTR*)&namePart))
            return 0;

        // Return a copy of the path
        return XMLString::replicate(tmpPath, manager);
    }
     else
    {
        // Transcode the incoming string
        char* tmpSrcPath = XMLString::transcode(srcPath, fgMemoryManager);
        ArrayJanitor<char> janSrcPath(tmpSrcPath, fgMemoryManager);

        // Use a local buffer that is big enough for the largest legal path
        const unsigned int bufSize = 511;
        char tmpPath[511 + 1];

        char* namePart = 0;
        if (!::GetFullPathNameA(tmpSrcPath, bufSize, tmpPath, &namePart))
            return 0;

        // Return a transcoded copy of the path
        return XMLString::transcode(tmpPath, manager);
    }
}

bool XMLPlatformUtils::isRelative(const XMLCh* const toCheck
                                  , MemoryManager* const manager)
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
    //  it's a remote file.
    //
    if (isBackSlash(toCheck[0]) && isBackSlash(toCheck[1]))
        return false;

    // Else assume its a relative path
    return true;
}

XMLCh* XMLPlatformUtils::getCurrentDirectory(MemoryManager* const manager)
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

        if (!::GetCurrentDirectoryW(bufSize, (LPWSTR)tmpPath))
            return 0;

        // Return a copy of the path
        return XMLString::replicate(tmpPath, manager);
    }
     else
    {
        // Use a local buffer that is big enough for the largest legal path
        const unsigned int bufSize = 511;
        char tmpPath[511 + 1];

        if (!::GetCurrentDirectoryA(bufSize, tmpPath))
            return 0;

        // Return a transcoded copy of the path
        return XMLString::transcode(tmpPath, manager);
    }
}

inline bool XMLPlatformUtils::isAnySlash(XMLCh c)
{
    return c == chBackSlash    ||
           c == chForwardSlash ||
           c == chYenSign      ||
           c == chWonSign;
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
    delete (CRITICAL_SECTION*)mtxHandle;
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
#if defined WIN64
    return ::InterlockedCompareExchangePointer(toFill, (void*)newValue, (void*)toCompare);
#else

    //
    //  InterlockedCompareExchange is only supported on Windows 98,
    //  Windows NT 4.0, and newer -- not on Windows 95...
    //  If you are willing to give up Win95 support change this to #if 0
    //  otherwise we are back to using assembler.
    //  (But only if building with compilers that support inline assembler.)
    //
    #if (defined(_MSC_VER) || defined(__BCPLUSPLUS__)) && !defined(XERCES_NO_ASM)

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

    return (void*) ::InterlockedCompareExchange((LPLONG)toFill, (LONG)newValue, (LONG)toCompare);

    #endif

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
#if defined (XML_USE_INMEM_MESSAGELOADER)
    return new InMemMsgLoader(msgDomain);
#elif defined (XML_USE_WIN32_MSGLOADER)
    return new Win32MsgLoader(msgDomain);
#elif defined (XML_USE_ICU_MESSAGELOADER)
    return new ICUMsgLoader(msgDomain);
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
#elif defined (XML_USE_CYGWIN_TRANSCODER)
    return new CygwinTransService;
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

#if 0 && defined(_DEBUG)
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

#include <xercesc/util/LogicalPath.c>

XERCES_CPP_NAMESPACE_END
