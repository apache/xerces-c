/*
 * Copyright 1999-2004 The Apache Software Foundation.
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
 * Revision 1.8  2004/09/29 18:59:18  peiyongz
 * [jira1207] --patch from Dan Rosen
 *
 * Revision 1.7  2004/09/08 13:55:58  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.6  2004/01/29 11:46:29  cargilld
 * Code cleanup changes to get rid of various compiler diagnostic messages.
 *
 * Revision 1.5  2003/05/16 21:36:55  knoaman
 * Memory manager implementation: Modify constructors to pass in the memory manager.
 *
 * Revision 1.4  2003/05/15 18:26:07  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.3  2002/11/04 15:00:21  tng
 * C++ Namespace Support.
 *
 * Revision 1.2  2002/08/21 18:54:52  tng
 * [Bug 11869] Add the const modifier (XMLBuffer.hpp).
 *
 * Revision 1.1.1.1  2002/02/01 22:21:51  peiyongz
 * sane_include
 *
 * Revision 1.6  2001/06/27 20:29:09  tng
 * [Bug 2365] Huge performance problem with the parser in XMLScanner::sendCharData() .  By David Bertoni.
 *
 * Revision 1.5  2000/03/02 19:54:24  roddey
 * This checkin includes many changes done while waiting for the
 * 1.1.0 code to be finished. I can't list them all here, but a list is
 * available elsewhere.
 *
 * Revision 1.4  2000/02/24 20:00:22  abagchi
 * Swat for removing Log from API docs
 *
 * Revision 1.3  2000/02/15 01:21:30  roddey
 * Some initial documentation improvements. More to come...
 *
 * Revision 1.2  2000/02/06 07:47:47  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.1.1.1  1999/11/09 01:08:29  twl
 * Initial checkin
 *
 * Revision 1.2  1999/11/08 20:44:36  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */


#if !defined(XMLBUFFER_HPP)
#define XMLBUFFER_HPP

#include <xercesc/util/XMemory.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/framework/MemoryManager.hpp>

XERCES_CPP_NAMESPACE_BEGIN

class XMLBufferFullHandler;

/**
 *  XMLBuffer is a lightweight, expandable Unicode text buffer. Since XML is
 *  inherently theoretically unbounded in terms of the sizes of things, we
 *  very often need to have expandable buffers. The primary concern here is
 *  that appends of characters and other buffers or strings be very fast, so
 *  it always maintains the current buffer size.
 *
 *  The buffer is not nul terminated until some asks to see the raw buffer
 *  contents. This also avoids overhead during append operations.
 */
 class XMLPARSER_EXPORT XMLBuffer : public XMemory
{
public :
    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------

    /** @name Constructor */
    //@{
    XMLBuffer(const unsigned int capacity = 1023
              , MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager) :

        fUsed(false)
        , fIndex(0)
        , fCapacity(capacity)
        , fMemoryManager(manager)
        , fFullHandler(0)
        , fFullSize(0)
        , fBuffer(0)
    {
        // Buffer is one larger than capacity, to allow for zero term
        fBuffer = (XMLCh*) manager->allocate((capacity+1) * sizeof(XMLCh)); //new XMLCh[fCapacity+1];

        // Keep it null terminated
        fBuffer[0] = XMLCh(0);
    }
    //@}

    /** @name Destructor */
    //@{
    ~XMLBuffer()
    {
        fMemoryManager->deallocate(fBuffer); //delete [] fBuffer;
    }
    //@}

    // -----------------------------------------------------------------------
    //  Buffer Full Handler Management
    // -----------------------------------------------------------------------
    void setFullHandler(XMLBufferFullHandler* handler, const unsigned int fullSize)
    {
        fFullHandler = handler;
        fFullSize = fullSize;
    }

    // -----------------------------------------------------------------------
    //  Buffer Management
    // -----------------------------------------------------------------------
    void append(const XMLCh toAppend)
    {
        // Put in char and bump the index
        if (fIndex == fCapacity)
            insureCapacity(1);
        fBuffer[fIndex++] = toAppend;
    }

    void append (const XMLCh* const chars, const unsigned int count = 0);

    void set (const XMLCh* const chars, const unsigned int count = 0)
    {
        fIndex = 0;
        append(chars, count);
    }

    const XMLCh* getRawBuffer() const
    {
        fBuffer[fIndex] = 0;
        return fBuffer;
    }

    XMLCh* getRawBuffer()
    {
        fBuffer[fIndex] = 0;
        return fBuffer;
    }

    void reset()
    {
        fIndex = 0;
        fBuffer[0] = 0;
    }

    // -----------------------------------------------------------------------
    //  Getters
    // -----------------------------------------------------------------------
    bool getInUse() const
    {
        return fUsed;
    }

    unsigned int getLen() const
    {
        return fIndex;
    }

    bool isEmpty() const
    {
        return (fIndex == 0);
    }

    // -----------------------------------------------------------------------
    //  Setters
    // -----------------------------------------------------------------------
    void setInUse(const bool newValue)
    {
        fUsed = newValue;
    }

private :
    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    XMLBuffer(const XMLBuffer&);
    XMLBuffer& operator=(const XMLBuffer&);

    // -----------------------------------------------------------------------
    //  Declare our friends
    // -----------------------------------------------------------------------
    friend class XMLBufBid;

    // -----------------------------------------------------------------------
    //  Private helpers
    // -----------------------------------------------------------------------
    void insureCapacity(const unsigned int extraNeeded);


    // -----------------------------------------------------------------------
    //  Private data members
    //
    //  fBuffer
    //      The pointer to the buffer data. Its grown as needed. Its always
    //      one larger than fCapacity, to leave room for the null terminator.
    //
    //  fIndex
    //      The current index into the buffer, as characters are appended
    //      to it. If its zero, then the buffer is empty.
    //
    //  fCapacity
    //      The current capacity of the buffer. Its actually always one
    //      larger, to leave room for the null terminator.
    //
    //  fUsed
    //      Indicates whether this buffer is in use or not.
    //
    //  fFullHandler, fFullSize
    //      If fFullHandler is non-null, the buffer has a maximum size
    //      indicated by fFullSize. If writing to the buffer would exceed the
    //      buffer's maximum size, fFullHandler's bufferFull callback is
    //      invoked, to empty the buffer.
    // -----------------------------------------------------------------------
    bool            fUsed;
    unsigned int    fIndex;
    unsigned int    fCapacity;
    MemoryManager* const        fMemoryManager;
    XMLBufferFullHandler*       fFullHandler;
    unsigned int                fFullSize;
    XMLCh*          fBuffer;
};

/**
 *  XMLBufferFullHandler is a callback interface for clients of
 *  XMLBuffers that impose a size restriction (e.g. XMLScanner).
 *  Note that this is intended solely as a mix-in for internal
 *  use, and therefore does not derive from XMemory (to avoid
 *  the ambiguous base class problem).
 */
class XMLPARSER_EXPORT XMLBufferFullHandler
{
public :

    /**
     * Callback method, intended to allow clients of an XMLBuffer which has
     * become full to empty it appropriately.
     * @return true if the handler was able to empty the buffer (either
     * partially or completely), otherwise false to indicate an error.
     */
    virtual bool bufferFull(XMLBuffer&) = 0;

};

XERCES_CPP_NAMESPACE_END

#endif
