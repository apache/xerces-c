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
 * Revision 1.10  2000/02/04 00:52:57  rahulj
 * Changed size_t to int.
 *
 * Revision 1.9  2000/02/03 23:07:27  andyh
 * Add several new functions from Robert Weir to DOMString.
 *
 * Revision 1.8  2000/01/29 00:39:08  andyh
 * Redo synchronization in DOMStringHandle allocator.  There
 * was a bug in the use of Compare and Swap.  Switched to mutexes.
 *
 * Changed a few plain deletes to delete [].
 *
 * Revision 1.7  2000/01/18 19:55:37  andyh
 * Remove dependencies on XMLStdout and err, as these are about
 * to stop working.
 *
 * Revision 1.6  2000/01/05 22:16:26  robweir
 * Move DOMString implementation class declarations into a new
 * file: DOMStringImpl.hpp.  Include this header in DOMString.hpp
 * for XML_DEBUG builds so the underlying character array will be
 * visible in the debugger.  <robert_weir@lotus.com>
 *
 * Revision 1.5  1999/12/17 02:09:41  andyh
 * Fix bug in DOMString::insertData() that occured if the source
 * and destination strings were the same and the orginal buffer had
 * enough space to hold the result.
 *
 * Revision 1.4  1999/12/15 19:44:46  roddey
 * Changed to use new LCP transcoder scheme.
 *
 * Revision 1.3  1999/12/03 00:11:22  andyh
 * Added DOMString.clone() to node parameters in and out of the DOM,
 * where they had been missed.
 *
 * DOMString::rawBuffer, removed incorrect assumptions about it
 * being null terminated.
 *
 * Revision 1.2  1999/11/30 21:16:25  roddey
 * Changes to add the transcode() method to DOMString, which returns a transcoded
 * version (to local code page) of the DOM string contents. And I changed all of the
 * exception 'throw by pointer' to 'throw by value' style.
 *
 * Revision 1.1.1.1  1999/11/09 01:08:47  twl
 * Initial checkin
 *
 * Revision 1.3  1999/11/08 20:44:12  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */

#include <stdio.h>
#include <util/PlatformUtils.hpp>
#include <util/RuntimeException.hpp>
#include <util/TransService.hpp>
#include <util/XMLString.hpp>
#include "DOMString.hpp"

#ifndef XML_DEBUG
#include "DOMStringImpl.hpp"
#endif

#include <assert.h>
#include <string.h>

 

//----------------------------------------------
//
//      DOMStringData
//
//----------------------------------------------

void DOMStringData::removeRef()
{
    int result = XMLPlatformUtils::atomicDecrement(fRefCount);
    if (result==0)
    {
        fBufferLength = 0xcccc;
        fRefCount     = 0xcccc;
        delete this;
        XMLPlatformUtils::atomicDecrement(DOMString::gLiveStringDataCount);
    };
};


void DOMStringData::addRef()
{
    XMLPlatformUtils::atomicIncrement(fRefCount);
};


DOMStringData *DOMStringData::allocateBuffer(int length)
{
    int sizeToAllocate = sizeof(DOMStringData) //  buffer will contain an
        + length*sizeof(XMLCh);                //  extra elem because of stub
                                               //  array in DOMStringData struct.
    DOMStringData *buf = (DOMStringData *) new char[sizeToAllocate];
    XMLPlatformUtils::atomicIncrement(DOMString::gLiveStringDataCount);
    XMLPlatformUtils::atomicIncrement(DOMString::gTotalStringDataCount);
    buf->fBufferLength = length;
    buf->fRefCount = 1;
    buf->fData[0] = 0;
    return buf;
}




//----------------------------------------------------
//
//      DOMStringHandle
//
//-----------------------------------------------------



