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
 * Revision 1.9  2003/12/15 17:23:48  cargilld
 * psvi updates; cleanup revisits and bug fixes
 *
 * Revision 1.8  2003/11/27 16:42:00  neilg
 * fixes for segfaults and infinite loops in schema component model implementation; thanks to David Cargill
 *
 * Revision 1.7  2003/11/25 18:08:31  knoaman
 * Misc. PSVI updates. Thanks to David Cargill.
 *
 * Revision 1.6  2003/11/21 17:19:30  knoaman
 * PSVI update.
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
 * Revision 1.3  2003/11/10 21:56:54  neilg
 * make internal code use the new, stateless, method of traversing attribute lists
 *
 * Revision 1.2  2003/11/06 15:30:04  neilg
 * first part of PSVI/schema component model implementation, thanks to David Cargill.  This covers setting the PSVIHandler on parser objects, as well as implementing XSNotation, XSSimpleTypeDefinition, XSIDCDefinition, and most of XSWildcard, XSComplexTypeDefinition, XSElementDeclaration, XSAttributeDeclaration and XSAttributeUse.
 *
 * Revision 1.1  2003/09/16 14:33:36  neilg
 * PSVI/schema component model classes, with Makefile/configuration changes necessary to build them
 *
 */

#include <xercesc/framework/psvi/XSComplexTypeDefinition.hpp>
#include <xercesc/framework/psvi/XSWildcard.hpp>
#include <xercesc/framework/psvi/XSSimpleTypeDefinition.hpp>
#include <xercesc/framework/psvi/XSAttributeUse.hpp>
#include <xercesc/framework/psvi/XSModel.hpp>
#include <xercesc/framework/psvi/XSAnnotation.hpp>
#include <xercesc/framework/psvi/XSParticle.hpp>
#include <xercesc/validators/schema/ComplexTypeInfo.hpp>
#include <xercesc/validators/schema/SchemaElementDecl.hpp>
#include <xercesc/validators/schema/SchemaAttDefList.hpp>


XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  XSComplexTypeDefinition: Constructors and Destructor
// ---------------------------------------------------------------------------
XSComplexTypeDefinition::XSComplexTypeDefinition
(
    ComplexTypeInfo* const          complexTypeInfo
    , XSWildcard* const             xsWildcard
    , XSSimpleTypeDefinition* const xsSimpleType
    , XSAttributeUseList* const     xsAttList
    , XSTypeDefinition* const       xsBaseType
    , XSParticle* const             xsParticle
    , XSAnnotation* const           headAnnot
    , XSModel* const                xsModel
    , MemoryManager* const          manager
)
    : XSTypeDefinition(COMPLEX_TYPE, xsBaseType, xsModel, manager)
    , fComplexTypeInfo(complexTypeInfo)
    , fXSWildcard(xsWildcard)
    , fXSAttributeUseList(xsAttList)
    , fXSSimpleTypeDefinition(xsSimpleType)
    , fXSAnnotationList(0)
    , fParticle(xsParticle)
    , fProhibitedSubstitution(0)
{
    int blockset = fComplexTypeInfo->getBlockSet();
    if (blockset)
    {
        if (blockset & SchemaSymbols::XSD_EXTENSION)
            fProhibitedSubstitution |= XSConstants::DERIVATION_EXTENSION;

        if (blockset & SchemaSymbols::XSD_RESTRICTION)
            fProhibitedSubstitution |= XSConstants::DERIVATION_RESTRICTION;
    }

    if (headAnnot)
    {
        fXSAnnotationList = new (manager) RefVectorOf<XSAnnotation>(1, false, manager);
        XSAnnotation* annot = headAnnot;

        do
        {
            fXSAnnotationList->addElement(annot);
            annot = annot->getNext();        
        } while (annot);
    }
}

XSComplexTypeDefinition::~XSComplexTypeDefinition() 
{
    // don't delete fXSWildcard - deleted by XSModel
    // don't delete fXSSimpleTypeDefinition - deleted by XSModel
    if (fXSAttributeUseList)
        delete fXSAttributeUseList;

    if (fXSAnnotationList)
        delete fXSAnnotationList;

    if (fParticle)
        delete fParticle;
}

// ---------------------------------------------------------------------------
//  XSComplexTypeDefinition: access methods
// ---------------------------------------------------------------------------
XSConstants::DERIVATION_TYPE XSComplexTypeDefinition::getDerivationMethod() const
{
    switch(fComplexTypeInfo->getDerivedBy()) {
        case SchemaSymbols::XSD_RESTRICTION:
            return XSConstants::DERIVATION_RESTRICTION;
        case SchemaSymbols::XSD_EXTENSION:
            return XSConstants::DERIVATION_EXTENSION;
        default:
            return XSConstants::DERIVATION_NONE;
    }
}

bool XSComplexTypeDefinition::getAbstract() const
{
    return fComplexTypeInfo->getAbstract();
}


XSComplexTypeDefinition::CONTENT_TYPE XSComplexTypeDefinition::getContentType() const
{
    switch(fComplexTypeInfo->getContentType()) {
        case SchemaElementDecl::Simple:
            return CONTENTTYPE_SIMPLE;
        case SchemaElementDecl::Empty:
            return CONTENTTYPE_EMPTY;
        case SchemaElementDecl::Children:
            return CONTENTTYPE_ELEMENT;
        default:
            //case SchemaElementDecl::Mixed_Complex:
            //case SchemaElementDecl::Mixed_Simple:
            //case SchemaElementDecl::Any:
            return CONTENTTYPE_MIXED;
    }
}

bool XSComplexTypeDefinition::isProhibitedSubstitution(XSConstants::DERIVATION_TYPE toTest)                                                     
{
    if (fProhibitedSubstitution & toTest)
        return true;

    return false;
}

XSAnnotationList *XSComplexTypeDefinition::getAnnotations()
{    
    return fXSAnnotationList;
}

// ---------------------------------------------------------------------------
//  XSComplexTypeDefinition: virtual methods
// ---------------------------------------------------------------------------
const XMLCh *XSComplexTypeDefinition::getName() 
{
    return fComplexTypeInfo->getTypeLocalName();
}

const XMLCh *XSComplexTypeDefinition::getNamespace() 
{
    return fComplexTypeInfo->getTypeUri();
}

XSNamespaceItem *XSComplexTypeDefinition::getNamespaceItem() 
{
    return fXSModel->getNamespaceItem(getNamespace());
}

bool XSComplexTypeDefinition::getAnonymous() const
{
    return fComplexTypeInfo->getAnonymous(); 
}

XSTypeDefinition *XSComplexTypeDefinition::getBaseType() 
{
    return fBaseType;
}

bool XSComplexTypeDefinition::derivedFromType(const XSTypeDefinition * const ancestorType)
{
    if (!ancestorType)
        return false;

    XSTypeDefinition* type = this;

    while (type && (type != ancestorType))
    {
        type = type->getBaseType();
    }

    return (type == ancestorType);
}

XERCES_CPP_NAMESPACE_END
