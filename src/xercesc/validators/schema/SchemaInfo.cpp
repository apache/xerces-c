/*
 * Copyright 2001,2004 The Apache Software Foundation.
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
 * Revision 1.13  2004/09/29 20:58:10  knoaman
 * [Bug 1209] Problem with id usage across schema
 * http://issues.apache.org/jira/browse/XERCESC-1209
 *
 * Revision 1.12  2004/09/08 13:56:56  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.11  2003/12/11 19:26:27  knoaman
 * Store non schema attributes from parent in XSAnnotation
 *
 * Revision 1.10  2003/05/16 21:43:21  knoaman
 * Memory manager implementation: Modify constructors to pass in the memory manager.
 *
 * Revision 1.9  2002/11/04 14:49:41  tng
 * C++ Namespace Support.
 *
 * Revision 1.8  2002/09/24 20:12:48  tng
 * Performance: use XMLString::equals instead of XMLString::compareString
 *
 * Revision 1.7  2002/06/21 11:52:24  knoaman
 * cleanup
 *
 * Revision 1.6  2002/05/27 20:23:48  knoaman
 * Performance: lazily store top-level components to eliminate unnecessary traversal
 * of DOM tree when looking up for a top level component.
 *
 * Revision 1.5  2002/05/21 19:30:47  tng
 * DOM Reorganization: modify to use the new DOM interface.
 *
 * Revision 1.4  2002/03/19 21:22:39  knoaman
 * Fix for declarations referenced from a different NS in the case of a circular import.
 *
 * Revision 1.3  2002/03/19 15:57:12  knoaman
 * Fix for bug 7074.
 *
 * Revision 1.2  2002/02/06 22:24:59  knoaman
 * Use IDOM for schema processing.
 *
 * Revision 1.1.1.1  2002/02/01 22:22:46  peiyongz
 * sane_include
 *
 * Revision 1.6  2001/12/12 20:52:19  peiyongz
 * memory leak: fRedefineList
 *
 * Revision 1.5  2001/10/04 15:08:56  knoaman
 * Add support for circular import.
 *
 * Revision 1.4  2001/07/31 15:26:54  knoaman
 * Added support for <attributeGroup>.
 *
 * Revision 1.3  2001/07/09 14:29:43  knoaman
 * Fixes for import/include declarations
 *
 * Revision 1.2  2001/05/11 13:27:36  tng
 * Copyright update.
 *
 * Revision 1.1  2001/05/10 16:33:16  knoaman
 * Traverse Schema Part III + error messages.
 *
 */

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/validators/schema/SchemaInfo.hpp>
#include <xercesc/validators/schema/XUtil.hpp>
#include <xercesc/validators/schema/SchemaSymbols.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/internal/ValidationContextImpl.hpp>

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  SchemaInfo: Constructors and Destructor
// ---------------------------------------------------------------------------
SchemaInfo::SchemaInfo(const unsigned short elemAttrDefaultQualified,
                       const int blockDefault,
                       const int finalDefault,
                       const int targetNSURI,
                       const int scopeCount,
                       const unsigned int namespaceScopeLevel,
                       XMLCh* const schemaURL,
                       const XMLCh* const targetNSURIString,
                       const DOMElement* const root,
                       MemoryManager* const manager)
    : fAdoptInclude(false)
    , fProcessed(false)
    , fElemAttrDefaultQualified(elemAttrDefaultQualified)
    , fBlockDefault(blockDefault)
    , fFinalDefault(finalDefault)
    , fTargetNSURI(targetNSURI)
    , fScopeCount(scopeCount)
    , fNamespaceScopeLevel(namespaceScopeLevel)
    , fCurrentSchemaURL(schemaURL)
    , fTargetNSURIString(targetNSURIString)
    , fSchemaRootElement(root)
    , fIncludeInfoList(0)
    , fImportedInfoList(0)
    , fImportingInfoList(0)
    , fFailedRedefineList(0)
    , fImportedNSList(0)
    , fRecursingAnonTypes(0)
    , fRecursingTypeNames(0)
    , fNonXSAttList(0)
    , fValidationContext(0)
    , fMemoryManager(manager)
{
    fImportingInfoList = new (fMemoryManager) RefVectorOf<SchemaInfo>(4, false, fMemoryManager);
    for (unsigned int i = 0; i < C_Count; i++)
        fTopLevelComponents[i] = 0;
    fNonXSAttList = new (fMemoryManager) ValueVectorOf<DOMNode*>(2, fMemoryManager);
    fValidationContext = new (fMemoryManager) ValidationContextImpl(fMemoryManager);
}


