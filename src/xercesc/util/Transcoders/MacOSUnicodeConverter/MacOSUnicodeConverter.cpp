/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 1999-2004 The Apache Software Foundation.  All rights
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
 *	$Id$
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/util/XercesDefs.hpp>

#include <algorithm>
#include <cstddef>
#include <cstring>

#if defined(XML_METROWERKS) || (__GNUC__ >= 3 && _GLIBCPP_USE_WCHAR_T)
	// Only used under metrowerks.
	#include <cwctype>
#endif

#if defined(__APPLE__)
    //	Framework includes from ProjectBuilder
    #include <CoreServices/CoreServices.h>
#else
    //	Classic includes otherwise
    #include <MacErrors.h>
    #include <Script.h>
    #include <TextUtils.h>
    #include <TextEncodingConverter.h>
    #include <TextCommon.h>
    #include <CodeFragments.h>
    #include <UnicodeConverter.h>
    #include <UnicodeUtilities.h>
    #include <CFCharacterSet.h>
    #include <CFString.h>
#endif

#include <xercesc/util/Transcoders/MacOSUnicodeConverter/MacOSUnicodeConverter.hpp>
#include <xercesc/util/XMLUniDefs.hpp>
#include <xercesc/util/XMLUni.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/TranscodingException.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/Janitor.hpp>
#include <xercesc/util/Platforms/MacOS/MacOSPlatformUtils.hpp>

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//	Typedefs
// ---------------------------------------------------------------------------

//	TempUniBuf is used for cases where we need a temporary conversion due to
//	a mismatch between UniChar (the 16-bit type that the Unicode converter uses)
//	and XMLCh (the type that Xerces uses to represent a Unicode character).
//	In the case of Metrowerks, these are the same size. For ProjectBuilder, they
//	used to differ, but they are now the same since XMLCh is now always fixed
//	as a 16 bit character, rather than floating with wchar_t as it used to.
//	*** Most uses of this buffer should be removed from this code in time! ***
//	TempUniBuf is also used for a few cases where we want to discard the
//	output from the unicode converter.
const std::size_t kTempUniBufCount = 256;
typedef UniChar	TempUniBuf[kTempUniBufCount];


// ---------------------------------------------------------------------------
//  Local, const data
// ---------------------------------------------------------------------------
static const XMLCh gMyServiceId[] =
{
    chLatin_M, chLatin_a, chLatin_c, chLatin_O, chLatin_S, chNull
};

//	Detect a mismatch in unicode character size.
#define UNICODE_SIZE_MISMATCH (sizeof(XMLCh) != sizeof(UniChar))


// ---------------------------------------------------------------------------
//  MacOSUnicodeConverter: Constructors and Destructor
// ---------------------------------------------------------------------------
MacOSUnicodeConverter::MacOSUnicodeConverter()
{
	//	Test for presense of unicode collation functions
	mHasUnicodeCollation = (UCCompareTextDefault != (void*)kUnresolvedCFragSymbolAddress);
}


MacOSUnicodeConverter::~MacOSUnicodeConverter()
{
}


// ---------------------------------------------------------------------------
//  MacOSUnicodeConverter: The virtual transcoding service API
// ---------------------------------------------------------------------------
int MacOSUnicodeConverter::compareIString(  const XMLCh* const    comp1
                                          , const XMLCh* const    comp2)
{
	//	If unicode collation routines are available, use them.
	//	This should be the case on Mac OS 8.6 and later,
	//	with Carbon 1.0.2 or later, and under Mac OS X.
	//
	//	Otherwise, but only for Metrowerks, since only Metrowerks
	//	has a c library with a valid set of wchar routines,
	//	fall back to the standard library.

	if (mHasUnicodeCollation)
	{
		// Use the Unicode Utilities to do the compare

		//	This has gotten more painful with the need to allow
		//	conversion between different sizes of XMLCh and UniChar.
		//	We allocate a static buffer and do multiple passes
		//	to allow for the case where the strings being compared
		//	are larger than the static buffer.

		UCCollateOptions collateOptions =
								kUCCollateComposeInsensitiveMask
								| kUCCollateWidthInsensitiveMask
								| kUCCollateCaseInsensitiveMask
								| kUCCollatePunctuationSignificantMask
								;
						
		std::size_t srcOffset = 0;
		std::size_t cnt1 = XMLString::stringLen(comp1);
		std::size_t cnt2 = XMLString::stringLen(comp2);
		
		//	Do multiple passes over source, comparing each pass.
		//	The first pass that's not equal wins.
		int result = 0;
		while (result == 0 && (cnt1 || cnt2))
		{
			TempUniBuf buf1;
			TempUniBuf buf2;
			
			const UniChar* src1;
			const UniChar* src2;
			
			std::size_t passCnt1;
			std::size_t passCnt2;
			
			if (UNICODE_SIZE_MISMATCH)
			{
				passCnt1 = std::min(cnt1, kTempUniBufCount);
				passCnt2 = std::min(cnt2, kTempUniBufCount);
				
				src1 = CopyXMLChsToUniChars(comp1 + srcOffset, buf1, passCnt1, kTempUniBufCount);
				src2 = CopyXMLChsToUniChars(comp2 + srcOffset, buf2, passCnt2, kTempUniBufCount);
			}
			else
			{
				passCnt1 = cnt1;
				passCnt2 = cnt2;

				src1 = reinterpret_cast<const UniChar*>(comp1);
				src2 = reinterpret_cast<const UniChar*>(comp2);
			}

			//	Do the actual compare for this pass
			Boolean equivalent = false;
			SInt32 order = 0;
			OSStatus status = UCCompareTextDefault(
									collateOptions,	
									src1,
									passCnt1,
									src2,
									passCnt2,
									&equivalent,
									&order
									);
									
			result = ((status != noErr) || equivalent) ? 0 : order;
			
			srcOffset += kTempUniBufCount;
			cnt1 -= passCnt1;
			cnt2 -= passCnt2;
		}
		
		return result;
	}
#if defined(XML_METROWERKS)
	else
	{
		const XMLCh* cptr1 = comp1;
		const XMLCh* cptr2 = comp2;
		
		while ( (*cptr1 != 0) && (*cptr2 != 0) )
		{
			std::wint_t wch1 = std::towupper(*cptr1);
			std::wint_t wch2 = std::towupper(*cptr2);
			if (wch1 != wch2)
				break;
			
			cptr1++;
			cptr2++;
		}
		return (int) (std::towupper(*cptr1) - std::towupper(*cptr2));
	}
#else
	else
	{
		//	For some reason there is no platform utils available
		//	where we expect it. Bail.
		XMLPlatformUtils::panic(PanicHandler::Panic_NoTransService);
		return 0;
	}
#endif
}


