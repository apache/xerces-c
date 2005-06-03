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
 * Revision 1.8  2004/09/08 13:56:53  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.7  2004/01/29 11:51:22  cargilld
 * Code cleanup changes to get rid of various compiler diagnostic messages.
 *
 * Revision 1.6  2003/12/17 00:18:39  cargilld
 * Update to memory management so that the static memory manager (one used to call Initialize) is only for static data.
 *
 * Revision 1.5  2003/09/30 21:31:30  peiyongz
 * Implementation of Serialization/Deserialization
 *
 * Revision 1.4  2003/05/15 18:53:26  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.3  2002/12/18 14:17:55  gareth
 * Fix to bug #13438. When you eant a vector that calls delete[] on its members you should use RefArrayVectorOf.
 *
 * Revision 1.2  2002/11/04 14:53:28  tng
 * C++ Namespace Support.
 *
 * Revision 1.1.1.1  2002/02/01 22:22:41  peiyongz
 * sane_include
 *
 * Revision 1.12  2001/10/09 20:47:45  peiyongz
 * init(): take 1 arg
 *
 * Revision 1.11  2001/10/02 18:59:29  peiyongz
 * Invalid_Facet_Tag to display the tag name
 *
 * Revision 1.10  2001/09/24 15:33:15  peiyongz
 * DTV Reorganization: virtual methods moved to *.cpp
 *
 * Revision 1.9  2001/09/19 20:35:23  peiyongz
 * DTV reorganization: inherit from AbstractStringVaildator
 *
 * Revision 1.8  2001/08/21 18:42:53  peiyongz
 * Bugzilla# 2816: cleanUp() declared with external linkage and called
 *                          before defined as inline
 *
 * Revision 1.7  2001/06/20 17:56:21  peiyongz
 * support for "fixed" option on constrainning facets
 *
 * Revision 1.6  2001/05/29 19:49:35  tng
 * Schema: Constraint Checking Fix in datatypeValidators.  By Pei Yong Zhang.
 *
 * Revision 1.5  2001/05/28 21:11:18  tng
 * Schema: Various DatatypeValidator fix.  By Pei Yong Zhang
 *
 * Revision 1.4  2001/05/18 13:36:47  tng
 * Schema: Catch RegularExpression exception and NumberFormatException
 *
 * Revision 1.3  2001/05/18 13:23:49  tng
 * Schema: Exception messages in DatatypeValidator.  By Pei Yong Zhang.
 *
 * Revision 1.2  2001/05/17 18:13:47  tng
 * Schema Fix: issue error message when binary data is invalid.
 *
 * Revision 1.1  2001/05/16 15:24:43  tng
 * Schema: Add Base64 and HexBin.  By Pei Yong Zhang.
 *
 */

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/validators/datatype/HexBinaryDatatypeValidator.hpp>
#include <xercesc/validators/datatype/InvalidDatatypeFacetException.hpp>
#include <xercesc/validators/datatype/InvalidDatatypeValueException.hpp>
#include <xercesc/util/HexBin.hpp>

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  Constructors and Destructor
// ---------------------------------------------------------------------------
HexBinaryDatatypeValidator::HexBinaryDatatypeValidator(MemoryManager* const manager)
:AbstractStringValidator(0, 0, 0, DatatypeValidator::HexBinary, manager)
{}

HexBinaryDatatypeValidator::~HexBinaryDatatypeValidator()
{}

HexBinaryDatatypeValidator::HexBinaryDatatypeValidator(
                          DatatypeValidator*            const baseValidator
                        , RefHashTableOf<KVStringPair>* const facets
                        , RefArrayVectorOf<XMLCh>*      const enums
                        , const int                           finalSet
                        , MemoryManager* const                manager)
:AbstractStringValidator(baseValidator, facets, finalSet, DatatypeValidator::HexBinary, manager)
{
    init(enums, manager);
}

DatatypeValidator* HexBinaryDatatypeValidator::newInstance
(
      RefHashTableOf<KVStringPair>* const facets
    , RefArrayVectorOf<XMLCh>* const      enums
    , const int                           finalSet
    , MemoryManager* const                manager
)
{
    return (DatatypeValidator*) new (manager) HexBinaryDatatypeValidator(this, facets, enums, finalSet, manager);
}

// ---------------------------------------------------------------------------
//  Utilities
// ---------------------------------------------------------------------------

void HexBinaryDatatypeValidator::checkValueSpace(const XMLCh* const content
                                                 , MemoryManager* const manager)
{
    if (getLength(content, manager) < 0)
    {
        ThrowXMLwithMemMgr1(InvalidDatatypeValueException
                , XMLExcepts::VALUE_Not_HexBin
                , content
                , manager);
    }
}

int HexBinaryDatatypeValidator::getLength(const XMLCh* const content
                                      , MemoryManager* const) const
{
    return HexBin::getDataLength(content);
}

/***
 * Support for Serialization/De-serialization
 ***/

IMPL_XSERIALIZABLE_TOCREATE(HexBinaryDatatypeValidator)

void HexBinaryDatatypeValidator::serialize(XSerializeEngine& serEng)
{
    AbstractStringValidator::serialize(serEng);
}

XERCES_CPP_NAMESPACE_END

/**
  * End of file HexBinaryDatatypeValidator.cpp
  */
