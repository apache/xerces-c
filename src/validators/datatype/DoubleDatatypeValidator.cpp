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
 * Revision 1.3  2001/08/21 18:42:53  peiyongz
 * Bugzilla# 2816: cleanUp() declared with external linkage and called
 *                          before defined as inline
 *
 * Revision 1.2  2001/08/15 18:08:44  peiyongz
 * Fix to potential leakage in strEnumeration
 *
 * Revision 1.1  2001/07/24 13:59:03  peiyongz
 * DoubleDTV
 *
 */

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <validators/datatype/DoubleDatatypeValidator.hpp>
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
DoubleDatatypeValidator::DoubleDatatypeValidator()
:DatatypeValidator(0, 0, 0, DatatypeValidator::Double)
, fEnumerationInherited(false)
, fMaxInclusive(0)
, fMaxExclusive(0)
, fMinInclusive(0)
, fMinExclusive(0)
, fEnumeration(0)
{
}

DoubleDatatypeValidator::~DoubleDatatypeValidator()
{
    cleanUp();
}

DoubleDatatypeValidator::DoubleDatatypeValidator(
                          DatatypeValidator*            const baseValidator
                        , RefHashTableOf<KVStringPair>* const facets
                        , RefVectorOf<XMLCh>*           const enums
                        , const int                           finalSet)
:DatatypeValidator(baseValidator, facets, finalSet, DatatypeValidator::Double)
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

