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
 * $Log$
 * Revision 1.5  2003/10/09 19:11:53  peiyongz
 * Fix to linkage error on Solaris
 *
 * Revision 1.4  2003/10/07 19:38:31  peiyongz
 * API for Template_Class Object Serialization/Deserialization
 *
 * Revision 1.3  2003/09/25 22:22:00  peiyongz
 * Introduction of readString/writeString
 *
 * Revision 1.2  2003/09/19 04:29:11  neilg
 * fix compilation problems under GCC
 *
 * Revision 1.1  2003/09/18 18:31:24  peiyongz
 * OSU: Object Serialization Utilities
 *
 * $Id$
 *
 */

#if !defined(XSERIALIZE_ENGINE_HPP)
#define XSERIALIZE_ENGINE_HPP

#include <xercesc/framework/BinOutputStream.hpp>
#include <xercesc/util/BinInputStream.hpp>
#include <xercesc/util/RefHashTableOf.hpp>
#include <xercesc/util/ValueVectorOf.hpp>
#include <xercesc/internal/XSerializationException.hpp>
#include <xercesc/util/XMLExceptMsgs.hpp>

XERCES_CPP_NAMESPACE_BEGIN

class XSerializable;
class XProtoType;
class MemoryManager;
class XSerializedObjectId;

class XMLUTIL_EXPORT XSerializeEngine
{
public:

    enum { mode_Store
         , mode_Load 
    };

    typedef unsigned long   XSerializedObjectId_t;

    /***
      *
      *  Destructor 
      *
      ***/
    ~XSerializeEngine();

    /***
      *
      *  Constructor for de-serialization(loading)
      *
      *  Application needs to make sure that the instance of
      *  BinInputStream, persists beyond the life of this
      *  SerializeEngine.
      *
      *  Param
      *     inStream         input stream
      *     manager          MemoryManager
      *     bufSize          the size of the internal buffer
      *
      ***/
    XSerializeEngine(BinInputStream*         inStream
                   , MemoryManager* const    manager = XMLPlatformUtils::fgMemoryManager
                   , unsigned long           bufSize = 8192 );

    /***
      *
      *  Constructor for serialization(storing)
      *
      *  Application needs to make sure that the instance of
      *  BinOutputStream, persists beyond the life of this
      *  SerializeEngine.
      *
      *  Param
      *     outStream        output stream
      *     manager          MemoryManager
      *     bufSize          the size of the internal buffer
      *
      ***/
    XSerializeEngine(BinOutputStream*        outStream
                   , MemoryManager* const    manager = XMLPlatformUtils::fgMemoryManager
                   , unsigned long           bufSize = 8192 );

    /***
      *
      *  When serialization, flush out the internal buffer
      *
      *  Return: 
      *
      ***/
    void flush();

    /***
      *
      *  Checking if the serialize engine is doing serialization(storing)
      *
      *  Return: true, if it is 
      *          false, otherwise
      *
      ***/
    inline bool isStoring() const;

    /***
      *
      *  Checking if the serialize engine is doing de-serialization(loading)
      *
      *  Return: true, if it is 
      *          false, otherwise
      *
      ***/
    inline bool isLoading() const;

    /***
      *
      *  Write object to the internal buffer.
      *
      *  Param
      *     objectToWrite:    the object to be serialized
      *
      *  Return:
      *
      ***/
           void           write(XSerializable* const objectToWrite);

    /***
      *
      *  Write prototype info to the internal buffer.
      *
      *  Param
      *     protoType:    instance of prototype
      *
      *  Return:
      *
      ***/
           void           write(XProtoType* const protoType);

    /***
      *
      *  Write a stream of XMLByte to the internal buffer.
      *
      *  Param
      *     toWrite:   the stream of XMLByte to write
      *     writeLen:  the length of the stream
      *
      *  Return:
      *
      ***/
           void           write(const XMLByte* const toWrite
                               ,      int            writeLen);

