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
 * $Log$
 * Revision 1.5  2003/03/09 16:57:18  peiyongz
 * PanicHandler
 *
 * Revision 1.4  2002/11/19 13:04:32  gareth
 * Bug# 14661 Caldera implemented openFileToWrite and writeBufferToFile. Patch from Cameron Dorrat.
 *
 * Revision 1.3  2002/11/04 15:13:01  tng
 * C++ Namespace Support.
 *
 * Revision 1.2  2002/05/21 20:31:47  tng
 * Minor update: Remove obsolete code
 *
 * Revision 1.1  2002/02/27 22:57:59  peiyongz
 * Bug# 6445 Caldera (SCO) OpenServer Port : patch from Martin Kalen
 *
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------

#if !defined (APP_NO_THREADS)
#include     <pthread.h>
#endif


#include    <unistd.h>
#include    <stdio.h>
#include    <stdlib.h>
#include    <errno.h>
#include    <libgen.h>
#include    <sys/time.h>
#include    <netinet/in.h>
#include    <arpa/inet.h>
#include    <xercesc/util/PlatformUtils.hpp>
#include    <xercesc/util/RuntimeException.hpp>
#include    <xercesc/util/Janitor.hpp>
#include    <xercesc/util/Mutexes.hpp>
#include    <xercesc/util/XMLString.hpp>
#include    <xercesc/util/XMLUniDefs.hpp>
#include    <xercesc/util/XMLUni.hpp>
#include    <xercesc/util/PanicHandler.hpp>

#if defined (XML_USE_ICU_TRANSCODER)
#   include <xercesc/util/Transcoders/ICU/ICUTransService.hpp>
#else
//  Same as -DXML_USE_NATIVE_TRANSCODER
#   include <xercesc/util/Transcoders/Iconv/IconvTransService.hpp>
#endif


#if defined(XML_USE_ICU_MESSAGELOADER)
#   include <xercesc/util/MsgLoaders/ICU/ICUMsgLoader.hpp>
#elif defined (XML_USE_ICONV_MESSAGELOADER)
#   include <xercesc/util/MsgLoaders/MsgCatalog/MsgCatalogLoader.hpp>
#else
//  Same as -DXML_USE_INMEM_MESSAGELOADER
#   include <xercesc/util/MsgLoaders/InMemory/InMemMsgLoader.hpp>
#endif


#if defined (XML_USE_NETACCESSOR_LIBWWW)
#   include <xercesc/util/NetAccessors/libWWW/LibWWWNetAccessor.hpp>
#else
//  Same as -DXML_USE_NETACCESSOR_SOCKET
#   include <xercesc/util/NetAccessors/Socket/SocketNetAccessor.hpp>
#endif

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  XMLPlatformUtils: Private Static Methods
// ---------------------------------------------------------------------------

//
//  This method is called by the platform independent part of this class
//  when client code asks to have one of the supported message sets loaded.
//
XMLMsgLoader* XMLPlatformUtils::loadAMsgSet(const XMLCh* const msgDomain)
{
    XMLMsgLoader* retVal = 0;
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
    catch(...)
    {
        panic(PanicHandler::Panic_CantLoadMsgDomain);
    }
    return retVal;
}


XMLNetAccessor* XMLPlatformUtils::makeNetAccessor()
{
#if defined (XML_USE_NETACCESSOR_LIBWWW)
    return new LibWWWNetAccessor();
#else
    return new SocketNetAccessor();
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
#if defined (XML_USE_ICU_TRANSCODER)
	return new ICUTransService;
#else
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
    // Get the current position
    int curPos = ftell((FILE*)theFile);
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
    long int curPos = ftell((FILE*)theFile);
    if (curPos == -1)
        ThrowXML(XMLPlatformUtilsException,
                 XMLExcepts::File_CouldNotGetCurPos);

    // Seek to the end and save that value for return
	if (fseek((FILE*)theFile, 0, SEEK_END))
        ThrowXML(XMLPlatformUtilsException,
                 XMLExcepts::File_CouldNotSeekToEnd);

    long int retVal = ftell((FILE*)theFile);
    if (retVal == -1)
        ThrowXML(XMLPlatformUtilsException,
                 XMLExcepts::File_CouldNotSeekToEnd);

    // And put the pointer back
    if (fseek((FILE*)theFile, curPos, SEEK_SET))
        ThrowXML(XMLPlatformUtilsException,
                 XMLExcepts::File_CouldNotSeekToPos);

    return (unsigned int)retVal;
}

