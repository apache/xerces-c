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

// ---------------------------------------------------------------------------
//  Constructors and Destructor
// ---------------------------------------------------------------------------
DecimalDatatypeValidator::DecimalDatatypeValidator(
                          DatatypeValidator*            const baseValidator
                        , RefHashTableOf<KVStringPair>* const facets
                        , const int                           finalSet)
:DatatypeValidator(baseValidator, facets, finalSet, DatatypeValidator::Decimal)
, fTotalDigits(0)
, fFractionDigits(0)
, fMaxInclusive(0)
, fMaxExclusive(0)
, fMinInclusive(0)
, fMinExclusive(0)
, fEnumeration(0)
{
    try
    {
        init(baseValidator, facets);
    }

    catch (XMLException&)
    {
        cleanUp();
        throw;
    }

}

void DecimalDatatypeValidator::init(DatatypeValidator*            const baseValidator
                                  , RefHashTableOf<KVStringPair>* const facets)
{
    // Set Facets if any defined
    if (facets)
    {
        XMLCh* key;
        XMLCh* value;
        RefVectorOf<XMLCh>*             fStrEnumeration = 0; // save the literal value
        Janitor<RefVectorOf<XMLCh> >    janStrEnum(fStrEnumeration);

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
            else if (XMLString::compareString(key, SchemaSymbols::fgELT_ENUMERATION)==0)
            {
                if (fStrEnumeration)
                    delete fStrEnumeration;

                fStrEnumeration = XMLString::tokenizeString(value);
                setFacetsDefined(DatatypeValidator::FACET_ENUMERATION);
            }
            else if (XMLString::compareString(key, SchemaSymbols::fgELT_MAXINCLUSIVE)==0)
            {
                try
                {
                    setMaxInclusive(new XMLBigDecimal(value));
                }
                catch (NumberFormatException)
                {
                     ThrowXML(InvalidDatatypeFacetException, XMLExcepts::FACET_Len_maxLen);
                    // ThrowXML(InvalidDatatypeFacetException, XMLExcepts::FACET_Inv_MaxInc);
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
                     ThrowXML(InvalidDatatypeFacetException, XMLExcepts::FACET_Len_maxLen);
                    // ThrowXML(InvalidDatatypeFacetException, XMLExcepts::FACET_Inv_MaxExc);
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
                     ThrowXML(InvalidDatatypeFacetException, XMLExcepts::FACET_Len_maxLen);
                    // ThrowXML(InvalidDatatypeFacetException, XMLExcepts::FACET_Inv_MinInc);
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
                     ThrowXML(InvalidDatatypeFacetException, XMLExcepts::FACET_Len_maxLen);
                    // ThrowXML(InvalidDatatypeFacetException, XMLExcepts::FACET_Inv_MinExc);
                }
                setFacetsDefined(DatatypeValidator::FACET_MINEXCLUSIVE);
            }
            else if (XMLString::compareString(key, SchemaSymbols::fgELT_TOTALDIGITS)==0)
            {
                try
                {
                    setTotalDigits(XMLString::parseInt(value));
                }
                catch (NumberFormatException)
                {
                     ThrowXML(InvalidDatatypeFacetException, XMLExcepts::FACET_Len_maxLen);
                    // ThrowXML(InvalidDatatypeFacetException, XMLExcepts::FACET_Inv_Precision);
                }

                // check 4.3.11.c0 must: totalDigits > 0
                if ( fTotalDigits <= 0 )
                     ThrowXML(InvalidDatatypeFacetException, XMLExcepts::FACET_Len_maxLen);
                    // ThrowXML(InvalidDatatypeFacetException, XMLExcepts::FACET_Neg_Precision);
                    //InvalidDatatypeFacetException("totalDigits value '"+fTotalDigits+"' must be a positiveInteger.");

                setFacetsDefined(DatatypeValidator::FACET_PRECISSION);
            }
            else if (XMLString::compareString(key, SchemaSymbols::fgELT_FRACTIONDIGITS)==0)
            {
                try
                {
                    setFractionDigits(XMLString::parseInt(value));
                }
                catch (NumberFormatException)
                {
                     ThrowXML(InvalidDatatypeFacetException, XMLExcepts::FACET_Len_maxLen);
                    // ThrowXML(InvalidDatatypeFacetException, XMLExcepts::FACET_Inv_Precision);
                }

                // check 4.3.12.c0 must: fractionDigits > 0
                if ( fFractionDigits < 0 )
                     ThrowXML(InvalidDatatypeFacetException, XMLExcepts::FACET_Len_maxLen);
                    // ThrowXML(InvalidDatatypeFacetException, XMLExcepts::FACET_Neg_Fraction);
                    //InvalidDatatypeFacetException("fractionDigits value '"+fFractionDigits+"' must be a positiveInteger.");

                setFacetsDefined(DatatypeValidator::FACET_SCALE);
            }
            else
            {
                 ThrowXML(InvalidDatatypeFacetException, XMLExcepts::FACET_Invalid_Tag);
            }
        }//while

        /***
           Schema constraint: Part I -- self checking
        ***/

        if ( getFacetsDefined() != 0 )
        {
            // non co-existence checking
            // check 4.3.8.c1 error: maxInclusive + maxExclusive
            if ( ((getFacetsDefined() & DatatypeValidator::FACET_MAXEXCLUSIVE) != 0) &&
                 ((getFacetsDefined() & DatatypeValidator::FACET_MAXINCLUSIVE) != 0) )
            {
                 ThrowXML(InvalidDatatypeFacetException, XMLExcepts::FACET_Len_maxLen);
                 // ThrowXML(InvalidDatatypeFacetException, XMLExcepts::FACET_max_inc_exc);
                 // "It is an error for both maxInclusive and maxExclusive to be specified
                 // for the same datatype." );
            }

            // non co-existence checking
            // check 4.3.9.c1 error: minInclusive + minExclusive
            if ( ((getFacetsDefined() & DatatypeValidator::FACET_MINEXCLUSIVE) != 0) &&
                 ((getFacetsDefined() & DatatypeValidator::FACET_MININCLUSIVE) != 0) )
            {
                 ThrowXML(InvalidDatatypeFacetException, XMLExcepts::FACET_Len_maxLen);
                 // ThrowXML(InvalidDatatypeFacetException, XMLExcepts::FACET_min_inc_exc);
                 // "It is an error for both minInclusive and minExclusive to be specified
                 // for the same datatype." );
            }

            //
            // minExclusive < minInclusive <= maxInclusive < maxExclusive
            //
            // check 4.3.7.c1 must: minInclusive <= maxInclusive
            if ( ((getFacetsDefined() & DatatypeValidator::FACET_MAXINCLUSIVE) != 0) &&
                 ((getFacetsDefined() & DatatypeValidator::FACET_MININCLUSIVE) != 0) )
            {
                if ( XMLBigDecimal::compareValues(getMinInclusive(), getMaxInclusive()) == 1 )
                    ThrowXML(InvalidDatatypeFacetException, XMLExcepts::FACET_Len_maxLen);
                    // ThrowXML(InvalidDatatypeFacetException, XMLExcepts::FACET_inc_min_max);
                    // "minInclusive value ='" + getMinInclusive(false) + "'must be <=
                    // maxInclusive value ='" + getMaxInclusive(false) + "'. " );
            }

            // check 4.3.8.c2 must: minExclusive <= maxExclusive ??? minExclusive < maxExclusive
            if ( ((getFacetsDefined() & DatatypeValidator::FACET_MAXEXCLUSIVE) != 0) &&
                 ((getFacetsDefined() & DatatypeValidator::FACET_MINEXCLUSIVE) != 0) )
            {
                if ( XMLBigDecimal::compareValues(getMinExclusive(), getMaxExclusive()) == 1 )
                    ThrowXML(InvalidDatatypeFacetException, XMLExcepts::FACET_Len_maxLen);
                    // ThrowXML(InvalidDatatypeFacetException, XMLExcepts::FACET_exc_min_max);
                    // "minExclusive value ='" + getMinExclusive(false) + "'must be <=
                    // maxExclusive value ='" + getMaxExclusive(false) + "'. " );
            }

            // check 4.3.9.c2 must: minExclusive < maxInclusive
            if ( ((getFacetsDefined() & DatatypeValidator::FACET_MAXINCLUSIVE) != 0) &&
                 ((getFacetsDefined() & DatatypeValidator::FACET_MINEXCLUSIVE) != 0) )
            {
                if ( XMLBigDecimal::compareValues(getMinExclusive(), getMaxInclusive()) != -1 )
                    ThrowXML(InvalidDatatypeFacetException, XMLExcepts::FACET_Len_maxLen);
                    // ThrowXML(InvalidDatatypeFacetException, XMLExcepts::FACET_maxinc_minexc);
                    // "minExclusive value ='" + getMinExclusive(false) + "'must be >
                    // maxInclusive value ='" + getMaxInclusive(false) + "'. " );
            }

            // check 4.3.10.c1 must: minInclusive < maxExclusive
            if ( ((getFacetsDefined() & DatatypeValidator::FACET_MAXEXCLUSIVE) != 0) &&
                 ((getFacetsDefined() & DatatypeValidator::FACET_MININCLUSIVE) != 0) )
            {
                if ( XMLBigDecimal::compareValues(getMinInclusive(), getMaxExclusive()) != -1 )
                    ThrowXML(InvalidDatatypeFacetException, XMLExcepts::FACET_Len_maxLen);
                    // ThrowXML(InvalidDatatypeFacetException, XMLExcepts::FACET_mininc_maxexc);
                   // "minInclusive value ='" + getMinInclusive(false) + "'must be <
                   // maxExclusive value ='" + getMaxExclusive(false) + "'. " );
             }

            // check 4.3.12.c1 must: fractionDigits <= totalDigits
            if ( ((getFacetsDefined() & DatatypeValidator::FACET_SCALE) != 0) &&
                 ((getFacetsDefined() & DatatypeValidator::FACET_PRECISSION) != 0) )
            {
                if ( fFractionDigits > fTotalDigits )
                    ThrowXML(InvalidDatatypeFacetException, XMLExcepts::FACET_Len_maxLen);
                    //ThrowXML(InvalidDatatypeFacetException, XMLExcepts::FACET_scale_precision);
                    // "fractionDigits value ='" + this.fFractionDigits + "'must be <=
                    // totalDigits value ='" + this.fTotalDigits + "'. " );
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

                if ( ((getFacetsDefined() & DatatypeValidator::FACET_MAXINCLUSIVE) != 0) )
                {
                    if ( ((numBase->getFacetsDefined() & DatatypeValidator::FACET_MAXINCLUSIVE) != 0) &&
                         ( XMLBigDecimal::compareValues(getMaxInclusive(), numBase->getMaxInclusive()) == 1 ))
                        ThrowXML(InvalidDatatypeFacetException, XMLExcepts::FACET_Len_maxLen);
                        //ThrowXML(InvalidDatatypeFacetException, XMLExcepts::FACET_maxinc_base_maxinc);
                        // "maxInclusive value ='" + getMaxInclusive(false) + "' must be <=
                        // base.maxInclusive value ='" + getMaxInclusive(true) + "'." );

                    if ( ((numBase->getFacetsDefined() & DatatypeValidator::FACET_MAXEXCLUSIVE) != 0) &&
                         ( XMLBigDecimal::compareValues(getMaxInclusive(), numBase->getMaxExclusive()) != -1 ))
                        ThrowXML(InvalidDatatypeFacetException, XMLExcepts::FACET_Len_maxLen);
                        //ThrowXML(InvalidDatatypeFacetException, XMLExcepts::FACET_maxinc_base_maxexc);
                        // "maxInclusive value ='" + getMaxInclusive(false) + "' must be <
                        // base.maxExclusive value ='" + getMaxExclusive(true) + "'." );

                    if ( (( numBase->getFacetsDefined() & DatatypeValidator::FACET_MININCLUSIVE) != 0) &&
                         ( XMLBigDecimal::compareValues(getMaxInclusive(), numBase->getMinInclusive()) == -1 ))
                        ThrowXML(InvalidDatatypeFacetException, XMLExcepts::FACET_Len_maxLen);
                        //ThrowXML(InvalidDatatypeFacetException, XMLExcepts::FACET_maxinc_base_mininc);
                        // "maxInclusive value ='" + getMaxInclusive(false) + "' must be >=
                        // base.minInclusive value ='" + getMinInclusive(true) + "'." );

                    if ( (( numBase->getFacetsDefined() & DatatypeValidator::FACET_MINEXCLUSIVE) != 0) &&
                         ( XMLBigDecimal::compareValues(getMaxInclusive(), numBase->getMinExclusive() ) != 1 ))
                        ThrowXML(InvalidDatatypeFacetException, XMLExcepts::FACET_Len_maxLen);
                        //ThrowXML(InvalidDatatypeFacetException, XMLExcepts::FACET_maxinc_base_minexc);
                        // "maxInclusive value ='" + getMaxInclusive(false) + "' must be >
                        // base.minExclusive value ='" + getMinExclusive(true) + "'." );
                }

                // check 4.3.8.c3 error:
                // maxExclusive > base.maxExclusive
                // maxExclusive > base.maxInclusive
                // maxExclusive <= base.minInclusive
                // maxExclusive <= base.minExclusive
                if ( ((getFacetsDefined() & DatatypeValidator::FACET_MAXEXCLUSIVE) != 0) )
                {
                    if ( (( numBase->getFacetsDefined() & DatatypeValidator::FACET_MAXEXCLUSIVE) != 0) &&
                         ( XMLBigDecimal::compareValues(getMaxExclusive(), numBase->getMaxExclusive()) == 1 ))
                        ThrowXML(InvalidDatatypeFacetException, XMLExcepts::FACET_Len_maxLen);
                        //ThrowXML(InvalidDatatypeFacetException, XMLExcepts::FACET_maxexc_base_maxexc);
                        // "maxExclusive value ='" + getMaxExclusive(false) + "' must be
                        // <= base.maxExclusive value ='" + getMaxExclusive(true) + "'." );

                    if ( (( numBase->getFacetsDefined() & DatatypeValidator::FACET_MAXINCLUSIVE) != 0) &&
                         ( XMLBigDecimal::compareValues(getMaxExclusive(), numBase->getMaxInclusive()) == 1 ))
                        ThrowXML(InvalidDatatypeFacetException, XMLExcepts::FACET_Len_maxLen);
                        //ThrowXML(InvalidDatatypeFacetException, XMLExcepts::FACET_maxexc_base_maxinc);
                        // "maxExclusive value ='" + getMaxExclusive(false) + "' must be
                        // <= base.maxInclusive value ='" + getMaxInclusive(true) + "'." );

                    if ( (( numBase->getFacetsDefined() & DatatypeValidator::FACET_MINEXCLUSIVE) != 0) &&
                         ( XMLBigDecimal::compareValues(getMaxExclusive(), numBase->getMinExclusive() ) != 1 ))
                        ThrowXML(InvalidDatatypeFacetException, XMLExcepts::FACET_Len_maxLen);
                        //ThrowXML(InvalidDatatypeFacetException, XMLExcepts::FACET_maxexc_base_mininc);
                        // "maxExclusive value ='" + getMaxExclusive(false) + "' must be
                        // > base.minExclusive value ='" + getMinExclusive(true) + "'." );

                    if ( (( numBase->getFacetsDefined() & DatatypeValidator::FACET_MININCLUSIVE) != 0) &&
                         ( XMLBigDecimal::compareValues(getMaxExclusive(), numBase->getMinInclusive()) != 1 ))
                        ThrowXML(InvalidDatatypeFacetException, XMLExcepts::FACET_Len_maxLen);
                        //ThrowXML(InvalidDatatypeFacetException, XMLExcepts::FACET_maxexc_base_minexc);
                        // "maxExclusive value ='" + getMaxExclusive(false) + "' must be
                        // > base.minInclusive value ='" + getMinInclusive(true) + "'." );
                }

                // check 4.3.9.c3 error:
                // minExclusive < base.minExclusive
                // minExclusive > base.maxInclusive ??? minExclusive >= base.maxInclusive
                // minExclusive < base.minInclusive
                // minExclusive >= base.maxExclusive
                if ( ((getFacetsDefined() & DatatypeValidator::FACET_MINEXCLUSIVE) != 0) )
                {
                    if ( (( numBase->getFacetsDefined() & DatatypeValidator::FACET_MINEXCLUSIVE) != 0) &&
                         ( XMLBigDecimal::compareValues(getMinExclusive(), numBase->getMinExclusive() ) == -1 ))
                        ThrowXML(InvalidDatatypeFacetException, XMLExcepts::FACET_Len_maxLen);
                        //ThrowXML(InvalidDatatypeFacetException, XMLExcepts::FACET_mininc_base_minexc);
                        // "minExclusive value ='" + getMinExclusive(false) + "' must be
                        // >= base.minExclusive value ='" + getMinExclusive(true) + "'." );

                    if ( (( numBase->getFacetsDefined() & DatatypeValidator::FACET_MAXINCLUSIVE) != 0) &&
                         ( XMLBigDecimal::compareValues(getMinExclusive(), numBase->getMaxInclusive()) == 1 ))
                        ThrowXML(InvalidDatatypeFacetException, XMLExcepts::FACET_Len_maxLen);
                        //ThrowXML(InvalidDatatypeFacetException, XMLExcepts::FACET_mininc_base_maxinc);
                        // "minExclusive value ='" + getMinExclusive(false) + "' must be
                        // <= base.maxInclusive value ='" + getMaxInclusive(true) + "'." );

                    if ( (( numBase->getFacetsDefined() & DatatypeValidator::FACET_MININCLUSIVE) != 0) &&
                         ( XMLBigDecimal::compareValues(getMinExclusive(), numBase->getMinInclusive()) == -1 ))
                        ThrowXML(InvalidDatatypeFacetException, XMLExcepts::FACET_Len_maxLen);
                        //ThrowXML(InvalidDatatypeFacetException, XMLExcepts::FACET_mininc_base_mininc);
                        // "minExclusive value ='" + getMinExclusive(false) + "' must be
                        // >= base.minInclusive value ='" + getMinInclusive(true) + "'." );

                    if ( (( numBase->getFacetsDefined() & DatatypeValidator::FACET_MAXEXCLUSIVE) != 0) &&
                         ( XMLBigDecimal::compareValues(getMinExclusive(), numBase->getMaxExclusive()) != -1 ))
                        ThrowXML(InvalidDatatypeFacetException, XMLExcepts::FACET_Len_maxLen);
                        //ThrowXML(InvalidDatatypeFacetException, XMLExcepts::FACET_mininc_base_maxexc);
                        // "minExclusive value ='" + getMinExclusive(false) + "' must be
                        // < base.maxExclusive value ='" + getMaxExclusive(true) + "'." );

                }

                // check 4.3.10.c2 error:
                // minInclusive < base.minInclusive
                // minInclusive > base.maxInclusive
                // minInclusive <= base.minExclusive
                // minInclusive >= base.maxExclusive
                if ( ((getFacetsDefined() & DatatypeValidator::FACET_MININCLUSIVE) != 0) )
                {
                    if ( ((numBase->getFacetsDefined() & DatatypeValidator::FACET_MININCLUSIVE) != 0) &&
                         ( XMLBigDecimal::compareValues(getMinInclusive(), numBase->getMinInclusive()) == -1 ))
                        ThrowXML(InvalidDatatypeFacetException, XMLExcepts::FACET_Len_maxLen);
                        //ThrowXML(InvalidDatatypeFacetException, XMLExcepts::FACET_mininc_base_mininc);
                        // "minInclusive value ='" + getMinInclusive(false) + "' must be
                        // >= base.minInclusive value ='" + getMinInclusive(true) + "'." );

                    if ( (( numBase->getFacetsDefined() & DatatypeValidator::FACET_MAXINCLUSIVE) != 0) &&
                         ( XMLBigDecimal::compareValues(getMinInclusive(), numBase->getMaxInclusive()) == 1 ))
                        ThrowXML(InvalidDatatypeFacetException, XMLExcepts::FACET_Len_maxLen);
                        //ThrowXML(InvalidDatatypeFacetException, XMLExcepts::FACET_mininc_base_maxinc);
                        // "minInclusive value ='" + getMinInclusive(false) + "' must be
                        // <= base.maxInclusive value ='" + getMaxInclusive(true) + "'." );

                    if ( (( numBase->getFacetsDefined() & DatatypeValidator::FACET_MINEXCLUSIVE) != 0) &&
                         ( XMLBigDecimal::compareValues(getMinInclusive(), numBase->getMinExclusive() ) != 1 ))
                        ThrowXML(InvalidDatatypeFacetException, XMLExcepts::FACET_Len_maxLen);
                        //ThrowXML(InvalidDatatypeFacetException, XMLExcepts::FACET_mininc_base_minexc);
                        // "minInclusive value ='" + getMinInclusive(false) + "' must be
                        // > base.minExclusive value ='" + getMinExclusive(true) + "'." );

                    if ( (( numBase->getFacetsDefined() & DatatypeValidator::FACET_MAXEXCLUSIVE) != 0) &&
                         ( XMLBigDecimal::compareValues(getMinInclusive(), numBase->getMaxExclusive()) != -1 ))
                        ThrowXML(InvalidDatatypeFacetException, XMLExcepts::FACET_Len_maxLen);
                        //ThrowXML(InvalidDatatypeFacetException, XMLExcepts::FACET_mininc_base_maxexc);
                        // "minInclusive value ='" + getMinInclusive(false) + "' must be
                        // < base.maxExclusive value ='" + getMaxExclusive(true) + "'." );

                }

                // check 4.3.11.c1 error: totalDigits > base.totalDigits
                if (( getFacetsDefined() & DatatypeValidator::FACET_PRECISSION) != 0)
                {
                    if ( (( numBase->getFacetsDefined() & DatatypeValidator::FACET_PRECISSION) != 0) &&
                         ( fTotalDigits > numBase->fTotalDigits ))
                        ThrowXML(InvalidDatatypeFacetException, XMLExcepts::FACET_Len_maxLen);
                        //ThrowXML(InvalidDatatypeFacetException, XMLExcepts::FACET_totdigit_base_totdigit);
                        // "totalDigits value ='" + fTotalDigits + "' must be
                        // <= base.totalDigits value ='" + numBase.fTotalDigits + "'." );

                }

                // check question error: fractionDigits > base.fractionDigits ???
                // check question error: fractionDigits > base.totalDigits ???
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
                    int i = 0;
                    int enumLength = fStrEnumeration->size();
                    try
                    {
                        for ( ; i < enumLength; i++)
                            // ask parent do a complete check
                            numBase->checkContent(fStrEnumeration->elementAt(i), false);
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
                        setEnumeration( new RefVectorOf<XMLBigDecimal>(enumLength, true));
                        for ( i = 0; i < enumLength; i++)
                            getEnumeration()->insertElementAt(new XMLBigDecimal(fStrEnumeration->elementAt(i)), i);

                    }
                    catch ( NumberFormatException& )
                    {
                        ThrowXML1(InvalidDatatypeFacetException
                                , XMLExcepts::FACET_enum_base
                                , fStrEnumeration->elementAt(i));
                    }

                }

            }

            /***
               Schema constraint: Part III -- inherit from base
            ***/

                // inherit enumeration
                if ((( numBase->getFacetsDefined() & DatatypeValidator::FACET_ENUMERATION) !=0) &&
                    (( getFacetsDefined() & DatatypeValidator::FACET_ENUMERATION) == 0))
                {
                    setFacetsDefined(DatatypeValidator::FACET_ENUMERATION);
                    // need to adopt the Vector
                    RefVectorOf<XMLBigDecimal>*  fBaseEnumeration = numBase->getEnumeration();
                    int enumLength = fBaseEnumeration->size();
                    setEnumeration(new RefVectorOf<XMLBigDecimal>(enumLength, true));
                    for ( int i = 0; i < enumLength; i++)
                        //invoke XMLBigDecimal's copy ctor
                        getEnumeration()->insertElementAt(new XMLBigDecimal(*(fBaseEnumeration->elementAt(i))), i);

                }

                // inherit maxExclusive
                if ( (( numBase->getFacetsDefined() & DatatypeValidator::FACET_MAXEXCLUSIVE) != 0) &&
                     (( getFacetsDefined() & DatatypeValidator::FACET_MAXEXCLUSIVE) == 0) &&
                     (( getFacetsDefined() & DatatypeValidator::FACET_MAXINCLUSIVE) == 0) )
                {
                    setMaxExclusive(new XMLBigDecimal(*(numBase->getMaxExclusive())));
                    setFacetsDefined(DatatypeValidator::FACET_MAXEXCLUSIVE);
                }

                // inherit maxInclusive
                if ( (( numBase->getFacetsDefined() & DatatypeValidator::FACET_MAXINCLUSIVE) != 0) &&
                     (( getFacetsDefined() & DatatypeValidator::FACET_MAXEXCLUSIVE) == 0) &&
                     (( getFacetsDefined() & DatatypeValidator::FACET_MAXINCLUSIVE) == 0) )
                {
                    setMaxInclusive(new XMLBigDecimal(*(numBase->getMaxInclusive())));
                    setFacetsDefined(DatatypeValidator::FACET_MAXINCLUSIVE);
                }

                // inherit minExclusive
                if ( (( numBase->getFacetsDefined() & DatatypeValidator::FACET_MINEXCLUSIVE) != 0) &&
                     (( getFacetsDefined() & DatatypeValidator::FACET_MINEXCLUSIVE) == 0) &&
                     (( getFacetsDefined() & DatatypeValidator::FACET_MININCLUSIVE) == 0) )
                {
                    setMinExclusive(new XMLBigDecimal(*(numBase->getMinExclusive())));
                    setFacetsDefined(DatatypeValidator::FACET_MINEXCLUSIVE);
                }

                // inherit minExclusive
                if ( (( numBase->getFacetsDefined() & DatatypeValidator::FACET_MININCLUSIVE) != 0) &&
                     (( getFacetsDefined() & DatatypeValidator::FACET_MINEXCLUSIVE) == 0) &&
                     (( getFacetsDefined() & DatatypeValidator::FACET_MININCLUSIVE) == 0) )
                {
                    setMinInclusive(new XMLBigDecimal(*(numBase->getMinInclusive())));
                    setFacetsDefined(DatatypeValidator::FACET_MININCLUSIVE);
                }

                // inherit totalDigits
                if ( (( numBase->getFacetsDefined() & DatatypeValidator::FACET_PRECISSION) != 0) &&
                     (( getFacetsDefined() & DatatypeValidator::FACET_PRECISSION) == 0) )
                {
                    setTotalDigits(numBase->fTotalDigits);
                    setFacetsDefined(DatatypeValidator::FACET_PRECISSION);
                }

                // inherit fractionDigits
                if ( (( numBase->getFacetsDefined() & DatatypeValidator::FACET_SCALE) != 0) &&
                     (( getFacetsDefined() & DatatypeValidator::FACET_SCALE) == 0) )
                {
                    setFractionDigits(numBase->fFractionDigits);
                    setFacetsDefined(DatatypeValidator::FACET_SCALE);
                }

        } //if baseValidator

    }// End of Facet setting

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
        if (getRegex() ==0)
            setRegex(new RegularExpression(getPattern(), SchemaSymbols::fgRegEx_XOption));

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


    if ( (getFacetsDefined() & DatatypeValidator::FACET_SCALE) != 0 )
    {
        if ( theData->getScale() > fFractionDigits )
            ThrowXML(InvalidDatatypeValueException, XMLExcepts::FACET_Len_maxLen);
            //ThrowXML(InvalidDatatypeValueException, XMLExcepts::VALUE_exceed_scale);
            // "'" + content + "'" + " with totalDigits = '"+ totalDigits +"'"
            // "'" + fTotalDigits + "'"} ));
    }

    if ( (getFacetsDefined() & DatatypeValidator::FACET_PRECISSION) != 0 )
    {
        if ( theData->getTotalDigit() > fTotalDigits )
            ThrowXML(InvalidDatatypeValueException, XMLExcepts::FACET_Len_maxLen);
            //ThrowXML(InvalidDatatypeValueException, XMLExcepts::VALUE_exceed_precision);
            // "'" + content + "'" + " with totalDigits = '"+ totalDigits +"'"
            // "'" + fTotalDigits + "'"} ));
    }

    if ( (getFacetsDefined() & DatatypeValidator::FACET_MAXEXCLUSIVE) != 0 )
    {
        // must be < MaxExclusive
        if (XMLBigDecimal::compareValues(theData, getMaxExclusive()) != -1)
            ThrowXML(InvalidDatatypeValueException, XMLExcepts::FACET_Len_maxLen);
            //ThrowXML(InvalidDatatypeValueException, XMLExcepts::VALUE_exceed_maxexc);
    }

    if ( (getFacetsDefined() & DatatypeValidator::FACET_MAXINCLUSIVE) != 0 )
    {
        // must be <= MaxInclusive
        if (XMLBigDecimal::compareValues(theData, getMaxInclusive()) == 1)
            ThrowXML(InvalidDatatypeValueException, XMLExcepts::FACET_Len_maxLen);
            //ThrowXML(InvalidDatatypeValueException, XMLExcepts::VALUE_exceed_maxinc);
    }

    if ( (getFacetsDefined() & DatatypeValidator::FACET_MININCLUSIVE) != 0 )
    {
        // must be >= MinInclusive
        if (XMLBigDecimal::compareValues(theData, getMinInclusive()) == -1)
            ThrowXML(InvalidDatatypeValueException, XMLExcepts::FACET_Len_maxLen);
            //ThrowXML(InvalidDatatypeValueException, XMLExcepts::VALUE_exceed_mininc);
    }

    if ( (getFacetsDefined() & DatatypeValidator::FACET_MINEXCLUSIVE) != 0 )
    {
        // must be > MinExclusive
        if (XMLBigDecimal::compareValues(theData, getMinExclusive()) != 1)
            ThrowXML(InvalidDatatypeValueException, XMLExcepts::FACET_Len_maxLen);
            //ThrowXML(InvalidDatatypeValueException, XMLExcepts::VALUE_exceed_mininc);
    }

}



// ---------------------------------------------------------------------------
//  Whitespace handling methods
// ---------------------------------------------------------------------------


/**
  * End of file DecimalDatatypeValidator::cpp
  */

