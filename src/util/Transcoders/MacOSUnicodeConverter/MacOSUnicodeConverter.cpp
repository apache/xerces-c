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
 * Revision 1.1  2000/07/18 18:26:15  andyh
 * Mac OS update.
 * Contributed by James Berry <jberry@criticalpath.com>
 *
 * Revision 1.8  2000/02/06 07:48:34  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.7  2000/01/25 23:14:19  roddey
 * Borland does not support wcsupr(), but does support _wcsupr(). Since VC++ does also,
 * _wcsupr() was used in order to have the both work with the same transcoding code.
 *
 * Revision 1.6  2000/01/25 22:49:58  roddey
 * Moved the supportsSrcOfs() method from the individual transcoder to the
 * transcoding service, where it should have been to begin with.
 *
 * Revision 1.5  2000/01/25 19:19:09  roddey
 * Simple addition of a getId() method to the xcode and netacess abstractions to
 * allow each impl to give back an id string.
 *
 * Revision 1.4  1999/12/18 00:22:33  roddey
 * Changes to support the new, completely orthagonal, transcoder architecture.
 *
 * Revision 1.3  1999/12/15 19:44:02  roddey
 * Now implements the new transcoding abstractions, with separate interface
 * classes for XML transcoders and local code page transcoders.
 *
 * Revision 1.2  1999/12/01 18:54:26  roddey
 * Small syntactical change to make it compile under Borland's compiler.
 * It does not make it worse under other compilers, so why not. Basically
 * it was just changing ::iswspace() to iswspace(), i.e. get rid of the ::
 * prefix, which freaked out Borland for some reason.
 *
 * Revision 1.1.1.1  1999/11/09 01:06:04  twl
 * Initial checkin
 *
 * Revision 1.2  1999/11/08 20:45:35  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <MacOSUnicodeConverter.hpp>
#include <util/XMLUni.hpp>
#include <util/XMLString.hpp>
#include <util/TranscodingException.hpp>

#if defined(XML_METROWERKS)
#include <cwctype>
#endif

#include <cstddef>
#include <cstring>

#include <Errors.h>
#include <Script.h>
#include <TextUtils.h>
#include <TextEncodingConverter.h>
#include <TextCommon.h>
#include <UnicodeConverter.h>
#include <UnicodeUtilities.h>


// ---------------------------------------------------------------------------
//  Local, const data
// ---------------------------------------------------------------------------
static const XMLCh gMyServiceId[] =
{
    chLatin_M, chLatin_a, chLatin_c, chLatin_O, chLatin_S, chNull
};



// ---------------------------------------------------------------------------
//  MacOSUnicodeConverter: Constructors and Destructor
// ---------------------------------------------------------------------------
MacOSUnicodeConverter::MacOSUnicodeConverter()
{
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
#if 0
	// We could use this if we have a reasonable c library
	// metrowerks supports all of this.
	// Note however that the c library version of towupper is probably
	// not very good. The Unicode Utilities compare routine is probably
	// more diligent, though surely not as quick.
	const XMLCh* cptr1 = comp1;
	const XMLCh* cptr2 = comp2;
	
	while ( (*cptr1 != 0) && (*cptr2 != 0) )
	{
	   wint_t wch1 = std::towupper(*cptr1);
	   wint_t wch2 = std::towupper(*cptr2);
	   if (wch1 != wch2)
	       break;
	   
	   cptr1++;
	   cptr2++;
	}
	return (int) ( std::towupper(*cptr1) - std::towupper(*cptr2) );
#else
	// Use the Unicode Utilities to do the compare
    UCCollateOptions collateOptions = 
                              kUCCollateComposeInsensitiveMask
                            | kUCCollateWidthInsensitiveMask
                            | kUCCollateCaseInsensitiveMask
                            | kUCCollatePunctuationSignificantMask
                            ;
                            
    Boolean equivalent = false;
    SInt32 order = 0;
    OSStatus status = UCCompareTextDefault(
                            collateOptions,	
                            reinterpret_cast<const UniChar* const>(comp1),
                            XMLString::stringLen(comp1),
                            reinterpret_cast<const UniChar* const>(comp2),
                            XMLString::stringLen(comp2),
                            &equivalent,
                            &order
                            );
                            
    return ((status != noErr) || equivalent) ? 0 : order;
#endif
}


