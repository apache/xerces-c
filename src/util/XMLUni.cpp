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
 * Revision 1.10  2000/02/18 22:03:31  roddey
 * One more tweak for EBCDIC encoding aliases.
 *
 * Revision 1.9  2000/02/18 19:55:04  roddey
 * Added Windows-1252 as an intrinsic encoding.
 *
 * Revision 1.8  2000/02/17 00:51:13  roddey
 * Updates for EBCDIC code page issues.
 *
 * Revision 1.7  2000/02/08 00:59:19  roddey
 * Support for new intrinsic encoder for IBM-1047 code page.
 *
 * Revision 1.6  2000/02/06 07:48:06  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.5  2000/01/27 23:32:17  roddey
 * There is now an official URL for the magic "xmlns" prefix. So I updated our code
 * to use this new URL, instead of the proprietary one we used previously.
 *
 * Revision 1.4  2000/01/12 20:44:49  roddey
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
 * Revision 1.1.1.1  1999/11/09 01:05:54  twl
 * Initial checkin
 *
 * Revision 1.5  1999/11/08 20:45:21  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <util/XML4CDefs.hpp>
#include <util/XMLUni.hpp>


// ---------------------------------------------------------------------------
//  XMLUni: Static data
// ---------------------------------------------------------------------------
const XMLCh XMLUni::fgAnyString[] =
{
    chLatin_A, chLatin_N, chLatin_Y, chNull
};

const XMLCh XMLUni::fgAttListString[] =
{
    chLatin_A, chLatin_T, chLatin_T, chLatin_L, chLatin_I, chLatin_S, chLatin_T, chNull
};

const XMLCh XMLUni::fgCommentString[] =
{
    chOpenAngle, chBang, chDash, chDash, chNull
};

const XMLCh XMLUni::fgCDATAString[] =
{
    chLatin_C, chLatin_D, chLatin_A, chLatin_T, chLatin_A, chNull
};

const XMLCh XMLUni::fgDefaultString[] =
{
    chPound, chLatin_D, chLatin_E, chLatin_F, chLatin_A, chLatin_U, chLatin_L, chLatin_T, chNull
};

const XMLCh XMLUni::fgDocTypeString[] =
{
    chOpenAngle, chBang, chLatin_D, chLatin_O, chLatin_C, chLatin_T, chLatin_Y, chLatin_P, chLatin_E, chNull
};

const XMLCh XMLUni::fgEBCDICEncodingString[] =
{
    // Just plain 'EBCDIC'
    chLatin_E, chLatin_B, chLatin_C, chLatin_D, chLatin_I, chLatin_C, chNull
};

const XMLCh XMLUni::fgElemString[] =
{
    chLatin_E, chLatin_L, chLatin_E, chLatin_M, chLatin_E, chLatin_N, chLatin_T, chNull
};

const XMLCh XMLUni::fgEmptyString[] =
{
    chLatin_E, chLatin_M, chLatin_P, chLatin_T, chLatin_Y, chNull
};

const XMLCh XMLUni::fgEncodingString[] =
{
    chLatin_e, chLatin_n, chLatin_c, chLatin_o, chLatin_d, chLatin_i, chLatin_n, chLatin_g, chNull
};

const XMLCh XMLUni::fgEntitString[] =
{
    chLatin_E, chLatin_N, chLatin_T, chLatin_I, chLatin_T, chNull
};

const XMLCh XMLUni::fgEntityString[] =
{
    chLatin_E, chLatin_N, chLatin_T, chLatin_I, chLatin_T, chLatin_Y, chNull
};

const XMLCh XMLUni::fgEntitiesString[] =
{
    chLatin_E, chLatin_N, chLatin_T, chLatin_I, chLatin_T, chLatin_I, chLatin_E, chLatin_S, chNull
};

