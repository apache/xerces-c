/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 2002, 2003 The Apache Software Foundation.  All rights
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
#include <xercesc/internal/SGXMLScanner.hpp>
#include <xercesc/util/RuntimeException.hpp>
#include <xercesc/util/UnexpectedEOFException.hpp>
#include <xercesc/framework/LocalFileInputSource.hpp>
#include <xercesc/framework/URLInputSource.hpp>
#include <xercesc/framework/XMLDocumentHandler.hpp>
#include <xercesc/framework/XMLEntityHandler.hpp>
#include <xercesc/framework/XMLPScanToken.hpp>
#include <xercesc/framework/MemoryManager.hpp>
#include <xercesc/framework/XMLGrammarPool.hpp>
#include <xercesc/framework/XMLSchemaDescription.hpp>
#include <xercesc/framework/psvi/PSVIHandler.hpp>
#include <xercesc/framework/psvi/PSVIAttributeList.hpp>
#include <xercesc/internal/EndOfEntityException.hpp>
#include <xercesc/validators/common/ContentLeafNameTypeVector.hpp>
#include <xercesc/validators/schema/SchemaValidator.hpp>
#include <xercesc/validators/schema/TraverseSchema.hpp>
#include <xercesc/validators/schema/XSDDOMParser.hpp>
#include <xercesc/validators/schema/SubstitutionGroupComparator.hpp>
#include <xercesc/validators/schema/identity/FieldActivator.hpp>
#include <xercesc/validators/schema/identity/XPathMatcherStack.hpp>
#include <xercesc/validators/schema/identity/ValueStoreCache.hpp>
#include <xercesc/validators/schema/identity/IC_Selector.hpp>
#include <xercesc/validators/schema/identity/ValueStore.hpp>
#include <xercesc/util/OutOfMemoryException.hpp>
#include <xercesc/util/XMLResourceIdentifier.hpp>
#include <xercesc/util/HashPtr.hpp>

XERCES_CPP_NAMESPACE_BEGIN

inline XMLAttDefList& getAttDefList(ComplexTypeInfo* currType, XMLElementDecl* elemDecl);

// ---------------------------------------------------------------------------
//  SGXMLScanner: Constructors and Destructor
// ---------------------------------------------------------------------------
SGXMLScanner::SGXMLScanner( XMLValidator* const valToAdopt
                          , GrammarResolver* const grammarResolver
                          , MemoryManager* const manager) :

    XMLScanner(valToAdopt, grammarResolver, manager)
    , fSeeXsi(false)
    , fGrammarType(Grammar::UnKnown)
    , fElemStateSize(16)
    , fElemState(0)
    , fContent(1023, manager)
    , fEntityTable(0)
    , fRawAttrList(0)
    , fSchemaGrammar(0)
    , fSchemaValidator(0)
    , fMatcherStack(0)
    , fValueStoreCache(0)
    , fFieldActivator(0)
    , fElemNonDeclPool(0)
    , fElemCount(0)
    , fAttDefRegistry(0)
    , fUndeclaredAttrRegistryNS(0)
    , fPSVIAttrList(0)
    , fModel(0)
    , fPSVIElement(0)
    , fErrorStack(0)       
{
    try
    {
         commonInit();

         if (valToAdopt)
         {
             if (!valToAdopt->handlesSchema())
                ThrowXMLwithMemMgr(RuntimeException, XMLExcepts::Gen_NoSchemaValidator, fMemoryManager);
         }
         else
         {
             fValidator = fSchemaValidator;
         }
    }
    catch(const OutOfMemoryException&)
    {
        throw;
    }
    catch(...)
    {
        cleanUp();
        throw;
    }
}

SGXMLScanner::SGXMLScanner( XMLDocumentHandler* const docHandler
                          , DocTypeHandler* const     docTypeHandler
                          , XMLEntityHandler* const   entityHandler
                          , XMLErrorReporter* const   errHandler
                          , XMLValidator* const       valToAdopt
                          , GrammarResolver* const    grammarResolver
                          , MemoryManager* const      manager) :

    XMLScanner(docHandler, docTypeHandler, entityHandler, errHandler, valToAdopt, grammarResolver, manager)
    , fSeeXsi(false)
    , fGrammarType(Grammar::UnKnown)
    , fElemStateSize(16)
    , fElemState(0)
    , fContent(1023, manager)
    , fEntityTable(0)
    , fRawAttrList(0)
    , fSchemaGrammar(0)
    , fSchemaValidator(0)
    , fMatcherStack(0)
    , fValueStoreCache(0)
    , fFieldActivator(0)
    , fElemNonDeclPool(0)
    , fElemCount(0)
    , fAttDefRegistry(0)
    , fUndeclaredAttrRegistryNS(0)
    , fPSVIAttrList(0)
    , fModel(0)
    , fPSVIElement(0)
    , fErrorStack(0)        
{
    try
    {	
        commonInit();

         if (valToAdopt)
         {
             if (!valToAdopt->handlesSchema())
                ThrowXMLwithMemMgr(RuntimeException, XMLExcepts::Gen_NoSchemaValidator, fMemoryManager);
         }
         else
         {
             fValidator = fSchemaValidator;
         }
    }
    catch(const OutOfMemoryException&)
    {
        throw;
    }
    catch(...)
    {
        cleanUp();
        throw;
    }
}

SGXMLScanner::~SGXMLScanner()
{
    cleanUp();
}

// ---------------------------------------------------------------------------
//  XMLScanner: Getter methods
// ---------------------------------------------------------------------------
NameIdPool<DTDEntityDecl>* SGXMLScanner::getEntityDeclPool()
{
    return 0;
}

const NameIdPool<DTDEntityDecl>* SGXMLScanner::getEntityDeclPool() const
{
    return 0;
}

// ---------------------------------------------------------------------------
//  SGXMLScanner: Main entry point to scan a document
// ---------------------------------------------------------------------------
void SGXMLScanner::scanDocument(const InputSource& src)
{
    //  Bump up the sequence id for this parser instance. This will invalidate
    //  any previous progressive scan tokens.
    fSequenceId++;

    try
    {
        //  Reset the scanner and its plugged in stuff for a new run. This
        //  resets all the data structures, creates the initial reader and
        //  pushes it on the stack, and sets up the base document path.
        scanReset(src);

        // If we have a document handler, then call the start document
        if (fDocHandler)
            fDocHandler->startDocument();

        //  Scan the prolog part, which is everything before the root element
        //  including the DTD subsets.
        scanProlog();

        //  If we got to the end of input, then its not a valid XML file.
        //  Else, go on to scan the content.
        if (fReaderMgr.atEOF())
        {
            emitError(XMLErrs::EmptyMainEntity);
        }
        else
        {
            // Scan content, and tell it its not an external entity
            if (scanContent())
            {
                // Do post-parse validation if required
                if (fValidate)
                {
                    //  We handle ID reference semantics at this level since
                    //  its required by XML 1.0.
                    checkIDRefs();

                    // Then allow the validator to do any extra stuff it wants
//                    fValidator->postParseValidation();
                }

                // That went ok, so scan for any miscellaneous stuff
                if (!fReaderMgr.atEOF())
                    scanMiscellaneous();
            }
        }

        // If we have a document handler, then call the end document
        if (fDocHandler)
            fDocHandler->endDocument();

        // Reset the reader manager to close all files, sockets, etc...
        fReaderMgr.reset();
    }
    //  NOTE:
    //
    //  In all of the error processing below, the emitError() call MUST come
    //  before the flush of the reader mgr, or it will fail because it tries
    //  to find out the position in the XML source of the error.
    catch(const XMLErrs::Codes)
    {
        // This is a 'first fatal error' type exit, so reset and fall through
        fReaderMgr.reset();
    }
    catch(const XMLValid::Codes)
    {
        // This is a 'first fatal error' type exit, so reset and fall through
        fReaderMgr.reset();
    }
    catch(const XMLException& excToCatch)
    {
        //  Emit the error and catch any user exception thrown from here. Make
        //  sure in all cases we flush the reader manager.
        fInException = true;
        try
        {
            if (excToCatch.getErrorType() == XMLErrorReporter::ErrType_Warning)
                emitError
                (
                    XMLErrs::XMLException_Warning
                    , excToCatch.getType()
                    , excToCatch.getMessage()
                );
            else if (excToCatch.getErrorType() >= XMLErrorReporter::ErrType_Fatal)
                emitError
                (
                    XMLErrs::XMLException_Fatal
                    , excToCatch.getType()
                    , excToCatch.getMessage()
                );
            else
                emitError
                (
                    XMLErrs::XMLException_Error
                    , excToCatch.getType()
                    , excToCatch.getMessage()
                );
        }
        catch(const OutOfMemoryException&)
        {
            throw;
        }
        catch(...)
        {
            // Flush the reader manager and rethrow user's error
            fReaderMgr.reset();
            throw;
        }

        // If it returned, then reset the reader manager and fall through
        fReaderMgr.reset();
    }
    catch(const OutOfMemoryException&)
    {
        throw;
    }
    catch(...)
    {
        // Reset and rethrow
        fReaderMgr.reset();
        throw;
    }
}


bool SGXMLScanner::scanNext(XMLPScanToken& token)
{
    // Make sure this token is still legal
    if (!isLegalToken(token))
        ThrowXMLwithMemMgr(RuntimeException, XMLExcepts::Scan_BadPScanToken, fMemoryManager);

    // Find the next token and remember the reader id
    unsigned int orgReader;
    XMLTokens curToken;

    bool retVal = true;

    try
    {
        while (true)
        {
            //  We have to handle any end of entity exceptions that happen here.
            //  We could be at the end of X nested entities, each of which will
            //  generate an end of entity exception as we try to move forward.
            try
            {
                curToken = senseNextToken(orgReader);
                break;
            }
            catch(const EndOfEntityException& toCatch)
            {
                // Send an end of entity reference event
                if (fDocHandler)
                    fDocHandler->endEntityReference(toCatch.getEntity());
            }
        }

        if (curToken == Token_CharData)
        {
            scanCharData(fCDataBuf);
        }
        else if (curToken == Token_EOF)
        {
            if (!fElemStack.isEmpty())
            {
                const ElemStack::StackElem* topElem = fElemStack.popTop();
                emitError
                (
                    XMLErrs::EndedWithTagsOnStack
                    , topElem->fThisElement->getFullName()
                );
            }

            retVal = false;
        }
        else
        {
            // Its some sort of markup
            bool gotData = true;
            switch(curToken)
            {
                case Token_CData :
                    // Make sure we are within content
                    if (fElemStack.isEmpty())
                        emitError(XMLErrs::CDATAOutsideOfContent);
                    scanCDSection();
                    break;

                case Token_Comment :
                    scanComment();
                    break;

                case Token_EndTag :
                    scanEndTag(gotData);
                    break;

                case Token_PI :
                    scanPI();
                    break;

                case Token_StartTag :
                    scanStartTag(gotData);
                    break;

                default :
                    fReaderMgr.skipToChar(chOpenAngle);
                    break;
            }

            if (orgReader != fReaderMgr.getCurrentReaderNum())
                emitError(XMLErrs::PartialMarkupInEntity);

            // If we hit the end, then do the miscellaneous part
            if (!gotData)
            {
                // Do post-parse validation if required
                if (fValidate)
                {
                    //  We handle ID reference semantics at this level since
                    //  its required by XML 1.0.
                    checkIDRefs();

                    // Then allow the validator to do any extra stuff it wants
//                    fValidator->postParseValidation();
                }

                // That went ok, so scan for any miscellaneous stuff
                scanMiscellaneous();

                if (fValidate)
                    fValueStoreCache->endDocument();

                if (fDocHandler)
                    fDocHandler->endDocument();
            }
        }
    }
    //  NOTE:
    //
    //  In all of the error processing below, the emitError() call MUST come
    //  before the flush of the reader mgr, or it will fail because it tries
    //  to find out the position in the XML source of the error.
    catch(const XMLErrs::Codes)
    {
        // This is a 'first failure' exception, so reset and return failure
        fReaderMgr.reset();
        return false;
    }
    catch(const XMLValid::Codes)
    {
        // This is a 'first fatal error' type exit, so reset and reuturn failure
        fReaderMgr.reset();
        return false;
    }
    catch(const XMLException& excToCatch)
    {
        //  Emit the error and catch any user exception thrown from here. Make
        //  sure in all cases we flush the reader manager.
        fInException = true;
        try
        {
            if (excToCatch.getErrorType() == XMLErrorReporter::ErrType_Warning)
                emitError
                (
                    XMLErrs::XMLException_Warning
                    , excToCatch.getType()
                    , excToCatch.getMessage()
                );
            else if (excToCatch.getErrorType() >= XMLErrorReporter::ErrType_Fatal)
                emitError
                (
                    XMLErrs::XMLException_Fatal
                    , excToCatch.getType()
                    , excToCatch.getMessage()
                );
            else
                emitError
                (
                    XMLErrs::XMLException_Error
                    , excToCatch.getType()
                    , excToCatch.getMessage()
                );
        }
        catch(const OutOfMemoryException&)
        {
            throw;
        }
        catch(...)
        {
            // Reset and rethrow user error
            fReaderMgr.reset();
            throw;
        }

        // Reset and return failure
        fReaderMgr.reset();
        return false;
    }
    catch(const OutOfMemoryException&)
    {
        throw;
    }
    catch(...)
    {
        // Reset and rethrow original error
        fReaderMgr.reset();
        throw;
    }

    // If we hit the end, then flush the reader manager
    if (!retVal)
        fReaderMgr.reset();

    return retVal;
}

// ---------------------------------------------------------------------------
//  SGXMLScanner: Private scanning methods
// ---------------------------------------------------------------------------

//  This method is called from scanStartTag() to handle the very raw initial
//  scan of the attributes. It just fills in the passed collection with
//  key/value pairs for each attribute. No processing is done on them at all.
unsigned int
SGXMLScanner::rawAttrScan(const   XMLCh* const                elemName
                          ,       RefVectorOf<KVStringPair>&  toFill
                          ,       bool&                       isEmpty)
{
    //  Keep up with how many attributes we've seen so far, and how many
    //  elements are available in the vector. This way we can reuse old
    //  elements until we run out and then expand it.
    unsigned int attCount = 0;
    unsigned int curVecSize = toFill.size();

    // Assume it is not empty
    isEmpty = false;

    //  We loop until we either see a /> or >, handling key/value pairs util
    //  we get there. We place them in the passed vector, which we will expand
    //  as required to hold them.
    while (true)
    {
        // Get the next character, which should be non-space
        XMLCh nextCh = fReaderMgr.peekNextChar();

        //  If the next character is not a slash or closed angle bracket,
        //  then it must be whitespace, since whitespace is required
        //  between the end of the last attribute and the name of the next
        //  one.
        //
        if (attCount)
        {
            if ((nextCh != chForwardSlash) && (nextCh != chCloseAngle))
            {
                if (fReaderMgr.getCurrentReader()->isWhitespace(nextCh))
                {
                    // Ok, skip by them and get another char
                    fReaderMgr.getNextChar();
                    fReaderMgr.skipPastSpaces();
                    nextCh = fReaderMgr.peekNextChar();
                }
                 else
                {
                    // Emit the error but keep on going
                    emitError(XMLErrs::ExpectedWhitespace);
                }
            }
        }

        //  Ok, here we first check for any of the special case characters.
        //  If its not one, then we do the normal case processing, which
        //  assumes that we've hit an attribute value, Otherwise, we do all
        //  the special case checks.
        if (!fReaderMgr.getCurrentReader()->isSpecialStartTagChar(nextCh))
        {
            //  Assume its going to be an attribute, so get a name from
            //  the input.
            if (!fReaderMgr.getName(fAttNameBuf))
            {
                emitError(XMLErrs::ExpectedAttrName);
                fReaderMgr.skipPastChar(chCloseAngle);
                return attCount;
            }

            // And next must be an equal sign
            if (!scanEq())
            {
                static const XMLCh tmpList[] =
                {
                    chSingleQuote, chDoubleQuote, chCloseAngle
                    , chOpenAngle, chForwardSlash, chNull
                };

                emitError(XMLErrs::ExpectedEqSign);

                //  Try to sync back up by skipping forward until we either
                //  hit something meaningful.
                const XMLCh chFound = fReaderMgr.skipUntilInOrWS(tmpList);

                if ((chFound == chCloseAngle) || (chFound == chForwardSlash))
                {
                    // Jump back to top for normal processing of these
                    continue;
                }
                else if ((chFound == chSingleQuote)
                      ||  (chFound == chDoubleQuote)
                      ||  fReaderMgr.getCurrentReader()->isWhitespace(chFound))
                {
                    // Just fall through assuming that the value is to follow
                }
                else if (chFound == chOpenAngle)
                {
                    // Assume a malformed tag and that new one is starting
                    emitError(XMLErrs::UnterminatedStartTag, elemName);
                    return attCount;
                }
                else
                {
                    // Something went really wrong
                    return attCount;
                }
            }

            //  Next should be the quoted attribute value. We just do a simple
            //  and stupid scan of this value. The only thing we do here
            //  is to expand entity references.
            if (!basicAttrValueScan(fAttNameBuf.getRawBuffer(), fAttValueBuf))
            {
                static const XMLCh tmpList[] =
                {
                    chCloseAngle, chOpenAngle, chForwardSlash, chNull
                };

                emitError(XMLErrs::ExpectedAttrValue);

                //  It failed, so lets try to get synced back up. We skip
                //  forward until we find some whitespace or one of the
                //  chars in our list.
                const XMLCh chFound = fReaderMgr.skipUntilInOrWS(tmpList);

                if ((chFound == chCloseAngle)
                ||  (chFound == chForwardSlash)
                ||  fReaderMgr.getCurrentReader()->isWhitespace(chFound))
                {
                    //  Just fall through and process this attribute, though
                    //  the value will be "".
                }
                else if (chFound == chOpenAngle)
                {
                    // Assume a malformed tag and that new one is starting
                    emitError(XMLErrs::UnterminatedStartTag, elemName);
                    return attCount;
                }
                else
                {
                    // Something went really wrong
                    return attCount;
                }
            }

            //  Make sure that the name is basically well formed for namespace
            //  enabled rules. It either has no colons, or it has one which
            //  is neither the first or last char.
            const int colonFirst = XMLString::indexOf(fAttNameBuf.getRawBuffer(), chColon);
            if (colonFirst != -1)
            {
                const int colonLast = XMLString::lastIndexOf(fAttNameBuf.getRawBuffer(), chColon);

                if (colonFirst != colonLast)
                {
                    emitError(XMLErrs::TooManyColonsInName);
                    continue;
                }
                else if ((colonFirst == 0)
                      ||  (colonLast == (int)fAttNameBuf.getLen() - 1))
                {
                    emitError(XMLErrs::InvalidColonPos);
                    continue;
                }
            }

            //  And now lets add it to the passed collection. If we have not
            //  filled it up yet, then we use the next element. Else we add
            //  a new one.
            KVStringPair* curPair = 0;
            if (attCount >= curVecSize)
            {
                curPair = new (fMemoryManager) KVStringPair
                (
                    fAttNameBuf.getRawBuffer()
                    , fAttValueBuf.getRawBuffer()
                    , fMemoryManager
                );
                toFill.addElement(curPair);
            }
             else
            {
                curPair = toFill.elementAt(attCount);
                curPair->set(fAttNameBuf.getRawBuffer(), fAttValueBuf.getRawBuffer());
            }

            // And bump the count of attributes we've gotten
            attCount++;

            // And go to the top again for another attribute
            continue;
        }

        //  It was some special case character so do all of the checks and
        //  deal with it.
        if (!nextCh)
            ThrowXMLwithMemMgr(UnexpectedEOFException, XMLExcepts::Gen_UnexpectedEOF, fMemoryManager);

        if (nextCh == chForwardSlash)
        {
            fReaderMgr.getNextChar();
            isEmpty = true;
            if (!fReaderMgr.skippedChar(chCloseAngle))
                emitError(XMLErrs::UnterminatedStartTag, elemName);
            break;
        }
        else if (nextCh == chCloseAngle)
        {
            fReaderMgr.getNextChar();
            break;
        }
        else if (nextCh == chOpenAngle)
        {
            //  Check for this one specially, since its going to be common
            //  and it is kind of auto-recovering since we've already hit the
            //  next open bracket, which is what we would have seeked to (and
            //  skipped this whole tag.)
            emitError(XMLErrs::UnterminatedStartTag, elemName);
            break;
        }
        else if ((nextCh == chSingleQuote) || (nextCh == chDoubleQuote))
        {
            //  Check for this one specially, which is probably a missing
            //  attribute name, e.g. ="value". Just issue expected name
            //  error and eat the quoted string, then jump back to the
            //  top again.
            emitError(XMLErrs::ExpectedAttrName);
            fReaderMgr.getNextChar();
            fReaderMgr.skipQuotedString(nextCh);
            fReaderMgr.skipPastSpaces();
            continue;
        }
    }

    return attCount;
}


