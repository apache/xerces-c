/*
 * Copyright 2003,2004 The Apache Software Foundation.
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
 * Revision 1.6  2005/04/27 18:21:51  cargilld
 * Fix for problem on Solaris where open may return 0 as a valid FileHandle.  Check for -1 instead.
 *
 * Revision 1.5  2004/09/08 13:56:13  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.4  2004/02/16 04:02:34  neilg
 * fix for bug 26936
 *
 * Revision 1.3  2004/01/29 11:46:30  cargilld
 * Code cleanup changes to get rid of various compiler diagnostic messages.
 *
 * Revision 1.2  2003/12/17 13:58:02  cargilld
 * Platform update for memory management so that the static memory manager (one
 * used to call Initialize) is only for static data.
 *
 * Revision 1.1  2003/09/18 18:39:12  peiyongz
 * Binary File Output Stream:
 *
 * $Id$
 */

#if !defined(BINFILEOUTPUTSTREAM_HPP)
#define BINFILEOUTPUTSTREAM_HPP

#include <xercesc/framework/BinOutputStream.hpp>
#include <xercesc/util/PlatformUtils.hpp>

XERCES_CPP_NAMESPACE_BEGIN

class XMLUTIL_EXPORT BinFileOutputStream : public BinOutputStream
{
public :
    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------

    ~BinFileOutputStream();

    BinFileOutputStream
    (
        const   XMLCh* const    fileName
        , MemoryManager* const  manager = XMLPlatformUtils::fgMemoryManager
    );

    BinFileOutputStream
    (
         const   char* const     fileName
       , MemoryManager* const    manager = XMLPlatformUtils::fgMemoryManager
    );

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

    virtual void writeBytes
    (
          const XMLByte* const      toGo
        , const unsigned int        maxToWrite
    );


private :
    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    BinFileOutputStream(const BinFileOutputStream&);
    BinFileOutputStream& operator=(const BinFileOutputStream&); 

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
//  BinFileOutputStream: Getter methods
// ---------------------------------------------------------------------------
inline bool BinFileOutputStream::getIsOpen() const
{
    return (fSource != (FileHandle) XERCES_Invalid_File_Handle);
}

XERCES_CPP_NAMESPACE_END

#endif
