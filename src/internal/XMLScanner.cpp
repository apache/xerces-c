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
 * Revision 1.9  2000/02/06 07:47:54  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.8  2000/01/19 00:55:45  roddey
 * Changes to get rid of dependence on old utils standard streams classes
 * and a small fix in the progressive parseFirst() call.
 *
 * Revision 1.7  2000/01/15 01:26:16  rahulj
 * Added support for HTTP to the parser using libWWW 5.2.8.
 * Renamed URL.[ch]pp to XMLURL.[ch]pp and like wise for the class name.
 * Only tested under NT 4.0 SP 5.
 * Removed URL.hpp from files where it was not used.
 *
 * Revision 1.6  2000/01/12 23:52:46  roddey
 * These are trivial changes required to get the C++ and Java versions
 * of error messages more into sync. Mostly it was where the Java version
 * was passing out one or more parameter than the C++ version was. In
 * some cases the change just required an extra parameter to get the
 * needed info to the place where the error was issued.
 *
 * Revision 1.5  2000/01/12 00:15:04  roddey
 * Changes to deal with multiply nested, relative pathed, entities and to deal
 * with the new URL class changes.
 *
 * Revision 1.4  1999/12/08 00:15:06  roddey
 * Some small last minute fixes to get into the 3.0.1 build that is going to be
 * going out anyway for platform fixes.
 *
 * Revision 1.3  1999/12/02 19:02:57  roddey
 * Get rid of a few statically defined XMLMutex objects, and lazy eval them
 * using atomic compare and swap. I somehow let it get by me that we don't
 * want any static/global objects at all.
 *
 * Revision 1.2  1999/11/30 20:23:13  roddey
 * Added changes to handle exceptions thrown from the user's handlers for
 * emitError().
 *
 * Revision 1.1.1.1  1999/11/09 01:08:22  twl
 * Initial checkin
 *
 * Revision 1.7  1999/11/08 20:56:54  droddey
 * If the main xml entity does not exist, we need to get the error handling for that
 * inside the main XMLScanner::scanDocument() try block so that it gets reported
 * in the normal way. We have to add a little extra safety code because, when this
 * happens, there is no reader on the reader stack to get position ino from.
 *
 * Revision 1.6  1999/11/08 20:44:51  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <util/Janitor.hpp>
#include <util/Mutexes.hpp>
#include <util/PlatformUtils.hpp>
#include <util/RefVectorOf.hpp>
#include <util/RuntimeException.hpp>
#include <util/UnexpectedEOFException.hpp>
#include <util/XMLURL.hpp>
#include <util/XMLMsgLoader.hpp>
#include <util/XMLUni.hpp>
#include <sax/InputSource.hpp>
#include <sax/SAXException.hpp>
#include <framework/LocalFileInputSource.hpp>
#include <framework/URLInputSource.hpp>
#include <framework/XMLDocumentHandler.hpp>
#include <framework/XMLElementDecl.hpp>
#include <framework/XMLErrorReporter.hpp>
#include <framework/XMLEntityDecl.hpp>
#include <framework/XMLEntityHandler.hpp>
#include <framework/XMLPScanToken.hpp>
#include <framework/XMLValidator.hpp>
#include <framework/XMLValidityCodes.hpp>
#include <internal/XMLScanner.hpp>
#include <internal/EndOfEntityException.hpp>



// ---------------------------------------------------------------------------
//  Local static data
// ---------------------------------------------------------------------------
static XMLUInt32       gScannerId;
static XMLMsgLoader*   gMsgLoader;


// ---------------------------------------------------------------------------
//  Local, static functions
// ---------------------------------------------------------------------------

//
//  We need to fault in this mutex. But, since its used for synchronization
//  itself, we have to do this the low level way using a compare and swap.
//
static XMLMutex& gScannerMutex()
{
    static XMLMutex* scannerMutex = 0;
    if (!scannerMutex)
    {
        XMLMutex* tmpMutex = new XMLMutex;
        if (XMLPlatformUtils::compareAndSwap((void**)&scannerMutex, tmpMutex, 0))
        {
            // Someone beat us to it, so let's clean up ours
            delete tmpMutex;
        }
    }
    return *scannerMutex;
}


// ---------------------------------------------------------------------------
//  XMLScanner: Constructors and Destructor
// ---------------------------------------------------------------------------
XMLScanner::XMLScanner(XMLValidator* const validator) :

    fAttrList(0)
    , fDocHandler(0)
    , fDoNamespaces(false)
    , fDoValidation(false)
    , fEntityHandler(0)
    , fErrorReporter(0)
    , fExitOnFirstFatal(true)
    , fIDRefList(0)
    , fInException(false)
    , fRawAttrList(0)
    , fReuseValidator(false)
    , fScannerId(0)
    , fSequenceId(0xFFFFFFFF)
    , fStandalone(false)
    , fValidator(validator)
{
    commonInit();
}

