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
 * Revision 1.23  2005/01/07 15:28:49  amassari
 * Removed warnings
 *
 * Revision 1.22  2004/10/28 20:21:06  peiyongz
 * Data member reshuffle
 *
 * Revision 1.21  2004/10/13 17:55:40  cargilld
 * Serialization fix, fPVSIScope not serialized.  Problem reported by David Bertoni.
 *
 * Revision 1.20  2004/09/08 13:56:56  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.19  2004/01/29 11:52:31  cargilld
 * Code cleanup changes to get rid of various compiler diagnostic messages.
 *
 * Revision 1.18  2003/12/24 17:42:03  knoaman
 * Misc. PSVI updates
 *
 * Revision 1.17  2003/12/17 00:18:40  cargilld
 * Update to memory management so that the static memory manager (one used to call Initialize) is only for static data.
 *
 * Revision 1.16  2003/12/12 18:36:37  peiyongz
 * getObjectType()
 *
 * Revision 1.15  2003/11/13 23:20:47  peiyongz
 * initSize
 *
 * Revision 1.14  2003/10/17 21:17:12  peiyongz
 * using XTemplateSerializer
 *
 * Revision 1.12  2003/10/14 15:22:28  peiyongz
 * Implementation of Serialization/Deserialization
 *
 * Revision 1.11  2003/10/05 02:08:05  neilg
 * Because it makes grammars un-sharable between parsers running on multiple threads, xsi:type should not be handled by modifying element declarations.  Modifying implementation so it no longer relies on this kind of behaviour; marking methods as deprecated which imply that xsi:type will be handled in this way.  Once their behaviour is handled elsewhere, these methods should eventually be removed
 *
 * Revision 1.10  2003/05/18 14:02:08  knoaman
 * Memory manager implementation: pass per instance manager.
 *
 * Revision 1.9  2003/05/16 21:43:21  knoaman
 * Memory manager implementation: Modify constructors to pass in the memory manager.
 *
 * Revision 1.8  2003/05/15 18:57:27  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.7  2003/01/29 19:47:16  gareth
 * added DOMTypeInfo and some PSVI methods
 *
 * Revision 1.6  2002/11/04 14:49:41  tng
 * C++ Namespace Support.
 *
 * Revision 1.5  2002/07/12 15:17:48  knoaman
 * For a given global element, store info about a substitution group element
 * as a SchemaElementDecl and not as a string.
 *
 * Revision 1.4  2002/05/08 13:53:37  peiyongz
 * Bug#8898: SchemaElementDecl doesn't compile with Intel C++ for IA32,
 *                   patch from Curt Arnold
 *
 * Revision 1.3  2002/03/21 16:31:43  knoaman
 * Remove data/methods from SchemaElementDecl that are not used.
 *
 * Revision 1.2  2002/02/06 22:30:50  knoaman
 * Added a new attribute to store the wild card information for elements of type 'anyType'.
 *
 * Revision 1.1.1.1  2002/02/01 22:22:46  peiyongz
 * sane_include
 *
 * Revision 1.18  2002/01/02 15:20:22  tng
 * Schema Fix: should not store a temp value as the key in the element pool and the attribute pool.
 *
 * Revision 1.17  2001/11/02 14:13:45  knoaman
 * Add support for identity constraints.
 *
 * Revision 1.16  2001/10/11 12:07:39  tng
 * Schema: model type should be based on complextypeinfo if exists.
 *
 * Revision 1.15  2001/09/14 14:50:22  tng
 * Schema: Fix some wildcard bugs, and some retrieving qualified/unqualified element decl problems.
 *
 * Revision 1.14  2001/09/11 13:02:40  tng
 * [Bug 3523] SchemaElementDecl.cpp(242) : error C2202 : not all control paths return a value
 *
 * Revision 1.13  2001/08/29 20:52:35  tng
 * Schema: xsi:type support
 *
 * Revision 1.12  2001/08/21 16:06:11  tng
 * Schema: Unique Particle Attribution Constraint Checking.
 *
 * Revision 1.11  2001/07/24 18:33:46  knoaman
 * Added support for <group> + extra constraint checking for complexType
 *
 * Revision 1.10  2001/07/09 15:22:43  knoaman
 * complete <any> declaration.
 *
 * Revision 1.9  2001/06/13 20:51:18  peiyongz
 * fIsMixed: to handle mixed Content Model
 *
 * Revision 1.8  2001/05/11 13:27:35  tng
 * Copyright update.
 *
 * Revision 1.7  2001/05/03 20:34:42  tng
 * Schema: SchemaValidator update
 *
 * Revision 1.6  2001/05/03 19:17:59  knoaman
 * TraverseSchema Part II.
 *
 * Revision 1.5  2001/04/19 17:43:16  knoaman
 * More schema implementation classes.
 *
 * Revision 1.4  2001/03/21 21:56:33  tng
 * Schema: Add Schema Grammar, Schema Validator, and split the DTDValidator into DTDValidator, DTDScanner, and DTDGrammar.
 *
 * Revision 1.3  2001/03/21 19:30:15  tng
 * Schema: Content Model Updates, by Pei Yong Zhang.
 *
 * Revision 1.2  2001/03/07 17:42:12  tng
 * fix typo
 *
 * Revision 1.1  2001/02/27 18:48:22  tng
 * Schema: Add SchemaAttDef, SchemaElementDecl, SchemaAttDefList.
 *
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/XMLUniDefs.hpp>
#include <xercesc/util/XMLUni.hpp>
#include <xercesc/validators/schema/SchemaAttDefList.hpp>
#include <xercesc/validators/schema/SchemaElementDecl.hpp>
#include <xercesc/validators/schema/identity/IdentityConstraint.hpp>

