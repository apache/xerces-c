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
 * Revision 1.6  2002/11/26 22:19:15  peiyongz
 * Schema Errata E2-16 maxExclusive
 *
 * Revision 1.5  2002/11/04 14:53:27  tng
 * C++ Namespace Support.
 *
 * Revision 1.4  2002/09/24 19:44:40  tng
 * Performance: use XMLString::equals instead of XMLString::compareString
 *
 * Revision 1.3  2002/04/01 20:17:46  peiyongz
 * Bug#7551: Exceptions are caught by value, rather than by reference
 *
 * Revision 1.2  2002/02/14 15:17:31  peiyongz
 * getEnumString()
 *
 * Revision 1.1.1.1  2002/02/01 22:22:39  peiyongz
 * sane_include
 *
 * Revision 1.6  2001/12/13 16:48:29  peiyongz
 * Avoid dangling pointer
 *
 * Revision 1.5  2001/12/10 22:13:21  peiyongz
 * swap checking to avoid "dangling pointer" reported by BoundsChecker
 *
 * Revision 1.4  2001/11/12 20:37:57  peiyongz
 * SchemaDateTimeException defined
 *
 * Revision 1.3  2001/10/16 16:29:34  tng
 * Schema: Fix Typo
 *
 * Revision 1.2  2001/10/09 21:17:41  peiyongz
 * . macro to simplify code
 * .save get***() to temp vars.
 *
 * Revision 1.1  2001/10/01 16:13:56  peiyongz
 * DTV Reorganization:new classes: AbstractNumericFactValidator/ AbstractNumericValidator
 *
 */

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/validators/datatype/AbstractNumericFacetValidator.hpp>
#include <xercesc/validators/schema/SchemaSymbols.hpp>
#include <xercesc/validators/datatype/InvalidDatatypeFacetException.hpp>
#include <xercesc/util/NumberFormatException.hpp>

XERCES_CPP_NAMESPACE_BEGIN

const int AbstractNumericFacetValidator::INDETERMINATE = 2;

#define  REPORT_FACET_ERROR(val1, val2, except_code)    \
  XMLCh* value1 = (val1)->toString();                   \
  ArrayJanitor<XMLCh> jan1(value1);                     \
  XMLCh* value2 = (val2)->toString();                   \
  ArrayJanitor<XMLCh> jan2(value2);                     \
  ThrowXML2(InvalidDatatypeFacetException               \
          , except_code                                 \
          , value2                                      \
          , value1);


#define  FROM_BASE_VALUE_SPACE(val, facetFlag, except_code)   \
  if ((thisFacetsDefined & facetFlag) != 0)                   \
{                                                             \
    XMLCh* value1 = (val)->toString();                        \
    ArrayJanitor<XMLCh> jan(value1);                          \
    try                                                       \
{                                                             \
        numBase->checkContent(value1, false);                 \
}                                                             \
    catch ( XMLException& )                                   \
{                                                             \
        ThrowXML1(InvalidDatatypeFacetException               \
                , except_code                                 \
                , value1);                                    \
}                                                             \
}


// ---------------------------------------------------------------------------
//  Constructors and Destructor
// ---------------------------------------------------------------------------
AbstractNumericFacetValidator::~AbstractNumericFacetValidator()
{
    if (!fMaxInclusiveInherited && fMaxInclusive)
        delete fMaxInclusive;

    if (!fMaxExclusiveInherited && fMaxExclusive)
        delete fMaxExclusive;

    if (!fMinInclusiveInherited && fMinInclusive)
        delete fMinInclusive;

    if (!fMinExclusiveInherited && fMinExclusive)
        delete fMinExclusive;

    //~RefVectorOf will delete all adopted elements
    if (!fEnumerationInherited &&  fEnumeration)
        delete fEnumeration;

    if (!fEnumerationInherited &&  fStrEnumeration)
        delete fStrEnumeration;
}

AbstractNumericFacetValidator::AbstractNumericFacetValidator(
                          DatatypeValidator*            const baseValidator
                        , RefHashTableOf<KVStringPair>* const facets
                        , const int                           finalSet
                        , const ValidatorType                 type)
