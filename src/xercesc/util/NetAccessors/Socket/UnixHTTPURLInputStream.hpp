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
 * Revision 1.8  2004/09/08 13:56:35  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.7  2004/05/11 13:39:35  amassari
 * The net accessor input source now can be used to get data using PUT or POST, as well as GET
 *
 * Revision 1.6  2004/01/29 11:51:20  cargilld
 * Code cleanup changes to get rid of various compiler diagnostic messages.
 *
 * Revision 1.5  2003/12/24 15:24:13  cargilld
 * More updates to memory management so that the static memory manager.
 *
 * Revision 1.4  2002/12/09 13:12:12  tng
 * Fix compilation error.
 *
 * Revision 1.3  2002/12/09 09:57:27  gareth
 * Fixed compile error by adding private member. Not very efficient. Should be looked at again.
 *
 * Revision 1.2  2002/11/04 15:11:38  tng
 * C++ Namespace Support.
 *
 * Revision 1.1.1.1  2002/02/01 22:22:23  peiyongz
 * sane_include
 *
 * Revision 1.3  2000/07/21 03:31:42  andyh
 * Improved (but still weak) http access by the parser.
 *
 * Revision 1.2  2000/03/22 00:58:12  rahulj
 * Now we throw exceptions when errors occur.
 * Simplified code based on assumption that calling
 * function will allocate enough storage to store the
 * incoming data.
 *
 * Revision 1.1  2000/03/20 23:48:51  rahulj
 * Added Socket based NetAccessor. This will enable one to
 * use HTTP URL's for system id's. Default build options do
 * not use this NetAccessor. Specify the '-n socket' option
 * to 'runConfigure' to configure Xerces-C to use this new
 * feature. The code works under Solaris 2.6, Linux, AIX
 * and HPUX 11 with aCC.
 * Todo's: enable proper error handling.
 *
 */

#if !defined(UNIXHTTPURLINPUTSTREAM_HPP)
#define UNIXHTTPURLINPUTSTREAM_HPP


#include <xercesc/util/XMLURL.hpp>
#include <xercesc/util/XMLExceptMsgs.hpp>
#include <xercesc/util/BinInputStream.hpp>
#include <xercesc/util/XMLNetAccessor.hpp>

XERCES_CPP_NAMESPACE_BEGIN

//
// This class implements the BinInputStream interface specified by the XML
// parser.
//

class XMLUTIL_EXPORT UnixHTTPURLInputStream : public BinInputStream
{
public :
    UnixHTTPURLInputStream(const XMLURL&  urlSource, const XMLNetHTTPInfo* httpInfo=0);
    ~UnixHTTPURLInputStream();

    unsigned int curPos() const;
    unsigned int readBytes
    (
                XMLByte* const  toFill
        , const unsigned int    maxToRead
    );


private :
    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    UnixHTTPURLInputStream(const UnixHTTPURLInputStream&);
    UnixHTTPURLInputStream& operator=(const UnixHTTPURLInputStream&);

    // -----------------------------------------------------------------------
    //  Private data members
    //
    //  fSocket
    //      The socket representing the connection to the remote file.
    //  fBytesProcessed
    //      Its a rolling count of the number of bytes processed off this
    //      input stream.
    //  fBuffer
    //      Holds the http header, plus the first part of the actual
    //      data.  Filled at the time the stream is opened, data goes
    //      out to user in response to readBytes().
    //  fBufferPos, fBufferEnd
    //      Pointers into fBuffer, showing start and end+1 of content
    //      that readBytes must return.
    // -----------------------------------------------------------------------

    int                 fSocket;
    unsigned int        fBytesProcessed;
    char                fBuffer[4000];
    char *              fBufferEnd;
    char *              fBufferPos;
    MemoryManager*      fMemoryManager;
}; // UnixHTTPURLInputStream


inline unsigned int UnixHTTPURLInputStream::curPos() const
{
    return fBytesProcessed;
}

XERCES_CPP_NAMESPACE_END

#endif // UNIXHTTPURLINPUTSTREAM_HPP
