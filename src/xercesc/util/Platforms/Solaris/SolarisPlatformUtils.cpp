/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 1999-2004 The Apache Software Foundation.  All rights
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

#if !defined (APP_NO_THREADS)
#  if defined (XML_USE_DCE)
#    include  <dce/pthread.h>
#  else
#    include    <pthread.h>
#  endif
#endif // APP_NO_THREADS


#include    <unistd.h>
#include    <stdio.h>
#include    <stdlib.h>
#include    <errno.h>
#include    <libgen.h>
#include    <sys/timeb.h>
#include    <string.h>
#include    <link.h>
#include    <limits.h>
#include    <dlfcn.h>
#include    <sys/types.h>
#include    <sys/stat.h>
#include    <fcntl.h>
#include    <xercesc/util/Janitor.hpp>
#include    <xercesc/util/PlatformUtils.hpp>
#include    <xercesc/util/RuntimeException.hpp>
#include    <xercesc/util/Mutexes.hpp>
#include    <xercesc/util/XMLString.hpp>
#include    <xercesc/util/XMLUniDefs.hpp>
#include    <xercesc/util/XMLUni.hpp>
#include    <xercesc/util/PanicHandler.hpp>
#include    <xercesc/util/OutOfMemoryException.hpp>

#if defined (XML_USE_ICU_TRANSCODER)
    #include <xercesc/util/Transcoders/ICU/ICUTransService.hpp>
#else   // use native transcoder
    #include <xercesc/util/Transcoders/Iconv/IconvTransService.hpp>
#endif

#if defined (XML_USE_ICU_MESSAGELOADER)
    #include <xercesc/util/MsgLoaders/ICU/ICUMsgLoader.hpp>
#elif defined (XML_USE_ICONV_MESSAGELOADER)
    #include <xercesc/util/MsgLoaders/MsgCatalog/MsgCatalogLoader.hpp>
#else   // use In-memory message loader
    #include <xercesc/util/MsgLoaders/InMemory/InMemMsgLoader.hpp>
#endif


#if defined (XML_USE_NETACCESSOR_SOCKET)
    #include <xercesc/util/NetAccessors/Socket/SocketNetAccessor.hpp>
#endif


#if defined (SOLARIS)
extern "C" int ftime(struct timeb *); // Solaris headers missing this decl
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
//  In our case, we use the ICU based message loader mechanism.
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

#if defined (XML_USE_ICU_TRANSCODER)
{
    return new ICUTransService;
}
#elif defined (XML_USE_ICONV_TRANSCODER)
{
    return new IconvTransService;
}
#else // Use Native transcoding service
{
    return new IconvTransService;

}
#endif


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
    // Get the current position
    int curPos = tell( (int)theFile);
    if (curPos == -1)
        ThrowXMLwithMemMgr(XMLPlatformUtilsException,
                 XMLExcepts::File_CouldNotGetSize, manager);

    return (unsigned int)curPos;
}

void XMLPlatformUtils::closeFile(FileHandle theFile
                                 , MemoryManager* const manager)
{
    if (close((int) theFile))
        ThrowXMLwithMemMgr(XMLPlatformUtilsException,
                 XMLExcepts::File_CouldNotCloseFile, manager);
}

unsigned int XMLPlatformUtils::fileSize(FileHandle theFile
                                        , MemoryManager* const manager)
{
    // Get the current position
    long  int curPos = tell((int) theFile);
    if (curPos == -1)
        ThrowXMLwithMemMgr(XMLPlatformUtilsException,
                 XMLExcepts::File_CouldNotGetCurPos, manager);

    // Seek to the end and save that value for return
    if (lseek( (int) theFile, 0, SEEK_END) == (off_t)-1)
        ThrowXMLwithMemMgr(XMLPlatformUtilsException,
                 XMLExcepts::File_CouldNotSeekToEnd, manager);

    long int retVal = tell((int) theFile);
    if (retVal == -1)
        ThrowXMLwithMemMgr(XMLPlatformUtilsException,
                 XMLExcepts::File_CouldNotSeekToEnd, manager);

    // And put the pointer back
    if (lseek((int) theFile, curPos, SEEK_SET) == (off_t)-1)
        ThrowXMLwithMemMgr(XMLPlatformUtilsException,
                 XMLExcepts::File_CouldNotSeekToPos, manager);

    return (unsigned int)retVal;
}

