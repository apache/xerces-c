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
 * Revision 1.16  2001/10/01 16:16:38  peiyongz
 * DTV Reorganization:derived from AbstractNumericValidator
 *
 * Revision 1.15  2001/09/20 13:11:42  knoaman
 * Regx  + misc. fixes
 *
 * Revision 1.14  2001/08/22 18:28:01  peiyongz
 * Memory leak: delete the bufffer allocated by ::toString()
 *
 * Revision 1.13  2001/08/21 18:42:53  peiyongz
 * Bugzilla# 2816: cleanUp() declared with external linkage and called
 *                          before defined as inline
 *
 * Revision 1.12  2001/08/15 18:08:44  peiyongz
 * Fix to potential leakage in strEnumeration
 *
 * Revision 1.11  2001/07/17 21:30:12  peiyongz
 * bug fix: numBase->getMinInclusive()->toString()
 *
 * Revision 1.10  2001/06/20 17:56:32  peiyongz
 * support for "fixed" option on constrainning facets
 *
 * Revision 1.8  2001/05/29 19:49:34  tng
 * Schema: Constraint Checking Fix in datatypeValidators.  By Pei Yong Zhang.
 *
 * Revision 1.7  2001/05/28 21:11:18  tng
 * Schema: Various DatatypeValidator fix.  By Pei Yong Zhang
 *
 * Revision 1.6  2001/05/18 20:18:02  tng
 * Schema: More exception messages in XMLBigDecimal/XMLBigInteger/DecimalDatatypeValidator.  By Pei Yong Zhang.
 *
 * Revision 1.5  2001/05/18 13:36:45  tng
 * Schema: Catch RegularExpression exception and NumberFormatException
 *
 * Revision 1.4  2001/05/18 13:23:46  tng
 * Schema: Exception messages in DatatypeValidator.  By Pei Yong Zhang.
 *
 * Revision 1.3  2001/05/11 17:17:23  tng
 * Schema: DatatypeValidator fixes.  By Pei Yong Zhang.
 *
 * Revision 1.2  2001/05/11 13:27:28  tng
 * Copyright update.
 *
 * Revision 1.1  2001/05/10 20:51:29  tng
 * Schema: Add DecimalDatatypeValidator and XMLBigDecimal, XMLBigInteger.  By Pei Yong Zhang.
 *
 */

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <validators/datatype/DecimalDatatypeValidator.hpp>
#include <validators/schema/SchemaSymbols.hpp>
#include <validators/datatype/InvalidDatatypeFacetException.hpp>
#include <validators/datatype/InvalidDatatypeValueException.hpp>
#include <util/NumberFormatException.hpp>

static const int BUF_LEN = 64;
static XMLCh value1[BUF_LEN+1];
static XMLCh value2[BUF_LEN+1];

// ---------------------------------------------------------------------------
//  Constructors and Destructor
// ---------------------------------------------------------------------------
DecimalDatatypeValidator::DecimalDatatypeValidator()
:AbstractNumericValidator(0, 0, 0, DatatypeValidator::Decimal)
, fTotalDigits(0)
, fFractionDigits(0)
{}

DecimalDatatypeValidator::DecimalDatatypeValidator(
                          DatatypeValidator*            const baseValidator
                        , RefHashTableOf<KVStringPair>* const facets
                        , RefVectorOf<XMLCh>*           const enums
                        , const int                           finalSet)
:AbstractNumericValidator(baseValidator, facets, finalSet, DatatypeValidator::Decimal)
, fTotalDigits(0)
, fFractionDigits(0)
{
    init(enums);
}

DecimalDatatypeValidator::~DecimalDatatypeValidator()
{}

// -----------------------------------------------------------------------
// Compare methods
// -----------------------------------------------------------------------
int DecimalDatatypeValidator::compare(const XMLCh* const lValue
                                           , const XMLCh* const rValue)
{
    return XMLBigDecimal::compareValues(new XMLBigDecimal(lValue)
                                      , new XMLBigDecimal(rValue));
}

DatatypeValidator* DecimalDatatypeValidator::newInstance(
                                      RefHashTableOf<KVStringPair>* const facets
                                    , RefVectorOf<XMLCh>*           const enums
                                    , const int                           finalSet)
{
    return (DatatypeValidator*) new DecimalDatatypeValidator(this, facets, enums, finalSet);
}

