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
 * Revision 1.4  1999/12/03 02:35:22  aruna1
 * mutex recursive attribute introduced in makeMutex
 *
 * Revision 1.3  1999/12/01 02:55:42  aruna1
 * Added panic calls and modified makeTransService for  AIX
 *
 * Revision 1.2  1999/11/23 02:00:34  rahulj
 * Code now works under HPUX 11. Tested inmemory message loader.Revamped makefiles. Builds with both DCE threads as well as pthread libraries.
 *
 * Revision 1.1.1.1  1999/11/09 01:07:11  twl
 * Initial checkin
 *
 * Revision 1.6  1999/11/08 20:45:29  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#ifndef APP_NO_THREADS
#include    <pthread.h>
#include    <sys/atomic_op.h>
#endif

#include    <stdio.h>
#include    <stdlib.h>
#include    <errno.h>
#include    <libgen.h>
#include    <sys/timeb.h>
#include    <string.h>
#include    <unistd.h>
#include 	<sys/ldr.h>

#include    <util/PlatformUtils.hpp>
#include    <util/RuntimeException.hpp>
#include    <util/Janitor.hpp>
#include    <util/XMLString.hpp>

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
    #include <util/MsgLoaders/InMemory/InMemMsgLoader.hpp>   //hint for the user to include this file.
#endif

// ---------------------------------------------------------------------------
//  Local Methods
// ---------------------------------------------------------------------------
static void WriteCharStr( FILE* stream, const char* const toWrite)
{
    if (fputs(toWrite, stream) == EOF)
    {
		ThrowXML(XMLPlatformUtilsException, XML4CExcepts::Strm_StdErrWriteFailure);
    }
}

static void WriteUStrStdErr( const XMLCh* const toWrite)
{
    char* tmpVal = XMLString::transcode(toWrite);
    ArrayJanitor<char> janText(tmpVal);
    if (fputs(tmpVal, stderr) == EOF)
    {
		ThrowXML(XMLPlatformUtilsException, XML4CExcepts::Strm_StdErrWriteFailure);
    }
}

static void WriteUStrStdOut( const XMLCh* const toWrite)
{
    char* tmpVal = XMLString::transcode(toWrite);
    ArrayJanitor<char> janText(tmpVal);
    if (fputs(tmpVal, stdout) == EOF)
    {
		ThrowXML(XMLPlatformUtilsException, XML4CExcepts::Strm_StdOutWriteFailure);
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
    // Here you would also set the fgLibLocation global variable
    // XMLPlatformUtils::fgLibLocation is the variable to be set

    char * libraryPath = 0;

    char libName[256];
    strcpy(libName, XML4C_DLLName);
    strcat(libName, gXML4CVersionStr);
    strcat(libName, ".a");

    const int bufLen = 4096;   // arbitrary value big enough to hold path name
    char buf[bufLen];		   // it's in the stack anyway

    if (load(libName, L_LIBPATH_EXEC, ".") == NULL) {
		panic( XMLPlatformUtils::Panic_CantFindLib );
    }

    int retval = loadquery(L_GETINFO, buf, bufLen);
    if (retval == -1) {
		panic( XMLPlatformUtils::Panic_CantFindLib );
    }

    struct ld_info* oneBufPtr = (struct ld_info*)buf;

    while (oneBufPtr)
    {
        if (oneBufPtr != (struct ld_info*) buf)
        {
            char* fileName = oneBufPtr->ldinfo_filename;
			if (strstr(fileName, libName) != NULL) {
				char* copyTo = strrchr(fileName, '/');
				size_t chars_to_extract = copyTo - fileName;
				char *libPathName = new char[chars_to_extract + 1];
				strncpy(libPathName, fileName, chars_to_extract);
				libPathName[chars_to_extract] = 0;
				libraryPath = new char[strlen(libPathName)+ strlen("/icu/data/")+1];
				strcpy((char *) libraryPath, libPathName);
				strcat((char *) libraryPath, "/icu/data/");
				delete libPathName;
				break;
			}
        }
        if (oneBufPtr->ldinfo_next == 0)
            oneBufPtr = NULL;
        else
            oneBufPtr = (struct ld_info*) ((char*) oneBufPtr + oneBufPtr->ldinfo_next);
    }

    XMLPlatformUtils::fgLibLocation = libraryPath;

    if (XMLPlatformUtils::fgLibLocation == NULL)
    {
        panic( XMLPlatformUtils::Panic_CantFindLib );
    }
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
    //  We need to figure out the path to the Intl classes. They will be
    //  in the .\Intl subdirectory under this DLL.
    //

    static const char * xml4cIntlDirEnvVar	= "ICU_DATA";
    char * 				intlPath 			= 0;

    char* envVal = getenv(xml4cIntlDirEnvVar);
    //check if environment variable is set
    if (envVal != NULL)
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
        delete intlPath;

        return new ICUTransService;
    }

 	//
    //  If the environment variable ICU_DATA is not set, assume that the
    //  converter files are stored relative to the Xerces-C library.
    //

    unsigned int  lent = strlen(XMLPlatformUtils::fgLibLocation) +
                         strlen("/icu/data/") + 1;
    intlPath = new char[lent];
    strcpy(intlPath, XMLPlatformUtils::fgLibLocation);
    strcat(intlPath, "/icu/data/");

	ICUTransService::setICUPath(intlPath);
    delete intlPath;

    return new ICUTransService;
}
#else
{
    return new IconvTransService;
}
#endif

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