//  This method will kick off the scanning of the primary content of the
//  document, i.e. the elements.
bool SGXMLScanner::scanContent()
{
    //  Go into a loop until we hit the end of the root element, or we fall
    //  out because there is no root element.
    //
    //  We have to do kind of a deeply nested double loop here in order to
    //  avoid doing the setup/teardown of the exception handler on each
    //  round. Doing it this way we only do it when an exception actually
    //  occurs.
    bool gotData = true;
    bool inMarkup = false;
    while (gotData)
    {
        try
        {
            while (gotData)
            {
                //  Sense what the next top level token is. According to what
                //  this tells us, we will call something to handle that kind
                //  of thing.
                unsigned int orgReader;
                const XMLTokens curToken = senseNextToken(orgReader);

                //  Handle character data and end of file specially. Char data
                //  is not markup so we don't want to handle it in the loop
                //  below.
                if (curToken == Token_CharData)
                {
                    //  Scan the character data and call appropriate events. Let
                    //  him use our local character data buffer for efficiency.
                    scanCharData(fCDataBuf);
                    continue;
                }
                else if (curToken == Token_EOF)
                {
                    //  The element stack better be empty at this point or we
                    //  ended prematurely before all elements were closed.
                    if (!fElemStack.isEmpty())
                    {
                        const ElemStack::StackElem* topElem = fElemStack.popTop();
                        emitError
                        (
                            XMLErrs::EndedWithTagsOnStack
                            , topElem->fThisElement->getFullName()
                        );
                    }

                    // Its the end of file, so clear the got data flag
                    gotData = false;
                    continue;
                }

                // We are in some sort of markup now
                inMarkup = true;

                //  According to the token we got, call the appropriate
                //  scanning method.
                switch(curToken)
                {
                    case Token_CData :
                        // Make sure we are within content
                        if (fElemStack.isEmpty())
                            emitError(XMLErrs::CDATAOutsideOfContent);
                        scanCDSection();
                        break;

                    case Token_Comment :
                        scanComment();
                        break;

                    case Token_EndTag :
                        scanEndTag(gotData);
                        break;

                    case Token_PI :
                        scanPI();
                        break;

                    case Token_StartTag :
                        scanStartTag(gotData);
                        break;

                    default :
                        fReaderMgr.skipToChar(chOpenAngle);
                        break;
                }

                if (orgReader != fReaderMgr.getCurrentReaderNum())
                    emitError(XMLErrs::PartialMarkupInEntity);

                // And we are back out of markup again
                inMarkup = false;
            }
        }
        catch(const EndOfEntityException& toCatch)
        {
            //  If we were in some markup when this happened, then its a
            //  partial markup error.
            if (inMarkup)
                emitError(XMLErrs::PartialMarkupInEntity);

            // Send an end of entity reference event
            if (fDocHandler)
                fDocHandler->endEntityReference(toCatch.getEntity());

            inMarkup = false;
        }
    }

    // It went ok, so return success
    return true;
}


void SGXMLScanner::scanEndTag(bool& gotData)
{
    //  Assume we will still have data until proven otherwise. It will only
    //  ever be false if this is the end of the root element.
    gotData = true;

    //  Check if the element stack is empty. If so, then this is an unbalanced
    //  element (i.e. more ends than starts, perhaps because of bad text
    //  causing one to be skipped.)
    if (fElemStack.isEmpty())
    {
        emitError(XMLErrs::MoreEndThanStartTags);
        fReaderMgr.skipPastChar(chCloseAngle);
        ThrowXMLwithMemMgr(RuntimeException, XMLExcepts::Scan_UnbalancedStartEnd, fMemoryManager);
    }

    // After the </ is the element QName, so get a name from the input
    if (!fReaderMgr.getName(fQNameBuf))
    {
        // It failed so we can't really do anything with it
        emitError(XMLErrs::ExpectedElementName);
        fReaderMgr.skipPastChar(chCloseAngle);
        return;
    }

    int prefixColonPos = -1;
    unsigned int uriId = resolveQName
    (
        fQNameBuf.getRawBuffer()
        , fPrefixBuf
        , ElemStack::Mode_Element
        , prefixColonPos
    );

    //  Pop the stack of the element we are supposed to be ending. Remember
    //  that we don't own this. The stack just keeps them and reuses them.
    //
    //  NOTE: We CANNOT do this until we've resolved the element name because
    //  the element stack top contains the prefix to URI mappings for this
    //  element.
    unsigned int topUri = fElemStack.getCurrentURI();
    const ElemStack::StackElem* topElem = fElemStack.popTop();

    // See if it was the root element, to avoid multiple calls below
    const bool isRoot = fElemStack.isEmpty();

    // Make sure that its the end of the element that we expect
    XMLElementDecl* tempElement = topElem->fThisElement;
    const XMLCh* rawNameBuf = fQNameBuf.getRawBuffer();

    // reset error occurred
    fPSVIElemContext.fErrorOccurred = fErrorStack->pop();

    if ((topUri != uriId) ||
        (!XMLString::equals(tempElement->getBaseName(), &rawNameBuf[prefixColonPos + 1])))
    {
        emitError
        (
            XMLErrs::ExpectedEndOfTagX
            , topElem->fThisElement->getFullName()
        );
    }

    // Make sure we are back on the same reader as where we started
    if (topElem->fReaderNum != fReaderMgr.getCurrentReaderNum())
        emitError(XMLErrs::PartialTagMarkupError);

    // Skip optional whitespace
    fReaderMgr.skipPastSpaces();

    // Make sure we find the closing bracket
    if (!fReaderMgr.skippedChar(chCloseAngle))
    {
        emitError
        (
            XMLErrs::UnterminatedEndTag
            , topElem->fThisElement->getFullName()
        );
    }

    if (fValidate && topElem->fThisElement->isDeclared())
    {
        fPSVIElemContext.fCurrentTypeInfo = ((SchemaValidator*) fValidator)->getCurrentTypeInfo();
        if(!fPSVIElemContext.fCurrentTypeInfo)
            fPSVIElemContext.fCurrentDV = ((SchemaValidator*) fValidator)->getCurrentDatatypeValidator();
        else
            fPSVIElemContext.fCurrentDV = 0;
        if (fPSVIHandler)
        {
            fPSVIElemContext.fNormalizedValue = ((SchemaValidator*) fValidator)->getNormalizedValue();

            if (XMLString::equals(fPSVIElemContext.fNormalizedValue, XMLUni::fgZeroLenString))
                fPSVIElemContext.fNormalizedValue = 0;

        }
    }
    else
    {
        fPSVIElemContext.fCurrentDV = 0;
        fPSVIElemContext.fCurrentTypeInfo = 0;
        fPSVIElemContext.fNormalizedValue = 0;
    }

    //  If validation is enabled, then lets pass him the list of children and
    //  this element and let him validate it.
    DatatypeValidator* psviMemberType = 0;
    if (fValidate)
    {
        int res = fValidator->checkContent
        (
            topElem->fThisElement
            , topElem->fChildren
            , topElem->fChildCount
        );

        if (res >= 0)
        {
            //  One of the elements is not valid for the content. NOTE that
            //  if no children were provided but the content model requires
            //  them, it comes back with a zero value. But we cannot use that
            //  to index the child array in this case, and have to put out a
            //  special message.
            if (!topElem->fChildCount)
            {
                fValidator->emitError
                (
                    XMLValid::EmptyNotValidForContent
                    , topElem->fThisElement->getFormattedContentModel()
                );
            }
            else if ((unsigned int)res >= topElem->fChildCount)
            {
                fValidator->emitError
                (
                    XMLValid::NotEnoughElemsForCM
                    , topElem->fThisElement->getFormattedContentModel()
                );
            }
            else
            {
                fValidator->emitError
                (
                    XMLValid::ElementNotValidForContent
                    , topElem->fChildren[res]->getRawName()
                    , topElem->fThisElement->getFormattedContentModel()
                );
            }
            
        }

        // update PSVI info
        if (((SchemaValidator*) fValidator)->getErrorOccurred())
            fPSVIElemContext.fErrorOccurred = true;
        else if (fPSVIElemContext.fCurrentDV && fPSVIElemContext.fCurrentDV->getType() == DatatypeValidator::Union)
            psviMemberType = fValidationContext->getValidatingMemberType();
        if (fPSVIHandler)
        {
            fPSVIElemContext.fIsSpecified = ((SchemaValidator*) fValidator)->getIsElemSpecified();
            if(fPSVIElemContext.fIsSpecified)
                fPSVIElemContext.fNormalizedValue = ((SchemaElementDecl *)topElem->fThisElement)->getDefaultValue();
        }
        // call matchers and de-activate context
        int oldCount = fMatcherStack->getMatcherCount();

        if (oldCount ||
            ((SchemaElementDecl*)topElem->fThisElement)->getIdentityConstraintCount()) {

            for (int i = oldCount - 1; i >= 0; i--) {

                XPathMatcher* matcher = fMatcherStack->getMatcherAt(i);
                matcher->endElement(*(topElem->fThisElement), fContent.getRawBuffer());
            }

            if (fMatcherStack->size() > 0) {
                fMatcherStack->popContext();
            }

            // handle everything *but* keyref's.
            int newCount = fMatcherStack->getMatcherCount();

            for (int j = oldCount - 1; j >= newCount; j--) {

                XPathMatcher* matcher = fMatcherStack->getMatcherAt(j);
                IdentityConstraint* ic = matcher->getIdentityConstraint();

                if (ic  && (ic->getType() != IdentityConstraint::KEYREF))
                    fValueStoreCache->transplant(ic, matcher->getInitialDepth());
            }

            // now handle keyref's...
            for (int k = oldCount - 1; k >= newCount; k--) {

                XPathMatcher* matcher = fMatcherStack->getMatcherAt(k);
                IdentityConstraint* ic = matcher->getIdentityConstraint();

                if (ic && (ic->getType() == IdentityConstraint::KEYREF)) {

                    ValueStore* values = fValueStoreCache->getValueStoreFor(ic, matcher->getInitialDepth());

                    if (values) { // nothing to do if nothing matched!
                        values->endDcocumentFragment(fValueStoreCache);
                    }
                }
            }

            fValueStoreCache->endElement();
        }
    }
    if(!isRoot)
        ((SchemaElementDecl *)fElemStack.topElement()->fThisElement)->updateValidityFromElement(topElem->fThisElement, fGrammarType);

    if (fPSVIHandler)
    {
        endElementPSVI
        (
            (SchemaElementDecl*)topElem->fThisElement, psviMemberType
        );
    }
    // now we can reset the datatype buffer, since the 
    // application has had a chance to copy the characters somewhere else
    ((SchemaValidator *)fValidator)->clearDatatypeBuffer();

    // If we have a doc handler, tell it about the end tag
    if (fDocHandler)
    {
        fDocHandler->endElement
        (
            *topElem->fThisElement
            , uriId
            , isRoot
            , fPrefixBuf.getRawBuffer()
        );
        // pass information about type info:
        const XMLCh *typeName = SchemaSymbols::fgATTVAL_ANYTYPE;
        const XMLCh *typeURI = SchemaSymbols::fgURI_SCHEMAFORSCHEMA; 
        if(!fPSVIElemContext.fErrorOccurred)
        {
            if(fPSVIElemContext.fCurrentTypeInfo != 0)
            {
                if(fPSVIElemContext.fCurrentTypeInfo->getAnonymous())
                    typeName = XMLUni::fgZeroLenString;
                else 
                    typeName = fPSVIElemContext.fCurrentTypeInfo->getTypeLocalName();
                typeURI = fPSVIElemContext.fCurrentTypeInfo->getTypeUri();
            } 
            else
            {
                DatatypeValidator *actualDV = (psviMemberType)?psviMemberType:fPSVIElemContext.fCurrentDV;
                if(actualDV)
                {
                    if(actualDV->getAnonymous())
                        typeName = XMLUni::fgZeroLenString;
                    else
                        typeName = actualDV->getTypeLocalName();
                    typeURI = actualDV->getTypeUri();
                }
            }
        }
        else 
        {
            // idiosyncratically, if there's an
            // error but this element was simpleType-validated,
            // the tests demand anySimpleType be returned
            if(fPSVIElemContext.fCurrentDV)
                typeName = SchemaSymbols::fgDT_ANYSIMPLETYPE;
        }
        fDocHandler->elementTypeInfo(typeName, typeURI);
    }

    // reset xsi:type ComplexTypeInfo    
    ((SchemaElementDecl*)topElem->fThisElement)->reset();
    if (!isRoot)
    {
        ((SchemaElementDecl*)(fElemStack.topElement()->fThisElement))->
            setXsiComplexTypeInfo(((SchemaValidator*)fValidator)->getCurrentTypeInfo());
        // update error information
        fErrorStack->push(fErrorStack->pop() || fPSVIElemContext.fErrorOccurred);
    }

    // If this was the root, then done with content
    gotData = !isRoot;

    if (gotData) {

        // Restore the grammar
        fGrammar = fElemStack.getCurrentGrammar();
        fGrammarType = fGrammar->getGrammarType();
        fValidator->setGrammar(fGrammar);

        // Restore the validation flag
        fValidate = fElemStack.getValidationFlag();
    }
}


//  This method handles the high level logic of scanning the DOCType
//  declaration. This calls the DTDScanner and kicks off both the scanning of
//  the internal subset and the scanning of the external subset, if any.
//
//  When we get here the '<!DOCTYPE' part has already been scanned, which is
//  what told us that we had a doc type decl to parse.
void SGXMLScanner::scanDocTypeDecl()
{
    // Just skips over it
    // REVISIT: Should we issue a warning
    static const XMLCh doctypeIE[] =
    {
            chOpenSquare, chCloseAngle, chNull
    };
    XMLCh nextCh = fReaderMgr.skipUntilIn(doctypeIE);

    if (nextCh == chOpenSquare)
        fReaderMgr.skipPastChar(chCloseSquare);

    fReaderMgr.skipPastChar(chCloseAngle);
}

