/*
 * Copyright 2003,2004 The Apache Software Foundation.
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
 * Revision 1.24  2004/09/08 13:56:08  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.23  2004/06/09 17:01:48  gareth
 * Fixed null pointer bug. Patch by John Snelson.
 *
 * Revision 1.22  2004/05/04 19:12:42  cargilld
 * Enable IDs to work on all kinds of schema components
 *
 * Revision 1.21  2004/05/04 19:02:40  cargilld
 * Enable IDs to work on all kinds of schema components
 *
 * Revision 1.20  2004/02/05 18:09:53  cargilld
 * Fix a seg fault with PSVI and set basetype of anysimpletype to be anytype.
 *
 * Revision 1.19  2004/01/29 11:46:30  cargilld
 * Code cleanup changes to get rid of various compiler diagnostic messages.
 *
 * Revision 1.18  2004/01/06 19:07:16  knoaman
 * Fix segfault when adding S4S
 *
 * Revision 1.17  2003/12/30 21:35:46  neilg
 * even if there are no grammars to add to an XSModel, the S4S grammar must be included
 *
 * Revision 1.16  2003/12/29 16:15:42  knoaman
 * More PSVI updates
 *
 * Revision 1.15  2003/12/24 17:42:02  knoaman
 * Misc. PSVI updates
 *
 * Revision 1.14  2003/12/17 00:18:34  cargilld
 * Update to memory management so that the static memory manager (one used to call Initialize) is only for static data.
 *
 * Revision 1.13  2003/12/15 17:23:48  cargilld
 * psvi updates; cleanup revisits and bug fixes
 *
 * Revision 1.12  2003/12/13 23:35:28  neilg
 * fix bug 25494; thanks to Han Ming
 *
 * Revision 1.11  2003/12/01 20:41:47  neilg
 * do not throw an exception if there is no user-defined registry
 *
 * Revision 1.10  2003/11/26 16:12:23  knoaman
 * Add a method to return the XSObject mapped to a schema grammar component
 *
 * Revision 1.9  2003/11/25 18:08:31  knoaman
 * Misc. PSVI updates. Thanks to David Cargill.
 *
 * Revision 1.8  2003/11/21 22:34:45  neilg
 * More schema component model implementation, thanks to David Cargill.
 * In particular, this cleans up and completes the XSModel, XSNamespaceItem,
 * XSAttributeDeclaration and XSAttributeGroup implementations.
 *
 * Revision 1.7  2003/11/21 17:25:09  knoaman
 * Use XSObjectFactory to create various components.
 *
 * Revision 1.6  2003/11/17 18:38:51  neilg
 * work around HP compiler bug
 *
 * Revision 1.5  2003/11/14 22:47:53  neilg
 * fix bogus log message from previous commit...
 *
 * Revision 1.4  2003/11/14 22:33:30  neilg
 * Second phase of schema component model implementation.  
 * Implement XSModel, XSNamespaceItem, and the plumbing necessary
 * to connect them to the other components.
 * Thanks to David Cargill.
 *
 * Revision 1.3  2003/11/06 15:30:04  neilg
 * first part of PSVI/schema component model implementation, thanks to David Cargill.  This covers setting the PSVIHandler on parser objects, as well as implementing XSNotation, XSSimpleTypeDefinition, XSIDCDefinition, and most of XSWildcard, XSComplexTypeDefinition, XSElementDeclaration, XSAttributeDeclaration and XSAttributeUse.
 *
 * Revision 1.2  2003/10/10 18:37:51  neilg
 * update XSModel and XSObject interface so that IDs can be used to query components in XSModels, and so that those IDs can be recovered from components
 *
 * Revision 1.1  2003/09/16 14:33:36  neilg
 * PSVI/schema component model classes, with Makefile/configuration changes necessary to build them
 *
 */