// -----------------------------------------------------------------------
// ctor provided to be used by derived classes
// -----------------------------------------------------------------------
DecimalDatatypeValidator::DecimalDatatypeValidator(DatatypeValidator*            const baseValidator
                                                 , RefHashTableOf<KVStringPair>* const facets
                                                 , const int                           finalSet
                                                 , const ValidatorType                 type)
:AbstractNumericValidator(baseValidator, facets, finalSet, type)
, fTotalDigits(0)
, fFractionDigits(0)
{
    //do not invoke init here !!!
}

void DecimalDatatypeValidator::assignAdditionalFacet(const XMLCh* const key
                                                   , const XMLCh* const value)
{
    if (XMLString::compareString(key, SchemaSymbols::fgELT_TOTALDIGITS)==0)
    {
        int val;
        try
        {
            val = XMLString::parseInt(value);
        }
        catch (NumberFormatException)
        {
            ThrowXML1(InvalidDatatypeFacetException, XMLExcepts::FACET_Invalid_TotalDigit, value);
        }

        // check 4.3.11.c0 must: totalDigits > 0
        if ( val <= 0 )
            ThrowXML1(InvalidDatatypeFacetException, XMLExcepts::FACET_NonNeg_TotalDigit, value);

        setTotalDigits(val);
        setFacetsDefined(DatatypeValidator::FACET_TOTALDIGITS);
    }
    else if (XMLString::compareString(key, SchemaSymbols::fgELT_FRACTIONDIGITS)==0)
    {
        int val;
        try
        {
            val = XMLString::parseInt(value);
        }
        catch (NumberFormatException)
        {
            ThrowXML1(InvalidDatatypeFacetException, XMLExcepts::FACET_Invalid_FractDigit, value);
        }

        // check 4.3.12.c0 must: fractionDigits > 0
        if ( val < 0 )
            ThrowXML1(InvalidDatatypeFacetException, XMLExcepts::FACET_NonNeg_FractDigit, value);

        setFractionDigits(val);
        setFacetsDefined(DatatypeValidator::FACET_FRACTIONDIGITS);
    }
    else
    {
        ThrowXML(InvalidDatatypeFacetException, XMLExcepts::FACET_Invalid_Tag);
    }
}

void DecimalDatatypeValidator::inheritAdditionalFacet()
{

    DecimalDatatypeValidator *numBase = (DecimalDatatypeValidator*) getBaseValidator();

    if (!numBase)
        return;

    // inherit totalDigits         
    if ((( numBase->getFacetsDefined() & DatatypeValidator::FACET_TOTALDIGITS) != 0) &&          
        (( getFacetsDefined() & DatatypeValidator::FACET_TOTALDIGITS) == 0) )              
    {          
        setTotalDigits(numBase->fTotalDigits);              
        setFacetsDefined(DatatypeValidator::FACET_TOTALDIGITS);              
    }
          
    // inherit fractionDigits          
    if ((( numBase->getFacetsDefined() & DatatypeValidator::FACET_FRACTIONDIGITS) != 0) &&          
        (( getFacetsDefined() & DatatypeValidator::FACET_FRACTIONDIGITS) == 0) )              
    {          
        setFractionDigits(numBase->fFractionDigits);              
        setFacetsDefined(DatatypeValidator::FACET_FRACTIONDIGITS);              
    }
}

void DecimalDatatypeValidator::checkAdditionalFacetConstraints() const
{
    // check 4.3.12.c1 must: fractionDigits <= totalDigits
    if ( ((getFacetsDefined() & DatatypeValidator::FACET_FRACTIONDIGITS) != 0) &&
        ((getFacetsDefined() & DatatypeValidator::FACET_TOTALDIGITS) != 0) )
    {
        if ( fFractionDigits > fTotalDigits )
        {
            XMLString::binToText(getFractionDigits(), value1, BUF_LEN, 10);
            XMLString::binToText(getTotalDigits(), value2, BUF_LEN, 10);
            ThrowXML2(InvalidDatatypeFacetException
                                 , XMLExcepts::FACET_TotDigit_FractDigit
                                 , value2
                                 , value1);
        }
    }

}

