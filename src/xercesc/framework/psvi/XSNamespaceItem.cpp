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
 * Revision 1.2  2003/11/06 15:30:04  neilg
 * first part of PSVI/schema component model implementation, thanks to David Cargill.  This covers setting the PSVIHandler on parser objects, as well as implementing XSNotation, XSSimpleTypeDefinition, XSIDCDefinition, and most of XSWildcard, XSComplexTypeDefinition, XSElementDeclaration, XSAttributeDeclaration and XSAttributeUse.
 *
 * Revision 1.1  2003/09/16 14:33:36  neilg
 * PSVI/schema component model classes, with Makefile/configuration changes necessary to build them
 *
 */

#include <xercesc/framework/psvi/XSNamespaceItem.hpp>
#include <xercesc/validators/schema/SchemaGrammar.hpp>

XERCES_CPP_NAMESPACE_BEGIN

XSNamespaceItem::XSNamespaceItem( MemoryManager* const manager ):  
        fMemoryManager(manager)
{
}

// XSNamespaceItem methods

/**
 * [schema namespace]: A namespace name or <code>null</code>
 * corresponding to the target namespace of the schema document.
 */
const XMLCh *XSNamespaceItem::getSchemaNamespace()
{
    return fGrammar->getTargetNamespace();
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
XSNamedMap <XSObject *> *XSNamespaceItem::getComponents(XSConstants::COMPONENT_TYPE objectType)
{
    // REVISIT
    return 0;
}

/**
 *  [annotations]: a set of annotations.
 */
XSAnnotationList *XSNamespaceItem::getAnnotations()
{
    // REVISIT
    return 0;
}

/**
 * Convenience method. Returns a top-level element declaration. 
 * @param name The name of the declaration.
 * @return A top-level element declaration or <code>null</code> if such 
 *   declaration does not exist. 
 */
XSElementDeclaration *XSNamespaceItem::getElementDeclaration(const XMLCh *name)
{
    // REVISIT
    return 0;
}

/**
 * Convenience method. Returns a top-level attribute declaration. 
 * @param name The name of the declaration.
 * @return A top-level attribute declaration or <code>null</code> if such 
 *   declaration does not exist. 
 */
XSAttributeDeclaration *XSNamespaceItem::getAttributeDeclaration(const XMLCh *name)
{
    // REVISIT
    return 0;
}

/**
 * Convenience method. Returns a top-level simple or complex type 
 * definition. 
 * @param name The name of the definition.
 * @return An <code>XSTypeDefinition</code> or <code>null</code> if such 
 *   definition does not exist. 
 */
XSTypeDefinition *XSNamespaceItem::getTypeDefinition(const XMLCh *name)
{
    // REVISIT
    return 0;
}

/**
 * Convenience method. Returns a top-level attribute group definition. 
 * @param name The name of the definition.
 * @return A top-level attribute group definition or <code>null</code> if 
 *   such definition does not exist. 
 */
XSAttributeGroupDefinition *XSNamespaceItem::getAttributeGroup(const XMLCh *name)
{
    // REVISIT
    return 0;
}

/**
 * Convenience method. Returns a top-level model group definition. 
 * @param name The name of the definition.
 * @return A top-level model group definition definition or 
 *   <code>null</code> if such definition does not exist. 
 */
XSModelGroupDefinition *XSNamespaceItem::getModelGroupDefinition(const XMLCh *name)
{
    // REVISIT
    return 0;
}

/**
 * Convenience method. Returns a top-level notation declaration. 
 * @param name The name of the declaration.
 * @return A top-level notation declaration or <code>null</code> if such 
 *   declaration does not exist. 
 */
XSNotationDeclaration *XSNamespaceItem::getNotationDeclaration(const XMLCh *name)
{
    // REVISIT
    return 0;
}

/**
 * [document location] - a list of locations URI for the documents that 
 * contributed to the XSModel.
 */
StringList *XSNamespaceItem::getDocumentLocations()
{
    // REVISIT
    return 0;
}

XERCES_CPP_NAMESPACE_END


