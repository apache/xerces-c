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
 * Revision 1.13  2000/07/25 22:30:24  aruna1
 * Char definitions in XMLUni moved to XMLUniDefs
 *
 * Revision 1.12  2000/03/18 00:00:00  roddey
 * Initial updates for two way transcoding support
 *
 * Revision 1.11  2000/03/02 21:10:37  abagchi
 * Added empty function platformTerm()
 *
 * Revision 1.10  2000/03/02 19:55:29  roddey
 * This checkin includes many changes done while waiting for the
 * 1.1.0 code to be finished. I can't list them all here, but a list is
 * available elsewhere.
 *
 * Revision 1.9  2000/02/23 00:35:49  abagchi
 * Added OS/390 changes to comply with new ICUDATA library
 *
 * Revision 1.8  2000/02/08 18:55:59  abagchi
 * Changed it to use Iconv390 transcoder
 *
 * Revision 1.7  2000/02/08 18:37:28  abagchi
 * set __MUTEX_RECURSIVE
 *
 * Revision 1.4  2000/02/06 07:48:29  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.3  2000/01/21 22:17:57  abagchi
 * OS390 Change: updates for Version 3.1.0
 *
 * Revision 1.2  1999/12/18 00:47:01  rahulj
 * Merged in some changes for OS390.
 *
 * Revision 1.1.1.1  1999/11/09 01:06:38  twl
 * Initial checkin
 *
 * Revision 1.2  1999/11/08 20:45:31  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#ifdef OS390
#define _OPEN_SYS
#endif

#ifndef APP_NO_THREADS
#include    <pthread.h>
#endif

#include    <util/PlatformUtils.hpp>
#include    <util/RuntimeException.hpp>
#include    <util/Janitor.hpp>
#include    <stdio.h>
#include    <stdlib.h>
#include    <errno.h>
#include    <libgen.h>
#ifdef EXM_OS390
#include    <timeb.h>
#else
#include    <sys/timeb.h>
#endif
#include    <string.h>
#include    <unistd.h>
#include    <limits.h>	
#include    <util/XMLString.hpp>
#include    <util/XMLUniDefs.hpp>
#include    <util/XMLUni.hpp>

#if defined (XML_USE_ICU_TRANSCODER)
    #include <util/Transcoders/ICU/ICUTransService.hpp>
#else   // use native transcoder
    #include <util/Transcoders/Iconv390/Iconv390TransService.hpp>
#endif

#if defined (XML_USE_ICU_MESSAGELOADER)
    #include <util/MsgLoaders/ICU/ICUMsgLoader.hpp>
#elif defined (XML_USE_ICONV_MESSAGELOADER)
    #include <util/MsgLoaders/MsgCatalog/MsgCatalogLoader.hpp>
#else   // use In-memory message loader
    #include <util/MsgLoaders/InMemory/InMemMsgLoader.hpp>   //hint for the user to include this file.
#endif


// ---------------------------------------------------------------------------
//  Local Methods
// ---------------------------------------------------------------------------
static void WriteCharStr( FILE* stream, const char* const toWrite)
{
    if (!fputs(toWrite, stream))
    {
	ThrowXML(XMLPlatformUtilsException, XML4CExcepts::Strm_StdErrWriteFailure);
    }
}

static void WriteUStrStdErr( const XMLCh* const toWrite)
{
    char* tmpVal = XMLString::transcode(toWrite);
        ArrayJanitor<char> janText(tmpVal);
    if (!fputs(tmpVal, stderr))
    {
	ThrowXML(XMLPlatformUtilsException, XML4CExcepts::Strm_StdErrWriteFailure);
    }
}

static void WriteUStrStdOut( const XMLCh* const toWrite)
{
    char* tmpVal = XMLString::transcode(toWrite);
        ArrayJanitor<char> janText(tmpVal);

    if (!fputs(tmpVal, stdout))
    {
	ThrowXML(XMLPlatformUtilsException, XML4CExcepts::Strm_StdErrWriteFailure);
    }
}