const XMLCh XMLUni::fgEnumerationString[] =
{
    chLatin_E, chLatin_N, chLatin_U, chLatin_M, chLatin_E, chLatin_R, chLatin_A, chLatin_T, chLatin_I, chLatin_O, chLatin_N, chNull
};

const XMLCh XMLUni::fgExceptDomain[] =
{
        chLatin_h, chLatin_t, chLatin_t, chLatin_p, chColon, chForwardSlash
    ,   chForwardSlash, chLatin_a, chLatin_p, chLatin_a, chLatin_c
    ,   chLatin_h, chLatin_e, chPeriod, chLatin_o, chLatin_r, chLatin_g
    ,   chForwardSlash, chLatin_x, chLatin_m, chLatin_l, chForwardSlash
    ,   chLatin_m, chLatin_e, chLatin_s, chLatin_s, chLatin_a, chLatin_g
    ,   chLatin_e, chLatin_s, chForwardSlash, chLatin_X, chLatin_M, chLatin_L
    ,   chDigit_4, chLatin_C, chLatin_E, chLatin_r, chLatin_r, chLatin_o
    ,   chLatin_r, chLatin_s, chNull
};

const XMLCh XMLUni::fgFixedString[] =
{
    chPound, chLatin_F, chLatin_I, chLatin_X, chLatin_E, chLatin_D, chNull
};

const XMLCh XMLUni::fgGlobalNSURIName[] = 
{
        chLatin_h, chLatin_t, chLatin_t, chLatin_p, chColon, chForwardSlash
    ,   chForwardSlash, chLatin_a, chLatin_p, chLatin_a, chLatin_c
    ,   chLatin_h, chLatin_e, chPeriod, chLatin_o, chLatin_r, chLatin_g
    ,   chForwardSlash, chLatin_x, chLatin_m, chLatin_l, chForwardSlash
    ,   chLatin_G, chLatin_l, chLatin_o, chLatin_b, chLatin_a, chLatin_l
    ,   chLatin_N, chLatin_S, chNull
};


const XMLCh XMLUni::fgIBM037EncodingString[] =
{
    chLatin_E, chLatin_B, chLatin_C, chLatin_D, chLatin_I, chLatin_C, chDash, chLatin_C, chLatin_P, chDash, chLatin_U, chLatin_S, chNull
};

const XMLCh XMLUni::fgIBM037EncodingString2[] =
{
    chLatin_C, chLatin_P, chLatin_I, chLatin_B, chLatin_M, chDigit_3, chDigit_7, chNull
};

const XMLCh XMLUni::fgIBM037EncodingString3[] =
{
    chLatin_I, chLatin_B, chLatin_M, chDigit_0, chDigit_3, chDigit_7, chNull
};

const XMLCh XMLUni::fgIBM037EncodingString4[] =
{
    chLatin_C, chLatin_P, chDigit_3, chDigit_7, chNull
};

const XMLCh XMLUni::fgIBM037EncodingString5[] =
{
    chLatin_C, chLatin_P, chDigit_0, chDigit_3, chDigit_7, chNull
};

const XMLCh XMLUni::fgIBM037EncodingString6[] =
{
        chLatin_C, chLatin_S, chLatin_I, chLatin_B, chLatin_M, chDigit_0
    ,   chDigit_3, chDigit_7, chNull
};

const XMLCh XMLUni::fgIBM1140EncodingString[] =
{
    chLatin_C, chLatin_P, chLatin_I, chLatin_B, chLatin_M, chDigit_1, chDigit_1, chDigit_4, chDigit_0, chNull
};

const XMLCh XMLUni::fgIBM1140EncodingString2[] =
{
    chLatin_I, chLatin_B, chLatin_M, chDigit_1, chDigit_1, chDigit_4, chDigit_0, chNull
};

const XMLCh XMLUni::fgIESString[] =
{
    chLatin_I, chLatin_E, chLatin_S, chNull
};

const XMLCh XMLUni::fgIDString[] =
{
    chLatin_I, chLatin_D, chNull
};

