/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 1999-2000 The Apache Software Foundation.  All rights
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
 * Revision 1.2  2001/03/21 19:30:17  tng
 * Schema: Content Model Updates, by Pei Yong Zhang.
 *
 * Revision 1.1  2001/02/27 18:48:23  tng
 * Schema: Add SchemaAttDef, SchemaElementDecl, SchemaAttDefList.
 *
 */


#if !defined(SCHEMAELEMENTDECL_HPP)
#define SCHEMAELEMENTDECL_HPP

#include <util/RefHash2KeysTableOf.hpp>
#include <util/QName.hpp>
#include <framework/XMLElementDecl.hpp>
#include <validators/schema/SchemaAttDef.hpp>

class ContentSpecNode;
class SchemaAttDefList;
class DatatypeValidator;


//
//  This class is a derivative of the basic element decl. This one implements
//  the virtuals so that they work for a Schema.
//
class VALIDATORS_EXPORT SchemaElementDecl : public XMLElementDecl
{
public :
    // -----------------------------------------------------------------------
    //  Class specific types
    //
    //  ModelTypes
    //      Indicates the type of content model that an element has. This
    //      indicates how the content model is represented and validated.
    // -----------------------------------------------------------------------
    enum ModelTypes
    {
        Empty
        , Any
        , Mixed
        , Children
        , Simple

        , ModelTypes_Count
    };


    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    SchemaElementDecl();

    SchemaElementDecl
    (
        const XMLCh* const       prefix
      , const XMLCh* const       localPart
      , const int                uriId
      , const ModelTypes         modelType = Any
      , const int                enclosingScope = -1
    );

    ~SchemaElementDecl();


    // -----------------------------------------------------------------------
    //  The virtual element decl interface
    // -----------------------------------------------------------------------
    virtual XMLAttDef* findAttr
    (
        const   XMLCh* const    qName
        , const unsigned int    uriId
        , const XMLCh* const    baseName
        , const XMLCh* const    prefix
        , const LookupOpts      options
        ,       bool&           wasAdded
    )   const;
    virtual XMLAttDefList& getAttDefList() const;
    virtual const XMLCh* getBaseName() const;
    virtual const int getURI() const;
    virtual CharDataOpts getCharDataOpts() const;
    virtual const XMLCh* getFullName() const;
    virtual bool hasAttDefs() const;
    virtual bool resetDefs();
    virtual const ContentSpecNode* getContentSpec() const;
    virtual ContentSpecNode* getContentSpec();
    virtual void setContentSpec(ContentSpecNode* toAdopt);


    // -----------------------------------------------------------------------
    //  Getter methods
    // -----------------------------------------------------------------------
    const SchemaAttDef* getAttDef(const XMLCh* const baseName, const int uriId) const;
    SchemaAttDef* getAttDef(const XMLCh* const baseName, const int uriId);
    QName* getElementName() const;
    ModelTypes getModelType() const;
    DatatypeValidator* getDatatypeValidator() const;
    int getEnclosingScope() const;


    // -----------------------------------------------------------------------
    //  Setter methods
    // -----------------------------------------------------------------------
    void addAttDef(SchemaAttDef* const toAdd);
    void setElementName(const XMLCh* const prefix
                      , const XMLCh* const localPart
                      , const int          uriId = -1);
    void setModelType(const SchemaElementDecl::ModelTypes toSet);
    void setDatatypeValidator(DatatypeValidator* newDatatypeValidator);
    void setEnclosingScope(const int enclosingScope);


protected :
    // -----------------------------------------------------------------------
    //  Protected, virtual methods
    // -----------------------------------------------------------------------
    virtual XMLContentModel* makeContentModel(XMLValidator* pValidator=0) const;
    virtual XMLCh* formatContentModel
    (
        const   XMLValidator&   validator
    )   const;


private :
    // -----------------------------------------------------------------------
    //  Private helper methods
    // -----------------------------------------------------------------------
    XMLContentModel* createChildModel(XMLValidator* pValidator=0) const;
    void faultInAttDefList() const;


    // -----------------------------------------------------------------------
    //  Private data members
    //
    //  fAttDefs
    //      The list of attributes that are defined for this element. Each
    //      element is its own little 'namespace' for attributes, so each
    //      element maintains its own list of owned attribute defs. It is
    //      faulted in when an attribute is actually added.
    //
    //  fAttList
    //      We have to return a view of our att defs via the abstract view
    //      that the scanner understands. It may or may not ever be asked
    //      for so we fault it in as needed.
    //
    //  fContentSpec
    //      This is the content spec for the node. It contains the original
    //      content spec that was read from the Schema, as a tree of nodes. This
    //      one is always set up, and is used to build the fContentModel
    //      version if we are validating.
    //
    //  fElementName
    //      This is the name of the element decl.
    //
    //  fModelType
    //      The content model type of this element. This tells us what kind
    //      of content model to create.
    //
    //  fDatatypeValidator
    //      The DatatypeValidator used to validate this element type.
    //
    //  fEnclosingScope
    //      The enclosing scope where this element is declared.
    // -----------------------------------------------------------------------
    RefHash2KeysTableOf<SchemaAttDef>*  fAttDefs;
    SchemaAttDefList*              fAttList;
    ContentSpecNode*               fContentSpec;
    QName*                         fElementName;
    ModelTypes                     fModelType;
    DatatypeValidator*             fDatatypeValidator;
    int                            fEnclosingScope;
};

// ---------------------------------------------------------------------------
//  SchemaElementDecl: XMLElementDecl virtual interface implementation
// ---------------------------------------------------------------------------
inline const int SchemaElementDecl::getURI() const
{
    return fElementName->getURI();
}

inline const XMLCh* SchemaElementDecl::getFullName() const
{
    return fElementName->getRawName();
}

inline ContentSpecNode* SchemaElementDecl::getContentSpec()
{
    return fContentSpec;
}

inline const ContentSpecNode* SchemaElementDecl::getContentSpec() const
{
    return fContentSpec;
}

// ---------------------------------------------------------------------------
//  SchemaElementDecl: Getter methods
// ---------------------------------------------------------------------------
inline QName* SchemaElementDecl::getElementName() const
{
    return fElementName;
}

inline SchemaElementDecl::ModelTypes SchemaElementDecl::getModelType() const
{
    return fModelType;
}

inline DatatypeValidator* SchemaElementDecl::getDatatypeValidator() const
{
    return fDatatypeValidator;
}

inline int SchemaElementDecl::getEnclosingScope() const
{
    return fEnclosingScope;
}

// ---------------------------------------------------------------------------
//  SchemaElementDecl: Setter methods
// ---------------------------------------------------------------------------
inline void SchemaElementDecl::setElementName(const XMLCh* const        prefix
                            , const XMLCh* const        localPart
                            , const int                 uriId )
{
   fElementName->setName(prefix, localPart, uriId);
}

inline void
SchemaElementDecl::setModelType(const SchemaElementDecl::ModelTypes toSet)
{
    fModelType = toSet;
}

inline void SchemaElementDecl::setDatatypeValidator(DatatypeValidator* newDatatypeValidator)
{
    fDatatypeValidator = newDatatypeValidator;
}

inline void SchemaElementDecl::setEnclosingScope(const int newEnclosingScope)
{
    fEnclosingScope = newEnclosingScope;
}

#endif
