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
 * Revision 1.12  2004/09/08 13:56:52  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.11  2004/01/29 11:51:22  cargilld
 * Code cleanup changes to get rid of various compiler diagnostic messages.
 *
 * Revision 1.10  2003/12/17 00:18:38  cargilld
 * Update to memory management so that the static memory manager (one used to call Initialize) is only for static data.
 *
 * Revision 1.9  2003/11/06 15:30:07  neilg
 * first part of PSVI/schema component model implementation, thanks to David Cargill.  This covers setting the PSVIHandler on parser objects, as well as implementing XSNotation, XSSimpleTypeDefinition, XSIDCDefinition, and most of XSWildcard, XSComplexTypeDefinition, XSElementDeclaration, XSAttributeDeclaration and XSAttributeUse.
 *
 * Revision 1.8  2003/10/07 19:39:03  peiyongz
 * Implementation of Serialization/Deserialization
 *
 * Revision 1.7  2003/10/01 01:09:35  knoaman
 * Refactoring of some code to improve performance.
 *
 * Revision 1.6  2003/05/15 18:53:26  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.5  2003/01/13 20:16:51  knoaman
 * [Bug 16024] SchemaSymbols.hpp conflicts C++ Builder 6 dir.h
 *
 * Revision 1.4  2002/12/18 14:17:55  gareth
 * Fix to bug #13438. When you eant a vector that calls delete[] on its members you should use RefArrayVectorOf.
 *
 * Revision 1.3  2002/11/04 14:53:27  tng
 * C++ Namespace Support.
 *
 * Revision 1.2  2002/02/14 15:17:31  peiyongz
 * getEnumString()
 *
 * Revision 1.1.1.1  2002/02/01 22:22:40  peiyongz
 * sane_include
 *
 * Revision 1.1  2001/08/24 17:12:01  knoaman
 * Add support for anySimpleType.
 * Remove parameter 'baseValidator' from the virtual method 'newInstance'.
 *
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/validators/datatype/AnySimpleTypeDatatypeValidator.hpp>
#include <xercesc/util/RuntimeException.hpp>

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  AnySimpleTypeDatatypeValidator: Constructors and Destructor
// ---------------------------------------------------------------------------
AnySimpleTypeDatatypeValidator::AnySimpleTypeDatatypeValidator(MemoryManager* const manager)
    : DatatypeValidator(0, 0, SchemaSymbols::XSD_RESTRICTION, DatatypeValidator::AnySimpleType, manager)
{
    setWhiteSpace(DatatypeValidator::PRESERVE);
    setFinite(true);
}

AnySimpleTypeDatatypeValidator::~AnySimpleTypeDatatypeValidator()
{

}

// ---------------------------------------------------------------------------
//  AnySimpleTypeDatatypeValidator: Instance methods
// ---------------------------------------------------------------------------
DatatypeValidator* AnySimpleTypeDatatypeValidator::newInstance
(
      RefHashTableOf<KVStringPair>* const facets
    , RefArrayVectorOf<XMLCh>* const      enums
    , const int
    , MemoryManager* const                manager
)
{
    // We own them, so we will delete them first
    delete facets;
    delete enums;

    ThrowXMLwithMemMgr(RuntimeException, XMLExcepts::DV_InvalidOperation, manager);

    // to satisfy some compilers
    return 0;
}

const RefArrayVectorOf<XMLCh>* AnySimpleTypeDatatypeValidator::getEnumString() const
{
	return 0;
}

/***
 * Support for Serialization/De-serialization
 ***/

IMPL_XSERIALIZABLE_TOCREATE(AnySimpleTypeDatatypeValidator)

void AnySimpleTypeDatatypeValidator::serialize(XSerializeEngine& serEng)
{
    DatatypeValidator::serialize(serEng);
}

XERCES_CPP_NAMESPACE_END

/**
  * End of file AnySimpleTypeDatatypeValidator.cpp
  */