int MacOSUnicodeConverter::compareNIString( const   XMLCh* const    comp1
                                        , const XMLCh* const    comp2
                                        , const unsigned int    maxChars)
{
#if 0
	// We could use this if we have a reasonable c library
	// metrowerks supports all of this.
	// Note however that the c library version of towupper is probably
	// not very good. The Unicode Utilities compare routine is probably
	// more diligent, though surely not as quick.
    unsigned int  n = 0;
    const XMLCh* cptr1 = comp1;
    const XMLCh* cptr2 = comp2;

    while ( (*cptr1 != 0) && (*cptr2 != 0) && (n < maxChars) )
    {
        wint_t wch1 = towupper(*cptr1);
        wint_t wch2 = towupper(*cptr2);
        if (wch1 != wch2)
            break;
        
        cptr1++;
        cptr2++;
        n++;
    }
    return (int) ( towupper(*cptr1) - towupper(*cptr2) );
#else
	// Use the Unicode Utilities to do the compare
    UCCollateOptions collateOptions = 
                              kUCCollateComposeInsensitiveMask
                            | kUCCollateWidthInsensitiveMask
                            | kUCCollateCaseInsensitiveMask
                            | kUCCollatePunctuationSignificantMask
                            ;
                            
	std::size_t len1 = XMLString::stringLen(comp1);
	std::size_t len2 = XMLString::stringLen(comp2);
	
	if (len1 > maxChars)
		len1 = maxChars;
		
	if (len2 > maxChars)
		len2 = maxChars;
                            
    Boolean equivalent = false;
    SInt32 order = 0;
    OSStatus status = UCCompareTextDefault(
                            collateOptions,	
                            reinterpret_cast<const UniChar* const>(comp1),
                            len1,
                            reinterpret_cast<const UniChar* const>(comp2),
                            len2,
                            &equivalent,
                            &order
                            );
                            
    return ((status != noErr) || equivalent) ? 0 : order;
#endif
}


const XMLCh* MacOSUnicodeConverter::getId() const
{
    return gMyServiceId;
}


bool MacOSUnicodeConverter::isSpace(const XMLCh toCheck) const
{
#if defined(XML_METROWERKS)
	// Use this if there's a reasonable c library available.
	// ProjectBuilder currently has no support for iswspace ;(
    return (std::iswspace(toCheck) != 0);
#elif 0
	// This is okay but probably kind of slow for what we need
    UCCharPropertyValue propValue = 0;
    
    OSStatus status = UCGetCharProperty(
                            reinterpret_cast<const UniChar*>(&toCheck),
                            1,					// size of toCheck in UniChars, right? Not bytes???
                            kUCCharPropTypeGenlCategory,
                            &propValue);
    return (status == noErr) && (propValue == kUCGenlCatSeparatorSpace);
#elif defined(XML_MACOSX) || true
	// This looks fairly good, assuming we're on an ascii compiler.
	// We'll use this under ProjectBuilder for now.
	return (toCheck == L' ');
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
	status = UpgradeScriptInfoToTextEncoding (
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
#if defined(XML_METROWERKS)
	// Use this if there's a reasonable c library available.
	// Metrowerks does this reasonably
	wchar_t * p = (wchar_t*) toUpperCase;
	wchar_t c;
	
	while ((c = *p) != 0)
		*p++ = std::towupper(c);
#elif 1
	// This might work, assuming we're on an ascii compiler.
	// We'll use this under ProjectBuilder for now.
	// Note that this only handles the ascii portion of the
	// string, leaving all other characters in original case.
	wchar_t * p = (wchar_t*) toUpperCase;
	wchar_t c;
	
	while ((c = *p) != 0)
	{
		if (c >= 'a' && c <= 'z')
			c += 'A' - 'a';
		*p++ = c;
	}
#elif 0
	// This looks like it would work, but doesn't.
	// Apart from CFString, there doesn't appear to be any
	// other likely candidates for this support in the OS.
	// Seems like the Unicode converter should have a routine
	// do this!!!
    UppercaseText(reinterpret_cast<Ptr>(toUpperCase), XMLString::stringLen(toUpperCase), smUnicodeScript);
#else
	#error Sorry, no support for upperCase
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
                                        , const unsigned int			blockSize)
{
	XMLTranscoder* result = NULL;
	resValue = XMLTransService::Ok;
	
	TextToUnicodeInfo textToUnicodeInfo = NULL;
	UnicodeToTextInfo unicodeToTextInfo = NULL;

	//	Map the encoding to a Mac OS Encoding value
	TextEncoding textEncoding;
	Str255 pasEncodingName;
	char cEncodingName[256];
	
	// TODO: Use Transcode instead here!
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
		result = new MacOSTranscoder(encodingName, textToUnicodeInfo, unicodeToTextInfo, blockSize);
	
    return result;
}



// ---------------------------------------------------------------------------
//  MacOSTranscoder: Constructors and Destructor
// ---------------------------------------------------------------------------
MacOSTranscoder::MacOSTranscoder(const  XMLCh* const encodingName
								, TextToUnicodeInfo	 textToUnicodeInfo
								, UnicodeToTextInfo	 unicodeToTextInfo
                                , const unsigned int blockSize) :
    XMLTranscoder(encodingName, blockSize),
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
MacOSTranscoder::transcodeFrom(  const   XMLByte* const			srcData
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
		,				// control flags
    	0,				// ioffset count
    	NULL,			// ioffset array
    	0,				// ooffset count
    	NULL,			// ooffset array
    	maxChars * sizeof(UniChar),
    	&bytesConsumed,
    	&bytesProduced,
    	reinterpret_cast<UniCharArrayPtr>(toFill));
    	
    if (status != noErr)
    {
        ThrowXML(TranscodingException, XMLExcepts::Trans_BadSrcSeq);

    	bytesConsumed = 0;
    	bytesProduced = 0;
    }
    	
    bytesEaten = bytesConsumed;
    return bytesProduced / sizeof(UniChar);
}


