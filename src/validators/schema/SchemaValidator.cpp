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
 * Revision 1.9  2001/07/11 21:39:58  peiyongz
 * fix to normalizeWhiteSpace: synchronize fDatatypeBuffer with toFill.
 *
 * Revision 1.8  2001/05/17 18:14:32  tng
 * Schema Fix: if nillable, it's an error to have default value
 *
 * Revision 1.7  2001/05/11 15:17:46  tng
 * Schema: Nillable fixes.
 *
 * Revision 1.6  2001/05/11 13:27:37  tng
 * Copyright update.
 *
 * Revision 1.5  2001/05/10 17:49:42  tng
 * Schema: SchemaValidator fixes
 *
 * Revision 1.4  2001/05/03 20:34:44  tng
 * Schema: SchemaValidator update
 *
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
#include <util/Janitor.hpp>
#include <util/KVStringPair.hpp>
#include <framework/XMLDocumentHandler.hpp>
#include <internal/XMLReader.hpp>
#include <internal/XMLScanner.hpp>
#include <validators/datatype/InvalidDatatypeValueException.hpp>
#include <validators/datatype/InvalidDatatypeFacetException.hpp>
#include <validators/schema/SchemaSymbols.hpp>
#include <validators/schema/SchemaValidator.hpp>
#include <validators/schema/SubstitutionGroupComparator.hpp>

// ---------------------------------------------------------------------------
//  SchemaValidator: Constructors and Destructor
// ---------------------------------------------------------------------------
SchemaValidator::SchemaValidator(XMLErrorReporter* const errReporter) :

    XMLValidator(errReporter)
    , fGrammarResolver(0)
    , fSchemaGrammar(0)
    , fTrailing(false)
    , fXsiType(0)
    , fXsiTypeValidator(0)
    , fNil(false)
{
}

SchemaValidator::~SchemaValidator()
{
    delete fXsiType;
}