int MacOSUnicodeConverter::compareNIString( const   XMLCh* const    comp1
                                        , const XMLCh* const    comp2
                                        , const unsigned int    maxChars)
{
	//	If unicode collation routines are available, use them.
	//	This should be the case on Mac OS 8.6 and later,
	//	with Carbon 1.0.2 or later, and under Mac OS X.
	//
	//	Otherwise, but only for Metrowerks, since only Metrowerks
	//	has a c library with a valid set of wchar routines,
	//	fall back to the standard library.

	if (mHasUnicodeCollation)
	{
		// Use the Unicode Utilities to do the compare

		//	This has gotten more painful with the need to allow
		//	conversion between different sizes of XMLCh and UniChar.
		//	We allocate a static buffer and do multiple passes
		//	to allow for the case where the strings being compared
		//	are larger than the static buffer.

		UCCollateOptions collateOptions =
								kUCCollateComposeInsensitiveMask
								| kUCCollateWidthInsensitiveMask
								| kUCCollateCaseInsensitiveMask
								| kUCCollatePunctuationSignificantMask
								;
						
		std::size_t srcOffset = 0;
		std::size_t cnt1 = XMLString::stringLen(comp1);
		std::size_t cnt2 = XMLString::stringLen(comp2);
		
		//	Restrict view of source characters to first {maxChars}
		if (cnt1 > maxChars)
			cnt1 = maxChars;
			
		if (cnt2 > maxChars)
			cnt2 = maxChars;
		
		//	Do multiple passes over source, comparing each pass.
		//	The first pass that's not equal wins.
		int result = 0;
		while (result == 0 && (cnt1 || cnt2))
		{
			TempUniBuf buf1;
			TempUniBuf buf2;
			
			const UniChar* src1;
			const UniChar* src2;
			
			std::size_t passCnt1;
			std::size_t passCnt2;
			
			if (UNICODE_SIZE_MISMATCH)
			{
				passCnt1 = std::min(cnt1, kTempUniBufCount);
				passCnt2 = std::min(cnt2, kTempUniBufCount);
				
				src1 = CopyXMLChsToUniChars(comp1 + srcOffset, buf1, passCnt1, kTempUniBufCount);
				src2 = CopyXMLChsToUniChars(comp2 + srcOffset, buf2, passCnt2, kTempUniBufCount);
			}
			else
			{
				passCnt1 = cnt1;
				passCnt2 = cnt2;

				src1 = reinterpret_cast<const UniChar*>(comp1);
				src2 = reinterpret_cast<const UniChar*>(comp2);
			}

			//	Do the actual compare for this pass
			Boolean equivalent = false;
			SInt32 order = 0;
			OSStatus status = UCCompareTextDefault(
									collateOptions,	
									src1,
									passCnt1,
									src2,
									passCnt2,
									&equivalent,
									&order
									);
									
			result = ((status != noErr) || equivalent) ? 0 : order;
			
			srcOffset += kTempUniBufCount;
			cnt1 -= passCnt1;
			cnt2 -= passCnt2;
		}
		
		return result;
	}
#if defined(XML_METROWERKS)
	else
	{
		unsigned int  n = 0;
		const XMLCh* cptr1 = comp1;
		const XMLCh* cptr2 = comp2;
	
		while ( (*cptr1 != 0) && (*cptr2 != 0) && (n < maxChars) )
		{
			std::wint_t wch1 = std::towupper(*cptr1);
			std::wint_t wch2 = std::towupper(*cptr2);
			if (wch1 != wch2)
				break;
			
			cptr1++;
			cptr2++;
			n++;
		}
		return (int)(std::towupper(*cptr1) - std::towupper(*cptr2));
	}
#else
	else
	{
		//	For some reason there is no platform utils available
		//	where we expect it. Bail.
		XMLPlatformUtils::panic(PanicHandler::Panic_NoTransService);
		return 0;
	}
#endif
}


const XMLCh* MacOSUnicodeConverter::getId() const
{
    return gMyServiceId;
}


