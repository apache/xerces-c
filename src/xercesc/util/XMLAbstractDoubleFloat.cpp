/*
 * Copyright 2001,2004 The Apache Software Foundation.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *      http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * $Id$
 * $Log$
 * Revision 1.31  2004/11/24 02:34:08  cargilld
 * Various bug fixes and code cleanup for XSValue.
 *
 * Revision 1.30  2004/10/28 20:14:58  peiyongz
 * store/load fDataOverflowed
 *
 * Revision 1.29  2004/09/09 20:09:30  peiyongz
 * getDataOverflowed()
 *
 * Revision 1.28  2004/09/08 13:56:23  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.27  2004/08/23 16:06:49  peiyongz
 * Fix to memory leakage in getCanRep
 *
 * Revision 1.26  2004/01/29 11:48:46  cargilld
 * Code cleanup changes to get rid of various compiler diagnostic messages.
 *
 * Revision 1.25  2004/01/13 19:50:56  peiyongz
 * remove parseContent()
 *
 * Revision 1.24  2004/01/12 16:23:39  neilg
 * remove use of static buffers
 *
 * Revision 1.23  2004/01/03 00:03:18  peiyongz
 * parseContent
 *
 * Revision 1.22  2003/12/23 21:48:14  peiyongz
 * Absorb exception thrown in getCanonicalRepresentation and return 0
 *
 * Revision 1.21  2003/12/20 06:21:34  neilg
 * fix canonical representation production
 *
 * Revision 1.20  2003/12/17 00:18:35  cargilld
 * Update to memory management so that the static memory manager (one used to call Initialize) is only for static data.
 *
 * Revision 1.19  2003/12/12 04:51:29  peiyongz
 * trailing zeros for double/float w/o decimal point
 *
 * Revision 1.18  2003/12/11 21:38:12  peiyongz
 * support for Canonical Representation for Datatype
 *
 * Revision 1.17  2003/10/15 14:50:01  peiyongz
 * Bugzilla#22821: locale-sensitive function used to validate 'double' type, patch
 * from jsweeney@spss.com (Jeff Sweeney)
 *
 * Revision 1.16  2003/09/25 22:24:28  peiyongz
 * Using writeString/readString
 *
 * Revision 1.15  2003/09/25 15:23:25  peiyongz
 * add sizeof(XMLCh) when allocating memory
 *
 * Revision 1.14  2003/09/23 18:16:07  peiyongz
 * Inplementation for Serialization/Deserialization
 *
 * Revision 1.13  2003/05/16 06:01:52  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.12  2003/05/15 19:07:46  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.11  2003/03/12 20:45:46  peiyongz
 * format string for value converted to Zero.
 *
 * Revision 1.10  2003/03/10 20:55:58  peiyongz
 * Schema Errata E2-40 double/float
 *
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
#include <xercesc/util/XMLBigDecimal.hpp>
#include <xercesc/util/XMLUniDefs.hpp>
#include <xercesc/util/NumberFormatException.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/Janitor.hpp>

#include <locale.h>

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  local data member
// ---------------------------------------------------------------------------
static const int BUF_LEN = 64;

static XMLCh expSign[] = {chLatin_e, chLatin_E, chNull};

// ---------------------------------------------------------------------------
//  ctor/dtor
// ---------------------------------------------------------------------------
XMLAbstractDoubleFloat::XMLAbstractDoubleFloat(MemoryManager* const manager)
: fValue(0)
, fType(Normal)
, fDataConverted(false)
, fDataOverflowed(false)
, fSign(0)
, fRawData(0)
, fFormattedString(0)
, fMemoryManager(manager)
{
}

XMLAbstractDoubleFloat::~XMLAbstractDoubleFloat()
{
     fMemoryManager->deallocate(fRawData);//delete [] fRawData;
     fMemoryManager->deallocate(fFormattedString);//delete [] fFormattedString;
}

void XMLAbstractDoubleFloat::init(const XMLCh* const strValue)
{
    if ((!strValue) || (!*strValue))
        ThrowXMLwithMemMgr(NumberFormatException, XMLExcepts::XMLNUM_emptyString, fMemoryManager);

    fRawData = XMLString::replicate(strValue, fMemoryManager);   // preserve the raw data form

    XMLCh* tmpStrValue = XMLString::replicate(strValue, fMemoryManager);
    ArrayJanitor<XMLCh> janTmpName(tmpStrValue, fMemoryManager);
    XMLString::trim(tmpStrValue);

    if (!*tmpStrValue) 
        ThrowXMLwithMemMgr(NumberFormatException, XMLExcepts::XMLNUM_emptyString, fMemoryManager);

    normalizeZero(tmpStrValue);

    if (XMLString::equals(tmpStrValue, XMLUni::fgNegINFString) )
    {
        fType = NegINF;
        fSign = -1;
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
    // Return data using global operator new
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
    fFormattedString = (XMLCh*) fMemoryManager->allocate
    (
        (rawDataLen + 8) * sizeof(XMLCh)
    );//new XMLCh [ rawDataLen + 8];
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
    case PosINF:
        XMLString::catString(fFormattedString, XMLUni::fgPosINFString);
        break;
    case NaN:
        XMLString::catString(fFormattedString, XMLUni::fgNaNString);
        break;
    default:
        // its zero
        XMLString::catString(fFormattedString, XMLUni::fgPosZeroString);
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
                                        , const XMLAbstractDoubleFloat* const rValue
                                        , MemoryManager* const manager)
{
    //
    // case#1: lValue normal
    //         rValue normal
    //
    if ((!lValue->isSpecialValue()) &&
        (!rValue->isSpecialValue())  )
    {
        if (lValue->fValue == rValue->fValue)
            return EQUAL;
        else
            return (lValue->fValue > rValue->fValue) ? GREATER_THAN : LESS_THAN;

    }
    //
    // case#2: lValue special
    //         rValue special
    //
    // Schema Errata E2-40
    // 
    // Positive Infinity is greater than all other non-NAN value.
    // Nan equals itself but is not comparable with (neither greater than nor less than)
    //     any other value in the value space
    // Negative Infinity is less than all other non-NAN values.
    //
    else
    if ((lValue->isSpecialValue()) &&
        (rValue->isSpecialValue())  )
    {
        if (lValue->fType == rValue->fType)
            return EQUAL;
        else
        {
            if ((lValue->fType == NaN) ||
                (rValue->fType == NaN)  )
            {
                return INDETERMINATE;
            }
            else
            {
                return (lValue->fType > rValue->fType) ? GREATER_THAN : LESS_THAN;
            }
        }

    }
    //
    // case#3: lValue special
    //         rValue normal
    //
    else
    if ((lValue->isSpecialValue()) &&
        (!rValue->isSpecialValue())  )
    {
        return compareSpecial(lValue, manager);
    }
    //
    // case#4: lValue normal
    //         rValue special
    //
    else
    {
        return (-1) * compareSpecial(rValue, manager);
    }

    return 0;
}

int XMLAbstractDoubleFloat::compareSpecial(const XMLAbstractDoubleFloat* const specialValue                                         
                                         , MemoryManager* const manager)
{
    switch (specialValue->fType)
    {
    case NegINF:
        return LESS_THAN;
    case PosINF:
        return GREATER_THAN;
    case NaN:
        // NaN is not comparable to any other value
        return INDETERMINATE;

    default:
        XMLCh value1[BUF_LEN+1];
        XMLString::binToText(specialValue->fType, value1, 16, 10, manager);
        ThrowXMLwithMemMgr1(NumberFormatException
                , XMLExcepts::XMLNUM_DBL_FLT_InvalidType
                , value1, manager);
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

void XMLAbstractDoubleFloat::normalizeDecimalPoint(char* const toNormal)
{
    // find the locale-specific decimal point delimiter
    lconv* lc = localeconv();
    char delimiter = *lc->decimal_point;

    // replace '.' with the locale-specific decimal point delimiter
    if ( delimiter != '.' )
    {
        char* period = strchr( toNormal, '.' );
        if ( period )
        {
            *period = delimiter;
        }
    }
}

/***
 * E2-40
 *
 *   3.2.4 float
 *   3.2.5 double
 *
 * . the exponent must be indicated by "E". 
 *   if the exponent is zero, it must be indicated by "E0". 
 *
 * . For the mantissa, 
 *      the preceding optional "+" sign is prohibited and 
 *      the decimal point is required. 
 *
 * . For the exponent, 
 *      the preceding optional "+" sign is prohibited. 
 *      Leading zeroes are prohibited.
 *      
 * . Leading and trailing zeroes are prohibited subject to the following: 
 *   number representations must be normalized such that 
 *     . there is a single digit, which is non-zero, to the left of the decimal point and
 *     . at least a single digit to the right of the decimal point.
 *     . unless the value being represented is zero. 
 *       The canonical representation for zero is 0.0E0
 *
 ***/     
