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

/**
 * $Log$
 * Revision 1.4  2000/02/09 21:42:38  abagchi
 * Copyright swatswat
 *
 * Revision 1.3  2000/01/12 23:52:48  roddey
 * These are trivial changes required to get the C++ and Java versions
 * of error messages more into sync. Mostly it was where the Java version
 * was passing out one or more parameter than the C++ version was. In
 * some cases the change just required an extra parameter to get the
 * needed info to the place where the error was issued.
 *
 * Revision 1.2  1999/11/30 20:24:45  roddey
 * Fixes for incorrect deletion of temporary decl objects, which would cause
 * a double delete when the parser is deleted.
 *
 * Revision 1.1.1.1  1999/11/09 01:03:35  twl
 * Initial checkin
 *
 * Revision 1.4  1999/11/08 20:45:40  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <util/Janitor.hpp>
#include <util/XMLMsgLoader.hpp>
#include <util/XMLUni.hpp>
#include <internal/ReaderMgr.hpp>
#include <internal/XMLScanner.hpp>
#include <framework/XMLBuffer.hpp>
#include <framework/XMLContentModel.hpp>
#include <framework/XMLElementDecl.hpp>
#include <framework/XMLEntityDecl.hpp>
#include <framework/XMLNotationDecl.hpp>
#include <validators/DTD/DTDValidator.hpp>
#include <validators/DTD/DocTypeHandler.hpp>


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
    , fDocTypeHandler(0)
    , fDumAttDef(0)
    , fDumElemDecl(0)
    , fDumEntityDecl(0)
    , fElemDeclPool(0)
    , fEntityDeclPool(0)
    , fInternalSubset(false)
    , fNextAttrId(1)
    , fNotationDeclPool(0)
    , fPEntityDeclPool(0)
    , fRootElemId(XMLElementDecl::fgInvalidElemId)
{
    //
    //  Init all the pool members. 
    //
    //  <TBD> Investigate what the optimum values would be for the various
    //  pools.
    //
    fElemDeclPool = new NameIdPool<DTDElementDecl>(109);
    fEntityDeclPool = new NameIdPool<DTDEntityDecl>(109);
    fNotationDeclPool = new NameIdPool<XMLNotationDecl>(109);
    fPEntityDeclPool = new NameIdPool<DTDEntityDecl>(109);

    //
    //  Call our own reset method. This lets us have the pool setup stuff
    //  done in just one place (because this stame setup stuff has to be
    //  done every time we are reset.)
    //
    reset();
}

DTDValidator::~DTDValidator()
{
    delete fDumAttDef;
    delete fDumElemDecl;
    delete fDumEntityDecl;
    delete fElemDeclPool;
    delete fEntityDeclPool;
    delete fNotationDeclPool;
    delete fPEntityDeclPool;
}


// ---------------------------------------------------------------------------
//  DTDValidator: Pool manipulation methods
// ---------------------------------------------------------------------------
unsigned int DTDValidator::findElemId(const XMLCh* const qName) const
{
    // Look up this name in the element decl pool
    const DTDElementDecl* decl = fElemDeclPool->getByKey(qName);
    if (!decl)
        return XMLElementDecl::fgInvalidElemId;
    return decl->getId();
}
 

// ---------------------------------------------------------------------------
//  DTDValidator: Implementation of the XMLValidator interface
// ---------------------------------------------------------------------------
int DTDValidator::addOrFindNSId(const XMLCh* const uriText)
{
    return fURIStringPool.addOrFind(uriText);
}


int DTDValidator::checkContent( const   unsigned int    elemId
                                , const unsigned int*   childIds
                                , const unsigned int    childCount)
{
    //
    //  Look up the element id in our element decl pool. This will get us
    //  the element decl in our own way of looking at them.
    //
    DTDElementDecl* elemDecl = fElemDeclPool->getById(elemId);
    if (!elemDecl)
        ThrowXML(RuntimeException, XML4CExcepts::Val_InvalidElemId);

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
        ThrowXML(RuntimeException, XML4CExcepts::CM_UnknownCMType);
    }

    // Went ok, so return success
    return XMLValidator::Success;
}


bool DTDValidator::checkRootElement(const unsigned int elemId)
{
    //
    //  If the root element was never set, then there was never a DOCTYPE. So
    //  we just return false.
    //
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


const XMLElementDecl*
DTDValidator::findElemDecl( const   unsigned int    uriId
                            , const XMLCh* const    baseName
                            , const XMLCh* const    qName
                            , const LookupOpts      options
                            ,       bool&           wasAdded) const
{
    // See it it exists
    DTDElementDecl* retVal = fElemDeclPool->getByKey(qName);

    // If not, and they want us to add it, then fault one in
    if (!retVal && (options == XMLValidator::AddIfNotFound))
    {
        retVal = new DTDElementDecl(qName, DTDElementDecl::Any);
        const unsigned int elemId = fElemDeclPool->put(retVal);
        retVal->setId(elemId);
        wasAdded = true;
    }
      else
    {
        wasAdded = false;
    }
    return retVal;
}


XMLElementDecl*
DTDValidator::findElemDecl( const   unsigned int    uriId
                            , const XMLCh* const    baseName
                            , const XMLCh* const    qName
                            , const LookupOpts      options
                            ,       bool&           wasAdded)
{
    // See it it exists
    DTDElementDecl* retVal = fElemDeclPool->getByKey(qName);

    // If not, and they want us to add it, then fault one in
    if (!retVal && (options == XMLValidator::AddIfNotFound))
    {
        retVal = new DTDElementDecl(qName, DTDElementDecl::Any);
        const unsigned int elemId = fElemDeclPool->put(retVal);
        retVal->setId(elemId);
        wasAdded = true;
    }
     else
    {
        wasAdded = false;
    }
    return retVal;
}


const XMLEntityDecl*
DTDValidator::findEntityDecl(const  XMLCh* const    entName
                            , const bool            isPE) const
{
    if (isPE)
        return fPEntityDeclPool->getByKey(entName);
    return fEntityDeclPool->getByKey(entName);
}


XMLEntityDecl* DTDValidator::findEntityDecl(const XMLCh* const entName
                                            , const bool       isPE)
{
    if (isPE)
        return fPEntityDeclPool->getByKey(entName);
    return fEntityDeclPool->getByKey(entName);
}


unsigned int
DTDValidator::findElemId(const  unsigned int    uriId
                        , const XMLCh* const    baseName
                        , const XMLCh* const    qName) const
{
    //
    //  In this case, we don't return zero to mean 'not found', so we have to
    //  map it to the official not found value if we don't find it.
    //
    const DTDElementDecl* decl = fElemDeclPool->getByKey(qName);
    if (!decl)
        return XMLElementDecl::fgInvalidElemId;
    return decl->getId();
}


const XMLNotationDecl*
DTDValidator::findNotationDecl(const XMLCh* const notName) const
{
    return fNotationDeclPool->getByKey(notName);
}


XMLNotationDecl* DTDValidator::findNotationDecl(const XMLCh* const notName)
{
    return fNotationDeclPool->getByKey(notName);
}


unsigned int DTDValidator::findNSId(const XMLCh* const uriText) const
{
    //
    //  Look up the id for this one. It returns zero if not found, which is
    //  what we want to return.
    //
    return fURIStringPool.getId(uriText);
}


const XMLElementDecl* DTDValidator::getElemDecl(const unsigned int elemId) const
{
    // Look up this element decl by id
    return fElemDeclPool->getById(elemId);
}


XMLElementDecl* DTDValidator::getElemDecl(const unsigned int elemId)
{
    // Look up this element decl by id
    return fElemDeclPool->getById(elemId);
}


bool DTDValidator::getURIText(  const   unsigned int    uriId
                                ,       XMLBuffer&      uriBufToFill) const
{
    // Look up the URI in the string pool and return its id
    const XMLCh* value = fURIStringPool.getValueForId(uriId);
    if (!value)
        return false;

    uriBufToFill.set(value);
    return true;
}


void DTDValidator::postParseValidation()
{
    //
    //  At this time, there is nothing to do here. The scanner itself handles
    //  ID/IDREF validation, since that is the same no matter what kind of
    //  validator.
    //
}


void DTDValidator::reset()
{
    //
    //  We need to reset all of the pools and a couple of the other members.
    //  The other stuff is not sensitive across uses of the validator.
    //
    fElemDeclPool->removeAll();
    fEntityDeclPool->removeAll();
    fInternalSubset = false;
    fNotationDeclPool->removeAll();
    fPEntityDeclPool->removeAll();
    fURIStringPool.flushAll();
    fRootElemId = XMLElementDecl::fgInvalidElemId;

    // If there is a doc type handler, reset it
    if (fDocTypeHandler)
        fDocTypeHandler->resetDocType();

    //
    //  Add some special URIs to the URI string pool. These represent special
    //  URIs that are predefined and don't come from the user data. We pass
    //  the ids to our base class who will store them and make them available
    //  to client code.
    //
    setBaseFields
    (
        fURIStringPool.addOrFind(XMLUni::fgZeroLenString)
        , fURIStringPool.addOrFind(XMLUni::fgGlobalNSURIName)
        , fURIStringPool.addOrFind(XMLUni::fgUnknownURIName)
        , fURIStringPool.addOrFind(XMLUni::fgXMLURIName)
        , fURIStringPool.addOrFind(XMLUni::fgXMLNSURIName)
    );

    //
    //  Add the default entity entries for the character refs that must always
    //  be present. We indicate that they are from the internal subset. They
    //  aren't really, but they have to look that way so that they are still
    //  valid for use within a standalone document.
    //
    //  We also mark them as special char entities, which allows them to be
    //  used in places whether other non-numeric general entities cannot.
    //
    fEntityDeclPool->put(new DTDEntityDecl(gAmp, chAmpersand, true, true));
    fEntityDeclPool->put(new DTDEntityDecl(gLT, chOpenAngle, true, true));
    fEntityDeclPool->put(new DTDEntityDecl(gGT, chCloseAngle, true, true));
    fEntityDeclPool->put(new DTDEntityDecl(gQuot, chDoubleQuote, true, true));
    fEntityDeclPool->put(new DTDEntityDecl(gApos, chSingleQuote, true, true));
}


bool DTDValidator::requiresNamespaces() const
{
    // Namespaces are optional for DTDs
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
            emitError(XML4CValid::NotSameAsFixedValue, fullName, attrValue, valueText);
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
        emitError(XML4CValid::InvalidEmptyAttValue, fullName);
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
    XMLCh* pszTmpVal = XMLString::replicate(attrValue);
    ArrayJanitor<XMLCh> janTmpVal(pszTmpVal);
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
                emitError(XML4CValid::AttrValNotName, fullName);
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
                    emitError(XML4CValid::NoMultipleValues, fullName);
                    return;
                }
                break;
            }

            if (!XMLReader::isNameChar(*valPtr))
            {
                emitError(XML4CValid::AttrValNotName, fullName);
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
                        emitError(XML4CValid::ReusedIDValue, pszTmpVal);
                }
                 else
                {
                    find = new XMLRefInfo(pszTmpVal);
                    getScanner()->getIDRefList().put(find);
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
            const XMLEntityDecl* decl = findEntityDecl(pszTmpVal, false);
            if (decl)
            {
                if (!decl->isUnparsed())
                    emitError(XML4CValid::BadEntityRefAttr, fullName);
            }
             else
            {
                emitError
                (
                    XML4CValid::UnknownEntityRefAttr
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
                emitError(XML4CValid::DoesNotMatchEnumList, fullName);
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


// ---------------------------------------------------------------------------
//  DTDValidator: DTD handler interface.
// ---------------------------------------------------------------------------
void DTDValidator::scanDTD(const bool reuseValidator)
{
    //
    //  And now call out to the DTDValidator2.cpp file to do the actual
    //  scanning. When we get back, we will be through the internal and
    //  external subsets.
    //
    scanDocTypeDecl(reuseValidator);

    // If validating, then do the pre-content validation
    if (getScanner()->getDoValidation())
    {
        //
        //  Lets enumerate all of the elements in the element decl pool
        //  and put out an error for any that did not get declared.
        //  We also check all of the attributes as well.
        //
        NameIdPoolEnumerator<DTDElementDecl> elemEnum(fElemDeclPool);
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
                        XML4CErrs::UndeclaredElemInAttList
                        , curElem.getFullName()
                    );
                }
                 else if (reason == XMLElementDecl::AsRootElem)
                {
                    emitError
                    (
                        XML4CValid::UndeclaredElemInDocType
                        , curElem.getFullName()
                    );
                }
                 else if (reason == XMLElementDecl::InContentModel)
                {
                    getScanner()->emitError
                    (
                        XML4CErrs::UndeclaredElemInCM
                        , curElem.getFullName()
                    );
                }
                else
                {
                    #if defined(XML4C_DEBUG)
                    // <TBD> Should throw here
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
                            XML4CValid::MultipleIdAttrs
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

                        if (!fNotationDeclPool->getByKey(lastPtr))
                        {
                            emitError
                            (
                                XML4CValid::UnknownNotRefAttr
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
        NameIdPoolEnumerator<DTDEntityDecl> entEnum(fEntityDeclPool);
        while (entEnum.hasMoreElements())
        {
            const DTDEntityDecl& curEntity = entEnum.nextElement();

            if (!curEntity.getNotationName())
                continue;

            // It has a notation name, so look it up
            if (!fNotationDeclPool->getByKey(curEntity.getNotationName()))
            {
                emitError
                (
                    XML4CValid::NotationNotDeclared
                    , curEntity.getNotationName()
                );
            }
        }
    }
}

