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
 * Revision 1.7  2004/09/08 13:56:36  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.6  2004/05/05 22:08:07  amassari
 * Content reported by a web site as text/xml is now read correctly; added an extra way of getting the size of the document in case no redirection has been performed
 *
 * Revision 1.5  2004/01/29 11:51:20  cargilld
 * Code cleanup changes to get rid of various compiler diagnostic messages.
 *
 * Revision 1.4  2004/01/15 16:07:16  amassari
 * Fix for bug#24929; patch by Michael Wuschek
 *
 * Revision 1.3  2003/05/17 05:54:18  knoaman
 * Update NetAccessors to use the memory manager.
 *
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

//
// Forward reference the libWWW constructs here, so as to avoid including
// any of the libWWW headers in this file. Just being careful in isolating
// the files that explicitly need to include the libWWW headers.
//

struct _HTAnchor;

XERCES_CPP_NAMESPACE_BEGIN

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
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    BinURLInputStream(const BinURLInputStream&);
    BinURLInputStream& operator=(const BinURLInputStream&); 

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
    int                 fRemoteFileSize;
    unsigned int        fBytesProcessed;
    MemoryManager*      fMemoryManager;
};

XERCES_CPP_NAMESPACE_END

#endif // BINURLINPUTSTREAM_HPP
