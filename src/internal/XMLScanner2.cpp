/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 1999-2001 The Apache Software Foundation.  All rights
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
//  This file holds some of the grunt work methods of XMLScanner.cpp to keep
//  it a little more readable.
// ---------------------------------------------------------------------------


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <util/BinMemInputStream.hpp>
#include <util/Janitor.hpp>
#include <util/PlatformUtils.hpp>
#include <util/TransService.hpp>
#include <util/UnexpectedEOFException.hpp>
#include <util/XMLUniDefs.hpp>
#include <util/XMLUni.hpp>
#include <util/XMLURL.hpp>
#include <sax/InputSource.hpp>
#include <framework/LocalFileInputSource.hpp>
#include <framework/URLInputSource.hpp>
#include <framework/XMLErrorReporter.hpp>
#include <framework/XMLDocumentHandler.hpp>
#include <framework/XMLEntityHandler.hpp>
#include <framework/XMLPScanToken.hpp>
#include <framework/XMLRefInfo.hpp>
#include <framework/XMLValidator.hpp>
#include <internal/XMLScanner.hpp>
#include <internal/EndOfEntityException.hpp>
#include <internal/XMLInternalErrorHandler.hpp>
#include <parsers/DOMParser.hpp>
#include <dom/DOM_DOMException.hpp>
#include <sax/EntityResolver.hpp>
#include <validators/common/ContentLeafNameTypeVector.hpp>
#include <validators/datatype/DatatypeValidator.hpp>
#include <validators/schema/SchemaSymbols.hpp>
#include <validators/schema/SchemaGrammar.hpp>
#include <validators/schema/TraverseSchema.hpp>
#include <validators/schema/SubstitutionGroupComparator.hpp>



// ---------------------------------------------------------------------------
//  XMLScanner: Private helper methods
// ---------------------------------------------------------------------------

//
//  This method is called from scanStartTagNS() to build up the list of
//  XMLAttr objects that will be passed out in the start tag callout. We
//  get the key/value pairs from the raw scan of explicitly provided attrs,
//  which have not been normalized. And we get the element declaration from
//  which we will get any defaulted or fixed attribute defs and add those
//  in as well.
//
unsigned int
XMLScanner::buildAttList(const  RefVectorOf<KVStringPair>&  providedAttrs
                        , const unsigned int                attCount
                        ,       XMLElementDecl&             elemDecl
                        ,       RefVectorOf<XMLAttr>&       toFill)
{
    //
    //  Ask the element to clear the 'provided' flag on all of the att defs
    //  that it owns, and to return us a boolean indicating whether it has
    //  any defs.
    //
    const bool hasDefs = elemDecl.resetDefs();

    //
    //  If there are no expliclitily provided attributes and there are no
    //  defined attributes for the element, the we don't have anything to do.
    //  So just return zero in this case.
    //
    if (!hasDefs && !attCount)
        return 0;

    // Keep up with how many attrs we end up with total
    unsigned int retCount = 0;

    //
    //  And get the current size of the output vector. This lets us use
    //  existing elements until we fill it, then start adding new ones.
    //
    const unsigned int curAttListSize = toFill.size();

    //
    //  We need a buffer into which raw scanned attribute values will be
    //  normalized.
    //
    XMLBufBid bbNormal(&fBufMgr);
    XMLBuffer& normBuf = bbNormal.getBuffer();

    //
    //  Loop through our explicitly provided attributes, which are in the raw
    //  scanned form, and build up XMLAttr objects.
    //
    unsigned int index;
    for (index = 0; index < attCount; index++)
    {
        const KVStringPair* curPair = providedAttrs.elementAt(index);

        //
        //  We have to split the name into its prefix and name parts. Then
        //  we map the prefix to its URI.
        //
        const XMLCh* const namePtr = curPair->getKey();
        ArrayJanitor<XMLCh> janName(0);

        // use a stack-based buffer when possible.
        XMLCh tempBuffer[100];

        const int colonInd = XMLString::indexOf(namePtr, chColon);
        const XMLCh* prefPtr = XMLUni::fgZeroLenString;
        const XMLCh* suffPtr = XMLUni::fgZeroLenString;
        if (colonInd != -1)
        {
            // We have to split the string, so make a copy.
             if (XMLString::stringLen(namePtr) < sizeof(tempBuffer) / sizeof(tempBuffer[0]))
            {
                XMLString::copyString(tempBuffer, namePtr);
                tempBuffer[colonInd] = chNull;
                prefPtr = tempBuffer;
            }
            else
            {
                janName.reset(XMLString::replicate(namePtr));
                janName[colonInd] = chNull;
                prefPtr = janName.get();
            }

            suffPtr = prefPtr + colonInd + 1;
        }
         else
        {
            // No colon, so we just have a name with no prefix
            suffPtr = namePtr;
        }

        //
        //  Map the prefix to a URI id. We tell him that we are mapping an
        //  attr prefix, so any xmlns attrs at this level will not affect it.
        //
        const unsigned int uriId = resolvePrefix(prefPtr, ElemStack::Mode_Attribute);

        //
        //  If the uri comes back as the xmlns or xml URI or its just a name
        //  and that name is 'xmlns', then we handle it specially. So set a
        //  boolean flag that lets us quickly below know which we are dealing
        //  with.
        //
        const bool isNSAttr = (uriId == fXMLNSNamespaceId)
                              || (uriId == fXMLNamespaceId)
                              || !XMLString::compareString(suffPtr, XMLUni::fgXMLNSString)
                              || !XMLString::compareString(getURIText(uriId), SchemaSymbols::fgURI_XSI);


        //
        //  If its not a special case namespace attr of some sort, then we
        //  do normal checking and processing.
        //
        XMLAttDef::AttTypes attType;
        if (!isNSAttr || fGrammar->getGrammarType() == Grammar::DTDGrammarType)
        {
            //
            //  Find this attribute within the parent element. We pass both
            //  the uriID/name and the raw QName buffer, since we don't know
            //  how the derived validator and its elements store attributes.
            //
            bool wasAdded = false;
            XMLAttDef* attDef = elemDecl.findAttr
            (
                curPair->getKey()
                , uriId
                , suffPtr
                , prefPtr
                , XMLElementDecl::AddIfNotFound
                , wasAdded
            );

            //
            //  Its not valid for this element, so issue an error if we are
            //  validating.
            //
            if (wasAdded)
            {
                if (fValidate)
                {
                    // This is to tell the Validator that this attribute was
                    // faulted-in, was not an attribute in the attdef originally
                    attDef->setCreateReason(XMLAttDef::JustFaultIn);

                    XMLBuffer bufURI;
                    getURIText(uriId, bufURI);

                    XMLBuffer bufMsg;
                    bufMsg.append(chOpenCurly);
                    bufMsg.append(bufURI.getRawBuffer());
                    bufMsg.append(chCloseCurly);
                    bufMsg.append(suffPtr);
                    fValidator->emitError
                    (
                        XMLValid::AttNotDefinedForElement
                        , bufMsg.getRawBuffer()
                        , elemDecl.getFullName()
                    );
                }
            }
            else
            {
                // If this attribute was faulted-in and first occurence,
                // then emit an error
                if (fValidate
                    && attDef->getCreateReason() == XMLAttDef::JustFaultIn
                    && !attDef->getProvided())
                {
                    XMLBuffer bufURI;
                    getURIText(uriId, bufURI);

                    XMLBuffer bufMsg;
                    bufMsg.append(chOpenCurly);
                    bufMsg.append(bufURI.getRawBuffer());
                    bufMsg.append(chCloseCurly);
                    bufMsg.append(suffPtr);
                    fValidator->emitError
                    (
                        XMLValid::AttNotDefinedForElement
                        , bufMsg.getRawBuffer()
                        , elemDecl.getFullName()
                    );
                }
             }

            //
            //  If its already provided, then there are more than one of
            //  this attribute in this start tag, so emit an error.
            //
            if (attDef->getProvided())
            {
                emitError
                (
                    XMLErrs::AttrAlreadyUsedInSTag
                    , attDef->getFullName()
                    , elemDecl.getFullName()
                );
            }
             else
            {
                // Mark this one as already seen
                attDef->setProvided(true);
            }

            //
            //  Now normalize the raw value since we have the attribute type. We
            //  don't care about the return status here. If it failed, an error
            //  was issued, which is all we care about.
            //
            normalizeAttValue
            (
                attDef
                , curPair->getValue()
                , normBuf
            );

            //
            //  If we found an attdef for this one, then lets validate it.
            //
            if (!wasAdded && attDef->getCreateReason() != XMLAttDef::JustFaultIn)
            {
                if (fValidate)
                {
                    if (fGrammar->getGrammarType() == Grammar::SchemaGrammarType)
                    {
                        // normalize the attribute according to schema whitespace facet
                        XMLBuffer toFill;
                        DatatypeValidator* tempDV = ((SchemaAttDef*) attDef)->getDatatypeValidator();
                        ((SchemaValidator*) fValidator)->normalizeWhiteSpace(tempDV, normBuf.getRawBuffer(), toFill);
                        normBuf.set(toFill.getRawBuffer());
                    }

                    fValidator->validateAttrValue
                    (
                        attDef
                        , normBuf.getRawBuffer()
                    );
                }
            }

            // Save the type for later use
            attType = attDef->getType();
        }
         else
        {
            // Just normalize as CDATA
            attType = XMLAttDef::CData;
            normalizeAttRawValue
            (
                curPair->getKey()
                , curPair->getValue()
                , normBuf
            );
        }

        //
        //  Add this attribute to the attribute list that we use to pass them
        //  to the handler. We reuse its existing elements but expand it as
        //  required.
        //
        XMLAttr* curAttr;
        if (retCount >= curAttListSize)
        {
            curAttr = new XMLAttr
            (
                uriId
                , suffPtr
                , prefPtr
                , normBuf.getRawBuffer()
                , attType
                , true
            );
            toFill.addElement(curAttr);
        }
         else
        {
            curAttr = toFill.elementAt(retCount);
            curAttr->set
            (
                uriId
                , suffPtr
                , prefPtr
                , normBuf.getRawBuffer()
                , attType
            );
            curAttr->setSpecified(true);
        }

        // Bump the count of attrs in the list
        retCount++;
    }

    //
    //  Now, if there are any attributes declared by this element, let's
    //  go through them and make sure that any required ones are provided,
    //  and fault in any fixed ones and defaulted ones that are not provided
    //  literally.
    //
    if (hasDefs && fValidate)
    {
        //
        // Check after all specified attrs are scanned
        // (1) report error for REQUIRED attrs that are missing (V_TAGc)
        // (2) add default attrs if missing (FIXED and NOT_FIXED)
        //
        XMLAttDefList& attDefList = elemDecl.getAttDefList();
        while (attDefList.hasMoreElements())
        {
            // Get the current att def, for convenience and its def type
            const XMLAttDef& curDef = attDefList.nextElement();
            const XMLAttDef::DefAttTypes defType = curDef.getDefaultType();

            if (!curDef.getProvided())
            {
                //the attributes is not provided
                if (fValidate)
                {
                    // If we are validating and its required, then an error
                    if ((defType == XMLAttDef::Required) ||
                        (defType == XMLAttDef::Required_And_Fixed)  )

                    {
                        fValidator->emitError
                        (
                            XMLValid::RequiredAttrNotProvided
                            , curDef.getFullName()
                        );
                    }
                    else if ((defType == XMLAttDef::Default) ||
                             (defType == XMLAttDef::Fixed)  )
                    {
                        if (fStandalone && curDef.isExternal())
                        {
                            //
                            // XML 1.0 Section 2.9
                            // Document is standalone, so attributes must not be defaulted.
                            //
                            fValidator->emitError(XMLValid::NoDefAttForStandalone, curDef.getFullName(), elemDecl.getFullName());
                        }
                    }
                }

                //
                //  Fault in the value if needed, and bump the att count.
                //  We have to
                //
                if ((defType == XMLAttDef::Default)
                ||  (defType == XMLAttDef::Fixed))
                {
                    XMLAttr* curAtt;
                    if (retCount >= curAttListSize)
                    {
                        curAtt = new XMLAttr;
                        fValidator->faultInAttr(*curAtt, curDef);
                        fAttrList->addElement(curAtt);
                    }
                    else
                    {
                        curAtt = fAttrList->elementAt(retCount);
                        fValidator->faultInAttr(*curAtt, curDef);
                    }

                    if (fGrammar->getGrammarType() == Grammar::DTDGrammarType)
                    {
                        //
                        //  Map the new attribute's prefix to a URI id and store
                        //  that in the attribute object.
                        //
                        curAtt->setURIId
                        (
                            resolvePrefix(curAtt->getPrefix(), ElemStack::Mode_Attribute)
                        );
                    }

                    // Indicate it was not explicitly specified and bump count
                    curAtt->setSpecified(false);
                    retCount++;
                }
            }
            else
            {
                //attribute is provided
                // (schema) report error for PROHIBITED attrs that are present (V_TAGc)
                if (defType == XMLAttDef::Prohibited && fValidate)
                    fValidator->emitError
                    (
                        XMLValid::ProhibitedAttributePresent
                        , curDef.getFullName()
                    );
            }
        }
    }
    return retCount;
}


