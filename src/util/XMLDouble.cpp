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
 * $Id$
 * $Log$
 * Revision 1.1  2001/07/24 13:58:11  peiyongz
 * XMLDouble and related supporting methods from XMLBigInteger/XMLBigDecimal
 *
 */

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <util/XMLDouble.hpp>
#include <util/PlatformUtils.hpp>
#include <util/XMLString.hpp>
#include <util/XMLUniDefs.hpp>
#include <util/NumberFormatException.hpp>
#include <util/TransService.hpp>
#include <util/Janitor.hpp>
#include <math.h>
#include <iostream.h>

// 
// The special values positive and negative zero, 
// positive and negative infinity and not-a-number 
// have lexical representations 0, -0, INF, -INF and NaN, respectively
//
const XMLCh XMLDouble::fgNegINFString[] =
{
    chDash, chLatin_I, chLatin_N, chLatin_F, chNull
};

const XMLCh XMLDouble::fgNegZeroString[] =
{
    chDash, chDigit_0, chNull
};

const XMLCh XMLDouble::fgPosZeroString[] =
{
    chDigit_0, chNull
};

const XMLCh XMLDouble::fgPosINFString[] =
{
    chLatin_I, chLatin_N, chLatin_F, chNull
};

const XMLCh XMLDouble::fgNaNString[] =
{
    chLatin_N, chLatin_a, chLatin_N, chNull
};

XMLCh fgEString[] =
{
    chLatin_E, chNull
};

XMLCh fgZeroString[] =
{
    chDigit_0, chNull
};

/***
 *   Algo:
 *
 *   . Check for special cases
 *   . construct fMantissa, fExponent (optional)
 *   . No checking on the value space
 *   .
***/
XMLDouble::XMLDouble(const XMLCh* const strValue)
:fMantissa(0)
,fExponent(0)
,fType(Normal)
,fValue(0)
{
    if ((!strValue) || (!*strValue))
        ThrowXML(NumberFormatException, XMLExcepts::XMLNUM_emptyString);

//    char *p1 = XMLString::transcode(strValue);
    XMLCh* tmpStrValue = XMLString::replicate(strValue);
    ArrayJanitor<XMLCh> janTmpName(tmpStrValue);
    XMLString::trim(tmpStrValue);
//    char *p2 = XMLString::transcode(tmpStrValue);

    if (XMLString::compareString(tmpStrValue, fgNegINFString) == 0)
    {
        fType = LiteralType::NegINF;
        return;
    }
    else if (XMLString::compareString(tmpStrValue, fgNegZeroString) == 0)
    {
        fType = LiteralType::NegZero;
        return;
    }
    else if (XMLString::compareString(tmpStrValue, fgPosZeroString) == 0)
    {
        fType = LiteralType::PosZero;
        return;
    }
    else if (XMLString::compareString(tmpStrValue, fgPosINFString) == 0)
    {
        fType = LiteralType::PosINF;
        return;
    }
    else if (XMLString::compareString(tmpStrValue, fgNaNString) == 0)
    {
        fType = LiteralType::NaN;
        return;
    }

    //
    // Normal case
    //
    int ePos = 0;
    int tmpStrLen = XMLString::stringLen(tmpStrValue);

    if ((( ePos = XMLString::indexOf(tmpStrValue, chLatin_E)) != -1 ) ||
        (( ePos = XMLString::indexOf(tmpStrValue, chLatin_e)) != -1 )  )
    {
        XMLCh* tmpMantissa = new XMLCh [ePos+1];
        XMLString::subString(tmpMantissa, tmpStrValue, 0, ePos);
        ArrayJanitor<XMLCh> janMantissa(tmpMantissa);
        fMantissa = new XMLBigDecimal(tmpMantissa);

        //
        // 1234.56E78
        // 
        if ( ePos < tmpStrLen - 1)
        {
            XMLCh* tmpExponent = new XMLCh [tmpStrLen - ePos];
            XMLString::subString(tmpExponent, tmpStrValue, ePos + 1, tmpStrLen);
            ArrayJanitor<XMLCh> janExponent(tmpExponent);
            fExponent = new XMLBigInteger(tmpExponent);
        }
        //
        // 1234.56E
        // 
        else
        {        
            fExponent = new XMLBigInteger(fgZeroString);
        }

    }
    else
    {
        fMantissa = new XMLBigDecimal(tmpStrValue);
        fExponent = new XMLBigInteger(fgZeroString);
    }


    fValue = fMantissa->doubleValue() * pow(10.0, fExponent->intValue());
}

