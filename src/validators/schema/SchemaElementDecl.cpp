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
 * Revision 1.12  2001/08/21 16:06:11  tng
 * Schema: Unique Particle Attribution Constraint Checking.
 *
 * Revision 1.11  2001/07/24 18:33:46  knoaman
 * Added support for <group> + extra constraint checking for complexType
 *
 * Revision 1.10  2001/07/09 15:22:43  knoaman
 * complete <any> declaration.
 *
 * Revision 1.9  2001/06/13 20:51:18  peiyongz
 * fIsMixed: to handle mixed Content Model
 *
 * Revision 1.8  2001/05/11 13:27:35  tng
 * Copyright update.
 *
 * Revision 1.7  2001/05/03 20:34:42  tng
 * Schema: SchemaValidator update
 *
 * Revision 1.6  2001/05/03 19:17:59  knoaman
 * TraverseSchema Part II.
 *
 * Revision 1.5  2001/04/19 17:43:16  knoaman
 * More schema implementation classes.
 *
 * Revision 1.4  2001/03/21 21:56:33  tng
 * Schema: Add Schema Grammar, Schema Validator, and split the DTDValidator into DTDValidator, DTDScanner, and DTDGrammar.
 *
 * Revision 1.3  2001/03/21 19:30:15  tng
 * Schema: Content Model Updates, by Pei Yong Zhang.
 *
 * Revision 1.2  2001/03/07 17:42:12  tng
 * fix typo
 *
 * Revision 1.1  2001/02/27 18:48:22  tng
 * Schema: Add SchemaAttDef, SchemaElementDecl, SchemaAttDefList.
 *
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <util/XMLString.hpp>
#include <util/XMLUniDefs.hpp>
#include <util/XMLUni.hpp>
#include <validators/schema/SchemaAttDefList.hpp>
#include <validators/schema/SchemaElementDecl.hpp>

// ---------------------------------------------------------------------------
//  SchemaElementDecl: Constructors and Destructor
// ---------------------------------------------------------------------------
SchemaElementDecl::SchemaElementDecl() :

    fModelType(Any)
    , fDatatypeValidator(0)
    , fEnclosingScope(Grammar::TOP_LEVEL_SCOPE)
    , fBlockSet(0)
    , fFinalSet(0)
    , fMiscFlags(0)
    , fDefaultValue(0)
    , fSubstitutionGroupName(0)
    , fTypeFromAnotherSchemaURI(0)
    , fComplexTypeInfo(0)
    , fAttDefs(0)
{
}

SchemaElementDecl::SchemaElementDecl(const XMLCh* const                  prefix
                                   , const XMLCh* const                  localPart
                                   , const int                           uriId
                                   , const SchemaElementDecl::ModelTypes type
                                   , const int                           enclosingScope
                                   ) :
    fModelType(type)
    , fDatatypeValidator(0)
    , fEnclosingScope(enclosingScope)
    , fBlockSet(0)
    , fFinalSet(0)
    , fMiscFlags(0)
    , fDefaultValue(0)
    , fSubstitutionGroupName(0)
    , fTypeFromAnotherSchemaURI(0)
    , fComplexTypeInfo(0)
    , fAttDefs(0)
{
    setElementName(prefix, localPart, uriId);
}

SchemaElementDecl::SchemaElementDecl(const QName* const                  elementName
                                   , const SchemaElementDecl::ModelTypes type
                                   , const int                           enclosingScope
                                   ) :
    fModelType(type)
    , fDatatypeValidator(0)
    , fEnclosingScope(enclosingScope)
    , fBlockSet(0)
    , fFinalSet(0)
    , fMiscFlags(0)
    , fDefaultValue(0)
    , fSubstitutionGroupName(0)
    , fTypeFromAnotherSchemaURI(0)
    , fComplexTypeInfo(0)
    , fAttDefs(0)
{
    setElementName(elementName);
}

SchemaElementDecl::~SchemaElementDecl()
{
    delete [] fDefaultValue;
    delete [] fSubstitutionGroupName;
    delete [] fTypeFromAnotherSchemaURI;
    delete fAttDefs;
}


// ---------------------------------------------------------------------------
//  SchemaElementDecl: XMLElementDecl virtual interface implementation
// ---------------------------------------------------------------------------
XMLAttDef* SchemaElementDecl::findAttr(const XMLCh* const    qName
                                     , const unsigned int    uriId
                                     , const XMLCh* const    baseName
                                     , const XMLCh* const    prefix
                                     , const LookupOpts      options
                                     , bool&           wasAdded) const
{
    if (fComplexTypeInfo == 0) {
        if (options == XMLElementDecl::AddIfNotFound) {
            SchemaAttDef* retVal = 0;

            // If no att list exist yet, then create one
            if (!fAttDefs) {
                // Use a hash modulus of 29 and tell it owns its elements
                ((SchemaElementDecl*)this)->fAttDefs = new RefHash2KeysTableOf<SchemaAttDef>(29, true);
            }

            retVal = fAttDefs->get(baseName, uriId);

            // Fault it in if not found and ask to add it
            if (!retVal)
            {
                // And add a default attribute for this name
                retVal = new SchemaAttDef(prefix, baseName, uriId);
                retVal->setElemId(getId());
                fAttDefs->put((void*)baseName, uriId, retVal);

                wasAdded = true;
            }
             else
            {
                wasAdded = false;
            }
            return retVal;
        }
        else {
            wasAdded = false;
            return 0;
        }
    }

    return fComplexTypeInfo->findAttr(qName, uriId, baseName, prefix, options, wasAdded);
}


XMLAttDefList& SchemaElementDecl::getAttDefList() const
{
    if (fComplexTypeInfo == 0) {
        throw; // REVISIT: add proper error message
    }

	return fComplexTypeInfo->getAttDefList();
}


XMLElementDecl::CharDataOpts SchemaElementDecl::getCharDataOpts() const
{
    XMLElementDecl::CharDataOpts retVal;
    switch(fModelType)
    {
        case Children :
            retVal = XMLElementDecl::SpacesOk;
            break;

        case Empty :
            retVal = XMLElementDecl::NoCharData;
            break;

        default :
            retVal = XMLElementDecl::AllCharData;
            break;
    }
    return retVal;
}


bool SchemaElementDecl::hasAttDefs() const
{
    // If the collection hasn't been faulted in, then no att defs
    if (fComplexTypeInfo == 0)
        return false;

    return fComplexTypeInfo->hasAttDefs();
}


bool SchemaElementDecl::resetDefs()
{
    if (fComplexTypeInfo == 0) {
        return false;
    }

    return fComplexTypeInfo->resetDefs();
}

const XMLCh*
SchemaElementDecl::getFormattedContentModel() const
{
    if (fComplexTypeInfo != 0) {
        return fComplexTypeInfo->getFormattedContentModel();
    }
    return 0; 
}  

// ---------------------------------------------------------------------------
//  SchemaElementDecl: Getter methods
// ---------------------------------------------------------------------------
const SchemaAttDef* SchemaElementDecl::getAttDef(const XMLCh* const baseName, const int uriId) const
{
    // If no complex type, then return a null
    if (fComplexTypeInfo == 0)
        return 0;

    return fComplexTypeInfo->getAttDef(baseName, uriId);
}

SchemaAttDef* SchemaElementDecl::getAttDef(const XMLCh* const baseName, const int uriId)
{
    // If no complex type, then return a null
    if (fComplexTypeInfo == 0)
        return 0;

    return fComplexTypeInfo->getAttDef(baseName, uriId);
}