#include <xercesc/framework/psvi/XSModel.hpp>
#include <xercesc/framework/psvi/XSNamespaceItem.hpp>
#include <xercesc/internal/XMLGrammarPoolImpl.hpp>
#include <xercesc/validators/schema/SchemaGrammar.hpp>
#include <xercesc/validators/common/GrammarResolver.hpp>
#include <xercesc/validators/schema/XercesAttGroupInfo.hpp>
#include <xercesc/validators/schema/XercesGroupInfo.hpp>
#include <xercesc/internal/XSObjectFactory.hpp>
#include <xercesc/framework/psvi/XSAttributeDeclaration.hpp>
#include <xercesc/framework/psvi/XSElementDeclaration.hpp>
#include <xercesc/framework/psvi/XSAttributeGroupDefinition.hpp>
#include <xercesc/framework/psvi/XSNotationDeclaration.hpp>
#include <xercesc/framework/psvi/XSAnnotation.hpp>
#include <xercesc/framework/psvi/XSComplexTypeDefinition.hpp>
#include <xercesc/framework/psvi/XSModelGroupDefinition.hpp>

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  XSModel: Constructors and Destructor
// ---------------------------------------------------------------------------
XSModel::XSModel( XMLGrammarPool *grammarPool
                , MemoryManager* const manager)
    : fMemoryManager(manager)
    , fNamespaceStringList(0)
    , fXSNamespaceItemList(0)
    , fURIStringPool(0)
    , fXSAnnotationList(0)
    , fHashNamespace(0)
    , fObjFactory(0)
    , fDeleteNamespace(0)
    , fParent(0)
    , fDeleteParent(false)
    , fAddedS4SGrammar(false)
{
    fURIStringPool = grammarPool->getURIStringPool();
    fObjFactory = new (fMemoryManager) XSObjectFactory(manager);

    // Populate XSNamedMaps by going through the components
    for (unsigned int i=0; i<XSConstants::MULTIVALUE_FACET; i++)
    {
        switch (i+1)
        {
            case XSConstants::ATTRIBUTE_DECLARATION:
            case XSConstants::ELEMENT_DECLARATION:
            case XSConstants::TYPE_DEFINITION:
            case XSConstants::ATTRIBUTE_GROUP_DEFINITION:
            case XSConstants::MODEL_GROUP_DEFINITION:
            case XSConstants::NOTATION_DECLARATION:
                fComponentMap[i] = new (fMemoryManager) XSNamedMap<XSObject>
                (
                    20,     // size
                    29,     // modulus
                    fURIStringPool,
                    false,  // adoptElems
                    fMemoryManager
                );
                break;
            default:
                // ATTRIBUTE_USE
                // MODEL_GROUP
                // PARTICLE
                // IDENTITY_CONSTRAINT
                // WILDCARD
                // ANNOTATION
                // FACET
                // MULTIVALUE
                fComponentMap[i] = 0;
                break;
        }
        fIdVector[i] = new (fMemoryManager) RefVectorOf<XSObject>(30, false, fMemoryManager);
    }

    fNamespaceStringList        = new (manager) RefArrayVectorOf <XMLCh>(10, true, manager);
    fXSNamespaceItemList        = new (manager) RefVectorOf <XSNamespaceItem>(10, true, manager);
    fXSAnnotationList           = new (manager) RefVectorOf <XSAnnotation> (10, false, manager);
    fHashNamespace              = new (manager) RefHashTableOf<XSNamespaceItem> (11, false, manager);

    // Loop through all grammars in the grammar pool to create the XSNamespaceItem's
    //  which will have access to Annotation Information which can be used later when
    //  we create all the XS components.
    XSNamespaceItem* namespaceItem = 0;
    RefHashTableOfEnumerator<Grammar> grammarEnum = grammarPool->getGrammarEnumerator();
    while (grammarEnum.hasMoreElements())
    {
        SchemaGrammar& sGrammar = (SchemaGrammar&) grammarEnum.nextElement();
        if (sGrammar.getGrammarType() != Grammar::SchemaGrammarType ||
            XMLString::equals(sGrammar.getTargetNamespace(), SchemaSymbols::fgURI_SCHEMAFORSCHEMA))
            continue;

        // NOTE: In the grammarpool, preprocessed grammars without targetnamespace
        //       will use an empty string...
        XMLCh* NameSpace = XMLString::replicate(sGrammar.getTargetNamespace(), manager);
        fNamespaceStringList->addElement(NameSpace);
        namespaceItem = new (manager) XSNamespaceItem(this, &sGrammar, manager);
        fXSNamespaceItemList->addElement(namespaceItem);
        fHashNamespace->put(NameSpace, namespaceItem);
    }

    // Now loop through all of the NamespaceItem's
    // First, we add S4S namespace (irrespective of whether we have any grammars)
    namespaceItem = new (manager) XSNamespaceItem
    (
        this, SchemaSymbols::fgURI_SCHEMAFORSCHEMA, manager
    );

    fNamespaceStringList->addElement
    (
        XMLString::replicate(SchemaSymbols::fgURI_SCHEMAFORSCHEMA,manager)
    );
    fXSNamespaceItemList->addElement(namespaceItem);
    fHashNamespace->put
    (
        (void*) SchemaSymbols::fgURI_SCHEMAFORSCHEMA
        , namespaceItem
    );

    DatatypeValidatorFactory dvFactory(manager);
    dvFactory.expandRegistryToFullSchemaSet();
    addS4SToXSModel
    (
        namespaceItem
        , dvFactory.getBuiltInRegistry()
    );
    // don't include  S4S (thus the -1)
    unsigned int numberOfNamespaces = fXSNamespaceItemList->size() -1;
    for (unsigned int j = 0; j < numberOfNamespaces; j++)
        addGrammarToXSModel(fXSNamespaceItemList->elementAt(j));
}

