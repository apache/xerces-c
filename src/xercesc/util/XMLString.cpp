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
//  Includes
// ---------------------------------------------------------------------------
#include <string.h>
#include <ctype.h>
#include <xercesc/util/ArrayIndexOutOfBoundsException.hpp>
#include <xercesc/util/IllegalArgumentException.hpp>
#include <xercesc/util/NumberFormatException.hpp>
#include <xercesc/util/Janitor.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/RuntimeException.hpp>
#include <xercesc/util/TransService.hpp>
#include <xercesc/util/TranscodingException.hpp>
#include <xercesc/util/ValueArrayOf.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/XMLUniDefs.hpp>
#include <xercesc/util/XMLUni.hpp>
#include <xercesc/internal/XMLReader.hpp>


// ---------------------------------------------------------------------------
//  Local static data
//
//  gConverter
//      This is initialized when the user calls the platform init method,
//      which calls our init method. This is the converter used for default
//      conversion to/from the local code page.
// ---------------------------------------------------------------------------
static XMLLCPTranscoder*    gTranscoder = 0;
static XMLCh                gNullStr[] =
{
    chOpenCurly, chLatin_n, chLatin_u, chLatin_l, chLatin_l, chCloseCurly, chNull
};


// ---------------------------------------------------------------------------
//  XMLString: Public static methods
// ---------------------------------------------------------------------------
void XMLString::binToText(  const   unsigned long   toFormat
                            ,       char* const     toFill
                            , const unsigned int    maxChars
                            , const unsigned int    radix)
{
    static const char digitList[16] =
    {
          '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'
        , 'A', 'B', 'C', 'D', 'E', 'F'
    };

    if (!maxChars)
        ThrowXML(IllegalArgumentException, XMLExcepts::Str_ZeroSizedTargetBuf);

    // Handle special case
    if (!toFormat)
    {
        toFill[0] = '0';
        toFill[1] = 0;
        return;
    }

    // This is used to fill the temp buffer
    unsigned int tmpIndex = 0;

    // A copy of the conversion value that we can modify
    unsigned int tmpVal = toFormat;

    //
    //  Convert into a temp buffer that we know is large enough. This avoids
    //  having to check for overflow in the inner loops, and we have to flip
    //  the resulting XMLString anyway.
    //
    char tmpBuf[128];

    //
    //  For each radix, do the optimal thing. For bin and hex, we can special
    //  case them and do shift and mask oriented stuff. For oct and decimal
    //  there isn't much to do but bull through it with divides.
    //
    if (radix == 2)
    {
        while (tmpVal)
        {
            if (tmpVal & 0x1UL)
                tmpBuf[tmpIndex++] = '1';
            else
                tmpBuf[tmpIndex++] = '0';
            tmpVal >>= 1;
        }
    }
     else if (radix == 16)
    {
        while (tmpVal)
        {
            const unsigned int charInd = (tmpVal & 0xFUL);
            tmpBuf[tmpIndex++] = digitList[charInd];
            tmpVal >>= 4;
        }
    }
     else if ((radix == 8) || (radix == 10))
    {
        while (tmpVal)
        {
            const unsigned int charInd = (tmpVal % radix);
            tmpBuf[tmpIndex++] = digitList[charInd];
            tmpVal /= radix;
        }
    }
     else
    {
        ThrowXML(RuntimeException, XMLExcepts::Str_UnknownRadix);
    }

    // See if have enough room in the caller's buffer
    if (tmpIndex > maxChars)
        ThrowXML(IllegalArgumentException, XMLExcepts::Str_TargetBufTooSmall);

    // Reverse the tmp buffer into the caller's buffer
    unsigned int outIndex = 0;
    for (; tmpIndex > 0; tmpIndex--)
        toFill[outIndex++] = tmpBuf[tmpIndex-1];

    // And cap off the caller's buffer
    toFill[outIndex] = char(0);
}

void XMLString::binToText(  const   unsigned int    toFormat
                            ,       char* const     toFill
                            , const unsigned int    maxChars
                            , const unsigned int    radix)
{
    // Just call the unsigned long version
    binToText((unsigned long)toFormat, toFill, maxChars, radix);
}

void XMLString::binToText(  const   long            toFormat
                            ,       char* const     toFill
                            , const unsigned int    maxChars
                            , const unsigned int    radix)
{
    //
    //  If its negative, then put a negative sign into the output and flip
    //  the sign of the local temp value.
    //
    unsigned int startInd = 0;
    unsigned long actualVal;
    if (toFormat < 0)
    {
        toFill[0] = '-';
        startInd++;
        actualVal = (unsigned long)(toFormat * -1);
    }
     else
    {
        actualVal = (unsigned long)(toFormat);
    }

    // And now call the unsigned long version
    binToText(actualVal, &toFill[startInd], maxChars, radix);
}

void XMLString::binToText(  const   int             toFormat
                            ,       char* const     toFill
                            , const unsigned int    maxChars
                            , const unsigned int    radix)
{
    //
    //  If its negative, then put a negative sign into the output and flip
    //  the sign of the local temp value.
    //
    unsigned int startInd = 0;
    unsigned long actualVal;
    if (toFormat < 0)
    {
        toFill[0] = '-';
        startInd++;
        actualVal = (unsigned long)(toFormat * -1);
    }
     else
    {
        actualVal = (unsigned long)(toFormat);
    }

    // And now call the unsigned long version
    binToText(actualVal, &toFill[startInd], maxChars, radix);
}


void XMLString::catString(char* const target, const char* const src)
{
    strcat(target, src);
}


int XMLString::compareIString(const char* const str1, const char* const str2)
{
    return stricmp(str1, str2);
}


