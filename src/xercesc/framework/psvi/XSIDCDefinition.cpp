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
 * Revision 1.9  2004/09/08 13:56:08  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.8  2004/01/29 11:46:30  cargilld
 * Code cleanup changes to get rid of various compiler diagnostic messages.
 *
 * Revision 1.7  2003/12/15 17:23:48  cargilld
 * psvi updates; cleanup revisits and bug fixes
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

#include <xercesc/framework/psvi/XSIDCDefinition.hpp>
#include <xercesc/validators/schema/identity/IC_KeyRef.hpp>
#include <xercesc/validators/schema/identity/IC_Selector.hpp>
#include <xercesc/validators/schema/identity/XercesXPath.hpp>
#include <xercesc/framework/psvi/XSModel.hpp>
#include <xercesc/framework/psvi/XSAnnotation.hpp>
#include <xercesc/util/StringPool.hpp>

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  XSIDCDefinition: Constructors and Destructor
// ---------------------------------------------------------------------------
XSIDCDefinition::XSIDCDefinition(IdentityConstraint* const identityConstraint,
                                 XSIDCDefinition*  const   keyIC,
                                 XSAnnotation* const       headAnnot,
                                 StringList* const         stringList,
                                 XSModel* const            xsModel,
                                 MemoryManager* const      manager)
    : XSObject(XSConstants::IDENTITY_CONSTRAINT, xsModel, manager)
    , fIdentityConstraint(identityConstraint)
    , fKey(keyIC)
    , fStringList(stringList)
    , fXSAnnotationList(0)
{
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

XSIDCDefinition::~XSIDCDefinition()
{
    if (fStringList)
        delete fStringList;

    // don't delete fKey - deleted by XSModel
    if (fXSAnnotationList)
        delete fXSAnnotationList;
}

// ---------------------------------------------------------------------------
//  XSIDCDefinition: XSObject virtual methods
// ---------------------------------------------------------------------------
const XMLCh *XSIDCDefinition::getName() 
{
    return fIdentityConstraint->getIdentityConstraintName();
}

const XMLCh *XSIDCDefinition::getNamespace() 
{
    return fXSModel->getURIStringPool()->getValueForId(fIdentityConstraint->getNamespaceURI());
}

XSNamespaceItem *XSIDCDefinition::getNamespaceItem() 
{
    return fXSModel->getNamespaceItem(getNamespace());
}

// ---------------------------------------------------------------------------
//  XSIDCDefinition: access methods
// ---------------------------------------------------------------------------
XSIDCDefinition::IC_CATEGORY XSIDCDefinition::getCategory() const
{
    switch(fIdentityConstraint->getType()) {
        case IdentityConstraint::UNIQUE:
            return IC_UNIQUE;
        case IdentityConstraint::KEY:
            return IC_KEY;
        case IdentityConstraint::KEYREF:
            return IC_KEYREF;
        default:
            // REVISIT:
            // should never really get here... IdentityConstraint::Unknown is the other
            // choice so need a default case for completeness; should issues error?
            return IC_KEY;
    }
}

const XMLCh *XSIDCDefinition::getSelectorStr()
{
    return fIdentityConstraint->getSelector()->getXPath()->getExpression();
}


XSAnnotationList *XSIDCDefinition::getAnnotations()
{
    return fXSAnnotationList;
}

XERCES_CPP_NAMESPACE_END


