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
 * Revision 1.3  2001/04/19 18:17:39  tng
 * Schema: SchemaValidator update, and use QName in Content Model
 *
 * Revision 1.2  2001/03/30 16:35:19  tng
 * Schema: Whitespace normalization.
 *
 * Revision 1.1  2001/03/21 21:56:33  tng
 * Schema: Add Schema Grammar, Schema Validator, and split the DTDValidator into DTDValidator, DTDScanner, and DTDGrammar.
 *
 */

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <util/KVStringPair.hpp>
#include <internal/XMLReader.hpp>
#include <internal/XMLScanner.hpp>
#include <validators/datatype/InvalidDatatypeValueException.hpp>
#include <validators/schema/SchemaValidator.hpp>

// ---------------------------------------------------------------------------
//  SchemaValidator: Constructors and Destructor
// ---------------------------------------------------------------------------
SchemaValidator::SchemaValidator(XMLErrorReporter* const errReporter) :

    XMLValidator(errReporter)
    , fGrammarResolver(0)
    , fTrailing(false)
    , fBufferDatatype(0)
{
}

SchemaValidator::~SchemaValidator()
{
}

// ---------------------------------------------------------------------------
//  SchemaValidator: Implementation of the XMLValidator interface
// ---------------------------------------------------------------------------
int SchemaValidator::checkContent (const   unsigned int  elemId
                                 , QName** const         children
                                 , const unsigned int    childCount)
{
    //
    //  Look up the element id in our element decl pool. This will get us
    //  the element decl in our own way of looking at them.
    //
    SchemaElementDecl* elemDecl = (SchemaElementDecl*) fSchemaGrammar->getElemDecl(elemId);
    if (!elemDecl)
        ThrowXML(RuntimeException, XMLExcepts::Val_InvalidElemId);

    //
    //  Get the content spec type of this element. This will tell us what
    //  to do to validate it.
    //
    const SchemaElementDecl::ModelTypes modelType = elemDecl->getModelType();

    if (modelType == SchemaElementDecl::Empty)
    {
        //
        //  We can do this one here. It cannot have any children. If it does
        //  we return 0 as the index of the first bad child.
        //
        if (childCount)
            return 0;
    }
     else if (modelType == SchemaElementDecl::Any)
    {
        // We pass no judgement on this one, anything goes
    }
     else if ((modelType == SchemaElementDecl::Mixed)
          ||  (modelType == SchemaElementDecl::Children))
    {
        // Get the element's content model or fault it in
        const XMLContentModel* elemCM = elemDecl->getContentModel();

        // Ask it to validate and return its return
        int result = elemCM->validateContent(children, childCount, getScanner()->getEmptyNamespaceId());
        if (result != -1) {
//            SubstitutionGroupComparator comparator(fGrammarResolver, fStringPool);
//            elemCM->setSubstitutionGroupComparator(comparator);
            result = elemCM->validateContentSpecial(children, childCount, getScanner()->getEmptyNamespaceId());
        }
        return result;
    }
     else if (modelType == SchemaElementDecl::Simple)
    {
         if (childCount > 0) {
             emitError(XMLValid::SimpleTypeHasChild, elemDecl->getFullName());
         } else {
             try {
                 if (!fCurrentDV) {
                     //no character data
                     fCurrentDV = elemDecl->getDatatypeValidator();
                 }

                 // If there is xsi:type validator, substitute it.
                if (!fXsiTypeValidator) {
                   fCurrentDV = fXsiTypeValidator;
                   fXsiTypeValidator = 0;
                }
                if (!fCurrentDV) {
                    emitError(XMLValid::NoDatatypeValidatorForSimpleType, elemDecl->getFullName());
                } else {
                    XMLCh* value =fDatatypeBuffer.getRawBuffer();
                    fCurrentDV->validate(value);
                }
            } catch (InvalidDatatypeValueException idve) {
                emitError (XMLValid::DatatypeError, idve.getType(), idve.getMessage());
            }
            fCurrentDV = 0;
            fBufferDatatype=false;
            fDatatypeBuffer.reset();
         }
    }
     else
    {
        ThrowXML(RuntimeException, XMLExcepts::CM_UnknownCMType);
    }

    fTrailing=false;

    // Went ok, so return success
    return -1;
}

bool SchemaValidator::checkRootElement (const   unsigned int    elemId)
{
	return false;
}

void SchemaValidator::faultInAttr (XMLAttr&    toFill, const XMLAttDef&  attDef)   const
{
}

void SchemaValidator::preContentValidation(bool reuseGrammar)
{
}

void SchemaValidator::postParseValidation()
{
}

void SchemaValidator::reset()
{
}

bool SchemaValidator::requiresNamespaces() const
{
    return true;
}

void SchemaValidator::validateAttrValue (const   XMLAttDef& attDef
                                       , const XMLCh* const attrValue)
{
}

// ---------------------------------------------------------------------------
//  SchemaValidator: Validator method
// ---------------------------------------------------------------------------
// Do Schema Normalization depends on the WhiteSpace Facet
// preserve : No normalization is done
// replace  : All occurrences of #x9 (tab), #xA (linefeed) and #xD (carriage return)
//            are replaced with #x20 (space).
// collapse : Subsequent to the replacements specified above under replace,
//            contiguous sequences of #x20s are collapsed to a single #x20,
//            and initial and/or final #x20s are deleted.
//
void SchemaValidator::normalizeWhiteSpace(DatatypeValidator* dV, const XMLCh* const value, XMLBuffer& toFill)
{
    fCurrentDV = dV;
    short fWhiteSpace = DatatypeValidator::PRESERVE;
    if (fCurrentDV)
        fWhiteSpace = fCurrentDV->getWSFacet();

    enum States
    {
        InWhitespace
        , InContent
    };

    States curState = InContent;
    toFill.reset();

    //
    //  Loop through the chars of the source value and normalize it according
    //  to the whitespace facet
    //
    bool firstNonWS = false;
    XMLCh nextCh;
    const XMLCh* srcPtr = value;

    if ((fWhiteSpace==DatatypeValidator::COLLAPSE) && fTrailing)
        toFill.append(chSpace);

    while (*srcPtr)
    {
        nextCh = *srcPtr;

        if (fWhiteSpace == DatatypeValidator::PRESERVE)
        {
            // do nothing
            break;
        }
        else if (fWhiteSpace == DatatypeValidator::REPLACE)
        {
            if (XMLReader::isWhitespace(nextCh))
                nextCh = chSpace;
        }
        else // COLLAPSE case
        {
            if (curState == InWhitespace)
            {
                if (!XMLReader::isWhitespace(nextCh))
                {
                    if (firstNonWS)
                        toFill.append(chSpace);
                    curState = InContent;
                    firstNonWS = true;
                }
                 else
                {
                    srcPtr++;
                    continue;
                }
            }
             else if (curState == InContent)
            {
                if (XMLReader::isWhitespace(nextCh))
                {
                    curState = InWhitespace;
                    srcPtr++;
                    continue;
                }
                firstNonWS = true;
            }
        }
        // Add this char to the target buffer
        toFill.append(nextCh);

        // stored the content if we are in simple type element string content
        if (fBufferDatatype)
            fDatatypeBuffer.append(nextCh);

        // And move up to the next character in the source
        srcPtr++;
    }
    srcPtr--;
    nextCh = *srcPtr;
    if (XMLReader::isWhitespace(nextCh))
        fTrailing = true;
}
