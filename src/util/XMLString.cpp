/*
 * The Apache Software License, Version 1.1
 * 
 * Copyright (c) 1999 The Apache Software Foundation.  All rights 
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
 * Revision 1.1  1999/11/09 01:05:51  twl
 * Initial revision
 *
 * Revision 1.2  1999/11/08 20:45:20  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <memory.h>
#include <string.h>
#include <util/ArrayIndexOutOfBoundsException.hpp>
#include <util/IllegalArgumentException.hpp>
#include <util/Janitor.hpp>
#include <util/PlatformUtils.hpp>
#include <util/StdOut.hpp>
#include <util/RuntimeException.hpp>
#include <util/TransService.hpp>
#include <util/TranscodingException.hpp>
#include <util/ValueArrayOf.hpp>
#include <util/XMLString.hpp>
#include <util/XMLUni.hpp>


// ---------------------------------------------------------------------------
//  Local static data
//
//  gConverter
//      This is initialized when the user calls the platform init method,
//      which calls our init method. This is the converter used for default
//      conversion to/from the local code page.
// ---------------------------------------------------------------------------
static XMLTranscoder*   gTranscoder = 0;
static XMLCh            gNullStr[] =
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
        ThrowXML(IllegalArgumentException, XML4CExcepts::Str_ZeroSizedTargetBuf);

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
        ThrowXML(RuntimeException, XML4CExcepts::Str_UnknownRadix);
    }

    // See if have enough room in the caller's buffer
    if (tmpIndex > maxChars)
        ThrowXML(IllegalArgumentException, XML4CExcepts::Str_TargetBufTooSmall);

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


unsigned int XMLString::hash(   const   char* const     tohash
                                , const unsigned int    hashModulus)
{
    unsigned int hashVal = 0;
    const char* curCh = tohash;
    while (*curCh)
    {
        unsigned int top = hashVal >> 24;
        hashVal += (hashVal * 37) + top + (unsigned int)(*curCh);
        curCh++;
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
        ThrowXML(ArrayIndexOutOfBoundsException, XML4CExcepts::Str_StartIndexPastEnd);

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
        if (toTrim[skip] > ' ')
            break;
    }

    for (scrape = len; scrape > skip; scrape--)
    {
        if (toTrim[scrape - 1] > ' ')
            break;
    }

    // Cap off at the scrap point
    if (scrape)
        toTrim[scrape] = 0;

    if (skip)
    {
        // Copy the chars down
        unsigned int index = 0;
        while (toTrim[skip])
            toTrim[index++] = toTrim[skip++];
    }
}



// ---------------------------------------------------------------------------
//  Unicode versions of most of the string methods
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
        ThrowXML(IllegalArgumentException, XML4CExcepts::Str_ZeroSizedTargetBuf);

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
        ThrowXML(RuntimeException, XML4CExcepts::Str_UnknownRadix);
    }

    // See if have enough room in the caller's buffer
    if (tmpIndex > maxChars)
        ThrowXML(IllegalArgumentException, XML4CExcepts::Str_TargetBufTooSmall);

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
    return XMLPlatformUtils::fgTransService->compareNIString(str1, str2, maxChars);
}


int XMLString::compareString(   const   XMLCh* const    str1
                                , const XMLCh* const    str2)
{
    const XMLCh* psz1 = str1;
    const XMLCh* psz2 = str2;
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


unsigned int XMLString::hash(   const   XMLCh* const    tohash
                                , const unsigned int    hashModulus)
{
    unsigned int hashVal = 0;
    const XMLCh* curCh = tohash;
    while (*curCh)
    {
        unsigned int top = hashVal >> 24;
        hashVal += (hashVal * 37) + top + (unsigned int)(*curCh);
        curCh++;
    }

    // Divide by modulus
    return hashVal % hashModulus;
}


unsigned int XMLString::hashN(   const   XMLCh* const    tohash
                                , const unsigned int    n
                                , const unsigned int    hashModulus)
{
    unsigned int hashVal = 0;
    const XMLCh* curCh = tohash;
    int i = n;
    while (i--)
    {
        unsigned int top = hashVal >> 24;
        hashVal += (hashVal * 37) + top + (unsigned int)(*curCh);
        curCh++;
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
        ThrowXML(ArrayIndexOutOfBoundsException, XML4CExcepts::Str_StartIndexPastEnd);

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


XMLCh* XMLString::replicate(const XMLCh* const toRep)
{
    // If a null string, return a null string
    if (!toRep)
        return 0;

    XMLCh* ret = new XMLCh[stringLen(toRep) + 1];
    copyString(ret, toRep);
    return ret;
}


bool XMLString::startsWith( const   XMLCh* const    toTest
                            , const XMLCh* const    prefix)
{
    return (compareNString(toTest, prefix, stringLen(prefix)) == 0);
}


bool XMLString::startsWithI(const   XMLCh* const    toTest
                            , const XMLCh* const    prefix)
{
    return (compareNIString(toTest, prefix, stringLen(prefix)) == 0);
}


unsigned int XMLString::stringLen(const XMLCh* const src)
{
    if (!src)
        return 0;

    unsigned int len = 0;
    const XMLCh* pszTmp = src;
    while (*pszTmp++)
        len++;
    return len;
}


void XMLString::trim(XMLCh* const toTrim)
{
    const unsigned int len = stringLen(toTrim);

    unsigned int skip, scrape;
    for (skip = 0; skip < len; skip++)
    {
        if (XMLPlatformUtils::fgTransService->isSpace(toTrim[skip]))
            break;
    }

    for (scrape = len; scrape > skip; scrape--)
    {
        if (XMLPlatformUtils::fgTransService->isSpace(toTrim[scrape - 1]))
            break;
    }

    // Cap off at the scrap point
    if (scrape)
        toTrim[scrape] = 0;

    if (skip)
    {
        // Copy the chars down
        unsigned int index = 0;
        while (toTrim[skip])
            toTrim[index++] = toTrim[skip++];
    }
}



// ---------------------------------------------------------------------------
//  XMLString: Private static methods
// ---------------------------------------------------------------------------
void XMLString::initString(XMLTranscoder* const defToUse)
{
    // Store away the default transcoder that we are to use
    gTranscoder = defToUse;
}
