/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 2001 The Apache Software Foundation.  All rights
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
 * originally based on software copyright (c) 2001, International
 * Business Machines, Inc., http://www.ibm.com .  For more information
 * on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 */

/*
 * $Log$
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
                       const DOMElement* const root)
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
{
    fImportingInfoList = new RefVectorOf<SchemaInfo>(4, false);
    for (unsigned int i = 0; i < C_Count; i++)
        fTopLevelComponents[i] = 0;
}


SchemaInfo::~SchemaInfo()
{
    delete [] fCurrentSchemaURL;
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

        compList= new ValueVectorOf<DOMElement*>(16);
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

/**
  * End of file SchemaInfo.cpp
  */


