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
 * Revision 1.9  2003/12/29 17:06:31  knoaman
 * PSVI: return value constraint only if global declaration
 *
 * Revision 1.8  2003/12/29 16:15:42  knoaman
 * More PSVI updates
 *
 * Revision 1.7  2003/11/21 22:34:45  neilg
 * More schema component model implementation, thanks to David Cargill.
 * In particular, this cleans up and completes the XSModel, XSNamespaceItem,
 * XSAttributeDeclaration and XSAttributeGroup implementations.
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

#include <xercesc/framework/psvi/XSAttributeDeclaration.hpp>
#include <xercesc/framework/psvi/XSModel.hpp>
#include <xercesc/framework/psvi/XSNamespaceItem.hpp>
#include <xercesc/util/StringPool.hpp>
#include <xercesc/validators/schema/SchemaGrammar.hpp>
#include <xercesc/validators/schema/SchemaAttDef.hpp>

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  XSAttributeDeclaration: Constructors and Destructor
// ---------------------------------------------------------------------------
XSAttributeDeclaration::XSAttributeDeclaration(SchemaAttDef* const           attDef,
                                               XSSimpleTypeDefinition* const typeDef,
                                               XSAnnotation* const           annot,
                                               XSModel* const                xsModel,
                                               XSConstants::SCOPE            scope,
                                               XSComplexTypeDefinition*      enclosingCTDefinition,
                                               MemoryManager * const         manager)
    : XSObject(XSConstants::ATTRIBUTE_DECLARATION, xsModel, manager)
    , fAttDef(attDef)
    , fTypeDefinition(typeDef)
    , fAnnotation(annot)
    , fScope(scope)
    , fEnclosingCTDefinition(enclosingCTDefinition)    
{
}

XSAttributeDeclaration::~XSAttributeDeclaration() 
{
    // don't delete fTypeDefinition - deleted by XSModel
}

// ---------------------------------------------------------------------------
//  XSAttributeDeclaration: XSObject virtual methods
// ---------------------------------------------------------------------------
const XMLCh *XSAttributeDeclaration::getName() 
{
    return fAttDef->getAttName()->getLocalPart();
}

const XMLCh *XSAttributeDeclaration::getNamespace() 
{
    return fXSModel->getURIStringPool()->getValueForId(fAttDef->getAttName()->getURI());
}

XSNamespaceItem *XSAttributeDeclaration::getNamespaceItem() 
{
    return fXSModel->getNamespaceItem(getNamespace());
}

unsigned int XSAttributeDeclaration::getId() const
{
    return fId;
}

// ---------------------------------------------------------------------------
//  XSAttributeDeclaration: access methods
// ---------------------------------------------------------------------------

XSConstants::VALUE_CONSTRAINT XSAttributeDeclaration::getConstraintType() const
{
    if (fAttDef->getDefaultType() & XMLAttDef::Default)
        return XSConstants::VC_DEFAULT;

    if ((fAttDef->getDefaultType() == XMLAttDef::Fixed) ||
        (fAttDef->getDefaultType() == XMLAttDef::Required_And_Fixed))
        return XSConstants::VC_FIXED;

    return XSConstants::VC_NONE;
}

const XMLCh *XSAttributeDeclaration::getConstraintValue()
{
    if (fScope == XSConstants::SCOPE_GLOBAL)
        return fAttDef->getValue();

    return 0;
}

bool XSAttributeDeclaration::getRequired() const
{
    if (fAttDef->getDefaultType() == XMLAttDef::Required ||
        fAttDef->getDefaultType() == XMLAttDef::Required_And_Fixed)
        return true;

    return false;
}

XERCES_CPP_NAMESPACE_END