void DecimalDatatypeValidator::checkAdditionalFacetConstraintsBase() const
{

    DecimalDatatypeValidator *numBase = (DecimalDatatypeValidator*) getBaseValidator();

    if (!numBase)
        return;

    int thisFacetsDefined = getFacetsDefined();
    int baseFacetsDefined = numBase->getFacetsDefined();

    // check 4.3.11.c1 error: totalDigits > base.totalDigits
    // totalDigits != base.totalDigits if (base.fixed)
    if (( thisFacetsDefined & DatatypeValidator::FACET_TOTALDIGITS) != 0)
    {
        if ( (( baseFacetsDefined & DatatypeValidator::FACET_TOTALDIGITS) != 0) &&
            ( fTotalDigits > numBase->fTotalDigits ))
        {
            XMLString::binToText(fTotalDigits, value1, BUF_LEN, 10);
            XMLString::binToText(numBase->fTotalDigits, value2, BUF_LEN, 10);
            ThrowXML2(InvalidDatatypeFacetException
                                 , XMLExcepts::FACET_totalDigit_base_totalDigit
                                 , value1
                                 , value2);
        }

        if ( (( baseFacetsDefined & DatatypeValidator::FACET_TOTALDIGITS) != 0) &&
            (( numBase->getFixed() & DatatypeValidator::FACET_TOTALDIGITS) != 0) &&
            ( fTotalDigits != numBase->fTotalDigits ))
        {
            XMLString::binToText(fTotalDigits, value1, BUF_LEN, 10);
            XMLString::binToText(numBase->fTotalDigits, value2, BUF_LEN, 10);
            ThrowXML2(InvalidDatatypeFacetException
                                 , XMLExcepts::FACET_totalDigit_base_fixed
                                 , value1
                                 , value2);
        }                    
    }

    if (( thisFacetsDefined & DatatypeValidator::FACET_FRACTIONDIGITS) != 0)
    {
        // check question error: fractionDigits > base.fractionDigits ???
        if ( (( baseFacetsDefined & DatatypeValidator::FACET_FRACTIONDIGITS) != 0) &&
            ( fFractionDigits > numBase->fFractionDigits ))
        {
            XMLString::binToText(fFractionDigits, value1, BUF_LEN, 10);
            XMLString::binToText(numBase->fFractionDigits, value2, BUF_LEN, 10);
            ThrowXML2(InvalidDatatypeFacetException
                                 , XMLExcepts::FACET_fractDigit_base_fractDigit
                                 , value1
                                 , value2);
                        }

        // check question error: fractionDigits > base.totalDigits ???
        if ( (( baseFacetsDefined & DatatypeValidator::FACET_TOTALDIGITS) != 0) &&
            ( fFractionDigits > numBase->fTotalDigits ))
        {
            XMLString::binToText(fFractionDigits, value1, BUF_LEN, 10);
            XMLString::binToText(numBase->fTotalDigits, value2, BUF_LEN, 10);
            ThrowXML2(InvalidDatatypeFacetException
                                 , XMLExcepts::FACET_fractDigit_base_totalDigit
                                 , value1
                                 , value2);
        }

        // fractionDigits != base.fractionDigits if (base.fixed)
        if ( (( baseFacetsDefined & DatatypeValidator::FACET_FRACTIONDIGITS) != 0) &&
            (( numBase->getFixed() & DatatypeValidator::FACET_FRACTIONDIGITS) != 0) &&
            ( fFractionDigits != numBase->fFractionDigits ))
        {
            XMLString::binToText(fFractionDigits, value1, BUF_LEN, 10);
            XMLString::binToText(numBase->fFractionDigits, value2, BUF_LEN, 10);
            ThrowXML2(InvalidDatatypeFacetException
                                 , XMLExcepts::FACET_fractDigit_base_fixed
                                 , value1
                                 , value2);
        }
    }

}

int  DecimalDatatypeValidator::compareValues(const XMLNumber* const lValue
                                           , const XMLNumber* const rValue)
{
    return XMLBigDecimal::compareValues((XMLBigDecimal*) lValue, (XMLBigDecimal*) rValue);
}

void  DecimalDatatypeValidator::setMaxInclusive(const XMLCh* const value)
{
    fMaxInclusive = new XMLBigDecimal(value);
}

void  DecimalDatatypeValidator::setMaxExclusive(const XMLCh* const value)
{
    fMaxExclusive = new XMLBigDecimal(value);
}

void  DecimalDatatypeValidator::setMinInclusive(const XMLCh* const value)
{
    fMinInclusive = new XMLBigDecimal(value);
}

void  DecimalDatatypeValidator::setMinExclusive(const XMLCh* const value)
{
    fMinExclusive = new XMLBigDecimal(value);
}

