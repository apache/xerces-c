/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 2003 The Apache Software Foundation.  All rights
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
 * $Id$
 * $Log$
 * Revision 1.11  2004/01/12 16:27:41  neilg
 * remove use of static buffers
 *
 * Revision 1.10  2003/12/17 00:18:34  cargilld
 * Update to memory management so that the static memory manager (one used to call Initialize) is only for static data.
 *
 * Revision 1.9  2003/11/25 20:37:40  jberry
 * Cleanup build errors/warnings from CodeWarrior
 *
 * Revision 1.8  2003/10/20 17:37:05  amassari
 * Removed compiler warning
 *
 * Revision 1.7  2003/10/17 21:09:03  peiyongz
 * renaming methods
 *
 * Revision 1.6  2003/10/07 19:38:31  peiyongz
 * API for Template_Class Object Serialization/Deserialization
 *
 * Revision 1.5  2003/09/25 22:22:00  peiyongz
 * Introduction of readString/writeString
 *
 * Revision 1.4  2003/09/25 15:21:12  peiyongz
 * Loose the assert condition so that Serializable class need NOT to check the
 * actual string length before read/write.
 *
 * Revision 1.3  2003/09/23 18:11:29  peiyongz
 * Using HashPtr
 *
 * Revision 1.1  2003/09/18 18:31:24  peiyongz
 * OSU: Object Serialization Utilities
 *
 *
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/internal/XSerializeEngine.hpp>
#include <xercesc/internal/XSerializable.hpp>
#include <xercesc/internal/XProtoType.hpp>

#include <xercesc/util/HashPtr.hpp>

XERCES_CPP_NAMESPACE_BEGIN

const bool XSerializeEngine::toWriteBufferLen = true;
const bool XSerializeEngine::toReadBufferLen  = true;
      int  XSerializeEngine::defaultBufferLen = 0;
      int  XSerializeEngine::defaultDataLen   = 0;

static const int noDataFollowed = -1;

static const XSerializeEngine::XSerializedObjectId_t fgNullObjectTag  = 0;           // indicating null ptrs
static const XSerializeEngine::XSerializedObjectId_t fgNewClassTag    = 0xFFFFFFFF;  // indicating new class
static const XSerializeEngine::XSerializedObjectId_t fgTemplateObjTag = 0xFFFFFFFE;  // indicating template object
static const XSerializeEngine::XSerializedObjectId_t fgClassMask      = 0x80000000;  // indicates class tag
static const XSerializeEngine::XSerializedObjectId_t fgMaxObjectCount = 0x3FFFFFFD;  

#define TEST_THROW_ARG1(condition, data, err_msg) \
if (condition) \
{ \
    XMLCh value1[16]; \
    XMLString::binToText(data, value1, 16, 10, fMemoryManager); \
    ThrowXMLwithMemMgr1(XSerializationException \
            , err_msg  \
            , value1 \
            , fMemoryManager); \
}

#define TEST_THROW_ARG2(condition, data1, data2, err_msg) \
if (condition) \
{ \
    XMLCh value1[16]; \
    XMLCh value2[16]; \
    XMLString::binToText(data1, value1, 16, 10, fMemoryManager); \
    XMLString::binToText(data2, value2, 16, 10, fMemoryManager); \
    ThrowXMLwithMemMgr2(XSerializationException \
            , err_msg  \
            , value1   \
            , value2 \
            , fMemoryManager); \
}

// ---------------------------------------------------------------------------
//  Constructor and Destructor
// ---------------------------------------------------------------------------
XSerializeEngine::XSerializeEngine(BinInputStream*         inStream
                                 , MemoryManager* const    manager
                                 , unsigned long           bufSize)
:fStoreLoad(mode_Load)
,fMemoryManager(manager)
,fInputStream(inStream)
,fOutputStream(0)
,fBufSize(bufSize)
,fBufStart( (XMLByte*) fMemoryManager->allocate(bufSize))
,fBufCur(fBufStart)
,fBufEnd(0)
,fBufLoadMax(fBufStart)
,fStorePool(0)
,fLoadPool( new (fMemoryManager) ValueVectorOf<void*>(29, fMemoryManager, false))
,fObjectCount(0)
{
    /*** 
     *  initialize buffer from the inStream
     ***/
    fillBuffer(1);

}