int XMLString::compareNString(  const   char* const     str1
                                , const char* const     str2
                                , const unsigned int    count)
{
    // Watch for pathological secenario
    if (!count)
        return 0;

    return strncmp(str1, str2, count);
}


int XMLString::compareNIString( const   char* const     str1
                                , const char* const     str2
                                , const unsigned int    count)
{
    if (!count)
        return 0;

    return strnicmp(str1, str2, count);
}


int XMLString::compareString(   const   char* const    str1
                                , const char* const    str2)
{
    return strcmp(str1, str2);
}


void XMLString::copyString(         char* const    target
                            , const char* const    src)
{
    strcpy(target, src);
}


void XMLString::cut(        XMLCh* const    toCutFrom
                    , const unsigned int    count)
{
    #if defined(XML_DEBUG)
    if (count > stringLen(toCutFrom))
    {
        // <TBD> This is bad of course
    }
    #endif

    // If count is zero, then nothing to do
    if (!count)
        return;

    XMLCh* targetPtr = toCutFrom;
    XMLCh* srcPtr = toCutFrom + count;
    while (*srcPtr)
        *targetPtr++ = *srcPtr++;

    // Cap it off at the new end
    *targetPtr = 0;
}


unsigned int XMLString::hash(   const   char* const     tohash
                                , const unsigned int    hashModulus)
{
    if (!hashModulus)
        ThrowXML(IllegalArgumentException, XMLExcepts::Pool_ZeroModulus);

    unsigned int hashVal = 0;
    if (tohash) {
        const char* curCh = tohash;
        while (*curCh)
        {
            unsigned int top = hashVal >> 24;
            hashVal += (hashVal * 37) + top + (unsigned int)(*curCh);
            curCh++;
        }
    }

    // Divide by modulus
    return hashVal % hashModulus;
}


int XMLString::indexOf(const char* const toSearch, const char ch)
{
    const unsigned int len = strlen(toSearch);
    for (unsigned int i = 0; i < len; i++)
    {
        if (toSearch[i] == ch)
            return i;
    }
    return -1;
}


int XMLString::indexOf( const   char* const     toSearch
                        , const char            ch
                        , const unsigned int    fromIndex)
{
    const unsigned int len = strlen(toSearch);

    // Make sure the start index is within the XMLString bounds
	if ((int)fromIndex > len-1)
        ThrowXML(ArrayIndexOutOfBoundsException, XMLExcepts::Str_StartIndexPastEnd);

    for (unsigned int i = fromIndex; i < len; i++)
    {
        if (toSearch[i] == ch)
            return i;
    }
    return -1;
}

int XMLString::lastIndexOf(const char* const toSearch, const char ch)
{
    const int len = strlen(toSearch);
    for (int i = len-1; i >= 0; i--)
    {
        if (toSearch[i] == ch)
            return i;
    }
    return -1;
}

int XMLString::lastIndexOf( const   char* const     toSearch
                            , const char            ch
                            , const unsigned int    fromIndex)
{
    const int len = strlen(toSearch);

    // Make sure the start index is within the XMLString bounds
	if ((int)fromIndex > len-1)
        ThrowXML(ArrayIndexOutOfBoundsException, XMLExcepts::Str_StartIndexPastEnd);

    for (int i = (int)fromIndex; i >= 0; i--)
    {
        if (toSearch[i] == ch)
            return i;
    }
    return -1;
}


unsigned int XMLString::replaceTokens(          XMLCh* const    errText
                                        , const unsigned int    maxChars
                                        , const XMLCh* const    text1
                                        , const XMLCh* const    text2
                                        , const XMLCh* const    text3
                                        , const XMLCh* const    text4)
{
    //
    //  We have to build the string back into the source string, so allocate
    //  a temp string and copy the orignal text to it. We'll then treat the
    //  incoming buffer as a target buffer. Put a janitor on it to make sure
    //  it gets cleaned up.
    //
    XMLCh* orgText = replicate(errText);
    ArrayJanitor<XMLCh> janText(orgText);

    XMLCh* pszSrc = orgText;
    unsigned int curOutInd = 0;

    while (*pszSrc && (curOutInd < maxChars))
    {
        //
        //  Loop until we see a { character. Until we do, just copy chars
        //  from src to target, being sure not to overrun the output buffer.
        //
        while ((*pszSrc != chOpenCurly) && (curOutInd < maxChars))
        {
            if (!*pszSrc)
                break;
            errText[curOutInd++] = *pszSrc++;
        }

        // If we did not find a curly, then we are done
        if (*pszSrc != chOpenCurly)
            break;

        //
        //  Probe this one to see if it matches our pattern of {x}. If not
        //  then copy over those chars and go back to the first loop.
        //
        if ((*(pszSrc+1) >= chDigit_0)
        &&  (*(pszSrc+1) <= chDigit_3)
        &&  (*(pszSrc+2) == chCloseCurly))
        {
            //
            //  Its one of our guys, so move the source pointer up past the
            //  token we are replacing. First though get out the token number
            //  character.
            //
            XMLCh tokCh = *(pszSrc+1);
            pszSrc += 3;

            // Now copy over the replacement text
            const XMLCh* repText = 0;
            if (tokCh == chDigit_0)
                repText = text1;
            else if (tokCh == chDigit_1)
                repText = text2;
            else if (tokCh == chDigit_2)
                repText = text3;
            else if (tokCh == chDigit_3)
                repText = text4;

            // If this one is null, copy over a null string
            if (!repText)
                repText = gNullStr;

            while (*repText && (curOutInd < maxChars))
                errText[curOutInd++] = *repText++;
        }
         else
        {
            // Escape the curly brace character and continue
            errText[curOutInd++] = *pszSrc++;
        }
    }

    // Copy over a null terminator
    errText[curOutInd] = 0;

    // And return the count of chars we output
    return curOutInd;
}


