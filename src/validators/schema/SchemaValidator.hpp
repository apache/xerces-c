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
 * Revision 1.3  2001/04/19 18:17:40  tng
 * Schema: SchemaValidator update, and use QName in Content Model
 *
 * Revision 1.2  2001/03/30 16:35:20  tng
 * Schema: Whitespace normalization.
 *
 * Revision 1.1  2001/03/21 21:56:33  tng
 * Schema: Add Schema Grammar, Schema Validator, and split the DTDValidator into DTDValidator, DTDScanner, and DTDGrammar.
 *
 */



#if !defined(SCHEMAVALIDATOR_HPP)
#define SCHEMAVALIDATOR_HPP

#include <framework/XMLBuffer.hpp>
#include <framework/XMLValidator.hpp>
#include <util/RefVectorOf.hpp>
#include <validators/common/GrammarResolver.hpp>
#include <validators/datatype/DatatypeValidator.hpp>
#include <validators/schema/SchemaElementDecl.hpp>
#include <validators/schema/SchemaGrammar.hpp>

//
//  This is a derivative of the abstract validator interface. This class
//  implements a validator that supports standard XML Schema semantics.
//  This class handles scanning the of the schema, and provides
//  the standard validation services against the Schema info it found.
//
class VALIDATORS_EXPORT SchemaValidator : public XMLValidator
{
public:
    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    SchemaValidator(XMLErrorReporter* const errReporter = 0);
    virtual ~SchemaValidator();

    // -----------------------------------------------------------------------
    //  Setter methods
    // -----------------------------------------------------------------------
    void setXsiTypeAttValue(const XMLCh* const xsiTypeAttValue);
    void setGrammarResolver(GrammarResolver* grammarResolver);

    // -----------------------------------------------------------------------
    //  Implementation of the XMLValidator interface
    // -----------------------------------------------------------------------
    virtual int checkContent
    (
        const   unsigned int    elemId
        , QName** const         children
        , const unsigned int    childCount
    );

    virtual bool checkRootElement
    (
        const   unsigned int    elemId
    );

    virtual void faultInAttr
    (
                XMLAttr&    toFill
        , const XMLAttDef&  attDef
    )   const;

    virtual void preContentValidation(bool reuseGrammar);

    virtual void postParseValidation();

    virtual void reset();

    virtual bool requiresNamespaces() const;

    virtual void validateAttrValue
    (
        const   XMLAttDef&                  attDef
        , const XMLCh* const                attrValue
    );

    virtual Grammar* getGrammar();
    virtual void setGrammar(Grammar* aGrammar);

    // -----------------------------------------------------------------------
    //  Virtual DTD handler interface.
    // -----------------------------------------------------------------------
    virtual bool handlesDTD() const;

    // -----------------------------------------------------------------------
    //  Virtual Schema handler interface. handlesSchema() always return false.
    // -----------------------------------------------------------------------
    virtual bool handlesSchema() const;

    // -----------------------------------------------------------------------
    //  Schema Validator methods
    // -----------------------------------------------------------------------
    void normalizeWhiteSpace(DatatypeValidator* dV, const XMLCh* const value, XMLBuffer& toFill);

private:
    // -----------------------------------------------------------------------
    //  Private data members
    //
    // -----------------------------------------------------------------------
    //  The following comes from the Scanner
    //  fSchemaGrammar
    //      The current schema grammar used by the validator
    //
    //  fGrammarResolver
    //      All the schema grammar stored
    //
    //  fXsiTypeAttValue
    //      Store the Schema Type Attribute Value if schema type is specified
    //
    // -----------------------------------------------------------------------
    //  The following used internally in the validator
    //
    //  fXsiTypeValidator
    //      The validator used for xsi type validation
    //
    //  fCurrentDV
    //      Current DataTypeValidator used by the validator
    //
    //  fDatatypeBuffer
    //      Buffer for simple type element string content
    //
    //  fBufferDatatype
    //      Indicate a simple type element is being scanned, and its
    //      string content should be stored in fDatatypeBuffer
    //
    //  fTrailing
    //      Previous chunk had a trailing space
    // -----------------------------------------------------------------------
    SchemaGrammar* fSchemaGrammar;
    GrammarResolver* fGrammarResolver;
    const XMLCh* fXsiTypeAttValue;

    DatatypeValidator* fXsiTypeValidator;
    DatatypeValidator* fCurrentDV;

    bool fBufferDatatype;
    XMLBuffer fDatatypeBuffer;
    bool fFirstChunk;
    bool fTrailing;
};


// ---------------------------------------------------------------------------
//  SchemaValidator: Setter methods
// ---------------------------------------------------------------------------
inline void SchemaValidator::setXsiTypeAttValue(const XMLCh* const xsiTypeAttValue) {
    fXsiTypeAttValue = xsiTypeAttValue;
}

inline void SchemaValidator::setGrammarResolver(GrammarResolver* grammarResolver) {
    fGrammarResolver = grammarResolver;
}

// ---------------------------------------------------------------------------
//  Virtual interface
// ---------------------------------------------------------------------------
inline Grammar* SchemaValidator::getGrammar() {
    return fSchemaGrammar;
}

inline void SchemaValidator::setGrammar(Grammar* aGrammar) {
    fSchemaGrammar = (SchemaGrammar*) aGrammar;
}

// ---------------------------------------------------------------------------
//  SchemaValidator: DTD handler interface
// ---------------------------------------------------------------------------
inline bool SchemaValidator::handlesDTD() const
{
    // No DTD scanning
    return false;
}

// ---------------------------------------------------------------------------
//  SchemaValidator: Schema handler interface
// ---------------------------------------------------------------------------
inline bool SchemaValidator::handlesSchema() const
{
    return true;
}

#endif