bool MacOSUnicodeConverter::isSpace(const XMLCh toCheck) const
{
#if TARGET_API_MAC_CARBON

   //	Return true if the specified character is in the set.
   CFCharacterSetRef wsSet = CFCharacterSetGetPredefined(kCFCharacterSetWhitespaceAndNewline);
   return CFCharacterSetIsCharacterMember(wsSet, toCheck);

#elif defined(XML_METROWERKS) || (__GNUC__ >= 3 && _GLIBCPP_USE_WCHAR_T)

	// Use this if there's a reasonable c library available.
	// ProjectBuilder currently has no support for iswspace ;(
    return (std::iswspace(toCheck) != 0);

#endif
}


XMLLCPTranscoder* MacOSUnicodeConverter::makeNewLCPTranscoder()
{
	XMLLCPTranscoder* result = NULL;
	OSStatus status = noErr;
	TextToUnicodeInfo textToUnicodeInfo = NULL;
	UnicodeToTextInfo unicodeToTextInfo = NULL;
	
	//	Get TextEncoding for current Mac System Script
	TextEncoding systemTextEncoding;
	status = UpgradeScriptInfoToTextEncoding(
		smSystemScript, kTextLanguageDontCare, kTextRegionDontCare,
		NULL, &systemTextEncoding);
	
	//	Try to create a Unicode converter from the system encoding to Unicode
	if (status == noErr)
		status = CreateTextToUnicodeInfoByEncoding(
							systemTextEncoding,
							&textToUnicodeInfo);
							
	//	Try to create a Unicode converter from Unicode to the system encoding
	if (status == noErr)
		status = CreateUnicodeToTextInfoByEncoding(
							systemTextEncoding,
							&unicodeToTextInfo);
				
	//	Clean up on error, else create our transcoder			
	if (status != noErr)
	{
		if (textToUnicodeInfo != NULL)
			DisposeTextToUnicodeInfo(&textToUnicodeInfo);
			
		if (unicodeToTextInfo != NULL)
			DisposeUnicodeToTextInfo(&unicodeToTextInfo);
	}
	else
		result = new MacOSLCPTranscoder(textToUnicodeInfo, unicodeToTextInfo);
	
    return result;
}


bool MacOSUnicodeConverter::supportsSrcOfs() const
{
	// For now, we don't support source offsets
    return false;
}


void MacOSUnicodeConverter::upperCase(XMLCh* const toUpperCase) const
{
#if TARGET_API_MAC_CARBON

   // If we're targeting carbon, use the CFString conversion to uppercase
   int len = XMLString::stringLen(toUpperCase);
   CFMutableStringRef cfString = CFStringCreateMutableWithExternalCharactersNoCopy(
        kCFAllocatorDefault,
        (UniChar*)toUpperCase,
        len,		// length
        len,		// capacity
        kCFAllocatorNull);
   CFStringUppercase(cfString, NULL);
   CFRelease(cfString);

#elif defined(XML_METROWERKS) || (__GNUC__ >= 3 && _GLIBCPP_USE_WCHAR_T)

	// Use this if there's a reasonable c library available.
	// Metrowerks does this reasonably
	wchar_t c;
	for (XMLCh* p = (XMLCh*)toUpperCase; ((c = *p) != 0); )
		*p++ = std::towupper(c);

#else
	#error Sorry, no support for upperCase
#endif
}


void MacOSUnicodeConverter::lowerCase(XMLCh* const toLowerCase) const
{
#if TARGET_API_MAC_CARBON

   // If we're targeting carbon, use the CFString conversion to uppercase
   int len = XMLString::stringLen(toLowerCase);
   CFMutableStringRef cfString = CFStringCreateMutableWithExternalCharactersNoCopy(
        kCFAllocatorDefault,
        (UniChar*)toLowerCase,
        len,		// length
        len,		// capacity
        kCFAllocatorNull);
   CFStringLowercase(cfString, NULL);
   CFRelease(cfString);

#elif defined(XML_METROWERKS) || (__GNUC__ >= 3 && _GLIBCPP_USE_WCHAR_T)

	// Use this if there's a reasonable c library available.
	// Metrowerks does this reasonably
	wchar_t c;
	for (XMLCh* p = (XMLCh*)toLowerCase; ((c = *p) != 0); )
		*p++ = std::towlower(c);

#else
	#error Sorry, no support for lowerCase
#endif
}


void
MacOSUnicodeConverter::ConvertWideToNarrow(const XMLCh* wide, char* narrow, std::size_t maxChars)
{
	while (maxChars-- > 0)
		if ((*narrow++ = *wide++) == 0)
			break;
}


