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
 * Revision 1.11  2004/09/08 13:56:09  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.10  2004/05/05 14:50:41  peiyongz
 * remove inline
 *
 * Revision 1.9  2004/05/04 19:02:40  cargilld
 * Enable IDs to work on all kinds of schema components
 *
 * Revision 1.8  2003/11/21 17:34:04  knoaman
 * PSVI update
 *
 * Revision 1.7  2003/11/15 21:18:39  neilg
 * fixes for compilation under gcc
 *
 * Revision 1.6  2003/11/14 22:47:53  neilg
 * fix bogus log message from previous commit...
 *
 * Revision 1.5  2003/11/14 22:33:30  neilg
 * Second phase of schema component model implementation.  
 * Implement XSModel, XSNamespaceItem, and the plumbing necessary
 * to connect them to the other components.
 * Thanks to David Cargill.
 *
 * Revision 1.4  2003/10/15 16:02:49  peiyongz
 * Solve Linkage error on Solaris
 *
 * Revision 1.3  2003/10/10 18:37:51  neilg
 * update XSModel and XSObject interface so that IDs can be used to query components in XSModels, and so that those IDs can be recovered from components
 *
 * Revision 1.2  2003/09/17 17:45:37  neilg
 * remove spurious inlines; hopefully this will make Solaris/AIX compilers happy.
 *
 * Revision 1.1  2003/09/16 14:33:36  neilg
 * PSVI/schema component model classes, with Makefile/configuration changes necessary to build them
 *
 */

#include <xercesc/framework/psvi/XSObject.hpp>
#include <xercesc/framework/psvi/XSModel.hpp>

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  XSObject: Constructors and Destructor
// ---------------------------------------------------------------------------
XSObject::XSObject(XSConstants::COMPONENT_TYPE compType,
                   XSModel* const xsModel,
                   MemoryManager* const manager)  
    : fComponentType(compType)
    , fXSModel(xsModel)
    , fMemoryManager(manager)
    , fId(0)
{
    if (xsModel)
    {
        xsModel->addComponentToIdVector(this, compType-1);
    }
}

XSObject::~XSObject()
{
}

// ---------------------------------------------------------------------------
//  XSObject: Virtual interface methods
// ---------------------------------------------------------------------------
const XMLCh *XSObject::getName() 
{
    return 0;
}

const XMLCh *XSObject::getNamespace() 
{
    return 0;
}

XSNamespaceItem *XSObject::getNamespaceItem() 
{
    return 0;
}

unsigned int XSObject::getId() const
{
    return fId;
}

void XSObject::setId(unsigned int id)
{
    fId = id;
}

XERCES_CPP_NAMESPACE_END