char* XMLString::replicate(const char* const toRep)
{
    // If a null string, return a null string
    if (!toRep)
        return 0;

    //
    //  Get the len of the source and allocate a new buffer. Make sure to
    //  account for the nul terminator.
    //
    const unsigned int srcLen = strlen(toRep);
    char* ret = new char[srcLen+1];

    // Copy over the text, adjusting for the size of a char
    memcpy(ret, toRep, (srcLen+1) * sizeof(char));
    return ret;
}


bool XMLString::startsWith(const char* const toTest, const char* const prefix)
{
    return (strncmp(toTest, prefix, strlen(prefix)) == 0);
}


bool XMLString::startsWithI(const   char* const toTest
                            , const char* const prefix)
{
    return (strnicmp(toTest, prefix, strlen(prefix)) == 0);
}


unsigned int XMLString::stringLen(const char* const src)
{
    return strlen(src);
}


char* XMLString::transcode(const XMLCh* const toTranscode)
{
    return gTranscoder->transcode(toTranscode);
}

bool XMLString::transcode(  const   XMLCh* const    toTranscode
                            ,       char* const     toFill
                            , const unsigned int    maxChars)
{
    if (!gTranscoder->transcode(toTranscode, toFill, maxChars))
        return false;
    return true;
}

XMLCh* XMLString::transcode(const char* const toTranscode)
{
    return gTranscoder->transcode(toTranscode);
}

bool XMLString::transcode(  const   char* const     toTranscode
                            ,       XMLCh* const    toFill
                            , const unsigned int    maxChars)
{
    if (!gTranscoder->transcode(toTranscode, toFill, maxChars))
        return false;
    return true;
}


void XMLString::trim(char* const toTrim)
{
    const unsigned int len = strlen(toTrim);

    unsigned int skip, scrape;
    for (skip = 0; skip < len; skip++)
    {
        if (! isspace(toTrim[skip]))
            break;
    }

    for (scrape = len; scrape > skip; scrape--)
    {
        if (! isspace(toTrim[scrape - 1] ))
            break;
    }

    // Cap off at the scrap point
    if (scrape != len)
        toTrim[scrape] = 0;

    if (skip)
    {
        // Copy the chars down
        unsigned int index = 0;
        while (toTrim[skip])
            toTrim[index++] = toTrim[skip++];

        toTrim[index] = 0;
    }
}


void XMLString::subString(char* const targetStr, const char* const srcStr
                          , const int startIndex, const int endIndex)
{
	//if (startIndex < 0 || endIndex < 0)
    //    ThrowXML(ArrayIndexOutOfBoundsException, XMLExcepts::Str_NegativeIndex);

	if (targetStr == 0)
        ThrowXML(IllegalArgumentException, XMLExcepts::Str_ZeroSizedTargetBuf);

    const int srcLen = strlen(srcStr);
	const int copySize = endIndex - startIndex;

    // Make sure the start index is within the XMLString bounds
	if (startIndex > srcLen-1 || endIndex > srcLen )
        ThrowXML(ArrayIndexOutOfBoundsException, XMLExcepts::Str_StartIndexPastEnd);

	for (int i= startIndex; i < endIndex; i++) {

		targetStr[i-startIndex] = srcStr[i];
	}

	targetStr[copySize] = 0;
}

/**
  * isValidNCName
  *
  *    NCName::= (Letter | '_') (NCNameChar)*
  *    NCNameChar ::= Letter | Digit | '.' | '-' | '_'
  *                   | CombiningChar | Extender
  */
bool XMLString::isValidNCName(const XMLCh* const name) {

    if (XMLString::stringLen(name) == 0
        || XMLString::indexOf(name, chColon) != -1) {
        return false;
    }

    const XMLCh* tempName = name;
    XMLCh firstChar = *tempName++;

    if (!XMLReader::isXMLLetter(firstChar) && firstChar != chUnderscore) {

        return false;
    }

    while(*tempName) {

        if (*tempName == chColon || !XMLReader::isNameChar(*tempName++)) {
            return false;
        }
    }

    return true;
}

/**
  * isValidName
  *
  *    Name::= (Letter | '_' | ':') (NameChar)*
  *    NameChar ::= Letter | Digit | '.' | '-' | '_' | ':'
  *                 | CombiningChar | Extender
  */
bool XMLString::isValidName(const XMLCh* const name) {

    if (!name ||
        (XMLString::stringLen(name) == 0))
        return false;

    const XMLCh* tempName = name;
    XMLCh firstChar = *tempName++;

    if (!XMLReader::isXMLLetter(firstChar) &&
        (firstChar != chUnderscore)        &&
        (firstChar != chColon)              )
        return false;

    while(*tempName)
        if (!XMLReader::isNameChar(*tempName++))
            return false;

    return true;
}

/**
  * isValidEncName
  *
  * [80] EncName ::= [A-Za-z] ([A-Za-z0-9._] | '-')*
  *
  */
bool XMLString::isValidEncName(const XMLCh* const name)
{

    if ( ( !name) ||
         ( XMLString::stringLen(name) == 0 ))
        return false;

    const XMLCh* tempName = name;
    XMLCh firstChar = *tempName++;

    if (!isAlpha(firstChar))
        return false;

    while(*tempName)
    {
        if (( !isAlpha(*tempName))       &&
            ( !isDigit(*tempName))       &&
            ( *tempName != chPeriod)     &&
            ( *tempName != chUnderscore) &&
            ( *tempName != chDash)        )
            return false;

        tempName++;
    }

    return true;
}

