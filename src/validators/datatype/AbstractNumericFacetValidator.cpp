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
 * Revision 1.1  2001/10/01 16:13:56  peiyongz
 * DTV Reorganization:new classes: AbstractNumericFactValidator/ AbstractNumericValidator
 *
 */

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <validators/datatype/AbstractNumericFacetValidator.hpp>
#include <validators/schema/SchemaSymbols.hpp>
#include <validators/datatype/InvalidDatatypeFacetException.hpp>
#include <validators/datatype/InvalidDatatypeValueException.hpp>
#include <util/NumberFormatException.hpp>

const int AbstractNumericFacetValidator::INDETERMINATE = 2;

static const int BUF_LEN = 64;
static XMLCh value1[BUF_LEN+1];
static XMLCh value2[BUF_LEN+1];

// ---------------------------------------------------------------------------
//  Constructors and Destructor
// ---------------------------------------------------------------------------
AbstractNumericFacetValidator::~AbstractNumericFacetValidator()
{
    if (fMaxInclusive && !fMaxInclusiveInherited)
        delete fMaxInclusive;

    if (fMaxExclusive && !fMaxExclusiveInherited)
        delete fMaxExclusive;

    if (fMinInclusive && !fMinInclusiveInherited)
        delete fMinInclusive;

    if (fMinExclusive && !fMinExclusiveInherited)
        delete fMinExclusive;

    //~RefVectorOf will delete all adopted elements
    if (fEnumeration && !fEnumerationInherited)
        delete fEnumeration;
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
//  P2. Facets
//   a. assign facets
//        assign common facets
//        assign additional facet
//
//   b. check facet among self
//         check common facets
//         check Additional Facet Constraint
//   c. check vs base
//         check common facets
//         check enumeration
//         check Additional Facet Constraint
//
//  P3. Inherit facet from base
//   a. inherit common facets
//   b. inherit additional facet
//
void AbstractNumericFacetValidator::init(RefVectorOf<XMLCh>* const enums)
{
    initializeValues();

    fStrEnumeration = enums; // save the literal value
    Janitor<RefVectorOf<XMLCh> >    janStrEnum(fStrEnumeration);

    if (enums)   
    {
        setFacetsDefined(DatatypeValidator::FACET_ENUMERATION);
    }

    assignFacet();
    inspectFacet();
    inspectFacetBase();
    inheritFacet();
}

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

        if (XMLString::compareString(key, SchemaSymbols::fgELT_PATTERN) == 0)
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
                setMaxInclusive(value);
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
                setMaxExclusive(value);
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
                setMinInclusive(value);
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
                setMinExclusive(value);
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
            assignAdditionalFacet(key, value);
        }

    }//while

}// end of assigneFacet()

