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
 * Revision 1.8  2004/09/08 13:56:36  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.7  2004/05/05 22:08:07  amassari
 * Content reported by a web site as text/xml is now read correctly; added an extra way of getting the size of the document in case no redirection has been performed
 *
 * Revision 1.6  2004/01/16 14:29:21  amassari
 * Removed usage of undeclared macro MIN
 *
 * Revision 1.5  2003/12/24 15:24:13  cargilld
 * More updates to memory management so that the static memory manager.
 *
 * Revision 1.4  2003/05/17 05:54:18  knoaman
 * Update NetAccessors to use the memory manager.
 *
 * Revision 1.3  2002/12/06 16:42:13  tng
 * Fix the error messages thrown from net accessor module.
 *
 * Revision 1.2  2002/11/04 15:11:39  tng
 * C++ Namespace Support.
 *
 * Revision 1.1.1.1  2002/02/01 22:22:22  peiyongz
 * sane_include
 *
 * Revision 1.6  2001/11/28 19:14:59  knoaman
 * Bug 2238: fix by Artur Klauser
 * Some broken proxy servers (e.g. the one behind which I happen to reside,
 * which apparently is Server: Microsoft-IIS/5.0)
 * implement wrong boundary conditions for range requests. In particular,
 * (1) request ranges straddling the content length of the document
 *     give bogus header information
 *       Content-Range: bytes X-Y/Z
 *     where Y > Z, and give 0 bytes of actual message content.
 * (2) request ranges beyond the content length of the document do not
 *     give an error response as asked for in RFC2616 (http/1.1)
 *
 * Since the NetAccessor code was just trying to fall off the end of the
 * document with the last range request (ie. hitting case 1 above), it
 * never gets the last chunk of data from this proxy server.
 *
 * Revision 1.5  2001/11/28 19:11:33  knoaman
 * Bug 2237: fix by Artur Klauser
 *
 * Revision 1.4  2001/03/02 14:39:21  tng
 * Enabling libWWW NetAccessor support under UNIX. Tested with latest tarball of libWWW
 * (w3c-libwww-5.3.2) under RedHat Linux 6.1.  Added by Martin Kalen.
 *
 * There is one MAJOR problem with the use of libwww and the patches
 * below, which someone with knowledge of libwww filters etc. might want
 * to look into. Default behavior for content-type text/xml is to consume
 * all xml data before it reaches the simple HTML presenter. Hence, only
 * files with content-type text/html will actually reach the xerces-c
 * library. If you have a *.xml file on the webbserver, processing of the
 * file will throw an exception stating "The main XML document cannot be
 * empty" (correct in a xerces point of view since if you enable debug
 * build you will see that libwww "eats" all text/xml).
 *
 * See "Diffs for enabling libWWW NetAccessor support under UNIX" posted in March 1, 2001
 * in the xerces-c-dev mailing list for further information.
 *
 * Revision 1.3  2000/05/15 22:31:31  andyh
 * Replace #include<memory.h> with <string.h> everywhere.
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

#include <xercesc/util/XMLNetAccessor.hpp>
#include <xercesc/util/NetAccessors/libWWW/BinURLInputStream.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/XMLExceptMsgs.hpp>
#include <strings.h>
#include <WWWInit.h>

XERCES_CPP_NAMESPACE_BEGIN

//
// This define specifies the size of the buffer used to read chunks
// out of the URL input stream.
//

#define URLISBUFMAXSIZE        8192


//
// We assume here that the URL is essentially composed of just ASCII characters
// and hence converting it to a 'char *' requires just to drop the leading zero
// byte. The reason, we can get away with this is that libWWW currently provides
// no wide character API's.
//
// The input Unicode string is assumed to be 0 terminated.
// The caller is responsible to free the memory allocated to store the resultant
// 'char *' string.
//

static char* localTranscode(const XMLCh* latinStrInUnicode
                            , MemoryManager* const  manager)
{
    unsigned int   lent = XMLString::stringLen(latinStrInUnicode);
    char*  retval = (char*) manager->allocate
    (
        (lent + 1) * sizeof(char)
    );//new char[lent + 1];
    unsigned int  i = 0;
    for (i = 0; i < lent; i++)
        retval[i] = (char) latinStrInUnicode[i]; // drop the leading byte.
    retval[lent] = 0;
    return retval;
}



