/*
 * Copyright 2001-2004 The Apache Software Foundation.
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
 * $Log$
 * Revision 1.28  2004/09/08 13:56:52  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.27  2004/08/17 21:09:27  peiyongz
 * canRep for nonPositivieInteger
 *
 * Revision 1.26  2004/01/13 21:18:18  peiyongz
 * revert code back to previous version
 *
 * Revision 1.25  2004/01/12 16:25:09  neilg
 * remove use of static buffers
 *
 * Revision 1.24  2004/01/06 18:13:59  peiyongz
 * using the no-exception-thrown ctor
 *
 * Revision 1.23  2004/01/06 04:42:53  neilg
 * On some platforms, it is problematic to throw a different exception from inside the catch block of another exception
 *
 * Revision 1.22  2004/01/03 00:04:36  peiyongz
 * using ctor/parseContent to avoid exception thrown from ctor
 *
 * Revision 1.21  2003/12/23 21:50:36  peiyongz
 * Absorb exception thrown in getCanonicalRepresentation and return 0,
 * only validate when required
 *
 * Revision 1.20  2003/12/19 23:02:25  cargilld
 * More memory management updates.
 *
 * Revision 1.19  2003/12/17 00:18:38  cargilld
 * Update to memory management so that the static memory manager (one used to call Initialize) is only for static data.
 *
 * Revision 1.18  2003/12/11 21:40:24  peiyongz
 * support for Canonical Representation for Datatype
 *
 * Revision 1.17  2003/11/28 18:53:07  peiyongz
 * Support for getCanonicalRepresentation
 *
 * Revision 1.16  2003/11/12 20:32:03  peiyongz
 * Statless Grammar: ValidationContext
 *
 * Revision 1.15  2003/11/06 15:30:07  neilg
 * first part of PSVI/schema component model implementation, thanks to David Cargill.  This covers setting the PSVIHandler on parser objects, as well as implementing XSNotation, XSSimpleTypeDefinition, XSIDCDefinition, and most of XSWildcard, XSComplexTypeDefinition, XSElementDeclaration, XSAttributeDeclaration and XSAttributeUse.
 *
 * Revision 1.14  2003/10/31 22:16:58  peiyongz
 * to serialize/deserialize data member
 *
 * Revision 1.13  2003/10/02 19:21:06  peiyongz
 * Implementation of Serialization/Deserialization
 *
 * Revision 1.12  2003/08/14 03:00:11  knoaman
 * Code refactoring to improve performance of validation.
 *
 * Revision 1.11  2003/05/18 14:02:07  knoaman
 * Memory manager implementation: pass per instance manager.
 *
 * Revision 1.10  2003/05/16 06:01:57  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.9  2003/05/15 18:53:26  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.8  2003/03/01 20:59:06  peiyongz
 * TotalDigits value must be a positiveInteger
 *
 * Revision 1.7  2003/02/25 17:24:37  peiyongz
 * Schema Errata: E2-44 totalDigits/fractDigits
 *
 * Revision 1.6  2003/01/30 21:56:22  tng
 * Performance: call getRawData instead of toString
 *
 * Revision 1.5  2002/12/18 14:17:55  gareth
 * Fix to bug #13438. When you eant a vector that calls delete[] on its members you should use RefArrayVectorOf.
 *
 * Revision 1.4  2002/11/04 14:53:28  tng
 * C++ Namespace Support.
 *
 * Revision 1.3  2002/09/24 19:44:40  tng
 * Performance: use XMLString::equals instead of XMLString::compareString
 *
 * Revision 1.2  2002/04/01 20:17:46  peiyongz
 * Bug#7551: Exceptions are caught by value, rather than by reference
 *
 * Revision 1.1.1.1  2002/02/01 22:22:41  peiyongz
 * sane_include
 *
 * Revision 1.19  2001/10/09 20:53:22  peiyongz
 * Optimization: save get***() to temp var
 *
 * Revision 1.18  2001/10/02 18:59:29  peiyongz
 * Invalid_Facet_Tag to display the tag name
 *
 * Revision 1.17  2001/10/01 21:04:40  peiyongz
 * DTV Reorganization:fix to memory leak in compare() method.
 *
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
#include <xercesc/validators/datatype/DecimalDatatypeValidator.hpp>
#include <xercesc/validators/datatype/XMLCanRepGroup.hpp>
#include <xercesc/validators/schema/SchemaSymbols.hpp>
#include <xercesc/validators/datatype/InvalidDatatypeFacetException.hpp>
#include <xercesc/validators/datatype/InvalidDatatypeValueException.hpp>

#include <xercesc/validators/datatype/DatatypeValidatorFactory.hpp>
#include <xercesc/util/NumberFormatException.hpp>
#include <xercesc/util/XMLBigDecimal.hpp>
#include <xercesc/util/XMLBigInteger.hpp>

XERCES_CPP_NAMESPACE_BEGIN

static const int BUF_LEN = 64;

// ---------------------------------------------------------------------------
//  Constructors and Destructor
// ---------------------------------------------------------------------------
DecimalDatatypeValidator::DecimalDatatypeValidator(MemoryManager* const manager)
:AbstractNumericValidator(0, 0, 0, DatatypeValidator::Decimal, manager)
, fTotalDigits(0)
, fFractionDigits(0)
{
    setOrdered(XSSimpleTypeDefinition::ORDERED_TOTAL);
    setNumeric(true);
}

DecimalDatatypeValidator::DecimalDatatypeValidator(
                          DatatypeValidator*            const baseValidator
                        , RefHashTableOf<KVStringPair>* const facets
                        , RefArrayVectorOf<XMLCh>*      const enums
                        , const int                           finalSet
                        , MemoryManager*                const manager)
:AbstractNumericValidator(baseValidator, facets, finalSet, DatatypeValidator::Decimal, manager)
, fTotalDigits(0)
, fFractionDigits(0)
{
    init(enums, manager);
}

DecimalDatatypeValidator::~DecimalDatatypeValidator()
{
}

// -----------------------------------------------------------------------
// Compare methods
// -----------------------------------------------------------------------
int DecimalDatatypeValidator::compare(const XMLCh* const lValue
                                    , const XMLCh* const rValue
                                    , MemoryManager* const manager)
{
    XMLBigDecimal lObj(lValue, manager);
    XMLBigDecimal rObj(rValue, manager);

    return compareValues(&lObj, &rObj);
}

DatatypeValidator* DecimalDatatypeValidator::newInstance
(
      RefHashTableOf<KVStringPair>* const facets
    , RefArrayVectorOf<XMLCh>* const      enums
    , const int                           finalSet
    , MemoryManager* const                manager
)
{
    return (DatatypeValidator*) new (manager) DecimalDatatypeValidator(this, facets, enums, finalSet, manager);
}

// -----------------------------------------------------------------------
// ctor provided to be used by derived classes
// -----------------------------------------------------------------------
DecimalDatatypeValidator::DecimalDatatypeValidator(DatatypeValidator*            const baseValidator
                                                 , RefHashTableOf<KVStringPair>* const facets
                                                 , const int                           finalSet
                                                 , const ValidatorType                 type
                                                 , MemoryManager* const                manager)
:AbstractNumericValidator(baseValidator, facets, finalSet, type, manager)
, fTotalDigits(0)
, fFractionDigits(0)
{
    //do not invoke init here !!!
}

void DecimalDatatypeValidator::assignAdditionalFacet(const XMLCh* const key
                                                   , const XMLCh* const value
                                                   , MemoryManager* const manager)
{
    if (XMLString::equals(key, SchemaSymbols::fgELT_TOTALDIGITS))
    {
        int val;
        try
        {
            val = XMLString::parseInt(value, manager);
        }
        catch (NumberFormatException&)
        {
            ThrowXMLwithMemMgr1(InvalidDatatypeFacetException, XMLExcepts::FACET_Invalid_TotalDigit, value, manager);
        }

        // check 4.3.11.c0 must: totalDigits > 0
        if ( val <= 0 )
            ThrowXMLwithMemMgr1(InvalidDatatypeFacetException, XMLExcepts::FACET_PosInt_TotalDigit, value, manager);

        setTotalDigits(val);
        setFacetsDefined(DatatypeValidator::FACET_TOTALDIGITS);
    }
    else if (XMLString::equals(key, SchemaSymbols::fgELT_FRACTIONDIGITS))
    {
        int val;
        try
        {
            val = XMLString::parseInt(value, manager);
        }
        catch (NumberFormatException&)
        {
            ThrowXMLwithMemMgr1(InvalidDatatypeFacetException, XMLExcepts::FACET_Invalid_FractDigit, value, manager);
        }

        // check 4.3.12.c0 must: fractionDigits > 0
        if ( val < 0 )
            ThrowXMLwithMemMgr1(InvalidDatatypeFacetException, XMLExcepts::FACET_NonNeg_FractDigit, value, manager);

        setFractionDigits(val);
        setFacetsDefined(DatatypeValidator::FACET_FRACTIONDIGITS);
    }
    else
    {
        ThrowXMLwithMemMgr1(InvalidDatatypeFacetException
                , XMLExcepts::FACET_Invalid_Tag
                , key
                , manager);
    }
}

void DecimalDatatypeValidator::inheritAdditionalFacet()
{

    DecimalDatatypeValidator *numBase = (DecimalDatatypeValidator*) getBaseValidator();

    if (!numBase)
        return;

    int thisFacetsDefined = getFacetsDefined();
    int baseFacetsDefined = numBase->getFacetsDefined();

    // inherit totalDigits
    if ((( baseFacetsDefined & DatatypeValidator::FACET_TOTALDIGITS) != 0) &&
        (( thisFacetsDefined & DatatypeValidator::FACET_TOTALDIGITS) == 0) )
    {
        setTotalDigits(numBase->fTotalDigits);
        setFacetsDefined(DatatypeValidator::FACET_TOTALDIGITS);
    }

    // inherit fractionDigits
    if ((( baseFacetsDefined & DatatypeValidator::FACET_FRACTIONDIGITS) != 0) &&
        (( thisFacetsDefined & DatatypeValidator::FACET_FRACTIONDIGITS) == 0) )
    {
        setFractionDigits(numBase->fFractionDigits);
        setFacetsDefined(DatatypeValidator::FACET_FRACTIONDIGITS);
    }
}

void DecimalDatatypeValidator::checkAdditionalFacetConstraints(MemoryManager* const manager) const
{
    int thisFacetsDefined = getFacetsDefined();

    // check 4.3.12.c1 must: fractionDigits <= totalDigits
    if ( ((thisFacetsDefined & DatatypeValidator::FACET_FRACTIONDIGITS) != 0) &&
         ((thisFacetsDefined & DatatypeValidator::FACET_TOTALDIGITS) != 0) )
    {
        if ( fFractionDigits > fTotalDigits )
        {
            XMLCh value1[BUF_LEN+1];
            XMLCh value2[BUF_LEN+1];
            XMLString::binToText(getFractionDigits(), value1, BUF_LEN, 10, manager);
            XMLString::binToText(getTotalDigits(), value2, BUF_LEN, 10, manager);
            ThrowXMLwithMemMgr2(InvalidDatatypeFacetException
                                 , XMLExcepts::FACET_TotDigit_FractDigit
                                 , value2
                                 , value1
                                 , manager);
        }
    }

}

void DecimalDatatypeValidator::checkAdditionalFacetConstraintsBase(MemoryManager* const manager) const
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
            XMLCh value1[BUF_LEN+1];
            XMLCh value2[BUF_LEN+1];
            XMLString::binToText(fTotalDigits, value1, BUF_LEN, 10, manager);
            XMLString::binToText(numBase->fTotalDigits, value2, BUF_LEN, 10, manager);
            ThrowXMLwithMemMgr2(InvalidDatatypeFacetException
                                 , XMLExcepts::FACET_totalDigit_base_totalDigit
                                 , value1
                                 , value2
                                 , manager);
        }

        if ( (( baseFacetsDefined & DatatypeValidator::FACET_TOTALDIGITS) != 0) &&
            (( numBase->getFixed() & DatatypeValidator::FACET_TOTALDIGITS) != 0) &&
            ( fTotalDigits != numBase->fTotalDigits ))
        {
            XMLCh value1[BUF_LEN+1];
            XMLCh value2[BUF_LEN+1];
            XMLString::binToText(fTotalDigits, value1, BUF_LEN, 10, manager);
            XMLString::binToText(numBase->fTotalDigits, value2, BUF_LEN, 10, manager);
            ThrowXMLwithMemMgr2(InvalidDatatypeFacetException
                                 , XMLExcepts::FACET_totalDigit_base_fixed
                                 , value1
                                 , value2
                                 , manager);
        }
    }

    if (( thisFacetsDefined & DatatypeValidator::FACET_FRACTIONDIGITS) != 0)
    {
        // check question error: fractionDigits > base.fractionDigits ???
        if ( (( baseFacetsDefined & DatatypeValidator::FACET_FRACTIONDIGITS) != 0) &&
            ( fFractionDigits > numBase->fFractionDigits ))
        {
            XMLCh value1[BUF_LEN+1];
            XMLCh value2[BUF_LEN+1];
            XMLString::binToText(fFractionDigits, value1, BUF_LEN, 10, manager);
            XMLString::binToText(numBase->fFractionDigits, value2, BUF_LEN, 10, manager);
            ThrowXMLwithMemMgr2(InvalidDatatypeFacetException
                                 , XMLExcepts::FACET_fractDigit_base_fractDigit
                                 , value1
                                 , value2
                                 , manager);
                        }

        // check question error: fractionDigits > base.totalDigits ???
        if ( (( baseFacetsDefined & DatatypeValidator::FACET_TOTALDIGITS) != 0) &&
            ( fFractionDigits > numBase->fTotalDigits ))
        {
            XMLCh value1[BUF_LEN+1];
            XMLCh value2[BUF_LEN+1];
            XMLString::binToText(fFractionDigits, value1, BUF_LEN, 10, manager);
            XMLString::binToText(numBase->fTotalDigits, value2, BUF_LEN, 10, manager);
            ThrowXMLwithMemMgr2(InvalidDatatypeFacetException
                                 , XMLExcepts::FACET_fractDigit_base_totalDigit
                                 , value1
                                 , value2
                                 , manager);
        }

        // fractionDigits != base.fractionDigits if (base.fixed)
        if ( (( baseFacetsDefined & DatatypeValidator::FACET_FRACTIONDIGITS) != 0) &&
            (( numBase->getFixed() & DatatypeValidator::FACET_FRACTIONDIGITS) != 0) &&
            ( fFractionDigits != numBase->fFractionDigits ))
        {
            XMLCh value1[BUF_LEN+1];
            XMLCh value2[BUF_LEN+1];
            XMLString::binToText(fFractionDigits, value1, BUF_LEN, 10, manager);
            XMLString::binToText(numBase->fFractionDigits, value2, BUF_LEN, 10, manager);
            ThrowXMLwithMemMgr2(InvalidDatatypeFacetException
                                 , XMLExcepts::FACET_fractDigit_base_fixed
                                 , value1
                                 , value2
                                 , manager);
        }
    }

}

int  DecimalDatatypeValidator::compareValues(const XMLNumber* const lValue
                                           , const XMLNumber* const rValue)
{
    return XMLBigDecimal::compareValues((XMLBigDecimal*) lValue, (XMLBigDecimal*) rValue,
                                        ((XMLBigDecimal*)lValue)->getMemoryManager());
}

void  DecimalDatatypeValidator::setMaxInclusive(const XMLCh* const value)
{
    fMaxInclusive = new (fMemoryManager) XMLBigDecimal(value, fMemoryManager);
}

void  DecimalDatatypeValidator::setMaxExclusive(const XMLCh* const value)
{
    fMaxExclusive = new (fMemoryManager) XMLBigDecimal(value, fMemoryManager);
}

void  DecimalDatatypeValidator::setMinInclusive(const XMLCh* const value)
{
    fMinInclusive = new (fMemoryManager) XMLBigDecimal(value, fMemoryManager);
}

void  DecimalDatatypeValidator::setMinExclusive(const XMLCh* const value)
{
    fMinExclusive = new (fMemoryManager) XMLBigDecimal(value, fMemoryManager);
}

void DecimalDatatypeValidator::setEnumeration(MemoryManager* const manager)
{
    // check 4.3.5.c0 must: enumeration values from the value space of base
    //
    // 1. shall be from base value space
    // 2. shall be from current value space as well ( shall go through boundsCheck() )
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
                numBase->checkContent(fStrEnumeration->elementAt(i), (ValidationContext*)0, false, manager);
            }
        }
        catch (XMLException&)
        {
            ThrowXMLwithMemMgr1(InvalidDatatypeFacetException
                    , XMLExcepts::FACET_enum_base
                    , fStrEnumeration->elementAt(i)
                    , manager);
        }
    }

    // We put the this->checkContent in a separate loop
    // to not block original message with in that method.
    //
    for ( i = 0; i < enumLength; i++)
    {
        checkContent(fStrEnumeration->elementAt(i), (ValidationContext*)0, false, manager);
    }

    fEnumeration = new (manager) RefVectorOf<XMLNumber>(enumLength, true, manager);
    fEnumerationInherited = false;

    for ( i = 0; i < enumLength; i++)
    {
        fEnumeration->insertElementAt(new (manager) XMLBigDecimal(fStrEnumeration->elementAt(i), manager), i);        
    }

}

// -----------------------------------------------------------------------
// Abstract interface from AbstractNumericValidator
// -----------------------------------------------------------------------
void DecimalDatatypeValidator::checkContent(const XMLCh*             const content
                                           ,      ValidationContext* const context
                                           ,      bool                     asBase
                                           ,      MemoryManager*     const manager)
{
    //validate against base validator if any
    DecimalDatatypeValidator *pBase = (DecimalDatatypeValidator*) this->getBaseValidator();
    if (pBase)
        pBase->checkContent(content, context, true, manager);

    int thisFacetsDefined = getFacetsDefined();

    // we check pattern first
    if ( (thisFacetsDefined & DatatypeValidator::FACET_PATTERN ) != 0 )
    {
        // lazy construction
        if (getRegex() ==0) {
            try {
                // REVISIT: cargillmem fMemoryManager vs manager
                setRegex(new (fMemoryManager) RegularExpression(getPattern(), SchemaSymbols::fgRegEx_XOption, fMemoryManager));                
            }
            catch (XMLException &e)
            {
                ThrowXMLwithMemMgr1(InvalidDatatypeValueException, XMLExcepts::RethrowError, e.getMessage(), manager);
            }
        }

        if (getRegex()->matches(content, manager) ==false)
        {
            ThrowXMLwithMemMgr2(InvalidDatatypeValueException
                    , XMLExcepts::VALUE_NotMatch_Pattern
                    , content
                    , getPattern()
                    , manager);
        }
    }

    // if this is a base validator, we only need to check pattern facet
    // all other facet were inherited by the derived type
    if (asBase)
        return;
    XMLCh *errorMsg = 0;
    try {
        XMLBigDecimal  compareDataValue(content, manager);
        XMLBigDecimal* compareData = &compareDataValue;        
        
        if (getEnumeration())
        {
            int i=0;
            int enumLength = getEnumeration()->size();
            for ( ; i < enumLength; i++)
            {
                if (compareValues(compareData, (XMLBigDecimal*) getEnumeration()->elementAt(i)) ==0 )
                    break;
            }

            if (i == enumLength)
                ThrowXMLwithMemMgr1(InvalidDatatypeValueException, XMLExcepts::VALUE_NotIn_Enumeration, content, manager);
        }

        boundsCheck(compareData, manager);

        if ( (thisFacetsDefined & DatatypeValidator::FACET_FRACTIONDIGITS) != 0 )
        {
            if ( compareData->getScale() > fFractionDigits )
            {                
                XMLCh value1[BUF_LEN+1];
                XMLCh value2[BUF_LEN+1];
                XMLString::binToText(compareData->getScale(), value1, BUF_LEN, 10, manager);
                XMLString::binToText(fFractionDigits, value2, BUF_LEN, 10, manager);
                ThrowXMLwithMemMgr3(InvalidDatatypeFacetException
                                 , XMLExcepts::VALUE_exceed_fractDigit
                                 , compareData->getRawData()
                                 , value1
                                 , value2
                                 , manager);
            }
        }

        if ( (thisFacetsDefined & DatatypeValidator::FACET_TOTALDIGITS) != 0 )
        {
            if ( compareData->getTotalDigit() > fTotalDigits )
            {                
                XMLCh value1[BUF_LEN+1];
                XMLCh value2[BUF_LEN+1];
                XMLString::binToText(compareData->getTotalDigit(), value1, BUF_LEN, 10, manager);
                XMLString::binToText(fTotalDigits, value2, BUF_LEN, 10, manager);
                ThrowXMLwithMemMgr3(InvalidDatatypeFacetException
                                 , XMLExcepts::VALUE_exceed_totalDigit
                                 , compareData->getRawData()
                                 , value1
                                 , value2
                                 , manager);
            }

            /***
             E2-44 totalDigits

             ... by restricting it to numbers that are expressible as i × 10^-n
             where i and n are integers such that |i| < 10^totalDigits and 0 <= n <= totalDigits.
            ***/

            if ( compareData->getScale() > fTotalDigits )  
            {                
                XMLCh value1[BUF_LEN+1];
                XMLCh value2[BUF_LEN+1];
                XMLString::binToText(compareData->getScale(), value1, BUF_LEN, 10, manager);
                XMLString::binToText(fTotalDigits, value2, BUF_LEN, 10, manager);
                ThrowXMLwithMemMgr3(InvalidDatatypeFacetException
                                 , XMLExcepts::VALUE_exceed_totalDigit
                                 , compareData->getRawData()
                                 , value1
                                 , value2
                                 , manager);
            }        
        }
    }
    catch (XMLException &e)
    {
       errorMsg = XMLString::replicate(e.getMessage(), manager);
    }
    if(errorMsg)
    {
       ArrayJanitor<XMLCh> jan(errorMsg, manager);
       ThrowXMLwithMemMgr1(InvalidDatatypeFacetException, XMLExcepts::RethrowError, errorMsg, manager);
    }
}

