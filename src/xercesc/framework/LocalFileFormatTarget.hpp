/*
 * Copyright 2002,2004 The Apache Software Foundation.
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
 * $Id$
 * $Log$
 * Revision 1.8  2004/11/02 17:09:42  peiyongz
 * Handling OutOfMemory exception
 *
 * Revision 1.7  2004/09/08 13:55:57  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.6  2003/05/16 21:36:55  knoaman
 * Memory manager implementation: Modify constructors to pass in the memory manager.
 *
 * Revision 1.5  2003/05/15 18:26:07  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.4  2003/01/24 20:20:22  tng
 * Add method flush to XMLFormatTarget
 *
 * Revision 1.3  2002/11/27 18:09:25  tng
 * [Bug 13447] Performance: Using LocalFileFormatTarget with DOMWriter is very slow.
 *
 * Revision 1.2  2002/11/04 15:00:21  tng
 * C++ Namespace Support.
 *
 * Revision 1.1  2002/06/19 21:59:26  peiyongz
 * DOM3:DOMSave Interface support: LocalFileFormatTarget
 *
 *
 */

#ifndef LocalFileFormatTarget_HEADER_GUARD_
#define LocalFileFormatTarget_HEADER_GUARD_

#include <xercesc/framework/XMLFormatter.hpp>

XERCES_CPP_NAMESPACE_BEGIN

class XMLPARSER_EXPORT LocalFileFormatTarget : public XMLFormatTarget {
public:

    /** @name constructors and destructor */
    //@{
    LocalFileFormatTarget
    (
        const XMLCh* const
        , MemoryManager* const  manager = XMLPlatformUtils::fgMemoryManager
    );

    LocalFileFormatTarget
    (
        const char* const
        , MemoryManager* const  manager = XMLPlatformUtils::fgMemoryManager
    );

    ~LocalFileFormatTarget();
    //@}

    // -----------------------------------------------------------------------
    //  Implementations of the format target interface
    // -----------------------------------------------------------------------
    virtual void writeChars(const XMLByte* const toWrite
                          , const unsigned int   count
                          , XMLFormatter* const  formatter);

    virtual void flush();

private:
    // -----------------------------------------------------------------------
    //  Unimplemented methods.
    // -----------------------------------------------------------------------
    LocalFileFormatTarget(const LocalFileFormatTarget&);
    LocalFileFormatTarget& operator=(const LocalFileFormatTarget&);

    // -----------------------------------------------------------------------
    //  Private helpers
    // -----------------------------------------------------------------------
    void flushBuffer();
    bool insureCapacity(const unsigned int extraNeeded);

    // -----------------------------------------------------------------------
    //  Private data members
    //
    //  fSource
    //      The source file that we represent. The FileHandle type is defined
    //      per platform.
    //
    //  fDataBuf
    //      The pointer to the buffer data. Its always
    //      one larger than fCapacity, to leave room for the null terminator.
    //
    //  fIndex
    //      The current index into the buffer, as characters are appended
    //      to it. If its zero, then the buffer is empty.
    //
    //  fCapacity
    //      The current capacity of the buffer. Its actually always one
    //      larger, to leave room for the null terminator.
    // -----------------------------------------------------------------------
    FileHandle      fSource;
    XMLByte*        fDataBuf;
    unsigned int    fIndex;
    unsigned int    fCapacity;
    MemoryManager*  fMemoryManager;
};


XERCES_CPP_NAMESPACE_END

#endif