XSerializeEngine::XSerializeEngine(BinOutputStream*        outStream
                                 , MemoryManager* const    manager
                                 , unsigned long           bufSize)
:fStoreLoad(mode_Store)
,fMemoryManager(manager)
,fInputStream(0)
,fOutputStream(outStream)
,fBufSize(bufSize)
,fBufStart((XMLByte*) fMemoryManager->allocate(bufSize))
,fBufCur(fBufStart)
,fBufEnd(fBufStart+bufSize)
,fBufLoadMax(0)
,fStorePool( new (fMemoryManager) RefHashTableOf<XSerializedObjectId>(29, true, new HashPtr(), fMemoryManager) )
,fLoadPool(0)
,fObjectCount(0)
{
    //initialize store pool
    fStorePool->put(0, new (fMemoryManager) XSerializedObjectId(fgNullObjectTag));

}

XSerializeEngine::~XSerializeEngine()
{
    if (isStoring())
    {
        flush();
        delete fStorePool;
    }
    else
    {
        delete fLoadPool;
    }

    fMemoryManager->deallocate(fBufStart);

}

void XSerializeEngine::flush()
{
    if (isStoring())
        flushBuffer();

}

// ---------------------------------------------------------------------------
//  Storing 
// ---------------------------------------------------------------------------
void XSerializeEngine::write(XSerializable* const objectToWrite)
{
    ensureStoring();
    //don't ensurePointer here !!!

    XSerializedObjectId_t   objIndex = 0;

	if (!objectToWrite)  // null pointer
	{
		*this << fgNullObjectTag;
	}
    else if (0 != (objIndex = lookupStorePool((void*) objectToWrite)))
	{
        // writing an object reference tag
        *this << objIndex;
	}
	else
	{
		// write protoType first
		XProtoType* protoType = objectToWrite->getProtoType();
		write(protoType);

		// put the object into StorePool
        addStorePool((void*)objectToWrite);

        // ask the object to serialize itself
		objectToWrite->serialize(*this);
	}

}

void XSerializeEngine::write(XProtoType* const protoType)
{
    ensureStoring();
    ensurePointer(protoType);

	XSerializedObjectId_t objIndex = lookupStorePool((void*)protoType);

    if (objIndex)
    {   
        //protoType seen in the store pool
        *this << (fgClassMask | objIndex);
	}
	else
	{
		// store protoType
		*this << fgNewClassTag;
		protoType->store(*this);
        addStorePool((void*)protoType);
	}

}

/***
 *
***/ 
void XSerializeEngine::write(const XMLCh* const toWrite
                           ,       int          writeLen)
{
    write((XMLByte*)toWrite, (sizeof(XMLCh)/sizeof(XMLByte)) * writeLen);
}


void XSerializeEngine::write(const XMLByte* const toWrite
                           ,       int            writeLen)
{
    ensureStoring();
    ensurePointer((void*)toWrite);
    ensureBufferLen(writeLen);
    ensureStoreBuffer();

    if (writeLen == 0)
        return;

    /***
     *  If the available space is sufficient, write it up
     ***/
    int bufAvail = fBufEnd - fBufCur;

    if (writeLen <= bufAvail)
    {
        memcpy(fBufCur, toWrite, writeLen);
        fBufCur += writeLen;
        return;
    }

    const XMLByte*  tempWrite   = (const XMLByte*) toWrite;
    unsigned int    writeRemain = writeLen;

    // fill up the avaiable space and flush
    memcpy(fBufCur, tempWrite, bufAvail);
    fBufCur     += bufAvail;
    tempWrite   += bufAvail;
    writeRemain -= bufAvail;
    flushBuffer();

    // write chunks of fBufSize
    while (writeRemain > fBufSize)
    {
        memcpy(fBufCur, tempWrite, fBufSize);
        fBufCur     += fBufSize;
        tempWrite   += fBufSize;
        writeRemain -= fBufSize;
        flushBuffer();
    }

    // write the remaining if any
    if (writeRemain)
    {
        memcpy(fBufCur, tempWrite, writeRemain);
        fBufCur += writeRemain;
    }

}

