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
    , MemoryManager* const               manager
)
    : XSObject(XSConstants::ELEMENT_DECLARATION, xsModel, manager)
    , fDisallowedSubstitutions(0)
    , fSubstitutionGroupExclusions(0)
    , fId(0)
    , fSchemaElementDecl(schemaElementDecl)
    , fTypeDefinition(typeDefinition)
    , fSubstitutionGroupAffiliation(substitutionGroupAffiliation)
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
    
    if (blockFinalSet = fSchemaElementDecl->getFinalSet())
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

unsigned int XSElementDeclaration::getId() const
{
    return fId;
}


// ---------------------------------------------------------------------------
//  XSElementDeclaration: access methods
// ---------------------------------------------------------------------------
XSConstants::SCOPE XSElementDeclaration::getScope() const
{
    if (fSchemaElementDecl->getEnclosingScope() == Grammar::TOP_LEVEL_SCOPE)
        return XSConstants::SCOPE_GLOBAL;
    else
        return XSConstants::SCOPE_LOCAL;
}

XSComplexTypeDefinition *XSElementDeclaration::getEnclosingCTDefinition() const
{
    // REVISIT
    return 0;
}

XSConstants::VALUE_CONSTRAINT XSElementDeclaration::getConstraintType() const
{
    if (fSchemaElementDecl->getMiscFlags() & SchemaSymbols::XSD_FIXED)
        return XSConstants::VC_FIXED;

    // REVISIT: need to verify this... don't appear to set a DEFAULT flag so thought
    // this might work...
    if (fSchemaElementDecl->getDefaultValue())
        return XSConstants::VC_DEFAULT;

    return XSConstants::VC_NONE;
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