XMLNetAccessor* XMLPlatformUtils::makeNetAccessor()
{
    return 0;
}

// ---------------------------------------------------------------------------
//  XMLPlatformUtils: Platform init method
// ---------------------------------------------------------------------------
void XMLPlatformUtils::platformInit()
{
}

// ---------------------------------------------------------------------------
//  XMLPlatformUtils: Private Static Methods
// ---------------------------------------------------------------------------

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

    catch(...)
    {
	panic(XMLPlatformUtils::Panic_CantLoadMsgDomain);
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
#else
{
    return new Iconv390TransService;
}
#endif


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
        reasonStr = "Cannot synchronize system or mutex";
    else if (reason == Panic_SystemInit)
        reasonStr = "Cannot initialize the system or mutex";

    fprintf(stderr, "%s\n", reasonStr);

    exit(-1);
}


// ---------------------------------------------------------------------------
//  XMLPlatformUtils: File Methods
// ---------------------------------------------------------------------------
unsigned int XMLPlatformUtils::curFilePos(FileHandle theFile)
{
    // Get the current position
    int curPos = ftell( (FILE*)theFile);
    if (curPos == -1)
	ThrowXML(XMLPlatformUtilsException, XML4CExcepts::File_CouldNotGetSize);

    return (unsigned int)curPos;
}

void XMLPlatformUtils::closeFile(FileHandle theFile)
{
    if (fclose((FILE*)theFile))
ThrowXML(XMLPlatformUtilsException, XML4CExcepts::File_CouldNotCloseFile);
}

unsigned int XMLPlatformUtils::fileSize(FileHandle theFile)
{
    // Get the current position
    long  int curPos = ftell((FILE*)theFile);
    if (curPos == -1)
	ThrowXML(XMLPlatformUtilsException, XML4CExcepts::File_CouldNotGetCurPos);

    // Seek to the end and save that value for return
     if (fseek( (FILE*)theFile, 0, SEEK_END) )
	ThrowXML(XMLPlatformUtilsException, XML4CExcepts::File_CouldNotSeekToEnd);

    long int retVal = ftell( (FILE*)theFile);
    if (retVal == -1)
	ThrowXML(XMLPlatformUtilsException, XML4CExcepts::File_CouldNotSeekToEnd);


    // And put the pointer back
    if (fseek( (FILE*)theFile, curPos, SEEK_SET) )
	ThrowXML(XMLPlatformUtilsException, XML4CExcepts::File_CouldNotSeekToPos);

    return (unsigned int)retVal;
}

FileHandle XMLPlatformUtils::openFile(const XMLCh* const fileName)
{
    char* tmpFileName = XMLString::transcode(fileName);
    ArrayJanitor<char> janText((char*)tmpFileName);
    FileHandle retVal = (FILE*)fopen( tmpFileName , "rb" );

#ifdef OS390BATCH
    if (retVal == NULL) {
    //
    // We failed to open the file using its native format (OE or MVS)
    // Try to go an extra step to map the path into a MVS dataset under BATCH:
    //
    //     /path/path2/filename.ext => //'path.path2.ext(filename)'
    //     /path/path2/filename     => //'path.path2.filename'
    // and
    //     path/path2/filename.ext  => //path.path2.ext(filename)
    //     path/path2/filename      => //path.path2.filename

    char* datasetName = new char[ strlen(tmpFileName) + 5 ];
    char *datasetPos = datasetName, *tmpPos = tmpFileName;
    FileHandle retVal;

    // We are in EBCDIC mode here
    // Specify "//" to indicate that the filename refers to a non-POSIX file
    // or a MVS data set

    strcpy(datasetName, "//");
    *datasetPos += 2;

    // If we have a leading '/' then the path is absolute

    if( *tmpPos == '/' ) {
        *datasetPos++ = '\'';
        tmpPos++;
    }

    // Find the last '/' in the path - this seperates the path from the
    // filename.  Then copy the pathname.

    char* pathEnd = strrchr( tmpPos, '/' );
    if( pathEnd == NULL ) pathEnd = tmpPos - 1;
    while( tmpPos <= pathEnd ) {
        switch( *tmpPos ) {
                case '/':
                        *datasetPos = '.';
                        break;
                default:
                        *datasetPos = *tmpPos;
        }
        datasetPos++; tmpPos++;
    }

    // Now we try to locate the extension, and copy that.

    char* extStart = strrchr( tmpFileName, '.' );
    if ( extStart != NULL ) {
        tmpPos = extStart + 1;
        while( *tmpPos != '\0' ) {
                *datasetPos++ = *tmpPos++;
        }
        *datasetPos++ = '(';
    }

    // Now we copy in the filename.

    tmpPos = pathEnd + 1;
    while( *tmpPos != '\0' && ((extStart == NULL) || (tmpPos < extStart)) ) {
        *datasetPos++ = *tmpPos++;
    }

    // Finally cap off the filename with optional ")" and "'", plus a null
    if( extStart != NULL ) *datasetPos++ = ')';
    if( *tmpFileName == '/' ) *datasetPos++ = '\'';
    *datasetPos = '\0';

    retVal = (FILE*)fopen( datasetName , "rb" );

    delete [] datasetName;
    }
#endif
    delete [] tmpFileName;
    if( retVal == NULL )
	return 0;
    return retVal;
}