/***
 *
 *     Storage scheme (normal):
 *
 *     <
 *     1st integer:    bufferLen (optional)
 *     2nd integer:    dataLen
 *     bytes following:
 *     >
 *
 *     Storage scheme (special):
 *     <
 *     only integer:   noDataFollowed
 *     >
 */

void XSerializeEngine::writeString(const XMLCh* const toWrite
                                 , const int          bufferLen
                                 , bool               toWriteBufLen)
{
    if (toWrite) 
    {
        if (toWriteBufLen)
            *this<<bufferLen;

        int strLen = XMLString::stringLen(toWrite);
        *this<<strLen;
        write(toWrite, strLen);
    }
    else
    {
        *this<<noDataFollowed;
    }

}

void XSerializeEngine::writeString(const XMLByte* const toWrite
                                 , const int            bufferLen
                                 , bool                 toWriteBufLen)
{

    if (toWrite) 
    {
        if (toWriteBufLen)
            *this<<bufferLen;

        int strLen = XMLString::stringLen((char*)toWrite);
        *this<<strLen;
        write(toWrite, strLen);
    }
    else
    {
        *this<<noDataFollowed;
    }

}

// ---------------------------------------------------------------------------
//  Loading
// ---------------------------------------------------------------------------
XSerializable* XSerializeEngine::read(XProtoType* const protoType)
{
    ensureLoading();
    ensurePointer(protoType);

	XSerializedObjectId_t    objectTag;
	XSerializable*           objRet;

    if (! read(protoType, &objectTag))
	{
        /***
         * We hava a reference to an existing object in
         * load pool, get it.
         */
        objRet = lookupLoadPool(objectTag);
	}
	else
	{
		// create the object from the prototype
		objRet = protoType->fCreateObject(fMemoryManager);
        Assert((objRet != 0), XMLExcepts::XSer_CreateObject_Fail);  
 
        // put it into load pool 
        addLoadPool(objRet);

        // de-serialize it
		objRet->serialize(*this);

	}

	return objRet;
}

bool XSerializeEngine::read(XProtoType*            const    protoType
                          , XSerializedObjectId_t*          objectTagRet)
{
    ensureLoading();
    ensurePointer(protoType);

	XSerializedObjectId_t obTag;

    *this >> obTag;

    // object reference tag found
    if (!(obTag & fgClassMask))
	{
		*objectTagRet = obTag;
		return false;
	}
    
	if (obTag == fgNewClassTag)
	{
        // what follows fgNewClassTag is the prototype object info
        // for the object anticipated, go and verify the info
        XProtoType::load(*this, protoType->fClassName, fMemoryManager);

        addLoadPool((void*)protoType);
	}
	else
	{
        // what follows class tag is an XSerializable object
		XSerializedObjectId_t classIndex = (obTag & ~fgClassMask);
        XSerializedObjectId_t loadPoolSize = (XSerializedObjectId_t)fLoadPool->size();

        TEST_THROW_ARG2(((classIndex == 0 ) || (classIndex > loadPoolSize))
                  , classIndex
                  , loadPoolSize
                  , XMLExcepts::XSer_Inv_ClassIndex
                  )

        ensurePointer(lookupLoadPool(classIndex));

   }

	return true;
}

void XSerializeEngine::read(XMLCh* const toRead
                          , int          readLen)
{
    read((XMLByte*)toRead, (sizeof(XMLCh)/sizeof(XMLByte))*readLen);
}

