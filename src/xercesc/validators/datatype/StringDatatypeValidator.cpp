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
 * Revision 1.13  2004/09/08 13:56:53  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.12  2004/01/29 11:51:22  cargilld
 * Code cleanup changes to get rid of various compiler diagnostic messages.
 *
 * Revision 1.11  2003/12/17 00:18:39  cargilld
 * Update to memory management so that the static memory manager (one used to call Initialize) is only for static data.
 *
 * Revision 1.10  2003/11/24 05:11:06  neilg
 * remove unused statics
 *
 * Revision 1.9  2003/10/02 19:20:42  peiyongz
 * fWhiteSpace moved to DatatypeValidator
 *
 * Revision 1.8  2003/10/01 01:09:35  knoaman
 * Refactoring of some code to improve performance.
 *
 * Revision 1.7  2003/09/30 21:31:37  peiyongz
 * Implementation of Serialization/Deserialization
 *
 * Revision 1.6  2003/09/29 21:47:35  peiyongz
 * Implementation of Serialization/Deserialization
 *
 * Revision 1.5  2003/05/15 18:53:27  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.4  2002/12/18 14:17:55  gareth
 * Fix to bug #13438. When you eant a vector that calls delete[] on its members you should use RefArrayVectorOf.
 *
 * Revision 1.3  2002/11/04 14:53:28  tng
 * C++ Namespace Support.
 *
 * Revision 1.2  2002/09/24 19:44:40  tng
 * Performance: use XMLString::equals instead of XMLString::compareString
 *
 * Revision 1.1.1.1  2002/02/01 22:22:42  peiyongz
 * sane_include
 *
 * Revision 1.17  2001/10/09 20:54:35  peiyongz
 * init(): take 1 arg.
 *
 * Revision 1.16  2001/09/27 13:51:25  peiyongz
 * DTV Reorganization: ctor/init created to be used by derived class
 *
 * Revision 1.15  2001/09/25 15:58:14  peiyongz
 * Check baseValidator in assignAdditinoalFacet() and inheritAdditionalFacet()
 *
 * Revision 1.14  2001/09/24 15:31:13  peiyongz
 * DTV Reorganization: inherit from AbstractStringValidator
 *
 * Revision 1.12  2001/08/21 18:42:53  peiyongz
 * Bugzilla# 2816: cleanUp() declared with external linkage and called
 *                          before defined as inline
 *
 * Revision 1.11  2001/06/20 17:56:09  peiyongz
 * support for "fixed" option on constrainning facets
 *
 * Revision 1.10  2001/05/29 19:49:36  tng
 * Schema: Constraint Checking Fix in datatypeValidators.  By Pei Yong Zhang.
 *
 * Revision 1.9  2001/05/28 21:11:18  tng
 * Schema: Various DatatypeValidator fix.  By Pei Yong Zhang
 *
 * Revision 1.8  2001/05/23 16:05:11  tng
 * Schema: NormalizedString fix.  By Pei Yong Zhang.
 *
 * Revision 1.7  2001/05/23 15:45:25  tng
 * Schema: NormalizedString fix.  By Pei Yong Zhang.
 *
 * Revision 1.6  2001/05/18 13:36:48  tng
 * Schema: Catch RegularExpression exception and NumberFormatException
 *
 * Revision 1.5  2001/05/18 13:23:51  tng
 * Schema: Exception messages in DatatypeValidator.  By Pei Yong Zhang.
 *
 * Revision 1.4  2001/05/16 14:33:40  tng
 * Schema Constraint checking fix.
 *
 * Revision 1.3  2001/05/11 17:17:28  tng
 * Schema: DatatypeValidator fixes.  By Pei Yong Zhang.
 *
 * Revision 1.2  2001/05/11 13:27:29  tng
 * Copyright update.
 *
 * Revision 1.1  2001/05/09 18:43:42  tng
 * Add StringDatatypeValidator and BooleanDatatypeValidator.  By Pei Yong Zhang.
 *
 */

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/validators/datatype/StringDatatypeValidator.hpp>
#include <xercesc/validators/schema/SchemaSymbols.hpp>
#include <xercesc/validators/datatype/InvalidDatatypeFacetException.hpp>
#include <xercesc/validators/datatype/InvalidDatatypeValueException.hpp>

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  Constructors and Destructor
// ---------------------------------------------------------------------------
StringDatatypeValidator::StringDatatypeValidator(MemoryManager* const manager)
:AbstractStringValidator(0, 0, 0, DatatypeValidator::String, manager)
{
    setWhiteSpace(DatatypeValidator::PRESERVE);
}

StringDatatypeValidator::StringDatatypeValidator(
                          DatatypeValidator*            const baseValidator
                        , RefHashTableOf<KVStringPair>* const facets
                        , RefArrayVectorOf<XMLCh>*      const enums
                        , const int                           finalSet
                        , MemoryManager* const                manager)
:AbstractStringValidator(baseValidator, facets, finalSet, DatatypeValidator::String, manager)
{
    setWhiteSpace(DatatypeValidator::PRESERVE);
    init(enums, manager);
}

StringDatatypeValidator::~StringDatatypeValidator()
{}

DatatypeValidator* StringDatatypeValidator::newInstance
(
      RefHashTableOf<KVStringPair>* const facets
    , RefArrayVectorOf<XMLCh>* const      enums
    , const int                           finalSet
    , MemoryManager* const                manager
)
{
    return (DatatypeValidator*) new (manager) StringDatatypeValidator(this, facets, enums, finalSet, manager);
}

