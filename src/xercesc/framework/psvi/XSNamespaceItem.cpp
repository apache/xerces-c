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
 * Revision 1.5  2003/11/21 17:34:04  knoaman
 * PSVI update
 *
 * Revision 1.4  2003/11/14 22:47:53  neilg
 * fix bogus log message from previous commit...
 *
 * Revision 1.3  2003/11/14 22:33:30  neilg
 * Second phase of schema component model implementation.  
 * Implement XSModel, XSNamespaceItem, and the plumbing necessary
 * to connect them to the other components.
 * Thanks to David Cargill.
 *
 * Revision 1.2  2003/11/06 15:30:04  neilg
 * first part of PSVI/schema component model implementation, thanks to David Cargill.  This covers setting the PSVIHandler on parser objects, as well as implementing XSNotation, XSSimpleTypeDefinition, XSIDCDefinition, and most of XSWildcard, XSComplexTypeDefinition, XSElementDeclaration, XSAttributeDeclaration and XSAttributeUse.
 *
 * Revision 1.1  2003/09/16 14:33:36  neilg
 * PSVI/schema component model classes, with Makefile/configuration changes necessary to build them
 *
 */

#include <xercesc/framework/psvi/XSNamespaceItem.hpp>
#include <xercesc/validators/schema/SchemaGrammar.hpp>
#include <xercesc/framework/psvi/XSModel.hpp>
#include <xercesc/framework/psvi/XSAnnotation.hpp>
#include <xercesc/validators/schema/XMLSchemaDescriptionImpl.hpp>

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  XSNamespaceItem: Constructors and Destructors
// ---------------------------------------------------------------------------
XSNamespaceItem::XSNamespaceItem(XSModel* const       xsModel,
                                 SchemaGrammar* const grammar,
                                 MemoryManager* const manager)
    : fMemoryManager(manager)
    , fGrammar(grammar)
    , fXSModel(xsModel)
    , fXSAnnotationList(0)

{
    // Populate XSNamedMaps by going through the components
    for (unsigned int i=0; i<XSConstants::MULTIVALUE_FACET; i++)
    {
        // REVISIT: what size & modulus & adopt
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
                    15,     // size
                    29,     // modulus
                    fXSModel->getURIStringPool(),
                    false,  // adoptElems 
                    fMemoryManager
                );               
                fHashMap[i] = new (fMemoryManager) RefHashTableOf<XSObject>
                (
                    29,
                    false,
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
                fHashMap[i] = 0;
                break;
        }
    }
    
    // Revisit size
    fXSAnnotationList = new (manager) RefVectorOf <XSAnnotation> (10, false, manager);
}

XSNamespaceItem::~XSNamespaceItem()
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
                delete fHashMap[i];             
                break;
        }
    }
    
    delete fXSAnnotationList;
}

// ---------------------------------------------------------------------------
//  XSNamespaceItem: access methods
// ---------------------------------------------------------------------------
const XMLCh *XSNamespaceItem::getSchemaNamespace()
{
    return fGrammar->getTargetNamespace();
}

XSNamedMap<XSObject> *XSNamespaceItem::getComponents(XSConstants::COMPONENT_TYPE objectType)
{
    return fComponentMap[objectType -1];
}

XSElementDeclaration *XSNamespaceItem::getElementDeclaration(const XMLCh *name)
{
    return (XSElementDeclaration*) fHashMap[XSConstants::ELEMENT_DECLARATION -1]->get(name);
}

XSAttributeDeclaration *XSNamespaceItem::getAttributeDeclaration(const XMLCh *name)
{
    return (XSAttributeDeclaration*) fHashMap[XSConstants::ATTRIBUTE_DECLARATION -1]->get(name);
}

XSTypeDefinition *XSNamespaceItem::getTypeDefinition(const XMLCh *name)
{
    return (XSTypeDefinition*) fHashMap[XSConstants::TYPE_DEFINITION -1]->get(name);
}

XSAttributeGroupDefinition *XSNamespaceItem::getAttributeGroup(const XMLCh *name)
{
    return (XSAttributeGroupDefinition*) fHashMap[XSConstants::ATTRIBUTE_GROUP_DEFINITION -1]->get(name);
}

XSModelGroupDefinition *XSNamespaceItem::getModelGroupDefinition(const XMLCh *name)
{
    return (XSModelGroupDefinition*) fHashMap[XSConstants::MODEL_GROUP_DEFINITION -1]->get(name);         
}

XSNotationDeclaration *XSNamespaceItem::getNotationDeclaration(const XMLCh *name)
{
    return (XSNotationDeclaration*) fHashMap[XSConstants::NOTATION_DECLARATION -1]->get(name);
}

StringList *XSNamespaceItem::getDocumentLocations()
{
    return ((XMLSchemaDescriptionImpl*) fGrammar->getGrammarDescription())->getLocationHints();
}

XERCES_CPP_NAMESPACE_END