const XMLCh XMLUni::fgIDRefString[] =
{
    chLatin_I, chLatin_D, chLatin_R, chLatin_E, chLatin_F, chNull
};

const XMLCh XMLUni::fgIDRefsString[] =
{
    chLatin_I, chLatin_D, chLatin_R, chLatin_E, chLatin_F, chLatin_S, chNull
};

const XMLCh XMLUni::fgImpliedString[] =
{
    chPound, chLatin_I, chLatin_M, chLatin_P, chLatin_L, chLatin_I, chLatin_E, chLatin_D, chNull
};

const XMLCh XMLUni::fgIgnoreString[] =
{
    chLatin_I, chLatin_G, chLatin_N, chLatin_O, chLatin_R, chLatin_E, chNull
};

const XMLCh XMLUni::fgIncludeString[] =
{
    chLatin_I, chLatin_N, chLatin_C, chLatin_L, chLatin_U, chLatin_D, chLatin_E, chNull
};

const XMLCh XMLUni::fgISO88591EncodingString[] =
{
    chLatin_I, chLatin_S, chLatin_O, chDigit_8, chDigit_8, chDigit_5, chDigit_9, chDash, chDigit_1, chNull
};

const XMLCh XMLUni::fgISO88591EncodingString2[] =
{
    chLatin_I, chLatin_S, chLatin_O, chDash, chDigit_8, chDigit_8, chDigit_5, chDigit_9, chDash, chDigit_1, chNull
};

const XMLCh XMLUni::fgISO88591EncodingString3[] =
{
    chLatin_I, chLatin_S, chLatin_O, chUnderscore, chDigit_8, chDigit_8, chDigit_5, chDigit_9, chDash, chDigit_1, chNull
};

const XMLCh XMLUni::fgISO88591EncodingString4[] =
{
    chLatin_I, chLatin_B, chLatin_M, chDash, chDigit_8, chDigit_1, chDigit_9, chNull
};

const XMLCh XMLUni::fgISO88591EncodingString5[] =
{
    chLatin_I, chLatin_B, chLatin_M, chDigit_8, chDigit_1, chDigit_9, chNull
};

const XMLCh XMLUni::fgISO88591EncodingString6[] =
{
    chLatin_L, chLatin_A, chLatin_T, chLatin_I, chLatin_N, chDigit_1, chNull
};

const XMLCh XMLUni::fgISO88591EncodingString7[] =
{
    chLatin_L, chLatin_A, chLatin_T, chLatin_I, chLatin_N, chDash, chDigit_1, chNull
};

const XMLCh XMLUni::fgISO88591EncodingString8[] =
{
    chLatin_L, chLatin_A, chLatin_T, chLatin_I, chLatin_N, chUnderscore, chDigit_1, chNull
};

const XMLCh XMLUni::fgLocalHostString[] =
{
    chLatin_l, chLatin_o, chLatin_c, chLatin_a, chLatin_l, chLatin_h, chLatin_o, chLatin_s, chLatin_t, chNull
};

const XMLCh XMLUni::fgNoString[] =
{
    chLatin_n, chLatin_o, chNull
};

const XMLCh XMLUni::fgNotationString[] =
{
    chLatin_N, chLatin_O, chLatin_T, chLatin_A, chLatin_T, chLatin_I, chLatin_O, chLatin_N, chNull
};

const XMLCh XMLUni::fgNDATAString[] =
{
    chLatin_N, chLatin_D, chLatin_A, chLatin_T, chLatin_A, chNull
};

const XMLCh XMLUni::fgNmTokenString[] =
{
    chLatin_N, chLatin_M, chLatin_T, chLatin_O, chLatin_K, chLatin_E, chLatin_N, chNull
};

const XMLCh XMLUni::fgNmTokensString[] =
{
    chLatin_N, chLatin_M, chLatin_T, chLatin_O, chLatin_K, chLatin_E, chLatin_N, chLatin_S, chNull
};

