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
 * Revision 1.4  2003/11/14 22:33:30  neilg
 * ./src/xercesc/framework/psvi/XSAnnotation.cpp
 * ./src/xercesc/framework/psvi/XSAnnotation.hpp
 * ./src/xercesc/framework/psvi/XSAttributeDeclaration.cpp
 * ./src/xercesc/framework/psvi/XSAttributeDeclaration.hpp
 * ./src/xercesc/framework/psvi/XSAttributeGroupDefinition.cpp
 * ./src/xercesc/framework/psvi/XSAttributeGroupDefinition.hpp
 * ./src/xercesc/framework/psvi/XSAttributeUse.cpp
 * ./src/xercesc/framework/psvi/XSAttributeUse.hpp
 * ./src/xercesc/framework/psvi/XSComplexTypeDefinition.cpp
 * ./src/xercesc/framework/psvi/XSComplexTypeDefinition.hpp
 * ./src/xercesc/framework/psvi/XSElementDeclaration.cpp
 * ./src/xercesc/framework/psvi/XSElementDeclaration.hpp
 * ./src/xercesc/framework/psvi/XSFacet.cpp
 * ./src/xercesc/framework/psvi/XSFacet.hpp
 * ./src/xercesc/framework/psvi/XSIDCDefinition.cpp
 * ./src/xercesc/framework/psvi/XSIDCDefinition.hpp
 * ./src/xercesc/framework/psvi/XSModel.cpp
 * ./src/xercesc/framework/psvi/XSModel.hpp
 * ./src/xercesc/framework/psvi/XSModelGroup.cpp
 * ./src/xercesc/framework/psvi/XSModelGroup.hpp
 * ./src/xercesc/framework/psvi/XSModelGroupDefinition.cpp
 * ./src/xercesc/framework/psvi/XSModelGroupDefinition.hpp
 * ./src/xercesc/framework/psvi/XSMultiValueFacet.cpp
 * ./src/xercesc/framework/psvi/XSMultiValueFacet.hpp
 * ./src/xercesc/framework/psvi/XSNamespaceItem.cpp
 * ./src/xercesc/framework/psvi/XSNamespaceItem.hpp
 * ./src/xercesc/framework/psvi/XSNotationDeclaration.cpp
 * ./src/xercesc/framework/psvi/XSNotationDeclaration.hpp
 * ./src/xercesc/framework/psvi/XSObject.cpp
 * ./src/xercesc/framework/psvi/XSObject.hpp
 * ./src/xercesc/framework/psvi/XSParticle.cpp
 * ./src/xercesc/framework/psvi/XSParticle.hpp
 * ./src/xercesc/framework/psvi/XSSimpleTypeDefinition.cpp
 * ./src/xercesc/framework/psvi/XSSimpleTypeDefinition.hpp
 * ./src/xercesc/framework/psvi/XSTypeDefinition.cpp
 * ./src/xercesc/framework/psvi/XSTypeDefinition.hpp
 * ./src/xercesc/framework/psvi/XSWildcard.cpp
 * ./src/xercesc/framework/psvi/XSWildcard.hpp
 * ./src/xercesc/internal/XMLGrammarPoolImpl.cpp
 * ./src/xercesc/internal/XMLGrammarPoolImpl.hpp
 * ./src/xercesc/validators/schema/identity/IdentityConstraint.cpp
 * ./src/xercesc/validators/schema/identity/IdentityConstraint.hpp
 * ./src/xercesc/validators/schema/SchemaGrammar.hpp
 * ./src/xercesc/validators/schema/TraverseSchema.cpp
 *
 * Revision 1.3  2003/11/06 15:30:04  neilg
 * first part of PSVI/schema component model implementation, thanks to David Cargill.  This covers setting the PSVIHandler on parser objects, as well as implementing XSNotation, XSSimpleTypeDefinition, XSIDCDefinition, and most of XSWildcard, XSComplexTypeDefinition, XSElementDeclaration, XSAttributeDeclaration and XSAttributeUse.
 *
 * Revision 1.2  2003/09/17 17:45:37  neilg
 * remove spurious inlines; hopefully this will make Solaris/AIX compilers happy.
 *
 * Revision 1.1  2003/09/16 14:33:36  neilg
 * PSVI/schema component model classes, with Makefile/configuration changes necessary to build them
 *
 */