void DecimalDatatypeValidator::setEnumeration()
{
    // check 4.3.5.c0 must: enumeration values from the value space of base
    //
    // 1. shall be from base value space
    // 2. shall be from current value space as well
    //    ( **shall go through boundsCheck() )
    //
    if (!fStrEnumeration)
        return;

    int i = 0;
    int enumLength = fStrEnumeration->size();

    DecimalDatatypeValidator *numBase = (DecimalDatatypeValidator*) getBaseValidator();
    if (numBase)
    {
        try
        {
            for ( i = 0; i < enumLength; i++)
            {
                numBase->checkContent(fStrEnumeration->elementAt(i), false);
            }
        }              
        catch (XMLException&)
        {
            ThrowXML1(InvalidDatatypeFacetException
                , XMLExcepts::FACET_enum_base
                , fStrEnumeration->elementAt(i));

        }
    }

    // We put the this->checkContent in a separate loop
    // to not block original message with in that method.
    // 
    for ( i = 0; i < enumLength; i++)
    {
        checkContent(fStrEnumeration->elementAt(i), false);
    }              

    fEnumeration = new RefVectorOf<XMLNumber>(enumLength, true);
    fEnumerationInherited = false;

    for ( i = 0; i < enumLength; i++)
    {
        fEnumeration->insertElementAt(new XMLBigDecimal(fStrEnumeration->elementAt(i)), i);
    }              

}

void DecimalDatatypeValidator::checkContent( const XMLCh* const content, bool asBase)
{

    //validate against base validator if any
    DecimalDatatypeValidator *pBase = (DecimalDatatypeValidator*) this->getBaseValidator();
    if (pBase)
        pBase->checkContent(content, true);

    // we check pattern first
    if ( (getFacetsDefined() & DatatypeValidator::FACET_PATTERN ) != 0 )
    {
        // lazy construction
        if (getRegex() ==0) {
            try {
                setRegex(new RegularExpression(getPattern(), SchemaSymbols::fgRegEx_XOption));
            }
            catch (XMLException &e)
            {
                ThrowXML1(InvalidDatatypeValueException, XMLExcepts::RethrowError, e.getMessage());
            }
        }

        if (getRegex()->matches(content) ==false)
        {
            ThrowXML2(InvalidDatatypeValueException
                    , XMLExcepts::VALUE_NotMatch_Pattern
                    , content
                    , getPattern());
        }
    }

    // if this is a base validator, we only need to check pattern facet
    // all other facet were inherited by the derived type
    if (asBase)
        return;

    try {
        XMLBigDecimal theValue(content);
        XMLBigDecimal *theData = &theValue;

        if (getEnumeration())
        {
            int i=0;
            int enumLength = getEnumeration()->size();
            for ( ; i < enumLength; i++)
            {
                if (XMLBigDecimal::compareValues(theData, (XMLBigDecimal*) getEnumeration()->elementAt(i))==0)
                    break;
            }

            if (i == enumLength)
                ThrowXML1(InvalidDatatypeValueException, XMLExcepts::VALUE_NotIn_Enumeration, content);
        }

        boundsCheck(theData);

        if ( (getFacetsDefined() & DatatypeValidator::FACET_FRACTIONDIGITS) != 0 )
        {
            if ( theData->getScale() > fFractionDigits )
            {
                XMLCh* value = theData->toString();
                ArrayJanitor<XMLCh> jan(value);
                XMLString::binToText(theData->getScale(), value1, BUF_LEN, 10);
                XMLString::binToText(fFractionDigits, value2, BUF_LEN, 10);
                ThrowXML3(InvalidDatatypeFacetException
                                 , XMLExcepts::VALUE_exceed_fractDigit
                                 , value
                                 , value1
                                 , value2);
            }
        }

        if ( (getFacetsDefined() & DatatypeValidator::FACET_TOTALDIGITS) != 0 )
        {
            if ( theData->getTotalDigit() > fTotalDigits )
            {
                XMLCh* value = theData->toString();
                ArrayJanitor<XMLCh> jan(value);
                XMLString::binToText(theData->getTotalDigit(), value1, BUF_LEN, 10);
                XMLString::binToText(fTotalDigits, value2, BUF_LEN, 10);
                ThrowXML3(InvalidDatatypeFacetException
                                 , XMLExcepts::VALUE_exceed_totalDigit
                                 , value
                                 , value1
                                 , value2);
            }
        }

    }
    catch (XMLException &e)
    {
       ThrowXML1(InvalidDatatypeFacetException, XMLExcepts::RethrowError, e.getMessage());
    }

}



// ---------------------------------------------------------------------------
//  Whitespace handling methods
// ---------------------------------------------------------------------------


/**
  * End of file DecimalDatatypeValidator::cpp
  */

