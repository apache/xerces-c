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
 * Revision 1.4  2004/09/08 13:56:21  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.3  2003/05/16 03:11:22  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.2  2002/11/04 15:22:03  tng
 * C++ Namespace Support.
 *
 * Revision 1.1.1.1  2002/02/01 22:22:10  peiyongz
 * sane_include
 *
 * Revision 1.5  2001/11/28 19:05:29  knoaman
 * Bug 4442: fix by mlovett@uk.ibm.com
 *
 * Revision 1.4  2000/05/15 22:31:20  andyh
 * Replace #include<memory.h> with <string.h> everywhere.
 *
 * Revision 1.3  2000/03/02 19:54:38  roddey
 * This checkin includes many changes done while waiting for the
 * 1.1.0 code to be finished. I can't list them all here, but a list is
 * available elsewhere.
 *
 * Revision 1.2  2000/02/06 07:48:01  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.1.1.1  1999/11/09 01:04:05  twl
 * Initial checkin
 *
 * Revision 1.2  1999/11/08 20:45:04  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/util/BinMemInputStream.hpp>
#include <xercesc/framework/MemoryManager.hpp>
#include <string.h>

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  BinMemInputStream: Constructors and Destructor
// ---------------------------------------------------------------------------
BinMemInputStream::BinMemInputStream( const XMLByte* const       initData
                                    , const unsigned int         capacity
                                    , const BufOpts              bufOpt
                                    ,       MemoryManager* const manager) :
    fBuffer(0)
    , fBufOpt(bufOpt)
    , fCapacity(capacity)
    , fCurIndex(0)
    , fMemoryManager(manager)
{
    // According to the buffer option, do the right thing
    if (fBufOpt == BufOpt_Copy)
    {
        XMLByte* tmpBuf = (XMLByte*) fMemoryManager->allocate
        (
            fCapacity * sizeof(XMLByte)
        );//new XMLByte[fCapacity];
        memcpy(tmpBuf, initData, capacity);
        fBuffer = tmpBuf;
    }
     else
    {
        fBuffer = initData;
    }
}

BinMemInputStream::~BinMemInputStream()
{
    //
    //  If we adopted or copied the buffer, then clean it up. We have to
    //  cast off the const'ness in that case in order to delete it.
    //
    if ((fBufOpt == BufOpt_Adopt) || (fBufOpt == BufOpt_Copy))
        fMemoryManager->deallocate((XMLByte*)fBuffer);//delete [] (XMLByte*)fBuffer;
}


// ---------------------------------------------------------------------------
//  MemBinInputStream: Implementation of the input stream interface
// ---------------------------------------------------------------------------
unsigned int BinMemInputStream::readBytes(          XMLByte* const  toFill
                                            , const unsigned int    maxToRead)
{
    //
    //  Figure out how much we can really read. Its the smaller of the
    //  amount available and the amount asked for.
    //
    const unsigned int available = (fCapacity - fCurIndex);
    if (!available)
        return 0;

    const unsigned int actualToRead = available < maxToRead ?
                                      available : maxToRead;

    memcpy(toFill, &fBuffer[fCurIndex], actualToRead);
    fCurIndex += actualToRead;
    return actualToRead;
}

XERCES_CPP_NAMESPACE_END