void DoubleDatatypeValidator::init(DatatypeValidator*            const baseValidator
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
                    setMaxInclusive(new XMLDouble(value));
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
                     setMaxExclusive(new XMLDouble(value));
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
                    setMinInclusive(new XMLDouble(value));
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
                    setMinExclusive(new XMLDouble(value));
                }
                catch (NumberFormatException)
                {
                    ThrowXML1(InvalidDatatypeFacetException, XMLExcepts::FACET_Invalid_MinExcl, value);
                }
                setFacetsDefined(DatatypeValidator::FACET_MINEXCLUSIVE);
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
                    if ( XMLDouble::compareValues(getMinInclusive(), getMaxInclusive()) == 1 )
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
                    if ( XMLDouble::compareValues(getMinExclusive(), getMaxExclusive()) == 1 )
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
                    if ( XMLDouble::compareValues(getMinExclusive(), getMaxInclusive()) != -1 )
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
                    if ( XMLDouble::compareValues(getMinInclusive(), getMaxExclusive()) != -1 )
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

            } // if getFacetsDefined

            /***
               Schema constraint: Part II -- self vs base
            ***/

            if ( baseValidator != 0 )
            {
                DoubleDatatypeValidator* numBase = (DoubleDatatypeValidator*)baseValidator;

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
                             ( XMLDouble::compareValues(getMaxInclusive(), numBase->getMaxInclusive()) == 1 ))
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
                             ( XMLDouble::compareValues(getMaxInclusive(), numBase->getMaxExclusive()) != -1 ))
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
                             ( XMLDouble::compareValues(getMaxInclusive(), numBase->getMinInclusive()) == -1 ))
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
                             ( XMLDouble::compareValues(getMaxInclusive(), numBase->getMinExclusive() ) != 1 ))
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
                             ( XMLDouble::compareValues(getMaxInclusive(), numBase->getMaxInclusive()) != 0 ))
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
                             ( XMLDouble::compareValues(getMaxExclusive(), numBase->getMaxExclusive()) == 1 ))
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
                             ( XMLDouble::compareValues(getMaxExclusive(), numBase->getMaxInclusive()) == 1 ))
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
                             ( XMLDouble::compareValues(getMaxExclusive(), numBase->getMinExclusive() ) != 1 ))
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
                             ( XMLDouble::compareValues(getMaxExclusive(), numBase->getMinInclusive()) != 1 ))
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
                             ( XMLDouble::compareValues(getMaxExclusive(), numBase->getMaxExclusive()) != 0 ))
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
                             ( XMLDouble::compareValues(getMinExclusive(), numBase->getMinExclusive() ) == -1 ))
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
                             ( XMLDouble::compareValues(getMinExclusive(), numBase->getMaxInclusive()) == 1 ))
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
                             ( XMLDouble::compareValues(getMinExclusive(), numBase->getMinInclusive()) == -1 ))
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
                             ( XMLDouble::compareValues(getMinExclusive(), numBase->getMaxExclusive()) != -1 ))
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
                             ( XMLDouble::compareValues(getMinExclusive(), numBase->getMinExclusive() ) != 0 ))
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
                             ( XMLDouble::compareValues(getMinInclusive(), numBase->getMinInclusive()) == -1 ))
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
                             ( XMLDouble::compareValues(getMinInclusive(), numBase->getMaxInclusive()) == 1 ))
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
                             ( XMLDouble::compareValues(getMinInclusive(), numBase->getMinExclusive() ) != 1 ))
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
                             ( XMLDouble::compareValues(getMinInclusive(), numBase->getMaxExclusive()) != -1 ))
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
                             ( XMLDouble::compareValues(getMinInclusive(), numBase->getMinInclusive()) != 0 ))
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
                            fEnumeration = new RefVectorOf<XMLDouble>(enumLength, true);
                            fEnumerationInherited = false;
                            for ( i = 0; i < enumLength; i++)
                                fEnumeration->insertElementAt(new XMLDouble(fStrEnumeration->elementAt(i)), i);

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
        DoubleDatatypeValidator* numBase = (DoubleDatatypeValidator*)baseValidator;         
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
            setMaxExclusive(new XMLDouble(*(numBase->getMaxExclusive())));              
            setFacetsDefined(DatatypeValidator::FACET_MAXEXCLUSIVE);              
        }
         
        // inherit maxInclusive          
        if ((( numBase->getFacetsDefined() & DatatypeValidator::FACET_MAXINCLUSIVE) != 0) &&         
            (( getFacetsDefined() & DatatypeValidator::FACET_MAXEXCLUSIVE) == 0) &&              
            (( getFacetsDefined() & DatatypeValidator::FACET_MAXINCLUSIVE) == 0) )              
        {          
            setMaxInclusive(new XMLDouble(*(numBase->getMaxInclusive())));
            setFacetsDefined(DatatypeValidator::FACET_MAXINCLUSIVE);
        }

        // inherit minExclusive          
        if ((( numBase->getFacetsDefined() & DatatypeValidator::FACET_MINEXCLUSIVE) != 0) &&          
            (( getFacetsDefined() & DatatypeValidator::FACET_MINEXCLUSIVE) == 0) &&              
            (( getFacetsDefined() & DatatypeValidator::FACET_MININCLUSIVE) == 0) )              
        {          
            setMinExclusive(new XMLDouble(*(numBase->getMinExclusive())));              
            setFacetsDefined(DatatypeValidator::FACET_MINEXCLUSIVE);              
        }
          
        // inherit minExclusive          
        if ((( numBase->getFacetsDefined() & DatatypeValidator::FACET_MININCLUSIVE) != 0) &&          
            (( getFacetsDefined() & DatatypeValidator::FACET_MINEXCLUSIVE) == 0) &&
            (( getFacetsDefined() & DatatypeValidator::FACET_MININCLUSIVE) == 0) )              
        {          
            setMinInclusive(new XMLDouble(*(numBase->getMinInclusive())));              
            setFacetsDefined(DatatypeValidator::FACET_MININCLUSIVE);              
        }

         
        // inherit "fixed" option
        setFixed(getFixed() | numBase->getFixed());
          
    }

}

void DoubleDatatypeValidator::checkContent( const XMLCh* const content, bool asBase)
{

    char *p = XMLString::transcode(content);

    //validate against base validator if any
    DoubleDatatypeValidator *pBase = (DoubleDatatypeValidator*) this->getBaseValidator();
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
        XMLDouble theValue(content);
        XMLDouble *theData = &theValue;

        if (getEnumeration() != 0)
        {
            int i=0;
            int enumLength = getEnumeration()->size();
            for ( ; i < enumLength; i++)
            {
                if (XMLDouble::compareValues(theData, getEnumeration()->elementAt(i))==0)
                    break;
            }

            if (i == enumLength)
                ThrowXML1(InvalidDatatypeValueException, XMLExcepts::VALUE_NotIn_Enumeration, content);
        }

        if ( (getFacetsDefined() & DatatypeValidator::FACET_MAXEXCLUSIVE) != 0 )
        {
            // must be < MaxExclusive
            if (XMLDouble::compareValues(theData, getMaxExclusive()) != -1)
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
            if (XMLDouble::compareValues(theData, getMaxInclusive()) == 1)
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
            if (XMLDouble::compareValues(theData, getMinInclusive()) == -1)
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
            if (XMLDouble::compareValues(theData, getMinExclusive()) != 1)
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
  * End of file DoubleDatatypeValidator::cpp
  */

