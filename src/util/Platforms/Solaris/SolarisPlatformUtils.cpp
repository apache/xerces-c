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
 * Revision 1.5  2000/01/14 02:51:34  aruna1
 * Modified for FullPath, weavePath, openFile
 *
 * Revision 1.4  1999/12/08 23:10:07  aruna1
 * Recursive locking mechanism added
 *
 * Revision 1.3  1999/12/02 23:07:13  aruna1
 * Solaris Atomic Mutex initailization changed to native calls
 *
 * Revision 1.2  1999/11/19 23:50:56  aruna1
 * added changes for platformInit and makeTransService functions
 * PR:
 * Obtained from:
 * Submitted by:
 * Reviewed by:
 *
 * Revision 1.1.1.1  1999/11/09 01:06:31  twl
 * Initial checkin
 *
 * Revision 1.4  1999/11/08 20:45:32  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------

#if !defined (APP_NO_THREADS)
#include    <pthread.h>
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
#include    <util/Janitor.hpp>
#include    <util/PlatformUtils.hpp>
#include    <util/RuntimeException.hpp>
#include    <util/Mutexes.hpp>
#include    <util/XMLString.hpp>
#include    <util/XMLUni.hpp>

#if defined (XML_USE_ICU_TRANSCODER)
    #include <util/Transcoders/ICU/ICUTransService.hpp>
#else   // use native transcoder
    #include <util/Transcoders/Iconv/IconvTransService.hpp>
#endif

#if defined (XML_USE_ICU_MESSAGELOADER)
    #include <util/MsgLoaders/ICU/ICUMsgLoader.hpp>
#elif defined (XML_USE_ICONV_MESSAGELOADER)
    #include <util/MsgLoaders/MsgCatalog/MsgCatalogLoader.hpp>
#else   // use In-memory message loader
    #include <util/MsgLoaders/InMemory/InMemMsgLoader.hpp>
#endif


// ---------------------------------------------------------------------------
//  Local Methods
// ---------------------------------------------------------------------------
static void WriteCharStr( FILE* stream, const char* const toWrite)
{
    if (fputs(toWrite, stream) == EOF)
    {
        ThrowXML(XMLPlatformUtilsException,
                 XML4CExcepts::Strm_StdErrWriteFailure);
    }
}

static void WriteUStrStdErr( const XMLCh* const toWrite)
{
    char* tmpVal = XMLString::transcode(toWrite);
    ArrayJanitor<char> janText(tmpVal);
    if (fputs(tmpVal, stderr) == EOF)
    {
        ThrowXML(XMLPlatformUtilsException,
                 XML4CExcepts::Strm_StdErrWriteFailure);
    }
}

static void WriteUStrStdOut( const XMLCh* const toWrite)
{
    char* tmpVal = XMLString::transcode(toWrite);
    ArrayJanitor<char> janText(tmpVal);
    if (fputs(tmpVal, stdout) == EOF)
    {
        ThrowXML(XMLPlatformUtilsException,
                 XML4CExcepts::Strm_StdOutWriteFailure);
    }
}