FileHandle XMLPlatformUtils::openFile(const unsigned short* const fileName)
{
    const char* tmpFileName = XMLString::transcode(fileName);
    ArrayJanitor<char> janText((char*)tmpFileName);
    FileHandle retVal = (FILE*)fopen( tmpFileName , "rb" );

    if (retVal == NULL)
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



XMLCh* XMLPlatformUtils::getBasePath(const XMLCh* const srcPath)
{

    //
    //  NOTE: THe path provided has always already been opened successfully,
    //  so we know that its not some pathological freaky path. It comes in
    //  in native format, and goes out as Unicode always
    //
    char* newSrc = XMLString::transcode(srcPath);
    ArrayJanitor<char> janText(newSrc);

    // Use a local buffer that is big enough for the largest legal path
    char* tmpPath = dirname((char*)newSrc);
    if (!tmpPath)
    {
		ThrowXML(XMLPlatformUtilsException, XML4CExcepts::File_CouldNotGetBasePathName);
    }

    char* newXMLString = new char [strlen(tmpPath) +2];
    ArrayJanitor<char> newJanitor(newXMLString);
    strcpy(newXMLString, tmpPath);
    strcat(newXMLString , "/");
    // Return a copy of the path, in Unicode format
    return XMLString::transcode(newXMLString);
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
#ifndef APP_NO_THREADS
void XMLPlatformUtils::closeMutex(void* const mtxHandle)
{
    if (mtxHandle == NULL)
        return;
    if (pthread_mutex_destroy( (pthread_mutex_t*)mtxHandle))
    {
		ThrowXML(XMLPlatformUtilsException, XML4CExcepts::Mutex_CouldNotDestroy);
    }
    if ( (pthread_mutex_t*)mtxHandle)
        delete mtxHandle;
}
void XMLPlatformUtils::lockMutex(void* const mtxHandle)
{
    if (mtxHandle == NULL)
        return;
    if (pthread_mutex_lock( (pthread_mutex_t*)mtxHandle))
    {
		ThrowXML(XMLPlatformUtilsException, XML4CExcepts::Mutex_CouldNotLock);
    }

}
void* XMLPlatformUtils::makeMutex()
{
    pthread_mutex_t* mutex = new pthread_mutex_t;
    if (mutex ==  NULL)
    {
        ThrowXML(XMLPlatformUtilsException,
                 XML4CExcepts::Mutex_CouldNotCreate);
    }
    pthread_mutexattr_t*  attr = new pthread_mutexattr_t;
    pthread_mutexattr_init(attr);
    pthread_mutexattr_setkind_np(attr, MUTEX_RECURSIVE_NP);
    if (pthread_mutex_init(mutex, attr))
    {
        ThrowXML(XMLPlatformUtilsException,
                 XML4CExcepts::Mutex_CouldNotCreate);
    }
    pthread_mutexattr_destroy(attr);
    delete attr;
    return (void*)(mutex);
}
void XMLPlatformUtils::unlockMutex(void* const mtxHandle)
{
    if (mtxHandle == NULL)
        return;
    if (pthread_mutex_unlock( (pthread_mutex_t*)mtxHandle))
    {
		ThrowXML(XMLPlatformUtilsException, XML4CExcepts::Mutex_CouldNotUnlock);
    }
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

#ifndef APP_NO_THREADS

// -----------------------------------------------------------------------
//  Miscellaneous synchronization methods
// -----------------------------------------------------------------------
void* XMLPlatformUtils::compareAndSwap ( void**      toFill ,
                    const void* const newValue ,
                    const void* const toCompare)
{
    boolean_t boolVar = compare_and_swap((atomic_p)toFill, (int *)&toCompare, (int)newValue );
    return (void *)toCompare;
}

int XMLPlatformUtils::atomicIncrement(int &location)
{
    int retVal = fetch_and_add( (atomic_p)&location, 1);
    return retVal+1;
}
int XMLPlatformUtils::atomicDecrement(int &location)
{
    int retVal = fetch_and_add( (atomic_p)&location, -1);
    return retVal-1;
}

#else

// -----------------------------------------------------------------------
//  Miscellaneous synchronization methods
// -----------------------------------------------------------------------

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