void AbstractNumericFacetValidator::inspectFacet()
{
    
    int thisFacetsDefined = getFacetsDefined();

    if (!thisFacetsDefined)
        return;

    // non co-existence checking
    // check 4.3.8.c1 error: maxInclusive + maxExclusive
    if ( ((thisFacetsDefined & DatatypeValidator::FACET_MAXEXCLUSIVE) != 0) &&
        ((thisFacetsDefined & DatatypeValidator::FACET_MAXINCLUSIVE) != 0) )
        ThrowXML(InvalidDatatypeFacetException, XMLExcepts::FACET_max_Incl_Excl);

    // non co-existence checking
    // check 4.3.9.c1 error: minInclusive + minExclusive
    if ( ((thisFacetsDefined & DatatypeValidator::FACET_MINEXCLUSIVE) != 0) &&
        ((thisFacetsDefined & DatatypeValidator::FACET_MININCLUSIVE) != 0) )
        ThrowXML(InvalidDatatypeFacetException, XMLExcepts::FACET_min_Incl_Excl);

    //
    // minExclusive < minInclusive <= maxInclusive < maxExclusive
    //
    // check 4.3.7.c1 must: minInclusive <= maxInclusive
    if ( ((thisFacetsDefined & DatatypeValidator::FACET_MAXINCLUSIVE) != 0) &&
        ((thisFacetsDefined & DatatypeValidator::FACET_MININCLUSIVE) != 0) )
    {
        int result = compareValues(getMinInclusive(), getMaxInclusive());
        if ( result == 1 || result == INDETERMINATE )
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

    // check 4.3.8.c2 must: minExclusive <= maxExclusive
    if ( ((thisFacetsDefined & DatatypeValidator::FACET_MAXEXCLUSIVE) != 0) &&
        ((thisFacetsDefined & DatatypeValidator::FACET_MINEXCLUSIVE) != 0) )
    {
        int result = compareValues(getMinExclusive(), getMaxExclusive());
        if ( result == 1 || result == INDETERMINATE )
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
    if ( ((thisFacetsDefined & DatatypeValidator::FACET_MAXINCLUSIVE) != 0) &&
        ((thisFacetsDefined & DatatypeValidator::FACET_MINEXCLUSIVE) != 0) )
    {
        int result = compareValues(getMinExclusive(), getMaxInclusive());
        if ( result != -1 )
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
    if ( ((thisFacetsDefined & DatatypeValidator::FACET_MAXEXCLUSIVE) != 0) &&
        ((thisFacetsDefined & DatatypeValidator::FACET_MININCLUSIVE) != 0) )
    {
        int result = compareValues(getMinInclusive(), getMaxExclusive());
        if ( result != -1)
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

    checkAdditionalFacetConstraints();  

}// end of inspectFacet()

void AbstractNumericFacetValidator::inspectFacetBase()
{

    AbstractNumericFacetValidator* numBase = (AbstractNumericFacetValidator*) getBaseValidator();
    int thisFacetsDefined = getFacetsDefined();

    if ( (!thisFacetsDefined && !fEnumeration) ||
         !numBase           )
        return;
    
    int baseFacetsDefined = numBase->getFacetsDefined();

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
            int result = compareValues(getMaxInclusive(), numBase->getMaxInclusive());           
        
            if (((numBase->getFixed() & DatatypeValidator::FACET_MAXINCLUSIVE) != 0) &&
                 (result != 0 ))
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

            if (result == 1 || result == INDETERMINATE)
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

        }

        if ((baseFacetsDefined & DatatypeValidator::FACET_MAXEXCLUSIVE) != 0) 
        {
            int result = compareValues(getMaxInclusive(), numBase->getMaxExclusive());
            if (result != -1 )
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
        }


        if ((baseFacetsDefined & DatatypeValidator::FACET_MININCLUSIVE) != 0) 
        {
            int result = compareValues(getMaxInclusive(), numBase->getMinInclusive());
            if (result == -1 || result == INDETERMINATE)
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
        }

        if ((baseFacetsDefined & DatatypeValidator::FACET_MINEXCLUSIVE) != 0) 
        {
            int result = compareValues(getMaxInclusive(), numBase->getMinExclusive());
            if (result != 1 )
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
            int result = compareValues(getMaxExclusive(), numBase->getMaxExclusive());

            if (((numBase->getFixed() & DatatypeValidator::FACET_MAXEXCLUSIVE) != 0) &&
                 (result != 0 ))                
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

            if (result == 1 || result == INDETERMINATE)
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
        }


        if (( baseFacetsDefined & DatatypeValidator::FACET_MAXINCLUSIVE) != 0)
        {
            int result = compareValues(getMaxExclusive(), numBase->getMaxInclusive());
            if (result == 1 || result == INDETERMINATE)
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
        }

        if (( baseFacetsDefined & DatatypeValidator::FACET_MINEXCLUSIVE) != 0)
        {
            int result = compareValues(getMaxExclusive(), numBase->getMinExclusive());
            if (result != 1)
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
        }

        if (( baseFacetsDefined & DatatypeValidator::FACET_MININCLUSIVE) != 0)
        {
            int result = compareValues(getMaxExclusive(), numBase->getMinInclusive());
            if (result != 1)
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
            int result = compareValues(getMinExclusive(), numBase->getMinExclusive());

            if (((numBase->getFixed() & DatatypeValidator::FACET_MINEXCLUSIVE) != 0) &&
                 (result != 0 ))                
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
       
            if (result == -1 || result == INDETERMINATE)
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
        }

        if (( baseFacetsDefined & DatatypeValidator::FACET_MAXINCLUSIVE) != 0)
        {
            int result = compareValues(getMinExclusive(), numBase->getMaxInclusive());
            if (result == 1 || result == INDETERMINATE)
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
        }

        if (( baseFacetsDefined & DatatypeValidator::FACET_MININCLUSIVE) != 0)
        {
            int result = compareValues(getMinExclusive(), numBase->getMinInclusive());
            if (result == -1 || result == INDETERMINATE)
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
        }

        if (( baseFacetsDefined & DatatypeValidator::FACET_MAXEXCLUSIVE) != 0)
        {
            int result = compareValues(getMinExclusive(), numBase->getMaxExclusive());
            if (result != -1)
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
            int result = compareValues(getMinInclusive(), numBase->getMinInclusive());

            if (((numBase->getFixed() & DatatypeValidator::FACET_MININCLUSIVE) != 0) &&
                 (result != 0 ))                
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

            if (result == -1 || result == INDETERMINATE)
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
        }

        if (( baseFacetsDefined & DatatypeValidator::FACET_MAXINCLUSIVE) != 0)
        {
            int result = compareValues(getMinInclusive(), numBase->getMaxInclusive());
            if (result == 1 || result == INDETERMINATE)
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
        }

        if (( baseFacetsDefined & DatatypeValidator::FACET_MINEXCLUSIVE) != 0)
        {
            int result = compareValues(getMinInclusive(), numBase->getMinExclusive());
            if (result != 1)
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
        }

        if (( baseFacetsDefined & DatatypeValidator::FACET_MAXEXCLUSIVE) != 0)
        {
            int result = compareValues(getMinInclusive(), numBase->getMaxExclusive());
            if (result != -1)
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

    if ((thisFacetsDefined & DatatypeValidator::FACET_MAXINCLUSIVE) != 0)
    {
        XMLCh* value1 = getMaxInclusive()->toString();
        ArrayJanitor<XMLCh> jan(value1);
        try
        {
            numBase->checkContent(value1, false);
        }
        catch ( XMLException& )
        {
            ThrowXML1(InvalidDatatypeFacetException
                , XMLExcepts::FACET_maxIncl_notFromBase
                , value1
                );
        }
    }

    if ((thisFacetsDefined & DatatypeValidator::FACET_MAXEXCLUSIVE) != 0)
    {
        XMLCh* value1 = getMaxExclusive()->toString();
        ArrayJanitor<XMLCh> jan(value1);
        try
        {
            numBase->checkContent(value1, false);
        }
        catch ( XMLException& )
        {
            ThrowXML1(InvalidDatatypeFacetException
                , XMLExcepts::FACET_maxExcl_notFromBase
                , value1
                );
        }
    }

    if ((thisFacetsDefined & DatatypeValidator::FACET_MININCLUSIVE) != 0)
    {
        XMLCh* value1 = getMinInclusive()->toString();
        ArrayJanitor<XMLCh> jan(value1);
        try
        {
            numBase->checkContent(value1, false);
        }
        catch ( XMLException& )
        {
            ThrowXML1(InvalidDatatypeFacetException
                , XMLExcepts::FACET_minIncl_notFromBase
                , value1
                );
        }
    }

    if ((thisFacetsDefined & DatatypeValidator::FACET_MINEXCLUSIVE) != 0)
    {
        XMLCh* value1 = getMinExclusive()->toString();
        ArrayJanitor<XMLCh> jan(value1);
        try
        {
            numBase->checkContent(value1, false);  
        }
        catch ( XMLException& )
        {
            ThrowXML1(InvalidDatatypeFacetException
                , XMLExcepts::FACET_minExcl_notFromBase
                , value1
                );
        }
    }

} //end of inspectFacetBase


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
        (( getFacetsDefined() & DatatypeValidator::FACET_MAXEXCLUSIVE) == 0) &&              
        (( getFacetsDefined() & DatatypeValidator::FACET_MAXINCLUSIVE) == 0) )              
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

/**
  * End of file AbstractNumericFacetValidator::cpp
  */