//  This method is called to scan a start tag when we are processing
//  namespaces. This method is called after we've scanned the < of a
//  start tag. So we have to get the element name, then scan the attributes, 
//  after which we are either going to see >, />, or attributes followed 
//  by one of those sequences.
bool SGXMLScanner::scanStartTag(bool& gotData)
{
    //  Assume we will still have data until proven otherwise. It will only
    //  ever be false if this is the root and its empty.
    gotData = true;

    // Reset element content
    fContent.reset();

    //  The current position is after the open bracket, so we need to read in
    //  in the element name.
    if (!fReaderMgr.getName(fQNameBuf))
    {
        emitError(XMLErrs::ExpectedElementName);
        fReaderMgr.skipToChar(chOpenAngle);
        return false;
    }

    // See if its the root element
    const bool isRoot = fElemStack.isEmpty();

    // Skip any whitespace after the name
    fReaderMgr.skipPastSpaces();

    //  First we have to do the rawest attribute scan. We don't do any
    //  normalization of them at all, since we don't know yet what type they
    //  might be (since we need the element decl in order to do that.)
    bool isEmpty;
    unsigned int attCount = rawAttrScan
    (
        fQNameBuf.getRawBuffer()
        , *fRawAttrList
        , isEmpty
    );

    // save the contentleafname and currentscope before addlevel, for later use
    ContentLeafNameTypeVector* cv = 0;
    XMLContentModel* cm = 0;
    int currentScope = Grammar::TOP_LEVEL_SCOPE;
    bool laxThisOne = false;
    if (!isRoot)
    {
        // schema validator will have correct type if validating
        SchemaElementDecl* tempElement = (SchemaElementDecl*)
            fElemStack.topElement()->fThisElement;
        SchemaElementDecl::ModelTypes modelType = tempElement->getModelType();
        ComplexTypeInfo *currType = 0;

        if (fValidate)
        {
            currType = ((SchemaValidator*)fValidator)->getCurrentTypeInfo();
            if (currType)
                modelType = (SchemaElementDecl::ModelTypes)currType->getContentType();
            else // something must have gone wrong
                modelType = SchemaElementDecl::Any;
        }
        else
        {
            currType = tempElement->getComplexTypeInfo();
        }

        if ((modelType == SchemaElementDecl::Mixed_Simple)
          ||  (modelType == SchemaElementDecl::Mixed_Complex)
          ||  (modelType == SchemaElementDecl::Children))
        {
            cm = currType->getContentModel();
            cv = cm->getContentLeafNameTypeVector();
            currentScope = fElemStack.getCurrentScope();
        }
        else if (modelType == SchemaElementDecl::Any) {
            laxThisOne = true;
        }
    }

    //  Now, since we might have to update the namespace map for this element,
    //  but we don't have the element decl yet, we just tell the element stack
    //  to expand up to get ready.
    unsigned int elemDepth = fElemStack.addLevel();
    fElemStack.setValidationFlag(fValidate);

    //  Check if there is any external schema location specified, and if we are at root,
    //  go through them first before scanning those specified in the instance document
    if (isRoot
        && (fExternalSchemaLocation || fExternalNoNamespaceSchemaLocation)) {

        if (fExternalSchemaLocation)
            parseSchemaLocation(fExternalSchemaLocation);
        if (fExternalNoNamespaceSchemaLocation)
            resolveSchemaGrammar(fExternalNoNamespaceSchemaLocation, XMLUni::fgZeroLenString);
    }

    //  Make an initial pass through the list and find any xmlns attributes or
    //  schema attributes.
    if (attCount)
        scanRawAttrListforNameSpaces(attCount);

    //  Resolve the qualified name to a URI and name so that we can look up
    //  the element decl for this element. We have now update the prefix to
    //  namespace map so we should get the correct element now.
    int prefixColonPos = -1;
    const XMLCh* qnameRawBuf = fQNameBuf.getRawBuffer();
    unsigned int uriId = resolveQName
    (
        qnameRawBuf
        , fPrefixBuf
        , ElemStack::Mode_Element
        , prefixColonPos
    );

    //if schema, check if we should lax or skip the validation of this element
    bool parentValidation = fValidate;
    if (cv) {
        QName element(fPrefixBuf.getRawBuffer(), &qnameRawBuf[prefixColonPos + 1], uriId, fMemoryManager);
        // elementDepth will be > 0, as cv is only constructed if element is not
        // root.
        laxThisOne = laxElementValidation(&element, cv, cm, elemDepth - 1);
    }

    //  Look up the element now in the grammar. This will get us back a
    //  generic element decl object. We tell him to fault one in if he does
    //  not find it.
    XMLElementDecl* elemDecl = 0;
    bool wasAdded = false;
    bool errorBeforeElementFound = false;
    bool laxBeforeElementFound = false;
    const XMLCh* nameRawBuf = &qnameRawBuf[prefixColonPos + 1];
    const XMLCh* original_uriStr = fGrammar->getTargetNamespace();
    unsigned orgGrammarUri = fURIStringPool->getId(original_uriStr);

    if (uriId != fEmptyNamespaceId) {

        // Check in current grammar before switching if necessary
        elemDecl = fGrammar->getElemDecl
        (
          uriId
          , nameRawBuf
          , qnameRawBuf
          , currentScope
        );
        if(!elemDecl) 
        {
            // look in the list of undeclared elements, as would have been done
            // before we made grammars stateless:
            elemDecl = fElemNonDeclPool->getByKey(nameRawBuf, uriId, currentScope);
        }
        if (!elemDecl && (orgGrammarUri != uriId)) {
            // not found, switch to the specified grammar
            const XMLCh* uriStr = getURIText(uriId);
            bool errorCondition = !switchGrammar(uriStr) && fValidate;
            if (errorCondition && !laxThisOne)
            {
                fValidator->emitError
                (
                    XMLValid::GrammarNotFound
                    ,uriStr
                );
                errorBeforeElementFound = true;
            } 
            else if(errorCondition)
                laxBeforeElementFound = true;

            elemDecl = fGrammar->getElemDecl
            (
              uriId
              , nameRawBuf
              , qnameRawBuf
              , currentScope
            );
        }

        if (!elemDecl && currentScope != Grammar::TOP_LEVEL_SCOPE) {
            // if not found, then it may be a reference, try TOP_LEVEL_SCOPE
            elemDecl = fGrammar->getElemDecl
                       (
                           uriId
                           , nameRawBuf
                           , qnameRawBuf
                           , Grammar::TOP_LEVEL_SCOPE
                       );
            if(!elemDecl) 
            {
                // look in the list of undeclared elements, as would have been done
                // before we made grammars stateless:
                elemDecl = fElemNonDeclPool->getByKey(nameRawBuf, uriId, Grammar::TOP_LEVEL_SCOPE);
            }
            if(!elemDecl) {
                // still not found in specified uri
                // try emptyNamesapce see if element should be un-qualified.
                elemDecl = fGrammar->getElemDecl
                           (
                               fEmptyNamespaceId
                               , nameRawBuf
                               , qnameRawBuf
                               , currentScope
                           ); 
                bool errorCondition = elemDecl && elemDecl->getCreateReason() != XMLElementDecl::JustFaultIn;
                if (errorCondition && fValidate) {
                    fValidator->emitError
                    (
                        XMLValid::ElementNotUnQualified
                        , elemDecl->getFullName()
                    );
                    errorBeforeElementFound = true;
                }
                else if(errorCondition) 
                    laxBeforeElementFound = true;

            }
        }

        if (!elemDecl) {
            // still not found, fault this in and issue error later
            // switch back to original grammar first
            switchGrammar(original_uriStr);
            elemDecl = new (fMemoryManager) SchemaElementDecl
            (
                fPrefixBuf.getRawBuffer()
                , nameRawBuf
                , uriId
                , SchemaElementDecl::Any
                , Grammar::TOP_LEVEL_SCOPE
                , fMemoryManager
            );
            elemDecl->setId(fElemNonDeclPool->put((void*)elemDecl->getBaseName(), uriId, currentScope, (SchemaElementDecl*)elemDecl));
            wasAdded = true;
        }
    }
    else if (!elemDecl)
    {
        //the element has no prefix,
        //thus it is either a non-qualified element defined in current targetNS
        //or an element that is defined in the globalNS

        //try unqualifed first
        elemDecl = fGrammar->getElemDecl
                   (
                      uriId
                    , nameRawBuf
                    , qnameRawBuf
                    , currentScope
                    );
        if(!elemDecl)
        {
            // look in the list of undeclared elements, as would have been done
            // before we made grammars stateless:
            elemDecl = fElemNonDeclPool->getByKey(nameRawBuf, uriId, currentScope);
        }
        if (!elemDecl && orgGrammarUri != fEmptyNamespaceId) {
            //not found, switch grammar and try globalNS
            bool errorCondition = !switchGrammar(XMLUni::fgZeroLenString) && fValidate;
            if (errorCondition && !laxThisOne)
            {
                fValidator->emitError
                (
                    XMLValid::GrammarNotFound
                  , XMLUni::fgZeroLenString
                );
                errorBeforeElementFound = true;
                
            }
            else if(errorCondition)
                laxBeforeElementFound = true; 

            elemDecl = fGrammar->getElemDecl
            (
              uriId
              , nameRawBuf
              , qnameRawBuf
              , currentScope
            );
        }

        if (!elemDecl && currentScope != Grammar::TOP_LEVEL_SCOPE) {
            // if not found, then it may be a reference, try TOP_LEVEL_SCOPE
            elemDecl = fGrammar->getElemDecl
                       (
                           uriId
                           , nameRawBuf
                           , qnameRawBuf
                           , Grammar::TOP_LEVEL_SCOPE
                       );
            if(!elemDecl)
            {
                // look in the list of undeclared elements, as would have been done
                // before we made grammars stateless:
                elemDecl = fElemNonDeclPool->getByKey(nameRawBuf, uriId, Grammar::TOP_LEVEL_SCOPE);
            }
            if (!elemDecl && orgGrammarUri != fEmptyNamespaceId) {
                // still Not found in specified uri
                // go to original Grammar again to see if element needs to be fully qualified.
                bool errorCondition = !switchGrammar(original_uriStr) && fValidate;
                if (errorCondition && !laxThisOne)
                {
                    fValidator->emitError
                    (
                        XMLValid::GrammarNotFound
                        ,original_uriStr
                    );
                    errorBeforeElementFound = true;
                }
                else if(errorCondition)
                    laxBeforeElementFound = true; 

                elemDecl = fGrammar->getElemDecl
                           (
                               orgGrammarUri
                               , nameRawBuf
                               , qnameRawBuf
                               , currentScope
                           );
                if (elemDecl && elemDecl->getCreateReason() != XMLElementDecl::JustFaultIn && fValidate) {
                    fValidator->emitError
                    (
                        XMLValid::ElementNotQualified
                        , elemDecl->getFullName()
                    );
                    errorBeforeElementFound = true;
                }
            }
        }

        if (!elemDecl) {
            // still not found, fault this in and issue error later
            // switch back to original grammar first
            switchGrammar(original_uriStr);
            elemDecl = new (fMemoryManager) SchemaElementDecl
            (
                fPrefixBuf.getRawBuffer()
                , nameRawBuf
                , uriId
                , SchemaElementDecl::Any
                , Grammar::TOP_LEVEL_SCOPE
                , fMemoryManager
            );
            elemDecl->setId(fElemNonDeclPool->put((void*)elemDecl->getBaseName(), uriId, currentScope, (SchemaElementDecl*)elemDecl));
            wasAdded = true;
        }
    }

    // this info needed for DOMTypeInfo
    fPSVIElemContext.fErrorOccurred = false;

    //  We do something different here according to whether we found the
    //  element or not.
    if (wasAdded)
    {
        if (laxThisOne) {
            fValidate = false;
            fElemStack.setValidationFlag(fValidate);
        }
        else if(fValidate) {
            ((SchemaElementDecl *)(elemDecl))->setValidationAttempted(PSVIDefs::FULL);
        }

        // If validating then emit an error
        if (fValidate)
        {
            // This is to tell the reuse Validator that this element was
            // faulted-in, was not an element in the grammar pool originally
            elemDecl->setCreateReason(XMLElementDecl::JustFaultIn);

            fValidator->emitError
            (
                XMLValid::ElementNotDefined
                , elemDecl->getFullName()
            );
            ((SchemaElementDecl *)(elemDecl))->setValidity(PSVIDefs::INVALID);
            fPSVIElemContext.fErrorOccurred = true;
        }
    }
    else
    {
        if(!laxBeforeElementFound) {
            if (fValidate) {
                ((SchemaElementDecl *)(elemDecl))->setValidationAttempted(PSVIDefs::FULL);
                ((SchemaElementDecl *)(elemDecl))->setValidity(PSVIDefs::VALID);
            }
        }

        // If its not marked declared and validating, then emit an error
        if (!elemDecl->isDeclared()) {
            if(elemDecl->getCreateReason() == XMLElementDecl::NoReason) {
                ((SchemaElementDecl *)(elemDecl))->setValidity(PSVIDefs::INVALID);
                ((SchemaElementDecl *)(elemDecl))->setValidationAttempted(PSVIDefs::FULL);
                fPSVIElemContext.fErrorOccurred = true;
            }
            if (laxThisOne) {
                fValidate = false;
                fElemStack.setValidationFlag(fValidate);                
            }
            
            if (fValidate)
                {
                fValidator->emitError
                (
                    XMLValid::ElementNotDefined
                    , elemDecl->getFullName()
                );
            }
        }
        
        ((SchemaElementDecl*)elemDecl)->setXsiComplexTypeInfo(0);
        ((SchemaElementDecl*)elemDecl)->setXsiSimpleTypeInfo(0);
    }


    if(errorBeforeElementFound) {
        ((SchemaElementDecl *)(elemDecl))->setValidity(PSVIDefs::INVALID);
    }

    //  Now we can update the element stack to set the current element
    //  decl. We expanded the stack above, but couldn't store the element
    //  decl because we didn't know it yet.
    fElemStack.setElement(elemDecl, fReaderMgr.getCurrentReaderNum());
    fElemStack.setCurrentURI(uriId);

    if (isRoot)
    {
        fRootGrammar = fGrammar;
        fRootElemName = XMLString::replicate(qnameRawBuf, fMemoryManager);
    }

    if (fPSVIHandler)
    {
        fPSVIElemContext.fElemDepth++;

        if (elemDecl->isDeclared())
        {
            fPSVIElemContext.fNoneValidationDepth = fPSVIElemContext.fElemDepth;
        }
        else
        {
            fPSVIElemContext.fFullValidationDepth = fPSVIElemContext.fElemDepth;

            /******
             * While we report an error for historical reasons, this should
             * actually result in lax assessment - NG.
            if (isRoot && fValidate)
                fPSVIElemContext.fErrorOccurred = true;
            ******/
        }
    }

    //  Validate the element
    if (fValidate)
    {
        fValidator->validateElement(elemDecl);
        if (((SchemaValidator*) fValidator)->getErrorOccurred())
            fPSVIElemContext.fErrorOccurred = true;
    }

    ComplexTypeInfo* typeinfo = (fValidate)
        ? ((SchemaValidator*)fValidator)->getCurrentTypeInfo()
        : ((SchemaElementDecl*) elemDecl)->getComplexTypeInfo();

    if (typeinfo) {
        currentScope = typeinfo->getScopeDefined();

        // switch grammar if the typeinfo has a different grammar (happens when there is xsi:type)
        XMLCh* typeName = typeinfo->getTypeName();
        const XMLCh poundStr[] = {chPound, chNull};
        if (!XMLString::startsWith(typeName, poundStr)) {
            const int comma = XMLString::indexOf(typeName, chComma);
            if (comma > 0) {
                XMLBuffer prefixBuf(comma+1, fMemoryManager);
                prefixBuf.append(typeName, comma);
                const XMLCh* uriStr = prefixBuf.getRawBuffer();

                bool errorCondition = !switchGrammar(uriStr) && fValidate;
                if (errorCondition && !laxThisOne)
                {
                    fValidator->emitError
                    (
                        XMLValid::GrammarNotFound
                        , prefixBuf.getRawBuffer()
                    );
                    ((SchemaElementDecl *)(elemDecl))->setValidity(PSVIDefs::INVALID);
                }
                else if(errorCondition) {
                    ((SchemaElementDecl *)(elemDecl))->setValidationAttempted(PSVIDefs::NONE);
                    ((SchemaElementDecl *)(elemDecl))->setValidity(PSVIDefs::UNKNOWN);
                }

            }
        }
    }
    fElemStack.setCurrentScope(currentScope);

    // Set element next state
    if (elemDepth >= fElemStateSize) {
        resizeElemState();
    }

    fElemState[elemDepth] = 0;
    fElemStack.setCurrentGrammar(fGrammar);

    //  If this is the first element and we are validating, check the root
    //  element.
    if (isRoot)
    {
        if (fValidate)
        {
            //  Some validators may also want to check the root, call the
            //  XMLValidator::checkRootElement
            if (fValidatorFromUser && !fValidator->checkRootElement(elemDecl->getId())) {
                fValidator->emitError(XMLValid::RootElemNotLikeDocType);
                ((SchemaElementDecl *)(elemDecl))->setValidity(PSVIDefs::INVALID);
            }
        }
    }
    else if (parentValidation)
    {
        //  If the element stack is not empty, then add this element as a
        //  child of the previous top element. If its empty, this is the root
        //  elem and is not the child of anything.
        fElemStack.addChild(elemDecl->getElementName(), true);
    }

    // clear the map used to detect duplicate attributes
    fUndeclaredAttrRegistryNS->removeAll();

    // PSVI handling:  must reset this, even if no attributes...
    if(getPSVIHandler())
        fPSVIAttrList->reset();

    //  Now lets get the fAttrList filled in. This involves faulting in any
    //  defaulted and fixed attributes and normalizing the values of any that
    //  we got explicitly.
    //
    //  We update the attCount value with the total number of attributes, but
    //  it goes in with the number of values we got during the raw scan of
    //  explictly provided attrs above.
    attCount = buildAttList(*fRawAttrList, attCount, elemDecl, *fAttrList);

    // activate identity constraints
    if (fValidate) {

        unsigned int count = ((SchemaElementDecl*) elemDecl)->getIdentityConstraintCount();

        if (count || fMatcherStack->getMatcherCount()) {

            fValueStoreCache->startElement();
            fMatcherStack->pushContext();
            fValueStoreCache->initValueStoresFor((SchemaElementDecl*) elemDecl, (int) elemDepth);

            for (unsigned int i = 0; i < count; i++) {
                activateSelectorFor(((SchemaElementDecl*) elemDecl)->getIdentityConstraintAt(i), (int) elemDepth);
            }

            // call all active identity constraints
            count = fMatcherStack->getMatcherCount();

            for (unsigned int j = 0; j < count; j++) {

                XPathMatcher* matcher = fMatcherStack->getMatcherAt(j);
                matcher->startElement(*elemDecl, uriId, fPrefixBuf.getRawBuffer(), *fAttrList, attCount);
            }
        }
    }

    // Since the element may have default values, call start tag now regardless if it is empty or not
    // If we have a document handler, then tell it about this start tag
    if (fDocHandler)
    {
        fDocHandler->startElement
        (
            *elemDecl
            , uriId
            , fPrefixBuf.getRawBuffer()
            , *fAttrList
            , attCount
            , false
            , isRoot
        );
    } // may be where we output something...

    // if we have a PSVIHandler, now's the time to call
    // its handleAttributesPSVI method:
    if(fPSVIHandler)
    {
        QName *eName = elemDecl->getElementName();
        fPSVIHandler->handleAttributesPSVI
        (
            eName->getLocalPart()
            , fURIStringPool->getValueForId(eName->getURI())
            , fPSVIAttrList
        );
    }

    //  If empty, validate content right now if we are validating and then
    //  pop the element stack top. Else, we have to update the current stack
    //  top's namespace mapping elements.
    if (isEmpty)
    {
        // Pop the element stack back off since it'll never be used now
        fElemStack.popTop();

        // reset current type info
        DatatypeValidator* psviMemberType = 0;
        if (fGrammarType == Grammar::SchemaGrammarType)
        {
            if (fValidate && elemDecl->isDeclared())
            {
                fPSVIElemContext.fCurrentTypeInfo = ((SchemaValidator*) fValidator)->getCurrentTypeInfo();
                if(!fPSVIElemContext.fCurrentTypeInfo)
                    fPSVIElemContext.fCurrentDV = ((SchemaValidator*) fValidator)->getCurrentDatatypeValidator();
                else
                    fPSVIElemContext.fCurrentDV = 0;
                if(fPSVIHandler)
                {
                    fPSVIElemContext.fNormalizedValue = ((SchemaValidator*) fValidator)->getNormalizedValue();

                    if (XMLString::equals(fPSVIElemContext.fNormalizedValue, XMLUni::fgZeroLenString))
                        fPSVIElemContext.fNormalizedValue = 0;
                }
            }
            else
            {
                fPSVIElemContext.fCurrentDV = 0;
                fPSVIElemContext.fCurrentTypeInfo = 0;
                fPSVIElemContext.fNormalizedValue = 0;
            }
        }

        // If validating, then insure that its legal to have no content
        if (fValidate)
        {
            const int res = fValidator->checkContent(elemDecl, 0, 0);
            if (res >= 0)
            {
                // REVISIT:  in the case of xsi:type, this may
                // return the wrong string...
                fValidator->emitError
                (
                    XMLValid::ElementNotValidForContent
                    , elemDecl->getFullName()
                    , elemDecl->getFormattedContentModel()
                );
                ((SchemaElementDecl *)(elemDecl))->setValidity(PSVIDefs::INVALID);
            }

            if (((SchemaValidator*) fValidator)->getErrorOccurred())
                fPSVIElemContext.fErrorOccurred = true;
            // note that if we're empty, won't be a current DV
            else 
            {
                if (fPSVIHandler)
                {
                    fPSVIElemContext.fIsSpecified = ((SchemaValidator*) fValidator)->getIsElemSpecified();
                    if(fPSVIElemContext.fIsSpecified)
                        fPSVIElemContext.fNormalizedValue = ((SchemaElementDecl *)elemDecl)->getDefaultValue();
                }
                if (fPSVIElemContext.fCurrentDV && fPSVIElemContext.fCurrentDV->getType() == DatatypeValidator::Union)
                    psviMemberType = fValidationContext->getValidatingMemberType(); 
            }

            // call matchers and de-activate context
            int oldCount = fMatcherStack->getMatcherCount();
            if (oldCount || ((SchemaElementDecl*) elemDecl)->getIdentityConstraintCount()) {

                for (int i = oldCount - 1; i >= 0; i--) {

                    XPathMatcher* matcher = fMatcherStack->getMatcherAt(i);
                    matcher->endElement(*elemDecl, fContent.getRawBuffer());
                }

                if (fMatcherStack->size() > 0) {
                    fMatcherStack->popContext();
                }

                // handle everything *but* keyref's.
                int newCount = fMatcherStack->getMatcherCount();

                for (int j = oldCount - 1; j >= newCount; j--) {

                    XPathMatcher* matcher = fMatcherStack->getMatcherAt(j);
                    IdentityConstraint* ic = matcher->getIdentityConstraint();

                    if (ic  && (ic->getType() != IdentityConstraint::KEYREF))
                        fValueStoreCache->transplant(ic, matcher->getInitialDepth());
                }

                // now handle keyref's...
                for (int k = oldCount - 1; k >= newCount; k--) {

                    XPathMatcher* matcher = fMatcherStack->getMatcherAt(k);
                    IdentityConstraint* ic = matcher->getIdentityConstraint();

                    if (ic && (ic->getType() == IdentityConstraint::KEYREF)) {

                        ValueStore* values = fValueStoreCache->getValueStoreFor(ic, matcher->getInitialDepth());

                        if (values) { // nothing to do if nothing matched!
                            values->endDcocumentFragment(fValueStoreCache);
                        }
                    }
                }

                fValueStoreCache->endElement();
            }
        }

        if(!isRoot)
           ((SchemaElementDecl *)fElemStack.topElement()->fThisElement)->updateValidityFromElement(elemDecl, fGrammarType);

        if (fPSVIHandler)
        {
            endElementPSVI
            (
                (SchemaElementDecl*)elemDecl, psviMemberType
            );
        }

        // If we have a doc handler, tell it about the end tag
        if (fDocHandler)
        {
            fDocHandler->endElement
            (
                *elemDecl
                , uriId
                , isRoot
                , fPrefixBuf.getRawBuffer()
            );
            // pass information about type info:
            const XMLCh *typeName = SchemaSymbols::fgATTVAL_ANYTYPE;
            const XMLCh *typeURI = SchemaSymbols::fgURI_SCHEMAFORSCHEMA; 
            if(!fPSVIElemContext.fErrorOccurred)
            {
                if(fPSVIElemContext.fCurrentTypeInfo != 0)
                {
                    if(fPSVIElemContext.fCurrentTypeInfo->getAnonymous())
                        typeName = XMLUni::fgZeroLenString;
                    else 
                        typeName = fPSVIElemContext.fCurrentTypeInfo->getTypeLocalName();
                    typeURI = fPSVIElemContext.fCurrentTypeInfo->getTypeUri();
                } 
                else
                {
                    DatatypeValidator *actualDV = (psviMemberType)?psviMemberType:fPSVIElemContext.fCurrentDV;
                    if(actualDV)
                    {
                        if(actualDV->getAnonymous())
                            typeName = XMLUni::fgZeroLenString;
                        else
                            typeName = actualDV->getTypeLocalName();
                        typeURI = actualDV->getTypeUri();
                    }
                }
            }
            else 
            {
                // idiosyncratically, if there's an
                // error but this element was simpleType-validated,
                // the tests demand anySimpleType be returned
                if(fPSVIElemContext.fCurrentDV)
                    typeName = SchemaSymbols::fgDT_ANYSIMPLETYPE;
            }
            fDocHandler->elementTypeInfo(typeName, typeURI);
        }

        // reset xsi:type ComplexTypeInfo        
        ((SchemaElementDecl*)elemDecl)->reset();
        if (!isRoot)
            ((SchemaElementDecl*)(fElemStack.topElement()->fThisElement))->
                setXsiComplexTypeInfo(((SchemaValidator*)fValidator)->getCurrentTypeInfo());

        // If the elem stack is empty, then it was an empty root
        if (isRoot)
            gotData = false;
        else
        {
            // Restore the grammar
            fGrammar = fElemStack.getCurrentGrammar();
            fGrammarType = fGrammar->getGrammarType();
            fValidator->setGrammar(fGrammar);

            // Restore the validation flag
            fValidate = fElemStack.getValidationFlag();
        }
    }
    else    // not empty
        fErrorStack->push(fPSVIElemContext.fErrorOccurred);

    return true;
}


