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
 * Revision 1.13  2004/12/01 17:44:20  cargilld
 * Minor code cleanup.
 *
 * Revision 1.12  2004/09/08 13:56:52  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.11  2003/12/23 21:50:36  peiyongz
 * Absorb exception thrown in getCanonicalRepresentation and return 0,
 * only validate when required
 *
 * Revision 1.10  2003/12/17 00:18:38  cargilld
 * Update to memory management so that the static memory manager (one used to call Initialize) is only for static data.
 *
 * Revision 1.9  2003/12/11 21:40:24  peiyongz
 * support for Canonical Representation for Datatype
 *
 * Revision 1.8  2003/11/28 18:53:07  peiyongz
 * Support for getCanonicalRepresentation
 *
 * Revision 1.7  2003/11/12 20:32:03  peiyongz
 * Statless Grammar: ValidationContext
 *
 * Revision 1.6  2003/10/02 19:21:06  peiyongz
 * Implementation of Serialization/Deserialization
 *
 * Revision 1.5  2003/05/15 18:53:26  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.4  2003/02/02 23:55:40  peiyongz
 * getFormattedString() added to return original and converted value.
 *
 * Revision 1.3  2003/01/30 21:56:22  tng
 * Performance: call getRawData instead of toString
 *
 * Revision 1.2  2002/11/04 14:53:27  tng
 * C++ Namespace Support.
 *
 * Revision 1.1.1.1  2002/02/01 22:22:39  peiyongz
 * sane_include
 *
 * Revision 1.2  2001/10/09 21:16:31  peiyongz
 * . macro to simplify code
 *
 * Revision 1.1  2001/10/01 16:13:56  peiyongz
 * DTV Reorganization:new classes: AbstractNumericFactValidator/ AbstractNumericValidator
 *
 */

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/validators/datatype/AbstractNumericValidator.hpp>
#include <xercesc/validators/datatype/InvalidDatatypeValueException.hpp>
#include <xercesc/util/XMLAbstractDoubleFloat.hpp>

XERCES_CPP_NAMESPACE_BEGIN

#define  REPORT_VALUE_ERROR(val1, val2, except_code, manager)    \
  ThrowXMLwithMemMgr2(InvalidDatatypeValueException               \
          , except_code                                 \
          , val1->getFormattedString()                  \
          , val2->getFormattedString()                  \
          , manager);

// ---------------------------------------------------------------------------
//  Constructors and Destructor
// ---------------------------------------------------------------------------
AbstractNumericValidator::~AbstractNumericValidator()
{}

AbstractNumericValidator::AbstractNumericValidator(
                          DatatypeValidator*            const baseValidator
                        , RefHashTableOf<KVStringPair>* const facets
                        , const int                           finalSet
                        , const ValidatorType                 type
                        , MemoryManager* const                manager)
:AbstractNumericFacetValidator(baseValidator, facets, finalSet, type, manager)
{
    //do not invoke init() here !!!
}

void AbstractNumericValidator::validate(const XMLCh*             const content
                                       ,      ValidationContext* const context
                                       ,      MemoryManager*     const manager)
{
    checkContent(content, context, false, manager);
}

void AbstractNumericValidator::boundsCheck(const XMLNumber*         const theData
                                          ,      MemoryManager*     const manager)
{
    int thisFacetsDefined = getFacetsDefined();
    int result;

    if (thisFacetsDefined == 0)
        return;

    try
    {

        // must be < MaxExclusive
        if ( (thisFacetsDefined & DatatypeValidator::FACET_MAXEXCLUSIVE) != 0 )
        {
            result = compareValues(theData, getMaxExclusive());
            if ( result != -1)
            {
                REPORT_VALUE_ERROR(theData
                                 , getMaxExclusive()
                                 , XMLExcepts::VALUE_exceed_maxExcl
                                 , manager)
            }
        } 	

        // must be <= MaxInclusive
        if ( (thisFacetsDefined & DatatypeValidator::FACET_MAXINCLUSIVE) != 0 )
        {
            result = compareValues(theData, getMaxInclusive());
            if (result == 1)
            {
                REPORT_VALUE_ERROR(theData
                                 , getMaxInclusive()
                                 , XMLExcepts::VALUE_exceed_maxIncl
                                 , manager)
            }
        }

        // must be >= MinInclusive
        if ( (thisFacetsDefined & DatatypeValidator::FACET_MININCLUSIVE) != 0 )
        {
            result = compareValues(theData, getMinInclusive());
            if (result == -1)
            {
                REPORT_VALUE_ERROR(theData
                                 , getMinInclusive()
                                 , XMLExcepts::VALUE_exceed_minIncl
                                 , manager)
            }
        }

        // must be > MinExclusive
        if ( (thisFacetsDefined & DatatypeValidator::FACET_MINEXCLUSIVE) != 0 )
        {
            result = compareValues(theData, getMinExclusive());
            if (result != 1)
            {
                REPORT_VALUE_ERROR(theData
                                 , getMinExclusive()
                                 , XMLExcepts::VALUE_exceed_minExcl
                                 , manager)
            }
        }
    }
    catch (XMLException &e)
    {
       ThrowXMLwithMemMgr1(InvalidDatatypeValueException, XMLExcepts::RethrowError, e.getMessage(), manager);
    }

}

const XMLCh* AbstractNumericValidator::getCanonicalRepresentation(const XMLCh*         const rawData
                                                                 ,      MemoryManager* const memMgr
                                                                 ,      bool                 toValidate) const
{
    MemoryManager* toUse = memMgr? memMgr : fMemoryManager;

    if (toValidate)
    {
        AbstractNumericValidator* temp = (AbstractNumericValidator*) this;

        try 
        {
            temp->checkContent(rawData, 0, false, toUse);
        }
        catch (...)
        {
            return 0;
        }
    }

    // XMLAbstractDoubleFloat::getCanonicalRepresentation handles
    // exceptional cases
    return XMLAbstractDoubleFloat::getCanonicalRepresentation(rawData, toUse);

}

/***
 * Support for Serialization/De-serialization
 ***/

IMPL_XSERIALIZABLE_NOCREATE(AbstractNumericValidator)

void AbstractNumericValidator::serialize(XSerializeEngine& serEng)
{
    AbstractNumericFacetValidator::serialize(serEng);

    /***
     * Need not to do anything else here
     *
     * Note: its derivatives, Doubledv, Floatdv and Decimaldv writes
     *       number type info into the binary data stream to be read
     *       by AbstractNumericFacetVaildator during loading, therefore
     *       this class can NOT write/read anything into/from the binary
     *       data stream.
     *
     *       Later on, if this class has to write/read something into/from
     *       the binary data stream, we need to add a numberType data
     *       to XMLNumber and let AbstractNumericFacetValidator to write/read
     *       this number type info.
     ***/
}

XERCES_CPP_NAMESPACE_END

/**
  * End of file AbstractNumericValidator::cpp
  */