//
//  Specialized new and delete operators for DOMStringHandles.
//      These are used, rather than the standard system operator new,
//      for improved performance.
//
//      We allocate largish blocks of memory using the standard system
//      new function, and sub-allocate string handles from that block.
//      Un-allocated string handles within the allocated blocks are kept
//      in a singly linked list, making allocation and deallocation
//      very quick in the common case.
//
//      String handle allocation is thread safe.  A multi-threaded
//      application may have threads concurrently accessing multiple
//      DOM documents; since all string handles come from the same pool,
//      this allocator must be safe.  The compare and exchange function,
//      which is available as a single instruction in most processor
//      architectures, and typically surfaced as an OS function,
//      is used to safely update the string handle free list.
//
void *DOMStringHandle::freeListPtr = 0;   // Point to the head of the
                                          //  free list of un-allocated
                                          //  string handles, or 0 if there
                                          //  are no free string handles.

static const int allocGroupSize = 1024;   // Number of string handles to allocate
                                          //  as a chunk from the system's
                                          //  memory allocator.


//
//  There is one global mutex that is used to synchronize access to the
//     allocator free list for DOMStringHandles.  This function gets that
//     mutex, and will create it on the first attempt to get it.
//
XMLMutex& DOMStringHandle::getMutex()
{
    static XMLMutex* DOMStringHandleMutex = 0;
    if (!DOMStringHandleMutex)
    {
        XMLMutex* tmpMutex = new XMLMutex;
        if (XMLPlatformUtils::compareAndSwap((void**)&DOMStringHandleMutex, tmpMutex, 0))
        {
            // Someone beat us to it, so let's clean up ours
            delete tmpMutex;
        }
    }
    return *DOMStringHandleMutex;
}


//
//  Operator new for DOMStringHandles.  Called implicitly from the
//          DOMStringHandle constructor.
//
void *DOMStringHandle::operator new(size_t sizeToAlloc)
{
    assert(sizeToAlloc == sizeof(DOMStringHandle));
    void    *retPtr;
    XMLMutexLock lock(&getMutex());    // Lock the DOMStringHandle mutex for
                                       //  the duration of this function.
    
    if (freeListPtr == 0) 
    {
        // Uncommon case.  The free list of string handles is empty
        // Allocate a new batch of them, using the system's 
        // operator new to get a chunk of memory.
        //
        // Link all of these new StringHandles into our free list
        //
        DOMStringHandle *dsg = 
            ::new DOMStringHandle[allocGroupSize];
        int   i;
        for (i=0; i<allocGroupSize-1; i++) {
            *(void **)&dsg[i] = freeListPtr;
            freeListPtr = &dsg[i];
        }
    }
    
    retPtr = freeListPtr;
    freeListPtr = *(void **)freeListPtr;
    
    return retPtr;
};


//
//  Operator delete for DOMStringHandles.  Called implicitly from the 
//              Destructor for DOMStringHandle.
//
void DOMStringHandle::operator delete(void *pMem)
{
    XMLMutexLock   lock(&getMutex());    // Lock the DOMStringHandle mutex for the
                                         //    duration of this function.
   *(void **)pMem = freeListPtr;
    freeListPtr = pMem;
};
    

void DOMStringHandle::addRef()
{
    XMLPlatformUtils::atomicIncrement(fRefCount);
};


void DOMStringHandle::removeRef()
{
    int result = XMLPlatformUtils::atomicDecrement(fRefCount);
    if (result==0)
    {
        fDSData->removeRef();
        delete this;
        XMLPlatformUtils::atomicDecrement(DOMString::gLiveStringHandleCount);
    };
};


DOMStringHandle *DOMStringHandle::createNewStringHandle(int bufLength)
{
    DOMStringHandle  *h = new DOMStringHandle;
    XMLPlatformUtils::atomicIncrement(DOMString::gLiveStringHandleCount);
    XMLPlatformUtils::atomicIncrement(DOMString::gTotalStringHandleCount);
    h -> fLength = 0;
    h -> fRefCount = 1;
    h -> fDSData = DOMStringData::allocateBuffer(bufLength);
    return h;
};