unsigned int
XMLPlatformUtils::readFileBuffer(  FileHandle      theFile
                                , const unsigned int    toRead
                                , XMLByte* const  toFill)
{
    size_t noOfItemsRead = fread( (void*) toFill, 1, toRead, (FILE*)theFile);

    if(ferror((FILE*)theFile))
    {
	ThrowXML(XMLPlatformUtilsException, XML4CExcepts::File_CouldNotReadFromFile);
    }

    return (unsigned int)noOfItemsRead;
}


void XMLPlatformUtils::resetFile(FileHandle theFile)
{
    // Seek to the start of the file
    if (fseek((FILE*)theFile, 0, SEEK_SET) )
	ThrowXML(XMLPlatformUtilsException, XML4CExcepts::File_CouldNotResetFile);
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
    char *absPath = new char[_POSIX_PATH_MAX];
        //get the absolute path
        char* retPath = realpath(newSrc, absPath);
    ArrayJanitor<char> janText2(retPath);

    if (!retPath)
    {
                ThrowXML(XMLPlatformUtilsException, XML4CExcepts::File_CouldNotGetBasePathName);
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
    //  Check for 0x2F instead of '/' because '/' will be compiled into
    //  0x61
    //
    if (toCheck[0] == 0x2F)
        return false;

#ifdef OS390BATCH
    //
    //  Determine whether we are using the DD convention
    //  'D': 0x44 'd': 0x64 ':': 0x3A
    //
    if (XMLString::stringLen(toCheck) > 3) {
      if (
          ((toCheck[0] == (0x44)) || (toCheck[0] == (0x64))) &&
          ((toCheck[1] == (0x44)) || (toCheck[1] == (0x64))) &&
          (toCheck[2] == XMLCh(0x3A))
	 )
	return false;
    }
#endif

    // Else assume its a relative path
    return true;
}

XMLCh* XMLPlatformUtils::weavePaths
    (
        const   XMLCh* const    basePath
        , const XMLCh* const    relativePath
    )
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
                ThrowXML(PlatformUtilsException, File_BasePathUnderflow);
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

// -----------------------------------------------------------------------
//  Mutex methods
//  OS390BATCH: Unless POSIX(ON) is specified, pthread functions are
//  not available. We will use compareAndSwap for now untill a block
//  locking mechanism is developed
// -----------------------------------------------------------------------
#ifndef APP_NO_THREADS
void XMLPlatformUtils::closeMutex(void* const mtxHandle)
{
    if (mtxHandle == NULL)
        return;
#ifdef OS390BATCH
    if (__isPosixOn()) {
#endif
    if (pthread_mutex_destroy( (pthread_mutex_t*)mtxHandle))
    {
	ThrowXML(XMLPlatformUtilsException, XML4CExcepts::Mutex_CouldNotDestroy);
    }
    if ( (pthread_mutex_t*)mtxHandle)
        delete mtxHandle;
#ifdef OS390BATCH
    } // __isPosixOn
    else {
    if ( (int*)mtxHandle)
        delete mtxHandle;
    }
#endif
}
void XMLPlatformUtils::lockMutex(void* const mtxHandle)
{
    if (mtxHandle == NULL)
        return;
#ifdef OS390BATCH
    if (__isPosixOn()) {
#endif
    if (pthread_mutex_lock( (pthread_mutex_t*)mtxHandle))
    {
	ThrowXML(XMLPlatformUtilsException, XML4CExcepts::Mutex_CouldNotLock);
    }
#ifdef OS390BATCH
    } // __isPosixOn
    else {
	int locked = 1, unlocked;

    do {
        unlocked = 0;
        compareAndSwap( (void**) &mtxHandle, (void*) &locked, (void*) &unlocked );
    } while( unlocked != 0 );
    }
#endif
    return;

}
void* XMLPlatformUtils::makeMutex()
{
#ifdef OS390BATCH
    if (__isPosixOn()) {
#endif
    pthread_mutex_t* mutex = new pthread_mutex_t;
    if (mutex == NULL)
    {
	ThrowXML(XMLPlatformUtilsException, XML4CExcepts::Mutex_CouldNotCreate);
    }

    pthread_mutexattr_t*  attr = new pthread_mutexattr_t;
    pthread_mutexattr_init(attr);
    pthread_mutexattr_setkind_np(attr, __MUTEX_RECURSIVE);
    if (pthread_mutex_init(mutex, attr))
    {
        ThrowXML(XMLPlatformUtilsException,
                XML4CExcepts::Mutex_CouldNotCreate);
    }
    pthread_mutexattr_destroy(attr);
    delete attr;

    return (void*)(mutex);

#ifdef OS390BATCH
    } // __isPosixOn
    else {
	int* mutex = new int;
	*mutex = 0;
	return (void*)(mutex);
    }
#endif
}
void XMLPlatformUtils::unlockMutex(void* const mtxHandle)
{
    if (mtxHandle == NULL)
        return;
#ifdef OS390BATCH
    if (__isPosixOn()) {
#endif
    if (pthread_mutex_unlock( (pthread_mutex_t*)mtxHandle))
    {
	ThrowXML(XMLPlatformUtilsException, XML4CExcepts::Mutex_CouldNotUnlock);
    }
#ifdef OS390BATCH
    } // __isPosixOn
    else {
	if (*(int*) mtxHandle == 0 )
	*(int*) mtxHandle = 0;
    }
#endif
}

#else // #ifndef APP_NO_THREADS

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

#endif // APP_NO_THREADS

// -----------------------------------------------------------------------
//  Miscellaneous synchronization methods
// -----------------------------------------------------------------------
void* XMLPlatformUtils::compareAndSwap ( void**      toFill ,
                    const void* const newValue ,
                    const void* const toCompare)
{
  int boolVar = cs( (cs_t*) &toCompare, (cs_t*) toFill, (cs_t) newValue );
  return (void*) toCompare;
}

int XMLPlatformUtils::atomicIncrement(int &location)
{
  int newVal, oldVal = location;

  do {
    newVal = oldVal + 1;
    oldVal = (int) compareAndSwap( (void**) &location, (void*) newVal, (void*) oldVal );
  } while( newVal != oldVal + 1 );

  return newVal;
}
int XMLPlatformUtils::atomicDecrement(int &location)
{
  int newVal, oldVal = location;

  do {
    newVal = oldVal - 1;
    oldVal = (int) compareAndSwap( (void**) &location, (void*) newVal, (void*) oldVal );
  } while( newVal != oldVal - 1 );

  return newVal;
}


FileHandle XMLPlatformUtils::openStdInHandle()
{
    return (FileHandle)fdopen(dup(0), "rb");
}

void XMLPlatformUtils::platformTerm()
{
    // We don't have any termination requirements at this time
}