:DatatypeValidator(baseValidator, facets, finalSet, type)
, fMaxInclusiveInherited(false)
, fMaxExclusiveInherited(false)
, fMinInclusiveInherited(false)
, fMinExclusiveInherited(false)
, fEnumerationInherited(false)
, fMaxInclusive(0)
, fMaxExclusive(0)
, fMinInclusive(0)
, fMinExclusive(0)
, fEnumeration(0)
, fStrEnumeration(0)
{
    //do not invoke init() here !!!
}

//
//  P1. Enumeration
//
void AbstractNumericFacetValidator::init(RefVectorOf<XMLCh>* const enums)
{

    fStrEnumeration = enums; // save the literal value
	                         // which is needed for getEnumString()

    if (enums)
    {
        setFacetsDefined(DatatypeValidator::FACET_ENUMERATION);
    }

    assignFacet();
    inspectFacet();
    inspectFacetBase();
    inheritFacet();
}

//
//   Assign facets
//        assign common facets
//        assign additional facet
//
void AbstractNumericFacetValidator::assignFacet()
{

    RefHashTableOf<KVStringPair>* facets = getFacets();

    if (!facets)     // no facets defined
        return;

    XMLCh* key;
    XMLCh* value;

    RefHashTableOfEnumerator<KVStringPair> e(facets);

    while (e.hasMoreElements())
    {
        KVStringPair pair = e.nextElement();
        key = pair.getKey();
        value = pair.getValue();

        if (XMLString::equals(key, SchemaSymbols::fgELT_PATTERN))
        {
            setPattern(value);
            if (getPattern())
                setFacetsDefined(DatatypeValidator::FACET_PATTERN);
            // do not construct regex until needed
        }
        else if (XMLString::equals(key, SchemaSymbols::fgELT_MAXINCLUSIVE))
        {
            try
            {
                setMaxInclusive(value);
            }
            catch (NumberFormatException&)
            {
                ThrowXML1(InvalidDatatypeFacetException, XMLExcepts::FACET_Invalid_MaxIncl, value);
            }
            setFacetsDefined(DatatypeValidator::FACET_MAXINCLUSIVE);
        }
        else if (XMLString::equals(key, SchemaSymbols::fgELT_MAXEXCLUSIVE))
        {
            try
            {
                setMaxExclusive(value);
            }
            catch (NumberFormatException&)
            {
                ThrowXML1(InvalidDatatypeFacetException, XMLExcepts::FACET_Invalid_MaxExcl, value);
            }
            setFacetsDefined(DatatypeValidator::FACET_MAXEXCLUSIVE);
        }
        else if (XMLString::equals(key, SchemaSymbols::fgELT_MININCLUSIVE))
        {
            try
            {
                setMinInclusive(value);
            }
            catch (NumberFormatException&)
            {
                ThrowXML1(InvalidDatatypeFacetException, XMLExcepts::FACET_Invalid_MinIncl, value);
            }
            setFacetsDefined(DatatypeValidator::FACET_MININCLUSIVE);
        }
        else if (XMLString::equals(key, SchemaSymbols::fgELT_MINEXCLUSIVE))
        {
            try
            {
                setMinExclusive(value);
            }
            catch (NumberFormatException&)
            {
                ThrowXML1(InvalidDatatypeFacetException, XMLExcepts::FACET_Invalid_MinExcl, value);
            }
            setFacetsDefined(DatatypeValidator::FACET_MINEXCLUSIVE);
        }
        else if (XMLString::equals(key, SchemaSymbols::fgATT_FIXED))
        {
            unsigned int val;
            bool         retStatus;
            try
            {
                retStatus = XMLString::textToBin(value, val);
            }
            catch (RuntimeException&)
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
            assignAdditionalFacet(key, value);
        }

    }//while

}// end of assigneFacet()