unsigned int
SGXMLScanner::resolveQName(const   XMLCh* const qName
                           ,       XMLBuffer&   prefixBuf
                           , const short        mode
                           ,       int&         prefixColonPos)
{
    //  Lets split out the qName into a URI and name buffer first. The URI
    //  can be empty.
    prefixColonPos = XMLString::indexOf(qName, chColon);
    if (prefixColonPos == -1)
    {
        //  Its all name with no prefix, so put the whole thing into the name
        //  buffer. Then map the empty string to a URI, since the empty string
        //  represents the default namespace. This will either return some
        //  explicit URI which the default namespace is mapped to, or the
        //  the default global namespace.
        bool unknown = false;

        prefixBuf.reset();
        return fElemStack.mapPrefixToURI(XMLUni::fgZeroLenString, (ElemStack::MapModes) mode, unknown);
    }
    else
    {
        //  Copy the chars up to but not including the colon into the prefix
        //  buffer.
        prefixBuf.set(qName, prefixColonPos);

        //  Watch for the special namespace prefixes. We always map these to
        //  special URIs. 'xml' gets mapped to the official URI that its defined
        //  to map to by the NS spec. xmlns gets mapped to a special place holder
        //  URI that we define (so that it maps to something checkable.)
        const XMLCh* prefixRawBuf = prefixBuf.getRawBuffer();
        if (XMLString::equals(prefixRawBuf, XMLUni::fgXMLNSString)) {

            // if this is an element, it is an error to have xmlns as prefix
            if (mode == ElemStack::Mode_Element)
                emitError(XMLErrs::NoXMLNSAsElementPrefix, qName);

            return fXMLNSNamespaceId;
        }
        else if (XMLString::equals(prefixRawBuf, XMLUni::fgXMLString)) {
            return  fXMLNamespaceId;
        }
        else
        {
            bool unknown = false;
            unsigned int uriId = fElemStack.mapPrefixToURI(prefixRawBuf, (ElemStack::MapModes) mode, unknown);

            if (unknown)
                emitError(XMLErrs::UnknownPrefix, prefixRawBuf);

            return uriId;
        }
    }
}

// ---------------------------------------------------------------------------
//  SGXMLScanner: IC activation methos
// ---------------------------------------------------------------------------
void SGXMLScanner::activateSelectorFor(IdentityConstraint* const ic, const int initialDepth) {

    IC_Selector* selector = ic->getSelector();

    if (!selector)
        return;

    XPathMatcher* matcher = selector->createMatcher
    (
        fFieldActivator
        , initialDepth
        , fMemoryManager
    );

    fMatcherStack->addMatcher(matcher);
    matcher->startDocumentFragment();
}

// ---------------------------------------------------------------------------
//  SGXMLScanner: Grammar preparsing
// ---------------------------------------------------------------------------
Grammar* SGXMLScanner::loadGrammar(const   InputSource& src
                                   , const short        grammarType
                                   , const bool         toCache)
{
    Grammar* loadedGrammar = 0;

    try
    {
        fGrammarResolver->cacheGrammarFromParse(false);
		// if the new grammar has to be cached, better use the already cached
		// grammars, or the an exception will be thrown when caching an already
		// cached grammar
        fGrammarResolver->useCachedGrammarInParse(toCache);
        fRootGrammar = 0;

        if (fValScheme == Val_Auto) {
            fValidate = true;
        }

        // Reset some status flags
        fInException = false;
        fStandalone = false;
        fErrorCount = 0;
        fHasNoDTD = true;
        fSeeXsi = false;

        if (grammarType == Grammar::SchemaGrammarType) {
            loadedGrammar = loadXMLSchemaGrammar(src, toCache);
        }

        // Reset the reader manager to close all files, sockets, etc...
        fReaderMgr.reset();
    }
    //  NOTE:
    //
    //  In all of the error processing below, the emitError() call MUST come
    //  before the flush of the reader mgr, or it will fail because it tries
    //  to find out the position in the XML source of the error.
    catch(const XMLErrs::Codes)
    {
        // This is a 'first fatal error' type exit, so reset and fall through
        fReaderMgr.reset();
    }
    catch(const XMLValid::Codes)
    {
        // This is a 'first fatal error' type exit, so reset and fall through
        fReaderMgr.reset();

    }
    catch(const XMLException& excToCatch)
    {
        //  Emit the error and catch any user exception thrown from here. Make
        //  sure in all cases we flush the reader manager.
        fInException = true;
        try
        {
            if (excToCatch.getErrorType() == XMLErrorReporter::ErrType_Warning)
                emitError
                (
                    XMLErrs::DisplayErrorMessage
                    , excToCatch.getMessage()
                );
            else if (excToCatch.getErrorType() >= XMLErrorReporter::ErrType_Fatal)
                emitError
                (
                    XMLErrs::XMLException_Fatal
                    , excToCatch.getType()
                    , excToCatch.getMessage()
                );
            else
                emitError
                (
                    XMLErrs::XMLException_Error
                    , excToCatch.getType()
                    , excToCatch.getMessage()
                );
        }
        catch(const OutOfMemoryException&)
        {
            throw;
        }
        catch(...)
        {
            // Flush the reader manager and rethrow user's error
            fReaderMgr.reset();
            throw;
        }

        // If it returned, then reset the reader manager and fall through
        fReaderMgr.reset();
    }
    catch(const OutOfMemoryException&)
    {
        throw;
    }
    catch(...)
    {
        // Reset and rethrow
        fReaderMgr.reset();
        throw;
    }

    return loadedGrammar;
}

// ---------------------------------------------------------------------------
//  SGXMLScanner: Private helper methods
// ---------------------------------------------------------------------------
//  This method handles the common initialization, to avoid having to do
//  it redundantly in multiple constructors.
void SGXMLScanner::commonInit()
{
    //  Create the element state array
    fElemState = (unsigned int*) fMemoryManager->allocate
    (
        fElemStateSize * sizeof(unsigned int)
    ); //new unsigned int[fElemStateSize];

    //  And we need one for the raw attribute scan. This just stores key/
    //  value string pairs (prior to any processing.)
    fRawAttrList = new (fMemoryManager) RefVectorOf<KVStringPair>(32, true, fMemoryManager);

    // Create dummy schema grammar
    fSchemaGrammar = new (fGrammarPoolMemoryManager) SchemaGrammar(fGrammarPoolMemoryManager);

    //  Create the Validator and init them
    fSchemaValidator = new (fMemoryManager) SchemaValidator(0, fMemoryManager);
    initValidator(fSchemaValidator);

    // Create IdentityConstraint info
    fMatcherStack = new (fMemoryManager) XPathMatcherStack(fMemoryManager);
    fValueStoreCache = new (fMemoryManager) ValueStoreCache(fMemoryManager);
    fFieldActivator = new (fMemoryManager) FieldActivator(fValueStoreCache, fMatcherStack, fMemoryManager);
    fValueStoreCache->setScanner(this);

    //  Add the default entity entries for the character refs that must always
    //  be present.
    fEntityTable = new (fMemoryManager) ValueHashTableOf<XMLCh>(11, fMemoryManager);
    fEntityTable->put((void*) XMLUni::fgAmp, chAmpersand);
    fEntityTable->put((void*) XMLUni::fgLT, chOpenAngle);
    fEntityTable->put((void*) XMLUni::fgGT, chCloseAngle);
    fEntityTable->put((void*) XMLUni::fgQuot, chDoubleQuote);
    fEntityTable->put((void*) XMLUni::fgApos, chSingleQuote);
    fElemNonDeclPool = new (fMemoryManager) RefHash3KeysIdPool<SchemaElementDecl>(29, true, 128, fMemoryManager);
    fAttDefRegistry = new (fMemoryManager) RefHashTableOf<unsigned int>
    (
        131, false, new (fMemoryManager)HashPtr(), fMemoryManager
    );
    fUndeclaredAttrRegistryNS = new (fMemoryManager) RefHash2KeysTableOf<unsigned int>
    (
        7, false, new (fMemoryManager)HashXMLCh(), fMemoryManager
    );
    fPSVIAttrList = new (fMemoryManager) PSVIAttributeList(fMemoryManager);
}

void SGXMLScanner::cleanUp()
{
    fMemoryManager->deallocate(fElemState); //delete [] fElemState;
    delete fSchemaGrammar;
    delete fEntityTable;
    delete fRawAttrList;
    delete fSchemaValidator;
    delete fFieldActivator;
    delete fMatcherStack;
    delete fValueStoreCache;
    delete fElemNonDeclPool;
    delete fAttDefRegistry;
    delete fUndeclaredAttrRegistryNS;
    delete fPSVIAttrList;
    delete fPSVIElement;
    delete fErrorStack;
}

void SGXMLScanner::resizeElemState() {

    unsigned int newSize = fElemStateSize * 2;
    unsigned int* newElemState = (unsigned int*) fMemoryManager->allocate
    (
        newSize * sizeof(unsigned int)
    ); //new unsigned int[newSize];

    // Copy the existing values
    unsigned int index = 0;
    for (; index < fElemStateSize; index++)
        newElemState[index] = fElemState[index];

    for (; index < newSize; index++)
        newElemState[index] = 0;

    // Delete the old array and udpate our members
    fMemoryManager->deallocate(fElemState); //delete [] fElemState;
    fElemState = newElemState;
    fElemStateSize = newSize;
}

