/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 1999-2001 The Apache Software Foundation.  All rights
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
 * Revision 1.7  2002/12/06 16:43:33  tng
 * Fix the error messages thrown from net accessor module.
 *
 * Revision 1.6  2002/12/02 20:41:17  tng
 * [Bug 12490] Patches required to build Xerces-C++ on BeOS R5.  Patch from Andrew Bachmann.
 *
 * Revision 1.5  2002/11/04 15:11:38  tng
 * C++ Namespace Support.
 *
 * Revision 1.4  2002/05/29 14:30:50  peiyongz
 * Bug9489: Malformed HTTP GET Requests in UnixHTTPUrlInputStream ,
 *                 patch from Gereon Steffens (gereon.steffens@web.de)
 *
 * Revision 1.3  2002/05/07 14:36:44  tng
 * [Bug 8852] UnixHTTPURLInputStream.cpp includes unneeded file.
 *
 * Revision 1.2  2002/03/11 21:08:23  peiyongz
 * Bug#7000: fix to corrupted URL, patch from kent@iastate.edu (Kent)
 *
 * Revision 1.1.1.1  2002/02/01 22:22:23  peiyongz
 * sane_include
 *
 * Revision 1.7  2001/09/04 17:53:09  peiyongz
 * Bugzilla# 3170: patch from Kevin Philips to handle Query in XMLURL.
 *
 * Revision 1.6  2001/06/25 16:27:04  tng
 * AS400 changes by Linda Swan.
 *
 * Revision 1.5  2000/07/21 03:31:41  andyh
 * Improved (but still weak) http access by the parser.
 *
 * Revision 1.4  2000/05/15 22:31:28  andyh
 * Replace #include<memory.h> with <string.h> everywhere.
 *
 * Revision 1.3  2000/03/24 01:30:32  rahulj
 * Connect to the port specified in the URL, rather than the default
 * one.
 *
 * Revision 1.2  2000/03/22 00:58:11  rahulj
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#if !defined(XML_BEOS)
  #include <netinet/in.h>
  #include <arpa/inet.h>
#endif
#include <netdb.h>
#include <errno.h>

#include <xercesc/util/XMLNetAccessor.hpp>
#include <xercesc/util/NetAccessors/Socket/UnixHTTPURLInputStream.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/XMLExceptMsgs.hpp>
#include <xercesc/util/Janitor.hpp>
#include <xercesc/util/XMLUniDefs.hpp>

XERCES_CPP_NAMESPACE_BEGIN


