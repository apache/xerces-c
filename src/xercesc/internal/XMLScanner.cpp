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
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/util/Janitor.hpp>
#include <xercesc/util/Mutexes.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/RefVectorOf.hpp>
#include <xercesc/util/RuntimeException.hpp>
#include <xercesc/util/UnexpectedEOFException.hpp>
#include <xercesc/util/XMLMsgLoader.hpp>
#include <xercesc/util/XMLRegisterCleanup.hpp>
#include <xercesc/util/XMLUniDefs.hpp>
#include <xercesc/util/XMLUni.hpp>
#include <xercesc/util/XMLURL.hpp>
#include <xercesc/sax/InputSource.hpp>
#include <xercesc/sax/SAXException.hpp>
#include <xercesc/framework/LocalFileInputSource.hpp>
#include <xercesc/framework/URLInputSource.hpp>
#include <xercesc/framework/XMLDocumentHandler.hpp>
#include <xercesc/framework/XMLErrorReporter.hpp>
#include <xercesc/framework/XMLEntityHandler.hpp>
#include <xercesc/framework/XMLPScanToken.hpp>
#include <xercesc/framework/XMLValidator.hpp>
#include <xercesc/framework/XMLValidityCodes.hpp>
#include <xercesc/internal/XMLScanner.hpp>
#include <xercesc/internal/EndOfEntityException.hpp>
#include <xercesc/validators/DTD/DocTypeHandler.hpp>
#include <xercesc/validators/DTD/DTDScanner.hpp>
#include <xercesc/validators/schema/SchemaSymbols.hpp>
#include <xercesc/validators/schema/identity/FieldActivator.hpp>
#include <xercesc/validators/schema/identity/XPathMatcherStack.hpp>
#include <xercesc/validators/schema/identity/ValueStoreCache.hpp>
#include <xercesc/validators/schema/identity/IC_Selector.hpp>
#include <xercesc/validators/schema/identity/ValueStore.hpp>

// ---------------------------------------------------------------------------
//  Local static data
// ---------------------------------------------------------------------------
static XMLUInt32       gScannerId;
static bool            sRegistered = false;

static XMLMutex*       sScannerMutex = 0;
static XMLRegisterCleanup scannerMutexCleanup;

static XMLMsgLoader*   gMsgLoader = 0;
static XMLRegisterCleanup cleanupMsgLoader;

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
//  Local, static functions
// ---------------------------------------------------------------------------

// -----------------------------------------------------------------------
//  Cleanup for the message loader
// -----------------------------------------------------------------------
void XMLScanner::reinitMsgLoader()
{
	delete gMsgLoader;
	gMsgLoader = 0;
}

// -----------------------------------------------------------------------
//  Cleanup for the scanner mutex
// -----------------------------------------------------------------------
void XMLScanner::reinitScannerMutex()
{
    delete sScannerMutex;
    sScannerMutex = 0;
    sRegistered = false;
}

//
//  We need to fault in this mutex. But, since its used for synchronization
//  itself, we have to do this the low level way using a compare and swap.
//
static XMLMutex& gScannerMutex()
{

    if (!sScannerMutex)
    {
        XMLMutex* tmpMutex = new XMLMutex;
        if (XMLPlatformUtils::compareAndSwap((void**)&sScannerMutex, tmpMutex, 0))
        {
            // Someone beat us to it, so let's clean up ours
            delete tmpMutex;
        }

        // Now lock it and try to register it
        XMLMutexLock lock(sScannerMutex);

        // If we got here first, then register it and set the registered flag
        if (!sRegistered)
        {
			scannerMutexCleanup.registerCleanup(XMLScanner::reinitScannerMutex);
            sRegistered = true;
        }
    }
    return *sScannerMutex;
}


// ---------------------------------------------------------------------------
//  XMLScanner: Constructors and Destructor
// ---------------------------------------------------------------------------
XMLScanner::XMLScanner(XMLValidator* const valToAdopt) :

    fDoNamespaces(false)
    , fExitOnFirstFatal(true)
    , fValidationConstraintFatal(false)
    , fInException(false)
    , fStandalone(false)
    , fHasNoDTD(true)
    , fValidate(false)
    , fValidatorFromUser(false)
    , fDoSchema(false)
    , fSchemaFullChecking(false)
    , fSeeXsi(false)
    , fToCacheGrammar(false)
    , fUseCachedGrammar(false)
    , fErrorCount(0)
    , fEmptyNamespaceId(0)
    , fUnknownNamespaceId(0)
    , fXMLNamespaceId(0)
    , fXMLNSNamespaceId(0)
    , fSchemaNamespaceId(0)
    , fElemStateSize(16)
    , fScannerId(0)
    , fSequenceId(0)
    , fElemState(0)
    , fAttrList(0)
    , fDocHandler(0)
    , fDocTypeHandler(0)
    , fEntityHandler(0)
    , fErrorReporter(0)
    , fErrorHandler(0)
    , fIDRefList(0)
    , fRawAttrList(0)
    , fValidator(valToAdopt)
    , fDTDValidator(0)
    , fSchemaValidator(0)
    , fValScheme(Val_Never)
    , fGrammarResolver(0)
    , fGrammar(0)
    , fRootGrammar(0)
    , fDTDGrammar(0)
    , fURIStringPool(0)
    , fMatcherStack(0)
    , fValueStoreCache(0)
    , fFieldActivator(0)
    , fRootElemName(0)
    , fExternalSchemaLocation(0)
    , fExternalNoNamespaceSchemaLocation(0)
    , fLoadExternalDTD(true)
    , fNormalizeData(true)
{
   commonInit();

   if (fValidator) {
       fValidatorFromUser = true;
       initValidator(fValidator);
   }
   else {
       //use fDTDValidator as the default validator
       fValidator = fDTDValidator;
   }
}

XMLScanner::XMLScanner( XMLDocumentHandler* const  docHandler
                        , DocTypeHandler* const    docTypeHandler
                        , XMLEntityHandler* const  entityHandler
                        , XMLErrorReporter* const  errHandler
                        , XMLValidator* const      valToAdopt) :

    fDoNamespaces(false)
    , fExitOnFirstFatal(true)
    , fValidationConstraintFatal(false)
    , fInException(false)
    , fStandalone(false)
    , fHasNoDTD(true)
    , fValidate(false)
    , fValidatorFromUser(false)
    , fDoSchema(false)
    , fSchemaFullChecking(false)
    , fSeeXsi(false)
    , fToCacheGrammar(false)
    , fUseCachedGrammar(false)
    , fErrorCount(0)
    , fEmptyNamespaceId(0)
    , fUnknownNamespaceId(0)
    , fXMLNamespaceId(0)
    , fXMLNSNamespaceId(0)
    , fSchemaNamespaceId(0)
    , fElemStateSize(16)
    , fScannerId(0)
    , fSequenceId(0)
    , fElemState(0)
    , fAttrList(0)
    , fDocHandler(docHandler)
    , fDocTypeHandler(docTypeHandler)
    , fEntityHandler(entityHandler)
    , fErrorReporter(errHandler)
    , fErrorHandler(0)
    , fIDRefList(0)
    , fRawAttrList(0)
    , fValidator(valToAdopt)
    , fDTDValidator(0)
    , fSchemaValidator(0)
    , fValScheme(Val_Never)
    , fGrammarResolver(0)
    , fGrammar(0)
    , fRootGrammar(0)
    , fDTDGrammar(0)
    , fURIStringPool(0)
    , fMatcherStack(0)
    , fValueStoreCache(0)
    , fFieldActivator(0)
    , fRootElemName(0)
    , fExternalSchemaLocation(0)
    , fExternalNoNamespaceSchemaLocation(0)
    , fLoadExternalDTD(true)
    , fNormalizeData(true)
{
   commonInit();

   if (valToAdopt){
       fValidatorFromUser = true;
       initValidator(fValidator);
   }
   else {
       //use fDTDValidator as the default validator
       fValidator = fDTDValidator;
   }
}

XMLScanner::~XMLScanner()
{
    delete [] fElemState;
    delete fAttrList;
    delete fIDRefList;
    delete fRawAttrList;
    if (fValidatorFromUser)
        delete fValidator;

    delete fDTDValidator;
    delete fSchemaValidator;

    //fGrammarResolver will delete the fGrammar as well
    delete fGrammarResolver;

    delete fURIStringPool;

    delete fFieldActivator;
    delete fMatcherStack;
    delete fValueStoreCache;

    delete [] fRootElemName;
    delete [] fExternalSchemaLocation;
    delete [] fExternalNoNamespaceSchemaLocation;
}


// ---------------------------------------------------------------------------
//  XMLScanner: Main entry point to scan a document
// ---------------------------------------------------------------------------
void XMLScanner::scanDocument(  const   XMLCh* const    systemId)
{
    //
    //  First we try to parse it as a URL. If that fails, we assume its
    //  a file and try it that way.
    //
    InputSource* srcToUse = 0;
    try
    {
        //
        //  Create a temporary URL. Since this is the primary document,
        //  it has to be fully qualified. If not, then assume we are just
        //  mistaking a file for a URL.
        //

        XMLURL tmpURL(systemId);
        if (tmpURL.isRelative()) {
            srcToUse = new LocalFileInputSource(systemId);
        }
        else
        {
            srcToUse = new URLInputSource(tmpURL);
        }

    }

    catch(const MalformedURLException&)
    {
        srcToUse = new LocalFileInputSource(systemId);
    }

    catch(const XMLException& excToCatch)
    {
        //
        //  For any other XMLException,
        //  emit the error and catch any user exception thrown from here.
        //
        fInException = true;
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
        return;
    }

    catch(...)
    {
        // Just rethrow this, since its not our problem
        throw;
    }

    Janitor<InputSource> janSrc(srcToUse);
    scanDocument(*srcToUse);
}

void XMLScanner::scanDocument(  const   char* const systemId)
{
    // We just delegate this to the XMLCh version after transcoding
    XMLCh* tmpBuf = XMLString::transcode(systemId);
    ArrayJanitor<XMLCh> janBuf(tmpBuf);
    scanDocument(tmpBuf);
}


void XMLScanner::scanDocument(const InputSource& src)
{
    //
    //  Bump up the sequence id for this parser instance. This will invalidate
    //  any previous progressive scan tokens.
    //
    fSequenceId++;

    try
    {
        //
        //  Reset the scanner and its plugged in stuff for a new run. This
        //  resets all the data structures, creates the initial reader and
        //  pushes it on the stack, and sets up the base document path.
        //
        scanReset(src);

        // If we have a document handler, then call the start document
        if (fDocHandler)
            fDocHandler->startDocument();

        fValueStoreCache->startDocument();

        //
        //  Scan the prolog part, which is everything before the root element
        //  including the DTD subsets.
        //
        scanProlog();

        //
        //  If we got to the end of input, then its not a valid XML file.
        //  Else, go on to scan the content.
        //
        if (fReaderMgr.atEOF())
        {
            emitError(XMLErrs::EmptyMainEntity);
        }
         else
        {
            // Scan content, and tell it its not an external entity
            if (scanContent(false))
            {
                // Do post-parse validation if required
                if (fValidate)
                {
                    //
                    //  We handle ID reference semantics at this level since
                    //  its required by XML 1.0.
                    //
                    checkIDRefs();

                    // Then allow the validator to do any extra stuff it wants
                    fValidator->postParseValidation();
                }

                // That went ok, so scan for any miscellaneous stuff
                if (!fReaderMgr.atEOF())
                    scanMiscellaneous();
            }
        }

        if (fValidate)
            fValueStoreCache->endDocument();

        // If we have a document handler, then call the end document
        if (fDocHandler)
            fDocHandler->endDocument();

        // Reset the reader manager to close all files, sockets, etc...
        fReaderMgr.reset();
    }

    //
    //  NOTE:
    //
    //  In all of the error processing below, the emitError() call MUST come
    //  before the flush of the reader mgr, or it will fail because it tries
    //  to find out the position in the XML source of the error.
    //
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
        //
        //  Emit the error and catch any user exception thrown from here. Make
        //  sure in all cases we flush the reader manager.
        //
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

        catch(...)
        {
            // Flush the reader manager and rethrow user's error
            fReaderMgr.reset();
            throw;
        }

        // If it returned, then reset the reader manager and fall through
        fReaderMgr.reset();
    }

    catch(...)
    {
        // Reset and rethrow
        fReaderMgr.reset();
        throw;
    }
}