// ---------------------------------------------------------------------------
//  MacOSTransService: The protected virtual transcoding service API
// ---------------------------------------------------------------------------
XMLTranscoder*
MacOSUnicodeConverter::makeNewXMLTranscoder(const   XMLCh* const		encodingName
                                        ,       XMLTransService::Codes& resValue
                                        , const unsigned int			blockSize
                                        ,       MemoryManager* const    manager)
{
	XMLTranscoder* result = NULL;
	resValue = XMLTransService::Ok;
	
	TextToUnicodeInfo textToUnicodeInfo = NULL;
	UnicodeToTextInfo unicodeToTextInfo = NULL;

	//	Map the encoding to a Mac OS Encoding value
	TextEncoding textEncoding;
	Str255 pasEncodingName;
	char cEncodingName[256];
	
	// еее TODO: Use Transcode instead here!
	ConvertWideToNarrow(encodingName, cEncodingName, sizeof(cEncodingName));
	CopyCStringToPascal(cEncodingName, pasEncodingName);
	
	OSStatus status = TECGetTextEncodingFromInternetName (
							&textEncoding,
							pasEncodingName);
							
	//	Try to create a Unicode converter from the encoding to Unicode
	if (status == noErr)
		status = CreateTextToUnicodeInfoByEncoding(
							textEncoding,
							&textToUnicodeInfo);

	//	Try to create a Unicode converter from Unicode to the encoding
	if (status == noErr)
		status = CreateUnicodeToTextInfoByEncoding(
							textEncoding,
							&unicodeToTextInfo);

	//	Now create our own encoder, passing along the unicode converters.
	if (status != noErr)
	{
		if (textToUnicodeInfo != NULL)
			DisposeTextToUnicodeInfo(&textToUnicodeInfo);
			
		if (unicodeToTextInfo != NULL)
			DisposeUnicodeToTextInfo(&unicodeToTextInfo);

		resValue = XMLTransService::UnsupportedEncoding;
	}
	else
		result = new (manager) MacOSTranscoder(encodingName, textToUnicodeInfo, unicodeToTextInfo, blockSize, manager);
	
    return result;
}


// ---------------------------------------------------------------------------
//  IsMacOSUnicodeConverterSupported
// ---------------------------------------------------------------------------
bool
MacOSUnicodeConverter::IsMacOSUnicodeConverterSupported(void)
{
    return UpgradeScriptInfoToTextEncoding != (void*)kUnresolvedCFragSymbolAddress
        && CreateTextToUnicodeInfoByEncoding != (void*)kUnresolvedCFragSymbolAddress
        ;
}


// ---------------------------------------------------------------------------
//  MacOSTranscoder: Constructors and Destructor
// ---------------------------------------------------------------------------
MacOSTranscoder::MacOSTranscoder(const  XMLCh* const encodingName
								, TextToUnicodeInfo	 textToUnicodeInfo
								, UnicodeToTextInfo	 unicodeToTextInfo
                                , const unsigned int blockSize
                                , MemoryManager* const manager) :
    XMLTranscoder(encodingName, blockSize, manager),
    mTextToUnicodeInfo(textToUnicodeInfo),
    mUnicodeToTextInfo(unicodeToTextInfo)
{
}


MacOSTranscoder::~MacOSTranscoder()
{
	//	Dispose our unicode converters
	DisposeTextToUnicodeInfo(&mTextToUnicodeInfo);
	DisposeUnicodeToTextInfo(&mUnicodeToTextInfo);
}


// ---------------------------------------------------------------------------
//  MacOSTranscoder: The virtual transcoder API
// ---------------------------------------------------------------------------

unsigned int
MacOSTranscoder::transcodeFrom(  const  XMLByte* const			srcData
                                , const unsigned int			srcCount
                                ,       XMLCh* const			toFill
                                , const unsigned int			maxChars
                                ,       unsigned int&			bytesEaten
                                ,       unsigned char* const	charSizes)
{
    ByteCount	bytesConsumed = 0;
    ByteCount	bytesProduced = 0;

    OSStatus status = ConvertFromTextToUnicode(
    	mTextToUnicodeInfo,
    	srcCount,
    	srcData,
    	kUnicodeUseFallbacksMask
		// | kUnicodeKeepInfoMask
		| kUnicodeDefaultDirectionMask
		| kUnicodeLooseMappingsMask
		// | kUnicodeStringUnterminatedMask
		// | kUnicodeTextRunMask
		,									// control flags
    	0,									// ioffset count
    	NULL,								// ioffset array
    	0,									// ooffset count
    	NULL,								// ooffset array
    	maxChars * sizeof(UniChar),			// iOutputBufLen (bytes)
    	&bytesConsumed,
    	&bytesProduced,
    	reinterpret_cast<UniCharArrayPtr>(toFill));
		
	if (status == kTECOutputBufferFullStatus || status == kTECUsedFallbacksStatus)
		status = noErr;
    	
    if (status != noErr)
        ThrowXML(TranscodingException, XMLExcepts::Trans_BadSrcSeq);
	
	std::size_t charsProduced = bytesProduced / sizeof(UniChar);
	
	//	If XMLCh is not same length as UniChar (previously the case under GCC)
	//	then we need to convert the UniChar characters up to
	//	XMLCh. We lied about the max buffer length above in
	//	order to leave room in our output buffer. So we know
	//	we're in good shape here to just convert in place.
	if (UNICODE_SIZE_MISMATCH)
		CopyUniCharsToXMLChs(reinterpret_cast<UniChar* const>(toFill), toFill, charsProduced, maxChars);
		    	
    bytesEaten = bytesConsumed;
    return charsProduced;
}