#include <xercesc/internal/XTemplateSerializer.hpp>

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  SchemaElementDecl: Constructors and Destructor
// ---------------------------------------------------------------------------
SchemaElementDecl::SchemaElementDecl(MemoryManager* const manager) :
    XMLElementDecl(manager)
    , fModelType(Any)
    , fPSVIScope(PSVIDefs::SCP_ABSENT)
    , fValidity(PSVIDefs::UNKNOWN)
    , fValidation(PSVIDefs::NONE)
    , fEnclosingScope(Grammar::TOP_LEVEL_SCOPE)
    , fFinalSet(0)
    , fBlockSet(0)    
    , fMiscFlags(0)
    , fDefaultValue(0)
    , fComplexTypeInfo(0)
    , fAttDefs(0)
    , fXsiComplexTypeInfo(0)
    , fXsiSimpleTypeInfo(0)    
    , fIdentityConstraints(0)
    , fAttWildCard(0)
    , fSubstitutionGroupElem(0)
    , fDatatypeValidator(0)
    , fSeenValidation(false)
    , fSeenNoValidation(false)
    , fHadContent(false)
{
}

SchemaElementDecl::SchemaElementDecl(const XMLCh* const                  prefix
                                   , const XMLCh* const                  localPart
                                   , const int                           uriId
                                   , const SchemaElementDecl::ModelTypes type
                                   , const int                           enclosingScope
                                   , MemoryManager* const                manager) :
    XMLElementDecl(manager)
    , fModelType(type)
    , fPSVIScope(PSVIDefs::SCP_ABSENT)
    , fValidity(PSVIDefs::UNKNOWN)
    , fValidation(PSVIDefs::NONE)
    , fEnclosingScope(enclosingScope)
    , fFinalSet(0)
    , fBlockSet(0)    
    , fMiscFlags(0)
    , fDefaultValue(0)
    , fComplexTypeInfo(0)
    , fAttDefs(0)
    , fXsiComplexTypeInfo(0)
    , fXsiSimpleTypeInfo(0)    
    , fIdentityConstraints(0)
    , fAttWildCard(0)
    , fSubstitutionGroupElem(0)
    , fDatatypeValidator(0)
    , fSeenValidation(false)
    , fSeenNoValidation(false)
    , fHadContent(false)
{
    setElementName(prefix, localPart, uriId);
}