//
//  This method begins a progressive parse. It scans through the prolog and
//  returns a token to be used on subsequent scanNext() calls. If the return
//  value is true, then the token is legal and ready for further use. If it
//  returns false, then the scan of the prolog failed and the token is not
//  going to work on subsequent scanNext() calls.
//
bool XMLScanner::scanFirst( const   XMLCh* const    systemId
                            ,       XMLPScanToken&  toFill)
{
    //
    //  First we try to parse it as a URL. If that fails, we assume its
    //  a file and try it that way.
    //
    InputSource* srcToUse = 0;
    try
    {
        //
        //  Create a temporary URL. Since this is the primary document,
        //  it has to be fully qualified. If not, then assume we are just
        //  mistaking a file for a URL.
        //

        XMLURL tmpURL(systemId);
        if (tmpURL.isRelative()) {
            srcToUse = new LocalFileInputSource(systemId);
        }
        else
        {
            srcToUse = new URLInputSource(tmpURL);
        }
    }

    catch(const MalformedURLException&)
    {
        srcToUse = new LocalFileInputSource(systemId);
    }

    catch(const XMLException& excToCatch)
    {
        //
        //  For any other XMLException,
        //  emit the error and catch any user exception thrown from here.
        //
        fInException = true;
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
        return false;
    }

    catch(...)
    {
        // Just rethrow this, since its not our problem
        throw;
    }

    Janitor<InputSource> janSrc(srcToUse);
    return scanFirst(*srcToUse, toFill);
}

bool XMLScanner::scanFirst( const   char* const     systemId
                            ,       XMLPScanToken&  toFill)
{
    // We just delegate this to the XMLCh version after transcoding
    XMLCh* tmpBuf = XMLString::transcode(systemId);
    ArrayJanitor<XMLCh> janBuf(tmpBuf);
    return scanFirst(tmpBuf, toFill);
}

bool XMLScanner::scanFirst( const   InputSource&    src
                            ,       XMLPScanToken&  toFill)
{
    //
    //  Bump up the sequence id for this new scan cycle. This will invalidate
    //  any previous tokens we've returned.
    //
    fSequenceId++;

    //
    // Reset the scanner and its plugged in stuff for a new run.  This
    // resets all the data structures, creates the initial reader and
    // pushes it on the stack, and sets up the base document path
    //
    scanReset(src);

    // If we have a document handler, then call the start document
    if (fDocHandler)
        fDocHandler->startDocument();

    fValueStoreCache->startDocument();

    try
    {
        //
        //  Scan the prolog part, which is everything before the root element
        //  including the DTD subsets. This is all that is done on the scan
        //  first.
        //
        scanProlog();

        //
        //  If we got to the end of input, then its not a valid XML file.
        //  Else, go on to scan the content.
        //
        if (fReaderMgr.atEOF())
        {
            emitError(XMLErrs::EmptyMainEntity);
        }
    }

    //
    //  NOTE:
    //
    //  In all of the error processing below, the emitError() call MUST come
    //  before the flush of the reader mgr, or it will fail because it tries
    //  to find out the position in the XML source of the error.
    //
    catch(const XMLErrs::Codes)
    {
        // This is a 'first failure' exception so reset and return a failure
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
        //
        //  Emit the error and catch any user exception thrown from here. Make
        //  sure in all cases we flush the reader manager.
        //
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

        catch(...)
        {
            // Reset and rethrow the user error
            fReaderMgr.reset();
            throw;
        }

        // Reset and return a failure
        fReaderMgr.reset();
        return false;
    }

    catch(...)
    {
        // Reset and rethrow original error
        fReaderMgr.reset();
        throw;
    }

    // Fill in the caller's token to make it legal and return success
    toFill.set(fScannerId, fSequenceId);
    return true;
}


