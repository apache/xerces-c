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
 * Revision 1.9  2003/02/02 23:54:43  peiyongz
 * getFormattedString() added to return original and converted value.
 *
 * Revision 1.8  2003/01/30 21:55:22  tng
 * Performance: create getRawData which is similar to toString but return the internal data directly, user is not required to delete the returned memory.
 *
 * Revision 1.7  2002/12/11 00:20:02  peiyongz
 * Doing businesss in value space. Converting out-of-bound value into special values.
 *
 * Revision 1.6  2002/11/04 15:22:05  tng
 * C++ Namespace Support.
 *
 * Revision 1.5  2002/09/24 19:51:24  tng
 * Performance: use XMLString::equals instead of XMLString::compareString
 *
 * Revision 1.4  2002/05/03 16:05:45  peiyongz
 * Bug 7341: Missing newline at end of util and DOM source files,
 * patch from Martin Kalen.
 *
 * Revision 1.3  2002/03/06 19:13:12  peiyongz
 * Patch: more valid lexcial representation for positive/negative zero
 *
 * Revision 1.2  2002/03/01 18:47:37  peiyongz
 * fix: more valid lexcial representation forms for "neural zero"
 *
 * Revision 1.1.1.1  2002/02/01 22:22:14  peiyongz
 * sane_include
 *
 * Revision 1.2  2001/11/22 21:39:00  peiyongz
 * Allow "0.0" to be a valid lexcial representation of ZERO.
 *
 * Revision 1.1  2001/11/19 21:33:42  peiyongz
 * Reorganization: Double/Float
 *
 *
 */

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/util/XMLAbstractDoubleFloat.hpp>
#include <xercesc/util/XMLUniDefs.hpp>
#include <xercesc/util/NumberFormatException.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/Janitor.hpp>

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  local data member
// ---------------------------------------------------------------------------
static const int BUF_LEN = 64;
static XMLCh value1[BUF_LEN+1];

// ---------------------------------------------------------------------------
//  ctor/dtor
// ---------------------------------------------------------------------------
XMLAbstractDoubleFloat::XMLAbstractDoubleFloat()
:fValue(0)
,fType(Normal)
,fDataConverted(false)
,fSign(0)
,fRawData(0)
,fFormattedString(0)
{
}

XMLAbstractDoubleFloat::~XMLAbstractDoubleFloat()
{
     delete [] fRawData;
     delete [] fFormattedString;
}

void XMLAbstractDoubleFloat::init(const XMLCh* const strValue)
{
    if ((!strValue) || (!*strValue))
        ThrowXML(NumberFormatException, XMLExcepts::XMLNUM_emptyString);

    fRawData = XMLString::replicate(strValue);   // preserve the raw data form

    XMLCh* tmpStrValue = XMLString::replicate(strValue);
    ArrayJanitor<XMLCh> janTmpName(tmpStrValue);
    XMLString::trim(tmpStrValue);

    normalizeZero(tmpStrValue);

    if (XMLString::equals(tmpStrValue, XMLUni::fgNegINFString) )
    {
        fType = NegINF;
        fSign = -1;
    }
    else if (XMLString::equals(tmpStrValue, XMLUni::fgNegZeroString) )
    {
        fType = NegZero;
        fSign = -1;
    }
    else if (XMLString::equals(tmpStrValue, XMLUni::fgPosZeroString) )
    {
        fType = PosZero;
        fSign = 1;
    }
    else if (XMLString::equals(tmpStrValue, XMLUni::fgPosINFString) )
    {
        fType = PosINF;
        fSign = 1;
    }
    else if (XMLString::equals(tmpStrValue, XMLUni::fgNaNString) )
    {
        fType = NaN;
        fSign = 1;
    }
    else
        //
        // Normal case
        //
    {
        checkBoundary(tmpStrValue);
    }

}

//
// 
//
XMLCh*  XMLAbstractDoubleFloat::toString() const
{
    return XMLString::replicate(fRawData);
}

XMLCh*  XMLAbstractDoubleFloat::getRawData() const
{
    return fRawData;
}

const XMLCh*  XMLAbstractDoubleFloat::getFormattedString() const
{
    if (!fDataConverted)
    {
        return fRawData;
    }
    else 
    {
        if (!fFormattedString)    	
        {
            XMLAbstractDoubleFloat *temp = (XMLAbstractDoubleFloat *) this;
            temp->formatString();
        }

        return fFormattedString;           
    }

}