DOMStringHandle *DOMStringHandle::cloneStringHandle()
{
    DOMStringHandle *h = new DOMStringHandle;
    XMLPlatformUtils::atomicIncrement(DOMString::gLiveStringHandleCount);
    h->fLength   = fLength;
    h->fRefCount = 1;
    h->fDSData   = fDSData;
    h->fDSData->addRef();
    return h;
}

//------------------------------------------------------------
//
//      DOMString
//
//------------------------------------------------------------


int DOMString::gLiveStringDataCount    = 0;
int DOMString::gTotalStringDataCount   = 0;
int DOMString::gLiveStringHandleCount  = 0;
int DOMString::gTotalStringHandleCount = 0;


DOMString::DOMString()
{
    fHandle = 0;
};


DOMString::DOMString(const DOMString &other)
{
    fHandle = other.fHandle;
    if (fHandle)
        fHandle->addRef();
};


DOMString::DOMString(const XMLCh *data)
{
    fHandle = 0;
    if (data != 0)
    {
        int dataLength = 0;
        while (data[dataLength] != 0)
            ++dataLength;
                
        if (dataLength != 0)
        {
            fHandle = DOMStringHandle::createNewStringHandle(dataLength+1);
            fHandle->fLength = dataLength;
            XMLCh *strData = fHandle->fDSData->fData;
            int i;
            for (i=0; i<dataLength ; ++i)
                strData[i] = data[i];

            strData[dataLength] = 0;
        }
    }
}



DOMString::DOMString(const XMLCh *data, int dataLength)
{
    fHandle = 0;
    if (data != 0)
    {
        if (dataLength > 0)
        {
            fHandle = DOMStringHandle::createNewStringHandle(dataLength+1);
            fHandle->fLength = dataLength;
            XMLCh *strData = fHandle->fDSData->fData;
            int i;
            for (i=0; i<dataLength ; ++i)
                strData[i] = data[i];

            strData[dataLength] = 0;
        }
    }
}




//  getDOMConverter - get the converter from the system default
//          codepage to Unicode that is to be used when
//          a DOMString is constructed from a char *.
//
XMLLCPTranscoder*  getDomConverter()
{
    static XMLLCPTranscoder* gDomConverter = 0;
    if (!gDomConverter) 
    {
        XMLLCPTranscoder* transcoder =
        XMLPlatformUtils::fgTransService->makeNewLCPTranscoder();
        if (!transcoder)
            XMLPlatformUtils::panic(XMLPlatformUtils::Panic_NoDefTranscoder
            );

        if (XMLPlatformUtils::compareAndSwap((void **)&gDomConverter,
        transcoder, 0) != 0)
            delete gDomConverter;
    }
    return gDomConverter;
};



//
//  Create a DOMString from a char * string in the default code page
//                     of the system on which we are executing.
// 
//
DOMString::DOMString(const char *srcString)
{
    fHandle = 0;
    if (srcString != 0)
    {
        XMLLCPTranscoder*  uniConverter = getDomConverter();
        unsigned int    len = 0;

        unsigned int srcLen = strlen(srcString);
        if (srcLen == 0)
            return;

        const unsigned int charsNeeded =
            uniConverter->calcRequiredSize(srcString);

        fHandle = DOMStringHandle::createNewStringHandle(charsNeeded + 1);
        fHandle->fLength = charsNeeded;
        XMLCh *strData = fHandle->fDSData->fData;
        if (!uniConverter->transcode(srcString, strData, charsNeeded))
        {
            // <TBD> We should throw something here?
        }
    }
};



DOMString::DOMString(int nullValue)
{
   assert(nullValue == 0);
   fHandle = 0;
};


DOMString::~DOMString()
{
    if (fHandle)
        fHandle->removeRef();

    fHandle = 0;
};


DOMString & DOMString::operator =(const DOMString &other)
{
    if (this == &other)
        return *this;

    if (fHandle)
        fHandle->removeRef();

    fHandle = other.fHandle;
    
    if (fHandle)
        fHandle->addRef();

    return *this;
};