//
//  This method is called after the content scan to insure that all the
//  ID/IDREF attributes match up (i.e. that all IDREFs refer to IDs.) This is
//  an XML 1.0 rule, so we can do here in the core.
//
void XMLScanner::checkIDRefs()
{
    //
    //
    //  Iterate the id ref list. If we find any entries here which are used
    //  but not declared, then that's an error.
    //
    RefHashTableOfEnumerator<XMLRefInfo> refEnum(fIDRefList);
    while (refEnum.hasMoreElements())
    {
        // Get a ref to the current element
        const XMLRefInfo& curRef = refEnum.nextElement();

        // If its used but not declared, then its an error
        if (!curRef.getDeclared() && curRef.getUsed() && fValidate)
            fValidator->emitError(XMLValid::IDNotDeclared, curRef.getRefName());
    }
}


//
//  This just does a simple check that the passed progressive scan token is
//  legal for this scanner.
//
bool XMLScanner::isLegalToken(const XMLPScanToken& toCheck)
{
    return ((fScannerId == toCheck.fScannerId)
    &&      (fSequenceId == toCheck.fSequenceId));
}


//
//  This method will take a raw attribute value and normalize it according to
//  the rules of the attribute type. It will put the resulting value into the
//  passed buffer.
//
//  This code assumes that escaped characters in the original value (via char
//  refs) are prefixed by a 0xFFFF character. This is because some characters
//  are legal if escaped only. And some escape chars are not subject to
//  normalization rules.
//
bool XMLScanner::normalizeAttValue( const   XMLAttDef* const    attDef
                                    , const XMLCh* const        value
                                    ,       XMLBuffer&          toFill)
{
    // A simple state value for a whitespace processing state machine
    enum States
    {
        InWhitespace
        , InContent
    };

    // Get the type and name
    const XMLAttDef::AttTypes type = attDef->getType();
    const XMLCh* const attrName = attDef->getFullName();

    // Assume its going to go fine, and empty the target buffer in preperation
    bool retVal = true;
    toFill.reset();

    //
    // Get attribute def - to check to see if it's declared externally or not
    //
    bool  isAttExternal = attDef->isExternal();

    //
    //  Loop through the chars of the source value and normalize it according
    //  to the type.
    //
    States curState = InContent;
    bool escaped;
    bool firstNonWS = false;
    XMLCh nextCh;
    const XMLCh* srcPtr = value;
    while (*srcPtr)
    {
        //
        //  Get the next character from the source. We have to watch for
        //  escaped characters (which are indicated by a 0xFFFF value followed
        //  by the char that was escaped.)
        //
        nextCh = *srcPtr;
        escaped = (nextCh == 0xFFFF);
        if (escaped)
            nextCh = *++srcPtr;

        //
        //  If its not escaped, then make sure its not a < character, which is
        //  not allowed in attribute values.
        //
        if (!escaped && (*srcPtr == chOpenAngle))
        {
            emitError(XMLErrs::BracketInAttrValue, attrName);
            retVal = false;
        }

        if (type == XMLAttDef::CData)
        {
            if (!escaped)
            {
                if ((nextCh == 0x09) || (nextCh == 0x0A) || (nextCh == 0x0D))
                {
                    //
                    // Check Validity Constraint for Standalone document declaration
                    // XML 1.0, Section 2.9
                    //
                    if (fStandalone && fValidate && isAttExternal)
                    {
                         //
                         // Can't have a standalone document declaration of "yes" if  attribute
                         // values are subject to normalisation
                         //
                         fValidator->emitError(XMLValid::NoAttNormForStandalone, attrName);
                    }
                    nextCh = chSpace;
                }
            }
        }
         else
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
                    //
                    // Check Validity Constraint for Standalone document declaration
                    // XML 1.0, Section 2.9
                    //
                    if (fStandalone && fValidate && isAttExternal)
                    {
                        if (!firstNonWS || (nextCh != chSpace) || (fReaderMgr.lookingAtSpace()))
                        {
                             //
                             // Can't have a standalone document declaration of "yes" if  attribute
                             // values are subject to normalisation
                             //
                             fValidator->emitError(XMLValid::NoAttNormForStandalone, attrName);
                        }
                    }
                    continue;
                }
                firstNonWS = true;
            }
        }

        // Add this char to the target buffer
        toFill.append(nextCh);

        // And move up to the next character in the source
        srcPtr++;
    }
    return retVal;
}

//
//  This method will just normalize the input value as CDATA without
//  any standalone checking.
//
bool XMLScanner::normalizeAttRawValue( const   XMLCh* const        attrName
                                    , const XMLCh* const        value
                                    ,       XMLBuffer&          toFill)
{
    // A simple state value for a whitespace processing state machine
    enum States
    {
        InWhitespace
        , InContent
    };

    // Assume its going to go fine, and empty the target buffer in preperation
    bool retVal = true;
    toFill.reset();

    //
    //  Loop through the chars of the source value and normalize it according
    //  to the type.
    //
    States curState = InContent;
    bool escaped;
    bool firstNonWS = false;
    XMLCh nextCh;
    const XMLCh* srcPtr = value;
    while (*srcPtr)
    {
        //
        //  Get the next character from the source. We have to watch for
        //  escaped characters (which are indicated by a 0xFFFF value followed
        //  by the char that was escaped.)
        //
        nextCh = *srcPtr;
        escaped = (nextCh == 0xFFFF);
        if (escaped)
            nextCh = *++srcPtr;

        //
        //  If its not escaped, then make sure its not a < character, which is
        //  not allowed in attribute values.
        //
        if (!escaped && (*srcPtr == chOpenAngle))
        {
            emitError(XMLErrs::BracketInAttrValue, attrName);
            retVal = false;
        }

        if (!escaped)
        {
            //
            //  NOTE: Yes this is a little redundant in that a 0x20 is
            //  replaced with an 0x20. But its faster to do this (I think)
            //  than checking for 9, A, and D separately.
            //
            if (XMLReader::isWhitespace(nextCh))
                nextCh = chSpace;
        }

        // Add this char to the target buffer
        toFill.append(nextCh);

        // And move up to the next character in the source
        srcPtr++;
    }
    return retVal;
}

unsigned int
XMLScanner::resolvePrefix(  const   XMLCh* const        prefix
                            , const ElemStack::MapModes mode)
{
    //
    //  Watch for the special namespace prefixes. We always map these to
    //  special URIs. 'xml' gets mapped to the official URI that its defined
    //  to map to by the NS spec. xmlns gets mapped to a special place holder
    //  URI that we define (so that it maps to something checkable.)
    //
    if (!XMLString::compareString(prefix, XMLUni::fgXMLNSString))
        return fXMLNSNamespaceId;
    else if (!XMLString::compareString(prefix, XMLUni::fgXMLString))
        return fXMLNamespaceId;


    //
    //  Ask the element stack to search up itself for a mapping for the
    //  passed prefix.
    //
    bool unknown;
    unsigned int uriId = fElemStack.mapPrefixToURI(prefix, mode, unknown);

    // If it was unknown, then the URI was faked in but we have to issue an error
    if (unknown)
        emitError(XMLErrs::UnknownPrefix, prefix);

    return uriId;
}