XMLScanner::XMLScanner( XMLDocumentHandler* const   docHandler
                        , XMLEntityHandler* const   entityHandler
                        , XMLErrorReporter* const   errHandler
                        , XMLValidator* const       validator) :

    fAttrList(0)
    , fDocHandler(docHandler)
    , fDoNamespaces(false)
    , fDoValidation(false)
    , fEntityHandler(entityHandler)
    , fErrorReporter(errHandler)
    , fExitOnFirstFatal(true)
    , fIDRefList(0)
    , fInException(false)
    , fRawAttrList(0)
    , fReuseValidator(false)
    , fScannerId(0)
    , fSequenceId(0xFFFFFFFF)
    , fStandalone(false)
    , fValidator(validator)
{
    commonInit();
}

XMLScanner::~XMLScanner()
{
    delete fAttrList;
    delete fIDRefList;
    delete fRawAttrList;
}


// ---------------------------------------------------------------------------
//  XMLScanner: Main entry point to scan a document
// ---------------------------------------------------------------------------
void XMLScanner::scanDocument(  const   XMLCh* const    systemId
                                , const bool            reuseValidator)
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
        if (tmpURL.isRelative())
            ThrowXML(MalformedURLException, XML4CExcepts::URL_NoProtocolPresent);
        srcToUse = new URLInputSource(tmpURL);
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

    Janitor<InputSource> janSrc(srcToUse);
    scanDocument(*srcToUse, reuseValidator);
}

void XMLScanner::scanDocument(  const   char* const systemId
                                , const bool        reuseValidator)
{
    // We just delegate this to the XMLCh version after transcoding
    XMLCh* tmpBuf = XMLString::transcode(systemId);
    ArrayJanitor<XMLCh> janBuf(tmpBuf);
    scanDocument(tmpBuf, reuseValidator);
}


