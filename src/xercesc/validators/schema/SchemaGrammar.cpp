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
 * Revision 1.22  2004/10/28 20:21:06  peiyongz
 * Data member reshuffle
 *
 * Revision 1.21  2004/09/08 13:56:56  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.20  2004/03/03 23:03:48  peiyongz
 * Using serialize() to save/load object created in ctor
 *
 * Revision 1.19  2004/01/29 11:52:31  cargilld
 * Code cleanup changes to get rid of various compiler diagnostic messages.
 *
 * Revision 1.18  2003/11/13 23:21:19  peiyongz
 * don't serialize/deserialize fElemNonDecl
 *
 * Revision 1.17  2003/11/12 20:35:31  peiyongz
 * Stateless Grammar: ValidationContext
 *
 * Revision 1.16  2003/11/11 22:48:13  knoaman
 * Serialization of XSAnnotation.
 *
 * Revision 1.15  2003/11/06 21:52:17  neilg
 * fix typo
 *
 * Revision 1.14  2003/11/06 19:28:11  knoaman
 * PSVI support for annotations.
 *
 * Revision 1.13  2003/11/05 16:38:08  peiyongz
 * serialize DatatypeValidatorFactory first
 *
 * Revision 1.12  2003/10/29 16:22:30  peiyongz
 * serialize/deserialize substitutiongroup
 *
 * Revision 1.11  2003/10/17 21:17:12  peiyongz
 * using XTemplateSerializer
 *
 * Revision 1.10  2003/10/14 15:22:28  peiyongz
 * Implementation of Serialization/Deserialization
 *
 * Revision 1.9  2003/10/01 16:32:42  neilg
 * improve handling of out of memory conditions, bug #23415.  Thanks to David Cargill.
 *
 * Revision 1.8  2003/09/22 19:49:02  neilg
 * implement change to Grammar::putElem(XMLElementDecl, bool).  If Grammars are used only to hold declared objects, there will be no need for the fElemNonDeclPool tables; make Grammar implementations lazily create them only if the application requires them (which good cpplications should not.)
 *
 * Revision 1.7  2003/07/31 17:12:10  peiyongz
 * Grammar embed grammar description
 *
 * Revision 1.6  2003/05/21 21:35:31  knoaman
 * Pass the memory manager.
 *
 * Revision 1.5  2003/05/18 14:02:08  knoaman
 * Memory manager implementation: pass per instance manager.
 *
 * Revision 1.4  2003/05/15 18:57:27  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.3  2002/11/04 14:49:41  tng
 * C++ Namespace Support.
 *
 * Revision 1.2  2002/07/11 18:21:20  knoaman
 * Grammar caching/preparsing - initial implementation.
 *
 * Revision 1.1.1.1  2002/02/01 22:22:46  peiyongz
 * sane_include
 *
 * Revision 1.11  2002/01/02 15:20:22  tng
 * Schema Fix: should not store a temp value as the key in the element pool and the attribute pool.
 *
 * Revision 1.10  2001/11/19 18:26:31  knoaman
 * no message
 *
 * Revision 1.9  2001/10/09 12:18:26  tng
 * Leak fix: should use delete [] to delete fTargetNamespace.
 *
 * Revision 1.8  2001/09/14 14:50:22  tng
 * Schema: Fix some wildcard bugs, and some retrieving qualified/unqualified element decl problems.
 *
 * Revision 1.7  2001/08/21 16:06:11  tng
 * Schema: Unique Particle Attribution Constraint Checking.
 *
 * Revision 1.6  2001/07/31 15:27:10  knoaman
 * Added support for <attributeGroup>.
 *
 * Revision 1.5  2001/07/24 18:33:46  knoaman
 * Added support for <group> + extra constraint checking for complexType
 *
 * Revision 1.4  2001/06/25 12:51:56  knoaman
 * Add constraint checking on elements in complex types to prevent same
 * element names from having different definitions - use substitueGroups.
 *
 * Revision 1.3  2001/05/11 13:27:36  tng
 * Copyright update.
 *
 * Revision 1.2  2001/04/19 17:43:18  knoaman
 * More schema implementation classes.
 *
 * Revision 1.1  2001/03/21 21:56:33  tng
 * Schema: Add Schema Grammar, Schema Validator, and split the DTDValidator into DTDValidator, DTDScanner, and DTDGrammar.
 *
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/validators/schema/SchemaGrammar.hpp>
#include <xercesc/validators/schema/NamespaceScope.hpp>
#include <xercesc/validators/schema/ComplexTypeInfo.hpp>
#include <xercesc/validators/schema/SchemaSymbols.hpp>
#include <xercesc/validators/schema/XercesGroupInfo.hpp>
#include <xercesc/validators/schema/XercesAttGroupInfo.hpp>
#include <xercesc/validators/schema/XMLSchemaDescriptionImpl.hpp>
#include <xercesc/util/OutOfMemoryException.hpp>
#include <xercesc/util/HashPtr.hpp>
#include <xercesc/framework/psvi/XSAnnotation.hpp>