//
// Check facet among self
//         check common facets
//         check Additional Facet Constraint
//
void AbstractNumericFacetValidator::inspectFacet()
{

    int thisFacetsDefined = getFacetsDefined();
    XMLNumber *thisMaxInclusive = getMaxInclusive();
    XMLNumber *thisMaxExclusive = getMaxExclusive();
    XMLNumber *thisMinInclusive = getMinInclusive();
    XMLNumber *thisMinExclusive = getMinExclusive();

    if (!thisFacetsDefined)
        return;

    // non co-existence checking
    // check 4.3.8.c1 error: maxInclusive + maxExclusive
    if (((thisFacetsDefined & DatatypeValidator::FACET_MAXEXCLUSIVE) != 0) &&
        ((thisFacetsDefined & DatatypeValidator::FACET_MAXINCLUSIVE) != 0) )
        ThrowXML(InvalidDatatypeFacetException, XMLExcepts::FACET_max_Incl_Excl);

    // non co-existence checking
    // check 4.3.9.c1 error: minInclusive + minExclusive
    if (((thisFacetsDefined & DatatypeValidator::FACET_MINEXCLUSIVE) != 0) &&
        ((thisFacetsDefined & DatatypeValidator::FACET_MININCLUSIVE) != 0) )
        ThrowXML(InvalidDatatypeFacetException, XMLExcepts::FACET_min_Incl_Excl);

    //
    // minExclusive < minInclusive <= maxInclusive < maxExclusive
    //
    // check 4.3.7.c1 must: minInclusive <= maxInclusive
    if (((thisFacetsDefined & DatatypeValidator::FACET_MAXINCLUSIVE) != 0) &&
        ((thisFacetsDefined & DatatypeValidator::FACET_MININCLUSIVE) != 0) )
    {
        int result = compareValues(thisMinInclusive, thisMaxInclusive);
        if ( result == 1 || result == INDETERMINATE )
        {
            REPORT_FACET_ERROR(thisMinInclusive
                             , thisMaxInclusive
                             , XMLExcepts::FACET_maxIncl_minIncl)
        }
    }

    // check 4.3.8.c2 must: minExclusive <= maxExclusive
    if ( ((thisFacetsDefined & DatatypeValidator::FACET_MAXEXCLUSIVE) != 0) &&
        ((thisFacetsDefined & DatatypeValidator::FACET_MINEXCLUSIVE) != 0) )
    {
        int result = compareValues(getMinExclusive(), getMaxExclusive());
        if ( result == 1 || result == INDETERMINATE )
        {
            REPORT_FACET_ERROR(thisMinExclusive
                             , thisMaxExclusive
                             , XMLExcepts::FACET_maxExcl_minExcl)
        }
    }

    // check 4.3.9.c2 must: minExclusive < maxInclusive
    if ( ((thisFacetsDefined & DatatypeValidator::FACET_MAXINCLUSIVE) != 0) &&
        ((thisFacetsDefined & DatatypeValidator::FACET_MINEXCLUSIVE) != 0) )
    {
        int result = compareValues(getMinExclusive(), getMaxInclusive());
        if ( result != -1 )
        {
            REPORT_FACET_ERROR(thisMinExclusive
                             , thisMaxInclusive
                             , XMLExcepts::FACET_maxIncl_minExcl)
        }
    }

    // check 4.3.10.c1 must: minInclusive < maxExclusive
    if ( ((thisFacetsDefined & DatatypeValidator::FACET_MAXEXCLUSIVE) != 0) &&
        ((thisFacetsDefined & DatatypeValidator::FACET_MININCLUSIVE) != 0) )
    {
        int result = compareValues(getMinInclusive(), getMaxExclusive());
        if ( result != -1)
        {
            REPORT_FACET_ERROR(thisMinInclusive
                             , thisMaxExclusive
                             , XMLExcepts::FACET_maxExcl_minIncl)
        }
    }

    checkAdditionalFacetConstraints();

}// end of inspectFacet()

