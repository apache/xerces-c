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
 * Revision 1.9  2001/07/24 18:33:46  knoaman
 * Added support for <group> + extra constraint checking for complexType
 *
 * Revision 1.8  2001/06/13 20:51:15  peiyongz
 * fIsMixed: to handle mixed Content Model
 *
 * Revision 1.7  2001/05/11 13:27:36  tng
 * Copyright update.
 *
 * Revision 1.6  2001/05/03 20:34:43  tng
 * Schema: SchemaValidator update
 *
 * Revision 1.5  2001/05/03 19:18:01  knoaman
 * TraverseSchema Part II.
 *
 * Revision 1.4  2001/04/19 17:43:17  knoaman
 * More schema implementation classes.
 *
 * Revision 1.3  2001/03/21 21:56:33  tng
 * Schema: Add Schema Grammar, Schema Validator, and split the DTDValidator into DTDValidator, DTDScanner, and DTDGrammar.
 *
 * Revision 1.2  2001/03/21 19:30:17  tng
 * Schema: Content Model Updates, by Pei Yong Zhang.
 *
 * Revision 1.1  2001/02/27 18:48:23  tng
 * Schema: Add SchemaAttDef, SchemaElementDecl, SchemaAttDefList.
 *
 */


#if !defined(SCHEMAELEMENTDECL_HPP)
#define SCHEMAELEMENTDECL_HPP

#include <util/QName.hpp>
#include <validators/common/Grammar.hpp>
#include <validators/schema/ComplexTypeInfo.hpp>

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
      , const int                enclosingScope = Grammar::TOP_LEVEL_SCOPE
    );

    SchemaElementDecl
    (
        const QName* const       elementName
      , const ModelTypes         modelType = Any
      , const int                enclosingScope = Grammar::TOP_LEVEL_SCOPE
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
    virtual CharDataOpts getCharDataOpts() const;
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
    ModelTypes getModelType() const;
    DatatypeValidator* getDatatypeValidator() const;
    int getEnclosingScope() const;
    int getDefinedScope() const;
    int getFinalSet() const;
    int getBlockSet() const;
    int getMiscFlags() const;
    XMLCh* getDefaultValue() const;
    XMLCh* getSubstitutionGroupName() const;
    XMLCh* getTypeFromAnotherSchemaURI() const;
    ComplexTypeInfo* getComplexTypeInfo() const;


    // -----------------------------------------------------------------------
    //  Setter methods
    // -----------------------------------------------------------------------
    void setModelType(const SchemaElementDecl::ModelTypes toSet);
    void setDatatypeValidator(DatatypeValidator* newDatatypeValidator);
    void setEnclosingScope(const int enclosingScope);
    void setDefinedScope(const int definedScope);
    void setFinalSet(const int finalSet);
    void setBlockSet(const int blockSet);
    void setMiscFlags(const int flags);
    void setDefaultValue(const XMLCh* const value);
    void setSubstitutionGroupName(const XMLCh* const name);
    void setTypeFromAnotherSchemaURI(const XMLCh* const uriStr);
    void setComplexTypeInfo(ComplexTypeInfo* const typeInfo);


protected :
    // -----------------------------------------------------------------------
    //  Protected, virtual methods
    // -----------------------------------------------------------------------
    virtual XMLContentModel* makeContentModel() ;
    virtual XMLCh* formatContentModel()   const;


private :
    // -----------------------------------------------------------------------
    //  Private helper methods
    // -----------------------------------------------------------------------
    XMLContentModel* createChildModel(const bool isMixed) ;
    void faultInAttDefList() const;


    // -----------------------------------------------------------------------
    //  Private data members
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
    //
    //  fDefinedScope
    //      The Complex Element that this element belongs to.
    //
    //  fFinalSet
    //      The value set of the 'final' attribute.
    //
    //  fBlockSet
    //      The value set of the 'block' attribute.
    //
    //  fMiscFlags
    //      Stores 'abstract/nullable' values
    //
    //  fDefaultValue
    //      The defalut/fixed value
    //
    //  fSubstitutionGroupName
    //      The substitution group full name ("uristring','local")
    //
    //  fTypeFromAnotherSchemaURI
    //      The URI of type if it belongs to a different schema
    //
    //  fComplexTypeInfo
    //      Stores complex type information
    //      (no need to delete - handled by schema grammar)
    //
    //  fAttDefs
    //      The list of attributes that are faulted in for this element
    //      when ComplexTypeInfo does not exist.  We want to keep track
    //      of these faulted in attributes to avoid duplicate redundant
    //      error.

    // -----------------------------------------------------------------------
    ModelTypes                     fModelType;
    DatatypeValidator*             fDatatypeValidator;
    int                            fEnclosingScope;
    int                            fDefinedScope;
    int                            fFinalSet;
    int                            fBlockSet;
    int                            fMiscFlags;
    XMLCh*                         fDefaultValue;
    XMLCh*                         fSubstitutionGroupName;
    XMLCh*                         fTypeFromAnotherSchemaURI;
    ComplexTypeInfo*               fComplexTypeInfo;
    RefHash2KeysTableOf<SchemaAttDef>* fAttDefs;
};