void XSerializeEngine::read(XMLByte* const toRead
                          , int            readLen)
{
    ensureLoading();
    ensureBufferLen(readLen);
    ensurePointer(toRead);
    ensureLoadBuffer();

    if (readLen == 0)
        return;

    /***
     *  If unread is sufficient, read it up
     ***/
    int dataAvail = fBufLoadMax - fBufCur;

    if (readLen <= dataAvail)
    {
        memcpy(toRead, fBufCur, readLen);
        fBufCur += readLen;
        return;
    }

    XMLByte*     tempRead   = (XMLByte*) toRead;
    unsigned int readRemain = readLen;

    // read the unread
    memcpy(tempRead, fBufCur, dataAvail);
    fBufCur    += dataAvail;
    tempRead   += dataAvail;
    readRemain -= dataAvail;

    // read chunks of fBufSize
    while (readRemain > fBufSize)
    {
        fillBuffer(fBufSize);
        memcpy(tempRead, fBufCur, fBufSize);
        fBufCur    += fBufSize;
        tempRead   += fBufSize;
        readRemain -= fBufSize;
    }

    // read the remaining if any
    if (readRemain)
    {
        fillBuffer(readRemain);
        memcpy(tempRead, fBufCur, readRemain);
        fBufCur += readRemain;
    }

}

/***
 *
 *     Storage scheme (normal):
 *
 *     <
 *     1st integer:    bufferLen (optional)
 *     2nd integer:    dataLen
 *     bytes following:
 *     >
 *
 *     Storage scheme (special):
 *     <
 *     only integer:   noDataFollowed
 *     >
 */
void XSerializeEngine::readString(XMLCh*&  toRead
                                , int&     bufferLen
                                , int&     dataLen
                                , bool     toReadBufLen)
{
    /***
     * Check if any data written
     ***/
    *this>>bufferLen;
    if (bufferLen == noDataFollowed)
    {
        toRead = 0;
        bufferLen = 0;
        dataLen = 0;
        return;
    }

    if (toReadBufLen)
    {
        *this>>dataLen;
    }
    else
    {
        dataLen = bufferLen++;        
    }

    toRead = (XMLCh*) fMemoryManager->allocate(bufferLen * sizeof(XMLCh));
    read(toRead, dataLen);
    toRead[dataLen] = 0;
}

void XSerializeEngine::readString(XMLByte*&  toRead
                                , int&       bufferLen
                                , int&       dataLen
                                , bool       toReadBufLen)
{
    /***
     * Check if any data written
     ***/
    *this>>bufferLen;
    if (bufferLen == noDataFollowed)
    {
        toRead = 0;
        bufferLen = 0;
        dataLen = 0;
        return;
    }

    if (toReadBufLen)
    {
        *this>>dataLen;
    }
    else
    {
        dataLen = bufferLen++;
    }

    toRead = (XMLByte*) fMemoryManager->allocate(bufferLen * sizeof(XMLByte));
    read(toRead, dataLen);
    toRead[dataLen] = 0;

}

// ---------------------------------------------------------------------------
//  Insertion
// ---------------------------------------------------------------------------
XSerializeEngine& XSerializeEngine::operator<<(XMLCh xch)
{ 
    checkAndFlushBuffer(sizeof(XMLCh));

    *(XMLCh*)fBufCur = xch; 
    fBufCur += sizeof(XMLCh); 
    return *this; 
}
 
XSerializeEngine& XSerializeEngine::operator<<(XMLByte by)
{ 
    checkAndFlushBuffer(sizeof(XMLByte));

    *(XMLByte*)fBufCur = by; 
    fBufCur += sizeof(XMLByte); 
    return *this; 
}

XSerializeEngine& XSerializeEngine::operator<<(bool b)
{ 
    checkAndFlushBuffer(sizeof(bool));

    *(bool*)fBufCur = b; 
    fBufCur += sizeof(bool); 
    return *this; 
}

XSerializeEngine& XSerializeEngine::operator<<(char ch)
{ 
    return XSerializeEngine::operator<<((XMLByte)ch); 
}

XSerializeEngine& XSerializeEngine::operator<<(short sh)
{ 
    checkAndFlushBuffer(sizeof(short));

    *(short*)fBufCur = sh; 
    fBufCur += sizeof(short); 
    return *this; 
}

XSerializeEngine& XSerializeEngine::operator<<(int i)
{ 
    return XSerializeEngine::operator<<((long)i); 
}

