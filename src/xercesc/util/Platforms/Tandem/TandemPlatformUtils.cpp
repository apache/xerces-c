/*
 * Copyright 1999-2000,2004 The Apache Software Foundation.
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
 * Revision 1.13  2004/09/08 13:56:43  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.12  2003/12/17 15:16:10  cargilld
 * Platform update for memory management so that the static memory manager (one
 * used to call Initialize) is only for static data.
 *
 * Revision 1.11  2003/12/17 13:58:03  cargilld
 * Platform update for memory management so that the static memory manager (one
 * used to call Initialize) is only for static data.
 *
 * Revision 1.10  2003/10/01 16:32:40  neilg
 * improve handling of out of memory conditions, bug #23415.  Thanks to David Cargill.
 *
 * Revision 1.9  2003/05/15 18:37:49  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.8  2003/04/24 02:58:31  peiyongz
 * Logical Path Resolution
 *
 * Revision 1.7  2003/03/09 17:00:11  peiyongz
 * PanicHandler
 *
 * Revision 1.6  2003/02/05 18:29:27  tng
 * [Bug 13437] Incorrect memory management in XXXPlatformUtils.cpp.
 *
 * Revision 1.5  2003/01/09 15:30:39  tng
 * Missing panic function in Tandem
 *
 * Revision 1.4  2002/12/12 16:29:30  peiyongz
 * loadAMsgSet() added
 *
 * Revision 1.3  2002/11/04 15:13:01  tng
 * C++ Namespace Support.
 *
 * Revision 1.2  2002/05/21 20:31:48  tng
 * Minor update: Remove obsolete code
 *
 * Revision 1.1.1.1  2002/02/01 22:22:26  peiyongz
 * sane_include
 *
 * Revision 1.4  2000/03/02 21:10:38  abagchi
 * Added empty function platformTerm()
 *
 * Revision 1.3  2000/03/02 19:55:32  roddey
 * This checkin includes many changes done while waiting for the
 * 1.1.0 code to be finished. I can't list them all here, but a list is
 * available elsewhere.
 *
 * Revision 1.2  2000/02/06 07:48:30  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.1.1.1  1999/11/09 01:06:24  twl
 * Initial checkin
 *
 * Revision 1.2  1999/11/08 20:45:32  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
// XXX #include    <pthread.h>
// XXX #include    <sys/atomic_op.h>

#include    <xercesc/util/PlatformUtils.hpp>
#include    <xercesc/util/RuntimeException.hpp>
#include    <xercesc/util/Janitor.hpp>
#include    <xercesc/util/PanicHandler.hpp>
#include    <stdio.h>
#include    <stdlib.h>
#include    <errno.h>
#include    <libgen.h>
#include    <sys/timeb.h>
#include    <string.h>
#include    <xercesc/util/OutOfMemoryException.hpp>

#if defined (XML_USE_ICU_MESSAGELOADER)
    #include <xercesc/util/MsgLoaders/ICU/ICUMsgLoader.hpp>
#elif defined (XML_USE_ICONV_MESSAGELOADER)
    #include <xercesc/util/MsgLoaders/MsgCatalog/MsgCatalogLoader.hpp>
#else   // use In-memory message loader
    #include <xercesc/util/MsgLoaders/InMemory/InMemMsgLoader.hpp>
#endif

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  XMLPlatformUtils: Platform init method
// ---------------------------------------------------------------------------
void XMLPlatformUtils::platformInit()
{
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
    int curPos = ftell( (FILE*)theFile);
    if (curPos == -1)
        throw XMLPlatformUtilsException("XMLPlatformUtils::curFilePos - Could not get current pos");

    return (unsigned int)curPos;
}

void XMLPlatformUtils::closeFile(FileHandle theFile
                                 , MemoryManager* const manager)
{
    if (fclose((FILE*)theFile))
        throw XMLPlatformUtilsException("XMLPlatformUtils::closeFile - Could not close the file handle");
}

unsigned int XMLPlatformUtils::fileSize(FileHandle theFile
                                        , MemoryManager* const manager)
{
    // Get the current position
    long  int curPos = ftell((FILE*)theFile);
    if (curPos == -1)
        throw XMLPlatformUtilsException("XMLPlatformUtils::fileSize - Could not get current pos");

    // Seek to the end and save that value for return
     if (fseek( (FILE*)theFile, 0, SEEK_END) )
        throw XMLPlatformUtilsException("XMLPlatformUtils::fileSize - Could not seek to end");

    long int retVal = ftell( (FILE*)theFile);
    if (retVal == -1)
        throw XMLPlatformUtilsException("XMLPlatformUtils::fileSize - Could not get the file size");

    // And put the pointer back
    if (fseek( (FILE*)theFile, curPos, SEEK_SET) )
        throw XMLPlatformUtilsException("XMLPlatformUtils::fileSize - Could not seek back to original pos");

    return (unsigned int)retVal;
}

FileHandle XMLPlatformUtils::openFile(const unsigned short* const fileName
                                      , MemoryManager* const manager)
{
    const char* tmpFileName = XMLString::transcode(fileName, manager);
    ArrayJanitor<char> tmpFileNameJan((char*)tmpFileName , manager);
    FileHandle retVal = (FILE*)fopen( tmpFileName , "rb" );

    if (retVal == NULL)
        return 0;
    return retVal;
}

unsigned int
XMLPlatformUtils::readFileBuffer(  FileHandle      theFile
                                , const unsigned int    toRead
                                , XMLByte* const  toFill
                                , MemoryManager* const manager)
{
    size_t noOfItemsRead = fread( (void*) toFill, 1, toRead, (FILE*)theFile);

    if(ferror((FILE*)theFile))
    {
        throw XMLPlatformUtilsException("XMLPlatformUtils::readFileBuffer - Read failed");
    }

    return (unsigned int)noOfItemsRead;
}


void XMLPlatformUtils::resetFile(FileHandle theFile
                                 , MemoryManager* const manager)
{
    // Seek to the start of the file
    if (fseek((FILE*)theFile, 0, SEEK_SET) )
        throw XMLPlatformUtilsException("XMLPlatformUtils::resetFile - Could not seek to beginning");
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
    char* newSrc = XMLString::transcode(srcPath, fgMemoryManager);

    // Use a local buffer that is big enough for the largest legal path
     char* tmpPath = dirname((char*)newSrc);
    if (!tmpPath)
    {
        throw XMLPlatformUtilsException("XMLPlatformUtils::resetFile - Could not get the base path name");
    }

    char* newXMLString = (char*) fgMemoryManager->allocate
    (
        (strlen(tmpPath) +1) * sizeof(char)
    );//new char [strlen(tmpPath) +1];
    ArrayJanitor<char> newJanitor(newXMLString, fgMemoryManager);
    strcpy(newXMLString, tmpPath);
        strcat(newXMLString , "/");
    // Return a copy of the path, in Unicode format
    return XMLString::transcode(newXMLString, manager);
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

    /*** 
     *  REVISIT:
     * 
     *   To be implemented later
    ***/

    XMLCh curDir[]={ chPeriod, chForwardSlash, chNull};
    return getFullPath(curDir);
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