UnixHTTPURLInputStream::UnixHTTPURLInputStream(const XMLURL& urlSource)
      : fSocket(0)
      , fBytesProcessed(0)
{
    //
    // Pull all of the parts of the URL out of th urlSource object, and transcode them
    //   and transcode them back to ASCII.
    //
    const XMLCh*        hostName = urlSource.getHost();
    char*               hostNameAsCharStar = XMLString::transcode(hostName);
    ArrayJanitor<char>  janBuf1(hostNameAsCharStar);

    const XMLCh*        path = urlSource.getPath();
    char*               pathAsCharStar = XMLString::transcode(path);
    ArrayJanitor<char>  janBuf2(pathAsCharStar);

    const XMLCh*        fragment = urlSource.getFragment();
    char*               fragmentAsCharStar = 0;
    if (fragment)
        fragmentAsCharStar = XMLString::transcode(fragment);
    ArrayJanitor<char>  janBuf3(fragmentAsCharStar);

    const XMLCh*        query = urlSource.getQuery();
    char*               queryAsCharStar = 0;
    if (query)
        queryAsCharStar = XMLString::transcode(query);
    ArrayJanitor<char>  janBuf4(queryAsCharStar);		

    unsigned short      portNumber = (unsigned short) urlSource.getPortNum();

    //
    // Set up a socket.
    //
    struct hostent*     hostEntPtr = 0;
    struct sockaddr_in  sa;


    if ((hostEntPtr = gethostbyname(hostNameAsCharStar)) == NULL)
    {
        unsigned long  numAddress = inet_addr(hostNameAsCharStar);
        if (numAddress < 0)
        {
            ThrowXML1(NetAccessorException,
                     XMLExcepts::NetAcc_TargetResolution, hostName);
        }
        if ((hostEntPtr =
                gethostbyaddr((char *) &numAddress,
                              sizeof(unsigned long), AF_INET)) == NULL)
        {
            ThrowXML1(NetAccessorException,
                     XMLExcepts::NetAcc_TargetResolution, hostName);
        }
    }

    memcpy((void *) &sa.sin_addr,
           (const void *) hostEntPtr->h_addr, hostEntPtr->h_length);
    sa.sin_family = hostEntPtr->h_addrtype;
    sa.sin_port = htons(portNumber);

    int s = socket(hostEntPtr->h_addrtype, SOCK_STREAM, 0);
    if (s < 0)
    {
        ThrowXML1(NetAccessorException,
                 XMLExcepts::NetAcc_CreateSocket, urlSource.getURLText());
    }

    if (connect(s, (struct sockaddr *) &sa, sizeof(sa)) < 0)
    {
        ThrowXML1(NetAccessorException,
                 XMLExcepts::NetAcc_ConnSocket, urlSource.getURLText());
    }

    // The port is open and ready to go.
    // Build up the http GET command to send to the server.
    // To do:  We should really support http 1.1.  This implementation
    //         is weak.
    strcpy(fBuffer, "GET ");
    strcat(fBuffer, pathAsCharStar);

    if (queryAsCharStar != 0)
    {		
        size_t n = strlen(fBuffer);
        fBuffer[n] = chQuestion;
        fBuffer[n+1] = chNull;
        strcat(fBuffer, queryAsCharStar);
    }

    if (fragmentAsCharStar != 0)
    {
        strcat(fBuffer, fragmentAsCharStar);
    }
    strcat(fBuffer, " HTTP/1.0\r\n");


    strcat(fBuffer, "Host: ");
    strcat(fBuffer, hostNameAsCharStar);
    if (portNumber != 80)
    {
        int i = strlen(fBuffer);
		sprintf(fBuffer+i, ":%d", portNumber);
        // _itoa(portNumber, fBuffer+i, 10);
    }
    strcat(fBuffer, "\r\n\r\n");

    // Send the http request
    int lent = strlen(fBuffer);
    int  aLent = 0;
    if ((aLent = write(s, (void *) fBuffer, lent)) != lent)
    {
        ThrowXML1(NetAccessorException,
                 XMLExcepts::NetAcc_WriteSocket, urlSource.getURLText());
    }

    //
    // get the response, check the http header for errors from the server.
    //
    aLent = read(s, (void *)fBuffer, sizeof(fBuffer)-1);
    if (aLent <= 0)
    {
        ThrowXML1(NetAccessorException, XMLExcepts::NetAcc_ReadSocket, urlSource.getURLText());
    }

    fBufferEnd = fBuffer+aLent;
    *fBufferEnd = 0;

    // Find the break between the returned http header and any data.
    //  (Delimited by a blank line)
    // Hang on to any data for use by the first read from this BinHTTPURLInputStream.
    //
    fBufferPos = strstr(fBuffer, "\r\n\r\n");
    if (fBufferPos != 0)
    {
        fBufferPos += 4;
        *(fBufferPos-2) = 0;
    }
    else
    {
        fBufferPos = strstr(fBuffer, "\n\n");
        if (fBufferPos != 0)
        {
            fBufferPos += 2;
            *(fBufferPos-1) = 0;
        }
        else
            fBufferPos = fBufferEnd;
    }

    // Make sure the header includes an HTTP 200 OK response.
    //
    char *p = strstr(fBuffer, "HTTP");
    if (p == 0)
    {
        ThrowXML1(NetAccessorException, XMLExcepts::NetAcc_ReadSocket, urlSource.getURLText());
    }

    p = strchr(p, ' ');
    if (p == 0)
    {
        ThrowXML1(NetAccessorException, XMLExcepts::NetAcc_ReadSocket, urlSource.getURLText());
    }

    int httpResponse = atoi(p);
    if (httpResponse != 200)
    {
        // Most likely a 404 Not Found error.
        //   Should recognize and handle the forwarding responses.
        //
        ThrowXML1(NetAccessorException, XMLExcepts::File_CouldNotOpenFile, urlSource.getURLText());
    }


    fSocket = s;

}



UnixHTTPURLInputStream::~UnixHTTPURLInputStream()
{
    shutdown(fSocket, 2);
    close(fSocket);
}


unsigned int UnixHTTPURLInputStream::readBytes(XMLByte* const    toFill
                                      , const unsigned int    maxToRead)
{
    unsigned int len = fBufferEnd - fBufferPos;
    if (len > 0)
    {
        // If there's any data left over in the buffer into which we first
        //   read from the server (to get the http header), return that.
        if (len > maxToRead)
            len = maxToRead;
        memcpy(toFill, fBufferPos, len);
        fBufferPos += len;
    }
    else
    {
        // There was no data in the local buffer.
        // Read some from the socket, straight into our caller's buffer.
        //
        len = read(fSocket, (void *) toFill, maxToRead);
        if (len == -1)
        {
            ThrowXML1(NetAccessorException, XMLExcepts::NetAcc_ReadSocket, urlSource.getURLText());
        }
    }

    fBytesProcessed += len;
    return len;
}

XERCES_CPP_NAMESPACE_END

