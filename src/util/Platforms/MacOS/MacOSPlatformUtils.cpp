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

/**
 * $Log$
 * Revision 1.2  2000/02/06 07:48:28  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.1.1.1  1999/11/09 01:06:55  twl
 * Initial checkin
 *
 * Revision 1.2  1999/11/08 20:45:30  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */


// ---------------------------------------------------------------------------
// Note:
//   XML4C is not officially supported on Macintosh. This file was sent
//   in by one of the Macintosh users and is included in the distribution
//   just for convenience. Please send any defects / modification
//   reports to xml4c@us.ibm.com
// ---------------------------------------------------------------------------


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <util/Janitor.hpp>
#include <util/PlatformUtils.hpp>
#include <util/RuntimeException.hpp>
#include <util/String.hpp>
#include <util/XMLUni.hpp>
#include <util/MacOSDefs.hpp>
#include <iostream.h>
#include <files.h>
#include <gestalt.h>
#include <traps.h>
#include <lowmem.h>
#include <Multiprocessing.h>
#include <string.h>
#include <stdlib.h>



//----------------------------------------------------------------------------
//  Local Data
//
//  gFileSystemReady
//    This flag indicates if the file system is ready to be used.  The only
//    thing that might stop us is the absence of FSspec file manipulation
//    routines in the OS.
//
//  gGestaltAvail
//    This flag indicates the presence of the gestalt manager.  This version
//    of the MacOS port can not run without it.
//----------------------------------------------------------------------------
static const char *resBaseStr = "/Access The Resource Fork Instead of the data fork?";
static bool gFileSystemReady = false;
static bool gGestaltAvail = false;
static MPCriticalRegionID gCriticalRegion;


//----------------------------------------------------------------------------
// XMLResFile methods
//----------------------------------------------------------------------------
XMLResFile::XMLResFile();
        

void XMLResFile::open(const char* const resInfo)
{
    char option[32], value[32], command[70];
    int cmdEnd = 0, cmdStart = 0;
    int optEnd = 0, sep;
    int mode = -1;
    int typeValid = 0;
    
    if (!strchr(&resInfo[cmdStart], '/'))
        throw XMLPlatformUtilsException("XMLPlatformUtils::openFile -- Malformed resource locater");
        
    while(resInfo[cmdEnd] != '/')
    {
        if (strchr(&resInfo[cmdStart], '&') < strchr(&resInfo[cmdStart], '/') && strchr(&resInfo[cmdStart], '&') != 0)
            cmdEnd = strchr(&resInfo[cmdStart], '&') - resInfo - 1;
        else
            cmdEnd = strchr(&resInfo[cmdStart], '/') - resInfo - 1;
            
        if (cmdEnd - cmdStart > 68)
            throw XMLPlatformUtilsException("XMLPlatformUtils::openFile -- resource option too long (>68 chars)");
            
        memcpy(command, &resInfo[cmdStart], cmdEnd - cmdStart + 1);
        command[cmdEnd - cmdStart + 1] = 0;
        if (!strchr(command, '='))
            throw XMLPlatformUtilsException("XMLPlatformUtils::openFile -- Malformed resource locater");
            
        sep = strchr(command, '=') - command;
        memcpy(option, command, sep);
        option[sep] = 0;
        memcpy(value, &command[sep+1], strlen(command) - sep);
        
        if (!strcmp(option, "mode"))
        {
            if (!strcmp(value, "by_id"))
                mode = 1;
            else if (!strcmp(value, "by_id1"))
                mode = 2;
            else if (!strcmp(value, "by_name"))
                mode = 3;
            else if (!strcmp(value, "by_name1"))
                mode = 4;
            else
                throw XMLPlatformUtilsException("XMLPlatformUtils::openFile -- 'mode' has to be 'by_id' or 'by_id1' or 'by_name' or 'by_name1'");
        }
        if (!strcmp(option, "type"))
        {
            if (strlen(value) != 4)
                throw XMLPlatformUtilsException("XMLPlatformUtils::openFile -- 'type' has to be four characters long");
            typeValid = 1;
            type = 0;
            type += value[0] << 24;
            type += value[1] << 16;
            type += value[2] << 8;
            type += value[3];
        }
        
        cmdStart = cmdEnd + 2;
        cmdEnd++;
    }
    
    if (mode == 0)
        throw XMLPlatformUtilsException("XMLPlatformUtils::openFile -- Malformed resource locater requires a 'mode'");
    if (typeValid == 0)
        throw XMLPlatformUtilsException("XMLPlatformUtils::openFile -- Malformed resource locater requires a 'type'");
        
    switch(mode)
    {
        case 1: case 2:
            id = atol(&resInfo[cmdEnd+1]);
            if (mode == 1)
                data = GetResource(type, id);
            else
                data = Get1Resource(type, id);
            break;
            
        case 3: case 4:
            if (strlen(&resInfo[cmdEnd]) >= 255)
                throw XMLPlatformUtilsException("XMLPlatformUtils::openFile -- Resource names have to be 255 characters or less");
            strcpy((char*)name, &resInfo[cmdEnd]);
            name[0] = strlen((char*)&name[1]);
            if (mode == 3)
                data = GetNamedResource(type, name);
            else
                data = Get1NamedResource(type, name);
            break;
    }
    
    if (ResError() != noErr)
        throw XMLPlatformUtilsException("XMLPlatformUtils::openFile -- Error opening resource");
        
    GetResInfo(data, &id, &type, name);
    len = GetResourceSizeOnDisk(data);
    if (ResError() != noErr)
        throw XMLPlatformUtilsException("XMLPlatformUtils::openFile -- Error loading resource info");
        
    valid = 1;
}