    /***
      *
      *  Write a stream of XMLCh to the internal buffer.
      *
      *  Param
      *     toWrite:   the stream of XMLCh to write
      *     writeLen:  the length of the stream
      *
      *  Return:
      *
      ***/
           void           write(const XMLCh* const toWrite
                               ,      int          writeLen);

    /***
      *
      *  Write a stream of XMLCh to the internal buffer.
      *
      *  Write the bufferLen first if requested, then the length
      *  of the stream followed by the stream.
      *
      *  Param
      *     toWrite:        the stream of XMLCh to write
      *     bufferLen:      the maximum size of the buffer
      *     toWriteBufLen:  specify if the bufferLen need to be written or not
      *
      *  Return:
      *
      ***/
           void           writeString(const XMLCh* const toWrite
                                    , const int          bufferLen = 0
                                    , bool               toWriteBufLen = false);

    /***
      *
      *  Write a stream of XMLByte to the internal buffer.
      *
      *  Write the bufferLen first if requested, then the length
      *  of the stream followed by the stream.
      *
      *  Param
      *     toWrite:        the stream of XMLByte to write
      *     bufferLen:      the maximum size of the buffer
      *     toWriteBufLen:  specify if the bufferLen need to be written or not
      *
      *  Return:
      *
      ***/
           void           writeString(const XMLByte* const toWrite
                                    , const int            bufferLen = 0
                                    , bool                 toWriteBufLen = false);

    static const bool toWriteBufferLen;

    /***
      *
      *  Read/Create object from the internal buffer.
      *
      *  Param
      *     protoType:    an instance of prototype of the object anticipated
      *
      *  Return:          to object read/created
      *
      ***/
	       XSerializable* read(XProtoType* const protoType);

    /***
      *
      *  Read prototype object from the internal buffer.
      *  Verify if the same prototype object found in buffer.
      *
      *  Param
      *     protoType:    an instance of prototype of the object anticipated
      *     objTag:       the object Tag to an existing object
      *
      *  Return:          true  : if matching found
      *                   false : otherwise
      *
      ***/
           bool           read(XProtoType* const    protoType
		                     , XSerializedObjectId_t*       objTag);

    /***
      *
      *  Read XMLByte stream from the internal buffer.
      *
      *  Param
      *     toRead:   the buffer to hold the XMLByte stream
      *     readLen:  the length of the XMLByte to read in
      *
      *  Return:
      *
      ***/
           void           read(XMLByte* const toRead
                             , int            readLen);

    /***
      *
      *  Read XMLCh stream from the internal buffer.
      *
      *  Param
      *     toRead:   the buffer to hold the XMLCh stream
      *     readLen:  the length of the XMLCh to read in
      *
      *  Return:
      *
      ***/
           void           read(XMLCh* const toRead
                             , int          readLen);

    /***
      *
      *  Read a stream of XMLCh from the internal buffer.
      *
      *  Read the bufferLen first if requested, then the length
      *  of the stream followed by the stream.
      *
      *  Param
      *     toRead:       the pointer to the buffer to hold the XMLCh stream
      *     bufferLen:    the size of the buffer created
      *     dataLen:       the length of the stream
      *     toReadBufLen: specify if the bufferLen need to be read or not
      *
      *  Return:
      *
      ***/
           void           readString(XMLCh*&        toRead
                                   , int&           bufferLen    = defaultBufferLen
                                   , int&           dataLen      = defaultDataLen
                                   , bool           toReadBufLen = false);

    /***
      *
      *  Read a stream of XMLByte from the internal buffer.
      *
      *  Read the bufferLen first if requested, then the length
      *  of the stream followed by the stream.
      *
      *  Param
      *     toRead:       the pointer to the buffer to hold the XMLByte stream
      *     bufferLen:    the size of the buffer created
      *     dataLen:       the length of the stream
      *     toReadBufLen: specify if the bufferLen need to be read or not
      *
      *  Return:
      *
      ***/
           void           readString(XMLByte*&      toRead
                                   , int&           bufferLen    = defaultBufferLen
                                   , int&           dataLen      = defaultDataLen
                                   , bool           toReadBufLen = false);