// ---------------------------------------------------------------------------
//  SchemaElementDecl: XMLElementDecl virtual interface implementation
// ---------------------------------------------------------------------------
inline ContentSpecNode* SchemaElementDecl::getContentSpec()
{
    if (fComplexTypeInfo != 0) {
        return fComplexTypeInfo->getContentSpec();
    }

	return 0;
}

inline const ContentSpecNode* SchemaElementDecl::getContentSpec() const
{
    if (fComplexTypeInfo != 0) {
        return fComplexTypeInfo->getContentSpec();
    }

	return 0;
}

inline void
SchemaElementDecl::setContentSpec(ContentSpecNode* toAdopt)
{
    //Handled by complexType
}

// ---------------------------------------------------------------------------
//  SchemaElementDecl: Getter methods
// ---------------------------------------------------------------------------
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

inline int SchemaElementDecl::getDefinedScope() const
{
    return fDefinedScope;
}

inline int SchemaElementDecl::getFinalSet() const
{
    return fFinalSet;
}

inline int SchemaElementDecl::getBlockSet() const
{
    return fBlockSet;
}

inline int SchemaElementDecl::getMiscFlags() const
{
    return fMiscFlags;
}

inline XMLCh* SchemaElementDecl::getDefaultValue() const
{
    return fDefaultValue;
}

inline XMLCh* SchemaElementDecl::getSubstitutionGroupName() const
{

    return fSubstitutionGroupName;
}

inline XMLCh* SchemaElementDecl::getTypeFromAnotherSchemaURI() const {

    return fTypeFromAnotherSchemaURI;
}

inline ComplexTypeInfo* SchemaElementDecl::getComplexTypeInfo() const
{
    return fComplexTypeInfo;
}

// ---------------------------------------------------------------------------
//  SchemaElementDecl: Setter methods
// ---------------------------------------------------------------------------
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

inline void SchemaElementDecl::setDefinedScope(const int newDefinedScope)
{
    fDefinedScope = newDefinedScope;
}

inline void SchemaElementDecl::setFinalSet(const int finalSet)
{
    fFinalSet = finalSet;
}

inline void SchemaElementDecl::setBlockSet(const int blockSet)
{
    fBlockSet = blockSet;
}

inline void SchemaElementDecl::setMiscFlags(const int flags)
{
    fMiscFlags = flags;
}

inline void SchemaElementDecl::setDefaultValue(const XMLCh* const value)
{
    if (fDefaultValue) {
        delete[] fDefaultValue;
    }

    fDefaultValue = XMLString::replicate(value);
}

inline void SchemaElementDecl::setSubstitutionGroupName(const XMLCh* const name)
{
    if (fSubstitutionGroupName) {
        delete [] fSubstitutionGroupName;
    }

    fSubstitutionGroupName = XMLString::replicate(name);
}

inline void
SchemaElementDecl::setTypeFromAnotherSchemaURI(const XMLCh* const uriStr) {

    if (fTypeFromAnotherSchemaURI) {
        delete [] fTypeFromAnotherSchemaURI;
    }

    fTypeFromAnotherSchemaURI = XMLString::replicate(uriStr);
}

inline void
SchemaElementDecl::setComplexTypeInfo(ComplexTypeInfo* const typeInfo)
{
    fComplexTypeInfo = typeInfo;
}

#endif