// ---------------------------------------------------------------------------
//  SchemaValidator: Implementation of the XMLValidator interface
// ---------------------------------------------------------------------------
int SchemaValidator::checkContent (XMLElementDecl* const elemDecl
                                 , QName** const         children
                                 , const unsigned int    childCount)
{
    //
    //  Look up the element id in our element decl pool. This will get us
    //  the element decl in our own way of looking at them.
    //
    if (!elemDecl)
        ThrowXML(RuntimeException, XMLExcepts::Val_InvalidElemId);

    //
    //  Get the content spec type of this element. This will tell us what
    //  to do to validate it.
    //
    const SchemaElementDecl::ModelTypes modelType = ((SchemaElementDecl*) elemDecl)->getModelType();

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
        // if nillable, it's an error to have value
        // XML Schema REC: Validation Rule: Element Locally Valid (Element)
        // 3.2.1 The element information item must have no
        // character or element information item [children].
        //
        if (fNil) {
            if (childCount > 0 || XMLString::compareString(fDatatypeBuffer.getRawBuffer(), XMLUni::fgZeroLenString))
                emitError(XMLValid::NilAttrNotEmpty, elemDecl->getFullName());

        }
        else {
            // Get the element's content model or fault it in
            XMLContentModel* elemCM = elemDecl->getContentModel();

            // Ask it to validate and return its return
            int result = elemCM->validateContent(children, childCount, getScanner()->getEmptyNamespaceId());
            if (result != -1) {
                result = elemCM->validateContentSpecial(children
                                                      , childCount
                                                      , getScanner()->getEmptyNamespaceId()
                                                      , fGrammarResolver
                                                      , getScanner()->getURIStringPool());
            }
            return result;
        }
    }
    else if (modelType == SchemaElementDecl::Simple)
    {
        if (childCount > 0) {
            emitError(XMLValid::SimpleTypeHasChild, elemDecl->getFullName());
        } else {
            try {
                DatatypeValidator* fCurrentDV = ((SchemaElementDecl*)elemDecl)->getDatatypeValidator();
                // If there is xsi:type validator, substitute it.
                if (fXsiTypeValidator) {
                    fCurrentDV = fXsiTypeValidator;
                    fXsiTypeValidator = 0;
                }
                if (!fCurrentDV) {
                    emitError(XMLValid::NoDatatypeValidatorForSimpleType, elemDecl->getFullName());
                } else {
                    XMLCh* value = fDatatypeBuffer.getRawBuffer();
                    XMLCh* elemDefaultValue = ((SchemaElementDecl*) elemDecl)->getDefaultValue();

                    if (elemDefaultValue) {
                        // a default value was specified

                        // if nillable, it's an error to have default value
                        if (fNil)
                            emitError(XMLValid::NilAttrNotEmpty, elemDecl->getFullName());

                        if (!XMLString::compareString(value, XMLUni::fgZeroLenString)) {
                            // if this element didn't specified any value
                            // use default value
                            getScanner()->getDocHandler()->docCharacters(elemDefaultValue, XMLString::stringLen(elemDefaultValue), false);
                        }
                        else {
                            // this element has specified some value
                            // if the flag is FIXED, then this value must be same as default value
                            if ((((SchemaElementDecl*)elemDecl)->getMiscFlags() == SchemaSymbols::FIXED) != 0) {
                                if (fCurrentDV->compare(value, elemDefaultValue) != 0 )
                                    emitError(XMLValid::FixedDifferentFromActual, elemDecl->getFullName());
                            }

                            // if nillable, it's an error to have value
                            if (fNil)
                                emitError(XMLValid::NilAttrNotEmpty, elemDecl->getFullName());
                            else
                               fCurrentDV->validate(value);
                        }
                    }
                    else {
                        // no default value, then check nillable
                        if (fNil) {
                            if (XMLString::compareString(value, XMLUni::fgZeroLenString))
                                emitError(XMLValid::NilAttrNotEmpty, elemDecl->getFullName());
                        }
                        else
                            fCurrentDV->validate(value);
                    }
                }
            } catch (InvalidDatatypeValueException idve) {
                emitError (XMLValid::DatatypeError, idve.getType(), idve.getMessage());
            }
            catch (InvalidDatatypeFacetException idve) {
                emitError (XMLValid::DatatypeError, idve.getType(), idve.getMessage());
            }
            catch (...) {
                emitError(XMLValid::GenericError);
            }
        }
    }
     else
    {
        ThrowXML(RuntimeException, XMLExcepts::CM_UnknownCMType);
    }

    fDatatypeBuffer.reset();
    fNil = false;
    fTrailing=false;

    // Went ok, so return success
    return -1;
}

bool SchemaValidator::checkRootElement (const   unsigned int    elemId)
{
    // we have nothing to check for root in schema
    // always return true
    return true;
}

void SchemaValidator::faultInAttr (XMLAttr&    toFill, const XMLAttDef&  attDef)   const
{
    //
    //  At this level, we cannot set the URI id. So we just set it to zero
    //  and leave it at that. The scanner, who called us, will look at the
    //  prefix we stored (if any), resolve it, and store the URL id if any.
    //
    SchemaAttDef* schemaAttDef = (SchemaAttDef*) &attDef;
    QName* attName = schemaAttDef->getAttName();

    toFill.set
    (
          attName->getURI()
        , attName->getLocalPart()
        , attName->getPrefix()
        , schemaAttDef->getValue()
        , schemaAttDef->getType()
    );
}

void SchemaValidator::reset()
{
}

bool SchemaValidator::requiresNamespaces() const
{
    return true;
}