XMLNetAccessor* XMLPlatformUtils::makeNetAccessor()
{
    return 0;
}

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

    if (pthread_mutex_init(gAtomicOpMutex, NULL))
    {
	printf("atomicOpMutex not created \n");
	exit(1);
        //panic( XMLPlatformUtils::Panic_MutexInit ); //to be changed later
    }

    // Here you would also set the fgLibLocation global variable
    // XMLPlatformUtils::fgLibLocation is the variable to be set

    static const char *sharedLibEnvVar = "LD_LIBRARY_PATH";
    static const char * libraryPath = 0;

    char libName[256];

    // Construct the library name from the global variables
    strcpy(libName, XML4C_DLLName);
    strcat(libName, gXML4CVersionStr);
    strcat(libName, ".so");

    char* libEnvVar = getenv(sharedLibEnvVar);
    char* libPath = NULL;

    if (libEnvVar == NULL)
    {
        panic( XMLPlatformUtils::Panic_NoTransService );
    }

    //
    // Its necessary to create a copy because strtok() modifies the
    // string as it returns tokens. We don't want to modify the string
    // returned to by getenv().

    libPath = new char[strlen(libEnvVar) + 1];
    strcpy(libPath, libEnvVar);

    // First do the searching process for the first directory listing
    char*  allPaths = libPath;
    char*  libPathName;

    while ((libPathName = strtok(allPaths, ":")) != NULL)
    {
        FILE*  dummyFptr = 0;
        allPaths = 0;

        char* libfile = new char[strlen(libPathName) + strlen(libName) + 2];
        strcpy(libfile, libPathName);
        strcat(libfile, "/");
        strcat(libfile, libName);

        dummyFptr = (FILE *) fopen(libfile, "rb");
        delete [] libfile;
        if (dummyFptr != NULL)
        {
            fclose(dummyFptr);
            libraryPath = new char[strlen(libPathName)+1];
            strcpy((char *) libraryPath, libPathName);
            break;
        }

    }

    delete libPath;

    XMLPlatformUtils::fgLibLocation = libraryPath;

    if (XMLPlatformUtils::fgLibLocation == NULL)
    {
        panic( XMLPlatformUtils::Panic_NoTransService );
     }

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
        panic(XMLPlatformUtils::Panic_NoDefTranscoder);
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
    //
    //  We need to figure out the path to the Intl converter files.
    //

    static const char * xml4cIntlDirEnvVar = "ICU_DATA";
    static const char * sharedLibEnvVar    = "LD_LIBRARY_PATH";
    static const char * intlPath = 0;

    char* envVal = getenv(xml4cIntlDirEnvVar);
    //check if environment variable is set
    if (envVal != NULL) // We have found an environment variable
    {
        // Store this string in the static member
        unsigned int pathLen = strlen(envVal);
        intlPath = new char[pathLen + 2];

        strcpy((char *) intlPath, envVal);
        if (envVal[pathLen - 1] != '/')
        {
            strcat((char *) intlPath, "/");
        }

        ICUTransService::setICUPath(intlPath);
        if (intlPath != NULL) delete (char*)intlPath;

        return new ICUTransService;
    }


    //  If we did not find the environment var, so lets try to go the auto
    //  search route.
    //
    char libName[256];
    strcpy(libName, XML4C_DLLName);
    strcat(libName, gXML4CVersionStr);
    strcat(libName, ".so");

    char* libEnvVar = getenv(sharedLibEnvVar);
    char* libPath = NULL;

    if (libEnvVar == NULL)
    {
        panic( XMLPlatformUtils::Panic_NoTransService );
    }

    //
    // Its necessary to create a copy because strtok() modifies the
    // string as it returns tokens. We don't want to modify the string
    // returned to by getenv().
    //

    libPath = new char[strlen(libEnvVar) + 1];
    strcpy(libPath, libEnvVar);

    // First do the searching process for the first directory listing
    char*  allPaths = libPath;
    char*  libPathName;

    while ((libPathName = strtok(allPaths, ":")) != NULL)
    {
        FILE*  dummyFptr = 0;
        allPaths = 0;

        char* libfile = new char[strlen(libPathName) + strlen(libName) + 2];
        strcpy(libfile, libPathName);
        strcat(libfile, "/");
        strcat(libfile, libName);

        dummyFptr = (FILE *) fopen(libfile, "rb");
        delete [] libfile;
        if (dummyFptr != NULL)
        {
            fclose(dummyFptr);
            intlPath = new char[strlen(libPathName)+ strlen("/icu/data/")+1];
            strcpy((char *) intlPath, libPathName);
            strcat((char *) intlPath, "/icu/data/");
            break;
        }

    } // while

    delete libPath;

    ICUTransService::setICUPath(intlPath);

    if (intlPath == NULL)
    {
        panic( XMLPlatformUtils::Panic_NoTransService );
    }
    if (intlPath != NULL) delete (char*)intlPath;

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
void XMLPlatformUtils::panic(const PanicReasons reason)
{
    //
    //  We just print a message and exit
    //
    
    fprintf(stderr,
            "The XML4C system could not be initialized.\n");
    fprintf(stderr,
            "The most likely reason for this failure is the inability to find\n");
    fprintf(stderr,
            "the ICU coverter files, if you are using ICU. The converter files\n");
    fprintf(stderr,
            "have the extension .cnv and exist in a directory 'icu/data' relative\n");
    fprintf(stderr,
            "to the XML4C shared library. If you have installed the converter files\n");
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
    int curPos = ftell( (FILE*)theFile);
    if (curPos == -1)
        ThrowXML(XMLPlatformUtilsException,
                 XML4CExcepts::File_CouldNotGetSize);

    return (unsigned int)curPos;
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
        ThrowXML(XMLPlatformUtilsException,
                 XML4CExcepts::File_CouldNotGetCurPos);

    // Seek to the end and save that value for return
    if (fseek( (FILE*) theFile, 0, SEEK_END) )
        ThrowXML(XMLPlatformUtilsException,
                 XML4CExcepts::File_CouldNotSeekToEnd);

    long int retVal = ftell((FILE*) theFile);
    if (retVal == -1)
        ThrowXML(XMLPlatformUtilsException,
                 XML4CExcepts::File_CouldNotSeekToEnd);

    // And put the pointer back
    if (fseek((FILE*) theFile, curPos, SEEK_SET))
        ThrowXML(XMLPlatformUtilsException,
                 XML4CExcepts::File_CouldNotSeekToPos);

    return (unsigned int)retVal;
}