#include <xercesc/framework/psvi/XSElementDeclaration.hpp>
#include <xercesc/validators/schema/SchemaElementDecl.hpp>
#include <xercesc/framework/psvi/XSSimpleTypeDefinition.hpp>
#include <xercesc/framework/psvi/XSComplexTypeDefinition.hpp>
#include <xercesc/framework/psvi/XSIDCDefinition.hpp>
#include <xercesc/util/StringPool.hpp>
#include <xercesc/framework/psvi/XSModel.hpp>
#include <xercesc/framework/psvi/XSAnnotation.hpp>

XERCES_CPP_NAMESPACE_BEGIN
   
XSElementDeclaration::XSElementDeclaration(SchemaElementDecl*       schemaElementDecl,
                                           XSModel*                 xsModel,
                                           MemoryManager * const    manager):
    fSchemaElementDecl(schemaElementDecl),
    fSubstitutionGroupAffiliation(0),
    fDisallowedSubstitutions(0),
    fSubstitutionGroupExclusions(0),
    fTypeDefinition(0),
    fIdentityConstraints(0),
    XSObject(XSConstants::ELEMENT_DECLARATION, xsModel, manager )
{
    int blockorfinalset;
    if (blockorfinalset = fSchemaElementDecl->getBlockSet()) 
    {
        if (blockorfinalset & SchemaSymbols::XSD_EXTENSION)
        {
            fDisallowedSubstitutions |= XSConstants::DERIVATION_EXTENSION;
        }
        if (blockorfinalset & SchemaSymbols::XSD_RESTRICTION)
        {
            fDisallowedSubstitutions |= XSConstants::DERIVATION_RESTRICTION;
        }
        if (blockorfinalset & SchemaSymbols::XSD_SUBSTITUTION)
        {
            fDisallowedSubstitutions |= XSConstants::DERIVATION_SUBSTITUTION;
        }
    }
    
    if (blockorfinalset = fSchemaElementDecl->getFinalSet()) 
    {
        if (blockorfinalset & SchemaSymbols::XSD_EXTENSION)
        {
            fSubstitutionGroupExclusions |= XSConstants::DERIVATION_EXTENSION;
        }
        if (blockorfinalset & SchemaSymbols::XSD_RESTRICTION)
        {
            fSubstitutionGroupExclusions |= XSConstants::DERIVATION_RESTRICTION;
        }
    }
    
    if (fSchemaElementDecl->getSubstitutionGroupElem())
    {
        fSubstitutionGroupAffiliation = (XSElementDeclaration*) getObjectFromMap((void*)fSchemaElementDecl->getSubstitutionGroupElem());
        if (!fSubstitutionGroupAffiliation)
        {
            fSubstitutionGroupAffiliation = new (manager) XSElementDeclaration(fSchemaElementDecl->getSubstitutionGroupElem(), fXSModel, manager);
            putObjectInMap((void*)fSchemaElementDecl->getSubstitutionGroupElem(),fSubstitutionGroupAffiliation);
        }
    }

    if (fSchemaElementDecl->getDatatypeValidator())
    {
        // simple type
        fTypeDefinition = (XSTypeDefinition*) getObjectFromMap((void *)fSchemaElementDecl->getDatatypeValidator());
        if (!fTypeDefinition)
        {
            fTypeDefinition = (XSTypeDefinition*) new (manager) XSSimpleTypeDefinition(fSchemaElementDecl->getDatatypeValidator(), fXSModel, manager);
            putObjectInMap((void *)fSchemaElementDecl->getDatatypeValidator(), fTypeDefinition);
        }
    }
    else if (fSchemaElementDecl->getComplexTypeInfo()) 
    {
        // complex type
        fTypeDefinition = (XSTypeDefinition*) getObjectFromMap((void *)fSchemaElementDecl->getComplexTypeInfo());
        if (!fTypeDefinition)
        {
            fTypeDefinition = (XSTypeDefinition*) new (manager) XSComplexTypeDefinition(fSchemaElementDecl->getComplexTypeInfo(), fXSModel, manager);
            putObjectInMap((void *)fSchemaElementDecl->getComplexTypeInfo(), fTypeDefinition);
        }
    }

    unsigned int count = fSchemaElementDecl->getIdentityConstraintCount();
    if (count)
    {
        //REVISIT: size of hash table....   
        fIdentityConstraints = new (manager) XSNamedMap <XSIDCDefinition> (count, 29, fXSModel->getURIStringPool(), false, manager);
        for (unsigned int i = 0; i < count; i++) 
        {
            XSIDCDefinition*    definition = (XSIDCDefinition*) getObjectFromMap((void*)fSchemaElementDecl->getIdentityConstraintAt(i));
            if (!definition)
            {
                definition = new (manager) XSIDCDefinition(fSchemaElementDecl->getIdentityConstraintAt(i), fXSModel, manager);
                putObjectInMap((void*) fSchemaElementDecl->getIdentityConstraintAt(i), definition);
            }
            fIdentityConstraints->addElement(definition, definition->getName(), definition->getNamespace());
        }
    }
}