unsigned int XMLResFile::read(const unsigned int buffLen, XMLByte* const buff)
{
    unsigned int totAvail = len - pos;
    unsigned int numRead = (buffLen >= totAvail) ? totAvail : buffLen;
    
    HLock(data);
    memcpy(buff, *data, numRead);
    HUnlock(data);
    pos += numRead;
    return numRead;
}

void XMLResFile::close()
{
    if (!valid)
        throw XMLPlatformUtilsException("XMLPlatformUtils::curFilePos -- Not a valid file");
    ReleaseResource(data);
    valid = 0;
}
        
unsigned int XMLResFile::currPos()
{
    if (!valid)
        throw XMLPlatformUtilsException("XMLPlatformUtils::curFilePos -- Not a valid file");
    return pos;
}

void XMLResFile::reset()
{
    if (!valid)
        throw XMLPlatformUtilsException("XMLPlatformUtils::resetFile -- Not a valid file");
    pos = 0;
}

unsigned int XMLResFile::size()
{
    if (!valid)
        throw XMLPlatformUtilsException("XMLPlatformUtils::fileSize -- Not a valid file");
    return len;
}

XMLResFile::~XMLResFile()
{
    if (valid)
        close();
}

//----------------------------------------------------------------------------
// XMLMacFile methods
//----------------------------------------------------------------------------

unsigned int XMLMacFile::currPos()
{
    long len;
    if (!valid)
        throw XMLPlatformUtilsException("XMLPlatformUtils::curFilePos -- Not a valid file");
    if (noErr != GetFPos(fileRef, &len))
        throw XMLPlatformUtilsException("XMLPlatformUtils::curFilePos -- Error getting file position");
    return len;
}

void XMLMacFile::close()
{
    if (!valid)
        throw XMLPlatformUtilsException("XMLPlatformUtils::closeFile -- Not a valid file");
    if (noErr != FSClose(fileRef))
        throw XMLPlatformUtilsException("XMLPlatformUtils::closeFile -- Error closing file");
    valid = 0;
}

unsigned int XMLMacFile::size()
{
    long len;
    
    if (!valid)
        throw XMLPlatformUtilsException("XMLPlatformUtils::fileSize -- Not a valid file");
    if (noErr != GetEOF(fileRef, &len))
        throw XMLPlatformUtilsException("XMLPlatformUtils::fileSize -- Error closing file");
    return len;
}

void XMLMacFile::open(const char* const fileName)
{
    OSErr myErr;

    if (valid)
        throw XMLPlatformUtilsException("XMLPlatformUtils::openFile -- Can't open the same file twice");
    if (strlen(fileName) > 255)
        throw XMLPlatformUtilsException("XMLPlatformUtils::openFile(const char* const) -- File name too long (>255 chars)");
        
    if (fileName[0] == '/')
    {
        strcpy((char*)&pStr[1], &fileName[1]);
        pStr[0] = strlen(fileName) - 1;
        pStr[strlen(fileName)] = 0;
    } else {
        strcpy((char*)&pStr[1], fileName);
        pStr[0] = strlen(fileName);
        pStr[strlen(fileName) + 1] = 0;
    }
    
    for(int i = 1; i <= pStr[0]; i++)
        if (pStr[i] == '/')
            pStr[i] = ':';
            
    myErr = HOpenDF(0, 0, pStr, fsRdWrPerm, &fileRef);
    if (myErr != noErr)
    {
        cerr << myErr << endl;
        throw XMLPlatformUtilsException("XMLPlatformUtils::openFile(const char* const) -- Failed to open file.");
    }
        
    valid = 1;
}