FileHandle XMLPlatformUtils::openFile(const char* const fileName
                                      , MemoryManager* const manager)
{
    int retVal = open( fileName , O_RDONLY | O_LARGEFILE);

    if (retVal == -1)
        return 0;
    return (FileHandle)retVal;
}

FileHandle XMLPlatformUtils::openFile(const XMLCh* const fileName
                                      , MemoryManager* const manager)
{
    const char* tmpFileName = XMLString::transcode(fileName, manager);
    ArrayJanitor<char> janText((char*)tmpFileName, manager);
    int retVal = open( tmpFileName , O_RDONLY | O_LARGEFILE);

    if (retVal == -1)
        return 0;
    return (FileHandle)retVal;
}

FileHandle XMLPlatformUtils::openFileToWrite(const XMLCh* const fileName
                                             , MemoryManager* const manager)
{
    const char* tmpFileName = XMLString::transcode(fileName, manager);
    ArrayJanitor<char> janText((char*)tmpFileName, manager);
    return (FileHandle)open( tmpFileName , O_WRONLY | O_CREAT | O_LARGEFILE, 0666);
}

FileHandle XMLPlatformUtils::openFileToWrite(const char* const fileName
                                             , MemoryManager* const manager)
{
    return (FileHandle)open( fileName , O_WRONLY | O_CREAT | O_LARGEFILE, 0666);
}

unsigned int
XMLPlatformUtils::readFileBuffer(FileHandle              theFile
                               , const unsigned int      toRead
                               , XMLByte* const          toFill
                               , MemoryManager* const    manager)
{
    ssize_t noOfItemsRead =
      read((int) theFile, (void*) toFill, toRead);

    if(noOfItemsRead == -1)
    {
        ThrowXMLwithMemMgr(XMLPlatformUtilsException,
                 XMLExcepts::File_CouldNotReadFromFile, manager);
    }

    return (unsigned int) noOfItemsRead;
}