const XMLCh XMLUni::fgPCDATAString[] =
{
    chPound, chLatin_P, chLatin_C, chLatin_D, chLatin_A, chLatin_T, chLatin_A, chNull
};

const XMLCh XMLUni::fgPIString[] =
{
    chOpenAngle, chQuestion, chNull
};

const XMLCh XMLUni::fgPubIDString[] =
{
    chLatin_P, chLatin_U, chLatin_B, chLatin_L, chLatin_I, chLatin_C, chNull
};

const XMLCh XMLUni::fgRefString[] =
{
    chLatin_R, chLatin_E, chLatin_F, chNull
};

const XMLCh XMLUni::fgRequiredString[] =
{
    chPound, chLatin_R, chLatin_E, chLatin_Q, chLatin_U, chLatin_I, chLatin_R, chLatin_E, chLatin_D, chNull
};

const XMLCh XMLUni::fgStandaloneString[] =
{
    chLatin_s, chLatin_t, chLatin_a, chLatin_n, chLatin_d, chLatin_a, chLatin_l, chLatin_o, chLatin_n, chLatin_e, chNull
};

const XMLCh XMLUni::fgSupportedVersion[] =
{
    chDigit_1, chPeriod, chDigit_0, chNull
};

const XMLCh XMLUni::fgSysIDString[] =
{
    chLatin_S, chLatin_Y, chLatin_S, chLatin_T, chLatin_E, chLatin_M, chNull
};

const XMLCh XMLUni::fgUnknownURIName[] = 
{
        chLatin_h, chLatin_t, chLatin_t, chLatin_p, chColon, chForwardSlash
    ,   chForwardSlash, chLatin_a, chLatin_p, chLatin_a, chLatin_c
    ,   chLatin_h, chLatin_e, chPeriod, chLatin_o, chLatin_r, chLatin_g
    ,   chForwardSlash, chLatin_x, chLatin_m, chLatin_l, chForwardSlash
    ,   chLatin_U, chLatin_k, chLatin_n, chLatin_o, chLatin_w, chLatin_n
    ,   chLatin_N, chLatin_S, chNull
};

const XMLCh XMLUni::fgUCS4EncodingString[] =
{
    chLatin_U, chLatin_C, chLatin_S, chDigit_4, chNull
};

const XMLCh XMLUni::fgUCS4EncodingString2[] =
{
    chLatin_U, chLatin_C, chLatin_S, chDash, chDigit_4, chNull
};

const XMLCh XMLUni::fgUCS4EncodingString3[] =
{
    chLatin_U, chLatin_C, chLatin_S, chUnderscore, chDigit_4, chNull
};

const XMLCh XMLUni::fgUCS4BEncodingString[] =
{
    chLatin_U, chLatin_C, chLatin_S, chDash, chDigit_4, chSpace, chOpenParen, chLatin_B, chLatin_E, chCloseParen, chNull
};

const XMLCh XMLUni::fgUCS4BEncodingString2[] =
{
    chLatin_U, chLatin_C, chLatin_S, chDash, chDigit_4, chLatin_B, chLatin_E, chNull
};

const XMLCh XMLUni::fgUCS4LEncodingString[] =
{
    chLatin_U, chLatin_C, chLatin_S, chDash, chDigit_4, chSpace, chOpenParen, chLatin_L, chLatin_E, chCloseParen, chNull
};

const XMLCh XMLUni::fgUCS4LEncodingString2[] =
{
    chLatin_U, chLatin_C, chLatin_S, chDash, chDigit_4, chLatin_L, chLatin_E, chNull
};

const XMLCh XMLUni::fgUSASCIIEncodingString[] =
{
    chLatin_U, chLatin_S, chDash, chLatin_A, chLatin_S, chLatin_C, chLatin_I, chLatin_I, chNull
};