DOMString & DOMString::operator = (DOM_NullPtr *arg)
{
    assert(arg == 0);
    if (fHandle)
        fHandle->removeRef();

    fHandle = 0;
    return *this;
};


#if 0
DOMString DOMString::operator + (const DOMString &other)
{
    DOMString retString = this->clone();
    retString.appendData(other);
    return retString;
};
#endif

bool DOMString::operator ==(const DOMString &other) const
{
    return this->fHandle == other.fHandle;
};


bool DOMString::operator !=(const DOMString &other) const
{
    return this->fHandle != other.fHandle;
};


bool DOMString::operator == (const DOM_NullPtr *p) const
{
    return (fHandle == 0);
};

bool DOMString::operator != (const DOM_NullPtr *p) const
{
    return (fHandle != 0);
};



void DOMString::reserve(int size)
{
	if (fHandle == 0)
	{
	    if (size > 0)
	        fHandle = DOMStringHandle::createNewStringHandle(size);
	}
}



void DOMString::appendData(const DOMString &other)
{
    int i;
    if (other.fHandle == 0 || other.fHandle->fLength == 0)
        return;

    // If this string is empty and this string does not have an
    //   already allocated buffer sufficient to hold the string being
    //   appended, return a clone of the other string.
    //
    if (fHandle == 0 || (fHandle->fLength == 0 &&
        fHandle->fDSData->fBufferLength < other.fHandle->fLength))
    {
        if (fHandle) fHandle->removeRef();
        this->fHandle = other.fHandle->cloneStringHandle();
        return;
    }

    int newLength = fHandle->fLength + other.fHandle->fLength;
    if (newLength >= fHandle->fDSData->fBufferLength ||
        fHandle->fDSData->fRefCount > 1)
    {
        // We can't stick the data to be added onto the end of the
        //  existing string, either because there is not space in
        //  the buffer, or because the buffer is being shared with
        //  some other string.  So, make a new buffer.
        DOMStringData *newBuf = DOMStringData::allocateBuffer(newLength);
        XMLCh *newP = newBuf->fData;
        XMLCh *oldP = fHandle->fDSData->fData;
        for (i=0; i<fHandle->fLength; ++i)
            newP[i] = oldP[i];
        
        fHandle->fDSData->removeRef();
        fHandle->fDSData = newBuf;
    }

    //
    // This string now had enough buffer room to hold the data to
    //  be appended.  Go ahead and copy it in.
    XMLCh *srcP = other.fHandle->fDSData->fData;
    XMLCh *destP = &fHandle->fDSData->fData[fHandle->fLength];
    for (i=0; i<=other.fHandle->fLength; i++)
        destP[i] = srcP[i];

    fHandle->fLength += other.fHandle->fLength;
}



void DOMString::appendData(XMLCh ch)
{
	int newLength = 0;

	if (fHandle == 0)
	{
		fHandle = DOMStringHandle::createNewStringHandle(1);
		newLength = 1;
	}
	else
		newLength = fHandle->fLength + 1;
    
    if (newLength >= fHandle->fDSData->fBufferLength ||
        fHandle->fDSData->fRefCount > 1)
    {
        // We can't stick the data to be added onto the end of the
        //  existing string, either because there is not space in
        //  the buffer, or because the buffer is being shared with
        //  some other string.  So, make a new buffer.
        DOMStringData *newBuf = DOMStringData::allocateBuffer(newLength);
        XMLCh *newP = newBuf->fData;
        XMLCh *oldP = fHandle->fDSData->fData;
        for (int i=0; i<fHandle->fLength; ++i)
            newP[i] = oldP[i];
        
        fHandle->fDSData->removeRef();
        fHandle->fDSData = newBuf;
    }

    XMLCh *destP = &fHandle->fDSData->fData[fHandle->fLength];
	destP[0] = ch;

    fHandle->fLength ++;
}

