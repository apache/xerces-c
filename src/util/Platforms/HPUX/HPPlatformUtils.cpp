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
 * Revision 1.5  2000/01/20 20:37:55  abagchi
 * Removed fgLibLocation and cleaned up ICU path related stuff
 *
 * Revision 1.4  2000/01/18 21:33:17  aruna1
 * Changed getBasePath to getFullPath,
 * added weavePath()
 *
 * Revision 1.3  1999/12/14 23:53:31  rahulj
 * Removed the offending Ctrl-M's from the commit message
 * logs which was giving packaging problems.
 *
 * PR:
 * Obtained from:
 * Submitted by:
 * Reviewed by:
 *
 * Revision 1.2  1999/11/23 02:00:44  rahulj
 * Code now works under HPUX 11. Tested inmemory message loader.
 * Revamped makefiles. Builds with both DCE threads as well as pthread libraries.
 *
 * Revision 1.1.1.1  1999/11/09 01:07:05  twl
 * Initial checkin
 *
 * Revision 1.3  1999/11/08 20:45:29  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------

#if !defined(APP_NO_THREADS)
#include    <pthread.h>
#endif // APP_NO_THREADS

#include    <util/PlatformUtils.hpp>
#include    <util/RuntimeException.hpp>
#include    <util/Janitor.hpp>
#include    <util/Mutexes.hpp>
#include    <dl.h>
#include    <stdio.h>
#include    <stdlib.h>
#include    <errno.h>
#include    <libgen.h>
#include    <string.h>
#include    <limits.h>
#include    <unistd.h>
#include    <util/XMLString.hpp>
#include    <util/XMLUni.hpp>


#if defined(XML_USE_ICU_TRANSCODER)
    #include <util/Transcoders/ICU/ICUTransService.hpp>
#else
    // Use native transcoder. Same as -DXML_USE_NATIVE_TRANSCODER
    #include <util/Transcoders/Iconv/IconvTransService.hpp>
#endif


#if defined(XML_USE_ICU_MESSAGELOADER)
    #include <util/MsgLoaders/ICU/ICUMsgLoader.hpp>

#else
    // Same as -DXML_USE_INMEM_MESSAGELOADER
    #include <util/MsgLoaders/InMemory/InMemMsgLoader.hpp>
#endif



#if !defined(XML_HPUX_KAICC)
#include    <sys/timeb.h>             // does not work with KAI compiler
#endif


// ---------------------------------------------------------------------------
//  Local Methods
// ---------------------------------------------------------------------------

static void WriteCharStr(FILE* stream, const char* const toWrite)
{
    if (fputs(toWrite, stream) == EOF)
    {
        ThrowXML(XMLPlatformUtilsException,
                 XML4CExcepts::Strm_StdErrWriteFailure);
    }
}

static void WriteUStrStdErr(const XMLCh* const toWrite)
{
    char* tmpVal = XMLString::transcode(toWrite);
    ArrayJanitor<char> janText(tmpVal);
    if (fputs(tmpVal, stderr) == EOF)
    {
        ThrowXML(XMLPlatformUtilsException,
                 XML4CExcepts::Strm_StdErrWriteFailure);
    }
}

static void WriteUStrStdOut(const XMLCh* const toWrite)
{
    char* tmpVal = XMLString::transcode(toWrite);
    ArrayJanitor<char> janText(tmpVal);
    if (fputs(tmpVal, stdout) == EOF)
    {
        ThrowXML(XMLPlatformUtilsException,
                 XML4CExcepts::Strm_StdOutWriteFailure);
    }
}


//
//  This method is called by the platform independent part of this class
//  during initialization. We have to create the type of net accessor that
//  we want to use.
//

XMLNetAccessor* XMLPlatformUtils::makeNetAccessor()
{
    return 0;
}


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
    atomicOpsMutex.fHandle = XMLPlatformUtils::makeMutex();

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
#else
        // same as -DXML_USE_INMEM_MESSAGELOADER
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
{

#if defined (XML_USE_ICU_TRANSCODER)
    return new ICUTransService;
#else

    // Use native transcoding services.
    // same as -DXML_USE_INMEM_MESSAGELOADER
    return new IconvTransService;

#endif

} // XMLPlatformUtils::makeTransService



