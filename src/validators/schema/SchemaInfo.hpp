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
 * $Id$
 */

#if !defined(SCHEMAINFO_HPP)
#define SCHEMAINFO_HPP


/** When in a <redefine>, type definitions being used (and indeed
  * refs to <group>'s and <attributeGroup>'s) may refer to info
  * items either in the schema being redefined, in the <redefine>,
  * or else in the schema doing the redefining.  Because of this
  * latter we have to be prepared sometimes to look for our type
  * definitions outside the schema stored in fSchemaRootElement.
  * This simple class does this; it's just a linked list that
  * lets us look at the <schema>'s on the queue; note also that this
  * should provide us with a mechanism to handle nested <redefine>'s.
  * It's also a handy way of saving schema info when importing/including.
  */

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <dom/DOM_Element.hpp>
#include <util/RefVectorOf.hpp>
#include <util/RefHash2KeysTableOf.hpp>
#include <util/StringPool.hpp>

class SchemaInfo
{
public:

    enum ListType {
        // Redefine is treated as an include
        IMPORT = 1,
        INCLUDE = 2
    };

    // -----------------------------------------------------------------------
    //  Constructor/Destructor
    // -----------------------------------------------------------------------
    SchemaInfo(const unsigned short fElemAttrDefaultQualified,
               const int blockDefault,
               const int finalDefault,
               const int targetNSURI,
               const int currentScope,
               const int scopeCount,
               const unsigned int namespaceScopeLevel,
               XMLCh* const schemaURL,
               const XMLCh* const targetNSURIString,
               XMLStringPool* const stringPool,
               const DOM_Element& root);
    ~SchemaInfo();


	// -----------------------------------------------------------------------
    //  Getter methods
    // -----------------------------------------------------------------------
    XMLCh*                   getCurrentSchemaURL() const;
    const XMLCh* const       getTargetNSURIString() const;
    DOM_Element              getRoot() const;
    int                      getBlockDefault() const;
    int                      getFinalDefault() const;
    int                      getTargetNSURI() const;
    int                      getCurrentScope() const;
    int                      getScopeCount() const;
    unsigned int             getNamespaceScopeLevel() const;
    unsigned short           getElemAttrDefaultQualified() const;
    RefVectorEnumerator<SchemaInfo> getImportingListEnumerator() const;

	// -----------------------------------------------------------------------
    //  Setter methods
    // -----------------------------------------------------------------------
    void setCurrentScope(const int aValue);
    void setScopeCount(const int aValue);
    
	// -----------------------------------------------------------------------
    //  Access methods
    // -----------------------------------------------------------------------
    void addSchemaInfo(SchemaInfo* const toAdd, const ListType aListType);
    void addRedefineInfo(const XMLCh* const categName, const XMLCh* const typeName,
                         SchemaInfo* const toAdd);
    bool containsInfo(const SchemaInfo* const toCheck, const ListType aListType) const;
    SchemaInfo* getImportInfo(const unsigned int namespaceURI) const;
    SchemaInfo* getRedefInfo(const XMLCh* const categName,
                             const XMLCh* const typeName);
    DOM_Element getTopLevelComponent(const XMLCh* const compCategory,
                                     const XMLCh* const name);
    DOM_Element getTopLevelComponent(const XMLCh* const compCategory,
                                     const XMLCh* const name,
                                     SchemaInfo** enclosingSchema);
    void updateImportingInfo(SchemaInfo* const importingInfo);
    bool circularImportExist(const unsigned int nameSpaceURI);

private:
    // -----------------------------------------------------------------------
    //  Private data members
    // -----------------------------------------------------------------------
    unsigned short     fElemAttrDefaultQualified;
    int                fBlockDefault;
    int                fFinalDefault;
    int                fTargetNSURI;
    int                fCurrentScope;
    int                fScopeCount;
    unsigned int       fNamespaceScopeLevel;
    XMLCh*             fCurrentSchemaURL;
    const XMLCh*       fTargetNSURIString;
    XMLStringPool*     fStringPool;
    DOM_Element        fSchemaRootElement;
    RefVectorOf<SchemaInfo>* fIncludeList;
    RefVectorOf<SchemaInfo>* fImportList;
    RefVectorOf<SchemaInfo>* fImportingList;
    RefHash2KeysTableOf<SchemaInfo>* fRedefineList;
};

