/*
 * The Apache Software License, Version 1.1
 * 
 * Copyright (c) 1999-2000 The Apache Software Foundation.  All rights
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
#include <util/BinMemInputStream.hpp>
#include <string.h>


// ---------------------------------------------------------------------------
//  BinMemInputStream: Constructors and Destructor
// ---------------------------------------------------------------------------
BinMemInputStream::BinMemInputStream(   const   XMLByte* const  initData
                                        , const unsigned int    capacity
                                        , const BufOpts         bufOpt) :
    fBuffer(0)
    , fBufOpt(bufOpt)
    , fCapacity(capacity)
    , fCurIndex(0)
{
    // According to the buffer option, do the right thing
    if (fBufOpt == BufOpt_Copy)
    {
        XMLByte* tmpBuf = new XMLByte[fCapacity];
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
        delete [] (XMLByte*)fBuffer;
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