/**
  * isValidQName
  *
  * [6]  QName ::=  (Prefix ':')? LocalPart
  * [7]  Prefix ::=  NCName
  * [8]  LocalPart ::=  NCName
  *
  */
bool XMLString::isValidQName(const XMLCh* const name)
{
    if (!name)
        return false;

    int strLen = XMLString::stringLen(name);
    if (strLen == 0)
        return false;

    int colonPos = XMLString::indexOf(name, chColon);
    if ((colonPos == 0) ||         // ":abcd"
        (colonPos == strLen-1))    // "abcd:"
        return false;

    //
    // prefix
    //
    if (colonPos != -1)
    {
        XMLCh *prefix = new XMLCh[colonPos+1];
        XMLString::subString(prefix, name, 0, colonPos);
        ArrayJanitor<XMLCh> janName(prefix);
        if (XMLString::isValidNCName(prefix)==false)
            return false;
    }

    //
    // LocalPart
    //
    return XMLString::isValidNCName(name+colonPos+1);
}

bool XMLString::isAlpha(XMLCh const theChar)
{
    if ((( theChar >= chLatin_a ) && ( theChar <= chLatin_z )) ||
        (( theChar >= chLatin_A ) && ( theChar <= chLatin_Z )) )
        return true;

    return false;
}

bool XMLString::isDigit(XMLCh const theChar)
{
    if (( theChar >= chDigit_0 ) && ( theChar <= chDigit_9 ))
        return true;

    return false;
}

bool XMLString::isAlphaNum(XMLCh const theChar)
{
    return (isAlpha(theChar) || isDigit(theChar));
}

bool XMLString::isHex(XMLCh const theChar)
{
	return (isDigit(theChar) ||
			(theChar >= chLatin_a && theChar <= chLatin_f) ||
			(theChar >= chLatin_A && theChar <= chLatin_F));
}

bool XMLString::isAllWhiteSpace(const XMLCh* const toCheck)
{
    if ( !toCheck )
        return true;

    const XMLCh* startPtr = toCheck;

    while (*startPtr)
    {
        if (!XMLPlatformUtils::fgTransService->isSpace(*startPtr))
            return false;

        startPtr++;
    }

    return true;
}

// ---------------------------------------------------------------------------
//  Wide char versions of most of the string methods
// ---------------------------------------------------------------------------
void XMLString::binToText(  const   unsigned long   toFormat
                            ,       XMLCh* const    toFill
                            , const unsigned int    maxChars
                            , const unsigned int    radix)
{
    static const XMLCh digitList[16] =
    {
            chDigit_0, chDigit_1, chDigit_2, chDigit_3, chDigit_4, chDigit_5
        ,   chDigit_6, chDigit_7, chDigit_8, chDigit_9, chLatin_A, chLatin_B
        ,   chLatin_C, chLatin_D, chLatin_e, chLatin_F
    };

    if (!maxChars)
        ThrowXML(IllegalArgumentException, XMLExcepts::Str_ZeroSizedTargetBuf);

    // Handle special case
    if (!toFormat)
    {
        toFill[0] = chDigit_0;
        toFill[1] = chNull;
        return;
    }

    // This is used to fill the temp buffer
    unsigned int tmpIndex = 0;

    // A copy of the conversion value that we can modify
    unsigned int tmpVal = toFormat;

    //
    //  Convert into a temp buffer that we know is large enough. This avoids
    //  having to check for overflow in the inner loops, and we have to flip
    //  the resulting sring anyway.
    //
    XMLCh tmpBuf[128];

    //
    //  For each radix, do the optimal thing. For bin and hex, we can special
    //  case them and do shift and mask oriented stuff. For oct and decimal
    //  there isn't much to do but bull through it with divides.
    //
    if (radix == 2)
    {
        while (tmpVal)
        {
            if (tmpVal & 0x1UL)
                tmpBuf[tmpIndex++] = chDigit_1;
            else
                tmpBuf[tmpIndex++] = chDigit_0;
            tmpVal >>= 1;
        }
    }
     else if (radix == 16)
    {
        while (tmpVal)
        {
            const unsigned int charInd = (tmpVal & 0xFUL);
            tmpBuf[tmpIndex++] = digitList[charInd];
            tmpVal >>= 4;
        }
    }
     else if ((radix == 8) || (radix == 10))
    {
        while (tmpVal)
        {
            const unsigned int charInd = (tmpVal % radix);
            tmpBuf[tmpIndex++] = digitList[charInd];
            tmpVal /= radix;
        }
    }
     else
    {
        ThrowXML(RuntimeException, XMLExcepts::Str_UnknownRadix);
    }

    // See if have enough room in the caller's buffer
    if (tmpIndex > maxChars)
        ThrowXML(IllegalArgumentException, XMLExcepts::Str_TargetBufTooSmall);

    // Reverse the tmp buffer into the caller's buffer
    unsigned int outIndex = 0;
    for (; tmpIndex > 0; tmpIndex--)
        toFill[outIndex++] = tmpBuf[tmpIndex-1];

    // And cap off the caller's buffer
    toFill[outIndex] = chNull;
}

void XMLString::binToText(  const   unsigned int    toFormat
                            ,       XMLCh* const    toFill
                            , const unsigned int    maxChars
                            , const unsigned int    radix)
{
    // Just call the unsigned long version
    binToText((unsigned long)toFormat, toFill, maxChars, radix);
}

void XMLString::binToText(  const   long            toFormat
                            ,       XMLCh* const    toFill
                            , const unsigned int    maxChars
                            , const unsigned int    radix)
{
    //
    //  If its negative, then put a negative sign into the output and flip
    //  the sign of the local temp value.
    //
    unsigned int startInd = 0;
    unsigned long actualVal;
    if (toFormat < 0)
    {
        toFill[0] = chDash;
        startInd++;
        actualVal = (unsigned long)(toFormat * -1);
    }
     else
    {
        actualVal = (unsigned long)(toFormat);
    }

    // And now call the unsigned long version
    binToText(actualVal, &toFill[startInd], maxChars, radix);
}

