/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 1999-2001 The Apache Software Foundation.  All rights
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
 * $Id$
 */


// ---------------------------------------------------------------------------
//  This file contains the grunt work constants for Unicode characters and
//  common Unicode constant strings. These cannot be created normally because
//  we have to compile on systems that cannot do the L"" style prefix. So
//  they must be created as constant values for Unicode code points and the
//  strings built up as arrays of those constants.
// ---------------------------------------------------------------------------

#if !defined(XMLUNI_HPP)
#define XMLUNI_HPP

#include <util/XercesDefs.hpp>


class XMLUTIL_EXPORT XMLUni
{
public :
    // -----------------------------------------------------------------------
    //  These are constant strings that are common in XML data. Because
    //  of the limitation of the compilers we have to work with, these are
    //  done as arrays of XMLCh characters, not as constant strings.
    // -----------------------------------------------------------------------
    static const XMLCh fgAnyString[];
    static const XMLCh fgAttListString[];
    static const XMLCh fgCommentString[];
    static const XMLCh fgCDATAString[];
    static const XMLCh fgDefaultString[];
    static const XMLCh fgDocTypeString[];
    static const XMLCh fgEBCDICEncodingString[];
    static const XMLCh fgElemString[];
    static const XMLCh fgEmptyString[];
    static const XMLCh fgEncodingString[];
    static const XMLCh fgEntitString[];
    static const XMLCh fgEntityString[];
    static const XMLCh fgEntitiesString[];
    static const XMLCh fgEnumerationString[];
    static const XMLCh fgExceptDomain[];
    static const XMLCh fgFixedString[];
    static const XMLCh fgIBM037EncodingString[];
    static const XMLCh fgIBM037EncodingString2[];
    static const XMLCh fgIBM1140EncodingString[];
    static const XMLCh fgIESString[];
    static const XMLCh fgIDString[];
    static const XMLCh fgIDRefString[];
    static const XMLCh fgIDRefsString[];
    static const XMLCh fgImpliedString[];
    static const XMLCh fgIgnoreString[];
    static const XMLCh fgIncludeString[];
    static const XMLCh fgISO88591EncodingString[];
    static const XMLCh fgISO88591EncodingString2[];
    static const XMLCh fgISO88591EncodingString3[];
    static const XMLCh fgISO88591EncodingString4[];
    static const XMLCh fgISO88591EncodingString5[];
    static const XMLCh fgISO88591EncodingString6[];
    static const XMLCh fgISO88591EncodingString7[];
    static const XMLCh fgISO88591EncodingString8[];
    static const XMLCh fgLocalHostString[];
    static const XMLCh fgNoString[];
    static const XMLCh fgNotationString[];
    static const XMLCh fgNDATAString[];
    static const XMLCh fgNmTokenString[];
    static const XMLCh fgNmTokensString[];
    static const XMLCh fgPCDATAString[];
    static const XMLCh fgPIString[];
    static const XMLCh fgPubIDString[];
    static const XMLCh fgRefString[];
    static const XMLCh fgRequiredString[];
    static const XMLCh fgStandaloneString[];
    static const XMLCh fgSupportedVersion[];
    static const XMLCh fgSysIDString[];
    static const XMLCh fgUnknownURIName[];
    static const XMLCh fgUCS4EncodingString[];
    static const XMLCh fgUCS4EncodingString2[];
    static const XMLCh fgUCS4EncodingString3[];
    static const XMLCh fgUCS4BEncodingString[];
    static const XMLCh fgUCS4BEncodingString2[];
    static const XMLCh fgUCS4LEncodingString[];
    static const XMLCh fgUCS4LEncodingString2[];
    static const XMLCh fgUSASCIIEncodingString[];
    static const XMLCh fgUSASCIIEncodingString2[];
    static const XMLCh fgUSASCIIEncodingString3[];
    static const XMLCh fgUSASCIIEncodingString4[];
    static const XMLCh fgUTF8EncodingString[];
    static const XMLCh fgUTF8EncodingString2[];
    static const XMLCh fgUTF16EncodingString[];
    static const XMLCh fgUTF16EncodingString2[];
    static const XMLCh fgUTF16EncodingString3[];
    static const XMLCh fgUTF16EncodingString4[];
    static const XMLCh fgUTF16BEncodingString[];
    static const XMLCh fgUTF16BEncodingString2[];
    static const XMLCh fgUTF16LEncodingString[];
    static const XMLCh fgUTF16LEncodingString2[];
    static const XMLCh fgVersionString[];
    static const XMLCh fgValidityDomain[];
    static const XMLCh fgWin1252EncodingString[];
    static const XMLCh fgXMLChEncodingString[];
    static const XMLCh fgXMLString[];
    static const XMLCh fgXMLStringSpace[];
    static const XMLCh fgXMLStringHTab[];
    static const XMLCh fgXMLStringCR[];
    static const XMLCh fgXMLStringLF[];
    static const XMLCh fgXMLStringSpaceU[];
    static const XMLCh fgXMLStringHTabU[];
    static const XMLCh fgXMLStringCRU[];
    static const XMLCh fgXMLStringLFU[];
    static const XMLCh fgXMLDeclStringSpace[];
    static const XMLCh fgXMLDeclStringHTab[];
    static const XMLCh fgXMLDeclStringLF[];
    static const XMLCh fgXMLDeclStringCR[];
    static const XMLCh fgXMLDeclStringSpaceU[];
    static const XMLCh fgXMLDeclStringHTabU[];
    static const XMLCh fgXMLDeclStringLFU[];
    static const XMLCh fgXMLDeclStringCRU[];

    static const XMLCh fgXMLNSString[];
    static const XMLCh fgXMLNSColonString[];
    static const XMLCh fgXMLNSURIName[];
    static const XMLCh fgXMLErrDomain[];
    static const XMLCh fgXMLURIName[];
    static const XMLCh fgYesString[];
    static const XMLCh fgZeroLenString[];



	static const XMLCh fgArrayIndexOutOfBoundsException_Name[];
	static const XMLCh fgEmptyStackException_Name[];
	static const XMLCh fgIllegalArgumentException_Name[];
	static const XMLCh fgInvalidCastException_Name[];
	static const XMLCh fgIOException_Name[];
	static const XMLCh fgNoSuchElementException_Name[];
	static const XMLCh fgNullPointerException_Name[];
	static const XMLCh fgXMLPlatformUtilsException_Name[];
	static const XMLCh fgRuntimeException_Name[];
	static const XMLCh fgTranscodingException_Name[];
	static const XMLCh fgUnexpectedEOFException_Name[];
	static const XMLCh fgUnsupportedEncodingException_Name[];
	static const XMLCh fgUTFDataFormatException_Name[];
	static const XMLCh fgNetAccessorException_Name[];
	static const XMLCh fgMalformedURLException_Name[];
	static const XMLCh fgNumberFormatException_Name[];
	static const XMLCh fgParseException_Name[];
	static const XMLCh fgInvalidDatatypeFacetException_Name[];
	static const XMLCh fgInvalidDatatypeValueException_Name[];
};

#endif
