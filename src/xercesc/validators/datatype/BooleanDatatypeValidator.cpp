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
 * $Log$
 * Revision 1.18  2004/09/08 13:56:52  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.17  2004/08/11 16:52:59  peiyongz
 * using Literal constants from XMLUni
 *
 * Revision 1.16  2004/01/29 11:51:22  cargilld
 * Code cleanup changes to get rid of various compiler diagnostic messages.
 *
 * Revision 1.15  2004/01/13 20:57:06  peiyongz
 * revert code back to previous version
 *
 * Revision 1.13  2003/12/23 21:50:36  peiyongz
 * Absorb exception thrown in getCanonicalRepresentation and return 0,
 * only validate when required
 *
 * Revision 1.12  2003/12/17 00:18:38  cargilld
 * Update to memory management so that the static memory manager (one used to call Initialize) is only for static data.
 *
 * Revision 1.11  2003/11/28 18:53:07  peiyongz
 * Support for getCanonicalRepresentation
 *
 * Revision 1.10  2003/11/12 20:32:03  peiyongz
 * Statless Grammar: ValidationContext
 *
 * Revision 1.9  2003/10/07 19:39:03  peiyongz
 * Implementation of Serialization/Deserialization
 *
 * Revision 1.8  2003/05/16 06:01:57  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.7  2003/05/15 18:53:26  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.6  2002/12/18 14:17:55  gareth
 * Fix to bug #13438. When you eant a vector that calls delete[] on its members you should use RefArrayVectorOf.
 *
 * Revision 1.5  2002/11/04 14:53:28  tng
 * C++ Namespace Support.
 *
 * Revision 1.4  2002/09/24 19:44:40  tng
 * Performance: use XMLString::equals instead of XMLString::compareString
 *
 * Revision 1.3  2002/08/23 20:16:50  tng
 * Memory leak fix: enums is not deleted if an error occurred.
 *
 * Revision 1.2  2002/02/14 15:17:31  peiyongz
 * getEnumString()
 *
 * Revision 1.1.1.1  2002/02/01 22:22:40  peiyongz
 * sane_include
 *
 * Revision 1.7  2001/10/02 18:59:29  peiyongz
 * Invalid_Facet_Tag to display the tag name
 *
 * Revision 1.6  2001/06/22 16:26:01  peiyongz
 * fix: redefinition of SIZE
 *
 * Revision 1.5  2001/05/29 19:49:33  tng
 * Schema: Constraint Checking Fix in datatypeValidators.  By Pei Yong Zhang.
 *
 * Revision 1.4  2001/05/28 21:11:17  tng
 * Schema: Various DatatypeValidator fix.  By Pei Yong Zhang
 *
 * Revision 1.3  2001/05/18 13:36:44  tng
 * Schema: Catch RegularExpression exception and NumberFormatException
 *
 * Revision 1.2  2001/05/11 13:27:26  tng
 * Copyright update.
 *
 * Revision 1.1  2001/05/09 18:43:35  tng
 * Add StringDatatypeValidator and BooleanDatatypeValidator.  By Pei Yong Zhang.
 *
 */

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/validators/datatype/BooleanDatatypeValidator.hpp>
#include <xercesc/validators/schema/SchemaSymbols.hpp>
#include <xercesc/validators/datatype/InvalidDatatypeFacetException.hpp>
#include <xercesc/validators/datatype/InvalidDatatypeValueException.hpp>

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  Constructors and Destructor
// ---------------------------------------------------------------------------
BooleanDatatypeValidator::BooleanDatatypeValidator(
                          DatatypeValidator*            const baseValidator
                        , RefHashTableOf<KVStringPair>* const facets
                        , RefArrayVectorOf<XMLCh>*      const enums
                        , const int                           finalSet
                        , MemoryManager* const                manager)
:DatatypeValidator(baseValidator, facets, finalSet, DatatypeValidator::Boolean, manager)
{

    // Set Facets if any defined
    if ( facets )
    {

        // Boolean shall NOT have enumeration
        if (enums) {
            delete enums;
            ThrowXMLwithMemMgr1(InvalidDatatypeFacetException
                    , XMLExcepts::FACET_Invalid_Tag
                    , "enumeration"
                    , manager);
        }

        XMLCh* key;
        XMLCh* value;
        RefHashTableOfEnumerator<KVStringPair> e(facets, false, manager);

        while (e.hasMoreElements())
        {
            KVStringPair pair = e.nextElement();
            key = pair.getKey();
            value = pair.getValue();

            if (XMLString::equals(key, SchemaSymbols::fgELT_PATTERN))
            {
                setPattern(value);
                setFacetsDefined(DatatypeValidator::FACET_PATTERN);
            }
            else
            {
                ThrowXMLwithMemMgr1(InvalidDatatypeFacetException
                        , XMLExcepts::FACET_Invalid_Tag
                        , key
                        , manager);
            }

        }

    }// End of facet setting
}

