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
 * Written by James D. Berry, Critical Path Software, Inc., 2002-02-17
 *
 */

/*
 * $Id$
 */

#include <cstdlib>
#include <cstring>

#include <xercesc/util/XMLNetAccessor.hpp>
#include <xercesc/util/NetAccessors/MacOSURLAccessCF/URLAccessCFBinInputStream.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/XMLExceptMsgs.hpp>
#include <xercesc/util/Janitor.hpp>

XERCES_CPP_NAMESPACE_BEGIN

URLAccessCFBinInputStream::URLAccessCFBinInputStream(const XMLURL& urlSource)
      : mBytesProcessed(0),
        mDataRef(NULL)
{
    //	Figure out what we're dealing with
    const XMLCh* urlText = urlSource.getURLText();
    unsigned int urlLength = XMLString::stringLen(urlText);

    //	Create a CFString from the path
    CFStringRef stringRef = NULL;
    if (urlText)
    {
        stringRef = CFStringCreateWithCharacters(
            kCFAllocatorDefault,
            urlText,
            urlLength
            );
    }

    //	Create a URLRef from the CFString
    CFURLRef urlRef = NULL;
    if (stringRef)
    {
        urlRef = CFURLCreateWithString(
            kCFAllocatorDefault,
            stringRef,
            NULL				// CFURLRef baseURL
            );
    }

	//	Fetch the data
    mDataRef = NULL;
    SInt32 errorCode = 0;
    Boolean success = false;
    if (stringRef)
    {
        success = CFURLCreateDataAndPropertiesFromResource(
            kCFAllocatorDefault,
            urlRef,
            &mDataRef,
            NULL,				// CFDictionaryRef *properties,
            NULL,				// CFArrayRef desiredProperties,
            &errorCode
            );
    }

    //	Cleanup temporary stuff
    if (stringRef)
        CFRelease(stringRef);
    if (urlRef)
        CFRelease(urlRef);

    //	Check for an error in fetching the data
    if (!success || errorCode)
    {
        //	Dispose any potential dataRef
        if (mDataRef)
        {
            CFRelease(mDataRef);
            mDataRef = NULL;
        }

        //	Do a best attempt at mapping some errors
        switch (errorCode)
        {
            case kCFURLUnknownSchemeError:
                ThrowXML(MalformedURLException, XMLExcepts::URL_UnsupportedProto);
                break;

            case kCFURLRemoteHostUnavailableError:
                ThrowXML1(NetAccessorException,  XMLExcepts::NetAcc_TargetResolution, urlSource.getHost());
                break;

            case kCFURLUnknownError:
                ThrowXML1(NetAccessorException, XMLExcepts::NetAcc_ReadSocket, urlText);
                break;

            case kCFURLResourceNotFoundError:
            case kCFURLResourceAccessViolationError:
            case kCFURLTimeoutError:
                ThrowXML1(NetAccessorException, XMLExcepts::File_CouldNotOpenFile, urlText);
                break;

            case kCFURLImproperArgumentsError:
            case kCFURLUnknownPropertyKeyError:
            case kCFURLPropertyKeyUnavailableError:
            default:
                ThrowXML1(NetAccessorException, XMLExcepts::NetAcc_InternalError, urlText);
                break;
        }
    }
}


URLAccessCFBinInputStream::~URLAccessCFBinInputStream()
{
    //	Release any dataRef
    if (mDataRef)
        CFRelease(mDataRef);
}


//
//	We've already read the data into a dataRef.
//	Just spoon it out to the caller as they ask for it.
//
unsigned int
URLAccessCFBinInputStream::readBytes(XMLByte* const    toFill
                                    , const unsigned int    maxToRead)
{
    //	If we don't have a dataRef, we can't return any data
    if (!mDataRef)
        return 0;

    //	Get the length of the data we've fetched
    CFIndex dataLength = CFDataGetLength(mDataRef);

    //	Calculate how much to return based on how much
    //	we've already returned, and how much the user wants
    CFIndex n = dataLength - mBytesProcessed;			// Amount remaining
    CFIndex desired = maxToRead & 0x7fffffff;			// CFIndex is signed
    if (n > desired)									// Amount desired
        n = desired;

    //	Read the appropriate bytes into the user buffer
    CFRange range = CFRangeMake(mBytesProcessed, n);
    CFDataGetBytes(mDataRef, range, reinterpret_cast<UInt8*>(toFill));
	
    //	Update bytes processed
    mBytesProcessed += n;

    //	Return the number of bytes delivered
    return n;
}

XERCES_CPP_NAMESPACE_END