XSModel::XSModel( XSModel *baseModel
                , GrammarResolver *grammarResolver
                , MemoryManager* const manager)
    : fMemoryManager(manager)
    , fNamespaceStringList(0)
    , fXSNamespaceItemList(0)
    , fURIStringPool(0)
    , fXSAnnotationList(0)
    , fHashNamespace(0)    
    , fObjFactory(0)
    , fDeleteNamespace(0)
    , fParent(baseModel)
    , fDeleteParent(true)
    , fAddedS4SGrammar(false)
{
    fURIStringPool = grammarResolver->getStringPool();
    fObjFactory = new (manager) XSObjectFactory(manager);

    unsigned int i;
    // Populate XSNamedMaps by going through the components
    for (i=0; i<XSConstants::MULTIVALUE_FACET; i++)
    {
        switch (i+1)
        {
            case XSConstants::ATTRIBUTE_DECLARATION:
            case XSConstants::ELEMENT_DECLARATION:
            case XSConstants::TYPE_DEFINITION:
            case XSConstants::ATTRIBUTE_GROUP_DEFINITION:
            case XSConstants::MODEL_GROUP_DEFINITION:
            case XSConstants::NOTATION_DECLARATION:
                fComponentMap[i] = new (fMemoryManager) XSNamedMap<XSObject>
                (
                    20,     // size
                    29,     // modulus
                    fURIStringPool,
                    false,  // adoptElems
                    fMemoryManager
                );
                break;
            default:
                // ATTRIBUTE_USE
                // MODEL_GROUP
                // PARTICLE
                // IDENTITY_CONSTRAINT
                // WILDCARD
                // ANNOTATION
                // FACET
                // MULTIVALUE
                fComponentMap[i] = 0;
                break;
        }
        fIdVector[i] = new (fMemoryManager) RefVectorOf<XSObject>(30, false, fMemoryManager);
    }

    fNamespaceStringList        = new (manager) RefArrayVectorOf <XMLCh>(10, true, manager);
    fXSNamespaceItemList        = new (manager) RefVectorOf <XSNamespaceItem>(10, false, manager);
    fDeleteNamespace            = new (manager) RefVectorOf <XSNamespaceItem>(10, true, manager);
    fXSAnnotationList           = new (manager) RefVectorOf <XSAnnotation> (10, false, manager); 
    fHashNamespace              = new (manager) RefHashTableOf<XSNamespaceItem> (11, false, manager);

    if (fParent)
    {
        if (fParent->fAddedS4SGrammar)
            fAddedS4SGrammar = true;

        // Need to copy information from parent so it can be returned in this object...
        for (i=0; i<fParent->fXSNamespaceItemList->size(); i++)
        {
            XSNamespaceItem* namespaceItem = fParent->fXSNamespaceItemList->elementAt(i);
            fXSNamespaceItemList->addElement(namespaceItem);
            fNamespaceStringList->addElement
            (
                XMLString::replicate
                (
                    namespaceItem->getSchemaNamespace(), manager
                )
            );
        }

        for (i=0; i<XSConstants::MULTIVALUE_FACET; i++)
        {
            switch (i+1) 
            {
                case XSConstants::ATTRIBUTE_DECLARATION:
                case XSConstants::ELEMENT_DECLARATION:
                case XSConstants::TYPE_DEFINITION:
                case XSConstants::ATTRIBUTE_GROUP_DEFINITION:
                case XSConstants::MODEL_GROUP_DEFINITION:
                case XSConstants::NOTATION_DECLARATION:
                    for (unsigned int j=0; j<fParent->fComponentMap[i]->getLength(); j++)
                    {
                        XSObject* copyObj = fParent->fComponentMap[i]->item(j);
                        fComponentMap[i]->addElement(copyObj,
                                                     copyObj->getName(),
                                                     copyObj->getNamespace());
                    }
                break;
            }
            for (unsigned int j=0; j<fParent->fIdVector[i]->size(); j++)
            {
                fIdVector[i]->addElement(fParent->fIdVector[i]->elementAt(j));
            }
        }

        for (i=0; i<fParent->fXSAnnotationList->size(); i++)
        {
            fXSAnnotationList->addElement(fParent->fXSAnnotationList->elementAt(i));
        }

    } // end of copying parent info

    // Now add information from the new grammars but first create the
    // XSNamespaceItem's so we can have access to the XSAnnotations...
    ValueVectorOf<SchemaGrammar*>* grammarsToAdd = grammarResolver->getGrammarsToAddToXSModel();
    unsigned int numberOfNamespaces = fXSNamespaceItemList->size();
    unsigned int numberOfNamespacesToAdd = 0;
    for (i=0; i < grammarsToAdd->size(); i++)
    {
        SchemaGrammar* lGrammar = grammarsToAdd->elementAt(i);
        if (lGrammar->getGrammarType() != Grammar::SchemaGrammarType ||
            XMLString::equals(lGrammar->getTargetNamespace(), SchemaSymbols::fgURI_SCHEMAFORSCHEMA))
            continue;

        XMLCh* NameSpace = XMLString::replicate(lGrammar->getTargetNamespace(), manager);
        fNamespaceStringList->addElement(NameSpace);

        XSNamespaceItem* namespaceItem = new (manager) XSNamespaceItem(this, lGrammar, manager);
        fXSNamespaceItemList->addElement(namespaceItem);
        fHashNamespace->put(NameSpace, namespaceItem);
        fDeleteNamespace->addElement(namespaceItem);
        ++numberOfNamespacesToAdd;
    }

    // Add S4S namespace if needed
    if (!fAddedS4SGrammar)
    {
        DatatypeValidatorFactory dvFactory(manager);
        dvFactory.expandRegistryToFullSchemaSet();

        XSNamespaceItem* namespaceItem = new (manager) XSNamespaceItem
        (
            this, SchemaSymbols::fgURI_SCHEMAFORSCHEMA, manager
        );

        fNamespaceStringList->addElement
        (
            XMLString::replicate(SchemaSymbols::fgURI_SCHEMAFORSCHEMA,manager)
        );
        fXSNamespaceItemList->addElement(namespaceItem);
        fHashNamespace->put
        (
            (void*) SchemaSymbols::fgURI_SCHEMAFORSCHEMA , namespaceItem
        );
        fDeleteNamespace->addElement(namespaceItem);
        addS4SToXSModel
        (
            namespaceItem
            , dvFactory.getBuiltInRegistry()
        );
    }

    // Now loop through all of the newly created NamespaceItem's
    for (i=numberOfNamespaces; i<(numberOfNamespaces+numberOfNamespacesToAdd); i++)
    {
        addGrammarToXSModel(fXSNamespaceItemList->elementAt(i));
    } // end of namespaceItem loop
}

