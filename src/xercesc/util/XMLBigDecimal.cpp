/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 2001-2003 The Apache Software Foundation.  All rights
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
 * Revision 1.9  2003/05/16 06:01:53  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.8  2003/05/15 19:07:46  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.7  2003/04/29 18:13:36  peiyongz
 * cut link to XMLBigInteger, patch from Khaled Noaman
 *
 * Revision 1.6  2003/02/25 17:24:18  peiyongz
 * Schema Errata: E2-44 totalDigits/fractDigits
 *
 * Revision 1.5  2003/02/02 23:54:43  peiyongz
 * getFormattedString() added to return original and converted value.
 *
 * Revision 1.4  2003/01/30 21:55:22  tng
 * Performance: create getRawData which is similar to toString but return the internal data directly, user is not required to delete the returned memory.
 *
 * Revision 1.3  2002/11/04 15:22:05  tng
 * C++ Namespace Support.
 *
 * Revision 1.2  2002/08/13 22:11:23  peiyongz
 * Fix to Bug#9442
 *
 * Revision 1.1.1.1  2002/02/01 22:22:14  peiyongz
 * sane_include
 *
 * Revision 1.8  2001/08/08 18:33:44  peiyongz
 * fix: unresolved symbol warning for 'pow'.
 *
 * Revision 1.7  2001/07/25 19:07:42  peiyongz
 * Fix to AIX compilation error: The function abs must have a prototype.
 *
 * Revision 1.6  2001/07/24 13:58:11  peiyongz
 * XMLDouble and related supporting methods from XMLBigInteger/XMLBigDecimal
 *
 * Revision 1.5  2001/06/07 20:55:21  tng
 * Fix no newline at the end warning.  By Pei Yong Zhang.
 *
 * Revision 1.4  2001/05/18 20:17:55  tng
 * Schema: More exception messages in XMLBigDecimal/XMLBigInteger/DecimalDatatypeValidator.  By Pei Yong Zhang.
 *
 * Revision 1.3  2001/05/18 13:22:54  tng
 * Schema: Exception messages in DatatypeValidator.  By Pei Yong Zhang.
 *
 * Revision 1.2  2001/05/11 13:26:30  tng
 * Copyright update.
 *
 * Revision 1.1  2001/05/10 20:51:20  tng
 * Schema: Add DecimalDatatypeValidator and XMLBigDecimal, XMLBigInteger.  By Pei Yong Zhang.
 *
 */

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/util/XMLBigDecimal.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/TransService.hpp>
#include <xercesc/util/NumberFormatException.hpp>
#include <xercesc/util/XMLUniDefs.hpp>


XERCES_CPP_NAMESPACE_BEGIN

/**
 * Constructs a BigDecimal from a string containing an optional (plus | minus)
 * sign followed by a sequence of zero or more decimal digits, optionally
 * followed by a fraction, which consists of a decimal point followed by
 * zero or more decimal digits.  The string must contain at least one
 * digit in the integer or fractional part.  The scale of the resulting
 * BigDecimal will be the number of digits to the right of the decimal
 * point in the string, or 0 if the string contains no decimal point.
 * Any extraneous characters (including whitespace) will result in
 * a NumberFormatException.

 * since parseBigDecimal()  may throw exception,
 * caller of XMLBigDecimal need to catch it.
//
**/

XMLBigDecimal::XMLBigDecimal(const XMLCh* const strValue,
                             MemoryManager* const manager)
: fSign(0)
, fTotalDigits(0)
, fScale(0)
, fIntVal(0)
, fRawData(0)
, fMemoryManager(manager)
{
    if ((!strValue) || (!*strValue))
        ThrowXML(NumberFormatException, XMLExcepts::XMLNUM_emptyString);

    try
    {
        parseBigDecimal(strValue);
        fRawData = XMLString::replicate(strValue, fMemoryManager);
    }
    catch(...)
    {
        cleanUp();
        throw;
    }
}

XMLBigDecimal::~XMLBigDecimal()
{
    cleanUp();
}

