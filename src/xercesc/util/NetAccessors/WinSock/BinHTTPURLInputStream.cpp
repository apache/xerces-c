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
 * Revision 1.3  2002/12/06 16:43:55  tng
 * Fix the error messages thrown from net accessor module.
 *
 * Revision 1.2  2002/11/04 15:11:39  tng
 * C++ Namespace Support.
 *
 * Revision 1.1.1.1  2002/02/01 22:22:23  peiyongz
 * sane_include
 *
 * Revision 1.12  2001/12/13 21:16:52  peiyongz
 * Fix: Invalid Argument to FreeLibrary (Hint: 0x0000000)
 *
 * Revision 1.11  2001/10/25 16:26:39  tng
 * [Bug 4213] BinHTTPURLInputStream initialisation not thread safe.  By Mark Weaver.
 *
 * Revision 1.10  2001/10/25 16:10:46  tng
 * [Bug 4121] BinHTTPUrlInputStream needds to read entire HTTP header. By John Clayton.
 *
 * Revision 1.9  2001/10/24 20:17:54  tng
 * [Bug 3813] BinHTTPURLInputStream has weak HTTP request capabilities.  By Kevin Philips.
 *
 * Revision 1.8  2001/10/24 20:03:03  tng
 * [Bug 2305] Include <stdlib.h> to BinHTTPURLInputStream.cpp.  By Peter A. Volchek.
 *
 * Revision 1.7  2001/09/04 17:52:57  peiyongz
 * Bugzilla# 3170: patch from Kevin Philips to handle Query in XMLURL.
 *
 * Revision 1.6  2001/01/22 16:43:38  tng
 * Loads winsock dynamically.  Fixed by Curt Arnold.
 * Winsock2 is not initialized unless an http URL is used.    If an http
 * URL is used and the Winsock 2 DLL is not installed, then an NetAccessor
 * initialization exception is thrown.
 *
 * Revision 1.5  2000/07/21 03:22:44  andyh
 * Improved (but still weak) http access by the parser.
 * Windows only.  UNIX will follow, probably tomorrow.
 *
 * Revision 1.4  2000/05/15 22:31:29  andyh
 * Replace #include<memory.h> with <string.h> everywhere.
 *
 * Revision 1.3  2000/03/24 00:32:15  rahulj
 * Connect to the port specified in the URL, rather than the default one.
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


#define _WINSOCKAPI_

#define INCL_WINSOCK_API_TYPEDEFS 1
#include <winsock2.h>
#include <windows.h>
#include <tchar.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLNetAccessor.hpp>
#include <xercesc/util/NetAccessors/WinSock/BinHTTPURLInputStream.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/XMLExceptMsgs.hpp>
#include <xercesc/util/Janitor.hpp>
#include <xercesc/util/XMLUniDefs.hpp>

XERCES_CPP_NAMESPACE_BEGIN

HMODULE gWinsockLib = NULL;
LPFN_GETHOSTBYNAME gWSgethostbyname = NULL;
LPFN_INET_ADDR gWSinet_addr = NULL;
LPFN_GETHOSTBYADDR gWSgethostbyaddr = NULL;
LPFN_HTONS gWShtons = NULL;
LPFN_SOCKET gWSsocket = NULL;
LPFN_CONNECT gWSconnect = NULL;
LPFN_SEND gWSsend = NULL;
LPFN_RECV gWSrecv = NULL;
LPFN_SHUTDOWN gWSshutdown = NULL;
LPFN_CLOSESOCKET gWSclosesocket = NULL;
LPFN_WSACLEANUP gWSACleanup = NULL;

bool BinHTTPURLInputStream::fInitialized = false;
XMLMutex* BinHTTPURLInputStream::fInitMutex = 0;