XSModel::~XSModel()
{
    for (unsigned int i=0; i<XSConstants::MULTIVALUE_FACET; i++)
    {
        switch (i+1)
        {
            case XSConstants::ATTRIBUTE_DECLARATION:
            case XSConstants::ELEMENT_DECLARATION:
            case XSConstants::TYPE_DEFINITION:
            case XSConstants::ATTRIBUTE_GROUP_DEFINITION:
            case XSConstants::MODEL_GROUP_DEFINITION:
            case XSConstants::NOTATION_DECLARATION:
                delete fComponentMap[i];
                break;
        }
        delete fIdVector[i];
    }

    delete fNamespaceStringList;
    delete fXSNamespaceItemList;
    delete fXSAnnotationList;
    delete fHashNamespace;
    delete fObjFactory;

    if (fDeleteNamespace)
        delete fDeleteNamespace;

    if (fDeleteParent && fParent && fParent->fDeleteParent)
        delete fParent;
}

// ---------------------------------------------------------------------------
//  XSModel: Helper methods
// ---------------------------------------------------------------------------
void XSModel::addComponentToIdVector(XSObject* const component,
                                     int componentIndex)
{
    component->setId(fIdVector[componentIndex]->size());
    fIdVector[componentIndex]->addElement(component);
}


void XSModel::addComponentToNamespace(XSNamespaceItem* const namespaceItem,
                                      XSObject* const component,
                                      int componentIndex,
                                      bool addToXSModel)
{
    namespaceItem->fComponentMap[componentIndex]->addElement
    (
        component, component->getName(), namespaceItem->getSchemaNamespace()
    );
    namespaceItem->fHashMap[componentIndex]->put
    (
        (void *) component->getName(), component
    );

    if (addToXSModel)
    {
        fComponentMap[componentIndex]->addElement
        (
            component, component->getName(), namespaceItem->getSchemaNamespace()
        );
    }
}