XSerializeEngine& XSerializeEngine::operator<<(unsigned int ui)
{ 
    return XSerializeEngine::operator<<((unsigned long)ui); 
}

XSerializeEngine& XSerializeEngine::operator<<(long l)
{ 
    checkAndFlushBuffer(sizeof(long));

    *(long*)fBufCur = l; 
    fBufCur += sizeof(long); 
    return *this; 
}

XSerializeEngine& XSerializeEngine::operator<<(unsigned long ul)
{ 
    checkAndFlushBuffer(sizeof(unsigned long));

    *(unsigned long*)fBufCur = ul; 
    fBufCur += sizeof(unsigned long); 
    return *this; 
}

XSerializeEngine& XSerializeEngine::operator<<(float f)
{ 
    checkAndFlushBuffer(sizeof(float));

    *(float*)fBufCur = *(float*)&f; 
    fBufCur += sizeof(float); 
    return *this;
}

XSerializeEngine& XSerializeEngine::operator<<(double d)
{ 
    checkAndFlushBuffer(sizeof(double));

    *(double*)fBufCur = *(double*)&d; 
    fBufCur += sizeof(double); 
    return *this; 
}

// ---------------------------------------------------------------------------
//  Extraction
// ---------------------------------------------------------------------------
/***
XSerializeEngine& operator>>(XSerializeEngine& serEng
                           , XSerializable*& serObj)
{
    serObj = serEng.read(0); 
    return serEng; 
}

XSerializeEngine& operator>>(XSerializeEngine& serEng
                           , const XSerializable*& serObj)
{
    serObj = serEng.read(0); 
    return serEng; 
}
***/

XSerializeEngine& XSerializeEngine::operator>>(XMLCh& xch)
{ 
    checkAndFillBuffer(sizeof(XMLCh));

    xch = *(XMLCh*)fBufCur; 
    fBufCur += sizeof(XMLCh); 
    return *this; 
}

XSerializeEngine& XSerializeEngine::operator>>(XMLByte& by)
{ 
    checkAndFillBuffer(sizeof(XMLByte));

    by = *(XMLByte*)fBufCur; 
    fBufCur += sizeof(XMLByte); 
    return *this; 
}

XSerializeEngine& XSerializeEngine::operator>>(bool& b)
{ 
    checkAndFillBuffer(sizeof(bool));

    b = *(bool*)fBufCur; 
    fBufCur += sizeof(bool); 
    return *this; 
}

XSerializeEngine& XSerializeEngine::operator>>(char& ch)
{ 
    return XSerializeEngine::operator>>((XMLByte&)ch); 
}

XSerializeEngine& XSerializeEngine::operator>>(short& sh)
{ 
    checkAndFillBuffer(sizeof(short));

    sh = *(short*)fBufCur; 
    fBufCur += sizeof(short); 
    return *this; 
}

XSerializeEngine& XSerializeEngine::operator>>(int& i)
{ 
    return XSerializeEngine::operator>>((long&)i); 
}

XSerializeEngine& XSerializeEngine::operator>>(unsigned int& ui)
{ 
    return XSerializeEngine::operator>>((unsigned long&)ui); 
}

XSerializeEngine& XSerializeEngine::operator>>(long& l)
{ 
    checkAndFillBuffer(sizeof(long));

    l = *(long*)fBufCur; 
    fBufCur += sizeof(long); 
    return *this; 
}

XSerializeEngine& XSerializeEngine::operator>>(unsigned long& ul)
{ 
    checkAndFillBuffer(sizeof(unsigned long));

    ul = *(unsigned long*)fBufCur; 
    fBufCur += sizeof(unsigned long); 
    return *this; 
}

XSerializeEngine& XSerializeEngine::operator>>(float& f)
{ 
    checkAndFillBuffer(sizeof(float));

    *(float*)&f = *(float*)fBufCur; 
    fBufCur += sizeof(float); 
    return *this; 
}

XSerializeEngine& XSerializeEngine::operator>>(double& d)
{ 
    checkAndFillBuffer(sizeof(double));

    *(double*)&d = *(double*)fBufCur; 
    fBufCur += sizeof(double); 
    return *this; 
}