unsigned int XMLMacFile::read(const unsigned int toRead, XMLByte* const toFill)
{
    long bytesRead = toRead;

    if (!valid)
        throw XMLPlatformUtilsException("XMLPlatformUtils::readFileBuff -- Not a valid file");
    if (noErr != FSRead(fileRef, &bytesRead, toFill))
        throw XMLPlatformUtilsException("XMLPlatformUtils::readFileBuffer - Read failed");
    return (unsigned int)bytesRead;
}


void XMLMacFile::reset()
{
    if (!valid)
        throw XMLPlatformUtilsException("XMLPlatformUtils::resetFile -- Not a valid file");
    if (noErr != SetFPos(fileRef, fsFromStart, 0))
        throw XMLPlatformUtilsException("XMLPlatformUtils::fileSize -- Error closing file");
}

XMLMacFile::~XMLMacFile()
{
    if (valid)
        close();
}

// ---------------------------------------------------------------------------
//  Local methods
// ---------------------------------------------------------------------------
static void WriteCharStrStdErr(const char* const toWrite)
{
    // Just print the data out the the MetroWerks standard error.
    cerr << toWrite;
}


static void WriteCharStrStdOut(const char* const toWrite)
{
    // Just print the data out the the MetroWerks standard error.
    cout << toWrite;
}


static void WriteUStrStdErr(const XMLCh* const toWrite)
{
    // Oh well, got to do it the hard way
    char* tmpVal = XMLString::transcode(toWrite);
    ArrayJanitor<char> janTmp(tmpVal);
    WriteCharStrStdErr(tmpVal);
}


static void WriteUStrStdOut(const XMLCh* const toWrite)
{
    // Oh well, got to do it the hard way
    char* tmpVal = XMLString::transcode(toWrite);
    ArrayJanitor<char> janTmp(tmpVal);
    WriteCharStrStdOut(tmpVal);
}



// ---------------------------------------------------------------------------
//  XMLPlatformUtils: File Methods
// ---------------------------------------------------------------------------
unsigned int XMLPlatformUtils::curFilePos(const FileHandle theFile)
{
    return theFile->currPos();
}

void XMLPlatformUtils::closeFile(const FileHandle theFile)
{
    theFile->close();
}

unsigned int XMLPlatformUtils::fileSize(const FileHandle theFile)
{
    return theFile->size();
}

FileHandle XMLPlatformUtils::openFile(const char* const fileName)
{
     FileHandle file = 0;
     int isRes = 0;
     
    // Check to make sure the file system is in a state where we can use it     
     if (!gFileSystemReady)
        throw XMLPlatformUtilsException("XMLPlatformUtils::openFile(const char* const) -- File system not ready."
                  "  Maybe missing gestalt or no support for FSSpec's.");
                  
    if (strlen(fileName) >= strlen(resBaseStr))
        if (strstr(fileName, resBaseStr) == fileName)
            isRes = 1;
            
    if (isRes == 0)
    {
        file = new XMLMacFile();
        file->open(fileName);
    }
    else
    {
        file = new XMLResFile();
        file->open(&fileName[strlen(resBaseStr)]);
    }
    if (file == 0)
        throw XMLPlatformUtilsException("XMLPlatformUtils::openFile(const char* const) -- Failed to allocate file object.");
    return file;
}

FileHandle XMLPlatformUtils::openFile(const XMLCh* const fileName)
{
    char* tmpName = XMLString::transcode(fileName);
    FileHandle retVal = openFile(tmpName);
    delete [] tmpName;

    return retVal;
}


FileHandle XMLPlatformUtils::openStdInHandle()
{
    throw XMLPlatformUtilsException("XMLPlatformUtils::openStdInHandle -- Standard input not supported on MacOS");
    return NULL;//(void*)&cin;
}