XMLCh* XMLAbstractDoubleFloat::getCanonicalRepresentation(const XMLCh*         const rawData
                                                        ,       MemoryManager* const memMgr)
{
    // before anything, let's look for special tokens since that
    // breaks the calls to parse below.
    if(XMLString::equals(rawData, XMLUni::fgNegINFString) || 
       XMLString::equals(rawData, XMLUni::fgPosINFString) || 
       XMLString::equals(rawData, XMLUni::fgNaNString)     )
    {
        return XMLString::replicate(rawData, memMgr);
    }

    try 
    {
        int    strLen = XMLString::stringLen(rawData);
        XMLCh* manStr = (XMLCh*) memMgr->allocate((strLen + 1) * sizeof(XMLCh));
        ArrayJanitor<XMLCh> janManStr(manStr, memMgr);
        XMLCh* manBuf = (XMLCh*) memMgr->allocate((strLen + 1) * sizeof(XMLCh));
        ArrayJanitor<XMLCh> janManBuf(manBuf, memMgr);
        XMLCh* expStr = (XMLCh*) memMgr->allocate((strLen + 1) * sizeof(XMLCh));
        ArrayJanitor<XMLCh> janExpStr(expStr, memMgr);
        XMLCh* retBuffer = (XMLCh*) memMgr->allocate((strLen + 8) * sizeof(XMLCh));
        ArrayJanitor<XMLCh> janRetBuffer(retBuffer, memMgr);
        retBuffer[0] = 0;

        int sign, totalDigits, fractDigits, expValue = 0;

        const XMLCh* ePosition = XMLString::findAny(rawData, expSign);

        /***
         *  parse mantissa and exp separately
        ***/
        if (!ePosition)
        {
            XMLBigDecimal::parseDecimal(rawData, manBuf, sign, totalDigits, fractDigits, memMgr);
            expValue = 0;
        }
        else
        {
            int    manLen = ePosition - rawData;
            XMLString::copyNString(manStr, rawData, manLen);
            *(manStr + manLen) = chNull;
            XMLBigDecimal::parseDecimal(manStr, manBuf, sign, totalDigits, fractDigits, memMgr);

            int    expLen = strLen - manLen - 1;
            ePosition++;
            XMLString::copyNString(expStr, ePosition, expLen);
            *(expStr + expLen) = chNull;
            expValue = XMLString::parseInt(expStr); 
        }

        if ( (sign == 0) || (totalDigits == 0) )
        {
            retBuffer[0] = chDigit_0;
            retBuffer[1] = chPeriod;
            retBuffer[2] = chDigit_0;
            retBuffer[3] = chLatin_E;
            retBuffer[4] = chDigit_0;
            retBuffer[5] = chNull;
        }
        else
        {
            XMLCh* retPtr = retBuffer;

            if (sign == -1)
            {
                *retPtr++ = chDash;
            }

            *retPtr++ = manBuf[0];
            *retPtr++ = chPeriod;

            //XMLBigDecimal::parseDecimal() will eliminate trailing zeros
            // iff there is a decimal points
            // eg. 56.7800e0  -> manBuf = 5678, totalDigits = 4, fractDigits = 2
            // we print it as 5.678e1
            //
            // but it wont remove trailing zeros if there is no decimal point.
            // eg.  567800e0 -> manBuf = 567800, totalDigits = 6, fractDigits = 0
            // we print it 5.67800e5
            //
            // for the latter, we need to print it as 5.678e5 instead
            //
            XMLCh* endPtr = manBuf + totalDigits;

            if (fractDigits == 0)
            {
                while(*(endPtr - 1) == chDigit_0)
                    endPtr--;
            }

            int remainLen = endPtr - &(manBuf[1]);

            if (remainLen)
            {
                XMLString::copyNString(retPtr, &(manBuf[1]), remainLen);
                retPtr += remainLen;
            }
            else
            {
                *retPtr++ = chDigit_0;
            }

            /***
             * 
             *  . adjust expValue
             *   
             *  new_fractDigits = totalDigits - 1  
             *  new_expValue = old_expValue + (new_fractDigits - fractDigits)
             *
             ***/
            expValue += (totalDigits - 1) - fractDigits ;
            XMLString::binToText(expValue, expStr, strLen, 10, memMgr);
            *retPtr++  = chLatin_E;
            *retPtr = chNull;

            XMLString::catString(&(retBuffer[0]), expStr);
        }

        janRetBuffer.release();
        return retBuffer;

    } //try

    catch (...)
    {
        return 0;
    }
}        

/***
 * Support for Serialization/De-serialization
 ***/

IMPL_XSERIALIZABLE_NOCREATE(XMLAbstractDoubleFloat)

void XMLAbstractDoubleFloat::serialize(XSerializeEngine& serEng)
{
    //REVISIT: may not need to call base since it does nothing
    XMLNumber::serialize(serEng);

    if (serEng.isStoring())
    {
        serEng << fValue;
        serEng << fType;
        serEng << fDataConverted;
        serEng << fDataOverflowed;
        serEng << fSign;

        serEng.writeString(fRawData);

        // Do not serialize fFormattedString

    }
    else
    {
        serEng >> fValue;

        int type = 0;
        serEng >> type;
        fType = (LiteralType) type;

        serEng >> fDataConverted;
        serEng >> fDataOverflowed;
        serEng >> fSign;

        serEng.readString(fRawData);

        // Set it to 0 force it to re-format if needed
        fFormattedString = 0;

    }

}

XERCES_CPP_NAMESPACE_END
