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
// NOTE:
//
//  Xerces is not officially supported on Tandem. This file was sent
//  in by one of the Tandem users and is included in the distribution
//  just for convenience. Please send any defects / modification
//  reports to xml4c@us.ibm.com
// ---------------------------------------------------------------------------



// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
// XXX #include    <pthread.h>
// XXX #include    <sys/atomic_op.h>

#include    <util/PlatformUtils.hpp>
#include    <util/RuntimeException.hpp>
#include    <util/Janitor.hpp>
#include    <stdio.h>
#include    <stdlib.h>
#include    <errno.h>
#include    <libgen.h>
#include    <sys/timeb.h>
#include    <string.h>


// ---------------------------------------------------------------------------
//  Local Methods
// ---------------------------------------------------------------------------
static void WriteCharStr( FILE* stream, const char* const toWrite)
{
    if (!fputs(toWrite, stream))
    {
        throw XMLPlatformUtilsException("Could not write to standard out/err");
    }
}

static void WriteUStrStdErr( const XMLCh* const toWrite)
{
    char* tmpVal = XMLString::transcode(toWrite);
    if (!fputs(tmpVal, stderr))
    {
        throw XMLPlatformUtilsException("Could not write to standard error file");
    }
}

static void WriteUStrStdOut( const XMLCh* const toWrite)
{
    char* tmpVal = XMLString::transcode(toWrite);
    if (!fputs(tmpVal, stdout))
    {
        throw XMLPlatformUtilsException("Could not write to standard out file");
    }
}


// ---------------------------------------------------------------------------
//  XMLPlatformUtils: Platform init method
// ---------------------------------------------------------------------------
void XMLPlatformUtils::platformInit()
{
}


void XMLPlatformUtils::setupIntlPath()
{
    //
    //  We need to figure out the path to the Intl classes. They will be
    //  in the .\Intl subdirectory under this DLL.
    //
    char* tmpBuf = getenv("XML4C2INTLDIR");
    //check if environment variable is set
    if (tmpBuf !=0) { 
        // Store this string in the static member
        char    cBuf[1024];

        strcpy(cBuf, tmpBuf);
        unsigned int pathLen = strlen(tmpBuf);
    
        if (*(tmpBuf + pathLen - 1) != '/')
            strcpy((tmpBuf + pathLen), "/");

        fgIntlPath = new char[strlen(tmpBuf)+1 ]; 
        strcpy((char*)fgIntlPath, tmpBuf);
        return;
    }

    //
    //  If we did not find the environment var, so lets try to go the auto
    //  search route.
    //

    static const char* const pubDLLNameBase = Xerces_DLLName;
    char pubDLLNameReal[256];
    strcpy(pubDLLNameReal, pubDLLNameBase);
    strcat(pubDLLNameReal, gXML4C2VersionStr);
    strcat(pubDLLNameReal, ".a");

    char* libName = new char[ strlen(pubDLLNameReal)];
    strcpy(libName, pubDLLNameReal);
    char* libPath = getenv("LIBPATH");

    //Now create an array of the directory listing in this path

    //First do the searching process for the first directory listing
    char*  libPathName = strtok(libPath, ":");

    char* libfile = new char[ strlen(libPathName) + strlen(libName) +2];
    strcpy(libfile, libPathName);
    strcat(libfile, "/");
    strcat(libfile, libName);

    if ((FILE*)fopen(libfile, "rb"))
    {
        delete libfile;
        libfile=0;
        if (tmpBuf) {
            delete tmpBuf;
            tmpBuf =0;
        }
        tmpBuf = new char[strlen(libPathName)+ strlen("/intlFiles/locales/")+1];
        strcpy(tmpBuf, libPathName);
        strcat(tmpBuf, "/intlFiles/locales/");
        fgIntlPath = new char[strlen(tmpBuf)+1 ];
        strcpy((char*)fgIntlPath, tmpBuf);
                return;
    }

    //do the searching process for the rest directory listings
    while  (libPathName != NULL)
    {
        libPathName = strtok(0, ":");

        if (libPathName == NULL)
                break;

        libfile = new char[ strlen(libPathName) + strlen(libName) +2];
        strcpy(libfile, libPathName);
        strcat(libfile, "/");
        strcat(libfile, libName);

        if ((FILE*)fopen(libfile, "rb"))
        {
                delete libfile;
                libfile=0;
                if (tmpBuf) {
                        delete tmpBuf;
                        tmpBuf =0;
                }
                tmpBuf = new char[strlen(libPathName)+ strlen("/intlFiles/locales/")+1];
                strcpy(tmpBuf, libPathName);
                strcat(tmpBuf, "/intlFiles/locales/");
                fgIntlPath = new char[strlen(tmpBuf)+1 ];
                strcpy((char*)fgIntlPath, tmpBuf);
                return;
        }
    }
    if (fgIntlPath == NULL) {
    printf("Environment variable 'XML4C2INTLDIR' not defined \n");
    throw RuntimeException(
            "Environment variable 'XML4C2INTLDIR' not defined."
                          );
    }
}


// ---------------------------------------------------------------------------
//  XMLPlatformUtils: File Methods
// ---------------------------------------------------------------------------
unsigned int XMLPlatformUtils::curFilePos(FileHandle theFile)
{
    // Get the current position
    int curPos = ftell( (FILE*)theFile);
    if (curPos == -1)
        throw XMLPlatformUtilsException("XMLPlatformUtils::curFilePos - Could not get current pos");

    return (unsigned int)curPos;
}

void XMLPlatformUtils::closeFile(FileHandle theFile)
{
    if (fclose((FILE*)theFile))
        throw XMLPlatformUtilsException("XMLPlatformUtils::closeFile - Could not close the file handle");
}

unsigned int XMLPlatformUtils::fileSize(FileHandle theFile)
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

FileHandle XMLPlatformUtils::openFile(const unsigned short* const fileName)
{
    const char* tmpFileName = XMLString::transcode(fileName);
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
        throw XMLPlatformUtilsException("XMLPlatformUtils::readFileBuffer - Read failed");
    }

    return (unsigned int)noOfItemsRead;
}


void XMLPlatformUtils::resetFile(FileHandle theFile)
{
    // Seek to the start of the file
    if (fseek((FILE*)theFile, 0, SEEK_SET) )
        throw XMLPlatformUtilsException("XMLPlatformUtils::resetFile - Could not seek to beginning");
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

    // Use a local buffer that is big enough for the largest legal path
     char* tmpPath = dirname((char*)newSrc);
    if (!tmpPath)
    {
        throw XMLPlatformUtilsException("XMLPlatformUtils::resetFile - Could not get the base path name");
    }

    char* newXMLString = new char [strlen(tmpPath) +1];
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
        delete mtxHandle;
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


FileHandle XMLPlatformUtils::openStdInHandle()
{
    return (FileHandle)fdopen(dup(0), "rb");
}
#endif