    /***
      *
      *  Check if the template object has been stored or not
      *
      *  Param
      *    objectPtr:     the template object pointer
      *
      *  Return:          true  : the object has NOT been stored yet
      *                   false : otherwise
      *
      ***/
           bool           needToWriteTemplateObject(void* const templateObjectToWrite);

    /***
      *
      *  Check if the template object has been loaded or not
      *
      *  Param
      *    objectPtr:     the address of the template object pointer
      *
      *  Return:          true  : the object has NOT been loaded yet
      *                   false : otherwise
      *
      ***/
           bool           needToReadTemplateObject(void**       templateObjectToRead);

    /***
      *
      *  In the case of needToReadTemplateObject() return true, the client
      *  application needs to instantiate an expected template object, and
      *  register the address to the engine.
      *
      *  Param
      *    objectPtr:     the template object pointer newly instantiated
      *
      *  Return:  
      *
      ***/
           void           registerTemplateObject(void* const templateObjectToRegister);

    static const bool toReadBufferLen;

    static int defaultBufferLen;

    static int defaultDataLen;

    /***
      *
      *  Insertion operator for serializable classes
      *
      ***/

	friend XSerializeEngine& operator<<(XSerializeEngine&
                                      , XSerializable* const );

    /***
      *
      *  Insertion operators for 
      *     . basic Xerces data types
      *     . built-in types 
      *
      ***/
           XSerializeEngine& operator<<(XMLByte);
           XSerializeEngine& operator<<(XMLCh);

           XSerializeEngine& operator<<(char);
           XSerializeEngine& operator<<(short);
           XSerializeEngine& operator<<(int);
           XSerializeEngine& operator<<(unsigned int);
           XSerializeEngine& operator<<(long);
           XSerializeEngine& operator<<(unsigned long);
           XSerializeEngine& operator<<(float);
           XSerializeEngine& operator<<(double);
           XSerializeEngine& operator<<(bool);

    /***
      *
      *  Extraction operators for 
      *     . basic Xerces data types
      *     . built-in types 
      *
      ***/
           XSerializeEngine& operator>>(XMLByte&);
           XSerializeEngine& operator>>(XMLCh&);

           XSerializeEngine& operator>>(char&);
           XSerializeEngine& operator>>(short&);
           XSerializeEngine& operator>>(int&);
           XSerializeEngine& operator>>(unsigned int&);
           XSerializeEngine& operator>>(long&);
           XSerializeEngine& operator>>(unsigned long&);
           XSerializeEngine& operator>>(float&);
           XSerializeEngine& operator>>(double&);
           XSerializeEngine& operator>>(bool&);

private:
    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
	XSerializeEngine();
    XSerializeEngine(const XSerializeEngine&);
	XSerializeEngine& operator=(const XSerializeEngine&);

    /***
      *
      *   Store Pool Opertions
      *
      ***/
           XSerializedObjectId_t  lookupStorePool(void* const objectPtr) const;
           void                   addStorePool(void* const objectPtr);

    /***
      *
      *   Load Pool Opertions
      *
      ***/
           XSerializable* lookupLoadPool(XSerializedObjectId_t objectTag) const;
           void           addLoadPool(void* const objectPtr);
   
    /***
      *   
      *    Intenal Buffer Operations
      *
      ***/
    inline void           checkAndFillBuffer(int bytesNeedToRead);

    inline void           checkAndFlushBuffer(int bytesNeedToWrite);

           void           fillBuffer(int bytesNeedToRead);

           void           flushBuffer();

           void           pumpCount();

    /***
      *   
      *    Helper
      *
      ***/
    inline void            ensureStoring()                          const;

    inline void            ensureLoading()                          const;

    inline void            ensureStoreBuffer()                      const;

    inline void            ensureLoadBuffer()                       const;