void XMLString::binToText(  const   int             toFormat
                            ,       XMLCh* const    toFill
                            , const unsigned int    maxChars
                            , const unsigned int    radix)
{
    //
    //  If its negative, then put a negative sign into the output and flip
    //  the sign of the local temp value.
    //
    unsigned int startInd = 0;
    unsigned long actualVal;
    if (toFormat < 0)
    {
        toFill[0] = chDash;
        startInd++;
        actualVal = (unsigned long)(toFormat * -1);
    }
     else
    {
        actualVal = (unsigned long)(toFormat);
    }

    // And now call the unsigned long version
    binToText(actualVal, &toFill[startInd], maxChars, radix);
}


void XMLString::catString(XMLCh* const target, const XMLCh* const src)
{
    // Get the starting point for the cat on the target XMLString
    unsigned int index = stringLen(target);

    // While the source is not zero, add them to target and bump
    const XMLCh* pszTmp = src;
    while (*pszTmp)
        target[index++] = *pszTmp++;

    // Cap off the target where we ended
    target[index] = chNull;
}


int XMLString::compareIString(  const   XMLCh* const    str1
                                , const XMLCh* const    str2)
{
    // Refer this one to the transcoding service
    return XMLPlatformUtils::fgTransService->compareIString(str1, str2);
}


int XMLString::compareNString(  const   XMLCh* const    str1
                                , const XMLCh* const    str2
                                , const unsigned int    maxChars)
{
    const XMLCh* psz1 = str1;
    const XMLCh* psz2 = str2;

    unsigned int curCount = 0;
    while (true)
    {
        // If an inequality, then return difference
        if (*psz1 != *psz2)
            return int(*psz1) - int(*psz2);

        // If either ended, then both ended, so equal
        if (!*psz1 || !*psz2)
            break;

        // Move upwards to next chars
        psz1++;
        psz2++;

        //
        //  Bump the count of chars done. If it equals the max then we are
        //  equal for the requested count, so break out and return equal.
        //
        curCount++;
        if (curCount == maxChars)
            break;
    }
    return 0;
}


int XMLString::compareNIString( const   XMLCh* const    str1
                                , const XMLCh* const    str2
                                , const unsigned int    maxChars)
{
    // Refer this oneto the transcoding service
    return XMLPlatformUtils::fgTransService->compareNIString(str1, str2, maxChars);
}


int XMLString::compareString(   const   XMLCh* const    str1
                                , const XMLCh* const    str2)
{
    const XMLCh* psz1 = str1;
    const XMLCh* psz2 = str2;

    if (psz1 == 0 || psz2 == 0) {

        if (psz1 == 0) {
            return 0 - XMLString::stringLen(psz2);
        }
		else if (psz2 == 0) {
            return XMLString::stringLen(psz1);
        }
    }

    while (true)
    {
        // If an inequality, then return the difference
        if (*psz1 != *psz2)
            return int(*psz1) - int(*psz2);

        // If either has ended, then they both ended, so equal
        if (!*psz1 || !*psz2)
            break;

        // Move upwards for the next round
        psz1++;
        psz2++;
    }
    return 0;
}


bool XMLString::regionMatches(const   XMLCh* const	str1
							  , const int			offset1
							  , const XMLCh* const	str2
							  , const int			offset2
							  , const unsigned int	charCount)
{
	if (!validateRegion(str1, offset1,str2, offset2, charCount))
		return false;

	if (compareNString(str1+offset1, str2+offset2, charCount) != 0)
		return false;

	return true;
}

bool XMLString::regionIMatches(const   XMLCh* const	str1
						 	   , const int			offset1
							   , const XMLCh* const	str2
							   , const int			offset2
							   , const unsigned int	charCount)
{
	if (!validateRegion(str1, offset1,str2, offset2, charCount))
		return false;

	if (compareNIString(str1+offset1, str2+offset2, charCount) != 0)
		return false;

	return true;
}

void XMLString::copyString(XMLCh* const target, const XMLCh* const src)
{
    if (!src)
    {
        *target = 0;
        return;
    }

    XMLCh* pszOut = target;
    const XMLCh* pszIn = src;
    while (*pszIn)
        *pszOut++ = *pszIn++;

    // Capp off the target where we ended
    *pszOut = 0;
}


bool XMLString::copyNString(        XMLCh* const    target
                            , const XMLCh* const    src
                            , const unsigned int    maxChars)
{
    XMLCh* outPtr = target;
    const XMLCh* srcPtr = src;
    const XMLCh* endPtr = target + maxChars - 1;

    while (*srcPtr && (outPtr <= endPtr))
        *outPtr++ = *srcPtr++;

    // Cap it off here
    *outPtr = 0;

    // Return whether we copied it all or hit the max
    return (*srcPtr == 0);
}


unsigned int XMLString::hash(   const   XMLCh* const    tohash
                                , const unsigned int    hashModulus)
{
    if (!hashModulus)
        ThrowXML(IllegalArgumentException, XMLExcepts::Pool_ZeroModulus);

    unsigned int hashVal = 0;
    if (tohash) {
        const XMLCh* curCh = tohash;
        while (*curCh)
        {
            unsigned int top = hashVal >> 24;
            hashVal += (hashVal * 37) + top + (unsigned int)(*curCh);
            curCh++;
        }
    }

    // Divide by modulus
    return hashVal % hashModulus;
}


