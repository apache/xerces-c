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
 * Revision 1.7  2000/02/09 23:06:03  abagchi
 * Removed fgOtherEncodingStr
 *
 * Revision 1.6  2000/02/08 00:59:19  roddey
 * Support for new intrinsic encoder for IBM-1047 code page.
 *
 * Revision 1.5  2000/02/06 07:48:06  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.4  2000/01/12 20:44:50  roddey
 * Ooops, I checked in the URL.cpp file with a L"localhost" style string in it. This is not legal for
 * some of our compilers, so a new string was added XMLUni and used in its place.
 *
 * Revision 1.3  1999/12/18 00:18:10  roddey
 * More changes to support the new, completely orthagonal support for
 * intrinsic encodings.
 *
 * Revision 1.2  1999/12/15 19:41:29  roddey
 * Support for the new transcoder system, where even intrinsic encodings are
 * done via the same transcoder abstraction as external ones.
 *
 * Revision 1.1.1.1  1999/11/09 01:05:56  twl
 * Initial checkin
 *
 * Revision 1.4  1999/11/08 20:45:21  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
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

#include <util/XML4CDefs.hpp>

// ---------------------------------------------------------------------------
//  Constants for the Unicode characters of interest to us in an XML parser
//  We don't put these inside the class because then they could not be const
//  inline values, which would have significant performance ramifications.
//
//  We cannot use a namespace because of the requirement to support old
//  compilers.
// ---------------------------------------------------------------------------
const XMLCh chNull                  = 0x00;
const XMLCh chHTab                  = 0x09;
const XMLCh chLF                    = 0x0A;
const XMLCh chCR                    = 0x0D;
const XMLCh chAmpersand             = 0x26;
const XMLCh chAsterisk              = 0x2A;
const XMLCh chAt                    = 0x40;
const XMLCh chBackSlash             = 0x5C;
const XMLCh chBang                  = 0x21;
const XMLCh chCloseAngle            = 0x3E;
const XMLCh chCloseCurly            = 0x7D;
const XMLCh chCloseParen            = 0x29;
const XMLCh chCloseSquare           = 0x5D;
const XMLCh chColon                 = 0x3A;
const XMLCh chComma                 = 0x2C;
const XMLCh chDash                  = 0x2D;
const XMLCh chDollarSign            = 0x24;
const XMLCh chDoubleQuote           = 0x22;
const XMLCh chEqual                 = 0x3D;
const XMLCh chForwardSlash          = 0x2F;
const XMLCh chGrave                 = 0x60;
const XMLCh chOpenAngle             = 0x3C;
const XMLCh chOpenCurly             = 0x7B;
const XMLCh chOpenParen             = 0x28;
const XMLCh chOpenSquare            = 0x5B;
const XMLCh chPercent               = 0x25;
const XMLCh chPeriod                = 0x2E;
const XMLCh chPipe                  = 0x7C;
const XMLCh chPlus                  = 0x2B;
const XMLCh chPound                 = 0x23;
const XMLCh chQuestion              = 0x3F;
const XMLCh chSingleQuote           = 0x27;
const XMLCh chSpace                 = 0x20;
const XMLCh chSemiColon             = 0x3B;
const XMLCh chTilde                 = 0x7E;
const XMLCh chUnderscore            = 0x5F;

const XMLCh chSwappedUnicodeMarker  = XMLCh(0xFFFE);
const XMLCh chUnicodeMarker         = XMLCh(0xFEFF);

const XMLCh chDigit_0               = 0x30;
const XMLCh chDigit_1               = 0x31;
const XMLCh chDigit_2               = 0x32;
const XMLCh chDigit_3               = 0x33;
const XMLCh chDigit_4               = 0x34;
const XMLCh chDigit_5               = 0x35;
const XMLCh chDigit_6               = 0x36;
const XMLCh chDigit_7               = 0x37;
const XMLCh chDigit_8               = 0x38;
const XMLCh chDigit_9               = 0x39;

