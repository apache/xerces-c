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
 * $Id$
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <util/Janitor.hpp>
#include <util/XMLUniDefs.hpp>
#include <util/XMLUni.hpp>
#include <internal/ReaderMgr.hpp>
#include <internal/XMLScanner.hpp>
#include <validators/DTD/DTDValidator.hpp>


// ---------------------------------------------------------------------------
//  Local const data
//
//  These are the text for the require char refs that must always be present.
//  We init these into the entity pool upon construction.
// ---------------------------------------------------------------------------
static const XMLCh gAmp[] = { chLatin_a, chLatin_m, chLatin_p, chNull };
static const XMLCh gLT[] = { chLatin_l, chLatin_t, chNull };
static const XMLCh gGT[] = { chLatin_g, chLatin_t, chNull };
static const XMLCh gQuot[] = { chLatin_q, chLatin_u, chLatin_o, chLatin_t, chNull };
static const XMLCh gApos[] = { chLatin_a, chLatin_p, chLatin_o, chLatin_s, chNull };



// ---------------------------------------------------------------------------
//  Local methods
// ---------------------------------------------------------------------------

//
//  This method is called when we get a notation or enumeration type attribute
//  to validate. We have to confirm that the passed value to find is one of
//  the values in the passed list. The list is a space separated string of
//  values to match against.
//
static bool isInList(const XMLCh* const toFind, const XMLCh* const enumList)
{
    //
    //  We loop through the values in the list via this outer loop. We end
    //  when we hit the end of the enum list or get a match.
    //
    const XMLCh* listPtr = enumList;
    const unsigned int findLen = XMLString::stringLen(toFind);
    while (*listPtr)
    {
        unsigned int testInd;
        for (testInd = 0; testInd < findLen; testInd++)
        {
            //
            //  If they don't match, then reset and try again. Note that
            //  hitting the end of the current item will cause a mismatch
            //  because there can be no spaces in the toFind string.
            //
            if (listPtr[testInd] != toFind[testInd])
                break;
        }

        //
        //  If we went the distance, see if we matched. If we did, the current
        //  list character has to be null or space.
        //
        if (testInd == findLen)
        {
            if ((listPtr[testInd] == chSpace) || !listPtr[testInd])
                return true;
        }

        // Run the list pointer up to the next substring
        while ((*listPtr != chSpace) && *listPtr)
            listPtr++;

        // If we hit the end, then we failed
        if (!*listPtr)
            return false;

        // Else move past the space and try again
        listPtr++;
    }

    // We never found it
    return false;
}



// ---------------------------------------------------------------------------
//  DTDValidator: Constructors and Destructor
// ---------------------------------------------------------------------------
DTDValidator::DTDValidator(XMLErrorReporter* const errReporter) :

    XMLValidator(errReporter)
    , fDTDGrammar(0)
{
    reset();
}

DTDValidator::~DTDValidator()
{
}


// ---------------------------------------------------------------------------
//  DTDValidator: Implementation of the XMLValidator interface
// ---------------------------------------------------------------------------
int DTDValidator::checkContent( const   unsigned int    elemId
                                , const unsigned int*   childIds
                                , const unsigned int    childCount)
{
    //
    //  Look up the element id in our element decl pool. This will get us
    //  the element decl in our own way of looking at them.
    //
    DTDElementDecl* elemDecl = (DTDElementDecl*) fDTDGrammar->getElemDecl(elemId);
    if (!elemDecl)
        ThrowXML(RuntimeException, XMLExcepts::Val_InvalidElemId);

    //
    //  Get the content spec type of this element. This will tell us what
    //  to do to validate it.
    //
    const DTDElementDecl::ModelTypes modelType = elemDecl->getModelType();

    if (modelType == DTDElementDecl::Empty)
    {
        //
        //  We can do this one here. It cannot have any children. If it does
        //  we return 0 as the index of the first bad child.
        //
        if (childCount)
            return 0;
    }
     else if (modelType == DTDElementDecl::Any)
    {
        // We pass no judgement on this one, anything goes
    }
     else if ((modelType == DTDElementDecl::Mixed)
          ||  (modelType == DTDElementDecl::Children))
    {
        // Get the element's content model or fault it in
        const XMLContentModel* elemCM = elemDecl->getContentModel();

        // Ask it to validate and return its return
        return elemCM->validateContent(childIds, childCount);
    }
     else
    {
        ThrowXML(RuntimeException, XMLExcepts::CM_UnknownCMType);
    }

    // Went ok, so return success
    return -1;
}


bool DTDValidator::checkRootElement(const unsigned int elemId)
{
    //
    //  If the root element was never set, then there was never a DOCTYPE. So
    //  we just return false.
    //
    unsigned int fRootElemId = fDTDGrammar->getRootElemId();
    if (fRootElemId == XMLElementDecl::fgInvalidElemId)
        return false;

    // Else return true if our stored root element is the same as the passed one
    return (elemId == fRootElemId);
}

