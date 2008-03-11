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

#include <winsock2.h>
#include <windows.h>

#ifdef WITH_IPV6
#include <ws2tcpip.h>
#endif

#include <stdio.h>
#include <string.h>

#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLNetAccessor.hpp>
#include <xercesc/util/NetAccessors/WinSock/BinHTTPURLInputStream.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/XMLExceptMsgs.hpp>
#include <xercesc/util/Janitor.hpp>
#include <xercesc/util/XMLUniDefs.hpp>
#include <xercesc/util/Base64.hpp>
#include <xercesc/util/Mutexes.hpp>

XERCES_CPP_NAMESPACE_BEGIN

typedef u_short (WSAAPI * LPFN_HTONS)(u_short hostshort);
typedef SOCKET (WSAAPI * LPFN_SOCKET)(int af, int type, int protocol);
typedef int (WSAAPI * LPFN_CONNECT)(SOCKET s, const struct sockaddr* name, int namelen);
typedef int (WSAAPI * LPFN_SEND)(SOCKET s, const char* buf, int len, int flags);
typedef int (WSAAPI * LPFN_RECV)(SOCKET s, char* buf, int len, int flags);
typedef int (WSAAPI * LPFN_SHUTDOWN)(SOCKET s, int how);
typedef int (WSAAPI * LPFN_CLOSESOCKET)(SOCKET s);
typedef int (WSAAPI * LPFN_WSACLEANUP)();
typedef int (WSAAPI * LPFN_WSASTARTUP)(WORD wVersionRequested, LPWSADATA lpWSAData);
#ifdef WITH_IPV6
typedef int (WSAAPI * LPFN_GETADDRINFO)(const char* nodename, const char * servname, const struct addrinfo * hints, struct addrinfo ** res);
typedef void (WSAAPI * LPFN_FREEADDRINFO)(struct addrinfo * ai);
#else
typedef struct hostent *(WSAAPI * LPFN_GETHOSTBYNAME)(const char* name);
typedef struct hostent *(WSAAPI * LPFN_GETHOSTBYADDR)(const char* addr, int len, int type);
typedef unsigned long (WSAAPI * LPFN_INET_ADDR)(const char* cp);
#endif

static HMODULE gWinsockLib = NULL;
static LPFN_HTONS gWShtons = NULL;
static LPFN_SOCKET gWSsocket = NULL;
static LPFN_CONNECT gWSconnect = NULL;
static LPFN_SEND gWSsend = NULL;
static LPFN_RECV gWSrecv = NULL;
static LPFN_SHUTDOWN gWSshutdown = NULL;
static LPFN_CLOSESOCKET gWSclosesocket = NULL;
static LPFN_WSACLEANUP gWSACleanup = NULL;
#ifdef WITH_IPV6
static LPFN_GETADDRINFO gWSgetaddrinfo = NULL;
static LPFN_FREEADDRINFO gWSfreeaddrinfo = NULL;
#else
static LPFN_GETHOSTBYNAME gWSgethostbyname = NULL;
static LPFN_GETHOSTBYADDR gWSgethostbyaddr = NULL;
static LPFN_INET_ADDR gWSinet_addr = NULL;
#endif

static u_short wrap_htons(u_short hostshort)
{
	return (*gWShtons)(hostshort);
}

static SOCKET wrap_socket(int af,int type,int protocol)
{
	return (*gWSsocket)(af,type,protocol);
}

static int wrap_connect(SOCKET s,const struct sockaddr* name,int namelen)
{
	return (*gWSconnect)(s,name,namelen);
}

static int wrap_send(SOCKET s,const char* buf,int len,int flags)
{
	return (*gWSsend)(s,buf,len,flags);
}

static int wrap_recv(SOCKET s,char* buf,int len,int flags)
{
	return (*gWSrecv)(s,buf,len,flags);
}

static int wrap_shutdown(SOCKET s,int how)
{
	return (*gWSshutdown)(s,how);
}

static int wrap_closesocket(SOCKET socket)
{
	return (*gWSclosesocket)(socket);
}

#ifdef WITH_IPV6
static int wrap_getaddrinfo(const char* nodename,const char* servname,const struct addrinfo* hints,struct addrinfo** res)
{
   return (*gWSgetaddrinfo)(nodename,servname,hints,res);
}

static void wrap_freeaddrinfo(struct addrinfo* ai)
{
    (*gWSfreeaddrinfo)(ai);
}
#else
static struct hostent* wrap_gethostbyname(const char* name)
{
	return (*gWSgethostbyname)(name);
}