unsigned int
MacOSTranscoder::transcodeTo(const  XMLCh* const    srcData
                            , const unsigned int    srcCount
                            ,       XMLByte* const  toFill
                            , const unsigned int    maxBytes
                            ,       unsigned int&   charsEaten
                            , const UnRepOpts       options)
{
    ByteCount	bytesConsumed = 0;
    ByteCount	bytesProduced = 0;
    
	OptionBits controlFlags =
		  0
		// | kUnicodeUseFallbacksMask
		| kUnicodeLooseMappingsMask
		// | kUnicodeKeepInfoMask
		// | kUnicodeStringUnterminatedMask
		;
		
	if (options == UnRep_RepChar)
		controlFlags |= kUnicodeUseFallbacksMask;

    OSStatus status = ConvertFromUnicodeToText(
    	mUnicodeToTextInfo,
    	srcCount * sizeof(XMLCh),	// src byte count
    	reinterpret_cast<const UniChar*>(srcData),
    	controlFlags,		// control flags
    	0,					// ioffset count
    	NULL,				// ioffset array
    	0,					// ooffset count
    	NULL,				// ooffset array
    	maxBytes,
    	&bytesConsumed,
    	&bytesProduced,
    	toFill);
    	
    if (status != noErr)
    {
    	if (status == kTECUnmappableElementErr && options == UnRep_Throw)
    	{
    		XMLCh tmpBuf[16];
            XMLString::binToText((unsigned int)&srcData[bytesConsumed/sizeof(XMLCh)], tmpBuf, 16, 16);
            ThrowXML2
            (
                TranscodingException
                , XMLExcepts::Trans_Unrepresentable
                , tmpBuf
                , getEncodingName()
            );
    	}
    	
    	bytesConsumed = 0;
    	bytesProduced = 0;
    }
    	
    charsEaten = bytesConsumed / sizeof(XMLCh);
    return bytesProduced;
}