#include <xercesc/internal/XTemplateSerializer.hpp>
#include <xercesc/internal/ValidationContextImpl.hpp>

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  SchemaGrammar: Constructors and Destructor
// ---------------------------------------------------------------------------
SchemaGrammar::SchemaGrammar(MemoryManager* const manager) :
    fTargetNamespace(0)
    , fElemDeclPool(0)
    , fElemNonDeclPool(0)
    , fGroupElemDeclPool(0)
    , fNotationDeclPool(0)
    , fAttributeDeclRegistry(0)
    , fComplexTypeRegistry(0)
    , fGroupInfoRegistry(0)
    , fAttGroupInfoRegistry(0)
    , fNamespaceScope(0)
    , fValidSubstitutionGroups(0)
    , fValidationContext(0)
    , fMemoryManager(manager)
    , fGramDesc(0)
    , fAnnotations(0)
    , fValidated(false)
    , fDatatypeRegistry(manager)
{
    //
    //  Init all the pool members.
    //
    //  <TBD> Investigate what the optimum values would be for the various
    //  pools.
    //
    fElemDeclPool = new (fMemoryManager) RefHash3KeysIdPool<SchemaElementDecl>(109, true, 128, fMemoryManager);

    try {
        // should not be necessary now that grammars, once built,
        // are read-only
        // fElemNonDeclPool = new (fMemoryManager) RefHash3KeysIdPool<SchemaElementDecl>(29, true, 128, fMemoryManager);
        fGroupElemDeclPool = new (fMemoryManager) RefHash3KeysIdPool<SchemaElementDecl>(109, false, 128, fMemoryManager);
        fNotationDeclPool = new (fMemoryManager) NameIdPool<XMLNotationDecl>(109, 128, fMemoryManager);
        fValidationContext = new (fMemoryManager) ValidationContextImpl(fMemoryManager);
        fDatatypeRegistry.expandRegistryToFullSchemaSet();

        //REVISIT: use grammarPool to create
        fGramDesc = new (fMemoryManager) XMLSchemaDescriptionImpl(XMLUni::fgXMLNSURIName, fMemoryManager);

        // Create annotation table
        fAnnotations = new (fMemoryManager) RefHashTableOf<XSAnnotation>
        (
            29, true, new (fMemoryManager) HashPtr(), fMemoryManager
        );

        //
        //  Call our own reset method. This lets us have the pool setup stuff
        //  done in just one place (because this stame setup stuff has to be
        //  done every time we are reset.)
        //
        reset();
    }
    catch(const OutOfMemoryException&)
    {
        throw;
    }
    catch(...) {

        cleanUp();
        throw;
    }
}

SchemaGrammar::~SchemaGrammar()
{
    cleanUp();
}


// -----------------------------------------------------------------------
//  Virtual methods
// -----------------------------------------------------------------------
XMLElementDecl* SchemaGrammar::findOrAddElemDecl (const   unsigned int    uriId
        , const XMLCh* const    baseName
        , const XMLCh* const    prefixName
        , const XMLCh* const    qName
        , unsigned int          scope
        ,       bool&           wasAdded )
{
    // See it it exists
    SchemaElementDecl* retVal = (SchemaElementDecl*) getElemDecl(uriId, baseName, qName, scope);

    // if not, then add this in
    if (!retVal)
    {
        retVal = new (fMemoryManager) SchemaElementDecl
        (
            prefixName
            , baseName
            , uriId
            , SchemaElementDecl::Any
            , Grammar::TOP_LEVEL_SCOPE
            , fMemoryManager
        );
        if(!fElemNonDeclPool)
            fElemNonDeclPool = new (fMemoryManager) RefHash3KeysIdPool<SchemaElementDecl>(29, true, 128, fMemoryManager);
        const unsigned int elemId = fElemNonDeclPool->put((void*)retVal->getBaseName(), uriId, scope, retVal);
        retVal->setId(elemId);
        wasAdded = true;
    }
     else
    {
        wasAdded = false;
    }
    return retVal;
}