// ---------------------------------------------------------------------------
//  SchemaInfo: Getter methods
// ---------------------------------------------------------------------------
inline unsigned short SchemaInfo::getElemAttrDefaultQualified() const {

    return fElemAttrDefaultQualified;
}

inline int SchemaInfo::getBlockDefault() const {

    return fBlockDefault;
}

inline int SchemaInfo::getFinalDefault() const {

    return fFinalDefault;
}

inline unsigned int SchemaInfo::getNamespaceScopeLevel() const {
    return fNamespaceScopeLevel;
}

inline XMLCh* SchemaInfo::getCurrentSchemaURL() const {

    return fCurrentSchemaURL;
}

inline const XMLCh* const SchemaInfo::getTargetNSURIString() const {

    return fTargetNSURIString;
}

inline DOM_Element SchemaInfo::getRoot() const {

    return fSchemaRootElement;
}

inline int SchemaInfo::getTargetNSURI() const {

    return fTargetNSURI;
}

inline int SchemaInfo::getCurrentScope() const {

    return fCurrentScope;
}

inline int SchemaInfo::getScopeCount() const {

    return fScopeCount;
}

inline RefVectorEnumerator<SchemaInfo>
SchemaInfo::getImportingListEnumerator() const {

    return RefVectorEnumerator<SchemaInfo>(fImportingList);
}

// ---------------------------------------------------------------------------
//  Setter methods
// ---------------------------------------------------------------------------
inline void SchemaInfo::setCurrentScope(const int aValue) {

    fCurrentScope = aValue;
}

inline void SchemaInfo::setScopeCount(const int aValue) {

    fScopeCount = aValue;
}

// ---------------------------------------------------------------------------
//  SchemaInfo: Access methods
// ---------------------------------------------------------------------------
inline void SchemaInfo::addSchemaInfo(SchemaInfo* const toAdd,
                                      const ListType aListType) {

    if (aListType == IMPORT) {

        if (!fImportList)
            fImportList = new RefVectorOf<SchemaInfo>(8, false);

        if (!fImportList->containsElement(toAdd)) {
		    fImportList->addElement(toAdd);
            toAdd->updateImportingInfo(this);
        }
    }
    else {

        if (!fIncludeList)
            fIncludeList = new RefVectorOf<SchemaInfo>(8, false);

        if (!fIncludeList->containsElement(toAdd))
		    fIncludeList->addElement(toAdd);
    }
}

inline void SchemaInfo::addRedefineInfo(const XMLCh* const categName,
                                        const XMLCh* const typeName,
                                        SchemaInfo* const toAdd) {

    if (!fRedefineList) {
        fRedefineList = new RefHash2KeysTableOf<SchemaInfo>(8, false);
    }

    fRedefineList->put((void*) categName, fStringPool->addOrFind(typeName), toAdd);
}

inline SchemaInfo* SchemaInfo::getImportInfo(const unsigned int namespaceURI) const {

    unsigned int importSize = (fImportList) ? fImportList->size() : 0;
    SchemaInfo* currInfo = 0;

    for (unsigned int i=0; i < importSize; i++) {

        currInfo = fImportList->elementAt(i);

        if (currInfo->getTargetNSURI() == (int) namespaceURI)
            break;
    }

    return currInfo;
}

inline SchemaInfo* SchemaInfo::getRedefInfo(const XMLCh* const categName,
                                            const XMLCh* const typeName) {

    if (fRedefineList) {
        return fRedefineList->get(categName, fStringPool->addOrFind(typeName));
    }

    return 0;
}

inline bool SchemaInfo::containsInfo(const SchemaInfo* const toCheck,
                                     const ListType aListType) const {

    if ((aListType == INCLUDE) && fIncludeList) {
        return fIncludeList->containsElement(toCheck);        
    }
    else if ((aListType == IMPORT) && fImportList) {
        return fImportList->containsElement(toCheck);
    }

    return false;
}

inline bool SchemaInfo::circularImportExist(const unsigned int namespaceURI) {

    unsigned int importSize = fImportingList->size();

    for (unsigned int i=0; i < importSize; i++) {
        if (fImportingList->elementAt(i)->getTargetNSURI() == (int) namespaceURI) {
            return true;
        }
    }

    return false;
}

#endif

/**
  * End of file SchemaInfo.hpp
  */

