/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 2001 The Apache Software Foundation.  All rights
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
 * originally based on software copyright (c) 2001, International
 * Business Machines, Inc., http://www.ibm.com .  For more information
 * on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 */

/*
 * $Log$
 * Revision 1.4  2001/05/18 20:17:57  tng
 * Schema: More exception messages in XMLBigDecimal/XMLBigInteger/DecimalDatatypeValidator.  By Pei Yong Zhang.
 *
 * Revision 1.3  2001/05/18 13:23:01  tng
 * Schema: Exception messages in DatatypeValidator.  By Pei Yong Zhang.
 *
 * Revision 1.2  2001/05/11 13:26:31  tng
 * Copyright update.
 *
 * Revision 1.1  2001/05/10 20:51:23  tng
 * Schema: Add DecimalDatatypeValidator and XMLBigDecimal, XMLBigInteger.  By Pei Yong Zhang.
 *
 */

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <util/XMLBigInteger.hpp>
#include <util/XMLString.hpp>
#include <util/NumberFormatException.hpp>
#include <util/PlatformUtils.hpp>
#include <util/TransService.hpp>
#include <util/XMLUniDefs.hpp>
#include <util/Janitor.hpp>

/***
   *
   *  Leading and trailing whitespaces are allowed, and trimmed
   *
   *  Only one and either of (+,-) after the leading whitespace, before
   *  any other characters are allowed, and trimmed
   *
   *  Leading zero, after leading whitespace, (+|-), before any other
   *  characters are allowed, and trimmed
   *
   *  '.' NOT allowed

   *  return status: void
   *  ret_buf: w/o leading and/or trailing whitespace
   *           w/o '+' and '-'
   *           w/o leading zero
   *
   *  see XMLString::parseInt();
   *      XMLString::textToBin();
   *
   *  "    +000203456"            "203456"
   *  "    -000203456"            "203456"
   *
***/

void XMLBigInteger::parseBigInteger(const XMLCh* const toConvert
                                  , XMLCh* const retBuffer
                                  , int&   signValue)
{
    // If no string, then its a failure
    if ((!toConvert) || (!*toConvert))
        ThrowXML(NumberFormatException, XMLExcepts::XMLNUM_emptyString);

    //
    // Note: in Java's BigInteger, it seems any leading and/or trailing
    // whitespaces are not allowed. If this is the case, we may
    // need to skip the trimming below.
    //

    // Scan past any whitespace. If we hit the end, then return failure
    const XMLCh* startPtr = toConvert;
    while (XMLPlatformUtils::fgTransService->isSpace(*startPtr))
        startPtr++;

    if (!*startPtr)
        ThrowXML(NumberFormatException, XMLExcepts::XMLNUM_WSString);

    // Start at the end and work back through any whitespace
    const XMLCh* endPtr = toConvert + XMLString::stringLen(toConvert);
    while (XMLPlatformUtils::fgTransService->isSpace(*(endPtr - 1)))
        endPtr--;

    //
    //  Work through what remains and convert each char to a digit.
    //  anything other than '
    //
    XMLCh* retPtr = retBuffer;
    signValue = 1;

    //
    // '+' or '-' is allowed only at the first position
    //
    if (*startPtr == chDash)
    {
        signValue = -1;
        startPtr++;
    }
    else if (*startPtr == chPlus)
    {
        // skip the '+'
        startPtr++;
    }

    // Scan past any leading zero.
    while (*startPtr == chDigit_0)
        startPtr++;

    if (!*startPtr)
    {
        signValue = 0;
        // containning zero, only zero, nothing but zero
        // it is a zero, indeed
        return;
    }

    while (startPtr < endPtr)
    {
        // If not valid decimal digit, then an error
        if ((*startPtr < chDigit_0) || (*startPtr > chDigit_9))
            ThrowXML(NumberFormatException, XMLExcepts::XMLNUM_Inv_chars);

        // copy over
        *retPtr = *startPtr;
        retPtr++;
        startPtr++;
    }

    *retPtr = 0;   //terminated
    return;
}

/**
	 * Translates a string containing an optional minus sign followed by a
	 * sequence of one or more digits into a BigInteger.
	 * Any extraneous characters (including whitespace),
	 * inclusive, will result in a NumberFormatException.
 */