XMLElementDecl* SchemaGrammar::putElemDecl (const   unsigned int    uriId
        , const XMLCh* const    baseName
        , const XMLCh* const    prefixName
        , const XMLCh* const
        , unsigned int          scope
        , const bool            notDeclared)
{
    SchemaElementDecl* retVal = new (fMemoryManager) SchemaElementDecl
    (
        prefixName
        , baseName
        , uriId
        , SchemaElementDecl::Any
        , Grammar::TOP_LEVEL_SCOPE
        , fMemoryManager
    );
    if(notDeclared)
    {
        if(!fElemNonDeclPool)
            fElemNonDeclPool = new (fMemoryManager) RefHash3KeysIdPool<SchemaElementDecl>(29, true, 128, fMemoryManager);
        retVal->setId(fElemNonDeclPool->put((void*)retVal->getBaseName(), uriId, scope, retVal));
    } else 
    {
        retVal->setId(fElemDeclPool->put((void*)retVal->getBaseName(), uriId, scope, retVal));
    }
    return retVal;
}

void SchemaGrammar::reset()
{
    //
    //  We need to reset all of the pools.
    //
    fElemDeclPool->removeAll();
    if(fElemNonDeclPool)
        fElemNonDeclPool->removeAll();
    fGroupElemDeclPool->removeAll();
    fNotationDeclPool->removeAll();
    fAnnotations->removeAll();
    fValidated = false;
}


void SchemaGrammar::cleanUp()
{

    delete fElemDeclPool;
    if(fElemNonDeclPool)
        delete fElemNonDeclPool;
    delete fGroupElemDeclPool;
    delete fNotationDeclPool;
    fMemoryManager->deallocate(fTargetNamespace);//delete [] fTargetNamespace;
    delete fAttributeDeclRegistry;
    delete fComplexTypeRegistry;
    delete fGroupInfoRegistry;
    delete fAttGroupInfoRegistry;
    delete fNamespaceScope;
    delete fValidSubstitutionGroups;
    delete fValidationContext;
    delete fGramDesc;
    delete fAnnotations;
}

void SchemaGrammar::setGrammarDescription(XMLGrammarDescription* gramDesc)
{
    if ((!gramDesc) || 
        (gramDesc->getGrammarType() != Grammar::SchemaGrammarType))
        return;

    if (fGramDesc)
        delete fGramDesc;

    //adopt the grammar Description
    fGramDesc = (XMLSchemaDescription*) gramDesc;
}

// ---------------------------------------------------------------------------
//  SchemaGrammar: Helper methods
// ---------------------------------------------------------------------------
void SchemaGrammar::putAnnotation(void* key, XSAnnotation* const annotation)
{
    fAnnotations->put(key, annotation);
}

void SchemaGrammar::addAnnotation(XSAnnotation* const annotation)
{
    XSAnnotation* lAnnot = fAnnotations->get(this);
	
    if (lAnnot)
        lAnnot->setNext(annotation);
    else
        fAnnotations->put(this, annotation);
}

/***
 * Support for Serialization/De-serialization
 ***/

IMPL_XSERIALIZABLE_TOCREATE(SchemaGrammar)