unsigned int
XMLScanner::resolvePrefix(  const   XMLCh* const        prefix
                            ,       XMLBuffer&          bufToFill
                            , const ElemStack::MapModes mode)
{
    //
    //  Watch for the special namespace prefixes. We always map these to
    //  special URIs. 'xml' gets mapped to the official URI that its defined
    //  to map to by the NS spec. xmlns gets mapped to a special place holder
    //  URI that we define (so that it maps to something checkable.)
    //
    if (!XMLString::compareString(prefix, XMLUni::fgXMLNSString))
        return fXMLNSNamespaceId;
    else if (!XMLString::compareString(prefix, XMLUni::fgXMLString))
        return fXMLNamespaceId;

    //
    //  Ask the element stack to search up itself for a mapping for the
    //  passed prefix.
    //
    bool unknown;
    unsigned int uriId = fElemStack.mapPrefixToURI(prefix, mode, unknown);

    // If it was unknown, then the URI was faked in but we have to issue an error
    if (unknown)
        emitError(XMLErrs::UnknownPrefix, prefix);

    getURIText(uriId,bufToFill);

    return uriId;
}


//
//  This method will reset the scanner data structures, and related plugged
//  in stuff, for a new scan session. We get the input source for the primary
//  XML entity, create the reader for it, and push it on the stack so that
//  upon successful return from here we are ready to go.
//
void XMLScanner::scanReset(const InputSource& src)
{
    //
    //  This call implicitly tells us that we are going to reuse the scanner
    //  if it was previously used. So tell the validator to reset itself.
    //
    //  But, if the fReuseGrammar flag is set, then don't reset it.
    //
    //  NOTE:   The ReaderMgr is flushed on the way out, because that is
    //          required to insure that files are closed.
    //
    if (!fReuseGrammar) {
        fGrammarResolver->reset();

        resetEntityDeclPool();
        if (fDoNamespaces)
            resetURIStringPool();

        // create a default grammar first
        if (fValidatorFromUser) {
            if (fValidator->handlesDTD()) {
                fGrammar = new DTDGrammar();
            }
            else {
                fGrammar = new SchemaGrammar();
            }
        }
        else {
            // set fValidator as fDTDValidator
            fValidator = fDTDValidator;
            fGrammar = new DTDGrammar();
        }

        fGrammarResolver->putGrammar(XMLUni::fgZeroLenString, fGrammar);
        fValidator->setGrammar(fGrammar);

        if (fValScheme == Val_Auto) {
            fValidate = false;
        }
    }
    else {
        // reusing grammar, thus the fGrammar must pre-exist already
        // make sure the validator handles this reuse grammar type
        if (fGrammar->getGrammarType() == Grammar::SchemaGrammarType && !fValidator->handlesSchema()) {
            if (fValidatorFromUser)
                ThrowXML(RuntimeException, XMLExcepts::Gen_NoSchemaValidator);
            else {
                fValidator = fSchemaValidator;
            }
        }
        else if (fGrammar->getGrammarType() == Grammar::DTDGrammarType && !fValidator->handlesDTD()) {
            if (fValidatorFromUser)
                ThrowXML(RuntimeException, XMLExcepts::Gen_NoDTDValidator);
            else {
                fValidator = fDTDValidator;
            }
        }

        if (!fValidator->getGrammar())
            fValidator->setGrammar(fGrammar);
    }


    //
    //  And for all installed handlers, send reset events. This gives them
    //  a chance to flush any cached data.
    //
    if (fDocHandler)
        fDocHandler->resetDocument();
    if (fEntityHandler)
        fEntityHandler->resetEntities();
    if (fErrorReporter)
        fErrorReporter->resetErrors();

    // Clear out the id reference list
    fIDRefList->removeAll();

    //
    //  Reset the element stack, and give it the latest ids for the special
    //  URIs it has to know about.
    //
    fElemStack.reset
    (
        fEmptyNamespaceId
        , fUnknownNamespaceId
        , fXMLNamespaceId
        , fXMLNSNamespaceId
    );

    // Reset some status flags
    fInException = false;
    fStandalone = false;
    fErrorCount = 0;
    fHasNoDTD = true;
    fSeeXsi = false;

    //
    //  Handle the creation of the XML reader object for this input source.
    //  This will provide us with transcoding and basic lexing services.
    //
    XMLReader* newReader = fReaderMgr.createReader
    (
        src
        , true
        , XMLReader::RefFrom_NonLiteral
        , XMLReader::Type_General
        , XMLReader::Source_External
    );
    if (!newReader)
        ThrowXML1(RuntimeException, XMLExcepts::Scan_CouldNotOpenSource, src.getSystemId());

    // Push this read onto the reader manager
    fReaderMgr.pushReader(newReader, 0);
}


//
//  This method is called between markup in content. It scans for character
//  data that is sent to the document handler. It watches for any markup
//  characters that would indicate that the character data has ended. It also
//  handles expansion of general and character entities.
//
//  sendData() is a local static helper for this method which handles some
//  code that must be done in three different places here.
//
void XMLScanner::sendCharData(XMLBuffer& toSend)
{
    // If no data in the buffer, then nothing to do
    if (toSend.isEmpty())
        return;

    //
    //  We do different things according to whether we are validating or
    //  not. If not, its always just characters; else, it depends on the
    //  current element's content model.
    //
    if (fValidate)
    {
        // Get the raw data we need for the callback
        const XMLCh* const rawBuf = toSend.getRawBuffer();
        const unsigned int len = toSend.getLen();

        // And see if the current element is a 'Children' style content model
        const ElemStack::StackElem* topElem = fElemStack.topElement();

        // Get the character data opts for the current element
        XMLElementDecl::CharDataOpts charOpts = topElem->fThisElement->getCharDataOpts();

        if (charOpts == XMLElementDecl::NoCharData)
        {
            // They definitely cannot handle any type of char data
            fValidator->emitError(XMLValid::NoCharDataInCM);
        }
         else if (XMLReader::isAllSpaces(rawBuf, len))
        {
            //
            //  Its all spaces. So, if they can take spaces, then send it
            //  as ignorable whitespace. If they can handle any char data
            //  send it as characters.
            //
            if (fDocHandler)
            {
                if (charOpts == XMLElementDecl::SpacesOk)
                    fDocHandler->ignorableWhitespace(rawBuf, len, false);
                else if (charOpts == XMLElementDecl::AllCharData)
                    fDocHandler->docCharacters(rawBuf, len, false);
            }
        }
         else
        {
            //
            //  If they can take any char data, then send it. Otherwise, they
            //  can only handle whitespace and can't handle this stuff so
            //  issue an error.
            //
            if (charOpts == XMLElementDecl::AllCharData)
            {
                if (fDocHandler)
                {
                    if (fGrammar->getGrammarType() != Grammar::SchemaGrammarType)
                    {
                        fDocHandler->docCharacters(rawBuf, len, false);
                    }
                    else
                    {
                        // The normalized data can only be as large as the
                        // original size, so this will avoid allocating way
                        // too much or too little memory.
                        XMLBuffer toFill(toSend.getLen()+1);

                        // normalize the character according to schema whitespace facet
                        DatatypeValidator* tempDV = ((SchemaElementDecl*) topElem->fThisElement)->getDatatypeValidator();
                        ((SchemaValidator*) fValidator)->normalizeWhiteSpace(tempDV, rawBuf, toFill);

                        fDocHandler->docCharacters(toFill.getRawBuffer(), toFill.getLen(), false);
                    }
                }
            }
             else
            {
                fValidator->emitError(XMLValid::NoCharDataInCM);
            }
        }
    }
     else
    {
        // Always assume its just char data if not validating
        if (fDocHandler)
            fDocHandler->docCharacters(toSend.getRawBuffer(), toSend.getLen(), false);
    }

    // Reset buffer
    toSend.reset();
}


//
//  This method will handle figuring out what the next top level token is
//  in the input stream. It will return an enumerated value that indicates
//  what it believes the next XML level token must be. It will eat as many
//  chars are required to figure out what is next.
//
XMLScanner::XMLTokens XMLScanner::senseNextToken(unsigned int& orgReader)
{
    //
    //  Get the next character and use it to guesstimate what the next token
    //  is going to be. We turn on end of entity exceptions when we do this
    //  in order to catch the scenario where the current entity ended at
    //  the > of some markup.
    //
    XMLCh nextCh;
    {
        ThrowEOEJanitor janMgr(&fReaderMgr, true);
        nextCh = fReaderMgr.peekNextChar();
    }


    //
    //  Check for special chars. Start with the most
    //  obvious end of file, which should be legal here at top level.
    //
    if (!nextCh)
        return Token_EOF;


    //
    //  If it's not a '<' we must be in content.
    //
    //  This includes entity references '&' of some sort. These must
    //  be character data because that's the only place a reference can
    //  occur in content.
    //
    if (nextCh != chOpenAngle)
        return Token_CharData;

    //
    //
    //  Ok it had to have been a '<' character. So get it out of the reader
    //  and store the reader number where we saw it, passing it back to the
    //  caller.
    //
    fReaderMgr.getNextChar();
    orgReader = fReaderMgr.getCurrentReaderNum();

    //
    //  Ok, so lets go through the things that it could be at this point which
    //  are all some form of markup.
    //
    nextCh = fReaderMgr.peekNextChar();

    if (nextCh == chForwardSlash)
    {
        fReaderMgr.getNextChar();
        return Token_EndTag;
    }
    else if (nextCh == chBang)
    {
        static const XMLCh gCDATAStr[] =
        {
                chBang, chOpenSquare, chLatin_C, chLatin_D, chLatin_A
            ,   chLatin_T, chLatin_A, chNull
        };

        static const XMLCh gCommentString[] =
        {
            chBang, chDash, chDash, chNull
        };

        if (fReaderMgr.skippedString(gCDATAStr))
            return Token_CData;

        if (fReaderMgr.skippedString(gCommentString))
            return Token_Comment;

        emitError(XMLErrs::ExpectedCommentOrCDATA);
        return Token_Unknown;
    }
     else if (nextCh == chQuestion)
    {
        // It must be a PI
        fReaderMgr.getNextChar();
        return Token_PI;
    }

    //
    //  Assume its an element name, so return with a start tag token. If it
    //  turns out not to be, then it will fail when it cannot get a valid tag.
    //
    return Token_StartTag;
}


