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
 * Revision 1.1  2000/03/17 02:37:54  rahulj
 * First cut at adding HTTP capability via native sockets.
 * Still need to add:
 *   error handling capability, ports other than 80,
 *   escaped URL's
 * Will add options in project file only when I am done with these
 * above changes.
 *
 */


#define _WINSOCKAPI_

#include <winsock2.h>
#include <windows.h>

#include <stdio.h>
#include <string.h>
#include <memory.h>

#include <util/XMLNetAccessor.hpp>
#include <util/NetAccessors/WinSock/BinHTTPURLInputStream.hpp>
#include <util/XMLString.hpp>
#include <util/XMLExceptMsgs.hpp>
#include <util/Janitor.hpp>



//
// This define specifies the size of the buffer used to read chunks
// out of the URL input stream.
//

#define URLISBUFMAXSIZE        8192


//
// URL's, as per the standards, is essentially composed of just ASCII characters
// and hence converting it to a 'char *' requires just to drop the leading zero
// byte. However, the URL's have to be 'escaped', meaning that certain unsafe
// and reserved characters have to be escaped to their corresponding hex values.
//
// The input Unicode string is assumed to be 0 terminated.
// The caller is responsible to free the memory allocated to store the resultant
// 'char *' string.
//

static char* localTranscode(const XMLCh* latinStrInUnicode)
{
    unsigned int   lent = XMLString::stringLen(latinStrInUnicode);
    char*  retval = new char[lent + 1];
    unsigned int  i = 0;
    for (i = 0; i < lent; i++)
        retval[i] = (char) latinStrInUnicode[i]; // drop the leading byte.
    retval[lent] = 0;
    return retval;
}



BinHTTPURLInputStream::BinHTTPURLInputStream(const XMLURL& urlSource)
      : fBuffer(0)
      , fBufferSize(0)
      , fBufferIndex(0)
      , fSocketHandle(0)
      , fBytesProcessed(0)
{
    const XMLCh*        uri = urlSource.getURLText();
    char*               uriAsCharStar = localTranscode(uri);
    ArrayJanitor<char>  janBuf(uriAsCharStar);
    const XMLCh*        hostName = urlSource.getHost();
    char*               hostNameAsCharStar = localTranscode(hostName);
    ArrayJanitor<char>  janBuf1(hostNameAsCharStar);
    struct hostent*     hostEntPtr = 0;
    struct sockaddr_in  sa;
    char                obuf[1024];  // URL's should be < 1018 bytes.


    if ((hostEntPtr = gethostbyname(hostNameAsCharStar)) == NULL)
    {
        unsigned long  numAddress = inet_addr(hostNameAsCharStar);
        if (numAddress == INADDR_NONE)
        {
            // Throw cannot connect exception here.
            // call WSAGetLastError() to get the error number.
        }
        if ((hostEntPtr = 
                gethostbyaddr((const char *) &numAddress, 
                              sizeof(unsigned long), AF_INET)) == NULL)
        {
            // Throw cannot connect exception here.
            // call WSAGetLastError() to get the error number.
        }
    }

    memcpy((void *) &sa.sin_addr,
           (const void *) hostEntPtr->h_addr, hostEntPtr->h_length);
    sa.sin_family = hostEntPtr->h_addrtype;
    sa.sin_port = htons(80);

    SOCKET s = socket(hostEntPtr->h_addrtype, SOCK_STREAM, 0);
    if (s == INVALID_SOCKET)
    {
        // Throw could not create socket exception.
        // call WSAGetLastError() to get the error number.
    }

    if (connect(s, (struct sockaddr *) &sa, sizeof(sa)) == SOCKET_ERROR)
    {
        // Throw could not connect the socket exception.
        // call WSAGetLastError() to get the error number.
    }
    
    // Now you can simply read and write from/to the socket.

    sprintf(obuf, "GET %s\n\n", uriAsCharStar);
    int lent = strlen(obuf);
    int  aLent = 0;
    if ((aLent = send(s, (const char *) obuf, lent, 0)) != lent)
    {
        // Throw could not write URL to the socket exception.
        // call WSAGetLastError() to get the error number.
        // wrote only 'aLent' bytes.
    }

    fSocketHandle = (unsigned int) s;

    fBuffer = new XMLByte[URLISBUFMAXSIZE];

}



BinHTTPURLInputStream::~BinHTTPURLInputStream()
{
    shutdown(fSocketHandle, SD_BOTH);
    closesocket(fSocketHandle);
    delete [] fBuffer;
    fBuffer = 0;
}


unsigned int BinHTTPURLInputStream::curPos() const
{
    return fBytesProcessed;
}


unsigned int BinHTTPURLInputStream::readBytes(XMLByte* const    toFill
                                    , const unsigned int    maxToRead)
{
    unsigned int  retval = 0;
    unsigned int  bytesAsked = maxToRead;
    unsigned int  bytesForCopy = 0;

    // Wipe out the old stuff from the destination buffer to fill.

    memset((void*)toFill, 0x00, sizeof(XMLByte) * maxToRead);
    
    if (fBufferSize > 0)
        bytesForCopy = fBufferSize - fBufferIndex;

    if (bytesAsked <= bytesForCopy)
    {
        // ...then you can satisfy this request completely from fBuffer.
        // Simply copy over the bytes to the destination array.
        memcpy((void*) toFill, (void*) (fBuffer + fBufferIndex), bytesAsked);
        fBufferIndex += bytesAsked;
        if (fBufferIndex >= fBufferSize)
        {
            fBufferSize = 0;
            fBufferIndex = 0;
        }
        fBytesProcessed += bytesAsked;
        retval = bytesAsked;
    }

    else
    {
        // ...will need to read some more bytes out of the stream.
        unsigned int    bufToFillIndex = 0;

        // First copy over what is left in fBuffer, before reading another
        // chunk out of the stream.

        if (bytesForCopy != 0)
        {
            memcpy((void*) toFill, (void*) (fBuffer + fBufferSize), bytesForCopy);
            fBufferSize = 0;
            fBufferIndex = 0;
            fBytesProcessed += bytesForCopy;
            bufToFillIndex = bytesForCopy;
            retval = bytesForCopy;
        }

        unsigned int    bytesRemainingToFill = bytesAsked - bytesForCopy;

        // Now blow away the internal buffer and read next 'chunk' into it
        // from the stream.

        memset((void*) fBuffer, 0x00, URLISBUFMAXSIZE);
        int lent = recv((SOCKET) fSocketHandle,
                        (char *) fBuffer,
                        URLISBUFMAXSIZE - 1, 0);

        if (lent == SOCKET_ERROR)
        {
            // Throw socket error exception.
            // call WSAGetLastError() to get the error number.
        }
        else if (lent == 0)
        {
            // No more data is available.
            fBufferSize = 0;
            fBufferIndex = 0;
        }
        else
        {
            fBufferIndex = 0;
            fBufferSize = lent;

            // Now fill the destination buffer with the new data just read.

            if (bytesRemainingToFill > fBufferSize)
            {
                bytesRemainingToFill = fBufferSize;
            }
            memcpy((void*) (toFill + bufToFillIndex),
                   (void*) fBuffer,
                   bytesRemainingToFill);

            // Update counters.
            retval += bytesRemainingToFill;
            fBufferIndex += bytesRemainingToFill;
            fBytesProcessed += bytesRemainingToFill;

        }

    }

    return retval;
}
