/*
 * The Apache Software License, Version 1.1
 * 
 * Copyright (c) 1999 The Apache Software Foundation.  All rights 
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
 * Revision 1.3  1999/12/04 01:13:16  roddey
 * Fixed the logic for checking for PIs that start with 'xml'. It was doing doing "if (stringICompare()) "
 * instead of "if (!stringICompare()).
 *
 * Revision 1.2  1999/11/30 20:24:45  roddey
 * Fixes for incorrect deletion of temporary decl objects, which would cause
 * a double delete when the parser is deleted.
 *
 * Revision 1.1.1.1  1999/11/09 01:03:38  twl
 * Initial checkin
 *
 * Revision 1.4  1999/11/08 20:45:42  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <util/BinMemInputStream.hpp>
#include <util/FlagJanitor.hpp>
#include <util/Janitor.hpp>
#include <util/PlatformUtils.hpp>
#include <util/UnexpectedEOFException.hpp>
#include <util/URL.hpp>
#include <util/XMLUni.hpp>
#include <framework/XMLBufferMgr.hpp>
#include <framework/XMLDocumentHandler.hpp>
#include <framework/XMLElementDecl.hpp>
#include <framework/XMLEntityHandler.hpp>
#include <framework/XMLNotationDecl.hpp>
#include <internal/URLInputSource.hpp>
#include <internal/EndOfEntityException.hpp>
#include <internal/ReaderMgr.hpp>
#include <internal/XMLScanner.hpp>
#include <validators/DTD/ContentSpecNode.hpp>
#include <validators/DTD/DTDEntityDecl.hpp>
#include <validators/DTD/DTDValidator.hpp>
#include <validators/DTD/DocTypeHandler.hpp>
#include <validators/DTD/MixedContentModel.hpp>



// ---------------------------------------------------------------------------
//  Local methods
// ---------------------------------------------------------------------------

//
//  This method automates the grunt work of looking at a char and see if its
//  a repetition suffix. If so, it creates a new correct rep node and wraps
//  the pass node in it. Otherwise, it returns the previous node.
//
static ContentSpecNode*
makeRepNode(const XMLCh testCh, ContentSpecNode* const prevNode)
{
    if (testCh == chQuestion)
    {
        return new ContentSpecNode
        (
            ContentSpecNode::ZeroOrOne
            , prevNode
            , 0
        );
    }
     else if (testCh == chPlus)
    {
        return new ContentSpecNode
        (
            ContentSpecNode::OneOrMore
            , prevNode
            , 0
        );
    }
     else if (testCh == chAsterisk)
    {
        return new ContentSpecNode
        (
            ContentSpecNode::ZeroOrMore
            , prevNode
            , 0
        );
    }

    // Just return the incoming node
    return prevNode;
}



// ---------------------------------------------------------------------------
//  DTDValidator: Private scanning methods
// ---------------------------------------------------------------------------
bool DTDValidator::checkForPERef(const  bool    spaceRequired
                                , const bool    inLiteral
                                , const bool    inMarkup
                                , const bool    throwAtEndExt)
{
    bool gotSpace = false;

    //
    //  See if we have any spaces up front. If so, then skip them and set
    //  the gotSpaces flag.
    //
    if (getReaderMgr()->skippedSpace())
    {
        getReaderMgr()->skipPastSpaces();
        gotSpace = true;
    }

    // If the next char is a percent, then expand the PERef
    if (getReaderMgr()->skippedChar(chPercent))
        expandPERef(false, inLiteral, inMarkup, throwAtEndExt);

    // And skip any more spaces in the expanded value
    if (getReaderMgr()->skippedSpace())
    {
        getReaderMgr()->skipPastSpaces();
        gotSpace = true;
    }
    return gotSpace;
}


bool DTDValidator::expandPERef( const   bool    scanExternal
                                , const bool    inLiteral
                                , const bool    inMarkup
                                , const bool    throwEndOfExt)
{
    XMLBufBid bbName(getBufMgr());

    //
    //  If we are in the internal subset and in markup, then this is
    //  an error but we go ahead and do it anyway.
    //
    if (fInternalSubset && inMarkup)
        getScanner()->emitError(XML4CErrs::PERefInMarkupInIntSubset);

    if (!getReaderMgr()->getName(bbName.getBuffer()))
    {
        getScanner()->emitError(XML4CErrs::ExpectedPEName);

        // Skip the semicolon if that's what we ended up on
        getReaderMgr()->skippedChar(chSemiColon);
        return false;
    }

    // If no terminating semicolon, emit an error but try to keep going
    if (!getReaderMgr()->skippedChar(chSemiColon))
        getScanner()->emitError(XML4CErrs::UnterminatedEntityRef);

    //
    //  Look it up in the PE decl pool and see if it exists. If not, just
    //  emit an error and continue.
    //
    XMLEntityDecl* decl = findEntityDecl(bbName.getRawBuffer(), true);
    if (!decl)
    {
        getScanner()->emitError(XML4CErrs::EntityNotFound, bbName.getRawBuffer());
        return false;
    }

    //
    //  If we are a standalone document, then it has to have been declared
    //  in the internal subset. Keep going though.
    //
    if (getScanner()->getStandalone() && !decl->getDeclaredInIntSubset())
        getScanner()->emitError(XML4CErrs::IllegalRefInStandalone, bbName.getRawBuffer());

    //
    //  Okee dokee, we found it. So create either a memory stream with
    //  the entity value contents, or a file stream if its an external
    //  entity.
    //
    if (decl->isExternal())
    {
        // And now create a reader to read this entity
        InputSource* srcUsed;
        XMLReader* reader = getReaderMgr()->createReader
        (
            decl->getSystemId()
            , decl->getPublicId()
            , false
            , inLiteral ? XMLReader::RefFrom_Literal : XMLReader::RefFrom_NonLiteral
            , XMLReader::Type_PE
            , XMLReader::Source_External
            , srcUsed
        );

        // Put a janitor on the source so its cleaned up on exit
        Janitor<InputSource> janSrc(srcUsed);

        // If the creation failed then throw an exception
        if (!reader)
            ThrowXML1(RuntimeException, XML4CExcepts::Gen_CouldNotOpenExtEntity, srcUsed->getSystemId());

        // Set the 'throw at end' flag, to the one we were given
        reader->setThrowAtEnd(throwEndOfExt);

        //
        //  Push the reader. If its a recursive expansion, then emit an error
        //  and return an failure.
        //
        if (!getReaderMgr()->pushReader(reader, decl))
        {
            getScanner()->emitError(XML4CErrs::RecursiveEntity, decl->getName());
            return false;
        }

        //
        //  If the caller wants us to scan the external entity, then lets
        //  do that now.
        //
        if (scanExternal)
        {
            XMLEntityHandler* entHandler = getScanner()->getEntityHandler();

            // If we have an entity handler, tell it we are starting this entity
            if (entHandler)
                entHandler->startInputSource(*srcUsed);

            //
            //  Scan the external entity now. The parameter tells it that
            //  it is not in an include section. Get the current reader
            //  level so we can catch partial markup errors and be sure
            //  to get back to here if we get an exception out of the
            //  ext subset scan.
            //
            const unsigned int readerNum = getReaderMgr()->getCurrentReaderNum();
            try
            {
                scanExtSubsetDecl(false);
            }

            catch(...)
            {
                // Pop the reader back to the original level
                getReaderMgr()->cleanStackBackTo(readerNum);

                // End the input source, even though its not happy
                if (entHandler)
                    entHandler->endInputSource(*srcUsed);
                throw;
            }

            // If we have an entity handler, tell it we are ending this entity
            if (entHandler)
                entHandler->endInputSource(*srcUsed);
        }
    }
     else
    {
        // Create a reader over a memory stream over the entity value
        XMLReader* valueReader = getReaderMgr()->createIntEntReader
        (
            decl->getName()
            , inLiteral ? XMLReader::RefFrom_Literal : XMLReader::RefFrom_NonLiteral
            , XMLReader::Type_PE
            , decl->getValue()
            , decl->getValueLen()
            , false
        );

        //
        //  Trt to push the entity reader onto the reader manager stack,
        //  where it will become the subsequent input. If it fails, that
        //  means the entity is recursive, so issue an error. The reader
        //  will have just been discarded, but we just keep going.
        //
        if (!getReaderMgr()->pushReader(valueReader, decl))
            getScanner()->emitError(XML4CErrs::RecursiveEntity, decl->getName());
    }

    return true;
}


bool DTDValidator::getQuotedString(XMLBuffer& toFill)
{
    // Reset the target buffer
    toFill.reset();

    // Get the next char which must be a single or double quote
    XMLCh quoteCh;
    if (!getReaderMgr()->skipIfQuote(quoteCh))
        return false;

    while (true)
    {
        // Get another char
        const XMLCh nextCh = getReaderMgr()->getNextChar();

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


XMLAttDef*
DTDValidator::scanAttDef(DTDElementDecl& parentElem, XMLBuffer& bufToUse)
{
    // Check for PE ref or optional whitespace
    checkForPERef(false, false, true);

    // Get the name of the attribute
    if (!getReaderMgr()->getName(bufToUse))
    {
        getScanner()->emitError(XML4CErrs::ExpectedAttrName);
        return 0;
    }

    //
    //  Look up this attribute in the parent element's attribute list. If
    //  it already exists, then use the dummy.
    //
    DTDAttDef* decl = parentElem.getAttDef(bufToUse.getRawBuffer());
    if (decl)
    {
        // It already exists, so put out a warning
        getScanner()->emitError(XML4CErrs::AttListAlreadyExists, bufToUse.getRawBuffer());

        // Use the dummy decl to parse into and set its name to the name we got
        if (!fDumAttDef)
        {
            fDumAttDef = new DTDAttDef;
            fDumAttDef->setId(fNextAttrId++);
        }
        fDumAttDef->setName(bufToUse.getRawBuffer());
        decl = fDumAttDef;
    }
     else
    {
        //
        //  It does not already exist so create a new one, give it the next
        //  available unique id, and add it
        //
        decl = new DTDAttDef(bufToUse.getRawBuffer());
        decl->setId(fNextAttrId++);
        parentElem.addAttDef(decl);
    }

    // Set a flag to indicate whether we are doing a dummy parse
    const bool isIgnored = (decl == fDumAttDef);

    // Space is required here, so check for PE ref, and require space
    if (!checkForPERef(true, false, true))
        getScanner()->emitError(XML4CErrs::ExpectedWhitespace);

    //
    //  Next has to be one of the attribute type strings. This tells us what
    //  is to follow.
    //
    if (getReaderMgr()->skippedString(XMLUni::fgCDATAString))
    {
        decl->setType(XMLAttDef::CData);
    }
     else if (getReaderMgr()->skippedString(XMLUni::fgIDString))
    {
        if (!getReaderMgr()->skippedString(XMLUni::fgRefString))
            decl->setType(XMLAttDef::ID);
        else if (!getReaderMgr()->skippedChar(chLatin_S))
            decl->setType(XMLAttDef::IDRef);
        else
            decl->setType(XMLAttDef::IDRefs);
    }
     else if (getReaderMgr()->skippedString(XMLUni::fgEntitString))
    {
        if (getReaderMgr()->skippedChar(chLatin_Y))
        {
            decl->setType(XMLAttDef::Entity);
        }
         else if (getReaderMgr()->skippedString(XMLUni::fgIESString))
        {
            decl->setType(XMLAttDef::Entities);
        }
         else
        {
            getScanner()->emitError(XML4CErrs::ExpectedAttributeType);
            return 0;
        }
    }
     else if (getReaderMgr()->skippedString(XMLUni::fgNmTokenString))
    {
        if (getReaderMgr()->skippedChar(chLatin_S))
            decl->setType(XMLAttDef::NmTokens);
        else
            decl->setType(XMLAttDef::NmToken);
    }
     else if (getReaderMgr()->skippedString(XMLUni::fgNotationString))
    {
        // Check for PE ref and require space
        if (!checkForPERef(true, false, true))
            getScanner()->emitError(XML4CErrs::ExpectedWhitespace);

        decl->setType(XMLAttDef::Notation);
        if (!scanEnumeration(bufToUse, true))
            return 0;

        // Set the value as the enumeration for this decl
        decl->setEnumeration(bufToUse.getRawBuffer());
    }
     else if (getReaderMgr()->skippedChar(chOpenParen))
    {
        decl->setType(XMLAttDef::Enumeration);
        if (!scanEnumeration(bufToUse, false))
            return 0;

        // Set the value as the enumeration for this decl
        decl->setEnumeration(bufToUse.getRawBuffer());
    }
     else
    {
        getScanner()->emitError(XML4CErrs::ExpectedAttributeType);
        return 0;
    }

    // Space is required here, so check for PE ref, and require space
    if (!checkForPERef(true, false, true))
        getScanner()->emitError(XML4CErrs::ExpectedWhitespace);

    // And then scan for the optional default value declaration
    scanDefaultDecl(*decl);

    // If validating, then do a couple of validation constraints
    if (getScanner()->getDoValidation())
    {
        if (decl->getType() == XMLAttDef::ID)
        {
            if ((decl->getDefaultType() != XMLAttDef::Implied)
            &&  (decl->getDefaultType() != XMLAttDef::Required))
            {
                emitError(XML4CValid::BadIDAttrDefType, decl->getFullName());
            }
        }
    }

    // If we have a doc type handler, tell it about this attdef.
    if (fDocTypeHandler)
        fDocTypeHandler->attDef(parentElem, *decl, isIgnored);
    return decl;
}


void DTDValidator::scanAttListDecl()
{
    // Space is required here, so check for a PE ref
    if (!checkForPERef(true, false, true))
    {
        getScanner()->emitError(XML4CErrs::ExpectedWhitespace);
        getReaderMgr()->skipPastChar(chCloseAngle);
        return;
    }

    //
    //  Next should be the name of the element it belongs to, so get a buffer
    //  and get the name into it.
    //
    XMLBufBid bbName(getBufMgr());
    if (!getReaderMgr()->getName(bbName.getBuffer()))
    {
        getScanner()->emitError(XML4CErrs::ExpectedElementName);
        getReaderMgr()->skipPastChar(chCloseAngle);
        return;
    }

    //
    //  Find this element's declaration. If it has not been declared yet,
    //  we will force one into the list, but not mark it as declared.
    //
    DTDElementDecl* elemDecl = fElemDeclPool->getByKey(bbName.getRawBuffer());
    if (!elemDecl)
    {
        //
        //  Lets fault in a declaration and add it to the pool. We mark
        //  it having been created because of an attlist. Later, if its
        //  declared, this will be updated.
        //
        elemDecl = new DTDElementDecl(bbName.getRawBuffer());
        elemDecl->setCreateReason(XMLElementDecl::AttList);
        fElemDeclPool->put(elemDecl);
    }

    // If we have a doc type handler, tell it the att list is starting
    if (fDocTypeHandler)
        fDocTypeHandler->startAttList(*elemDecl);

    //
    //  Now we loop until we are done with all of the attributes in this
    //  list. We need a buffer to use for local processing.
    //
    XMLBufBid   bbTmp(getBufMgr());
    XMLBuffer&  tmpBuf = bbTmp.getBuffer();
    bool        seenAnId = false;
    while (true)
    {
        // Get the next char out and see what it tells us to do
        const XMLCh nextCh = getReaderMgr()->peekNextChar();

        // Watch for EOF
        if (!nextCh)
            ThrowXML(UnexpectedEOFException, XML4CExcepts::Gen_UnexpectedEOF);

        if (nextCh == chCloseAngle)
        {
            // We are done with this attribute list
            getReaderMgr()->getNextChar();
            break;
        }
         else if (XMLReader::isWhitespace(nextCh))
        {
            //
            //  If advanced callbacks are enabled and we have a doc
            //  type handler, then gather up the white space and call
            //  back on the doctype handler. Otherwise, just skip
            //  whitespace.
            //
            if (fDocTypeHandler)
            {
                getReaderMgr()->getSpaces(tmpBuf);
                fDocTypeHandler->doctypeWhitespace
                (
                    tmpBuf.getRawBuffer()
                    , tmpBuf.getLen()
                );
            }
             else
            {
                getReaderMgr()->skipPastSpaces();
            }
        }
         else if (nextCh == chPercent)
        {
            // Eat the percent and expand the ref
            getReaderMgr()->getNextChar();
            expandPERef(false, false, true);
        }
         else
        {
            //
            //  It must be an attribute name, so scan it. We let
            //  it use our local buffer for its name scanning.
            //
            XMLAttDef* attDef = scanAttDef(*elemDecl, tmpBuf);

            if (!attDef)
            {
                getReaderMgr()->skipPastChar(chCloseAngle);
                break;
            }

            //
            //  If we are validating and its an ID type, then we have to
            //  make sure that we have not seen an id attribute yet. Set
            //  the flag to say that we've seen one now also.
            //
            if (attDef->getType() == XMLAttDef::ID)
            {
                if (seenAnId)
                    emitError(XML4CValid::MultipleIdAttrs, elemDecl->getFullName());
                seenAnId = true;
            }
        }
    }

    // If we have a doc type handler, tell it the att list is ending
    if (fDocTypeHandler)
        fDocTypeHandler->endAttList(*elemDecl);
}


//
//  This method is called to scan the value of an attribute in content. This
//  involves some normalization and replacement of general entity and
//  character references.
//
//  End of entity's must be dealt with here. During DTD scan, they can come
//  from external entities. During content, they can come from any entity.
//  We just eat the end of entity and continue with our scan until we come
//  to the closing quote. If an unterminated value causes us to go through
//  subsequent entities, that will cause errors back in the calling code,
//  but there's little we can do about it here.
//
bool DTDValidator::scanAttValue(        XMLBuffer&          toFill
                                , const XMLAttDef::AttTypes type)
{
    enum States
    {
        InWhitespace
        , InContent
    };

    // Reset the target buffer
    toFill.reset();

    // Get the next char which must be a single or double quote
    XMLCh quoteCh;
    if (!getReaderMgr()->skipIfQuote(quoteCh))
        return false;

    //
    //  We have to get the current reader because we have to ignore closing
    //  quotes until we hit the same reader again.
    //
    const unsigned int curReader = getReaderMgr()->getCurrentReaderNum();

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
            // Get another char. Use second char from prevous is its there
            if (secondCh)
            {
                nextCh = secondCh;
                secondCh = 0;
            }
             else
            {
                nextCh = getReaderMgr()->getNextChar();
            }

            if (!nextCh)
                ThrowXML(UnexpectedEOFException, XML4CExcepts::Gen_UnexpectedEOF);

            // Check for our ending quote in the same entity
            if (nextCh == quoteCh)
            {
                if (curReader == getReaderMgr()->getCurrentReaderNum())
                    return true;

                // Watch for spillover into a previous entity
                if (curReader > getReaderMgr()->getCurrentReaderNum())
                {
                    getScanner()->emitError(XML4CErrs::PartialMarkupInEntity);
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
                if (scanEntityRef(nextCh, secondCh, escaped) != EntityExp_Returned)
                {
                    gotLeadingSurrogate = false;
                    continue;
                }
            }

            // Check for correct surrogate pairs
            if ((nextCh >= 0xD800) && (nextCh <= 0xDBFF))
            {
                if (gotLeadingSurrogate)
                    getScanner()->emitError(XML4CErrs::Expected2ndSurrogateChar);
                else
                    gotLeadingSurrogate = true;
            }
             else
            {
                if (gotLeadingSurrogate)
                {
                    if ((nextCh < 0xDC00) && (nextCh > 0xDFFF))
                        getScanner()->emitError(XML4CErrs::Expected2ndSurrogateChar);
                }
                gotLeadingSurrogate = false;

                // Its got to at least be a valid XML character
                if (!XMLReader::isXMLChar(nextCh))
                    getScanner()->emitError(XML4CErrs::InvalidCharacter);
            }

            //
            //  If its not escaped, then make sure its not a < character, which
            //  is not allowed in attribute values.
            //
            if (!escaped && (nextCh == chOpenAngle))
                getScanner()->emitError(XML4CErrs::BracketInAttrValue);

            //
            //  If the attribute is a CDATA type we do simple replacement of
            //  tabs and new lines with spaces, if the character is not escaped
            //  by way of a char ref.
            //
            //  Otherwise, we do the standard non-CDATA normalization of
            //  compressing whitespace to single spaces and getting rid of
            //  leading and trailing whitespace.
            //
            if (type == XMLAttDef::CData)
            {
                if (!escaped)
                {
                    if ((nextCh == 0x09) || (nextCh == 0x0A) || (nextCh == 0x0D))
                        nextCh = chSpace;
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


bool DTDValidator::scanCharRef(XMLCh& first, XMLCh& second)
{
    bool gotOne = false;
    unsigned int value = 0;

    //
    //  Set the radix. Its supposed to be a lower case x if hex. But, in
    //  order to recover well, we check for an upper and put out an error
    //  for that.
    //
    unsigned int radix = 10;

    if (getReaderMgr()->skippedChar(chLatin_x))
    {
        radix = 16;
    }
     else if (getReaderMgr()->skippedChar(chLatin_X))
    {
        getScanner()->emitError(XML4CErrs::HexRadixMustBeLowerCase);
        radix = 16;
    }

    while (true)
    {
        const XMLCh nextCh = getReaderMgr()->peekNextChar();

        // Watch for EOF
        if (!nextCh)
            ThrowXML(UnexpectedEOFException, XML4CExcepts::Gen_UnexpectedEOF);

        // Break out on the terminating semicolon
        if (nextCh == chSemiColon)
        {
            getReaderMgr()->getNextChar();
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
            //
            //  If we got at least a sigit, then do an unterminated ref
            //  error. Else, do an expected a numerical ref thing.
            //
            if (gotOne)
                getScanner()->emitError(XML4CErrs::UnterminatedCharRef);
            else
                getScanner()->emitError(XML4CErrs::ExpectedNumericalCharRef);

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
            getScanner()->emitError(XML4CErrs::BadDigitForRadix, tmpStr);
        }
         else
        {
            value = (value * radix) + nextVal;
        }

        // Indicate that we got at least one good digit
        gotOne = true;

        // Eat the char we just processed
        getReaderMgr()->getNextChar();
    }

    // Return the char (or chars)
    if (value >= 0x10000)
    {
        value -= 0x10000;
        first = XMLCh((value >> 10) + 0xD800);
        second = XMLCh((value & 0x3FF) + 0xDC00);
    }
     else
    {
        first = XMLCh(value);
        second = 0;
    }
    return true;
}


ContentSpecNode* DTDValidator::scanChildren(XMLBuffer& bufToUse)
{
    // Check for a PE ref here, but don't require spaces
    checkForPERef(false, false, true);

    // We have to check entity nesting here
    unsigned int curReader;

    //
    //  We know that the caller just saw an opening parenthesis, so we need
    //  to parse until we hit the end of it, recursing for other nested
    //  parentheses we see.
    //
    //  We have to check for one up front, since it could be something like
    //  (((a)*)) etc...
    //
    ContentSpecNode* curNode = 0;
    if (getReaderMgr()->skippedChar(chOpenParen))
    {
        curReader = getReaderMgr()->getCurrentReaderNum();

        // Lets call ourself and get back the resulting node
        curNode = scanChildren(bufToUse);

        // If that failed, no need to go further, return failure
        if (!curNode)
            return 0;

        if (curReader != getReaderMgr()->getCurrentReaderNum())
            getScanner()->emitError(XML4CErrs::PartialMarkupInEntity);
    }
     else
    {
        // Not a nested paren, so it must be a leaf node
        if (!getReaderMgr()->getName(bufToUse))
        {
            getScanner()->emitError(XML4CErrs::ExpectedElementName);
            return 0;
        }

        //
        //  Create a leaf node for it. If we can find the element id for
        //  this element, then use it. Else, we have to fault in an element
        //  decl, marked as created because of being in a content model.
        //
        unsigned int elemId = findElemId(bufToUse.getRawBuffer());
        if (elemId == XMLElementDecl::fgInvalidElemId)
        {
            DTDElementDecl* decl = new DTDElementDecl(bufToUse.getRawBuffer());
            decl->setCreateReason(XMLElementDecl::InContentModel);
            fElemDeclPool->put(decl);
            elemId = decl->getId();
        }
        curNode = new ContentSpecNode(elemId);

        // Check for a PE ref here, but don't require spaces
        const bool gotSpaces = checkForPERef(false, false, true);

        // Check for a repetition character after the leaf
        const XMLCh repCh = getReaderMgr()->peekNextChar();
        ContentSpecNode* tmpNode = makeRepNode(repCh, curNode);
        if (tmpNode != curNode)
        {
            if (gotSpaces)
                getScanner()->emitError(XML4CErrs::UnexpectedWhitespace);
            getReaderMgr()->getNextChar();
            curNode = tmpNode;
        }
    }

    // Check for a PE ref here, but don't require spaces
    checkForPERef(false, false, true);

    //
    //  Ok, the next character tells us what kind of content this particular
    //  model this particular parentesized section is. Its either a choice if
    //  we see ',', a sequence if we see '|', or a single leaf node if we see
    //  a closing paren.
    //
    const XMLCh opCh = getReaderMgr()->peekNextChar();

    if ((opCh != chComma)
    &&  (opCh != chPipe)
    &&  (opCh != chCloseParen))
    {
        // Not a legal char, so delete our node and return failure
        getScanner()->emitError(XML4CErrs::ExpectedSeqChoiceLeaf);
        delete curNode;
        return 0;
    }

    //
    //  Create the head node of the correct type. We need this to remember
    //  the top of the local tree. If it was a single subexpr, then just
    //  set the head node to the current node. For the others, we'll build
    //  the tree off the second child as we move across.
    //
    ContentSpecNode* headNode = 0;
    ContentSpecNode::NodeTypes curType;
    if (opCh == chComma)
    {
        curType = ContentSpecNode::Sequence;
        headNode = new ContentSpecNode(curType, curNode, 0);
        curNode = headNode;
    }
     else if (opCh == chPipe)
    {
        curType = ContentSpecNode::Choice;
        headNode = new ContentSpecNode(curType, curNode, 0);
        curNode = headNode;
    }
     else
    {
        headNode = curNode;
        getReaderMgr()->getNextChar();
    }

    //
    //  If it was a sequence or choice, we just loop until we get to the
    //  end of our section, adding each new leaf or sub expression to the
    //  right child of the current node, and making that new node the current
    //  node.
    //
    if ((opCh == chComma) || (opCh == chPipe))
    {
        ContentSpecNode* lastNode = 0;
        while (true)
        {
            // Check for a PE ref here, but don't require spaces
            checkForPERef(false, false, true);

            //
            //  The next thing must either be another | or , character followed
            //  by another leaf or subexpression, or a closing parenthesis.
            //
            if (getReaderMgr()->skippedChar(chCloseParen))
            {
                //
                //  We've hit the end of this section, so break out. But, we
                //  need to see if we left a partial sequence of choice node
                //  without a second node. If so, we have to undo that and
                //  put its left child into the right node of the previous
                //  node.
                //
                if ((curNode->getType() == ContentSpecNode::Choice)
                ||  (curNode->getType() == ContentSpecNode::Sequence))
                {
                    if (!curNode->getSecond())
                    {
                        ContentSpecNode* saveFirst = curNode->orphanFirst();
                        lastNode->setSecond(saveFirst);
                        curNode = lastNode;
                    }
                }
                break;
            }
             else if (getReaderMgr()->skippedChar(opCh))
            {
                // Check for a PE ref here, but don't require spaces
                checkForPERef(false, false, true);

                if (getReaderMgr()->skippedChar(chOpenParen))
                {
                    curReader = getReaderMgr()->getCurrentReaderNum();

                    // Recurse to handle this new guy
                    ContentSpecNode* subNode = scanChildren(bufToUse);

                    // If it failed, we are done, clean up here and return failure
                    if (!subNode)
                    {
                        delete headNode;
                        return 0;
                    }

                    if (curReader != getReaderMgr()->getCurrentReaderNum())
                        getScanner()->emitError(XML4CErrs::PartialMarkupInEntity);

                    // Else patch it in and make it the new current
                    ContentSpecNode* newCur = new ContentSpecNode
                    (
                        curType
                        , subNode
                        , 0
                    );
                    curNode->setSecond(newCur);
                    lastNode = curNode;
                    curNode = newCur;
                }
                 else
                {
                    //
                    //  Got to be a leaf node, so get a name. If we cannot get
                    //  one, then clean up and get outa here.
                    //
                    if (!getReaderMgr()->getName(bufToUse))
                    {
                        delete headNode;
                        getScanner()->emitError(XML4CErrs::ExpectedElementName);
                        return 0;
                    }

                    //
                    //  Create a leaf node for it. If we can find the element
                    //  id for this element, then use it. Else, we have to
                    //  fault in an element decl, marked as created because
                    //  of being in a content model.
                    //
                    unsigned int elemId = findElemId(bufToUse.getRawBuffer());
                    if (elemId == XMLElementDecl::fgInvalidElemId)
                    {
                        DTDElementDecl* decl = new DTDElementDecl(bufToUse.getRawBuffer());
                        decl->setCreateReason(XMLElementDecl::InContentModel);
                        fElemDeclPool->put(decl);
                        elemId = decl->getId();
                    }
                    ContentSpecNode* tmpLeaf = new ContentSpecNode(elemId);

                    // Check for a repetition character after the leaf
                    const XMLCh repCh = getReaderMgr()->peekNextChar();
                    ContentSpecNode* tmpLeaf2 = makeRepNode(repCh, tmpLeaf);
                    if (tmpLeaf != tmpLeaf2)
                        getReaderMgr()->getNextChar();

                    //
                    //  Create a new sequence or choice node, with the leaf
                    //  (or rep surrounding it) we just got as its first node.
                    //  Make the new node the second node of the current node,
                    //  and then make it the current node.
                    //
                    ContentSpecNode* newCur = new ContentSpecNode
                    (
                        curType
                        , tmpLeaf2
                        , 0
                    );
                    curNode->setSecond(newCur);
                    lastNode = curNode;
                    curNode = newCur;
                }
            }
             else
            {
                // Cannot be valid
                if (opCh == chComma)
                    getScanner()->emitError(XML4CErrs::ExpectedChoiceOrCloseParen);
                else
                    getScanner()->emitError(XML4CErrs::ExpectedSeqOrCloseParen);
                delete headNode;
                return 0;
            }
        }
    }

    //
    //  We saw the terminating parenthesis so lets check for any repetition
    //  character, and create a node for that, making the head node the child
    //  of it.
    //
    XMLCh repCh = getReaderMgr()->peekNextChar();
    ContentSpecNode* retNode = makeRepNode(repCh, headNode);
    if (retNode != headNode)
        getReaderMgr()->getNextChar();

    return retNode;
}


//
//  We get here after the '<!--' part of the comment. We scan past the
//  terminating '-->' It will calls the appropriate handler with the comment
//  text, if one is provided. A comment can be in either the document or
//  the DTD, so the fInDocument flag is used to know which handler to send
//  it to.
//
void DTDValidator::scanComment()
{
    enum States
    {
        InText
        , OneDash
        , TwoDashes
    };

    // Get a buffer for this
    XMLBufBid bbComment(getBufMgr());

    //
    //  Get the comment text into a temp buffer. Be sure to use temp buffer
    //  two here, since its to be used for stuff that is potentially longer
    //  than just a name.
    //
    States curState = InText;
    while (true)
    {
        // Get the next character
        const XMLCh nextCh = getReaderMgr()->getNextChar();

        //  Watch for an end of file
        if (!nextCh)
        {
            getScanner()->emitError(XML4CErrs::UnterminatedComment);
            ThrowXML(UnexpectedEOFException, XML4CExcepts::Gen_UnexpectedEOF);
        }

        // Make sure its a valid XML character
        if (!XMLReader::isXMLChar(nextCh))
            getScanner()->emitError(XML4CErrs::InvalidCharacter);

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
                getScanner()->emitError(XML4CErrs::IllegalSequenceInComment);
                getReaderMgr()->skipPastChar(chCloseAngle);
                return;
            }
            break;
        }
    }

    // If there is a doc type handler, then pass on the comment stuff
    if (fDocTypeHandler)
        fDocTypeHandler->doctypeComment(bbComment.getRawBuffer());
}


bool DTDValidator::scanContentSpec(DTDElementDecl& toFill)
{
    //
    //  Check for for a couple of the predefined content type strings. If
    //  its not one of these, its got to be a parenthesized reg ex type
    //  expression.
    //
    if (getReaderMgr()->skippedString(XMLUni::fgEmptyString))
    {
        toFill.setModelType(DTDElementDecl::Empty);
        return true;
    }

    if (getReaderMgr()->skippedString(XMLUni::fgAnyString))
    {
        toFill.setModelType(DTDElementDecl::Any);
        return true;
    }

    // Its got to be a parenthesized regular expression
    if (!getReaderMgr()->skippedChar(chOpenParen))
    {
        getScanner()->emitError(XML4CErrs::ExpectedContentSpecExpr);
        return false;
    }

    // Get the current reader id, so we can test for partial markup
    const unsigned int curReader = getReaderMgr()->getCurrentReaderNum();

    // We could have a PE ref here, but don't require space
    checkForPERef(false, false, true);

    //
    //  Now we look for a PCDATA string. If its PCDATA, then it must be a
    //  MIXED model. Otherwise, it must be a regular list of children in
    //  a regular expression perhaps.
    //
    bool status;
    if (getReaderMgr()->skippedString(XMLUni::fgPCDATAString))
    {
        // Set the model to mixed
        toFill.setModelType(DTDElementDecl::Mixed);
        status = scanMixed(toFill);

        //
        //  If we are validating we have to check that there are no multiple
        //  uses of any child elements.
        //
        if (getScanner()->getDoValidation())
        {
            if (((const MixedContentModel*)toFill.getContentModel())->hasDups())
                emitError(XML4CValid::RepElemInMixed);
        }
    }
     else
    {
        //
        //  We have to do a recursive scan of the content model. Create a
        //  buffer for it to use, for efficiency. It returns the top ofthe
        //  content spec node tree, which we set if successful.
        //
        toFill.setModelType(DTDElementDecl::Children);
        XMLBufBid bbTmp(getBufMgr());
        ContentSpecNode* resNode = scanChildren(bbTmp.getBuffer());
        status = (resNode != 0);
        if (status)
            toFill.setContentSpec(resNode);
    }

    // Make sure we are on the same reader as where we started
    if (curReader != getReaderMgr()->getCurrentReaderNum())
        getScanner()->emitError(XML4CErrs::PartialMarkupInEntity);

    return status;
}


void DTDValidator::scanDefaultDecl(DTDAttDef& toFill)
{
    if (getReaderMgr()->skippedString(XMLUni::fgRequiredString))
    {
        toFill.setDefaultType(XMLAttDef::Required);
        return;
    }

    if (getReaderMgr()->skippedString(XMLUni::fgImpliedString))
    {
        toFill.setDefaultType(XMLAttDef::Implied);
        return;
    }

    if (getReaderMgr()->skippedString(XMLUni::fgFixedString))
    {
        //
        //  There must be space before the fixed value. If there is not, then
        //  emit an error but keep going.
        //
        if (!getReaderMgr()->skippedSpace())
            getScanner()->emitError(XML4CErrs::ExpectedWhitespace);
        else
            getReaderMgr()->skipPastSpaces();
        toFill.setDefaultType(XMLAttDef::Fixed);
    }
     else
    {
        toFill.setDefaultType(XMLAttDef::Default);
    }

    //
    //  If we got here, its fixed or default, so we need to get a value.
    //  If we don't, then emit an error but just set the default value to
    //  an empty string and try to keep going.
    //
    XMLBufBid bbValue(getBufMgr());
    if (!scanAttValue(bbValue.getBuffer(), toFill.getType()))
        getScanner()->emitError(XML4CErrs::ExpectedDefAttrDecl);

    toFill.setValue(bbValue.getRawBuffer());
}


//
//  This method handles the high level logic of scanning the DOCType
//  declaration. This kicks off both the scanning of the internal subset and
//  the scanning of the external subset, if any.
//
//  When we get here the '<!DOCTYPE' part has already been scanned, which is
//  what told us that we had a doc type decl to parse.
//
void DTDValidator::scanDocTypeDecl(const bool reuseValidator)
{
    // There must be some space after DOCTYPE
    if (!getReaderMgr()->skipPastSpaces())
    {
        getScanner()->emitError(XML4CErrs::ExpectedWhitespace);

        // Just skip the Doctype declaration and return
        getReaderMgr()->skipPastChar(chCloseAngle);
        return;
    }

    // Get a buffer for the root element
    XMLBufBid bbRootName(getBufMgr());

    //
    //  Get a name from the input, which should be the name of the root
    //  element of the upcoming content.
    //
    getReaderMgr()->getName(bbRootName.getBuffer());
    if (bbRootName.isEmpty())
    {
        getScanner()->emitError(XML4CErrs::NoRootElemInDOCTYPE);
        getReaderMgr()->skipPastChar(chCloseAngle);
        return;
    }

    //
    //  This element obviously is not going to exist in the element decl
    //  pool yet, but we need to store away an element id. So force it into
    //  the element decl pool, marked as being there because it was in
    //  the DOCTYPE. Later, when its declared, the status will be updated.
    //
    DTDElementDecl* rootDecl = new DTDElementDecl(bbRootName.getRawBuffer());
    rootDecl->setCreateReason(DTDElementDecl::AsRootElem);
    fRootElemId = fElemDeclPool->put(rootDecl);

    // Skip any spaces after the name
    getReaderMgr()->skipPastSpaces();

    bool    hasIntSubset = false;
    bool    hasExtSubset = false;
    XMLCh*  sysId = 0;
    XMLCh*  pubId = 0;

    //
    //  If the next character is '[' then we have no external subset cause
    //  there is no system id, just the opening character of the internal
    //  subset. Else, has to be an id.
    //
    if (getReaderMgr()->skippedChar(chOpenSquare))
    {
        hasIntSubset = true;
    }
     else
    {
        // Indicate we have an external subset
        hasExtSubset = true;

        // Get buffers for the ids
        XMLBufBid bbPubId(getBufMgr());
        XMLBufBid bbSysId(getBufMgr());

        // Get the external subset id
        if (!scanId(bbPubId.getBuffer(), bbSysId.getBuffer(), IDType_External))
        {
            getScanner()->emitError(XML4CErrs::ExpectedSystemOrPublicId);
            getReaderMgr()->skipPastChar(chCloseAngle);
            return;
        }

        // Get copies of the ids we got
        pubId = XMLString::replicate(bbPubId.getRawBuffer());
        sysId = XMLString::replicate(bbSysId.getRawBuffer());

        // Skip spaces and check again for the opening of an internal subset
        getReaderMgr()->skipPastSpaces();
        if (getReaderMgr()->skippedChar(chOpenSquare))
            hasIntSubset = true;
    }

    // Insure that the ids get cleaned up, if they got allocated
    ArrayJanitor<XMLCh> janSysId(sysId);
    ArrayJanitor<XMLCh> janPubId(pubId);

    //
    //  If we have a doc type handler and advanced callbacks are enabled,
    //  call the doctype event.
    //
    if (fDocTypeHandler)
        fDocTypeHandler->doctypeDecl(*rootDecl, pubId, sysId, hasIntSubset);

    //
    //  Ok, if we had an internal subset, we are just past the [ character
    //  and need to parse that first.
    //
    if (hasIntSubset)
    {
        // We can't have any internal subset if we are reusing the validator
        if (reuseValidator)
            ThrowXML(RuntimeException, XML4CExcepts::Val_CantHaveIntSS);

        // Indicate we are in the internal subset now
        FlagJanitor<bool> janContentFlag(&fInternalSubset, true);

        // Set the current subset flag to let the scanning code know which
        if (!scanInternalSubset())
        {
            getReaderMgr()->skipPastChar(chCloseAngle);
            return;
        }

        //
        //  Do a sanity check that some expanded PE did not propogate out of
        //  the doctype. This could happen if it was terminated early by bad
        //  syntax.
        //
        if (getReaderMgr()->getCurrentReaderNum() > 1)
        {
            getScanner()->emitError(XML4CErrs::PEPropogated);

            // Ask the reader manager to pop back down to the main level
            getReaderMgr()->cleanStackBackTo(1);
        }

        getReaderMgr()->skipPastSpaces();
    }

    // And that should leave us at the closing > of the DOCTYPE line
    if (!getReaderMgr()->skippedChar(chCloseAngle))
    {
        //
        //  Do a special check for the common scenario of an extra ] char at
        //  the end. This is easy to recover from.
        //
        if (getReaderMgr()->skippedChar(chCloseSquare)
        &&  getReaderMgr()->skippedChar(chCloseAngle))
        {
            getScanner()->emitError(XML4CErrs::ExtraCloseSquare);
        }
         else
        {
            getScanner()->emitError(XML4CErrs::UnterminatedDOCTYPE);
            getReaderMgr()->skipPastChar(chCloseAngle);
        }
    }

    //
    //  If we had an external subset, then we need to deal with that one
    //  next. If we are reusing the validator, then don't scan it.
    //
    if (hasExtSubset && !reuseValidator)
    {
        // Indicate we are in the external subset now
        FlagJanitor<bool> janContentFlag(&fInternalSubset, false);

        // And now create a reader to read this entity
        InputSource* srcUsed;
        XMLReader* reader = getReaderMgr()->createReader
        (
            sysId
            , pubId
            , false
            , XMLReader::RefFrom_NonLiteral
            , XMLReader::Type_General
            , XMLReader::Source_External
            , srcUsed
        );

        // Put a janitor on the input source
        Janitor<InputSource> janSrc(srcUsed);

        //
        //  If it failed then throw an exception
        //
        if (!reader)
            ThrowXML1(RuntimeException, XML4CExcepts::Gen_CouldNotOpenDTD, srcUsed->getSystemId());

        //
        //  In order to make the processing work consistently, we have to
        //  make this look like an external entity. So create an entity
        //  decl and fill it in and push it with the reader, as happens
        //  with an external entity. Put a janitor on it to insure it gets
        //  cleaned up. The reader manager does not adopt them.
        //
        const XMLCh gDTDStr[] = { chLatin_D, chLatin_T, chLatin_D , chNull };
        DTDEntityDecl* declDTD = new DTDEntityDecl(gDTDStr);
        declDTD->setSystemId(sysId);
        Janitor<DTDEntityDecl> janDecl(declDTD);

        // Mark this one as a throw at end
        reader->setThrowAtEnd(true);

        // And push it onto the stack, with its pseudo name
        getReaderMgr()->pushReader(reader, declDTD);

        // Tell it its not in an include section
        scanExtSubsetDecl(false);
    }
}


//
//  This is called after seeing '<!ELEMENT' which indicates that an element
//  markup is starting. This guy scans the rest of it and adds it to the
//  element decl pool if it has not already been declared.
//
void DTDValidator::scanElementDecl()
{
    //
    //  Space is legal (required actually) here so check for a PE ref. If
    //  we don't get our whitespace, then issue and error, but try to keep
    //  going.
    //
    if (!checkForPERef(true, false, true))
        getScanner()->emitError(XML4CErrs::ExpectedWhitespace);

    // Get a buffer for the element name and scan in the name
    XMLBufBid bbName(getBufMgr());
    if (!getReaderMgr()->getName(bbName.getBuffer()))
    {
        getScanner()->emitError(XML4CErrs::ExpectedElementName);
        getReaderMgr()->skipPastChar(chCloseAngle);
        return;
    }

    // Look this guy up in the element decl pool
    DTDElementDecl* decl = fElemDeclPool->getByKey(bbName.getRawBuffer());

    //
    //  If it does not exist, then we need to create it. If it does and
    //  its marked as declared, then that's an error, but we still need to
    //  scan over the content model so use the dummy declaration that the
    //  parsing code can fill in.
    //
    if (decl)
    {
        if (decl->isDeclared())
        {
            if (getScanner()->getDoValidation())
                emitError(XML4CValid::ElementAlreadyExists, bbName.getRawBuffer());

            if (!fDumElemDecl)
                fDumElemDecl = new DTDElementDecl(bbName.getRawBuffer());
            else
                fDumElemDecl->setName(bbName.getRawBuffer());
        }
    }
     else
    {
        //
        //  Create the new empty declaration to fill in and put it into
        //  the decl pool.
        //
        decl = new DTDElementDecl(bbName.getRawBuffer());
        fElemDeclPool->put(decl);
    }

    // Set a flag for whether we will ignore this one
    const bool isIgnored = (decl == fDumElemDecl);

    // Mark this one as being declared
    decl->setCreateReason(XMLElementDecl::Declared);

    // Another check for a PE ref, with at least required whitespace
    if (!checkForPERef(true, false, true))
        getScanner()->emitError(XML4CErrs::ExpectedWhitespace);

    // And now scan the content model for this guy.
    if (!scanContentSpec(*decl))
    {
        getReaderMgr()->skipPastChar(chCloseAngle);
        return;
    }

    // Another check for a PE ref, but we don't require whitespace here
    checkForPERef(false, false, true);

    // And we should have the ending angle bracket
    if (!getReaderMgr()->skippedChar(chCloseAngle))
    {
        getScanner()->emitError(XML4CErrs::UnterminatedElementDecl, bbName.getRawBuffer());
        getReaderMgr()->skipPastChar(chCloseAngle);
    }

    //
    //  If we have a DTD handler tell it about the new element decl. We
    //  tell it if its one that can be ignored, cause its an override of a
    //  previously existing decl. If it is being ignored, only call back
    //  if advanced callbacks are enabled.
    //
    if (fDocTypeHandler)
        fDocTypeHandler->elementDecl(*decl, isIgnored);
}


//
//  This method will process a general or parameter entity reference. The
//  entity name and entity text will be stored in the entity pool. The value
//  of the entity will be scanned for any other parameter entity or char
//  references which will be expanded. So the stored value can only have
//  general entity references when done.
//
void DTDValidator::scanEntityDecl()
{
    //
    //  Space is required here, but we cannot check for a PE Ref since
    //  there could be a legal (no-ref) percent sign here. Since any
    //  entity that ended here would be illegal, we just skip spaces
    //  and then check for a percent.
    //
    if (!getReaderMgr()->lookingAtSpace())
        getScanner()->emitError(XML4CErrs::ExpectedWhitespace);
    else
        getReaderMgr()->skipPastSpaces();
    const bool isPEDecl = getReaderMgr()->skippedChar(chPercent);

    //
    //  If a PE decl, then eat the percent and check for spaces or a
    //  PE ref on the other side of it. At least spaces are required.
    //
    if (isPEDecl)
    {
        if (!checkForPERef(true, false, true))
            getScanner()->emitError(XML4CErrs::ExpectedWhitespace);
    }

    //
    //  Now lets get a name, which should be the name of the entity. We
    //  have to get a buffer for this.
    //
    XMLBufBid bbName(getBufMgr());
    if (!getReaderMgr()->getName(bbName.getBuffer()))
    {
        getScanner()->emitError(XML4CErrs::ExpectedPEName);
        getReaderMgr()->skipPastChar(chCloseAngle);
        return;
    }

    // If namespaces are enabled, then no colons allowed
    if (getScanner()->getDoNamespaces())
    {
        if (XMLString::indexOf(bbName.getRawBuffer(), chColon) != -1)
            getScanner()->emitError(XML4CErrs::ColonNotLegalWithNS);
    }

    //
    //  See if this entity already exists. If so, then the existing one
    //  takes precendence. So we use the local dummy decl to parse into
    //  and just ignore the results.
    //
    DTDEntityDecl* entityDecl = 0;
    if (isPEDecl)
        entityDecl = fPEntityDeclPool->getByKey(bbName.getRawBuffer());
    else
        entityDecl = fEntityDeclPool->getByKey(bbName.getRawBuffer());

    if (entityDecl)
    {
        if (!fDumEntityDecl)
            fDumEntityDecl = new DTDEntityDecl;
        fDumEntityDecl->setName(bbName.getRawBuffer());
        entityDecl = fDumEntityDecl;
    }
     else
    {
        // Its not in existence already, then create an entity decl for it
        entityDecl = new DTDEntityDecl(bbName.getRawBuffer());

        //
        //  Set the declaration location. The parameter indicates whether its
        //  declared in the content/internal subset, so we know whether or not
        //  its in the external subset.
        //
        entityDecl->setDeclaredInIntSubset(fInternalSubset);

        // Add it to the appropriate entity decl pool
        if (isPEDecl)
            fPEntityDeclPool->put(entityDecl);
        else
            fEntityDeclPool->put(entityDecl);

    }

    // Set a flag that indicates whether we are ignoring this one
    const bool isIgnored = (entityDecl == fDumEntityDecl);

    // Set the PE flag on it
    entityDecl->setIsParameter(isPEDecl);

    //
    //  Space is legal (required actually) here so check for a PE ref. If
    //  we don't get our whitespace, then issue an error, but try to keep
    //  going.
    //
    if (!checkForPERef(true, false, true))
        getScanner()->emitError(XML4CErrs::ExpectedWhitespace);

    // According to the type call the value scanning method
    if (!scanEntityDef(*entityDecl, isPEDecl))
    {
        getReaderMgr()->skipPastChar(chCloseAngle);
        getScanner()->emitError(XML4CErrs::ExpectedEntityValue);
        return;
    }

    // Space is legal (but not required) here so check for a PE ref
    checkForPERef(false, false, true);

    // And then we have to have the closing angle bracket
    if (!getReaderMgr()->skippedChar(chCloseAngle))
    {
        getScanner()->emitError(XML4CErrs::UnterminatedEntityDecl);
        getReaderMgr()->skipPastChar(chCloseAngle);
    }

    //
    //  If we have a doc type handler, then call it. But only call it for
    //  ignored elements if advanced callbacks are enabled.
    //
    if (fDocTypeHandler)
        fDocTypeHandler->entityDecl(*entityDecl, isPEDecl, isIgnored);
}


//
//  This method will scan a general/character entity ref. It will either
//  expand a char ref and return the value directly, or it will expand
//  a general entity and a reader for it onto the reader stack.
//
//  The return value indicates whether the value was returned directly or
//  pushed as a reader or it failed.
//
//  The escaped flag tells the caller whether the returnd parameter resulted
//  from a character reference, which escapes the character in some cases. It
//  only makes any difference if the return indicates the value was returned
//  directly.
//
//  NOTE: This is only called when scanning attribute values, so we always
//  expand general entities.
//
DTDValidator::EntityExpRes
DTDValidator::scanEntityRef(XMLCh& firstCh, XMLCh& secondCh, bool& escaped)
{
    // Assume no escape and no second char
    escaped = false;
    secondCh = 0;

    // We have to insure its all done in a single entity
    const unsigned int curReader = getReaderMgr()->getCurrentReaderNum();

    //
    //  If the next char is a pound, then its a character reference and we
    //  need to expand it always.
    //
    if (getReaderMgr()->skippedChar(chPound))
    {
        //
        //  Its a character reference, so scan it and get back the numeric
        //  value it represents. If it fails, just return immediately.
        //
        if (!scanCharRef(firstCh, secondCh))
            return EntityExp_Failed;

        if (curReader != getReaderMgr()->getCurrentReaderNum())
            getScanner()->emitError(XML4CErrs::PartialMarkupInEntity);

        // Its now escaped since it was a char ref
        escaped = true;
        return EntityExp_Returned;
    }

    // Get the name of the general entity
    XMLBufBid bbName(getBufMgr());
    if (!getReaderMgr()->getName(bbName.getBuffer()))
    {
        getScanner()->emitError(XML4CErrs::ExpectedEntityRefName);
        return EntityExp_Failed;
    }

    //
    //  Next char must be a semi-colon. But if its not, just emit
    //  an error and try to continue.
    //
    if (!getReaderMgr()->skippedChar(chSemiColon))
        getScanner()->emitError(XML4CErrs::UnterminatedEntityRef);

    // Make sure it was all in one entity reader
    if (curReader != getReaderMgr()->getCurrentReaderNum())
        getScanner()->emitError(XML4CErrs::PartialMarkupInEntity);

    // Look it up the name the general entity pool
    XMLEntityDecl* decl = findEntityDecl(bbName.getRawBuffer(), false);

    // If it does not exist, then obviously an error
    if (!decl)
    {
        getScanner()->emitError(XML4CErrs::EntityNotFound, bbName.getRawBuffer());
        return EntityExp_Failed;
    }

    //
    //  If we are a standalone document, then it has to have been declared
    //  in the internal subset. Keep going though.
    //
    if (getScanner()->getStandalone() && !decl->getDeclaredInIntSubset())
        getScanner()->emitError(XML4CErrs::IllegalRefInStandalone, bbName.getRawBuffer());

    //
    //  If its a special char reference, then its escaped and we can return
    //  it directly.
    //
    if (decl->getIsSpecialChar())
    {
        firstCh = decl->getValue()[0];
        escaped = true;
        return EntityExp_Returned;
    }

    if (decl->isExternal())
    {
        // If its unparsed, then its not valid here
        if (decl->isUnparsed())
        {
            getScanner()->emitError(XML4CErrs::NoUnparsedEntityRefs, bbName.getRawBuffer());
            return EntityExp_Failed;
        }

        // We are in an attribute value, so not valid. But keep going
        getScanner()->emitError(XML4CErrs::NoExtRefsInAttValue);

        // And now create a reader to read this entity
        InputSource* srcUsed;
        XMLReader* reader = getReaderMgr()->createReader
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
        //  If the creation failed then throw an exception
        //
        if (!reader)
            ThrowXML1(RuntimeException, XML4CExcepts::Gen_CouldNotOpenExtEntity, srcUsed->getSystemId());

        //
        //  Push the reader. If its a recursive expansion, then emit an error
        //  and return an failure.
        //
        if (!getReaderMgr()->pushReader(reader, decl))
        {
            getScanner()->emitError(XML4CErrs::RecursiveEntity, decl->getName());
            return EntityExp_Failed;
        }

        // Do a start entity reference event
        if (getScanner()->getDocHandler())
            getScanner()->getDocHandler()->startEntityReference(*decl);

        // If it starts with the XML string, then parse a text decl
        if (getReaderMgr()->skippedString(XMLUni::fgXMLDeclString))
            scanTextDecl();
    }
     else
    {
        //
        //  Create a reader over a memory stream over the entity value
        //  We force it to assume UTF-16 by passing in an encoding
        //  string. This way it won't both trying to predecode the
        //  first line, looking for an XML/TextDecl.
        //
        XMLReader* valueReader = getReaderMgr()->createIntEntReader
        (
            decl->getName()
            , XMLReader::RefFrom_NonLiteral
            , XMLReader::Type_General
            , decl->getValue()
            , decl->getValueLen()
            , false
        );

        //
        //  Trt to push the entity reader onto the reader manager stack,
        //  where it will become the subsequent input. If it fails, that
        //  means the entity is recursive, so issue an error. The reader
        //  will have just been discarded, but we just keep going.
        //
        if (!getReaderMgr()->pushReader(valueReader, decl))
            getScanner()->emitError(XML4CErrs::RecursiveEntity, decl->getName());

        // Do a start entity reference event
        if (getScanner()->getDocHandler())
            getScanner()->getDocHandler()->startEntityReference(*decl);
    }

    return EntityExp_Pushed;
}


//
//  This method will scan a quoted literal of an entity value. It has to
//  deal with replacement of PE references; however, since this is a DTD
//  scanner, all such entity literals are in entity decls and therefore
//  general entities are not expanded.
//
bool DTDValidator::scanEntityLiteral(XMLBuffer& toFill, const bool isPE)
{
    toFill.reset();

    // Get the next char which must be a single or double quote
    XMLCh quoteCh;
    if (!getReaderMgr()->skipIfQuote(quoteCh))
        return false;

    // Get a buffer for pulling in entity names when we see GE refs
    XMLBufBid bbName(getBufMgr());
    XMLBuffer& nameBuf = bbName.getBuffer();

    // Remember the current reader
    const unsigned int orgReader = getReaderMgr()->getCurrentReaderNum();

    //
    //  Loop until we see the ending quote character, handling any references
    //  in the process.
    //
    XMLCh   nextCh;
    XMLCh   secondCh = 0;
    bool    gotLeadingSurrogate = false;
    while (true)
    {
        // Get the second char if we have one, else get another
        if (secondCh)
        {
            nextCh = secondCh;
            secondCh = 0;
        }
         else
        {
            nextCh = getReaderMgr()->getNextChar();
        }

        //
        //  Watch specifically for EOF and issue a more meaningful error
        //  if that occurs (since an unterminated quoted char can cause
        //  this easily.)
        //
        if (!nextCh)
        {
            getScanner()->emitError(XML4CErrs::UnterminatedEntityLiteral);
            ThrowXML(UnexpectedEOFException, XML4CExcepts::Gen_UnexpectedEOF);
        }

        //
        //  Break out on our terminating quote char when we are back in the
        //  same reader. Otherwise, we might trigger on a nested quote char
        //  in an expanded entity.
        //
        if ((nextCh == quoteCh)
        &&  (getReaderMgr()->getCurrentReaderNum() == orgReader))
        {
            break;
        }

        if (nextCh == chPercent)
        {
            //
            //  Put the PE's value on the reader stack and then jump back
            //  to the top to start processing it. The parameter indicates
            //  that it should not scan the reference's content as an external
            //  subset.
            //
            expandPERef(false, true, true);
            continue;
        }

        //
        //  Ok, now that all the other special stuff is checked, we can
        //  look for a general entity. In here, we cannot have a naked &
        //  and will only expand numerical char refs or the intrinsic char
        //  refs. Others will be left alone.
        //
        if (nextCh == chAmpersand)
        {
            //
            //  Here, we only expand numeric char refs, but not any general
            //  entities. However, the stupid XML spec requires that we check
            //  and make sure it does refer to a general entity if its not
            //  a char ref (i.e. no naked '&' chars.)
            //
            if (getReaderMgr()->skippedChar(chPound))
            {
                // If it failed, then just jump back to the top and try to pick up
                if (!scanCharRef(nextCh, secondCh))
                {
                    gotLeadingSurrogate = false;
                    continue;
                }
            }
             else
            {
                if (!getReaderMgr()->getName(nameBuf))
                {
                    getScanner()->emitError(XML4CErrs::ExpectedEntityRefName);
                }
                 else
                {
                    //
                    //  Since we are not expanding any of this, we have to
                    //  put the amp and name into the target buffer as data.
                    //
                    toFill.append(chAmpersand);
                    toFill.append(nameBuf.getRawBuffer());

                    // Make sure we skipped a trailing semicolon
                    if (!getReaderMgr()->skippedChar(chSemiColon))
                        getScanner()->emitError(XML4CErrs::UnterminatedEntityRef);

                    // And make the new character the semicolon
                    nextCh = chSemiColon;
                }

                // Either way here we reset the surrogate flag
                gotLeadingSurrogate = false;
            }
        }

        if ((nextCh >= 0xD800) && (nextCh <= 0xDBFF))
        {
            if (gotLeadingSurrogate)
                getScanner()->emitError(XML4CErrs::Expected2ndSurrogateChar);
            else
                gotLeadingSurrogate = true;
        }
         else
        {
            if (gotLeadingSurrogate)
            {
                if ((nextCh < 0xDC00) && (nextCh > 0xDFFF))
                    getScanner()->emitError(XML4CErrs::Expected2ndSurrogateChar);
            }
             else if (!XMLReader::isXMLChar(nextCh))
            {
                getScanner()->emitError(XML4CErrs::InvalidCharacter);
                getReaderMgr()->skipPastChar(quoteCh);
                return false;
            }
            gotLeadingSurrogate = false;
        }

        // Looks ok, so add it to the literal
        toFill.append(nextCh);
    }

    //
    //  If we got here and did not get back to the original reader level,
    //  then we propogated some entity out of the literal, so issue an
    //  error, but don't fail.
    //
    if (getReaderMgr()->getCurrentReaderNum() != orgReader)
        getScanner()->emitError(XML4CErrs::PartialMarkupInEntity);

    return true;
}


//
//  This method is called after the entity name has been scanned, and any
//  PE referenced following the name is handled. The passed decl will be
//  filled in with the info scanned.
//
bool DTDValidator::scanEntityDef(DTDEntityDecl& decl, const bool isPEDecl)
{
    // Its got to be an entity literal
    if (getReaderMgr()->lookingAtChar(chSingleQuote)
    ||  getReaderMgr()->lookingAtChar(chDoubleQuote))
    {
        // Get a buffer for the literal
        XMLBufBid bbValue(getBufMgr());

        if (!scanEntityLiteral(bbValue.getBuffer(), isPEDecl))
            return false;

        // Set it on the entity decl
        decl.setValue(bbValue.getRawBuffer());
        return true;
    }

    //
    //  Its got to be an external entity, so there must be an external id.
    //  Get buffers for them and scan an external id into them.
    //
    XMLBufBid bbPubId(getBufMgr());
    XMLBufBid bbSysId(getBufMgr());
    if (!scanId(bbPubId.getBuffer(), bbSysId.getBuffer(), IDType_External))
        return false;

    // Fill in the id fields of the decl with the info we got
    decl.setPublicId(bbPubId.getRawBuffer());
    decl.setSystemId(bbSysId.getRawBuffer());

    // If its a PE decl, we are done
    bool gotSpaces = checkForPERef(false, false, true);
    if (isPEDecl)
    {
        //
        //  Check for a common error here. NDATA is not allowed for PEs
        //  so check for the NDATA string. If found give a nice meaningful
        //  error and continue parsing to eat the NDATA text.
        //
        if (gotSpaces)
        {
            if (getReaderMgr()->skippedString(XMLUni::fgNDATAString))
                getScanner()->emitError(XML4CErrs::NDATANotValidForPE);
        }
         else
        {
            return true;
        }
    }

    // If looking at close angle now, we are done
    if (getReaderMgr()->lookingAtChar(chCloseAngle))
        return true;

    // Else we had to have seem the whitespace    
    if (!gotSpaces)
        getScanner()->emitError(XML4CErrs::ExpectedWhitespace);

    // We now have to see a notation data string
    if (!getReaderMgr()->skippedString(XMLUni::fgNDATAString))
        getScanner()->emitError(XML4CErrs::ExpectedNDATA);

    // Space is required here, but try to go on if not
    if (!checkForPERef(false, false, true))
        getScanner()->emitError(XML4CErrs::ExpectedWhitespace);

    // Get a name
    XMLBufBid bbName(getBufMgr());
    if (!getReaderMgr()->getName(bbName.getBuffer()))
    {
        getScanner()->emitError(XML4CErrs::ExpectedNotationName);
        return false;
    }

    // Set the decl's notation name
    decl.setNotationName(bbName.getRawBuffer());
    
    return true;
}


//
//  This method is called after an attribute decl name or a notation decl has
//  been scanned and then an opening parenthesis was see, indicating the list
//  of values. It scans the enumeration values and creates a single string
//  which has a single space between each value.
//
//  The terminating close paren ends this scan.
//
bool DTDValidator::scanEnumeration(XMLBuffer& toFill, const bool notation)
{
    // Reset the passed buffer
    toFill.reset();

    // Check for PE ref but don't require space
    checkForPERef(false, false, true);

    // If this is a notation, we need an opening paren
    if (notation)
    {
        if (!getReaderMgr()->skippedChar(chOpenParen))
            getScanner()->emitError(XML4CErrs::ExpectedOpenParen);
    }

    // We need a local buffer to use as well
    XMLBufBid bbTmp(getBufMgr());

    while (true)
    {
        // Space is allowed here for either type so check for PE ref
        checkForPERef(false, false, true);

        // And then get either a name or a name token
        bool success;
        if (notation)
            success = getReaderMgr()->getName(bbTmp.getBuffer());
        else
            success = getReaderMgr()->getNameToken(bbTmp.getBuffer());

        if (!success)
        {
            getScanner()->emitError(XML4CErrs::ExpectedEnumValue);
            return false;
        }

        // Append this value to the target value
        toFill.append(bbTmp.getRawBuffer(), bbTmp.getLen());

        // Space is allowed here for either type so check for PE ref
        checkForPERef(false, false, true);

        // Check for the terminating paren
        if (getReaderMgr()->skippedChar(chCloseParen))
            break;

        // And append a space separator
        toFill.append(chSpace);

        // Check for the pipe character separator
        if (!getReaderMgr()->skippedChar(chPipe))
        {
            getScanner()->emitError(XML4CErrs::ExpectedEnumSepOrParen);
            return false;
        }
    }
    return true;
}


bool DTDValidator::scanEq()
{
    getReaderMgr()->skipPastSpaces();
    if (getReaderMgr()->skippedChar(chEqual))
    {
        getReaderMgr()->skipPastSpaces();
        return true;
    }
    return false;
}


//
//  This method is called when an external entity reference is seen in the
//  DTD or an external DTD subset is encountered, and their contents pushed
//  onto the reader stack. This method will scan that contents.
//
void DTDValidator::scanExtSubsetDecl(const bool inIncludeSect)
{
    bool bAcceptDecl = !inIncludeSect;

    // Get a buffer for whitespace
    XMLBufBid bbSpace(getBufMgr());

    //
    //  If we have a doc type handler and we are not being called recursively
    //  to handle an include section, tell it the ext subset starts
    //
    if (fDocTypeHandler && !inIncludeSect)
        fDocTypeHandler->startExtSubset();

    //
    //  We have to play a trick here if the current entity we are parsing
    //  is a PE. Because the spooling code will put out a whitespace before
    //  and after an expanded PE if its being scanned outside the context of
    //  a literal entity, this will confuse this external subset code.
    //
    //  So, we see if that is what is happening and, if so, eat the single
    //  space, a check for the <?xml string. If we find it, we parse that
    //  markup right now and put the space back.
    //
    if (getReaderMgr()->isScanningPERefOutOfLiteral())
    {
        if (getReaderMgr()->skippedSpace())
        {
            if (getReaderMgr()->skippedString(XMLUni::fgXMLDeclStringSpace))
            {
                scanTextDecl();
                bAcceptDecl = false;

                // <TBD> Figure out how to do this
                // getReaderMgr()->unGet(chSpace);
            }
        }
    }

    // Get the current reader number
    const unsigned int orgReader = getReaderMgr()->getCurrentReaderNum();

    //
    //  Loop until we hit the end of the external subset entity. Note that
    //  we use a double loop here in order to avoid the overhead of doing
    //  the exception setup/teardown work on every loop.
    //
    bool inMarkup = false;
    bool inCharData = false;
    while (true)
    {
    try
    {
        while (true)
        {
            const XMLCh nextCh = getReaderMgr()->peekNextChar();

            if (nextCh == chOpenAngle)
            {
                // Get the reader we started this on
                const unsigned int orgReader = getReaderMgr()->getCurrentReaderNum();

                //
                //  Now scan the markup. Set the flag so that we will know that
                //  we were in markup if an end of entity exception occurs.
                //
                getReaderMgr()->getNextChar();
                inMarkup = true;
                scanMarkupDecl(bAcceptDecl);
                inMarkup = false;

                //
                //  And see if we got back to the same level. If not, then its
                //  a partial markup error.
                //
                if (getReaderMgr()->getCurrentReaderNum() != orgReader)
                    getScanner()->emitError(XML4CErrs::PartialMarkupInEntity);
            }
             else if (XMLReader::isWhitespace(nextCh))
            {
                //
                //  If we have a doc type handler, and advanced callbacks are
                //  enabled, then gather up whitespace and call back. Otherwise
                //  just skip whitespaces.
                //
                if (fDocTypeHandler)
                {
                    inCharData = true;
                    getReaderMgr()->getSpaces(bbSpace.getBuffer());
                    inCharData = false;

                    fDocTypeHandler->doctypeWhitespace
                    (
                        bbSpace.getRawBuffer()
                        , bbSpace.getLen()
                    );
                }
                 else
                {
                    //
                    //  If we hit an end of entity in the middle of white
                    //  space, that's fine. We'll just come back in here
                    //  again on the next round and skip some more.
                    //
                    getReaderMgr()->skipPastSpaces();
                }
            }
             else if (nextCh == chPercent)
            {
                //
                //  Expand (and scan if external) the reference value. Tell
                //  it to throw an end of entity exception at the end of the
                //  entity.
                //
                getReaderMgr()->getNextChar();
                expandPERef(true, false, false, true);
            }
             else if (inIncludeSect && (nextCh == chCloseSquare))
            {
                //
                //  Its the end of a conditional include section. So scan it and
                //  decrement the include depth counter.
                //
                getReaderMgr()->getNextChar();
                if (!getReaderMgr()->skippedChar(chCloseSquare))
                {
                    getScanner()->emitError(XML4CErrs::ExpectedEndOfConditional);
                    getReaderMgr()->skipPastChar(chCloseAngle);
                }
                 else if (!getReaderMgr()->skippedChar(chCloseAngle))
                {
                    getScanner()->emitError(XML4CErrs::ExpectedEndOfConditional);
                    getReaderMgr()->skipPastChar(chCloseAngle);
                }
                return;
            }
             else
            {
                getReaderMgr()->getNextChar();
                if (!XMLReader::isXMLChar(nextCh))
                    getScanner()->emitError(XML4CErrs::InvalidCharacter);
                else
                    getScanner()->emitError(XML4CErrs::InvalidDocumentStructure);

                // Try to get realigned
                static const XMLCh toSkip[] =
                {
                    chPercent, chCloseSquare, chOpenAngle, chNull
                };
                getReaderMgr()->skipUntilInOrWS(toSkip);
            }
            bAcceptDecl = false;
        }
    }

    catch(const EndOfEntityException& toCatch)
    {
        //
        //  If the external entity ended while we were in markup, then that's
        //  a partial markup error.
        //
        if (inMarkup)
        {
            getScanner()->emitError(XML4CErrs::PartialMarkupInEntity);
            inMarkup = false;
        }

        // If we were in char data, then send what we got
        if (inCharData)
        {
            // Send what we got, then rethrow
            if (fDocTypeHandler)
            {
                fDocTypeHandler->doctypeWhitespace
                (
                    bbSpace.getRawBuffer()
                    , bbSpace.getLen()
                );
            }
            inCharData = false;
        }

        //
        //  If the entity that just ended was the entity that we started
        //  on, then this is the end of the external subset.
        //
        if (orgReader == toCatch.getReaderNum())
            break;
    }
    }

    // If we have a doc type handler, tell it the ext subset ends
    if (fDocTypeHandler)
        fDocTypeHandler->endExtSubset();
}


//
//  This method will scan for an id, either public or external.
//
bool DTDValidator::scanId(          XMLBuffer&  pubIdToFill
                            ,       XMLBuffer&  sysIdToFill
                            , const IDTypes     whatKind)
{
    // Clean out both return buffers
    pubIdToFill.reset();
    sysIdToFill.reset();

    //
    //  Check first for the system id first. If we find it, and system id
    //  is one of the legal values, then lets try to scan it.
    //
    if (getReaderMgr()->skippedString(XMLUni::fgSysIDString))
    {
        // If they were looking for a public id, then we failed
        if (whatKind == IDType_Public)
        {
            getScanner()->emitError(XML4CErrs::ExpectedPublicId);
            return false;
        }

        // We must skip spaces
        if (!getReaderMgr()->skipPastSpaces())
        {
            getScanner()->emitError(XML4CErrs::ExpectedWhitespace);
            return false;
        }

        // Get the system literal value
        return scanSystemLiteral(sysIdToFill);
    }

    // See if we have a public id string. If not, we are done and found nothing
    if (!getReaderMgr()->skippedString(XMLUni::fgPubIDString))
        return false;

    //
    //  So following this we must have whitespace, a public literal, whitespace,
    //  and a system literal.
    //
    if (!getReaderMgr()->skipPastSpaces())
    {
        getScanner()->emitError(XML4CErrs::ExpectedWhitespace);

        //
        //  Just in case, if they just forgot the whitespace but the next char
        //  is a single or double quote, then keep going.
        //
        const XMLCh chPeek = getReaderMgr()->peekNextChar();
        if ((chPeek != chDoubleQuote) && (chPeek != chSingleQuote))
            return false;
    }

    if (!scanPublicLiteral(pubIdToFill))
    {
        getScanner()->emitError(XML4CErrs::ExpectedPublicId);
        return false;
    }

    // If they wanted a public id, then this is all
    if (whatKind == IDType_Public)
        return true;

    // Else lets get the system id
    if (!getReaderMgr()->skipPastSpaces())
    {
        //
        //  In order to recover best here we need to see if we don't have
        //  whitespace because the next thing is a quote or because the next
        //  thing is some non-quote character.
        //
        const XMLCh chPeek = getReaderMgr()->peekNextChar();
        const bool bIsQuote =  ((chPeek == chDoubleQuote)
                               || (chPeek == chSingleQuote));

        if (whatKind == IDType_External)
        {
            //
            //  If its an external Id, then we need to see the system id.
            //  So, emit the error. But, if the next char is a quote, don't
            //  give up since its probably going to work. The user just
            //  missed the separating space. Otherwise, fail.
            //
            getScanner()->emitError(XML4CErrs::ExpectedWhitespace);
            if (!bIsQuote)
                return false;
        }
         else
        {
            //
            //  We can legally return here. But, if the next char is a quote,
            //  then that's probably not what was desired, since its probably
            //  just that space was forgotten and there really is a system
            //  id to follow.
            //
            //  So treat it like missing whitespace if so and keep going.
            //  Else, just return success.
            //
            if (bIsQuote)
                getScanner()->emitError(XML4CErrs::ExpectedWhitespace);
             else
                return true;
        }
    }

    if (!scanSystemLiteral(sysIdToFill))
    {
        // Its only an error to not have it if we are doing an external
        if (whatKind == IDType_External)
        {
            getScanner()->emitError(XML4CErrs::ExpectedSystemId);
            return false;
        }
    }
    return true;
}


//
//  This method will scan the contents of an ignored section. It assumes that
//  we already are in the body, i.e. we've seen <![IGNORE[ at this point. So
//  we have to just scan until we see a matching ]]> closing markup.
//
void DTDValidator::scanIgnoredSection()
{
    //
    //  Depth starts at one because we are already in one section and want
    //  to parse until we hit its end.
    //
    unsigned long depth = 1;
    while (true)
    {
        const XMLCh nextCh = getReaderMgr()->getNextChar();

        if (!nextCh)
            ThrowXML(UnexpectedEOFException, XML4CExcepts::Gen_UnexpectedEOF);

        if (nextCh == chOpenAngle)
        {
            if (getReaderMgr()->skippedChar(chBang)
            &&  getReaderMgr()->skippedChar(chOpenSquare))
            {
                depth++;
            }
        }
         else if (nextCh == chCloseSquare)
        {
            if (getReaderMgr()->skippedChar(chCloseSquare))
            {
                while (getReaderMgr()->skippedChar(chCloseSquare))
                {
                    // Do nothing, just skip them
                }

                if (getReaderMgr()->skippedChar(chCloseAngle))
                {
                    depth--;
                    if (!depth)
                        break;
                }
            }
        }
         else if (!XMLReader::isXMLChar(nextCh))
        {
            getScanner()->emitError(XML4CErrs::InvalidCharacter);
        }
    }
}


//
//  This method scans the entire internal subset. All we can have here is
//  decl markup, and PE references. The expanded PE references must contain
//  whole markup, so we don't have to worry about their content at this
//  level. We just scan them, expand them, push them, and parse their content
//  right there, via the expandERef() method.
//
bool DTDValidator::scanInternalSubset()
{
    // If we have a doc type handler, tell it the internal subset starts
    if (fDocTypeHandler)
        fDocTypeHandler->startIntSubset();

    // Get a buffer for whitespace
    XMLBufBid bbSpace(getBufMgr());

    bool noErrors = true;
    while (true)
    {
        const XMLCh nextCh = getReaderMgr()->peekNextChar();

        //
        //  If we get an end of file marker, just unget it and return a
        //  failure status. The caller will then see the end of file and
        //  faill out correctly.
        //
        if (!nextCh)
            return false;

        // Watch for the end of internal subset marker
        if (nextCh == chCloseSquare)
        {
            getReaderMgr()->getNextChar();
            break;
        }

        if (nextCh == chPercent)
        {
            //
            //  Expand (and scan if external) the reference value. Tell
            //  it to set the reader to cause an end of entity exception
            //  when this reader dies, which is what the scanExtSubset
            //  method wants (who is called to scan this.)
            //
            getReaderMgr()->getNextChar();
            expandPERef(true, false, false, true);
        }
         else if (nextCh == chOpenAngle)
        {
            // Remember this reader before we start the scan
            const unsigned int orgReader = getReaderMgr()->getCurrentReaderNum();

            // And scan this markup
            getReaderMgr()->getNextChar();
            scanMarkupDecl(false);

            // If we did not get back to entry level, then partial markup
            if (getReaderMgr()->getCurrentReaderNum() != orgReader)
                getScanner()->emitError(XML4CErrs::PartialMarkupInEntity);
        }
         else if (XMLReader::isWhitespace(nextCh))
        {
            //
            //  IF we are doing advanced callbacks and have a doc type
            //  handler, then get the whitespace and call the doc type
            //  handler with it. Otherwise, just skip whitespace.
            //
            if (fDocTypeHandler)
            {
                getReaderMgr()->getSpaces(bbSpace.getBuffer());
                fDocTypeHandler->doctypeWhitespace
                (
                    bbSpace.getRawBuffer()
                    , bbSpace.getLen()
                );
            }
             else
            {
                getReaderMgr()->skipPastSpaces();
            }
        }
         else
        {
            getReaderMgr()->getNextChar();

            // Not valid, so emit an error
            getScanner()->emitError(XML4CErrs::InvalidCharacterInIntSubset);

            //
            //  If an '>', then probably an abnormally terminated
            //  internal subset so just return.
            //
            if (nextCh == chCloseAngle)
            {
                noErrors = false;
                break;
            }

            //
            //  Otherwise, try to sync back up by scanning forward for
            //  a reasonable start character.
            //
            static const XMLCh toSkip[] =
            {
                chPercent, chCloseSquare, chOpenAngle, chNull
            };
            getReaderMgr()->skipUntilInOrWS(toSkip);
        }
    }

    // If we have a doc type handler, tell it the internal subset ends
    if (fDocTypeHandler)
        fDocTypeHandler->endIntSubset();

    return noErrors;
}


//
//  This method is called once we see a < in the input of an int/ext subset,
//  which indicates the start of some sort of markup.
//
void DTDValidator::scanMarkupDecl(const bool parseTextDecl)
{
    //
    //  We only have two valid first characters here. One is a ! which opens
    //  some markup decl. The other is a ?, which could begin either a PI
    //  or a text decl. If parseTextDecl is false, we cannot accept a text
    //  decl.
    //
    const XMLCh nextCh = getReaderMgr()->getNextChar();

    if (nextCh == chBang)
    {
        if (getReaderMgr()->skippedChar(chDash))
        {
            if (getReaderMgr()->skippedChar(chDash))
            {
                scanComment();
            }
             else
            {
                getScanner()->emitError(XML4CErrs::CommentsMustStartWith);
                getReaderMgr()->skipPastChar(chCloseAngle);
            }
        }
         else if (getReaderMgr()->skippedChar(chOpenSquare))
        {
            //
            //  Its a conditional section. This is only valid in the external
            //  subset, so issue an error if we aren't there.
            //
            if (fInternalSubset)
            {
                getScanner()->emitError(XML4CErrs::ConditionalSectInIntSubset);
                getReaderMgr()->skipPastChar(chCloseAngle);
                return;
            }

            // A PE ref can happen here, but space is not required
            checkForPERef(false, false, true);

            if (getReaderMgr()->skippedString(XMLUni::fgIncludeString))
            {
                checkForPERef(false, false, true);

                // Check for the following open square bracket
                if (!getReaderMgr()->skippedChar(chOpenSquare))
                    getScanner()->emitError(XML4CErrs::ExpectedINCLUDEBracket);

                checkForPERef(false, false, true);

                //
                //  Recurse back to the ext subset call again, telling it its
                //  in an include section.
                //
                scanExtSubsetDecl(true);
            }
             else if (getReaderMgr()->skippedString(XMLUni::fgIgnoreString))
            {
                checkForPERef(false, false, true);

                // Check for the following open square bracket
                if (!getReaderMgr()->skippedChar(chOpenSquare))
                    getScanner()->emitError(XML4CErrs::ExpectedINCLUDEBracket);

                checkForPERef(false, false, true);

                // And scan over the ignored part
                scanIgnoredSection();
            }
             else
            {
                getScanner()->emitError(XML4CErrs::ExpectedIncOrIgn);
                getReaderMgr()->skipPastChar(chCloseAngle);
            }
        }
         else if (getReaderMgr()->skippedString(XMLUni::fgAttListString))
        {
            scanAttListDecl();
        }
         else if (getReaderMgr()->skippedString(XMLUni::fgElemString))
        {
            scanElementDecl();
        }
         else if (getReaderMgr()->skippedString(XMLUni::fgEntityString))
        {
            scanEntityDecl();
        }
         else if (getReaderMgr()->skippedString(XMLUni::fgNotationString))
        {
            scanNotationDecl();
        }
         else
        {
            getScanner()->emitError(XML4CErrs::ExpectedMarkupDecl);
            getReaderMgr()->skipPastChar(chCloseAngle);
        }
    }
     else if (nextCh == chQuestion)
    {
        // It could be a PI or the XML declaration. Check for Decl
        bool gotDecl = getReaderMgr()->skippedString(XMLUni::fgXMLStringSpace);

        //
        //  Just in case, check for upper case. If found, issue
        //  an error, but keep going.
        //
        if (!gotDecl)
        {
            gotDecl = getReaderMgr()->skippedString(XMLUni::fgXMLStringSpaceU);
            if (gotDecl)
                getScanner()->emitError(XML4CErrs::XMLDeclMustBeLowerCase);
        }

        if (gotDecl)
        {
            // If we are not accepting text decls, its an error
            if (parseTextDecl)
            {
                scanTextDecl();
            }
             else
            {
                // Emit the error and skip past this markup
                getScanner()->emitError(XML4CErrs::TextDeclNotLegalHere);
                getReaderMgr()->skipPastChar(chCloseAngle);
            }
        }
         else
        {
            // It has to be a PI
            scanPI();
        }
    }
     else
    {
        // Can't be valid so emit error and try to skip past end of this decl
        getScanner()->emitError(XML4CErrs::ExpectedMarkupDecl);
        getReaderMgr()->skipPastChar(chCloseAngle);
    }
}


//
//  This method is called for a mixed model element's content mode. We've
//  already scanned past the '(PCDATA' part by the time we get here. So
//  everything else is element names separated by | characters until we
//  hit the end. The passed element decl's content model is filled in with
//  the information found.
//
bool DTDValidator::scanMixed(DTDElementDecl& toFill)
{
    //
    //  The terminating star is only required if there is something more
    //  than (PCDATA).
    //
    bool starRequired = false;

    // Get a buffer to be used below to get element names
    XMLBufBid bbName(getBufMgr());
    XMLBuffer& nameBuf = bbName.getBuffer();

    //
    //  Create an initial content spec node. Its just a leaf node with a
    //  PCDATA element id. This current node pointer will be pushed down the
    //  tree as we go.
    //
    ContentSpecNode* curNode = new ContentSpecNode(XMLElementDecl::fgPCDataElemId);

    //
    //  Set the initial leaf as the temporary head. If we hit the first choice
    //  node, it will be set up here. When done, this is the node that's set
    //  as the content spec for the element.
    //
    ContentSpecNode* headNode = curNode;

    // Remember the original node so we can sense the first choice node
    ContentSpecNode* orgNode = curNode;

    //
    //  We just loop around, getting the | character at the top and then
    //  looking for the next element name. We keep up with the last node
    //  and add each new one to its right node.
    //
    while (true)
    {
        // Spaces are legal here, so check for a PE ref, but don't require space
        checkForPERef(false, false, true);

        //
        //  If its a star, then tell them they can't have reps in
        //  mixed model, but eat it and keep going.
        //
        if (getReaderMgr()->skippedChar(chAsterisk))
        {
            getScanner()->emitError(XML4CErrs::NoRepInMixed);
            continue;
        }

        // Check for the next choice indicator
        if (!getReaderMgr()->skippedChar(chPipe))
        {
            // Has to be the closing paren now.
            if (!getReaderMgr()->skippedChar(chCloseParen))
            {
                getScanner()->emitError(XML4CErrs::UnterminatedContentModel);
                delete headNode;
                return false;
            }

            if (!getReaderMgr()->skippedChar(chAsterisk) && starRequired)
                getScanner()->emitError(XML4CErrs::ExpectedAsterisk);

            //
            //  Create a zero or more node and make the original head
            //  node its first child.
            //
            headNode = new ContentSpecNode
            (
                ContentSpecNode::ZeroOrMore
                , headNode
                , 0
            );

            // Store the head node as the content spec of the element.
            toFill.setContentSpec(headNode);
            break;
        }

        // Its more than just a PCDATA, so an ending star will be required now
        starRequired = true;

        // Space is legal here so check for a PE ref, but don't require space
        checkForPERef(false, false, true);

        // Get a name token
        if (!getReaderMgr()->getName(nameBuf))
        {
            getScanner()->emitError(XML4CErrs::ExpectedElementName);
            delete headNode;
            return false;
        }

        //
        //  Create a leaf node for it. If we can find the element id for
        //  this element, then use it. Else, we have to fault in an element
        //  decl, marked as created because of being in a content model.
        //
        unsigned int elemId = findElemId(nameBuf.getRawBuffer());
        if (elemId == XMLElementDecl::fgInvalidElemId)
        {
            DTDElementDecl* decl = new DTDElementDecl(nameBuf.getRawBuffer());
            decl->setCreateReason(XMLElementDecl::InContentModel);
            fElemDeclPool->put(decl);
            elemId = decl->getId();
        }

        //
        //  If the current node is the original node, this is the first choice
        //  node, so create an initial choice node with the current node and
        //  the new element id. Store this as the head node.
        //
        //  Otherwise, we have to steal the right node of the previous choice
        //  and weave in another choice node there, which has the old choice
        //  as its left and the new leaf as its right.
        //
        if (curNode == orgNode)
        {
            curNode = new ContentSpecNode
            (
                ContentSpecNode::Choice
                , curNode
                , new ContentSpecNode(elemId)
            );

            // Remember the top node
            headNode = curNode;
        }
         else
        {
            ContentSpecNode* oldRight = curNode->orphanSecond();
            curNode->setSecond
            (
                new ContentSpecNode
                (
                    ContentSpecNode::Choice
                    , oldRight
                    , new ContentSpecNode(elemId)
                )
            );

            // Make the new right node the current node
            curNode = curNode->getSecond();
        }
    }

    return true;
}


//
//  This method is called when we see a '<!NOTATION' string while scanning
//  markup decl. It parses out the notation and its id and stores a new
//  notation decl object in the notation decl pool.
//
void DTDValidator::scanNotationDecl()
{
    // Space is required here so check for a PE ref, and require space
    if (!checkForPERef(true, false, true))
    {
        getScanner()->emitError(XML4CErrs::ExpectedWhitespace);
        getReaderMgr()->skipPastChar(chCloseAngle);
        return;
    }

    //
    //  And now we get a name, which is the name of the notation. Get a
    //  buffer for the name.
    //
    XMLBufBid bbName(getBufMgr());
    if (!getReaderMgr()->getName(bbName.getBuffer()))
    {
        getScanner()->emitError(XML4CErrs::ExpectedNotationName);
        getReaderMgr()->skipPastChar(chCloseAngle);
        return;
    }

    // If namespaces are enabled, then no colons allowed
    if (getScanner()->getDoNamespaces())
    {
        if (XMLString::indexOf(bbName.getRawBuffer(), chColon) != -1)
            getScanner()->emitError(XML4CErrs::ColonNotLegalWithNS);
    }

    // Space is required here so check for a PE ref, and require space
    if (!checkForPERef(true, false, true))
    {
        getScanner()->emitError(XML4CErrs::ExpectedWhitespace);
        getReaderMgr()->skipPastChar(chCloseAngle);
        return;
    }

    //
    //  And scan an external or public id. We need buffers to use for both
    //  of these.
    //
    XMLBufBid bbPubId(getBufMgr());
    XMLBufBid bbSysId(getBufMgr());
    if (!scanId(bbPubId.getBuffer(), bbSysId.getBuffer(), IDType_Either))
    {
        getReaderMgr()->skipPastChar(chCloseAngle);
        return;
    }

    // We can have an optional space or PE ref here
    checkForPERef(false, false, true);

    //
    //  See if it already exists. If so, add it to the notatino decl pool.
    //  Otherwise, if advanced callbacks are on, create a temp one and
    //  call out for that one.
    //
    XMLNotationDecl* decl = fNotationDeclPool->getByKey(bbName.getRawBuffer());
    bool isIgnoring = (decl != 0);
    if (isIgnoring)
    {
        getScanner()->emitError(XML4CErrs::NotationAlreadyExists, bbName.getRawBuffer());
    }
     else
    {
        // Fill in a new notation declaration and add it to the pool
        decl = new XMLNotationDecl
        (
            bbName.getRawBuffer()
            , bbPubId.getRawBuffer()
            , bbSysId.getRawBuffer()
        );
        fNotationDeclPool->put(decl);
    }

    //
    //  If we have a document type handler, then tell it about this. If we
    //  are ignoring it, only call out if advanced callbacks are enabled.
    //
    if (fDocTypeHandler)
    {
        fDocTypeHandler->notationDecl
        (
            *decl
            , isIgnoring
        );
    }

    // And one more optional space or PE ref
    checkForPERef(false, false, true);

    // And skip the terminating bracket
    if (!getReaderMgr()->skippedChar(chCloseAngle))
        getScanner()->emitError(XML4CErrs::UnterminatedNotationDecl);
}


//
//  Scans a PI and calls the appropriate callbacks. A PI can happen in either
//  the document or the DTD, so it calls the appropriate handler according
//  to the fInDocument flag.
//
//  At entry we have just scanned the <? part, and need to now start on the
//  PI target name.
//
void DTDValidator::scanPI()
{
    const XMLCh* namePtr = 0;
    const XMLCh* targetPtr = 0;

    // And skip any subsequent spaces before the name
    getReaderMgr()->skipPastSpaces();

    // Get a buffer for the PI name and scan it in
    XMLBufBid bbName(getBufMgr());
    if (!getReaderMgr()->getNameToken(bbName.getBuffer()))
    {
        getScanner()->emitError(XML4CErrs::PINameExpected);
        getReaderMgr()->skipPastChar(chCloseAngle);
        return;
    }

    // Point the name pointer at the raw data
    namePtr = bbName.getRawBuffer();

    // See if it issome form of 'xml' and emit a warning
    if (!XMLString::compareIString(namePtr, XMLUni::fgXMLString))
        getScanner()->emitError(XML4CErrs::NoPIStartsWithXML);

    // If namespaces are enabled, then no colons allowed
    if (getScanner()->getDoNamespaces())
    {
        if (XMLString::indexOf(namePtr, chColon) != -1)
            getScanner()->emitError(XML4CErrs::ColonNotLegalWithNS);
    }

    //
    //  If we don't hit a space next, then the PI has no target. If we do
    //  then get out the target. Get a buffer for it as well
    //
    XMLBufBid bbTarget(getBufMgr());
    if (getReaderMgr()->skippedSpace())
    {
        // Skip any leading spaces
        getReaderMgr()->skipPastSpaces();

        // It does have a target, so lets move on to deal with that.
        while (1)
        {
            const XMLCh nextCh = getReaderMgr()->getNextChar();

            // Watch for an end of file, which is always bad here
            if (!nextCh)
            {
                getScanner()->emitError(XML4CErrs::UnterminatedPI);
                ThrowXML(UnexpectedEOFException, XML4CExcepts::Gen_UnexpectedEOF);
            }

            // Watch for potential terminating character
            if (nextCh == chQuestion)
            {
                // It must be followed by '>' to be a termination of the target
                if (getReaderMgr()->skippedChar(chCloseAngle))
                    break;
            }

            // Watch for invalid chars but try to keep going
            if (!XMLReader::isXMLChar(nextCh))
                getScanner()->emitError(XML4CErrs::InvalidCharacter);

            bbTarget.append(nextCh);
        }
    }
     else
    {
        // No target, but make sure its terminated ok
        if (!getReaderMgr()->skippedChar(chQuestion))
        {
            getScanner()->emitError(XML4CErrs::UnterminatedPI);
            getReaderMgr()->skipPastChar(chCloseAngle);
            return;
        }

        if (!getReaderMgr()->skippedChar(chCloseAngle))
        {
            getScanner()->emitError(XML4CErrs::UnterminatedPI);
            getReaderMgr()->skipPastChar(chCloseAngle);
            return;
        }
    }

    // Point the target pointer at the raw data
    targetPtr = bbTarget.getRawBuffer();

    //
    //  If we have a handler, then call it.
    //
    if (fDocTypeHandler)
    {
        fDocTypeHandler->doctypePI
        (
            namePtr
            , targetPtr
        );
    }
}


//
//  This method scans a public literal. It must be quoted and all of its
//  characters must be valid public id characters. The quotes are discarded
//  and the results are returned.
//
bool DTDValidator::scanPublicLiteral(XMLBuffer& toFill)
{
    toFill.reset();

    // Get the next char which must be a single or double quote
    XMLCh quoteCh;
    if (!getReaderMgr()->skipIfQuote(quoteCh))
        return false;

    while (true)
    {
        const XMLCh nextCh = getReaderMgr()->getNextChar();

        // Watch for EOF
        if (!nextCh)
            ThrowXML(UnexpectedEOFException, XML4CExcepts::Gen_UnexpectedEOF);

        if (nextCh == quoteCh)
            break;

        //
        //  If its not a valid public id char, then report it but keep going
        //  since that's the best recovery scheme.
        //
        if (!XMLReader::isPublicIdChar(nextCh))
            getScanner()->emitError(XML4CErrs::InvalidPublicIdChar);

        toFill.append(nextCh);
    }
    return true;
}


//
//  This method handles scanning in a quoted system literal. It expects to
//  start on the open quote and returns after eating the ending quote. There
//  are not really any restrictions on the contents of system literals.
//
bool DTDValidator::scanSystemLiteral(XMLBuffer& toFill)
{
    toFill.reset();

    // Get the next char which must be a single or double quote
    XMLCh quoteCh;
    if (!getReaderMgr()->skipIfQuote(quoteCh))
        return false;

    bool retVal = true;
    while (retVal)
    {
        const XMLCh nextCh = getReaderMgr()->getNextChar();

        // Watch for EOF
        if (!nextCh)
            ThrowXML(UnexpectedEOFException, XML4CExcepts::Gen_UnexpectedEOF);

        // Break out on terminating quote
        if (nextCh == quoteCh)
            break;

        toFill.append(nextCh);
    }
    return retVal;
}



//
//  This method is called to scan a text decl line, which can be the first
//  line in an external entity or external subset.
//
//  On entry the <? has been scanned, and next should be 'xml' followed by
//  some whitespace, version string, etc...
//
void DTDValidator::scanTextDecl()
{
    // Skip any subsequent whitespace before the version string
    getReaderMgr()->skipPastSpaces();

    // Next should be the version string
    XMLBufBid bbVersion(getBufMgr());
    if (getReaderMgr()->skippedString(XMLUni::fgVersionString))
    {
        if (!scanEq())
        {
            getScanner()->emitError(XML4CErrs::ExpectedEqSign);
            getReaderMgr()->skipPastChar(chCloseAngle);
            return;
        }

        //
        //  Followed by a single or double quoted version. Get a buffer for
        //  the string.
        //
        if (!getQuotedString(bbVersion.getBuffer()))
        {
            getScanner()->emitError(XML4CErrs::BadXMLVersion);
            getReaderMgr()->skipPastChar(chCloseAngle);
            return;
        }

        // If its not our supported version, issue an error but continue
        if (XMLString::compareString(bbVersion.getRawBuffer(), XMLUni::fgSupportedVersion))
            getScanner()->emitError(XML4CErrs::UnsupportedXMLVersion, bbVersion.getRawBuffer());
    }

    // Ok, now we can have an encoding string, but we don't have to
    XMLBufBid bbEncoding(getBufMgr());
    getReaderMgr()->skipPastSpaces();
    bool gotEncoding = false;
    if (getReaderMgr()->skippedString(XMLUni::fgEncodingString))
    {
        // There must be a equal sign next
        if (!scanEq())
        {
            getScanner()->emitError(XML4CErrs::ExpectedEqSign);
            getReaderMgr()->skipPastChar(chCloseAngle);
            return;
        }

        // Followed by a single or double quoted version string
        getQuotedString(bbEncoding.getBuffer());
        if (bbEncoding.isEmpty())
        {
            getScanner()->emitError(XML4CErrs::BadXMLEncoding);
            getReaderMgr()->skipPastChar(chCloseAngle);
            return;
        }

        // Indicate that we got an encoding
        gotEncoding = true;
    }

    getReaderMgr()->skipPastSpaces();
    if (!getReaderMgr()->skippedChar(chQuestion))
    {
        getScanner()->emitError(XML4CErrs::UnterminatedXMLDecl);
        getReaderMgr()->skipPastChar(chCloseAngle);
    }
     else if (!getReaderMgr()->skippedChar(chCloseAngle))
    {
        getScanner()->emitError(XML4CErrs::UnterminatedXMLDecl);
        getReaderMgr()->skipPastChar(chCloseAngle);
    }

    //
    //  If we have a document type handler and advanced callbacks are on,
    //  then call the TextDecl callback
    //
    if (fDocTypeHandler)
    {
        fDocTypeHandler->TextDecl
        (
            bbVersion.getRawBuffer()
            , bbEncoding.getRawBuffer()
        );
    }

    //
    //  If we got an encoding string, then we have to call back on the reader
    //  to tell it what the encoding is.
    //
    if (!bbEncoding.isEmpty())
    {
        if (!getReaderMgr()->getCurrentReader()->setEncoding(bbEncoding.getRawBuffer()))
            getScanner()->emitError(XML4CErrs::ContradictoryEncoding, bbEncoding.getRawBuffer());
    }
}
