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
#include <validators/schema/SchemaInfo.hpp>
#include <validators/schema/XUtil.hpp>
#include <validators/schema/SchemaSymbols.hpp>
#include <util/XMLString.hpp>

// ---------------------------------------------------------------------------
//  SchemaInfo: Constructors and Destructor
// ---------------------------------------------------------------------------
SchemaInfo::SchemaInfo(const unsigned short elemAttrDefaultQualified,
                       const int blockDefault,
                       const int finalDefault,
                       const int targetNSURI,
                       const int currentScope,
                       const int scopeCount,
                       const unsigned int namespaceScopeLevel,
                       XMLCh* const schemaURL,
                       const XMLCh* const targetNSURIString,
                       XMLStringPool* const stringPool,
                       const DOM_Element& root)
    : fElemAttrDefaultQualified(elemAttrDefaultQualified)
    , fBlockDefault(blockDefault)
    , fFinalDefault(finalDefault)
    , fTargetNSURI(targetNSURI)
    , fCurrentScope(currentScope)
    , fScopeCount(scopeCount)
    , fNamespaceScopeLevel(namespaceScopeLevel)
    , fCurrentSchemaURL(schemaURL)
    , fTargetNSURIString(targetNSURIString)
    , fStringPool(stringPool)
    , fSchemaRootElement(root)
    , fIncludeList(0)
    , fImportList(0)
    , fImportingList(0)
    , fRedefineList(0)
{
    fImportingList = new RefVectorOf<SchemaInfo>(4, false);
}


SchemaInfo::~SchemaInfo()
{
    delete [] fCurrentSchemaURL;
    delete fImportList;
    delete fIncludeList;
    delete fImportingList;

    fImportList = fIncludeList = fImportingList = 0;

    delete fRedefineList;
    fRedefineList = 0;
}

// ---------------------------------------------------------------------------
//  SchemaInfo:
// ---------------------------------------------------------------------------
DOM_Element
SchemaInfo::getTopLevelComponent(const XMLCh* const compCategory,
                                 const XMLCh* const name,
                                 SchemaInfo** enclosingSchema) {

    SchemaInfo* currentInfo = this;
    DOM_Element child = getTopLevelComponent(compCategory, name);

    if (child == 0) {

        unsigned int listSize = (fIncludeList) ? fIncludeList->size() : 0;

        for (unsigned int i=0; i < listSize; i++) {

            currentInfo = fIncludeList->elementAt(i);

            child = currentInfo->getTopLevelComponent(compCategory, name);

            if (child != 0) {

                *enclosingSchema = currentInfo;
                break;
            }
        }

        if (child == 0 && fRedefineList) { // try redefine list

			currentInfo = fRedefineList->get(compCategory, fStringPool->addOrFind(name));

            if (currentInfo) {
                child = currentInfo->getTopLevelComponent(compCategory, name);

                if (child != 0) {
                    *enclosingSchema = currentInfo;
                }
            }
        }
    }

    return child;
}


DOM_Element
SchemaInfo::getTopLevelComponent(const XMLCh* const compCategory,
                                 const XMLCh* const name) {

    DOM_Element child = XUtil::getFirstChildElement(fSchemaRootElement);

    while (child != 0) {

        if (child.getLocalName().equals(compCategory)) {

            if (child.getAttribute(SchemaSymbols::fgATT_NAME).equals(name)) {
                break;
            }
        }
        else if (child.getLocalName().equals(SchemaSymbols::fgELT_REDEFINE)) { // if redefine

            DOM_Element redefineChild = XUtil::getFirstChildElement(child);

            while (redefineChild != 0) {

                if (redefineChild.getLocalName().equals(compCategory)) {

                    if (redefineChild.getAttribute(SchemaSymbols::fgATT_NAME).equals(name)) {
                        break;
                    }
                }

                redefineChild = XUtil::getNextSiblingElement(redefineChild);
            }

            if (redefineChild != 0) {

                child = redefineChild;
                break;
            }
        }

        child = XUtil::getNextSiblingElement(child);
    }

    return child;
}

void SchemaInfo::updateImportingInfo(SchemaInfo* const importingInfo) {

    if (!fImportingList->containsElement(importingInfo)) {
        fImportingList->addElement(importingInfo);
    }

    unsigned int listSize = importingInfo->fImportingList->size();

    for (unsigned int i=0; i < listSize; i++) {

        SchemaInfo* tmpInfo = importingInfo->fImportingList->elementAt(i);

        if (tmpInfo != this && !fImportingList->containsElement(tmpInfo)) {
            fImportingList->addElement(tmpInfo);
        }
    }
}

/**
  * End of file SchemaInfo.cpp
  */