//
//  Check vs base
//         check common facets
//         check enumeration
//         check Additional Facet Constraint
//
void AbstractNumericFacetValidator::inspectFacetBase()
{

    AbstractNumericFacetValidator* numBase = (AbstractNumericFacetValidator*) getBaseValidator();
    int thisFacetsDefined = getFacetsDefined();

    if ( (!thisFacetsDefined && !fEnumeration) ||
         !numBase           )
        return;

    int baseFacetsDefined = numBase->getFacetsDefined();

    XMLNumber *thisMaxInclusive = getMaxInclusive();
    XMLNumber *thisMaxExclusive = getMaxExclusive();
    XMLNumber *thisMinInclusive = getMinInclusive();
    XMLNumber *thisMinExclusive = getMinExclusive();

    XMLNumber *baseMaxInclusive = numBase->getMaxInclusive();
    XMLNumber *baseMaxExclusive = numBase->getMaxExclusive();
    XMLNumber *baseMinInclusive = numBase->getMinInclusive();
    XMLNumber *baseMinExclusive = numBase->getMinExclusive();
    int       baseFixed = numBase->getFixed();

                //                                     this
                //                 minExclusive                          maxExclusive
                //                    minInclusive                  maxInclusive
                //
                //                                     base
                //  minExclusive                                                          maxExclusive
                //      minInclusive                                                   maxInclusive
                //

    // check 4.3.7.c2 error:
    // maxInclusive > base.maxInclusive && maxInclusive != base.maxInclusive if (base.fixed)
    // maxInclusive >= base.maxExclusive
    // maxInclusive < base.minInclusive
    // maxInclusive <= base.minExclusive

    if ((thisFacetsDefined & DatatypeValidator::FACET_MAXINCLUSIVE) != 0)
    {
        if ((baseFacetsDefined & DatatypeValidator::FACET_MAXINCLUSIVE) != 0)
        {
            int result = compareValues(thisMaxInclusive, baseMaxInclusive);

            if (((baseFixed & DatatypeValidator::FACET_MAXINCLUSIVE) != 0) &&
                 (result != 0 ))
            {
                REPORT_FACET_ERROR(baseMaxInclusive
                                 , thisMaxInclusive
                                 , XMLExcepts::FACET_maxIncl_base_fixed)
            }

            if (result == 1 || result == INDETERMINATE)
            {
                REPORT_FACET_ERROR(baseMaxInclusive
                                 , thisMaxInclusive
                                 , XMLExcepts::FACET_maxIncl_base_maxIncl)
            }

        }

        if ((baseFacetsDefined & DatatypeValidator::FACET_MAXEXCLUSIVE) != 0)
        {
            int result = compareValues(thisMaxInclusive, baseMaxExclusive);
            if (result != -1 )
            {
                REPORT_FACET_ERROR(baseMaxExclusive
                                 , thisMaxInclusive
                                 , XMLExcepts::FACET_maxIncl_base_maxExcl)
            }
        }


        if ((baseFacetsDefined & DatatypeValidator::FACET_MININCLUSIVE) != 0)
        {
            int result = compareValues(thisMaxInclusive, baseMinInclusive);
            if (result == -1 || result == INDETERMINATE)
            {
                REPORT_FACET_ERROR(baseMinInclusive
                                 , thisMaxInclusive
                                 , XMLExcepts::FACET_maxIncl_base_minIncl)
            }
        }

        if ((baseFacetsDefined & DatatypeValidator::FACET_MINEXCLUSIVE) != 0)
        {
            int result = compareValues(thisMaxInclusive, baseMinExclusive);
            if (result != 1 )
            {
                REPORT_FACET_ERROR(baseMinExclusive
                                 , thisMaxInclusive
                                 , XMLExcepts::FACET_maxIncl_base_minExcl)
            }
        }

    }

    // check 4.3.8.c3 error:
    // maxExclusive > base.maxExclusive  && maxExclusive != base.maxExclusive if (base.fixed)
    // maxExclusive > base.maxInclusive
    // maxExclusive <= base.minInclusive
    // maxExclusive <= base.minExclusive

    if ((thisFacetsDefined & DatatypeValidator::FACET_MAXEXCLUSIVE) != 0)
    {
        if (( baseFacetsDefined & DatatypeValidator::FACET_MAXEXCLUSIVE) != 0)
        {
            int result = compareValues(thisMaxExclusive, baseMaxExclusive);

            if (((baseFixed & DatatypeValidator::FACET_MAXEXCLUSIVE) != 0) &&
                 (result != 0 ))
            {
                REPORT_FACET_ERROR(baseMaxExclusive
                                 , thisMaxExclusive
                                 , XMLExcepts::FACET_maxExcl_base_fixed)
             }

            if (result == 1 || result == INDETERMINATE)
            {
                REPORT_FACET_ERROR(baseMaxExclusive
                                 , thisMaxExclusive
                                 , XMLExcepts::FACET_maxExcl_base_maxExcl)
            }

            /**
             * Schema Errata 
             * E2-16 maxExclusive
             *             
             *   derived type's maxExclusive must either be
             *   1) equal to base' maxExclusive or
             *   2) from the base type value space
             *  
             */
            if (result != 0)
            {
                FROM_BASE_VALUE_SPACE(thisMaxExclusive
                        , DatatypeValidator::FACET_MAXEXCLUSIVE
                        , XMLExcepts::FACET_maxExcl_notFromBase)
            }
        }
        else  // base has no maxExclusive
        {
            FROM_BASE_VALUE_SPACE(thisMaxExclusive
                        , DatatypeValidator::FACET_MAXEXCLUSIVE
                        , XMLExcepts::FACET_maxExcl_notFromBase)
        }

        if (( baseFacetsDefined & DatatypeValidator::FACET_MAXINCLUSIVE) != 0)
        {
            int result = compareValues(thisMaxExclusive, baseMaxInclusive);
            if (result == 1 || result == INDETERMINATE)
            {
                REPORT_FACET_ERROR(baseMaxInclusive
                                 , thisMaxExclusive
                                 , XMLExcepts::FACET_maxExcl_base_maxIncl)
            }
        }

        if (( baseFacetsDefined & DatatypeValidator::FACET_MINEXCLUSIVE) != 0)
        {
            int result = compareValues(thisMaxExclusive, baseMinExclusive);
            if (result != 1)
            {
                REPORT_FACET_ERROR(baseMinExclusive
                                 , thisMaxExclusive
                                 , XMLExcepts::FACET_maxExcl_base_minExcl)
            }
        }

        if (( baseFacetsDefined & DatatypeValidator::FACET_MININCLUSIVE) != 0)
        {
            int result = compareValues(thisMaxExclusive, baseMinInclusive);
            if (result != 1)
            {
                REPORT_FACET_ERROR(baseMinInclusive
                                 , thisMaxExclusive
                                 , XMLExcepts::FACET_maxExcl_base_minExcl)
            }
        }
    }

    // check 4.3.9.c3 error:
    // minExclusive < base.minExclusive     minExclusive != base.minExclusive if (base.fixed)
    // minExclusive > base.maxInclusive ??? minExclusive >= base.maxInclusive
    // minExclusive < base.minInclusive
    // minExclusive >= base.maxExclusive

    if ((thisFacetsDefined & DatatypeValidator::FACET_MINEXCLUSIVE) != 0)
    {
        if (( baseFacetsDefined & DatatypeValidator::FACET_MINEXCLUSIVE) != 0)
        {
            int result = compareValues(thisMinExclusive, baseMinExclusive);

            if (((baseFixed & DatatypeValidator::FACET_MINEXCLUSIVE) != 0) &&
                 (result != 0 ))
            {
                REPORT_FACET_ERROR(baseMinExclusive
                                 , thisMinExclusive
                                 , XMLExcepts::FACET_minExcl_base_fixed)
            }

            if (result == -1 || result == INDETERMINATE)
            {
                REPORT_FACET_ERROR(baseMinExclusive
                                 , thisMinExclusive
                                 , XMLExcepts::FACET_minExcl_base_minExcl)
            }

            /**
             * Schema Errata 
             * E2-16 maxExclusive
             *             
             *   derived type's minExclusive must either be
             *   1) equal to base' minxExclusive or
             *   2) from the base type value space
             *
             *  Note: deduced from, not explicitly expressed in the Errata
             */
            if (result != 0)
            {
                FROM_BASE_VALUE_SPACE(thisMinExclusive
                        , DatatypeValidator::FACET_MINEXCLUSIVE
                        , XMLExcepts::FACET_minExcl_notFromBase)
            }
        }
        else // base has no minExclusive
        {

            FROM_BASE_VALUE_SPACE(thisMinExclusive
                        , DatatypeValidator::FACET_MINEXCLUSIVE
                        , XMLExcepts::FACET_minExcl_notFromBase)
        }

        if (( baseFacetsDefined & DatatypeValidator::FACET_MAXINCLUSIVE) != 0)
        {
            int result = compareValues(thisMinExclusive, baseMaxInclusive);
            if (result == 1 || result == INDETERMINATE)
            {
                REPORT_FACET_ERROR(baseMaxInclusive
                                 , thisMinExclusive
                                 , XMLExcepts::FACET_minExcl_base_maxIncl)
            }
        }

        if (( baseFacetsDefined & DatatypeValidator::FACET_MININCLUSIVE) != 0)
        {
            int result = compareValues(thisMinExclusive, baseMinInclusive);
            if (result == -1 || result == INDETERMINATE)
            {
                REPORT_FACET_ERROR(baseMinInclusive
                                 , thisMinExclusive
                                 , XMLExcepts::FACET_minExcl_base_minIncl)
            }
        }

        if (( baseFacetsDefined & DatatypeValidator::FACET_MAXEXCLUSIVE) != 0)
        {
            int result = compareValues(thisMinExclusive, baseMaxExclusive);
            if (result != -1)
            {
                REPORT_FACET_ERROR(baseMaxExclusive
                                 , thisMinExclusive
                                 , XMLExcepts::FACET_minExcl_base_maxExcl)
            }
        }

    }

    // check 4.3.10.c2 error:
    // minInclusive < base.minInclusive   minInclusive != base.minInclusive if (base.fixed)
    // minInclusive > base.maxInclusive
    // minInclusive <= base.minExclusive
    // minInclusive >= base.maxExclusive


    if ((thisFacetsDefined & DatatypeValidator::FACET_MININCLUSIVE) != 0)
    {
        if ((baseFacetsDefined & DatatypeValidator::FACET_MININCLUSIVE) != 0)
        {
            int result = compareValues(thisMinInclusive, baseMinInclusive);

            if (((baseFixed & DatatypeValidator::FACET_MININCLUSIVE) != 0) &&
                 (result != 0 ))
            {
                REPORT_FACET_ERROR(baseMinInclusive
                                 , thisMinInclusive
                                 , XMLExcepts::FACET_minIncl_base_fixed)
            }

            if (result == -1 || result == INDETERMINATE)
            {
                REPORT_FACET_ERROR(baseMinInclusive
                                 , thisMinInclusive
                                 , XMLExcepts::FACET_minIncl_base_minIncl)
            }
        }

        if (( baseFacetsDefined & DatatypeValidator::FACET_MAXINCLUSIVE) != 0)
        {
            int result = compareValues(thisMinInclusive, baseMaxInclusive);
            if (result == 1 || result == INDETERMINATE)
            {
                REPORT_FACET_ERROR(baseMaxInclusive
                                 , thisMinInclusive
                                 , XMLExcepts::FACET_minIncl_base_maxIncl)
            }
        }

        if (( baseFacetsDefined & DatatypeValidator::FACET_MINEXCLUSIVE) != 0)
        {
            int result = compareValues(thisMinInclusive, baseMinExclusive);
            if (result != 1)
            {
                REPORT_FACET_ERROR(baseMinExclusive
                                 , thisMinInclusive
                                 , XMLExcepts::FACET_minIncl_base_minExcl)
            }
        }

        if (( baseFacetsDefined & DatatypeValidator::FACET_MAXEXCLUSIVE) != 0)
        {
            int result = compareValues(thisMinInclusive, baseMaxExclusive);
            if (result != -1)
            {
                REPORT_FACET_ERROR(baseMaxExclusive
                                 , thisMinInclusive
                                 , XMLExcepts::FACET_minIncl_base_maxExcl)
            }
        }

    }

    checkAdditionalFacetConstraintsBase();

    // check 4.3.5.c0 must: enumeration values from the value space of base
    //
    // In fact, the values in the enumeration shall go through validation
    // of this class as well.
    // this->checkContent(value, false);
    //
    if ( ((thisFacetsDefined & DatatypeValidator::FACET_ENUMERATION) != 0) &&
        ( fStrEnumeration ))
    {
        setEnumeration();
    }

    //
    // maxInclusive, maxExclusive, minInclusive and minExclusive
    // shall come from the base's value space as well
    //

    FROM_BASE_VALUE_SPACE(thisMaxInclusive
                        , DatatypeValidator::FACET_MAXINCLUSIVE
                        , XMLExcepts::FACET_maxIncl_notFromBase)

    FROM_BASE_VALUE_SPACE(thisMinInclusive
                        , DatatypeValidator::FACET_MININCLUSIVE
                        , XMLExcepts::FACET_minIncl_notFromBase)

} //end of inspectFacetBase