const XMLCh XMLUni::fgUSASCIIEncodingString2[] =
{
    chLatin_U, chLatin_S, chLatin_A, chLatin_S, chLatin_C, chLatin_I, chLatin_I, chNull
};

const XMLCh XMLUni::fgUSASCIIEncodingString3[] =
{
    chLatin_A, chLatin_S, chLatin_C, chLatin_I, chLatin_I, chNull
};

const XMLCh XMLUni::fgUSASCIIEncodingString4[] =
{
    chLatin_U, chLatin_S, chUnderscore, chLatin_A, chLatin_S, chLatin_C, chLatin_I, chLatin_I, chNull
};

const XMLCh XMLUni::fgUTF8EncodingString[] =
{
    chLatin_U, chLatin_T, chLatin_F, chDash, chDigit_8, chNull
};

const XMLCh XMLUni::fgUTF8EncodingString2[] =
{
    chLatin_U, chLatin_T, chLatin_F, chDigit_8, chNull
};

const XMLCh XMLUni::fgUTF16EncodingString[] =
{
    chLatin_U, chLatin_T, chLatin_F, chDash, chDigit_1, chDigit_6, chNull
};

const XMLCh XMLUni::fgUTF16EncodingString2[] =
{
    chLatin_U, chLatin_C, chLatin_S, chDigit_2, chNull
};

const XMLCh XMLUni::fgUTF16EncodingString3[] =
{
    chLatin_I, chLatin_B, chLatin_M, chDigit_1, chDigit_2, chDigit_0, chDigit_0, chNull
};

const XMLCh XMLUni::fgUTF16EncodingString4[] =
{
    chLatin_I, chLatin_B, chLatin_M, chDash, chDigit_1, chDigit_2, chDigit_0, chDigit_0, chNull
};

const XMLCh XMLUni::fgUTF16BEncodingString[] =
{
    chLatin_U, chLatin_T, chLatin_F, chDash, chDigit_1, chDigit_6, chSpace, chOpenParen, chLatin_B, chLatin_E, chCloseParen, chNull
};

const XMLCh XMLUni::fgUTF16BEncodingString2[] =
{
    chLatin_U, chLatin_T, chLatin_F, chDash, chDigit_1, chDigit_6, chLatin_B, chLatin_E, chNull
};

const XMLCh XMLUni::fgUTF16LEncodingString[] =
{
    chLatin_U, chLatin_T, chLatin_F, chDash, chDigit_1, chDigit_6, chSpace, chOpenParen, chLatin_L, chLatin_E, chCloseParen, chNull
};

const XMLCh XMLUni::fgUTF16LEncodingString2[] =
{
    chLatin_U, chLatin_T, chLatin_F, chDash, chDigit_1, chDigit_6, chLatin_L, chLatin_E, chNull
};

const XMLCh XMLUni::fgVersionString[] =
{
    chLatin_v, chLatin_e, chLatin_r, chLatin_s, chLatin_i, chLatin_o, chLatin_n, chNull
};

const XMLCh XMLUni::fgValidityDomain[] =
{
        chLatin_h, chLatin_t, chLatin_t, chLatin_p, chColon, chForwardSlash
    ,   chForwardSlash, chLatin_a, chLatin_p, chLatin_a, chLatin_c, chLatin_h
    ,   chLatin_e, chPeriod, chLatin_o, chLatin_r, chLatin_g, chForwardSlash
    ,   chLatin_x, chLatin_m, chLatin_l, chForwardSlash, chLatin_m, chLatin_e
    ,   chLatin_s, chLatin_s, chLatin_a, chLatin_g, chLatin_e, chLatin_s
    ,   chForwardSlash, chLatin_X, chLatin_M, chLatin_L, chLatin_V, chLatin_a
    ,   chLatin_l, chLatin_i, chLatin_d, chLatin_i, chLatin_t, chLatin_y, chNull
};