void
XMLPlatformUtils::writeBufferToFile( FileHandle     const  theFile
                                   , long                  toWrite
                                   , const XMLByte* const  toFlush
                                   , MemoryManager*  const manager)
{
    if (!theFile        ||
        (toWrite <= 0 ) ||
        !toFlush         )
        return;

    const XMLByte* tmpFlush = (const XMLByte*) toFlush;
    ssize_t bytesWritten = 0;

    while (true)
    {
      bytesWritten=write((int)theFile, (const void *)tmpFlush, toWrite);

        if(bytesWritten == -1)
        {
            ThrowXMLwithMemMgr(XMLPlatformUtilsException, XMLExcepts::File_CouldNotWriteToFile, manager);
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

void XMLPlatformUtils::resetFile(FileHandle theFile
                                 , MemoryManager* const manager)
{
    // Seek to the start of the file
    if (lseek((int) theFile, 0, SEEK_SET) == -1)
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
    //get the absolute path
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
    timeb aTime;
    ftime(&aTime);
    return (unsigned long)(aTime.time*1000 + aTime.millitm);
}


// -----------------------------------------------------------------------
//  Mutex methods
// -----------------------------------------------------------------------

#if !defined (APP_NO_THREADS)

// ---------------------------------------------------------------------------
//  XMLPlatformUtils: Platform init method
// ---------------------------------------------------------------------------

static pthread_mutex_t* gAtomicOpMutex =0 ;

void XMLPlatformUtils::platformInit()
{
    //
    // The gAtomicOpMutex mutex needs to be created
    // because compareAndSwap and incrementlocation and decrementlocation
    // does not have the atomic system calls for usage
    // Normally, mutexes are created on first use, but there is a
    // circular dependency between compareAndExchange() and
    // mutex creation that must be broken.

    gAtomicOpMutex = new pthread_mutex_t;

#if defined(XML_USE_DCE)
    if (pthread_mutex_init(gAtomicOpMutex, pthread_mutexattr_default)) {
	delete gAtomicOpMutex;
	gAtomicOpMutex = 0;
        panic( PanicHandler::Panic_SystemInit );
    }
#else // XML_USE_DCE
    if (pthread_mutex_init(gAtomicOpMutex, NULL)) {
	delete gAtomicOpMutex;
	gAtomicOpMutex = 0;
        panic( PanicHandler::Panic_SystemInit );
    }
#endif // XML_USE_DCE
}

#ifndef XML_USE_DCE
// inlining the class with dce threading causes segmentation fault
class  RecursiveMutex
{
public:
    pthread_mutex_t   mutex;
    int               recursionCount;
    pthread_t         tid;

    RecursiveMutex() {
               if (pthread_mutex_init(&mutex, NULL))
                   ThrowXMLwithMemMgr(XMLPlatformUtilsException, XMLExcepts::Mutex_CouldNotCreate, XMLPlatformUtils::fgMemoryManager);
                       recursionCount = 0;
                       tid = 0;
                     }

    ~RecursiveMutex() {
            if (pthread_mutex_destroy(&mutex))
                ThrowXMLwithMemMgr(XMLPlatformUtilsException, XMLExcepts::Mutex_CouldNotDestroy, XMLPlatformUtils::fgMemoryManager);
                      }

     void lock()      {
              if (pthread_equal(tid, pthread_self()))
              {
                  recursionCount++;
                  return;
              }
              if (pthread_mutex_lock(&mutex) != 0)
                  ThrowXMLwithMemMgr(XMLPlatformUtilsException, XMLExcepts::Mutex_CouldNotLock, XMLPlatformUtils::fgMemoryManager);
              tid = pthread_self();
              recursionCount = 1;
              }


     void unlock()    {
                          if (--recursionCount > 0)
                              return;

              if (pthread_mutex_unlock(&mutex) != 0)
                  ThrowXMLwithMemMgr(XMLPlatformUtilsException, XMLExcepts::Mutex_CouldNotUnlock, XMLPlatformUtils::fgMemoryManager);
                          tid = 0;
                       }
   };
#endif // ifndef XML_USE_DCE

void* XMLPlatformUtils::makeMutex()
{
#if defined(XML_USE_DCE)
    pthread_mutex_t* mutex = new pthread_mutex_t;
    if (mutex ==  NULL)
    {
        ThrowXMLwithMemMgr(XMLPlatformUtilsException,
                 XMLExcepts::Mutex_CouldNotCreate, fgMemoryManager);
    }
    pthread_mutexattr_t attr;
    pthread_mutexattr_create(&attr);
    pthread_mutexattr_setkind_np(&attr, MUTEX_RECURSIVE_NP);
    if (pthread_mutex_init(mutex, attr))
    {
        ThrowXMLwithMemMgr(XMLPlatformUtilsException,
                 XMLExcepts::Mutex_CouldNotCreate, fgMemoryManager);
    }
    pthread_mutexattr_delete(&attr);
    return (void*)(mutex);
#else
    return new RecursiveMutex;
#endif
}


void XMLPlatformUtils::closeMutex(void* const mtxHandle)
{
    if (mtxHandle == NULL)
        return;
#if defined(XML_USE_DCE)
    pthread_mutex_t *rm = (pthread_mutex_t *)mtxHandle;
    pthread_mutex_destroy(rm);
#else
    RecursiveMutex *rm = (RecursiveMutex *)mtxHandle;
#endif
    delete rm;
}


void XMLPlatformUtils::lockMutex(void* const mtxHandle)
{
    if (mtxHandle == NULL)
        return;
#if defined(XML_USE_DCE)
    pthread_mutex_t *rm = (pthread_mutex_t *)mtxHandle;
    pthread_mutex_lock(rm);
#else
    RecursiveMutex *rm = (RecursiveMutex *)mtxHandle;
    rm->lock();
#endif
}

void XMLPlatformUtils::unlockMutex(void* const mtxHandle)
{
    if (mtxHandle == NULL)
        return;
#if defined(XML_USE_DCE)
    pthread_mutex_t *rm = (pthread_mutex_t *)mtxHandle;
    pthread_mutex_unlock(rm);
#else
    RecursiveMutex *rm = (RecursiveMutex *)mtxHandle;
    rm->unlock();
#endif
}

// -----------------------------------------------------------------------
//  Miscellaneous synchronization methods
// -----------------------------------------------------------------------
//atomic system calls in Solaris is only restricted to kernel libraries
//So, to make operations thread safe we implement static mutex and lock
//the atomic operations. It makes the process slow but what's the alternative!

void* XMLPlatformUtils::compareAndSwap ( void**      toFill ,
                    const void* const newValue ,
                    const void* const toCompare)
{
    //return ((void*)cas32( (uint32_t*)toFill,  (uint32_t)toCompare, (uint32_t)newValue) );
    // the below calls are temporarily made till the above functions are part of user library
    // Currently its supported only in the kernel mode

    if (pthread_mutex_lock( gAtomicOpMutex))
        panic(PanicHandler::Panic_SynchronizationErr);

    void *retVal = *toFill;
    if (*toFill == toCompare)
              *toFill = (void *)newValue;

    if (pthread_mutex_unlock( gAtomicOpMutex))
        panic(PanicHandler::Panic_SynchronizationErr);

    return retVal;
}

int XMLPlatformUtils::atomicIncrement(int &location)
{
    //return (int)atomic_add_32_nv( (uint32_t*)&location, 1);

    if (pthread_mutex_lock( gAtomicOpMutex))
        panic(PanicHandler::Panic_SynchronizationErr);

    int tmp = ++location;

    if (pthread_mutex_unlock( gAtomicOpMutex))
        panic(PanicHandler::Panic_SynchronizationErr);

    return tmp;
}
int XMLPlatformUtils::atomicDecrement(int &location)
{
    //return (int)atomic_add_32_nv( (uint32_t*)&location, -1);

    if (pthread_mutex_lock( gAtomicOpMutex))
        panic(PanicHandler::Panic_SynchronizationErr);

    int tmp = --location;

    if (pthread_mutex_unlock( gAtomicOpMutex))
        panic(PanicHandler::Panic_SynchronizationErr);

    return tmp;
}

#else // #if !defined (APP_NO_THREADS)

void XMLPlatformUtils::platformInit()
{
}

void XMLPlatformUtils::closeMutex(void* const mtxHandle)
{
}

void XMLPlatformUtils::lockMutex(void* const mtxHandle)
{
}

void* XMLPlatformUtils::makeMutex()
{
        return 0;
}

void XMLPlatformUtils::unlockMutex(void* const mtxHandle)
{
}

void* XMLPlatformUtils::compareAndSwap ( void**      toFill,
                                   const void* const newValue,
                                   const void* const toCompare)
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



FileHandle XMLPlatformUtils::openStdInHandle(MemoryManager* const manager)
{
        return (FileHandle)dup(0);
}

void XMLPlatformUtils::platformTerm()
{
#if !defined(APP_NO_THREADS)
	pthread_mutex_destroy(gAtomicOpMutex);
    delete gAtomicOpMutex;
	gAtomicOpMutex = 0;
#endif
}

#include <xercesc/util/LogicalPath.c>

XERCES_CPP_NAMESPACE_END
