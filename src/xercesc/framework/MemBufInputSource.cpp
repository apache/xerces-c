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

/**
 * $Log$
 * Revision 1.7  2004/09/28 02:14:13  cargilld
 * Add support for validating annotations.
 *
 * Revision 1.6  2004/09/08 13:55:57  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.5  2003/05/16 21:36:55  knoaman
 * Memory manager implementation: Modify constructors to pass in the memory manager.
 *
 * Revision 1.4  2003/05/16 03:11:30  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.3  2003/05/15 18:26:07  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.2  2002/11/04 15:00:21  tng
 * C++ Namespace Support.
 *
 * Revision 1.1.1.1  2002/02/01 22:21:50  peiyongz
 * sane_include
 *
 * Revision 1.2  2000/02/06 07:47:46  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.1  2000/01/12 00:13:26  roddey
 * These were moved from internal/ to framework/, which was something that should have
 * happened long ago. They are really framework type of classes.
 *
 * Revision 1.1.1.1  1999/11/09 01:08:10  twl
 * Initial checkin
 *
 * Revision 1.2  1999/11/08 20:44:42  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/util/BinMemInputStream.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  MemBufInputSource: Constructors and Destructor
// ---------------------------------------------------------------------------
MemBufInputSource::MemBufInputSource( const XMLByte* const  srcDocBytes
                                    , const unsigned int    byteCount
                                    , const XMLCh* const    bufId
                                    , const bool            adoptBuffer
                                    , MemoryManager* const  manager) :
    InputSource(bufId, manager)
    , fAdopted(adoptBuffer)
    , fByteCount(byteCount)
    , fCopyBufToStream(true)
    , fSrcBytes(srcDocBytes)
{
}

MemBufInputSource::MemBufInputSource( const XMLByte* const  srcDocBytes
                                    , const unsigned int    byteCount
                                    , const char* const     bufId
                                    , const bool            adoptBuffer
                                    , MemoryManager* const  manager) :
    InputSource(bufId, manager)
    , fAdopted(adoptBuffer)
    , fByteCount(byteCount)
    , fCopyBufToStream(true)
    , fSrcBytes(srcDocBytes)
{
}

MemBufInputSource::~MemBufInputSource()
{
    if (fAdopted)
        delete [] (XMLByte*)fSrcBytes;
}

void MemBufInputSource::resetMemBufInputSource(const   XMLByte* const  srcDocBytes
                                , const unsigned int    byteCount)
{
    fByteCount = byteCount;
    fSrcBytes  = srcDocBytes;
}

// ---------------------------------------------------------------------------
//  MemBufInputSource: InputSource interface implementation
// ---------------------------------------------------------------------------
BinInputStream* MemBufInputSource::makeStream() const
{
    //
    //  Create a memory input stream over our buffer. According to our
    //  fCopyBufToStream flag, we either tell it to copy the buffer or to
    //  just reference it.
    //
    return new (getMemoryManager()) BinMemInputStream
    (
        fSrcBytes
        , fByteCount
        , fCopyBufToStream ? BinMemInputStream::BufOpt_Copy
                           : BinMemInputStream::BufOpt_Reference
        , getMemoryManager()
    );
}

XERCES_CPP_NAMESPACE_END