void
XSModel::addS4SToXSModel(XSNamespaceItem* const namespaceItem,
                         RefHashTableOf<DatatypeValidator>* const builtInDV)
{
    addComponentToNamespace
    (
        namespaceItem
        , fObjFactory->addOrFind
          (
              ComplexTypeInfo::getAnyType
              (
                  fURIStringPool->getId(XMLUni::fgZeroLenString)
              )
              , this
          )
        , XSConstants::TYPE_DEFINITION - 1
    );

    // Loop through built-in simple types
    // First add 'anySimpleType' which is the base for the other built-ins
    DatatypeValidator* dv = builtInDV->get(SchemaSymbols::fgDT_ANYSIMPLETYPE);
    addComponentToNamespace
    (
        namespaceItem
        , fObjFactory->addOrFind(dv, this, true)
        , XSConstants::TYPE_DEFINITION - 1
    );

    // add remaining built-in
    RefHashTableOfEnumerator<DatatypeValidator> simpleEnum = 
        RefHashTableOfEnumerator<DatatypeValidator> (builtInDV, false, fMemoryManager);
    while (simpleEnum.hasMoreElements())
    {
        DatatypeValidator& curSimple = simpleEnum.nextElement();
        if (&curSimple == dv)
            continue;

        addComponentToNamespace
        (
            namespaceItem
            , fObjFactory->addOrFind(&curSimple, this)
            , XSConstants::TYPE_DEFINITION - 1
        );
    }

    // Set flag to indicate that we have added S4S grammar info
    fAddedS4SGrammar = true;
}