//
//  This method is called with a key/value string pair that represents an
//  xmlns="xxx" or xmlns:xxx="yyy" attribute. This method will update the
//  current top of the element stack based on this data. We know that when
//  we get here, that it is one of these forms, so we don't bother confirming
//  it.
//
void XMLScanner::updateNSMap(const  XMLCh* const    attrName
                            , const XMLCh* const    attrValue)
{
    // We need a buffer to normalize the attribute value into
    XMLBufBid bbNormal(&fBufMgr);
    XMLBuffer& normalBuf = bbNormal.getBuffer();

    //  We either have the default prefix (""), or we point it into the attr
    //  name parameter. Note that the xmlns is not the prefix we care about
    //  here. To us, the 'prefix' is really the local part of the attrName
    //  parameter.
    //
    const XMLCh* prefPtr = XMLUni::fgZeroLenString;
    const unsigned int colonOfs = XMLString::indexOf(attrName, chColon);
    if (colonOfs != -1)
        prefPtr = &attrName[colonOfs + 1];

    //
    //  Normalize the value into the passed buffer. In this case, we don't
    //  care about the return value. An error was issued for the error, which
    //  is all we care about here.
    //
    normalizeAttRawValue(attrName, attrValue, normalBuf);

    //
    //  Ok, we have to get the unique id for the attribute value, which is the
    //  URI that this value should be mapped to. The validator has the
    //  namespace string pool, so we ask him to find or add this new one. Then
    //  we ask the element stack to add this prefix to URI Id mapping.
    //
    fElemStack.addPrefix
    (
        prefPtr
        , fURIStringPool->addOrFind(normalBuf.getRawBuffer())
    );
}

void XMLScanner::scanRawAttrListforNameSpaces(const RefVectorOf<KVStringPair>* theRawAttrList, int attCount) {

    //  Schema Xsi Type yyyy (e.g. xsi:type="yyyyy")
    XMLBuffer fXsiType;

    //
    //  Make an initial pass through the list and find any xmlns attributes or
    //  schema attributes.
    //  When we find one, send it off to be used to update the element stack's
    //  namespace mappings.
    //

    int index = 0;

    for (index = 0; index < attCount; index++)
    {
        // each attribute has the prefix:suffix="value"
        const KVStringPair* curPair = fRawAttrList->elementAt(index);
        const XMLCh* valuePtr = curPair->getValue();
        const XMLCh* rawPtr = curPair->getKey();

        QName attName(rawPtr, fEmptyNamespaceId);
        const XMLCh* suffPtr = attName.getLocalPart();

        //  If either the key begins with "xmlns:" or its just plain
        //  "xmlns", then use it to update the map.
        //
        if (!XMLString::compareNString(rawPtr, XMLUni::fgXMLNSColonString, 6)
        ||  !XMLString::compareString(rawPtr, XMLUni::fgXMLNSString))
        {
            updateNSMap(rawPtr, valuePtr);

            // if the schema URI is seen in the the valuePtr, set the boolean seeXsi
            if (!XMLString::compareString(valuePtr, SchemaSymbols::fgURI_XSI)) {
                fSeeXsi = true;
            }
        }
    }

    // walk through the list again to deal with "xsi:...."
    if (fDoSchema && fSeeXsi)
    {
        for (index = 0; index < attCount; index++)
        {
            // each attribute has the prefix:suffix="value"
            const KVStringPair* curPair = fRawAttrList->elementAt(index);
            const XMLCh* valuePtr = curPair->getValue();
            const XMLCh* rawPtr = curPair->getKey();

            QName attName(rawPtr, fEmptyNamespaceId);
            const XMLCh* prefPtr = attName.getPrefix();
            const XMLCh* suffPtr = attName.getLocalPart();

            // if schema URI has been seen, scan for the schema location and uri
            // and resolve the schema grammar; or scan for schema type
            if (resolvePrefix(prefPtr, ElemStack::Mode_Attribute) == fSchemaNamespaceId) {

                if (!fReuseGrammar) {
                    if (!XMLString::compareString(suffPtr, SchemaSymbols::fgXSI_SCHEMALOCACTION))
                        parseSchemaLocation(valuePtr);
                    else if (!XMLString::compareString(suffPtr, SchemaSymbols::fgXSI_NONAMESPACESCHEMALOCACTION))
                        resolveSchemaGrammar(valuePtr, XMLUni::fgZeroLenString);
                }

                if (!XMLString::compareString(suffPtr, SchemaSymbols::fgXSI_TYPE)) {
                        fXsiType.set(valuePtr);
                }
                else if (!XMLString::compareString(suffPtr, SchemaSymbols::fgATT_NILL)
                         && fValidator
                         && !XMLString::compareString(valuePtr, SchemaSymbols::fgATTVAL_TRUE)) {
                            ((SchemaValidator*)fValidator)->setNillable(true);
                }
            }
        }

        if (fValidator) {
            if (!fXsiType.isEmpty()) {
                unsigned int uriId = resolveQName (
                      fXsiType.getRawBuffer()
                    , fNameBuf
                    , fPrefixBuf
                    , ElemStack::Mode_Element
                );
                ((SchemaValidator*)fValidator)->setXsiType(fPrefixBuf.getRawBuffer(), fNameBuf.getRawBuffer(), uriId);
            }
        }
    }
}

void XMLScanner::parseSchemaLocation(const XMLCh* const schemaLocationStr)
{
    RefVectorOf<XMLCh>* schemaLocation = XMLString::tokenizeString(schemaLocationStr);
    unsigned int size = schemaLocation->size();
    if (size % 2 != 0 ) {
        emitError(XMLErrs::BadSchemaLocation);
    } else {
        for(unsigned int i=0; i<size; i=i+2) {
            resolveSchemaGrammar(schemaLocation->elementAt(i+1), schemaLocation->elementAt(i));
        }
    }

    delete schemaLocation;
}

void XMLScanner::resolveSchemaGrammar(const XMLCh* const loc, const XMLCh* const uri) {

    //
    //  Since we have seen a grammar, set our validation flag
    //  at this point if the validation scheme is auto
    //
    if (fValScheme == Val_Auto) {
        fValidate = true;
        fElemStack.setValidationFlag(fValidate);
    }

    // we have seen a schema, so set up the fValidator as fSchemaValidator
    if (!fValidator->handlesSchema())
    {
        if (fValidatorFromUser) {
            // the fValidator is from user
            ThrowXML(RuntimeException, XMLExcepts::Gen_NoSchemaValidator);
        }
        else {
            fValidator = fSchemaValidator;
        }
    }

    Grammar* grammar = fGrammarResolver->getGrammar(uri);

    if (!grammar || grammar->getGrammarType() == Grammar::DTDGrammarType) {
        DOMParser parser;
        XMLInternalErrorHandler internalErrorHandler(fErrorHandler);
        parser.setValidationScheme(DOMParser::Val_Never);
        parser.setDoNamespaces(true);
        parser.setErrorHandler((ErrorHandler*) &internalErrorHandler);
        parser.setEntityResolver(fEntityResolver);

        // Create a buffer for expanding the system id
        XMLBuffer expSysId;

        //
        //  Allow the entity handler to expand the system id if they choose
        //  to do so.
        //
        if (fEntityHandler)
        {
            if (!fEntityHandler->expandSystemId(loc, expSysId))
                expSysId.set(loc);
        }
         else
        {
            expSysId.set(loc);
        }

        // Call the entity resolver interface to get an input source
        InputSource* srcToFill = 0;
        if (fEntityResolver)
        {
            srcToFill = fEntityResolver->resolveEntity
            (
                XMLUni::fgZeroLenString
                , expSysId.getRawBuffer()
            );
        }

        //
        //  If they didn't create a source via the entity resolver, then we
        //  have to create one on our own.
        //
        if (!srcToFill)
        {
            ReaderMgr::LastExtEntityInfo lastInfo;
            fReaderMgr.getLastExtEntityInfo(lastInfo);

            try
            {
                XMLURL urlTmp(lastInfo.systemId, expSysId.getRawBuffer());
                if (urlTmp.isRelative())
                {
                    ThrowXML
                    (
                        MalformedURLException
                        , XMLExcepts::URL_NoProtocolPresent
                    );
                }
                srcToFill = new URLInputSource(urlTmp);
            }

            catch(const MalformedURLException&)
            {
                // Its not a URL, so lets assume its a local file name.
                srcToFill = new LocalFileInputSource
                (
                    lastInfo.systemId
                    , expSysId.getRawBuffer()
                );
            }
        }

        // Put a janitor on the input source
        Janitor<InputSource> janSrc(srcToFill);

        parser.parse( *srcToFill) ;
        if (internalErrorHandler.getSawFatal() && fExitOnFirstFatal)
            emitError(XMLErrs::SchemaScanFatalError);

        DOM_Document  document = parser.getDocument(); //Our Grammar

        DOM_Element root = document.getDocumentElement();// This is what we pass to TraverserSchema
        if (fValidate && root.isNull()) {
            fValidator->emitError(XMLValid::SchemaRootError, loc);
        }
        else
        {
            if (fValidate && (!uri || !root.getAttribute(SchemaSymbols::fgATT_TARGETNAMESPACE).equals(uri)))
                fValidator->emitError(XMLValid::WrongTargetNamespace, loc, uri);

            grammar = new SchemaGrammar();
            TraverseSchema traverseSchema(root, fURIStringPool, (SchemaGrammar*) grammar, fGrammarResolver, this, fValidator, srcToFill->getSystemId(), fEntityResolver, fErrorHandler);
        }
    }
    fGrammar = grammar;
    fValidator->setGrammar(fGrammar);
}

// ---------------------------------------------------------------------------
//  XMLScanner: Private parsing methods
// ---------------------------------------------------------------------------


