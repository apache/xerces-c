/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 2002 The Apache Software Foundation.  All rights
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
#include <xercesc/internal/IGXMLScanner.hpp>
#include <xercesc/util/RuntimeException.hpp>
#include <xercesc/util/HashPtr.hpp>
#include <xercesc/util/UnexpectedEOFException.hpp>
#include <xercesc/sax/InputSource.hpp>
#include <xercesc/framework/XMLDocumentHandler.hpp>
#include <xercesc/framework/XMLEntityHandler.hpp>
#include <xercesc/framework/XMLPScanToken.hpp>
#include <xercesc/internal/EndOfEntityException.hpp>
#include <xercesc/framework/MemoryManager.hpp>
#include <xercesc/framework/XMLGrammarPool.hpp>
#include <xercesc/framework/XMLDTDDescription.hpp>
#include <xercesc/framework/psvi/PSVIHandler.hpp>
#include <xercesc/framework/psvi/PSVIAttributeList.hpp>
#include <xercesc/validators/common/GrammarResolver.hpp>
#include <xercesc/validators/DTD/DocTypeHandler.hpp>
#include <xercesc/validators/DTD/DTDScanner.hpp>
#include <xercesc/validators/DTD/DTDValidator.hpp>
#include <xercesc/validators/schema/SchemaValidator.hpp>
#include <xercesc/validators/schema/identity/FieldActivator.hpp>
#include <xercesc/validators/schema/identity/XPathMatcherStack.hpp>
#include <xercesc/validators/schema/identity/ValueStoreCache.hpp>
#include <xercesc/validators/schema/identity/IC_Selector.hpp>
#include <xercesc/validators/schema/identity/ValueStore.hpp>
#include <xercesc/util/OutOfMemoryException.hpp>

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  IGXMLScanner: Constructors and Destructor
// ---------------------------------------------------------------------------
IGXMLScanner::IGXMLScanner( XMLValidator* const  valToAdopt
                          , GrammarResolver* const grammarResolver
                          , MemoryManager* const manager) :

    XMLScanner(valToAdopt, grammarResolver, manager)
    , fSeeXsi(false)
    , fGrammarType(Grammar::UnKnown)
    , fElemStateSize(16)
    , fElemState(0)
    , fContent(1023, manager)
    , fRawAttrList(0)
    , fDTDValidator(0)
    , fSchemaValidator(0)
    , fDTDGrammar(0)
    , fMatcherStack(0)
    , fValueStoreCache(0)
    , fFieldActivator(0)
    , fLocationPairs(0)
    , fDTDElemNonDeclPool(0)
    , fSchemaElemNonDeclPool(0)
    , fElemCount(0)
    , fAttDefRegistry(0)
    , fUndeclaredAttrRegistry(0)
    , fUndeclaredAttrRegistryNS(0)
    , fPSVIAttrList(0)
    , fModel(0)
    , fPSVIElement(0)
    , fErrorStack(0)        
{
    try
    {
         commonInit();

         // use fDTDValidator as the default validator
         if (!valToAdopt)
             fValidator = fDTDValidator;
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

IGXMLScanner::IGXMLScanner( XMLDocumentHandler* const docHandler
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
    , fRawAttrList(0)
    , fDTDValidator(0)
    , fSchemaValidator(0)
    , fDTDGrammar(0)
    , fMatcherStack(0)
    , fValueStoreCache(0)
    , fFieldActivator(0)
    , fLocationPairs(0)
    , fDTDElemNonDeclPool(0)
    , fSchemaElemNonDeclPool(0)
    , fElemCount(0)
    , fAttDefRegistry(0)
    , fUndeclaredAttrRegistry(0)
    , fUndeclaredAttrRegistryNS(0)
    , fPSVIAttrList(0)
    , fModel(0)
    , fPSVIElement(0)
    , fErrorStack(0)       
{
    try
    {	
        commonInit();

        //use fDTDValidator as the default validator
        if (!valToAdopt)
            fValidator = fDTDValidator;
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

IGXMLScanner::~IGXMLScanner()
{
    cleanUp();
}

// ---------------------------------------------------------------------------
//  XMLScanner: Getter methods
// ---------------------------------------------------------------------------
NameIdPool<DTDEntityDecl>* IGXMLScanner::getEntityDeclPool()
{
    if(!fDTDGrammar)
        return 0;
    return fDTDGrammar->getEntityDeclPool();
}

const NameIdPool<DTDEntityDecl>* IGXMLScanner::getEntityDeclPool() const
{
    if(!fDTDGrammar)
        return 0;
    return fDTDGrammar->getEntityDeclPool();
}

// ---------------------------------------------------------------------------
//  IGXMLScanner: Main entry point to scan a document
// ---------------------------------------------------------------------------
void IGXMLScanner::scanDocument(const InputSource& src)
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

        //cargill debug:
        //fGrammarResolver->getXSModel();

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


bool IGXMLScanner::scanNext(XMLPScanToken& token)
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
                    if (fDoNamespaces)
                        scanStartTagNS(gotData);
                    else
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
//  IGXMLScanner: Private helper methods. Most of these are implemented in
//  IGXMLScanner2.Cpp.
// ---------------------------------------------------------------------------

//  This method handles the common initialization, to avoid having to do
//  it redundantly in multiple constructors.
void IGXMLScanner::commonInit()
{

    //  Create the element state array
    fElemState = (unsigned int*) fMemoryManager->allocate
    (
        fElemStateSize * sizeof(unsigned int)
    ); //new unsigned int[fElemStateSize];

    //  And we need one for the raw attribute scan. This just stores key/
    //  value string pairs (prior to any processing.)
    fRawAttrList = new (fMemoryManager) RefVectorOf<KVStringPair>(32, true, fMemoryManager);

    //  Create the Validator and init them
    fDTDValidator = new (fMemoryManager) DTDValidator();
    initValidator(fDTDValidator);
    fSchemaValidator = new (fMemoryManager) SchemaValidator(0, fMemoryManager);
    initValidator(fSchemaValidator);

    // Create IdentityConstraint info
    fMatcherStack = new (fMemoryManager) XPathMatcherStack(fMemoryManager);
    fValueStoreCache = new (fMemoryManager) ValueStoreCache(fMemoryManager);
    fFieldActivator = new (fMemoryManager) FieldActivator(fValueStoreCache, fMatcherStack, fMemoryManager);
    fValueStoreCache->setScanner(this);

    // Create schemaLocation pair info
    fLocationPairs = new (fMemoryManager) ValueVectorOf<XMLCh*>(8, fMemoryManager);
    // create pools for undeclared elements
    fDTDElemNonDeclPool = new (fMemoryManager) NameIdPool<DTDElementDecl>(29, 128, fMemoryManager);
    fSchemaElemNonDeclPool = new (fMemoryManager) RefHash3KeysIdPool<SchemaElementDecl>(29, true, 128, fMemoryManager); 
    fAttDefRegistry = new (fMemoryManager) RefHashTableOf<unsigned int>
    (
        131, false, new (fMemoryManager)HashPtr(), fMemoryManager
    );
    fUndeclaredAttrRegistry = new (fMemoryManager) RefHashTableOf<unsigned int>
    (
        7, false, new (fMemoryManager)HashXMLCh(), fMemoryManager
    );
    fUndeclaredAttrRegistryNS = new (fMemoryManager) RefHash2KeysTableOf<unsigned int>
    (
        7, false, new (fMemoryManager)HashXMLCh(), fMemoryManager
    );
    fPSVIAttrList = new (fMemoryManager) PSVIAttributeList(fMemoryManager);
}

void IGXMLScanner::cleanUp()
{
    fMemoryManager->deallocate(fElemState); //delete [] fElemState;
    delete fRawAttrList;
    delete fDTDValidator;
    delete fSchemaValidator;
    delete fFieldActivator;
    delete fMatcherStack;
    delete fValueStoreCache;
    delete fLocationPairs;
    delete fDTDElemNonDeclPool;
    delete fSchemaElemNonDeclPool;
    delete fAttDefRegistry;
    delete fUndeclaredAttrRegistry;
    delete fUndeclaredAttrRegistryNS;
    delete fPSVIAttrList;
    delete fPSVIElement;
    delete fErrorStack;
}

// ---------------------------------------------------------------------------
//  IGXMLScanner: Private scanning methods
// ---------------------------------------------------------------------------

//  This method is called from scanStartTag() to handle the very raw initial
//  scan of the attributes. It just fills in the passed collection with
//  key/value pairs for each attribute. No processing is done on them at all.
unsigned int
IGXMLScanner::rawAttrScan(const   XMLCh* const                elemName
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
bool IGXMLScanner::scanContent()
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
                        if (fDoNamespaces)
                            scanStartTagNS(gotData);
                        else
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


void IGXMLScanner::scanEndTag(bool& gotData)
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
        //REVISIT: Do we restore PSVI information?
        return;
    }

    unsigned int uriId = fEmptyNamespaceId;
    int prefixColonPos = -1;
    if (fDoNamespaces)
    {
        uriId = resolveQName
        (
            fQNameBuf.getRawBuffer()
            , fPrefixBuf
            , ElemStack::Mode_Element
            , prefixColonPos
        );
    }

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
    if (fDoNamespaces && fGrammarType == Grammar::SchemaGrammarType) {

        // reset error occurred
        fPSVIElemContext.fErrorOccurred = fErrorStack->pop();
        const XMLCh* rawNameBuf = fQNameBuf.getRawBuffer();
        if ((topUri != uriId) || (!XMLString::equals(tempElement->getBaseName(), &rawNameBuf[prefixColonPos + 1])))
        {
            emitError
            (
                XMLErrs::ExpectedEndOfTagX
                , topElem->fThisElement->getFullName()
            );
        }
    }
    else {
        if (!XMLString::equals(tempElement->getFullName(), fQNameBuf.getRawBuffer()))
        {
            emitError
            (
                XMLErrs::ExpectedEndOfTagX
                , topElem->fThisElement->getFullName()
            );
        }
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

    if (fGrammarType == Grammar::SchemaGrammarType)
    {
        if (fValidate && topElem->fThisElement->isDeclared())
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


        if (fGrammarType == Grammar::SchemaGrammarType) {
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
    }

    if (fGrammarType == Grammar::SchemaGrammarType)
    {
        if(!isRoot)
            ((SchemaElementDecl *)fElemStack.topElement()->fThisElement)->updateValidityFromElement(topElem->fThisElement, fGrammarType);

        if (fPSVIHandler)
        {
            endElementPSVI(
                (SchemaElementDecl*)topElem->fThisElement, psviMemberType);
        }
        // now we can reset the datatype buffer, since the 
        // application has had a chance to copy the characters somewhere else
        ((SchemaValidator *)fValidator)->clearDatatypeBuffer();
    }

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
        if(fGrammarType == Grammar::SchemaGrammarType)
        {
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
        else 
        {
            fDocHandler->elementTypeInfo(0, 0); 
        }
    }

    // reset xsi:type ComplexTypeInfo
    if (fGrammarType == Grammar::SchemaGrammarType) {        
        ((SchemaElementDecl*)topElem->fThisElement)->reset();
        if (!isRoot)
        {
            ((SchemaElementDecl*)(fElemStack.topElement()->fThisElement))->
                setXsiComplexTypeInfo(((SchemaValidator*)fValidator)->getCurrentTypeInfo());
            // update error information
            fErrorStack->push(fErrorStack->pop() || fPSVIElemContext.fErrorOccurred);
        }
    }

    // If this was the root, then done with content
    gotData = !isRoot;

    if (gotData) {
        if (fDoNamespaces) {
            // Restore the grammar
            fGrammar = fElemStack.getCurrentGrammar();
            fGrammarType = fGrammar->getGrammarType();
            if (fGrammarType == Grammar::SchemaGrammarType && !fValidator->handlesSchema()) {
                if (fValidatorFromUser)
                    ThrowXMLwithMemMgr(RuntimeException, XMLExcepts::Gen_NoSchemaValidator, fMemoryManager);
                else {
                    fValidator = fSchemaValidator;
                }
            }
            else if (fGrammarType == Grammar::DTDGrammarType && !fValidator->handlesDTD()) {
                if (fValidatorFromUser)
                    ThrowXMLwithMemMgr(RuntimeException, XMLExcepts::Gen_NoDTDValidator, fMemoryManager);
                else {
                    fValidator = fDTDValidator;
                }
            }

            fValidator->setGrammar(fGrammar);
        }

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
void IGXMLScanner::scanDocTypeDecl()
{
    //  We have a doc type. So, switch the Grammar.
    switchGrammar(XMLUni::fgDTDEntityString);

    if (fDocTypeHandler)
        fDocTypeHandler->resetDocType();

    // There must be some space after DOCTYPE
    if (!fReaderMgr.skipPastSpaces())
    {
        emitError(XMLErrs::ExpectedWhitespace);

        // Just skip the Doctype declaration and return
        fReaderMgr.skipPastChar(chCloseAngle);
        return;
    }

    // Get a buffer for the root element
    XMLBufBid bbRootName(&fBufMgr);

    //  Get a name from the input, which should be the name of the root
    //  element of the upcoming content.
    fReaderMgr.getName(bbRootName.getBuffer());
    if (bbRootName.isEmpty())
    {
        emitError(XMLErrs::NoRootElemInDOCTYPE);
        fReaderMgr.skipPastChar(chCloseAngle);
        return;
    }

    //  Store the root element name for later check
    setRootElemName(bbRootName.getRawBuffer());

    //  This element obviously is not going to exist in the element decl
    //  pool yet, but we need to call docTypeDecl. So force it into
    //  the element decl pool, marked as being there because it was in
    //  the DOCTYPE. Later, when its declared, the status will be updated.
    //
    //  Only do this if we are not reusing the validator! If we are reusing,
    //  then look it up instead. It has to exist!
    DTDElementDecl* rootDecl = new (fGrammarPoolMemoryManager) DTDElementDecl
    (
        bbRootName.getRawBuffer()
        , fEmptyNamespaceId
        , DTDElementDecl::Any
        , fGrammarPoolMemoryManager
    ); 
    rootDecl->setCreateReason(DTDElementDecl::AsRootElem);
    rootDecl->setExternalElemDeclaration(true);
    if(!fUseCachedGrammar) 
    {
        // this will break getRootElemId on DTDGrammar when
        // cached grammars are in use, but 
        // why would one use this anyway???
        try {
            ((DTDGrammar*)fGrammar)->setRootElemId(fGrammar->putElemDecl(rootDecl));
        }
        catch(const XMLException&)
        {
            delete rootDecl;
            throw;
        }
    } else
    {
        // attach this to the undeclared element pool so that it gets deleted
        XMLElementDecl* elemDecl = fDTDElemNonDeclPool->getByKey(bbRootName.getRawBuffer());
        if (elemDecl)
        {
            rootDecl->setId(elemDecl->getId());
        }
        else
        {
            rootDecl->setId(fDTDElemNonDeclPool->put((DTDElementDecl*)rootDecl));
        }
    }

    // Skip any spaces after the name
    fReaderMgr.skipPastSpaces();

    //  And now if we are looking at a >, then we are done. It is not
    //  required to have an internal or external subset, though why you
    //  would not escapes me.
    if (fReaderMgr.skippedChar(chCloseAngle)) {

        //  If we have a doc type handler and advanced callbacks are enabled,
        //  call the doctype event.
        if (fDocTypeHandler)
            fDocTypeHandler->doctypeDecl(*rootDecl, 0, 0, false);
        return;
    }

    // either internal/external subset
    if (fValScheme == Val_Auto && !fValidate)
        fValidate = true;

    bool    hasIntSubset = false;
    bool    hasExtSubset = false;
    XMLCh*  sysId = 0;
    XMLCh*  pubId = 0;

    DTDScanner dtdScanner
    (
        (DTDGrammar*) fGrammar
        , fDocTypeHandler
        , fGrammarPoolMemoryManager
        , fMemoryManager
    );
    dtdScanner.setScannerInfo(this, &fReaderMgr, &fBufMgr);

    //  If the next character is '[' then we have no external subset cause
    //  there is no system id, just the opening character of the internal
    //  subset. Else, has to be an id.
    //
    // Just look at the next char, don't eat it.
    if (fReaderMgr.peekNextChar() == chOpenSquare)
    {
        hasIntSubset = true;
    }
    else
    {
        // Indicate we have an external subset
        hasExtSubset = true;
        fHasNoDTD = false;

        // Get buffers for the ids
        XMLBufBid bbPubId(&fBufMgr);
        XMLBufBid bbSysId(&fBufMgr);

        // Get the external subset id
        if (!dtdScanner.scanId(bbPubId.getBuffer(), bbSysId.getBuffer(), DTDScanner::IDType_External))
        {
            fReaderMgr.skipPastChar(chCloseAngle);
            return;
        }

        // Get copies of the ids we got
        pubId = XMLString::replicate(bbPubId.getRawBuffer(), fMemoryManager);
        sysId = XMLString::replicate(bbSysId.getRawBuffer(), fMemoryManager);

        // Skip spaces and check again for the opening of an internal subset
        fReaderMgr.skipPastSpaces();

        // Just look at the next char, don't eat it.
        if (fReaderMgr.peekNextChar() == chOpenSquare) {
            hasIntSubset = true;
        }
    }

    // Insure that the ids get cleaned up, if they got allocated
    ArrayJanitor<XMLCh> janSysId(sysId, fMemoryManager);
    ArrayJanitor<XMLCh> janPubId(pubId, fMemoryManager);

    //  If we have a doc type handler and advanced callbacks are enabled,
    //  call the doctype event.
    if (fDocTypeHandler)
        fDocTypeHandler->doctypeDecl(*rootDecl, pubId, sysId, hasIntSubset, hasExtSubset);

    //  Ok, if we had an internal subset, we are just past the [ character
    //  and need to parse that first.
    if (hasIntSubset)
    {
        // Eat the opening square bracket
        fReaderMgr.getNextChar();

        // We can't have any internal subset if we are reusing the validator
        if (fUseCachedGrammar || fToCacheGrammar)
            ThrowXMLwithMemMgr(RuntimeException, XMLExcepts::Val_CantHaveIntSS, fMemoryManager);

        //  And try to scan the internal subset. If we fail, try to recover
        //  by skipping forward tot he close angle and returning.
        if (!dtdScanner.scanInternalSubset())
        {
            fReaderMgr.skipPastChar(chCloseAngle);
            return;
        }

        //  Do a sanity check that some expanded PE did not propogate out of
        //  the doctype. This could happen if it was terminated early by bad
        //  syntax.
        if (fReaderMgr.getReaderDepth() > 1)
        {
            emitError(XMLErrs::PEPropogated);

            // Ask the reader manager to pop back down to the main level
            fReaderMgr.cleanStackBackTo(1);
        }

        fReaderMgr.skipPastSpaces();
    }

    // And that should leave us at the closing > of the DOCTYPE line
    if (!fReaderMgr.skippedChar(chCloseAngle))
    {
        //  Do a special check for the common scenario of an extra ] char at
        //  the end. This is easy to recover from.
        if (fReaderMgr.skippedChar(chCloseSquare)
        &&  fReaderMgr.skippedChar(chCloseAngle))
        {
            emitError(XMLErrs::ExtraCloseSquare);
        }
         else
        {
            emitError(XMLErrs::UnterminatedDOCTYPE);
            fReaderMgr.skipPastChar(chCloseAngle);
        }
    }

    //  If we had an external subset, then we need to deal with that one
    //  next. If we are reusing the validator, then don't scan it.
    if (hasExtSubset) {

        if (fUseCachedGrammar)
        {
            InputSource* sysIdSrc = resolveSystemId(sysId);
            Janitor<InputSource> janSysIdSrc(sysIdSrc);
            Grammar* grammar = fGrammarResolver->getGrammar(sysIdSrc->getSystemId());

            if (grammar && grammar->getGrammarType() == Grammar::DTDGrammarType) {

                fDTDGrammar = (DTDGrammar*) grammar;
                fGrammar = fDTDGrammar;
                fValidator->setGrammar(fGrammar);
                // should not be modifying cached grammars!
                /********
                rootDecl = (DTDElementDecl*) fGrammar->getElemDecl(fEmptyNamespaceId, 0, bbRootName.getRawBuffer(), Grammar::TOP_LEVEL_SCOPE);

                if (rootDecl)
                    ((DTDGrammar*)fGrammar)->setRootElemId(rootDecl->getId());
                else {
                    rootDecl = new (fGrammarPoolMemoryManager) DTDElementDecl
                    (
                        bbRootName.getRawBuffer()
                        , fEmptyNamespaceId
                        , DTDElementDecl::Any
                        , fGrammarPoolMemoryManager
                    );
                    rootDecl->setCreateReason(DTDElementDecl::AsRootElem);
                    rootDecl->setExternalElemDeclaration(true);
                    ((DTDGrammar*)fGrammar)->setRootElemId(fGrammar->putElemDecl(rootDecl));
                }
                ********/

                return;
            }
        }

        if (fLoadExternalDTD || fValidate)
        {
            // And now create a reader to read this entity
            InputSource* srcUsed;
            XMLReader* reader = fReaderMgr.createReader
            (
                sysId
                , pubId
                , false
                , XMLReader::RefFrom_NonLiteral
                , XMLReader::Type_General
                , XMLReader::Source_External
                , srcUsed
                , fCalculateSrcOfs
            );

            // Put a janitor on the input source
            Janitor<InputSource> janSrc(srcUsed);

            //  If it failed then throw an exception
            if (!reader)
                ThrowXMLwithMemMgr1(RuntimeException, XMLExcepts::Gen_CouldNotOpenDTD, srcUsed->getSystemId(), fMemoryManager);

            if (fToCacheGrammar) {

                unsigned int stringId = fGrammarResolver->getStringPool()->addOrFind(srcUsed->getSystemId());
                const XMLCh* sysIdStr = fGrammarResolver->getStringPool()->getValueForId(stringId);

                fGrammarResolver->orphanGrammar(XMLUni::fgDTDEntityString);
                ((XMLDTDDescription*) (fGrammar->getGrammarDescription()))->setRootName(sysIdStr);
                fGrammarResolver->putGrammar(fGrammar);
            }

            //  In order to make the processing work consistently, we have to
            //  make this look like an external entity. So create an entity
            //  decl and fill it in and push it with the reader, as happens
            //  with an external entity. Put a janitor on it to insure it gets
            //  cleaned up. The reader manager does not adopt them.
            const XMLCh gDTDStr[] = { chLatin_D, chLatin_T, chLatin_D , chNull };
            DTDEntityDecl* declDTD = new (fGrammarPoolMemoryManager) DTDEntityDecl(gDTDStr, false, fGrammarPoolMemoryManager);
            declDTD->setSystemId(sysId);
            Janitor<DTDEntityDecl> janDecl(declDTD);

            // Mark this one as a throw at end
            reader->setThrowAtEnd(true);

            // And push it onto the stack, with its pseudo name
            fReaderMgr.pushReader(reader, declDTD);

            // Tell it its not in an include section
            dtdScanner.scanExtSubsetDecl(false, true);
        }
    }
}

bool IGXMLScanner::scanStartTag(bool& gotData)
{
    //  Assume we will still have data until proven otherwise. It will only
    //  ever be false if this is the root and its empty.
    gotData = true;

    //  Get the QName. In this case, we are not doing namespaces, so we just
    //  use it as is and don't have to break it into parts.
    if (!fReaderMgr.getName(fQNameBuf))
    {
        emitError(XMLErrs::ExpectedElementName);
        fReaderMgr.skipToChar(chOpenAngle);
        return false;
    }

    // Assume it won't be an empty tag
    bool isEmpty = false;

    //  Lets try to look up the element in the validator's element decl pool
    //  We can pass bogus values for the URI id and the base name. We know that
    //  this can only be called if we are doing a DTD style validator and that
    //  he will only look at the QName.
    //
    //  We tell him to fault in a decl if he does not find one.
    //  Actually, we *don't* tell him to fault in a decl if he does not find one- NG
    bool wasAdded = false;
    const XMLCh *rawQName = fQNameBuf.getRawBuffer();
    XMLElementDecl* elemDecl = fGrammar->getElemDecl
    (
        fEmptyNamespaceId
        , 0
        , rawQName
        , Grammar::TOP_LEVEL_SCOPE
    );
    // look for it in the undeclared pool:
    if(!elemDecl) 
    {
        elemDecl = fDTDElemNonDeclPool->getByKey(rawQName);
    }
    if(!elemDecl) 
    {
        // we're assuming this must be a DTD element.  DTD's can be
        // used with or without namespaces, but schemas cannot be used without
        // namespaces.
        wasAdded = true;
        elemDecl = new (fMemoryManager) DTDElementDecl 
        (
            rawQName
            , fEmptyNamespaceId
            , DTDElementDecl::Any
            , fMemoryManager
        );
        elemDecl->setId(fDTDElemNonDeclPool->put((DTDElementDecl*)elemDecl));
    }

    //  We do something different here according to whether we found the
    //  element or not.
    if (wasAdded)
    {
        // If validating then emit an error
        if (fValidate)
        {
            // This is to tell the reuse Validator that this element was
            // faulted-in, was not an element in the validator pool originally
            elemDecl->setCreateReason(XMLElementDecl::JustFaultIn);

            fValidator->emitError
            (
                XMLValid::ElementNotDefined
                , elemDecl->getFullName()
            );
        }
    }
    else
    {
        // If its not marked declared and validating, then emit an error
        if (fValidate && !elemDecl->isDeclared())
        {
            fValidator->emitError
            (
                XMLValid::ElementNotDefined
                , elemDecl->getFullName()
            );
        }
    }

    // See if its the root element
    const bool isRoot = fElemStack.isEmpty();

    // Expand the element stack and add the new element
    fElemStack.addLevel(elemDecl, fReaderMgr.getCurrentReaderNum());
    fElemStack.setValidationFlag(fValidate);

    //  Validate the element
    if (fValidate)
        fValidator->validateElement(elemDecl);

    //  If this is the first element and we are validating, check the root
    //  element.
    if (isRoot)
    {
        fRootGrammar = fGrammar;

        if (fValidate)
        {
            //  If a DocType exists, then check if it matches the root name there.
            if (fRootElemName && !XMLString::equals(fQNameBuf.getRawBuffer(), fRootElemName))
                fValidator->emitError(XMLValid::RootElemNotLikeDocType);

            //  Some validators may also want to check the root, call the
            //  XMLValidator::checkRootElement
            if (fValidatorFromUser && !fValidator->checkRootElement(elemDecl->getId()))
                fValidator->emitError(XMLValid::RootElemNotLikeDocType);
        }
    }
    else
    {
        //  If the element stack is not empty, then add this element as a
        //  child of the previous top element. If its empty, this is the root
        //  elem and is not the child of anything.
        fElemStack.addChild(elemDecl->getElementName(), true);
    }

    //  Ask the element decl to clear out the 'provided' flag on all of its
    //  att defs.
    elemDecl->resetDefs();

    // Skip any whitespace after the name
    fReaderMgr.skipPastSpaces();

    //  We loop until we either see a /> or >, handling attribute/value
    //  pairs until we get there.
    unsigned int    attCount = 0;
    unsigned int    curAttListSize = fAttrList->size();
    wasAdded = false;
    // clear the map used to detect duplicate attributes
    fUndeclaredAttrRegistry->removeAll();
    fUndeclaredAttrRegistryNS->removeAll();

    fElemCount++;

    while (true)
    {
        // And get the next non-space character
        XMLCh nextCh = fReaderMgr.peekNextChar();

        //  If the next character is not a slash or closed angle bracket,
        //  then it must be whitespace, since whitespace is required
        //  between the end of the last attribute and the name of the next
        //  one.
        if (attCount)
        {
            if ((nextCh != chForwardSlash) && (nextCh != chCloseAngle))
            {
                if (fReaderMgr.getCurrentReader()->isWhitespace(nextCh))
                {
                    // Ok, skip by them and peek another char
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
                return false;
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
                    emitError(XMLErrs::UnterminatedStartTag, elemDecl->getFullName());
                    return false;
                }
                else
                {
                    // Something went really wrong
                    return false;
                }
            }
            //  See if this attribute is declared for this element. If we are
            //  not validating of course it will not be at first, but we will
            //  fault it into the pool (to avoid lots of redundant errors.)
            XMLCh * namePtr = fAttNameBuf.getRawBuffer();
            XMLAttDef* attDef = ((DTDElementDecl *)elemDecl)->getAttDef(namePtr);

            //  Add this attribute to the attribute list that we use to
            //  pass them to the handler. We reuse its existing elements
            //  but expand it as required.
            // Note that we want to this first since this will
            // make a copy of the namePtr; we can then make use of
            // that copy in the hashtable lookup that checks
            // for duplicates.  This will mean we may have to update
            // the type of the XMLAttr later.
            XMLAttr* curAtt;
            if (attCount >= curAttListSize)
            {
                curAtt = new (fMemoryManager) XMLAttr
                (
                    -1
                    , namePtr
                    , XMLUni::fgZeroLenString
                    , XMLUni::fgZeroLenString
                    , (attDef)?attDef->getType():XMLAttDef::CData
                    , true
                    , fMemoryManager
                );
                fAttrList->addElement(curAtt);
            }
            else
            {
                curAtt = fAttrList->elementAt(attCount);
                curAtt->set
                (
                    -1
                    , namePtr
                    , XMLUni::fgZeroLenString
                    , XMLUni::fgZeroLenString
                    , (attDef)?attDef->getType():XMLAttDef::CData
                );
                curAtt->setSpecified(true);
            }
            // reset namePtr so it refers to newly-allocated memory
            namePtr = (XMLCh *)curAtt->getName();

            if (!attDef)
            {
                //  If there is a validation handler, then we are validating
                //  so emit an error.
                if (fValidate)
                { 
                    fValidator->emitError
                    (
                        XMLValid::AttNotDefinedForElement
                        , fAttNameBuf.getRawBuffer()
                        , elemDecl->getFullName()
                    );
                }
                unsigned int *curCountPtr = fUndeclaredAttrRegistry->get(namePtr);
                if(!curCountPtr)
                {
                    curCountPtr = getNewUIntPtr();
                     *curCountPtr = fElemCount;
                    fUndeclaredAttrRegistry->put((void *)namePtr, curCountPtr);
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
                }
            }
            else
            {
                // prepare for duplicate detection
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
                }
            }

            //  Skip any whitespace before the value and then scan the att
            //  value. This will come back normalized with entity refs and
            //  char refs expanded.
            fReaderMgr.skipPastSpaces();
            if (!scanAttValue(attDef, namePtr, fAttValueBuf))
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
                    emitError(XMLErrs::UnterminatedStartTag, elemDecl->getFullName());
                    return false;
                }
                else
                {
                    // Something went really wrong
                    return false;
                }
            }
            // must set the newly-minted value on the XMLAttr:
            curAtt->setValue(fAttValueBuf.getRawBuffer());

            //  Now that its all stretched out, lets look at its type and
            //  determine if it has a valid value. It will output any needed
            //  errors, but we just keep going. We only need to do this if
            //  we are validating.
            if (attDef)
            {
                // Let the validator pass judgement on the attribute value
                if (fValidate)
                {
                    fValidator->validateAttrValue
                    (
                        attDef
                        , fAttValueBuf.getRawBuffer()
                        , false
                        , elemDecl
                    );
                }
            }

            attCount++;
            // And jump back to the top of the loop
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
                emitError(XMLErrs::UnterminatedStartTag, elemDecl->getFullName());
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
            emitError(XMLErrs::UnterminatedStartTag, elemDecl->getFullName());
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

    //  Ok, so lets get an enumerator for the attributes of this element
    //  and run through them for well formedness and validity checks. But
    //  make sure that we had any attributes before we do it, since the list
    //  would have have gotten faulted in anyway.
    if (elemDecl->hasAttDefs())
    {
        // N.B.:  this assumes DTD validation.
        XMLAttDefList& attDefList = elemDecl->getAttDefList();
        for(unsigned int i=0; i<attDefList.getAttDefCount(); i++)
        {
            // Get the current att def, for convenience and its def type
            const XMLAttDef& curDef = attDefList.getAttDef(i);
            const XMLAttDef::DefAttTypes defType = curDef.getDefaultType();

            unsigned int *attCountPtr = fAttDefRegistry->get(&curDef);
            if (!attCountPtr || *attCountPtr < fElemCount)
            { // did not occur
                if (fValidate)
                {
                    // If we are validating and its required, then an error
                    if (defType == XMLAttDef::Required)
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
                            // XML 1.0 Section 2.9
                            // Document is standalone, so attributes must not be defaulted.
                            fValidator->emitError(XMLValid::NoDefAttForStandalone, curDef.getFullName(), elemDecl->getFullName());

                        }
                    }
                }

                // Fault in the value if needed, and bump the att count
                if ((defType == XMLAttDef::Default)
                ||  (defType == XMLAttDef::Fixed))
                {
                    // Let the validator pass judgement on the attribute value
                    if (fValidate)
                    {
                        fValidator->validateAttrValue
                        (
                            &curDef
                            , curDef.getValue()
                            , false
                            , elemDecl
                        );
                    }

                    XMLAttr* curAtt;
                    if (attCount >= curAttListSize)
                    {
                        curAtt = new (fMemoryManager) XMLAttr
                        (
                            -1
                            , curDef.getFullName()
                            , XMLUni::fgZeroLenString
                            , curDef.getValue()
                            , curDef.getType()
                            , false
                            , fMemoryManager
                        );
                        fAttrList->addElement(curAtt);
                        curAttListSize++;
                    }
                    else
                    {
                        curAtt = fAttrList->elementAt(attCount);
                        curAtt->set
                        (
                            -1
                            , curDef.getFullName()
                            , XMLUni::fgZeroLenString
                            , curDef.getValue()
                            , curDef.getType()
                        );
                        curAtt->setSpecified(false);
                    }
                    attCount++;
                }
            }
        }
    }

    //  If empty, validate content right now if we are validating and then
    //  pop the element stack top. Else, we have to update the current stack
    //  top's namespace mapping elements.
    if (isEmpty)
    {
        // If validating, then insure that its legal to have no content
        if (fValidate)
        {
            const int res = fValidator->checkContent(elemDecl, 0, 0);
            if (res >= 0)
            {
                fValidator->emitError
                (
                    XMLValid::ElementNotValidForContent
                    , elemDecl->getFullName()
                    , elemDecl->getFormattedContentModel()
                );
            }
        }

        // Pop the element stack back off since it'll never be used now
        fElemStack.popTop();

        // If the elem stack is empty, then it was an empty root
        if (isRoot)
            gotData = false;
        else {
            // Restore the validation flag
            fValidate = fElemStack.getValidationFlag();
        }
    }

    //  If we have a document handler, then tell it about this start tag. We
    //  don't have any URI id to send along, so send fEmptyNamespaceId. We also do not send
    //  any prefix since its just one big name if we are not doing namespaces.
    if (fDocHandler)
    {
        fDocHandler->startElement
        (
            *elemDecl
            , fEmptyNamespaceId
            , 0
            , *fAttrList
            , attCount
            , isEmpty
            , isRoot
        );
    }

    return true;
}


//  This method is called to scan a start tag when we are processing
//  namespaces. There are two different versions of this method, one for
//  namespace aware processing and one for non-namespace aware processing.
//
//  This method is called after we've scanned the < of a start tag. So we
//  have to get the element name, then scan the attributes, after which
//  we are either going to see >, />, or attributes followed by one of those
//  sequences.
bool IGXMLScanner::scanStartTagNS(bool& gotData)
{
    //  Assume we will still have data until proven otherwise. It will only
    //  ever be false if this is the root and its empty.
    gotData = true;

    // Reset element content buffer
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

    // clear the map used to detect duplicate attributes
    fUndeclaredAttrRegistry->removeAll();
    fUndeclaredAttrRegistryNS->removeAll();

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

    if (!isRoot && fGrammarType == Grammar::SchemaGrammarType)
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
        && fDoSchema
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

    //  Also find any default or fixed xmlns attributes in DTD defined for
    //  this element.
    XMLElementDecl* elemDecl = 0;
    if (fGrammarType == Grammar::DTDGrammarType) {
        const XMLCh *rawQName = fQNameBuf.getRawBuffer();
        elemDecl = fGrammar->getElemDecl
        (
            fEmptyNamespaceId
            , 0
            , rawQName
            , Grammar::TOP_LEVEL_SCOPE
        );
        // may have not been declared:
        if(!elemDecl)
            elemDecl = fDTDElemNonDeclPool->getByKey(rawQName);
        if (elemDecl) {
            if (elemDecl->hasAttDefs()) {
                XMLAttDefList& attDefList = elemDecl->getAttDefList();
                for(unsigned int i=0; i<attDefList.getAttDefCount(); i++)
                {
                    // Get the current att def, for convenience and its def type
                    const XMLAttDef& curDef = attDefList.getAttDef(i);
                    const XMLAttDef::DefAttTypes defType = curDef.getDefaultType();

                    // update the NSMap if there are any default/fixed xmlns attributes
                    if ((defType == XMLAttDef::Default)
                    ||  (defType == XMLAttDef::Fixed))
                    {
                        const XMLCh* rawPtr = curDef.getFullName();
                        if (!XMLString::compareNString(rawPtr, XMLUni::fgXMLNSColonString, 6)
                        ||  XMLString::equals(rawPtr, XMLUni::fgXMLNSString))
                            updateNSMap(rawPtr, curDef.getValue());
                    }
                }
            }
        }
    }

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
    bool wasAdded = false;
    bool errorBeforeElementFound = false;
    bool laxBeforeElementFound = false;

    const XMLCh* nameRawBuf = &qnameRawBuf[prefixColonPos + 1];
    const XMLCh* original_uriStr = fGrammar->getTargetNamespace();
    unsigned orgGrammarUri = fURIStringPool->getId(original_uriStr);

    // REVISIT:  since all this code only really
    // makes sense for schemas, why can DTD validation theoretically pass 
    // through it?  - NG
    if (uriId != fEmptyNamespaceId) {

        // Check in current grammar before switching if necessary
        const XMLCh *rawQName = fQNameBuf.getRawBuffer();
        elemDecl = fGrammar->getElemDecl
        (
          uriId
          , nameRawBuf
          , rawQName
          , currentScope
        );
        // may have not been declared; must look everywhere:
        if (!elemDecl)
            if(fGrammarType == Grammar::DTDGrammarType) 
            {
                // should never occur in practice
                elemDecl = fDTDElemNonDeclPool->getByKey(rawQName);
            }
            else if (fGrammarType == Grammar::SchemaGrammarType) 
            {
                elemDecl = fSchemaElemNonDeclPool->getByKey(nameRawBuf, uriId, currentScope);
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
                elemDecl = fSchemaElemNonDeclPool->getByKey(nameRawBuf, uriId, Grammar::TOP_LEVEL_SCOPE);
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
            if(fGrammarType == Grammar::DTDGrammarType) 
            {
                elemDecl = new (fMemoryManager) DTDElementDecl
                (
                    qnameRawBuf
                    , uriId
                    , DTDElementDecl::Any
                    , fMemoryManager
                );
                elemDecl->setId(fDTDElemNonDeclPool->put((DTDElementDecl*)elemDecl));
            } else if (fGrammarType == Grammar::SchemaGrammarType) 
            {
                elemDecl = new (fMemoryManager) SchemaElementDecl
                (
                    fPrefixBuf.getRawBuffer()
                    , nameRawBuf
                    , uriId
                    , SchemaElementDecl::Any
                    , Grammar::TOP_LEVEL_SCOPE
                    , fMemoryManager
                );
                elemDecl->setId(fSchemaElemNonDeclPool->put((void*)elemDecl->getBaseName(), uriId, currentScope, (SchemaElementDecl*)elemDecl));
            }
            wasAdded = true;
        }
    }
    else if (!elemDecl)
    {
        //the element has no prefix,
        //thus it is either a non-qualified element defined in current targetNS
        //or an element that is defined in the globalNS

        //try unqualified first
        elemDecl = fGrammar->getElemDecl
                   (
                      uriId
                    , nameRawBuf
                    , qnameRawBuf
                    , currentScope
                    );

        // may have not been declared; must look everywhere:
        if(!elemDecl)
            if (fGrammarType == Grammar::DTDGrammarType) 
            {
                // should never happen in practice?
                elemDecl = fDTDElemNonDeclPool->getByKey(qnameRawBuf);
            }
            else if (fGrammarType == Grammar::SchemaGrammarType) 
            {
                // look in the list of undeclared elements, as would have been done
                // before we made grammars stateless:
                elemDecl = fSchemaElemNonDeclPool->getByKey(nameRawBuf, uriId, currentScope);
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
                elemDecl = fSchemaElemNonDeclPool->getByKey(nameRawBuf, uriId, Grammar::TOP_LEVEL_SCOPE);
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
            if(fGrammarType == Grammar::DTDGrammarType) 
            {
                elemDecl = new (fMemoryManager) DTDElementDecl
                (
                    qnameRawBuf
                    , uriId
                    , DTDElementDecl::Any
                    , fMemoryManager
                );
                elemDecl->setId(fDTDElemNonDeclPool->put((DTDElementDecl*)elemDecl));
            } else if (fGrammarType == Grammar::SchemaGrammarType) 
            {
                elemDecl = new (fMemoryManager) SchemaElementDecl
                (
                    fPrefixBuf.getRawBuffer()
                    , nameRawBuf
                    , uriId
                    , SchemaElementDecl::Any
                    , Grammar::TOP_LEVEL_SCOPE
                    , fMemoryManager
                );
                elemDecl->setId(fSchemaElemNonDeclPool->put((void*)elemDecl->getBaseName(), uriId, currentScope, (SchemaElementDecl*)elemDecl));
            }
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
        else if (fValidate)
        {
            // If validating then emit an error

            // This is to tell the reuse Validator that this element was
            // faulted-in, was not an element in the grammar pool originally
            elemDecl->setCreateReason(XMLElementDecl::JustFaultIn);

            fValidator->emitError
            (
                XMLValid::ElementNotDefined
                , elemDecl->getFullName()
            );

            if(fGrammarType == Grammar::SchemaGrammarType)
            {
                ((SchemaElementDecl *)(elemDecl))->setValidationAttempted(PSVIDefs::FULL);
                ((SchemaElementDecl *)(elemDecl))->setValidity(PSVIDefs::INVALID);\
                fPSVIElemContext.fErrorOccurred = true;
            }
        }
    }
    else
    {
        if(!laxBeforeElementFound && fGrammarType == Grammar::SchemaGrammarType) {
            if (fValidate) {
                ((SchemaElementDecl *)(elemDecl))->setValidationAttempted(PSVIDefs::FULL);
                ((SchemaElementDecl *)(elemDecl))->setValidity(PSVIDefs::VALID);
            }
        }

        // If its not marked declared and validating, then emit an error
        if (!elemDecl->isDeclared()) {
            if(elemDecl->getCreateReason() == XMLElementDecl::NoReason) {
                if(fGrammarType == Grammar::SchemaGrammarType) {
                    ((SchemaElementDecl *)(elemDecl))->setValidity(PSVIDefs::INVALID);
                    ((SchemaElementDecl *)(elemDecl))->setValidationAttempted(PSVIDefs::FULL);
                    fPSVIElemContext.fErrorOccurred = true;
                }
            }
            
            if (laxThisOne) {
                fValidate = false;
                fElemStack.setValidationFlag(fValidate);
            }
            else if (fValidate)
            {
                fValidator->emitError
                (
                    XMLValid::ElementNotDefined
                    , elemDecl->getFullName()
                );
            }
        }
       
        if (fGrammarType == Grammar::SchemaGrammarType) {
            ((SchemaElementDecl*)elemDecl)->setXsiComplexTypeInfo(0);
            ((SchemaElementDecl*)elemDecl)->setXsiSimpleTypeInfo(0);
        }
    }

    if(errorBeforeElementFound && fGrammarType == Grammar::SchemaGrammarType) {
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
        if (fGrammarType == Grammar::SchemaGrammarType && !fRootElemName)
            fRootElemName = XMLString::replicate(qnameRawBuf, fMemoryManager);
    }

    if (fGrammarType == Grammar::SchemaGrammarType && fPSVIHandler)
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
            *****/
        }
    }

    //  Validate the element
    if (fValidate)
    {
        fValidator->validateElement(elemDecl);
        if (fValidator->handlesSchema())
        {
            if (((SchemaValidator*) fValidator)->getErrorOccurred())
                fPSVIElemContext.fErrorOccurred = true;
        }
    }

    if (fGrammarType == Grammar::SchemaGrammarType) {

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
    }

    fElemStack.setCurrentGrammar(fGrammar);

    //  If this is the first element and we are validating, check the root
    //  element.
    if (isRoot)
    {
        if (fValidate)
        {
            //  If a DocType exists, then check if it matches the root name there.
            if (fRootElemName && !XMLString::equals(qnameRawBuf, fRootElemName))
                fValidator->emitError(XMLValid::RootElemNotLikeDocType);

            //  Some validators may also want to check the root, call the
            //  XMLValidator::checkRootElement
            if (fValidatorFromUser && !fValidator->checkRootElement(elemDecl->getId()))
                fValidator->emitError(XMLValid::RootElemNotLikeDocType);

            if(fGrammarType == Grammar::SchemaGrammarType)
            {
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

    // PSVI handling:  even if it turns out there are
    // no attributes, we need to reset this list...
    if(getPSVIHandler() && fGrammarType == Grammar::SchemaGrammarType )
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
    if (fValidate && fGrammar && fGrammarType == Grammar::SchemaGrammarType) {

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
    }

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
                fValidator->emitError
                (
                    XMLValid::ElementNotValidForContent
                    , elemDecl->getFullName()
                    , elemDecl->getFormattedContentModel()
                );
            }

            if (fGrammarType == Grammar::SchemaGrammarType) {

                if (((SchemaValidator*) fValidator)->getErrorOccurred())
                {
                    fPSVIElemContext.fErrorOccurred = true;
                    ((SchemaElementDecl *)(elemDecl))->setValidity(PSVIDefs::INVALID);
                }
                else 
                {
                    if (fPSVIHandler)
                    {
                        fPSVIElemContext.fIsSpecified = ((SchemaValidator*) fValidator)->getIsElemSpecified();
                        if(fPSVIElemContext.fIsSpecified)
                            fPSVIElemContext.fNormalizedValue = ((SchemaElementDecl *)elemDecl)->getDefaultValue();
                    }
                    // note that if we're empty, won't be a current DV
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
        }

        if (fGrammarType == Grammar::SchemaGrammarType)
        {
            if(!isRoot)
                ((SchemaElementDecl *)fElemStack.topElement()->fThisElement)->updateValidityFromElement(elemDecl, fGrammarType);

            if (fPSVIHandler)
            {
                endElementPSVI((SchemaElementDecl*)elemDecl, psviMemberType);
            }
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
            if(fGrammarType == Grammar::SchemaGrammarType)
            {
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
            else 
            {
                fDocHandler->elementTypeInfo(0, 0); 
            }
        }

        // reset xsi:type ComplexTypeInfo
        if (fGrammarType == Grammar::SchemaGrammarType) {           
            ((SchemaElementDecl*)elemDecl)->reset();
            if (!isRoot)
                ((SchemaElementDecl*)(fElemStack.topElement()->fThisElement))->
                    setXsiComplexTypeInfo(((SchemaValidator*)fValidator)->getCurrentTypeInfo());
        }

        // If the elem stack is empty, then it was an empty root
        if (isRoot)
            gotData = false;
        else
        {
            // Restore the grammar
            fGrammar = fElemStack.getCurrentGrammar();
            fGrammarType = fGrammar->getGrammarType();
            if (fGrammarType == Grammar::SchemaGrammarType && !fValidator->handlesSchema()) {
                if (fValidatorFromUser)
                    ThrowXMLwithMemMgr(RuntimeException, XMLExcepts::Gen_NoSchemaValidator, fMemoryManager);
                else {
                    fValidator = fSchemaValidator;
                }
            }
            else if (fGrammarType == Grammar::DTDGrammarType && !fValidator->handlesDTD()) {
                if (fValidatorFromUser)
                    ThrowXMLwithMemMgr(RuntimeException, XMLExcepts::Gen_NoDTDValidator, fMemoryManager);
                else {
                    fValidator = fDTDValidator;
                }
            }

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
IGXMLScanner::resolveQName(const   XMLCh* const qName
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
//  IGXMLScanner: Helper methos
// ---------------------------------------------------------------------------
void IGXMLScanner::resizeElemState() {

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

// ---------------------------------------------------------------------------
//  IGXMLScanner: IC activation methos
// ---------------------------------------------------------------------------
void IGXMLScanner::activateSelectorFor(IdentityConstraint* const ic, const int initialDepth) {

    IC_Selector* selector = ic->getSelector();

    if (!selector)
        return;

    XPathMatcher* matcher = selector->createMatcher(fFieldActivator, initialDepth, fMemoryManager);

    fMatcherStack->addMatcher(matcher);
    matcher->startDocumentFragment();
}

// ---------------------------------------------------------------------------
//  IGXMLScanner: Grammar preparsing
// ---------------------------------------------------------------------------
Grammar* IGXMLScanner::loadGrammar(const   InputSource& src
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
        else if (grammarType == Grammar::DTDGrammarType) {
            loadedGrammar = loadDTDGrammar(src, toCache);
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

Grammar* IGXMLScanner::loadDTDGrammar(const InputSource& src,
                                      const bool toCache)
{
    // Reset the validators
    fDTDValidator->reset();
    if (fValidatorFromUser)
        fValidator->reset();

    if (!fValidator->handlesDTD()) {
        if (fValidatorFromUser && fValidate)
            ThrowXMLwithMemMgr(RuntimeException, XMLExcepts::Gen_NoDTDValidator, fMemoryManager);
        else {
            fValidator = fDTDValidator;
        }
    }

    fDTDGrammar = (DTDGrammar*) fGrammarResolver->getGrammar(XMLUni::fgDTDEntityString);

    if (fDTDGrammar) {
        fDTDGrammar->reset();
    }
    else {
        fDTDGrammar = new (fGrammarPoolMemoryManager) DTDGrammar(fGrammarPoolMemoryManager);
        fGrammarResolver->putGrammar(fDTDGrammar);
    }

    fGrammar = fDTDGrammar;
    fGrammarType = fGrammar->getGrammarType();
    fValidator->setGrammar(fGrammar);

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
    // and clear out the darned undeclared DTD element pool...
    fDTDElemNonDeclPool->removeAll();

    if (toCache) {

        unsigned int sysId = fGrammarResolver->getStringPool()->addOrFind(src.getSystemId());
        const XMLCh* sysIdStr = fGrammarResolver->getStringPool()->getValueForId(sysId);
              
        fGrammarResolver->orphanGrammar(XMLUni::fgDTDEntityString);
        ((XMLDTDDescription*) (fGrammar->getGrammarDescription()))->setRootName(sysIdStr);
        fGrammarResolver->putGrammar(fGrammar);
    }

    //  Handle the creation of the XML reader object for this input source.
    //  This will provide us with transcoding and basic lexing services.
    XMLReader* newReader = fReaderMgr.createReader
    (
        src
        , false
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

    //  In order to make the processing work consistently, we have to
    //  make this look like an external entity. So create an entity
    //  decl and fill it in and push it with the reader, as happens
    //  with an external entity. Put a janitor on it to insure it gets
    //  cleaned up. The reader manager does not adopt them.
    const XMLCh gDTDStr[] = { chLatin_D, chLatin_T, chLatin_D , chNull };
    DTDEntityDecl* declDTD = new (fGrammarPoolMemoryManager) DTDEntityDecl(gDTDStr, false, fGrammarPoolMemoryManager);
    declDTD->setSystemId(src.getSystemId());
    Janitor<DTDEntityDecl> janDecl(declDTD);

    // Mark this one as a throw at end
    newReader->setThrowAtEnd(true);

    // And push it onto the stack, with its pseudo name
    fReaderMgr.pushReader(newReader, declDTD);

    //  If we have a doc type handler and advanced callbacks are enabled,
    //  call the doctype event.
    if (fDocTypeHandler) {

        // Create a dummy root
        DTDElementDecl* rootDecl = new (fGrammarPoolMemoryManager) DTDElementDecl
        (
            gDTDStr
            , fEmptyNamespaceId
            , DTDElementDecl::Any
            , fGrammarPoolMemoryManager
        );
        rootDecl->setCreateReason(DTDElementDecl::AsRootElem);
        rootDecl->setExternalElemDeclaration(true);
        Janitor<DTDElementDecl> janSrc(rootDecl);

        fDocTypeHandler->doctypeDecl(*rootDecl, src.getPublicId(), src.getSystemId(), false, true);
    }

    // Create DTDScanner
    DTDScanner dtdScanner
    (
        (DTDGrammar*) fGrammar
        , fDocTypeHandler
        , fGrammarPoolMemoryManager
        , fMemoryManager
    );
    dtdScanner.setScannerInfo(this, &fReaderMgr, &fBufMgr);

    // Tell it its not in an include section
    dtdScanner.scanExtSubsetDecl(false, true);

    if (fValidate) {
        //  validate the DTD scan so far
        fValidator->preContentValidation(false, true);
    }

    if (toCache)
        fGrammarResolver->cacheGrammars();

    return fDTDGrammar;
}

// ---------------------------------------------------------------------------
//  IGXMLScanner: Helper methods
// ---------------------------------------------------------------------------
void IGXMLScanner::processSchemaLocation(XMLCh* const schemaLoc)
{
    XMLCh* locStr = schemaLoc;
    XMLReader* curReader = fReaderMgr.getCurrentReader();

    fLocationPairs->removeAllElements();
    while (*locStr)
    {
        do {
            if (!curReader->isWhitespace(*locStr))
               break;

            *locStr = chNull;
        } while (*++locStr);

        if (*locStr) {
            
            fLocationPairs->addElement(locStr);

            while (*++locStr) {
                if (curReader->isWhitespace(*locStr))
                    break;
            }
        }
    }
}

void IGXMLScanner::endElementPSVI(SchemaElementDecl* const elemDecl,
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

void IGXMLScanner::resetPSVIElemContext()
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
