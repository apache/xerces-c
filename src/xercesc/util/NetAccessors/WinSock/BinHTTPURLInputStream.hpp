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
 * Revision 1.4  2004/01/29 11:51:20  cargilld
 * Code cleanup changes to get rid of various compiler diagnostic messages.
 *
 * Revision 1.3  2003/12/17 00:18:37  cargilld
 * Update to memory management so that the static memory manager (one used to call Initialize) is only for static data.
 *
 * Revision 1.2  2002/11/04 15:11:39  tng
 * C++ Namespace Support.
 *
 * Revision 1.1.1.1  2002/02/01 22:22:23  peiyongz
 * sane_include
 *
 * Revision 1.5  2001/10/25 16:26:39  tng
 * [Bug 4213] BinHTTPURLInputStream initialisation not thread safe.  By Mark Weaver.
 *
 * Revision 1.4  2001/01/22 16:43:39  tng
 * Loads winsock dynamically.  Fixed by Curt Arnold.
 * Winsock2 is not initialized unless an http URL is used.    If an http
 * URL is used and the Winsock 2 DLL is not installed, then an NetAccessor
 * initialization exception is thrown.
 *
 * Revision 1.3  2000/07/21 03:22:45  andyh
 * Improved (but still weak) http access by the parser.
 * Windows only.  UNIX will follow, probably tomorrow.
 *
 * Revision 1.2  2000/03/22 00:21:10  rahulj
 * Now we throw exceptions when errors occur.
 * Simplified the code, based on the assumption that
 * the calling function will make sure that the buffer into
 * which the data has to be read is large enough.
 *
 * Revision 1.1  2000/03/17 02:37:54  rahulj
 * First cut at adding HTTP capability via native sockets.
 * Still need to add:
 *   error handling capability, ports other than 80,
 *   escaped URL's
 * Will add options in project file only when I am done with these
 * above changes.
 *
 */

#if !defined(BINHTTPURLINPUTSTREAM_HPP)
#define BINHTTPURLINPUTSTREAM_HPP


#include <xercesc/util/XMLURL.hpp>
#include <xercesc/util/XMLExceptMsgs.hpp>
#include <xercesc/util/BinInputStream.hpp>
#include <xercesc/util/Mutexes.hpp>


//
// This class implements the BinInputStream interface specified by the XML
// parser.
//
struct hostent;
struct sockaddr;

XERCES_CPP_NAMESPACE_BEGIN

class XMLUTIL_EXPORT BinHTTPURLInputStream : public BinInputStream
{
public :
    BinHTTPURLInputStream(const XMLURL&  urlSource);
    ~BinHTTPURLInputStream();

    unsigned int curPos() const;
    unsigned int readBytes
    (
                XMLByte* const  toFill
        , const unsigned int    maxToRead
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
    unsigned int        fBytesProcessed;
    char                fBuffer[4000];
    char *              fBufferEnd;
    char *              fBufferPos;
    static bool         fInitialized;
    static XMLMutex*        fInitMutex;

	static void Initialize(MemoryManager* const manager  = XMLPlatformUtils::fgMemoryManager);

	inline static hostent* gethostbyname(const char* name);
	inline static unsigned long inet_addr(const char* cp);
	inline static hostent* gethostbyaddr(const char* addr,int len,int type);
	inline static unsigned short htons(unsigned short hostshort);
	inline static unsigned short socket(int af,int type,int protocol);
	inline static int connect(unsigned short s,const sockaddr* name,int namelen);
	inline static int send(unsigned short s,const char* buf,int len,int flags);
	inline static int recv(unsigned short s,char* buf,int len,int flags);
	inline static int shutdown(unsigned int s,int how);
	inline static int closesocket(unsigned int socket);
};


inline unsigned int BinHTTPURLInputStream::curPos() const
{
    return fBytesProcessed;
}

XERCES_CPP_NAMESPACE_END

#endif // BINHTTPURLINPUTSTREAM_HPP