unsigned int
MacOSTranscoder::transcodeTo(const  XMLCh* const    srcData
                            , const unsigned int    srcCount
                            ,       XMLByte* const  toFill
                            , const unsigned int    maxBytes
                            ,       unsigned int&   charsEaten
                            , const UnRepOpts       options)
{
    ByteCount totalCharsConsumed = 0;
    ByteCount totalCharsProduced = 0;
	
	const XMLCh* src	= srcData;
	std::size_t srcCnt	= srcCount;

	OptionBits controlFlags =
		  0
		// | kUnicodeUseFallbacksMask
		| kUnicodeLooseMappingsMask
		// | kUnicodeKeepInfoMask
		// | kUnicodeStringUnterminatedMask
		;
		
	if (options == UnRep_RepChar)
		controlFlags |= kUnicodeUseFallbacksMask;

	//	Do multiple passes of conversion, potentially,
	//	in order to handle the case of a character size
	//	mismatch.
	OSStatus status;
	for (status = noErr; status == noErr && srcCnt > 0; )
	{
	    ByteCount		bytesConsumed = 0;
	    ByteCount		bytesProduced = 0;
		std::size_t		passCnt = 0;
		const UniChar*	passSrc = NULL;
		
		//	Setup source buffer as needed to accomodate a unicode
		//	character size mismatch.
		TempUniBuf	buf;
		if (UNICODE_SIZE_MISMATCH)
		{
			passCnt = std::min(srcCnt, kTempUniBufCount);
			passSrc = CopyXMLChsToUniChars(src, buf, passCnt, kTempUniBufCount);
		}
		else
		{
			passCnt = srcCnt;
			passSrc = reinterpret_cast<const UniChar*>(src);
		}

	    status = ConvertFromUnicodeToText(
					mUnicodeToTextInfo,
					passCnt * sizeof(UniChar),	// src byte count
					reinterpret_cast<const UniChar*>(passSrc),
					controlFlags,		// control flags
					0,					// ioffset count
					NULL,				// ioffset array
					0,					// ooffset count
					NULL,				// ooffset array
					maxBytes - totalCharsProduced,
					&bytesConsumed,
					&bytesProduced,
					toFill + totalCharsProduced);
				
		if (status == kTECUsedFallbacksStatus)
			status = noErr;
    	
		if (UNICODE_SIZE_MISMATCH && status == kTECOutputBufferFullStatus)
			status = noErr;

		std::size_t charsConsumed = bytesConsumed / sizeof(UniChar);
		
		src		+= charsConsumed;
		srcCnt	-= charsConsumed;
		
		totalCharsConsumed += charsConsumed;
		totalCharsProduced += bytesProduced;
					
		controlFlags |= kUnicodeKeepInfoMask;
	}
    	
    if (status != noErr)
    {
    	if (status == kTECUnmappableElementErr && options == UnRep_Throw)
    	{
    		XMLCh tmpBuf[17];
            XMLString::binToText((unsigned int)&srcData[totalCharsConsumed], tmpBuf, 16, 16);
            ThrowXML2
            (
                TranscodingException
                , XMLExcepts::Trans_Unrepresentable
                , tmpBuf
                , getEncodingName()
            );
    	}
    }
    	
    charsEaten = totalCharsConsumed;
    return totalCharsProduced;
}


bool
MacOSTranscoder::canTranscodeTo(const unsigned int toCheck) const
{
	//
    //  If the passed value is really a surrogate embedded together, then
    //  we need to break it out into its two chars. Else just one.
    //
    unsigned int    srcCnt = 0;
    UniChar         srcBuf[2];

    if (toCheck & 0xFFFF0000)
    {
        srcBuf[srcCnt++] = XMLCh(toCheck >> 10)   + 0xD800;
        srcBuf[srcCnt++] = XMLCh(toCheck & 0x3FF) + 0xDC00;
    }
    else
    {
        srcBuf[srcCnt++] = XMLCh(toCheck);
    }

    //
    //  Use a local temp buffer that would hold any sane multi-byte char
    //  sequence and try to transcode this guy into it.
    //
    char tmpBuf[64];

    OSStatus status;
	OptionBits controlFlags = kUnicodeLooseMappingsMask;
	ByteCount bytesConsumed;
	ByteCount bytesProduced;

    status = ConvertFromUnicodeToText(
		mUnicodeToTextInfo,
		srcCnt * sizeof(UniChar),
		srcBuf,
		controlFlags,		// control flags
		0,					// ioffset count
		NULL,				// ioffset array
		0,					// ooffset count
		NULL,				// ooffset array
		sizeof(tmpBuf),
		&bytesConsumed,
		&bytesProduced,
		tmpBuf
	);
	
	//	Return true if we transcoded the character(s)
	//	successfully
	return status == noErr && bytesProduced > 0;
}


// ---------------------------------------------------------------------------
//  MacOSLCPTranscoder: Constructors and Destructor
// ---------------------------------------------------------------------------
MacOSLCPTranscoder::MacOSLCPTranscoder(TextToUnicodeInfo textToUnicodeInfo, UnicodeToTextInfo unicodeToTextInfo)
 : mTextToUnicodeInfo(textToUnicodeInfo),
   mUnicodeToTextInfo(unicodeToTextInfo)
{
}


MacOSLCPTranscoder::~MacOSLCPTranscoder()
{
	//	Dispose our unicode converters
	DisposeTextToUnicodeInfo(&mTextToUnicodeInfo);
	DisposeUnicodeToTextInfo(&mUnicodeToTextInfo);
}


