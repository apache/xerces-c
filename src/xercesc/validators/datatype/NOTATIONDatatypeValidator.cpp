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
 * Revision 1.10  2004/09/08 13:56:53  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.9  2004/08/11 16:52:20  peiyongz
 * using XMLString::isValidNOTATION()
 *
 * Revision 1.8  2003/12/17 00:18:39  cargilld
 * Update to memory management so that the static memory manager (one used to call Initialize) is only for static data.
 *
 * Revision 1.7  2003/10/01 16:32:41  neilg
 * improve handling of out of memory conditions, bug #23415.  Thanks to David Cargill.
 *
 * Revision 1.6  2003/09/30 21:31:30  peiyongz
 * Implementation of Serialization/Deserialization
 *
 * Revision 1.5  2003/05/16 06:01:57  knoaman
 * Partial implementation of the configurable memory manager.
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
 * Revision 1.1.1.1  2002/02/01 22:22:42  peiyongz
 * sane_include
 *
 * Revision 1.10  2001/10/10 15:11:21  peiyongz
 * extract/validate URI iff present
 *
 * Revision 1.9  2001/10/10 14:17:50  peiyongz
 * <URI>:<LocalPart> where <URI> can be optional.
 *
 * Revision 1.8  2001/10/09 20:46:19  peiyongz
 * NOTATION: checkContent(): <URI>:<localPart>
 *
 * Revision 1.7  2001/10/02 18:59:29  peiyongz
 * Invalid_Facet_Tag to display the tag name
 *
 * Revision 1.6  2001/09/24 15:33:15  peiyongz
 * DTV Reorganization: virtual methods moved to *.cpp
 *
 * Revision 1.5  2001/09/20 15:14:47  peiyongz
 * DTV reorganization: inherit from AbstractStringVaildator
 *
 * Revision 1.3  2001/08/21 18:42:53  peiyongz
 * Bugzilla# 2816: cleanUp() declared with external linkage and called
 *                          before defined as inline
 *
 * Revision 1.2  2001/08/14 22:11:56  peiyongz
 * new exception message added
 *
 * Revision 1.1  2001/07/05 20:15:27  peiyongz
 * NOTATIONDatatypeValidator
 *
 */

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/validators/datatype/NOTATIONDatatypeValidator.hpp>
#include <xercesc/util/XMLUri.hpp>
#include <xercesc/validators/datatype/InvalidDatatypeFacetException.hpp>
#include <xercesc/validators/datatype/InvalidDatatypeValueException.hpp>
#include <xercesc/util/OutOfMemoryException.hpp>

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  Constructors and Destructor
// ---------------------------------------------------------------------------
NOTATIONDatatypeValidator::NOTATIONDatatypeValidator(MemoryManager* const manager)
:AbstractStringValidator(0, 0, 0, DatatypeValidator::NOTATION, manager)
{}

NOTATIONDatatypeValidator::~NOTATIONDatatypeValidator()
{}

NOTATIONDatatypeValidator::NOTATIONDatatypeValidator(
                          DatatypeValidator*            const baseValidator
                        , RefHashTableOf<KVStringPair>* const facets
                        , RefArrayVectorOf<XMLCh>*           const enums
                        , const int                           finalSet
                        , MemoryManager* const manager)
:AbstractStringValidator(baseValidator, facets, finalSet, DatatypeValidator::NOTATION, manager)
{
    init(enums, manager);
}

DatatypeValidator* NOTATIONDatatypeValidator::newInstance
(
      RefHashTableOf<KVStringPair>* const facets
    , RefArrayVectorOf<XMLCh>* const      enums
    , const int                           finalSet
    , MemoryManager* const                manager
)
{
    return (DatatypeValidator*) new (manager) NOTATIONDatatypeValidator(this, facets, enums, finalSet, manager);
}

// ---------------------------------------------------------------------------
//  Utilities
// ---------------------------------------------------------------------------

void NOTATIONDatatypeValidator::checkValueSpace(const XMLCh* const content
                                                , MemoryManager* const manager)
{

    if (!XMLString::isValidNOTATION(content, manager))
    {
        ThrowXMLwithMemMgr1(InvalidDatatypeValueException
                , XMLExcepts::VALUE_NOTATION_Invalid
                , content
                , manager);
    }

}

/***
 * Support for Serialization/De-serialization
 ***/

IMPL_XSERIALIZABLE_TOCREATE(NOTATIONDatatypeValidator)

void NOTATIONDatatypeValidator::serialize(XSerializeEngine& serEng)
{
    AbstractStringValidator::serialize(serEng);
}

XERCES_CPP_NAMESPACE_END

/**
  * End of file NOTATIONDatatypeValidator.cpp
  */