XMLBigInteger::XMLBigInteger(const XMLCh* const strValue)
{
    if (!strValue)
        ThrowXML(NumberFormatException, XMLExcepts::XMLNUM_emptyString);

    XMLCh* ret_value = new XMLCh[XMLString::stringLen(strValue)+1];
    ArrayJanitor<XMLCh> janName(ret_value);

    parseBigInteger(strValue, ret_value, fSign);

    if (fSign == 0)
        fMagnitude = XMLString::replicate(XMLUni::fgZeroLenString);
    else
        fMagnitude = XMLString::replicate(ret_value);

}

XMLBigInteger::~XMLBigInteger()
{
    delete[] fMagnitude;
}

XMLBigInteger::XMLBigInteger(const XMLBigInteger& toCopy)
{
    setSign(toCopy.getSign());
    fMagnitude = XMLString::replicate(toCopy.fMagnitude);
}

/**
 * Returns -1, 0 or 1 as lValue is less than, equal to, or greater
 * than rValue.
*/
int  XMLBigInteger::compareValues(const XMLBigInteger* const lValue
                                , const XMLBigInteger* const rValue)
{
    if ((!lValue) || (!rValue) )
        ThrowXML(NumberFormatException, XMLExcepts::XMLNUM_null_ptr);

    int lSign = lValue->getSign();
    int rSign = rValue->getSign();

    //
    // different sign
    //
    if (lSign != rSign)
        return(lSign > rSign ? 1 : -1);

    //
    // same sign
    //
    if (lSign == 0)    // optimization
        return 0;

    int lStrLen = XMLString::stringLen(lValue->fMagnitude);
    int rStrLen = XMLString::stringLen(rValue->fMagnitude);

    //
    // different length
    //
    if (lStrLen > rStrLen)
        return ( lSign > 0 ? 1 : -1 );
    else if (lStrLen < rStrLen)
        return ( lSign > 0 ? -1 : 1 );

    //
    // same length
    // XMLString::compareString() return > 0, 0 and <0
    // we need to convert it to 1, 0, and -1
    //
    int retVal = XMLString::compareString(lValue->fMagnitude, rValue->fMagnitude);

    if ( retVal > 0 )
    {
        return ( lSign > 0 ? 1 : -1 );
    }
    else if ( retVal < 0 )
    {
        return ( lSign > 0 ? -1 : 1 );
    }
    else
        return 0;

}

/**
 * Shift the fMagnitude to the left
 */

void XMLBigInteger::multiply(const unsigned int byteToShift)
{
    if (byteToShift <= 0)
        return;

    int strLen = XMLString::stringLen(fMagnitude);
    XMLCh* tmp = new XMLCh[strLen+byteToShift+1];
    XMLString::moveChars(tmp, fMagnitude, strLen);

    unsigned int i = 0;
    for ( ; i < byteToShift; i++)
        tmp[strLen+i] = chDigit_0;

    tmp[strLen+i] = chNull;

    delete[] fMagnitude;
    fMagnitude = tmp;
}

/**
 * Shift the fMagnitude to the right
 * by doing this, we lose precision.
 */
void XMLBigInteger::divide(const unsigned int byteToShift)
{
    if (byteToShift <= 0)
        return;

    int strLen = XMLString::stringLen(fMagnitude);
    XMLCh* tmp = new XMLCh[strLen-byteToShift+1];
    XMLString::moveChars(tmp, fMagnitude, strLen-byteToShift);

    tmp[strLen-byteToShift] = chNull;

    delete[] fMagnitude;
    fMagnitude = tmp;
}

//
// The caller needs to de-allocate the memory allocated by this function
// return buffer ALWAYS has a leading sign
//
XMLCh*  XMLBigInteger::toString() const
{

    if ( fSign == 0 )
    {
        XMLCh* retBuf = new XMLCh[3];
        retBuf[0] = chPlus;
        retBuf[1] = chDigit_0;
        retBuf[2] = chNull;
        return retBuf;
    }
 
    // Add the leading sign here    
    int strLen = XMLString::stringLen(fMagnitude);
    XMLCh* retBuf = new XMLCh[strLen+2];
    (fSign == 1)? retBuf[0] = chPlus : retBuf[0] = chDash;
    XMLString::moveChars(&(retBuf[1]), &(fMagnitude[0]), strLen);
    retBuf[strLen+1] = chNull;

    return retBuf;
}