// ---------------------------------------------------------------------------
//  StorePool/LoadPool Opertions
// ---------------------------------------------------------------------------
XSerializeEngine::XSerializedObjectId_t 
XSerializeEngine::lookupStorePool(void* const objToLookup) const
{
    //0 indicating object is not in the StorePool
    XSerializedObjectId* data = fStorePool->get(objToLookup);   
    return (XSerializeEngine::XSerializedObjectId_t) (data ? data->getValue() : 0);

}

void XSerializeEngine::addStorePool(void* const objToAdd)
{
    pumpCount();
    fStorePool->put(objToAdd, new XSerializedObjectId(fObjectCount));
}

XSerializable* XSerializeEngine::lookupLoadPool(XSerializedObjectId_t objectTag) const
{

    /***
      *  an object tag read from the binary refering to
      *  an object beyond the upper most boundary of the load pool
      ***/    
    TEST_THROW_ARG2( (objectTag > fLoadPool->size())
              , objectTag
              , fLoadPool->size()
              , XMLExcepts::XSer_LoadPool_UppBnd_Exceed
              )

    if (objectTag == 0)
        return 0;

    /***
     *   A non-null object tag starts from 1 while fLoadPool starts from 0
     ***/
    return (XSerializable*) fLoadPool->elementAt(objectTag - 1);
}

void XSerializeEngine::addLoadPool(void* const objToAdd)
{

    TEST_THROW_ARG2( (fLoadPool->size() != fObjectCount)
               , fObjectCount
               , fLoadPool->size()
               , XMLExcepts::XSer_LoadPool_NoTally_ObjCnt
               )

    pumpCount();
    fLoadPool->addElement(objToAdd);

}

void XSerializeEngine::pumpCount()
{

    TEST_THROW_ARG2( (fObjectCount >= fgMaxObjectCount)
               , fObjectCount
               , fgMaxObjectCount
               , XMLExcepts::XSer_ObjCount_UppBnd_Exceed
              )

    fObjectCount++;

}

// ---------------------------------------------------------------------------
//  Buffer Opertions
// ---------------------------------------------------------------------------
/***
 *
 *  Always try to fill up the whole buffer
 *
 ***/
void XSerializeEngine::fillBuffer(int miniBytesNeeded)
{
    ensureLoading();
    ensureLoadBuffer();

	TEST_THROW_ARG2( ((miniBytesNeeded <= 0) || ((unsigned long)miniBytesNeeded > fBufSize))
              , miniBytesNeeded
              , fBufSize
              , XMLExcepts::XSer_Inv_FillBuffer_Size
              );

    /***
     *   Move remaing bytes to the beginning of the buffer, if any
     ***/
	const int unRead = fBufLoadMax - fBufCur;
    const int unUsed = fBufSize - unRead;

    if (unRead > 0)
    {
        memcpy(fBufStart, fBufCur, unRead);
    }

    fBufCur = fBufStart + unRead;
    
    int bytesRead = fInputStream->readBytes(fBufCur, unUsed);

    /***
     * InputStream MUST read in at leaset miniBytesNeeded
     ***/
    TEST_THROW_ARG2( (bytesRead < miniBytesNeeded)
               , bytesRead
               , miniBytesNeeded
               , XMLExcepts::XSer_InStream_Read_LT_Req
               )

    /***
     * This is worse: buffer overflow bug
     ***/
    TEST_THROW_ARG2( (bytesRead > unUsed)
               , bytesRead
               , miniBytesNeeded
               , XMLExcepts::XSer_InStream_Read_OverFlow
               )

    fBufLoadMax = fBufCur + bytesRead;
    fBufCur     = fBufStart;

    ensureLoadBuffer();

}

/***
 *
 *  Flush out whatever left in the buffer.
 *
 ***/
void XSerializeEngine::flushBuffer()
{
    ensureStoring();
    ensureStoreBuffer();

    fOutputStream->writeBytes(fBufStart, fBufCur - fBufStart);
    fBufCur = fBufStart;

    ensureStoreBuffer();
}