BinURLInputStream::BinURLInputStream(const XMLURL& urlSource)
      : fBuffer(0)
      , fBufferSize(0)
      , fBufferIndex(0)
      , fRemoteFileSize(0)
      , fAnchor(0)
      , fBytesProcessed(0)
      , fMemoryManager(urlSource.getMemoryManager())
{
    fBuffer = (XMLByte*) fMemoryManager->allocate
    (
        URLISBUFMAXSIZE * sizeof(XMLByte)
    );//new XMLByte[URLISBUFMAXSIZE];
    const XMLCh*  uri = urlSource.getURLText();
    char*   uriAsCharStar = localTranscode(uri, fMemoryManager);

    //
    // First find the size of the remote resource being asked for.
    // We use the ContentCounter stream provided by libWWW.
    //

    fAnchor = HTAnchor_findAddress(uriAsCharStar);
    HTRequest*   request = HTRequest_new();
    HTRequest_setOutputFormat(request, WWW_SOURCE);
    HTStream*    counterStrm = HTContentCounter(HTBlackHole(), request, 0xFFFF);
    BOOL  status = HTLoadToStream(uriAsCharStar, counterStrm, request);
    if (status == YES)
    {
        HTParentAnchor * anchor = HTRequest_anchor(request);
        fRemoteFileSize=HTAnchor_length(anchor);
        if(fRemoteFileSize < 0)
        {
            // Patch by Artur Klauser
            // When a redirection is processed in libWWW, it seems that
            // HTAnchor_length(anchor) == -1 on the original anchor, whereas
            // HTResponse_length(response) gives the correct content length of
            // the redirection target. This has confused fRemoteFileSize and it was
            // not checked for a -1 response at all.
            HTResponse * response = HTRequest_response (request);
            fRemoteFileSize = HTResponse_length(response);
            if (fRemoteFileSize < 0) {
                ThrowXMLwithMemMgr(NetAccessorException, XMLExcepts::NetAcc_LengthError, fMemoryManager);
            }
        }
    }

    // Cleanup, before you throw any errors.
    fMemoryManager->deallocate(uriAsCharStar);
    HTRequest_delete(request);
    // Don't know whether I am supposed to delete counterStrm.

    if (status == NO)
    {
        ThrowXMLwithMemMgr(NetAccessorException, XMLExcepts::NetAcc_LengthError, fMemoryManager);
    }
}



BinURLInputStream::~BinURLInputStream()
{
    fMemoryManager->deallocate(fBuffer);//delete [] fBuffer;
    fBuffer = 0;
    // Do not delete the fAnchor. Its deleted when the destructor of
    // libWWWNetAccessor is called.
}


void BinURLInputStream::reset()
{
    fBufferSize = 0;
    fBytesProcessed = 0;
    fBufferIndex = 0;
    memset((void*) fBuffer, 0x00, sizeof(XMLByte) * URLISBUFMAXSIZE);
}


unsigned int BinURLInputStream::curPos() const
{
    return fBytesProcessed;
}


unsigned int BinURLInputStream::bytesAvail() const
{
    unsigned int  retval = fBufferSize - fBufferIndex;
    return retval;
}


unsigned int BinURLInputStream::readBytes(XMLByte* const  toFill
                                  , const unsigned int    maxToRead)
{
    unsigned int  retval = 0;
    unsigned int  bytesAsked = maxToRead;
    unsigned int  bytesForCopy = 0;

    // Wipe out the old stuff from the destination buffer to fill.

    memset((void*)toFill, 0x00, sizeof(XMLByte) * maxToRead);

    // You can only read till the end of the remote resource file.
    // So, adjust the count of bytes you want to read now.

    if (fBytesProcessed + bytesAsked >= fRemoteFileSize)
    {
        bytesAsked = fRemoteFileSize - fBytesProcessed;
    }

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
        HTRequest*      request = HTRequest_new();
        HTChunk*        result = NULL;
        char            ranges[64];

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

        unsigned int    bytesRemainingForCopy = bytesAsked - bytesForCopy;

        // Now read a new chunk from the stream. HTTP lets you specify the
        // range of bytes that you would like.

        sprintf(ranges, "%ld-%ld", fBytesProcessed,
                fRemoteFileSize<(fBytesProcessed + URLISBUFMAXSIZE)? fRemoteFileSize - 1:  fBytesProcessed + URLISBUFMAXSIZE - 1);
        HTRequest_addRange(request, "bytes", ranges);
        HTRequest_setOutputFormat(request, WWW_SOURCE);
        result = HTLoadAnchorToChunk(fAnchor, request);
        fBufferSize = HTChunk_size(result);
        if (fBufferSize > 0)
        {
            // Store the read chunk in fBuffer.
            memset((void*) fBuffer, 0x00, URLISBUFMAXSIZE);
            memcpy((void*) fBuffer, (void*) HTChunk_data(result), fBufferSize);
            fBufferIndex = 0;
        }

        HTRequest_delete(request);
        HTChunk_delete(result);

        // Now fill the destination buffer with the new data just read.

        bytesForCopy = fBufferSize;
        if (bytesRemainingForCopy > fBufferSize)
        {
            bytesRemainingForCopy = fBufferSize;
        }
        memcpy((void*) (toFill + bufToFillIndex),
               (void*) fBuffer,
               bytesRemainingForCopy);

        // Update counters.
        retval += bytesRemainingForCopy;
        fBufferIndex += bytesRemainingForCopy;
        fBytesProcessed += bytesRemainingForCopy;
    }

    return retval;
}

XERCES_CPP_NAMESPACE_END