XMLDouble::XMLDouble(const XMLDouble& toCopy)
:fMantissa(0)
,fExponent(0)
,fType(Normal)
,fValue(0)
{
    fMantissa = new XMLBigDecimal(*(toCopy.fMantissa));
    fExponent = new XMLBigInteger(*(toCopy.fExponent));
    fType     = toCopy.fType;
    fValue    = toCopy.fValue;
}

//
// Add the 'E' as necessary
// The caller needs to de-allocate the memory allocated by this function
// Deallocate the memory allocated by XMLBigInteger
//
XMLCh*  XMLDouble::toString() const
{
    switch (fType) 
    {
    case NegINF:
        return XMLString::replicate(fgNegINFString);

    case NegZero:
        return XMLString::replicate(fgNegZeroString);

    case PosZero:
        return XMLString::replicate(fgPosZeroString);

    case PosINF:
        return XMLString::replicate(fgPosINFString);

    case NaN:
        return XMLString::replicate(fgNaNString);

    case Normal:

        XMLCh *ret_mantissa;
        ret_mantissa = fMantissa->toString();

        if (fExponent)
        {
            XMLCh *ret_exponent = fExponent->toString();
            XMLCh *ret_val = new XMLCh [ XMLString::stringLen(ret_mantissa) + 
                                         XMLString::stringLen(ret_exponent) + 2];
            *ret_val = 0;
            XMLString::catString(ret_val, ret_mantissa);
            XMLString::catString(ret_val, fgEString);
            XMLString::catString(ret_val, ret_exponent);

            delete [] ret_mantissa;
            delete [] ret_exponent;
            return ret_val;
        }
        else
            return ret_mantissa;

        break;
    default:
        ThrowXML(NumberFormatException, XMLExcepts::XMLNUM_emptyString);
        //internal error
    };

    return (XMLCh*) 0;
}

//
//
//
int XMLDouble::compareValues(const XMLDouble* const lValue
                           , const XMLDouble* const rValue)
{
    //
    // case#1: lValue normal
    //         rValue normal
    //
    if ((!lValue->isSpecialValue()) &&
        (!rValue->isSpecialValue())  )
    {
        //
        // if we use fValue to compare two 
        // sequences "12.3456E4" and "1234.56E2",
        // they are _NOT_ the same. so we ask
        // BigDecimal to compare.
        //
        XMLBigDecimal ldv = XMLBigDecimal(*(lValue->fMantissa), lValue->fExponent->intValue());
        XMLBigDecimal rdv = XMLBigDecimal(*(rValue->fMantissa), rValue->fExponent->intValue());

        return XMLBigDecimal::compareValues(&ldv, &rdv);
    }
    //
    // case#2: lValue special
    //         rValue special
    //
    else
    if ((lValue->isSpecialValue()) &&
        (rValue->isSpecialValue())  )
    {
        if (lValue->fType == rValue->fType)
            return 0;
        else
            return (lValue->fType > rValue->fType ? 1 : -1);
    }
    //
    // case#3: lValue special
    //         rValue normal
    //
    else
    if ((lValue->isSpecialValue()) &&
        (!rValue->isSpecialValue())  )
    {
        return compareSpecial(lValue, rValue);
    }
    //
    // case#4: lValue normal
    //         rValue special
    //
    else
    {
        return (-1) * compareSpecial(rValue, lValue);
    }

    return 0;
}

int XMLDouble::compareSpecial(const XMLDouble* const specialValue
                            , const XMLDouble* const normalValue)
{
    switch (specialValue->fType) 
    {
    case NegINF:
        return -1;

    case NegZero:
    case PosZero:
        return (normalValue->doubleValue() > 0 ? -1 : 1);

    case PosINF:
        return 1;

    case NaN:
        return 1;

    default:
        ThrowXML(NumberFormatException, XMLExcepts::XMLNUM_emptyString);
        //internal error
    }
}