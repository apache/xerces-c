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
 * Revision 1.1  1999/11/09 01:06:44  twl
 * Initial revision
 *
 * Revision 1.2  1999/11/08 20:45:31  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */



// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#ifndef APP_NO_THREADS
#endif

#define INCL_DOSSEMAPHORES
#define INCL_DOSERRORS
#define INCL_DOSMISC

#include    <util/PlatformUtils.hpp>
#include    <util/RuntimeException.hpp>
#include    <util/Janitor.hpp>
#include    <util/XML4CDefs.hpp>
#include    <stdio.h>
#include    <stdlib.h>
#include    <IO.h>
#include    <OS2.h>



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
        ArrayJanitor<char> janText(tmpVal);
        if (!fputs(tmpVal, stderr))
        {
                throw XMLPlatformUtilsException("Could not write to standard error file");
        }
}

static void WriteUStrStdOut( const XMLCh* const toWrite)
{
        char* tmpVal = XMLString::transcode(toWrite);
        ArrayJanitor<char> janText(tmpVal);
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

    static const char * xml4cIntlDirEnvVar = "XML4C2INTLDIR";
    static const char * sharedLibEnvVar    = "LIBPATH";

    char* envVal = getenv(xml4cIntlDirEnvVar);
    //check if environment variable is set
    if (envVal != NULL)
    {
        // Store this string in the static member
        unsigned int pathLen = strlen(envVal);
        fgIntlPath = new char[pathLen + 2];

        strcpy((char *) fgIntlPath, envVal);
        if (envVal[pathLen - 1] != '/')
        {
            strcat((char *) fgIntlPath, "/");
        }
        return;
    }

    //
    //  If we did not find the environment var, so lets try to go the auto
    //  search route.
    //

    char libName[256];
    strcpy(libName, XML4C2_DLLName);
    strcat(libName, gXML4C2VersionStr);
    strcat(libName, ".a");

    char* libEnvVar = getenv(sharedLibEnvVar);
    char* libPath = NULL;

    if (libEnvVar == NULL)
    {
        fprintf(stderr,
                "Error: Could not locate i18n converter files.\n");
        fprintf(stderr,
                "Environment variable '%s' is not defined.\n", sharedLibEnvVar);
        fprintf(stderr,
                "Environment variable 'XML4C2INTLDIR' is also not defined.\n");
        exit(-1);
    }

    //
    // Its necessary to create a copy because strtok() modifies the
    // string as it returns tokens. We don't want to modify the string
    // returned to by getenv().
    //

    libPath = new char[strlen(libEnvVar) + 1];
    strcpy(libPath, libEnvVar);

    //First do the searching process for the first directory listing
    //
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
            fgIntlPath =
              new char[strlen(libPathName)+ strlen("/icu/data/")+1];
            strcpy((char *) fgIntlPath, libPathName);
            strcat((char *) fgIntlPath, "/icu/data/");
            break;
        }

    } // while

    delete libPath;

    if (fgIntlPath == NULL)
    {
        fprintf(stderr,
        "Could not find %s in %s for auto locating the converter files.\n",
                libName, sharedLibEnvVar);
        fprintf(stderr,
                "And the environment variable 'XML4C2INTLDIR' not defined.\n");
        exit(-1);
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
// TBT
{
    size_t noOfItemsRead = fread( (void*) toFill, 1, toRead, (FILE*)theFile);

    if(ferror((FILE*)theFile))
    {
        throw XMLPlatformUtilsException("XMLPlatformUtils::readFileBuffer - Read failed");
    }

    return (unsigned int)noOfItemsRead;
}


void XMLPlatformUtils::resetFile(FileHandle theFile)
// TBT
{
    // Seek to the start of the file
    if (fseek((FILE*)theFile, 0, SEEK_SET) )
        throw XMLPlatformUtilsException("XMLPlatformUtils::resetFile - Could not seek to beginning");
}



// ---------------------------------------------------------------------------
//  XMLPlatformUtils: Timing Methods
// ---------------------------------------------------------------------------
unsigned long XMLPlatformUtils::getCurrentMillis()
// TBT
{
   APIRET  retr;
   ULONG   timerBuf = 0;

   retr =  DosQuerySysInfo( QSV_MS_COUNT, QSV_MS_COUNT, (PVOID) &timerBuf,
                            sizeof( ULONG ) );
   if ( retr != NO_ERROR )
      return (timerBuf);


   return (timerBuf);
}


/* Function dirname (used in XMLPlatformUtils::getBasePath)
*
*  Description:
*    Function returns directoryname
*
*  In:
*   Path and file: e.gG.: "d:\TestDir\testfile.txt
*
*  return:
*    "": ERROR
*    Path: Path is returned in the Format: "[drive]:\[path]\"
*          e.g.: "d:\TestDir\"
*
*/
char* dirname(char* PathName)
// TBT
//         new function
{
   APIRET rc = NO_ERROR;
   ULONG  MaxPathLength = 0;
   ULONG  aulSysInfo[QSV_MAX];

   // variables for path information
   char drive[2 + 1];  // eg: drive = "d:\n";

   // get system information
   rc = DosQuerySysInfo(1L,
                        QSV_MAX,
                        (PVOID)aulSysInfo,
                        sizeof(ULONG)*QSV_MAX);
   if(rc != NO_ERROR) return("");

   // Get Maximum Path Length
   MaxPathLength = aulSysInfo[QSV_MAX_PATH_LENGTH - 1];

   // allocate space for pathinformation
   char* dir        = new char[MaxPathLength + 1];
   char* fname      = new char[MaxPathLength + 1];
   char* ext        = new char[MaxPathLength + 1];
   char* returnPath = new char[MaxPathLength + 1];

   // extract pathinformation
   _splitpath(PathName, drive, dir, fname, ext) ;

   strcpy(returnPath,drive);
   strcat(returnPath,dir);

   return((char*) returnPath);
}

XMLCh* XMLPlatformUtils::getBasePath(const XMLCh* const srcPath)
// TBT
//        So please check if it works correct.
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
    if (strlen(tmpPath) == 0)
    {
        throw XMLPlatformUtilsException("XMLPlatformUtils::resetFile - Could not get the base path name");
    }

    char* newXMLString = new char [strlen(tmpPath) +2];
    ArrayJanitor<char> newJanitor(newXMLString);
    strcpy(newXMLString, tmpPath);
    // TBT

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
//  Base of Mutex handling is copied from Win32PlatformUtil.cpp and
//  patially from AIXPlatformUtil.cpp
//  (depended on which code was easier to understand)
// -----------------------------------------------------------------------
#ifndef APP_NO_THREADS


void XMLPlatformUtils::closeMutex(void* const mtxHandle)
// TBT
{
  if (mtxHandle == NULL)
    return;

  if (DosCloseMutexSem( (HMTX)mtxHandle)) 
  {
    throw XMLPlatformUtilsException("Could not destroy a mutex");
  }
}


void XMLPlatformUtils::lockMutex(void* const mtxHandle)
// 
{
  if (mtxHandle == NULL)
    return;

  if (DosRequestMutexSem( (HMTX)mtxHandle,(ULONG) SEM_INDEFINITE_WAIT) ) 
  {
    throw XMLPlatformUtilsException("Could not lock a mutex");
  }
}


void* XMLPlatformUtils::makeMutex()
// 
{
    HMTX hRet; // Mutex Handle

    if (DosCreateMutexSem(NULL, &hRet, 0, FALSE))
        throw XMLPlatformUtilsException("XMLPlatformUtils::makeMutex - Could not create mutex");
    return (void*)hRet;
}


void XMLPlatformUtils::unlockMutex(void* const mtxHandle)
// 
{
  if (mtxHandle == NULL)
     return;

  if (DosReleaseMutexSem( (HMTX)mtxHandle))
  {
    throw XMLPlatformUtilsException("Could not unlock a mutex");
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

// -----------------------------------------------------------------------
//  Miscellaneous synchronization methods
// -----------------------------------------------------------------------
void* XMLPlatformUtils::compareAndSwap ( void**      toFill ,
                                        const void* const newValue ,
                                        const void* const toCompare)
// TBT
// 
{
    void *retVal = *toFill;
    if (*toFill == toCompare)
       *toFill = (void *)newValue;
    return retVal;
}

int XMLPlatformUtils::atomicIncrement(int &location)
// TBT
// 
{
   return ++location;
}
int XMLPlatformUtils::atomicDecrement(int &location)
// TBT
// 
{
    return --location;
}


FileHandle XMLPlatformUtils::openStdInHandle()
// TBT
{
 return (FileHandle)fdopen(dup(0), "rb"); 
}