void SchemaValidator::validateAttrValue (const   XMLAttDef* attDef
                                       , const XMLCh* const attrValue)
{
    //
    //  Get quick refs to lot of the stuff in the passed objects in
    //  order to simplify the code below, which will reference them very
    //  often.
    //
    XMLAttDef::AttTypes            type      = attDef->getType();
    const XMLAttDef::DefAttTypes   defType   = attDef->getDefaultType();
    const XMLCh* const             valueText = attDef->getValue();
    const XMLCh* const             fullName  = attDef->getFullName();
    XMLBuffer enumList;
    enumList.set(attDef->getEnumeration());

    //
    //  If the default type is fixed, then make sure the passed value maps
    //  to the fixed value.
    //
    if (defType == XMLAttDef::Fixed)
    {
        if (XMLString::compareString(attrValue, valueText))
            emitError(XMLValid::NotSameAsFixedValue, fullName, attrValue, valueText);
    }

    // An empty string cannot be valid for non_CDATA any of the other types
    if (!attrValue[0] && type != XMLAttDef::Simple)
    {
        emitError(XMLValid::InvalidEmptyAttValue, fullName);
        return;
    }

    if (type == XMLAttDef::Simple) {
        DatatypeValidator* attDefDV = ((SchemaAttDef*) attDef)->getDatatypeValidator();
        if (!attDefDV) {
            emitError(XMLValid::NoDatatypeValidatorForAttribute, fullName);
        }
        else {
            try {
                attDefDV->validate(attrValue);
            } catch (InvalidDatatypeValueException idve) {
                emitError (XMLValid::DatatypeError, idve.getType(), idve.getMessage());
            }
            catch (InvalidDatatypeFacetException idve) {
                emitError (XMLValid::DatatypeError, idve.getType(), idve.getMessage());
            }
            catch (...) {
                emitError(XMLValid::GenericError);
            }
        }
    }
    else {
        //  Check the Any Type and Simple Type using datatype validator
        if (type == XMLAttDef::Any_Any
         || type == XMLAttDef::Any_List
         || type == XMLAttDef::Any_Local
         || type == XMLAttDef::Any_Other) {

            if (defType == XMLAttDef::ProcessContents_Skip) {
                // attribute should just be bypassed,
            }
            else if (defType == XMLAttDef::ProcessContents_Strict
                 ||  defType == XMLAttDef::ProcessContents_Lax) {

                bool reportError = false;
                bool processContentStrict = (defType == XMLAttDef::ProcessContents_Strict);
                QName* const      attName = ((SchemaAttDef*)attDef)->getAttName();

                if (attName->getURI() == getScanner()->getEmptyNamespaceId()) {
                    if (processContentStrict)
                        reportError = true;
                } else {
                    SchemaGrammar* sGrammar = (SchemaGrammar*) fGrammarResolver->getGrammar(getScanner()->getURIText(attName->getURI()));
                    if (!sGrammar) {
                        if (processContentStrict)
                            reportError = true;
                    } else {
                        RefHashTableOf<XMLAttDef>* attRegistry = sGrammar->getAttributeDeclRegistry();
                        if (!attRegistry) {
                            if (processContentStrict)
                                reportError = true;
                        } else {
                            SchemaAttDef* attDecl = (SchemaAttDef*) attRegistry->get(attName->getLocalPart());
                            if (!attDecl) {
                                if (processContentStrict)
                                    reportError = true;
                            } else {
                                DatatypeValidator* attDeclDV = attDecl->getDatatypeValidator();
                                if (!attDeclDV) {
                                    if (processContentStrict)
                                        reportError = true;
                                }
                                else {
                                    try {
                                        DatatypeValidator::ValidatorType attDeclDVType = attDeclDV->getType();
                                        type = attDecl->getType();
                                        enumList.set(attDecl->getEnumeration());
                                        if (type == XMLAttDef::Simple)
                                            attDeclDV->validate(attrValue);
                                    } catch (InvalidDatatypeValueException idve) {
                                        emitError (XMLValid::DatatypeError, idve.getType(), idve.getMessage());
                                    }
                                    catch (InvalidDatatypeFacetException idve) {
                                        emitError (XMLValid::DatatypeError, idve.getType(), idve.getMessage());
                                    }
                                    catch (...) {
                                        emitError(XMLValid::GenericError);
                                    }
                                }
                            }
                        }
                    }
                }

                if (reportError) {
                    emitError(XMLValid::AttNotDefinedForElement, fullName, XMLUni::fgZeroLenString);
                }
            }
        }
        // check the ID, IDREF, IDREFS, NMTOKEN, NMTOKENS, ENTITY, ENTITIES, NOTATION using the old methods

        // See whether we are doing multiple values or not
        const bool multipleValues =
        (
            (type == XMLAttDef::IDRefs)
            || (type == XMLAttDef::Entities)
            || (type == XMLAttDef::NmTokens)
            || (type == XMLAttDef::Notation)
        );

        // And whether we must check for a first name char
        const bool firstNameChar =
        (
            (type == XMLAttDef::ID)
            || (type == XMLAttDef::IDRef)
            || (type == XMLAttDef::IDRefs)
            || (type == XMLAttDef::Entity)
            || (type == XMLAttDef::Entities)
            || (type == XMLAttDef::Notation)
        );

        // Some trigger flags to avoid issuing redundant errors and whatnot
        bool sawOneValue;
        bool alreadyCapped = false;

        //
        //  Make a copy of the text that we can mangle and get a pointer we can
        //  move through the value
        //

        // Use a stack-based buffer, when possible...
        XMLCh   tempBuffer[100];

        XMLCh* pszTmpVal = 0;

        ArrayJanitor<XMLCh> janTmpVal(0);

        if (XMLString::stringLen(attrValue) < sizeof(tempBuffer) / sizeof(tempBuffer[0]))
        {
            XMLString::copyString(tempBuffer, attrValue);
            pszTmpVal = tempBuffer;
        }
        else
        {
            janTmpVal.reset(XMLString::replicate(attrValue));
            pszTmpVal = janTmpVal.get();
        }

        XMLCh* valPtr = pszTmpVal;

        while (true)
        {
            // Reset the trigger flags
            sawOneValue = false;

            //
            //  Make sure the first character is a valid first name char, i.e.
            //  if its a Name value. For NmToken values we don't treat the first
            //  char any differently.
            //
            if (firstNameChar)
            {
                // If its not, emit and error but try to keep going
                if (!XMLReader::isFirstNameChar(*valPtr))
                    emitError(XMLValid::AttrValNotName, fullName);
                valPtr++;
            }

            // Make sure all the remaining chars are valid name chars
            while (*valPtr)
            {
                //
                //  If we hit a whitespace, its either a break between two
                //  or more values, or an error if we have a single value.
                //
                if (XMLReader::isWhitespace(*valPtr))
                {
                    if (!multipleValues)
                    {
                        emitError(XMLValid::NoMultipleValues, fullName);
                        return;
                    }
                    break;
                }

                if (!XMLReader::isNameChar(*valPtr))
                {
                    emitError(XMLValid::AttrValNotName, fullName);
                    return;
                }
                valPtr++;
            }

            //
            //  Cap it off at the current non-name char. If already capped,
            //  then remember this.
            //
            if (!(*valPtr))
                alreadyCapped = true;
            *valPtr = 0;

            //
            //  If this type of attribute requires that we track reference
            //  stuff, then handle that.
            //
            if ((type == XMLAttDef::ID)
            ||  (type == XMLAttDef::IDRef)
            ||  (type == XMLAttDef::IDRefs))
            {
                XMLRefInfo* find = getScanner()->getIDRefList().get(pszTmpVal);
                if (find)
                {
                    if (find->getDeclared() && (type == XMLAttDef::ID))
                        emitError(XMLValid::ReusedIDValue, pszTmpVal);
                }
                 else
                {
                    find = new XMLRefInfo(pszTmpVal);
                    getScanner()->getIDRefList().put((void*)find->getRefName(), find);
                }

                //
                //  Mark it declared or used, which might be redundant in some cases
                //  but not worth checking
                //
                if (type == XMLAttDef::ID)
                    find->setDeclared(true);
                else
                    find->setUsed(true);
            }
             else if ((type == XMLAttDef::Entity) || (type == XMLAttDef::Entities))
            {
                //
                //  If its refering to a entity, then look up the name in the
                //  general entity pool. If not there, then its an error. If its
                //  not an external unparsed entity, then its an error.
                //
                const XMLEntityDecl* decl = getScanner()->getEntityDecl(pszTmpVal);
                if (decl)
                {
                    if (!decl->isUnparsed())
                        emitError(XMLValid::BadEntityRefAttr, fullName);
                }
                 else
                {
                    emitError
                    (
                        XMLValid::UnknownEntityRefAttr
                        , fullName
                        , pszTmpVal
                    );
                }
            }
             else if (type == XMLAttDef::Notation)
            {
                //
                //  Make sure that this value maps to one of the
                //  notation values in the enumList parameter. We don't have to
                //  look it up in the notation pool (if a notation) because we
                //  will look up the enumerated values themselves. If they are in
                //  the notation pool (after the Grammar is parsed), then obviously
                //  this value will be legal since it matches one of them.
                //
                XMLBuffer nameBuf;
                XMLBuffer prefixBuf;
                XMLBuffer notationBuf;
                unsigned int uriId = getScanner()->resolveQName(pszTmpVal, nameBuf, prefixBuf, ElemStack::Mode_Attribute);
                notationBuf.set(getScanner()->getURIText(uriId));
                notationBuf.append(chColon);
                notationBuf.append(nameBuf.getRawBuffer());

                if (!XMLString::isInList(notationBuf.getRawBuffer(), enumList.getRawBuffer()))
                    emitError(XMLValid::DoesNotMatchEnumList, fullName);
            }

            // If not doing multiple values, then we are done
            if (!multipleValues)
                break;

            //
            //  If we are at the end, then break out now, else move up to the
            //  next char and update the base pointer.
            //
            if (alreadyCapped)
                break;

            valPtr++;
            pszTmpVal = valPtr;
        }
    }
}