const XMLCh chLatin_A               = 0x41;
const XMLCh chLatin_B               = 0x42;
const XMLCh chLatin_C               = 0x43;
const XMLCh chLatin_D               = 0x44;
const XMLCh chLatin_E               = 0x45;
const XMLCh chLatin_F               = 0x46;
const XMLCh chLatin_G               = 0x47;
const XMLCh chLatin_H               = 0x48;
const XMLCh chLatin_I               = 0x49;
const XMLCh chLatin_J               = 0x4A;
const XMLCh chLatin_K               = 0x4B;
const XMLCh chLatin_L               = 0x4C;
const XMLCh chLatin_M               = 0x4D;
const XMLCh chLatin_N               = 0x4E;
const XMLCh chLatin_O               = 0x4F;
const XMLCh chLatin_P               = 0x50;
const XMLCh chLatin_Q               = 0x51;
const XMLCh chLatin_R               = 0x52;
const XMLCh chLatin_S               = 0x53;
const XMLCh chLatin_T               = 0x54;
const XMLCh chLatin_U               = 0x55;
const XMLCh chLatin_V               = 0x56;
const XMLCh chLatin_W               = 0x57;
const XMLCh chLatin_X               = 0x58;
const XMLCh chLatin_Y               = 0x59;
const XMLCh chLatin_Z               = 0x5A;

const XMLCh chLatin_a               = 0x61;
const XMLCh chLatin_b               = 0x62;
const XMLCh chLatin_c               = 0x63;
const XMLCh chLatin_d               = 0x64;
const XMLCh chLatin_e               = 0x65;
const XMLCh chLatin_f               = 0x66;
const XMLCh chLatin_g               = 0x67;
const XMLCh chLatin_h               = 0x68;
const XMLCh chLatin_i               = 0x69;
const XMLCh chLatin_j               = 0x6A;
const XMLCh chLatin_k               = 0x6B;
const XMLCh chLatin_l               = 0x6C;
const XMLCh chLatin_m               = 0x6D;
const XMLCh chLatin_n               = 0x6E;
const XMLCh chLatin_o               = 0x6F;
const XMLCh chLatin_p               = 0x70;
const XMLCh chLatin_q               = 0x71;
const XMLCh chLatin_r               = 0x72;
const XMLCh chLatin_s               = 0x73;
const XMLCh chLatin_t               = 0x74;
const XMLCh chLatin_u               = 0x75;
const XMLCh chLatin_v               = 0x76;
const XMLCh chLatin_w               = 0x77;
const XMLCh chLatin_x               = 0x78;
const XMLCh chLatin_y               = 0x79;
const XMLCh chLatin_z               = 0x7A;


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
    static const XMLCh fgDefaultEBCDICEncodingString[];
    static const XMLCh fgEBCDICEncodingString[];
    static const XMLCh fgEBCDICEncodingString2[];
    static const XMLCh fgElemString[];
    static const XMLCh fgEmptyString[];
    static const XMLCh fgEncodingString[];
    static const XMLCh fgEntitString[];
    static const XMLCh fgEntityString[];
    static const XMLCh fgEntitiesString[];
    static const XMLCh fgEnumerationString[];
    static const XMLCh fgExceptDomain[];
    static const XMLCh fgFixedString[];
    static const XMLCh fgGlobalNSURIName[];
    static const XMLCh fgIBM037EncodingString[];
    static const XMLCh fgIBM1047EncodingString[];
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
    static const XMLCh fgXMLDeclString[];
    static const XMLCh fgXMLString[];
    static const XMLCh fgXMLStringSpace[];
    static const XMLCh fgXMLStringSpaceU[];
    static const XMLCh fgXMLDeclStringSpace[];
    static const XMLCh fgXMLNSString[];
    static const XMLCh fgXMLNSColonString[];
    static const XMLCh fgXMLNSURIName[];
    static const XMLCh fgXMLErrDomain[];
    static const XMLCh fgXMLURIName[];
    static const XMLCh fgYesString[];
    static const XMLCh fgZeroLenString[];
};

#endif