// TODO: A custom version could be written more efficiently, avoiding
// the creation of the temporary DOMString
void DOMString::appendData(const XMLCh* other)
{
	appendData(DOMString(other));
}


DOMString& DOMString::operator +=(const DOMString &other)
{
	appendData(other);

	return *this;
}

DOMString& DOMString::operator +=(const XMLCh *str)
{
	appendData(str);

	return *this;
}

DOMString& DOMString::operator +=(XMLCh ch)
{
	appendData(ch);

	return *this;
}



XMLCh     DOMString::charAt(int index) const
{
    XMLCh retCh = 0;
    if (fHandle != 0  && index >= 0 &&  index < fHandle->fLength)
        retCh = fHandle->fDSData->fData[index];
    return retCh;
};


DOMString DOMString::clone() const
{
    DOMString retString;

    if (fHandle != 0)
        retString.fHandle = this->fHandle->cloneStringHandle();

    return retString;
};



void DOMString::deleteData(int offset, int delLength)
{
    assert(delLength >= 0);
    int stringLen = this->length();
    assert(offset < stringLen);

    if (delLength == 0)
        return;


    if (offset + delLength >= stringLen)
        delLength = stringLen - offset;

    int newStringLength = stringLen - delLength;
    if (fHandle->fDSData->fRefCount > 1 && offset+delLength < stringLen)
    {
        // The deletion is of a range in the middle of the string
        //  and there's another string handle using the buffer so
        //  we need to make a new buffer before moving characters
        //  around.
        DOMStringData *newBuf = DOMStringData::allocateBuffer(newStringLength);
        XMLCh *newP = newBuf->fData;
        XMLCh *oldP = fHandle->fDSData->fData;
        int i;
        for (i=0; i<offset; i++)
            newP[i] = oldP[i];

        for (i=offset; i<newStringLength; i++)
            newP[i] = oldP[i+delLength];

        fHandle->fLength = newStringLength;
        fHandle->fDSData->removeRef();
        fHandle->fDSData = newBuf;
    }
    else if (offset+delLength < stringLen)
    {
        // The deletion is of a range in the middle of the string,
        // but no other string is sharing the buffer, so we can
        // just delete in place.
        int i;
        XMLCh *bufP =  fHandle->fDSData->fData;
        for (i=offset; i<newStringLength; i++)
            bufP[i] = bufP[i+delLength];

        fHandle->fLength = newStringLength;
    }
    else 
    {
        // The deletion continues to the end of the string.
        // Simply reset the length.  We don't need to worry
        // about other strings sharing the buffer because
        // no characters are moved.
        fHandle->fLength = newStringLength;
    }
};



bool DOMString::equals(const DOMString &other) const
{
        bool retVal = true;
    if (this->fHandle != 0  && other.fHandle != 0)
    {
        if (this->fHandle->fLength != other.fHandle->fLength)
        {
            retVal =  false;
        }
        else
        {
            XMLCh *thisP  = this->fHandle->fDSData->fData;
            XMLCh *otherP = other.fHandle->fDSData->fData;
            int i;
            for (i=0; i<this->fHandle->fLength; i++)
            {
                if (thisP[i] != otherP[i])
                {
                    retVal = false;
                    break;
                }
            }
        }
    }
    else
    {
        // At this point, one or more of the fHandle
        //  pointers is known to be zero.
        if (fHandle       && fHandle->fLength != 0  ||
            other.fHandle && other.fHandle->fLength != 0)
            retVal = false;

    }
    return retVal;
};