// ---------------------------------------------------------------------------
//  XMLPlatformUtils: The panic method
// ---------------------------------------------------------------------------
void XMLPlatformUtils::panic(const PanicReasons reason)
{
    //
    //  We just print a message and exit
    //
    
    fprintf(stderr,
        "The Xerces-C system could not be initialized.\n");
    fprintf(stderr,
        "If you are using ICU, then the most likely reason for this failure\n");
    fprintf(stderr,
        "is the inability to find the ICU coverter files. The converter files\n");
    fprintf(stderr,
        "have the extension .cnv and exist in a directory 'icu/data' relative\n");
    fprintf(stderr,
        "to the Xerces-C shared library. If you have installed the converter files\n");
    fprintf(stderr,
        "in a different location, you need to set up the environment variable\n");
    fprintf(stderr,
        "'ICU_DATA' to point directly to the directory containing the\n");
    fprintf(stderr,
        "converter files.\n");
    
    exit(-1);
}


// ---------------------------------------------------------------------------
//  XMLPlatformUtils: File Methods
// ---------------------------------------------------------------------------

unsigned int XMLPlatformUtils::curFilePos(FileHandle theFile)
{
    // Get the current position
    long  curPos = ftell((FILE*) theFile);
    if (curPos == -1)
        ThrowXML(XMLPlatformUtilsException,
                 XML4CExcepts::File_CouldNotGetCurPos);

    return (unsigned int) curPos;
}


void XMLPlatformUtils::closeFile(FileHandle theFile)
{
    if (fclose((FILE*) theFile))
        ThrowXML(XMLPlatformUtilsException,
                 XML4CExcepts::File_CouldNotCloseFile);
}


unsigned int XMLPlatformUtils::fileSize(FileHandle theFile)
{
    // Get the current position
    long  int curPos = ftell((FILE*) theFile);
    if (curPos == -1)
    {
        ThrowXML(XMLPlatformUtilsException,
                 XML4CExcepts::File_CouldNotGetCurPos);
    }

    // Seek to the end and save that value for return
     if (fseek((FILE*) theFile, 0, SEEK_END))
     {
        ThrowXML(XMLPlatformUtilsException,
                 XML4CExcepts::File_CouldNotSeekToEnd);
     }

    long int retVal = ftell((FILE*) theFile);
    if (retVal == -1)
        ThrowXML(XMLPlatformUtilsException,
                 XML4CExcepts::File_CouldNotGetSize);

    // And put the pointer back
    if (fseek((FILE*) theFile, curPos, SEEK_SET))
        ThrowXML(XMLPlatformUtilsException,
                 XML4CExcepts::File_CouldNotSeekToPos);

    return (unsigned int) retVal;
}

FileHandle XMLPlatformUtils::openFile(const char* const fileName)
{
    FileHandle retVal = (FILE*) fopen(fileName , "rb");
    return retVal;
}

FileHandle XMLPlatformUtils::openFile(const XMLCh* const fileName)
{
    char* tmpFileName = XMLString::transcode(fileName);
    FileHandle retVal = (FILE*) fopen(tmpFileName , "rb");
    delete [] tmpFileName;
    return retVal;
}


FileHandle XMLPlatformUtils::openStdInHandle()
{
    return (FileHandle) fdopen(dup(0), "rb");
}


unsigned int
XMLPlatformUtils::readFileBuffer(       FileHandle      theFile
                                , const unsigned int    toRead
                                ,       XMLByte* const  toFill)
{
    size_t noOfItemsRead = fread((void*) toFill, 1, toRead, (FILE*)theFile);

    if (ferror((FILE*)theFile))
    {
        ThrowXML(XMLPlatformUtilsException,
                 XML4CExcepts::File_CouldNotReadFromFile);
    }

    return (unsigned int) noOfItemsRead;
}