unsigned int
XMLPlatformUtils::readFileBuffer(   const   FileHandle      theFile
                                    , const unsigned int    toRead
                                    ,       XMLByte* const  toFill)
{
    return theFile->read(toRead, toFill);
}


void XMLPlatformUtils::resetFile(FileHandle theFile)
{
    theFile->reset();
}

// ---------------------------------------------------------------------------
//  XMLPlatformUtils: File Methods
// ---------------------------------------------------------------------------
void XMLPlatformUtils::writeToStdErr(const XMLCh* const toWrite)
{
    WriteUStrStdErr(toWrite);
}

void XMLPlatformUtils::writeToStdErr(const char* const toWrite)
{
    WriteCharStrStdErr(toWrite);
}

void XMLPlatformUtils::writeToStdOut(const XMLCh* const toWrite)
{
    WriteUStrStdOut(toWrite);
}

void XMLPlatformUtils::writeToStdOut(const char* const toWrite)
{
    WriteCharStrStdOut(toWrite);
}



// ---------------------------------------------------------------------------
//  XMLPlatformUtils: Platform methods
// ---------------------------------------------------------------------------
XMLCh* XMLPlatformUtils::getBasePath(const XMLCh* const srcPath)
{
    char* workingPath = XMLString::transcode(srcPath);
    XMLCh *result;
    char dirSep = '/', *lastSep = 0;
    
    for(int i = 0; workingPath && workingPath[i]; i++)
        if (workingPath[i] == dirSep)
            lastSep = &workingPath[i];
            
    if (lastSep == 0)
        return XMLString::transcode("");
        
    lastSep++;
    dirSep = *lastSep;
    *lastSep = 0;
//    cout << "Returning base path: " << workingPath << endl;
    result = XMLString::transcode(workingPath);
    *lastSep = dirSep;
    delete [] workingPath;

    return result;    
}

bool XMLPlatformUtils::isRelative(const XMLCh* const toCheck)
{
    char* workingPath = XMLString::transcode(toCheck);
    char dirSep = '/', *sep;
    short refNum;
    long freeSpace;
    unsigned char volName[260];
    
    if (!*workingPath)
        return false;
    
    if (strstr(workingPath, ":"))
        dirSep = ':';
        
    if ( dirSep == '/' && workingPath[0] == '/')
        return false;
    if ( dirSep == '/' && workingPath[0] != '/')
        return true;
        
    sep = strstr(workingPath, ":");
    if (sep == 0)
        return true;
        
    dirSep = *sep;
    *sep = 0;
    
    for(short i = 0; i < 7; i++)
    {
        if ( noErr == GetVInfo(i, volName, &refNum, &freeSpace))
        {
            if (strlen(workingPath) != volName[0])
                continue;
                
            for(int j = 0, refNum = 0; j < volName[0]; j++)
                if (workingPath[j] != volName[j+1])
                    refNum = 1;
                    
            if (!refNum)
            {
                *sep = dirSep;
                delete [] workingPath;
                return false;
            }
        }
    }
    *sep = dirSep;
    delete [] workingPath;
    return true;
}



// ---------------------------------------------------------------------------
//  XMLPlatformUtils: Timing Methods
// ---------------------------------------------------------------------------
unsigned long XMLPlatformUtils::getCurrentMillis()
{
    return TickCount()*10;
}



// ---------------------------------------------------------------------------
//  Mutex methods
// ---------------------------------------------------------------------------
void XMLPlatformUtils::closeMutex(void* const mtxHandle)
{
    if (!MPLibraryIsLoaded())
        throw XMLPlatformUtilsException("XMLPlatformUtils::makeMutex -- Multiprocessing library not installed correctly.  Please Install");

    MPSemaphoreID *sema = (MPSemaphoreID*)mtxHandle;
    if (!sema) return;
        
    if (noErr != MPDeleteSemaphore(*sema))
        throw XMLPlatformUtilsException("XMLPlatformUtils::closeMutex -- Error deleting semaphore");
}


void XMLPlatformUtils::lockMutex(void* const mtxHandle)
{
    if (!MPLibraryIsLoaded())
        throw XMLPlatformUtilsException("XMLPlatformUtils::makeMutex -- Multiprocessing library not installed correctly.  Please Install");

    MPSemaphoreID *sema = (MPSemaphoreID*)mtxHandle;
    if (!sema) return;
    
    if (noErr != MPWaitOnSemaphore(*sema, -1))
        throw XMLPlatformUtilsException("XMLPlatformUtils::lockMutex -- Error locking semaphore");
}