void XMLScanner::scanDocument(const InputSource& src, const bool reuseValidator)
{
    //
    //  Bump up the sequence id for this parser instance. This will invalidate
    //  any previous progressive scan tokens.
    //
    fSequenceId++;

    // Store the reuse validator flag
    fReuseValidator = reuseValidator;

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

        //
        //  Scan the prolog part, which is everything before the root element
        //  including the DTD subsets.
        //
        scanProlog();

        //
        //  At this point, we know which type of validation we are going to
        //  use (if the plugged in validator handles either DTD or Schemas)
        //  since we will have seen the DOCTYPE or PI that set it up. So lets
        //  ask the validator whether it requires namespaces or not. If it
        //  does, we have to override the namespace enablement flag.
        //
        if (fValidator->requiresNamespaces() && !fDoNamespaces)
            fDoNamespaces = true;

        //
        //  If we got to the end of input, then its not a valid XML file.
        //  Else, go on to scan the content.
        //
        if (fReaderMgr.atEOF())
        {
            emitError(XML4CErrs::EmptyMainEntity);
        }
         else
        {
            // Scan content, and tell it its not an external entity
            if (scanContent(false))
            {
                // Do post-parse validation if required
                if (fDoValidation)
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
    catch(const XML4CErrs::Codes)
    {
        // This is a 'first fatal error' type exit, so reset and fall through
        fReaderMgr.reset();
    }

    catch(const XML4CValid::Codes)
    {
        // This is a 'first fatal error' type exit, so reset and fall through
        fReaderMgr.reset();
    }

    catch(const SAXException&)
    {
        //
        //  We have to propogate SAX exceptions.
        //
        //  Make sure that the reader manager gets reset, then rethrow this
        //  exception since it means nothing much to us.
        //
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
            emitError
            (
                XML4CErrs::XMLException
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
                            ,       XMLPScanToken&  toFill
                            , const bool            reuseValidator)
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
        if (tmpURL.isRelative())
            ThrowXML(MalformedURLException, XML4CExcepts::URL_NoProtocolPresent);
        srcToUse = new URLInputSource(tmpURL);
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

    Janitor<InputSource> janSrc(srcToUse);
    return scanFirst(*srcToUse, toFill, reuseValidator);
}

bool XMLScanner::scanFirst( const   char* const     systemId
                            ,       XMLPScanToken&  toFill
                            , const bool            reuseValidator)
{
    // We just delegate this to the XMLCh version after transcoding
    XMLCh* tmpBuf = XMLString::transcode(systemId);
    ArrayJanitor<XMLCh> janBuf(tmpBuf);
    return scanFirst(tmpBuf, toFill, reuseValidator);
}

bool XMLScanner::scanFirst( const   InputSource&    src
                            ,       XMLPScanToken&  toFill
                            , const bool            reuseValidator)
{
    // Store the reuse validator flag
    fReuseValidator = reuseValidator;

    //
    //  Bump up the sequence id for this new scan cycle. This will invalidate
    //  any previous tokens we've returned.
    //
    fSequenceId++;

    // Reset the scanner and its plugged in stuff for a new run
    scanReset(src);

    try
    {
        //
        //  Scan the prolog part, which is everything before the root element
        //  including the DTD subsets. This is all that is done on the scan
        //  first.
        //
        scanProlog();
    }

    //
    //  NOTE:
    //
    //  In all of the error processing below, the emitError() call MUST come
    //  before the flush of the reader mgr, or it will fail because it tries
    //  to find out the position in the XML source of the error.
    //
    catch(const XML4CErrs::Codes)
    {
        // This is a 'first failure' exception so reset and return a failure
        fReaderMgr.reset();
        return false;
    }

    catch(const XML4CValid::Codes)
    {
        // This is a 'first fatal error' type exit, so reset and fall through
        fReaderMgr.reset();
    }

    // We have to propogate SAX exceptions
    catch(const SAXException&)
    {
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
            emitError
            (
                XML4CErrs::XMLException
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
        ThrowXML(RuntimeException, XML4CExcepts::Scan_BadPScanToken);

    bool retVal = true;
    try
    {
        // Find the next token and remember the reader id
        unsigned int orgReader;
        const XMLTokens curToken = senseNextToken(orgReader);

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
                    XML4CErrs::EndedWithTagsOnStack
                    , topElem->fThisElement->getFullName()
                );
            }

            retVal = false;
        }
         else
        {
            // Its some sort of markup
            bool gotData;
            switch(curToken)
            {
                case Token_CData :
                    // Make sure we are within content
                    if (fElemStack.isEmpty())
                        emitError(XML4CErrs::CDATAOutsideOfContent);
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
                emitError(XML4CErrs::PartialMarkupInEntity);

            // If we hit the end, then do the miscellaneous part
            if (!gotData)
                scanMiscellaneous();
        }
    }

    //
    //  NOTE:
    //
    //  In all of the error processing below, the emitError() call MUST come
    //  before the flush of the reader mgr, or it will fail because it tries
    //  to find out the position in the XML source of the error.
    //
    catch(const XML4CErrs::Codes)
    {
        // This is a 'first failure' exception, so reset and return failure
        fReaderMgr.reset();
        return false;
    }

    catch(const XML4CValid::Codes)
    {
        // This is a 'first fatal error' type exit, so reset and fall through
        fReaderMgr.reset();
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
            emitError
            (
                XML4CErrs::XMLException
                , excToCatch.getType()
                , excToCatch.getMessage()
            );
        }

        catch(...)
        {
            // REset and rethrow user error
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
            // MUST check again since we could have gotten beat to the lock
            if (!gMsgLoader)
            {
                gMsgLoader = XMLPlatformUtils::loadMsgSet(XMLUni::fgXMLErrDomain);
                if (!gMsgLoader)
                    XMLPlatformUtils::panic(XMLPlatformUtils::Panic_CantLoadMsgDomain);
            }
        }

        // And assign ourselves the next available scanner id
        fScannerId = ++gScannerId;
    }

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

    //
    //  Tell the validator about the stuff it needs to know in order to
    //  do its work.
    //
    fValidator->setScannerInfo(this, &fReaderMgr, &fBufMgr);
}


// ---------------------------------------------------------------------------
//  XMLScanner: Error emitting methods
// ---------------------------------------------------------------------------

//
//  These methods are called whenever the scanner wants to emit an error.
//  It handles getting the message loaded, doing token replacement, etc...
//  and then calling the error handler, if its installed.
//
void XMLScanner::emitError(const XML4CErrs::Codes toEmit)
{
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
            , XML4CErrs::errorType(toEmit)
            , errText
            , lastInfo.systemId
            , lastInfo.publicId
            , lastInfo.lineNumber
            , lastInfo.colNumber
        );
    }

    // Bail out if its fatal an we are to give up on the first fatal error
    if (XML4CErrs::isFatal(toEmit) && fExitOnFirstFatal && !fInException)
        throw toEmit;
}

void XMLScanner::emitError( const   XML4CErrs::Codes    toEmit
                            , const XMLCh* const        text1
                            , const XMLCh* const        text2
                            , const XMLCh* const        text3
                            , const XMLCh* const        text4)
{
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
            , XML4CErrs::errorType(toEmit)
            , errText
            , lastInfo.systemId
            , lastInfo.publicId
            , lastInfo.lineNumber
            , lastInfo.colNumber
        );
    }

    // Bail out if its fatal an we are to give up on the first fatal error
    if (XML4CErrs::isFatal(toEmit) && fExitOnFirstFatal && !fInException)
        throw toEmit;
}

