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
 * Revision 1.16  2004/10/28 20:21:06  peiyongz
 * Data member reshuffle
 *
 * Revision 1.15  2004/10/13 17:55:40  cargilld
 * Serialization fix, fPVSIScope not serialized.  Problem reported by David Bertoni.
 *
 * Revision 1.14  2004/09/08 13:56:56  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.13  2004/01/29 11:52:31  cargilld
 * Code cleanup changes to get rid of various compiler diagnostic messages.
 *
 * Revision 1.12  2003/12/24 17:42:03  knoaman
 * Misc. PSVI updates
 *
 * Revision 1.11  2003/12/17 20:50:35  knoaman
 * PSVI: fix for annotation of attributes in attributeGroup/derived types
 *
 * Revision 1.10  2003/11/25 18:15:00  knoaman
 * Serialize enclosing complex type. Thanks to David Cargill.
 *
 * Revision 1.9  2003/11/21 22:34:46  neilg
 * More schema component model implementation, thanks to David Cargill.
 * In particular, this cleans up and completes the XSModel, XSNamespaceItem,
 * XSAttributeDeclaration and XSAttributeGroup implementations.
 *
 * Revision 1.8  2003/10/17 21:17:12  peiyongz
 * using XTemplateSerializer
 *
 * Revision 1.7  2003/10/10 16:25:40  peiyongz
 * Implementation of Serialization/Deserialization
 *
 * Revision 1.6  2003/05/18 14:02:07  knoaman
 * Memory manager implementation: pass per instance manager.
 *
 * Revision 1.5  2003/05/16 21:43:21  knoaman
 * Memory manager implementation: Modify constructors to pass in the memory manager.
 *
 * Revision 1.4  2003/02/06 13:51:55  gareth
 * fixed bug with multiple attributes being validated by the same union type.
 *
 * Revision 1.3  2003/01/29 19:47:16  gareth
 * added DOMTypeInfo and some PSVI methods
 *
 * Revision 1.2  2002/11/04 14:49:41  tng
 * C++ Namespace Support.
 *
 * Revision 1.1.1.1  2002/02/01 22:22:45  peiyongz
 * sane_include
 *
 * Revision 1.4  2001/08/09 15:23:16  knoaman
 * add support for <anyAttribute> declaration.
 *
 * Revision 1.3  2001/07/31 15:26:54  knoaman
 * Added support for <attributeGroup>.
 *
 * Revision 1.2  2001/05/11 13:27:33  tng
 * Copyright update.
 *
 * Revision 1.1  2001/02/27 18:48:22  tng
 * Schema: Add SchemaAttDef, SchemaElementDecl, SchemaAttDefList.
 *
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/framework/XMLElementDecl.hpp>
#include <xercesc/validators/schema/SchemaAttDef.hpp>

#include <xercesc/internal/XTemplateSerializer.hpp>

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  SchemaAttDef: Implementation of the XMLAttDef interface
// ---------------------------------------------------------------------------
const XMLCh* SchemaAttDef::getFullName() const
{
   return fAttName->getRawName();
}

// ---------------------------------------------------------------------------
//  SchemaAttDef: Constructors and Destructor
// ---------------------------------------------------------------------------
SchemaAttDef::SchemaAttDef(MemoryManager* const manager) :
    XMLAttDef(XMLAttDef::CData, XMLAttDef::Implied, manager)
    , fElemId(XMLElementDecl::fgInvalidElemId)
    , fValidity(PSVIDefs::UNKNOWN)
    , fValidation(PSVIDefs::NONE)
    , fPSVIScope(PSVIDefs::SCP_ABSENT)    
    , fAttName(0)
    , fDatatypeValidator(0)
    , fAnyDatatypeValidator(0)
    , fMemberTypeValidator(0)
    , fNamespaceList(0)
    , fBaseAttDecl(0)
{
}

SchemaAttDef::SchemaAttDef( const XMLCh* const           prefix
                          , const XMLCh* const           localPart
                          , const int                    uriId
                          , const XMLAttDef::AttTypes    type
                          , const XMLAttDef::DefAttTypes defType
                          , MemoryManager* const         manager) :
    XMLAttDef(type, defType, manager)
    , fElemId(XMLElementDecl::fgInvalidElemId)
    , fValidity(PSVIDefs::UNKNOWN)
    , fValidation(PSVIDefs::NONE)
    , fPSVIScope(PSVIDefs::SCP_ABSENT)
    , fDatatypeValidator(0)
    , fAnyDatatypeValidator(0)
    , fMemberTypeValidator(0)    
    , fNamespaceList(0)
    , fBaseAttDecl(0)
{
    fAttName = new (manager) QName(prefix, localPart, uriId, manager);
}

