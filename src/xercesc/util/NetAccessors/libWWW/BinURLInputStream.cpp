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

/**
 * $Log$
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



BinURLInputStream::BinURLInputStream(const XMLURL& urlSource)
      : fBuffer(0)
      , fBufferSize(0)
      , fBufferIndex(0)
      , fRemoteFileSize(0)
      , fAnchor(0)
      , fBytesProcessed(0)
{
    fBuffer = new XMLByte[URLISBUFMAXSIZE];
    const XMLCh*  uri = urlSource.getURLText();
    char*   uriAsCharStar = localTranscode(uri);

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
        // Patch by Artur Klauser
        // When a redirection is processed in libWWW, it seems that
        // HTAnchor_length(anchor) == -1 on the original anchor, whereas
        // HTResponse_length(response) gives the correct content length of
        // the redirection target. This has confusedfRemoteFileSize and it was
        // not checked for a -1 response at all.
        HTResponse * response = HTRequest_response (request);
        fRemoteFileSize = HTResponse_length(response);
        if (fRemoteFileSize < 0) {
            ThrowXML(NetAccessorException, XMLExcepts::NetAcc_LengthError);
        }
    }

    // Cleanup, before you throw any errors.
    delete [] uriAsCharStar;
    HTRequest_delete(request);
    // Don't know whether I am supposed to delete counterStrm.

    if (status == NO)
    {
        ThrowXML(NetAccessorException, XMLExcepts::NetAcc_LengthError);
    }
}



BinURLInputStream::~BinURLInputStream()
{
    delete [] fBuffer;
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
                MIN(fBytesProcessed + URLISBUFMAXSIZE - 1,  fRemoteFileSize - 1));
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