SchemaInfo::~SchemaInfo()
{
    fMemoryManager->deallocate(fCurrentSchemaURL);//delete [] fCurrentSchemaURL;
    delete fImportedInfoList;

    if (fAdoptInclude)
        delete fIncludeInfoList;

    delete fImportingInfoList;

    fImportedInfoList = fIncludeInfoList = fImportingInfoList = 0;

    delete fImportedNSList;
    fImportedNSList = 0;

    delete fFailedRedefineList;
    fFailedRedefineList = 0;

    delete fRecursingAnonTypes;
    fRecursingAnonTypes = 0;

    delete fRecursingTypeNames;
    fRecursingTypeNames = 0;

    for (unsigned int i = 0; i < C_Count; i++) {

        delete fTopLevelComponents[i];
        fTopLevelComponents[i] = 0;
    }

    delete fNonXSAttList;
    fNonXSAttList = 0;

    delete fValidationContext;
    fValidationContext = 0;
}

// ---------------------------------------------------------------------------
//  SchemaInfo:
// ---------------------------------------------------------------------------
DOMElement*
SchemaInfo::getTopLevelComponent(const unsigned short compCategory,
                                 const XMLCh* const compName,
                                 const XMLCh* const name,
                                 SchemaInfo** enclosingSchema) {

    SchemaInfo* currentInfo = this;
    DOMElement* child = getTopLevelComponent(compCategory, compName, name);

    if (child == 0) {

        unsigned int listSize = (fIncludeInfoList) ? fIncludeInfoList->size() : 0;

        for (unsigned int i=0; i < listSize; i++) {

            currentInfo = fIncludeInfoList->elementAt(i);

            if (currentInfo == this)
                continue;

            child = currentInfo->getTopLevelComponent(compCategory, compName, name);

            if (child != 0) {

                *enclosingSchema = currentInfo;
                break;
            }
        }
    }

    return child;
}


DOMElement*
SchemaInfo::getTopLevelComponent(const unsigned short compCategory,
                                 const XMLCh* const compName,
                                 const XMLCh* const name) {

    if (compCategory >= C_Count)
        return 0;

    DOMElement* child = XUtil::getFirstChildElement(fSchemaRootElement);

    if (!child)
        return 0;

    ValueVectorOf<DOMElement*>* compList = fTopLevelComponents[compCategory];

    if (fTopLevelComponents[compCategory] == 0) {

        compList= new (fMemoryManager) ValueVectorOf<DOMElement*>(16, fMemoryManager);
        fTopLevelComponents[compCategory] = compList;
    }
    else {
        unsigned int listLen = compList->size();

        for (unsigned int i= 0; i < listLen; i++) {

            child = compList->elementAt(i);
            if (XMLString::equals(child->getAttribute(SchemaSymbols::fgATT_NAME), name))
                return child;
        }
    }

    DOMElement* redefParent = (DOMElement*) child->getParentNode();

    // Parent is not "redefine"
    if (!XMLString::equals(redefParent->getLocalName(),SchemaSymbols::fgELT_REDEFINE))
        redefParent = 0;

    while (child != 0) {

        if (XMLString::equals(child->getLocalName(), compName)) {

            compList->addElement(child);

            if (XMLString::equals(child->getAttribute(SchemaSymbols::fgATT_NAME), name))
                return child;
        }
        else if (XMLString::equals(child->getLocalName(),SchemaSymbols::fgELT_REDEFINE)
                 && (!fFailedRedefineList || !fFailedRedefineList->containsElement(child))) { // if redefine

            DOMElement* redefineChild = XUtil::getFirstChildElement(child);

            while (redefineChild != 0) {

                if ((!fFailedRedefineList || !fFailedRedefineList->containsElement(redefineChild))
                    && XMLString::equals(redefineChild->getLocalName(), compName)) {

                    compList->addElement(redefineChild);

                    if (XMLString::equals(redefineChild->getAttribute(SchemaSymbols::fgATT_NAME), name))
                        return redefineChild;
                }

                redefineChild = XUtil::getNextSiblingElement(redefineChild);
            }
        }

        child = XUtil::getNextSiblingElement(child);

        if (child == 0 && redefParent) {

            child = XUtil::getNextSiblingElement(redefParent);
            redefParent = 0;
        }
    }

    return child;
}

void SchemaInfo::updateImportingInfo(SchemaInfo* const importingInfo) {

    if (!fImportingInfoList->containsElement(importingInfo)) {
        fImportingInfoList->addElement(importingInfo);
    }

    unsigned int listSize = importingInfo->fImportingInfoList->size();

    for (unsigned int i=0; i < listSize; i++) {

        SchemaInfo* tmpInfo = importingInfo->fImportingInfoList->elementAt(i);

        if (tmpInfo != this && !fImportingInfoList->containsElement(tmpInfo)) {
            fImportingInfoList->addElement(tmpInfo);
        }
    }
}

XERCES_CPP_NAMESPACE_END

/**
  * End of file SchemaInfo.cpp
  */