void XMLPlatformUtils::resetFile(FileHandle theFile)
{
    // Seek to the start of the file
    if (fseek((FILE*) theFile, 0, SEEK_SET))
        ThrowXML(XMLPlatformUtilsException,
                 XML4CExcepts::File_CouldNotResetFile);
}



// ---------------------------------------------------------------------------
//  XMLPlatformUtils: Timing Methods
// ---------------------------------------------------------------------------
 
unsigned long XMLPlatformUtils::getCurrentMillis()
{
#if defined(XML_HPUX_KAICC)         // should be reimplemented by someone with
                                    // HP/UX and KAI knowledge.
    return (unsigned long) 0;
#else
    timeb aTime;
    ftime(&aTime);
    return (unsigned long)(aTime.time*1000 + aTime.millitm);
#endif
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
	char absPath[PATH_MAX];
	//get the absolute path
	char* retPath = realpath(newSrc, &absPath[0]);
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
    if (toCheck[0] == XMLCh('/'))
        return false;

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

            if (basePtr < basePath)
            {
                // The base cannot provide enough levels, so its in error
                // <TBD>
            }
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
// -----------------------------------------------------------------------

#if !defined(APP_NO_THREADS)

void* XMLPlatformUtils::makeMutex()
{
    pthread_mutex_t* mutex = new pthread_mutex_t;
    pthread_mutexattr_t*  attr = new pthread_mutexattr_t;

#if defined(XML_USE_DCE)
    pthread_mutexattr_create(attr);
    pthread_mutexattr_setkind_np(attr, MUTEX_RECURSIVE_NP);
    if (pthread_mutex_init(mutex, *attr))
    {
        ThrowXML(XMLPlatformUtilsException,
                 XML4CExcepts::Mutex_CouldNotCreate);
    }
    pthread_mutexattr_delete(attr);
#else
    pthread_mutexattr_init(attr);
    pthread_mutexattr_settype(attr, PTHREAD_MUTEX_RECURSIVE);
    if (pthread_mutex_init(mutex, attr))
    {
        ThrowXML(XMLPlatformUtilsException,
                 XML4CExcepts::Mutex_CouldNotCreate);
    }
    pthread_mutexattr_destroy(attr);
#endif
    delete attr;

    return (void*) mutex;
}

void XMLPlatformUtils::closeMutex(void* const mtxHandle)
{
    pthread_mutex_t* mutex = (pthread_mutex_t *) mtxHandle;
    if (mutex != NULL)
    {
        if (pthread_mutex_destroy(mutex))
            ThrowXML(XMLPlatformUtilsException,
                  XML4CExcepts::Mutex_CouldNotDestroy);
        delete mutex;
    }
}

void XMLPlatformUtils::lockMutex(void* const mtxHandle)
{
    if (mtxHandle != NULL)
    {
        if (pthread_mutex_lock((pthread_mutex_t *) mtxHandle))
        {
            ThrowXML(XMLPlatformUtilsException,
                     XML4CExcepts::Mutex_CouldNotLock);
        }
    }
}

void XMLPlatformUtils::unlockMutex(void* const mtxHandle)
{
    if (mtxHandle != NULL)
    {
        if (pthread_mutex_unlock((pthread_mutex_t *) mtxHandle))
        {
            ThrowXML(XMLPlatformUtilsException,
                     XML4CExcepts::Mutex_CouldNotUnlock);
        }
    }
}


// -----------------------------------------------------------------------
//  Miscellaneous synchronization methods
// -----------------------------------------------------------------------

void* XMLPlatformUtils::compareAndSwap ( void**      toFill, 
                                   const void* const newValue, 
                                   const void* const toCompare)
{
    XMLMutexLock  localLock(&atomicOpsMutex);
    void *retVal = *toFill;
    if (*toFill == toCompare)
    {
       *toFill = (void *)newValue;
    }
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

