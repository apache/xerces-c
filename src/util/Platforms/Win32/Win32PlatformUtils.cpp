/*
 * The Apache Software License, Version 1.1
 * 
 * Copyright (c) 1999 The Apache Software Foundation.  All rights 
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

/**
 * $Log$
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

#if defined (XML_USE_ICU_TRANSCODER)
	#include <util/Transcoders/ICU/ICUTransService.hpp>
#elif defined (XML_USE_WIN32_TRANSCODER)
	#include <util/Transcoders/Win32/Win32TransService.hpp>
#else
	#error A transcoding service must be chosen
#endif

#if defined (XML_USE_INMEMORY_MSGLOADER)
	#include <util/MsgLoaders/InMemory/InMemMsgLoader.hpp>
#elif defined (XML_USE_WIN32_MSGLOADER)
	#include <util/MsgLoaders/Win32/Win32MsgLoader.hpp>
#else
	#error A message loading service must be chosen
#endif


// ---------------------------------------------------------------------------
//  Local data
//
//  gOnNT
//      We figure out during init if we are on NT or not. If we are, then
//      we can avoid a lot of transcoding in our system services stuff.
//
//  gStdErr
//  gStdOut
//      The file handles for standard error and standard out. We set these
//      up during init. Note that they can be zero if there are no std
//      handles, 
//
//  gStdErrRedir
//  gStdOutRedir
//      These flags are set to indicate whether their respective output
//      handles are redirected. If they are not, then we can use console
//      APIs on NT to write Unicode straight to the output. Otherwise we have
//      to use file APIs, and we transcode it.
// ---------------------------------------------------------------------------
static bool     gOnNT;
static HANDLE   gStdErr;
static bool     gStdErrRedir;
static HANDLE   gStdOut;
static bool     gStdOutRedir;


// ---------------------------------------------------------------------------
//  Local methods
// ---------------------------------------------------------------------------
static void WriteCharStrStdErr(const char* const toWrite)
{
    // We always just use the file APIs for these
    DWORD written;
    if (!::WriteFile
    (
        gStdErr
        , toWrite
        , strlen(toWrite)
        , &written
        , 0))
    {
        //
        //  If if fails due to an invalid handle, then just assume that our
        //  handles were disconnected and zero it out. This will prevent us
        //  from getting called again.
        //
        if (::GetLastError() == ERROR_INVALID_HANDLE)
            gStdErr = 0;
        else
            ThrowXML(XMLPlatformUtilsException, XML4CExcepts::Strm_StdErrWriteFailure);
    }
}


static void WriteCharStrStdOut(const char* const toWrite)
{
    // We always just use the file APIs for these
    DWORD written;
    if (!::WriteFile
    (
        gStdOut
        , toWrite
        , strlen(toWrite)
        , &written
        , 0))
    {
        //
        //  If if fails due to an invalid handle, then just assume that our
        //  handles were disconnected and zero it out. This will prevent us
        //  from getting called again.
        //
        if (::GetLastError() == ERROR_INVALID_HANDLE)
            gStdOut = 0;
        else
            ThrowXML(XMLPlatformUtilsException, XML4CExcepts::Strm_StdOutWriteFailure);
    }
}


static void WriteUStrStdErr(const XMLCh* const toWrite)
{
    //
    //  If we are on NT and the handle is not redirected, then we can use
    //  the console API directly to send out Unicode. Otherwise we have to
    //  use the file APIs and transcode.
    //
    DWORD written;
    if (gOnNT && !gStdErrRedir)
    {
        if (!::WriteConsoleW
        (
            gStdErr
            , toWrite
            , XMLString::stringLen(toWrite)
            , &written
            , 0))
        {
            ThrowXML(XMLPlatformUtilsException, XML4CExcepts::Strm_ConWriteFailure);
        }
        return;
    }

    // Oh well, got to do it the hard way
    char* tmpVal = XMLString::transcode(toWrite);
    ArrayJanitor<char> janTmp(tmpVal);
    WriteCharStrStdErr(tmpVal);
}


static void WriteUStrStdOut(const XMLCh* const toWrite)
{
    //
    //  If we are on NT and the handle is not redirected, then we can use
    //  the console API directly to send out Unicode. Otherwise we have to
    //  use the file APIs and transcode.
    //
    DWORD written;
    if (gOnNT && !gStdOutRedir)
    {
        if (!::WriteConsoleW
        (
            gStdOut
            , toWrite
            , XMLString::stringLen(toWrite)
            , &written
            , 0))
        {
            ThrowXML(XMLPlatformUtilsException, XML4CExcepts::Strm_ConWriteFailure);
        }
        return;
    }

    // Oh well, got to do it the hard way
    char* tmpVal = XMLString::transcode(toWrite);
    ArrayJanitor<char> janTmp(tmpVal);
    WriteCharStrStdOut(tmpVal);
}



// ---------------------------------------------------------------------------
//  XMLPlatformUtils: The panic method
// ---------------------------------------------------------------------------
void XMLPlatformUtils::panic(const PanicReasons reason)
{
    const char* reasonStr = "Uknown reason";
    if (reason == Panic_NoTransService)
        reasonStr = "Could not load a transcoding service";
    else if (reason == Panic_NoDefTranscoder)
        reasonStr = "Could not load a local code page transcoder";
    else if (reason == Panic_CantFindLib)
        reasonStr = "Could not find the XML4C DLL";
    else if (reason == Panic_UnknownMsgDomain)
        reasonStr = "Unknown message domain";
    else if (reason == Panic_CantLoadMsgDomain)
        reasonStr = "Cannot load message domain";

    //
    //  We just do a popup and exit. Replace this code to do whatever
    //  you need to do.
    //
    MessageBoxA
    (
        0
        , "XML4C Panic Error"
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
        ThrowXML(XMLPlatformUtilsException, XML4CExcepts::File_CouldNotGetCurPos);

    return curPos;
}

void XMLPlatformUtils::closeFile(FileHandle theFile)
{
    if (!::CloseHandle(theFile))
        ThrowXML(XMLPlatformUtilsException, XML4CExcepts::File_CouldNotCloseFile);
}

unsigned int XMLPlatformUtils::fileSize(const FileHandle theFile)
{
    // Get the current position
    const unsigned int curPos = ::SetFilePointer(theFile, 0, 0, FILE_CURRENT);
    if (curPos == 0xFFFFFFFF)
        ThrowXML(XMLPlatformUtilsException, XML4CExcepts::File_CouldNotGetCurPos);

    // Seek to the end and save that value for return
    const unsigned int retVal = ::SetFilePointer(theFile, 0, 0, FILE_END);
    if (curPos == 0xFFFFFFFF)
        ThrowXML(XMLPlatformUtilsException, XML4CExcepts::File_CouldNotSeekToEnd);

    // And put the pointer back
    if (::SetFilePointer(theFile, curPos, 0, FILE_BEGIN) == 0xFFFFFFFF)
        ThrowXML(XMLPlatformUtilsException, XML4CExcepts::File_CouldNotSeekToPos);

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
    FileHandle retVal = 0;
    if (gOnNT)
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
        ThrowXML(XMLPlatformUtilsException, XML4CExcepts::File_CouldNotOpenFile);

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
        ThrowXML(XMLPlatformUtilsException, XML4CExcepts::File_CouldNotDupHandle);
    }
    return retHandle;
}


unsigned int
XMLPlatformUtils::readFileBuffer(       FileHandle      theFile
                                , const unsigned int    toRead
                                ,       XMLByte* const  toFill)
{
    unsigned long bytesRead;
    if (!::ReadFile(theFile, toFill, toRead, &bytesRead, 0))
        ThrowXML(XMLPlatformUtilsException, XML4CExcepts::File_CouldNotReadFromFile);
    return (unsigned int)bytesRead;
}


void XMLPlatformUtils::resetFile(FileHandle theFile)
{
    // Seek to the start of the file
    if (::SetFilePointer(theFile, 0, 0, FILE_BEGIN) == 0xFFFFFFFF)
        ThrowXML(XMLPlatformUtilsException, XML4CExcepts::File_CouldNotResetFile);
}


// ---------------------------------------------------------------------------
//  XMLPlatformUtils: File Methods
// ---------------------------------------------------------------------------
void XMLPlatformUtils::writeToStdErr(const XMLCh* const toWrite)
{
    // If handles never got opened, then eat the output, else output
    if (gStdErr)
        WriteUStrStdErr(toWrite);
}

void XMLPlatformUtils::writeToStdErr(const char* const toWrite)
{
    // If handles never got opened, then eat the output, else output
    if (gStdErr)
        WriteCharStrStdErr(toWrite);
}

void XMLPlatformUtils::writeToStdOut(const XMLCh* const toWrite)
{
    // If handles never got opened, then eat the output, else output
    if (gStdOut)
        WriteUStrStdOut(toWrite);
}

void XMLPlatformUtils::writeToStdOut(const char* const toWrite)
{
    // If handles never got opened, then eat the output, else output
    if (gStdOut)
        WriteCharStrStdOut(toWrite);
}



// ---------------------------------------------------------------------------
//  XMLPlatformUtils: File system methods
// ---------------------------------------------------------------------------
XMLCh* XMLPlatformUtils::getBasePath(const XMLCh* const srcPath)
{
    //
    //  NOTE: THe path provided has always already been opened successfully,
    //  so we know that its not some pathological freaky path.
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

        // Cap it off at the name part, leaving just the full path
        if (namePart)
            *namePart = 0;

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

        // Cap it off at the name part, leaving just the full path
        if (namePart)
            *namePart = 0;

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
    if (!::CloseHandle(mtxHandle))
        ThrowXML(XMLPlatformUtilsException, XML4CExcepts::Mutex_CouldNotClose);
}


void XMLPlatformUtils::lockMutex(void* const mtxHandle)
{
    unsigned int res = ::WaitForSingleObject(mtxHandle, INFINITE);
    if (res == WAIT_FAILED)
        ThrowXML(XMLPlatformUtilsException, XML4CExcepts::Mutex_CouldNotLock);
}


void* XMLPlatformUtils::makeMutex()
{
    HANDLE hRet = ::CreateMutex(0, 0, 0);
    if (!hRet)
        ThrowXML(XMLPlatformUtilsException, XML4CExcepts::Mutex_CouldNotCreate);
    return hRet;
}


void XMLPlatformUtils::unlockMutex(void* const mtxHandle)
{
    if (!::ReleaseMutex(mtxHandle))
        ThrowXML(XMLPlatformUtilsException, XML4CExcepts::Mutex_CouldNotUnlock);
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
    // Windows supports InterlockedCompareExchange only on Windows 98, NT 4.0,
    //  and newer. Not on Win 95. So we are back to using assembler.
    //
    #if defined(DEVENV_VCPP)

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

    #elif defined(XML_IBMVAW32) 

    // <TBD> Why is this not using the interlocked call below?
    void  *retval = *toFill;
    if( *toFill == toCompare)
       *toFill = (void *) newValue;
    return retVal;

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
//  we want to use.
//
//  <TBD> For now we return zero, but later we will return the desired type
//  of accessor object.
//
XMLNetAccessor* XMLPlatformUtils::makeNetAccessor()
{
    return 0;
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
    char tmpBuf[4096];
    if (!::GetEnvironmentVariableA("ICU_DATA", tmpBuf, 4096))
    {
        strcpy(tmpBuf, fgLibLocation);
        strcat(tmpBuf, "icu\\data\\");
        ICUTransService::setICUPath(tmpBuf);
    }
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
    //
    //  Lets get our own DLL path and store it. The fgLibLocation static
    //  member must be filled in with the path to the shared Lib or DLL
    //  so that other code can find any files relative to it.
    //
    HINSTANCE hmod = ::GetModuleHandleA(XML4C_DLLName);
    if (!hmod)
    {
        //
        //  If we didn't find it, its probably because its a development
        //  build which is built as separate DLLs, so lets look for the DLL
        //  that we are part of.
        //
        static const char* const privDLLName = "IXUTIL";
        hmod = ::GetModuleHandle(privDLLName);

        // If neither exists, then we give up
        if (!hmod)
            panic(Panic_CantFindLib);
    }

    //
    //  Get the path to our module. We explicitly get the ASCII version here
    //  since its stored as ASCII (or the local code page to be more specific,
    //  so it might be EBCDIC on some platforms.)
    //
    char tmpBuf[MAX_PATH + 1];
    if (!::GetModuleFileNameA(hmod, tmpBuf, MAX_PATH))
        panic(Panic_CantFindLib);

    // Find the last separator in the list and put a null in the next char
    char* sepPtr = 0;
    sepPtr = strrchr(tmpBuf, '\\');
    if (sepPtr)
        *(sepPtr+1)= 0;
    const unsigned int pathLen = strlen(tmpBuf);

    // Allocate a buffer and copy the text into it. Then store it in the static
    char* actualBuf = new char[pathLen + 1];
    strcpy(actualBuf, tmpBuf);
    fgLibLocation = actualBuf;

    //
    //  Figure out if we are on NT and save that flag for later use.
    //
    OSVERSIONINFO   OSVer;
    OSVer.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    ::GetVersionEx(&OSVer);
    gOnNT = (OSVer.dwPlatformId == VER_PLATFORM_WIN32_NT);

    //
    //  Ok, we have to do a little dance here to determine if we have any
    //  standard output handles. First we open up the potentially redirected
    //  standard handles.
    //
    gStdOut = ::GetStdHandle(STD_OUTPUT_HANDLE);
    gStdErr = ::GetStdHandle(STD_ERROR_HANDLE);

    //
    //  If we got the handles, then get the console mode for them. If this
    //  fails, then assume for the time being that they are just redirected
    //  files.
    //
    //  Above, when they are actually used, if they fail because of an
    //  invalid handle error, the gStdOut and gStdErr handles will get zeroed
    //  out all further output will be eaten.
    //
    DWORD dummyParm;
    if (gStdOut)
    {
        if (!::GetConsoleMode(gStdOut, &dummyParm))
            gStdOutRedir = true;
    }

    if (gStdErr)
    {
        if (!::GetConsoleMode(gStdErr, &dummyParm))
            gStdErrRedir = true;
    }
}