FileHandle XMLPlatformUtils::openFile(const char* const fileName) 
{
    FileHandle retVal = (FILE*)fopen( fileName , "rb" );

    if (retVal == NULL)
        return 0;
    return retVal;
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

unsigned int
XMLPlatformUtils::readFileBuffer(FileHandle              theFile
                               , const unsigned int      toRead
                               , XMLByte* const          toFill)
{
    size_t noOfItemsRead =
               fread((void*) toFill, 1, toRead, (FILE*) theFile);

    if(ferror((FILE*) theFile))
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

#if defined (SOLARIS)
extern "C" int ftime(struct timeb *); // Solaris headers missing this decl
#endif
 
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
    char *absPath = new char[PATH_MAX];
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
//  Standard out/error support
// -----------------------------------------------------------------------

void XMLPlatformUtils::writeToStdErr(const char* const toWrite)
{
    WriteCharStr(stderr, toWrite);
}
void XMLPlatformUtils::writeToStdErr(const XMLCh* const toWrite)
{
    WriteUStrStdErr(toWrite);
}
void XMLPlatformUtils::writeToStdOut(const XMLCh* const toWrite)
{
    WriteUStrStdOut(toWrite);
}
void XMLPlatformUtils::writeToStdOut(const char* const toWrite)
{
    WriteCharStr(stdout, toWrite);
}


// -----------------------------------------------------------------------
//  Mutex methods 
// -----------------------------------------------------------------------

#if !defined (APP_NO_THREADS)

class  RecursiveMutex
{
public:
    pthread_mutex_t   mutex;
    int               recursionCount;
    pthread_t         tid;

    RecursiveMutex() { 
		       if (pthread_mutex_init(&mutex, NULL))
			    ThrowXML(XMLPlatformUtilsException, XML4CExcepts::Mutex_CouldNotCreate);
                       recursionCount = 0;
                       tid = 0;
                     };

    ~RecursiveMutex() {
			if (pthread_mutex_destroy(&mutex))
			    ThrowXML(XMLPlatformUtilsException, XML4CExcepts::Mutex_CouldNotDestroy);
                      };

     void lock()      {
			  if (pthread_equal(tid, pthread_self()))
			  {
			      recursionCount++;
			      return;
			  }
			  if (pthread_mutex_lock(&mutex) != 0)
			      ThrowXML(XMLPlatformUtilsException, XML4CExcepts::Mutex_CouldNotLock);
			  tid = pthread_self();
			  recursionCount = 1;
		      };


     void unlock()    {
                          if (--recursionCount > 0)
                              return;

			  if (pthread_mutex_unlock(&mutex) != 0)
			      ThrowXML(XMLPlatformUtilsException, XML4CExcepts::Mutex_CouldNotUnlock);
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
    {
	printf("could not lock atomicOpMutex\n");
	exit(1);
	//call panic instead
    }

    void *retVal = *toFill;
    if (*toFill == toCompare)
              *toFill = (void *)newValue;

    if (pthread_mutex_unlock( gAtomicOpMutex))
    {
	printf("could not unlock atomicOpMutex\n");
	exit(1);
	//call panic instead
    }

    return retVal;
}

int XMLPlatformUtils::atomicIncrement(int &location)
{
    //return (int)atomic_add_32_nv( (uint32_t*)&location, 1);

    if (pthread_mutex_lock( gAtomicOpMutex))
    {
	printf("could not lock atomicOpMutex\n");
	exit(1);
	//call panic instead
    }
    int tmp = ++location;
    if (pthread_mutex_unlock( gAtomicOpMutex))
    {
	printf("could not unlock atomicOpMutex\n");
	exit(1);
	//call panic instead
    }
    return tmp;
}
int XMLPlatformUtils::atomicDecrement(int &location)
{
    //return (int)atomic_add_32_nv( (uint32_t*)&location, -1);

    if (pthread_mutex_lock( gAtomicOpMutex))
    {
	printf("could not lock atomicOpMutex\n");
	exit(1);
	//call panic instead
    }
	
    int tmp = --location;
    if (pthread_mutex_unlock( gAtomicOpMutex))
    {
	printf("could not unlock atomicOpMutex\n");
	exit(1);
	//call panic instead
    }
    return tmp;
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



FileHandle XMLPlatformUtils::openStdInHandle()
{
        return (FileHandle)fdopen(dup(0), "rb");
}