static struct hostent* wrap_gethostbyaddr(const char* addr,int len,int type)
{
	return (*gWSgethostbyaddr)(addr,len,type);
}

static unsigned long wrap_inet_addr(const char* cp)
{
	return (*gWSinet_addr)(cp);
}

#endif


class SocketJanitor
{
public:
    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    SocketJanitor(SOCKET* toDelete) : fData(toDelete) {}
    ~SocketJanitor() { reset(); }

    SOCKET* get() const { return fData; }
    SOCKET* release() {	SOCKET* p = fData; fData = 0; return p; }

    void reset(SOCKET* p = 0) { if(fData) wrap_closesocket(*fData); fData=p; }
    bool isDataNull() { return (fData == 0); }

private :
    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    SocketJanitor();
    SocketJanitor(const SocketJanitor&);
    SocketJanitor& operator=(const SocketJanitor&);

    // -----------------------------------------------------------------------
    //  Private data members
    //
    //  fData
    //      This is the pointer to the socket that must be closed when
    //      this object is destroyed.
    // -----------------------------------------------------------------------
    SOCKET*  fData;
};

bool BinHTTPURLInputStream::fInitialized = false;

void BinHTTPURLInputStream::Initialize(MemoryManager* const manager)
{
    //
    // Initialize the WinSock library here.
    //
    WORD        wVersionRequested;
    WSADATA     wsaData;

    LPFN_WSASTARTUP startup = NULL;
    if(gWinsockLib == NULL)
    {
#ifdef WITH_IPV6
      gWinsockLib = LoadLibraryA("WS2_32");
#else
      gWinsockLib = LoadLibraryA("WSOCK32");
#endif
      if(gWinsockLib == NULL)
      {
          ThrowXMLwithMemMgr(NetAccessorException, XMLExcepts::NetAcc_InitFailed, manager);
      }
      else
      {
          startup = (LPFN_WSASTARTUP) GetProcAddress(gWinsockLib,"WSAStartup");
          gWSACleanup = (LPFN_WSACLEANUP) GetProcAddress(gWinsockLib,"WSACleanup");
          gWShtons = (LPFN_HTONS) GetProcAddress(gWinsockLib,"htons");
          gWSsocket = (LPFN_SOCKET) GetProcAddress(gWinsockLib,"socket");
          gWSconnect = (LPFN_CONNECT) GetProcAddress(gWinsockLib,"connect");
          gWSsend = (LPFN_SEND) GetProcAddress(gWinsockLib,"send");
          gWSrecv = (LPFN_RECV) GetProcAddress(gWinsockLib,"recv");
          gWSshutdown = (LPFN_SHUTDOWN) GetProcAddress(gWinsockLib,"shutdown");
          gWSclosesocket = (LPFN_CLOSESOCKET) GetProcAddress(gWinsockLib,"closesocket");
#ifdef WITH_IPV6
          gWSgetaddrinfo = (LPFN_GETADDRINFO) GetProcAddress(gWinsockLib,"getaddrinfo");
          gWSfreeaddrinfo = (LPFN_FREEADDRINFO) GetProcAddress(gWinsockLib,"freeaddrinfo");
#else
          gWSgethostbyname = (LPFN_GETHOSTBYNAME) GetProcAddress(gWinsockLib,"gethostbyname");
          gWSgethostbyaddr = (LPFN_GETHOSTBYADDR) GetProcAddress(gWinsockLib,"gethostbyaddr");
          gWSinet_addr = (LPFN_INET_ADDR) GetProcAddress(gWinsockLib,"inet_addr");
#endif

          if(startup == NULL
             || gWSACleanup == NULL
             || gWShtons == NULL
             || gWSsocket == NULL
             || gWSconnect == NULL
             || gWSsend == NULL
             || gWSrecv == NULL
             || gWSshutdown == NULL
             || gWSclosesocket == NULL
#ifdef WITH_IPV6
             || gWSgetaddrinfo == NULL
             || gWSfreeaddrinfo == NULL
#else
             || gWSgethostbyname == NULL
             || gWSgethostbyaddr == NULL
             || gWSinet_addr == NULL
#endif
          )
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

void BinHTTPURLInputStream::Cleanup()
{
    XMLMutexLock lock(XMLPlatformUtils::fgAtomicMutex);

    if(fInitialized)
    {
        if(gWSACleanup) (*gWSACleanup)();
        gWSACleanup = NULL;
        FreeLibrary(gWinsockLib);
        gWinsockLib = NULL;
        gWShtons = NULL;
        gWSsocket = NULL;
        gWSconnect = NULL;
        gWSsend = NULL;
        gWSrecv = NULL;
        gWSshutdown = NULL;
        gWSclosesocket = NULL;
#ifdef WITH_IPV6
        gWSgetaddrinfo = NULL;
        gWSfreeaddrinfo = NULL;
#else
        gWSgethostbyname = NULL;
        gWSgethostbyaddr = NULL;
        gWSinet_addr = NULL;
#endif

        fInitialized = false;
    }
}


BinHTTPURLInputStream::BinHTTPURLInputStream(const XMLURL& urlSource, const XMLNetHTTPInfo* httpInfo /*=0*/)
      : fSocketHandle(0)
      , fBytesProcessed(0)
{
    // Check if we need to load the winsock library. While locking the
    // mutex every time may be somewhat slow, we don't care in this
    // particular case since the next operation will most likely be
    // the network access which is a lot slower.
    //
    {
        XMLMutexLock lock(XMLPlatformUtils::fgAtomicMutex);

        if (!fInitialized)
          Initialize(urlSource.getMemoryManager());
    }

    fMemoryManager = urlSource.getMemoryManager();
    //
    // Pull all of the parts of the URL out of th urlSource object, and transcode them
    //   and transcode them back to ASCII.
    //
    const XMLCh*        hostName = urlSource.getHost();
    char*               hostNameAsCharStar = XMLString::transcode(hostName, fMemoryManager);
    ArrayJanitor<char>  janHostName(hostNameAsCharStar, fMemoryManager);

    const XMLCh*        path = urlSource.getPath();
    char*               pathAsCharStar = XMLString::transcode(path, fMemoryManager);
    ArrayJanitor<char>  janPath(pathAsCharStar, fMemoryManager);

    const XMLCh*        fragment = urlSource.getFragment();
    char*               fragmentAsCharStar = 0;
    if (fragment)
        fragmentAsCharStar = XMLString::transcode(fragment, fMemoryManager);
    ArrayJanitor<char>  janFragment(fragmentAsCharStar, fMemoryManager);

    const XMLCh*        query = urlSource.getQuery();
    char*               queryAsCharStar = 0;
    if (query)
        queryAsCharStar = XMLString::transcode(query, fMemoryManager);
    ArrayJanitor<char>  janQuery(queryAsCharStar, fMemoryManager);

    unsigned short      portNumber = (unsigned short) urlSource.getPortNum();

    const XMLCh* username = urlSource.getUser();
    const XMLCh* password = urlSource.getPassword();
    //
    // Set up a socket.
    //
    bool sawRedirect;
    int redirectCount = 0;
    SOCKET s;
    SocketJanitor janSock(0);
    bool lookUpHost = true;

    do {

#ifdef WITH_IPV6
		struct addrinfo hints, *res, *ai;

		memset(&hints, 0, sizeof(struct addrinfo));
		hints.ai_family = PF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;
		char tempbuf[10];
		XMLString::binToText(portNumber, tempbuf, 10, 10);
		int n = wrap_getaddrinfo(hostNameAsCharStar,(const char*)tempbuf,&hints, &res);
		if(n<0)
		{
			hints.ai_flags = AI_NUMERICHOST;
			n = wrap_getaddrinfo(hostNameAsCharStar,(const char*)tempbuf,&hints, &res);
			if(n<0)
				ThrowXMLwithMemMgr1(NetAccessorException, XMLExcepts::NetAcc_TargetResolution, hostName, fMemoryManager);
		}
		if (janSock.get())
			janSock.release();
		for (ai = res; ai != NULL; ai = ai->ai_next) {
			// Open a socket with the correct address family for this address.
			s = wrap_socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
			if (s == INVALID_SOCKET)
				continue;
			if (wrap_connect(s, ai->ai_addr, (int)ai->ai_addrlen) == SOCKET_ERROR)
			{
				wrap_freeaddrinfo(res);
				// Call WSAGetLastError() to get the error number.
				ThrowXMLwithMemMgr1(NetAccessorException,
						 XMLExcepts::NetAcc_ConnSocket, urlSource.getURLText(), fMemoryManager);
			}
			break;
		}
		wrap_freeaddrinfo(res);
		if (s == INVALID_SOCKET)
		{
			// Call WSAGetLastError() to get the error number.
			ThrowXMLwithMemMgr1(NetAccessorException,
					 XMLExcepts::NetAcc_CreateSocket, urlSource.getURLText(), fMemoryManager);
		}
		janSock.reset(&s);
#else
		struct hostent*     hostEntPtr = 0;
		struct sockaddr_in  sa;

        if (lookUpHost &&
            ((hostEntPtr = wrap_gethostbyname(hostNameAsCharStar)) == NULL))
        {
            unsigned long  numAddress = wrap_inet_addr(hostNameAsCharStar);
            if (numAddress == INADDR_NONE)
            {
                // Call WSAGetLastError() to get the error number.
                ThrowXMLwithMemMgr1(NetAccessorException,
                     XMLExcepts::NetAcc_TargetResolution, hostName, fMemoryManager);
            }
            if ((hostEntPtr =
                wrap_gethostbyaddr((const char *) &numAddress,
                              sizeof(unsigned long), AF_INET)) == NULL)
            {
                // Call WSAGetLastError() to get the error number.
                ThrowXMLwithMemMgr1(NetAccessorException,
                     XMLExcepts::NetAcc_TargetResolution, hostName, fMemoryManager);
            }
        }
        lookUpHost = false;

        memcpy((void *) &sa.sin_addr,
           (const void *) hostEntPtr->h_addr, hostEntPtr->h_length);

        sa.sin_family = hostEntPtr->h_addrtype;
        sa.sin_port = wrap_htons(portNumber);

        if (janSock.get())
            janSock.release();
        s = wrap_socket(hostEntPtr->h_addrtype, SOCK_STREAM, 0);
        if (s == INVALID_SOCKET)
        {
            // Call WSAGetLastError() to get the error number.
            ThrowXMLwithMemMgr1(NetAccessorException,
                 XMLExcepts::NetAcc_CreateSocket, urlSource.getURLText(), fMemoryManager);
        }
        janSock.reset(&s);

        if (wrap_connect(s, (struct sockaddr *) &sa, sizeof(sa)) == SOCKET_ERROR)
        {
            // Call WSAGetLastError() to get the error number.
            ThrowXMLwithMemMgr1(NetAccessorException,
                 XMLExcepts::NetAcc_ConnSocket, urlSource.getURLText(), fMemoryManager);
        }

#endif

        // Set a flag so we know that the headers have not been read yet.
        bool fHeaderRead = false;
        sawRedirect = false;

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
            size_t i = strlen(fBuffer);
            XMLString::binToText(portNumber, fBuffer+i, 10, 10);
        }
        strcat(fBuffer, "\r\n");

        if (username && password)
        {
            XMLBuffer userPass(256, fMemoryManager);
            userPass.append(username);
            userPass.append(chColon);
            userPass.append(password);
            char* userPassAsCharStar = XMLString::transcode(userPass.getRawBuffer(), fMemoryManager);
            ArrayJanitor<char>  janBuf(userPassAsCharStar, fMemoryManager);
            XMLSize_t len;
            XMLByte* encodedData = Base64::encode((XMLByte *)userPassAsCharStar, strlen(userPassAsCharStar), &len, fMemoryManager);
            ArrayJanitor<XMLByte>  janBuf2(encodedData, fMemoryManager);

            if (encodedData)
            {
                // HTTP doesn't want the 0x0A separating the data in chunks of 76 chars per line
                XMLByte* authData = (XMLByte*)fMemoryManager->allocate((len+1)*sizeof(XMLByte));
                ArrayJanitor<XMLByte>  janBuf(authData, fMemoryManager);
                XMLByte* cursor=authData;
                for(XMLSize_t i=0;i<len;i++)
                    if(encodedData[i]!=chLF)
                        *cursor++=encodedData[i];
                *cursor++=0;
                strcat(fBuffer, "Authorization: Basic ");
                strcat(fBuffer, (char*)authData);
                strcat(fBuffer, "\r\n");
            }
        }

        if(httpInfo!=0 && httpInfo->fHeaders!=0)
            strncat(fBuffer,httpInfo->fHeaders,httpInfo->fHeadersLen);

        strcat(fBuffer, "\r\n");

        // Send the http request
        int lent = (int)strlen(fBuffer);
        int  aLent = 0;
        if ((aLent = wrap_send(s, fBuffer, lent, 0)) != lent)
        {
            // Call WSAGetLastError() to get the error number.
            ThrowXMLwithMemMgr1(NetAccessorException,
                 XMLExcepts::NetAcc_WriteSocket, urlSource.getURLText(), fMemoryManager);
        }

        if(httpInfo!=0 && httpInfo->fPayload!=0) {
            if ((aLent = wrap_send(s, httpInfo->fPayload, httpInfo->fPayloadLen, 0)) != httpInfo->fPayloadLen)
            {
                // Call WSAGetLastError() to get the error number.
                ThrowXMLwithMemMgr1(NetAccessorException,
                     XMLExcepts::NetAcc_WriteSocket, urlSource.getURLText(), fMemoryManager);
            }
        }

        // get the response, check the http header for errors from the server.
        //
        memset(fBuffer, 0, sizeof(fBuffer));
        aLent = wrap_recv(s, fBuffer, sizeof(fBuffer)-1, 0);
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
                    aLent = wrap_recv(s, fBufferEnd, (int)((sizeof(fBuffer) - 1) - (fBufferEnd - fBuffer)), 0);
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
            // a 3xx response means there was a HTTP redirect
            if (httpResponse >= 300 && httpResponse < 400) {
                sawRedirect = true;
                redirectCount++;

                p = strstr(fBuffer, "Location: ");
                if (p == 0)
                {
                    ThrowXMLwithMemMgr1(NetAccessorException, XMLExcepts::NetAcc_ReadSocket, urlSource.getURLText(), fMemoryManager);
                }
                p += 10; // Length of string "Location: "

                char* endP = strstr(p, "\r\n");
                if (endP == 0)
                {
                    ThrowXMLwithMemMgr1(NetAccessorException, XMLExcepts::NetAcc_ReadSocket, urlSource.getURLText(), fMemoryManager);
                }
                endP[0] = chNull;

                XMLURL newURL(fMemoryManager);
                XMLCh* newURLString = XMLString::transcode(p, fMemoryManager);
                ArrayJanitor<XMLCh>  janNewURLString(newURLString, fMemoryManager);

                // The location string is either of the form:
                // local.xsd (ie. a relative URL)
                // http://host/path (ie. an absolute path)
                char* colonP = strstr(p, ":");
                if (colonP == 0) {
                    // if no colon assume relative url
                    newURL.setURL(urlSource, newURLString);
                }
                else {
                    // if colon then either a schema is specified or
                    // a port is specified
                    newURL.setURL(newURLString);

                    if (newURL.getProtocol() != XMLURL::HTTP) {
                        ThrowXMLwithMemMgr1(NetAccessorException, XMLExcepts::File_CouldNotOpenFile, newURL.getURLText(), fMemoryManager);
                    }

                    const XMLCh* newHostName = newURL.getHost();
                    if (XMLString::compareIStringASCII(hostName, newHostName) != 0) {
                        lookUpHost = true;
                        janHostName.release();
                        hostNameAsCharStar = XMLString::transcode(newHostName, fMemoryManager);
                        janHostName.reset(hostNameAsCharStar);
                    }
                }

                path = newURL.getPath();
                janPath.release();
                pathAsCharStar = XMLString::transcode(path, fMemoryManager);
                janPath.reset(pathAsCharStar);

                fragment = newURL.getFragment();
                janFragment.release();
                fragmentAsCharStar = 0;
                if (fragment)
                    fragmentAsCharStar = XMLString::transcode(fragment, fMemoryManager);
                janFragment.reset(fragmentAsCharStar);

                query = newURL.getQuery();
                janQuery.release();
                queryAsCharStar = 0;
                if (query)
                    queryAsCharStar = XMLString::transcode(query, fMemoryManager);
                janQuery.reset(queryAsCharStar);

                portNumber = (unsigned short) newURL.getPortNum();

                username = newURL.getUser();
                password = newURL.getPassword();
            }
            else {
                // Most likely a 404 Not Found error.
                //   Should recognize and handle the forwarding responses.
                //
                ThrowXMLwithMemMgr1(NetAccessorException, XMLExcepts::File_CouldNotOpenFile, urlSource.getURLText(), fMemoryManager);
            }
        }

    }
    while(sawRedirect && redirectCount <6);

    fSocketHandle = (unsigned int) *janSock.release();
}



BinHTTPURLInputStream::~BinHTTPURLInputStream()
{
    wrap_shutdown(fSocketHandle, SD_BOTH);
    wrap_closesocket(fSocketHandle);
}


//
//  readBytes
//
XMLSize_t BinHTTPURLInputStream::readBytes(XMLByte* const    toFill
                                    , const XMLSize_t        maxToRead)
{
    XMLSize_t len = fBufferEnd - fBufferPos;
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
        int iLen = wrap_recv((SOCKET) fSocketHandle, (char *) toFill, (int)maxToRead, 0);
        if (iLen == SOCKET_ERROR)
        {
            // Call WSAGetLastError() to get the error number.
            ThrowXMLwithMemMgr(NetAccessorException, XMLExcepts::NetAcc_ReadSocket, fMemoryManager);
        }
		len = iLen;
    }

    fBytesProcessed += len;
    return len;
}


XERCES_CPP_NAMESPACE_END
