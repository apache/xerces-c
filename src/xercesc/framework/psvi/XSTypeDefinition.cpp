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
 * Revision 1.10  2004/01/29 11:46:30  cargilld
 * Code cleanup changes to get rid of various compiler diagnostic messages.
 *
 * Revision 1.9  2003/12/24 15:25:07  cargilld
 * Improved algorithm for finding derivedFrom.
 *
 * Revision 1.8  2003/12/15 17:23:48  cargilld
 * psvi updates; cleanup revisits and bug fixes
 *
 * Revision 1.7  2003/11/25 18:08:31  knoaman
 * Misc. PSVI updates. Thanks to David Cargill.
 *
 * Revision 1.6  2003/11/21 17:34:04  knoaman
 * PSVI update
 *
 * Revision 1.5  2003/11/15 21:18:39  neilg
 * fixes for compilation under gcc
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

#include <xercesc/framework/psvi/XSTypeDefinition.hpp>
#include <xercesc/framework/psvi/XSModel.hpp>
#include <xercesc/util/XMLString.hpp>

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  XSTypeDefinition: Constructors and Destructor
// ---------------------------------------------------------------------------
XSTypeDefinition::XSTypeDefinition(TYPE_CATEGORY           typeCategory,
                                   XSTypeDefinition* const xsBaseType,
                                   XSModel* const          xsModel,
                                   MemoryManager* const    manager)
    : XSObject(XSConstants::TYPE_DEFINITION, xsModel, manager)
    , fTypeCategory(typeCategory)
    , fFinal(0)
    , fBaseType(xsBaseType)    
{
}

XSTypeDefinition::~XSTypeDefinition()
{
}

// ---------------------------------------------------------------------------
//  XSTypeDefinition: access methods
// ---------------------------------------------------------------------------
bool XSTypeDefinition::isFinal(short toTest)
{
    if (fFinal & toTest)
        return true;

    return false;

}

bool XSTypeDefinition::derivedFrom(const XMLCh *typeNamespace, 
                                   const XMLCh *name)
{    
    if (!name)
        return false;

    XSTypeDefinition* type = fXSModel->getTypeDefinition(name, typeNamespace);
    
    if (!type)
        return false;

    return derivedFromType(type);
}

XERCES_CPP_NAMESPACE_END