bool DOMString::equals(const XMLCh *other) const
{
    if (this->fHandle != 0  && other != 0)
    {
        // Both strings have non-null data pointers, so
        //  we can go ahead and actually compare them.
        XMLCh *thisP  = this->fHandle->fDSData->fData;
        int    len    = this->fHandle->fLength;

        int i;
        for (i=0; i<len; i++)
        {
            if (other[i] == 0)   // "other" is null terminated.
                return false;    //   (If there were no chance of a DOM
                                 //   string having a 0 char in the middle of
                                 //   it, this test could be omitted.)

            if (thisP[i] != other[i])
                return false;
        }

        if (other[len] != 0)     // This test for the end of the other
            return false;        //  string can't be done without first
                                 //  checking that we haven't walked off the
                                 //  end.  (It has actually happened - off end
                                 //  of string, page, and valid memory.)

        return true;
    }


    // At this point, we know that at least one of the strings had a null
    //  data pointer.
    if (fHandle  && fHandle->fLength != 0)
        return false; 
    
    if (other && *other != 0)
        return false;
        
    return true;  // Both strings are empty.  DOMString treats zero-length
                  //   and a null data pointer as equivalent.
};


void DOMString::insertData(int offset, const DOMString &src)
{
    assert(offset>=0);
    int origStrLength = this->length();
    assert(offset<=origStrLength);

    if (fHandle == 0)
    {
        *this = src.clone();
        return;
    }

    if (src.fHandle == 0 ||src.fHandle->fLength == 0)
        return;

    XMLCh *srcP = src.fHandle->fDSData->fData;
    int srcLength = src.fHandle->fLength;
    int newLength = fHandle->fLength + srcLength;
    if (newLength >= fHandle->fDSData->fBufferLength ||
        fHandle->fDSData->fRefCount > 1  || fHandle == src.fHandle )
    {
        // We can't stick the data to be added into the
        //  existing string, either because there is not space in
        //  the buffer, or because the buffer is being shared with
        //  some other string.
        //  So, make a new buffer.

        DOMStringData *newBuf = DOMStringData::allocateBuffer(newLength);
        XMLCh *newP  = newBuf->fData;
        XMLCh *oldP   = fHandle->fDSData->fData;
        int i;
        for (i=0; i<offset; ++i)
            newP[i] = oldP[i];

        for (i=0; i<srcLength; i++)
            newP[i+offset] = srcP[i];
        
        for (i=offset; i<origStrLength; i++)
            newP[i+srcLength] = oldP[i];

        fHandle->fDSData->removeRef();
        fHandle->fDSData = newBuf;
    }
    else
    {
        // There is room in the already-existing buffer to hold
        //  the data to be inserted.  Insert it.
        //
        XMLCh *destP = fHandle->fDSData->fData;
        int i;
        for (i=origStrLength-1; i>=offset; i--)
            destP[i+srcLength] = destP[i];

        for (i=0; i<srcLength; i++)
            destP[i+offset] = srcP[i];
    };

    fHandle->fLength += srcLength;
}



int DOMString::length() const
{
    int             len = 0;
    if (fHandle != 0)
        len = fHandle->fLength;

    return len;
};



void DOMString::print() const
{
    int len = this->length();

    if (len > 0)
    {
        XMLCh *p = fHandle->fDSData->fData;

        // Copy the data from the DOMString buffer into another buffer.
        //  This is only required because the data in the DOMString buffer
        //  may not be null terminated, but we need the null here.
		XMLCh* buffer = new XMLCh[len+1];
        int i;
		for (i=0; i<len; i++)
		   buffer[i] = p[i];
		buffer[len] = 0;

        // Transcode from Unicode to char * in whatever the system local code page is.
        char *pc = XMLString::transcode(buffer);
        fputs(pc, stdout);

        delete [] buffer;
        delete [] pc;
    };
};


void DOMString::println() const
{
	print();
    putchar('\n');
};



XMLCh *DOMString::rawBuffer() const
{
    XMLCh  *retP = 0;
    if (fHandle)
    {
        retP = fHandle->fDSData->fData;
    }
    return retP;
};