FileHandle XMLPlatformUtils::openFile(const XMLCh* const fileName)
{
    const char* tmpFileName = XMLString::transcode(fileName);
    ArrayJanitor<char> janText((char*)tmpFileName);
    FileHandle retVal = (FILE*)fopen(tmpFileName , "rb");

    if (retVal == NULL)
        return 0;

    return retVal;
}

FileHandle XMLPlatformUtils::openFile(const char* const fileName)
{
    FileHandle retVal = (FILE*)fopen(fileName, "rb");

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

unsigned int XMLPlatformUtils::readFileBuffer(FileHandle           theFile,
											  const unsigned int   toRead,
											  XMLByte* const       toFill)
{
    size_t noOfItemsRead = fread((void*)toFill, 1, toRead, (FILE*)theFile);

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
    //  NOTE: The path provided has always already been opened successfully,
    //  so we know that its not some pathological freaky path. It comes in
    //  in native format, and goes out as Unicode always
    //
    char* newSrc = XMLString::transcode(srcPath);
    ArrayJanitor<char> janText(newSrc);

    // Use a local buffer that is big enough for the largest legal path
	 // Without the *3 we get exceptions with gcc on OpenServer 5.0.5/6 when 
	 // relative paths are passed in
    char *absPath = new char[pathconf(newSrc, _PC_PATH_MAX)*3];
    ArrayJanitor<char> janText2(absPath);
    // Get the absolute path
    char* retPath = realpath(newSrc, absPath);

    if (!retPath)
    {
        ThrowXML(XMLPlatformUtilsException,
                 XMLExcepts::File_CouldNotGetBasePathName);
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

XMLCh* XMLPlatformUtils::weavePaths(const XMLCh* const   basePath,
                                    const XMLCh* const   relativePath)
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
		&& (*basePtr != chBackSlash))
    {
        while ((basePtr >= basePath)
			   && ((*basePtr != chForwardSlash) && (*basePtr != chBackSlash)))
            basePtr--;
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
        if ((*pathPtr != chForwardSlash)
			&& (*pathPtr != chBackSlash)
			&&  *pathPtr)
            break;

        if (*pathPtr)
            pathPtr++;

        // If its one period, just eat it, else move backwards in the base
        if (periodCount == 2)
        {
            basePtr--;
            while ((basePtr >= basePath) &&
				   ((*basePtr != chForwardSlash) && (*basePtr != chBackSlash)))
                basePtr--;

            // The base cannot provide enough levels, so its in error
            if (basePtr < basePath)
                ThrowXML(XMLPlatformUtilsException,
                         XMLExcepts::File_BasePathUnderflow);
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
	struct timeval   tp;
	if (gettimeofday(&tp, NULL) == -1)
		return 0;
    return (unsigned long)(tp.tv_sec * 1000 + tp.tv_usec / 1000);
}


// ---------------------------------------------------------------------------
//  XMLPlatformUtils: Methods for multi-threaded environment
// ---------------------------------------------------------------------------
#if !defined (APP_NO_THREADS)

static pthread_mutex_t* gAtomicOpMutex = 0;

// ---------------------------------------------------------------------------
//  XMLPlatformUtils: Platform init method
// ---------------------------------------------------------------------------
void XMLPlatformUtils::platformInit()
{
    //
    // The atomicOps mutex needs to be created early.
    // Normally, mutexes are created on first use, but there is a
    // circular dependency between compareAndExchange() and
    // mutex creation that must be broken.
	gAtomicOpMutex = new pthread_mutex_t;

	if (pthread_mutex_init(gAtomicOpMutex, NULL))
	{
		delete gAtomicOpMutex;
		gAtomicOpMutex = 0;
		panic(PanicHandler::Panic_SystemInit);
	}
}

// -----------------------------------------------------------------------
//  Mutex- and atomic operation methods
// -----------------------------------------------------------------------
class  RecursiveMutex
{
public:
	pthread_mutex_t   mutex;
	int               recursionCount;
	pthread_t         tid;

	RecursiveMutex() {
		if (pthread_mutex_init(&mutex, NULL))
			ThrowXML(XMLPlatformUtilsException,
					 XMLExcepts::Mutex_CouldNotCreate);
		recursionCount = 0;
		tid = 0;
	};

	~RecursiveMutex() {
		if (pthread_mutex_destroy(&mutex))
			ThrowXML(XMLPlatformUtilsException,
					 XMLExcepts::Mutex_CouldNotDestroy);
	};

	void lock() {
		if (pthread_equal(tid, pthread_self()))
		{
			recursionCount++;
			return;
		}
		if (pthread_mutex_lock(&mutex) != 0)
			ThrowXML(XMLPlatformUtilsException,
					 XMLExcepts::Mutex_CouldNotLock);
		tid = pthread_self();
		recursionCount = 1;
	};

	void unlock() {
		if (--recursionCount > 0)
			return;

		if (pthread_mutex_unlock(&mutex) != 0)
			ThrowXML(XMLPlatformUtilsException,
					 XMLExcepts::Mutex_CouldNotUnlock);
		tid = 0;
	};
};

void* XMLPlatformUtils::makeMutex()
{
	return new RecursiveMutex;
};

void XMLPlatformUtils::closeMutex(void* const mtxHandle)
{
	if (mtxHandle == NULL)
		return;

	RecursiveMutex *rm = (RecursiveMutex *)mtxHandle;
	delete rm;
};

void XMLPlatformUtils::lockMutex(void* const mtxHandle)
{
	if (mtxHandle == NULL)
		return;

	RecursiveMutex *rm = (RecursiveMutex *)mtxHandle;
	rm->lock();
}

void XMLPlatformUtils::unlockMutex(void* const mtxHandle)
{
	if (mtxHandle == NULL)
		return;

	RecursiveMutex *rm = (RecursiveMutex *)mtxHandle;
	rm->unlock();
}

// -----------------------------------------------------------------------
//  Miscellaneous synchronization methods
// -----------------------------------------------------------------------
// Atomic system calls in OpenServe is restricted to kernel libraries.
// To make operations thread safe, we implement static mutex and lock
// the atomic operations.

void* XMLPlatformUtils::compareAndSwap(void**              toFill,
									   const void* const   newValue,
									   const void* const   toCompare)
{
	if (pthread_mutex_lock(gAtomicOpMutex))
		panic(PanicHandler::Panic_SynchronizationErr);

	void *retVal = *toFill;
	if (*toFill == toCompare)
		*toFill = (void *)newValue;

	if (pthread_mutex_unlock(gAtomicOpMutex))
		panic(PanicHandler::Panic_SynchronizationErr);

	return retVal;
}

int XMLPlatformUtils::atomicIncrement(int &location)
{
	if (pthread_mutex_lock(gAtomicOpMutex))
		panic(PanicHandler::Panic_SynchronizationErr);

	int tmp = ++location;

	if (pthread_mutex_unlock(gAtomicOpMutex))
		panic(PanicHandler::Panic_SynchronizationErr);

	return tmp;
}

int XMLPlatformUtils::atomicDecrement(int &location)
{
	if (pthread_mutex_lock(gAtomicOpMutex))
		panic(PanicHandler::Panic_SynchronizationErr);

	int tmp = --location;

	if (pthread_mutex_unlock( gAtomicOpMutex))
		panic(PanicHandler::Panic_SynchronizationErr);

	return tmp;
}

#else // #if !defined (APP_NO_THREADS)


// ---------------------------------------------------------------------------
//  XMLPlatformUtils: Methods for single-threaded environment
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
//  XMLPlatformUtils: Platform init method
// ---------------------------------------------------------------------------
void XMLPlatformUtils::platformInit()
{
}

// -----------------------------------------------------------------------
//  Mutex- and atomic operation methods
// -----------------------------------------------------------------------
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

void* XMLPlatformUtils::compareAndSwap(void**              toFill,
									   const void* const   newValue,
									   const void* const   toCompare)
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


// ---------------------------------------------------------------------------
//  XMLPlatformUtils: Platform termination method
// ---------------------------------------------------------------------------
void XMLPlatformUtils::platformTerm()
{
#if !defined (APP_NO_THREADS)
	pthread_mutex_destroy(gAtomicOpMutex);
	delete gAtomicOpMutex;
	gAtomicOpMutex = 0;
#else
    // If not using threads, we don't have any termination requirements (yet)
#endif
}

XERCES_CPP_NAMESPACE_END