StringDatatypeValidator::StringDatatypeValidator(
                          DatatypeValidator*            const baseValidator
                        , RefHashTableOf<KVStringPair>* const facets
                        , const int                           finalSet
                        , const ValidatorType                 type
                        , MemoryManager* const                manager)
:AbstractStringValidator(baseValidator, facets, finalSet, type, manager)
{
    setWhiteSpace(DatatypeValidator::PRESERVE);
    // do not invoke init() here!!!
}

// ---------------------------------------------------------------------------
//  Utilities
// ---------------------------------------------------------------------------
void StringDatatypeValidator::assignAdditionalFacet(const XMLCh* const key
                                                  , const XMLCh* const value
                                                  , MemoryManager* const manager)
{
    if (XMLString::equals(key, SchemaSymbols::fgELT_WHITESPACE))
    {
        // whiteSpace = preserve | replace | collapse
        if (XMLString::equals(value, SchemaSymbols::fgWS_PRESERVE))
            setWhiteSpace(DatatypeValidator::PRESERVE);
        else if (XMLString::equals(value, SchemaSymbols::fgWS_REPLACE))
            setWhiteSpace(DatatypeValidator::REPLACE);
        else if (XMLString::equals(value, SchemaSymbols::fgWS_COLLAPSE))
            setWhiteSpace(DatatypeValidator::COLLAPSE);
        else
            ThrowXMLwithMemMgr1(InvalidDatatypeFacetException, XMLExcepts::FACET_Invalid_WS, value, manager);
        //("whiteSpace value '" + ws + "' must be one of 'preserve', 'replace', 'collapse'.");

        setFacetsDefined(DatatypeValidator::FACET_WHITESPACE);
    }
    else
    {
        ThrowXMLwithMemMgr1(InvalidDatatypeFacetException
                , XMLExcepts::FACET_Invalid_Tag
                , key
                , manager);
    }
}

void StringDatatypeValidator::inheritAdditionalFacet()
{
    StringDatatypeValidator *pBaseValidator = (StringDatatypeValidator*) getBaseValidator();

    if (!pBaseValidator)
        return;

    // inherit whitespace
    if (((pBaseValidator->getFacetsDefined() & DatatypeValidator::FACET_WHITESPACE) !=0) &&
        ((getFacetsDefined() & DatatypeValidator::FACET_WHITESPACE) == 0))
    {
        setWhiteSpace(getBaseValidator()->getWSFacet());
        setFacetsDefined(DatatypeValidator::FACET_WHITESPACE);
    }
}

void StringDatatypeValidator::checkAdditionalFacetConstraints(MemoryManager* const manager) const
{

    StringDatatypeValidator *pBaseValidator = (StringDatatypeValidator*) getBaseValidator();

    if (!pBaseValidator)
        return;

    short    thisWSFacet = getWSFacet();
    short    baseWSFacet = pBaseValidator->getWSFacet();

    // check 4.3.6.c1 error: whitespace
    if (((getFacetsDefined() & DatatypeValidator::FACET_WHITESPACE) != 0) &&
        ((pBaseValidator->getFacetsDefined() & DatatypeValidator::FACET_WHITESPACE) != 0 ))
    {
        if ((baseWSFacet == DatatypeValidator::COLLAPSE) &&
            ((thisWSFacet == DatatypeValidator::PRESERVE) ||
             (thisWSFacet == DatatypeValidator::REPLACE)))
             ThrowXMLwithMemMgr(InvalidDatatypeFacetException, XMLExcepts::FACET_WS_collapse, manager);

        if ((baseWSFacet == DatatypeValidator::REPLACE) &&
            (thisWSFacet == DatatypeValidator::PRESERVE))
            ThrowXMLwithMemMgr(InvalidDatatypeFacetException, XMLExcepts::FACET_WS_replace, manager);

        if (((pBaseValidator->getFixed() & DatatypeValidator::FACET_WHITESPACE) !=0) &&
            ( thisWSFacet != baseWSFacet))
        {
            ThrowXMLwithMemMgr2(InvalidDatatypeFacetException
                        , XMLExcepts::FACET_whitespace_base_fixed
                        , getWSstring(thisWSFacet)
                        , getWSstring(baseWSFacet)
                        , manager);
        }
    }

}

void StringDatatypeValidator::checkAdditionalFacet(const XMLCh* const content
                                                   , MemoryManager* const manager) const
{
    //
    // check WhiteSpace
    //
    if ((getFacetsDefined() & DatatypeValidator::FACET_WHITESPACE) != 0 )
    {
        if ( getWSFacet() == DatatypeValidator::REPLACE )
        {
            if (!XMLString::isWSReplaced(content))
                ThrowXMLwithMemMgr1(InvalidDatatypeValueException, XMLExcepts::VALUE_WS_replaced, content, manager);
        }
        else if ( getWSFacet() == DatatypeValidator::COLLAPSE )
        {
            if (!XMLString::isWSCollapsed(content))
                ThrowXMLwithMemMgr1(InvalidDatatypeValueException, XMLExcepts::VALUE_WS_collapsed, content, manager);
        }

    }
}

void StringDatatypeValidator::checkValueSpace(const XMLCh* const
                                              , MemoryManager* const)
{}

/***
 * Support for Serialization/De-serialization
 ***/

IMPL_XSERIALIZABLE_TOCREATE(StringDatatypeValidator)

void StringDatatypeValidator::serialize(XSerializeEngine& serEng)
{
    AbstractStringValidator::serialize(serEng);
}

XERCES_CPP_NAMESPACE_END

/**
  * End of file StringDatatypeValidator.cpp
  */