//  This method is called from scanStartTag() to build up the list of
//  XMLAttr objects that will be passed out in the start tag callout. We
//  get the key/value pairs from the raw scan of explicitly provided attrs,
//  which have not been normalized. And we get the element declaration from
//  which we will get any defaulted or fixed attribute defs and add those
//  in as well.
unsigned int
SGXMLScanner::buildAttList(const  RefVectorOf<KVStringPair>&  providedAttrs
                          , const unsigned int                attCount
                          ,       XMLElementDecl*             elemDecl
                          ,       RefVectorOf<XMLAttr>&       toFill)
{
    //  Ask the element to clear the 'provided' flag on all of the att defs
    //  that it owns, and to return us a boolean indicating whether it has
    //  any defs.
    ComplexTypeInfo *currType = (fValidate)
                ?((SchemaValidator*)fValidator)->getCurrentTypeInfo()
                :0;
    const bool hasDefs = (currType && fValidate) 
            ? currType->resetDefs()
            : elemDecl->resetDefs();

    fElemCount++;

    //  If there are no expliclitily provided attributes and there are no
    //  defined attributes for the element, the we don't have anything to do.
    //  So just return zero in this case.
    if (!hasDefs && !attCount)
        return 0;

    // Keep up with how many attrs we end up with total
    unsigned int retCount = 0;

    //  And get the current size of the output vector. This lets us use
    //  existing elements until we fill it, then start adding new ones.
    const unsigned int curAttListSize = toFill.size();

    //  We need a buffer into which raw scanned attribute values will be
    //  normalized.
    XMLBufBid bbNormal(&fBufMgr);
    XMLBuffer& normBuf = bbNormal.getBuffer();

    //  Loop through our explicitly provided attributes, which are in the raw
    //  scanned form, and build up XMLAttr objects.
    unsigned int index;
    for (index = 0; index < attCount; index++)
    {
        PSVIItem::VALIDITY_STATE attrValid = PSVIItem::VALIDITY_VALID;
        PSVIItem::ASSESSMENT_TYPE attrAssessed = PSVIItem::VALIDATION_FULL;
        const KVStringPair* curPair = providedAttrs.elementAt(index);

        //  We have to split the name into its prefix and name parts. Then
        //  we map the prefix to its URI.
        const XMLCh* const namePtr = curPair->getKey();
        ArrayJanitor <XMLCh> janName(0);

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
                janName.reset(XMLString::replicate(namePtr, fMemoryManager), fMemoryManager);
                janName[colonInd] = chNull;
                prefPtr = janName.get();
            }

            suffPtr = namePtr + colonInd + 1;
        }
        else
        {
            // No colon, so we just have a name with no prefix
            suffPtr = namePtr;
        }

        //  Map the prefix to a URI id. We tell him that we are mapping an
        //  attr prefix, so any xmlns attrs at this level will not affect it.
        const unsigned int uriId = resolvePrefix(prefPtr, ElemStack::Mode_Attribute);

        //  If the uri comes back as the xmlns or xml URI or its just a name
        //  and that name is 'xmlns', then we handle it specially. So set a
        //  boolean flag that lets us quickly below know which we are dealing
        //  with.
        const bool isNSAttr = (uriId == fXMLNSNamespaceId)
                              || (uriId == fXMLNamespaceId)
                              || XMLString::equals(suffPtr, XMLUni::fgXMLNSString)
                              || XMLString::equals(getURIText(uriId), SchemaSymbols::fgURI_XSI);


        //  If its not a special case namespace attr of some sort, then we
        //  do normal checking and processing.
        XMLAttDef::AttTypes attType;
        DatatypeValidator *attrValidator = 0;
        PSVIAttribute *psviAttr = 0;
        if (!isNSAttr)
        {
            // Some checking for attribute wild card first (for schema)
            bool laxThisOne = false;
            bool skipThisOne = false;

            XMLAttDef* attDefForWildCard = 0;
            XMLAttDef*  attDef = 0;

            if (fGrammarType == Grammar::SchemaGrammarType && currType) {

                //retrieve the att def
                attDef = currType->getAttDef(suffPtr, uriId);

                // if not found or faulted in - check for a matching wildcard attribute
                // if no matching wildcard attribute, check (un)qualifed cases and flag
                // appropriate errors
                if (!attDef || (attDef->getCreateReason() == XMLAttDef::JustFaultIn)) {

                    SchemaAttDef* attWildCard = currType->getAttWildCard();
                    if(!attWildCard)
                        // check explicitly-set wildcard
                        attWildCard = ((SchemaElementDecl*)elemDecl)->getAttWildCard();

                    if (attWildCard) {
                        //if schema, see if we should lax or skip the validation of this attribute
                        if (anyAttributeValidation(attWildCard, uriId, skipThisOne, laxThisOne)) {

                            if(!skipThisOne)
                            {
                                SchemaGrammar* sGrammar = (SchemaGrammar*) fGrammarResolver->getGrammar(getURIText(uriId));
                                if (sGrammar && sGrammar->getGrammarType() == Grammar::SchemaGrammarType) {
                                    RefHashTableOf<XMLAttDef>* attRegistry = sGrammar->getAttributeDeclRegistry();
                                    if (attRegistry) {
                                        attDefForWildCard = attRegistry->get(suffPtr);
                                    }
                                }
                            }
                        }
                    }
                    else {
                        // not found, see if the attDef should be qualified or not
                        if (uriId == fEmptyNamespaceId) {
                            attDef = currType->getAttDef(suffPtr, fURIStringPool->getId(fGrammar->getTargetNamespace()));
                            if (fValidate
                                && attDef
                                && attDef->getCreateReason() != XMLAttDef::JustFaultIn) {
                                // the attribute should be qualified
                                fValidator->emitError
                                (
                                    XMLValid::AttributeNotQualified
                                    , attDef->getFullName()
                                );
                                ((SchemaAttDef *)(attDef))->setValidity(PSVIDefs::INVALID);
                                fPSVIElemContext.fErrorOccurred = true;
                                if (getPSVIHandler())
                                {
                                    attrValid = PSVIItem::VALIDITY_INVALID;
                                }
                            }
                        }
                        else {
                            attDef = currType->getAttDef(suffPtr, fEmptyNamespaceId);
                            if (fValidate
                                && attDef
                                && attDef->getCreateReason() != XMLAttDef::JustFaultIn) {
                                // the attribute should be qualified
                                fValidator->emitError
                                (
                                    XMLValid::AttributeNotUnQualified
                                    , attDef->getFullName()
                                );
                                ((SchemaAttDef *)(attDef))->setValidity(PSVIDefs::INVALID);
                                fPSVIElemContext.fErrorOccurred = true;
                                if (getPSVIHandler())
                                {
                                    attrValid = PSVIItem::VALIDITY_INVALID;
                                }
                            }
                        }
                    }
                }
            }

            // now need to prepare for duplicate detection
            if(attDef)
            {
                unsigned int *curCountPtr = fAttDefRegistry->get(attDef);
                if(!curCountPtr)
                {
                    curCountPtr = getNewUIntPtr();
                    *curCountPtr = fElemCount;
                    fAttDefRegistry->put(attDef, curCountPtr);
                }
                else if(*curCountPtr < fElemCount)
                    *curCountPtr = fElemCount;
                else
                {
                    emitError
                    ( 
                        XMLErrs::AttrAlreadyUsedInSTag
                        , attDef->getFullName()
                        , elemDecl->getFullName()
                    );
                    fPSVIElemContext.fErrorOccurred = true;
                }
            }
            else
            {
                unsigned int *curCountPtr = fUndeclaredAttrRegistryNS->get(suffPtr, uriId);
                if(!curCountPtr)
                {
                    curCountPtr = getNewUIntPtr();
                    *curCountPtr = fElemCount;
                    fUndeclaredAttrRegistryNS->put((void *)suffPtr, uriId, curCountPtr);
                }
                else if(*curCountPtr < fElemCount)
                    *curCountPtr = fElemCount;
                else
                {
                    emitError
                    ( 
                        XMLErrs::AttrAlreadyUsedInSTag
                        , namePtr
                        , elemDecl->getFullName()
                    );
                    fPSVIElemContext.fErrorOccurred = true;
                }
            }
            if(!skipThisOne && fGrammarType == Grammar::SchemaGrammarType && attDef) {
                //we may have set it to invalid already, but this is the first time we are guarenteed to have the attDef
                if(((SchemaAttDef *)(attDef))->getValidity() != PSVIDefs::INVALID)
                {
                    ((SchemaAttDef *)(attDef))->setValidity(PSVIDefs::VALID);                
                }   
                ((SchemaAttDef *)(attDef))->setValidationAttempted(PSVIDefs::FULL);
            }

            // if we've found either an attDef or an attDefForWildCard,
            // then we're doing full validation and it may still be valid.
            if(!attDef && !attDefForWildCard)
            {
                if(!laxThisOne && !skipThisOne)
                {
                    fPSVIElemContext.fErrorOccurred = true;
                } 
                if(getPSVIHandler())
                {
                    if(!laxThisOne && !skipThisOne)
                    {
                        attrValid = PSVIItem::VALIDITY_INVALID;
                    }
                    else if(laxThisOne)
                    {
                        attrValid = PSVIItem::VALIDITY_NOTKNOWN;
                        attrAssessed = PSVIItem::VALIDATION_PARTIAL;
                    }
                    else
                    {
                        attrValid = PSVIItem::VALIDITY_NOTKNOWN;
                        attrAssessed = PSVIItem::VALIDATION_NONE;
                    }
                }
            }

            bool errorCondition = fValidate && !attDefForWildCard && !attDef;
            if (errorCondition && !skipThisOne && !laxThisOne)
            {
                //
                //  Its not valid for this element, so issue an error if we are
                //  validating.
                //
                XMLBufBid bbMsg(&fBufMgr);
                XMLBuffer& bufMsg = bbMsg.getBuffer();
                if (uriId != fEmptyNamespaceId) {
                    XMLBufBid bbURI(&fBufMgr);
                    XMLBuffer& bufURI = bbURI.getBuffer();

                    getURIText(uriId, bufURI);

                    bufMsg.append(chOpenCurly);
                    bufMsg.append(bufURI.getRawBuffer());
                    bufMsg.append(chCloseCurly);
                }
                bufMsg.append(suffPtr);
                fValidator->emitError
                (
                    XMLValid::AttNotDefinedForElement
                    , bufMsg.getRawBuffer()
                    , elemDecl->getFullName()
                );
                if(attDef)
                    ((SchemaAttDef *)attDef)->setValidity(PSVIDefs::INVALID);
            }
            else if(errorCondition && laxThisOne && attDef) {
                ((SchemaAttDef *)(attDef))->setValidationAttempted(PSVIDefs::NONE);
                ((SchemaAttDef *)(attDef))->setValidity(PSVIDefs::UNKNOWN);
            }

            //  Now normalize the raw value since we have the attribute type. We
            //  don't care about the return status here. If it failed, an error
            //  was issued, which is all we care about.
            if (attDefForWildCard) {
                if(attDef)
                    ((SchemaAttDef*)attDef)->setAnyDatatypeValidator(((SchemaAttDef*) attDefForWildCard)->getDatatypeValidator());
                normalizeAttValue
                (
                    attDefForWildCard
                    , namePtr
                    , curPair->getValue()
                    , normBuf
                );

                //  If we found an attdef for this one, then lets validate it.
                if (fNormalizeData)
                {
                    DatatypeValidator* tempDV = ((SchemaAttDef*) attDefForWildCard)->getDatatypeValidator();
                    if (tempDV && tempDV->getWSFacet() != DatatypeValidator::PRESERVE)
                    {
                        // normalize the attribute according to schema whitespace facet
                        XMLBufBid bbtemp(&fBufMgr);
                        XMLBuffer& tempBuf = bbtemp.getBuffer();

                        ((SchemaValidator*) fValidator)->normalizeWhiteSpace(tempDV, normBuf.getRawBuffer(), tempBuf);
                        normBuf.set(tempBuf.getRawBuffer());
                    }
                }

                if (fValidate ) {
                    fValidator->validateAttrValue
                    (
                        attDefForWildCard
                        , normBuf.getRawBuffer()
                        , false
                        , elemDecl
                    );
                    attrValidator = ((SchemaValidator *)fValidator)->getMostRecentAttrValidator();
                    if(((SchemaValidator *)fValidator)->getErrorOccurred())
                    {
                        fPSVIElemContext.fErrorOccurred = true;
                        if(getPSVIHandler())
                            attrValid = PSVIItem::VALIDITY_INVALID;
                    }
                }
                else // no decl; default DOMTypeInfo to anySimpleType
                    attrValidator = DatatypeValidatorFactory::getBuiltInRegistry()->get(SchemaSymbols::fgDT_ANYSIMPLETYPE);

                // Save the type for later use
                attType = attDefForWildCard->getType();

                if(attDef)
                    ((SchemaElementDecl *)(elemDecl))->updateValidityFromAttribute((SchemaAttDef *)attDef);

                DatatypeValidator* tempDV = ((SchemaAttDef*) attDefForWildCard)->getDatatypeValidator();
                if(tempDV && tempDV->getType() == DatatypeValidator::Union && attDef)
                    ((SchemaAttDef*)attDef)->setMembertypeValidator(attrValidator);
            }
            else {
                normalizeAttValue
                (
                    attDef
                    , namePtr
                    , curPair->getValue()
                    , normBuf
                );

                //  If we found an attdef for this one, then lets validate it.
                if (attDef)
                {
                    if (fNormalizeData && (fGrammarType == Grammar::SchemaGrammarType))
                    {
                        DatatypeValidator* tempDV = ((SchemaAttDef*) attDef)->getDatatypeValidator();
                        if (tempDV && tempDV->getWSFacet() != DatatypeValidator::PRESERVE)
                        {
                            // normalize the attribute according to schema whitespace facet
                            XMLBufBid bbtemp(&fBufMgr);
                            XMLBuffer& tempBuf = bbtemp.getBuffer();

                            ((SchemaValidator*) fValidator)->normalizeWhiteSpace(tempDV, normBuf.getRawBuffer(), tempBuf);
                            normBuf.set(tempBuf.getRawBuffer());
                        }
                    }

                    if (fValidate && !skipThisOne)
                    {
                        fValidator->validateAttrValue
                        (
                            attDef
                            , normBuf.getRawBuffer()
                            , false
                            , elemDecl
                        );
                        attrValidator = ((SchemaValidator *)fValidator)->getMostRecentAttrValidator();
                        if(((SchemaValidator *)fValidator)->getErrorOccurred())
                        {
                            fPSVIElemContext.fErrorOccurred = true;
                            if(getPSVIHandler())
                                attrValid = PSVIItem::VALIDITY_INVALID;
                        }
                    }
                    else
                        attrValidator = DatatypeValidatorFactory::getBuiltInRegistry()->get(SchemaSymbols::fgDT_ANYSIMPLETYPE);
                }
                else 
                    attrValidator = DatatypeValidatorFactory::getBuiltInRegistry()->get(SchemaSymbols::fgDT_ANYSIMPLETYPE);

                // Save the type for later use
                attType = (attDef)?attDef->getType():XMLAttDef::CData;
                if(attDef)
                {
                    ((SchemaElementDecl *)(elemDecl))->updateValidityFromAttribute((SchemaAttDef *)attDef);
                } 
            }

            // now fill in the PSVIAttributes entry for this attribute:
	        if(getPSVIHandler())
	        {
	            psviAttr = fPSVIAttrList->getPSVIAttributeToFill(suffPtr, fURIStringPool->getValueForId(uriId));
	            SchemaAttDef *actualAttDef = 0;
	            if(attDef)
	                actualAttDef = (SchemaAttDef *)attDef;
	            else if (attDefForWildCard)
	                actualAttDef = (SchemaAttDef *)attDefForWildCard;
                if(actualAttDef)
                {
	                XSAttributeDeclaration *attrDecl = (XSAttributeDeclaration *)fModel->getXSObject(actualAttDef);
                    DatatypeValidator * attrDataType = actualAttDef->getDatatypeValidator();
	                XSSimpleTypeDefinition *validatingType = (XSSimpleTypeDefinition *)fModel->getXSObject(attrDataType);
	                if(attrValid != PSVIItem::VALIDITY_VALID)
	                {
	                    psviAttr->reset
                        (
	                        fRootElemName
	                        , attrValid
	                        , attrAssessed
	                        , validatingType
	                        , 0
	                        , actualAttDef->getValue()
	                        , false
	                        , attrDecl
                            , 0
	                    );
	                }
	                else
	                {
	                    XSSimpleTypeDefinition *memberType = 0;
	                    if(validatingType->getVariety() == XSSimpleTypeDefinition::VARIETY_UNION)
	                        memberType = (XSSimpleTypeDefinition *)fModel->getXSObject(attrValidator);
	                    psviAttr->reset
                        (
	                        fRootElemName
	                        , attrValid
	                        , attrAssessed
	                        , validatingType
	                        , memberType
	                        , actualAttDef->getValue()
	                        , false
	                        , attrDecl
                            , (memberType)?attrValidator:attrDataType
	                    );
	                }
                }
                else
                {
	                psviAttr->reset
                    (
	                    fRootElemName
	                    , attrValid
	                    , attrAssessed
	                    , 0
	                    , 0
                        , 0
	                    , false
	                    , 0
                        , 0
	                );
                }
	        }
        }
        else
        {
            // Just normalize as CDATA
            attType = XMLAttDef::CData;
            normalizeAttRawValue
            (
                namePtr
                , curPair->getValue()
                , normBuf
            );
            if((uriId == fXMLNSNamespaceId)
                  || XMLString::equals(getURIText(uriId), SchemaSymbols::fgURI_XSI))
                attrValidator = DatatypeValidatorFactory::getBuiltInRegistry()->get(SchemaSymbols::fgDT_ANYURI);
            if(getPSVIHandler())
            {
                psviAttr = fPSVIAttrList->getPSVIAttributeToFill(suffPtr, fURIStringPool->getValueForId(uriId));
	            XSSimpleTypeDefinition *validatingType = (attrValidator)
                            ? (XSSimpleTypeDefinition *)fModel->getXSObject(attrValidator)
                            : 0;
                // no attribute declarations for these...
	            psviAttr->reset(
	                fRootElemName
	                , PSVIItem::VALIDITY_NOTKNOWN
	                , PSVIItem::VALIDATION_NONE
	                , validatingType
	                , 0
	                , 0
                    , false
	                , 0
                    , attrValidator
                );
            }
        }

        //  Add this attribute to the attribute list that we use to pass them
        //  to the handler. We reuse its existing elements but expand it as
        //  required.
        XMLAttr* curAttr;
        if (retCount >= curAttListSize)
        {
            curAttr = new (fMemoryManager) XMLAttr
            (
                uriId
                , suffPtr
                , prefPtr
                , normBuf.getRawBuffer()
                , attType
                , true
                , fMemoryManager
                , attrValidator
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
                , attrValidator
                , true
            );
            curAttr->setSpecified(true);
        }
        if(psviAttr)
            psviAttr->setValue(curAttr->getValue());

        // Bump the count of attrs in the list
        retCount++;
    }

    //  Now, if there are any attributes declared by this element, let's
    //  go through them and make sure that any required ones are provided,
    //  and fault in any fixed ones and defaulted ones that are not provided
    //  literally.
    if (hasDefs)
    {
        // Check after all specified attrs are scanned
        // (1) report error for REQUIRED attrs that are missing (V_TAGc)
        // (2) add default attrs if missing (FIXED and NOT_FIXED)

        XMLAttDefList& attDefList = getAttDefList(currType, elemDecl);

        for(unsigned int i=0; i<attDefList.getAttDefCount(); i++)
        {
            // Get the current att def, for convenience and its def type
            XMLAttDef *curDef = &attDefList.getAttDef(i);            
            const XMLAttDef::DefAttTypes defType = curDef->getDefaultType();

            unsigned int *attCountPtr = fAttDefRegistry->get(curDef);
            if (!attCountPtr || *attCountPtr < fElemCount)
            { // did not occur
                ((SchemaAttDef *)curDef)->setValidationAttempted(PSVIDefs::FULL);
                ((SchemaAttDef *)curDef)->setValidity(PSVIDefs::VALID);
                // note that since there is no attribute information
                // item present, there is no PSVI infoset to augment here *except*
                // that the element is invalid

                //the attribute is not provided
                if (fValidate)
                {
                    // If we are validating and its required, then an error
                    if ((defType == XMLAttDef::Required) ||
                        (defType == XMLAttDef::Required_And_Fixed)  )

                    {
                        fValidator->emitError
                        (
                            XMLValid::RequiredAttrNotProvided
                            , curDef->getFullName()
                        );
                        ((SchemaAttDef *)(curDef))->setValidity(PSVIDefs::INVALID);
                        fPSVIElemContext.fErrorOccurred = true;
                    }
                    else if ((defType == XMLAttDef::Default) ||
                             (defType == XMLAttDef::Fixed)  )
                    {
                        if (fStandalone && curDef->isExternal())
                        {
                            // XML 1.0 Section 2.9
                            // Document is standalone, so attributes must not be defaulted.
                            fValidator->emitError(XMLValid::NoDefAttForStandalone, curDef->getFullName(), elemDecl->getFullName());
                            ((SchemaAttDef *)(curDef))->setValidity(PSVIDefs::INVALID);
                        }
                    }
                }

                //  Fault in the value if needed, and bump the att count.
                if ((defType == XMLAttDef::Default)
                    ||  (defType == XMLAttDef::Fixed))
                {
                    // Let the validator pass judgement on the attribute value
                    if (fValidate)
                    {
                        fValidator->validateAttrValue
                        (
                            curDef
                            , curDef->getValue()
                            , false
                            , elemDecl
                        );
                    }

                    XMLAttr* curAtt;
                    if (retCount >= curAttListSize)
                    {
                        curAtt = new (fMemoryManager) XMLAttr(fMemoryManager);
                        fValidator->faultInAttr(*curAtt, *curDef);
                        fAttrList->addElement(curAtt);
                    }
                    else
                    {
                        curAtt = fAttrList->elementAt(retCount);
                        fValidator->faultInAttr(*curAtt, *curDef);
                    }

                    // Indicate it was not explicitly specified and bump count
                    curAtt->setSpecified(false);
                    retCount++;
                    if(getPSVIHandler())
                    {
                        QName *attName = ((SchemaAttDef *)curDef)->getAttName();
                        PSVIAttribute *defAttrToFill = fPSVIAttrList->getPSVIAttributeToFill
                        (
                            attName->getLocalPart(), fURIStringPool->getValueForId( attName->getURI())
                        );
                        XSAttributeDeclaration *defAttrDecl = (XSAttributeDeclaration *)fModel->getXSObject((void *)curDef);
                        DatatypeValidator * attrDataType = ((SchemaAttDef *)curDef)->getDatatypeValidator();
                        XSSimpleTypeDefinition *defAttrType = 
                            (XSSimpleTypeDefinition*)fModel->getXSObject(attrDataType);
                        // would have occurred during validation of default value
                        if(((SchemaValidator *)fValidator)->getErrorOccurred())
                        {
                            defAttrToFill->reset(
                                fRootElemName
                                , PSVIItem::VALIDITY_INVALID
                                , PSVIItem::VALIDATION_FULL
                                , defAttrType
                                , 0
                                , curDef->getValue()
                                , true 
                                , defAttrDecl
                                , 0
                            );
                        }
                        else
                        {
                            XSSimpleTypeDefinition *defAttrMemberType = 0;
                            if(defAttrType->getVariety() == XSSimpleTypeDefinition::VARIETY_UNION)
                            {
                                defAttrMemberType = (XSSimpleTypeDefinition *)fModel->getXSObject
                                (
                                    ((SchemaValidator*)fValidator)->getMostRecentAttrValidator()
                                );
                            }
                            defAttrToFill->reset
                            (
                                fRootElemName
                                , PSVIItem::VALIDITY_VALID
                                , PSVIItem::VALIDATION_FULL
                                , defAttrType
                                , defAttrMemberType
                                , curDef->getValue()
                                , true
                                , defAttrDecl
                                , (defAttrMemberType)?((SchemaValidator *)fValidator)->getMostRecentAttrValidator():attrDataType
                            );
                        }
                        defAttrToFill->setValue(curDef->getValue());
                    }
                }

                ((SchemaElementDecl *)elemDecl)->updateValidityFromAttribute((SchemaAttDef *)curDef);
            }
            else if (attCountPtr)
            {
                //attribute is provided
                // (schema) report error for PROHIBITED attrs that are present (V_TAGc)
                if (defType == XMLAttDef::Prohibited && fValidate) 
                {
                    fValidator->emitError
                    (
                        XMLValid::ProhibitedAttributePresent
                        , curDef->getFullName()
                    );
                    ((SchemaAttDef *)curDef)->setValidity(PSVIDefs::INVALID);         
                    fPSVIElemContext.fErrorOccurred = true;
                    if (getPSVIHandler())
                    {
                        QName *attQName = ((SchemaAttDef *)curDef)->getAttName();
                        // bad luck...
                        PSVIAttribute *prohibitedAttr = fPSVIAttrList->getAttributePSVIByName
                        (
                            attQName->getLocalPart(), 
                            fURIStringPool->getValueForId(attQName->getURI())
                        );
                        prohibitedAttr->updateValidity(PSVIItem::VALIDITY_INVALID);
                    }
                }
                ((SchemaElementDecl *)elemDecl)->updateValidityFromAttribute((SchemaAttDef *)curDef);
            }
        }
    }

    return retCount;
}