//
//  Inherit facet from base
//    a. inherit common facets
//    b. inherit additional facet
//
void AbstractNumericFacetValidator::inheritFacet()
{

    AbstractNumericFacetValidator* numBase = (AbstractNumericFacetValidator*) getBaseValidator();
    if (!numBase)
        return;

    int thisFacetsDefined = getFacetsDefined();
    int baseFacetsDefined = numBase->getFacetsDefined();

    // inherit enumeration
    if ((( baseFacetsDefined & DatatypeValidator::FACET_ENUMERATION) != 0) &&
        (( thisFacetsDefined & DatatypeValidator::FACET_ENUMERATION) == 0))
    {
        fEnumeration = numBase->fEnumeration;
        fEnumerationInherited = true;
        setFacetsDefined(DatatypeValidator::FACET_ENUMERATION);
    }

    // inherit maxInclusive
    if ((( baseFacetsDefined & DatatypeValidator::FACET_MAXINCLUSIVE) != 0) &&
        (( thisFacetsDefined & DatatypeValidator::FACET_MAXEXCLUSIVE) == 0) &&
        (( thisFacetsDefined & DatatypeValidator::FACET_MAXINCLUSIVE) == 0) )
    {
        fMaxInclusive = numBase->getMaxInclusive();
        fMaxInclusiveInherited = true;
        setFacetsDefined(DatatypeValidator::FACET_MAXINCLUSIVE);
    }

    // inherit maxExclusive
    if ((( baseFacetsDefined & DatatypeValidator::FACET_MAXEXCLUSIVE) != 0) &&
        (( thisFacetsDefined & DatatypeValidator::FACET_MAXEXCLUSIVE) == 0) &&
        (( thisFacetsDefined & DatatypeValidator::FACET_MAXINCLUSIVE) == 0) )
    {
        fMaxExclusive = numBase->getMaxExclusive();
        fMaxExclusiveInherited = true;
        setFacetsDefined(DatatypeValidator::FACET_MAXEXCLUSIVE);
    }

    // inherit minExclusive
    if ((( baseFacetsDefined & DatatypeValidator::FACET_MININCLUSIVE) != 0) &&
        (( thisFacetsDefined & DatatypeValidator::FACET_MINEXCLUSIVE) == 0) &&
        (( thisFacetsDefined & DatatypeValidator::FACET_MININCLUSIVE) == 0) )
    {
        fMinInclusive = numBase->getMinInclusive();
        fMinInclusiveInherited = true;
        setFacetsDefined(DatatypeValidator::FACET_MININCLUSIVE);
    }

    // inherit minExclusive
    if ((( baseFacetsDefined & DatatypeValidator::FACET_MINEXCLUSIVE) != 0) &&
        (( thisFacetsDefined & DatatypeValidator::FACET_MINEXCLUSIVE) == 0) &&
        (( thisFacetsDefined & DatatypeValidator::FACET_MININCLUSIVE) == 0) )
    {
        fMinExclusive = numBase->getMinExclusive();
        fMinExclusiveInherited = true;
        setFacetsDefined(DatatypeValidator::FACET_MINEXCLUSIVE);
    }

    inheritAdditionalFacet();

    // inherit "fixed" option
    setFixed(getFixed() | numBase->getFixed());

}

const RefVectorOf<XMLCh>* AbstractNumericFacetValidator::getEnumString() const
{
	return (fEnumerationInherited? getBaseValidator()->getEnumString() : fStrEnumeration );
}

XERCES_CPP_NAMESPACE_END

/**
  * End of file AbstractNumericFacetValidator::cpp
  */