// ---------------------------------------------------------------------------
//  MacOSLCPTranscoder: Implementation of the virtual transcoder interface
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
//	In order to implement calcRequiredSize we have to go ahead and do the
//	conversion, which seems quite painful. The Mac Unicode converter has
//	no way of saying "don't actually do the conversion." So we end up
//	converting twice. It would be nice if the calling code could do some
//	extra buffering to avoid this result.
// ---------------------------------------------------------------------------
unsigned int
MacOSLCPTranscoder::calcRequiredSize(const char* const srcText
                                     , MemoryManager* const manager)
{
	if (!srcText)
		return 0;

	std::size_t totalCharsProduced = 0;

	const char* src		= srcText;
	std::size_t srcCnt	= std::strlen(src);
	
	OptionBits options =
		  kUnicodeUseFallbacksMask
		// | kUnicodeKeepInfoMask
		| kUnicodeDefaultDirectionMask
		| kUnicodeLooseMappingsMask
		// | kUnicodeStringUnterminatedMask
		// | kUnicodeTextRunMask
		;

	OSStatus status;
	for (status = noErr; status == noErr && srcCnt > 0; )
	{
	    ByteCount	bytesConsumed = 0;
	    ByteCount	bytesProduced = 0;
		
		TempUniBuf buf;

	    status = ConvertFromTextToUnicode(
	    	mTextToUnicodeInfo,
	    	srcCnt,				// src byte count
	    	src,
	    	options,			// control flags
	    	0,					// ioffset count
	    	NULL,				// ioffset array
	    	0,					// ooffset count
	    	NULL,				// ooffset array
	    	kTempUniBufCount * sizeof(UniChar),
	    	&bytesConsumed,
	    	&bytesProduced,
	    	buf);
			
		src		+= bytesConsumed;
		srcCnt	-= bytesConsumed;
		totalCharsProduced += bytesProduced / sizeof(UniChar);

		if (status == kTECOutputBufferFullStatus || status == kTECUsedFallbacksStatus)
			status = noErr;

		options |= kUnicodeKeepInfoMask;
	}
	
	if (status != noErr && status != kTECPartialCharErr)
		totalCharsProduced = 0;
	
	//	Return number of XMLCh characters required (not counting terminating NULL!)
	return totalCharsProduced;
}


// ---------------------------------------------------------------------------
//	In order to implement calcRequiredSize we have to go ahead and do the
//	conversion, which seems quite painful. The Mac Unicode converter has
//	no way of saying "don't actually do the conversion." So we end up
//	converting twice. It would be nice if the calling code could do some
//	extra buffering to avoid this result.
// ---------------------------------------------------------------------------
unsigned int
MacOSLCPTranscoder::calcRequiredSize(const XMLCh* const srcText
                                     , MemoryManager* const manager)
{
	if (!srcText)
		return 0;

	std::size_t		totalBytesProduced = 0;

	const XMLCh*	src		= srcText;
	std::size_t		srcCnt	= XMLString::stringLen(src);

	OptionBits options =
		  kUnicodeUseFallbacksMask
		| kUnicodeLooseMappingsMask
		// | kUnicodeKeepInfoMask
		// | kUnicodeStringUnterminatedMask
		;

	OSStatus status;
	for (status = noErr; status == noErr && srcCnt > 0; )
	{
	    ByteCount		bytesConsumed = 0;
	    ByteCount		bytesProduced = 0;
		std::size_t		passCnt = 0;
		const UniChar*	passSrc = NULL;
		
		//	Setup source buffer as needed to accomodate a unicode
		//	character size mismatch.
		TempUniBuf	iBuf;
		if (UNICODE_SIZE_MISMATCH)
		{
			passCnt = std::min(srcCnt, kTempUniBufCount);
			passSrc = CopyXMLChsToUniChars(src, iBuf, passCnt, kTempUniBufCount);
		}
		else
		{
			passCnt = srcCnt;
			passSrc = reinterpret_cast<const UniChar*>(src);
		}
		
		TempUniBuf oBuf;

	    status = ConvertFromUnicodeToText(
	    	mUnicodeToTextInfo,
	    	passCnt * sizeof(UniChar),	// src byte count
	    	passSrc,			// source buffer
	    	options,			// control flags
	    	0,					// ioffset count
	    	NULL,				// ioffset array
	    	0,					// ooffset count
	    	NULL,				// ooffset array
			kTempUniBufCount * sizeof(UniChar),	// output buffer size in bytes
	    	&bytesConsumed,
	    	&bytesProduced,
			oBuf);
			
		std::size_t charsConsumed = bytesConsumed / sizeof(UniChar);
		src		+= charsConsumed;
		srcCnt	-= charsConsumed;

		totalBytesProduced += bytesProduced;

		if (status == kTECOutputBufferFullStatus || status == kTECUsedFallbacksStatus)
			status = noErr;

		options |= kUnicodeKeepInfoMask;
	}
	
	if (status != noErr && status != kTECPartialCharErr)
		totalBytesProduced = 0;
	
	//	Return number of bytes required (not counting terminating NULL!)
	return totalBytesProduced;
}


char*
MacOSLCPTranscoder::transcode(const XMLCh* const srcText)
{
	//	Transcode using a memory manager that allocates
	//	memory using new[].
	return transcode(srcText, XMLPlatformUtils::fgArrayMemoryManager);
}