//  This method will take a raw attribute value and normalize it according to
//  the rules of the attribute type. It will put the resulting value into the
//  passed buffer.
//
//  This code assumes that escaped characters in the original value (via char
//  refs) are prefixed by a 0xFFFF character. This is because some characters
//  are legal if escaped only. And some escape chars are not subject to
//  normalization rules.
bool SGXMLScanner::normalizeAttValue( const   XMLAttDef* const    attDef
                                      , const XMLCh* const        attrName
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
    const XMLAttDef::AttTypes type = (attDef)
                            ?attDef->getType()
                            :XMLAttDef::CData;

    // Assume its going to go fine, and empty the target buffer in preperation
    bool retVal = true;
    toFill.reset();

    // Get attribute def - to check to see if it's declared externally or not
    bool  isAttExternal = (attDef)
                        ?attDef->isExternal()
                        :false;

    //  Loop through the chars of the source value and normalize it according
    //  to the type.
    States curState = InContent;
    bool escaped;
    bool firstNonWS = false;
    XMLCh nextCh;
    const XMLCh* srcPtr = value;
    while (*srcPtr)
    {
        //  Get the next character from the source. We have to watch for
        //  escaped characters (which are indicated by a 0xFFFF value followed
        //  by the char that was escaped.)
        nextCh = *srcPtr;
        escaped = (nextCh == 0xFFFF);
        if (escaped)
            nextCh = *++srcPtr;

        //  If its not escaped, then make sure its not a < character, which is
        //  not allowed in attribute values.
        if (!escaped && (*srcPtr == chOpenAngle))
        {
            emitError(XMLErrs::BracketInAttrValue, attrName);
            retVal = false;
        }

        if (type == XMLAttDef::CData || type > XMLAttDef::Notation)
        {
            if (!escaped)
            {
                if ((nextCh == 0x09) || (nextCh == 0x0A) || (nextCh == 0x0D))
                {
                    // Check Validity Constraint for Standalone document declaration
                    // XML 1.0, Section 2.9
                    if (fStandalone && fValidate && isAttExternal)
                    {
                        // Can't have a standalone document declaration of "yes" if  attribute
                        // values are subject to normalisation
                        fValidator->emitError(XMLValid::NoAttNormForStandalone, attrName);
                        if(attDef)
                            ((SchemaAttDef *)attDef)->setValidity(PSVIDefs::INVALID);     
                        if (getPSVIHandler())
                        {
                            // REVISIT:               
                            // PSVIAttribute->setValidity(PSVIItem::VALIDITY_INVALID);
                        }
                    }
                    nextCh = chSpace;
                }
            }
        }
        else
        {
            if (curState == InWhitespace)
            {
                if (!fReaderMgr.getCurrentReader()->isWhitespace(nextCh))
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
                if (fReaderMgr.getCurrentReader()->isWhitespace(nextCh))
                {
                    curState = InWhitespace;
                    srcPtr++;

                    // Check Validity Constraint for Standalone document declaration
                    // XML 1.0, Section 2.9
                    if (fStandalone && fValidate && isAttExternal)
                    {
                        if (!firstNonWS || (nextCh != chSpace) || (!*srcPtr) || fReaderMgr.getCurrentReader()->isWhitespace(*srcPtr))
                        {
                            // Can't have a standalone document declaration of "yes" if  attribute
                            // values are subject to normalisation
                            fValidator->emitError(XMLValid::NoAttNormForStandalone, attrName);
                            if(attDef)
                                ((SchemaAttDef *)attDef)->setValidity(PSVIDefs::INVALID);
                            if (getPSVIHandler())
                            {
                                // REVISIT:                
                                // PSVIAttribute->setValidity(PSVIItem::VALIDITY_INVALID);
                            }
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

    if(attDef)
        ((SchemaElementDecl *)fElemStack.topElement()->fThisElement)->updateValidityFromAttribute((SchemaAttDef *)attDef);

    return retVal;
}

//  This method will just normalize the input value as CDATA without
//  any standalone checking.
bool SGXMLScanner::normalizeAttRawValue( const   XMLCh* const        attrName
                                      , const XMLCh* const        value
                                      ,       XMLBuffer&          toFill)
{
    // Assume its going to go fine, and empty the target buffer in preperation
    bool retVal = true;
    toFill.reset();

    //  Loop through the chars of the source value and normalize it according
    //  to the type.
    bool escaped;
    XMLCh nextCh;
    const XMLCh* srcPtr = value;
    while (*srcPtr)
    {
        //  Get the next character from the source. We have to watch for
        //  escaped characters (which are indicated by a 0xFFFF value followed
        //  by the char that was escaped.)
        nextCh = *srcPtr;
        escaped = (nextCh == 0xFFFF);
        if (escaped)
            nextCh = *++srcPtr;

        //  If its not escaped, then make sure its not a < character, which is
        //  not allowed in attribute values.
        if (!escaped && (*srcPtr == chOpenAngle))
        {
            emitError(XMLErrs::BracketInAttrValue, attrName);
            retVal = false;
        }

        if (!escaped)
        {
            //  NOTE: Yes this is a little redundant in that a 0x20 is
            //  replaced with an 0x20. But its faster to do this (I think)
            //  than checking for 9, A, and D separately.
            if (fReaderMgr.getCurrentReader()->isWhitespace(nextCh))
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
SGXMLScanner::resolvePrefix(  const   XMLCh* const        prefix
                              , const ElemStack::MapModes mode)
{
    //  Watch for the special namespace prefixes. We always map these to
    //  special URIs. 'xml' gets mapped to the official URI that its defined
    //  to map to by the NS spec. xmlns gets mapped to a special place holder
    //  URI that we define (so that it maps to something checkable.)
    if (XMLString::equals(prefix, XMLUni::fgXMLNSString))
        return fXMLNSNamespaceId;
    else if (XMLString::equals(prefix, XMLUni::fgXMLString))
        return fXMLNamespaceId;

    //  Ask the element stack to search up itself for a mapping for the
    //  passed prefix.
    bool unknown;
    unsigned int uriId = fElemStack.mapPrefixToURI(prefix, mode, unknown);

    // If it was unknown, then the URI was faked in but we have to issue an error
    if (unknown)
        emitError(XMLErrs::UnknownPrefix, prefix);

    return uriId;
}

unsigned int
SGXMLScanner::resolvePrefix(  const   XMLCh* const        prefix
                              ,       XMLBuffer&          bufToFill
                              , const ElemStack::MapModes mode)
{
    //  Watch for the special namespace prefixes. We always map these to
    //  special URIs. 'xml' gets mapped to the official URI that its defined
    //  to map to by the NS spec. xmlns gets mapped to a special place holder
    //  URI that we define (so that it maps to something checkable.)
    if (XMLString::equals(prefix, XMLUni::fgXMLNSString))
        return fXMLNSNamespaceId;
    else if (XMLString::equals(prefix, XMLUni::fgXMLString))
        return fXMLNamespaceId;

    //  Ask the element stack to search up itself for a mapping for the
    //  passed prefix.
    bool unknown;
    unsigned int uriId = fElemStack.mapPrefixToURI(prefix, mode, unknown);

    // If it was unknown, then the URI was faked in but we have to issue an error
    if (unknown)
        emitError(XMLErrs::UnknownPrefix, prefix);

    getURIText(uriId,bufToFill);

    return uriId;
}


//  This method will reset the scanner data structures, and related plugged
//  in stuff, for a new scan session. We get the input source for the primary
//  XML entity, create the reader for it, and push it on the stack so that
//  upon successful return from here we are ready to go.
void SGXMLScanner::scanReset(const InputSource& src)
{

    //  This call implicitly tells us that we are going to reuse the scanner
    //  if it was previously used. So tell the validator to reset itself.
    //
    //  But, if the fUseCacheGrammar flag is set, then don't reset it.
    //
    //  NOTE:   The ReaderMgr is flushed on the way out, because that is
    //          required to insure that files are closed.
    fGrammarResolver->cacheGrammarFromParse(fToCacheGrammar);
    fGrammarResolver->useCachedGrammarInParse(fUseCachedGrammar);

    fGrammar = fSchemaGrammar;
    fGrammarType = Grammar::DTDGrammarType;
    fRootGrammar = 0;

    fValidator->setGrammar(fGrammar);
    if (fValidatorFromUser) {

        ((SchemaValidator*) fValidator)->setErrorReporter(fErrorReporter);
        ((SchemaValidator*) fValidator)->setGrammarResolver(fGrammarResolver);
        ((SchemaValidator*) fValidator)->setExitOnFirstFatal(fExitOnFirstFatal);
    }

    // Reset validation
    fValidate = (fValScheme == Val_Always) ? true : false;

    //  And for all installed handlers, send reset events. This gives them
    //  a chance to flush any cached data.
    if (fDocHandler)
        fDocHandler->resetDocument();
    if (fEntityHandler)
        fEntityHandler->resetEntities();
    if (fErrorReporter)
        fErrorReporter->resetErrors();

    // Clear out the id reference list
    resetValidationContext();

    // Reset IdentityConstraints
    fValueStoreCache->startDocument();
    fMatcherStack->clear();

    //  Reset the element stack, and give it the latest ids for the special
    //  URIs it has to know about.
    fElemStack.reset
    (
        fEmptyNamespaceId
        , fUnknownNamespaceId
        , fXMLNamespaceId
        , fXMLNSNamespaceId
    );

    if (!fSchemaNamespaceId)
        fSchemaNamespaceId  = fURIStringPool->addOrFind(SchemaSymbols::fgURI_XSI);

    // Reset some status flags
    fInException = false;
    fStandalone = false;
    fErrorCount = 0;
    fHasNoDTD = true;
    fSeeXsi = false;
    fDoNamespaces = true;
    fDoSchema = true;

    // Reset PSVI context
    // Note that we always need this around for DOMTypeInfo
    if (!fPSVIElement)
        fPSVIElement = new (fMemoryManager) PSVIElement(fMemoryManager);

    if (!fErrorStack)
        fErrorStack = new (fMemoryManager) ValueStackOf<bool>(8, fMemoryManager);

    resetPSVIElemContext();

    // Reset the validators
    fSchemaValidator->reset();
    fSchemaValidator->setErrorReporter(fErrorReporter);
    fSchemaValidator->setExitOnFirstFatal(fExitOnFirstFatal);
    fSchemaValidator->setGrammarResolver(fGrammarResolver);
    if (fValidatorFromUser)
        fValidator->reset();

    //  Handle the creation of the XML reader object for this input source.
    //  This will provide us with transcoding and basic lexing services.
    XMLReader* newReader = fReaderMgr.createReader
    (
        src
        , true
        , XMLReader::RefFrom_NonLiteral
        , XMLReader::Type_General
        , XMLReader::Source_External
        , fCalculateSrcOfs
    );

    if (!newReader) {
        if (src.getIssueFatalErrorIfNotFound())
            ThrowXMLwithMemMgr1(RuntimeException, XMLExcepts::Scan_CouldNotOpenSource, src.getSystemId(), fMemoryManager);
        else
            ThrowXMLwithMemMgr1(RuntimeException, XMLExcepts::Scan_CouldNotOpenSource_Warning, src.getSystemId(), fMemoryManager);
    }

    // Push this read onto the reader manager
    fReaderMgr.pushReader(newReader, 0);

    // and reset security-related things if necessary:
    if(fSecurityManager != 0) 
    {
        fEntityExpansionLimit = fSecurityManager->getEntityExpansionLimit();
        fEntityExpansionCount = 0;
    }
    fElemCount = 0;
    if(fUIntPoolRowTotal >= 32) 
    { // 8 KB tied up with validating attributes...
        fAttDefRegistry->removeAll();
        fUndeclaredAttrRegistryNS->removeAll();
        recreateUIntPool();
    }
    else
    {
        // note that this will implicitly reset the values of the hashtables,
        // though their buckets will still be tied up
        resetUIntPool();
    }
    
}


//  This method is called between markup in content. It scans for character
//  data that is sent to the document handler. It watches for any markup
//  characters that would indicate that the character data has ended. It also
//  handles expansion of general and character entities.
//
//  sendData() is a local static helper for this method which handles some
//  code that must be done in three different places here.
void SGXMLScanner::sendCharData(XMLBuffer& toSend)
{
    // If no data in the buffer, then nothing to do
    if (toSend.isEmpty())
        return;

    //  We do different things according to whether we are validating or
    //  not. If not, its always just characters; else, it depends on the
    //  current element's content model.
    if (fValidate)
    {
        // Get the raw data we need for the callback
        const XMLCh* const rawBuf = toSend.getRawBuffer();
        const unsigned int len = toSend.getLen();

        // Get the character data opts for the current element
        XMLElementDecl::CharDataOpts charOpts = XMLElementDecl::AllCharData;
        // And see if the current element is a 'Children' style content model
        ComplexTypeInfo *currType = ((SchemaValidator*)fValidator)->getCurrentTypeInfo();
        if(currType) 
        {
            SchemaElementDecl::ModelTypes modelType = (SchemaElementDecl::ModelTypes) currType->getContentType(); 
            if(modelType == SchemaElementDecl::Children) 
                charOpts = XMLElementDecl::SpacesOk;
            else if(modelType == SchemaElementDecl::Empty) 
                charOpts = XMLElementDecl::NoCharData; 
        } 

        // should not be necessary once PSVI method on element decls
        // are removed
        const ElemStack::StackElem *topElem = fElemStack.topElement();
        if (charOpts == XMLElementDecl::NoCharData)
        {
            // They definitely cannot handle any type of char data
            fValidator->emitError(XMLValid::NoCharDataInCM);
            ((SchemaElementDecl *)topElem->fThisElement)->setValidity(PSVIDefs::INVALID);
            if (getPSVIHandler())
            {
                // REVISIT:           
                // PSVIElement->setValidity(PSVIItem::VALIDITY_INVALID);
            }
        }
        else if (fReaderMgr.getCurrentReader()->isAllSpaces(rawBuf, len))
        {
            //  Its all spaces. So, if they can take spaces, then send it
            //  as ignorable whitespace. If they can handle any char data
            //  send it as characters.
            if (charOpts == XMLElementDecl::SpacesOk) {
                if (fDocHandler)
                    fDocHandler->ignorableWhitespace(rawBuf, len, false);
            }
            else if (charOpts == XMLElementDecl::AllCharData)
            {
                // The normalized data can only be as large as the
                // original size, so this will avoid allocating way
                // too much or too little memory.
                XMLBuffer toFill(len+1, fMemoryManager);
                toFill.set(rawBuf);

                if (fNormalizeData)
                {
                    DatatypeValidator* tempDV = ((SchemaValidator*) fValidator)->getCurrentDatatypeValidator();
                    if (tempDV && tempDV->getWSFacet() != DatatypeValidator::PRESERVE)
                    {
                        // normalize the character according to schema whitespace facet
                        XMLBufBid bbtemp(&fBufMgr);
                        XMLBuffer& tempBuf = bbtemp.getBuffer();

                        ((SchemaValidator*) fValidator)->normalizeWhiteSpace(tempDV, toFill.getRawBuffer(),  tempBuf);
                        toFill.set(tempBuf.getRawBuffer());
                    }
                }

                // tell the schema validation about the character data for checkContent later
                ((SchemaValidator*)fValidator)->setDatatypeBuffer(toFill.getRawBuffer());

                // call all active identity constraints
                if (fMatcherStack->getMatcherCount())
                    fContent.append(toFill.getRawBuffer(), toFill.getLen());

                if (fDocHandler)
                    fDocHandler->docCharacters(toFill.getRawBuffer(), toFill.getLen(), false);
            }
        }
        else
        {
            //  If they can take any char data, then send it. Otherwise, they
            //  can only handle whitespace and can't handle this stuff so
            //  issue an error.
            if (charOpts == XMLElementDecl::AllCharData)
            {
                // The normalized data can only be as large as the
                // original size, so this will avoid allocating way
                // too much or too little memory.
                XMLBuffer toFill(len+1, fMemoryManager);
                toFill.set(rawBuf);

                if (fNormalizeData)
                {
                    DatatypeValidator* tempDV = ((SchemaValidator*) fValidator)->getCurrentDatatypeValidator();
                    if (tempDV && tempDV->getWSFacet() != DatatypeValidator::PRESERVE)
                    {
                        // normalize the character according to schema whitespace facet
                        XMLBufBid bbtemp(&fBufMgr);
                        XMLBuffer& tempBuf = bbtemp.getBuffer();

                        ((SchemaValidator*) fValidator)->normalizeWhiteSpace(tempDV, toFill.getRawBuffer(),  tempBuf);
                        toFill.set(tempBuf.getRawBuffer());
                    }
                }

                // tell the schema validation about the character data for checkContent later
                ((SchemaValidator*)fValidator)->setDatatypeBuffer(toFill.getRawBuffer());

                // call all active identity constraints
                if (fMatcherStack->getMatcherCount())
                    fContent.append(toFill.getRawBuffer(), toFill.getLen());

                if (fDocHandler)
                    fDocHandler->docCharacters(toFill.getRawBuffer(), toFill.getLen(), false);
            }
            else
            {
                fValidator->emitError(XMLValid::NoCharDataInCM);
                ((SchemaElementDecl *)topElem->fThisElement)->setValidity(PSVIDefs::INVALID);
                if (getPSVIHandler())
                {
                    // REVISIT:                
                    // PSVIElement->setValidity(PSVIItem::VALIDITY_INVALID);
                }
            }
        }
    }
    else
    {
        // call all active identity constraints
        if (fMatcherStack->getMatcherCount())
            fContent.append(toSend.getRawBuffer(), toSend.getLen());

        // Always assume its just char data if not validating
        if (fDocHandler)
            fDocHandler->docCharacters(toSend.getRawBuffer(), toSend.getLen(), false);
    }

    // Reset buffer
    toSend.reset();
}



//  This method is called with a key/value string pair that represents an
//  xmlns="yyy" or xmlns:xxx="yyy" attribute. This method will update the
//  current top of the element stack based on this data. We know that when
//  we get here, that it is one of these forms, so we don't bother confirming
//  it.
//
//  But we have to ensure
//      1. xxx is not xmlns
//      2. if xxx is xml, then yyy must match XMLUni::fgXMLURIName, and vice versa
//      3. yyy is not XMLUni::fgXMLNSURIName
//      4. if xxx is not null, then yyy cannot be an empty string.
void SGXMLScanner::updateNSMap(const  XMLCh* const    attrName
                              , const XMLCh* const    attrValue)
{
    // We need a buffer to normalize the attribute value into
    XMLBufBid bbNormal(&fBufMgr);
    XMLBuffer& normalBuf = bbNormal.getBuffer();

    //  Normalize the value into the passed buffer. In this case, we don't
    //  care about the return value. An error was issued for the error, which
    //  is all we care about here.
    normalizeAttRawValue(attrName, attrValue, normalBuf);
    XMLCh* namespaceURI = normalBuf.getRawBuffer();

    //  We either have the default prefix (""), or we point it into the attr
    //  name parameter. Note that the xmlns is not the prefix we care about
    //  here. To us, the 'prefix' is really the local part of the attrName
    //  parameter.
    //
    //  Check 1. xxx is not xmlns
    //        2. if xxx is xml, then yyy must match XMLUni::fgXMLURIName, and vice versa
    //        3. yyy is not XMLUni::fgXMLNSURIName
    //        4. if xxx is not null, then yyy cannot be an empty string.
    const XMLCh* prefPtr = XMLUni::fgZeroLenString;
    const int colonOfs = XMLString::indexOf(attrName, chColon);
    if (colonOfs != -1) {
        prefPtr = &attrName[colonOfs + 1];

        if (XMLString::equals(prefPtr, XMLUni::fgXMLNSString))
            emitError(XMLErrs::NoUseOfxmlnsAsPrefix);
        else if (XMLString::equals(prefPtr, XMLUni::fgXMLString)) {
            if (!XMLString::equals(namespaceURI, XMLUni::fgXMLURIName))
                emitError(XMLErrs::PrefixXMLNotMatchXMLURI);
        }

        if (!namespaceURI)
            emitError(XMLErrs::NoEmptyStrNamespace, attrName);
        else if(!*namespaceURI && fXMLVersion == XMLReader::XMLV1_0)
            emitError(XMLErrs::NoEmptyStrNamespace, attrName);
    }

    if (XMLString::equals(namespaceURI, XMLUni::fgXMLNSURIName))
        emitError(XMLErrs::NoUseOfxmlnsURI);
    else if (XMLString::equals(namespaceURI, XMLUni::fgXMLURIName)) {
        if (!XMLString::equals(prefPtr, XMLUni::fgXMLString))
            emitError(XMLErrs::XMLURINotMatchXMLPrefix);
    }

    //  Ok, we have to get the unique id for the attribute value, which is the
    //  URI that this value should be mapped to. The validator has the
    //  namespace string pool, so we ask him to find or add this new one. Then
    //  we ask the element stack to add this prefix to URI Id mapping.
    fElemStack.addPrefix
    (
        prefPtr
        , fURIStringPool->addOrFind(namespaceURI)
    );
}

void SGXMLScanner::scanRawAttrListforNameSpaces(int attCount)
{
    //  Make an initial pass through the list and find any xmlns attributes or
    //  schema attributes.
    //  When we find one, send it off to be used to update the element stack's
    //  namespace mappings.
    int index = 0;
    for (index = 0; index < attCount; index++)
    {
        // each attribute has the prefix:suffix="value"
        const KVStringPair* curPair = fRawAttrList->elementAt(index);
        const XMLCh* rawPtr = curPair->getKey();

        //  If either the key begins with "xmlns:" or its just plain
        //  "xmlns", then use it to update the map.
        if (!XMLString::compareNString(rawPtr, XMLUni::fgXMLNSColonString, 6)
        ||  XMLString::equals(rawPtr, XMLUni::fgXMLNSString))
        {
            const XMLCh* valuePtr = curPair->getValue();

            updateNSMap(rawPtr, valuePtr);

            // if the schema URI is seen in the the valuePtr, set the boolean seeXsi
            if (XMLString::equals(valuePtr, SchemaSymbols::fgURI_XSI)) {
                fSeeXsi = true;
            }
        }
    }

    // walk through the list again to deal with "xsi:...."
    if (fSeeXsi)
    {
        //  Schema Xsi Type yyyy (e.g. xsi:type="yyyyy")
        XMLBufBid bbXsi(&fBufMgr);
        XMLBuffer& fXsiType = bbXsi.getBuffer();

        QName attName(fMemoryManager);

        for (index = 0; index < attCount; index++)
        {
            // each attribute has the prefix:suffix="value"
            const KVStringPair* curPair = fRawAttrList->elementAt(index);
            const XMLCh* rawPtr = curPair->getKey();

            attName.setName(rawPtr, fEmptyNamespaceId);
            const XMLCh* prefPtr = attName.getPrefix();

            // if schema URI has been seen, scan for the schema location and uri
            // and resolve the schema grammar; or scan for schema type
            if (resolvePrefix(prefPtr, ElemStack::Mode_Attribute) == fSchemaNamespaceId) {

                const XMLCh* valuePtr = curPair->getValue();
                const XMLCh* suffPtr = attName.getLocalPart();

                if (XMLString::equals(suffPtr, SchemaSymbols::fgXSI_SCHEMALOCACTION))
                    parseSchemaLocation(valuePtr);
                else if (XMLString::equals(suffPtr, SchemaSymbols::fgXSI_NONAMESPACESCHEMALOCACTION))
                    resolveSchemaGrammar(valuePtr, XMLUni::fgZeroLenString);

                if (XMLString::equals(suffPtr, SchemaSymbols::fgXSI_TYPE)) {
                        fXsiType.set(valuePtr);
                }
                else if (XMLString::equals(suffPtr, SchemaSymbols::fgATT_NILL)
                         && fValidator && fValidator->handlesSchema()
                         && XMLString::equals(valuePtr, SchemaSymbols::fgATTVAL_TRUE)) {
                            ((SchemaValidator*)fValidator)->setNillable(true);
                }
            }
        }

        if (fValidator && fValidator->handlesSchema()) {
            if (!fXsiType.isEmpty()) {
                int colonPos = -1;
                unsigned int uriId = resolveQName (
                      fXsiType.getRawBuffer()
                    , fPrefixBuf
                    , ElemStack::Mode_Element
                    , colonPos
                );
                ((SchemaValidator*)fValidator)->setXsiType(fPrefixBuf.getRawBuffer(), fXsiType.getRawBuffer() + colonPos + 1, uriId);
            }
        }
    }
}

void SGXMLScanner::parseSchemaLocation(const XMLCh* const schemaLocationStr)
{
    BaseRefVectorOf<XMLCh>* schemaLocation = XMLString::tokenizeString(schemaLocationStr, fMemoryManager);
    ArrayJanitor<BaseRefVectorOf<XMLCh> > janLoc(schemaLocation, fMemoryManager);

    unsigned int size = schemaLocation->size();
    if (size % 2 != 0 ) {
        emitError(XMLErrs::BadSchemaLocation);
    } else {
        for(unsigned int i=0; i<size; i=i+2) {
            resolveSchemaGrammar(schemaLocation->elementAt(i+1), schemaLocation->elementAt(i));
        }
    }
}

void SGXMLScanner::resolveSchemaGrammar(const XMLCh* const loc, const XMLCh* const uri) {

    Grammar* grammar = fGrammarResolver->getGrammar(uri);

    if (!grammar || grammar->getGrammarType() == Grammar::DTDGrammarType) {
        XSDDOMParser parser(0, fMemoryManager, 0);

        parser.setValidationScheme(XercesDOMParser::Val_Never);
        parser.setDoNamespaces(true);
        parser.setUserEntityHandler(fEntityHandler);
        parser.setUserErrorReporter(fErrorReporter);

        // Create a buffer for expanding the system id
        XMLBufBid bbSys(&fBufMgr);
        XMLBuffer& expSysId = bbSys.getBuffer();
        XMLBuffer& normalizedSysId = bbSys.getBuffer();

        normalizeURI(loc, normalizedSysId);

        //  Allow the entity handler to expand the system id if they choose
        //  to do so.
        InputSource* srcToFill = 0;
        const XMLCh* normalizedURI = normalizedSysId.getRawBuffer();
        if (fEntityHandler)
        {
            if (!fEntityHandler->expandSystemId(normalizedURI, expSysId))
                expSysId.set(normalizedURI);

            ReaderMgr::LastExtEntityInfo lastInfo;
            fReaderMgr.getLastExtEntityInfo(lastInfo);
            XMLResourceIdentifier resourceIdentifier(XMLResourceIdentifier::SchemaGrammar,
                            expSysId.getRawBuffer(), uri, XMLUni::fgZeroLenString, lastInfo.systemId);
            srcToFill = fEntityHandler->resolveEntity(&resourceIdentifier);
        }
        else
        {
            expSysId.set(normalizedURI);
        }

        //  If they didn't create a source via the entity handler, then we
        //  have to create one on our own.
        if (!srcToFill)
        {
            ReaderMgr::LastExtEntityInfo lastInfo;
            fReaderMgr.getLastExtEntityInfo(lastInfo);

            XMLURL urlTmp(fMemoryManager);
            if ((!urlTmp.setURL(lastInfo.systemId, expSysId.getRawBuffer(), urlTmp)) ||
                (urlTmp.isRelative()))
            {
                if (!fStandardUriConformant)
                    srcToFill = new (fMemoryManager) LocalFileInputSource
                    (
                        lastInfo.systemId
                        , expSysId.getRawBuffer()
                        , fMemoryManager
                    );
                else
                    ThrowXMLwithMemMgr(MalformedURLException, XMLExcepts::URL_MalformedURL, fMemoryManager);            
            }
            else
            {
                if (fStandardUriConformant && urlTmp.hasInvalidChar())
                    ThrowXMLwithMemMgr(MalformedURLException, XMLExcepts::URL_MalformedURL, fMemoryManager);
                srcToFill = new (fMemoryManager) URLInputSource(urlTmp, fMemoryManager);
            }        
        }

        // Put a janitor on the input source
        Janitor<InputSource> janSrc(srcToFill);

        // Should just issue warning if the schema is not found
        bool flag = srcToFill->getIssueFatalErrorIfNotFound();
        srcToFill->setIssueFatalErrorIfNotFound(false);

        parser.parse(*srcToFill);

        // Reset the InputSource
        srcToFill->setIssueFatalErrorIfNotFound(flag);

        if (parser.getSawFatal() && fExitOnFirstFatal)
            emitError(XMLErrs::SchemaScanFatalError);

        DOMDocument* document = parser.getDocument(); //Our Grammar

        if (document != 0) {

            DOMElement* root = document->getDocumentElement();// This is what we pass to TraverserSchema
            if (root != 0)
            {
                const XMLCh* newUri = root->getAttribute(SchemaSymbols::fgATT_TARGETNAMESPACE);
                if (!XMLString::equals(newUri, uri)) {
                    if (fValidate || fValScheme == Val_Auto) {
                        fValidator->emitError(XMLValid::WrongTargetNamespace, loc, uri);
                    }

                    grammar = fGrammarResolver->getGrammar(newUri);
                }

                if (!grammar || grammar->getGrammarType() == Grammar::DTDGrammarType) {

                    //  Since we have seen a grammar, set our validation flag
                    //  at this point if the validation scheme is auto
                    if (fValScheme == Val_Auto && !fValidate) {
                        fValidate = true;
                        fElemStack.setValidationFlag(fValidate);
                    }

                    grammar = new (fGrammarPoolMemoryManager) SchemaGrammar(fGrammarPoolMemoryManager);
                    XMLSchemaDescription* gramDesc = (XMLSchemaDescription*) grammar->getGrammarDescription();
                    gramDesc->setContextType(XMLSchemaDescription::CONTEXT_PREPARSE);
                    gramDesc->setLocationHints(srcToFill->getSystemId());

                    TraverseSchema traverseSchema
                    (
                        root
                        , fURIStringPool
                        , (SchemaGrammar*) grammar
                        , fGrammarResolver
                        , this
                        , srcToFill->getSystemId()
                        , fEntityHandler
                        , fErrorReporter
                        , fMemoryManager
                    );

                    if (fGrammarType == Grammar::DTDGrammarType) {
                        fGrammar = grammar;
                        fGrammarType = Grammar::SchemaGrammarType;
                        fValidator->setGrammar(fGrammar);
                    }

                    if (fValidate) {
                        //  validate the Schema scan so far
                        fValidator->preContentValidation(false);
                    }
                }
            }
        }
    }
    else {

        //  Since we have seen a grammar, set our validation flag
        //  at this point if the validation scheme is auto
        if (fValScheme == Val_Auto && !fValidate) {
            fValidate = true;
            fElemStack.setValidationFlag(fValidate);
        }

        // we have seen a schema, so set up the fValidator as fSchemaValidator
        if (fGrammarType == Grammar::DTDGrammarType) {
            fGrammar = grammar;
            fGrammarType = Grammar::SchemaGrammarType;
            fValidator->setGrammar(fGrammar);
        }
    }
    // update fModel; rely on the grammar resolver to do this
    // efficiently
    if(getPSVIHandler())
        fModel = fGrammarResolver->getXSModel();
}

InputSource* SGXMLScanner::resolveSystemId(const XMLCh* const sysId)
{
    // Create a buffer for expanding the system id
    XMLBufBid bbSys(&fBufMgr);
    XMLBuffer& expSysId = bbSys.getBuffer();

    //  Allow the entity handler to expand the system id if they choose
    //  to do so.
    InputSource* srcToFill = 0;
    if (fEntityHandler)
    {
        if (!fEntityHandler->expandSystemId(sysId, expSysId))
            expSysId.set(sysId);

        ReaderMgr::LastExtEntityInfo lastInfo;
        fReaderMgr.getLastExtEntityInfo(lastInfo);
        XMLResourceIdentifier resourceIdentifier(XMLResourceIdentifier::ExternalEntity,
                            expSysId.getRawBuffer(), 0, XMLUni::fgZeroLenString, lastInfo.systemId);
        srcToFill = fEntityHandler->resolveEntity(&resourceIdentifier);
    }
    else
    {
        expSysId.set(sysId);
    }

    //  If they didn't create a source via the entity handler, then we
    //  have to create one on our own.
    if (!srcToFill)
    {
        ReaderMgr::LastExtEntityInfo lastInfo;
        fReaderMgr.getLastExtEntityInfo(lastInfo);

        XMLURL urlTmp(fMemoryManager);
        if ((!urlTmp.setURL(lastInfo.systemId, expSysId.getRawBuffer(), urlTmp)) ||
            (urlTmp.isRelative()))
        {
            if (!fStandardUriConformant)
                srcToFill = new (fMemoryManager) LocalFileInputSource
                (
                    lastInfo.systemId
                    , expSysId.getRawBuffer()
                    , fMemoryManager
                );
            else
                ThrowXMLwithMemMgr(MalformedURLException, XMLExcepts::URL_MalformedURL, fMemoryManager);            
        }
        else
        {
            if (fStandardUriConformant && urlTmp.hasInvalidChar())
                ThrowXMLwithMemMgr(MalformedURLException, XMLExcepts::URL_MalformedURL, fMemoryManager);
            srcToFill = new (fMemoryManager) URLInputSource(urlTmp, fMemoryManager);
        }              
    }

    return srcToFill;
}


// ---------------------------------------------------------------------------
//  SGXMLScanner: Private grammar preparsing methods
// ---------------------------------------------------------------------------
Grammar* SGXMLScanner::loadXMLSchemaGrammar(const InputSource& src,
                                          const bool toCache)
{
   // Reset the validators
    fSchemaValidator->reset();
    fSchemaValidator->setErrorReporter(fErrorReporter);
    fSchemaValidator->setExitOnFirstFatal(fExitOnFirstFatal);
    fSchemaValidator->setGrammarResolver(fGrammarResolver);

    if (fValidatorFromUser)
        fValidator->reset();

    XSDDOMParser parser(0, fMemoryManager, 0);

    parser.setValidationScheme(XercesDOMParser::Val_Never);
    parser.setDoNamespaces(true);
    parser.setUserEntityHandler(fEntityHandler);
    parser.setUserErrorReporter(fErrorReporter);

    // Should just issue warning if the schema is not found
    bool flag = src.getIssueFatalErrorIfNotFound();
    ((InputSource&) src).setIssueFatalErrorIfNotFound(false);

    parser.parse(src);

    // Reset the InputSource
    ((InputSource&) src).setIssueFatalErrorIfNotFound(flag);

    if (parser.getSawFatal() && fExitOnFirstFatal)
        emitError(XMLErrs::SchemaScanFatalError);

    DOMDocument* document = parser.getDocument(); //Our Grammar

    if (document != 0) {

        DOMElement* root = document->getDocumentElement();// This is what we pass to TraverserSchema
        if (root != 0)
        {
            SchemaGrammar* grammar = new (fGrammarPoolMemoryManager) SchemaGrammar(fGrammarPoolMemoryManager);
            XMLSchemaDescription* gramDesc = (XMLSchemaDescription*) grammar->getGrammarDescription();
            gramDesc->setContextType(XMLSchemaDescription::CONTEXT_PREPARSE);
            gramDesc->setLocationHints(src.getSystemId());

            TraverseSchema traverseSchema
            (
                root
                , fURIStringPool
                , (SchemaGrammar*) grammar
                , fGrammarResolver
                , this
                , src.getSystemId()
                , fEntityHandler
                , fErrorReporter
                , fMemoryManager
            );

            if (fValidate) {
                //  validate the Schema scan so far
                fValidator->setGrammar(grammar);
                fValidator->preContentValidation(false, true);
            }

            if (toCache) {
                fGrammarResolver->cacheGrammars();
            }

            if(getPSVIHandler())
                fModel = fGrammarResolver->getXSModel();

            return grammar;
        }
    }

    return 0;
}



// ---------------------------------------------------------------------------
//  SGXMLScanner: Private parsing methods
// ---------------------------------------------------------------------------

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
bool SGXMLScanner::basicAttrValueScan(const XMLCh* const attrName, XMLBuffer& toFill)
{
    // Reset the target buffer
    toFill.reset();

    // Get the next char which must be a single or double quote
    XMLCh quoteCh;
    if (!fReaderMgr.skipIfQuote(quoteCh))
        return false;

    //  We have to get the current reader because we have to ignore closing
    //  quotes until we hit the same reader again.
    const unsigned int curReader = fReaderMgr.getCurrentReaderNum();

    //  Loop until we get the attribute value. Note that we use a double
    //  loop here to avoid the setup/teardown overhead of the exception
    //  handler on every round.
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
                nextCh = fReaderMgr.getNextChar();

                if (!nextCh)
                    ThrowXMLwithMemMgr(UnexpectedEOFException, XMLExcepts::Gen_UnexpectedEOF, fMemoryManager);

                //  Check for our ending quote. It has to be in the same entity
                //  as where we started. Quotes in nested entities are ignored.
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

                //  Check for an entity ref . We ignore the empty flag in
                //  this one.
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
                else if ((nextCh >= 0xD800) && (nextCh <= 0xDBFF))
                {
                    // Deal with surrogate pairs
                    //  Its a leading surrogate. If we already got one, then
                    //  issue an error, else set leading flag to make sure that
                    //  we look for a trailing next time.
                    if (gotLeadingSurrogate)
                    {
                        emitError(XMLErrs::Expected2ndSurrogateChar);
                    }
                    else
                        gotLeadingSurrogate = true;
                }
                else
                {
                    //  If its a trailing surrogate, make sure that we are
                    //  prepared for that. Else, its just a regular char so make
                    //  sure that we were not expected a trailing surrogate.
                    if ((nextCh >= 0xDC00) && (nextCh <= 0xDFFF))
                    {
                        // Its trailing, so make sure we were expecting it
                        if (!gotLeadingSurrogate)
                            emitError(XMLErrs::Unexpected2ndSurrogateChar);
                    }
                    else
                    {
                        //  Its just a char, so make sure we were not expecting a
                        //  trailing surrogate.
                        if (gotLeadingSurrogate) {
                            emitError(XMLErrs::Expected2ndSurrogateChar);
                        }
                        // Its got to at least be a valid XML character
                        else if (!fReaderMgr.getCurrentReader()->isXMLChar(nextCh))
                        {
                            XMLCh tmpBuf[9];
                            XMLString::binToText
                            (
                                nextCh
                                , tmpBuf
                                , 8
                                , 16
                                , fMemoryManager
                            );
                            emitError(XMLErrs::InvalidCharacterInAttrValue, attrName, tmpBuf);
                        }
                    }
                    gotLeadingSurrogate = false;
                }

                //  If it was escaped, then put in a 0xFFFF value. This will
                //  be used later during validation and normalization of the
                //  value to know that the following character was via an
                //  escape char.
                if (escaped)
                    toFill.append(0xFFFF);

                // Else add it to the buffer
                toFill.append(nextCh);

                if (secondCh)
                    toFill.append(secondCh);
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


//  This method scans a CDATA section. It collects the character into one
//  of the temp buffers and calls the document handler, if any, with the
//  characters. It assumes that the <![CDATA string has been scanned before
//  this call.
void SGXMLScanner::scanCDSection()
{
    static const XMLCh CDataClose[] =
    {
            chCloseSquare, chCloseAngle, chNull
    };

    //  The next character should be the opening square bracket. If not
    //  issue an error, but then try to recover by skipping any whitespace
    //  and checking again.
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

    //  We just scan forward until we hit the end of CDATA section sequence.
    //  CDATA is effectively a big escape mechanism so we don't treat markup
    //  characters specially here.
    bool            emittedError = false;
    bool    gotLeadingSurrogate = false;

    // Get the character data opts for the current element
    XMLElementDecl::CharDataOpts charOpts = XMLElementDecl::AllCharData;
    // And see if the current element is a 'Children' style content model
    ComplexTypeInfo *currType = ((SchemaValidator*)fValidator)->getCurrentTypeInfo();
    if(currType) 
    {
        SchemaElementDecl::ModelTypes modelType = (SchemaElementDecl::ModelTypes) currType->getContentType(); 
        if(modelType == SchemaElementDecl::Children) 
            charOpts = XMLElementDecl::SpacesOk;
        else if(modelType == SchemaElementDecl::Empty) 
            charOpts = XMLElementDecl::NoCharData; 
    } 

    // should not be necessary when PSVI on element decl removed
    const ElemStack::StackElem* topElem = fElemStack.topElement();

    while (true)
    {
        const XMLCh nextCh = fReaderMgr.getNextChar();

        // Watch for unexpected end of file
        if (!nextCh)
        {
            emitError(XMLErrs::UnterminatedCDATASection);
            ThrowXMLwithMemMgr(UnexpectedEOFException, XMLExcepts::Gen_UnexpectedEOF, fMemoryManager);
        }

        if (fValidate && fStandalone && (fReaderMgr.getCurrentReader()->isWhitespace(nextCh)))
        {
            // This document is standalone; this ignorable CDATA whitespace is forbidden.
            // XML 1.0, Section 2.9
            // And see if the current element is a 'Children' style content model
            if (topElem->fThisElement->isExternal()) {

                if (charOpts == XMLElementDecl::SpacesOk) // Element Content
                {
                    // Error - standalone should have a value of "no" as whitespace detected in an
                    // element type with element content whose element declaration was external
                    fValidator->emitError(XMLValid::NoWSForStandalone);
                    ((SchemaElementDecl *)(topElem->fThisElement))->setValidity(PSVIDefs::INVALID);
                    if (getPSVIHandler())
                    {
                        // REVISIT:                
                        // PSVIElement->setValidity(PSVIItem::VALIDITY_INVALID);
                    }
                }
            }
        }

        //  If this is a close square bracket it could be our closing
        //  sequence.
        if (nextCh == chCloseSquare && fReaderMgr.skippedString(CDataClose))
        {
            //  make sure we were not expecting a trailing surrogate.
            if (gotLeadingSurrogate)
                emitError(XMLErrs::Expected2ndSurrogateChar);

            if (fValidate) {

                if (fNormalizeData)
                {
                    DatatypeValidator* tempDV = ((SchemaValidator*) fValidator)->getCurrentDatatypeValidator();
                    if (tempDV && tempDV->getWSFacet() != DatatypeValidator::PRESERVE)
                    {
                        // normalize the character according to schema whitespace facet
                        XMLBufBid bbtemp(&fBufMgr);
                        XMLBuffer& tempBuf = bbtemp.getBuffer();

                        ((SchemaValidator*) fValidator)->normalizeWhiteSpace(tempDV, bbCData.getRawBuffer(),  tempBuf);
                        bbCData.set(tempBuf.getRawBuffer());
                    }
                }

                // tell the schema validation about the character data for checkContent later
                ((SchemaValidator*)fValidator)->setDatatypeBuffer(bbCData.getRawBuffer());

                if (charOpts != XMLElementDecl::AllCharData)
                {
                    // They definitely cannot handle any type of char data
                    fValidator->emitError(XMLValid::NoCharDataInCM);
                    ((SchemaElementDecl *)topElem->fThisElement)->setValidity(PSVIDefs::INVALID);
                    if (getPSVIHandler())
                    {
                        // REVISIT:                
                        // PSVIElement->setValidity(PSVIItem::VALIDITY_INVALID);
                    }
                }
            }

            // call all active identity constraints
            if (fMatcherStack->getMatcherCount())
                fContent.append(bbCData.getRawBuffer(), bbCData.getLen());

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

        //  Make sure its a valid character. But if we've emitted an error
        //  already, don't bother with the overhead since we've already told
        //  them about it.
        if (!emittedError)
        {
            // Deal with surrogate pairs
            if ((nextCh >= 0xD800) && (nextCh <= 0xDBFF))
            {
                //  Its a leading surrogate. If we already got one, then
                //  issue an error, else set leading flag to make sure that
                //  we look for a trailing next time.
                if (gotLeadingSurrogate)
                    emitError(XMLErrs::Expected2ndSurrogateChar);
                else
                    gotLeadingSurrogate = true;
            }
            else
            {
                //  If its a trailing surrogate, make sure that we are
                //  prepared for that. Else, its just a regular char so make
                //  sure that we were not expected a trailing surrogate.
                if ((nextCh >= 0xDC00) && (nextCh <= 0xDFFF))
                {
                    // Its trailing, so make sure we were expecting it
                    if (!gotLeadingSurrogate)
                        emitError(XMLErrs::Unexpected2ndSurrogateChar);
                }
                else
                {
                    //  Its just a char, so make sure we were not expecting a
                    //  trailing surrogate.
                    if (gotLeadingSurrogate)
                        emitError(XMLErrs::Expected2ndSurrogateChar);

                    // Its got to at least be a valid XML character
                    else if (!fReaderMgr.getCurrentReader()->isXMLChar(nextCh))
                    {
                        XMLCh tmpBuf[9];
                        XMLString::binToText
                        (
                            nextCh
                            , tmpBuf
                            , 8
                            , 16
                            , fMemoryManager
                        );
                        emitError(XMLErrs::InvalidCharacter, tmpBuf);
                        emittedError = true;
                    }
                }
                gotLeadingSurrogate = false;
            }
        }

        // Add it to the buffer
        bbCData.append(nextCh);
    }
}


void SGXMLScanner::scanCharData(XMLBuffer& toUse)
{
    //  We have to watch for the stupid ]]> sequence, which is illegal in
    //  character data. So this is a little state machine that handles that.
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

    //  In order to be more efficient we have to use kind of a deeply nested
    //  set of blocks here. The outer block puts on a try and catches end of
    //  entity exceptions. The inner loop is the per-character loop. If we
    //  put the try inside the inner loop, it would work but would require
    //  the exception handling code setup/teardown code to be invoked for
    //  each character.
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
                //  Eat through as many plain content characters as possible without
                //  needing special handling.  Moving most content characters here,
                //  in this one call, rather than running the overall loop once
                //  per content character, is a speed optimization.
                if (curState == State_Waiting  &&  !gotLeadingSurrogate)
                {
                     fReaderMgr.movePlainContentChars(toUse);
                }

                // Try to get another char from the source
                //   The code from here on down covers all contengencies,
                if (!fReaderMgr.getNextCharIfNot(chOpenAngle, nextCh))
                {
                    // If we were waiting for a trailing surrogate, its an error
                    if (gotLeadingSurrogate)
                        emitError(XMLErrs::Expected2ndSurrogateChar);

                    notDone = false;
                    break;
                }

                //  Watch for a reference. Note that the escapement mechanism
                //  is ignored in this content.
                escaped = false;
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
                else if ((nextCh >= 0xD800) && (nextCh <= 0xDBFF))
                {
                    // Deal with surrogate pairs
                    //  Its a leading surrogate. If we already got one, then
                    //  issue an error, else set leading flag to make sure that
                    //  we look for a trailing next time.
                    if (gotLeadingSurrogate)
                        emitError(XMLErrs::Expected2ndSurrogateChar);
                    else
                        gotLeadingSurrogate = true;
                }
                else
                {
                    //  If its a trailing surrogate, make sure that we are
                    //  prepared for that. Else, its just a regular char so make
                    //  sure that we were not expected a trailing surrogate.
                    if ((nextCh >= 0xDC00) && (nextCh <= 0xDFFF))
                    {
                        // Its trailing, so make sure we were expecting it
                        if (!gotLeadingSurrogate)
                            emitError(XMLErrs::Unexpected2ndSurrogateChar);
                    }
                    else
                    {
                        //  Its just a char, so make sure we were not expecting a
                        //  trailing surrogate.
                        if (gotLeadingSurrogate)
                            emitError(XMLErrs::Expected2ndSurrogateChar);

                        // Make sure the returned char is a valid XML char
                        if (!fReaderMgr.getCurrentReader()->isXMLChar(nextCh))
                        {
                            XMLCh tmpBuf[9];
                            XMLString::binToText
                            (
                                nextCh
                                , tmpBuf
                                , 8
                                , 16
                                , fMemoryManager
                            );
                            emitError(XMLErrs::InvalidCharacter, tmpBuf);
                        }
                    }
                    gotLeadingSurrogate = false;
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

                // Add this char to the buffer
                toUse.append(nextCh);

                if (secondCh)
                    toUse.append(secondCh);
            }
        }
        catch(const EndOfEntityException& toCatch)
        {
            //  Some entity ended, so we have to send any accumulated
            //  chars and send an end of entity event.
            sendCharData(toUse);
            gotLeadingSurrogate = false;

            if (fDocHandler)
                fDocHandler->endEntityReference(toCatch.getEntity());
        }
    }

    // Check the validity constraints as per XML 1.0 Section 2.9
    if (fValidate && fStandalone)
    {
        // See if the text contains whitespace
        // Get the raw data we need for the callback
        const XMLCh* rawBuf = toUse.getRawBuffer();
        const unsigned int len = toUse.getLen();
        const bool isSpaces = fReaderMgr.getCurrentReader()->containsWhiteSpace(rawBuf, len);

        if (isSpaces)
        {
            // And see if the current element is a 'Children' style content model
            const ElemStack::StackElem* topElem = fElemStack.topElement();

            if (topElem->fThisElement->isExternal()) {

                // Get the character data opts for the current element
                XMLElementDecl::CharDataOpts charOpts = XMLElementDecl::AllCharData;
                // And see if the current element is a 'Children' style content model
                ComplexTypeInfo *currType = ((SchemaValidator*)fValidator)->getCurrentTypeInfo();
                if(currType) 
                {
                    SchemaElementDecl::ModelTypes modelType = (SchemaElementDecl::ModelTypes) currType->getContentType(); 
                    if(modelType == SchemaElementDecl::Children) 
                        charOpts = XMLElementDecl::SpacesOk;
                } 

                if (charOpts == XMLElementDecl::SpacesOk)  // => Element Content
                {
                    // Error - standalone should have a value of "no" as whitespace detected in an
                    // element type with element content whose element declaration was external
                    //
                    fValidator->emitError(XMLValid::NoWSForStandalone);
                    ((SchemaElementDecl *)fElemStack.topElement()->fThisElement)->setValidity(PSVIDefs::INVALID);
                    if (getPSVIHandler())
                    {
                        // REVISIT:                
                        // PSVIElement->setValidity(PSVIItem::VALIDITY_INVALID);
                    }
                }
            }
        }
    }
    // Send any char data that we accumulated into the buffer
    sendCharData(toUse);
}


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
SGXMLScanner::EntityExpRes
SGXMLScanner::scanEntityRef(  const   bool
                            ,       XMLCh&  firstCh
                            ,       XMLCh&  secondCh
                            ,       bool&   escaped)
{
    // Assume no escape
    secondCh = 0;
    escaped = false;

    // We have to insure that its all in one entity
    const unsigned int curReader = fReaderMgr.getCurrentReaderNum();

    //  If the next char is a pound, then its a character reference and we
    //  need to expand it always.
    if (fReaderMgr.skippedChar(chPound))
    {
        //  Its a character reference, so scan it and get back the numeric
        //  value it represents.
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

    //  Next char must be a semi-colon. But if its not, just emit
    //  an error and try to continue.
    if (!fReaderMgr.skippedChar(chSemiColon))
        emitError(XMLErrs::UnterminatedEntityRef, bbName.getRawBuffer());

    // Make sure we ended up on the same entity reader as the & char
    if (curReader != fReaderMgr.getCurrentReaderNum())
        emitError(XMLErrs::PartialMarkupInEntity);

    // Look up the name in the general entity pool
    // If it does not exist, then obviously an error
    if (!fEntityTable->containsKey(bbName.getRawBuffer()))
    {
        // XML 1.0 Section 4.1
        // Well-formedness Constraint for entity not found:
        //   In a document without any DTD, a document with only an internal DTD subset which contains no parameter entity references,
        //      or a document with "standalone='yes'", for an entity reference that does not occur within the external subset
        //      or a parameter entity
        if (fStandalone || fHasNoDTD)
            emitError(XMLErrs::EntityNotFound, bbName.getRawBuffer());

        return EntityExp_Failed;
    }

    // here's where we need to check if there's a SecurityManager,
    // how many entity references we've had
    if(fSecurityManager != 0 && ++fEntityExpansionCount > fEntityExpansionLimit) {
        XMLCh expLimStr[16];
        XMLString::binToText(fEntityExpansionLimit, expLimStr, 15, 10, fMemoryManager);
        emitError
        ( 
            XMLErrs::EntityExpansionLimitExceeded
            , expLimStr
        );
        // there seems nothing better to be done than to reset the entity expansion limit
        fEntityExpansionCount = 0;
    }

    firstCh = fEntityTable->get(bbName.getRawBuffer());
    escaped = true;
    return EntityExp_Returned;
}


bool SGXMLScanner::switchGrammar(const XMLCh* const newGrammarNameSpace)
{
    Grammar* tempGrammar = fGrammarResolver->getGrammar(newGrammarNameSpace);

    if (!tempGrammar) {
        tempGrammar = fSchemaGrammar;
    }

    if (!tempGrammar)
        return false;
    else {
        fGrammar = tempGrammar;
        fGrammarType = fGrammar->getGrammarType();
        if (fGrammarType == Grammar::DTDGrammarType) {
            ThrowXMLwithMemMgr(RuntimeException, XMLExcepts::Gen_NoDTDValidator, fMemoryManager);
        }

        fValidator->setGrammar(fGrammar);
        return true;
    }
}

// check if we should skip or lax the validation of the element
// if skip - no validation
// if lax - validate only if the element if found
bool SGXMLScanner::laxElementValidation(QName* element, ContentLeafNameTypeVector* cv,
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
                      && XMLString::equals(fElemMap->getLocalPart(), element->getLocalPart()))
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


// check if there is an AnyAttribute, and if so, see if we should lax or skip
// if skip - no validation
// if lax - validate only if the attribute if found
bool SGXMLScanner::anyAttributeValidation(SchemaAttDef* attWildCard, unsigned int uriId, bool& skipThisOne, bool& laxThisOne)
{
    XMLAttDef::AttTypes wildCardType = attWildCard->getType();
    bool anyEncountered = false;
    skipThisOne = false;
    laxThisOne = false;
    if (wildCardType == XMLAttDef::Any_Any)
        anyEncountered = true;
    else if (wildCardType == XMLAttDef::Any_Other) {
        if (attWildCard->getAttName()->getURI() != uriId
            && uriId != fEmptyNamespaceId)
            anyEncountered = true;
    }
    else if (wildCardType == XMLAttDef::Any_List) {
        ValueVectorOf<unsigned int>* nameURIList = attWildCard->getNamespaceList();
        unsigned int listSize = (nameURIList) ? nameURIList->size() : 0;

        if (listSize) {
            for (unsigned int i=0; i < listSize; i++) {
                if (nameURIList->elementAt(i) == uriId)
                    anyEncountered = true;
            }
        }
    }

    if (anyEncountered) {
        XMLAttDef::DefAttTypes   defType   = attWildCard->getDefaultType();
        if (defType == XMLAttDef::ProcessContents_Skip) {
            // attribute should just be bypassed,
            skipThisOne = true;
            attWildCard->setValidationAttempted(PSVIDefs::NONE);
            if (getPSVIHandler())
            {
                // REVISIT:
                // PSVIAttribute->setValidationAttempted(PSVIItem::VALIDATION_NONE);            
            }
        }
        else if (defType == XMLAttDef::ProcessContents_Lax) {
            laxThisOne = true;
        }
    }

    return anyEncountered;
}

void SGXMLScanner::normalizeURI(const XMLCh* const systemURI,
                                XMLBuffer& normalizedURI)
{
    const XMLCh* pszSrc = systemURI;

    normalizedURI.reset();

    while (*pszSrc) {

        if ((*(pszSrc) == chPercent)
        &&  (*(pszSrc+1) == chDigit_2)
        &&  (*(pszSrc+2) == chDigit_0))
        {
            pszSrc += 3;
            normalizedURI.append(chSpace);
        }
        else if (*pszSrc == 0xFFFF) { //escaped character
            pszSrc++;
        }
        else {
            normalizedURI.append(*pszSrc);
            pszSrc++;
        }
    }
}

inline XMLAttDefList& getAttDefList(ComplexTypeInfo* currType, XMLElementDecl* elemDecl)
{
    if (currType)
        return currType->getAttDefList();
    else
        return elemDecl->getAttDefList();
}

void SGXMLScanner::endElementPSVI(SchemaElementDecl* const elemDecl,
                                  DatatypeValidator* const memberDV)
{
    PSVIElement::ASSESSMENT_TYPE validationAttempted;
    PSVIElement::VALIDITY_STATE validity = PSVIElement::VALIDITY_NOTKNOWN;

    if (fPSVIElemContext.fElemDepth > fPSVIElemContext.fFullValidationDepth)
        validationAttempted = PSVIElement::VALIDATION_FULL;
    else if (fPSVIElemContext.fElemDepth > fPSVIElemContext.fNoneValidationDepth)
        validationAttempted = PSVIElement::VALIDATION_NONE;
    else
    {
        validationAttempted  = PSVIElement::VALIDATION_PARTIAL;
		fPSVIElemContext.fFullValidationDepth =
            fPSVIElemContext.fNoneValidationDepth = fPSVIElemContext.fElemDepth - 1;
    }

    if (fValidate && elemDecl->isDeclared())
    {
        validity = (fPSVIElemContext.fErrorOccurred)
            ? PSVIElement::VALIDITY_INVALID : PSVIElement::VALIDITY_VALID;
    }

    XSTypeDefinition* typeDef = 0;
    bool isMixed = false;
    if (fPSVIElemContext.fCurrentTypeInfo)
    {
        typeDef = (XSTypeDefinition*) fModel->getXSObject(fPSVIElemContext.fCurrentTypeInfo);
        SchemaElementDecl::ModelTypes modelType = (SchemaElementDecl::ModelTypes)fPSVIElemContext.fCurrentTypeInfo->getContentType();
        isMixed = (modelType == SchemaElementDecl::Mixed_Simple
                || modelType == SchemaElementDecl::Mixed_Complex);
    }
    else if (fPSVIElemContext.fCurrentDV)
        typeDef = (XSTypeDefinition*) fModel->getXSObject(fPSVIElemContext.fCurrentDV);

    XMLCh* canonicalValue = 0;
    if (fPSVIElemContext.fNormalizedValue && !isMixed &&
            validity == PSVIElement::VALIDITY_VALID)
    {
        if (memberDV)
            canonicalValue = (XMLCh*) memberDV->getCanonicalRepresentation(fPSVIElemContext.fNormalizedValue, fMemoryManager);
        else if (fPSVIElemContext.fCurrentDV)
            canonicalValue = (XMLCh*) fPSVIElemContext.fCurrentDV->getCanonicalRepresentation(fPSVIElemContext.fNormalizedValue, fMemoryManager);
    }

    fPSVIElement->reset
    (
        validity
        , validationAttempted
        , fRootElemName
        , fPSVIElemContext.fIsSpecified
        , (elemDecl->isDeclared()) 
            ? (XSElementDeclaration*) fModel->getXSObject(elemDecl) : 0
        , typeDef
        , (memberDV) ? (XSSimpleTypeDefinition*) fModel->getXSObject(memberDV) : 0
        , fModel
        , elemDecl->getDefaultValue()
        , fPSVIElemContext.fNormalizedValue
        , canonicalValue
    );

    fPSVIHandler->handleElementPSVI
    (
        elemDecl->getBaseName()
        , fURIStringPool->getValueForId(elemDecl->getURI())
        , fPSVIElement
    );

    // decrease element depth
    fPSVIElemContext.fElemDepth--;

}

void SGXMLScanner::resetPSVIElemContext()
{
    fPSVIElemContext.fIsSpecified = false;
    fPSVIElemContext.fErrorOccurred = false;
    fPSVIElemContext.fElemDepth = -1;
    fPSVIElemContext.fFullValidationDepth = -1;
    fPSVIElemContext.fNoneValidationDepth = -1;
    fPSVIElemContext.fCurrentDV = 0;
    fPSVIElemContext.fCurrentTypeInfo = 0;
    fPSVIElemContext.fNormalizedValue = 0;
}

XERCES_CPP_NAMESPACE_END
