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
 * Revision 1.13  2004/09/08 13:56:08  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.12  2004/07/06 14:58:15  cargilld
 * Rename VALUE_CONSTRAINT enumeration names to avoid naming conflict with AIX system header which already uses VC_DEFAULT as a macro.  Will need to document that this fix breaks source code compatibility.
 *
 * Revision 1.11  2004/05/04 19:02:40  cargilld
 * Enable IDs to work on all kinds of schema components
 *
 * Revision 1.10  2003/12/15 17:23:48  cargilld
 * psvi updates; cleanup revisits and bug fixes
 *
 * Revision 1.9  2003/12/02 01:27:01  knoaman
 * Properly set fAnnotation data member
 *
 * Revision 1.8  2003/11/25 20:37:40  jberry
 * Cleanup build errors/warnings from CodeWarrior
 *
 * Revision 1.7  2003/11/23 16:20:16  knoaman
 * PSVI: pass scope and enclosing type during construction.
 *
 * Revision 1.6  2003/11/21 17:29:53  knoaman
 * PSVI update
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
 * Revision 1.2  2003/09/17 17:45:37  neilg
 * remove spurious inlines; hopefully this will make Solaris/AIX compilers happy.
 *
 * Revision 1.1  2003/09/16 14:33:36  neilg
 * PSVI/schema component model classes, with Makefile/configuration changes necessary to build them
 *
 */

#include <xercesc/framework/psvi/XSElementDeclaration.hpp>
#include <xercesc/framework/psvi/XSSimpleTypeDefinition.hpp>
#include <xercesc/framework/psvi/XSComplexTypeDefinition.hpp>
#include <xercesc/framework/psvi/XSIDCDefinition.hpp>
#include <xercesc/framework/psvi/XSModel.hpp>
#include <xercesc/validators/schema/SchemaElementDecl.hpp>
#include <xercesc/util/StringPool.hpp>

XERCES_CPP_NAMESPACE_BEGIN
   
// ---------------------------------------------------------------------------
//  XSElementDeclaration: Constructors and Destructor
// ---------------------------------------------------------------------------
XSElementDeclaration::XSElementDeclaration
(
    SchemaElementDecl* const             schemaElementDecl
    , XSTypeDefinition* const            typeDefinition
    , XSElementDeclaration* const        substitutionGroupAffiliation
    , XSAnnotation* const                annot
    , XSNamedMap<XSIDCDefinition>* const identityConstraints
    , XSModel* const                     xsModel
    , XSConstants::SCOPE                 elemScope
    , XSComplexTypeDefinition* const     enclosingTypeDefinition
    , MemoryManager* const               manager
)
    : XSObject(XSConstants::ELEMENT_DECLARATION, xsModel, manager)
    , fDisallowedSubstitutions(0)
    , fSubstitutionGroupExclusions(0)
    , fScope(elemScope)
    , fSchemaElementDecl(schemaElementDecl)
    , fTypeDefinition(typeDefinition)
    , fEnclosingTypeDefinition(enclosingTypeDefinition)
    , fSubstitutionGroupAffiliation(substitutionGroupAffiliation)
    , fAnnotation(annot)
    , fIdentityConstraints(identityConstraints)
{
    // set block and final information
    // NOTE: rest of setup will be taken care of in construct()
    int blockFinalSet = fSchemaElementDecl->getBlockSet();
    if (blockFinalSet) 
    {
        if (blockFinalSet & SchemaSymbols::XSD_EXTENSION)
            fDisallowedSubstitutions |= XSConstants::DERIVATION_EXTENSION;

        if (blockFinalSet & SchemaSymbols::XSD_RESTRICTION)
            fDisallowedSubstitutions |= XSConstants::DERIVATION_RESTRICTION;

        if (blockFinalSet & SchemaSymbols::XSD_SUBSTITUTION)
            fDisallowedSubstitutions |= XSConstants::DERIVATION_SUBSTITUTION;
    }
    
    if (0 != (blockFinalSet = fSchemaElementDecl->getFinalSet()))
    {
        if (blockFinalSet & SchemaSymbols::XSD_EXTENSION)
            fSubstitutionGroupExclusions |= XSConstants::DERIVATION_EXTENSION;

        if (blockFinalSet & SchemaSymbols::XSD_RESTRICTION)
            fSubstitutionGroupExclusions |= XSConstants::DERIVATION_RESTRICTION;
    }
}

XSElementDeclaration::~XSElementDeclaration() 
{
    // don't delete fTypeDefinition - deleted by XSModel
    // don't delete fSubstitutionGroupAffiliation - deleted by XSModel
    if (fIdentityConstraints)
        delete fIdentityConstraints;
}

// ---------------------------------------------------------------------------
//  XSElementDeclaration: XSObject virtual methods
// ---------------------------------------------------------------------------
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
    return fXSModel->getNamespaceItem(getNamespace());
}


// ---------------------------------------------------------------------------
//  XSElementDeclaration: access methods
// ---------------------------------------------------------------------------
XSConstants::VALUE_CONSTRAINT XSElementDeclaration::getConstraintType() const
{
    if (fSchemaElementDecl->getMiscFlags() & SchemaSymbols::XSD_FIXED)
        return XSConstants::VALUE_CONSTRAINT_FIXED;

    if (fSchemaElementDecl->getDefaultValue())
        return XSConstants::VALUE_CONSTRAINT_DEFAULT;

    return XSConstants::VALUE_CONSTRAINT_NONE;
}

const XMLCh *XSElementDeclaration::getConstraintValue()
{
    return fSchemaElementDecl->getDefaultValue();
}

bool XSElementDeclaration::getNillable() const
{
    if (fSchemaElementDecl->getMiscFlags() & SchemaSymbols::XSD_NILLABLE)
        return true;

    return false;
}

bool XSElementDeclaration::isSubstitutionGroupExclusion(XSConstants::DERIVATION_TYPE exclusion)
{
    if (fSubstitutionGroupExclusions & exclusion)
        return true;

    return false;
}


bool XSElementDeclaration::isDisallowedSubstitution(XSConstants::DERIVATION_TYPE disallowed)
{
    if (fDisallowedSubstitutions & disallowed)
        return true;

    return false;
}


bool XSElementDeclaration::getAbstract() const
{
    if (fSchemaElementDecl->getMiscFlags() & SchemaSymbols::XSD_ABSTRACT)
        return true;

    return false;
}

XERCES_CPP_NAMESPACE_END
