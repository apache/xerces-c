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
 * Revision 1.11  2004/09/08 13:56:53  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.10  2004/01/29 11:51:22  cargilld
 * Code cleanup changes to get rid of various compiler diagnostic messages.
 *
 * Revision 1.9  2003/12/17 00:18:38  cargilld
 * Update to memory management so that the static memory manager (one used to call Initialize) is only for static data.
 *
 * Revision 1.8  2003/11/12 20:31:33  peiyongz
 * Using ValidationContext to validate()
 *
 * Revision 1.7  2003/09/30 18:17:53  peiyongz
 * Implementation of Serialization/Deserialization
 *
 * Revision 1.6  2003/05/15 18:53:26  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.5  2003/01/06 19:40:05  knoaman
 * Throw an invalid datatype value exception if fEntityDeclPool is NULL.
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
 * Revision 1.1.1.1  2002/02/01 22:22:41  peiyongz
 * sane_include
 *
 * Revision 1.7  2001/11/13 13:21:59  tng
 * Schema fix: Entities should be unparsed.
 *
 * Revision 1.6  2001/10/09 20:50:27  peiyongz
 * init(): take 1 arg
 *
 * Revision 1.5  2001/09/27 13:51:25  peiyongz
 * DTV Reorganization: ctor/init created to be used by derived class
 *
 * Revision 1.4  2001/09/24 21:39:29  peiyongz
 * DTV Reorganization: checkValueSpace()
 *
 * Revision 1.3  2001/08/14 22:11:56  peiyongz
 * new exception message added
 *
 * Revision 1.2  2001/07/24 21:23:39  tng
 * Schema: Use DatatypeValidator for ID/IDREF/ENTITY/ENTITIES/NOTATION.
 *
 * Revision 1.1  2001/07/06 20:19:52  peiyongz
 * ENTITYDTV
 *
 */

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/validators/datatype/ENTITYDatatypeValidator.hpp>
#include <xercesc/validators/datatype/InvalidDatatypeValueException.hpp>
#include <xercesc/framework/XMLEntityDecl.hpp>

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  Constructors and Destructor
// ---------------------------------------------------------------------------
ENTITYDatatypeValidator::ENTITYDatatypeValidator(MemoryManager* const manager)
:StringDatatypeValidator(0, 0, 0, DatatypeValidator::ENTITY, manager)
{}

ENTITYDatatypeValidator::ENTITYDatatypeValidator(
                          DatatypeValidator*            const baseValidator
                        , RefHashTableOf<KVStringPair>* const facets
                        , RefArrayVectorOf<XMLCh>*      const enums
                        , const int                           finalSet
                        , MemoryManager* const                manager)
:StringDatatypeValidator(baseValidator, facets, finalSet, DatatypeValidator::ENTITY, manager)
{
    init(enums, manager);
}

ENTITYDatatypeValidator::~ENTITYDatatypeValidator()
{}

DatatypeValidator* ENTITYDatatypeValidator::newInstance
(
      RefHashTableOf<KVStringPair>* const facets
    , RefArrayVectorOf<XMLCh>* const      enums
    , const int                           finalSet
    , MemoryManager* const                manager
)
{
    return (DatatypeValidator*) new (manager) ENTITYDatatypeValidator(this, facets, enums, finalSet, manager);
}

// -----------------------------------------------------------------------
// Compare methods
// -----------------------------------------------------------------------
int ENTITYDatatypeValidator::compare(const XMLCh* const lValue
                                   , const XMLCh* const rValue
                                   ,       MemoryManager*     const)
{
    return ( XMLString::equals(lValue, rValue)? 0 : -1);
}

void ENTITYDatatypeValidator::validate(const XMLCh*             const content
                                     ,       ValidationContext* const context
                                     ,       MemoryManager*     const manager)
{
    // use StringDatatypeValidator (which in turn, invoke
    // the baseValidator) to validate content against
    // facets if any.
    //
    StringDatatypeValidator::validate(content, context, manager);

    //
    // parse the entity iff an EntityDeclPool is provided
    //
    if (context)
    {
        context->checkEntity(content);
    }

}

void ENTITYDatatypeValidator::checkValueSpace(const XMLCh* const content
                                              , MemoryManager* const manager)
{
    //
    // 3.3.11 check must: "NCName"
    //
    if ( !XMLString::isValidNCName(content))
    {
        ThrowXMLwithMemMgr1(InvalidDatatypeValueException
                , XMLExcepts::VALUE_Invalid_NCName
                , content
                , manager);
    }

}

/***
 * Support for Serialization/De-serialization
 ***/

IMPL_XSERIALIZABLE_TOCREATE(ENTITYDatatypeValidator)

void ENTITYDatatypeValidator::serialize(XSerializeEngine& serEng)
{
    StringDatatypeValidator::serialize(serEng);
}

XERCES_CPP_NAMESPACE_END

 /**
  * End of file ENTITYDatatypeValidator.cpp
  */
