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
 * Revision 1.1  1999/11/09 01:07:05  twl
 * Initial revision
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
#include    <unistd.h>

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
{

#if defined (XML_USE_ICU_TRANSCODER)
    //
    //  We need to figure out the path to the ICU converter files.
    //

    static const char * xml4cIntlDirEnvVar = "ICU_DATA";

    //
    // Check if environment variable 'ICU_DATA' is set.
    //

    char* envVal = getenv(xml4cIntlDirEnvVar);

    if (envVal != NULL)
    {
        unsigned int pathLen  = strlen(envVal);
        char*        intlPath = new char[pathLen + 2];

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
    //  If we did not find the environment var, so lets try to go the auto
    //  search route.
    //

    char libName[256];
    strcpy(libName, XML4C_DLLName);
    strcat(libName, gXML4CVersionStr);
    strcat(libName, ".sl");

    shl_descriptor* desc = NULL;
    int             ret = 0;
    int             index = 1;

    ret = shl_get(index, &desc);
    while (ret != -1)
    {
       char* fileName = desc->filename;
       if (strstr(fileName, libName) != NULL)
       {
           char* lastSlash = strrchr(fileName, '/');
           size_t chars_to_extract = lastSlash - fileName;
           char *libPathName = new char[chars_to_extract + 1];
           strncpy(libPathName, fileName, chars_to_extract);
           libPathName[chars_to_extract] = 0;
           fgIntlPath = new char[strlen(libPathName)+ strlen("/icu/data/")+1];
           strcpy((char *) fgIntlPath, libPathName);
           strcat((char *) fgIntlPath, "/icu/data/");
           delete libPathName;
           break;
       }
       index++;
       ret = shl_get(index, &desc);
    }

    if (fgIntlPath == NULL)
    {
        fprintf(stderr,
                "Fatal error: Could not find /icu/data relative to %s \n",
                libName);
        fprintf(stderr, 
                "             while trying to auto detect the location ");
        fprintf(stderr, "of the converter files.\n");
        fprintf(stderr,
                "             And the environment variable 'ICU_DATA' is ");
        fprintf(stderr, "not defined.\n");
        panic(XMLPlatformUtils::Panic_NoTransService);
    }

    ICUTransService::setICUPath(fgIntlPath);
    return new ICUTransService;

#else // Use Native transcoding service

    return new IconvTransService;

#endif
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



XMLCh* XMLPlatformUtils::getBasePath(const XMLCh* const srcPath)
{

    //
    //  NOTE: The path provided has always already been opened successfully,
    //  so we know that it is valid. It comes in native format, and goes out
    //  as Unicode always
    //
    char* newSrc = XMLString::transcode(srcPath);

    // Use a local buffer that is big enough for the largest legal path.
    // Note #1186: dirName() is not thread safe.

    char* tmpPath = dirname(newSrc); // dirname() never returns NULL.
    delete [] newSrc;

    char* newXMLString = new char[strlen(tmpPath) + 2];
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
//  Mutex methods 
// -----------------------------------------------------------------------

#if !defined (APP_NO_THREADS)


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

void* XMLPlatformUtils::makeMutex()
{
    pthread_mutex_t* mutex = new pthread_mutex_t;

    if (pthread_mutex_init(mutex, pthread_mutexattr_default))
    {
        ThrowXML(XMLPlatformUtilsException,
                 XML4CExcepts::Mutex_CouldNotCreate);
    }
    return (void*) mutex;
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