void XSModel::addGrammarToXSModel(XSNamespaceItem* namespaceItem)
{
    // Loop through top-level attribute declarations in the grammar...
    RefHashTableOf<XMLAttDef>* attDeclRegistry = namespaceItem->fGrammar->getAttributeDeclRegistry();
    if(attDeclRegistry) {
        RefHashTableOfEnumerator<XMLAttDef> attrEnum = RefHashTableOfEnumerator<XMLAttDef> (attDeclRegistry, false, fMemoryManager);
        while (attrEnum.hasMoreElements())
        {
            XSAttributeDeclaration* xsAttrDecl = fObjFactory->addOrFind
            (
                (SchemaAttDef*) &(attrEnum.nextElement()), this
            );

            addComponentToNamespace
            (
                namespaceItem, xsAttrDecl, XSConstants::ATTRIBUTE_DECLARATION - 1
            );
        } // end of attribute loop
    }

    // Loop through top-level elements in the grammar...
    RefHash3KeysIdPoolEnumerator<SchemaElementDecl> elemEnum = namespaceItem->fGrammar->getElemEnumerator();    
    while (elemEnum.hasMoreElements())
    {
        SchemaElementDecl& curElem = elemEnum.nextElement();
        if (curElem.getEnclosingScope() == Grammar::TOP_LEVEL_SCOPE) 
        {
            XSElementDeclaration* xsElemDecl = fObjFactory->addOrFind
            (
                &curElem, this
            );

            addComponentToNamespace
            (
                namespaceItem, xsElemDecl, XSConstants::ELEMENT_DECLARATION -1
            );
        }
    } // end of element loop

    // Now loop through top-level User Defined simple type definitions in the grammar...
    DVHashTable* dvHT = namespaceItem->fGrammar->getDatatypeRegistry()->getUserDefinedRegistry();
    if (dvHT)
    {
        RefHashTableOfEnumerator<DatatypeValidator> simpleUserEnum = RefHashTableOfEnumerator<DatatypeValidator> (dvHT, false, fMemoryManager);
        while (simpleUserEnum.hasMoreElements())
        {
            DatatypeValidator& curSimple = simpleUserEnum.nextElement();
            if (!curSimple.getAnonymous())
            {
                addComponentToNamespace
                (
                    namespaceItem
                    , fObjFactory->addOrFind(&curSimple, this)
                    , XSConstants::TYPE_DEFINITION - 1
                );
            }
        } // end of simple User loop
    }

    // Loop through top-level COMPLEX type definitions in the grammar...
    RefHashTableOf<ComplexTypeInfo>* complexTypeRegistry = namespaceItem->fGrammar->getComplexTypeRegistry();
    if(complexTypeRegistry) {
        RefHashTableOfEnumerator<ComplexTypeInfo> complexEnum = RefHashTableOfEnumerator<ComplexTypeInfo> (complexTypeRegistry, false, fMemoryManager);
        while (complexEnum.hasMoreElements())
        {
            ComplexTypeInfo&  curComplex = complexEnum.nextElement();
            if (!curComplex.getAnonymous())
            {
                addComponentToNamespace
                (
                    namespaceItem
                    , fObjFactory->addOrFind(&curComplex, this)
                    , XSConstants::TYPE_DEFINITION - 1
                );
            }
        }  // end of type definition loop
    }

    // Loop through top-level attribute group definitions in the grammar...
    RefHashTableOf<XercesAttGroupInfo>* attGroupInfoRegistry = namespaceItem->fGrammar->getAttGroupInfoRegistry();
    if(attGroupInfoRegistry) {
        RefHashTableOfEnumerator<XercesAttGroupInfo> attrGroupEnum = RefHashTableOfEnumerator<XercesAttGroupInfo> (attGroupInfoRegistry, false, fMemoryManager);
        while (attrGroupEnum.hasMoreElements())
        {
            addComponentToNamespace
            (
                namespaceItem
                , fObjFactory->createXSAttGroupDefinition
                  (
                      &(attrGroupEnum.nextElement()), this
                  )
                , XSConstants::ATTRIBUTE_GROUP_DEFINITION - 1
            );
        } // end of attribute group loop
    }

    // Loop through top-level model group definitions in the grammar...
    RefHashTableOf<XercesGroupInfo>* groupInfoRegistry = namespaceItem->fGrammar->getGroupInfoRegistry();
    if(groupInfoRegistry) {
        RefHashTableOfEnumerator<XercesGroupInfo> modelGroupEnum = RefHashTableOfEnumerator<XercesGroupInfo> (groupInfoRegistry, false, fMemoryManager);
        while (modelGroupEnum.hasMoreElements())
        {
            addComponentToNamespace
            (
                namespaceItem
                , fObjFactory->createXSModelGroupDefinition
                  (
                      &(modelGroupEnum.nextElement()), this
                  )
                , XSConstants::MODEL_GROUP_DEFINITION - 1
            );
        } // end of model group loop
    }

    // Loop through notations in the grammar...    
    NameIdPoolEnumerator<XMLNotationDecl> notationEnum = namespaceItem->fGrammar->getNotationEnumerator();
    while (notationEnum.hasMoreElements())
    {
        addComponentToNamespace
        (
            namespaceItem
            , fObjFactory->addOrFind(&(notationEnum.nextElement()), this)
            , XSConstants::NOTATION_DECLARATION - 1
        );
    } // end of notation loop

    // Loop through annotations in the grammar...
    // As annotations are already created as XSAnnotations no need to create them
    // or store them in the XercesToXSMap.
    XSAnnotation* annot = namespaceItem->fGrammar->getAnnotation();
    while (annot)
    {
        fXSAnnotationList->addElement(annot);
        namespaceItem->fXSAnnotationList->addElement(annot);
        addComponentToIdVector(annot, XSConstants::ANNOTATION -1);
        annot = annot->getNext();
    } // end of annotation loop
}