SchemaElementDecl::SchemaElementDecl(const QName* const                  elementName
                                   , const SchemaElementDecl::ModelTypes type
                                   , const int                           enclosingScope
                                   , MemoryManager* const                manager) :
    XMLElementDecl(manager)
    , fModelType(type)
    , fPSVIScope(PSVIDefs::SCP_ABSENT)
    , fValidity(PSVIDefs::UNKNOWN)
    , fValidation(PSVIDefs::NONE)
    , fEnclosingScope(enclosingScope)
    , fFinalSet(0)
    , fBlockSet(0)    
    , fMiscFlags(0)
    , fDefaultValue(0)
    , fComplexTypeInfo(0)
    , fAttDefs(0)
    , fXsiComplexTypeInfo(0)
    , fXsiSimpleTypeInfo(0)    
    , fIdentityConstraints(0)
    , fAttWildCard(0)
    , fSubstitutionGroupElem(0)
    , fDatatypeValidator(0)
    , fSeenValidation(false)
    , fSeenNoValidation(false)
    , fHadContent(false)
{
    setElementName(elementName);
}

SchemaElementDecl::~SchemaElementDecl()
{
    getMemoryManager()->deallocate(fDefaultValue);//delete [] fDefaultValue;
    delete fAttDefs;
    delete fIdentityConstraints;
    delete fAttWildCard;
}


// ---------------------------------------------------------------------------
//  SchemaElementDecl: XMLElementDecl virtual interface implementation
// ---------------------------------------------------------------------------
XMLAttDef* SchemaElementDecl::findAttr(const XMLCh* const    qName
                                     , const unsigned int    uriId
                                     , const XMLCh* const    baseName
                                     , const XMLCh* const    prefix
                                     , const LookupOpts      options
                                     , bool&           wasAdded) const
{
    if (fComplexTypeInfo) {
        return fComplexTypeInfo->findAttr(qName, uriId, baseName, prefix, options, wasAdded);
    }
    else {
        if (options == XMLElementDecl::AddIfNotFound) {
            SchemaAttDef* retVal = 0;

            // If no att list exist yet, then create one
            if (!fAttDefs) {
                // Use a hash modulus of 29 and tell it owns its elements
                ((SchemaElementDecl*)this)->fAttDefs =
                    new (getMemoryManager()) RefHash2KeysTableOf<SchemaAttDef>(29, true, getMemoryManager());
            }

            retVal = fAttDefs->get(baseName, uriId);

            // Fault it in if not found and ask to add it
            if (!retVal)
            {
                // And add a default attribute for this name
                retVal = new (getMemoryManager()) SchemaAttDef
                (
                    prefix
                    , baseName
                    , uriId
                    , XMLAttDef::CData
                    , XMLAttDef::Implied
                    , getMemoryManager()
                );
                retVal->setElemId(getId());
                fAttDefs->put((void*)retVal->getAttName()->getLocalPart(), uriId, retVal);

                wasAdded = true;
            }
             else
            {
                wasAdded = false;
            }
            return retVal;
        }
        else {
            wasAdded = false;
            return 0;
        }
    }
}


XMLAttDefList& SchemaElementDecl::getAttDefList() const
{
    if (!fComplexTypeInfo)
	{
        ThrowXMLwithMemMgr(RuntimeException, XMLExcepts::DV_InvalidOperation, getMemoryManager());
    }

	return fComplexTypeInfo->getAttDefList();
}


XMLElementDecl::CharDataOpts SchemaElementDecl::getCharDataOpts() const
{
    SchemaElementDecl::ModelTypes modelType = fModelType;

    if (fComplexTypeInfo) {
        modelType = (SchemaElementDecl::ModelTypes) fComplexTypeInfo->getContentType();
    }

    XMLElementDecl::CharDataOpts retVal;
    switch(modelType)
    {
        case Children :
            retVal = XMLElementDecl::SpacesOk;
            break;

        case Empty :
            retVal = XMLElementDecl::NoCharData;
            break;

        default :
            retVal = XMLElementDecl::AllCharData;
            break;
    }
    return retVal;
}


bool SchemaElementDecl::hasAttDefs() const
{
    if (fComplexTypeInfo) {
        return fComplexTypeInfo->hasAttDefs();
    }

    // If the collection hasn't been faulted in, then no att defs
    return false;

}


bool SchemaElementDecl::resetDefs()
{
    if (fComplexTypeInfo) {
        return fComplexTypeInfo->resetDefs();
    }
    else if (fAttDefs) {
        //all the attdefs here are faulted-in, so just reset the fAttDefs
        //but still return false to indicate there is no real att defs
        // defined in this element
        fAttDefs->removeAll();
    }

    return false;
}

