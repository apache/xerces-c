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
 * Revision 1.1  2001/03/21 21:56:27  tng
 * Schema: Add Schema Grammar, Schema Validator, and split the DTDValidator into DTDValidator, DTDScanner, and DTDGrammar.
 *
 */



#if !defined(GRAMMAR_HPP)
#define GRAMMAR_HPP

#include <framework/XMLElementDecl.hpp>
#include <framework/XMLEntityDecl.hpp>
#include <framework/XMLNotationDecl.hpp>

//
// This abstract class specifies the interface for a Grammar
//

class VALIDATORS_EXPORT Grammar
{
public:
    enum GrammarType {
        DTDGrammarType
      , SchemaGrammarType
    };

    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    virtual ~Grammar(){};

    // -----------------------------------------------------------------------
    //  Virtual Getter methods
    // -----------------------------------------------------------------------
    virtual GrammarType getGrammarType()=0;

    // Element Decl
    virtual XMLElementDecl* findOrAddElemDecl
    (
        const   unsigned int    uriId
        , const XMLCh* const    baseName
        , const XMLCh* const    prefixName
        , const XMLCh* const    qName
        , unsigned int          scope
        ,       bool&           wasAdded
    ) = 0;

    virtual unsigned int getElemId
    (
        const   unsigned int    uriId
        , const XMLCh* const    baseName
        , const XMLCh* const    qName
        , unsigned int          scope
    )   const = 0;

    virtual const XMLElementDecl* getElemDecl
    (
        const   unsigned int    uriId
        , const XMLCh* const    baseName
        , const XMLCh* const    qName
        , unsigned int          scope
    )   const = 0;

    virtual XMLElementDecl* getElemDecl
    (
        const   unsigned int    uriId
        , const XMLCh* const    baseName
        , const XMLCh* const    qName
        , unsigned int          scope
    ) = 0;

    virtual const XMLElementDecl* getElemDecl
    (
        const   unsigned int    elemId
    )   const = 0;

    virtual XMLElementDecl* getElemDecl
    (
        const   unsigned int    elemId
    ) = 0;

    // Entity Decl
    virtual const XMLEntityDecl* getEntityDecl
    (
        const   XMLCh* const    entName
    )   const=0;

    virtual XMLEntityDecl* getEntityDecl
    (
        const   XMLCh* const    entName
    )=0;

    // Notation
    virtual const XMLNotationDecl* getNotationDecl
    (
        const   XMLCh* const    notName
    )   const=0;

    virtual XMLNotationDecl* getNotationDecl
    (
        const   XMLCh* const    notName
    )=0;

    // -----------------------------------------------------------------------
    //  Virtual Setter methods
    // -----------------------------------------------------------------------
    virtual unsigned int putElemDecl
    (
        XMLElementDecl* const elemDecl
    )   const = 0;

    virtual unsigned int putEntityDecl
    (
        XMLEntityDecl* const entityDecl
    )   const = 0;

    virtual unsigned int putNotationDecl
    (
        XMLNotationDecl* const notationDecl
    )   const=0;

    // -----------------------------------------------------------------------
    //  Virtual methods
    // -----------------------------------------------------------------------
    virtual void reset()=0;

protected :
    // -----------------------------------------------------------------------
    //  Hidden constructors
    // -----------------------------------------------------------------------
    Grammar() {};
};

#endif