    inline void            ensurePointer(void* const)               const;

    inline void            ensureBufferLen(int  bufferLen)          const;

    inline void            Assert(bool  toEval
                                , const XMLExcepts::Codes toThrow)  const;

    // -------------------------------------------------------------------------------
    //  data
    //
    //  fStoreLoad: 
    //               Indicator: storing(serialization) or loading(de-serialization)
    //
    //  fMemoryManager:
    //               used to allocate memory for internal consumption and/or
    //               allocate memory for object created in de-serialization.
    //
    //  fInputStream:
    //               Binary stream to read from (de-serialization), provided
    //               by client application, not owned.
    //
    //  fOutputStream:
    //               Binary stream to write to (serialization), provided 
    //               by client application, not owned.
    //
    //  fBufSize:
    //               The size of the internal buffer
    //
    //  fBufStart/fBufEnd:
    //               
    //               The internal buffer.
    //  fBufEnd:
    //               one beyond the last valid cell
    //               fBufEnd === (fBufStart + fBufSize)
    //
    //  fBufCur:
    //               The cursor of the buffer
    //
    //  fBufLoadMax:
    //               Indicating the end of the valid content in the buffer
    //
    //  fStorePool:
    //                Object collection for storing
    //
    //  fLoadPool:
    //                Object collection for loading
    //
    //  fMapCount:
    // -------------------------------------------------------------------------------
    const short                            fStoreLoad;   
    MemoryManager*   const                 fMemoryManager;
    BinInputStream*  const                 fInputStream;
    BinOutputStream* const                 fOutputStream;

    //buffer
    const unsigned long                    fBufSize;
	XMLByte* const                         fBufStart;
	XMLByte* const                         fBufEnd; 
    XMLByte*                               fBufCur;
    XMLByte*                               fBufLoadMax; 

    /***
     *   Map for storing object
     *
     *   key:   XSerializable*
     *          XProtoType*
     *
     *   value: XMLInteger*, owned
     *
     ***/
    RefHashTableOf<XSerializedObjectId>*   fStorePool;  

    /***
     *   Vector for loading object, objects are NOT owned
     *
     *   data:   XSerializable*
     *           XProtoType*
     *
     ***/
    ValueVectorOf<void*>*                  fLoadPool;    

    /***
     *   object counter
     ***/
	XSerializedObjectId_t                  fObjectCount;
};

inline bool XSerializeEngine::isStoring() const
{
    return (fStoreLoad == mode_Store);
}

inline bool XSerializeEngine::isLoading() const
{
    return (fStoreLoad == mode_Load);
}

inline XSerializeEngine& operator<<(XSerializeEngine&       serEng
                                  , XSerializable* const    serObj)
{
	serEng.write(serObj); 
    return serEng; 
}

inline void XSerializeEngine::ensureStoring() const
{
	Assert(isStoring(), XMLExcepts::XSer_Storing_Violation);
}

inline void XSerializeEngine::ensureLoading() const
{
	Assert(isLoading(), XMLExcepts::XSer_Loading_Violation);
}



inline void XSerializeEngine::Assert(bool toEval
                                   , const XMLExcepts::Codes toThrow) const
{
    if (!toEval)
    {
        ThrowXML(XSerializationException, toThrow);  
    }

}

/***
 *  Ought to be nested class
 ***/
class XSerializedObjectId : public XMemory
{
public:

    ~XSerializedObjectId(){};

private:

    inline XSerializedObjectId(XSerializeEngine::XSerializedObjectId_t val) { fData = val; };

    inline XSerializeEngine::XSerializedObjectId_t getValue() const {return fData; };

    friend class XSerializeEngine;

private:
    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
	XSerializedObjectId();
    XSerializedObjectId(const XSerializedObjectId&);
	XSerializedObjectId& operator=(const XSerializedObjectId&);

    XSerializeEngine::XSerializedObjectId_t    fData;

};


XERCES_CPP_NAMESPACE_END

#endif