// ---------------------------------------------------------------------------
//  XSModel: Access methods
// ---------------------------------------------------------------------------
/**
 * [schema components]: a list of top-level components, i.e. element 
 * declarations, attribute declarations, etc. 
 * @param objectType The type of the declaration, i.e. 
 *   <code>ELEMENT_DECLARATION</code>, 
 *   <code>TYPE_DEFINITION</code> and any other component type that
 * may be a property of a schema component.
 * @return A list of top-level definition of the specified type in 
 *   <code>objectType</code> or <code>null</code>. 
 */
XSNamedMap <XSObject> *XSModel::getComponents(XSConstants::COMPONENT_TYPE objectType)
{
    return fComponentMap[objectType -1];
}

/**
 * Convenience method. Returns a list of top-level component declarations 
 * that are defined within the specified namespace, i.e. element 
 * declarations, attribute declarations, etc. 
 * @param objectType The type of the declaration, i.e. 
 *   <code>ELEMENT_DECLARATION</code>.
 * @param compNamespace The namespace to which declaration belong or 
 *   <code>null</code> (for components with no target namespace).
 * @return A list of top-level definitions of the specified type in 
 *   <code>objectType</code> and defined in the specified 
 *   <code>namespace</code> or <code>null</code>. 
 */
XSNamedMap <XSObject> *XSModel::getComponentsByNamespace(XSConstants::COMPONENT_TYPE objectType,
                                               const XMLCh *compNamespace)
{
    XSNamespaceItem* namespaceItem;
    if (compNamespace)
        namespaceItem = getNamespaceItem(compNamespace);
    else
        namespaceItem = getNamespaceItem(XMLUni::fgZeroLenString);
    
    if (namespaceItem)
        return namespaceItem->getComponents(objectType);

    return 0;
}

/**
 *  [annotations]: a set of annotations.
 */
XSAnnotationList *XSModel::getAnnotations()
{
    return fXSAnnotationList;
}

/**
 * Convenience method. Returns a top-level element declaration. 
 * @param name The name of the declaration.
 * @param compNamespace The namespace of the declaration, null if absent.
 * @return A top-level element declaration or <code>null</code> if such 
 *   declaration does not exist. 
 */
XSElementDeclaration *XSModel::getElementDeclaration(const XMLCh *name
            , const XMLCh *compNamespace)
{
    XSNamespaceItem* namespaceItem;
    if (compNamespace)
        namespaceItem = getNamespaceItem(compNamespace);
    else
        namespaceItem = getNamespaceItem(XMLUni::fgZeroLenString);

    if (namespaceItem)
        return namespaceItem->getElementDeclaration(name);

    return 0;
}

/**
 * Convenience method. Returns a top-level attribute declaration. 
 * @param name The name of the declaration.
 * @param compNamespace The namespace of the declaration, null if absent.
 * @return A top-level attribute declaration or <code>null</code> if such 
 *   declaration does not exist. 
 */
XSAttributeDeclaration *XSModel::getAttributeDeclaration(const XMLCh *name
            , const XMLCh *compNamespace)
{
    XSNamespaceItem* namespaceItem;
    if (compNamespace)
        namespaceItem = getNamespaceItem(compNamespace);
    else
        namespaceItem = getNamespaceItem(XMLUni::fgZeroLenString);

    if (namespaceItem)
        return namespaceItem->getAttributeDeclaration(name);

    return 0;
}

