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

XERCES_CPP_NAMESPACE_BEGIN

/**
  * The constructor to be used wen a grammar pool contains all needed info
  *
  * @param grammarPool  the grammar pool containing the underlying data structures
  * @param  manager     The configurable memory manager
  */
XSModel::XSModel( XMLGrammarPool *grammarPool
            , MemoryManager* const manager ):
        fMemoryManager(manager)
{
    RefHashTableOfEnumerator<Grammar> grammarEnum = grammarPool->getGrammarEnumerator();
    while (grammarEnum.hasMoreElements())
    {
        SchemaGrammar& sGrammar = (SchemaGrammar&) grammarEnum.nextElement();
        if (sGrammar.getGrammarType() != Grammar::SchemaGrammarType)
            continue;
        if (!fNamespaceStringList) {
            // Revisit: size of vector
            fNamespaceStringList = new (manager) RefArrayVectorOf <XMLCh>(10, true, manager);
        }
        XMLCh* NameSpace = XMLString::replicate(sGrammar.getTargetNamespace(), manager);
        fNamespaceStringList->addElement(NameSpace);
        if (!fXSNamespaceItemList)
        {
            // Revisit: size of vector
            fXSNamespaceItemList = new (manager) RefVectorOf <XSNamespaceItem>(10, true, manager);
        }
        XSNamespaceItem* NamespaceItem = new XSNamespaceItem(manager);
        fXSNamespaceItemList->addElement(NamespaceItem);
    }
}

/**
  * The constructor to be used when the XSModel must represent all
  * components in the union of an existing XSModel and a newly-created
  * Grammar
  *
  * @param baseModel  the XSModel upon which this one is based
  * @param  grammar  the newly-created grammar whose components are to be merged
  * @param  manager     The configurable memory manager
  */
XSModel::XSModel( XSModel *baseModel
            , Grammar *grammar
            , MemoryManager* const manager ):
        fMemoryManager(manager)
{
}

// XSModel methods

/**
 * Convenience method. Returns a list of all namespaces that belong to 
 * this schema. The value <code>null</code> is not a valid namespace 
 * name, but if there are components that don't have a target namespace, 
 * <code>null</code> is included in this list. 
 */
StringList *XSModel::getNamespaces()
{
    return fNamespaceStringList;
}

/**
 * A set of namespace schema information information items ( of type 
 * <code>XSNamespaceItem</code>), one for each namespace name which 
 * appears as the target namespace of any schema component in the schema 
 * used for that assessment, and one for absent if any schema component 
 * in the schema had no target namespace. For more information see 
 * schema information. 
 */
XSNamespaceItemList *XSModel::getNamespaceItems()
{
    return fXSNamespaceItemList;
}

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
XSNamedMap <XSObject *> *XSModel::getComponents(XSConstants::COMPONENT_TYPE objectType)
{
    // REVISIT
    return 0;
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
XSNamedMap <XSObject *> *XSModel::getComponentsByNamespace(XSConstants::COMPONENT_TYPE objectType, 
                                               const XMLCh *compNamespace)
{
    // REVISIT
    return 0;
}

/**
 *  [annotations]: a set of annotations.
 */
XSAnnotationList *XSModel::getAnnotations()
{
    // REVISIT
    return 0;
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
    // REVISIT
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
    // REVISIT
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
    // REVISIT
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
    // REVISIT
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
    // REVISIT
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
    // REVISIT
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
    // Revisit: only elements & attributes have id's
    switch(compType) {
        case XSConstants::ELEMENT_DECLARATION:
            return 0;
        case XSConstants::ATTRIBUTE_DECLARATION:
            return 0;
        default:
            return 0;
    }
}

XERCES_CPP_NAMESPACE_END