char *DOMString::transcode() const
{
    if (!fHandle || fHandle->fLength == 0)
    {
        char* retP = new char[1];
        *retP = 0;
        return retP;
    }

    // We've got some data
    // DOMStrings are not always null terminated, so we may need to
    // copy to another buffer first in order to null terminate it for
    // use as input to the transcoding routines..
    //
    XMLCh* DOMStrData = fHandle->fDSData->fData;

    const int localBufLen = 1000;
    XMLCh localBuf[localBufLen];
    XMLCh *allocatedBuf = 0;
    XMLCh *srcP;

    if (DOMStrData[fHandle->fLength] == 0)
    {
        // The data in the DOMString itself happens to be null terminated.
        //  Just use it in place.
        srcP = DOMStrData;
    } 
    else if (fHandle->fLength < localBufLen-1)
    {
        // The data is not null terminated, but does fit in the
        //  local buffer (fast allocation).  Copy it over, and add 
        //  the null termination,
        memcpy(localBuf, DOMStrData, fHandle->fLength * sizeof(XMLCh));
        srcP = localBuf;
        srcP[fHandle->fLength] = 0;
    }
    else
    {
        // The data is too big for the local buffer.  Heap allocate one.
        allocatedBuf = srcP = new XMLCh[fHandle->fLength + 1];
        memcpy(allocatedBuf, DOMStrData, fHandle->fLength * sizeof(XMLCh));
        srcP[fHandle->fLength] = 0;
    }

    //
    //  Find out how many output chars we need and allocate a buffer big enough
    //  for that plus a null.
    //
    const unsigned int charsNeeded = getDomConverter()->calcRequiredSize(srcP);
    char* retP = new char[charsNeeded + 1];

    if (!getDomConverter()->transcode(srcP, retP, charsNeeded))
    {
        // <TBD> We should throw something here?
    }
    delete [] allocatedBuf;   // which will be null if we didn't allocate one.

    // Cap it off and return it
    retP[charsNeeded] = 0;
    return retP;
}


DOMString DOMString::transcode(const char* str)
{
    return DOMString(str);
}


int DOMString::compareString(const DOMString &other) const
{
    // Note: this strcmp does not match the semantics
    //       of the standard C strcmp.  All it needs to do is
    //       define some less than - equals - greater than ordering
    //       of strings.  How doesn't matter.
    //
    int thisLen = length();
    int otherLen = other.length();

    if (thisLen < otherLen)
        return -1;

    if (thisLen > otherLen)
        return 1;

    if (thisLen == 0)
        return 0;

    XMLCh *thisP =  this->fHandle->fDSData->fData;
    XMLCh *otherP = other.fHandle->fDSData->fData;
    int i;
    for (i=0; i<thisLen; i++)
    {
        if (thisP[i] < otherP[i])
            return -1;
        else if (thisP[i] > otherP[i])
            return 1;
    };

    return 0;
};


DOMString DOMString::substringData(int offset, int count) const
{
    DOMString retString;

    if (count > 0)
    {
        int thisLen = length();
        assert(offset>=0 && count>=0);
        if (offset+count > thisLen)
        {
            assert (offset < thisLen);
            count = thisLen - offset;
        }
        XMLCh *data = fHandle->fDSData->fData;
        retString = DOMString(data+offset, count);
    }
    return retString;
};


DOMString operator + (const DOMString &lhs, const DOMString &rhs)
{
    DOMString retString = lhs.clone();
    retString.appendData(rhs);
    return retString;
}

DOMString operator + (const DOMString &lhs, const XMLCh* rhs)
{
    DOMString retString = lhs.clone();
    retString.appendData(rhs);
    return retString;
}

DOMString operator + (const XMLCh* lhs, const DOMString& rhs)
{
    DOMString retString = DOMString(lhs);
    retString.appendData(rhs);
    return retString;
}


DOMString operator + (const DOMString &lhs, XMLCh rhs)
{
    DOMString retString = lhs.clone();
    retString.appendData(rhs);
    return retString;
}

DOMString operator + (XMLCh lhs, const DOMString& rhs)
{
    DOMString retString;
	retString.appendData(lhs);
    retString.appendData(rhs);
    return retString;
}