const XMLCh* XMLString::findAny(const   XMLCh* const    toSearch
                                , const XMLCh* const    searchList)
{
    const XMLCh* srcPtr = toSearch;
    while (*srcPtr)
    {
        const XMLCh* listPtr = searchList;
        const XMLCh  curCh = *srcPtr;

        while (*listPtr)
        {
            if (curCh == *listPtr++)
                return srcPtr;
        }
        srcPtr++;
    }
    return 0;
}

XMLCh* XMLString::findAny(          XMLCh* const    toSearch
                            , const XMLCh* const    searchList)
{
    XMLCh* srcPtr = toSearch;
    while (*srcPtr)
    {
        const XMLCh* listPtr = searchList;
        const XMLCh  curCh = *srcPtr;

        while (*listPtr)
        {
            if (curCh == *listPtr++)
                return srcPtr;
        }
        srcPtr++;
    }
    return 0;
}

int XMLString::patternMatch(        XMLCh* const    toSearch
                            , const XMLCh* const    pattern)
{
    if (!toSearch || !pattern )
        return -1;

    XMLCh* srcPtr = toSearch;
    const int patnLen = XMLString::stringLen(pattern);
    int  patnIndex = 0;

    while (*srcPtr)
    {
        if (*srcPtr++ != pattern[patnIndex++])
        {
            patnIndex = 0;
        }
        else
        {
            if (patnIndex == patnLen)
                // full pattern match found
                return (srcPtr - patnLen - toSearch);
        }
    }

    return -1;
}

unsigned int XMLString::hashN(  const   XMLCh* const    tohash
                                , const unsigned int    n
                                , const unsigned int    hashModulus)
{
    if (!hashModulus)
        ThrowXML(IllegalArgumentException, XMLExcepts::Pool_ZeroModulus);

    unsigned int hashVal = 0;
    if (tohash) {
        const XMLCh* curCh = tohash;
        int i = n;
        while (i--)
        {
            unsigned int top = hashVal >> 24;
            hashVal += (hashVal * 37) + top + (unsigned int)(*curCh);
            curCh++;
        }
    }

    // Divide by modulus
    return hashVal % hashModulus;
}


int XMLString::indexOf(const XMLCh* const toSearch, const XMLCh ch)
{
    const unsigned int len = stringLen(toSearch);
    for (unsigned int i = 0; i < len; i++)
    {
        if (toSearch[i] == ch)
            return i;
    }
    return -1;
}


int XMLString::indexOf( const   XMLCh* const    toSearch
                        , const XMLCh           ch
                        , const unsigned int    fromIndex)
{
    const int len = stringLen(toSearch);

    // Make sure the start index is within the XMLString bounds
	if ((int)fromIndex > len-1)
        ThrowXML(ArrayIndexOutOfBoundsException, XMLExcepts::Str_StartIndexPastEnd);

    for (int i = (int)fromIndex; i < len; i++)
    {
        if (toSearch[i] == ch)
            return i;
    }
    return -1;
}


int XMLString::lastIndexOf(const XMLCh* const toSearch, const XMLCh ch)
{
    const int len = stringLen(toSearch);
    for (int i = len-1; i >= 0; i--)
    {
        if (toSearch[i] == ch)
            return i;
    }
    return -1;
}

int XMLString::lastIndexOf( const   XMLCh* const    toSearch
                            , const XMLCh           ch
                            , const unsigned int    fromIndex)
{
    const int len = stringLen(toSearch);
	if ((int)fromIndex > len-1)
        ThrowXML(ArrayIndexOutOfBoundsException, XMLExcepts::Str_StartIndexPastEnd);

    for (int i = (int)fromIndex; i >= 0; i--)
    {
        if (toSearch[i] == ch)
            return i;
    }
    return -1;
}


XMLCh*
XMLString::makeUName(const XMLCh* const pszURI, const XMLCh* const pszName)
{
    //
    //  If there is a URI, then format out the full name in the {uri}name
    //  form. Otherwise, just set it to the same thing as the base name.
    //
    XMLCh* pszRet = 0;
    const unsigned int uriLen = stringLen(pszURI);
    if (uriLen)
    {
        pszRet = new XMLCh[uriLen + stringLen(pszName) + 3];

        XMLCh szTmp[2];
        szTmp[1] = 0;

        szTmp[0] = chOpenCurly;
        copyString(pszRet, szTmp);
        catString(pszRet, pszURI);
        szTmp[0] = chCloseCurly;
        catString(pszRet, szTmp);
        catString(pszRet, pszName);
    }
     else
    {
        pszRet = replicate(pszName);
    }
    return pszRet;
}


bool XMLString::textToBin(const XMLCh* const toConvert, unsigned int& toFill)
{
    toFill = 0;

    // If no string, then its a failure
    if (!toConvert)
        return false;
    if (!*toConvert)
        return false;

    // Scan past any whitespace. If we hit the end, then return failure
    const XMLCh* startPtr = toConvert;
    while (XMLPlatformUtils::fgTransService->isSpace(*startPtr))
        startPtr++;
    if (!*startPtr)
        return false;

    // Start at the end and work back through any whitespace
    const XMLCh* endPtr = toConvert + stringLen(toConvert);
    while (XMLPlatformUtils::fgTransService->isSpace(*(endPtr - 1)))
        endPtr--;

    //
    //  Work through what remains and convert each char to a digit. Any
    //  space or non-digit here is now an error.
    //
    unsigned long tmpVal = 0;
    while (startPtr < endPtr)
    {
        // If not valid decimal digit, then an error
        if ((*startPtr < chDigit_0) || (*startPtr > chDigit_9))
            return false;

        const unsigned int nextVal = (unsigned int)(*startPtr - chDigit_0);
        tmpVal = (tmpVal * 10) + nextVal;

        startPtr++;
    }

    // Make sure it didn't overflow
    if (tmpVal > ~0UL)
        ThrowXML(RuntimeException, XMLExcepts::Str_ConvertOverflow);

    toFill = (unsigned int)tmpVal;
    return true;
}