void XMLScanner::emitError( const   XML4CErrs::Codes    toEmit
                            , const char* const         text1
                            , const char* const         text2
                            , const char* const         text3
                            , const char* const         text4)
{
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
            , XML4CErrs::errorType(toEmit)
            , errText
            , lastInfo.systemId
            , lastInfo.publicId
            , lastInfo.lineNumber
            , lastInfo.colNumber
        );
    }

    // Bail out if its fatal an we are to give up on the first fatal error
    if (XML4CErrs::isFatal(toEmit) && fExitOnFirstFatal && !fInException)
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
                                ,       unsigned int&   lineToFill
                                ,       unsigned int&   colToFill)
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
//  XMLScanner: Setter methods methods
// ---------------------------------------------------------------------------
void XMLScanner::setDoValidation(const bool validate)
{
    fDoValidation = validate;
}

void XMLScanner::setValidator(XMLValidator* const validator)
{
    fValidator = validator;
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
                    emitError(XML4CErrs::ExpectedWhitespace);
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
                emitError(XML4CErrs::ExpectedAttrName);
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

                emitError(XML4CErrs::ExpectedEqSign);

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
                    emitError(XML4CErrs::UnterminatedStartTag, elemName);
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

                emitError(XML4CErrs::ExpectedAttrValue);

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
                    emitError(XML4CErrs::UnterminatedStartTag, elemName);
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
                    emitError(XML4CErrs::TooManyColonsInName);
                    continue;
                }
                 else if ((colonFirst == 0)
                      ||  (colonLast == (int)fAttNameBuf.getLen() - 1))
                {
                    emitError(XML4CErrs::InvalidColonPos);
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
            ThrowXML(UnexpectedEOFException, XML4CExcepts::Gen_UnexpectedEOF);

        if (nextCh == chForwardSlash)
        {
            fReaderMgr.getNextChar();
            if (fReaderMgr.skippedChar(chCloseAngle))
            {
                isEmpty = true;
                break;
            }
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
            emitError(XML4CErrs::UnterminatedStartTag, elemName);
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
            emitError(XML4CErrs::ExpectedAttrName);
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
                            XML4CErrs::EndedWithTagsOnStack
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
                            emitError(XML4CErrs::CDATAOutsideOfContent);
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
                    emitError(XML4CErrs::PartialMarkupInEntity);

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
                emitError(XML4CErrs::PartialMarkupInEntity);

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
        emitError(XML4CErrs::MoreEndThanStartTags);
        fReaderMgr.skipPastChar(chCloseAngle);
        ThrowXML(RuntimeException, XML4CExcepts::Scan_UnbalancedStartEnd);
    }

    // After the </ is the element QName, so get a name from the input
    XMLBufBid bbQName(&fBufMgr);
    XMLBuffer& qnameBuf = bbQName.getBuffer();
    if (!fReaderMgr.getName(qnameBuf))
    {
        // It failed so we can't really do anything with it
        emitError(XML4CErrs::ExpectedElementName);
        fReaderMgr.skipPastChar(chCloseAngle);
        return;
    }

    unsigned int elemId;
    unsigned int uriId = 0;
    if (fDoNamespaces)
    {
        XMLBufBid bbName(&fBufMgr);
        XMLBufBid bbPrefix(&fBufMgr);
        uriId = resolveQName
        (
            qnameBuf.getRawBuffer()
            , bbName.getBuffer()
            , bbPrefix.getBuffer()
            , ElemStack::Mode_Element
        );

        //
        //  Ask the validator for the element id for the {uri}name we got. He owns
        //  the element decl pool.
        //
        elemId = fValidator->findElemId
        (
            uriId
            , bbName.getBuffer().getRawBuffer()
            , qnameBuf.getRawBuffer()
        );
    }
     else
    {
        elemId = fValidator->findElemId(0, 0, qnameBuf.getRawBuffer());
    }

    //
    //  Pop the stack of the element we are supposed to be ending. Remember
    //  that we don't own this. The stack just keeps them and reuses them.
    //
    //  NOTE: We CANNOT do this until we've resolved the element name because
    //  the element stack top contains the prefix to URI mappings for this
    //  element.
    //
    const ElemStack::StackElem* topElem = fElemStack.popTop();

    // Make sure that its the end of the element that we expect
    if (topElem->fThisElement->getId() != elemId)
    {
        emitError
        (
            XML4CErrs::ExpectedEndOfTagX
            , topElem->fThisElement->getFullName()
        );
    }

    // Make sure we are back on the same reader as where we started
    if (topElem->fReaderNum != fReaderMgr.getCurrentReaderNum())
        emitError(XML4CErrs::PartialTagMarkupError);

    // See if it was the root element, to avoid multiple calls below
    const bool isRoot = fElemStack.isEmpty();

    // Skip optional whitespace
    fReaderMgr.skipPastSpaces();

    // Make sure we find the closing bracket
    if (!fReaderMgr.skippedChar(chCloseAngle))
    {
        emitError
        (
            XML4CErrs::UnterminatedEndTag
            , topElem->fThisElement->getFullName()
        );
    }

    // If we have a doc handler, tell it about the end tag
    if (fDocHandler)
    {
        fDocHandler->endElement
        (
            *topElem->fThisElement
            , uriId
            , isRoot
        );
    }

    //
    //  If validation is enabled, then lets pass him the list of children and
    //  this element and let him validate it.
    //
    if (fDoValidation)
    {
        int res = fValidator->checkContent
        (
            topElem->fThisElement->getId()
            , topElem->fChildIds
            , topElem->fChildCount
        );

        if (res >= 0)
        {
            //
            //  One of the elements is not valid for the content.
            //  NOTE that if no children were provided but the
            //  content model requires them, it comes back with
            //  a zero value. But we cannot use that to index
            //  the child array in this case, and have to put
            //  out a special message.
            //
            if (!topElem->fChildCount)
            {
                fValidator->emitError
                (
                    XML4CValid::EmptyNotValidForContent
                    , topElem->fThisElement->getFormattedContentModel(*fValidator)
                );
            }
             else if ((unsigned int)res >= topElem->fChildCount)
            {
                fValidator->emitError
                (
                    XML4CValid::NotEnoughElemsForCM
                    , topElem->fThisElement->getFormattedContentModel(*fValidator)
                );
            }
             else
            {
                // Find the element decl for the evil spawn
                XMLElementDecl* decl = fValidator->getElemDecl
                (
                    topElem->fChildIds[res]
                );

                fValidator->emitError
                (
                    XML4CValid::ElementNotValidForContent
                    , decl->getFullName()
                    , topElem->fThisElement->getFormattedContentModel(*fValidator)
                );
            }
        }
    }

    // If this was the root, then done with content
    gotData = !isRoot;
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
                if (fReaderMgr.skippedString(XMLUni::fgXMLStringSpace))
                {
                    // Can't have an XML decl here
                    emitError(XML4CErrs::NotValidAfterContent);
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
                    emitError(XML4CErrs::ExpectedCommentOrPI);
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
                emitError(XML4CErrs::ExpectedCommentOrPI);
                fReaderMgr.skipPastChar(chCloseAngle);
            }
        }

        catch(const EndOfEntityException&)
        {
            //
            //  Some entity leaked out of the content part of the document. Issue
            //  a warning and keep going.
            //
            emitError(XML4CErrs::EntityPropogated);
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

    // And skip any subsequent spaces before the name
    fReaderMgr.skipPastSpaces();

    // Get a buffer for the PI name and scan it in
    XMLBufBid bbName(&fBufMgr);
    if (!fReaderMgr.getNameToken(bbName.getBuffer()))
    {
        emitError(XML4CErrs::PINameExpected);
        fReaderMgr.skipPastChar(chCloseAngle);
        return;
    }

    // Point the name pointer at the raw data
    namePtr = bbName.getRawBuffer();

    // See if it is some form of 'xml' and emit a warning
    if (!XMLString::compareIString(namePtr, XMLUni::fgXMLString))
        emitError(XML4CErrs::NoPIStartsWithXML);

    // If namespaces are enabled, then no colons allowed
    if (fDoNamespaces)
    {
        if (XMLString::indexOf(namePtr, chColon) != -1)
            emitError(XML4CErrs::ColonNotLegalWithNS);
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

        // It does have a target, so lets move on to deal with that.
        while (1)
        {
            const XMLCh nextCh = fReaderMgr.getNextChar();

            // Watch for an end of file, which is always bad here
            if (!nextCh)
            {
                emitError(XML4CErrs::UnterminatedPI);
                ThrowXML(UnexpectedEOFException, XML4CExcepts::Gen_UnexpectedEOF);
            }

            // Watch for potential terminating character
            if (nextCh == chQuestion)
            {
                // It must be followed by '>' to be a termination of the target
                if (fReaderMgr.skippedChar(chCloseAngle))
                    break;
            }

            // Watch for invalid chars but try to keep going
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
                emitError(XML4CErrs::InvalidCharacter, tmpBuf);
            }

            bbTarget.append(nextCh);
        }
    }
     else
    {
        // No target, but make sure its terminated ok
        if (!fReaderMgr.skippedChar(chQuestion))
        {
            emitError(XML4CErrs::UnterminatedPI);
            fReaderMgr.skipPastChar(chCloseAngle);
            return;
        }

        if (!fReaderMgr.skippedChar(chCloseAngle))
        {
            emitError(XML4CErrs::UnterminatedPI);
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
    bool  acceptXMLDecl = true;
    while (true)
    {
    try
    {
        while (true)
        {
            const XMLCh nextCh = fReaderMgr.peekNextChar();

            // An end of file is legal here between markup
            if (!nextCh)
                return;

            if (nextCh == chOpenAngle)
            {
                //
                //  Ok, it could be the xml decl, a comment, the doc type line,
                //  or the start of the root element.
                //
                if (fReaderMgr.skippedString(XMLUni::fgXMLDeclStringSpace))
                {
                    //
                    //  If we are not at line 1, col 7, then the decl was not
                    //  the first text, so its invalid.
                    //
                    const XMLReader* curReader = fReaderMgr.getCurrentReader();
                    if ((curReader->getLineNumber() != 1)
                    ||  (curReader->getColumnNumber() != 7))
                    {
                        emitError(XML4CErrs::XMLDeclMustBeFirst);
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
                    //
                    //  We have a doc type. So, ask the validator if it
                    //  supports DTD scanning. if so, let it scan the
                    //  DTD.
                    //
                    if (fValidator->handlesDTD())
                        fValidator->scanDTD(fReuseValidator);
                    else
                        ThrowXML(RuntimeException, XML4CExcepts::Gen_NoDTDValidator);
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
                emitError(XML4CErrs::InvalidDocumentStructure);
                fReaderMgr.skipPastChar(chCloseAngle);
            }

            //
            //  Once we make it around once, then we cannot accept the XML
            //  decl now, since it must be first if present.
            //
            acceptXMLDecl = false;
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
            XML4CErrs::UnexpectedEOE
            , "in prolog"
        );
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
        emitError(XML4CErrs::ExpectedElementName);
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
    bool wasAdded;
    XMLElementDecl* elemDecl = fValidator->findElemDecl
    (
        0
        , 0
        , fQNameBuf.getRawBuffer()
        , XMLValidator::AddIfNotFound
        , wasAdded
    );

    //
    //  We do something different here according to whether we found the
    //  element or not.
    //
    if (wasAdded)
    {
        // If validating then emit an error
        if (fDoValidation)
        {
            fValidator->emitError
            (
                XML4CValid::ElementNotDefined
                , elemDecl->getFullName()
            );
        }

        //
        //  If we are not validating, then mark it as declared since it makes
        //  things simpler and we aren't going to do do the validation checks
        //  that need to know if it was really declared or not anyway.
        //
        if (!fDoValidation)
            elemDecl->setCreateReason(XMLElementDecl::Declared);
    }
     else
    {
        // If its not marked declared and validating, then emit an error
        if (!elemDecl->isDeclared() && fDoValidation)
        {
            fValidator->emitError
            (
                XML4CValid::ElementNotDefined
                , elemDecl->getFullName()
            );
        }
    }

    // See if its the root element
    const bool isRoot = fElemStack.isEmpty();

    // Expand the element stack and add the new element
    fElemStack.addLevel(elemDecl, fReaderMgr.getCurrentReaderNum());

    //
    //  If this is the first element and we are validating, check the root
    //  element. This may or may not have any meaning for the installed
    //  validator, in which case it may just always return success. Some
    //  validators will treat any element as the root.
    //
    if (isRoot)
    {
        if (fDoValidation)
        {
            if (!fValidator->checkRootElement(elemDecl->getId()))
                fValidator->emitError(XML4CValid::RootElemNotLikeDocType);
        }
    }
     else
    {
        //
        //  If the element stack is not empty, then add this element as a
        //  child of the previous top element. If its empty, this is the root
        //  elem and is not the child of anything.
        //
        fElemStack.addChild(elemDecl->getId(), true);
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
                    emitError(XML4CErrs::ExpectedWhitespace);
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
                emitError(XML4CErrs::ExpectedAttrName);
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

                emitError(XML4CErrs::ExpectedEqSign);

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
                    emitError(XML4CErrs::UnterminatedStartTag, elemDecl->getFullName());
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
            XMLAttDef* attDef = elemDecl->findAttr
            (
                fAttNameBuf.getRawBuffer()
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
                if (fDoValidation)
                {
                    fValidator->emitError
                    (
                        XML4CValid::AttNotDefinedForElement
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
                    XML4CErrs::AttrAlreadyUsedInSTag
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
            if (!scanAttValue(attDef->getFullName(), fAttValueBuf, attDef->getType()))
            {
                static const XMLCh tmpList[] =
                {
                    chCloseAngle, chOpenAngle, chForwardSlash, chNull
                };

                emitError(XML4CErrs::ExpectedAttrValue);

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
                    emitError(XML4CErrs::UnterminatedStartTag, elemDecl->getFullName());
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
            if (!wasAdded)
            {
                // Let the validator pass judgement on the attribute value
                fValidator->validateAttrValue
                (
                    *attDef
                    , fAttValueBuf.getRawBuffer()
                );
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
                    fValidator->getGlobalNamespaceId()
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
                    fValidator->getGlobalNamespaceId()
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
            ThrowXML(UnexpectedEOFException, XML4CExcepts::Gen_UnexpectedEOF);

        if (nextCh == chForwardSlash)
        {
            fReaderMgr.getNextChar();
            isEmpty = true;
            if (!fReaderMgr.skippedChar(chCloseAngle))
                emitError(XML4CErrs::UnterminatedStartTag, elemDecl->getFullName());
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
            emitError(XML4CErrs::UnterminatedStartTag, elemDecl->getFullName());
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
            emitError(XML4CErrs::ExpectedAttrName);
            scanAttValue(XMLUni::fgZeroLenString, fAttValueBuf, XMLAttDef::CData);
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
                if (fDoValidation)
                {
                    // If we are validating and its required, then an error
                    if (defType == XMLAttDef::Required)
                    {
                        fValidator->emitError
                        (
                            XML4CValid::RequiredAttrNotProvided
                            , curDef.getFullName()
                            , elemDecl->getFullName()
                        );
                    }
                }

                // Fault in the value if needed, and bump the att count
                if ((defType == XMLAttDef::Default)
                ||  (defType == XMLAttDef::Fixed))
                {
                    XMLAttr* curAtt;
                    if (attCount >= curAttListSize)
                    {
                        curAtt = new XMLAttr
                        (
                            fValidator->getGlobalNamespaceId()
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
                            fValidator->getGlobalNamespaceId()
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
        if (fDoValidation)
        {
            const int res = fValidator->checkContent(elemDecl->getId(), 0, 0);
            if (res >= 0)
            {
                fValidator->emitError
                (
                    XML4CValid::ElementNotValidForContent
                    , elemDecl->getFullName()
                    , elemDecl->getFormattedContentModel(*fValidator)
                );
            }
        }

        // Pop the element stack back off since it'll never be used now
        fElemStack.popTop();

        // If the elem stack is empty, then it was an empty root
        if (isRoot)
            gotData = false;
    }

    //
    //  If we have a document handler, then tell it about this start tag. We
    //  don't have any URI id to send along, so send zero. We also do not send
    //  any prefix since its just one big name if we are not doing namespaces.
    //
    if (fDocHandler)
    {
        fDocHandler->startElement
        (
            *elemDecl
            , 0
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
        emitError(XML4CErrs::ExpectedElementName);
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
        emitError(XML4CErrs::InvalidCharacter, tmpBuf);
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

    //
    //  Now, since we might have to update the namespace map for this element,
    //  but we don't have the element decl yet, we just tell the element stack
    //  to expand up to get ready.
    //
    fElemStack.addLevel();

    //
    //  Make an initial pass through the list and find any xmlns attributes.
    //  When we find one, send it off to be used to update the element stack's
    //  namespace mappings.
    //
    if (attCount)
    {
        for (unsigned int index = 0; index < attCount; index++)
        {
            //
            //  If either the key begins with "xmlns:" or its just plain
            //  "xmlns", then use it to update the map.
            //
            const KVStringPair* curPair = fRawAttrList->elementAt(index);
            if (!XMLString::compareNString( curPair->getKey()
                                            , XMLUni::fgXMLNSColonString
                                            , 6)
            ||  !XMLString::compareString(curPair->getKey(), XMLUni::fgXMLNSString))
            {
                updateNSMap(curPair->getKey(), curPair->getValue());
            }
        }
    }

    //
    //  Resolve the qualified name to a URI and name so that we can look up
    //  the element decl for this element. We have now update the prefix to
    //  namespace map so we should get the correct element now.
    //
    //  <TBD>
    //  <NOTE> There is an issue here in that (when Schema comes along) if
    //  there were defauled/fixed xmlns attributes defined for this element,
    //  it is too late for them to affect the prefix of this element (though
    //  technically they should.) There is no way out of this unfortunately.
    //
    const unsigned int uriId = resolveQName
    (
        fQNameBuf.getRawBuffer()
        , fNameBuf
        , fPrefixBuf
        , ElemStack::Mode_Element
    );

    //
    //  Look up the element now in the validator. This will get us back a
    //  generic element decl object. We tell him to fault one in if he does
    //  not find it.
    //
    bool wasAdded;
    XMLElementDecl* elemDecl = fValidator->findElemDecl
    (
        uriId
        , fNameBuf.getRawBuffer()
        , fQNameBuf.getRawBuffer()
        , XMLValidator::AddIfNotFound
        , wasAdded
    );

    //
    //  We do something different here according to whether we found the
    //  element or not.
    //
    if (wasAdded)
    {
        // If validating then emit an error
        if (fDoValidation)
        {
            fValidator->emitError
            (
                XML4CValid::ElementNotDefined
                , elemDecl->getFullName()
            );
        }

        //
        //  If we are not validating, then mark it as declared since it makes
        //  things simpler and we aren't going to do do the validation checks
        //  that need to know if it was really declared or not anyway.
        //
        if (!fDoValidation)
            elemDecl->setCreateReason(XMLElementDecl::Declared);
    }
     else
    {
        // If its not marked declared and validating, then emit an error
        if (!elemDecl->isDeclared() && fDoValidation)
        {
            fValidator->emitError
            (
                XML4CValid::ElementNotDefined
                , elemDecl->getFullName()
            );
        }
    }

    //
    //  Now we can update the element stack to set the current element
    //  decl. We expanded the stack above, but couldn't store the element
    //  decl because we didn't know it yet.
    //
    fElemStack.setElement(elemDecl, fReaderMgr.getCurrentReaderNum());

    //
    //  If this is the first element and we are validating, check the root
    //  element. This may or may not have any meaning for the installed
    //  validator, in which case it may just always return success. Some
    //  validators will treat any element as the root.
    //
    if (isRoot)
    {
        if (fDoValidation)
        {
            if (!fValidator->checkRootElement(elemDecl->getId()))
                fValidator->emitError(XML4CValid::RootElemNotLikeDocType);
        }
    }
     else
    {
        //
        //  If the element stack is not empty, then add this element as a
        //  child of the previous top element. If its empty, this is the root
        //  elem and is not the child of anything.
        //
        fElemStack.addChild(elemDecl->getId(), true);
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
    attCount = buildAttList(*fRawAttrList, attCount, *elemDecl, *fAttrList);

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
        if (fDoValidation)
        {
            const int res = fValidator->checkContent(elemDecl->getId(), 0, 0);
            if (res >= 0)
            {
                fValidator->emitError
                (
                    XML4CValid::ElementNotValidForContent
                    , elemDecl->getFullName()
                    , elemDecl->getFormattedContentModel(*fValidator)
                );
            }
        }

        // If the elem stack is empty, then it was an empty root
        if (isRoot)
            gotData = false;
    }

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
            , isEmpty
            , isRoot
        );
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
    XMLBuffer* buffers[StringCount] =
    {
        &bbVersion.getBuffer()
        , &bbEncoding.getBuffer()
        , &bbStand.getBuffer()
        , &bbDummy.getBuffer()
    };

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
            emitError(XML4CErrs::ExpectedWhitespace);

        //
        //  Get characters up to the next whitespace or equal's sign.
        //
        if (!scanUpToWSOr(nameBuf, chEqual))
            emitError(XML4CErrs::ExpectedDeclString);

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
            emitError(XML4CErrs::ExpectedDeclString, nameBuf.getRawBuffer());
        else if (flags[curString] != -1)
            emitError(XML4CErrs::DeclStringRep, nameBuf.getRawBuffer());
        else if (flags[curString] == -1)
            flags[curString] = ++curCount;

        //
        //  Scan for an equal's sign. If we don't find it, issue an error
        //  but keep trying to go on.
        //
        if (!scanEq())
            emitError(XML4CErrs::ExpectedEqSign);

        //
        //  Get a quote string into the buffer for the string that we are
        //  currently working on.
        //
        if (!getQuotedString(*buffers[curString]))
        {
            emitError(XML4CErrs::ExpectedQuotedString);
            fReaderMgr.skipPastChar(chCloseAngle);
            return;
        }

        // And validate the value according which one it was
        const XMLCh* rawValue = buffers[curString]->getRawBuffer();
        if (curString == VersionString)
        {
            if (XMLString::compareString(rawValue, XMLUni::fgSupportedVersion))
                emitError(XML4CErrs::UnsupportedXMLVersion, rawValue);
        }
         else if (curString == EncodingString)
        {
            if (!XMLString::stringLen(rawValue))
                emitError(XML4CErrs::BadXMLEncoding, rawValue);
        }
         else if (curString == StandaloneString)
        {
            if (!XMLString::compareString(rawValue, XMLUni::fgYesString))
                fStandalone = true;
            else if (!XMLString::compareString(rawValue, XMLUni::fgNoString))
                fStandalone = false;
            else
            {
                emitError(XML4CErrs::BadStandalone);
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
                emitError(XML4CErrs::DeclStringsInWrongOrder);
                break;
            }
            curTop = flags[index];
        }
    }

    //
    //  If its an XML decl, the version must be present. If its a Text decl
    //  then standalone must not be present.
    //
    if ((type == Decl_XML) && (flags[VersionString] == -1))
        emitError(XML4CErrs::XMLVersionRequired);
    else if ((type == Decl_Text) && (flags[StandaloneString] != -1))
        emitError(XML4CErrs::StandaloneNotLegal);

    if (!fReaderMgr.skippedChar(chQuestion))
    {
        emitError(XML4CErrs::UnterminatedXMLDecl);
        fReaderMgr.skipPastChar(chCloseAngle);
    }
     else if (!fReaderMgr.skippedChar(chCloseAngle))
    {
        emitError(XML4CErrs::UnterminatedXMLDecl);
        fReaderMgr.skipPastChar(chCloseAngle);
    }

    //
    //  If we have a document handler then call the XML Decl callback.
    //
    //  !NOTE! Do this before we possibly update the reader with the
    //  actual encoding string. Otherwise, we will pass the wrong thing
    //  for the last parameter!
    //
    if (fDocHandler)
    {
        fDocHandler->XMLDecl
        (
            bbVersion.getRawBuffer()
            , bbEncoding.getRawBuffer()
            , bbStand.getRawBuffer()
            , fReaderMgr.getCurrentEncodingStr()
        );
    }

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
            emitError(XML4CErrs::ContradictoryEncoding, bbEncoding.getRawBuffer());
    }
}