void BinHTTPURLInputStream::Initialize() {
    //
    // Initialize the WinSock library here.
    //
    WORD        wVersionRequested;
    WSADATA     wsaData;

	LPFN_WSASTARTUP startup = NULL;
	if(gWinsockLib == NULL) {
		gWinsockLib = LoadLibrary(_T("WSOCK32"));
		if(gWinsockLib == NULL) {
			ThrowXML(NetAccessorException, XMLExcepts::NetAcc_InitFailed);
		}
		else {
			startup = (LPFN_WSASTARTUP) GetProcAddress(gWinsockLib,_T("WSAStartup"));
			gWSACleanup = (LPFN_WSACLEANUP) GetProcAddress(gWinsockLib,_T("WSACleanup"));
			gWSgethostbyname = (LPFN_GETHOSTBYNAME) GetProcAddress(gWinsockLib,_T("gethostbyname"));
			gWSinet_addr = (LPFN_INET_ADDR) GetProcAddress(gWinsockLib,_T("inet_addr"));
			gWSgethostbyaddr = (LPFN_GETHOSTBYADDR) GetProcAddress(gWinsockLib,_T("gethostbyaddr"));
			gWShtons = (LPFN_HTONS) GetProcAddress(gWinsockLib,_T("htons"));
			gWSsocket = (LPFN_SOCKET) GetProcAddress(gWinsockLib,_T("socket"));
			gWSconnect = (LPFN_CONNECT) GetProcAddress(gWinsockLib,_T("connect"));
			gWSsend = (LPFN_SEND) GetProcAddress(gWinsockLib,_T("send"));
			gWSrecv = (LPFN_RECV) GetProcAddress(gWinsockLib,_T("recv"));
			gWSshutdown = (LPFN_SHUTDOWN) GetProcAddress(gWinsockLib,_T("shutdown"));
			gWSclosesocket = (LPFN_CLOSESOCKET) GetProcAddress(gWinsockLib,_T("closesocket"));

			if(startup == NULL ||
				gWSACleanup == NULL ||
				gWSgethostbyname == NULL ||
				gWSinet_addr == NULL ||
				gWSgethostbyaddr == NULL ||
				gWShtons == NULL ||
				gWSsocket == NULL ||
				gWSconnect == NULL ||
				gWSsend == NULL ||
				gWSrecv == NULL ||
				gWSshutdown == NULL ||
				gWSclosesocket == NULL)
			{
				gWSACleanup = NULL;
				Cleanup();
				ThrowXML(NetAccessorException, XMLExcepts::NetAcc_InitFailed);
			}
		}
	}
    wVersionRequested = MAKEWORD( 2, 2 );
    int err = (*startup)(wVersionRequested, &wsaData);
    if (err != 0)
    {
        // Call WSAGetLastError() to get the last error.
        ThrowXML(NetAccessorException, XMLExcepts::NetAcc_InitFailed);
    }
    fInitialized = true;
}

void BinHTTPURLInputStream::Cleanup() {
	if(fInitialized)
	{
		if(gWSACleanup) (*gWSACleanup)();
		gWSACleanup = NULL;
		FreeLibrary(gWinsockLib);
		gWinsockLib = NULL;
		gWSgethostbyname = NULL;
		gWSinet_addr = NULL;
		gWSgethostbyaddr = NULL;
		gWShtons = NULL;
		gWSsocket = NULL;
		gWSconnect = NULL;
		gWSsend = NULL;
		gWSrecv = NULL;
		gWSshutdown = NULL;
		gWSclosesocket = NULL;

      fInitialized = false;
      delete fInitMutex;
      fInitMutex = 0;
	}
}


hostent* BinHTTPURLInputStream::gethostbyname(const char* name)
{
	return (*gWSgethostbyname)(name);
}

unsigned long BinHTTPURLInputStream::inet_addr(const char* cp)
{
	return (*gWSinet_addr)(cp);
}

hostent* BinHTTPURLInputStream::gethostbyaddr(const char* addr,int len,int type)
{
	return (*gWSgethostbyaddr)(addr,len,type);
}

unsigned short BinHTTPURLInputStream::htons(unsigned short hostshort)
{
	return (*gWShtons)(hostshort);
}

unsigned short BinHTTPURLInputStream::socket(int af,int type,int protocol)
{
	return (*gWSsocket)(af,type,protocol);
}

int BinHTTPURLInputStream::connect(unsigned short s,const sockaddr* name,int namelen)
{
	return (*gWSconnect)(s,name,namelen);
}

int BinHTTPURLInputStream::send(unsigned short s,const char* buf,int len,int flags)
{
	return (*gWSsend)(s,buf,len,flags);
}

int BinHTTPURLInputStream::recv(unsigned short s,char* buf,int len,int flags)
{
	return (*gWSrecv)(s,buf,len,flags);
}

int BinHTTPURLInputStream::shutdown(unsigned int s,int how)
{
	return (*gWSshutdown)(s,how);
}

int BinHTTPURLInputStream::closesocket(unsigned int socket)
{
	return (*gWSclosesocket)(socket);
}


