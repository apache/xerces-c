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

class SchemaInfo
{
public:
    // -----------------------------------------------------------------------
    //  Constructor/Destructor
    // -----------------------------------------------------------------------
    SchemaInfo(const bool elemDefaultQualified,
               const bool attrDefaultQualified,
               const int blockDefault,
               const int finalDefault,
               XMLCh* const schemaURL,
               const DOM_Element& root,
               SchemaInfo* const nextRoot,
               SchemaInfo* const prevRoot);
    ~SchemaInfo();


	// -----------------------------------------------------------------------
    //  Getter methods
    // -----------------------------------------------------------------------
    SchemaInfo* getNext() const;
    SchemaInfo* getPrev() const;
    XMLCh*      getCurrentSchemaURL() const;
    DOM_Element getRoot() const;
    int         getBlockDefault() const;
    int         getFinalDefault() const;
    bool        isElementDefaultQualified() const;
    bool        isAttributeDefaultQualified() const;

	// -----------------------------------------------------------------------
    //  Setter methods
    // -----------------------------------------------------------------------
    void setNext(SchemaInfo* const nextInfo);

private:
    // -----------------------------------------------------------------------
    //  Private data members
    // -----------------------------------------------------------------------
    bool               fElementDefaultQualified;
    bool               fAttributeDefaultQualified;
    int                fBlockDefault;
    int                fFinalDefault;
    XMLCh*             fCurrentSchemaURL;
    DOM_Element        fSchemaRootElement;
    SchemaInfo*        fNext;
    SchemaInfo*        fPrev;
};

// ---------------------------------------------------------------------------
//  SchemaInfo: Getter methods
// ---------------------------------------------------------------------------
inline bool SchemaInfo::isAttributeDefaultQualified() const {

    return fAttributeDefaultQualified;
}


inline bool SchemaInfo::isElementDefaultQualified() const {

    return fElementDefaultQualified;
}

inline int SchemaInfo::getBlockDefault() const {

    return fBlockDefault;
}

inline int SchemaInfo::getFinalDefault() const {

    return fFinalDefault;
}

inline XMLCh* SchemaInfo::getCurrentSchemaURL() const {

    return fCurrentSchemaURL;
}

inline DOM_Element SchemaInfo::getRoot() const {

    return fSchemaRootElement;
}

inline SchemaInfo* SchemaInfo::getNext() const {

    return fNext;
}

inline SchemaInfo* SchemaInfo::getPrev() const {

    return fPrev;
}


// ---------------------------------------------------------------------------
//  SchemaInfo: Setter methods
// ---------------------------------------------------------------------------
inline void SchemaInfo::setNext(SchemaInfo* const nextInfo) {

    fNext = nextInfo;
}

#endif

/**
  * End of file SchemaInfo.hpp
  */