void SchemaValidator::validateElement(const   XMLElementDecl*  elemDef)
{
    if (!fXsiType) {
        //
        // xsi:type was not specified...
        // If the corresponding type is abstract, detect an error
        //
        ComplexTypeInfo* typeInfo = ((SchemaElementDecl*)elemDef)->getComplexTypeInfo();

        if (typeInfo) {
            if (typeInfo->getAbstract()) {
                emitError(XMLValid::XsiTypeAbstract, elemDef->getFullName());
            }
        }
    }

    //
    // Check whether this element is abstract.  If so, an error
    //
    int miscFlags = ((SchemaElementDecl*)elemDef)->getMiscFlags();
    if ((miscFlags & SchemaSymbols::ABSTRACT) != 0) {
        emitError(XMLValid::SchemaAbstractError, elemDef->getFullName());
    }
    if (fNil && (miscFlags & SchemaSymbols::NILLABLE) == 0 ) {
        fNil = false;
        emitError(XMLValid::NillNotAllowed, elemDef->getFullName());
    }

    fDatatypeBuffer.reset();
}

void SchemaValidator::preContentValidation(bool reuseGrammar)
{
    //
    //  Lets enumerate all of the elements in the element decl pool
    //  and put out an error for any that did not get declared.
    //  We also check all of the attributes as well.
    //
    RefHash3KeysIdPoolEnumerator<SchemaElementDecl> elemEnum = fSchemaGrammar->getElemEnumerator();
    while (elemEnum.hasMoreElements())
    {
        const SchemaElementDecl& curElem = elemEnum.nextElement();
        const SchemaElementDecl::CreateReasons reason = curElem.getCreateReason();

        //
        //  See if this element decl was ever marked as declared. If
        //  not, then put out an error. In some cases its just
        //  a warning, such as being referenced in a content model.
        //
        if (reason != XMLElementDecl::Declared)
        {
            if (reason == XMLElementDecl::AttList)
            {
                getScanner()->emitError
                (
                    XMLErrs::UndeclaredElemInAttList
                    , curElem.getFullName()
                );
            }
             else if (reason == XMLElementDecl::AsRootElem)
            {
                emitError
                (
                    XMLValid::UndeclaredElemInDocType
                    , curElem.getFullName()
                );
            }
             else if (reason == XMLElementDecl::InContentModel)
            {
                getScanner()->emitError
                (
                    XMLErrs::UndeclaredElemInCM
                    , curElem.getFullName()
                );
            }
            else
            {
            }
        }

        //
        //  Check all of the attributes of the current element.
        //  We check for:
        //
        //  1) Multiple ID attributes
        //  2) That all of the default values of attributes are
        //      valid for their type.
        //  3) That for any notation types, that their lists
        //      of possible values refer to declared notations.
        //
        if (curElem.hasAttDefs()) {
            XMLAttDefList& attDefList = curElem.getAttDefList();
            bool seenId = false;
            while (attDefList.hasMoreElements())
            {
                const XMLAttDef& curAttDef = attDefList.nextElement();

                if (curAttDef.getType() == XMLAttDef::ID)
                {
                    if (seenId)
                    {
                        emitError
                        (
                            XMLValid::MultipleIdAttrs
                            , curElem.getFullName()
                        );
                        break;
                    }

                    seenId = true;
                }
                 else if (curAttDef.getType() == XMLAttDef::Notation)
                {
                    //
                    //  We need to verify that all of its possible values
                    //  (in the enum list) refer to valid notations.
                    //
                    XMLCh* list = XMLString::replicate(curAttDef.getEnumeration());
                    ArrayJanitor<XMLCh> janList(list);

                    //
                    //  Search forward for a space or a null. If a null,
                    //  we are done. If a space, cap it and look it up.
                    //
                    bool    breakFlag = false;
                    XMLCh*  listPtr = list;
                    XMLCh*  lastPtr = listPtr;
                    while (true)
                    {
                        while (*listPtr && (*listPtr != chSpace))
                            listPtr++;

                        //
                        //  If at the end, indicate we need to break after
                        //  this one. Else, cap it off here.
                        //
                        if (!*listPtr)
                            breakFlag = true;
                        else
                            *listPtr = chNull;

                        if (!fSchemaGrammar->getNotationDecl(lastPtr))
                        {
                            emitError
                            (
                                XMLValid::UnknownNotRefAttr
                                , curAttDef.getFullName()
                                , lastPtr
                            );
                        }

                        // Break out if we hit the end last time
                        if (breakFlag)
                            break;

                        // Else move upwards and try again
                        listPtr++;
                        lastPtr = listPtr;
                    }
                }

                // If it has a default/fixed value, then validate it
                if (curAttDef.getValue())
                {
                    validateAttrValue
                    (
                        &curAttDef
                        , curAttDef.getValue()
                    );
                }
            }
        }
    }
}

void SchemaValidator::postParseValidation()
{
    //
    //  At this time, there is nothing to do here. The scanner itself handles
    //  ID/IDREF validation, since that is the same no matter what kind of
    //  validator.
    //
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
    short fWhiteSpace = DatatypeValidator::PRESERVE;
    if (dV)
        fWhiteSpace = dV->getWSFacet();

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

        // stored the content for validation later
        //fDatatypeBuffer.append(nextCh);

        // And move up to the next character in the source
        srcPtr++;
    }
    srcPtr--;
    nextCh = *srcPtr;
    if (XMLReader::isWhitespace(nextCh))
        fTrailing = true;

    fDatatypeBuffer.set(toFill.getRawBuffer());
}