/***
 * 3.2.3 decimal  
 *
 * . the preceding optional "+" sign is prohibited. 
 * . The decimal point is required. 
 * . Leading and trailing zeroes are prohibited subject to the following: 
 *   there must be at least one digit to the right and to the left of the decimal point which may be a zero.
 *
 *
 *  3.3.13 integer
 *  3.3.16 long
 *  3.3.17 int
 *  3.3.18 short 
 *  3.3.19 byte
 *  3.3.20 nonNegativeInteger
 *  3.3.25 positiveInteger
 *
 *   . the preceding optional "+" sign is prohibited and
 *   . leading zeroes are prohibited.
 *   
 *
 *  E2-27
 *  3.3.14 nonPositiveInteger
 *
 *   . In the canonical form for zero, the sign must be omitted. 
 *   . leading zeroes are prohibited.
 *  
 *  3.3.15 negativeInteger
 *  3.3.21 unsignedLong
 *  3.3.22 unsignedInt
 *  3.3.23 unsignedShort
 *  3.3.24 unsignedByte
 *
 *  . leading zeroes are prohibited.
 *  
 *  Summary:
 *  . leading zeros are prohibited for all three groups
 *  . '-' is required for nonPositiveInteger if it is zero
 *
 ***/

const XMLCh* DecimalDatatypeValidator::getCanonicalRepresentation(const XMLCh*         const rawData
                                                                 ,      MemoryManager* const memMgr
                                                                 ,      bool                 toValidate) const
{
    MemoryManager* toUse = memMgr? memMgr : fMemoryManager;
    DecimalDatatypeValidator* temp = (DecimalDatatypeValidator*) this;

    if (toValidate)
    {
        try
        {
            temp->checkContent(rawData, 0, false, toUse);   
        }
        catch (...)
        {
            return 0;
        }
    }

    // XMLBigInteger::getCanonicalRepresentation and
    // XMLBigDecimal::getCanonicalRepresentation will handle exceptional cases
    XMLCanRepGroup::CanRepGroup dvType = DatatypeValidatorFactory::getCanRepGroup(temp);

    if ((dvType == XMLCanRepGroup::Decimal_Derivated_signed)   ||
        (dvType == XMLCanRepGroup::Decimal_Derivated_unsigned) ||
        (dvType == XMLCanRepGroup::Decimal_Derivated_npi)        )
    {          
        return XMLBigInteger::getCanonicalRepresentation(rawData, toUse, dvType == XMLCanRepGroup::Decimal_Derivated_npi);
    }
    else if (dvType == XMLCanRepGroup::Decimal) 
    {
        return XMLBigDecimal::getCanonicalRepresentation(rawData, toUse);
    }
    else //in case?
    {
        return XMLString::replicate(rawData, toUse);
    }

}

/***
 * Support for Serialization/De-serialization
 ***/

IMPL_XSERIALIZABLE_TOCREATE(DecimalDatatypeValidator)

void DecimalDatatypeValidator::serialize(XSerializeEngine& serEng)
{
    /***
     * Note: 
     *
     *     During storing, we need write the specific number
     *     type info before calling base::serialize().
     *
     *     While loading, we do nothing here
     ***/

    if (serEng.isStoring())
    {
        serEng<<(int) (XMLNumber::BigDecimal);
    }

    AbstractNumericValidator::serialize(serEng);

    //don't serialize XMLBigDecimal*       
    if (serEng.isStoring())
    {
        serEng<<fTotalDigits;
        serEng<<fFractionDigits;
    }
    else
    {
        serEng>>fTotalDigits;
        serEng>>fFractionDigits;
    }

}

XERCES_CPP_NAMESPACE_END

/**
  * End of file DecimalDatatypeValidator::cpp
  */
