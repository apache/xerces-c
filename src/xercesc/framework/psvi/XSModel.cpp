/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 2003 The Apache Software Foundation.  All rights
 * reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. The end-user documentation included with the redistribution,
 *    if any, must include the following acknowledgment:
 *       "This product includes software developed by the
 *        Apache Software Foundation (http://www.apache.org/)."
 *    Alternately, this acknowledgment may appear in the software itself,
 *    if and wherever such third-party acknowledgments normally appear.
 *
 * 4. The names "Xerces" and "Apache Software Foundation" must
 *    not be used to endorse or promote products derived from this
 *    software without prior written permission. For written
 *    permission, please contact apache\@apache.org.
 *
 * 5. Products derived from this software may not be called "Apache",
 *    nor may "Apache" appear in their name, without prior written
 *    permission of the Apache Software Foundation.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE APACHE SOFTWARE FOUNDATION OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * ====================================================================
 *
 * This software consists of voluntary contributions made by many
 * individuals on behalf of the Apache Software Foundation, and was
 * originally based on software copyright (c) 1999, International
 * Business Machines, Inc., http://www.ibm.com .  For more information
 * on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 */

/*
 * $Log$
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

// local constant
static const XMLCh emptyString[]  = {0};

// the local routine to add a grammar to the XSModel
void XSModel::addGrammarToXSModel(XSNamespaceItem* namespaceItem)
{
    // First loop through top-level BUILTIN simple type definitions in the grammar...
    // all grammar's would be the same...    
    RefHashTableOfEnumerator<DatatypeValidator> simpleEnum = RefHashTableOfEnumerator<DatatypeValidator> (namespaceItem->getSchemaGrammar()->getDatatypeRegistry()->getBuiltInRegistry());
    while (simpleEnum.hasMoreElements())
    {
        DatatypeValidator& curSimple = simpleEnum.nextElement();
        XSSimpleTypeDefinition* xsSimple;
        xsSimple = fObjFactory->addOrFind(&curSimple, this);
        if (!fAddedBuiltinDatatypeValidators)
        {       
            fComponentMap[XSConstants::TYPE_DEFINITION -1]->addElement(xsSimple, xsSimple->getName(), namespaceItem->getSchemaNamespace());          
        }
        namespaceItem->fComponentMap[XSConstants::TYPE_DEFINITION -1]->addElement(xsSimple, xsSimple->getName(), namespaceItem->getSchemaNamespace());
        namespaceItem->fHashMap[XSConstants::TYPE_DEFINITION -1]->put((void *) xsSimple->getName(), xsSimple);
    } 
    fAddedBuiltinDatatypeValidators = true;
    // end of simple BuiltIn loop

    // Loop through top-level attribute declarations in the grammar...
    RefHashTableOfEnumerator<XMLAttDef> attrEnum = RefHashTableOfEnumerator<XMLAttDef> (namespaceItem->getSchemaGrammar()->getAttributeDeclRegistry());
        
    while (attrEnum.hasMoreElements())
    {
        SchemaAttDef* attDef = (SchemaAttDef*) &(attrEnum.nextElement());
        XSAttributeDeclaration* xsAttrDecl = fObjFactory->addOrFind(attDef, this);
        fComponentMap[XSConstants::ATTRIBUTE_DECLARATION-1]->addElement(xsAttrDecl, xsAttrDecl->getName(), namespaceItem->getSchemaNamespace());
        xsAttrDecl->setId(fAttributeDeclarationVector->size());
        fAttributeDeclarationVector->addElement(xsAttrDecl);
        namespaceItem->fComponentMap[XSConstants::ATTRIBUTE_DECLARATION-1]->addElement(xsAttrDecl, xsAttrDecl->getName(), namespaceItem->getSchemaNamespace());
        namespaceItem->fHashMap[XSConstants::ATTRIBUTE_DECLARATION -1]->put((void *) xsAttrDecl->getName(), xsAttrDecl);
    } // end of attribute loop

    // Loop through top-level elements in the grammar...
    RefHash3KeysIdPoolEnumerator<SchemaElementDecl> elemEnum = namespaceItem->getSchemaGrammar()->getElemEnumerator();
    
    while (elemEnum.hasMoreElements())
    {
        SchemaElementDecl& curElem = elemEnum.nextElement();
        if (curElem.getEnclosingScope() == Grammar::TOP_LEVEL_SCOPE) 
        {
            XSElementDeclaration* xsElemDecl = fObjFactory->addOrFind(&curElem, this);

            fComponentMap[XSConstants::ELEMENT_DECLARATION-1]->addElement(xsElemDecl, xsElemDecl->getName(), namespaceItem->getSchemaNamespace());
            xsElemDecl->setId(fElementDeclarationVector->size());
            fElementDeclarationVector->addElement(xsElemDecl);
            namespaceItem->fComponentMap[XSConstants::ELEMENT_DECLARATION -1]->addElement(xsElemDecl, xsElemDecl->getName(), namespaceItem->getSchemaNamespace());
            namespaceItem->fHashMap[XSConstants::ELEMENT_DECLARATION -1]->put((void *) xsElemDecl->getName(), xsElemDecl);
        }
    } // end of element loop

    // Now loop through top-level User Defined simple type definitions in the grammar...   
    RefHashTableOfEnumerator<DatatypeValidator> simpleUserEnum = RefHashTableOfEnumerator<DatatypeValidator> (namespaceItem->getSchemaGrammar()->getDatatypeRegistry()->getUserDefinedRegistry());
    while (simpleUserEnum.hasMoreElements())
    {
        DatatypeValidator& curSimple = simpleUserEnum.nextElement();
        if (!curSimple.getAnonymous())
        {
            XSSimpleTypeDefinition* xsSimple = fObjFactory->addOrFind(&curSimple, this);
            fComponentMap[XSConstants::TYPE_DEFINITION -1]->addElement(xsSimple, xsSimple->getName(), namespaceItem->getSchemaNamespace());
            namespaceItem->fComponentMap[XSConstants::TYPE_DEFINITION -1]->addElement(xsSimple, xsSimple->getName(), namespaceItem->getSchemaNamespace());
            namespaceItem->fHashMap[XSConstants::TYPE_DEFINITION -1]->put((void *) xsSimple->getName(), xsSimple);                
        }            
    }
    // end of simple User loop

    // Loop through top-level COMPLEX type definitions in the grammar...        
    RefHashTableOfEnumerator<ComplexTypeInfo> complexEnum = RefHashTableOfEnumerator<ComplexTypeInfo> (namespaceItem->getSchemaGrammar()->getComplexTypeRegistry());      
    while (complexEnum.hasMoreElements())
    {
        ComplexTypeInfo&  curComplex = complexEnum.nextElement();           
        if (!curComplex.getAnonymous())
        {
            XSComplexTypeDefinition* xsComplex = fObjFactory->addOrFind(&curComplex, this);
            fComponentMap[XSConstants::TYPE_DEFINITION -1]->addElement(xsComplex, xsComplex->getName(), namespaceItem->getSchemaNamespace());
            namespaceItem->fComponentMap[XSConstants::TYPE_DEFINITION -1]->addElement(xsComplex, xsComplex->getName(), namespaceItem->getSchemaNamespace());
            namespaceItem->fHashMap[XSConstants::TYPE_DEFINITION -1]->put((void *) xsComplex->getName(), xsComplex);                
        }            
    }  // end of type definition loop

    // Loop through top-level attribute group definitions in the grammar...
    RefHashTableOfEnumerator<XercesAttGroupInfo> attrGroupEnum = RefHashTableOfEnumerator<XercesAttGroupInfo> (namespaceItem->getSchemaGrammar()->getAttGroupInfoRegistry());
    while (attrGroupEnum.hasMoreElements())
    {
        XercesAttGroupInfo& curAttrGroup = attrGroupEnum.nextElement();
        XSAttributeGroupDefinition* xsAttrGroupDecl = fObjFactory->createXSAttGroupDefinition(&curAttrGroup, this);
        fComponentMap[XSConstants::ATTRIBUTE_GROUP_DEFINITION -1]->addElement(xsAttrGroupDecl, xsAttrGroupDecl->getName(), namespaceItem->getSchemaNamespace());
        namespaceItem->fComponentMap[XSConstants::ATTRIBUTE_GROUP_DEFINITION -1]->addElement(xsAttrGroupDecl, xsAttrGroupDecl->getName(), namespaceItem->getSchemaNamespace());
        namespaceItem->fHashMap[XSConstants::ATTRIBUTE_GROUP_DEFINITION -1]->put((void *) xsAttrGroupDecl->getName(), xsAttrGroupDecl);                
    } // end of attribute group loop
        
    // Loop through top-level model group definitions in the grammar...    
    // REVISIT: how to see if a model group definition is global or local?
    RefHashTableOfEnumerator<XercesGroupInfo> modelGroupEnum = RefHashTableOfEnumerator<XercesGroupInfo> (namespaceItem->getSchemaGrammar()->getGroupInfoRegistry());
    while (modelGroupEnum.hasMoreElements())
    {
        XercesGroupInfo& curModelGroup = modelGroupEnum.nextElement();
        XSModelGroupDefinition* xsModelGroupDecl = fObjFactory->createXSModelGroupDefinition(&curModelGroup, this);
        fComponentMap[XSConstants::MODEL_GROUP_DEFINITION -1]->addElement(xsModelGroupDecl, xsModelGroupDecl->getName(), namespaceItem->getSchemaNamespace());
        namespaceItem->fComponentMap[XSConstants::MODEL_GROUP_DEFINITION -1]->addElement(xsModelGroupDecl, xsModelGroupDecl->getName(), namespaceItem->getSchemaNamespace());
        namespaceItem->fHashMap[XSConstants::MODEL_GROUP_DEFINITION -1]->put((void *) xsModelGroupDecl->getName(), xsModelGroupDecl);                
    } // end of model group loop

    // Loop through notations in the grammar...    
    NameIdPoolEnumerator<XMLNotationDecl> notationEnum = namespaceItem->getSchemaGrammar()->getNotationEnumerator();
    while (notationEnum.hasMoreElements())
    {
        // REVISIT: do we need to store mapping between XMLNotationDecl objects and
        //          XSNotationDeclaration objects?  PSVIElement may need it to 
        //          get the XSNotationDecl...
        XMLNotationDecl& notationDecl = notationEnum.nextElement();            
        XSNotationDeclaration* xsNotationDecl = fObjFactory->addOrFind(&notationDecl, this);

        fComponentMap[XSConstants::NOTATION_DECLARATION -1]->addElement(xsNotationDecl, xsNotationDecl->getName(), namespaceItem->getSchemaNamespace());
        namespaceItem->fComponentMap[XSConstants::NOTATION_DECLARATION -1]->addElement(xsNotationDecl, xsNotationDecl->getName(), namespaceItem->getSchemaNamespace());
        namespaceItem->fHashMap[XSConstants::NOTATION_DECLARATION -1]->put((void *) xsNotationDecl->getName(), xsNotationDecl);                
    } // end of notation loop

    // Loop through annotations in the grammar...
    // As annotations are already created as XSAnnotations no need to create them
    // or store them in the XercesToXSMap.
    XSAnnotation* annot = namespaceItem->getSchemaGrammar()->getAnnotation();
    while (annot)
    {     
        fXSAnnotationList->addElement(annot);
        namespaceItem->fXSAnnotationList->addElement(annot);
        annot = annot->getNext();        
    } // end of annotation loop
}

/**
  * The constructor to be used when a grammar pool contains all needed info
  *
  * @param grammarPool  the grammar pool containing the underlying data structures
  * @param  manager     The configurable memory manager
  */
XSModel::XSModel( XMLGrammarPool *grammarPool
                , MemoryManager* const manager)
    : fMemoryManager(manager)
    , fNamespaceStringList(0)
    , fXSNamespaceItemList(0)
    , fElementDeclarationVector(0)
    , fAttributeDeclarationVector(0)
    , fURIStringPool(0)
    , fXSAnnotationList(0)
    , fHashNamespace(0)
    , fObjFactory(0)
    , fDeleteNamespace(0)
    , fParent(0)
    , fDeleteParent(false)
    , fAddedBuiltinDatatypeValidators(false)
{
    fURIStringPool = grammarPool->getURIStringPool();
    fObjFactory = new (fMemoryManager) XSObjectFactory(manager);

    // Populate XSNamedMaps by going through the components
    for (unsigned int i=0; i<XSConstants::MULTIVALUE_FACET; i++)
    {
        // REVISIT: what size & modulus
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
                    29,     // size
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
    }
   
    // Revisit: size of vector
    fNamespaceStringList        = new (manager) RefArrayVectorOf <XMLCh>(10, true, manager);
    fXSNamespaceItemList        = new (manager) RefVectorOf <XSNamespaceItem>(10, true, manager);
    fXSAnnotationList           = new (manager) RefVectorOf <XSAnnotation> (10, false, manager);
    fElementDeclarationVector   = new (manager) RefVectorOf<XSElementDeclaration> (10, false, manager);
    fAttributeDeclarationVector = new (manager) RefVectorOf<XSAttributeDeclaration>  (10, false, manager);
    fHashNamespace              = new (manager) RefHashTableOf<XSNamespaceItem> (29, false, manager);

    // Loop through all grammars in the grammar pool to create the XSNamespaceItem's
    //  which will have access to Annotation Information which can be used later when
    //  we create all the XS components.
    RefHashTableOfEnumerator<Grammar> grammarEnum = grammarPool->getGrammarEnumerator();

    while (grammarEnum.hasMoreElements())
    {
        SchemaGrammar& sGrammar = (SchemaGrammar&) grammarEnum.nextElement();
        if (sGrammar.getGrammarType() != Grammar::SchemaGrammarType)
            continue;
        
        // NOTE: In the grammarpool, preprocessed grammars without targetnamespace
        //       will use an empty string...
        XMLCh* NameSpace = XMLString::replicate(sGrammar.getTargetNamespace(), manager);
        fNamespaceStringList->addElement(NameSpace);
        
        XSNamespaceItem* namespaceItem = new (manager) XSNamespaceItem(this, &sGrammar, manager);
        fXSNamespaceItemList->addElement(namespaceItem);

        fHashNamespace->put(NameSpace, namespaceItem);
    }
    
    // Now loop through all of the NamespaceItem's
    unsigned int numberOfNamespaces = fXSNamespaceItemList->size();
    for (unsigned int j=0; j<numberOfNamespaces; j++)
    {       
        addGrammarToXSModel(fXSNamespaceItemList->elementAt(j));
    } // end of namespaceItem loop
}

/**
  * The constructor to be used when the XSModel must represent all
  * components in the union of an existing XSModel and a newly-created
  * Grammar(s) from the GrammarResolver
  *
  * @param baseModel  the XSModel upon which this one is based
  * @param grammarResolver  the grammar(s) whose components are to be merged
  * @param manager     The configurable memory manager
  */
XSModel::XSModel( XSModel *baseModel
                , GrammarResolver *grammarResolver
                , MemoryManager* const manager)
    : fMemoryManager(manager)
    , fNamespaceStringList(0)
    , fXSNamespaceItemList(0)
    , fElementDeclarationVector(0)
    , fAttributeDeclarationVector(0)
    , fURIStringPool(0)
    , fXSAnnotationList(0)
    , fHashNamespace(0)
    , fParent(baseModel)
    , fObjFactory(0)
    , fDeleteNamespace(0)
    , fDeleteParent(true)
    , fAddedBuiltinDatatypeValidators(false)
{
    fURIStringPool = grammarResolver->getStringPool();        
    fObjFactory = new (manager) XSObjectFactory(manager);

    unsigned int i;
    // Populate XSNamedMaps by going through the components
    for (i=0; i<XSConstants::MULTIVALUE_FACET; i++)
    {
        // REVISIT: what size & modulus
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
                    29,     // size
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
    }
   
    // Revisit: size of vector
    fNamespaceStringList        = new (manager) RefArrayVectorOf <XMLCh>(10, true, manager);
    fXSNamespaceItemList        = new (manager) RefVectorOf <XSNamespaceItem>(10, false, manager);
    fDeleteNamespace            = new (manager) RefVectorOf <XSNamespaceItem>(10, true, manager);
    fXSAnnotationList           = new (manager) RefVectorOf <XSAnnotation> (10, false, manager);
    fElementDeclarationVector   = new (manager) RefVectorOf<XSElementDeclaration> (10, false, manager);
    fAttributeDeclarationVector = new (manager) RefVectorOf<XSAttributeDeclaration>  (10, false, manager);
    fHashNamespace              = new (manager) RefHashTableOf<XSNamespaceItem> (29, false, manager);

    if (fParent)
    {
        if (fParent->fAddedBuiltinDatatypeValidators)
            fAddedBuiltinDatatypeValidators = true;

        // Need to copy information from parent so it can be returned in this object...
        for (i=0; i<fParent->fXSNamespaceItemList->size(); i++)
        {
            XSNamespaceItem* namespaceItem = fXSNamespaceItemList->elementAt(i);
            fXSNamespaceItemList->addElement(namespaceItem);

            XMLCh* NameSpace = XMLString::replicate(namespaceItem->getSchemaNamespace(), manager);
            fNamespaceStringList->addElement(NameSpace);
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
        }

        for (i=0; i<fParent->fElementDeclarationVector->size(); i++)
        {
            fElementDeclarationVector->addElement(fParent->fElementDeclarationVector->elementAt(i));
        }
        for (i=0; i<fParent->fAttributeDeclarationVector->size(); i++)
        {
            fAttributeDeclarationVector->addElement(fParent->fAttributeDeclarationVector->elementAt(i));
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
    unsigned int numberOfNamespacesToAdd = grammarsToAdd->size();
    for (i=0; i < numberOfNamespacesToAdd; i++)
    {
        XMLCh* NameSpace = XMLString::replicate(grammarsToAdd->elementAt(i)->getTargetNamespace(), manager);
        fNamespaceStringList->addElement(NameSpace);

        XSNamespaceItem* namespaceItem = new (manager) XSNamespaceItem(this, grammarsToAdd->elementAt(i), manager);
        fXSNamespaceItemList->addElement(namespaceItem);
        
        fHashNamespace->put(NameSpace, namespaceItem);

        fDeleteNamespace->addElement(namespaceItem);       
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
    }
   
    delete fNamespaceStringList; 
    delete fXSNamespaceItemList;
    delete fXSAnnotationList;
    delete fHashNamespace;
    delete fObjFactory;
    delete fElementDeclarationVector;
    delete fAttributeDeclarationVector;

    if (fDeleteNamespace)
        delete fDeleteNamespace;

    if (fDeleteParent && fParent && fParent->fDeleteParent)
        delete fParent;
}


// XSModel methods


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
        namespaceItem = getNamespaceItem(emptyString);
    
    if (namespaceItem)
        namespaceItem->getComponents(objectType);

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
        namespaceItem = getNamespaceItem(emptyString);
    
    if (namespaceItem)
        namespaceItem->getElementDeclaration(name);

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
        namespaceItem = getNamespaceItem(emptyString);    
    
    if (namespaceItem)
        namespaceItem->getAttributeDeclaration(name);

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
        namespaceItem = getNamespaceItem(emptyString);
    
    if (namespaceItem)
        namespaceItem->getTypeDefinition(name);

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
        namespaceItem = getNamespaceItem(emptyString);
   
    if (namespaceItem)
        namespaceItem->getAttributeGroup(name);

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
        namespaceItem = getNamespaceItem(emptyString);
   
    if (namespaceItem)
        namespaceItem->getModelGroupDefinition(name);

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
        namespaceItem = getNamespaceItem(emptyString);
   
    if (namespaceItem)
        namespaceItem->getNotationDeclaration(name);

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
    // only elements & attributes have id's
    switch(compType) {
        case XSConstants::ELEMENT_DECLARATION:
            if (compId < fElementDeclarationVector->size())
                return fElementDeclarationVector->elementAt(compId);
            break;    
        case XSConstants::ATTRIBUTE_DECLARATION:
            if (compId < fAttributeDeclarationVector->size())
                return fAttributeDeclarationVector->elementAt(compId);
            break;
    }
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


XERCES_CPP_NAMESPACE_END


