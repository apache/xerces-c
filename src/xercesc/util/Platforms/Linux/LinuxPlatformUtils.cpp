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
 * Revision 1.15  2003/04/25 17:19:53  peiyongz
 * throw exception if getcwd() fails
 *
 * Revision 1.14  2003/04/24 02:54:46  peiyongz
 * Logical Path Resolution
 *
 * Revision 1.13  2003/04/21 04:28:45  peiyongz
 * performance tuning
 *
 * Revision 1.12  2003/03/09 16:56:11  peiyongz
 * PanicHandler
 *
 * Revision 1.11  2003/02/05 18:27:34  tng
 * [Bug 13437] Incorrect memory management in LinuxPlatformUtils.cpp.  Fixed by Adam Zell.
 *
 * Revision 1.10  2002/12/02 19:16:46  tng
 * [Bug 14723] Memory leak in atomicOpsMutex.  Patch from Adam Zell.
 *
 * Revision 1.9  2002/11/07 22:38:13  peiyongz
 * build -miconv on hpux and linux
 *
 * Revision 1.8  2002/11/04 15:13:00  tng
 * C++ Namespace Support.
 *
 * Revision 1.7  2002/08/19 19:38:17  tng
 * [Bug 11771] Linux specific IconvGNU transcoder.  Patch from Vasily Tchekalkin.
 *
 * Revision 1.6  2002/07/15 21:53:04  peiyongz
 * CouldNotWriteToFile
 *
 * Revision 1.5  2002/06/24 15:55:21  peiyongz
 * Change from James Berry( jberry@criticalpath.com)
 *
 * Revision 1.4  2002/06/20 15:28:08  peiyongz
 * Fix to compilation error
 *
 * Revision 1.2  2002/05/21 20:31:47  tng
 * Minor update: Remove obsolete code
 *
 * Revision 1.1.1.1  2002/02/01 22:22:24  peiyongz
 * sane_include
 *
 * Revision 1.22  2001/10/25 15:36:35  tng
 * [Bug 4318] Single threaded build fails due to obsolete #define .
 *
 * Revision 1.21  2001/10/25 15:20:31  tng
 * Need to guard with NO_APP_THREADS when destroying the mutex.
 *
 * Revision 1.20  2001/10/23 23:10:48  peiyongz
 * [Bug#880] patch to PlatformUtils:init()/term() and related. from Mark Weaver
 *
 * Revision 1.19  2001/05/11 12:03:40  tng
 * Need to add header <linux/limits> for definition of PATH_MAX
 *
 * Revision 1.18  2001/05/10 20:40:44  lehors
 * built-in buffer limit could be smaller than system limit,
 * we now use PATH_MAX instead - patch from Christian Schuhegger - bug #1158
 *
 * Revision 1.17  2000/07/25 22:29:55  aruna1
 * Char definitions in XMLUni moved to XMLUniDefs
 *
 * Revision 1.16  2000/03/24 00:13:03  aruna1
 * Platform initialization taken care for both threaded and non-threaded environment
 *
 * Revision 1.15  2000/03/20 23:48:52  rahulj
 * Added Socket based NetAccessor. This will enable one to
 * use HTTP URL's for system id's. Default build options do
 * not use this NetAccessor. Specify the '-n socket' option
 * to 'runConfigure' to configure Xerces-C to use this new
 * feature. The code works under Solaris 2.6, Linux, AIX
 * and HPUX 11 with aCC.
 * Todo's: enable proper error handling.
 *
 * Revision 1.14  2000/03/18 00:00:00  roddey
 * Initial updates for two way transcoding support
 *
 * Revision 1.13  2000/03/02 21:10:36  abagchi
 * Added empty function platformTerm()
 *
 * Revision 1.12  2000/03/02 20:42:41  abagchi
 * Fixed typo in XMLExcepts
 *
 * Revision 1.11  2000/03/02 19:55:24  roddey
 * This checkin includes many changes done while waiting for the
 * 1.1.0 code to be finished. I can't list them all here, but a list is
 * available elsewhere.
 *
 * Revision 1.10  2000/02/22 00:58:15  aruna1
 * openFile API updated
 *
 * Revision 1.9  2000/02/06 07:48:28  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.8  2000/01/25 22:32:56  aruna1
 * Updated panic information
 *
 * Revision 1.7  2000/01/19 23:21:37  abagchi
 * Made this file compatible with ICU 1.4
 *
 * Revision 1.6  2000/01/19 17:37:48  abagchi
 * Removed the streaming classes
 *
 * Revision 1.5  2000/01/14 02:04:43  abagchi
 * Introduced getFullPath() and weavePath()
 *
 * Revision 1.4  1999/12/14 23:53:33  rahulj
 * Removed the offending Ctrl-M's from the commit message
 * logs which was giving packaging problems.
 *
 * PR:
 * Obtained from:
 * Submitted by:
 * Reviewed by:
 *
 * Revision 1.3  1999/11/23 02:00:48  rahulj
 * Code now works under HPUX 11. Tested inmemory message loader.
 * Revamped makefiles. Builds with both DCE threads as well as pthread libraries.
 *
 * Revision 1.2  1999/11/17 22:35:33  rahulj
 * Replaced default attr mutexes with recursive mutexes. Also code works with ICU transcoding service
 *
 * Revision 1.1.1.1  1999/11/09 01:07:01  twl
 * Initial checkin
 *
 * Revision 1.6  1999/11/08 20:45:30  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------

#if !defined(APP_NO_THREADS)
#include    <pthread.h>
#endif

#ifndef _GNU_SOURCE
    #error _GNU_SOURCE is not defined in your compile settings
#endif

#include    <unistd.h>
#include    <stdio.h>
#include    <stdlib.h>
#include    <errno.h>
#include    <libgen.h>
#include    <linux/limits.h>           //for PATH_MAX
#include    <sys/timeb.h>
#include    <string.h>
#include    <xercesc/util/PlatformUtils.hpp>
#include    <xercesc/util/RuntimeException.hpp>
#include    <xercesc/util/Janitor.hpp>
#include    <xercesc/util/Mutexes.hpp>
#include    <xercesc/util/XMLString.hpp>
#include    <xercesc/util/XMLUniDefs.hpp>
#include    <xercesc/util/XMLUni.hpp>
#include    <xercesc/util/PanicHandler.hpp>

#if defined(XML_USE_ICU_TRANSCODER)
    #include <xercesc/util/Transcoders/ICU/ICUTransService.hpp>
#elif defined (XML_USE_GNU_TRANSCODER)
    #include <xercesc/util/Transcoders/IconvGNU/IconvGNUTransService.hpp>
#else
    // Use native transcoder. Same as -DXML_USE_NATIVE_TRANSCODER
    #include <xercesc/util/Transcoders/Iconv/IconvTransService.hpp>
#endif


#if defined(XML_USE_ICU_MESSAGELOADER)
    #include <xercesc/util/MsgLoaders/ICU/ICUMsgLoader.hpp>
#elif defined (XML_USE_ICONV_MESSAGELOADER)
    #include <xercesc/util/MsgLoaders/MsgCatalog/MsgCatalogLoader.hpp>
#else
    // Same as -DXML_USE_INMEM_MESSAGELOADER
    #include <xercesc/util/MsgLoaders/InMemory/InMemMsgLoader.hpp>
#endif


#if defined (XML_USE_NETACCESSOR_SOCKET)
    #include <xercesc/util/NetAccessors/Socket/SocketNetAccessor.hpp>
#endif


XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  XMLPlatformUtils: Private Static Methods
// ---------------------------------------------------------------------------

XMLNetAccessor* XMLPlatformUtils::makeNetAccessor()
{
#if defined (XML_USE_NETACCESSOR_SOCKET)
    return new SocketNetAccessor();
#else
    return 0;
#endif
}



//
//  This method is called by the platform independent part of this class
//  when client code asks to have one of the supported message sets loaded.
//

XMLMsgLoader* XMLPlatformUtils::loadAMsgSet(const XMLCh* const msgDomain)
{
    XMLMsgLoader* retVal;
    try
    {
#if defined (XML_USE_ICU_MESSAGELOADER)
        retVal = new ICUMsgLoader(msgDomain);
#elif defined (XML_USE_ICONV_MESSAGELOADER)
        retVal = new MsgCatalogLoader(msgDomain);
#else
        // same as -DXML_USE_INMEM_MESSAGELOADER
        retVal = new InMemMsgLoader(msgDomain);
#endif
    }

    catch(...)
    {
        panic(PanicHandler::Panic_CantLoadMsgDomain);
    }
    return retVal;
}


//
//  This method is called very early in the bootstrapping process. This guy
//  must create a transcoding service and return it. It cannot use any string
//  methods, any transcoding services, throw any exceptions, etc... It just
//  makes a transcoding service and returns it, or returns zero on failure.
//

XMLTransService* XMLPlatformUtils::makeTransService()
{
#if defined (XML_USE_ICU_TRANSCODER)
    // Use ICU transcoding services.
    // same as -DXML_USE_ICU_MESSAGELOADER
    return new ICUTransService;
#elif defined (XML_USE_GNU_TRANSCODER)
    return new IconvGNUTransService;
#else
    // Use native transcoding services.
    // same as -DXML_USE_NATIVE_TRANSCODER
    return new IconvTransService;

#endif
}

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

unsigned int XMLPlatformUtils::curFilePos(FileHandle theFile)
{
    int curPos = ftell( (FILE*)theFile);
    if (curPos == -1)
        ThrowXML(XMLPlatformUtilsException,
                 XMLExcepts::File_CouldNotGetSize);

    return (unsigned int)curPos;
}

void XMLPlatformUtils::closeFile(FileHandle theFile)
{
    if (fclose((FILE*)theFile))
        ThrowXML(XMLPlatformUtilsException,
                 XMLExcepts::File_CouldNotCloseFile);
}

unsigned int XMLPlatformUtils::fileSize(FileHandle theFile)
{
    // Get the current position
    long  int curPos = ftell((FILE*) theFile);
    if (curPos == -1)
        ThrowXML(XMLPlatformUtilsException,
                 XMLExcepts::File_CouldNotGetCurPos);

    // Seek to the end and save that value for return
     if (fseek((FILE*) theFile, 0, SEEK_END))
        ThrowXML(XMLPlatformUtilsException,
                 XMLExcepts::File_CouldNotSeekToEnd);

    long int retVal = ftell((FILE*)theFile);
    if (retVal == -1)
        ThrowXML(XMLPlatformUtilsException,
                 XMLExcepts::File_CouldNotSeekToEnd);

    // And put the pointer back

    if (fseek( (FILE*)theFile, curPos, SEEK_SET) )
        ThrowXML(XMLPlatformUtilsException,
                 XMLExcepts::File_CouldNotSeekToPos);

    return (unsigned int)retVal;
}

FileHandle XMLPlatformUtils::openFile(const XMLCh* const fileName)
{
    const char* tmpFileName = XMLString::transcode(fileName);
    ArrayJanitor<char> janText((char*)tmpFileName);
    FileHandle retVal = (FILE*)fopen( tmpFileName , "rb" );

    if (retVal == NULL)
        return 0;
    return retVal;
}

FileHandle XMLPlatformUtils::openFile(const char* const fileName)
{
    FileHandle retVal = (FILE*)fopen( fileName , "rb" );

    if (retVal == NULL)
        return 0;
    return retVal;
}

FileHandle XMLPlatformUtils::openFileToWrite(const XMLCh* const fileName)
{
    const char* tmpFileName = XMLString::transcode(fileName);
    ArrayJanitor<char> janText((char*)tmpFileName);
    return fopen( tmpFileName , "wb" );
}

FileHandle XMLPlatformUtils::openFileToWrite(const char* const fileName)
{
    return fopen( fileName , "wb" );
}

FileHandle XMLPlatformUtils::openStdInHandle()
{
    return (FileHandle)fdopen(dup(0), "rb");
}

unsigned int
XMLPlatformUtils::readFileBuffer( FileHandle          theFile
                                , const unsigned int  toRead
                                , XMLByte* const      toFill)
{
    size_t noOfItemsRead = fread((void*) toFill, 1, toRead, (FILE*)theFile);

    if(ferror((FILE*)theFile))
    {
        ThrowXML(XMLPlatformUtilsException,
                 XMLExcepts::File_CouldNotReadFromFile);
    }

    return (unsigned int)noOfItemsRead;
}

void
XMLPlatformUtils::writeBufferToFile( FileHandle     const  theFile
                                   , long                  toWrite
                                   , const XMLByte* const  toFlush)
{
    if (!theFile        ||
        (toWrite <= 0 ) ||
        !toFlush         )
        return;

    const XMLByte* tmpFlush = (const XMLByte*) toFlush;
    size_t bytesWritten = 0;

    while (true)
    {
        bytesWritten=fwrite(tmpFlush, sizeof(XMLByte), toWrite, (FILE*)theFile);

        if(ferror((FILE*)theFile))
        {
            ThrowXML(XMLPlatformUtilsException, XMLExcepts::File_CouldNotWriteToFile);
        }

        if (bytesWritten < toWrite) //incomplete write
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

void XMLPlatformUtils::resetFile(FileHandle theFile)
{
    // Seek to the start of the file
    if (fseek((FILE*)theFile, 0, SEEK_SET))
        ThrowXML(XMLPlatformUtilsException,
                 XMLExcepts::File_CouldNotResetFile);
}


// ---------------------------------------------------------------------------
//  XMLPlatformUtils: File system methods
// ---------------------------------------------------------------------------
XMLCh* XMLPlatformUtils::getFullPath(const XMLCh* const srcPath)
{

    //
    //  NOTE: THe path provided has always already been opened successfully,
    //  so we know that its not some pathological freaky path. It comes in
    //  in native format, and goes out as Unicode always
    //
    char* newSrc = XMLString::transcode(srcPath);
    ArrayJanitor<char> janText(newSrc);

    // Use a local buffer that is big enough for the largest legal path
    char absPath[PATH_MAX + 1];
    // get the absolute path
    char* retPath = realpath(newSrc, &absPath[0]);

    if (!retPath)
    {
        ThrowXML(XMLPlatformUtilsException, XMLExcepts::File_CouldNotGetBasePathName);
    }
    return XMLString::transcode(absPath);
}

bool XMLPlatformUtils::isRelative(const XMLCh* const toCheck)
{
    // Check for pathological case of empty path
    if (!toCheck[0])
        return false;

    //
    //  If it starts with a slash, then it cannot be relative. This covers
    //  both something like "\Test\File.xml" and an NT Lan type remote path
    //  that starts with a node like "\\MyNode\Test\File.xml".
    //
    if (toCheck[0] == XMLCh('/'))
        return false;

    // Else assume its a relative path
    return true;
}

XMLCh* XMLPlatformUtils::getCurrentDirectory()
{
    char  dirBuf[PATH_MAX + 1];
    char  *curDir = getcwd(&dirBuf[0], PATH_MAX + 1);

    if (!curDir)
    {
        ThrowXML(XMLPlatformUtilsException,
                 XMLExcepts::File_CouldNotGetBasePathName);
    }

    return XMLString::transcode(curDir);
}

inline bool XMLPlatformUtils::isAnySlash(XMLCh c) 
{
    return ( chBackSlash == c || chForwardSlash == c);
}

// ---------------------------------------------------------------------------
//  XMLPlatformUtils: Timing Methods
// ---------------------------------------------------------------------------

unsigned long XMLPlatformUtils::getCurrentMillis()
{
    timeb aTime;
    ftime(&aTime);
    return (unsigned long)(aTime.time*1000 + aTime.millitm);

}

// -----------------------------------------------------------------------
//  Mutex methods
// -----------------------------------------------------------------------

#if !defined(APP_NO_THREADS)

// ---------------------------------------------------------------------------
//  XMLPlatformUtils: Platform init method
// ---------------------------------------------------------------------------

static XMLMutex atomicOpsMutex;

void XMLPlatformUtils::platformInit()
{
    //
    // The atomicOps mutex needs to be created early.
    // Normally, mutexes are created on first use, but there is a
    // circular dependency between compareAndExchange() and
    // mutex creation that must be broken.
    if (atomicOpsMutex.fHandle == 0)
        atomicOpsMutex.fHandle = XMLPlatformUtils::makeMutex();
}

void* XMLPlatformUtils::makeMutex()
{
    pthread_mutex_t* mutex = new pthread_mutex_t;
    pthread_mutexattr_t*  attr = new pthread_mutexattr_t;
    pthread_mutexattr_init(attr);
    pthread_mutexattr_settype(attr, PTHREAD_MUTEX_RECURSIVE_NP);
    if (pthread_mutex_init(mutex, attr))
    {
        ThrowXML(XMLPlatformUtilsException,
                 XMLExcepts::Mutex_CouldNotCreate);
    }
    pthread_mutexattr_destroy(attr);
    delete attr;
    return (void*)(mutex);

}

void XMLPlatformUtils::closeMutex(void* const mtxHandle)
{
    if (mtxHandle != NULL)
    {
        if (pthread_mutex_destroy((pthread_mutex_t*) mtxHandle))
        {
            ThrowXML(XMLPlatformUtilsException,
                     XMLExcepts::Mutex_CouldNotDestroy);
        }
        delete (pthread_mutex_t*) mtxHandle;
    }
}


void XMLPlatformUtils::lockMutex(void* const mtxHandle)
{
    if (mtxHandle != NULL)
    {
        if (pthread_mutex_lock((pthread_mutex_t*) mtxHandle))
        {
            ThrowXML(XMLPlatformUtilsException,
                     XMLExcepts::Mutex_CouldNotLock);
        }
    }
}


void XMLPlatformUtils::unlockMutex(void* const mtxHandle)
{
    if (mtxHandle != NULL)
    {
        if (pthread_mutex_unlock((pthread_mutex_t*) mtxHandle))
        {
            ThrowXML(XMLPlatformUtilsException,
                     XMLExcepts::Mutex_CouldNotUnlock);
        }
    }
}


// -----------------------------------------------------------------------
//  Miscellaneous synchronization methods
// -----------------------------------------------------------------------

void* XMLPlatformUtils::compareAndSwap(void**            toFill
                                     , const void* const newValue
                                     , const void* const toCompare)
{
    XMLMutexLock lockMutex(&atomicOpsMutex);

    void *retVal = *toFill;
    if (*toFill == toCompare)
        *toFill = (void *)newValue;

    return retVal;
}

int XMLPlatformUtils::atomicIncrement(int &location)
{
    XMLMutexLock localLock(&atomicOpsMutex);

    return ++location;
}

int XMLPlatformUtils::atomicDecrement(int &location)
{
    XMLMutexLock localLock(&atomicOpsMutex);

    return --location;
}

#else // #if !defined (APP_NO_THREADS)

void XMLPlatformUtils::platformInit()
{
}

void* XMLPlatformUtils::makeMutex()
{
        return 0;
}

void XMLPlatformUtils::closeMutex(void* const mtxHandle)
{
}

void XMLPlatformUtils::lockMutex(void* const mtxHandle)
{
}

void XMLPlatformUtils::unlockMutex(void* const mtxHandle)
{
}

void* XMLPlatformUtils::compareAndSwap (void**             toFill,
                                        const void* const  newValue,
                                        const void* const  toCompare)
{
    void *retVal = *toFill;
    if (*toFill == toCompare)
       *toFill = (void *)newValue;
    return retVal;
}

int XMLPlatformUtils::atomicIncrement(int &location)
{
    return ++location;
}

int XMLPlatformUtils::atomicDecrement(int &location)
{
    return --location;
}

#endif // APP_NO_THREADS

void XMLPlatformUtils::platformTerm()
{
#if !defined(APP_NO_THREADS)
    // delete the mutex we created
	closeMutex(atomicOpsMutex.fHandle);
	atomicOpsMutex.fHandle = 0;
#endif
}

#include <xercesc/util/LogicalPath.c>

XERCES_CPP_NAMESPACE_END

