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

/*
 * $Log$
 * Revision 1.1  2001/05/10 20:51:20  tng
 * Schema: Add DecimalDatatypeValidator and XMLBigDecimal, XMLBigInteger.  By Pei Yong Zhang.
 *
 */

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <string.h>
#include <iostream.h>
#include <util/XMLBigDecimal.hpp>
#include <util/PlatformUtils.hpp>
#include <util/XMLString.hpp>
#include <util/XMLUniDefs.hpp>
#include <util/NumberFormatException.hpp>
#include <util/RuntimeException.hpp>
#include <util/TransService.hpp>
#include <util/Janitor.hpp>

/**
	 * Constructs a BigDecimal from a string containing an optional minus
	 * sign followed by a sequence of zero or more decimal digits, optionally
	 * followed by a fraction, which consists of a decimal point followed by
	 * zero or more decimal digits.  The string must contain at least one
	 * digit in the integer or fractional part.  The scale of the resulting
	 * BigDecimal will be the number of digits to the right of the decimal
	 * point in the string, or 0 if the string contains no decimal point.
	 * Any extraneous characters (including whitespace) will result in
	 * a NumberFormatException.
*/
//
// since parseBigDecimal and XMLBigInteger() may
// throw exception, caller of XMLBigDecimal better
// be ready to catch it.
//

XMLBigDecimal::XMLBigDecimal(const XMLCh* const strValue)
:fIntVal(0)
,fScale(0)
{
    XMLCh* ret_value = new XMLCh[XMLString::stringLen(strValue)+1];
    ArrayJanitor<XMLCh> janName(ret_value);

    parseBigDecimal(strValue, ret_value, fScale);
    fIntVal = new XMLBigInteger(ret_value);
   
}

XMLBigDecimal::XMLBigDecimal(const XMLBigDecimal& toCopy)
:fIntVal(0)
,fScale(toCopy.getScale())
{
    
    //invoke XMLBigInteger' copy ctor
    fIntVal = new XMLBigInteger(*(toCopy.getValue())); 
}


/***
   *               
   *  Leading and trailing whitespaces are allowed, and trimmed
   *
   *  Only one and either of (+,-) after the leading whitespace, before
   *  any other characters are allowed, '+' removed
   *
   *  '.' allowed and removed

   *  return status: void
   *  retBuffer: w/o leading and/or trailing whitespace
   *             w/o '+' and containning one '-' if any
   *             w/o leading zero
   *             w/o '.'
   *
   *  scalevalue: indicate the number of digits, right to
   *              the '.'
   *
   *  see XMLBigInteger::parseBigInteger();
   *      XMLString::textToBin();
   *
   *  "    +000203.456"            "203456"
   *  "    -000203.456"            "-203456"
   *  "    -000.456"               "-456"
   *
***/

void XMLBigDecimal::parseBigDecimal(const XMLCh* const toConvert
                                  , XMLCh* const retBuffer
                                  , unsigned int & scaleValue)
{
    scaleValue = 0;

    // If no string, then its a failure
    if ((!toConvert) ||
        (!*toConvert))
        ThrowXML(NumberFormatException, XMLExcepts::CM_UnaryOpHadBinType);
        //ThrowXML(NumberFormatException, XMLExcepts::XMLINT_Invalid);

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
    XMLCh* retPtr = retBuffer;
    //
    // '+' or '-' is allowed only at the first position
    //
    if (*startPtr == chDash)
    {
        // copy the '-'
        *retPtr = chDash;
        startPtr++;
        retPtr++;
    }
    else if (*startPtr == chPlus)
    {
        // skip the '+'
        startPtr++;
    }
        
    // Leading zero will be taken care by BigInteger

    bool   dotSignFound = false;

    while (startPtr < endPtr)
    {
        //
        // '.' is allowed only once
        //
        if (*startPtr == chPeriod)
        {
            if (dotSignFound == false)
            {
                dotSignFound = true;
                scaleValue = endPtr - startPtr - 1;
                startPtr++;
                continue;
            }
            else
                ThrowXML(NumberFormatException, XMLExcepts::CM_UnaryOpHadBinType);
                //ThrowXML(NumberFormatException, XMLExcepts::XMLBIGDECIMAL_MANY_DOT);
        }

        // If not valid decimal digit, then an error
        if ((*startPtr < chDigit_0) || (*startPtr > chDigit_9))
            ThrowXML(NumberFormatException, XMLExcepts::CM_UnaryOpHadBinType);
            //ThrowXML(NumberFormatException, XMLExcepts::XMLBIGDECIMAL_Invalid);

        // copy over
        *retPtr = *startPtr;
        retPtr++;
        startPtr++;
    }

    *retPtr = 0;   //terminated 
    return;
}

/**
 * Returns -1, 0 or 1 as lValue is less than, equal to, or greater
 * than rValue.  Two BigDecimals that are equal in value but have a
 * different scale (e.g., 2.0, 2.00) are considered equal by this method.
*/

int XMLBigDecimal::compareValues(const XMLBigDecimal* const lValue
                               , const XMLBigDecimal* const rValue)
{                                                  
    //
    if ((!lValue) ||
        (!rValue) )
        ThrowXML(NumberFormatException, XMLExcepts::CM_UnaryOpHadBinType);
        //ThrowXML(NumberFormatException, XMLExcepts::XMLBIGDECIMAL_Null_value);        
    
    /* Optimization: would run fine without the next three lines */
	int sigDiff = lValue->getSign() - rValue->getSign();
	if (sigDiff != 0)
	    return (sigDiff > 0 ? 1 : -1);

    //
	// If signs match, scale and compare intVals 
    // since matchScale will destructively change the scale
    // we make a copy for both
    //
    if (lValue->getScale() != rValue->getScale())
    {
        XMLBigDecimal lTemp = *lValue;
  	    XMLBigDecimal rTemp = *rValue;

	    matchScale(&lTemp, &rTemp);
        return XMLBigInteger::compareValues(lTemp.getValue(), rTemp.getValue());
    }

    return XMLBigInteger::compareValues(lValue->getValue(), rValue->getValue());
}

/*
 * If the scales of lValue and rValue differ, rescale (destructively)
 * the lower-scaled BigDecimal so they match.
 *
 * rescale the lower-scaled will not lose precision.
 *
*/
void XMLBigDecimal::matchScale(XMLBigDecimal* const lValue
                             , XMLBigDecimal* const rValue)
{
	if (lValue->getScale() < rValue->getScale())
	    lValue->reScale(rValue->getScale());
	else
    if (lValue->getScale() > rValue->getScale())
	    rValue->reScale(lValue->getScale());
}


void XMLBigDecimal::reScale(unsigned int newScale)
{
	if (newScale < 0)
        return;

	/* Handle the easy cases */
	if (newScale == this->getScale())
	    return;
	else if (newScale > this->getScale())
    {
        fIntVal->multiply(newScale - this->getScale());
        fScale = newScale;
    }
	else /* scale < this.scale */
    {
        fIntVal->divide(this->getScale() - newScale);
        fScale = newScale;
	}

    return;
}

void XMLBigDecimal::dumpData() const
{
    cout<<"scale="<<"<"<<fScale<<">"<<endl;
    fIntVal->dumpData();
    cout<<endl;
}