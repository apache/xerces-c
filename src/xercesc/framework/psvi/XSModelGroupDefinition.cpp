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
 * Revision 1.6  2004/09/08 13:56:08  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.5  2003/11/21 17:29:53  knoaman
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
 * Revision 1.2  2003/09/17 17:45:37  neilg
 * remove spurious inlines; hopefully this will make Solaris/AIX compilers happy.
 *
 * Revision 1.1  2003/09/16 14:33:36  neilg
 * PSVI/schema component model classes, with Makefile/configuration changes necessary to build them
 *
 */

#include <xercesc/framework/psvi/XSModelGroupDefinition.hpp>
#include <xercesc/framework/psvi/XSParticle.hpp>
#include <xercesc/framework/psvi/XSModel.hpp>
#include <xercesc/validators/schema/XercesGroupInfo.hpp>


XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  XSModelGroupDefinition: Constructors and Destructors
// ---------------------------------------------------------------------------
XSModelGroupDefinition::XSModelGroupDefinition(XercesGroupInfo* const groupInfo,
                                               XSParticle* const      groupParticle,
                                               XSAnnotation* const    annot,
                                               XSModel* const         xsModel,
                                               MemoryManager* const   manager)
    : XSObject(XSConstants::MODEL_GROUP_DEFINITION, xsModel, manager)
    , fGroupInfo(groupInfo)
    , fModelGroupParticle(groupParticle)
    , fAnnotation(annot)
{
}

XSModelGroupDefinition::~XSModelGroupDefinition()
{
    if (fModelGroupParticle) // Not owned by XSModel
        delete fModelGroupParticle;
}

// ---------------------------------------------------------------------------
//  XSModelGroupDefinition: XSModel virtual methods
// ---------------------------------------------------------------------------
const XMLCh *XSModelGroupDefinition::getName() 
{
    return fXSModel->getURIStringPool()->getValueForId(fGroupInfo->getNameId());
}

const XMLCh *XSModelGroupDefinition::getNamespace() 
{
    return fXSModel->getURIStringPool()->getValueForId(fGroupInfo->getNamespaceId());
}

XSNamespaceItem *XSModelGroupDefinition::getNamespaceItem() 
{
    return fXSModel->getNamespaceItem(getNamespace());
}

// ---------------------------------------------------------------------------
//  XSModelGroupDefinition: access methods
// ---------------------------------------------------------------------------
XSModelGroup* XSModelGroupDefinition::getModelGroup()
{
    if (fModelGroupParticle)
        return fModelGroupParticle->getModelGroupTerm();

    return 0;
}


XERCES_CPP_NAMESPACE_END