void* XMLPlatformUtils::makeMutex()
{
    if (!MPLibraryIsLoaded())
        throw XMLPlatformUtilsException("XMLPlatformUtils::makeMutex -- Multiprocessing library not installed correctly.  Please Install");
        
    MPSemaphoreID *result = new MPSemaphoreID;
    if (!result)
        throw XMLPlatformUtilsException("XMLPlatformUtils::makeMutex -- Error allocating memory for a semaphore");
    
    if (noErr != MPCreateBinarySemaphore(result))
        throw XMLPlatformUtilsException("XMLPlatformUtils::makeMutex -- Error Creating semaphore");
        
    return (void*)result;
}


void XMLPlatformUtils::unlockMutex(void* const mtxHandle)
{
    if (!MPLibraryIsLoaded())
        throw XMLPlatformUtilsException("XMLPlatformUtils::makeMutex -- Multiprocessing library not installed correctly.  Please Install");

    MPSemaphoreID *sema = (MPSemaphoreID*)mtxHandle;
    if (!sema) return;
    
    if (noErr != MPSignalSemaphore(*sema))
        throw XMLPlatformUtilsException("XMLPlatformUtils::unlockMutex -- Error unlocking semaphore");
}


// ---------------------------------------------------------------------------
//  Miscellaneous synchronization methods
// ---------------------------------------------------------------------------
void*
XMLPlatformUtils::compareAndSwap(       void**      toFill
                                , const void* const newValue
                                , const void* const toCompare)
{
    // Note -- I'm note sure what this method is supposed to do.  I just copied
    // the code from the linux utils.
    
    void *retVal = *toFill;
    if (*toFill == toCompare)
              *toFill = (void *)newValue;
    return retVal;
    
    throw XMLPlatformUtilsException("XMLPlatformUtils::compareAndSwap -- Not Implemented Yet.");
}


int XMLPlatformUtils::atomicIncrement(int &location)
{
/*    if (!MPLibraryIsLoaded())
        throw XMLPlatformUtilsException("XMLPlatformUtils::platformInit -- Multiprocessing library not installed correctly.  Please Install");

    if (noErr != MPEnterCriticalRegion(gCriticalRegion, -1))
        throw XMLPlatformUtilsException("XMLPlatformUtils::atomicIncrement -- Failed to enter the critical region.");*/
    
    int result = location++;
    
/*    if (noErr != MPExitCriticalRegion(gCriticalRegion))
        throw XMLPlatformUtilsException("XMLPlatformUtils::atomicIncrement -- Failed to exit the critical region.");*/
        
    return result;
}


int XMLPlatformUtils::atomicDecrement(int &location)
{
/*    if (!MPLibraryIsLoaded())
        throw XMLPlatformUtilsException("XMLPlatformUtils::platformInit -- Multiprocessing library not installed correctly.  Please Install");

    if (noErr != MPEnterCriticalRegion(gCriticalRegion, -1))
        throw XMLPlatformUtilsException("XMLPlatformUtils::atomicIncrement -- Failed to enter the critical region.");*/
    
    int result = location--;
    
/*    if (noErr != MPExitCriticalRegion(gCriticalRegion))
        throw XMLPlatformUtilsException("XMLPlatformUtils::atomicIncrement -- Failed to exit the critical region.");*/
        
    return result;
}

// Routine to check to see if a system function is available
static bool MySWRoutineAvailable (int trapWord)
{
    TrapType trType;

    // first determine whether it is an Operating System or Toolbox routine
    if ((trapWord & 0x0800) == 0)
        trType = OSTrap;
    else
        trType = ToolTrap;
        
    // filter cases where older systems mask with 0x1FF rather than 0x3FF
    if (trType == ToolTrap && ((trapWord & 0x03FF) >= 0x200) &&
            (GetToolboxTrapAddress(0xA86E) == GetToolboxTrapAddress(0xAA6E)))
        return false;
    else
        return (NGetTrapAddress(trapWord, trType) !=
                    GetToolboxTrapAddress(_Unimplemented));
}

// ---------------------------------------------------------------------------
//  XMLPlatformUtils: Private Static Methods
// ---------------------------------------------------------------------------