//
//  This guy just scans out a single or double quoted string of characters.
//  It does not pass any judgement on the contents and assumes that it is
//  illegal to have another quote of the same kind inside the string's
//  contents.
//
//  NOTE: This is for simple stuff like the strings in the XMLDecl which
//  cannot have any entities inside them. So this guy does not handle any
//  end of entity stuff.
//
bool XMLScanner::getQuotedString(XMLBuffer& toFill)
{
    // Reset the target buffer
    toFill.reset();

    // Get the next char which must be a single or double quote
    XMLCh quoteCh;
    if (!fReaderMgr.skipIfQuote(quoteCh))
        return false;

    while (true)
    {
        // Get another char
        const XMLCh nextCh = fReaderMgr.getNextChar();

        // See if it matches the starting quote char
        if (nextCh == quoteCh)
            break;

        //
        //  We should never get either an end of file null char here. If we
        //  do, just fail. It will be handled more gracefully in the higher
        //  level code that called us.
        //
        if (!nextCh)
            return false;

        // Else add it to the buffer
        toFill.append(nextCh);
    }
    return true;
}


//
//  This method is called to do a raw scan of an attribute value. It does not
//  do normalization (since we don't know their types yet.) It just scans the
//  value and does entity expansion.
//
//  End of entity's must be dealt with here. During DTD scan, they can come
//  from external entities. During content, they can come from any entity.
//  We just eat the end of entity and continue with our scan until we come
//  to the closing quote. If an unterminated value causes us to go through
//  subsequent entities, that will cause errors back in the calling code,
//  but there's little we can do about it here.
//
bool XMLScanner::basicAttrValueScan(const XMLCh* const attrName, XMLBuffer& toFill)
{
    // Reset the target buffer
    toFill.reset();

    // Get the next char which must be a single or double quote
    XMLCh quoteCh;
    if (!fReaderMgr.skipIfQuote(quoteCh))
        return false;

    //
    //  We have to get the current reader because we have to ignore closing
    //  quotes until we hit the same reader again.
    //
    const unsigned int curReader = fReaderMgr.getCurrentReaderNum();

    //
    //  Loop until we get the attribute value. Note that we use a double
    //  loop here to avoid the setup/teardown overhead of the exception
    //  handler on every round.
    //
    XMLCh   nextCh;
    XMLCh   secondCh = 0;
    bool    gotLeadingSurrogate = false;
    bool    escaped;
    while (true)
    {
        try
        {
            while(true)
            {
                // Get another char. Use second char if one is waiting
                if (secondCh)
                {
                    nextCh = secondCh;
                    secondCh = 0;
                }
                 else
                {
                    nextCh = fReaderMgr.getNextChar();
                }

                if (!nextCh)
                    ThrowXML(UnexpectedEOFException, XMLExcepts::Gen_UnexpectedEOF);

                //
                //  Check for our ending quote. It has to be in the same entity
                //  as where we started. Quotes in nested entities are ignored.
                //
                if (nextCh == quoteCh)
                {
                    if (curReader == fReaderMgr.getCurrentReaderNum())
                        return true;

                    // Watch for spillover into a previous entity
                    if (curReader > fReaderMgr.getCurrentReaderNum())
                    {
                        emitError(XMLErrs::PartialMarkupInEntity);
                        return false;
                    }
                }

                //
                //  Check for an entity ref . We ignore the empty flag in
                //  this one.
                //
                escaped = false;
                if (nextCh == chAmpersand)
                {
                    // If it was not returned directly, then jump back up
                    if (scanEntityRef(true, nextCh, secondCh, escaped) != EntityExp_Returned)
                    {
                        gotLeadingSurrogate = false;
                        continue;
                    }
                }

                // Deal with surrogate pairs
                if ((nextCh >= 0xD800) && (nextCh <= 0xDBFF))
                {
                    //
                    //  Its a leading surrogate. If we already got one, then
                    //  issue an error, else set leading flag to make sure that
                    //  we look for a trailing next time.
                    //
                    if (gotLeadingSurrogate)
                    {
                        emitError(XMLErrs::Expected2ndSurrogateChar);
                    }
                     else
                    {
                        // Its got to at least be a valid XML character
                        if (!XMLReader::isXMLChar(nextCh))
                        {
                            XMLCh tmpBuf[9];
                            XMLString::binToText
                            (
                                nextCh
                                , tmpBuf
                                , 8
                                , 16
                            );
                            emitError(XMLErrs::InvalidCharacterInAttrValue, attrName, tmpBuf);
                        }
                        gotLeadingSurrogate = true;
                    }
                }
                 else
                {
                    //
                    //  If its a trailing surrogate, make sure that we are
                    //  prepared for that. Else, its just a regular char so make
                    //  sure that we were not expected a trailing surrogate.
                    //
                    if ((nextCh >= 0xDC00) && (nextCh <= 0xDFFF))
                    {
                        // Its trailing, so make sure we were expecting it
                        if (!gotLeadingSurrogate)
                            emitError(XMLErrs::Unexpected2ndSurrogateChar);
                    }
                     else
                    {
                        //
                        //  Its just a char, so make sure we were not expecting a
                        //  trailing surrogate.
                        //
                        if (gotLeadingSurrogate)
                            emitError(XMLErrs::Expected2ndSurrogateChar);
                    }
                    gotLeadingSurrogate = false;
                }

                //
                //  If it was escaped, then put in a 0xFFFF value. This will
                //  be used later during validation and normalization of the
                //  value to know that the following character was via an
                //  escape char.
                //
                if (escaped)
                    toFill.append(0xFFFF);

                // Else add it to the buffer
                toFill.append(nextCh);
            }
        }

        catch(const EndOfEntityException&)
        {
            // Just eat it and continue.
            gotLeadingSurrogate = false;
            escaped = false;
        }
    }
    return true;
}


bool XMLScanner::scanAttValue(  const   XMLAttDef* const    attDef
                                ,       XMLBuffer&          toFill)
{
    enum States
    {
        InWhitespace
        , InContent
    };

    // Get the type and name
    const XMLAttDef::AttTypes type = attDef->getType();
    const XMLCh* const attrName = attDef->getFullName();

    // Reset the target buffer
    toFill.reset();

    // Get the next char which must be a single or double quote
    XMLCh quoteCh;
    if (!fReaderMgr.skipIfQuote(quoteCh))
        return false;

    //
    //  We have to get the current reader because we have to ignore closing
    //  quotes until we hit the same reader again.
    //
    const unsigned int curReader = fReaderMgr.getCurrentReaderNum();

    //
    // Get attribute def - to check to see if it's declared externally or not
    //
    bool  isAttExternal = attDef->isExternal();

    //
    //  Loop until we get the attribute value. Note that we use a double
    //  loop here to avoid the setup/teardown overhead of the exception
    //  handler on every round.
    //
    XMLCh   nextCh;
    XMLCh   secondCh = 0;
    States  curState = InContent;
    bool    firstNonWS = false;
    bool    gotLeadingSurrogate = false;
    bool    escaped;
    while (true)
    {
    try
    {
        while(true)
        {
            // Get another char. Use second char if one is waiting
            if (secondCh)
            {
                nextCh = secondCh;
                secondCh = 0;
            }
             else
            {
                nextCh = fReaderMgr.getNextChar();
            }

            if (!nextCh)
                ThrowXML(UnexpectedEOFException, XMLExcepts::Gen_UnexpectedEOF);

            // Its got to at least be a valid XML character
            if (!XMLReader::isXMLChar(nextCh))
            {
                XMLCh tmpBuf[9];
                XMLString::binToText
                (
                    nextCh
                    , tmpBuf
                    , 8
                    , 16
                );
                emitError(XMLErrs::InvalidCharacterInAttrValue, attrName, tmpBuf);
            }

            // Check for our ending quote in the same entity
            if (nextCh == quoteCh)
            {
                if (curReader == fReaderMgr.getCurrentReaderNum())
                    return true;

                // Watch for spillover into a previous entity
                if (curReader > fReaderMgr.getCurrentReaderNum())
                {
                    emitError(XMLErrs::PartialMarkupInEntity);
                    return false;
                }
            }

            //
            //  Check for an entity ref now, before we let it affect our
            //  whitespace normalization logic below. We ignore the empty flag
            //  in this one.
            //
            escaped = false;
            if (nextCh == chAmpersand)
            {
                if (scanEntityRef(true, nextCh, secondCh, escaped) != EntityExp_Returned)
                {
                    gotLeadingSurrogate = false;
                    continue;
                }
            }

            // Deal with surrogate pairs
            if ((nextCh >= 0xD800) && (nextCh <= 0xDBFF))
            {
                //
                //  Its a leading surrogate. If we already got one, then
                //  issue an error, else set leading flag to make sure that
                //  we look for a trailing next time.
                //
                if (gotLeadingSurrogate)
                    emitError(XMLErrs::Expected2ndSurrogateChar);
                 else
                    gotLeadingSurrogate = true;
            }
             else
            {
                //
                //  If its a trailing surrogate, make sure that we are
                //  prepared for that. Else, its just a regular char so make
                //  sure that we were not expected a trailing surrogate.
                //
                if ((nextCh >= 0xDC00) && (nextCh <= 0xDFFF))
                {
                    // Its trailing, so make sure we were expecting it
                    if (!gotLeadingSurrogate)
                        emitError(XMLErrs::Unexpected2ndSurrogateChar);
                }
                 else
                {
                    //
                    //  Its just a char, so make sure we were not expecting a
                    //  trailing surrogate.
                    //
                    if (gotLeadingSurrogate)
                        emitError(XMLErrs::Expected2ndSurrogateChar);
                }
                gotLeadingSurrogate = false;
            }

            //
            //  If its not escaped, then make sure its not a < character, which
            //  is not allowed in attribute values.
            //
            if (!escaped && (nextCh == chOpenAngle))
                emitError(XMLErrs::BracketInAttrValue, attrName);

            //
            //  If the attribute is a CDATA type we do simple replacement of
            //  tabs and new lines with spaces, if the character is not escaped
            //  by way of a char ref.
            //
            //  Otherwise, we do the standard non-CDATA normalization of
            //  compressing whitespace to single spaces and getting rid of leading
            //  and trailing whitespace.
            //
            if (type == XMLAttDef::CData)
            {
                if (!escaped)
                {
                    if ((nextCh == 0x09) || (nextCh == 0x0A) || (nextCh == 0x0D))
                    {
                        //
                        // Check Validity Constraint for Standalone document declaration
                        // XML 1.0, Section 2.9
                        //
                        if (fStandalone && fValidate && isAttExternal)
                        {
                             //
                             // Can't have a standalone document declaration of "yes" if  attribute
                             // values are subject to normalisation
                             //
                             fValidator->emitError(XMLValid::NoAttNormForStandalone, attrName);
                        }
                        nextCh = chSpace;
                    }
                }
            }
             else
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
                        continue;
                    }
                }
                 else if (curState == InContent)
                {
                    if (XMLReader::isWhitespace(nextCh))
                    {
                        curState = InWhitespace;
                        //
                        // Check Validity Constraint for Standalone document declaration
                        // XML 1.0, Section 2.9
                        //
                        if (fStandalone && fValidate && isAttExternal)
                        {
                            if (!firstNonWS || (nextCh != chSpace) || (fReaderMgr.lookingAtSpace()))
                            {
                                 //
                                 // Can't have a standalone document declaration of "yes" if  attribute
                                 // values are subject to normalisation
                                 //
                                 fValidator->emitError(XMLValid::NoAttNormForStandalone, attrName);
                            }
                        }
                        continue;
                    }
                    firstNonWS = true;
                }
            }

            // Else add it to the buffer
            toFill.append(nextCh);
        }
    }

    catch(const EndOfEntityException&)
    {
        // Just eat it and continue.
        gotLeadingSurrogate = false;
        escaped = false;
    }
    }
    return true;
}