void BooleanDatatypeValidator::checkContent( const XMLCh*             const content
                                           ,       ValidationContext* const context
                                           ,       bool                     asBase
                                           ,       MemoryManager*     const manager)
{

    //validate against base validator if any
    BooleanDatatypeValidator *pBaseValidator = (BooleanDatatypeValidator*) this->getBaseValidator();
    if (pBaseValidator !=0)
        pBaseValidator->checkContent(content, context, true, manager);

    // we check pattern first
    if ( (getFacetsDefined() & DatatypeValidator::FACET_PATTERN ) != 0 )
    {
        // lazy construction
        if (getRegex() ==0) {
            try {
                setRegex(new (fMemoryManager) RegularExpression(getPattern(), SchemaSymbols::fgRegEx_XOption, fMemoryManager));
            }
            catch (XMLException &e)
            {
                ThrowXMLwithMemMgr1(InvalidDatatypeValueException, XMLExcepts::RethrowError, e.getMessage(), fMemoryManager);
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

    unsigned int   i = 0;
    for ( ; i < XMLUni::fgBooleanValueSpaceArraySize; i++ )
    {
        if ( XMLString::equals(content, XMLUni::fgBooleanValueSpace[i]))
            break;
    }

    if (i == XMLUni::fgBooleanValueSpaceArraySize)
        ThrowXMLwithMemMgr(InvalidDatatypeValueException, XMLExcepts::CM_UnaryOpHadBinType, manager);
        //Not valid boolean type

}

int BooleanDatatypeValidator::compare(const XMLCh* const lValue
                                    , const XMLCh* const rValue
                                    , MemoryManager* const)
{
    // need to check by bool semantics
    // 1 == true
    // 0 == false

    if (XMLString::equals(lValue, XMLUni::fgBooleanValueSpace[0])||
        XMLString::equals(lValue, XMLUni::fgBooleanValueSpace[2]))
    {
        if (XMLString::equals(rValue, XMLUni::fgBooleanValueSpace[0]) ||
            XMLString::equals(rValue, XMLUni::fgBooleanValueSpace[2]))
            return 0;
    }
    else
    if (XMLString::equals(lValue, XMLUni::fgBooleanValueSpace[1]) ||
        XMLString::equals(lValue, XMLUni::fgBooleanValueSpace[3]))
    {
        if (XMLString::equals(rValue, XMLUni::fgBooleanValueSpace[1]) ||
            XMLString::equals(rValue, XMLUni::fgBooleanValueSpace[3]))
            return 0;
    }

    return 1;
}

const RefArrayVectorOf<XMLCh>* BooleanDatatypeValidator::getEnumString() const
{
	return 0;
}

/***
 * 3.2.2.2 Canonical representation
 *
 * The canonical representation for boolean is the set of literals {true, false}.
 ***/
const XMLCh* BooleanDatatypeValidator::getCanonicalRepresentation(const XMLCh*         const rawData
                                                                ,       MemoryManager* const memMgr
                                                                ,       bool           toValidate) const
{

    MemoryManager* toUse = memMgr? memMgr : getMemoryManager();
    
    if (toValidate)
    {
        BooleanDatatypeValidator *temp = (BooleanDatatypeValidator*) this;

        try
        {
            temp->checkContent(rawData, 0, false, toUse);   
        }
        catch (...)
        {
            return 0;
        }
    }

    return ( XMLString::equals(rawData, XMLUni::fgBooleanValueSpace[0]) ||
             XMLString::equals(rawData, XMLUni::fgBooleanValueSpace[2])  ) ?
             XMLString::replicate(XMLUni::fgBooleanValueSpace[0], toUse) :
             XMLString::replicate(XMLUni::fgBooleanValueSpace[1], toUse) ;

}

/***
 * Support for Serialization/De-serialization
 ***/

IMPL_XSERIALIZABLE_TOCREATE(BooleanDatatypeValidator)

void BooleanDatatypeValidator::serialize(XSerializeEngine& serEng)
{
    DatatypeValidator::serialize(serEng);
}

XERCES_CPP_NAMESPACE_END

/**
  * End of file BooleanDatatypeValidator.cpp
  */