/**
 * Convenience method. Returns a top-level simple or complex type 
 * definition. 
 * @param name The name of the definition.
 * @param compNamespace The namespace of the declaration, null if absent.
 * @return An <code>XSTypeDefinition</code> or <code>null</code> if such 
 *   definition does not exist. 
 */
XSTypeDefinition *XSModel::getTypeDefinition(const XMLCh *name
            , const XMLCh *compNamespace)
{
    XSNamespaceItem* namespaceItem;
    if (compNamespace)
        namespaceItem = getNamespaceItem(compNamespace);
    else
        namespaceItem = getNamespaceItem(XMLUni::fgZeroLenString);
    
    if (namespaceItem)
        return namespaceItem->getTypeDefinition(name);

    return 0;
}

/**
 * Convenience method. Returns a top-level attribute group definition. 
 * @param name The name of the definition.
 * @param compNamespace The namespace of the declaration, null if absent.
 * @return A top-level attribute group definition or <code>null</code> if 
 *   such definition does not exist. 
 */
XSAttributeGroupDefinition *XSModel::getAttributeGroup(const XMLCh *name
            , const XMLCh *compNamespace)
{
    XSNamespaceItem* namespaceItem;
    if (compNamespace)
        namespaceItem = getNamespaceItem(compNamespace);
    else
        namespaceItem = getNamespaceItem(XMLUni::fgZeroLenString);

    if (namespaceItem)
        return namespaceItem->getAttributeGroup(name);

    return 0;
}

/**
 * Convenience method. Returns a top-level model group definition. 
 * @param name The name of the definition.
 * @param compNamespace The namespace of the declaration, null if absent.
 * @return A top-level model group definition definition or 
 *   <code>null</code> if such definition does not exist. 
 */
XSModelGroupDefinition *XSModel::getModelGroupDefinition(const XMLCh *name
            , const XMLCh *compNamespace)
{
    XSNamespaceItem* namespaceItem;
    if (compNamespace)
        namespaceItem = getNamespaceItem(compNamespace);
    else
        namespaceItem = getNamespaceItem(XMLUni::fgZeroLenString);

    if (namespaceItem)
        return namespaceItem->getModelGroupDefinition(name);

    return 0;
}

/**
 * Convenience method. Returns a top-level notation declaration. 
 * @param name The name of the declaration.
 * @param compNamespace The namespace of the declaration, null if absent.
 * @return A top-level notation declaration or <code>null</code> if such 
 *   declaration does not exist. 
 */
XSNotationDeclaration *XSModel::getNotationDeclaration(const XMLCh *name
            , const XMLCh *compNamespace)
{
    XSNamespaceItem* namespaceItem;
    if (compNamespace)
        namespaceItem = getNamespaceItem(compNamespace);
    else
        namespaceItem = getNamespaceItem(XMLUni::fgZeroLenString);

    if (namespaceItem)
        return namespaceItem->getNotationDeclaration(name);

    return 0;
}

/**
  * Optional.  Return a component given a component type and a unique Id.  
  * May not be supported for all component types.
  * @param compId unique Id of the component within its type
  * @param compType type of the component
  * @return the component of the given type with the given Id, or 0
  * if no such component exists or this is unsupported for
  * this type of component.
  */
XSObject *XSModel::getXSObjectById(unsigned int  compId
            , XSConstants::COMPONENT_TYPE compType)
{
    if (compId < fIdVector[compType -1]->size())
        return fIdVector[compType -1]->elementAt(compId);

    return 0;
}

XSNamespaceItem* XSModel::getNamespaceItem(const XMLCh* const key)
{
    XSNamespaceItem* xsName = fHashNamespace->get(key);
    if (xsName)
        return xsName;
    if (fParent)
        return fParent->getNamespaceItem(key);
    return 0;
}

XSObject* XSModel::getXSObject(void* key)
{
    XSObject* xsObj = fObjFactory->getObjectFromMap(key);

    if (!xsObj && fParent)
        xsObj = fParent->getXSObject(key);

    return xsObj;
}


XERCES_CPP_NAMESPACE_END