//
//  This method scans a CDATA section. It collects the character into one
//  of the temp buffers and calls the document handler, if any, with the
//  characters. It assumes that the <![CDATA string has been scanned before
//  this call.
//
void XMLScanner::scanCDSection()
{
    //
    //  This is the CDATA section opening sequence, minus the '<' character.
    //  We use this to watch for nested CDATA sections, which are illegal.
    //
    static const XMLCh CDataPrefix[] =
    {
            chBang, chOpenSquare, chLatin_C, chLatin_D, chLatin_A
        ,   chLatin_T, chLatin_A, chOpenSquare, chNull
    };

    static const XMLCh CDataClose[] =
    {
            chCloseSquare, chCloseAngle, chNull
    };


    //
    //  The next character should be the opening square bracket. If not
    //  issue an error, but then try to recover by skipping any whitespace
    //  and checking again.
    //
    if (!fReaderMgr.skippedChar(chOpenSquare))
    {
        emitError(XMLErrs::ExpectedOpenSquareBracket);
        fReaderMgr.skipPastSpaces();

        // If we still don't find it, then give up, else keep going
        if (!fReaderMgr.skippedChar(chOpenSquare))
            return;
    }

    // Get a buffer for this
    XMLBufBid bbCData(&fBufMgr);

    //
    //  We just scan forward until we hit the end of CDATA section sequence.
    //  CDATA is effectively a big escape mechanism so we don't treat markup
    //  characters specially here.
    //
    bool            emittedError = false;
    while (true)
    {
        const XMLCh nextCh = fReaderMgr.getNextChar();

        // Watch for unexpected end of file
        if (!nextCh)
        {
            emitError(XMLErrs::UnterminatedCDATASection);
            ThrowXML(UnexpectedEOFException, XMLExcepts::Gen_UnexpectedEOF);
        }

        if (fValidate && fStandalone && (XMLReader::isWhitespace(nextCh)))
        {
            //
            // This document is standalone; this ignorable CDATA whitespace is forbidden.
            // XML 1.0, Section 2.9
            //

            // And see if the current element is a 'Children' style content model
            const ElemStack::StackElem* topElem = fElemStack.topElement();

            if (topElem->fThisElement->isExternal()) {

                // Get the character data opts for the current element
                XMLElementDecl::CharDataOpts charOpts =  topElem->fThisElement->getCharDataOpts();

                if (charOpts == XMLElementDecl::SpacesOk) // Element Content
                {
                    // Error - standalone should have a value of "no" as whitespace detected in an
                    // element type with element content whose element declaration was external
                    //
                    fValidator->emitError(XMLValid::NoWSForStandalone);
                }
            }
        }

        //
        //  If this is a close square bracket it could be our closing
        //  sequence.
        //
        if (nextCh == chCloseSquare && fReaderMgr.skippedString(CDataClose))
        {
            // If we have a doc handler, call it
            if (fDocHandler)
            {
                fDocHandler->docCharacters
                    (
                    bbCData.getRawBuffer()
                    , bbCData.getLen()
                    , true
                    );
            }

            // And we are done
            break;
        }

        //
        //  Make sure its a valid character. But if we've emitted an error
        //  already, don't bother with the overhead since we've already told
        //  them about it.
        //
        if (!emittedError)
        {
            if (!XMLReader::isXMLChar(nextCh))
            {
                XMLCh tmpBuf[9];
                XMLString::binToText
                (
                    nextCh
                    , tmpBuf
                    , 8
                    , 16
                );
                emitError(XMLErrs::InvalidCharacter, tmpBuf);
                emittedError = true;
            }
        }

        // Add it to the buffer
        bbCData.append(nextCh);
    }
}


void XMLScanner::scanCharData(XMLBuffer& toUse)
{
    //
    //  We have to watch for the stupid ]]> sequence, which is illegal in
    //  character data. So this is a little state machine that handles that.
    //
    enum States
    {
        State_Waiting
        , State_GotOne
        , State_GotTwo
    };

    // Reset the buffer before we start
    toUse.reset();

    // Turn on the 'throw at end' flag of the reader manager
    ThrowEOEJanitor jan(&fReaderMgr, true);

    //
    //  In order to be more efficient we have to use kind of a deeply nested
    //  set of blocks here. The outer block puts on a try and catches end of
    //  entity exceptions. The inner loop is the per-character loop. If we
    //  put the try inside the inner loop, it would work but would require
    //  the exception handling code setup/teardown code to be invoked for
    //  each character.
    //
    XMLCh   nextCh;
    XMLCh   secondCh = 0;
    States  curState = State_Waiting;
    bool    escaped = false;
    bool    gotLeadingSurrogate = false;
    bool    notDone = true;
    while (notDone)
    {
        try
        {
            while (true)
            {
                if (secondCh)
                {
                    nextCh = secondCh;
                    secondCh = 0;
                }
                 else
                {
                     //  Eat through as many plain content characters as possible without
                     //  needing special handling.  Moving most content characters here,
                     //  in this one call, rather than running the overall loop once
                     //  per content character, is a speed optimization.
                     //
                    if (curState == State_Waiting  &&  !gotLeadingSurrogate)
                    {
                         fReaderMgr.movePlainContentChars(toUse);
                    }


                    // Try to get another char from the source
                    //   The code from here on down covers all contengencies,
                    //
                    if (!fReaderMgr.getNextCharIfNot(chOpenAngle, nextCh))
                    {
                        // If we were waiting for a trailing surrogate, its an error
                        if (gotLeadingSurrogate)
                            emitError(XMLErrs::Expected2ndSurrogateChar);

                        notDone = false;
                        break;
                    }
                }

                //
                //  Watch for a reference. Note that the escapement mechanism
                //  is ignored in this content.
                //
                if (nextCh == chAmpersand)
                {
                    sendCharData(toUse);

                    // Turn off the throwing at the end of entity during this
                    ThrowEOEJanitor jan(&fReaderMgr, false);

                    if (scanEntityRef(false, nextCh, secondCh, escaped) != EntityExp_Returned)
                    {
                        gotLeadingSurrogate = false;
                        continue;
                    }
                }
                 else
                {
                    escaped = false;
                }

                 // Keep the state machine up to date
                if (!escaped)
                {
                    if (nextCh == chCloseSquare)
                    {
                        if (curState == State_Waiting)
                            curState = State_GotOne;
                        else if (curState == State_GotOne)
                            curState = State_GotTwo;
                    }
                     else if (nextCh == chCloseAngle)
                    {
                        if (curState == State_GotTwo)
                            emitError(XMLErrs::BadSequenceInCharData);
                        curState = State_Waiting;
                    }
                     else
                    {
                        curState = State_Waiting;
                    }
                }
                 else
                {
                    curState = State_Waiting;
                }

                // Deal with surrogate pairs
                if ((nextCh >= 0xD800) && (nextCh <= 0xDBFF))
                {
                    //
                    //  Its a leading surrogate. If we already got one, then
                    //  issue an error, else set leading flag to make sure that
                    //  we look for a trailing next time.
                    //
                    if (gotLeadingSurrogate)
                        emitError(XMLErrs::Expected2ndSurrogateChar);
                    else
                        gotLeadingSurrogate = true;
                }
                 else
                {
                    //
                    //  If its a trailing surrogate, make sure that we are
                    //  prepared for that. Else, its just a regular char so make
                    //  sure that we were not expected a trailing surrogate.
                    //
                    if ((nextCh >= 0xDC00) && (nextCh <= 0xDFFF))
                    {
                        // Its trailing, so make sure we were expecting it
                        if (!gotLeadingSurrogate)
                            emitError(XMLErrs::Unexpected2ndSurrogateChar);
                    }
                     else
                    {
                        //
                        //  Its just a char, so make sure we were not expecting a
                        //  trailing surrogate.
                        //
                        if (gotLeadingSurrogate)
                            emitError(XMLErrs::Expected2ndSurrogateChar);

                        // Make sure the returned char is a valid XML char
                        if (!XMLReader::isXMLChar(nextCh))
                        {
                            XMLCh tmpBuf[9];
                            XMLString::binToText
                            (
                                nextCh
                                , tmpBuf
                                , 8
                                , 16
                            );
                            emitError(XMLErrs::InvalidCharacter, tmpBuf);
                        }
                    }
                    gotLeadingSurrogate = false;
                }

                // Add this char to the buffer
                toUse.append(nextCh);
            }
        }

        catch(const EndOfEntityException& toCatch)
        {
            //
            //  Some entity ended, so we have to send any accumulated
            //  chars and send an end of entity event.
            //
            sendCharData(toUse);
            gotLeadingSurrogate = false;

            if (fDocHandler)
                fDocHandler->endEntityReference(toCatch.getEntity());
        }
    }
    //
    // Check the validity constraints as per XML 1.0 Section 2.9
    //

    const XMLCh* rawBuf = toUse.getRawBuffer();
    const unsigned int len = toUse.getLen();

    if (fValidate && fStandalone)
    {
        // See if the text contains whitespace
        // Get the raw data we need for the callback
        const bool isSpaces = XMLReader::containsWhiteSpace(rawBuf, len);
        if (isSpaces)
        {
            // And see if the current element is a 'Children' style content model
            const ElemStack::StackElem* topElem = fElemStack.topElement();

            if (topElem->fThisElement->isExternal()) {

                // Get the character data opts for the current element
                XMLElementDecl::CharDataOpts charOpts =  topElem->fThisElement->getCharDataOpts();

                if (charOpts == XMLElementDecl::SpacesOk)  // => Element Content
                {
                    // Error - standalone should have a value of "no" as whitespace detected in an
                    // element type with element content whose element declaration was external
                    //
                    fValidator->emitError(XMLValid::NoWSForStandalone);
                }
            }
        }
    }
    // Send any char data that we accumulated into the buffer
    sendCharData(toUse);
}