#ifndef __TANDEM
// -----------------------------------------------------------------------
//  Mutex methods
// -----------------------------------------------------------------------
void XMLPlatformUtils::closeMutex(void* const mtxHandle)
{
    if (mtxHandle == NULL)
        return;
    if (pthread_mutex_destroy( (pthread_mutex_t*)mtxHandle))
    {
        throw XMLPlatformUtilsException("Could not destroy a mutex");
    }
    if ( (pthread_mutex_t*)mtxHandle)
        delete (pthread_mutex_t*) mtxHandle;
}
void XMLPlatformUtils::lockMutex(void* const mtxHandle)
{
    if (mtxHandle == NULL)
        return;
    if (pthread_mutex_lock( (pthread_mutex_t*)mtxHandle))
    {
        throw XMLPlatformUtilsException("Could not lock a mutex");
    }

}
void* XMLPlatformUtils::makeMutex()
{
    pthread_mutex_t* mutex = new pthread_mutex_t;
    if (mutex == NULL)
    {
        throw XMLPlatformUtilsException("Could not initialize a mutex");
    }

    if (pthread_mutex_init(mutex, NULL))
    {
        throw XMLPlatformUtilsException("Could not create a mutex");
    }

    return (void*)(mutex);
}
void XMLPlatformUtils::unlockMutex(void* const mtxHandle)
{
    if (mtxHandle == NULL)
        return;
    if (pthread_mutex_unlock( (pthread_mutex_t*)mtxHandle))
    {
        throw XMLPlatformUtilsException("Could not unlock a mutex");
    }
}


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


FileHandle XMLPlatformUtils::openStdInHandle(MemoryManager* const manager)
{
    return (FileHandle)fdopen(dup(0), "rb");
}
#endif

void XMLPlatformUtils::platformTerm()
{
    // We don't have any termination requirements at this time
}

#include <xercesc/util/LogicalPath.c>

XERCES_CPP_NAMESPACE_END