const XMLCh*
SchemaElementDecl::getFormattedContentModel() const
{
    if (fComplexTypeInfo) {
        return fComplexTypeInfo->getFormattedContentModel();
    }
    return 0;
}

// ---------------------------------------------------------------------------
//  SchemaElementDecl: Getter methods
// ---------------------------------------------------------------------------
const SchemaAttDef* SchemaElementDecl::getAttDef(const XMLCh* const baseName, const int uriId) const
{
    if (fComplexTypeInfo) {
        return fComplexTypeInfo->getAttDef(baseName, uriId);
    }

    // If no complex type, then return a null
    return 0;

}

SchemaAttDef* SchemaElementDecl::getAttDef(const XMLCh* const baseName, const int uriId)
{
    if (fComplexTypeInfo) {
        return fComplexTypeInfo->getAttDef(baseName, uriId);
    }

    // If no complex type, then return a null
    return 0;
}

/***
 * Support for Serialization/De-serialization
 ***/

IMPL_XSERIALIZABLE_TOCREATE(SchemaElementDecl)

void SchemaElementDecl::serialize(XSerializeEngine& serEng)
{

    XMLElementDecl::serialize(serEng);

    if (serEng.isStoring())
    {
        serEng<<(int)fModelType;
        serEng<<(int)fPSVIScope;
        serEng<<(int)fValidity;
        serEng<<(int)fValidation;

        serEng<<fEnclosingScope;
        serEng<<fFinalSet;
        serEng<<fBlockSet;
        serEng<<fMiscFlags;

        serEng.writeString(fDefaultValue);

        serEng<<fComplexTypeInfo;

        /***
         * Serialize RefHash2KeysTableOf<SchemaAttDef>* fAttDefs;
         ***/

        XTemplateSerializer::storeObject(fAttDefs, serEng);

        serEng<<fXsiComplexTypeInfo;

        DatatypeValidator::storeDV(serEng, (DatatypeValidator*)fXsiSimpleTypeInfo);

        /***
         * Serialize RefVectorOf<IdentityConstraint>*   fIdentityConstraints;
         ***/
        XTemplateSerializer::storeObject(fIdentityConstraints, serEng);

        serEng<<fAttWildCard;
        serEng<<fSubstitutionGroupElem;
        DatatypeValidator::storeDV(serEng, fDatatypeValidator);

        serEng<<fSeenValidation;
        serEng<<fSeenNoValidation;
        serEng<<fHadContent;
            
    }
    else
    {
        int i;
        serEng>>i;
        fModelType = (ModelTypes)i;
        serEng>>i;
        fPSVIScope = (PSVIDefs::PSVIScope)i;
        serEng>>i;
        fValidity = (PSVIDefs::Validity)i;
        serEng>> i;
        fValidation = (PSVIDefs::Validation)i;

        serEng>>fEnclosingScope;
        serEng>>fFinalSet;
        serEng>>fBlockSet;
        serEng>>fMiscFlags;

        serEng.readString(fDefaultValue);

        serEng>>fComplexTypeInfo;

        /***
         * DeSerialize RefHash2KeysTableOf<SchemaAttDef>* fAttDefs;
         ***/
        XTemplateSerializer::loadObject(&fAttDefs, 29, true, serEng);

        serEng>>fXsiComplexTypeInfo;

        fXsiSimpleTypeInfo = DatatypeValidator::loadDV(serEng);

        /***
         * DeSerialize RefVectorOf<IdentityConstraint>*   fIdentityConstraints;
         ***/
        XTemplateSerializer::loadObject(&fIdentityConstraints, 16, true, serEng);

        serEng>>fAttWildCard;
        serEng>>fSubstitutionGroupElem;
        fDatatypeValidator = DatatypeValidator::loadDV(serEng);

        serEng>>fSeenValidation;
        serEng>>fSeenNoValidation;
        serEng>>fHadContent;

    }

}

XMLElementDecl::objectType  SchemaElementDecl::getObjectType() const
{
    return Schema;
}

XERCES_CPP_NAMESPACE_END
