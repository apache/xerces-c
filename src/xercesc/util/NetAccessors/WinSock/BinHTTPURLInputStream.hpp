/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
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
 * $Id$
 */

#if !defined(XERCESC_INCLUDE_GUARD_BINHTTPURLINPUTSTREAM_HPP)
#define XERCESC_INCLUDE_GUARD_BINHTTPURLINPUTSTREAM_HPP


#include <xercesc/util/XMLURL.hpp>
#include <xercesc/util/XMLExceptMsgs.hpp>
#include <xercesc/util/BinInputStream.hpp>
#include <xercesc/util/Mutexes.hpp>
#include <xercesc/util/XMLNetAccessor.hpp>

//
// This class implements the BinInputStream interface specified by the XML
// parser.
//

XERCES_CPP_NAMESPACE_BEGIN

class XMLUTIL_EXPORT BinHTTPURLInputStream : public BinInputStream
{
public :
    BinHTTPURLInputStream(const XMLURL&  urlSource, const XMLNetHTTPInfo* httpInfo=0);
    ~BinHTTPURLInputStream();

    XMLFilePos curPos() const;
    XMLSize_t readBytes
    (
                XMLByte* const  toFill
        , const XMLSize_t       maxToRead
    );

	static void Cleanup();


private :
    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    BinHTTPURLInputStream(const BinHTTPURLInputStream&);
    BinHTTPURLInputStream& operator=(const BinHTTPURLInputStream&); 
    // -----------------------------------------------------------------------
    //  Private data members
    //
    //  fSocketHandle
    //      The socket representing the connection to the remote file.
    //      We deliberately did not define the type to be SOCKET, so as to
    //      avoid bringing in any Windows header into this file.
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
    MemoryManager*      fMemoryManager;
    unsigned int        fSocketHandle;
    XMLSize_t           fBytesProcessed;
    char                fBuffer[4000];
    char *              fBufferEnd;
    char *              fBufferPos;
    static bool         fInitialized;
    static XMLMutex*    fInitMutex;

	static void Initialize(MemoryManager* const manager  = XMLPlatformUtils::fgMemoryManager);

};


inline XMLFilePos BinHTTPURLInputStream::curPos() const
{
    return fBytesProcessed;
}

XERCES_CPP_NAMESPACE_END

#endif // BINHTTPURLINPUTSTREAM_HPP