XSElementDeclaration::~XSElementDeclaration() 
{
    // don't delete fTypeDefinition - deleted by XSModel
    // don't delete fSubstitutionGroupAffiliation - deleted by XSModel
    if (fIdentityConstraints)
    {
        delete fIdentityConstraints;
    }
}

// Overridden XSObject methods
const XMLCh *XSElementDeclaration::getName() 
{
    return fSchemaElementDecl->getElementName()->getLocalPart();
}

const XMLCh *XSElementDeclaration::getNamespace() 
{
    return fXSModel->getURIStringPool()->getValueForId(fSchemaElementDecl->getURI());
}

XSNamespaceItem *XSElementDeclaration::getNamespaceItem() 
{
    return getNamespaceItemFromHash(getNamespace());
}

// XSElementDeclaration methods


/**
 * [type definition]: either a simple type definition or a complex type 
 * definition. 
 */
XSTypeDefinition *XSElementDeclaration::getTypeDefinition()
{
    return fTypeDefinition;
}

/**
 * Optional. One of <code>SCOPE_GLOBAL</code>, <code>SCOPE_LOCAL</code>, 
 * or <code>SCOPE_ABSENT</code>. If the scope is local, then the 
 * <code>enclosingCTDefinition</code> is present. 
 */
XSConstants::SCOPE XSElementDeclaration::getScope() 
{
    if (fSchemaElementDecl->getEnclosingScope() == Grammar::TOP_LEVEL_SCOPE)
        return XSConstants::SCOPE_GLOBAL;
    else
        return XSConstants::SCOPE_LOCAL;
}

/**
 * The complex type definition for locally scoped declarations (see 
 * <code>scope</code>). 
 */
XSComplexTypeDefinition *XSElementDeclaration::getEnclosingCTDefinition()
{
    // REVISIT
    return 0;
}

/**
 * [Value constraint]: one of <code>VC_NONE, VC_DEFAULT, VC_FIXED</code>. 
 */
XSConstants::VALUE_CONSTRAINT XSElementDeclaration::getConstraintType()
{
    if (fSchemaElementDecl->getMiscFlags() & SchemaSymbols::XSD_FIXED)
    {
        return XSConstants::VC_FIXED;
    }
    // REVISIT: need to verify this... don't appear to set a DEFAULT flag so thought
    // this might work...
    if (fSchemaElementDecl->getDefaultValue())
    {
        return XSConstants::VC_DEFAULT;
    }
    return XSConstants::VC_NONE;
}

/**
 * [Value constraint]: the actual value with respect to the [type 
 * definition]. 
 */
const XMLCh *XSElementDeclaration::getConstraintValue()
{
    return fSchemaElementDecl->getDefaultValue();
}

