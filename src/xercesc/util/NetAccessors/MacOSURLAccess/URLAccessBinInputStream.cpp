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
 *
 * Written by James D. Berry 7/20/00
 * Critical Path Software, Inc.
 */

/*
 * $Id$
 */


#include <xercesc/util/XMLNetAccessor.hpp>
#include <xercesc/util/NetAccessors/MacOSURLAccess/URLAccessBinInputStream.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/XMLExceptMsgs.hpp>
#include <xercesc/util/Janitor.hpp>

#include <cstdlib>
#include <cstring>

XERCES_CPP_NAMESPACE_BEGIN

URLAccessBinInputStream::URLAccessBinInputStream(const XMLURL& urlSource)
      : mBytesProcessed(0),
        mURLReference(NULL),
        mBuffer(NULL),
        mBufPos(NULL),
        mBufAvailable(0)
{
	OSStatus status = noErr;
	
	//	Get the full URL from the source
    char*               url = XMLString::transcode(urlSource.getURLText(), urlSource.getMemoryManager());
    ArrayJanitor<char>  janBuf(url, urlSource.getMemoryManager());

	//	Create a URL reference from the URL
	status = URLNewReference(url, &mURLReference);
	
	//	Begin the transfer
	if (status == noErr)
		status = URLOpen(
					mURLReference,
					NULL,	// FSSpec* (not reading to file)
					0, 		// URLOpenFlags
					NULL,	// URLNotifyUPP
					0,		// URLEventMask
					0);		// userContext
	
	//	If we failed, we throw
	switch (status)
	{
		case noErr:
			break;
			
		case kURLInvalidURLError:
        	ThrowXML(MalformedURLException, XMLExcepts::URL_MalformedURL);
			break;
		case kURLUnsupportedSchemeError:
        	ThrowXML(MalformedURLException, XMLExcepts::URL_UnsupportedProto);
			break;
		
		default:
        	ThrowXML1(NetAccessorException, XMLExcepts::NetAcc_ConnSocket, urlSource.getURLText());
        	break;
	}
}



URLAccessBinInputStream::~URLAccessBinInputStream()
{
	OSStatus status = noErr;
	
	//	Release any buffer we've still got a hold of
	if (status == noErr && mBuffer)
	{
		status = URLReleaseBuffer(mURLReference, mBuffer);
		mBuffer = NULL;
	}

	//	Get the current state
	URLState state = 0;
	if (status == noErr)
		status = URLGetCurrentState(mURLReference, &state);
	
	//	Abort if we're not completed
	if (status == noErr && state != kURLNullState && state != kURLCompletedState)
		status = URLAbort(mURLReference);
		
	//	Loop til we reach a terminal state.
	//	This may be unneeded by URLAccess, but the docs are
	//	not very good.
	while (
			status == noErr
		 && state != kURLNullState
		 && state != kURLErrorOccurredState
		 && state != kURLCompletedState
		  )
	{
		status = URLIdle();
		if (status == noErr)
			status = URLGetCurrentState(mURLReference, &state);
	}
		
	//	Dispose the reference
	status = URLDisposeReference(mURLReference);
}


//
//	Call URLAccess to fullfill the read request. Since it
//	passes us back buffers full of data, our object maintains
//	a partial buffer across calls.
//
unsigned int
URLAccessBinInputStream::readBytes(XMLByte* const    toFill
                                    , const unsigned int    maxToRead)
{
	OSStatus status = noErr;

	XMLByte* writePos			= toFill;
	std::size_t bytesDesired	= maxToRead;
	URLState state				= kURLNullState;
	
	while (												// while...
			status == noErr								// there's been no error
		 && bytesDesired > 0							// more data is wanted
		 && (status = URLGetCurrentState(mURLReference, &state)) == noErr	// we can get the state
		 && (state != kURLErrorOccurredState)			// no error has occurred in the transaction
		 && (mBuffer || state != kURLCompletedState)	// we have data still buffered or the request isn't complete
		 && (mBuffer || bytesDesired == maxToRead)		// we have data still buffered or we've supplied absolutely none
		  )
	{
		//	Give time to URLAccess
		status = URLIdle();
		
		//	If we've got buffered data, use it
		if (status == noErr && mBuffer)
		{
			//	Supply as much as we can from the buffer
			std::size_t n = mBufAvailable;
			if (n > bytesDesired)
				n = bytesDesired;
				
			//	If we've got data, copy it over and update our pointers
			if (n > 0)
			{
				std::memcpy(writePos, mBufPos, n);
				
				writePos		+= n;
				bytesDesired	-= n;
				
				mBufPos			+= n;
				mBufAvailable	-= n;
				
				mBytesProcessed	+= n;
			}
			
			//	If we exhausted the buffer, release it
			if (mBufAvailable == 0)
			{
				status = URLReleaseBuffer(mURLReference, mBuffer);
				mBuffer = NULL;
			}
		}
		
		//	If the buffer is exhausted, get a new one
		if (status == noErr && !mBuffer)
		{
			status = URLGetBuffer(mURLReference, &mBuffer, &mBufAvailable);
			if (status == noErr)
				mBufPos = reinterpret_cast<char*>(mBuffer);
		}
	}
	
	//	Throw on any error
	if (status != noErr || state == kURLErrorOccurredState)
	    ThrowXML(NetAccessorException, XMLExcepts::NetAcc_ReadSocket);
	
	//	Return number of bytes delivered
	return maxToRead - bytesDesired;
}

XERCES_CPP_NAMESPACE_END