void XMLAbstractDoubleFloat::formatString()
{

    unsigned int rawDataLen = XMLString::stringLen(fRawData);
    fFormattedString = new XMLCh [ rawDataLen + 8];
    for (unsigned int i = 0; i < rawDataLen + 8; i++)
        fFormattedString[i] = chNull;

    XMLString::copyString(fFormattedString, fRawData);

    fFormattedString[rawDataLen] = chSpace;
    fFormattedString[rawDataLen + 1] = chOpenParen;

    switch (fType)
    {
    case NegINF:       
        XMLString::catString(fFormattedString, XMLUni::fgNegINFString);
        break;
    case NegZero:
        XMLString::catString(fFormattedString, XMLUni::fgNegZeroString);
        break;
    case PosZero:
        XMLString::catString(fFormattedString, XMLUni::fgPosZeroString);
        break;
    case PosINF:
        XMLString::catString(fFormattedString, XMLUni::fgPosINFString);
        break;
    case NaN:
        XMLString::catString(fFormattedString, XMLUni::fgNaNString);
        break;
    }

    fFormattedString[XMLString::stringLen(fFormattedString)] = chCloseParen;

}

int XMLAbstractDoubleFloat::getSign() const
{
    return fSign;
}

//
//
//
int XMLAbstractDoubleFloat::compareValues(const XMLAbstractDoubleFloat* const lValue
                                        , const XMLAbstractDoubleFloat* const rValue)
{
    //
    // case#1: lValue normal
    //         rValue normal
    //
    if ((!lValue->isSpecialValue()) &&
        (!rValue->isSpecialValue())  )
    {
        if (lValue->fValue == rValue->fValue)
            return 0;
        else
            return (lValue->fValue > rValue->fValue) ? 1: -1;

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
            return (lValue->fType > rValue->fType) ? 1 : -1;
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

int XMLAbstractDoubleFloat::compareSpecial(const XMLAbstractDoubleFloat* const specialValue
                                         , const XMLAbstractDoubleFloat* const normalValue)
{
    switch (specialValue->fType)
    {
    case NegINF:
        return -1;

    case NegZero:
    case PosZero:
        return (normalValue->getSign() > 0 ? -1 : 1);

    case PosINF:
        return 1;

    case NaN:
        return 1;

    default:
        XMLString::binToText(specialValue->fType, value1, 16, 10);
        ThrowXML1(NumberFormatException
                , XMLExcepts::XMLNUM_DBL_FLT_InvalidType
                , value1);
        //internal error
        return 0;
    }
}

//
//  Assumption: no leading space
//
//  1. The valid char set is "+-.0"
//  2. There shall be only one sign at the first position, if there is one.
//  3. There shall be only one dot '.', if there is one.
//
//  Return:
//
//  for input comforming to [+]? [0]* '.'? [0]*,
//            normalize the input to positive zero string
//  for input comforming to '-' [0]* '.'? [0]*,
//            normalize the input to negative zero string
//  otherwise, do nothing
//
void XMLAbstractDoubleFloat::normalizeZero(XMLCh* const inData)
{

	// do a quick check
	if (!inData  ||
		!*inData ||
        (XMLString::equals(inData, XMLUni::fgNegZeroString) ) ||
        (XMLString::equals(inData, XMLUni::fgPosZeroString) )  )
        return;

    XMLCh*   srcStr = inData;
	bool     minusSeen = false;

	// process sign if any
	if (*srcStr == chDash)
	{
		minusSeen = true;
		srcStr++;
	}
	else if (*srcStr == chPlus)
	{
		srcStr++;
	}

	// scan the string
	bool  dotSeen = false;
	bool  isValidStr = true;
    XMLCh theChar;
	while ((theChar=*srcStr++) && isValidStr)
	{
		if ( theChar != chPeriod && theChar != chDigit_0 )
			isValidStr = false;           		// invalid char
        else if (theChar == chPeriod)           // process dot
			dotSeen ? isValidStr = false : dotSeen = true;
	}

	// need not to worry about the memory problem
	// since either fgNegZeroString or fgPosZeroString
	// is the canonical form (meaning the shortest in length)
	// of their category respectively.
	if (isValidStr)
	{
		if (minusSeen)
			XMLString::copyString(inData, XMLUni::fgNegZeroString);
		else
			XMLString::copyString(inData, XMLUni::fgPosZeroString);
	}
    else
    {
        // we got to set the sign first, since this string may
        // eventaully turn out to be beyond the minimum representable 
        // number and reduced to -0 or +0.
        fSign = minusSeen ? -1 : 1;
    }

    return;
} 

XERCES_CPP_NAMESPACE_END