/**
 * If nillable is true, then an element may also be valid if it carries 
 * the namespace qualified attribute with local name <code>nil</code> 
 * from namespace <code>http://www.w3.org/2001/XMLSchema-instance</code> 
 * and value <code>true</code> (xsi:nil) even if it has no text or 
 * element content despite a <code>content type</code> which would 
 * otherwise require content. 
 */
bool XSElementDeclaration::getNillable()
{
    if (fSchemaElementDecl->getMiscFlags() & SchemaSymbols::XSD_NILLABLE) {
        return true;
    }
    return false;
}

/**
 * identity-constraint definitions: a set of constraint definitions. 
 */
XSNamedMap <XSIDCDefinition> *XSElementDeclaration::getIdentityConstraints()
{
    return fIdentityConstraints;
}

/**
 * [substitution group affiliation]: optional. A top-level element 
 * definition. 
 */
XSElementDeclaration *XSElementDeclaration::getSubstitutionGroupAffiliation()
{
    return fSubstitutionGroupAffiliation;
}

/**
 * Convenience method. Check if <code>exclusion</code> is a substitution 
 * group exclusion for this element declaration. 
 * @param exclusion  
 *   <code>DERIVATION_EXTENSION, DERIVATION_RESTRICTION</code> or 
 *   <code>DERIVATION_NONE</code>. Represents final set for the element.
 * @return True if <code>exclusion</code> is a part of the substitution 
 *   group exclusion subset. 
 */
bool XSElementDeclaration::isSubstitutionGroupExclusion(XSConstants::DERIVATION_TYPE exclusion)
{
    if (fSubstitutionGroupExclusions & exclusion)
    {
        return true;
    }
    return false;
}

/**
 *  [substitution group exclusions]: the returned value is a bit 
 * combination of the subset of {
 * <code>DERIVATION_EXTENSION, DERIVATION_RESTRICTION</code>} or 
 * <code>DERIVATION_NONE</code>. 
 */
short XSElementDeclaration::getSubstitutionGroupExclusions()
{
    return fSubstitutionGroupExclusions;
}

/**
 * Convenience method. Check if <code>disallowed</code> is a disallowed 
 * substitution for this element declaration. 
 * @param disallowed {
 *   <code>DERIVATION_SUBSTITUTION, DERIVATION_EXTENSION, DERIVATION_RESTRICTION</code>
 *   } or <code>DERIVATION_NONE</code>. Represents a block set for the 
 *   element.
 * @return True if <code>disallowed</code> is a part of the substitution 
 *   group exclusion subset. 
 */
bool XSElementDeclaration::isDisallowedSubstitution(XSConstants::DERIVATION_TYPE disallowed)
{
    if (fDisallowedSubstitutions & disallowed)
    {
        return true;
    }
    return false;
}

/**
 *  [disallowed substitutions]: the returned value is a bit combination of 
 * the subset of {
 * <code>DERIVATION_SUBSTITUTION, DERIVATION_EXTENSION, DERIVATION_RESTRICTION</code>
 * } corresponding to substitutions disallowed by this 
 * <code>XSElementDeclaration</code> or <code>DERIVATION_NONE</code>. 
 */
short XSElementDeclaration::getDisallowedSubstitutions()
{
    return fDisallowedSubstitutions;
}

/**
 * {abstract} A boolean. 
 */
bool XSElementDeclaration::getAbstract()
{
    if (fSchemaElementDecl->getMiscFlags() & SchemaSymbols::XSD_ABSTRACT) {
        return true;
    }
    return false;
}

/**
 * Optional. Annotation. 
 */
XSAnnotation *XSElementDeclaration::getAnnotation()
{
    return getAnnotationFromModel(fSchemaElementDecl);
}

/**
 * Process Id
 */ 
void XSElementDeclaration::setId(unsigned int id)
{
    fId = id;
}

unsigned int XSElementDeclaration::getId() const
{
    return fId;
}

XERCES_CPP_NAMESPACE_END