bool XMLScanner::scanNext(XMLPScanToken& token)
{
    // Make sure this token is still legal
    if (!isLegalToken(token))
        ThrowXML(RuntimeException, XMLExcepts::Scan_BadPScanToken);

    // Find the next token and remember the reader id
    unsigned int orgReader;
    XMLTokens curToken;

    bool retVal = true;

    try
    {
        while (true)
        {
            //
            //  We have to handle any end of entity exceptions that happen here.
            //  We could be at the end of X nested entities, each of which will
            //  generate an end of entity exception as we try to move forward.
            //

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
                    //
                    //  We handle ID reference semantics at this level since
                    //  its required by XML 1.0.
                    //
                    checkIDRefs();

                    // Then allow the validator to do any extra stuff it wants
                    fValidator->postParseValidation();
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

    //
    //  NOTE:
    //
    //  In all of the error processing below, the emitError() call MUST come
    //  before the flush of the reader mgr, or it will fail because it tries
    //  to find out the position in the XML source of the error.
    //
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

    // We have to propogate SAX exceptions
    catch(const SAXException&)
    {
        // Just reset our reader manager and rethrow SAX exception
        fReaderMgr.reset();
        throw;
    }

    catch(const XMLException& excToCatch)
    {
        //
        //  Emit the error and catch any user exception thrown from here. Make
        //  sure in all cases we flush the reader manager.
        //
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


void XMLScanner::scanReset(XMLPScanToken& token)
{
    // Make sure this token is still legal
    if (!isLegalToken(token))
        ThrowXML(RuntimeException, XMLExcepts::Scan_BadPScanToken);

    // Reset the reader manager
    fReaderMgr.reset();

    // And invalidate any tokens by bumping our sequence number
    fSequenceId++;

    // Reset our error count
    fErrorCount = 0;
}



// ---------------------------------------------------------------------------
//  XMLScanner: Private helper methods. Most of these are implemented in
//  XMLScanner2.Cpp.
// ---------------------------------------------------------------------------

//
//  This method handles the common initialization, to avoid having to do
//  it redundantly in multiple constructors.
//
void XMLScanner::commonInit()
{

    //
    //  We have to do a little init that involves statics, so we have to
    //  use the mutex to protect it.
    //
    {
        XMLMutexLock lockInit(&gScannerMutex());

        // If we haven't loaded our message yet, then do that
        if (!gMsgLoader)
        {
            gMsgLoader = XMLPlatformUtils::loadMsgSet(XMLUni::fgXMLErrDomain);
            if (!gMsgLoader)
                XMLPlatformUtils::panic(XMLPlatformUtils::Panic_CantLoadMsgDomain);

            // Register this object to be cleaned up at termination
			cleanupMsgLoader.registerCleanup(reinitMsgLoader);
        }

        // And assign ourselves the next available scanner id
        fScannerId = ++gScannerId;
    }

    //
    //  Create the element state array
    //
    fElemState = new unsigned int[fElemStateSize];

    //
    //  Create the attribute list, which is used to store attribute values
    //  during start tag processing. Give it a reasonable initial size that
    //  will serve for most folks, though it will grow as required.
    //
    fAttrList = new RefVectorOf<XMLAttr>(32);

    //
    //  And we need one for the raw attribute scan. This just stores key/
    //  value string pairs (prior to any processing.)
    //
    fRawAttrList = new RefVectorOf<KVStringPair>(32);

    //
    //  Create the id ref list. This is used to enforce XML 1.0 ID ref
    //  semantics, i.e. all id refs must refer to elements that exist
    //
    fIDRefList = new RefHashTableOf<XMLRefInfo>(109);

    //  Create the GrammarResolver
    fGrammarResolver = new GrammarResolver();

    //  Create the Validator and init them
    fDTDValidator = new DTDValidator();
    initValidator(fDTDValidator);
    fSchemaValidator = new SchemaValidator();
    initValidator(fSchemaValidator);

    // Create IdentityConstraint info
    fMatcherStack = new XPathMatcherStack();
    fValueStoreCache = new ValueStoreCache();
    fFieldActivator = new FieldActivator(fValueStoreCache, fMatcherStack);
    fValueStoreCache->setScanner(this);
}



void XMLScanner::initValidator(XMLValidator* theValidator) {
    //
    //  Tell the validator about the stuff it needs to know in order to
    //  do its work.
    //
    theValidator->setScannerInfo(this, &fReaderMgr, &fBufMgr);
    theValidator->setErrorReporter(fErrorReporter);

    if (theValidator->handlesSchema()) {

        ((SchemaValidator*) theValidator)->setGrammarResolver(fGrammarResolver);
        ((SchemaValidator*) theValidator)->setExitOnFirstFatal(fExitOnFirstFatal);
    }
}

void XMLScanner::resetURIStringPool() {
    fURIStringPool->flushAll();

    fEmptyNamespaceId   = fURIStringPool->addOrFind(XMLUni::fgZeroLenString);
    fUnknownNamespaceId = fURIStringPool->addOrFind(XMLUni::fgUnknownURIName);
    fXMLNamespaceId     = fURIStringPool->addOrFind(XMLUni::fgXMLURIName);
    fXMLNSNamespaceId   = fURIStringPool->addOrFind(XMLUni::fgXMLNSURIName);
    fSchemaNamespaceId  = fURIStringPool->addOrFind(SchemaSymbols::fgURI_XSI);
}

// ---------------------------------------------------------------------------
//  XMLScanner: Error emitting methods
// ---------------------------------------------------------------------------

//
//  These methods are called whenever the scanner wants to emit an error.
//  It handles getting the message loaded, doing token replacement, etc...
//  and then calling the error handler, if its installed.
//
void XMLScanner::emitError(const XMLErrs::Codes toEmit)
{
    // Bump the error count if it is not a warning
    if (XMLErrs::errorType(toEmit) != XMLErrorReporter::ErrType_Warning)
        incrementErrorCount();

    if (fErrorReporter)
    {
        // Load the message into a local for display
        const unsigned int msgSize = 1023;
        XMLCh errText[msgSize + 1];

        // Lock the mutex and load the text
        {
            XMLMutexLock lockInit(&gScannerMutex());
            if (!gMsgLoader->loadMsg(toEmit, errText, msgSize))
            {
                // <TBD> Probably should load a default msg here
            }
        }

        //
        //  Create a LastExtEntityInfo structure and get the reader manager
        //  to fill it in for us. This will give us the information about
        //  the last reader on the stack that was an external entity of some
        //  sort (i.e. it will ignore internal entities.
        //
        ReaderMgr::LastExtEntityInfo lastInfo;
        fReaderMgr.getLastExtEntityInfo(lastInfo);

        fErrorReporter->error
        (
            toEmit
            , XMLUni::fgXMLErrDomain
            , XMLErrs::errorType(toEmit)
            , errText
            , lastInfo.systemId
            , lastInfo.publicId
            , lastInfo.lineNumber
            , lastInfo.colNumber
        );
    }

    // Bail out if its fatal an we are to give up on the first fatal error
    if (XMLErrs::isFatal(toEmit) && fExitOnFirstFatal && !fInException)
        throw toEmit;
}

void XMLScanner::emitError( const   XMLErrs::Codes    toEmit
                            , const XMLCh* const        text1
                            , const XMLCh* const        text2
                            , const XMLCh* const        text3
                            , const XMLCh* const        text4)
{
    // Bump the error count if it is not a warning
    if (XMLErrs::errorType(toEmit) != XMLErrorReporter::ErrType_Warning)
        incrementErrorCount();

    if (fErrorReporter)
    {
        //
        //  Load the message into alocal and replace any tokens found in
        //  the text.
        //
        const unsigned int maxChars = 2047;
        XMLCh errText[maxChars + 1];

        // Lock the mutex and load the text
        {
            XMLMutexLock lockInit(&gScannerMutex());
            if (!gMsgLoader->loadMsg(toEmit, errText, maxChars, text1, text2, text3, text4))
            {
                // <TBD> Should probably load a default message here
            }
        }

        //
        //  Create a LastExtEntityInfo structure and get the reader manager
        //  to fill it in for us. This will give us the information about
        //  the last reader on the stack that was an external entity of some
        //  sort (i.e. it will ignore internal entities.
        //
        ReaderMgr::LastExtEntityInfo lastInfo;
        fReaderMgr.getLastExtEntityInfo(lastInfo);

        fErrorReporter->error
        (
            toEmit
            , XMLUni::fgXMLErrDomain
            , XMLErrs::errorType(toEmit)
            , errText
            , lastInfo.systemId
            , lastInfo.publicId
            , lastInfo.lineNumber
            , lastInfo.colNumber
        );
    }

    // Bail out if its fatal an we are to give up on the first fatal error
    if (XMLErrs::isFatal(toEmit) && fExitOnFirstFatal && !fInException)
        throw toEmit;
}

void XMLScanner::emitError( const   XMLErrs::Codes    toEmit
                            , const char* const         text1
                            , const char* const         text2
                            , const char* const         text3
                            , const char* const         text4)
{
    // Bump the error count if it is not a warning
    if (XMLErrs::errorType(toEmit) != XMLErrorReporter::ErrType_Warning)
        incrementErrorCount();

    if (fErrorReporter)
    {
        //
        //  Load the message into alocal and replace any tokens found in
        //  the text.
        //
        const unsigned int maxChars = 2047;
        XMLCh errText[maxChars + 1];

        // Lock the mutex and load the text
        {
            XMLMutexLock lockInit(&gScannerMutex());
            if (!gMsgLoader->loadMsg(toEmit, errText, maxChars, text1, text2, text3, text4))
            {
                // <TBD> Should probably load a default message here
            }
        }

        //
        //  Create a LastExtEntityInfo structure and get the reader manager
        //  to fill it in for us. This will give us the information about
        //  the last reader on the stack that was an external entity of some
        //  sort (i.e. it will ignore internal entities.
        //
        ReaderMgr::LastExtEntityInfo lastInfo;
        fReaderMgr.getLastExtEntityInfo(lastInfo);

        fErrorReporter->error
        (
            toEmit
            , XMLUni::fgXMLErrDomain
            , XMLErrs::errorType(toEmit)
            , errText
            , lastInfo.systemId
            , lastInfo.publicId
            , lastInfo.lineNumber
            , lastInfo.colNumber
        );
    }

    // Bail out if its fatal an we are to give up on the first fatal error
    if (XMLErrs::isFatal(toEmit) && fExitOnFirstFatal && !fInException)
        throw toEmit;
}


// ---------------------------------------------------------------------------
//  XMLScanner: Getter methods
// ---------------------------------------------------------------------------

//
//  This method allows the caller to query the current location of the scanner.
//  It will return the sys/public ids of the current entity, and the line/col
//  position within it.
//
//  NOTE: This API returns the location with the last external file. So if its
//  currently scanning an entity, the position returned will be the end of
//  the entity reference in the file that had the reference.
//
bool
XMLScanner::getLastExtLocation(         XMLCh* const    sysIdToFill
                                , const unsigned int    maxSysIdChars
                                ,       XMLCh* const    pubIdToFill
                                , const unsigned int    maxPubIdChars
                                ,       XMLSSize_t&     lineToFill
                                ,       XMLSSize_t&     colToFill) const
{
    // Create a local info object and get it filled in by the reader manager
    ReaderMgr::LastExtEntityInfo lastInfo;
    fReaderMgr.getLastExtEntityInfo(lastInfo);

    // Fill in the line and column number
    lineToFill = lastInfo.lineNumber;
    colToFill = lastInfo.colNumber;

    // And copy over as much of the ids as will fit
    sysIdToFill[0] = 0;
    if (lastInfo.systemId)
    {
        if (XMLString::stringLen(lastInfo.systemId) > maxSysIdChars)
            return false;
        XMLString::copyString(sysIdToFill, lastInfo.systemId);
    }

    pubIdToFill[0] = 0;
    if (lastInfo.publicId)
    {
        if (XMLString::stringLen(lastInfo.publicId) > maxPubIdChars)
            return false;
        XMLString::copyString(pubIdToFill, lastInfo.publicId);
    }
    return true;
}


// ---------------------------------------------------------------------------
//  XMLScanner: Private scanning methods
// ---------------------------------------------------------------------------

//
//  This method is called from scanStartTag() to handle the very raw initial
//  scan of the attributes. It just fills in the passed collection with
//  key/value pairs for each attribute. No processing is done on them at all.
//
unsigned int
XMLScanner::rawAttrScan(const   XMLCh* const                elemName
                        ,       RefVectorOf<KVStringPair>&  toFill
                        ,       bool&                       isEmpty)
{
    //
    //  Keep up with how many attributes we've seen so far, and how many
    //  elements are available in the vector. This way we can reuse old
    //  elements until we run out and then expand it.
    //
    unsigned int attCount = 0;
    unsigned int curVecSize = toFill.size();

    // Assume it is not empty
    isEmpty = false;

    //
    //  We loop until we either see a /> or >, handling key/value pairs util
    //  we get there. We place them in the passed vector, which we will expand
    //  as required to hold them.
    //
    while (true)
    {
        // Get the next character, which should be non-space
        XMLCh nextCh = fReaderMgr.peekNextChar();

        //
        //  If the next character is not a slash or closed angle bracket,
        //  then it must be whitespace, since whitespace is required
        //  between the end of the last attribute and the name of the next
        //  one.
        //
        if (attCount)
        {
            if ((nextCh != chForwardSlash) && (nextCh != chCloseAngle))
            {
                if (XMLReader::isWhitespace(nextCh))
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

        //
        //  Ok, here we first check for any of the special case characters.
        //  If its not one, then we do the normal case processing, which
        //  assumes that we've hit an attribute value, Otherwise, we do all
        //  the special case checks.
        //
        if (!XMLReader::isSpecialStartTagChar(nextCh))
        {
            //
            //  Assume its going to be an attribute, so get a name from
            //  the input.
            //
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

                //
                //  Try to sync back up by skipping forward until we either
                //  hit something meaningful.
                //
                const XMLCh chFound = fReaderMgr.skipUntilInOrWS(tmpList);

                if ((chFound == chCloseAngle) || (chFound == chForwardSlash))
                {
                    // Jump back to top for normal processing of these
                    continue;
                }
                 else if ((chFound == chSingleQuote)
                      ||  (chFound == chDoubleQuote)
                      ||  XMLReader::isWhitespace(chFound))
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

            //
            //  Next should be the quoted attribute value. We just do a simple
            //  and stupid scan of this value. The only thing we do here
            //  is to expand entity references.
            //
            if (!basicAttrValueScan(fAttNameBuf.getRawBuffer(), fAttValueBuf))
            {
                static const XMLCh tmpList[] =
                {
                    chCloseAngle, chOpenAngle, chForwardSlash, chNull
                };

                emitError(XMLErrs::ExpectedAttrValue);

                //
                //  It failed, so lets try to get synced back up. We skip
                //  forward until we find some whitespace or one of the
                //  chars in our list.
                //
                const XMLCh chFound = fReaderMgr.skipUntilInOrWS(tmpList);

                if ((chFound == chCloseAngle)
                ||  (chFound == chForwardSlash)
                ||  XMLReader::isWhitespace(chFound))
                {
                    //
                    //  Just fall through and process this attribute, though
                    //  the value will be "".
                    //
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

            //
            //  Make sure that the name is basically well formed for namespace
            //  enabled rules. It either has no colons, or it has one which
            //  is neither the first or last char.
            //
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


            //
            //  And now lets add it to the passed collection. If we have not
            //  filled it up yet, then we use the next element. Else we add
            //  a new one.
            //
            KVStringPair* curPair = 0;
            if (attCount >= curVecSize)
            {
                curPair = new KVStringPair
                (
                    fAttNameBuf.getRawBuffer()
                    , fAttValueBuf.getRawBuffer()
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

        //
        //  It was some special case character so do all of the checks and
        //  deal with it.
        //
        if (!nextCh)
            ThrowXML(UnexpectedEOFException, XMLExcepts::Gen_UnexpectedEOF);

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
            //
            //  Check for this one specially, since its going to be common
            //  and it is kind of auto-recovering since we've already hit the
            //  next open bracket, which is what we would have seeked to (and
            //  skipped this whole tag.)
            //
            emitError(XMLErrs::UnterminatedStartTag, elemName);
            break;
        }
         else if ((nextCh == chSingleQuote) || (nextCh == chDoubleQuote))
        {
            //
            //  Check for this one specially, which is probably a missing
            //  attribute name, e.g. ="value". Just issue expected name
            //  error and eat the quoted string, then jump back to the
            //  top again.
            //
            emitError(XMLErrs::ExpectedAttrName);
            fReaderMgr.getNextChar();
            fReaderMgr.skipQuotedString(nextCh);
            fReaderMgr.skipPastSpaces();
            continue;
        }
    }

    return attCount;
}


//
//  This method will kick off the scanning of the primary content of the
//  document, i.e. the elements.
//
bool XMLScanner::scanContent(const bool extEntity)
{
    //
    //  Go into a loop until we hit the end of the root element, or we fall
    //  out because there is no root element.
    //
    //  We have to do kind of a deeply nested double loop here in order to
    //  avoid doing the setup/teardown of the exception handler on each
    //  round. Doing it this way we only do it when an exception actually
    //  occurs.
    //
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
                //
                unsigned int orgReader;
                const XMLTokens curToken = senseNextToken(orgReader);

                //
                //  Handle character data and end of file specially. Char data
                //  is not markup so we don't want to handle it in the loop
                //  below.
                //
                if (curToken == Token_CharData)
                {
                    //
                    //  Scan the character data and call appropriate events. Let
                    //  him use our local character data buffer for efficiency.
                    //
                    scanCharData(fCDataBuf);
                    continue;
                }
                 else if (curToken == Token_EOF)
                {
                    //
                    //  The element stack better be empty at this point or we
                    //  ended prematurely before all elements were closed.
                    //
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

                //
                //  According to the token we got, call the appropriate
                //  scanning method.
                //
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
            //
            //  If we were in some markup when this happened, then its a
            //  partial markup error.
            //
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


void XMLScanner::scanEndTag(bool& gotData)
{
    //
    //  Assume we will still have data until proven otherwise. It will only
    //  ever be false if this is the end of the root element.
    //
    gotData = true;

    //
    //  Check if the element stack is empty. If so, then this is an unbalanced
    //  element (i.e. more ends than starts, perhaps because of bad text
    //  causing one to be skipped.)
    //
    if (fElemStack.isEmpty())
    {
        emitError(XMLErrs::MoreEndThanStartTags);
        fReaderMgr.skipPastChar(chCloseAngle);
        ThrowXML(RuntimeException, XMLExcepts::Scan_UnbalancedStartEnd);
    }

    // After the </ is the element QName, so get a name from the input
    XMLBufBid bbQName(&fBufMgr);
    XMLBuffer& qnameBuf = bbQName.getBuffer();
    if (!fReaderMgr.getName(qnameBuf))
    {
        // It failed so we can't really do anything with it
        emitError(XMLErrs::ExpectedElementName);
        fReaderMgr.skipPastChar(chCloseAngle);
        return;
    }

    unsigned int uriId = fEmptyNamespaceId;
    XMLBufBid bbName(&fBufMgr);
    XMLBufBid bbPrefix(&fBufMgr);
    if (fDoNamespaces)
    {
        uriId = resolveQName
        (
            qnameBuf.getRawBuffer()
            , bbName.getBuffer()
            , bbPrefix.getBuffer()
            , ElemStack::Mode_Element
        );
    }

    //
    //  Pop the stack of the element we are supposed to be ending. Remember
    //  that we don't own this. The stack just keeps them and reuses them.
    //
    //  NOTE: We CANNOT do this until we've resolved the element name because
    //  the element stack top contains the prefix to URI mappings for this
    //  element.
    //
    unsigned int topUri = fElemStack.getCurrentURI();
    const ElemStack::StackElem* topElem = fElemStack.popTop();

    // See if it was the root element, to avoid multiple calls below
    const bool isRoot = fElemStack.isEmpty();

    // Make sure that its the end of the element that we expect
    XMLElementDecl* tempElement = topElem->fThisElement;
    if (fDoNamespaces && fGrammarType == Grammar::SchemaGrammarType) {
        if ((topUri != uriId) || (XMLString::compareString(tempElement->getBaseName(), bbName.getRawBuffer())))
        {
            emitError
            (
                XMLErrs::ExpectedEndOfTagX
                , topElem->fThisElement->getFullName()
            );
        }
    }
    else {
        if (XMLString::compareString(tempElement->getFullName(), qnameBuf.getRawBuffer()))
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

    //
    //  If validation is enabled, then lets pass him the list of children and
    //  this element and let him validate it.
    //
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
            //
            //  One of the elements is not valid for the content. NOTE that
            //  if no children were provided but the content model requires
            //  them, it comes back with a zero value. But we cannot use that
            //  to index the child array in this case, and have to put out a
            //  special message.
            //
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

        // reset xsi:type ComplexTypeInfo
        if (fGrammarType == Grammar::SchemaGrammarType) {
            ((SchemaElementDecl*)topElem->fThisElement)->setXsiComplexTypeInfo(0);

            // call matchers and de-activate context
            int oldCount = fMatcherStack->getMatcherCount();

            if (oldCount ||
                ((SchemaElementDecl*)topElem->fThisElement)->getIdentityConstraintCount()) {

                for (int i = oldCount - 1; i >= 0; i--) {

                    XPathMatcher* matcher = fMatcherStack->getMatcherAt(i);
                    matcher->endElement(*(topElem->fThisElement));
                }

                if (fMatcherStack->size() > 0) {
                    fMatcherStack->popContext();
                }

                // handle everything *but* keyref's.
                int newCount = fMatcherStack->getMatcherCount();

                for (int j = oldCount - 1; j >= newCount; j--) {

                    XPathMatcher* matcher = fMatcherStack->getMatcherAt(j);
                    IdentityConstraint* ic = matcher->getIdentityConstraint();

                    if (ic  && (ic->getType() != IdentityConstraint::KEYREF)) {

                        matcher->endDocumentFragment();
                        fValueStoreCache->transplant(ic);
                    }
                    else if (!ic) {
                        matcher->endDocumentFragment();
                    }
                }

                // now handle keyref's...
                for (int k = oldCount - 1; k >= newCount; k--) {

                    XPathMatcher* matcher = fMatcherStack->getMatcherAt(k);
                    IdentityConstraint* ic = matcher->getIdentityConstraint();

                    if (ic && (ic->getType() == IdentityConstraint::KEYREF)) {

                        ValueStore* values = fValueStoreCache->getValueStoreFor(ic);

                        if (values) { // nothing to do if nothing matched!
                            values->endDcocumentFragment(fValueStoreCache);
                        }

                        matcher->endDocumentFragment();
                    }
                }

                fValueStoreCache->endElement();
            }
        }
    }

    // If we have a doc handler, tell it about the end tag
    if (fDocHandler)
    {
        fDocHandler->endElement
        (
            *topElem->fThisElement
            , uriId
            , isRoot
            , bbPrefix.getRawBuffer()
        );
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
                    ThrowXML(RuntimeException, XMLExcepts::Gen_NoSchemaValidator);
                else {
                    fValidator = fSchemaValidator;
                }
            }
            else if (fGrammarType == Grammar::DTDGrammarType && !fValidator->handlesDTD()) {
                if (fValidatorFromUser)
                    ThrowXML(RuntimeException, XMLExcepts::Gen_NoDTDValidator);
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


//
//  This method is called after the end of the root element, to handle
//  any miscellaneous stuff hanging around.
//
void XMLScanner::scanMiscellaneous()
{
    // Get a buffer for this work
    XMLBufBid bbCData(&fBufMgr);

    while (true)
    {
        try
        {
            const XMLCh nextCh = fReaderMgr.peekNextChar();

            // Watch for end of file and break out
            if (!nextCh)
                break;

            if (nextCh == chOpenAngle)
            {
                if (checkXMLDecl(true))
                {
                    // Can't have an XML decl here
                    emitError(XMLErrs::NotValidAfterContent);
                    fReaderMgr.skipPastChar(chCloseAngle);
                }
                 else if (fReaderMgr.skippedString(XMLUni::fgPIString))
                {
                    scanPI();
                }
                 else if (fReaderMgr.skippedString(XMLUni::fgCommentString))
                {
                    scanComment();
                }
                 else
                {
                    // This can't be possible, so just give up
                    emitError(XMLErrs::ExpectedCommentOrPI);
                    fReaderMgr.skipPastChar(chCloseAngle);
                }
            }
             else if (XMLReader::isWhitespace(nextCh))
            {
                //
                //  If we have a doc handler, then gather up the spaces and
                //  call back. Otherwise, just skip over whitespace.
                //
                if (fDocHandler)
                {
                    fReaderMgr.getSpaces(bbCData.getBuffer());
                    fDocHandler->ignorableWhitespace
                    (
                        bbCData.getRawBuffer()
                        , bbCData.getLen()
                        , false
                    );
                }
                 else
                {
                    fReaderMgr.skipPastSpaces();
                }
            }
             else
            {
                emitError(XMLErrs::ExpectedCommentOrPI);
                fReaderMgr.skipPastChar(chCloseAngle);
            }
        }

        catch(const EndOfEntityException&)
        {
            //
            //  Some entity leaked out of the content part of the document. Issue
            //  a warning and keep going.
            //
            emitError(XMLErrs::EntityPropogated);
        }
    }
}


//
//  Scans a PI and calls the appropriate callbacks. At entry we have just
//  scanned the <? part, and need to now start on the PI target name.
//
void XMLScanner::scanPI()
{
    const XMLCh* namePtr = 0;
    const XMLCh* targetPtr = 0;

    //
    //  If there are any spaces here, then warn about it. If we aren't in
    //  'first error' mode, then we'll come back and can easily pick up
    //  again by just skipping them.
    //
    if (fReaderMgr.lookingAtSpace())
    {
        emitError(XMLErrs::PINameExpected);
        fReaderMgr.skipPastSpaces();
    }

    // Get a buffer for the PI name and scan it in
    XMLBufBid bbName(&fBufMgr);
    if (!fReaderMgr.getName(bbName.getBuffer()))
    {
        emitError(XMLErrs::PINameExpected);
        fReaderMgr.skipPastChar(chCloseAngle);
        return;
    }

    // Point the name pointer at the raw data
    namePtr = bbName.getRawBuffer();

    // See if it is some form of 'xml' and emit a warning
    if (!XMLString::compareIString(namePtr, XMLUni::fgXMLString))
        emitError(XMLErrs::NoPIStartsWithXML);

    // If namespaces are enabled, then no colons allowed
    if (fDoNamespaces)
    {
        if (XMLString::indexOf(namePtr, chColon) != -1)
            emitError(XMLErrs::ColonNotLegalWithNS);
    }

    //
    //  If we don't hit a space next, then the PI has no target. If we do
    //  then get out the target. Get a buffer for it as well
    //
    XMLBufBid bbTarget(&fBufMgr);
    if (fReaderMgr.skippedSpace())
    {
        // Skip any leading spaces
        fReaderMgr.skipPastSpaces();

        bool gotLeadingSurrogate = false;

        // It does have a target, so lets move on to deal with that.
        while (1)
        {
            const XMLCh nextCh = fReaderMgr.getNextChar();

            // Watch for an end of file, which is always bad here
            if (!nextCh)
            {
                emitError(XMLErrs::UnterminatedPI);
                ThrowXML(UnexpectedEOFException, XMLExcepts::Gen_UnexpectedEOF);
            }

            // Watch for potential terminating character
            if (nextCh == chQuestion)
            {
                // It must be followed by '>' to be a termination of the target
                if (fReaderMgr.skippedChar(chCloseAngle))
                    break;
            }

            // Check for correct surrogate pairs
            if ((nextCh >= 0xD800) && (nextCh <= 0xDBFF))
            {
                if (gotLeadingSurrogate)
                    emitError(XMLErrs::Expected2ndSurrogateChar);
                else
                    gotLeadingSurrogate = true;
            }
             else
            {
                if (gotLeadingSurrogate)
                {
                    if ((nextCh < 0xDC00) || (nextCh > 0xDFFF))
                        emitError(XMLErrs::Expected2ndSurrogateChar);
                }
                // Its got to at least be a valid XML character
                else if (!XMLReader::isXMLChar(nextCh)) {

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

                gotLeadingSurrogate = false;
            }

            bbTarget.append(nextCh);
        }
    }
     else
    {
        // No target, but make sure its terminated ok
        if (!fReaderMgr.skippedChar(chQuestion))
        {
            emitError(XMLErrs::UnterminatedPI);
            fReaderMgr.skipPastChar(chCloseAngle);
            return;
        }

        if (!fReaderMgr.skippedChar(chCloseAngle))
        {
            emitError(XMLErrs::UnterminatedPI);
            fReaderMgr.skipPastChar(chCloseAngle);
            return;
        }
    }

    // Point the target pointer at the raw data
    targetPtr = bbTarget.getRawBuffer();

    // If we have a handler, then call it
    if (fDocHandler)
    {
        fDocHandler->docPI
        (
            namePtr
            , targetPtr
       );
    }
}

//
//  Scans all the input from the start of the file to the root element.
//  There does not have to be anything in the prolog necessarily, but usually
//  there is at least an XMLDecl.
//
//  On exit from here we are either at the end of the file or about to read
//  the opening < of the root element.
//
void XMLScanner::scanProlog()
{
    // Get a buffer for whitespace processing
    XMLBufBid bbCData(&fBufMgr);

    //
    //  Loop through the prolog. If there is no content, this could go all
    //  the way to the end of the file.
    //
    //  Note that we use a double loop here to avoid the overhead of the
    //  setup/teardown of the exception handler on each loop.
    //
    while (true)
    {
    try
    {
        while (true)
        {
            const XMLCh nextCh = fReaderMgr.peekNextChar();

            if (nextCh == chOpenAngle)
            {
                //
                //  Ok, it could be the xml decl, a comment, the doc type line,
                //  or the start of the root element.
                //
                if (checkXMLDecl(true))
                {
                    // There shall be at lease --ONE-- space in between
                    // the tag '<?xml' and the VersionInfo.
                    //
                    //
                    //  If we are not at line 1, col 6, then the decl was not
                    //  the first text, so its invalid.
                    //
                    const XMLReader* curReader = fReaderMgr.getCurrentReader();
                    if ((curReader->getLineNumber() != 1)
                    ||  (curReader->getColumnNumber() != 7))
                    {
                        emitError(XMLErrs::XMLDeclMustBeFirst);
                    }

                    scanXMLDecl(Decl_XML);
                }
                 else if (fReaderMgr.skippedString(XMLUni::fgPIString))
                {
                    scanPI();
                }
                 else if (fReaderMgr.skippedString(XMLUni::fgCommentString))
                {
                    scanComment();
                }
                 else if (fReaderMgr.skippedString(XMLUni::fgDocTypeString))
                {
                    scanDocTypeDecl();

                    // if reusing grammar, this has been validated already in first scan
                    // skip for performance
                    if (fValidate && !fGrammar->getValidated()) {
                        //  validate the DTD scan so far
                        fValidator->preContentValidation(fUseCachedGrammar, true);
                    }
                }
                 else
                {
                    // Assume its the start of the root element
                    return;
                }
            }
             else if (XMLReader::isWhitespace(nextCh))
            {
                //
                //  If we have a document handler then gather up the
                //  whitespace and call back. Otherwise just skip over spaces.
                //
                if (fDocHandler)
                {
                    fReaderMgr.getSpaces(bbCData.getBuffer());
                    fDocHandler->ignorableWhitespace
                    (
                        bbCData.getRawBuffer()
                        , bbCData.getLen()
                        , false
                    );
                }
                 else
                {
                    fReaderMgr.skipPastSpaces();
                }
            }
             else
            {
                emitError(XMLErrs::InvalidDocumentStructure);
                fReaderMgr.skipPastChar(chCloseAngle);
            }

        }
    }

    catch(const EndOfEntityException&)
    {
        //
        //  We should never get an end of entity here. They should only
        //  occur within the doc type scanning method, and not leak out to
        //  here.
        //
        emitError
        (
            XMLErrs::UnexpectedEOE
            , "in prolog"
        );
    }
    }
}

//
//  This method handles the high level logic of scanning the DOCType
//  declaration. This calls the DTDScanner and kicks off both the scanning of
//  the internal subset and the scanning of the external subset, if any.
//
//  When we get here the '<!DOCTYPE' part has already been scanned, which is
//  what told us that we had a doc type decl to parse.
//

void XMLScanner::scanDocTypeDecl()
{
    //
    //  We have a doc type. So, switch the Grammar.
    //
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

    //
    //  Get a name from the input, which should be the name of the root
    //  element of the upcoming content.
    //
    fReaderMgr.getName(bbRootName.getBuffer());
    if (bbRootName.isEmpty())
    {
        emitError(XMLErrs::NoRootElemInDOCTYPE);
        fReaderMgr.skipPastChar(chCloseAngle);
        return;
    }

    //
    //  Store the root element name for later check
    //
    setRootElemName(bbRootName.getRawBuffer());

    //
    //  This element obviously is not going to exist in the element decl
    //  pool yet, but we need to call docTypeDecl. So force it into
    //  the element decl pool, marked as being there because it was in
    //  the DOCTYPE. Later, when its declared, the status will be updated.
    //
    //  Only do this if we are not reusing the validator! If we are reusing,
    //  then look it up instead. It has to exist!
    //
    DTDElementDecl* rootDecl = new DTDElementDecl(bbRootName.getRawBuffer(), fEmptyNamespaceId);

    rootDecl->setCreateReason(DTDElementDecl::AsRootElem);
    rootDecl->setExternalElemDeclaration(true);
    ((DTDGrammar*)fGrammar)->setRootElemId(fGrammar->putElemDecl(rootDecl));

    // Skip any spaces after the name
    fReaderMgr.skipPastSpaces();

    //
    //  And now if we are looking at a >, then we are done. It is not
    //  required to have an internal or external subset, though why you
    //  would not escapes me.
    //
    if (fReaderMgr.skippedChar(chCloseAngle)) {
        //
        //  If we have a doc type handler and advanced callbacks are enabled,
        //  call the doctype event.
        //
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

    DTDScanner dtdScanner((DTDGrammar*)fGrammar, fDocTypeHandler);
    dtdScanner.setScannerInfo(this, &fReaderMgr, &fBufMgr);

    //
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
        pubId = XMLString::replicate(bbPubId.getRawBuffer());
        sysId = XMLString::replicate(bbSysId.getRawBuffer());

        // Skip spaces and check again for the opening of an internal subset
        fReaderMgr.skipPastSpaces();

        // Just look at the next char, don't eat it.
        if (fReaderMgr.peekNextChar() == chOpenSquare) {
            hasIntSubset = true;
        }
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
        // Eat the opening square bracket
        fReaderMgr.getNextChar();

        // We can't have any internal subset if we are reusing the validator
        if (fUseCachedGrammar || fToCacheGrammar)
            ThrowXML(RuntimeException, XMLExcepts::Val_CantHaveIntSS);

        //
        //  And try to scan the internal subset. If we fail, try to recover
        //  by skipping forward tot he close angle and returning.
        //
        if (!dtdScanner.scanInternalSubset())
        {
            fReaderMgr.skipPastChar(chCloseAngle);
            return;
        }

        //
        //  Do a sanity check that some expanded PE did not propogate out of
        //  the doctype. This could happen if it was terminated early by bad
        //  syntax.
        //
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
        //
        //  Do a special check for the common scenario of an extra ] char at
        //  the end. This is easy to recover from.
        //
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

    //
    //  If we had an external subset, then we need to deal with that one
    //  next. If we are reusing the validator, then don't scan it.
    //
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
                rootDecl = (DTDElementDecl*) fGrammar->getElemDecl(fEmptyNamespaceId, 0, bbRootName.getRawBuffer(), Grammar::TOP_LEVEL_SCOPE);

                if (rootDecl)
                    ((DTDGrammar*)fGrammar)->setRootElemId(rootDecl->getId());
                else {
                    rootDecl = new DTDElementDecl(bbRootName.getRawBuffer(), fEmptyNamespaceId);
                    rootDecl->setCreateReason(DTDElementDecl::AsRootElem);
                    rootDecl->setExternalElemDeclaration(true);
                    ((DTDGrammar*)fGrammar)->setRootElemId(fGrammar->putElemDecl(rootDecl));
                }

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
            );

            // Put a janitor on the input source
            Janitor<InputSource> janSrc(srcUsed);

            //
            //  If it failed then throw an exception
            //
            if (!reader)
                ThrowXML1(RuntimeException, XMLExcepts::Gen_CouldNotOpenDTD, srcUsed->getSystemId());

            if (fToCacheGrammar) {

                unsigned int stringId = fGrammarResolver->getStringPool()->addOrFind(srcUsed->getSystemId());
                const XMLCh* sysIdStr = fGrammarResolver->getStringPool()->getValueForId(stringId);

                fGrammarResolver->orphanGrammar(XMLUni::fgDTDEntityString);
                fGrammarResolver->putGrammar(sysIdStr, fGrammar);
            }

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
            fReaderMgr.pushReader(reader, declDTD);

            // Tell it its not in an include section
            dtdScanner.scanExtSubsetDecl(false);
        }
    }
}

bool XMLScanner::scanStartTag(bool& gotData)
{
    //
    //  Assume we will still have data until proven otherwise. It will only
    //  ever be false if this is the root and its empty.
    //
    gotData = true;

    //
    //  Get the QName. In this case, we are not doing namespaces, so we just
    //  use it as is and don't have to break it into parts.
    //
    if (!fReaderMgr.getName(fQNameBuf))
    {
        emitError(XMLErrs::ExpectedElementName);
        fReaderMgr.skipToChar(chOpenAngle);
        return false;
    }

    // Assume it won't be an empty tag
    bool isEmpty = false;

    //
    //  Lets try to look up the element in the validator's element decl pool
    //  We can pass bogus values for the URI id and the base name. We know that
    //  this can only be called if we are doing a DTD style validator and that
    //  he will only look at the QName.
    //
    //  We tell him to fault in a decl if he does not find one.
    //
    bool wasAdded = false;
    XMLElementDecl* elemDecl = fGrammar->findOrAddElemDecl
    (
        fEmptyNamespaceId
        , 0
        , 0
        , fQNameBuf.getRawBuffer()
        , Grammar::TOP_LEVEL_SCOPE
        , wasAdded
    );

    //
    //  We do something different here according to whether we found the
    //  element or not.
    //
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

    //
    //  If this is the first element and we are validating, check the root
    //  element.
    //
    if (isRoot)
    {
        fRootGrammar = fGrammar;

        if (fValidate)
        {
            //  If a DocType exists, then check if it matches the root name there.
            if (fRootElemName && XMLString::compareString(fQNameBuf.getRawBuffer(), fRootElemName))
                fValidator->emitError(XMLValid::RootElemNotLikeDocType);

            //  Some validators may also want to check the root, call the
            //  XMLValidator::checkRootElement
            if (fValidatorFromUser && !fValidator->checkRootElement(elemDecl->getId()))
                fValidator->emitError(XMLValid::RootElemNotLikeDocType);
        }
    }
     else
    {
        //
        //  If the element stack is not empty, then add this element as a
        //  child of the previous top element. If its empty, this is the root
        //  elem and is not the child of anything.
        //
        fElemStack.addChild(elemDecl->getElementName(), true);
    }

    //
    //  Ask the element decl to clear out the 'provided' flag on all of its
    //  att defs.
    //
    elemDecl->resetDefs();

    // Skip any whitespace after the name
    fReaderMgr.skipPastSpaces();

    //
    //  We loop until we either see a /> or >, handling attribute/value
    //  pairs until we get there.
    //
    unsigned int    attCount = 0;
    unsigned int    curAttListSize = fAttrList->size();
    while (true)
    {
        // And get the next non-space character
        XMLCh nextCh = fReaderMgr.peekNextChar();

        //
        //  If the next character is not a slash or closed angle bracket,
        //  then it must be whitespace, since whitespace is required
        //  between the end of the last attribute and the name of the next
        //  one.
        //
        if (attCount)
        {
            if ((nextCh != chForwardSlash) && (nextCh != chCloseAngle))
            {
                if (XMLReader::isWhitespace(nextCh))
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

        //
        //  Ok, here we first check for any of the special case characters.
        //  If its not one, then we do the normal case processing, which
        //  assumes that we've hit an attribute value, Otherwise, we do all
        //  the special case checks.
        //
        if (!XMLReader::isSpecialStartTagChar(nextCh))
        {
            //
            //  Assume its going to be an attribute, so get a name from
            //  the input.
            //
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

                //
                //  Try to sync back up by skipping forward until we either
                //  hit something meaningful.
                //
                const XMLCh chFound = fReaderMgr.skipUntilInOrWS(tmpList);

                if ((chFound == chCloseAngle) || (chFound == chForwardSlash))
                {
                    // Jump back to top for normal processing of these
                    continue;
                }
                 else if ((chFound == chSingleQuote)
                      ||  (chFound == chDoubleQuote)
                      ||  XMLReader::isWhitespace(chFound))
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

            //
            //  See if this attribute is declared for this element. If we are
            //  not validating of course it will not be at first, but we will
            //  fault it into the pool (to avoid lots of redundant errors.)
            //
            wasAdded = false;
            XMLAttDef* attDef = elemDecl->findAttr
            (
                fAttNameBuf.getRawBuffer()
                , 0
                , 0
                , 0
                , XMLElementDecl::AddIfNotFound
                , wasAdded
            );

            if (wasAdded)
            {
                //
                //  If there is a validation handler, then we are validating
                //  so emit an error.
                //
                if (fValidate)
                {
                    // This is to tell the Validator that this attribute was
                    // faulted-in, was not an attribute in the attdef originally
                    attDef->setCreateReason(XMLAttDef::JustFaultIn);

                    fValidator->emitError
                    (
                        XMLValid::AttNotDefinedForElement
                        , fAttNameBuf.getRawBuffer()
                        , elemDecl->getFullName()
                    );
                }
            }
            else
            {
                // If this attribute was faulted-in and first occurence,
                // then emit an error
                if (fValidate && attDef->getCreateReason() == XMLAttDef::JustFaultIn
                    && !attDef->getProvided())
                {
                    fValidator->emitError
                    (
                        XMLValid::AttNotDefinedForElement
                        , fAttNameBuf.getRawBuffer()
                        , elemDecl->getFullName()
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
                    , elemDecl->getFullName()
                );
            }
             else
            {
                // Mark this one as already seen
                attDef->setProvided(true);
            }

            //
            //  Skip any whitespace before the value and then scan the att
            //  value. This will come back normalized with entity refs and
            //  char refs expanded.
            //
            fReaderMgr.skipPastSpaces();
            if (!scanAttValue(attDef, fAttValueBuf))
            {
                static const XMLCh tmpList[] =
                {
                    chCloseAngle, chOpenAngle, chForwardSlash, chNull
                };

                emitError(XMLErrs::ExpectedAttrValue);

                //
                //  It failed, so lets try to get synced back up. We skip
                //  forward until we find some whitespace or one of the
                //  chars in our list.
                //
                const XMLCh chFound = fReaderMgr.skipUntilInOrWS(tmpList);

                if ((chFound == chCloseAngle)
                ||  (chFound == chForwardSlash)
                ||  XMLReader::isWhitespace(chFound))
                {
                    //
                    //  Just fall through and process this attribute, though
                    //  the value will be "".
                    //
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

            //
            //  Now that its all stretched out, lets look at its type and
            //  determine if it has a valid value. It will output any needed
            //  errors, but we just keep going. We only need to do this if
            //  we are validating.
            //
            if (!wasAdded && attDef->getCreateReason() != XMLAttDef::JustFaultIn)
            {
                // Let the validator pass judgement on the attribute value
                if (fValidate)
                {
                    fValidator->validateAttrValue
                    (
                        attDef
                        , fAttValueBuf.getRawBuffer()
                    );
                }
            }

            //
            //  Add this attribute to the attribute list that we use to
            //  pass them to the handler. We reuse its existing elements
            //  but expand it as required.
            //
            XMLAttr* curAtt;
            if (attCount >= curAttListSize)
            {
                curAtt = new XMLAttr
                (
                    -1
                    , fAttNameBuf.getRawBuffer()
                    , XMLUni::fgZeroLenString
                    , fAttValueBuf.getRawBuffer()
                    , attDef->getType()
                    , true
                );
                fAttrList->addElement(curAtt);
            }
             else
            {
                curAtt = fAttrList->elementAt(attCount);
                curAtt->set
                (
                    -1
                    , fAttNameBuf.getRawBuffer()
                    , XMLUni::fgZeroLenString
                    , fAttValueBuf.getRawBuffer()
                    , attDef->getType()
                );
                curAtt->setSpecified(true);
            }
            attCount++;

            // And jump back to the top of the loop
            continue;
        }

        //
        //  It was some special case character so do all of the checks and
        //  deal with it.
        //
        if (!nextCh)
            ThrowXML(UnexpectedEOFException, XMLExcepts::Gen_UnexpectedEOF);

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
            //
            //  Check for this one specially, since its going to be common
            //  and it is kind of auto-recovering since we've already hit the
            //  next open bracket, which is what we would have seeked to (and
            //  skipped this whole tag.)
            //
            emitError(XMLErrs::UnterminatedStartTag, elemDecl->getFullName());
            break;
        }
         else if ((nextCh == chSingleQuote) || (nextCh == chDoubleQuote))
        {
            //
            //  Check for this one specially, which is probably a missing
            //  attribute name, e.g. ="value". Just issue expected name
            //  error and eat the quoted string, then jump back to the
            //  top again.
            //
            emitError(XMLErrs::ExpectedAttrName);
            fReaderMgr.getNextChar();
            fReaderMgr.skipQuotedString(nextCh);
            fReaderMgr.skipPastSpaces();
            continue;
        }
    }

    //
    //  Ok, so lets get an enumerator for the attributes of this element
    //  and run through them for well formedness and validity checks. But
    //  make sure that we had any attributes before we do it, since the list
    //  would have have gotten faulted in anyway.
    //
    if (elemDecl->hasAttDefs())
    {
        XMLAttDefList& attDefList = elemDecl->getAttDefList();
        while (attDefList.hasMoreElements())
        {
            // Get the current att def, for convenience and its def type
            const XMLAttDef& curDef = attDefList.nextElement();
            const XMLAttDef::DefAttTypes defType = curDef.getDefaultType();

            if (!curDef.getProvided())
            {
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
                            //
                            // XML 1.0 Section 2.9
                            // Document is standalone, so attributes must not be defaulted.
                            //
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
                        );
                    }

                    XMLAttr* curAtt;
                    if (attCount >= curAttListSize)
                    {
                        curAtt = new XMLAttr
                        (
                            -1
                            , curDef.getFullName()
                            , XMLUni::fgZeroLenString
                            , curDef.getValue()
                            , curDef.getType()
                            , false
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

    //
    //  If empty, validate content right now if we are validating and then
    //  pop the element stack top. Else, we have to update the current stack
    //  top's namespace mapping elements.
    //
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

    //
    //  If we have a document handler, then tell it about this start tag. We
    //  don't have any URI id to send along, so send fEmptyNamespaceId. We also do not send
    //  any prefix since its just one big name if we are not doing namespaces.
    //
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


//
//
//  This method is called to scan a start tag when we are processing
//  namespaces. There are two different versions of this method, one for
//  namespace aware processing an done for non-namespace aware processing.
//
//  This method is called after we've scanned the < of a start tag. So we
//  have to get the element name, then scan the attributes, after which
//  we are either going to see >, />, or attributes followed by one of those
//  sequences.
//
bool XMLScanner::scanStartTagNS(bool& gotData)
{
    //
    //  Assume we will still have data until proven otherwise. It will only
    //  ever be false if this is the root and its empty.
    //
    gotData = true;

    //
    //  The current position is after the open bracket, so we need to read in
    //  in the element name.
    //
    if (!fReaderMgr.getName(fQNameBuf))
    {
        emitError(XMLErrs::ExpectedElementName);
        fReaderMgr.skipToChar(chOpenAngle);
        return false;
    }

    //
    //  Do a little sanity check here. One common problem is that
    //  badly encoded files cause getName() to exit above on a
    //  non-name char (an invalid XML char), then the scan start
    //  tag below fails. This is misleading, so check here that
    //  we are looking at a valid XML char.
    //
    if (!XMLReader::isXMLChar(fReaderMgr.peekNextChar()))
    {
        XMLCh tmpBuf[9];
        XMLString::binToText
        (
            fReaderMgr.getNextChar()
            , tmpBuf
            , 8
            , 16
        );
        emitError(XMLErrs::InvalidCharacter, tmpBuf);
    }

    // See if its the root element
    const bool isRoot = fElemStack.isEmpty();

    // Skip any whitespace after the name
    fReaderMgr.skipPastSpaces();

    //
    //  First we have to do the rawest attribute scan. We don't do any
    //  normalization of them at all, since we don't know yet what type they
    //  might be (since we need the element decl in order to do that.)
    //
    bool isEmpty;
    unsigned int attCount = rawAttrScan
    (
        fQNameBuf.getRawBuffer()
        , *fRawAttrList
        , isEmpty
    );
    const bool gotAttrs = (attCount != 0);

    // save the contentleafname and currentscope before addlevel, for later use
    ContentLeafNameTypeVector* cv = 0;
    XMLContentModel* cm = 0;
    int currentScope = Grammar::TOP_LEVEL_SCOPE;
    if (!isRoot && fGrammarType == Grammar::SchemaGrammarType) {
        SchemaElementDecl* tempElement = (SchemaElementDecl*) fElemStack.topElement()->fThisElement;
        SchemaElementDecl::ModelTypes modelType = tempElement->getModelType();

        if ((modelType == SchemaElementDecl::Mixed_Simple)
          ||  (modelType == SchemaElementDecl::Mixed_Complex)
          ||  (modelType == SchemaElementDecl::Children))
        {
            cm = tempElement->getContentModel();
            cv = cm->getContentLeafNameTypeVector();
            currentScope = fElemStack.getCurrentScope();
        }
    }

    //
    //  Now, since we might have to update the namespace map for this element,
    //  but we don't have the element decl yet, we just tell the element stack
    //  to expand up to get ready.
    //
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

    //
    //  Make an initial pass through the list and find any xmlns attributes or
    //  schema attributes.
    //
    if (attCount)
      scanRawAttrListforNameSpaces(fRawAttrList, attCount);

    //
    //  Also find any default or fixed xmlns attributes in DTD defined for
    //  this element.
    //
    if (fGrammarType == Grammar::DTDGrammarType) {
        XMLElementDecl* elemDecl = fGrammar->getElemDecl
        (
            fEmptyNamespaceId
            , 0
            , fQNameBuf.getRawBuffer()
            , Grammar::TOP_LEVEL_SCOPE
        );
        if (elemDecl) {
            if (elemDecl->hasAttDefs()) {
                XMLAttDefList& attDefList = elemDecl->getAttDefList();
                while (attDefList.hasMoreElements())
                {
                    // Get the current att def, for convenience and its def type
                    const XMLAttDef& curDef = attDefList.nextElement();
                    const XMLAttDef::DefAttTypes defType = curDef.getDefaultType();

                    // update the NSMap if there are any default/fixed xmlns attributes
                    if ((defType == XMLAttDef::Default)
                    ||  (defType == XMLAttDef::Fixed))
                    {
                        const XMLCh* rawPtr = curDef.getFullName();
                        if (!XMLString::compareNString(rawPtr, XMLUni::fgXMLNSColonString, 6)
                        ||  !XMLString::compareString(rawPtr, XMLUni::fgXMLNSString))
                            updateNSMap(rawPtr, curDef.getValue());
                    }
                }
            }
        }
    }

    //
    //  Resolve the qualified name to a URI and name so that we can look up
    //  the element decl for this element. We have now update the prefix to
    //  namespace map so we should get the correct element now.
    //
    unsigned int uriId = resolveQName
        (
            fQNameBuf.getRawBuffer()
            , fNameBuf
            , fPrefixBuf
            , ElemStack::Mode_Element
        );

    //if schema, check if we should lax or skip the validation of this element
    bool laxThisOne = false;
    if (cv) {
        QName element(fPrefixBuf.getRawBuffer(), fNameBuf.getRawBuffer(), uriId);
        // elementDepth will be > 0, as cv is only constructed if element is not
        // root.
        laxThisOne = laxElementValidation(&element, cv, cm, elemDepth - 1);
    }

    //
    //  Look up the element now in the grammar. This will get us back a
    //  generic element decl object. We tell him to fault one in if he does
    //  not find it.
    //
    bool wasAdded = false;
    XMLElementDecl* elemDecl;
    const XMLCh* nameRawBuf = fNameBuf.getRawBuffer();
    const XMLCh* qnameRawBuf = fQNameBuf.getRawBuffer();

    if (uriId != fEmptyNamespaceId) {

        // Check in current grammar before switching if necessary
        elemDecl = fGrammar->getElemDecl
        (
          uriId
          , nameRawBuf
          , qnameRawBuf
          , currentScope
        );

        if (!elemDecl && (fURIStringPool->getId(fGrammar->getTargetNamespace()) != uriId)) {
            // not found, switch to the specified grammar
            const XMLCh* uriStr = getURIText(uriId);
            if (!switchGrammar(uriStr) && fValidate && !laxThisOne)
            {
                fValidator->emitError
                (
                    XMLValid::GrammarNotFound
                    ,uriStr
                );
            }

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

                if (elemDecl && elemDecl->getCreateReason() != XMLElementDecl::JustFaultIn && fValidate) {
                    fValidator->emitError
                    (
                        XMLValid::ElementNotUnQualified
                        , elemDecl->getFullName()
                    );
                }
            }
        }

        if (!elemDecl) {
            // still not found, fault this in and issue error later
            elemDecl = fGrammar->putElemDecl(uriId
                        , nameRawBuf
                        , fPrefixBuf.getRawBuffer()
                        , qnameRawBuf
                        , currentScope
                        , true);
            wasAdded = true;
        }
    }
    else
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

        unsigned orgGrammarUri = fURIStringPool->getId(fGrammar->getTargetNamespace());

        if (!elemDecl && orgGrammarUri != fEmptyNamespaceId) {
            //not found, switch grammar and try globalNS
            if (!switchGrammar(XMLUni::fgZeroLenString) && fValidate && !laxThisOne)
            {
                fValidator->emitError
                (
                    XMLValid::GrammarNotFound
                  , XMLUni::fgZeroLenString
                );
            }

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

            if (!elemDecl && orgGrammarUri != fEmptyNamespaceId) {
                // still Not found in specified uri
                // go to original Grammar again to see if element needs to be fully qualified.
                const XMLCh* uriStr = getURIText(orgGrammarUri);
                if (!switchGrammar(uriStr) && fValidate && !laxThisOne)
                {
                    fValidator->emitError
                    (
                        XMLValid::GrammarNotFound
                        ,uriStr
                    );
                }

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
                }
            }
        }

        if (!elemDecl) {
            // still not found, fault this in and issue error later
            elemDecl = fGrammar->putElemDecl(uriId
                        , nameRawBuf
                        , fPrefixBuf.getRawBuffer()
                        , qnameRawBuf
                        , currentScope
                        , true);
            wasAdded = true;
        }
    }

    //
    //  We do something different here according to whether we found the
    //  element or not.
    //
    if (wasAdded)
    {
        if (laxThisOne) {
            fValidate = false;
            fElemStack.setValidationFlag(fValidate);
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
        }
    }
     else
    {
        // If its not marked declared and validating, then emit an error
        if (!elemDecl->isDeclared()) {
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

        if (fGrammarType == Grammar::SchemaGrammarType)
            ((SchemaElementDecl*)elemDecl)->setXsiComplexTypeInfo(0);
    }

    //
    //  Now we can update the element stack to set the current element
    //  decl. We expanded the stack above, but couldn't store the element
    //  decl because we didn't know it yet.
    //
    fElemStack.setElement(elemDecl, fReaderMgr.getCurrentReaderNum());
    fElemStack.setCurrentURI(uriId);

    if (isRoot)
        fRootGrammar = fGrammar;

    //  Validate the element
    if (fValidate)
        fValidator->validateElement(elemDecl);

    if (fGrammarType == Grammar::SchemaGrammarType) {
        ComplexTypeInfo* typeinfo = ((SchemaElementDecl*)elemDecl)->getComplexTypeInfo();
        if (typeinfo) {
            currentScope = typeinfo->getScopeDefined();

            // switch grammar if the typeinfo has a different grammar (happens when there is xsi:type)
            XMLCh* typeName = typeinfo->getTypeName();
            const XMLCh poundStr[] = {chPound, chNull};
            if (!XMLString::startsWith(typeName, poundStr)) {
                const int comma = XMLString::indexOf(typeName, chComma);
                if (comma > 0) {
                    XMLBuffer prefixBuf(comma+1);
                    prefixBuf.append(typeName, comma);
                    const XMLCh* uriStr = prefixBuf.getRawBuffer();
                    if (!switchGrammar(uriStr) && fValidate && !laxThisOne)
                    {
                        fValidator->emitError
                        (
                            XMLValid::GrammarNotFound
                            , prefixBuf.getRawBuffer()
                        );
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

    //
    //  If this is the first element and we are validating, check the root
    //  element.
    //
    if (isRoot)
    {
        if (fValidate)
        {
            //  If a DocType exists, then check if it matches the root name there.
            if (fRootElemName && XMLString::compareString(qnameRawBuf, fRootElemName))
                fValidator->emitError(XMLValid::RootElemNotLikeDocType);

            //  Some validators may also want to check the root, call the
            //  XMLValidator::checkRootElement
            if (fValidatorFromUser && !fValidator->checkRootElement(elemDecl->getId()))
                fValidator->emitError(XMLValid::RootElemNotLikeDocType);
        }
    }
     else
    {
        //
        //  If the element stack is not empty, then add this element as a
        //  child of the previous top element. If its empty, this is the root
        //  elem and is not the child of anything.
        //
        fElemStack.addChild(elemDecl->getElementName(), true);
    }

    //
    //  Now lets get the fAttrList filled in. This involves faulting in any
    //  defaulted and fixed attributes and normalizing the values of any that
    //  we got explicitly.
    //
    //  We update the attCount value with the total number of attributes, but
    //  it goes in with the number of values we got during the raw scan of
    //  explictly provided attrs above.
    //
    attCount = buildAttList(*fRawAttrList, attCount, elemDecl, *fAttrList);

    //
    // activate identity constraints
    //
    if (fValidate && fGrammar && fGrammarType == Grammar::SchemaGrammarType) {

        unsigned int count = ((SchemaElementDecl*) elemDecl)->getIdentityConstraintCount();

        if (count || fMatcherStack->getMatcherCount()) {

            fValueStoreCache->startElement();
            fMatcherStack->pushContext();
            fValueStoreCache->initValueStoresFor((SchemaElementDecl*) elemDecl);

            for (unsigned int i = 0; i < count; i++) {
                activateSelectorFor(((SchemaElementDecl*) elemDecl)->getIdentityConstraintAt(i));
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

    //
    //  If empty, validate content right now if we are validating and then
    //  pop the element stack top. Else, we have to update the current stack
    //  top's namespace mapping elements.
    //
    if (isEmpty)
    {
        // Pop the element stack back off since it'll never be used now
        fElemStack.popTop();

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

                // reset xsi:type ComplexTypeInfo
                ((SchemaElementDecl*)elemDecl)->setXsiComplexTypeInfo(0);

                // call matchers and de-activate context
                int oldCount = fMatcherStack->getMatcherCount();

                if (oldCount || ((SchemaElementDecl*) elemDecl)->getIdentityConstraintCount()) {

                    for (int i = oldCount - 1; i >= 0; i--) {

                        XPathMatcher* matcher = fMatcherStack->getMatcherAt(i);
                        matcher->endElement(*elemDecl);
                    }

                    if (fMatcherStack->size() > 0) {
                        fMatcherStack->popContext();
                    }

                    // handle everything *but* keyref's.
                    int newCount = fMatcherStack->getMatcherCount();

                    for (int j = oldCount - 1; j >= newCount; j--) {

                        XPathMatcher* matcher = fMatcherStack->getMatcherAt(j);
                        IdentityConstraint* ic = matcher->getIdentityConstraint();

                        if (ic  && (ic->getType() != IdentityConstraint::KEYREF)) {

                            matcher->endDocumentFragment();
                            fValueStoreCache->transplant(ic);
                        }
                        else if (!ic) {
                            matcher->endDocumentFragment();
                        }
                    }

                    // now handle keyref's...
                    for (int k = oldCount - 1; k >= newCount; k--) {

                        XPathMatcher* matcher = fMatcherStack->getMatcherAt(k);
                        IdentityConstraint* ic = matcher->getIdentityConstraint();

                        if (ic && (ic->getType() == IdentityConstraint::KEYREF)) {

                            ValueStore* values = fValueStoreCache->getValueStoreFor(ic);

                            if (values) { // nothing to do if nothing matched!
                                values->endDcocumentFragment(fValueStoreCache);
                            }

                            matcher->endDocumentFragment();
                        }
                    }

                    fValueStoreCache->endElement();
                }
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
                    ThrowXML(RuntimeException, XMLExcepts::Gen_NoSchemaValidator);
                else {
                    fValidator = fSchemaValidator;
                }
            }
            else if (fGrammarType == Grammar::DTDGrammarType && !fValidator->handlesDTD()) {
                if (fValidatorFromUser)
                    ThrowXML(RuntimeException, XMLExcepts::Gen_NoDTDValidator);
                else {
                    fValidator = fDTDValidator;
                }
            }

            fValidator->setGrammar(fGrammar);

            // Restore the validation flag
            fValidate = fElemStack.getValidationFlag();
        }
    }

    return true;
}


//
//  Scans the <?xml .... ?> line. This stuff is all sequential so we don't
//  do any state machine loop here. We just bull straight through it. It ends
//  past the closing bracket. If there is a document handler, then its called
//  on the XMLDecl callback.
//
//  On entry, the <?xml has been scanned, and we pick it up from there.
//
//  NOTE: In order to provide good recovery from bad XML here, we try to be
//  very flexible. No matter what order the stuff is in, we'll keep going
//  though we'll issue errors.
//
//  The parameter tells us which type of decl we should expect, Text or XML.
//    [23] XMLDecl ::= '<?xml' VersionInfo EncodingDecl? SDDecl? S? '?>'
//    [77] TextDecl::= '<?xml' VersionInfo? EncodingDecl S? '?>'
//
void XMLScanner::scanXMLDecl(const DeclTypes type)
{
    // Get us some buffers to use
    XMLBufBid bbVersion(&fBufMgr);
    XMLBufBid bbEncoding(&fBufMgr);
    XMLBufBid bbStand(&fBufMgr);
    XMLBufBid bbDummy(&fBufMgr);
    XMLBufBid bbName(&fBufMgr);

    //
    //  We use this little enum and array to keep up with what we found
    //  and what order we found them in. This lets us get them free form
    //  without too much overhead, but still know that they were in the
    //  wrong order.
    //
    enum Strings
    {
        VersionString
        , EncodingString
        , StandaloneString
        , UnknownString

        , StringCount
    };
    int flags[StringCount] = { -1, -1, -1, -1 };

    //
    //  Also set up a list of buffers in the right order so that we know
    //  where to put stuff.
    //
    XMLBuffer* buffers[StringCount] ;
    buffers[0] = &bbVersion.getBuffer();
    buffers[1] = &bbEncoding.getBuffer();
    buffers[2] = &bbStand.getBuffer();
    buffers[3] = &bbDummy.getBuffer();

    int curCount = 0;
    Strings curString;
    XMLBuffer& nameBuf = bbName.getBuffer();
    while (true)
    {
        // Skip any spaces
        const unsigned int spaceCount = fReaderMgr.skipPastSpaces();

        // If we are looking at a question mark, then break out
        if (fReaderMgr.lookingAtChar(chQuestion))
            break;

        // If this is not the first string, then we require the spaces
        if (!spaceCount && curCount)
            emitError(XMLErrs::ExpectedWhitespace);

        //
        //  Get characters up to the next whitespace or equal's sign.
        //
        if (!scanUpToWSOr(nameBuf, chEqual))
            emitError(XMLErrs::ExpectedDeclString);

        // See if it matches any of our expected strings
        if (!XMLString::compareString(nameBuf.getRawBuffer(), XMLUni::fgVersionString))
            curString = VersionString;
        else if (!XMLString::compareString(nameBuf.getRawBuffer(), XMLUni::fgEncodingString))
            curString = EncodingString;
        else if (!XMLString::compareString(nameBuf.getRawBuffer(), XMLUni::fgStandaloneString))
            curString = StandaloneString;
        else
            curString = UnknownString;

        //
        //  If its an unknown string, then give that error. Else check to
        //  see if this one has been done already and give that error.
        //
        if (curString == UnknownString)
            emitError(XMLErrs::ExpectedDeclString, nameBuf.getRawBuffer());
        else if (flags[curString] != -1)
            emitError(XMLErrs::DeclStringRep, nameBuf.getRawBuffer());
        else if (flags[curString] == -1)
            flags[curString] = ++curCount;

        //
        //  Scan for an equal's sign. If we don't find it, issue an error
        //  but keep trying to go on.
        //
        if (!scanEq())
            emitError(XMLErrs::ExpectedEqSign);

        //
        //  Get a quote string into the buffer for the string that we are
        //  currently working on.
        //
        if (!getQuotedString(*buffers[curString]))
        {
            emitError(XMLErrs::ExpectedQuotedString);
            fReaderMgr.skipPastChar(chCloseAngle);
            return;
        }

        // And validate the value according which one it was
        const XMLCh* rawValue = buffers[curString]->getRawBuffer();
        if (curString == VersionString)
        {
            if (XMLString::compareString(rawValue, XMLUni::fgSupportedVersion))
                emitError(XMLErrs::UnsupportedXMLVersion, rawValue);
        }
         else if (curString == EncodingString)
        {
            if (!XMLString::isValidEncName(rawValue))
                emitError(XMLErrs::BadXMLEncoding, rawValue);
        }
         else if (curString == StandaloneString)
        {
            if (!XMLString::compareString(rawValue, XMLUni::fgYesString))
                fStandalone = true;
            else if (!XMLString::compareString(rawValue, XMLUni::fgNoString))
                fStandalone = false;
            else
            {
                emitError(XMLErrs::BadStandalone);
                if (!XMLString::compareIString(rawValue, XMLUni::fgYesString))
                    fStandalone = true;
                else if (!XMLString::compareIString(rawValue, XMLUni::fgNoString))
                    fStandalone = false;
            }
        }
    }

    //
    //  Make sure that the strings present are in order. We don't care about
    //  which ones are present at this point, just that any there are in the
    //  right order.
    //
    int curTop = 0;
    for (int index = VersionString; index < StandaloneString; index++)
    {
        if (flags[index] != -1)
        {
            if (flags[index] !=  curTop + 1)
            {
                emitError(XMLErrs::DeclStringsInWrongOrder);
                break;
            }
            curTop = flags[index];
        }
    }

    //
    //  If its an XML decl, the version must be present.
    //  If its a Text decl, then encoding must be present AND standalone must not be present.
    //
    if ((type == Decl_XML) && (flags[VersionString] == -1))
        emitError(XMLErrs::XMLVersionRequired);
    else if (type == Decl_Text) {
        if (flags[StandaloneString] != -1)
            emitError(XMLErrs::StandaloneNotLegal);
        if (flags[EncodingString] == -1)
            emitError(XMLErrs::EncodingRequired);
    }

    if (!fReaderMgr.skippedChar(chQuestion))
    {
        emitError(XMLErrs::UnterminatedXMLDecl);
        fReaderMgr.skipPastChar(chCloseAngle);
    }
     else if (!fReaderMgr.skippedChar(chCloseAngle))
    {
        emitError(XMLErrs::UnterminatedXMLDecl);
        fReaderMgr.skipPastChar(chCloseAngle);
    }

    //  Do this before we possibly update the reader with the
    //  actual encoding string. Otherwise, we will pass the wrong thing
    //  for the last parameter!
    const XMLCh* actualEnc = fReaderMgr.getCurrentEncodingStr();

    //
    //  Ok, we've now seen the real encoding string, if there was one, so
    //  lets call back on the current reader and tell it what the real
    //  encoding string was. If it fails, that's because it represents some
    //  sort of contradiction with the autosensed format, and it keeps the
    //  original encoding.
    //
    //  NOTE: This can fail for a number of reasons, such as a bogus encoding
    //  name or because its in flagrant contradiction of the auto-sensed
    //  format.
    //
    if (flags[EncodingString] != -1)
    {
        if (!fReaderMgr.getCurrentReader()->setEncoding(bbEncoding.getRawBuffer()))
            emitError(XMLErrs::ContradictoryEncoding, bbEncoding.getRawBuffer());
        else
            actualEnc = bbEncoding.getRawBuffer();
    }

    //
    //  If we have a document handler then call the XML Decl callback.
    //
    //
    if (fDocHandler)
    {
        fDocHandler->XMLDecl
        (
            bbVersion.getRawBuffer()
            , bbEncoding.getRawBuffer()
            , bbStand.getRawBuffer()
            , actualEnc
        );
    }
}

const XMLCh* XMLScanner::getURIText(const   unsigned int    uriId) const
{
    if (fURIStringPool->exists(uriId)) {
        // Look up the URI in the string pool and return its id
        const XMLCh* value = fURIStringPool->getValueForId(uriId);
        if (!value)
            return XMLUni::fgZeroLenString;

        return value;
    }
    else
        return XMLUni::fgZeroLenString;
}

bool XMLScanner::getURIText(  const   unsigned int    uriId
                      ,       XMLBuffer&      uriBufToFill) const
{
    if (fURIStringPool->exists(uriId)) {
        // Look up the URI in the string pool and return its id
        const XMLCh* value = fURIStringPool->getValueForId(uriId);
        if (!value)
            return false;

        uriBufToFill.set(value);
        return true;
    }
    else
        return false;
}

unsigned int
XMLScanner::resolveQName(   const   XMLCh* const        qName
                            ,       XMLBuffer&          nameBuf
                            ,       XMLBuffer&          prefixBuf
                            , const ElemStack::MapModes mode)
{
    // Reset both target buffers in case we don't get anything for either
    nameBuf.reset();
    prefixBuf.reset();

    //
    //  Lets split out the qName into a URI and name buffer first. The URI
    //  can be empty.
    //
    const int colonPos = XMLString::indexOf(qName, chColon);
    unsigned int uriId = 0;
    if (colonPos == -1)
    {
        //
        //  Its all name with no prefix, so put the whole thing into the name
        //  buffer. Then map the empty string to a URI, since the empty string
        //  represents the default namespace. This will either return some
        //  explicit URI which the default namespace is mapped to, or the
        //  the default global namespace.
        //
        nameBuf.append(qName);
        bool unknown;
        uriId = fElemStack.mapPrefixToURI(prefixBuf.getRawBuffer(), mode, unknown);

        #if defined(XERCES_DEBUG)
        if (unknown)
        {
            // <TBD> This one should never be unknown
        }
        #endif
    }
     else
    {
        //
        //  Copy the chars up to but not including the colon into the prefix
        //  buffer.
        //
        prefixBuf.append(qName, colonPos);

        // And copy over the rest of the chars to the name buffer
        nameBuf.append(&qName[colonPos+1]);

        //
        //  Watch for the special namespace prefixes. We always map these to
        //  special URIs. 'xml' gets mapped to the official URI that its defined
        //  to map to by the NS spec. xmlns gets mapped to a special place holder
        //  URI that we define (so that it maps to something checkable.)
        //
        if (!XMLString::compareString(prefixBuf.getRawBuffer(), XMLUni::fgXMLNSString))
            uriId = fXMLNSNamespaceId;
        else if (!XMLString::compareString(prefixBuf.getRawBuffer(), XMLUni::fgXMLString))
            uriId = fXMLNamespaceId;
        else
        {
            bool unknown;
            uriId = fElemStack.mapPrefixToURI(prefixBuf.getRawBuffer(), mode, unknown);
            if (unknown)
                emitError(XMLErrs::UnknownPrefix, prefixBuf.getRawBuffer());
        }
    }
    return uriId;
}

bool XMLScanner::checkXMLDecl(bool startWithAngle) {
    //
    // [23] XMLDecl     ::= '<?xml' VersionInfo EncodingDecl? SDDecl? S? '?>'
    // [24] VersionInfo ::= S 'version' Eq ("'" VersionNum "'" | '"' VersionNum '"')
    //
    // [3]  S           ::= (#x20 | #x9 | #xD | #xA)+
    //

    if (startWithAngle) {
        if (fReaderMgr.peekString(XMLUni::fgXMLDeclString)) {
            if (fReaderMgr.skippedString(XMLUni::fgXMLDeclStringSpace)
               || fReaderMgr.skippedString(XMLUni::fgXMLDeclStringHTab)
               || fReaderMgr.skippedString(XMLUni::fgXMLDeclStringLF)
               || fReaderMgr.skippedString(XMLUni::fgXMLDeclStringCR))
            {
                return true;
            }
            else if (fReaderMgr.skippedString(XMLUni::fgXMLDeclStringSpaceU)
               || fReaderMgr.skippedString(XMLUni::fgXMLDeclStringHTabU)
               || fReaderMgr.skippedString(XMLUni::fgXMLDeclStringLFU)
               || fReaderMgr.skippedString(XMLUni::fgXMLDeclStringCRU))
            {
                //
                //  Just in case, check for upper case. If found, issue
                //  an error, but keep going.
                //
                emitError(XMLErrs::XMLDeclMustBeLowerCase);
                return true;
            }
        }
    }
    else {
        if (fReaderMgr.peekString(XMLUni::fgXMLString)) {
            if (fReaderMgr.skippedString(XMLUni::fgXMLStringSpace)
               || fReaderMgr.skippedString(XMLUni::fgXMLStringHTab)
               || fReaderMgr.skippedString(XMLUni::fgXMLStringLF)
               || fReaderMgr.skippedString(XMLUni::fgXMLStringCR))
            {
                return true;
            }
            else if (fReaderMgr.skippedString(XMLUni::fgXMLStringSpaceU)
               || fReaderMgr.skippedString(XMLUni::fgXMLStringHTabU)
               || fReaderMgr.skippedString(XMLUni::fgXMLStringLFU)
               || fReaderMgr.skippedString(XMLUni::fgXMLStringCRU))
            {
                //
                //  Just in case, check for upper case. If found, issue
                //  an error, but keep going.
                //
                emitError(XMLErrs::XMLDeclMustBeLowerCase);
                return true;
            }
        }
    }

    return false;
}


// ---------------------------------------------------------------------------
//  XMLScanner: Helper methos
// ---------------------------------------------------------------------------
void XMLScanner::resizeElemState() {

    unsigned int newSize = fElemStateSize * 2;
    unsigned int* newElemState = new unsigned int[newSize];

    // Copy the existing values
    unsigned int index = 0;
    for (; index < fElemStateSize; index++)
        newElemState[index] = fElemState[index];

    for (; index < newSize; index++)
        newElemState[index] = 0;

    // Delete the old array and udpate our members
    delete [] fElemState;
    fElemState = newElemState;
    fElemStateSize = newSize;
}

// ---------------------------------------------------------------------------
//  XMLScanner: IC activation methos
// ---------------------------------------------------------------------------
void XMLScanner::activateSelectorFor(IdentityConstraint* const ic) {

    IC_Selector* selector = ic->getSelector();

    if (!selector)
        return;

    XPathMatcher* matcher = selector->createMatcher(fFieldActivator);

    fMatcherStack->addMatcher(matcher);
    matcher->startDocumentFragment();
}

// ---------------------------------------------------------------------------
//  XMLScanner: Grammar preparsing
// ---------------------------------------------------------------------------
Grammar* XMLScanner::loadGrammar(const   XMLCh* const systemId
                                 , const short        grammarType
                                 , const bool         toCache)
{
    InputSource* srcToUse = 0;

    if (fEntityHandler){
        srcToUse = fEntityHandler->resolveEntity(XMLUni::fgZeroLenString, systemId);
    }

    //
    //  First we try to parse it as a URL. If that fails, we assume its
    //  a file and try it that way.
    //
    if (!srcToUse) {

        try
        {
            //
            //  Create a temporary URL. Since this is the primary document,
            //  it has to be fully qualified. If not, then assume we are just
            //  mistaking a file for a URL.
            //
            XMLURL tmpURL(systemId);
            if (tmpURL.isRelative()) {
                srcToUse = new LocalFileInputSource(systemId);
            }
            else
            {
                srcToUse = new URLInputSource(tmpURL);
            }
        }
        catch(const MalformedURLException&)
        {
            srcToUse = new LocalFileInputSource(systemId);
        }
        catch(...)
        {
            // Just rethrow this, since its not our problem
            throw;
        }
    }

    Janitor<InputSource> janSrc(srcToUse);
    return loadGrammar(*srcToUse, grammarType, toCache);
}

Grammar* XMLScanner::loadGrammar(const   char* const systemId
                                 , const short       grammarType
                                 , const bool        toCache)
{
    // We just delegate this to the XMLCh version after transcoding
    XMLCh* tmpBuf = XMLString::transcode(systemId);
    ArrayJanitor<XMLCh> janBuf(tmpBuf);
    return loadGrammar(tmpBuf, grammarType, toCache);
}


Grammar* XMLScanner::loadGrammar(const   InputSource& src
                                 , const short        grammarType
                                 , const bool         toCache)
{
    try
    {
        fGrammarResolver->cacheGrammarFromParse(false);
        fGrammarResolver->useCachedGrammarInParse(false);
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
            return loadXMLSchemaGrammar(src, toCache);
        }
        else if (grammarType == Grammar::DTDGrammarType) {
            return loadDTDGrammar(src, toCache);
        }

        // Reset the reader manager to close all files, sockets, etc...
        fReaderMgr.reset();
    }

    //
    //  NOTE:
    //
    //  In all of the error processing below, the emitError() call MUST come
    //  before the flush of the reader mgr, or it will fail because it tries
    //  to find out the position in the XML source of the error.
    //
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
        //
        //  Emit the error and catch any user exception thrown from here. Make
        //  sure in all cases we flush the reader manager.
        //
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

        catch(...)
        {
            // Flush the reader manager and rethrow user's error
            fReaderMgr.reset();
            throw;
        }

        // If it returned, then reset the reader manager and fall through
        fReaderMgr.reset();
    }

    catch(...)
    {
        // Reset and rethrow
        fReaderMgr.reset();
        throw;
    }

    return 0;
}

Grammar* XMLScanner::loadDTDGrammar(const InputSource& src,
                                    const bool toCache)
{
   // Reset the validators
    fDTDValidator->reset();
    if (fValidatorFromUser)
        fValidator->reset();

    if (!fValidator->handlesDTD()) {
        if (fValidatorFromUser && fValidate)
            ThrowXML(RuntimeException, XMLExcepts::Gen_NoDTDValidator);
        else {
            fValidator = fDTDValidator;
        }
    }

    fDTDGrammar = new DTDGrammar();
    fGrammarResolver->putGrammar(XMLUni::fgDTDEntityString, fDTDGrammar);
    fGrammar = fDTDGrammar;
    fGrammarType = fGrammar->getGrammarType();
    fValidator->setGrammar(fGrammar);

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

    if (toCache) {

        unsigned int sysId = fGrammarResolver->getStringPool()->addOrFind(src.getSystemId());
        const XMLCh* sysIdStr = fGrammarResolver->getStringPool()->getValueForId(sysId);

        fGrammarResolver->orphanGrammar(XMLUni::fgDTDEntityString);
        fGrammarResolver->putGrammar(sysIdStr, fGrammar);
    }

    //
    //  Handle the creation of the XML reader object for this input source.
    //  This will provide us with transcoding and basic lexing services.
    //
    XMLReader* newReader = fReaderMgr.createReader
    (
        src
        , false
        , XMLReader::RefFrom_NonLiteral
        , XMLReader::Type_General
        , XMLReader::Source_External
    );
    if (!newReader) {
        if (src.getIssueFatalErrorIfNotFound())
            ThrowXML1(RuntimeException, XMLExcepts::Scan_CouldNotOpenSource, src.getSystemId());
        else
            ThrowXML1(RuntimeException, XMLExcepts::Scan_CouldNotOpenSource_Warning, src.getSystemId());
    }

    //
    //  In order to make the processing work consistently, we have to
    //  make this look like an external entity. So create an entity
    //  decl and fill it in and push it with the reader, as happens
    //  with an external entity. Put a janitor on it to insure it gets
    //  cleaned up. The reader manager does not adopt them.
    //
    const XMLCh gDTDStr[] = { chLatin_D, chLatin_T, chLatin_D , chNull };
    DTDEntityDecl* declDTD = new DTDEntityDecl(gDTDStr);
    declDTD->setSystemId(src.getSystemId());
    Janitor<DTDEntityDecl> janDecl(declDTD);

    // Mark this one as a throw at end
    newReader->setThrowAtEnd(true);

    // And push it onto the stack, with its pseudo name
    fReaderMgr.pushReader(newReader, declDTD);

    //
    //  If we have a doc type handler and advanced callbacks are enabled,
    //  call the doctype event.
    //
    if (fDocTypeHandler) {

        // Create a dummy root
        DTDElementDecl* rootDecl = new DTDElementDecl(gDTDStr, fEmptyNamespaceId);
        rootDecl->setCreateReason(DTDElementDecl::AsRootElem);
        rootDecl->setExternalElemDeclaration(true);
        Janitor<DTDElementDecl> janSrc(rootDecl);

        fDocTypeHandler->doctypeDecl(*rootDecl, src.getPublicId(), src.getSystemId(), false);
    }

    // Create DTDScanner
    DTDScanner dtdScanner((DTDGrammar*)fGrammar, fDocTypeHandler);
    dtdScanner.setScannerInfo(this, &fReaderMgr, &fBufMgr);

    // Tell it its not in an include section
    dtdScanner.scanExtSubsetDecl(false);

    if (fValidate) {
        //  validate the DTD scan so far
        fValidator->preContentValidation(false, true);
    }

    if (toCache)
        fGrammarResolver->cacheGrammars();

    return fDTDGrammar;
}

// ---------------------------------------------------------------------------
//  XMLScanner: Rest pool of cached grammars
// ---------------------------------------------------------------------------
void XMLScanner::resetCachedGrammarPool()
{
    fGrammarResolver->resetCachedGrammar();
}