BinHTTPURLInputStream::BinHTTPURLInputStream(const XMLURL& urlSource)
      : fSocketHandle(0)
      , fBytesProcessed(0)
{
    if(!fInitialized)
    {
        if (!fInitMutex)
        {
            XMLMutex* tmpMutex = new XMLMutex;
            if (XMLPlatformUtils::compareAndSwap((void**)&fInitMutex, tmpMutex, 0))
            {
                // Someone beat us to it, so let's clean up ours
                delete tmpMutex;
            }
         }
         XMLMutexLock lock(fInitMutex);
         if (!fInitialized)
         {
             Initialize();
         }
    }

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
        if (numAddress == INADDR_NONE)
        {
            // Call WSAGetLastError() to get the error number.
            ThrowXML1(NetAccessorException,
                     XMLExcepts::NetAcc_TargetResolution, hostName);
        }
        if ((hostEntPtr =
                gethostbyaddr((const char *) &numAddress,
                              sizeof(unsigned long), AF_INET)) == NULL)
        {
            // Call WSAGetLastError() to get the error number.
            ThrowXML1(NetAccessorException,
                     XMLExcepts::NetAcc_TargetResolution, hostName);
        }
    }

    memcpy((void *) &sa.sin_addr,
           (const void *) hostEntPtr->h_addr, hostEntPtr->h_length);
    sa.sin_family = hostEntPtr->h_addrtype;
    sa.sin_port = htons(portNumber);

    SOCKET s = socket(hostEntPtr->h_addrtype, SOCK_STREAM, 0);
    if (s == INVALID_SOCKET)
    {
        // Call WSAGetLastError() to get the error number.
        ThrowXML1(NetAccessorException,
                 XMLExcepts::NetAcc_CreateSocket, urlSource.getURLText());
    }

    if (connect(s, (struct sockaddr *) &sa, sizeof(sa)) == SOCKET_ERROR)
    {
        // Call WSAGetLastError() to get the error number.
        ThrowXML1(NetAccessorException,
                 XMLExcepts::NetAcc_ConnSocket, urlSource.getURLText());
    }


    // Set a flag so we know that the headers have not been read yet.
    bool fHeaderRead = false;

    // The port is open and ready to go.
    // Build up the http GET command to send to the server.
    // To do:  We should really support http 1.1.  This implementation
    //         is weak.

    memset(fBuffer, 0, sizeof(fBuffer));

    strcpy(fBuffer, "GET ");
    strcat(fBuffer, pathAsCharStar);

    if (queryAsCharStar != 0)
    {
        // Tack on a ? before the fragment
        strcat(fBuffer,"?");
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
        strcat(fBuffer, ":");
        int i = strlen(fBuffer);
        _itoa(portNumber, fBuffer+i, 10);
    }
    strcat(fBuffer, "\r\n\r\n");

    // Send the http request
    int lent = strlen(fBuffer);
    int  aLent = 0;
    if ((aLent = send(s, fBuffer, lent, 0)) != lent)
    {
        // Call WSAGetLastError() to get the error number.
        ThrowXML1(NetAccessorException,
                 XMLExcepts::NetAcc_WriteSocket, urlSource.getURLText());
    }


    //
    // get the response, check the http header for errors from the server.
    //
    memset(fBuffer, 0, sizeof(fBuffer));
    aLent = recv(s, fBuffer, sizeof(fBuffer)-1, 0);
    if (aLent == SOCKET_ERROR || aLent == 0)
    {
        // Call WSAGetLastError() to get the error number.
        ThrowXML1(NetAccessorException, XMLExcepts::NetAcc_ReadSocket, urlSource.getURLText());
    }

    fBufferEnd = fBuffer+aLent;
    *fBufferEnd = 0;

    do {
        // Find the break between the returned http header and any data.
        //  (Delimited by a blank line)
        // Hang on to any data for use by the first read from this BinHTTPURLInputStream.
        //
        fBufferPos = strstr(fBuffer, "\r\n\r\n");
        if (fBufferPos != 0)
        {
            fBufferPos += 4;
            *(fBufferPos-2) = 0;
            fHeaderRead = true;
        }
        else
        {
            fBufferPos = strstr(fBuffer, "\n\n");
            if (fBufferPos != 0)
            {
                fBufferPos += 2;
                *(fBufferPos-1) = 0;
                fHeaderRead = true;
            }
            else
            {
                //
                // Header is not yet read, do another recv() to get more data...
                aLent = recv(s, fBufferEnd, (sizeof(fBuffer) - 1) - (fBufferEnd - fBuffer), 0);
                if (aLent == SOCKET_ERROR || aLent == 0)
                {
                    // Call WSAGetLastError() to get the error number.
                    ThrowXML1(NetAccessorException, XMLExcepts::NetAcc_ReadSocket, urlSource.getURLText());
                }
                fBufferEnd = fBufferEnd + aLent;
                *fBufferEnd = 0;
            }
        }
    } while(fHeaderRead == false);

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

    fSocketHandle = (unsigned int) s;
}



BinHTTPURLInputStream::~BinHTTPURLInputStream()
{
    shutdown(fSocketHandle, SD_BOTH);
    closesocket(fSocketHandle);
}


//
//  readBytes
//
unsigned int BinHTTPURLInputStream::readBytes(XMLByte* const    toFill
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
        len = recv((SOCKET) fSocketHandle, (char *) toFill, maxToRead, 0);
        if (len == SOCKET_ERROR)
        {
            // Call WSAGetLastError() to get the error number.
            ThrowXML(NetAccessorException, XMLExcepts::NetAcc_ReadSocket);
        }
    }

    fBytesProcessed += len;
    return len;
}


XERCES_CPP_NAMESPACE_END