void SchemaGrammar::serialize(XSerializeEngine& serEng)
{

    /***
     * don't serialize NamespaceScope*    fNamespaceScope;
     *                 ValidationContext* fValidationContext;
     *                                    fElemNonDeclPool
     ***/

    Grammar::serialize(serEng);

    if (serEng.isStoring())
    {
        //serialize DatatypeValidatorFactory first
        fDatatypeRegistry.serialize(serEng);

        /***
         *
         * Serialize RefHash3KeysIdPool<SchemaElementDecl>* fElemDeclPool;
         * Serialize RefHash3KeysIdPool<SchemaElementDecl>* fGroupElemDeclPool;
         *
        ***/
        XTemplateSerializer::storeObject(fElemDeclPool, serEng);
        XTemplateSerializer::storeObject(fGroupElemDeclPool, serEng);

        /***
         * Serialize NameIdPool<XMLNotationDecl>*           fNotationDeclPool;
         ***/
        XTemplateSerializer::storeObject(fNotationDeclPool, serEng);        
    
        /***
         *
         * Serialize RefHashTableOf<XMLAttDef>*             fAttributeDeclRegistry;
         * Serialize RefHashTableOf<ComplexTypeInfo>*       fComplexTypeRegistry;
         * Serialize RefHashTableOf<XercesGroupInfo>*       fGroupInfoRegistry;
         * Serialize RefHashTableOf<XercesAttGroupInfo>*    fAttGroupInfoRegistry;
         * Serialize RefHashTableOf<XMLRefInfo>*            fIDRefList;
         *
         ***/

        XTemplateSerializer::storeObject(fAttributeDeclRegistry, serEng);
        XTemplateSerializer::storeObject(fComplexTypeRegistry, serEng);
        XTemplateSerializer::storeObject(fGroupInfoRegistry, serEng);
        XTemplateSerializer::storeObject(fAttGroupInfoRegistry, serEng);
       
        /***
         * Serialize RefHash2KeysTableOf<ElemVector>*       fValidSubstitutionGroups;
         ***/
        XTemplateSerializer::storeObject(fValidSubstitutionGroups, serEng);

        /***
         * Serialize RefHashTableOf<XSAnnotation>*       fAnnotations;
         ***/
        XTemplateSerializer::storeObject(fAnnotations, serEng);

        serEng.writeString(fTargetNamespace);
        serEng<<fValidated;

        /***
         * serialize() method shall be used to store object
         * which has been created in ctor
         ***/
        fGramDesc->serialize(serEng);

    }
    else
    {
        fDatatypeRegistry.serialize(serEng);

        /***
         *
         * Deserialize RefHash3KeysIdPool<SchemaElementDecl>* fElemDeclPool;
         * Deserialize RefHash3KeysIdPool<SchemaElementDecl>* fGroupElemDeclPool;
         *
        ***/
        XTemplateSerializer::loadObject(&fElemDeclPool, 109, true, 128, serEng);
        XTemplateSerializer::loadObject(&fGroupElemDeclPool, 109, true, 128, serEng);

        /***
         * Deserialize NameIdPool<XMLNotationDecl>*           fNotationDeclPool;
         ***/
        XTemplateSerializer::loadObject(&fNotationDeclPool, 109, 128, serEng);        
    
        /***
         *
         * Deserialize RefHashTableOf<XMLAttDef>*             fAttributeDeclRegistry;
         * Deserialize RefHashTableOf<ComplexTypeInfo>*       fComplexTypeRegistry;
         * Deserialize RefHashTableOf<XercesGroupInfo>*       fGroupInfoRegistry;
         * Deserialize RefHashTableOf<XercesAttGroupInfo>*    fAttGroupInfoRegistry;
         * Deserialize RefHashTableOf<XMLRefInfo>*            fIDRefList;
         *
         ***/

        XTemplateSerializer::loadObject(&fAttributeDeclRegistry, 29, true, serEng);
        XTemplateSerializer::loadObject(&fComplexTypeRegistry, 29, true, serEng);
        XTemplateSerializer::loadObject(&fGroupInfoRegistry, 13, true, serEng);
        XTemplateSerializer::loadObject(&fAttGroupInfoRegistry, 13, true, serEng);
       
        /***
         * Deserialize RefHash2KeysTableOf<ElemVector>*       fValidSubstitutionGroups;
         ***/
        XTemplateSerializer::loadObject(&fValidSubstitutionGroups, 29, true, serEng);

        /***
         * Deserialize RefHashTableOf<XSAnnotation>*       fAnnotations;
         ***/
        XTemplateSerializer::loadObject(&fAnnotations, 29, true, serEng);

        serEng.readString(fTargetNamespace);
        serEng>>fValidated;

        /***
         * serialize() method shall be used to load object
         * which has been created in ctor
         ***/
        fGramDesc->serialize(serEng);

    }
}

XERCES_CPP_NAMESPACE_END