void XMLBigDecimal::cleanUp()
{
    if (fIntVal)
        fMemoryManager->deallocate(fIntVal); //delete [] fIntVal;

    if (fRawData)
        fMemoryManager->deallocate(fRawData); //XMLString::release(&fRawData);
}

void XMLBigDecimal::parseBigDecimal(const XMLCh* const toConvert)
{
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

    // '+' or '-' is allowed only at the first position
    fSign = 1;
    if (*startPtr == chDash)
    {
        fSign = -1;
        startPtr++;
    }
    else if (*startPtr == chPlus)
    {
        startPtr++;        // skip the '+'
    }

    // remove leading zeros
    while (*startPtr == chDigit_0)
        startPtr++;

    // containning zero, only zero, nothing but zero
    // it is a zero, indeed
    if (!*startPtr)
    {
        fSign = 0;
        fIntVal = (XMLCh*) fMemoryManager->allocate(sizeof(XMLCh)); //new XMLCh[1];
        fIntVal[0] = chNull;
        return;
    }

    fIntVal = (XMLCh*) fMemoryManager->allocate
    (
        (endPtr - startPtr + 1) * sizeof(XMLCh)
    ); //new XMLCh[endPtr - startPtr + 1];
    XMLCh* retPtr = fIntVal;

    // Scan data
    bool   dotSignFound = false;
    while (startPtr < endPtr)
    {
        if (*startPtr == chPeriod)
        {
            if (dotSignFound == false)
            {
                dotSignFound = true;
                fScale = endPtr - startPtr - 1;
                startPtr++;
                continue;
            }
            else  // '.' is allowed only once
                ThrowXML(NumberFormatException, XMLExcepts::XMLNUM_2ManyDecPoint);
        }

        // If not valid decimal digit, then an error
        if ((*startPtr < chDigit_0) || (*startPtr > chDigit_9))
            ThrowXML(NumberFormatException, XMLExcepts::XMLNUM_Inv_chars);

        // copy over
        *retPtr++ = *startPtr++;
        fTotalDigits++;
    }

    /***
    E2-44 totalDigits

     ... by restricting it to numbers that are expressible as i × 10^-n
     where i and n are integers such that |i| < 10^totalDigits and 0 <= n <= totalDigits. 

        normalization: remove all trailing zero after the '.'
                       and adjust the scaleValue as well.
    ***/
    while ((fScale > 0) && (*(retPtr-1) == chDigit_0))          
    {
        retPtr--;
        fScale--;
        fTotalDigits--;
    }

    *retPtr = chNull;   //terminated
    return;
}


int XMLBigDecimal::compareValues( const XMLBigDecimal* const lValue
                                , const XMLBigDecimal* const rValue)
{
    if ((!lValue) || (!rValue) )
        ThrowXML(NumberFormatException, XMLExcepts::XMLNUM_null_ptr);
        	
    return lValue->toCompare(*rValue);
}                                

/**
 * Returns -1, 0 or 1 as this is less than, equal to, or greater
 * than rValue.  
 * 
 * This method is based on the fact, that parsebigDecimal() would eliminate
 * unnecessary leading/trailing zeros. 
**/
int XMLBigDecimal::toCompare(const XMLBigDecimal& other) const
{
    /***
     * different sign
     */
    int lSign = this->getSign();
    if (lSign != other.getSign())
        return (lSign > other.getSign() ? 1 : -1);

    /***
     * same sign, zero
     */
    if (lSign == 0)    // optimization
        return 0;

    /***
     * same sign, non-zero
     */
    unsigned int lIntDigit = this->getTotalDigit() - this->getScale();
    unsigned int rIntDigit = other.getTotalDigit() - other.getScale();

    if (lIntDigit > rIntDigit)
    {
        return 1 * lSign;
    }
    else if (lIntDigit < rIntDigit)
    {
        return -1 * lSign;
    }
    else  // compare fraction
    {
        int res = XMLString::compareString
        ( this->getValue()
        , other.getValue()
        );

        if (res > 0)
            return 1 * lSign;
        else if (res < 0)
            return -1 * lSign;
        else
            return 0;
    }

}

XERCES_CPP_NAMESPACE_END