//
//  This method scans a character reference and returns the character that
//  was refered to. It assumes that we've already scanned the &# characters
//  that prefix the numeric code.
//
bool XMLScanner::scanCharRef(XMLCh& toFill, XMLCh& second)
{
    bool gotOne = false;
    unsigned int value = 0;

    //
    //  Set the radix. Its supposed to be a lower case x if hex. But, in
    //  order to recover well, we check for an upper and put out an error
    //  for that.
    //
    unsigned int radix = 10;
    if (fReaderMgr.skippedChar(chLatin_x))
    {
        radix = 16;
    }
     else if (fReaderMgr.skippedChar(chLatin_X))
    {
        emitError(XMLErrs::HexRadixMustBeLowerCase);
        radix = 16;
    }

    while (true)
    {
        const XMLCh nextCh = fReaderMgr.peekNextChar();

        // Watch for EOF
        if (!nextCh)
            ThrowXML(UnexpectedEOFException, XMLExcepts::Gen_UnexpectedEOF);

        // Break out on the terminating semicolon
        if (nextCh == chSemiColon)
        {
            fReaderMgr.getNextChar();
            break;
        }

        //
        //  Convert this char to a binary value, or bail out if its not
        //  one.
        //
        unsigned int nextVal;
        if ((nextCh >= chDigit_0) && (nextCh <= chDigit_9))
            nextVal = (unsigned int)(nextCh - chDigit_0);
         else if ((nextCh >= chLatin_A) && (nextCh <= chLatin_F))
            nextVal= (unsigned int)(10 + (nextCh - chLatin_A));
         else if ((nextCh >= chLatin_a) && (nextCh <= chLatin_f))
            nextVal = (unsigned int)(10 + (nextCh - chLatin_a));
         else
        {
            // Return a zero
            toFill = 0;

            //
            //  If we got at least a sigit, then do an unterminated ref error.
            //  Else, do an expected a numerical ref thing.
            //
            if (gotOne)
                emitError(XMLErrs::UnterminatedCharRef);
            else
                emitError(XMLErrs::ExpectedNumericalCharRef);

            // Return failure
            return false;
        }

        //
        //  Make sure its valid for the radix. If not, then just eat the
        //  digit and go on after issueing an error. Else, update the
        //  running value with this new digit.
        //
        if (nextVal >= radix)
        {
            XMLCh tmpStr[2];
            tmpStr[0] = nextCh;
            tmpStr[1] = chNull;
            emitError(XMLErrs::BadDigitForRadix, tmpStr);
        }
         else
        {
            value = (value * radix) + nextVal;
        }

        // Indicate that we got at least one good digit
        gotOne = true;

        // And eat the last char
        fReaderMgr.getNextChar();
    }

    //
    //  [2] Char ::= #x9 | #xA | #xD | [#x20-#xD7FF]        // any Unicode character, excluding the
    //               | [#xE000-#xFFFD] | [#x10000-#x10FFFF] // surrogate blocks, FFFE, and FFFF.
    //
    bool validChar = false;
    if (value < 0x20)
    {
      if (value == 0x09 || value == 0x0A || value == 0x0D)
      {
          validChar = true;
      }
    }
    else if (value <= 0xD7FF || (value >= 0xE000 && (value <= 0xFFFD || (value >= 0x10000 && value <= 0x10FFFF))))
    {
          validChar = true;
    }

    if (!validChar)
    {
      //
      // Character reference was not in the valid range
      emitError(XMLErrs::InvalidCharacterRef);
      return false;
      }

    // Return the char (or chars)
    if (value >= 0x10000)
    {
        value -= 0x10000;
        toFill = XMLCh((value >> 10) + 0xD800);
        second = XMLCh((value & 0x3FF) + 0xDC00);
    }
     else
    {
        toFill = XMLCh(value);
        second = 0;
    }

    return true;
}


//
//  We get here after the '<!--' part of the comment. We scan past the
//  terminating '-->' It will calls the appropriate handler with the comment
//  text, if one is provided. A comment can be in either the document or
//  the DTD, so the fInDocument flag is used to know which handler to send
//  it to.
//
void XMLScanner::scanComment()
{
    enum States
    {
        InText
        , OneDash
        , TwoDashes
    };

    // Get a buffer for this
    XMLBufBid bbComment(&fBufMgr);

    //
    //  Get the comment text into a temp buffer. Be sure to use temp buffer
    //  two here, since its to be used for stuff that is potentially longer
    //  than just a name.
    //
    States curState = InText;
    while (true)
    {
        // Get the next character
        const XMLCh nextCh = fReaderMgr.getNextChar();

        //  Watch for an end of file
        if (!nextCh)
        {
            emitError(XMLErrs::UnterminatedComment);
            ThrowXML(UnexpectedEOFException, XMLExcepts::Gen_UnexpectedEOF);
        }

        // Make sure its a valid XML character
        if (!XMLReader::isXMLChar(nextCh))
        {
            XMLCh tmpBuf[9];
            XMLString::binToText
            (
                nextCh
                , tmpBuf
                , 8
                , 16
            );
            emitError(XMLErrs::InvalidCharacter, tmpBuf);
        }

        if (curState == InText)
        {
            // If its a dash, go to OneDash state. Otherwise take as text
            if (nextCh == chDash)
                curState = OneDash;
            else
                bbComment.append(nextCh);
        }
         else if (curState == OneDash)
        {
            //
            //  If its another dash, then we change to the two dashes states.
            //  Otherwise, we have to put in the deficit dash and the new
            //  character and go back to InText.
            //
            if (nextCh == chDash)
            {
                curState = TwoDashes;
            }
             else
            {
                bbComment.append(chDash);
                bbComment.append(nextCh);
                curState = InText;
            }
        }
         else if (curState == TwoDashes)
        {
            // The next character must be the closing bracket
            if (nextCh != chCloseAngle)
            {
                emitError(XMLErrs::IllegalSequenceInComment);
                fReaderMgr.skipPastChar(chCloseAngle);
                return;
            }
            break;
        }
    }

    // If we have an available handler, call back with the comment.
    if (fDocHandler)
    {
        fDocHandler->docComment
        (
            bbComment.getRawBuffer()
        );
    }
}


//
//  Most equal signs can have white space around them, so this little guy
//  just makes the calling code cleaner by eating whitespace.
//
bool XMLScanner::scanEq()
{
    fReaderMgr.skipPastSpaces();
    if (fReaderMgr.skippedChar(chEqual))
    {
        fReaderMgr.skipPastSpaces();
        return true;
    }
    return false;
}