char*
MacOSLCPTranscoder::transcode(const XMLCh* const srcText,
                              MemoryManager* const manager)
{
	if (!srcText)
		return NULL;

	ArrayJanitor<char> result(0);
	const XMLCh* src		= srcText;
	std::size_t srcCnt		= XMLString::stringLen(src);
	std::size_t resultCnt	= 0;

	OptionBits options =
		  kUnicodeUseFallbacksMask
		| kUnicodeLooseMappingsMask
		// | kUnicodeKeepInfoMask
		// | kUnicodeStringUnterminatedMask
		;

	OSStatus status;
	for (status = noErr; status == noErr && srcCnt > 0; )
	{
		//	Convert an (internal) buffer full of text
	    ByteCount		bytesConsumed = 0;
	    ByteCount		bytesProduced = 0;
		std::size_t		passCnt = 0;
		const UniChar*	passSrc = NULL;
		
		//	Setup source buffer as needed to accomodate a unicode
		//	character size mismatch.
		TempUniBuf	iBuf;
		if (UNICODE_SIZE_MISMATCH)
		{
			passCnt = std::min(srcCnt, kTempUniBufCount);
			passSrc = CopyXMLChsToUniChars(src, iBuf, passCnt, kTempUniBufCount);
		}
		else
		{
			passCnt = srcCnt;
			passSrc = reinterpret_cast<const UniChar*>(src);
		}
		
		TempUniBuf oBuf;
		
	    status = ConvertFromUnicodeToText(
	    	mUnicodeToTextInfo,
	    	passCnt * sizeof(UniChar),	// src byte count
	    	passSrc,			// source buffer
	    	options,			// control flags
	    	0,					// ioffset count
	    	NULL,				// ioffset array
	    	0,					// ooffset count
	    	NULL,				// ooffset array
	    	kTempUniBufCount * sizeof(UniChar),
	    	&bytesConsumed,
	    	&bytesProduced,
	    	oBuf);
			
		//	Move the data to result buffer, reallocating as needed
		if (bytesProduced > 0)
		{
			//	Allocate space for result
			std::size_t newCnt = resultCnt + bytesProduced;
			ArrayJanitor<char> newResult
            (
                (char*) manager->allocate((newCnt + 1) * sizeof(char)) //new char[newCnt + 1]
                , manager
            );
			if (newResult.get() != NULL)
			{
				//	Incorporate previous result
				if (result.get() != NULL)
					std::memcpy(newResult.get(), result.get(), resultCnt);
				result.reset(newResult.release());

				//	Copy in new data
				std::memcpy(result.get() + resultCnt, oBuf, bytesProduced);
				resultCnt = newCnt;
				
				result[resultCnt] = '\0';					
			}
		}
		
		std::size_t charsConsumed = bytesConsumed / sizeof(UniChar);
		src		+= charsConsumed;
		srcCnt	-= charsConsumed;

		if (status == kTECOutputBufferFullStatus || status == kTECUsedFallbacksStatus)
			status = noErr;

		options |= kUnicodeKeepInfoMask;
	}
	
	if (status != noErr && status != kTECPartialCharErr)
		result.reset();
	else if (!result.get())
	{
		//	No error, and no result: we probably processed a zero length
		//	input, in which case we want a valid zero length output.
		result.reset
        (
            (char*) manager->allocate(sizeof(char))//new char[1]
            , manager
        );
		result[0] = '\0';
	}
	
	return result.release();
}


XMLCh*
MacOSLCPTranscoder::transcode(const char* const srcText)
{
	//	Transcode using a memory manager that allocates
	//	memory using new[].
	return transcode(srcText, XMLPlatformUtils::fgArrayMemoryManager);
}


XMLCh*
MacOSLCPTranscoder::transcode(const char* const srcText,
                              MemoryManager* const manager)
{
	if (!srcText)
		return NULL;

	ArrayJanitor<XMLCh> result(0);
	const char* src			= srcText;
	std::size_t srcCnt		= std::strlen(src);
	std::size_t resultCnt	= 0;

	OptionBits options =
		  kUnicodeUseFallbacksMask
		// | kUnicodeKeepInfoMask
		| kUnicodeDefaultDirectionMask
		| kUnicodeLooseMappingsMask
		// | kUnicodeStringUnterminatedMask
		// | kUnicodeTextRunMask
		;

	OSStatus status;
	for (status = noErr; status == noErr && srcCnt > 0; )
	{
		//	Convert an (internal) buffer full of text
	    ByteCount	bytesConsumed = 0;
	    ByteCount	bytesProduced = 0;
		
		TempUniBuf buf;

	    status = ConvertFromTextToUnicode(
	    	mTextToUnicodeInfo,
	    	srcCnt,				// src byte count
	    	src,
	    	options,			// control flags
	    	0,					// ioffset count
	    	NULL,				// ioffset array
	    	0,					// ooffset count
	    	NULL,				// ooffset array
	    	kTempUniBufCount * sizeof(UniChar),	// Byte count of destination buffer
	    	&bytesConsumed,
	    	&bytesProduced,
	    	buf);
		
		std::size_t charsProduced = bytesProduced / sizeof(UniChar);
		
		//	Move the data to result buffer, reallocating as needed
		if (charsProduced > 0)
		{
			//	Allocate space for result
			std::size_t newCnt = resultCnt + charsProduced;
			ArrayJanitor<XMLCh> newResult
            (
                (XMLCh*) manager->allocate((newCnt + 1) * sizeof(XMLCh))//new XMLCh[newCnt + 1]
                , manager
            );
			if (newResult.get() != NULL)
			{
				//	Incorporate previous result
				if (result.get() != NULL)
					std::memcpy(newResult.get(), result.get(), resultCnt * sizeof(XMLCh));
				result.reset(newResult.release());

				//	Copy in new data, converting character formats as necessary
				CopyUniCharsToXMLChs(buf, result.get() + resultCnt, charsProduced, charsProduced);
				resultCnt = newCnt;
				
				result[resultCnt] = 0;			
			}
		}

		src		+= bytesConsumed;
		srcCnt  -= bytesConsumed;

		if (status == kTECOutputBufferFullStatus || status == kTECUsedFallbacksStatus)
			status = noErr;
			
		options |= kUnicodeKeepInfoMask;
	}
	
	if (status != noErr && status != kTECPartialCharErr)
		result.reset();
	else if (!result.get())
	{
		//	No error, and no result: we probably processed a zero length
		//	input, in which case we want a valid zero length output.
		result.reset
        (
            (XMLCh*) manager->allocate(sizeof(XMLCh))//new XMLCh[1]
            , manager
        );
		result[0] = '\0';
	}
	
	return result.release();
}


