/*
 * Copyright 2001-2004 The Apache Software Foundation.
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
 * Revision 1.24  2005/04/05 18:36:00  cargilld
 * Change platform mutex code to do a panic instead of throwing an exception as the exception code uses mutexes and this can result in infinite recursion.
 *
 * Revision 1.23  2004/09/08 13:56:39  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.22  2004/01/29 11:51:20  cargilld
 * Code cleanup changes to get rid of various compiler diagnostic messages.
 *
 * Revision 1.21  2004/01/13 16:34:21  cargilld
 * Misc memory management changes.
 *
 * Revision 1.20  2004/01/06 17:31:20  neilg
 * fix static initialization problems, bug 28517; thanks to Reid Spencer
 *
 * Revision 1.19  2003/12/24 15:24:14  cargilld
 * More updates to memory management so that the static memory manager.
 *
 * Revision 1.18  2003/12/19 23:02:25  cargilld
 * More memory management updates.
 *
 * Revision 1.17  2003/12/17 15:16:10  cargilld
 * Platform update for memory management so that the static memory manager (one
 * used to call Initialize) is only for static data.
 *
 * Revision 1.16  2003/12/17 13:58:02  cargilld
 * Platform update for memory management so that the static memory manager (one
 * used to call Initialize) is only for static data.
 *
 * Revision 1.15  2003/12/02 15:04:52  amassari
 * openFile was opening the file for read and write, while only read was required (bug#20169)
 *
 * Revision 1.14  2003/10/01 16:32:39  neilg
 * improve handling of out of memory conditions, bug #23415.  Thanks to David Cargill.
 *
 * Revision 1.13  2003/05/15 18:37:47  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.12  2003/04/25 17:19:10  peiyongz
 * throw exception if getcwd() fails
 *
 * Revision 1.11  2003/04/24 02:53:29  peiyongz
 * Logical Path Resolution
 *
 * Revision 1.10  2003/04/21 04:27:22  peiyongz
 * performance tuning
 *
 * Revision 1.9  2003/03/09 16:53:27  peiyongz
 * PanicHandler
 *
 * Revision 1.8  2003/02/23 04:36:55  jberry
 * Minor bug fixes to FreeBSD port
 *
 * Revision 1.7  2002/12/12 16:28:48  peiyongz
 * MsgCatalogLoader added.
 *
 * Revision 1.6  2002/12/02 19:16:26  tng
 * [Bug 14723] Memory leak in atomicOpsMutex.  Patch from Adam Zell.
 *
 * Revision 1.5  2002/11/04 15:13:00  tng
 * C++ Namespace Support.
 *
 * Revision 1.4  2002/07/05 21:29:58  peiyongz
 * Bug# 10250: patch from James Berry
 *
 * Revision 1.3  2002/07/05 21:25:20  peiyongz
 * Bug# 10250: Implementation of new platform methods,
 *                      patch from max@cca.usart.ru (Max Gotlib)
 *
 * Revision 1.2  2002/05/21 20:31:47  tng
 * Minor update: Remove obsolete code
 *
 * Revision 1.1.1.1  2002/02/01 22:22:24  peiyongz
 * sane_include
 *
 * Revision 1.3  2001/12/03 16:25:00  tng
 * [Bug 5237] PATH_MAX undefined during build without threading support.
 *
 * Revision 1.2  2001/12/03 14:45:11  tng
 * FreeBSD native transcoder (IconvFBSD) added by Max Gotlib.
 *
 * Revision 1.1  2001/11/29 18:25:18  tng
 * FreeBSD support by Michael Huedepohl.
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
// #include    <libgen.h>
#include    <sys/time.h>
#include    <string.h>
#include    <limits.h>
#include    <xercesc/util/PlatformUtils.hpp>
#include    <xercesc/util/RuntimeException.hpp>
#include    <xercesc/util/Janitor.hpp>
#include    <xercesc/util/Mutexes.hpp>
#include    <xercesc/util/XMLString.hpp>
#include    <xercesc/util/XMLUniDefs.hpp>
#include    <xercesc/util/XMLUni.hpp>
#include    <xercesc/util/PanicHandler.hpp>
#include    <xercesc/util/OutOfMemoryException.hpp>

#if defined(XML_USE_ICU_TRANSCODER)
    #include <xercesc/util/Transcoders/ICU/ICUTransService.hpp>
#else
    // Use native transcoder. Same as -DXML_USE_NATIVE_TRANSCODER
    #include <xercesc/util/Transcoders/IconvFBSD/IconvFBSDTransService.hpp>
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
    catch(const OutOfMemoryException&)
    {
        throw;
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
#else
    // Use native transcoding services.
    // same as -DXML_USE_INMEM_MESSAGELOADER
    return new IconvFBSDTransService;

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

unsigned int XMLPlatformUtils::curFilePos(FileHandle theFile
                                          , MemoryManager* const manager)
{
    if (theFile == NULL)
	ThrowXMLwithMemMgr(XMLPlatformUtilsException,
		 XMLExcepts::CPtr_PointerIsZero, manager);
    off_t curPos = ftell( (FILE*)theFile);
    if (curPos == -1)
        ThrowXMLwithMemMgr(XMLPlatformUtilsException,
                 XMLExcepts::File_CouldNotGetSize, manager);

    return (unsigned int)curPos;
}

void XMLPlatformUtils::closeFile(FileHandle theFile
                                 , MemoryManager* const manager)
{
    if (theFile == NULL)
	ThrowXMLwithMemMgr(XMLPlatformUtilsException,
		 XMLExcepts::CPtr_PointerIsZero, manager);
    if (fclose((FILE*)theFile))
        ThrowXMLwithMemMgr(XMLPlatformUtilsException,
                 XMLExcepts::File_CouldNotCloseFile, manager);
}

unsigned int XMLPlatformUtils::fileSize(FileHandle theFile
                                        , MemoryManager* const manager)
{
    if (theFile == NULL)
	ThrowXMLwithMemMgr(XMLPlatformUtilsException,
		 XMLExcepts::CPtr_PointerIsZero, manager);
    // Get the current position
    off_t curPos = ftell((FILE*) theFile);
    if (curPos == -1)
        ThrowXMLwithMemMgr(XMLPlatformUtilsException,
                 XMLExcepts::File_CouldNotGetCurPos, manager);

    // Seek to the end and save that value for return
    if (fseek((FILE*) theFile, 0, SEEK_END))
        ThrowXMLwithMemMgr(XMLPlatformUtilsException,
                 XMLExcepts::File_CouldNotSeekToEnd, manager);

    off_t retVal = ftell((FILE*)theFile);
    if (retVal == -1)
        ThrowXMLwithMemMgr(XMLPlatformUtilsException,
                 XMLExcepts::File_CouldNotSeekToEnd, manager);

    // And put the pointer back
    if (fseek( (FILE*)theFile, curPos, SEEK_SET) )
        ThrowXMLwithMemMgr(XMLPlatformUtilsException,
                 XMLExcepts::File_CouldNotSeekToPos, manager);

    return (unsigned int)retVal;
}

FileHandle XMLPlatformUtils::openFile(const XMLCh* const fileName
                                      , MemoryManager* const manager)
{
    if (fileName == NULL)
	    ThrowXMLwithMemMgr(XMLPlatformUtilsException,
		    XMLExcepts::CPtr_PointerIsZero, manager);
    const char* tmpFileName = XMLString::transcode(fileName, manager);
    ArrayJanitor<char> janText((char*)tmpFileName, manager);
    FileHandle retVal = (FileHandle)fopen( tmpFileName , "r" );
    return retVal;
}

FileHandle XMLPlatformUtils::openFile(const char* const fileName
                                      , MemoryManager* const manager)
{
    if (fileName == NULL)
	    ThrowXMLwithMemMgr(XMLPlatformUtilsException,
		 XMLExcepts::CPtr_PointerIsZero, manager);
    FileHandle retVal = (FileHandle)fopen( fileName , "r" );
    return retVal;
}

FileHandle XMLPlatformUtils::openFileToWrite(const XMLCh* const fileName
                                             , MemoryManager* const manager)
{
    if (fileName == NULL)
	    ThrowXMLwithMemMgr(XMLPlatformUtilsException,
		 XMLExcepts::CPtr_PointerIsZero, manager);
    const char* tmpFileName = XMLString::transcode(fileName, manager);
    ArrayJanitor<char> janText((char*)tmpFileName, manager);
    return fopen( tmpFileName , "w" );
}

FileHandle XMLPlatformUtils::openFileToWrite(const char* const fileName
                                             , MemoryManager* const manager)
{
    if (fileName == NULL)
	    ThrowXMLwithMemMgr(XMLPlatformUtilsException,
		 XMLExcepts::CPtr_PointerIsZero, manager);
    return fopen( fileName , "w" );
}

FileHandle XMLPlatformUtils::openStdInHandle(MemoryManager* const manager)
{
    int nfd = dup(0);
    if (nfd == -1)
	    ThrowXMLwithMemMgr(XMLPlatformUtilsException,
		    XMLExcepts::File_CouldNotDupHandle, manager);
    return (FileHandle) fdopen(nfd, "r");
}

unsigned int
XMLPlatformUtils::readFileBuffer( FileHandle          theFile
                                , const unsigned int  toRead
                                , XMLByte* const      toFill
                                , MemoryManager* const manager)
{
    if ( !theFile || !toFill )
	    ThrowXMLwithMemMgr(XMLPlatformUtilsException,
		    XMLExcepts::CPtr_PointerIsZero, manager);
    if (toRead == 0)
	return 0;
    size_t noOfItemsRead = fread((void*) toFill, 1, toRead, (FILE*)theFile);

    if(ferror((FILE*)theFile))
    {
        ThrowXMLwithMemMgr(XMLPlatformUtilsException,
                 XMLExcepts::File_CouldNotReadFromFile, manager);
    }

    return (unsigned int)noOfItemsRead;
}

void XMLPlatformUtils::writeBufferToFile( FileHandle     const  theFile
					, long                  toWrite
					, const XMLByte* const  toFlush 
                    , MemoryManager* const  manager)
{
    if ( !theFile || !toFlush )
        ThrowXMLwithMemMgr(XMLPlatformUtilsException,
		    XMLExcepts::CPtr_PointerIsZero, manager);
    if ( toWrite <= 0 )
        return;

    const XMLByte* tmpFlush = (const XMLByte*) toFlush;
    size_t bytesWritten = 0;

    while (true)
    {
        bytesWritten = fwrite(tmpFlush, sizeof(XMLByte), toWrite, (FILE*)theFile);

        if(ferror((FILE*)theFile))
		{
			ThrowXMLwithMemMgr(XMLPlatformUtilsException,
				XMLExcepts::File_CouldNotWriteToFile, manager);
		}

        if (bytesWritten < (size_t) toWrite) //incomplete write
        {
            tmpFlush += bytesWritten;
            toWrite -= bytesWritten;
            bytesWritten = 0;
        }
        else
            return;
    }
}

void XMLPlatformUtils::resetFile(FileHandle theFile
                                 , MemoryManager* const manager)
{
    if (theFile == NULL)
	ThrowXMLwithMemMgr(XMLPlatformUtilsException,
		 XMLExcepts::CPtr_PointerIsZero, manager);
    // Seek to the start of the file
    if (fseek((FILE*)theFile, 0, SEEK_SET))
        ThrowXMLwithMemMgr(XMLPlatformUtilsException,
                 XMLExcepts::File_CouldNotResetFile, manager);
}


// ---------------------------------------------------------------------------
//  XMLPlatformUtils: File system methods
// ---------------------------------------------------------------------------
XMLCh* XMLPlatformUtils::getFullPath(const XMLCh* const srcPath,
                                     MemoryManager* const manager)
{

    //
    //  NOTE: THe path provided has always already been opened successfully,
    //  so we know that its not some pathological freaky path. It comes in
    //  in native format, and goes out as Unicode always
    //
    char* newSrc = XMLString::transcode(srcPath, manager);
    ArrayJanitor<char> janText(newSrc, manager);

    // Use a local buffer that is big enough for the largest legal path
    char absPath[PATH_MAX + 1];
    // get the absolute path
    char* retPath = realpath(newSrc, &absPath[0]);

    if (!retPath)
    {
        ThrowXMLwithMemMgr(XMLPlatformUtilsException, XMLExcepts::File_CouldNotGetBasePathName, manager);
    }
    return XMLString::transcode(absPath, manager);
}

bool XMLPlatformUtils::isRelative(const XMLCh* const toCheck
                                  , MemoryManager* const manager)
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

XMLCh* XMLPlatformUtils::getCurrentDirectory(MemoryManager* const manager)
{
    char  dirBuf[PATH_MAX + 2];
    char  *curDir = getcwd(&dirBuf[0], PATH_MAX + 1);

    if (!curDir)
    {
        ThrowXMLwithMemMgr(XMLPlatformUtilsException,
                 XMLExcepts::File_CouldNotGetBasePathName, manager);
    }

    return XMLString::transcode(curDir, manager);
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
    struct timeval aTime;
    gettimeofday(&aTime, NULL);
    return (unsigned long) (aTime.tv_sec * 1000 + aTime.tv_usec / 1000);

}

// -----------------------------------------------------------------------
//  Mutex methods
// -----------------------------------------------------------------------

#if !defined(APP_NO_THREADS)

// ---------------------------------------------------------------------------
//  XMLPlatformUtils: Platform init method
// ---------------------------------------------------------------------------

static XMLMutex *atomicOpsMutex = 0;

void XMLPlatformUtils::platformInit()
{
    //
    // The atomicOps mutex needs to be created early.
    // Normally, mutexes are created on first use, but there is a
    // circular dependency between compareAndExchange() and
    // mutex creation that must be broken.
    if(!atomicOpsMutex)
    {
        atomicOpsMutex = new (fgMemoryManager) XMLMutex();
        if (atomicOpsMutex->fHandle == 0)
            atomicOpsMutex->fHandle = XMLPlatformUtils::makeMutex();
    }
}

void* XMLPlatformUtils::makeMutex()
{
    pthread_mutex_t* mutex = new pthread_mutex_t;
    pthread_mutexattr_t*  attr = new pthread_mutexattr_t;
    pthread_mutexattr_init(attr);
    pthread_mutexattr_settype(attr, PTHREAD_MUTEX_RECURSIVE);
    if (pthread_mutex_init(mutex, attr))
    {
        panic(PanicHandler::Panic_MutexErr);
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
            ThrowXMLwithMemMgr(XMLPlatformUtilsException,
                     XMLExcepts::Mutex_CouldNotDestroy, fgMemoryManager);
        }
        delete (pthread_mutex_t*)mtxHandle;
    }
}


void XMLPlatformUtils::lockMutex(void* const mtxHandle)
{
    if (mtxHandle != NULL)
    {
        if (pthread_mutex_lock((pthread_mutex_t*) mtxHandle))
        {
            panic(PanicHandler::Panic_MutexErr);
        }
    }
}


void XMLPlatformUtils::unlockMutex(void* const mtxHandle)
{
    if (mtxHandle != NULL)
    {
        if (pthread_mutex_unlock((pthread_mutex_t*) mtxHandle))
        {
            panic(PanicHandler::Panic_MutexErr);
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
    XMLMutexLock lockMutex(atomicOpsMutex);

    void *retVal = *toFill;
    if (*toFill == toCompare)
        *toFill = (void *)newValue;

    return retVal;
}

int XMLPlatformUtils::atomicIncrement(int &location)
{
    XMLMutexLock localLock(atomicOpsMutex);

    return ++location;
}

int XMLPlatformUtils::atomicDecrement(int &location)
{
    XMLMutexLock localLock(atomicOpsMutex);

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
	closeMutex(atomicOpsMutex->fHandle);
	atomicOpsMutex->fHandle = 0;
    delete atomicOpsMutex;
    atomicOpsMutex = 0;
#endif
}

#include <xercesc/util/LogicalPath.c>

XERCES_CPP_NAMESPACE_END