const XMLCh XMLUni::fgWin1252EncodingString[] =
{
        chLatin_W, chLatin_I, chLatin_N, chLatin_D, chLatin_O, chLatin_W
    ,   chLatin_S, chDash, chDigit_1, chDigit_2, chDigit_5, chDigit_2, chNull
};

const XMLCh XMLUni::fgXMLString[] =
{
    chLatin_x, chLatin_m, chLatin_l, chNull
};

const XMLCh XMLUni::fgXMLStringSpace[] =
{
    chLatin_x, chLatin_m, chLatin_l, chSpace, chNull
};

const XMLCh XMLUni::fgXMLStringSpaceU[] =
{
    chLatin_X, chLatin_M, chLatin_L, chSpace, chNull
};

const XMLCh XMLUni::fgXMLDeclString[] =
{
    chOpenAngle, chQuestion, chLatin_x, chLatin_m, chLatin_l, chNull
};

const XMLCh XMLUni::fgXMLDeclStringSpace[] =
{
    chOpenAngle, chQuestion, chLatin_x, chLatin_m, chLatin_l, chSpace, chNull
};

const XMLCh XMLUni::fgXMLNSString[] =
{
    chLatin_x, chLatin_m, chLatin_l, chLatin_n, chLatin_s, chNull
};

const XMLCh XMLUni::fgXMLNSColonString[] =
{
    chLatin_x, chLatin_m, chLatin_l, chLatin_n, chLatin_s, chColon, chNull
};

const XMLCh XMLUni::fgXMLNSURIName[] =
{
        chLatin_h, chLatin_t, chLatin_t, chLatin_p, chColon, chForwardSlash
    ,   chForwardSlash, chLatin_w, chLatin_w, chLatin_w, chPeriod, chLatin_w
    ,   chDigit_3, chPeriod, chLatin_o, chLatin_r, chLatin_g, chForwardSlash
    ,   chDigit_2, chDigit_0, chDigit_0, chDigit_0, chForwardSlash
    ,   chLatin_x, chLatin_m, chLatin_l, chLatin_n, chLatin_s, chForwardSlash
    ,   chNull
};

const XMLCh XMLUni::fgXMLErrDomain[] =
{
        chLatin_h, chLatin_t, chLatin_t, chLatin_p, chColon, chForwardSlash
    ,   chForwardSlash, chLatin_a, chLatin_p, chLatin_a, chLatin_c, chLatin_h
    ,   chLatin_e, chPeriod, chLatin_o, chLatin_r, chLatin_g, chForwardSlash
    ,   chLatin_x, chLatin_m, chLatin_l, chForwardSlash, chLatin_m, chLatin_e
    ,   chLatin_s, chLatin_s, chLatin_a, chLatin_g, chLatin_e, chLatin_s
    ,   chForwardSlash, chLatin_X, chLatin_M, chLatin_L, chLatin_E, chLatin_r
    ,   chLatin_r, chLatin_o, chLatin_r, chLatin_s, chNull
};

const XMLCh XMLUni::fgXMLURIName[] =
{
        chLatin_h, chLatin_t, chLatin_t, chLatin_p, chColon, chForwardSlash
    ,   chForwardSlash, chLatin_w, chLatin_w, chLatin_w, chPeriod, chLatin_w
    ,   chDigit_3, chPeriod, chLatin_o, chLatin_r, chLatin_g, chForwardSlash
    ,   chLatin_X, chLatin_M, chLatin_L, chForwardSlash, chDigit_1, chDigit_9
    ,   chDigit_9, chDigit_8, chForwardSlash, chLatin_n, chLatin_a, chLatin_m
    ,   chLatin_e, chLatin_s, chLatin_p, chLatin_a, chLatin_c, chLatin_e, chNull
};

const XMLCh XMLUni::fgYesString[] =
{
    chLatin_y, chLatin_e, chLatin_s, chNull
};

const XMLCh XMLUni::fgZeroLenString[] = { chNull };