inline void XSerializeEngine::checkAndFlushBuffer(int bytesNeedToWrite)
{
    TEST_THROW_ARG1( (bytesNeedToWrite <= 0)
                   , bytesNeedToWrite
                   , XMLExcepts::XSer_Inv_checkFlushBuffer_Size
                   )

    // fBufStart ... fBufCur ...fBufEnd
    if ((fBufCur + bytesNeedToWrite) >= fBufEnd) 
        flushBuffer();
}

inline void XSerializeEngine::checkAndFillBuffer(int bytesNeedToRead)
{

    TEST_THROW_ARG1( (bytesNeedToRead <= 0)
                   , bytesNeedToRead
                   , XMLExcepts::XSer_Inv_checkFillBuffer_Size
                   )

    // fBufStart ... fBufCur ...fBufLoadMax
    if ((fBufCur + bytesNeedToRead) > fBufLoadMax)
        fillBuffer(fBufCur + bytesNeedToRead - fBufLoadMax);

}

inline void XSerializeEngine::ensureStoreBuffer() const
{

    TEST_THROW_ARG2 ( !((fBufStart <= fBufCur) && (fBufCur <= fBufEnd))
                    , (int)(fBufCur - fBufStart)
                    , (int)(fBufEnd - fBufCur)
                    , XMLExcepts::XSer_StoreBuffer_Violation
                    )

}

inline void XSerializeEngine::ensureLoadBuffer() const
{

    TEST_THROW_ARG2 ( !((fBufStart <= fBufCur) && (fBufCur <= fBufLoadMax))
                    , (int)(fBufCur - fBufStart)
                    , (int)(fBufLoadMax - fBufCur)
                    , XMLExcepts::XSer_LoadBuffer_Violation
                    )

}

inline void XSerializeEngine::ensurePointer(void* const ptr) const
{

    TEST_THROW_ARG1( (ptr == 0)
                   , 0
                   , XMLExcepts::XSer_Inv_Null_Pointer
                   )

}

inline void XSerializeEngine::ensureBufferLen(int bufferLen) const
{

    TEST_THROW_ARG1( (bufferLen < 0)
                   , bufferLen
                   , XMLExcepts::XSer_Inv_Buffer_Len
                   )

}

// ---------------------------------------------------------------------------
//  Template object
// ---------------------------------------------------------------------------
/***
 *
 *  Search the store pool to see if the address has been seen before or not.
 *
 *  If yes, write the corresponding object Tag to the internal buffer
 *  and return true.
 *
 *  Otherwise, add the address to the store pool and return false
 *  to notifiy the client application code to store the template object.
 *
 ***/
bool XSerializeEngine::needToStoreObject(void* const  templateObjectToWrite)
{
    ensureStoring(); //don't ensurePointer here !!!

    XSerializedObjectId_t   objIndex = 0;

	if (!templateObjectToWrite)  
	{
		*this << fgNullObjectTag; // null pointer
        return false;
	}
    else if (0 != (objIndex = lookupStorePool(templateObjectToWrite)))
	{
        *this << objIndex;         // write an object reference tag
        return false;
	}
	else
	{
        *this << fgTemplateObjTag;            // write fgTemplateObjTag to denote that actual
                                              // template object follows
        addStorePool(templateObjectToWrite); // put the address into StorePool
        return true;
	}

}

bool XSerializeEngine::needToLoadObject(void**  templateObjectToRead)
{
    ensureLoading();

	XSerializedObjectId_t obTag;

    *this >> obTag;
  
	if (obTag == fgTemplateObjTag)
	{
        /***
         * what follows fgTemplateObjTag is the actual template object
         * We need the client application to create a template object
         * and register it through registerObject(), and deserialize
         * template object
         ***/
        return true;
	}
	else
	{
        /***
         * We hava a reference to an existing template object, get it.
         */
        *templateObjectToRead = lookupLoadPool(obTag);
        return false;
   }

}

void XSerializeEngine::registerObject(void*  const templateObjectToRegister)
{
    ensureLoading();
    addLoadPool(templateObjectToRegister);
}

XERCES_CPP_NAMESPACE_END

