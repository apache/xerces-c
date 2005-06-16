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
 * Revision 1.11  2005/02/02 10:29:52  amassari
 * Mark global variables for this module as static (jira# 1331)
 *
 * Revision 1.10  2005/01/06 21:39:44  amassari
 * Removed warnings
 *
 * Revision 1.9  2004/09/08 13:56:36  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.8  2004/05/11 13:39:35  amassari
 * The net accessor input source now can be used to get data using PUT or POST, as well as GET
 *
 * Revision 1.7  2003/12/17 13:58:02  cargilld
 * Platform update for memory management so that the static memory manager (one
 * used to call Initialize) is only for static data.
 *
 * Revision 1.6  2003/12/17 00:18:37  cargilld
 * Update to memory management so that the static memory manager (one used to call Initialize) is only for static data.
 *
 * Revision 1.5  2003/12/02 14:12:20  amassari
 * Make the code compilable on Windows when UNICODE is defined (bug#16055)
 *
 * Revision 1.4  2003/05/17 05:54:18  knoaman
 * Update NetAccessors to use the memory manager.
 *
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

typedef struct hostent *(WSAAPI * LPFN_GETHOSTBYNAME)(const char* name);
typedef unsigned long(WSAAPI * LPFN_INET_ADDR)(const char* cp);
typedef struct hostent *(WSAAPI * LPFN_GETHOSTBYADDR)(const char* addr, int len, int type);
typedef u_short (WSAAPI * LPFN_HTONS)(u_short hostshort);
typedef SOCKET (WSAAPI * LPFN_SOCKET)(int af, int type, int protocol);
typedef int (WSAAPI * LPFN_CONNECT)(SOCKET s, const struct sockaddr* name, int namelen);
typedef int (WSAAPI * LPFN_SEND)(SOCKET s, const char* buf, int len, int flags);
typedef int (WSAAPI * LPFN_RECV)(SOCKET s, char* buf, int len, int flags);
typedef int (WSAAPI * LPFN_SHUTDOWN)(SOCKET s, int how);
typedef int (WSAAPI * LPFN_CLOSESOCKET)(SOCKET s);
typedef int (WSAAPI * LPFN_WSACLEANUP)();
typedef int (WSAAPI * LPFN_WSASTARTUP)(WORD wVersionRequested, LPWSADATA lpWSAData);

static HMODULE gWinsockLib = NULL;
static LPFN_GETHOSTBYNAME gWSgethostbyname = NULL;
static LPFN_INET_ADDR gWSinet_addr = NULL;
static LPFN_GETHOSTBYADDR gWSgethostbyaddr = NULL;
static LPFN_HTONS gWShtons = NULL;
static LPFN_SOCKET gWSsocket = NULL;
static LPFN_CONNECT gWSconnect = NULL;
static LPFN_SEND gWSsend = NULL;
static LPFN_RECV gWSrecv = NULL;
static LPFN_SHUTDOWN gWSshutdown = NULL;
static LPFN_CLOSESOCKET gWSclosesocket = NULL;
static LPFN_WSACLEANUP gWSACleanup = NULL;

bool BinHTTPURLInputStream::fInitialized = false;
XMLMutex* BinHTTPURLInputStream::fInitMutex = 0;

void BinHTTPURLInputStream::Initialize(MemoryManager* const manager) {
    //
    // Initialize the WinSock library here.
    //
    WORD        wVersionRequested;
    WSADATA     wsaData;

	LPFN_WSASTARTUP startup = NULL;
	if(gWinsockLib == NULL) {
		gWinsockLib = LoadLibrary(_T("WSOCK32"));
		if(gWinsockLib == NULL) {
			ThrowXMLwithMemMgr(NetAccessorException, XMLExcepts::NetAcc_InitFailed, manager);
		}
		else {
			startup = (LPFN_WSASTARTUP) GetProcAddress(gWinsockLib,"WSAStartup");
			gWSACleanup = (LPFN_WSACLEANUP) GetProcAddress(gWinsockLib,"WSACleanup");
			gWSgethostbyname = (LPFN_GETHOSTBYNAME) GetProcAddress(gWinsockLib,"gethostbyname");
			gWSinet_addr = (LPFN_INET_ADDR) GetProcAddress(gWinsockLib,"inet_addr");
			gWSgethostbyaddr = (LPFN_GETHOSTBYADDR) GetProcAddress(gWinsockLib,"gethostbyaddr");
			gWShtons = (LPFN_HTONS) GetProcAddress(gWinsockLib,"htons");
			gWSsocket = (LPFN_SOCKET) GetProcAddress(gWinsockLib,"socket");
			gWSconnect = (LPFN_CONNECT) GetProcAddress(gWinsockLib,"connect");
			gWSsend = (LPFN_SEND) GetProcAddress(gWinsockLib,"send");
			gWSrecv = (LPFN_RECV) GetProcAddress(gWinsockLib,"recv");
			gWSshutdown = (LPFN_SHUTDOWN) GetProcAddress(gWinsockLib,"shutdown");
			gWSclosesocket = (LPFN_CLOSESOCKET) GetProcAddress(gWinsockLib,"closesocket");

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
				ThrowXMLwithMemMgr(NetAccessorException, XMLExcepts::NetAcc_InitFailed, manager);
			}
		}
	}
    wVersionRequested = MAKEWORD( 2, 2 );
    int err = (*startup)(wVersionRequested, &wsaData);
    if (err != 0)
    {
        // Call WSAGetLastError() to get the last error.
        ThrowXMLwithMemMgr(NetAccessorException, XMLExcepts::NetAcc_InitFailed, manager);
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

unsigned int BinHTTPURLInputStream::socket(int af,int type,int protocol)
{
	return (*gWSsocket)(af,type,protocol);
}

int BinHTTPURLInputStream::connect(unsigned int s,const sockaddr* name,int namelen)
{
	return (*gWSconnect)(s,name,namelen);
}

int BinHTTPURLInputStream::send(unsigned int s,const char* buf,int len,int flags)
{
	return (*gWSsend)(s,buf,len,flags);
}

int BinHTTPURLInputStream::recv(unsigned int s,char* buf,int len,int flags)
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


BinHTTPURLInputStream::BinHTTPURLInputStream(const XMLURL& urlSource, const XMLNetHTTPInfo* httpInfo /*=0*/)
      : fSocketHandle(0)
      , fBytesProcessed(0)
{
    if(!fInitialized)
    {
        if (!fInitMutex)
        {
            XMLMutex* tmpMutex = new XMLMutex(XMLPlatformUtils::fgMemoryManager);
            if (XMLPlatformUtils::compareAndSwap((void**)&fInitMutex, tmpMutex, 0))
            {
                // Someone beat us to it, so let's clean up ours
                delete tmpMutex;
            }
         }
         XMLMutexLock lock(fInitMutex);
         if (!fInitialized)
         {
             Initialize(urlSource.getMemoryManager());
         }
    }

    fMemoryManager = urlSource.getMemoryManager();
    //
    // Pull all of the parts of the URL out of th urlSource object, and transcode them
    //   and transcode them back to ASCII.
    //
    const XMLCh*        hostName = urlSource.getHost();
    char*               hostNameAsCharStar = XMLString::transcode(hostName, urlSource.getMemoryManager());
    ArrayJanitor<char>  janBuf1(hostNameAsCharStar, urlSource.getMemoryManager());

    const XMLCh*        path = urlSource.getPath();
    char*               pathAsCharStar = XMLString::transcode(path, urlSource.getMemoryManager());
    ArrayJanitor<char>  janBuf2(pathAsCharStar, urlSource.getMemoryManager());

    const XMLCh*        fragment = urlSource.getFragment();
    char*               fragmentAsCharStar = 0;
    if (fragment)
        fragmentAsCharStar = XMLString::transcode(fragment, urlSource.getMemoryManager());
    ArrayJanitor<char>  janBuf3(fragmentAsCharStar, urlSource.getMemoryManager());

    const XMLCh*        query = urlSource.getQuery();
    char*               queryAsCharStar = 0;
    if (query)
        queryAsCharStar = XMLString::transcode(query, urlSource.getMemoryManager());
    ArrayJanitor<char>  janBuf4(queryAsCharStar, urlSource.getMemoryManager());		

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
            ThrowXMLwithMemMgr1(NetAccessorException,
                     XMLExcepts::NetAcc_TargetResolution, hostName, fMemoryManager);
        }
        if ((hostEntPtr =
                gethostbyaddr((const char *) &numAddress,
                              sizeof(unsigned long), AF_INET)) == NULL)
        {
            // Call WSAGetLastError() to get the error number.
            ThrowXMLwithMemMgr1(NetAccessorException,
                     XMLExcepts::NetAcc_TargetResolution, hostName, fMemoryManager);
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
        ThrowXMLwithMemMgr1(NetAccessorException,
                 XMLExcepts::NetAcc_CreateSocket, urlSource.getURLText(), fMemoryManager);
    }

    if (connect(s, (struct sockaddr *) &sa, sizeof(sa)) == SOCKET_ERROR)
    {
        // Call WSAGetLastError() to get the error number.
        ThrowXMLwithMemMgr1(NetAccessorException,
                 XMLExcepts::NetAcc_ConnSocket, urlSource.getURLText(), fMemoryManager);
    }


    // Set a flag so we know that the headers have not been read yet.
    bool fHeaderRead = false;

    // The port is open and ready to go.
    // Build up the http GET command to send to the server.
    // To do:  We should really support http 1.1.  This implementation
    //         is weak.

    memset(fBuffer, 0, sizeof(fBuffer));

    if(httpInfo==0)
        strcpy(fBuffer, "GET ");
    else {
        switch(httpInfo->fHTTPMethod) {
        case XMLNetHTTPInfo::GET:   strcpy(fBuffer, "GET "); break;
        case XMLNetHTTPInfo::PUT:   strcpy(fBuffer, "PUT "); break;
        case XMLNetHTTPInfo::POST:  strcpy(fBuffer, "POST "); break;
        }
    }
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
    strcat(fBuffer, "\r\n");

    if(httpInfo!=0 && httpInfo->fHeaders!=0)
        strncat(fBuffer,httpInfo->fHeaders,httpInfo->fHeadersLen);

    strcat(fBuffer, "\r\n");

    // Send the http request
    int lent = strlen(fBuffer);
    int  aLent = 0;
    if ((aLent = send(s, fBuffer, lent, 0)) != lent)
    {
        // Call WSAGetLastError() to get the error number.
        ThrowXMLwithMemMgr1(NetAccessorException,
                 XMLExcepts::NetAcc_WriteSocket, urlSource.getURLText(), fMemoryManager);
    }

    if(httpInfo!=0 && httpInfo->fPayload!=0) {
        int  aLent = 0;
        if ((aLent = send(s, httpInfo->fPayload, httpInfo->fPayloadLen, 0)) != httpInfo->fPayloadLen)
        {
            // Call WSAGetLastError() to get the error number.
            ThrowXMLwithMemMgr1(NetAccessorException,
                     XMLExcepts::NetAcc_WriteSocket, urlSource.getURLText(), fMemoryManager);
        }
    }

    //
    // get the response, check the http header for errors from the server.
    //
    memset(fBuffer, 0, sizeof(fBuffer));
    aLent = recv(s, fBuffer, sizeof(fBuffer)-1, 0);
    if (aLent == SOCKET_ERROR || aLent == 0)
    {
        // Call WSAGetLastError() to get the error number.
        ThrowXMLwithMemMgr1(NetAccessorException, XMLExcepts::NetAcc_ReadSocket, urlSource.getURLText(), fMemoryManager);
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
                    ThrowXMLwithMemMgr1(NetAccessorException, XMLExcepts::NetAcc_ReadSocket, urlSource.getURLText(), fMemoryManager);
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
        ThrowXMLwithMemMgr1(NetAccessorException, XMLExcepts::NetAcc_ReadSocket, urlSource.getURLText(), fMemoryManager);
    }

    p = strchr(p, ' ');
    if (p == 0)
    {
        ThrowXMLwithMemMgr1(NetAccessorException, XMLExcepts::NetAcc_ReadSocket, urlSource.getURLText(), fMemoryManager);
    }

    int httpResponse = atoi(p);
    if (httpResponse != 200)
    {
        // Most likely a 404 Not Found error.
        //   Should recognize and handle the forwarding responses.
        //
        ThrowXMLwithMemMgr1(NetAccessorException, XMLExcepts::File_CouldNotOpenFile, urlSource.getURLText(), fMemoryManager);
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
            ThrowXMLwithMemMgr(NetAccessorException, XMLExcepts::NetAcc_ReadSocket, fMemoryManager);
        }
    }

    fBytesProcessed += len;
    return len;
}


XERCES_CPP_NAMESPACE_END