//
//  This method will scan a general/character entity ref. It will either
//  expand a char ref and return it directly, or push a reader for a general
//  entity.
//
//  The return value indicates whether the char parameters hold the value
//  or whether the value was pushed as a reader, or that it failed.
//
//  The escaped flag tells the caller whether the returned parameter resulted
//  from a character reference, which escapes the character in some cases. It
//  only makes any difference if the return value indicates the value was
//  returned directly.
//
XMLScanner::EntityExpRes
XMLScanner::scanEntityRef(  const   bool    inAttVal
                            ,       XMLCh&  firstCh
                            ,       XMLCh&  secondCh
                            ,       bool&   escaped)
{
    // Assume no escape
    secondCh = 0;
    escaped = false;

    // We have to insure that its all in one entity
    const unsigned int curReader = fReaderMgr.getCurrentReaderNum();

    //
    //  If the next char is a pound, then its a character reference and we
    //  need to expand it always.
    //
    if (fReaderMgr.skippedChar(chPound))
    {
        //
        //  Its a character reference, so scan it and get back the numeric
        //  value it represents.
        //
        if (!scanCharRef(firstCh, secondCh))
            return EntityExp_Failed;

        escaped = true;

        if (curReader != fReaderMgr.getCurrentReaderNum())
            emitError(XMLErrs::PartialMarkupInEntity);

        return EntityExp_Returned;
    }

    // Expand it since its a normal entity ref
    XMLBufBid bbName(&fBufMgr);
    if (!fReaderMgr.getName(bbName.getBuffer()))
    {
        emitError(XMLErrs::ExpectedEntityRefName);
        return EntityExp_Failed;
    }

    //
    //  Next char must be a semi-colon. But if its not, just emit
    //  an error and try to continue.
    //
    if (!fReaderMgr.skippedChar(chSemiColon))
        emitError(XMLErrs::UnterminatedEntityRef, bbName.getRawBuffer());

    // Make sure we ended up on the same entity reader as the & char
    if (curReader != fReaderMgr.getCurrentReaderNum())
        emitError(XMLErrs::PartialMarkupInEntity);

    // Look up the name in the general entity pool
    XMLEntityDecl* decl = fEntityDeclPool->getByKey(bbName.getRawBuffer());

    // If it does not exist, then obviously an error
    if (!decl)
    {
        // XML 1.0 Section 4.1
        // Well-formedness Constraint for entity not found:
        //   In a document without any DTD, a document with only an internal DTD subset which contains no parameter entity references,
        //      or a document with "standalone='yes'", for an entity reference that does not occur within the external subset
        //      or a parameter entity
        //
        // Else it's Validity Constraint
        //
        if (fStandalone || fHasNoDTD)
            emitError(XMLErrs::EntityNotFound, bbName.getRawBuffer());
        else {
            if (fValidate)
                fValidator->emitError(XMLValid::VC_EntityNotFound, bbName.getRawBuffer());
        }

        return EntityExp_Failed;
    }

    //
    // XML 1.0 Section 2.9
    //  If we are a standalone document, then it has to have been declared
    //  in the internal subset. Keep going though.
    //
    if (fStandalone && !decl->getDeclaredInIntSubset() && fValidate)
        fValidator->emitError(XMLValid::IllegalRefInStandalone, bbName.getRawBuffer());

    if (decl->isExternal())
    {
        // If its unparsed, then its not valid here
        if (decl->isUnparsed())
        {
            emitError(XMLErrs::NoUnparsedEntityRefs, bbName.getRawBuffer());
            return EntityExp_Failed;
        }

        // If we are in an attribute value, then not valid but keep going
        if (inAttVal)
            emitError(XMLErrs::NoExtRefsInAttValue);

        // And now create a reader to read this entity
        InputSource* srcUsed;
        XMLReader* reader = fReaderMgr.createReader
        (
            decl->getSystemId()
            , decl->getPublicId()
            , false
            , XMLReader::RefFrom_NonLiteral
            , XMLReader::Type_General
            , XMLReader::Source_External
            , srcUsed
        );

        // Put a janitor on the source so it gets cleaned up on exit
        Janitor<InputSource> janSrc(srcUsed);

        //
        //  If the creation failed, and its not because the source was empty,
        //  then emit an error and return.
        //
        if (!reader)
            ThrowXML1(RuntimeException, XMLExcepts::Gen_CouldNotOpenExtEntity, srcUsed->getSystemId());

        //
        //  Push the reader. If its a recursive expansion, then emit an error
        //  and return an failure.
        //
        if (!fReaderMgr.pushReader(reader, decl))
        {
            emitError(XMLErrs::RecursiveEntity, decl->getName());
            return EntityExp_Failed;
        }

        //
        //  Do a start entity reference event.
        //
        //  <TBD> For now, we supress them in att values. Later, when
        //  the stuff is in place to correctly allow DOM to handle them
        //  we'll turn this back on.
        //
        if (fDocHandler && !inAttVal)
            fDocHandler->startEntityReference(*decl);

        // If it starts with the XML string, then parse a text decl
        if (checkXMLDecl(true))
            scanXMLDecl(Decl_Text);
    }
     else
    {
        //
        //  If its one of the special char references, then we can return
        //  it as a character, and its considered escaped.
        //
        if (decl->getIsSpecialChar())
        {
            firstCh = decl->getValue()[0];
            escaped = true;
            return EntityExp_Returned;
        }

        //
        //  Create a reader over a memory stream over the entity value
        //  We force it to assume UTF-16 by passing in an encoding
        //  string. This way it won't both trying to predecode the
        //  first line, looking for an XML/TextDecl.
        //
        XMLReader* valueReader = fReaderMgr.createIntEntReader
        (
            decl->getName()
            , XMLReader::RefFrom_NonLiteral
            , XMLReader::Type_General
            , decl->getValue()
            , decl->getValueLen()
            , false
        );

        //
        //  Try to push the entity reader onto the reader manager stack,
        //  where it will become the subsequent input. If it fails, that
        //  means the entity is recursive, so issue an error. The reader
        //  will have just been discarded, but we just keep going.
        //
        if (!fReaderMgr.pushReader(valueReader, decl))
            emitError(XMLErrs::RecursiveEntity, decl->getName());

        //
        //  Do a start entity reference event.
        //
        //  <TBD> For now, we supress them in att values. Later, when
        //  the stuff is in place to correctly allow DOM to handle them
        //  we'll turn this back on.
        //
        if (fDocHandler && !inAttVal)
            fDocHandler->startEntityReference(*decl);

        // If it starts with the XML string, then it's an error
        if (checkXMLDecl(true)) {
            emitError(XMLErrs::TextDeclNotLegalHere);
            fReaderMgr.skipPastChar(chCloseAngle);
        }
    }
    return EntityExp_Pushed;
}


unsigned int
XMLScanner::scanUpToWSOr(XMLBuffer& toFill, const XMLCh chEndChar)
{
    fReaderMgr.getUpToCharOrWS(toFill, chEndChar);
    return toFill.getLen();
}

bool XMLScanner::switchGrammar(int newGrammarNameSpaceIndex)
{
    XMLBuffer bufURI;
    getURIText(newGrammarNameSpaceIndex, bufURI);
    Grammar* tempGrammar = fGrammarResolver->getGrammar(bufURI.getRawBuffer());
    if (!tempGrammar) {
        // This is a case where namespaces is on with a DTD grammar.
        tempGrammar = fGrammarResolver->getGrammar(XMLUni::fgZeroLenString);
    }
    if (!tempGrammar)
        return false;
    else {
        fGrammar = tempGrammar;
        if (fGrammar->getGrammarType() == Grammar::SchemaGrammarType && !fValidator->handlesSchema()) {
            if (fValidatorFromUser)
                ThrowXML(RuntimeException, XMLExcepts::Gen_NoSchemaValidator);
            else {
                fValidator = fSchemaValidator;
            }
        }
        else if (fGrammar->getGrammarType() == Grammar::DTDGrammarType && !fValidator->handlesDTD()) {
            if (fValidatorFromUser)
                ThrowXML(RuntimeException, XMLExcepts::Gen_NoDTDValidator);
            else {
                fValidator = fDTDValidator;
            }
        }

        fValidator->setGrammar(fGrammar);
        return true;
    }
}

bool XMLScanner::switchGrammar(const XMLCh* const newGrammarNameSpace)
{
    Grammar* tempGrammar = fGrammarResolver->getGrammar(newGrammarNameSpace);
    if (!tempGrammar) {
        // This is a case where namespaces is on with a DTD grammar.
        tempGrammar = fGrammarResolver->getGrammar(XMLUni::fgZeroLenString);
    }
    if (!tempGrammar)
        return false;
    else {
        fGrammar = tempGrammar;
        if (fGrammar->getGrammarType() == Grammar::SchemaGrammarType && !fValidator->handlesSchema()) {
            if (fValidatorFromUser)
                ThrowXML(RuntimeException, XMLExcepts::Gen_NoSchemaValidator);
            else {
                fValidator = fSchemaValidator;
            }
        }
        else if (fGrammar->getGrammarType() == Grammar::DTDGrammarType && !fValidator->handlesDTD()) {
            if (fValidatorFromUser)
                ThrowXML(RuntimeException, XMLExcepts::Gen_NoDTDValidator);
            else {
                fValidator = fDTDValidator;
            }
        }

        fValidator->setGrammar(fGrammar);
        return true;
    }
}

// check if we should skip or lax the validation of the element
// if skip - no validation
// if lax - validate only if the element if found

bool XMLScanner::laxElementValidation(QName* element, ContentLeafNameTypeVector* cv,
                                      const XMLContentModel* const cm,
                                      const unsigned int parentElemDepth)
{
    bool skipThisOne = false;
    bool laxThisOne = false;
    unsigned int elementURI = element->getURI();
    unsigned int currState = fElemState[parentElemDepth];

    if (currState == XMLContentModel::gInvalidTrans) {
        return laxThisOne;
    }

    SubstitutionGroupComparator comparator(fGrammarResolver, fURIStringPool);

    if (cv) {
        unsigned int i = 0;
        unsigned int leafCount = cv->getLeafCount();

        for (; i < leafCount; i++) {

            QName* fElemMap = cv->getLeafNameAt(i);
            unsigned int uri = fElemMap->getURI();
            unsigned int nextState;
            bool anyEncountered = false;
            ContentSpecNode::NodeTypes type = cv->getLeafTypeAt(i);

            if (type == ContentSpecNode::Leaf) {
                if (((uri == elementURI)
                      && !XMLString::compareString(fElemMap->getLocalPart(), element->getLocalPart()))
                    || comparator.isEquivalentTo(element, fElemMap)) {

                    nextState = cm->getNextState(currState, i);

                    if (nextState != XMLContentModel::gInvalidTrans) { 
                        fElemState[parentElemDepth] = nextState;
                        break;
                    }
                }
            } else if ((type & 0x0f) == ContentSpecNode::Any) {
                anyEncountered = true;
            }
            else if ((type & 0x0f) == ContentSpecNode::Any_Other) {
                if (uri != elementURI) {
                    anyEncountered = true;
                }
            }
            else if ((type & 0x0f) == ContentSpecNode::Any_NS) {
                if (uri == elementURI) {
                    anyEncountered = true;
                }
            } 

            if (anyEncountered) {

                nextState = cm->getNextState(currState, i);
                if (nextState != XMLContentModel::gInvalidTrans) {
                    fElemState[parentElemDepth] = nextState;

                    if (type == ContentSpecNode::Any_Skip ||
                        type == ContentSpecNode::Any_NS_Skip ||
                        type == ContentSpecNode::Any_Other_Skip) {
                        skipThisOne = true;
                    }
                    else if (type == ContentSpecNode::Any_Lax ||
                             type == ContentSpecNode::Any_NS_Lax ||
                             type == ContentSpecNode::Any_Other_Lax) {
                        laxThisOne = true;
                    }

                    break;
                }
            }
        } // for

        if (i == leafCount) { // no match
            fElemState[parentElemDepth] = XMLContentModel::gInvalidTrans;
            return laxThisOne;
        }

    } // if

    if (skipThisOne) {
        fValidate = false;
        fElemStack.setValidationFlag(fValidate);
    }

    return laxThisOne;
}
