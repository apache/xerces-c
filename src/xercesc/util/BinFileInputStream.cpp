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
 * Revision 1.5  2004/09/08 13:56:21  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.4  2003/12/17 13:58:02  cargilld
 * Platform update for memory management so that the static memory manager (one
 * used to call Initialize) is only for static data.
 *
 * Revision 1.3  2003/05/16 03:11:22  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.2  2002/11/04 15:22:03  tng
 * C++ Namespace Support.
 *
 * Revision 1.1.1.1  2002/02/01 22:22:09  peiyongz
 * sane_include
 *
 * Revision 1.3  2000/03/02 19:54:38  roddey
 * This checkin includes many changes done while waiting for the
 * 1.1.0 code to be finished. I can't list them all here, but a list is
 * available elsewhere.
 *
 * Revision 1.2  2000/02/06 07:48:01  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.1.1.1  1999/11/09 01:03:59  twl
 * Initial checkin
 *
 * Revision 1.4  1999/11/08 20:56:55  droddey
 * If the main xml entity does not exist, we need to get the error handling for that
 * inside the main XMLScanner::scanDocument() try block so that it gets reported
 * in the normal way. We have to add a little extra safety code because, when this
 * happens, there is no reader on the reader stack to get position ino from.
 *
 * Revision 1.3  1999/11/08 20:45:03  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/util/BinFileInputStream.hpp>
#include <xercesc/util/Janitor.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLExceptMsgs.hpp>
#include <xercesc/util/XMLString.hpp>

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  BinFileInputStream: Constructors and Destructor
// ---------------------------------------------------------------------------
BinFileInputStream::BinFileInputStream(const XMLCh* const fileName
                                       , MemoryManager* const manager) :

    fSource(0)
  , fMemoryManager(manager)
{
    // Try to open the file
    fSource = XMLPlatformUtils::openFile(fileName, manager);
}

BinFileInputStream::BinFileInputStream(const char* const fileName,
                                       MemoryManager* const manager) :

    fSource(0)
  , fMemoryManager(manager)
{
    // Transcode the file name and put a janitor on the temp buffer
    XMLCh* realName = XMLString::transcode(fileName, manager);
    ArrayJanitor<XMLCh> janName(realName, manager);

    // Try to open the file
    fSource = XMLPlatformUtils::openFile(realName, manager);
}

BinFileInputStream::BinFileInputStream(const FileHandle toAdopt
                                       , MemoryManager* const manager) :

    fSource(toAdopt)
  , fMemoryManager(manager)
{
}

BinFileInputStream::~BinFileInputStream()
{
    if (fSource)
        XMLPlatformUtils::closeFile(fSource, fMemoryManager);
}


// ---------------------------------------------------------------------------
//  BinFileInputStream: Getter methods
// ---------------------------------------------------------------------------
unsigned int BinFileInputStream::getSize() const
{
    return XMLPlatformUtils::fileSize(fSource, fMemoryManager);
}


// ---------------------------------------------------------------------------
//  BinFileInputStream: Stream management methods
// ---------------------------------------------------------------------------
void BinFileInputStream::reset()
{
    XMLPlatformUtils::resetFile(fSource, fMemoryManager);
}


// ---------------------------------------------------------------------------
//  BinFileInputStream: Implementation of the input stream interface
// ---------------------------------------------------------------------------
unsigned int BinFileInputStream::curPos() const
{
    return XMLPlatformUtils::curFilePos(fSource, fMemoryManager);
}

unsigned int
BinFileInputStream::readBytes(          XMLByte* const  toFill
                                , const unsigned int    maxToRead)
{
    //
    //  Read up to the maximum bytes requested. We return the number
    //  actually read.
    //
    return XMLPlatformUtils::readFileBuffer(fSource, maxToRead, toFill, fMemoryManager);
}

XERCES_CPP_NAMESPACE_END