bool
MacOSLCPTranscoder::transcode( 		 const   char* const	toTranscode
                                    ,       XMLCh* const    toFill
                                    , const unsigned int    maxChars
                                    , MemoryManager* const  manager)
{
    // toFill must contain space for maxChars XMLCh characters + 1 (for terminating NULL).

    // Check for a couple of psycho corner cases
    if (!toTranscode || !maxChars)
    {
        toFill[0] = 0;
        return true;
    }

    if (!*toTranscode)
    {
        toFill[0] = 0;
        return true;
    }

    ByteCount	bytesConsumed = 0;
    ByteCount	bytesProduced = 0;
	
	OptionBits options =
		  kUnicodeUseFallbacksMask
		// | kUnicodeKeepInfoMask
		| kUnicodeDefaultDirectionMask
		| kUnicodeLooseMappingsMask
		// | kUnicodeStringUnterminatedMask
		// | kUnicodeTextRunMask
		;

    OSStatus status = ConvertFromTextToUnicode(
    	mTextToUnicodeInfo,
    	std::strlen(toTranscode),	// src byte count
    	toTranscode,
    	options,				// control flags
    	0,						// ioffset count
    	NULL,					// ioffset array
    	0,						// ooffset count
    	NULL,					// ooffset array
    	maxChars * sizeof(UniChar),
    	&bytesConsumed,
    	&bytesProduced,
    	reinterpret_cast<UniChar* const>(toFill));
		
    std::size_t charsProduced = bytesProduced / sizeof(UniChar);

	//	If we have a size mismatch, then convert from UniChar to
	//	XMLCh in place within the output buffer.
	if (UNICODE_SIZE_MISMATCH)
		CopyUniCharsToXMLChs(reinterpret_cast<UniChar* const>(toFill), toFill, charsProduced, maxChars);
		
    //	Zero terminate the output string
    toFill[charsProduced] = L'\0';

	if (status == kTECUsedFallbacksStatus)
		status = noErr;

	return (status == noErr);
}


bool
MacOSLCPTranscoder::transcode( 		const   XMLCh* const    toTranscode
                                    ,       char* const     toFill
                                    , const unsigned int    maxChars
                                    , MemoryManager* const  manager)
{
    //	toFill must contain space for maxChars bytes characters + 1 (for terminating NULL).
	const XMLCh* src = toTranscode;
	std::size_t	 srcCnt = XMLString::stringLen(src);
	std::size_t	 totalCharsProduced = 0;
	
    // Watch for a few psycho corner cases
    if (!toTranscode || !maxChars)
    {
        toFill[0] = 0;
        return true;
    }

	// Set up options for converter
	OptionBits options =
		  kUnicodeUseFallbacksMask
		| kUnicodeLooseMappingsMask
		// | kUnicodeKeepInfoMask
		// | kUnicodeStringUnterminatedMask
		;

	//	Do multiple passes of conversion, potentially,
	//	in order to handle the case of a character size
	//	mismatch.
	OSStatus status;
	for (status = noErr; status == noErr && srcCnt > 0; )
	{
	    ByteCount		bytesConsumed = 0;
	    ByteCount		bytesProduced = 0;
		std::size_t		passCnt = 0;
		const UniChar*	passSrc = NULL;
		
		//	Setup source buffer as needed to accomodate a unicode
		//	character size mismatch.
		TempUniBuf	buf;
		if (UNICODE_SIZE_MISMATCH)
		{
			passCnt = std::min(srcCnt, kTempUniBufCount);
			passSrc = CopyXMLChsToUniChars(src, buf, passCnt, kTempUniBufCount);
		}
		else
		{
			passCnt = srcCnt;
			passSrc = reinterpret_cast<const UniChar*>(src);
		}

		status = ConvertFromUnicodeToText(
			mUnicodeToTextInfo,
			passCnt * sizeof(UniChar),	// src byte count
			passSrc,				// src buffer
			options,				// control flags
			0,						// ioffset count
			NULL,					// ioffset array
			0,						// ooffset count
			NULL,					// ooffset array
			maxChars - totalCharsProduced,
			&bytesConsumed,
			&bytesProduced,
			toFill + totalCharsProduced);
			
		std::size_t charsConsumed = bytesConsumed / sizeof(UniChar);

		src		+= charsConsumed;
		srcCnt	-= charsConsumed;
		
		totalCharsProduced += bytesProduced;
			
		if (status == kTECUsedFallbacksStatus)
			status = noErr;
			
		if (UNICODE_SIZE_MISMATCH && status == kTECOutputBufferFullStatus)
			status = noErr;

		options |= kUnicodeKeepInfoMask;
	}
			
    //	Zero terminate the output string
    toFill[totalCharsProduced] = '\0';

	return (status == noErr);
}

XERCES_CPP_NAMESPACE_END