void DTDValidator::faultInAttr(XMLAttr& toFill, const XMLAttDef& attDef) const
{
    //
    //  At this level, we cannot set the URI id. So we just set it to zero
    //  and leave it at that. The scanner, who called us, will look at the
    //  prefix we stored (if any), resolve it, and store the URL id if any.
    //
    const XMLCh* fullName = attDef.getFullName();
    const int colonInd = XMLString::indexOf(fullName, chColon);
    if (colonInd == -1)
    {
        // There is no prefix, so we just do a simple and quick setting
        toFill.set
        (
            0
            , fullName
            , XMLUni::fgZeroLenString
            , attDef.getValue()
            , attDef.getType()
        );
    }
     else
    {
        //
        //  There is a colon, so we have to split apart the name and prefix
        //  part.
        //
        XMLBuffer bufTmp;
        XMLCh* tmpNameBuf = XMLString::replicate(fullName);
        ArrayJanitor<XMLCh> janNameBuf(tmpNameBuf);

        // Put a null where the colon is, to split it into two strings
        tmpNameBuf[colonInd] = chNull;

        //
        //  And now we can set the attribute object with the prefix and name
        //  parts.
        //
        toFill.set
        (
            0
            , &tmpNameBuf[colonInd+1]
            , tmpNameBuf
            , attDef.getValue()
            , attDef.getType()
        );
    }
}

void DTDValidator::reset()
{
}


bool DTDValidator::requiresNamespaces() const
{
    // Namespaces are not supported for DTDs
    return false;
}


void
DTDValidator::validateAttrValue(const   XMLAttDef&      attDef
                                , const XMLCh* const    attrValue)
{
    //
    //  Get quick refs to lost of of the stuff in the passed objects in
    //  order to simplify the code below, which will reference them very
    //  often.
    //
    const XMLAttDef::AttTypes       type = attDef.getType();
    const XMLAttDef::DefAttTypes    defType = attDef.getDefaultType();
    const XMLCh* const              valueText = attDef.getValue();
    const XMLCh* const              fullName = attDef.getFullName();
    const XMLCh* const              enumList = attDef.getEnumeration();

    //
    //  If the default type is fixed, then make sure the passed value maps
    //  to the fixed value.
    //
    if (defType == XMLAttDef::Fixed)
    {
        if (XMLString::compareString(attrValue, valueText))
            emitError(XMLValid::NotSameAsFixedValue, fullName, attrValue, valueText);
    }

    //
    //  If its a CDATA attribute, then we are done with any DTD level
    //  validation else do the rest.
    //
    if (type == XMLAttDef::CData)
        return;

    // An empty string cannot be valid for any of the other types
    if (!attrValue[0])
    {
        emitError(XMLValid::InvalidEmptyAttValue, fullName);
        return;
    }

    // See whether we are doing multiple values or not
    const bool multipleValues =
    (
        (type == XMLAttDef::IDRefs)
        || (type == XMLAttDef::Entities)
        || (type == XMLAttDef::NmTokens)
        || (type == XMLAttDef::Notation)
        || (type == XMLAttDef::Enumeration)
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

    // Whether it requires ref checking stuff
    const bool isARefType
    (
        (type == XMLAttDef::ID)
        || (type == XMLAttDef::IDRef)
        || (type == XMLAttDef::IDRefs)
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
        if (isARefType)
        {
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
        }
         else if ((type == XMLAttDef::Entity) || (type == XMLAttDef::Entities))
        {
            //
            //  If its refering to a entity, then look up the name in the
            //  general entity pool. If not there, then its an error. If its
            //  not an external unparsed entity, then its an error.
            //
            const XMLEntityDecl* decl = fDTDGrammar->getEntityDecl(pszTmpVal);
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
         else if ((type == XMLAttDef::Notation) || (type == XMLAttDef::Enumeration))
        {
            //
            //  Make sure that this value maps to one of the enumeration or
            //  notation values in the enumList parameter. We don't have to
            //  look it up in the notation pool (if a notation) because we
            //  will look up the enumerated values themselves. If they are in
            //  the notation pool (after the DTD is parsed), then obviously
            //  this value will be legal since it matches one of them.
            //
            if (!isInList(pszTmpVal, enumList))
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

void DTDValidator::preContentValidation(bool reuseGrammar)
{
    //
    //  Lets enumerate all of the elements in the element decl pool
    //  and put out an error for any that did not get declared.
    //  We also check all of the attributes as well.
    //
    NameIdPoolEnumerator<DTDElementDecl> elemEnum = fDTDGrammar->getElemEnumerator();
    while (elemEnum.hasMoreElements())
    {
        const DTDElementDecl& curElem = elemEnum.nextElement();
        const DTDElementDecl::CreateReasons reason = curElem.getCreateReason();

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
                #if defined(XERCES_DEBUG)
                  if(reuseGrammar && reason == XMLElementDecl::JustFaultIn){
                  }
                  else
                      ThrowXML(RuntimeException, XMLExcepts::DTD_UnknownCreateReason);
                #endif
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

                    if (!fDTDGrammar->getNotationDecl(lastPtr))
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
                    curAttDef
                    , curAttDef.getValue()
                );
            }
        }
    }

    //
    //  And enumerate all of the general entities. If any of them
    //  reference a notation, then make sure the notation exists.
    //
    NameIdPoolEnumerator<DTDEntityDecl> entEnum = fDTDGrammar->getEntityEnumerator();
    while (entEnum.hasMoreElements())
    {
        const DTDEntityDecl& curEntity = entEnum.nextElement();

        if (!curEntity.getNotationName())
            continue;

        // It has a notation name, so look it up
        if (!fDTDGrammar->getNotationDecl(curEntity.getNotationName()))
        {
            emitError
            (
                XMLValid::NotationNotDeclared
                , curEntity.getNotationName()
            );
        }
    }
}

void DTDValidator::postParseValidation()
{
    //
    //  At this time, there is nothing to do here. The scanner itself handles
    //  ID/IDREF validation, since that is the same no matter what kind of
    //  validator.
    //
}
