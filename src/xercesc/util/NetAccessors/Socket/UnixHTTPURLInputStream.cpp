/*
 * Copyright 1999-2004 The Apache Software Foundation.
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
 * Revision 1.19  2005/03/04 19:52:54  amassari
 * The URL fragments were leaked (jira# 1362)
 *
 * Revision 1.18  2004/09/08 13:56:35  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.17  2004/05/11 13:39:35  amassari
 * The net accessor input source now can be used to get data using PUT or POST, as well as GET
 *
 * Revision 1.16  2004/02/11 12:42:22  cargilld
 * Fix for bug 25541.
 *
 * Revision 1.15  2004/01/29 11:51:20  cargilld
 * Code cleanup changes to get rid of various compiler diagnostic messages.
 *
 * Revision 1.14  2004/01/13 16:34:21  cargilld
 * Misc memory management changes.
 *
 * Revision 1.13  2003/12/24 15:24:13  cargilld
 * More updates to memory management so that the static memory manager.
 *
 * Revision 1.12  2003/05/17 05:54:18  knoaman
 * Update NetAccessors to use the memory manager.
 *
 * Revision 1.11  2003/05/16 14:03:28  knoaman
 * Pass memory manager to makeNewTranscoder.
 *
 * Revision 1.10  2003/02/25 21:22:36  tng
 * Modify UnixHTTPURLInputStream for it to work on ebcdic platform.  Patch from Steve Dulin
 *
 * Revision 1.9  2002/12/09 13:12:12  tng
 * Fix compilation error.
 *
 * Revision 1.8  2002/12/09 09:57:27  gareth
 * Fixed compile error by adding private member. Not very efficient. Should be looked at again.
 *
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
#include <xercesc/util/TransService.hpp>
#include <xercesc/util/TranscodingException.hpp>
#include <xercesc/util/PlatformUtils.hpp>

XERCES_CPP_NAMESPACE_BEGIN


UnixHTTPURLInputStream::UnixHTTPURLInputStream(const XMLURL& urlSource, const XMLNetHTTPInfo* httpInfo/*=0*/)
      : fSocket(0)
      , fBytesProcessed(0)
      , fMemoryManager(urlSource.getMemoryManager())
{

    //
    //  Constants in ASCII to send/check in the HTTP request/response
    //

    const char GET[] =
    {
        chLatin_G, chLatin_E, chLatin_T, chSpace, chNull
    };

    const char PUT[] =
    {
        chLatin_P, chLatin_U, chLatin_T, chSpace, chNull
    };

    const char POST[] =
    {
        chLatin_P, chLatin_O, chLatin_S, chLatin_T, chSpace, chNull
    };

    const char HTTP[] =
    {
        chLatin_H, chLatin_T, chLatin_T, chLatin_P, chNull
    };

    const char HTTP10[] =
    {
        chSpace, chLatin_H, chLatin_T, chLatin_T, chLatin_P, chForwardSlash, chDigit_1, chPeriod, chDigit_0, chCR, chLF, chNull
    };

    const char CRLF[] =
    {
        chCR, chLF, chNull
    };

    const char CRLF2X[] =
    {
        chCR, chLF, chCR, chLF, chNull
    };

    const char LF2X[] =
    {
        chLF, chLF, chNull
    };

    const char HOST[] =
    {
        chLatin_H, chLatin_o, chLatin_s, chLatin_t, chColon, chSpace, chNull
    };

    const char COLON[] =
    {
        chColon, chNull
    };

    const char resp200 [] =
    {
        chSpace, chDigit_2, chDigit_0, chDigit_0, chSpace, chNull
    };

    unsigned int charsEaten;
    unsigned int transSize;
    XMLTransService::Codes failReason;
    const unsigned int blockSize = 2048;
    const unsigned int bufSize = 5;
    static XMLCh portBuffer[bufSize+1];

    //
    // Pull all of the parts of the URL out of the urlSource object
    //

    const XMLCh*        hostName = urlSource.getHost();
    const XMLCh*        path = urlSource.getPath();
    const XMLCh*        fragment = urlSource.getFragment();
    const XMLCh*        query = urlSource.getQuery();                        

    //
    //  Convert the hostName to the platform's code page for gethostbyname and
    //  inet_addr functions.
    //

    char*               hostNameAsCharStar = XMLString::transcode(hostName, fMemoryManager);
    ArrayJanitor<char>  janBuf1(hostNameAsCharStar, fMemoryManager);

    //
    //  Convert all the parts of the urlSource object to ASCII so they can be
    //  sent to the remote host in that format
    //

    transSize = XMLString::stringLen(hostName)+1;
    char*               hostNameAsASCII = (char*) fMemoryManager->allocate
    (
        (transSize+1) * sizeof(char)
    );//new char[transSize+1];
    ArrayJanitor<char>  janBuf2(hostNameAsASCII, fMemoryManager);

    XMLTranscoder* trans = XMLPlatformUtils::fgTransService->makeNewTranscoderFor("ISO8859-1", failReason, blockSize, fMemoryManager);
    trans->transcodeTo(hostName, transSize, (unsigned char *) hostNameAsASCII, transSize, charsEaten, XMLTranscoder::UnRep_Throw);

    char*               pathAsASCII = 0;
    ArrayJanitor<char>  janBuf3(pathAsASCII, fMemoryManager);
    if (path)
    {
        transSize = XMLString::stringLen(path)+1;
        pathAsASCII = (char*) fMemoryManager->allocate
        (
            (transSize+1) * sizeof(char)
        );//new char[transSize+1];
        janBuf3.reset(pathAsASCII, fMemoryManager);
        trans->transcodeTo(path, transSize, (unsigned char *) pathAsASCII, transSize, charsEaten, XMLTranscoder::UnRep_Throw);
    }

    char*               fragmentAsASCII = 0;
    ArrayJanitor<char>  janBuf4(fragmentAsASCII, fMemoryManager);
    if (fragment)
    {
        transSize = XMLString::stringLen(fragment)+1;
        fragmentAsASCII = (char*) fMemoryManager->allocate
        (
            (transSize+1) * sizeof(char)
        );//new char[transSize+1];
        janBuf4.reset(fragmentAsASCII, fMemoryManager);
        trans->transcodeTo(fragment, transSize, (unsigned char *) fragmentAsASCII, transSize, charsEaten, XMLTranscoder::UnRep_Throw);
    }

    char*               queryAsASCII = 0;
    ArrayJanitor<char>  janBuf5(queryAsASCII, fMemoryManager);
    if (query)
    {
        transSize = XMLString::stringLen(query)+1;
        queryAsASCII = (char*) fMemoryManager->allocate
        (
            (transSize+1) * sizeof(char)
        );//new char[transSize+1];
        janBuf5.reset(queryAsASCII, fMemoryManager);
        trans->transcodeTo(query, transSize, (unsigned char *) queryAsASCII, transSize, charsEaten, XMLTranscoder::UnRep_Throw);
    }

    unsigned short      portNumber = (unsigned short) urlSource.getPortNum();

    //
    //  Convert port number integer to unicode so we can transcode it to ASCII
    //

    XMLString::binToText((unsigned int) portNumber, portBuffer, bufSize, 10, fMemoryManager);
    transSize = XMLString::stringLen(portBuffer)+1;
    char*               portAsASCII = (char*) fMemoryManager->allocate
    (
        (transSize+1) * sizeof(char)
    );//new char[transSize+1];
    ArrayJanitor<char>  janBuf6(portAsASCII, fMemoryManager);
    trans->transcodeTo(portBuffer, transSize, (unsigned char *) portAsASCII, transSize, charsEaten, XMLTranscoder::UnRep_Throw);

    delete trans;

    //
    // Set up a socket.
    //
    struct hostent*     hostEntPtr = 0;
    struct sockaddr_in  sa;

    // Use the hostName in the local code page ....
    if ((hostEntPtr = gethostbyname(hostNameAsCharStar)) == NULL)
    {
        unsigned long  numAddress = inet_addr(hostNameAsCharStar);
        if (numAddress < 0)
        {
            ThrowXMLwithMemMgr1(NetAccessorException,
                     XMLExcepts::NetAcc_TargetResolution, hostName, fMemoryManager);
        }
        if ((hostEntPtr =
                gethostbyaddr((char *) &numAddress,
                              sizeof(unsigned long), AF_INET)) == NULL)
        {
            ThrowXMLwithMemMgr1(NetAccessorException,
                     XMLExcepts::NetAcc_TargetResolution, hostName, fMemoryManager);
        }
    }

    memset(&sa, '\0', sizeof(sockaddr_in));  // iSeries fix ??
    memcpy((void *) &sa.sin_addr,
           (const void *) hostEntPtr->h_addr, hostEntPtr->h_length);
    sa.sin_family = hostEntPtr->h_addrtype;
    sa.sin_port = htons(portNumber);

    int s = socket(hostEntPtr->h_addrtype, SOCK_STREAM, 0);
    if (s < 0)
    {
        ThrowXMLwithMemMgr1(NetAccessorException,
                 XMLExcepts::NetAcc_CreateSocket, urlSource.getURLText(), fMemoryManager);
    }

    if (connect(s, (struct sockaddr *) &sa, sizeof(sa)) < 0)
    {
        ThrowXMLwithMemMgr1(NetAccessorException,
                 XMLExcepts::NetAcc_ConnSocket, urlSource.getURLText(), fMemoryManager);
    }

    // The port is open and ready to go.
    // Build up the http GET command to send to the server.
    // To do:  We should really support http 1.1.  This implementation
    //         is weak.
    if(httpInfo==0)
      strcpy(fBuffer, GET);
    else
      switch(httpInfo->fHTTPMethod)
      {
        case XMLNetHTTPInfo::GET:   strcpy(fBuffer, GET); break;
        case XMLNetHTTPInfo::PUT:   strcpy(fBuffer, PUT); break;
        case XMLNetHTTPInfo::POST:  strcpy(fBuffer, POST); break;
      }
    if (pathAsASCII != 0)
    {
         strcat(fBuffer, pathAsASCII);
    }

    if (queryAsASCII != 0)
    {
        size_t n = strlen(fBuffer);
        fBuffer[n] = chQuestion;
        fBuffer[n+1] = chNull;
        strcat(fBuffer, queryAsASCII);
    }

    if (fragmentAsASCII != 0)
    {
        strcat(fBuffer, fragmentAsASCII);
    }
    strcat(fBuffer, HTTP10);

    strcat(fBuffer, HOST);
    strcat(fBuffer, hostNameAsASCII);
    if (portNumber != 80)
    {
        strcat(fBuffer,COLON);
        strcat(fBuffer,portAsASCII);
    }
    strcat(fBuffer, CRLF);

    if(httpInfo!=0 && httpInfo->fHeaders!=0)
        strncat(fBuffer,httpInfo->fHeaders,httpInfo->fHeadersLen);

    strcat(fBuffer, CRLF);

    // Send the http request
    int lent = strlen(fBuffer);
    int  aLent = 0;

    if ((aLent = write(s, (void *) fBuffer, lent)) != lent)
    {
        ThrowXMLwithMemMgr1(NetAccessorException,
                 XMLExcepts::NetAcc_WriteSocket, urlSource.getURLText(), fMemoryManager);
    }

    if(httpInfo!=0 && httpInfo->fPayload!=0) {
        int  aLent = 0;
        if ((aLent = write(s, (void *) httpInfo->fPayload, httpInfo->fPayloadLen)) != httpInfo->fPayloadLen)
        {
            ThrowXMLwithMemMgr1(NetAccessorException,
                     XMLExcepts::NetAcc_WriteSocket, urlSource.getURLText(), fMemoryManager);
        }
    }

    //
    // get the response, check the http header for errors from the server.
    //
    aLent = read(s, (void *)fBuffer, sizeof(fBuffer)-1);
    if (aLent <= 0)
    {
        ThrowXMLwithMemMgr1(NetAccessorException, XMLExcepts::NetAcc_ReadSocket, urlSource.getURLText(), fMemoryManager);
    }

    fBufferEnd = fBuffer+aLent;
    *fBufferEnd = 0;

    // Find the break between the returned http header and any data.
    //  (Delimited by a blank line)
    // Hang on to any data for use by the first read from this BinHTTPURLInputStream.
    //
    fBufferPos = strstr(fBuffer, CRLF2X);
    if (fBufferPos != 0)
    {
        fBufferPos += 4;
        *(fBufferPos-2) = 0;
    }
    else
    {
        fBufferPos = strstr(fBuffer, LF2X);
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
    char *p = strstr(fBuffer, HTTP);
    if (p == 0)
    {
        ThrowXMLwithMemMgr1(NetAccessorException, XMLExcepts::NetAcc_ReadSocket, urlSource.getURLText(), fMemoryManager);
    }

    p = strchr(p, chSpace);
    if (p == 0)
    {
        ThrowXMLwithMemMgr1(NetAccessorException, XMLExcepts::NetAcc_ReadSocket, urlSource.getURLText(), fMemoryManager);
    }
  
    if (memcmp(p, resp200, strlen(resp200)))
    {
        // Most likely a 404 Not Found error.
        //   Should recognize and handle the forwarding responses.
        //
        ThrowXMLwithMemMgr1(NetAccessorException, XMLExcepts::File_CouldNotOpenFile, urlSource.getURLText(), fMemoryManager);
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
            ThrowXMLwithMemMgr(NetAccessorException, XMLExcepts::NetAcc_ReadSocket, fMemoryManager);
        }
    }

    fBytesProcessed += len;
    return len;
}

XERCES_CPP_NAMESPACE_END