//
//  This method handles the MacOS basic init functions.
//
void XMLPlatformUtils::platformInit()
{
    OSErr myErr;
    long feature;
    
    // Figure out if we have the gestalt manager
    gGestaltAvail = MySWRoutineAvailable(_Gestalt);
    
    // Figure out if we have the FSspec function to use
    if (gGestaltAvail)
    {
        myErr = Gestalt(gestaltFSAttr, &feature);
        if (myErr == noErr)
        {
            if (feature & gestaltHasFSSpecCalls)
                gFileSystemReady = true;
        }
    }

/*    if (!MPLibraryIsLoaded())
        throw XMLPlatformUtilsException("XMLPlatformUtils::platformInit -- Multiprocessing library not installed correctly.  Please Install");
        
    if (noErr != MPCreateCriticalRegion(&gCriticalRegion))
        throw XMLPlatformUtilsException("XMLPlatformUtils::platformInit -- Error creating critical section");*/
}

//
//  This is the first thing called during init. Each platform needs to set
//  up the path to the Internationalization code.
//
//  NOTE:   It CANNOT use an String class methods, because that class has not
//          be set up yet.
//
void XMLPlatformUtils::setupIntlPath()
{
//    fgIntlPath = "Macintosh HD:Desktop Folder:xml4csrc2_2_0:intlFiles:data:locales:";
    short fileRef;
    unsigned char pStr[300];
    OSErr myErr;
    OSType textCode = 'TEXT';
//    StandardFileReply result;
//    FInfo fileInfo;    
    long feature;
    
    if (fgIntlPath != 0)
        return;
        
    // Figure out if we have the gestalt manager
    gGestaltAvail = MySWRoutineAvailable(_Gestalt);
    
    // Figure out if we have the FSspec function to use
    if (gGestaltAvail)
    {
        myErr = Gestalt(gestaltFSAttr, &feature);
        if (myErr == noErr)
        {
            if (feature & gestaltHasFSSpecCalls)
                gFileSystemReady = true;
        }
    }

    // Check to make sure the file system is in a state where we can use it
    if (!gFileSystemReady)
        throw XMLPlatformUtilsException("XMLPlatformUtils::setupIntlPath -- File system not ready."
                  "  Maybe missing gestalt or no support for FSSpec's.");

    if (noErr == HOpenDF(0, 0, "\p:locales:convrtrs.txt", fsRdWrPerm, &fileRef))
    {
        if (noErr != FSClose(fileRef))
            throw XMLPlatformUtilsException("XMLPlatformUtils::setupIntlPath -- ERROR");
            
        fgIntlPath = ":locales:";
        return;
    }
    
    throw XMLPlatformUtilsException("XMLPlatformUtils::setupIntlPath -- Couldn't find data files");
/*    if (noErr != StandardGetFile("\pPlease locate the convrtrs.txt file in the locales directory:", 0, 1, &textCode, &result))
            throw XMLPlatformUtilsException("XMLPlatformUtils::setupIntlPath -- StandardGetFile failed");
            
    if (result.sfGood == 0)
            throw XMLPlatformUtilsException("XMLPlatformUtils::setupIntlPath -- failed to locate locale files");
            
    fgIntlPath = new char[result.sfFile.[0]+1];
    fgIntlPath[result.sfFile.[0]] = 0;
    memcpy(fgIntlPath, result.sfFile., result.sfFile.[0]);
    cout << fgIntlPath << endl;*/
            //throw XMLPlatformUtilsException("XMLPlatformUtils::setupIntlPath -- failed to load file info");
}

// These functions are needed because MacOS doesn't define them
// even though they are used.
// Compare lexigraphically two strings
static char tolower(char c)
{
    if (c >= 'A' && c <= 'Z')
        return c + 'a' - 'A';
    return c;
}

int stricmp(const char *s1, const char *s2)
{
    char c1, c2;
    while (1)
    {
        c1 = tolower(*s1++);
        c2 = tolower(*s2++);
        if (c1 < c2) return -1;
        if (c1 > c2) return 1;
        if (c1 == 0) return 0;
    }
}

// Compare lexigraphically two strings up to a max length

int strnicmp(const char *s1, const char *s2, int n)
{
    int i;
    char c1, c2;
    for (i=0; i<n; i++)
    {
        c1 = tolower(*s1++);
        c2 = tolower(*s2++);
        if (c1 < c2) return -1;
        if (c1 > c2) return 1;
        if (!c1) return 0;
    }
    return 0;
}

