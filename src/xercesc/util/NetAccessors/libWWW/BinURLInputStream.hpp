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

/**
 * $Log$
 * Revision 1.2  2002/11/04 15:11:39  tng
 * C++ Namespace Support.
 *
 * Revision 1.1.1.1  2002/02/01 22:22:22  peiyongz
 * sane_include
 *
 * Revision 1.2  2000/02/26 07:56:36  rahulj
 * Fixed the license header as pointed out by Bill Schindler <developer@bitranch.com>
 *
 * Revision 1.1  2000/02/17 22:06:19  rahulj
 * Moved the four LibWWW files to its own sub-directory in the
 * NetAccessor directory.
 *
 *
 * Revision 1.1  2000/01/15 01:08:04  rahulj
 * Added support for HTTP to the parser.
 * Error handling is not very good. Also cannot guarantee that
 * there are no memory leaks.
 * Only tested under NT 4.0 SP 5 using libWWW 5.2.8.
 *
 */

#if !defined(BINURLINPUTSTREAM_HPP)
#define BINURLINPUTSTREAM_HPP


#include <xercesc/util/XMLURL.hpp>
#include <xercesc/util/XMLExceptMsgs.hpp>
#include <xercesc/util/BinInputStream.hpp>

XERCES_CPP_NAMESPACE_BEGIN

//
// Forward reference the libWWW constructs here, so as to avoid including
// any of the libWWW headers in this file. Just being careful in isolating
// the files that explicitly need to include the libWWW headers.
//

struct _HTAnchor;


//
// This class implements the BinInputStream interface specified by the XML
// parser.
//

class XMLUTIL_EXPORT BinURLInputStream : public BinInputStream
{
public :
    BinURLInputStream(const XMLURL&  urlSource);
    ~BinURLInputStream();

    unsigned int curPos() const;
    unsigned int readBytes
    (
                XMLByte* const  toFill
        , const unsigned int    maxToRead
    );
    void reset();
    unsigned int bytesAvail() const;


private :
    // -----------------------------------------------------------------------
    //  Private data members
    //
    //  fAnchor
    //      This is the handle that LibWWW returns for the remote file that
    //      is being addressed.
    //  fBuffer
    //      This is the array in which the data is stored after reading it
    //      of the network. The maximum size of this array is decided in the
    //      constructor via a file specific #define.
    //  fBufferIndex
    //      Its the index into fBuffer and points to the next unprocessed
    //      character. When the parser asks for more data to be read of the
    //      stream, then fBuffer[fBufferIndex] is the first byte returned,
    //      unless fBufferIndex equals fBufferSize indicating that all
    //      data in the fBuffer has been processed.
    //  fBufferSize
    //      This represents the extent of valid data in the fBuffer array.
    //  fRemoteFileSize
    //      This stores the size in bytes of the remote file addressed by
    //      this URL input stream.
    //  fBytesProcessed
    //      Its a rolling count of the number of bytes processed off this
    //      input stream. Its only reset back to zero, if the stream is
    //      reset. The maximum value this can have is fRemoteFileSize.
    // -----------------------------------------------------------------------

    struct _HTAnchor*   fAnchor;
    XMLByte*            fBuffer;
    unsigned int        fBufferIndex;
    unsigned int        fBufferSize;
    unsigned int        fRemoteFileSize;
    unsigned int        fBytesProcessed;
};

XERCES_CPP_NAMESPACE_END

#endif // BINURLINPUTSTREAM_HPP