bool
MacOSTranscoder::canTranscodeTo(const unsigned int toCheck) const
{
    // We'll try just about anything...once!
    //
    // Actually, this is pretty heavilly cheating. If I knew
    // better what I was supposed to do...
    //
    // The windows code essentially tries to convert the character
    // with no surragates being used.
    //
    // TODO: clean this up.
    return true;
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
//	converting twic. It would be nice if the calling code could do some
//	extra buffering to avoid this result.
// ---------------------------------------------------------------------------
unsigned int MacOSLCPTranscoder::calcRequiredSize(const char* const srcText)
{
	if (!srcText)
		return 0;

	const char* src = srcText;
	std::size_t totalBytesProduced = 0;

	OptionBits options =
		  kUnicodeUseFallbacksMask
		// | kUnicodeKeepInfoMask
		| kUnicodeDefaultDirectionMask
		| kUnicodeLooseMappingsMask
		// | kUnicodeStringUnterminatedMask
		// | kUnicodeTextRunMask
		;

	OSStatus status;
	for (status = kTECOutputBufferFullStatus; status == kTECOutputBufferFullStatus; )
	{
	    ByteCount	bytesConsumed = 0;
	    ByteCount	bytesProduced = 0;

	    status = ConvertFromTextToUnicode(
	    	mTextToUnicodeInfo,
	    	std::strlen(src),	// src byte count
	    	src,
	    	options,			// control flags
	    	0,					// ioffset count
	    	NULL,				// ioffset array
	    	0,					// ooffset count
	    	NULL,				// ooffset array
	    	kTempBufSize,
	    	&bytesConsumed,
	    	&bytesProduced,
	    	(UniChar*)mTempBuf);

		options |= kUnicodeKeepInfoMask;
		
		if (status == noErr || status == kTECOutputBufferFullStatus || status == kTECPartialCharErr)
		{
			totalBytesProduced	+= bytesProduced;
			src	+= bytesConsumed;
		}
	}
	
	if (status != noErr && status != kTECPartialCharErr)
		totalBytesProduced = 0;
	
	//	Return number of XMLCh characters required (not counting terminating NULL!)
	return totalBytesProduced / sizeof(XMLCh);
}


// ---------------------------------------------------------------------------
//	In order to implement calcRequiredSize we have to go ahead and do the
//	conversion, which seems quite painful. The Mac Unicode converter has
//	no way of saying "don't actually do the conversion." So we end up
//	converting twic. It would be nice if the calling code could do some
//	extra buffering to avoid this result.
// ---------------------------------------------------------------------------
unsigned int MacOSLCPTranscoder::calcRequiredSize(const XMLCh* const srcText)
{
	if (!srcText)
		return 0;

	const XMLCh* src = const_cast<const XMLCh*>(srcText);
	std::size_t totalBytesProduced = 0;

	OptionBits options =
		  kUnicodeUseFallbacksMask
		| kUnicodeLooseMappingsMask
		// | kUnicodeKeepInfoMask
		// | kUnicodeStringUnterminatedMask
		;

	OSStatus status;
	for (status = kTECOutputBufferFullStatus; status == kTECOutputBufferFullStatus; )
	{
	    ByteCount	bytesConsumed = 0;
	    ByteCount	bytesProduced = 0;

	    status = ConvertFromUnicodeToText(
	    	mUnicodeToTextInfo,
	    	XMLString::stringLen(src) * sizeof(XMLCh),	// src byte count
	    	reinterpret_cast<ConstUniCharArrayPtr>(src),
	    	options,			// control flags
	    	0,					// ioffset count
	    	NULL,				// ioffset array
	    	0,					// ooffset count
	    	NULL,				// ooffset array
	    	kTempBufSize,
	    	&bytesConsumed,
	    	&bytesProduced,
	    	mTempBuf);

		options |= kUnicodeKeepInfoMask;
		
		if (status == noErr || status == kTECOutputBufferFullStatus || status == kTECPartialCharErr)
		{
			totalBytesProduced	+= bytesProduced;
			src	= reinterpret_cast<const XMLCh*>(reinterpret_cast<const char*>(src) + bytesConsumed);
		}
	}
	
	if (status != noErr && status != kTECPartialCharErr)
		totalBytesProduced = 0;
	
	//	Return number of bytes required (not counting terminating NULL!)
	return totalBytesProduced;
}


char* MacOSLCPTranscoder::transcode(const XMLCh* const srcText)
{
	if (!srcText)
		return NULL;

	char* result			= NULL;
	const UniChar* src		= (const UniChar*)srcText;
	std::size_t srcLen		= XMLString::stringLen(srcText) * sizeof(XMLCh);
	std::size_t resultLen	= 0;

	OptionBits options =
		  kUnicodeUseFallbacksMask
		| kUnicodeLooseMappingsMask
		// | kUnicodeKeepInfoMask
		// | kUnicodeStringUnterminatedMask
		;

	OSStatus status;
	for (status = kTECOutputBufferFullStatus; status == kTECOutputBufferFullStatus; )
	{
		//	Convert an (internal) buffer full of text
	    ByteCount	bytesConsumed = 0;
	    ByteCount	bytesProduced = 0;

	    status = ConvertFromUnicodeToText(
	    	mUnicodeToTextInfo,
	    	srcLen,				// src byte count
	    	src,
	    	options,			// control flags
	    	0,					// ioffset count
	    	NULL,				// ioffset array
	    	0,					// ooffset count
	    	NULL,				// ooffset array
	    	kTempBufSize,
	    	&bytesConsumed,
	    	&bytesProduced,
	    	mTempBuf);

		options |= kUnicodeKeepInfoMask;
		
		//	Move the data to result buffer, reallocating as needed
		if (status == noErr || status == kTECOutputBufferFullStatus || status == kTECPartialCharErr)
		{
			//	Allocate space for result
			char* newResult = new char[resultLen + bytesProduced + 1];
			if (newResult != NULL)
			{
				//	Incorporate previous result
				if (result != NULL)
				{
					std::memcpy(newResult, result, resultLen);
					delete [] result;
				}
				result = newResult;

				//	Copy in new data
				std::memcpy(result + resultLen, mTempBuf, bytesProduced);
				resultLen += bytesProduced;
				result[resultLen] = '\0';					
			}
			
			src		= (const UniChar*)((const char*)src + bytesConsumed);
			srcLen -= bytesConsumed;
		}
	}
	
	if (status != noErr && status != kTECPartialCharErr)
	{
		delete [] result;
		result = NULL;
	}
	
	return result;
}


XMLCh* MacOSLCPTranscoder::transcode(const char* const srcText)
{
	if (!srcText)
		return NULL;

	XMLCh* result			= NULL;
	const char* src			= srcText;
	std::size_t resultLen	= 0;
	std::size_t srcLen		= std::strlen(src);

	OptionBits options =
		  kUnicodeUseFallbacksMask
		// | kUnicodeKeepInfoMask
		| kUnicodeDefaultDirectionMask
		| kUnicodeLooseMappingsMask
		// | kUnicodeStringUnterminatedMask
		// | kUnicodeTextRunMask
		;

	OSStatus status;
	for (status = kTECOutputBufferFullStatus; status == kTECOutputBufferFullStatus; )
	{
		//	Convert an (internal) buffer full of text
	    ByteCount	bytesConsumed = 0;
	    ByteCount	bytesProduced = 0;

	    status = ConvertFromTextToUnicode(
	    	mTextToUnicodeInfo,
	    	srcLen,				// src byte count
	    	src,
	    	options,			// control flags
	    	0,					// ioffset count
	    	NULL,				// ioffset array
	    	0,					// ooffset count
	    	NULL,				// ooffset array
	    	kTempBufSize,
	    	&bytesConsumed,
	    	&bytesProduced,
	    	(UniChar*)mTempBuf);

		options |= kUnicodeKeepInfoMask;
		
		//	Move the data to result buffer, reallocating as needed
		if (status == noErr || status == kTECOutputBufferFullStatus || status == kTECPartialCharErr)
		{
			//	Allocate space for result
			XMLCh* newResult = new XMLCh[(resultLen + bytesProduced) / sizeof(XMLCh) + 1];
			if (newResult != NULL)
			{
				//	Incorporate previous result
				if (result != NULL)
				{
					std::memcpy(newResult, result, resultLen);
					delete [] result;
				}
				result = newResult;

				//	Copy in new data
				std::memcpy(result + resultLen, mTempBuf, bytesProduced);
				resultLen += bytesProduced;
				result[resultLen / sizeof(XMLCh)] = 0;					
			}
			
			src		+= bytesConsumed;
			srcLen  -= bytesConsumed;
		}
	}
	
	if (status != noErr && status != kTECPartialCharErr)
	{
		delete [] result;
		result = NULL;
	}
	
	return result;
}


bool MacOSLCPTranscoder::transcode( const   char* const     toTranscode
                                    ,       XMLCh* const    toFill
                                    , const unsigned int    maxChars)
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
    
    //	Zero terminate the output string
    std::size_t charsProduced = bytesProduced / sizeof(XMLCh);
    toFill[charsProduced] = L'\0';

	return (status == noErr);
}


    bool MacOSLCPTranscoder::transcode( const   XMLCh* const    toTranscode
                                    ,       char* const     toFill
                                    , const unsigned int    maxChars)
{
    //	toFill must contain space for maxChars bytes characters + 1 (for terminating NULL).

    // Watch for a few psycho corner cases
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
		| kUnicodeLooseMappingsMask
		// | kUnicodeKeepInfoMask
		// | kUnicodeStringUnterminatedMask
		;

    OSStatus status = ConvertFromUnicodeToText(
    	mUnicodeToTextInfo,
    	XMLString::stringLen(toTranscode) * sizeof(XMLCh),	// src byte count
    	reinterpret_cast<const UniChar*>(toTranscode),
    	options,				// control flags
    	0,						// ioffset count
    	NULL,					// ioffset array
    	0,						// ooffset count
    	NULL,					// ooffset array
    	maxChars,
    	&bytesConsumed,
    	&bytesProduced,
    	toFill);
    	
    //	Zero terminate the output string
    toFill[bytesProduced] = '\0';

	return (status == noErr);
}