int XMLString::parseInt(const XMLCh* const toConvert)
{
    // If no string, then its a failure
    if ((!toConvert) ||
        (!*toConvert))
        ThrowXML(NumberFormatException, XMLExcepts::XMLNUM_null_ptr);

    // Scan past any whitespace. If we hit the end, then return failure
    const XMLCh* startPtr = toConvert;
    while (XMLPlatformUtils::fgTransService->isSpace(*startPtr))
        startPtr++;

    if (!*startPtr)
        ThrowXML(NumberFormatException, XMLExcepts::CM_UnaryOpHadBinType);
        //ThrowXML(NumberFormatException, XMLExcepts::XMLINT_Invalid);

    // Start at the end and work back through any whitespace
    const XMLCh* endPtr = toConvert + XMLString::stringLen(toConvert);
    while (XMLPlatformUtils::fgTransService->isSpace(*(endPtr - 1)))
        endPtr--;

    //
    //  Work through what remains and convert each char to a digit.
    //
    int signValue = 1;

    //
    // '+' or '-' is allowed only at the first position
    //
    if (*startPtr == chDash)
    {
        signValue = -1;
        startPtr++;  // skip the '-'
    }
    else if (*startPtr == chPlus)
        startPtr++;  // skip the '+'

    unsigned long tmpVal = 0;
    while (startPtr < endPtr)
    {
        // If not valid decimal digit, then an error
        if ((*startPtr < chDigit_0) || (*startPtr > chDigit_9))
            ThrowXML(NumberFormatException, XMLExcepts::XMLNUM_Inv_chars);

        const unsigned int nextVal = (unsigned int)(*startPtr - chDigit_0);
        tmpVal = (tmpVal * 10) + nextVal;

        startPtr++;
    }

    // Make sure it didn't overflow
    if (tmpVal > ~0UL)
        ThrowXML(NumberFormatException, XMLExcepts::Str_ConvertOverflow);

    return (int) signValue*tmpVal;
}

void XMLString::trim(XMLCh* const toTrim)
{
    const unsigned int len = stringLen(toTrim);

    unsigned int skip, scrape;
    for (skip = 0; skip < len; skip++)
    {
        if (!XMLPlatformUtils::fgTransService->isSpace(toTrim[skip]))
            break;
    }

    for (scrape = len; scrape > skip; scrape--)
    {
        if (!XMLPlatformUtils::fgTransService->isSpace(toTrim[scrape - 1]))
            break;
    }

    // Cap off at the scrap point
    if (scrape != len)
        toTrim[scrape] = 0;

    if (skip)
    {
        // Copy the chars down
        unsigned int index = 0;
        while (toTrim[skip])
            toTrim[index++] = toTrim[skip++];

        toTrim[index] = 0;
    }
}


void XMLString::upperCase(XMLCh* const toUpperCase)
{
    // Refer this one to the transcoding service
    XMLPlatformUtils::fgTransService->upperCase(toUpperCase);
}


void XMLString::lowerCase(XMLCh* const toLowerCase)
{
    // Refer this one to the transcoding service
    //XMLPlatformUtils::fgTransService->lowerCase(toLowerCase);
}


void XMLString::subString(XMLCh* const targetStr, const XMLCh* const srcStr
                          , const int startIndex, const int endIndex)
{
	//if (startIndex < 0 || endIndex < 0)
    //    ThrowXML(ArrayIndexOutOfBoundsException, XMLExcepts::Str_NegativeIndex);

	if (targetStr == 0)
        ThrowXML(IllegalArgumentException, XMLExcepts::Str_ZeroSizedTargetBuf);

    const int srcLen = stringLen(srcStr);
	const int copySize = endIndex - startIndex;

    // Make sure the start index is within the XMLString bounds
	if (startIndex > srcLen-1 || endIndex > srcLen )
        ThrowXML(ArrayIndexOutOfBoundsException, XMLExcepts::Str_StartIndexPastEnd);

	for (int i= startIndex; i < endIndex; i++) {

		targetStr[i-startIndex] = srcStr[i];
	}

	targetStr[copySize] = 0;
}

RefVectorOf<XMLCh>* XMLString::tokenizeString(const XMLCh* const tokenizeSrc)
{
    XMLCh* orgText = replicate(tokenizeSrc);
    ArrayJanitor<XMLCh> janText(orgText);
    XMLCh* tokenizeStr = orgText;

    RefVectorOf<XMLCh>* tokenStack = new RefVectorOf<XMLCh>(16, true);

    unsigned int len = stringLen(tokenizeStr);
    unsigned int skip;
    unsigned int index = 0;

    while (index != len) {
        // find the first non-space character
        for (skip = index; skip < len; skip++)
        {
            if (!XMLPlatformUtils::fgTransService->isSpace(tokenizeStr[skip]))
                break;
        }
        index = skip;

        // find the delimiter (space character)
        for (; skip < len; skip++)
        {
            if (XMLPlatformUtils::fgTransService->isSpace(tokenizeStr[skip]))
                break;
        }

        // we reached the end of the string
        if (skip == index)
            break;

        // these tokens are adopted in the RefVector and will be deleted
        // when the vector is deleted by the caller
        XMLCh* token = new XMLCh[skip+1-index];

        XMLString::subString(token, tokenizeStr, index, skip);
        tokenStack->addElement(token);
        index = skip;
    }
    return tokenStack;
}

