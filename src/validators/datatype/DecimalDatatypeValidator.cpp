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
DecimalDatatypeValidator::DecimalDatatypeValidator(
                          DatatypeValidator*            const baseValidator
                        , RefHashTableOf<KVStringPair>* const facets
                        , RefVectorOf<XMLCh>*           const enums
                        , const int                           finalSet)
:DatatypeValidator(baseValidator, facets, finalSet, DatatypeValidator::Decimal)
, fTotalDigits(0)
, fFractionDigits(0)
, fEnumerationInherited(false)
, fMaxInclusive(0)
, fMaxExclusive(0)
, fMinInclusive(0)
, fMinExclusive(0)
, fEnumeration(0)
{
    try
    {
        init(baseValidator, facets, enums);
    }

    catch (XMLException&)
    {
        cleanUp();
        throw;
    }

}

void DecimalDatatypeValidator::init(DatatypeValidator*            const baseValidator
                                  , RefHashTableOf<KVStringPair>* const facets
                                  , RefVectorOf<XMLCh>*           const enums)
{
    // Set Facets if any defined
    if (facets)
    {
        XMLCh* key;
        XMLCh* value;
        RefVectorOf<XMLCh>*             fStrEnumeration = enums; // save the literal value
        Janitor<RefVectorOf<XMLCh> >    janStrEnum(fStrEnumeration);

        if (enums)
        {
            setFacetsDefined(DatatypeValidator::FACET_ENUMERATION);
        }

        RefHashTableOfEnumerator<KVStringPair> e(facets);

        while (e.hasMoreElements())
        {
            KVStringPair pair = e.nextElement();
            key = pair.getKey();
            value = pair.getValue();

            if (XMLString::compareString(key, SchemaSymbols::fgELT_PATTERN)==0)
            {
                setPattern(value);
                if (getPattern())
                    setFacetsDefined(DatatypeValidator::FACET_PATTERN);
                // do not construct regex until needed
            }
            else if (XMLString::compareString(key, SchemaSymbols::fgELT_MAXINCLUSIVE)==0)
            {
                try
                {
                    setMaxInclusive(new XMLBigDecimal(value));
                }
                catch (NumberFormatException)
                {
                    ThrowXML1(InvalidDatatypeFacetException, XMLExcepts::FACET_Invalid_MaxIncl, value);
                }
                setFacetsDefined(DatatypeValidator::FACET_MAXINCLUSIVE);
            }
            else if (XMLString::compareString(key, SchemaSymbols::fgELT_MAXEXCLUSIVE)==0)
            {
                try
                {
                     setMaxExclusive(new XMLBigDecimal(value));
                }
                catch (NumberFormatException)
                {
                    ThrowXML1(InvalidDatatypeFacetException, XMLExcepts::FACET_Invalid_MaxExcl, value);
                }
                setFacetsDefined(DatatypeValidator::FACET_MAXEXCLUSIVE);
            }
            else if (XMLString::compareString(key, SchemaSymbols::fgELT_MININCLUSIVE)==0)
            {
                try
                {
                    setMinInclusive(new XMLBigDecimal(value));
                }
                catch (NumberFormatException)
                {
                    ThrowXML1(InvalidDatatypeFacetException, XMLExcepts::FACET_Invalid_MinIncl, value);
                }
                setFacetsDefined(DatatypeValidator::FACET_MININCLUSIVE);
            }
            else if (XMLString::compareString(key, SchemaSymbols::fgELT_MINEXCLUSIVE)==0)
            {
                try
                {
                    setMinExclusive(new XMLBigDecimal(value));
                }
                catch (NumberFormatException)
                {
                    ThrowXML1(InvalidDatatypeFacetException, XMLExcepts::FACET_Invalid_MinExcl, value);
                }
                setFacetsDefined(DatatypeValidator::FACET_MINEXCLUSIVE);
            }
            else if (XMLString::compareString(key, SchemaSymbols::fgELT_TOTALDIGITS)==0)
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
            else if (XMLString::compareString(key, SchemaSymbols::fgATT_FIXED)==0)
            {
                unsigned int val;
                bool         retStatus;
                try
                {
                     retStatus = XMLString::textToBin(value, val);
                }
                catch (RuntimeException)
                {
                    ThrowXML(InvalidDatatypeFacetException, XMLExcepts::FACET_internalError_fixed);
                }

                if (!retStatus)
                {
                    ThrowXML(InvalidDatatypeFacetException, XMLExcepts::FACET_internalError_fixed);
                }

                setFixed(val);
                //no setFacetsDefined here
            }
            else
            {
                 ThrowXML(InvalidDatatypeFacetException, XMLExcepts::FACET_Invalid_Tag);
            }
        }//while

        /***
           Schema constraint: Part I -- self checking
        ***/

        try {

            if ( getFacetsDefined() != 0 )
            {
                // non co-existence checking
                // check 4.3.8.c1 error: maxInclusive + maxExclusive
                if ( ((getFacetsDefined() & DatatypeValidator::FACET_MAXEXCLUSIVE) != 0) &&
                     ((getFacetsDefined() & DatatypeValidator::FACET_MAXINCLUSIVE) != 0) )
                     ThrowXML(InvalidDatatypeFacetException, XMLExcepts::FACET_max_Incl_Excl);

                // non co-existence checking
                // check 4.3.9.c1 error: minInclusive + minExclusive
                if ( ((getFacetsDefined() & DatatypeValidator::FACET_MINEXCLUSIVE) != 0) &&
                     ((getFacetsDefined() & DatatypeValidator::FACET_MININCLUSIVE) != 0) )
                     ThrowXML(InvalidDatatypeFacetException, XMLExcepts::FACET_min_Incl_Excl);

                //
                // minExclusive < minInclusive <= maxInclusive < maxExclusive
                //
                // check 4.3.7.c1 must: minInclusive <= maxInclusive
                if ( ((getFacetsDefined() & DatatypeValidator::FACET_MAXINCLUSIVE) != 0) &&
                     ((getFacetsDefined() & DatatypeValidator::FACET_MININCLUSIVE) != 0) )
                {
                    if ( XMLBigDecimal::compareValues(getMinInclusive(), getMaxInclusive()) == 1 )
                    {
                        XMLCh* value1 = getMinInclusive()->toString();
                        ArrayJanitor<XMLCh> jan1(value1);
                        XMLCh* value2 = getMaxInclusive()->toString();
                        ArrayJanitor<XMLCh> jan2(value2);
                        ThrowXML2(InvalidDatatypeFacetException
                                , XMLExcepts::FACET_maxIncl_minIncl
                                , value2
                                , value1);
                    }
                }

                // check 4.3.8.c2 must: minExclusive <= maxExclusive ??? minExclusive < maxExclusive
                if ( ((getFacetsDefined() & DatatypeValidator::FACET_MAXEXCLUSIVE) != 0) &&
                     ((getFacetsDefined() & DatatypeValidator::FACET_MINEXCLUSIVE) != 0) )
                {
                    if ( XMLBigDecimal::compareValues(getMinExclusive(), getMaxExclusive()) == 1 )
                    {
                        XMLCh* value1 = getMinExclusive()->toString();
                        ArrayJanitor<XMLCh> jan1(value1);
                        XMLCh* value2 = getMaxExclusive()->toString();
                        ArrayJanitor<XMLCh> jan2(value2);
                        ThrowXML2(InvalidDatatypeFacetException
                                , XMLExcepts::FACET_maxExcl_minExcl
                                , value2
                                , value1);
                    }
                }

                // check 4.3.9.c2 must: minExclusive < maxInclusive
                if ( ((getFacetsDefined() & DatatypeValidator::FACET_MAXINCLUSIVE) != 0) &&
                     ((getFacetsDefined() & DatatypeValidator::FACET_MINEXCLUSIVE) != 0) )
                {
                    if ( XMLBigDecimal::compareValues(getMinExclusive(), getMaxInclusive()) != -1 )
                    {
                        XMLCh* value1 = getMinExclusive()->toString();
                        ArrayJanitor<XMLCh> jan1(value1);
                        XMLCh* value2 = getMaxInclusive()->toString();
                        ArrayJanitor<XMLCh> jan2(value2);
                        ThrowXML2(InvalidDatatypeFacetException
                                , XMLExcepts::FACET_maxIncl_minExcl
                                , value2
                                , value1);
                    }
                }

                // check 4.3.10.c1 must: minInclusive < maxExclusive
                if ( ((getFacetsDefined() & DatatypeValidator::FACET_MAXEXCLUSIVE) != 0) &&
                     ((getFacetsDefined() & DatatypeValidator::FACET_MININCLUSIVE) != 0) )
                {
                    if ( XMLBigDecimal::compareValues(getMinInclusive(), getMaxExclusive()) != -1 )
                    {
                        XMLCh* value1 = getMinInclusive()->toString();
                        ArrayJanitor<XMLCh> jan1(value1);
                        XMLCh* value2 = getMaxExclusive()->toString();
                        ArrayJanitor<XMLCh> jan2(value2);
                        ThrowXML2(InvalidDatatypeFacetException
                                , XMLExcepts::FACET_maxExcl_minIncl
                                , value2
                                , value1);
                    }
                 }

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

            } // if getFacetsDefined

            /***
               Schema constraint: Part II -- self vs base
            ***/

            if ( baseValidator != 0 )
            {
                DecimalDatatypeValidator* numBase = (DecimalDatatypeValidator*)baseValidator;

                //                                     this
                //                 minExclusive                          maxExclusive
                //                    minInclusive                  maxInclusive
                //
                //                                     base
                //  minExclusive                                                          maxExclusive
                //      minInclusive                                                   maxExclusive
                //
                if ( getFacetsDefined() != 0 )
                {
                    // check 4.3.7.c2 error:
                    // maxInclusive > base.maxInclusive
                    // maxInclusive >= base.maxExclusive
                    // maxInclusive < base.minInclusive
                    // maxInclusive <= base.minExclusive
                    // maxInclusive != base.maxInclusive if (base.fixed)
                    if ( ((getFacetsDefined() & DatatypeValidator::FACET_MAXINCLUSIVE) != 0) )
                    {
                        if ( ((numBase->getFacetsDefined() & DatatypeValidator::FACET_MAXINCLUSIVE) != 0) &&
                             ( XMLBigDecimal::compareValues(getMaxInclusive(), numBase->getMaxInclusive()) == 1 ))
                        {
                            XMLCh* value1 = getMaxInclusive()->toString();
                            ArrayJanitor<XMLCh> jan1(value1);
                            XMLCh* value2 = numBase->getMaxInclusive()->toString();
                            ArrayJanitor<XMLCh> jan2(value2);
                            ThrowXML2(InvalidDatatypeFacetException
                                , XMLExcepts::FACET_maxIncl_base_maxIncl
                                , value1
                                , value2);
                        }

                        if ( ((numBase->getFacetsDefined() & DatatypeValidator::FACET_MAXEXCLUSIVE) != 0) &&
                             ( XMLBigDecimal::compareValues(getMaxInclusive(), numBase->getMaxExclusive()) != -1 ))
                        {
                            XMLCh* value1 = getMaxInclusive()->toString();
                            ArrayJanitor<XMLCh> jan1(value1);
                            XMLCh* value2 = numBase->getMaxExclusive()->toString();
                            ArrayJanitor<XMLCh> jan2(value2);
                            ThrowXML2(InvalidDatatypeFacetException
                                , XMLExcepts::FACET_maxIncl_base_maxExcl
                                , value1
                                , value2);
                        }

                        if ( (( numBase->getFacetsDefined() & DatatypeValidator::FACET_MININCLUSIVE) != 0) &&
                             ( XMLBigDecimal::compareValues(getMaxInclusive(), numBase->getMinInclusive()) == -1 ))
                        {
                            XMLCh* value1 = getMaxInclusive()->toString();
                            ArrayJanitor<XMLCh> jan1(value1);
                            XMLCh* value2 = numBase->getMinInclusive()->toString();
                            ArrayJanitor<XMLCh> jan2(value2);
                            ThrowXML2(InvalidDatatypeFacetException
                                , XMLExcepts::FACET_maxIncl_base_minIncl
                                , value1
                                , value2);
                        }

                        if ( (( numBase->getFacetsDefined() & DatatypeValidator::FACET_MINEXCLUSIVE) != 0) &&
                             ( XMLBigDecimal::compareValues(getMaxInclusive(), numBase->getMinExclusive() ) != 1 ))
                        {
                            XMLCh* value1 = getMaxInclusive()->toString();
                            ArrayJanitor<XMLCh> jan1(value1);
                            XMLCh* value2 = numBase->getMinExclusive()->toString();
                            ArrayJanitor<XMLCh> jan2(value2);
                            ThrowXML2(InvalidDatatypeFacetException
                                , XMLExcepts::FACET_maxIncl_base_minExcl
                                , value1
                                , value2);
                        }

                        if ( ((numBase->getFacetsDefined() & DatatypeValidator::FACET_MAXINCLUSIVE) != 0) &&
                             ((numBase->getFixed() & DatatypeValidator::FACET_MAXINCLUSIVE) != 0) &&
                             ( XMLBigDecimal::compareValues(getMaxInclusive(), numBase->getMaxInclusive()) != 0 ))
                        {
                            XMLCh* value1 = getMaxInclusive()->toString();
                            ArrayJanitor<XMLCh> jan1(value1);
                            XMLCh* value2 = numBase->getMaxInclusive()->toString();
                            ArrayJanitor<XMLCh> jan2(value2);
                            ThrowXML2(InvalidDatatypeFacetException
                                , XMLExcepts::FACET_maxIncl_base_fixed
                                , value1
                                , value2);
                        }
                    }

                    // check 4.3.8.c3 error:
                    // maxExclusive > base.maxExclusive
                    // maxExclusive > base.maxInclusive
                    // maxExclusive <= base.minInclusive
                    // maxExclusive <= base.minExclusive
                    // maxExclusive != base.maxExclusive if (base.fixed)
                    if ( ((getFacetsDefined() & DatatypeValidator::FACET_MAXEXCLUSIVE) != 0) )
                    {
                        if ( (( numBase->getFacetsDefined() & DatatypeValidator::FACET_MAXEXCLUSIVE) != 0) &&
                             ( XMLBigDecimal::compareValues(getMaxExclusive(), numBase->getMaxExclusive()) == 1 ))
                        {
                            XMLCh* value1 = getMaxExclusive()->toString();
                            ArrayJanitor<XMLCh> jan1(value1);
                            XMLCh* value2 = numBase->getMaxExclusive()->toString();
                            ArrayJanitor<XMLCh> jan2(value2);
                            ThrowXML2(InvalidDatatypeFacetException
                                , XMLExcepts::FACET_maxExcl_base_maxExcl
                                , value1
                                , value2);
                        }

                        if ( (( numBase->getFacetsDefined() & DatatypeValidator::FACET_MAXINCLUSIVE) != 0) &&
                             ( XMLBigDecimal::compareValues(getMaxExclusive(), numBase->getMaxInclusive()) == 1 ))
                        {
                            XMLCh* value1 = getMaxExclusive()->toString();
                            ArrayJanitor<XMLCh> jan1(value1);
                            XMLCh* value2 = numBase->getMaxInclusive()->toString();
                            ArrayJanitor<XMLCh> jan2(value2);
                            ThrowXML2(InvalidDatatypeFacetException
                                , XMLExcepts::FACET_maxExcl_base_maxIncl
                                , value1
                                , value2);
                        }

                        if ( (( numBase->getFacetsDefined() & DatatypeValidator::FACET_MINEXCLUSIVE) != 0) &&
                             ( XMLBigDecimal::compareValues(getMaxExclusive(), numBase->getMinExclusive() ) != 1 ))
                        {
                            XMLCh* value1 = getMaxExclusive()->toString();
                            ArrayJanitor<XMLCh> jan1(value1);
                            XMLCh* value2 = numBase->getMinExclusive()->toString();
                            ArrayJanitor<XMLCh> jan2(value2);
                            ThrowXML2(InvalidDatatypeFacetException
                                , XMLExcepts::FACET_maxExcl_base_minExcl
                                , value1
                                , value2);
                        }

                        if ( (( numBase->getFacetsDefined() & DatatypeValidator::FACET_MININCLUSIVE) != 0) &&
                             ( XMLBigDecimal::compareValues(getMaxExclusive(), numBase->getMinInclusive()) != 1 ))
                        {
                            XMLCh* value1 = getMaxExclusive()->toString();
                            ArrayJanitor<XMLCh> jan1(value1);
                            XMLCh* value2 = numBase->getMinInclusive()->toString();
                            ArrayJanitor<XMLCh> jan2(value2);
                            ThrowXML2(InvalidDatatypeFacetException
                                , XMLExcepts::FACET_maxExcl_base_minExcl
                                , value1
                                , value2);
                        }

                        if ( (( numBase->getFacetsDefined() & DatatypeValidator::FACET_MAXEXCLUSIVE) != 0) &&
                             (( numBase->getFixed() & DatatypeValidator::FACET_MAXEXCLUSIVE) != 0) &&
                             ( XMLBigDecimal::compareValues(getMaxExclusive(), numBase->getMaxExclusive()) != 0 ))
                        {
                            XMLCh* value1 = getMaxExclusive()->toString();
                            ArrayJanitor<XMLCh> jan1(value1);
                            XMLCh* value2 = numBase->getMaxExclusive()->toString();
                            ArrayJanitor<XMLCh> jan2(value2);
                            ThrowXML2(InvalidDatatypeFacetException
                                , XMLExcepts::FACET_maxExcl_base_fixed
                                , value1
                                , value2);
                        }                    
                    }

                    // check 4.3.9.c3 error:
                    // minExclusive < base.minExclusive
                    // minExclusive > base.maxInclusive ??? minExclusive >= base.maxInclusive
                    // minExclusive < base.minInclusive
                    // minExclusive >= base.maxExclusive
                    // minExclusive != base.minExclusive if (base.fixed)
                    if ( ((getFacetsDefined() & DatatypeValidator::FACET_MINEXCLUSIVE) != 0) )
                    {
                        if ( (( numBase->getFacetsDefined() & DatatypeValidator::FACET_MINEXCLUSIVE) != 0) &&
                             ( XMLBigDecimal::compareValues(getMinExclusive(), numBase->getMinExclusive() ) == -1 ))
                        {
                            XMLCh* value1 = getMinExclusive()->toString();
                            ArrayJanitor<XMLCh> jan1(value1);
                            XMLCh* value2 = numBase->getMinExclusive()->toString();
                            ArrayJanitor<XMLCh> jan2(value2);
                            ThrowXML2(InvalidDatatypeFacetException
                                , XMLExcepts::FACET_minExcl_base_minExcl
                                , value1
                                , value2);
                        }

                        if ( (( numBase->getFacetsDefined() & DatatypeValidator::FACET_MAXINCLUSIVE) != 0) &&
                             ( XMLBigDecimal::compareValues(getMinExclusive(), numBase->getMaxInclusive()) == 1 ))
                        {
                            XMLCh* value1 = getMinExclusive()->toString();
                            ArrayJanitor<XMLCh> jan1(value1);
                            XMLCh* value2 = numBase->getMaxInclusive()->toString();
                            ArrayJanitor<XMLCh> jan2(value2);
                            ThrowXML2(InvalidDatatypeFacetException
                                , XMLExcepts::FACET_minExcl_base_maxIncl
                                , value1
                                , value2);
                        }

                        if ( (( numBase->getFacetsDefined() & DatatypeValidator::FACET_MININCLUSIVE) != 0) &&
                             ( XMLBigDecimal::compareValues(getMinExclusive(), numBase->getMinInclusive()) == -1 ))
                        {
                            XMLCh* value1 = getMinExclusive()->toString();
                            ArrayJanitor<XMLCh> jan1(value1);
                            XMLCh* value2 = numBase->getMinInclusive()->toString();
                            ArrayJanitor<XMLCh> jan2(value2);
                            ThrowXML2(InvalidDatatypeFacetException
                                , XMLExcepts::FACET_minExcl_base_minIncl
                                , value1
                                , value2);
                        }

                        if ( (( numBase->getFacetsDefined() & DatatypeValidator::FACET_MAXEXCLUSIVE) != 0) &&
                             ( XMLBigDecimal::compareValues(getMinExclusive(), numBase->getMaxExclusive()) != -1 ))
                        {
                            XMLCh* value1 = getMinExclusive()->toString();
                            ArrayJanitor<XMLCh> jan1(value1);
                            XMLCh* value2 = numBase->getMaxExclusive()->toString();
                            ArrayJanitor<XMLCh> jan2(value2);
                            ThrowXML2(InvalidDatatypeFacetException
                                , XMLExcepts::FACET_minExcl_base_maxExcl
                                , value1
                                , value2);
                        }

                        if ( (( numBase->getFacetsDefined() & DatatypeValidator::FACET_MINEXCLUSIVE) != 0) &&
                             (( numBase->getFixed() & DatatypeValidator::FACET_MINEXCLUSIVE) != 0) &&
                             ( XMLBigDecimal::compareValues(getMinExclusive(), numBase->getMinExclusive() ) != 0 ))
                        {
                            XMLCh* value1 = getMinExclusive()->toString();
                            ArrayJanitor<XMLCh> jan1(value1);
                            XMLCh* value2 = numBase->getMinExclusive()->toString();
                            ArrayJanitor<XMLCh> jan2(value2);
                            ThrowXML2(InvalidDatatypeFacetException
                                , XMLExcepts::FACET_minExcl_base_fixed
                                , value1
                                , value2);
                        }                    
                    }

                    // check 4.3.10.c2 error:
                    // minInclusive < base.minInclusive
                    // minInclusive > base.maxInclusive
                    // minInclusive <= base.minExclusive
                    // minInclusive >= base.maxExclusive
                    // minInclusive != base.minInclusive if (base.fixed)
                    if ( ((getFacetsDefined() & DatatypeValidator::FACET_MININCLUSIVE) != 0) )
                    {
                        if ( ((numBase->getFacetsDefined() & DatatypeValidator::FACET_MININCLUSIVE) != 0) &&
                             ( XMLBigDecimal::compareValues(getMinInclusive(), numBase->getMinInclusive()) == -1 ))
                        {
                            XMLCh* value1 = getMinInclusive()->toString();
                            ArrayJanitor<XMLCh> jan1(value1);
                            XMLCh* value2 = numBase->getMinInclusive()->toString();
                            ArrayJanitor<XMLCh> jan2(value2);
                            ThrowXML2(InvalidDatatypeFacetException
                                , XMLExcepts::FACET_minIncl_base_minIncl
                                , value1
                                , value2);
                        }

                        if ( (( numBase->getFacetsDefined() & DatatypeValidator::FACET_MAXINCLUSIVE) != 0) &&
                             ( XMLBigDecimal::compareValues(getMinInclusive(), numBase->getMaxInclusive()) == 1 ))
                        {
                            XMLCh* value1 = getMinInclusive()->toString();
                            ArrayJanitor<XMLCh> jan1(value1);
                            XMLCh* value2 = numBase->getMaxInclusive()->toString();
                            ArrayJanitor<XMLCh> jan2(value2);
                            ThrowXML2(InvalidDatatypeFacetException
                                , XMLExcepts::FACET_minIncl_base_maxIncl
                                , value1
                                , value2);
                        }

                        if ( (( numBase->getFacetsDefined() & DatatypeValidator::FACET_MINEXCLUSIVE) != 0) &&
                             ( XMLBigDecimal::compareValues(getMinInclusive(), numBase->getMinExclusive() ) != 1 ))
                        {
                            XMLCh* value1 = getMinInclusive()->toString();
                            ArrayJanitor<XMLCh> jan1(value1);
                            XMLCh* value2 = numBase->getMinExclusive()->toString();
                            ArrayJanitor<XMLCh> jan2(value2);
                            ThrowXML2(InvalidDatatypeFacetException
                                , XMLExcepts::FACET_minIncl_base_minExcl
                                , value1
                                , value2);
                        }

                        if ( (( numBase->getFacetsDefined() & DatatypeValidator::FACET_MAXEXCLUSIVE) != 0) &&
                             ( XMLBigDecimal::compareValues(getMinInclusive(), numBase->getMaxExclusive()) != -1 ))
                        {
                            XMLCh* value1 = getMinInclusive()->toString();
                            ArrayJanitor<XMLCh> jan1(value1);
                            XMLCh* value2 = numBase->getMaxExclusive()->toString();
                            ArrayJanitor<XMLCh> jan2(value2);
                            ThrowXML2(InvalidDatatypeFacetException
                                , XMLExcepts::FACET_minIncl_base_maxExcl
                                , value1
                                , value2);
                        }

                        if ( ((numBase->getFacetsDefined() & DatatypeValidator::FACET_MININCLUSIVE) != 0) &&
                             ((numBase->getFixed() & DatatypeValidator::FACET_MININCLUSIVE) != 0) &&
                             ( XMLBigDecimal::compareValues(getMinInclusive(), numBase->getMinInclusive()) != 0 ))
                        {
                            XMLCh* value1 = getMinInclusive()->toString();
                            ArrayJanitor<XMLCh> jan1(value1);
                            XMLCh* value2 = numBase->getMinInclusive()->toString();
                            ArrayJanitor<XMLCh> jan2(value2);
                            ThrowXML2(InvalidDatatypeFacetException
                                , XMLExcepts::FACET_minIncl_base_fixed
                                , value1
                                , value2);
                        }
                    
                    }

                    // check 4.3.11.c1 error: totalDigits > base.totalDigits
                    // totalDigits != base.totalDigits if (base.fixed)
                    if (( getFacetsDefined() & DatatypeValidator::FACET_TOTALDIGITS) != 0)
                    {
                        if ( (( numBase->getFacetsDefined() & DatatypeValidator::FACET_TOTALDIGITS) != 0) &&
                             ( fTotalDigits > numBase->fTotalDigits ))
                        {
                            XMLString::binToText(fTotalDigits, value1, BUF_LEN, 10);
                            XMLString::binToText(numBase->fTotalDigits, value2, BUF_LEN, 10);
                            ThrowXML2(InvalidDatatypeFacetException
                                 , XMLExcepts::FACET_totalDigit_base_totalDigit
                                 , value1
                                 , value2);
                        }

                        if ( (( numBase->getFacetsDefined() & DatatypeValidator::FACET_TOTALDIGITS) != 0) &&
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

                   if (( getFacetsDefined() & DatatypeValidator::FACET_FRACTIONDIGITS) != 0)
                   {
                        // check question error: fractionDigits > base.fractionDigits ???
                        if ( (( numBase->getFacetsDefined() & DatatypeValidator::FACET_FRACTIONDIGITS) != 0) &&
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
                        if ( (( numBase->getFacetsDefined() & DatatypeValidator::FACET_TOTALDIGITS) != 0) &&
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
                        if ( (( numBase->getFacetsDefined() & DatatypeValidator::FACET_FRACTIONDIGITS) != 0) &&
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


                    // check question error: totalDigits conflicts with bounds ???

                    // check 4.3.5.c0 must: enumeration values from the value space of base
                    //
                    // In fact, the values in the enumeration shall go through validation
                    // of this class as well.
                    // this->checkContent(value, false);
                    //
                    if ( ((getFacetsDefined() & DatatypeValidator::FACET_ENUMERATION) != 0) &&
                         ( fStrEnumeration != 0 ))
                    {
                        int i;
                        int enumLength = fStrEnumeration->size();
                        try
                        {
                            for ( i = 0; i < enumLength; i++)
                            {
                                // ask parent do a complete check
                                numBase->checkContent(fStrEnumeration->elementAt(i), false);
                                // shall pass this->checkContent() as well
                                checkContent(fStrEnumeration->elementAt(i), false);
                            }
                                
                        }

                        catch ( XMLException& )
                        {
                            ThrowXML1(InvalidDatatypeFacetException
                                    , XMLExcepts::FACET_enum_base
                                    , fStrEnumeration->elementAt(i));
                        }

                        //
                        // we need to convert from fStrEnumeration to fEnumeration
                        try
                        {
                            fEnumeration = new RefVectorOf<XMLBigDecimal>(enumLength, true);
                            fEnumerationInherited = false;
                            for ( i = 0; i < enumLength; i++)
                                fEnumeration->insertElementAt(new XMLBigDecimal(fStrEnumeration->elementAt(i)), i);

                        }
                        catch ( NumberFormatException& )
                        {
                            ThrowXML1(InvalidDatatypeFacetException
                                    , XMLExcepts::FACET_enum_base
                                    , fStrEnumeration->elementAt(i));
                        }

                    }

                    //
                    // maxInclusive
                    // maxExclusive
                    // minInclusive
                    // minExclusive
                    // shall come from the base's value space as well
                    //
                    if ((getFacetsDefined() & DatatypeValidator::FACET_MAXINCLUSIVE) != 0)
                    {
                        try
                        {
                             // ask parent do a complete check
                             numBase->checkContent(getMaxInclusive()->toString(), false);
                        }
                        catch ( XMLException& )
                        {
                            XMLCh* value1 = getMaxInclusive()->toString();
                            ArrayJanitor<XMLCh> jan(value1);
                            ThrowXML1(InvalidDatatypeFacetException
                                    , XMLExcepts::FACET_maxIncl_notFromBase
                                    , value1
                                    );
                        }
                    }

                    if ((getFacetsDefined() & DatatypeValidator::FACET_MAXEXCLUSIVE) != 0)
                    {
                        try
                        {
                             // ask parent do a complete check
                             numBase->checkContent(getMaxExclusive()->toString(), false);
                        }
                        catch ( XMLException& )
                        {
                            XMLCh* value1 = getMaxExclusive()->toString();
                            ArrayJanitor<XMLCh> jan(value1);
                            ThrowXML1(InvalidDatatypeFacetException
                                    , XMLExcepts::FACET_maxExcl_notFromBase
                                    , value1
                                    );
                        }
                    }

                    if ((getFacetsDefined() & DatatypeValidator::FACET_MININCLUSIVE) != 0)
                    {
                        try
                        {
                             // ask parent do a complete check
                             numBase->checkContent(getMinInclusive()->toString(), false);
                        }
                        catch ( XMLException& )
                        {
                            XMLCh* value1 = getMinInclusive()->toString();
                            ArrayJanitor<XMLCh> jan(value1);
                            ThrowXML1(InvalidDatatypeFacetException
                                    , XMLExcepts::FACET_minIncl_notFromBase
                                    , value1
                                    );
                        }
                    }

                    if ((getFacetsDefined() & DatatypeValidator::FACET_MINEXCLUSIVE) != 0)
                    {
                        try
                        {
                             // ask parent do a complete check
                             numBase->checkContent(getMinExclusive()->toString(), false);
                        }
                        catch ( XMLException& )
                        {
                            XMLCh* value1 = getMinExclusive()->toString();
                            ArrayJanitor<XMLCh> jan(value1);
                            ThrowXML1(InvalidDatatypeFacetException
                                    , XMLExcepts::FACET_minExcl_notFromBase
                                    , value1
                                    );
                        }
                    }

                }

            } //if baseValidator
        }
        catch (XMLException &e)
        {
            ThrowXML1(InvalidDatatypeFacetException, XMLExcepts::RethrowError, e.getMessage());
        }

    }// End of Facet setting


    /***
         Schema constraint: Part III -- inherit from base
     ***/
    if ( baseValidator )    
    {     
        DecimalDatatypeValidator* numBase = (DecimalDatatypeValidator*)baseValidator;         
        // inherit enumeration          
        if ((( numBase->getFacetsDefined() & DatatypeValidator::FACET_ENUMERATION) !=0) &&          
            (( getFacetsDefined() & DatatypeValidator::FACET_ENUMERATION) == 0))              
        {          
            fEnumeration = numBase->getEnumeration();              
            fEnumerationInherited = true;              
            setFacetsDefined(DatatypeValidator::FACET_ENUMERATION);              
        }

        // inherit maxExclusive         
        if ((( numBase->getFacetsDefined() & DatatypeValidator::FACET_MAXEXCLUSIVE) != 0) &&          
            (( getFacetsDefined() & DatatypeValidator::FACET_MAXEXCLUSIVE) == 0) &&              
            (( getFacetsDefined() & DatatypeValidator::FACET_MAXINCLUSIVE) == 0) )              
        {          
            setMaxExclusive(new XMLBigDecimal(*(numBase->getMaxExclusive())));              
            setFacetsDefined(DatatypeValidator::FACET_MAXEXCLUSIVE);              
        }
         
        // inherit maxInclusive          
        if ((( numBase->getFacetsDefined() & DatatypeValidator::FACET_MAXINCLUSIVE) != 0) &&         
            (( getFacetsDefined() & DatatypeValidator::FACET_MAXEXCLUSIVE) == 0) &&              
            (( getFacetsDefined() & DatatypeValidator::FACET_MAXINCLUSIVE) == 0) )              
        {          
            setMaxInclusive(new XMLBigDecimal(*(numBase->getMaxInclusive())));
            setFacetsDefined(DatatypeValidator::FACET_MAXINCLUSIVE);
        }

        // inherit minExclusive          
        if ((( numBase->getFacetsDefined() & DatatypeValidator::FACET_MINEXCLUSIVE) != 0) &&          
            (( getFacetsDefined() & DatatypeValidator::FACET_MINEXCLUSIVE) == 0) &&              
            (( getFacetsDefined() & DatatypeValidator::FACET_MININCLUSIVE) == 0) )              
        {          
            setMinExclusive(new XMLBigDecimal(*(numBase->getMinExclusive())));              
            setFacetsDefined(DatatypeValidator::FACET_MINEXCLUSIVE);              
        }
          
        // inherit minExclusive          
        if ((( numBase->getFacetsDefined() & DatatypeValidator::FACET_MININCLUSIVE) != 0) &&          
            (( getFacetsDefined() & DatatypeValidator::FACET_MINEXCLUSIVE) == 0) &&
            (( getFacetsDefined() & DatatypeValidator::FACET_MININCLUSIVE) == 0) )              
        {          
            setMinInclusive(new XMLBigDecimal(*(numBase->getMinInclusive())));              
            setFacetsDefined(DatatypeValidator::FACET_MININCLUSIVE);              
        }

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

        // inherit "fixed" option
        setFixed(getFixed() | numBase->getFixed());
          
    }

}

void DecimalDatatypeValidator::checkContent( const XMLCh* const content, bool asBase)
{

    //validate against base validator if any
    DecimalDatatypeValidator *pBase = (DecimalDatatypeValidator*) this->getBaseValidator();
    if (pBase !=0)
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

        if (getEnumeration() != 0)
        {
            int i=0;
            int enumLength = getEnumeration()->size();
            for ( ; i < enumLength; i++)
            {
                if (XMLBigDecimal::compareValues(theData, getEnumeration()->elementAt(i))==0)
                    break;
            }

            if (i == enumLength)
                ThrowXML1(InvalidDatatypeValueException, XMLExcepts::VALUE_NotIn_Enumeration, content);
        }


        if ( (getFacetsDefined() & DatatypeValidator::FACET_FRACTIONDIGITS) != 0 )
        {
            if ( theData->getScale() > fFractionDigits )
            {
                XMLCh* value = theData->toString();
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

        if ( (getFacetsDefined() & DatatypeValidator::FACET_MAXEXCLUSIVE) != 0 )
        {
            // must be < MaxExclusive
            if (XMLBigDecimal::compareValues(theData, getMaxExclusive()) != -1)
            {
                XMLCh* value1 = theData->toString();
                ArrayJanitor<XMLCh> jan1(value1);
                XMLCh* value2 = getMaxExclusive()->toString();
                ArrayJanitor<XMLCh> jan2(value2);
                ThrowXML2(InvalidDatatypeFacetException
                                 , XMLExcepts::VALUE_exceed_maxExcl
                                 , value1
                                 , value2);
            }
        }

        if ( (getFacetsDefined() & DatatypeValidator::FACET_MAXINCLUSIVE) != 0 )
        {
            // must be <= MaxInclusive
            if (XMLBigDecimal::compareValues(theData, getMaxInclusive()) == 1)
            {
                XMLCh* value1 = theData->toString();
                ArrayJanitor<XMLCh> jan1(value1);
                XMLCh* value2 = getMaxInclusive()->toString();
                ArrayJanitor<XMLCh> jan2(value2);
                ThrowXML2(InvalidDatatypeFacetException
                                 , XMLExcepts::VALUE_exceed_maxIncl
                                 , value1
                                 , value2);

            }
        }

        if ( (getFacetsDefined() & DatatypeValidator::FACET_MININCLUSIVE) != 0 )
        {
            // must be >= MinInclusive
            if (XMLBigDecimal::compareValues(theData, getMinInclusive()) == -1)
            {
                XMLCh* value1 = theData->toString();
                ArrayJanitor<XMLCh> jan1(value1);
                XMLCh* value2 = getMinInclusive()->toString();
                ArrayJanitor<XMLCh> jan2(value2);
                ThrowXML2(InvalidDatatypeFacetException
                                 , XMLExcepts::VALUE_exceed_minIncl
                                 , value1
                                 , value2);
            }
        }

        if ( (getFacetsDefined() & DatatypeValidator::FACET_MINEXCLUSIVE) != 0 )
        {
            // must be > MinExclusive
            if (XMLBigDecimal::compareValues(theData, getMinExclusive()) != 1)
            {
                XMLCh* value1 = theData->toString();
                ArrayJanitor<XMLCh> jan1(value1);
                XMLCh* value2 = getMinExclusive()->toString();
                ArrayJanitor<XMLCh> jan2(value2);
                ThrowXML2(InvalidDatatypeFacetException
                                 , XMLExcepts::VALUE_exceed_minExcl
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

