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
 * Revision 1.8  2005/04/27 18:21:51  cargilld
 * Fix for problem on Solaris where open may return 0 as a valid FileHandle.  Check for -1 instead.
 *
 * Revision 1.7  2004/09/08 13:56:21  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.6  2004/01/29 11:48:46  cargilld
 * Code cleanup changes to get rid of various compiler diagnostic messages.
 *
 * Revision 1.5  2003/12/17 13:58:02  cargilld
 * Platform update for memory management so that the static memory manager (one
 * used to call Initialize) is only for static data.
 *
 * Revision 1.4  2003/06/03 18:12:29  knoaman
 * Add default value for memory manager argument.
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
 * Revision 1.3  2000/02/24 20:05:23  abagchi
 * Swat for removing Log from API docs
 *
 * Revision 1.2  2000/02/06 07:48:01  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.1.1.1  1999/11/09 01:04:00  twl
 * Initial checkin
 *
 * Revision 1.3  1999/11/08 20:45:03  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */

#if !defined(BINFILEINPUTSTREAM_HPP)
#define BINFILEINPUTSTREAM_HPP

#include <xercesc/util/BinInputStream.hpp>
#include <xercesc/util/PlatformUtils.hpp>

XERCES_CPP_NAMESPACE_BEGIN

class XMLUTIL_EXPORT BinFileInputStream : public BinInputStream
{
public :
    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    BinFileInputStream
    (
        const   XMLCh* const    fileName
        , MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager
    );

    BinFileInputStream
    (
        const   char* const     fileName
        , MemoryManager* const  manager = XMLPlatformUtils::fgMemoryManager
    );

    BinFileInputStream
    (
        const   FileHandle      toUse
        , MemoryManager* const  manager = XMLPlatformUtils::fgMemoryManager
    );

    virtual ~BinFileInputStream();


    // -----------------------------------------------------------------------
    //  Getter methods
    // -----------------------------------------------------------------------
    bool getIsOpen() const;
    unsigned int getSize() const;
    void reset();


    // -----------------------------------------------------------------------
    //  Implementation of the input stream interface
    // -----------------------------------------------------------------------
    virtual unsigned int curPos() const;

    virtual unsigned int readBytes
    (
                XMLByte* const      toFill
        , const unsigned int        maxToRead
    );


private :
    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    BinFileInputStream(const BinFileInputStream&);
    BinFileInputStream& operator=(const BinFileInputStream&);   

    // -----------------------------------------------------------------------
    //  Private data members
    //
    //  fSource
    //      The source file that we represent. The FileHandle type is defined
    //      per platform.
    // -----------------------------------------------------------------------
    FileHandle              fSource;
    MemoryManager* const    fMemoryManager;
};


// ---------------------------------------------------------------------------
//  BinFileInputStream: Getter methods
// ---------------------------------------------------------------------------
inline bool BinFileInputStream::getIsOpen() const
{
    return (fSource != (FileHandle) XERCES_Invalid_File_Handle);
}

XERCES_CPP_NAMESPACE_END

#endif