//
//  This method is called when we get a notation or enumeration type attribute
//  to validate. We have to confirm that the passed value to find is one of
//  the values in the passed list. The list is a space separated string of
//  values to match against.
//
bool XMLString::isInList(const XMLCh* const toFind, const XMLCh* const enumList)
{
    //
    //  We loop through the values in the list via this outer loop. We end
    //  when we hit the end of the enum list or get a match.
    //
    const XMLCh* listPtr = enumList;
    const unsigned int findLen = XMLString::stringLen(toFind);
    while (*listPtr)
    {
        unsigned int testInd;
        for (testInd = 0; testInd < findLen; testInd++)
        {
            //
            //  If they don't match, then reset and try again. Note that
            //  hitting the end of the current item will cause a mismatch
            //  because there can be no spaces in the toFind string.
            //
            if (listPtr[testInd] != toFind[testInd])
                break;
        }

        //
        //  If we went the distance, see if we matched. If we did, the current
        //  list character has to be null or space.
        //
        if (testInd == findLen)
        {
            if ((listPtr[testInd] == chSpace) || !listPtr[testInd])
                return true;
        }

        // Run the list pointer up to the next substring
        while ((*listPtr != chSpace) && *listPtr)
            listPtr++;

        // If we hit the end, then we failed
        if (!*listPtr)
            return false;

        // Else move past the space and try again
        listPtr++;
    }

    // We never found it
    return false;
}

//
// a string is whitespace:replaced, is having no
//    #xD  Carriage Return
//    #xA  Line Feed
//    #x9  TAB
//
bool XMLString::isWSReplaced(const XMLCh* const toCheck)
{
    // If no string, then its a OK
    if (( !toCheck ) || ( !*toCheck ))
        return true;

    const XMLCh* startPtr = toCheck;
    while ( *startPtr )
    {
        if ( ( *startPtr == chCR) ||
             ( *startPtr == chLF) ||
             ( *startPtr == chHTab))
        return false;

        startPtr++;
    }

    return true;
}

//
//   to replace characters listed below to #x20
//    #xD  Carriage Return
//    #xA  Line Feed
//    #x9  TAB
//
void XMLString::replaceWS(XMLCh* const toConvert)
{
    // If no string, then its a failure
    if (( !toConvert ) || ( !*toConvert ))
        return;

    int strLen = XMLString::stringLen(toConvert);
    XMLCh* retBuf = new XMLCh[strLen+1];
    XMLCh* retPtr = &(retBuf[0]);
    XMLCh* startPtr = toConvert;

    while ( *startPtr )
    {
        if ( ( *startPtr == chCR) ||
             ( *startPtr == chLF) ||
             ( *startPtr == chHTab))
            *retPtr = chSpace;
        else
            *retPtr = *startPtr;

        retPtr++;
        startPtr++;
    }

    retBuf[strLen] = chNull;

    XMLString::moveChars(toConvert, retBuf, strLen);
    delete[] retBuf;
    return;
}

//
// a string is whitespace:collapsed, is whitespace::replaced
// and no
//    leading space (#x20)
//    trailing space
//    no contiguous sequences of spaces
//
bool XMLString::isWSCollapsed(const XMLCh* const toCheck)
{
    if (( !toCheck ) || ( !*toCheck ))
        return true;

    // shall be whitespace::replaced first
    if ( !isWSReplaced(toCheck) )
        return false;

    // no leading or trailing space
    if ((*toCheck == chSpace) ||
        (toCheck[XMLString::stringLen(toCheck)-1] == chSpace))
        return false;

    const XMLCh* startPtr = toCheck;
    XMLCh theChar;
    bool  inSpace = false;
    while ( (theChar = *startPtr) != 0 )
    {
        if ( theChar == chSpace)
        {
            if (inSpace)
                return false;
            else
                inSpace = true;
        }
        else
            inSpace = false;

        startPtr++;

    }

    return true;
}

//
// no leading and/or trailing spaces
// no continuous sequences of spaces
//
void XMLString::collapseWS(XMLCh* const toConvert)
{
    // If no string, then its a failure
    if (( !toConvert ) || ( !*toConvert ))
        return;

    // replace whitespace first
    replaceWS(toConvert);

    // remove leading spaces
    const XMLCh* startPtr = toConvert;
    while ( *startPtr == chSpace )
        startPtr++;

    if (!*startPtr)
        return;

    // remove trailing spaces
    const XMLCh* endPtr = toConvert + stringLen(toConvert);
    while (*(endPtr - 1) == chSpace)
        endPtr--;

    //
    //  Work through what remains and chop continuous spaces
    //
    XMLCh* retBuf = new XMLCh[endPtr - startPtr + 1];
    XMLCh* retPtr = &(retBuf[0]);
    bool  inSpace = false;
    while (startPtr < endPtr)
    {
        if ( *startPtr == chSpace)
        {
            if (inSpace)
            {
                //discard it;
            }
            else
            {
                inSpace = true;
                *retPtr = chSpace;  //copy the first chSpace
                retPtr++;
            }
        }
        else
        {
            inSpace = false;
            *retPtr = *startPtr;
            retPtr++;
        }

        startPtr++;
    }

    *retPtr = chNull;
    XMLString::moveChars(toConvert, retBuf, stringLen(retBuf)+1); //copy the last chNull as well
    delete[] retBuf;
    return;
}

// ---------------------------------------------------------------------------
//  XMLString: Private static methods
// ---------------------------------------------------------------------------
void XMLString::initString(XMLLCPTranscoder* const defToUse)
{
    // Store away the default transcoder that we are to use
    gTranscoder = defToUse;
}

void XMLString::termString()
{
    // Just clean up our local code page transcoder
    delete gTranscoder;
    gTranscoder = 0;
}