SchemaAttDef::SchemaAttDef( const XMLCh* const           prefix
                          , const XMLCh* const           localPart
                          , const int                    uriId
                          , const XMLCh* const           attValue
                          , const XMLAttDef::AttTypes    type
                          , const XMLAttDef::DefAttTypes defType
                          , const XMLCh* const           enumValues
                          , MemoryManager* const         manager) :

    XMLAttDef(attValue, type, defType, enumValues, manager)
    , fElemId(XMLElementDecl::fgInvalidElemId)
    , fValidity(PSVIDefs::UNKNOWN)
    , fValidation(PSVIDefs::NONE)
    , fPSVIScope(PSVIDefs::SCP_ABSENT)
    , fDatatypeValidator(0)
    , fAnyDatatypeValidator(0)
    , fMemberTypeValidator(0)
    , fNamespaceList(0)
    , fBaseAttDecl(0)
{
    fAttName = new (manager) QName(prefix, localPart, uriId, manager);
}

SchemaAttDef::SchemaAttDef(const SchemaAttDef* other) :

    XMLAttDef(other->getValue(), other->getType(),
              other->getDefaultType(), other->getEnumeration(),
              other->getMemoryManager())
    , fElemId(XMLElementDecl::fgInvalidElemId)
    , fValidity(other->fValidity)
    , fValidation(other->fValidation)
    , fPSVIScope(other->fPSVIScope)
    , fAttName(0)
    , fDatatypeValidator(other->fDatatypeValidator)
    , fAnyDatatypeValidator(other->fAnyDatatypeValidator)
    , fMemberTypeValidator(other->fMemberTypeValidator)
    , fNamespaceList(0)
    , fBaseAttDecl(other->fBaseAttDecl)
{
    QName* otherName = other->getAttName();
    fAttName = new (getMemoryManager()) QName(otherName->getPrefix(),
                         otherName->getLocalPart(), otherName->getURI(),
                         getMemoryManager());

    if (other->fNamespaceList && other->fNamespaceList->size()) {
        fNamespaceList = new (getMemoryManager()) ValueVectorOf<unsigned int>(*(other->fNamespaceList));
    }
}

SchemaAttDef::~SchemaAttDef()
{
   delete fAttName;
   delete fNamespaceList;
}


// ---------------------------------------------------------------------------
//  SchemaAttDef: Setter methods
// ---------------------------------------------------------------------------
void SchemaAttDef::setAttName(const XMLCh* const        prefix
                            , const XMLCh* const        localPart
                            , const int                 uriId )
{
   fAttName->setName(prefix, localPart, uriId);
}

/***
 * Support for Serialization/De-serialization
 ***/

IMPL_XSERIALIZABLE_TOCREATE(SchemaAttDef)

void SchemaAttDef::serialize(XSerializeEngine& serEng)
{

    XMLAttDef::serialize(serEng);

    if (serEng.isStoring())
    {
        serEng<<fElemId;
        serEng<<(int)fValidity;
        serEng<<(int)fValidation;
        serEng<<(int)fPSVIScope;

        serEng<<fAttName;

        DatatypeValidator::storeDV(serEng, fDatatypeValidator);
        DatatypeValidator::storeDV(serEng, fAnyDatatypeValidator);
        DatatypeValidator::storeDV(serEng, (DatatypeValidator*)fMemberTypeValidator);

        /***
         * Serialize ValueVectorOf<unsigned int>
         ***/
        XTemplateSerializer::storeObject(fNamespaceList, serEng);

        serEng<<fBaseAttDecl;
    }
    else
    {

        serEng>>fElemId;
        int i;
        serEng>>i;
        fValidity = (PSVIDefs::Validity)i;

        serEng>>i;
        fValidation = (PSVIDefs::Validation)i;
        
        serEng>>i;
        fPSVIScope = (PSVIDefs::PSVIScope)i;

        serEng>>fAttName;

        fDatatypeValidator    = DatatypeValidator::loadDV(serEng);
        fAnyDatatypeValidator = DatatypeValidator::loadDV(serEng);
        fMemberTypeValidator  = DatatypeValidator::loadDV(serEng);

        /***
         * Deserialize ValueVectorOf<unsigned int>
         ***/
        XTemplateSerializer::loadObject(&fNamespaceList, 8, false, serEng);
       
        serEng>>fBaseAttDecl;
    }
}


XERCES_CPP_NAMESPACE_END
