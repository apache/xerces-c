/*
 * Copyright 1999-2004 The Apache Software Foundation.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *      http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * $Log$
 * Revision 1.45  2005/03/30 00:55:13  cargilld
 * Begin work on adding some new features by checking in the feature handling support.
 *
 * Revision 1.44  2005/03/20 19:02:45  cargilld
 * Implement versions of uppercase and compareIstring that only check a to z, instead of all characters, and don't rely on functionality provided in the transcoders.
 *
 * Revision 1.43  2005/02/25 11:31:07  amassari
 * Performance improvements by David Bertoni (jira# 1343)
 *
 * Revision 1.42  2004/12/30 15:23:41  amassari
 * Notify advanced handlers of the whitespace before and after the root document element (jira# 729)
 *
 * Revision 1.41  2004/12/07 19:45:43  knoaman
 * An option to ignore a cached DTD grammar when a document contains an
 * internal and external subset.
 *
 * Revision 1.40  2004/10/04 11:30:51  amassari
 * As start/endPrefixMapping doesn't use the XMLBufMgr variable, we need only one XMLBuffer
 *
 * Revision 1.39  2004/10/04 09:26:31  amassari
 * Use an XMLStringPool+ValueStackOf(int) object to store the prefixes currently in scope, instead of a XMLBufMgr+ValueStack(XMLBuffer), that has a limitation of 32 items (jira#866)
 *
 * Revision 1.38  2004/09/30 14:07:23  peiyongz
 * setInputBufferSize
 *
 * Revision 1.37  2004/09/28 02:14:14  cargilld
 * Add support for validating annotations.
 *
 * Revision 1.36  2004/09/23 01:09:55  cargilld
 * Add support for generating synthetic XSAnnotations.  When a schema component has non-schema attributes and no child attributes create a synthetic XSAnnotation (under feature control) so the non-schema attributes can be recovered under PSVI.
 *
 * Revision 1.35  2004/09/08 13:56:17  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.34  2004/04/13 16:53:26  peiyongz
 * get/setIdentityConstraintChecking
 *
 * Revision 1.33  2004/01/29 11:46:32  cargilld
 * Code cleanup changes to get rid of various compiler diagnostic messages.
 *
 * Revision 1.32  2003/12/17 00:18:35  cargilld
 * Update to memory management so that the static memory manager (one used to call Initialize) is only for static data.
 *
 * Revision 1.31  2003/11/21 22:38:50  neilg
 * Enable grammar pools and grammar resolvers to manufacture
 * XSModels.  This also cleans up handling in the
 * parser classes by eliminating the need to tell
 * the grammar pool that schema compoments need to be produced.
 * Thanks to David Cargill.
 *
 * Revision 1.30  2003/11/06 15:30:07  neilg
 * first part of PSVI/schema component model implementation, thanks to David Cargill.  This covers setting the PSVIHandler on parser objects, as well as implementing XSNotation, XSSimpleTypeDefinition, XSIDCDefinition, and most of XSWildcard, XSComplexTypeDefinition, XSElementDeclaration, XSAttributeDeclaration and XSAttributeUse.
 *
 * Revision 1.29  2003/10/30 21:37:31  knoaman
 * Enhanced Entity Resolver Support. Thanks to David Cargill.
 *
 * Revision 1.28  2003/10/01 16:32:38  neilg
 * improve handling of out of memory conditions, bug #23415.  Thanks to David Cargill.
 *
 * Revision 1.27  2003/09/16 18:30:54  neilg
 * make Grammar pool be responsible for creating and owning URI string pools.  This is one more step towards having grammars be independent of the parsers involved in their creation
 *
 * Revision 1.26  2003/08/13 15:43:24  knoaman
 * Use memory manager when creating SAX exceptions.
 *
 * Revision 1.25  2003/07/31 17:05:48  peiyongz
 * using getGrammar(URI)
 *
 * Revision 1.24  2003/07/10 19:48:24  peiyongz
 * Stateless Grammar: Initialize scanner with grammarResolver,
 *
 * Revision 1.23  2003/06/25 22:36:46  peiyongz
 * to use new GrammarResolver::getGrammar()
 *
 * Revision 1.22  2003/06/20 18:55:54  peiyongz
 * Stateless Grammar Pool :: Part I
 *
 * Revision 1.21  2003/05/18 14:02:05  knoaman
 * Memory manager implementation: pass per instance manager.
 *
 * Revision 1.20  2003/05/16 21:36:59  knoaman
 * Memory manager implementation: Modify constructors to pass in the memory manager.
 *
 * Revision 1.19  2003/05/16 06:01:52  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.18  2003/05/15 18:26:50  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.17  2003/04/17 21:58:50  neilg
 * Adding a new property,
 * http://apache.org/xml/properties/security-manager, with
 * appropriate getSecurityManager/setSecurityManager methods on DOM
 * and SAX parsers.  Also adding a new SecurityManager class.
 *
 * The purpose of these modifications is to permit applications a
 * means to have the parser reject documents whose processing would
 * otherwise consume large amounts of system resources.  Malicious
 * use of such documents could be used to launch a denial-of-service
 * attack against a system running the parser.  Initially, the
 * SecurityManager only knows about attacks that can result from
 * exponential entity expansion; this is the only known attack that
 * involves processing a single XML document.  Other, simlar attacks
 * can be launched if arbitrary schemas may be parsed; there already
 * exist means (via use of the EntityResolver interface) by which
 * applications can deny processing of untrusted schemas.  In future,
 * the SecurityManager will be expanded to take these other exploits
 * into account.
 *
 * Adding SecurityManager support
 * 
 * Revision 1.16  2003/01/03 20:09:36  tng
 * New feature StandardUriConformant to force strict standard uri conformance.
 *
 * Revision 1.15  2002/12/27 16:16:51  knoaman
 * Set scanner options and handlers.
 *
 * Revision 1.14  2002/12/11 22:14:54  knoaman
 * Performance: no need to use temporary buffer to hold namespace value.
 *
 * Revision 1.13  2002/12/04 01:57:09  knoaman
 * Scanner re-organization.
 *
 * Revision 1.12  2002/11/04 14:57:03  tng
 * C++ Namespace Support.
 *
 * Revision 1.11  2002/09/24 20:00:32  tng
 * Performance: use XMLString::equals instead of XMLString::compareString
 *
 * Revision 1.10  2002/08/14 15:20:38  knoaman
 * [Bug 3111] Problem with LexicalHandler::startDTD() and LexicalHandler::endDTD().
 *
 * Revision 1.9  2002/07/11 18:27:20  knoaman
 * Grammar caching/preparsing - initial implementation.
 *
 * Revision 1.8  2002/06/17 15:41:15  tng
 * To be consistent, SAX2 is updated with:
 * 1. the progressive parse methods should use the fReuseGrammar flag set from setFeature instead of using parameter
 * 2. add feature "http://apache.org/xml/features/continue-after-fatal-error", and users should use setFeature instead of setExitOnFirstFatalError
 * 3. add feature "http://apache.org/xml/features/validation-error-as-fatal", and users should use setFeature instead of setValidationConstraintFatal
 *
 * Revision 1.7  2002/05/30 16:20:09  tng
 * Add feature to optionally ignore external DTD.
 *
 * Revision 1.6  2002/05/29 21:37:47  knoaman
 * Add baseURI to resolveEntity to support DOMInputSource.
 *
 * Revision 1.5  2002/05/28 20:44:14  tng
 * [Bug 9104] prefixes dissapearing when schema validation turned on.
 *
 * Revision 1.4  2002/05/27 18:39:21  tng
 * To get ready for 64 bit large file, use XMLSSize_t to represent line and column number.
 *
 * Revision 1.3  2002/05/22 20:53:41  knoaman
 * Prepare for DOM L3 :
 * - Make use of the XMLEntityHandler/XMLErrorReporter interfaces, instead of using
 * EntityHandler/ErrorHandler directly.
 * - Add 'AbstractDOMParser' class to provide common functionality for XercesDOMParser
 * and DOMBuilder.
 *
 * Revision 1.2  2002/02/13 16:09:24  knoaman
 * Move SAX2 features/properties names constants to XMLUni.
 *
 * Revision 1.1.1.1  2002/02/01 22:22:06  peiyongz
 * sane_include
 *
 * Revision 1.25  2002/01/28 17:47:41  knoaman
 * Some SAX calls were not passed to the LexicalHandler.
 *
 * Revision 1.24  2002/01/28 17:08:47  knoaman
 * SAX2-ext's DeclHandler support.
 *
 * Revision 1.23  2002/01/28 16:29:21  knoaman
 * The namespace-prefixes feature in SAX2 should be off by default.
 *
 * Revision 1.22  2002/01/24 16:30:34  tng
 * [Bug 3111] Problem with LexicalHandler::startDTD() and LexicalHandler::endDTD() .
 *
 * Revision 1.21  2001/12/21 18:03:25  tng
 * [Bug 1833] LexicalHandler::startDTD not called correctly.
 *
 * Revision 1.20  2001/11/20 18:51:44  tng
 * Schema: schemaLocation and noNamespaceSchemaLocation to be specified outside the instance document.  New methods setExternalSchemaLocation and setExternalNoNamespaceSchemaLocation are added (for SAX2, two new properties are added).
 *
 * Revision 1.19  2001/10/25 19:46:15  tng
 * Comment outside root element should also be reported.
 *
 * Revision 1.18  2001/09/12 13:03:43  tng
 * [Bug 3155] SAX2 does not offer progressive parse.
 *
 * Revision 1.17  2001/08/02 19:00:46  tng
 * [Bug 1329] SAX2XMLReaderImpl leaks XMLBuffers.
 *
 * Revision 1.16  2001/08/01 19:11:02  tng
 * Add full schema constraint checking flag to the samples and the parser.
 *
 * Revision 1.15  2001/06/27 17:39:50  knoaman
 * Fix for bug #2353.
 *
 * Revision 1.14  2001/06/19 16:45:08  tng
 * Add installAdvDocHandler to SAX2XMLReader as the code is there already.
 *
 * Revision 1.13  2001/06/03 19:26:19  jberry
 * Add support for querying error count following parse; enables simple parse without requiring error handler.
 *
 * Revision 1.12  2001/05/11 13:26:21  tng
 * Copyright update.
 *
 * Revision 1.11  2001/05/03 20:34:33  tng
 * Schema: SchemaValidator update
 *
 * Revision 1.10  2001/03/30 16:46:57  tng
 * Schema: Use setDoSchema instead of setSchemaValidation which makes more sense.
 *
 * Revision 1.9  2001/03/21 21:56:08  tng
 * Schema: Add Schema Grammar, Schema Validator, and split the DTDValidator into DTDValidator, DTDScanner, and DTDGrammar.
 *
 * Revision 1.8  2001/02/15 15:56:29  tng
 * Schema: Add setSchemaValidation and getSchemaValidation for DOMParser and SAXParser.
 * Add feature "http://apache.org/xml/features/validation/schema" for SAX2XMLReader.
 * New data field  fSchemaValidation in XMLScanner as the flag.
 *
 * Revision 1.7  2001/01/15 21:26:33  tng
 * Performance Patches by David Bertoni.
 *
 * Details: (see xerces-c-dev mailing Jan 14)
 * XMLRecognizer.cpp: the internal encoding string XMLUni::fgXMLChEncodingString
 * was going through this function numerous times.  As a result, the top hot-spot
 * for the parse was _wcsicmp().  The real problem is that the Microsofts wide string
 * functions are unbelievably slow.  For things like encodings, it might be
 * better to use a special comparison function that only considers a-z and
 * A-Z as characters with case.  This works since the character set for
 * encodings is limit to printable ASCII characters.
 *
 *  XMLScanner2.cpp: This also has some case-sensitive vs. insensitive compares.
 * They are also much faster.  The other tweak is to only make a copy of an attribute
 * string if it needs to be split.  And then, the strategy is to try to use a
 * stack-based buffer, rather than a dynamically-allocated one.
 *
 * SAX2XMLReaderImpl.cpp: Again, more case-sensitive vs. insensitive comparisons.
 *
 * KVStringPair.cpp & hpp: By storing the size of the allocation, the storage can
 * likely be re-used many times, cutting down on dynamic memory allocations.
 *
 * XMLString.hpp: a more efficient implementation of stringLen().
 *
 * DTDValidator.cpp: another case of using a stack-based buffer when possible
 *
 * These patches made a big difference in parse time in some of our test
 * files, especially the ones are very attribute-heavy.
 *
 * Revision 1.6  2000/12/22 20:41:52  tng
 * XMLUni::fgEmptyString which is defined as "EMPTY" is incorrectly used as an empty string; in fact XMLUni::fgZeroLenString should be used instead
 *
 * Revision 1.5  2000/12/22 15:16:51  tng
 * SAX2-ext's LexicalHandler support added by David Bertoni.
 *
 * Revision 1.4  2000/08/09 23:39:58  jpolast
 * should be namespace-prefixes; not namespaces-prefixes
 *
 * Revision 1.3  2000/08/09 22:16:12  jpolast
 * many conformance & stability changes:
 *   - ContentHandler::resetDocument() removed
 *   - attrs param of ContentHandler::startDocument() made const
 *   - SAXExceptions thrown now have msgs
 *   - removed duplicate function signatures that had 'const'
 *       [ eg: getContentHander() ]
 *   - changed getFeature and getProperty to apply to const objs
 *   - setProperty now takes a void* instead of const void*
 *   - SAX2XMLReaderImpl does not inherit from SAXParser anymore
 *   - Reuse Validator (http://apache.org/xml/features/reuse-validator) implemented
 *   - Features & Properties now read-only during parse
 *
 * Revision 1.2  2000/08/07 22:53:44  jpolast
 * fixes for when 'namespaces' feature is turned off:
 *   * namespaces-prefixes only used when namespaces is on
 *   * URIs not looked up when namespaces is off, blank string instead
 *   * default validation scheme is validation on, auto-validation off.
 *
 * Revision 1.1  2000/08/02 18:04:41  jpolast
 * initial checkin of sax2 implemenation
 * submitted by Simon Fell (simon@fell.com)
 * and Joe Polastre (jpolast@apache.org)
 *
 *
 */

#include <xercesc/util/IOException.hpp>
#include <xercesc/util/RefStackOf.hpp>
#include <xercesc/util/XMLUniDefs.hpp>
#include <xercesc/util/Janitor.hpp>
#include <xercesc/sax2/ContentHandler.hpp>
#include <xercesc/sax2/LexicalHandler.hpp>
#include <xercesc/sax2/DeclHandler.hpp>
#include <xercesc/sax/DTDHandler.hpp>
#include <xercesc/sax/ErrorHandler.hpp>
#include <xercesc/sax/EntityResolver.hpp>
#include <xercesc/sax/SAXParseException.hpp>
#include <xercesc/sax/SAXException.hpp>
#include <xercesc/internal/XMLScannerResolver.hpp>
#include <xercesc/parsers/SAX2XMLReaderImpl.hpp>
#include <xercesc/validators/common/GrammarResolver.hpp>
#include <xercesc/framework/XMLGrammarPool.hpp>
#include <xercesc/framework/XMLSchemaDescription.hpp>
#include <xercesc/util/OutOfMemoryException.hpp>
#include <xercesc/util/XMLEntityResolver.hpp>
#include <string.h>

XERCES_CPP_NAMESPACE_BEGIN


const XMLCh gDTDEntityStr[] =
{
    chOpenSquare, chLatin_d, chLatin_t, chLatin_d, chCloseSquare, chNull
};

SAX2XMLReaderImpl::SAX2XMLReaderImpl(MemoryManager* const  manager
                                   , XMLGrammarPool* const gramPool):

    fNamespacePrefix(false)
    , fAutoValidation(false)
    , fValidation(true)
    , fParseInProgress(false)
    , fHasExternalSubset(false)
    , fElemDepth(0)
    , fAdvDHCount(0)
    , fAdvDHListSize(32)
    , fDocHandler(0)
    , fTempAttrVec(0)
    , fPrefixesStorage(0)
    , fPrefixes(0)
    , fPrefixCounts(0)
    , fDTDHandler(0)
    , fEntityResolver(0)
    , fXMLEntityResolver(0)
    , fErrorHandler(0)
    , fPSVIHandler(0)
    , fLexicalHandler(0)
    , fDeclHandler(0)
    , fAdvDHList(0)
    , fScanner(0)
    , fGrammarResolver(0)
    , fURIStringPool(0)
    , fValidator(0)
    , fMemoryManager(manager)
    , fGrammarPool(gramPool)
{
    try
    {
        initialize();
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

SAX2XMLReaderImpl::~SAX2XMLReaderImpl()
{
    cleanUp();
}

// ---------------------------------------------------------------------------
//  SAX2XMLReaderImpl: Initialize/Cleanup methods
// ---------------------------------------------------------------------------
void SAX2XMLReaderImpl::initialize()
{
    // Create grammar resolver and string pool that we pass to the scanner
    fGrammarResolver = new (fMemoryManager) GrammarResolver(fGrammarPool, fMemoryManager);
    fURIStringPool = fGrammarResolver->getStringPool();

    //  Create a scanner and tell it what validator to use. Then set us
    //  as the document event handler so we can fill the DOM document.
    fScanner = XMLScannerResolver::getDefaultScanner(0, fGrammarResolver, fMemoryManager);
    fScanner->setURIStringPool(fURIStringPool);

    // Create the initial advanced handler list array and zero it out
    fAdvDHList = (XMLDocumentHandler**) fMemoryManager->allocate
    (
        fAdvDHListSize * sizeof(XMLDocumentHandler*)
    );//new XMLDocumentHandler*[fAdvDHListSize];
    memset(fAdvDHList, 0, sizeof(void*) * fAdvDHListSize);
	
	// SAX2 default is for namespaces (feature http://xml.org/sax/features/namespaces) to be on
	setDoNamespaces(true) ;

	// default: schema is on
	setDoSchema(true);
	
    fPrefixesStorage = new (fMemoryManager) XMLStringPool(109, fMemoryManager) ;
	fPrefixes    = new (fMemoryManager) ValueStackOf<unsigned int> (30, fMemoryManager) ;
	fTempAttrVec  = new (fMemoryManager) RefVectorOf<XMLAttr>  (10, false, fMemoryManager) ;
	fPrefixCounts = new (fMemoryManager) ValueStackOf<unsigned int>(10, fMemoryManager) ;
}


void SAX2XMLReaderImpl::cleanUp()
{
    fMemoryManager->deallocate(fAdvDHList);//delete [] fAdvDHList;
    delete fScanner;
    delete fPrefixesStorage;
    delete fPrefixes;
    delete fTempAttrVec;
    delete fPrefixCounts;
    delete fGrammarResolver;
    // grammar pool must do this
    //delete fURIStringPool;
}

// ---------------------------------------------------------------------------
//  SAX2XMLReaderImpl: Advanced document handler list maintenance methods
// ---------------------------------------------------------------------------
void SAX2XMLReaderImpl::installAdvDocHandler(XMLDocumentHandler* const toInstall)
{
    // See if we need to expand and do so now if needed
    if (fAdvDHCount == fAdvDHListSize)
    {
        // Calc a new size and allocate the new temp buffer
        const unsigned int newSize = (unsigned int)(fAdvDHListSize * 1.5);
        XMLDocumentHandler** newList = (XMLDocumentHandler**) fMemoryManager->allocate
        (
            newSize * sizeof(XMLDocumentHandler*)
        );//new XMLDocumentHandler*[newSize];

        // Copy over the old data to the new list and zero out the rest
        memcpy(newList, fAdvDHList, sizeof(void*) * fAdvDHListSize);
        memset
        (
            &newList[fAdvDHListSize]
            , 0
            , sizeof(void*) * (newSize - fAdvDHListSize)
        );

        // And now clean up the old array and store the new stuff
        fMemoryManager->deallocate(fAdvDHList);//delete [] fAdvDHList;
        fAdvDHList = newList;
        fAdvDHListSize = newSize;
    }

    // Add this new guy into the empty slot
    fAdvDHList[fAdvDHCount++] = toInstall;

    //
    //  Install ourself as the document handler with the scanner. We might
    //  already be, but its not worth checking, just do it.
    //
    fScanner->setDocHandler(this);
}


bool SAX2XMLReaderImpl::removeAdvDocHandler(XMLDocumentHandler* const toRemove)
{
    // If our count is zero, can't be any installed
    if (!fAdvDHCount)
        return false;

    //
    //  Search the array until we find this handler. If we find a null entry
    //  first, we can stop there before the list is kept contiguous.
    //
    unsigned int index;
    for (index = 0; index < fAdvDHCount; index++)
    {
        //
        //  We found it. We have to keep the list contiguous, so we have to
        //  copy down any used elements after this one.
        //
        if (fAdvDHList[index] == toRemove)
        {
            //
            //  Optimize if only one entry (pretty common). Otherwise, we
            //  have to copy them down to compact them.
            //
            if (fAdvDHCount > 1)
            {
                index++;
                while (index < fAdvDHCount)
                    fAdvDHList[index - 1] = fAdvDHList[index];
            }

            // Bump down the count and zero out the last one
            fAdvDHCount--;
            fAdvDHList[fAdvDHCount] = 0;

            //
            //  If this leaves us with no advanced handlers and there is
            //  no SAX doc handler installed on us, then remove us from the
            //  scanner as the document handler.
            //
            if (!fAdvDHCount && !fDocHandler)
                fScanner->setDocHandler(0);

            return true;
        }
    }

    // Never found it
    return false;
}

// ---------------------------------------------------------------------------
//  SAX2XMLReaderImpl Validator functions
// ---------------------------------------------------------------------------
void SAX2XMLReaderImpl::setValidator(XMLValidator* valueToAdopt)
{
    fValidator = valueToAdopt;
	fScanner->setValidator(valueToAdopt);
}

XMLValidator* SAX2XMLReaderImpl::getValidator() const
{
	return fScanner->getValidator();
}

// ---------------------------------------------------------------------------
//  SAX2XMLReader Interface
// ---------------------------------------------------------------------------
int SAX2XMLReaderImpl::getErrorCount() const
{
    return fScanner->getErrorCount();
}

void SAX2XMLReaderImpl::setContentHandler(ContentHandler* const handler)
{
	fDocHandler = handler;
    if (fDocHandler)
    {
        //
        //  Make sure we are set as the document handler with the scanner.
        //  We may already be (if advanced handlers are installed), but its
        //  not worthing checking, just do it.
        //
        fScanner->setDocHandler(this);
    }
     else
    {
        //
        //  If we don't have any advanced handlers either, then deinstall us
        //  from the scanner because we don't need document events anymore.
        //
        if (!fAdvDHCount)
            fScanner->setDocHandler(0);
    }

}

void SAX2XMLReaderImpl::setDTDHandler(DTDHandler* const handler)
{
    fDTDHandler = handler;
    if (fDTDHandler)
        fScanner->setDocTypeHandler(this);
    else
        fScanner->setDocTypeHandler(0);
}


void SAX2XMLReaderImpl::setErrorHandler(ErrorHandler* const handler)
{
    //
    //  Store the handler. Then either install or deinstall us as the
    //  error reporter on the scanner.
    //
    fErrorHandler = handler;
    if (fErrorHandler) {
        fScanner->setErrorReporter(this);
        fScanner->setErrorHandler(fErrorHandler);
    }
    else {
        fScanner->setErrorReporter(0);
        fScanner->setErrorHandler(0);
    }
}

void SAX2XMLReaderImpl::setPSVIHandler(PSVIHandler* const handler)
{
    fPSVIHandler = handler;
    if (fPSVIHandler) {
        fScanner->setPSVIHandler(fPSVIHandler);        
    }
    else {
        fScanner->setPSVIHandler(0);       
    }
}

void SAX2XMLReaderImpl::setLexicalHandler(LexicalHandler* const handler)
{
    fLexicalHandler = handler;
    if (fLexicalHandler)
        fScanner->setDocTypeHandler(this);
    else
        fScanner->setDocTypeHandler(0);
}

void SAX2XMLReaderImpl::setDeclarationHandler(DeclHandler* const handler)
{
    fDeclHandler = handler;
    if (fDeclHandler)
        fScanner->setDocTypeHandler(this);
    else
        fScanner->setDocTypeHandler(0);
}


void SAX2XMLReaderImpl::setEntityResolver(EntityResolver* const resolver)
{
    fEntityResolver = resolver;
    if (fEntityResolver) {
        fScanner->setEntityHandler(this);
        fXMLEntityResolver = 0;
    }
    else {
        fScanner->setEntityHandler(0);
    }
}

void SAX2XMLReaderImpl::setXMLEntityResolver(XMLEntityResolver* const resolver)
{
    fXMLEntityResolver = resolver;
    if (fXMLEntityResolver) {
        fScanner->setEntityHandler(this);
        fEntityResolver = 0;
    }
    else {
        fScanner->setEntityHandler(0);
    }
}

void SAX2XMLReaderImpl::setExitOnFirstFatalError(const bool newState)
{
    fScanner->setExitOnFirstFatal(newState);
}

void SAX2XMLReaderImpl::setValidationConstraintFatal(const bool newState)
{
    fScanner->setValidationConstraintFatal(newState);
}

void SAX2XMLReaderImpl::parse (const   InputSource&    source)
{
    // Avoid multiple entrance
    if (fParseInProgress)
        ThrowXMLwithMemMgr(IOException, XMLExcepts::Gen_ParseInProgress, fMemoryManager);

    try
    {
        fParseInProgress = true;
        fScanner->scanDocument(source);
        fParseInProgress = false;
    }
    catch(const OutOfMemoryException&)
    {
        throw;
    }
    catch (...)
    {
        fParseInProgress = false;
        throw;
    }
}

void SAX2XMLReaderImpl::parse (const   XMLCh* const    systemId)
{
    // Avoid multiple entrance
    if (fParseInProgress)
        ThrowXMLwithMemMgr(IOException, XMLExcepts::Gen_ParseInProgress, fMemoryManager);

    try
    {
        fParseInProgress = true;
        fScanner->scanDocument(systemId);
        fParseInProgress = false;
    }
    catch(const OutOfMemoryException&)
    {
        throw;
    }
    catch (...)
    {
        fParseInProgress = false;
        throw;
    }
}

void SAX2XMLReaderImpl::parse (const   char* const     systemId)
{
    // Avoid multiple entrance
    if (fParseInProgress)
        ThrowXMLwithMemMgr(IOException, XMLExcepts::Gen_ParseInProgress, fMemoryManager);

    try
    {
        fParseInProgress = true;
        fScanner->scanDocument(systemId);
        fParseInProgress = false;
    }
    catch(const OutOfMemoryException&)
    {
        throw;
    }
    catch (...)
    {
        fParseInProgress = false;
        throw;
    }
}

// ---------------------------------------------------------------------------
//  SAX2XMLReaderImpl: Progressive parse methods
// ---------------------------------------------------------------------------
bool SAX2XMLReaderImpl::parseFirst( const   XMLCh* const    systemId
                            ,       XMLPScanToken&  toFill)
{
    //
    //  Avoid multiple entrance. We cannot enter here while a regular parse
    //  is in progress.
    //
    if (fParseInProgress)
        ThrowXMLwithMemMgr(IOException, XMLExcepts::Gen_ParseInProgress, fMemoryManager);

    return fScanner->scanFirst(systemId, toFill);
}

bool SAX2XMLReaderImpl::parseFirst( const   char* const     systemId
                            ,       XMLPScanToken&  toFill)
{
    //
    //  Avoid multiple entrance. We cannot enter here while a regular parse
    //  is in progress.
    //
    if (fParseInProgress)
        ThrowXMLwithMemMgr(IOException, XMLExcepts::Gen_ParseInProgress, fMemoryManager);

    return fScanner->scanFirst(systemId, toFill);
}

bool SAX2XMLReaderImpl::parseFirst( const   InputSource&    source
                            ,       XMLPScanToken&  toFill)
{
    //
    //  Avoid multiple entrance. We cannot enter here while a regular parse
    //  is in progress.
    //
    if (fParseInProgress)
        ThrowXMLwithMemMgr(IOException, XMLExcepts::Gen_ParseInProgress, fMemoryManager);

    return fScanner->scanFirst(source, toFill);
}

bool SAX2XMLReaderImpl::parseNext(XMLPScanToken& token)
{
    return fScanner->scanNext(token);
}

void SAX2XMLReaderImpl::parseReset(XMLPScanToken& token)
{
    // Reset the scanner
    fScanner->scanReset(token);
}

// ---------------------------------------------------------------------------
//  SAX2XMLReaderImpl: Overrides of the XMLDocumentHandler interface
// ---------------------------------------------------------------------------
void SAX2XMLReaderImpl::docCharacters(  const   XMLCh* const    chars
                                , const unsigned int    length
                                , const bool            cdataSection)
{
    // Suppress the chars before the root element.
    if (fElemDepth)
    {
        // Call the installed LexicalHandler.
        if (cdataSection && fLexicalHandler)
            fLexicalHandler->startCDATA();

        // Just map to the SAX document handler
        if (fDocHandler)
            fDocHandler->characters(chars, length);

        // Call the installed LexicalHandler.
        if (cdataSection && fLexicalHandler)
            fLexicalHandler->endCDATA();
    }

    //
    //  If there are any installed advanced handlers, then lets call them
    //  with this info.
    //
    for (unsigned int index = 0; index < fAdvDHCount; index++)
        fAdvDHList[index]->docCharacters(chars, length, cdataSection);
}


void SAX2XMLReaderImpl::docComment(const XMLCh* const commentText)
{
    // Call the installed LexicalHandler.
    if (fLexicalHandler)
    {
        // SAX2 reports comment text like characters -- as an
        // array with a length.
        fLexicalHandler->comment(commentText, XMLString::stringLen(commentText));
    }

    //
    //  OK, if there are any installed advanced handlers,
    // then let's call them with this info.
    //
    for (unsigned int index = 0; index < fAdvDHCount; index++)
        fAdvDHList[index]->docComment(commentText);
}


void SAX2XMLReaderImpl::XMLDecl( const  XMLCh* const    versionStr
                        , const XMLCh* const    encodingStr
                        , const XMLCh* const    standaloneStr
                        , const XMLCh* const    actualEncodingStr
                        )
{
    // SAX has no way to report this event. But, if there are any installed
    //  advanced handlers, then lets call them with this info.
    //
    for (unsigned int index = 0; index < fAdvDHCount; index++)
        fAdvDHList[index]->XMLDecl( versionStr,
                                    encodingStr,
                                    standaloneStr,
                                    actualEncodingStr );
}


void SAX2XMLReaderImpl::docPI(  const   XMLCh* const    target
                        , const XMLCh* const    data)
{
    // Just map to the SAX document handler
    if (fDocHandler)
        fDocHandler->processingInstruction(target, data);

    //
    //  If there are any installed advanced handlers, then lets call them
    //  with this info.
    //
    for (unsigned int index = 0; index < fAdvDHCount; index++)
        fAdvDHList[index]->docPI(target, data);
}


void SAX2XMLReaderImpl::endDocument()
{
    if (fDocHandler)
        fDocHandler->endDocument();

    //
    //  If there are any installed advanced handlers, then lets call them
    //  with this info.
    //
    for (unsigned int index = 0; index < fAdvDHCount; index++)
        fAdvDHList[index]->endDocument();
}


void SAX2XMLReaderImpl::endEntityReference(const XMLEntityDecl& entityDecl)
{
   // Call the installed LexicalHandler.
   if (fLexicalHandler)
        fLexicalHandler->endEntity(entityDecl.getName());

    //
    //  SAX has no way to report this event. But, if there are any installed
    //  advanced handlers, then lets call them with this info.
    //
    for (unsigned int index = 0; index < fAdvDHCount; index++)
        fAdvDHList[index]->endEntityReference(entityDecl);
}


void SAX2XMLReaderImpl::ignorableWhitespace(const   XMLCh* const    chars
                                    , const unsigned int    length
                                    , const bool            cdataSection)
{
    // Do not report the whitespace before the root element.
    if (!fElemDepth)
        return;

    // Just map to the SAX document handler
    if (fDocHandler)
        fDocHandler->ignorableWhitespace(chars, length);

    //
    //  If there are any installed advanced handlers, then lets call them
    //  with this info.
    //
    for (unsigned int index = 0; index < fAdvDHCount; index++)
        fAdvDHList[index]->ignorableWhitespace(chars, length, cdataSection);
}


void SAX2XMLReaderImpl::resetDocument()
{
    //
    //  If there are any installed advanced handlers, then lets call them
    //  with this info.
    //
    for (unsigned int index = 0; index < fAdvDHCount; index++)
        fAdvDHList[index]->resetDocument();

    // Make sure our element depth flag gets set back to zero
    fElemDepth = 0;

    // reset prefix counters and prefix map
    fPrefixCounts->removeAllElements();
    fPrefixes->removeAllElements();
    fPrefixesStorage->flushAll();
}


void SAX2XMLReaderImpl::startDocument()
{
    // Just map to the SAX document handler
    if (fDocHandler)
    {
        fDocHandler->setDocumentLocator(fScanner->getLocator());
        fDocHandler->startDocument();
    }

    //
    //  If there are any installed advanced handlers, then lets call them
    //  with this info.
    //
    for (unsigned int index = 0; index < fAdvDHCount; index++)
        fAdvDHList[index]->startDocument();
}


void SAX2XMLReaderImpl::
startElement(   const   XMLElementDecl&         elemDecl
                , const unsigned int            elemURLId
                , const XMLCh* const            elemPrefix
                , const RefVectorOf<XMLAttr>&   attrList
                , const unsigned int            attrCount
                , const bool                    isEmpty
                , const bool                    isRoot)
{
    // Bump the element depth counter if not empty
    if (!isEmpty)
        fElemDepth++;

    if (fDocHandler)
    {
        const XMLCh* const elemQName =
            elemDecl.getFullName();

        if (getDoNamespaces())
        {
            unsigned int numPrefix = 0;
            const XMLCh*   nsString = XMLUni::fgXMLNSString;
            const XMLAttr* tempAttr = 0;

            if (!fNamespacePrefix)
            {
                fTempAttrVec->removeAllElements();
            }

            for (unsigned int i = 0; i < attrCount; i++)
            {
                const XMLCh*   nsPrefix = 0;
                const XMLCh*   nsURI    = 0;

                tempAttr = attrList.elementAt(i);
                if (XMLString::equals(tempAttr->getQName(), nsString))
                    nsURI = tempAttr->getValue();
                if (XMLString::equals(tempAttr->getPrefix(), nsString))
                {
                    nsPrefix = tempAttr->getName();
                    nsURI = tempAttr->getValue();
                }
                if (!fNamespacePrefix)
                {
                    if (nsURI == 0)
                        fTempAttrVec->addElement((XMLAttr*)tempAttr);
                }
                if (nsURI != 0)
                {
                    if (nsPrefix == 0)
                        nsPrefix = XMLUni::fgZeroLenString;
                    fDocHandler->startPrefixMapping(nsPrefix, nsURI);
                    unsigned int nPrefixId=fPrefixesStorage->addOrFind(nsPrefix);
                    fPrefixes->push(nPrefixId) ;
                    numPrefix++;
                }
            }
            fPrefixCounts->push(numPrefix) ;
            if (!fNamespacePrefix)
                fAttrList.setVector(fTempAttrVec, fTempAttrVec->size(), fScanner);
            else
                fAttrList.setVector(&attrList, attrCount, fScanner);

            // call startElement() with namespace declarations
            fDocHandler->startElement
            (
                fScanner->getURIText(elemURLId)
                , elemDecl.getBaseName()
                , elemQName
                , fAttrList
            );
        }
        else // no namespace
        {
            fAttrList.setVector(&attrList, attrCount, fScanner);
            fDocHandler->startElement(XMLUni::fgZeroLenString,
										elemDecl.getBaseName(),
										elemQName,
										fAttrList);
        }


        // If its empty, send the end tag event now
        if (isEmpty)
        {
            // call endPrefixMapping appropriately.
            if (getDoNamespaces())
            {
                fDocHandler->endElement
                (
                    fScanner->getURIText(elemURLId)
                    , elemDecl.getBaseName()
                    , elemQName
                );

                unsigned int numPrefix = fPrefixCounts->pop();
                for (unsigned int i = 0; i < numPrefix; ++i)
                {
                    unsigned int nPrefixId = fPrefixes->pop() ;
                    fDocHandler->endPrefixMapping( fPrefixesStorage->getValueForId(nPrefixId) );
                }
            }
            else
            {
                fDocHandler->endElement(XMLUni::fgZeroLenString,
                                elemDecl.getBaseName(),
                                elemQName);
            }
        }
    }

    //
    //  If there are any installed advanced handlers, then lets call them
    //  with this info.
    //
    for (unsigned int index = 0; index < fAdvDHCount; index++)
    {
        fAdvDHList[index]->startElement
        (
            elemDecl
            , elemURLId
            , elemPrefix
            , attrList
            , attrCount
            , isEmpty
            , isRoot
        );
    }
}

void SAX2XMLReaderImpl::endElement( const   XMLElementDecl& elemDecl
                            , const unsigned int    uriId
                            , const bool            isRoot
                            , const XMLCh* const    elemPrefix)
{
    // Just map to the SAX document handler
    if (fDocHandler)
    {

        // get the prefixes back so that we can call endPrefixMapping()
        if (getDoNamespaces())
        {
            fDocHandler->endElement
            (
                fScanner->getURIText(uriId)
                , elemDecl.getBaseName()
                , elemDecl.getFullName()
            );

            unsigned int numPrefix = fPrefixCounts->pop();
            for (unsigned int i = 0; i < numPrefix; i++)
            {
                unsigned int nPrefixId = fPrefixes->pop() ;
                fDocHandler->endPrefixMapping( fPrefixesStorage->getValueForId(nPrefixId) );
            }
        }
        else
        {
            fDocHandler->endElement(XMLUni::fgZeroLenString,
            elemDecl.getBaseName(),
            elemDecl.getFullName() );
        }
    }

    //
    //  If there are any installed advanced handlers, then lets call them
    //  with this info.
    //
    for (unsigned int index = 0; index < fAdvDHCount; index++)
        fAdvDHList[index]->endElement(elemDecl, uriId, isRoot, elemPrefix);

    //
    //  Dump the element depth down again. Don't let it underflow in case
    //  of malformed XML.
    //
    if (fElemDepth)
        fElemDepth--;
}

void SAX2XMLReaderImpl::startEntityReference(const XMLEntityDecl& entityDecl)
{
   // Call the installed LexicalHandler.
   if (fLexicalHandler)
        fLexicalHandler->startEntity(entityDecl.getName());
    //
    //  SAX has no way to report this. But, If there are any installed
    //  advanced handlers, then lets call them with this info.
    //
    for (unsigned int index = 0; index < fAdvDHCount; index++)
        fAdvDHList[index]->startEntityReference(entityDecl);
}

// ---------------------------------------------------------------------------
//  SAX2XMLReaderImpl: Overrides of the DocTypeHandler interface
// ---------------------------------------------------------------------------
void SAX2XMLReaderImpl::attDef( const   DTDElementDecl& elemDecl
                        , const DTDAttDef&      attDef
                        , const bool            ignoring)
{
    if (fDeclHandler && !ignoring) {

        XMLAttDef::AttTypes attType = attDef.getType();
        XMLAttDef::DefAttTypes defAttType = attDef.getDefaultType();
        const XMLCh* defAttTypeStr = XMLUni::fgNullString;
        bool isEnumeration = (attType == XMLAttDef::Notation || attType == XMLAttDef::Enumeration);
        XMLBuffer enumBuf(128, fMemoryManager);

        if (defAttType == XMLAttDef::Fixed ||
            defAttType == XMLAttDef::Implied ||
            defAttType == XMLAttDef::Required) {
            defAttTypeStr = attDef.getDefAttTypeString(defAttType, fMemoryManager);
        }

        if (isEnumeration) {

            const XMLCh* enumString = attDef.getEnumeration();
            unsigned int enumLen = XMLString::stringLen(enumString);

            if (attType == XMLAttDef::Notation) {

                enumBuf.set(XMLUni::fgNotationString);
                enumBuf.append(chSpace);
            }

            enumBuf.append(chOpenParen);

            for (unsigned int i=0; i<enumLen; i++) {
                if (enumString[i] == chSpace)
                    enumBuf.append(chPipe);
                else
                    enumBuf.append(enumString[i]);
            }

            enumBuf.append(chCloseParen);
        }

        fDeclHandler->attributeDecl(elemDecl.getFullName(),
                                    attDef.getFullName(),
                                    (isEnumeration) ? enumBuf.getRawBuffer()
                                                    : attDef.getAttTypeString(attDef.getType(), fMemoryManager),
                                    defAttTypeStr,
                                    attDef.getValue());
    }
}


void SAX2XMLReaderImpl::doctypeComment(const XMLCh* const commentText)
{
   if (fLexicalHandler)
   {
        // SAX2 reports comment text like characters -- as an
        // array with a length.
        fLexicalHandler->comment(commentText, XMLString::stringLen(commentText));
   }
}


void SAX2XMLReaderImpl::doctypeDecl(const   DTDElementDecl& elemDecl
                            , const XMLCh* const    publicId
                            , const XMLCh* const    systemId
                            , const bool            hasIntSubset
                            , const bool            hasExtSubset)
{
    // Call the installed LexicalHandler.
    if (fLexicalHandler && (hasIntSubset || hasExtSubset))
        fLexicalHandler->startDTD(elemDecl.getFullName(), publicId, systemId);

    fHasExternalSubset = hasExtSubset;

    // Unused by SAX DTDHandler interface at this time
}


void SAX2XMLReaderImpl::doctypePI(  const   XMLCh* const
                            , const XMLCh* const)
{
    // Unused by SAX DTDHandler interface at this time
}


void SAX2XMLReaderImpl::doctypeWhitespace(  const   XMLCh* const
                                    , const unsigned int)
{
    // Unused by SAX DTDHandler interface at this time
}


void SAX2XMLReaderImpl::elementDecl(const DTDElementDecl& elemDecl,
                                    const bool isIgnored)
{
    if (fDeclHandler && !isIgnored)
        fDeclHandler->elementDecl(elemDecl.getFullName(),
                                  elemDecl.getFormattedContentModel());
}


void SAX2XMLReaderImpl::endAttList(const DTDElementDecl&)
{
    // Unused by SAX DTDHandler interface at this time
}


void SAX2XMLReaderImpl::endIntSubset()
{
   // Call the installed LexicalHandler.
   if (!fHasExternalSubset && fLexicalHandler)
        fLexicalHandler->endDTD();

    // Unused by SAX DTDHandler interface at this time
}


void SAX2XMLReaderImpl::endExtSubset()
{
    // Call the installed LexicalHandler.
    if (fLexicalHandler) {

        fLexicalHandler->endEntity(gDTDEntityStr);
        fLexicalHandler->endDTD();
    }

    // Unused by SAX DTDHandler interface at this time
}


void SAX2XMLReaderImpl::entityDecl( const   DTDEntityDecl&  entityDecl
                            , const bool            isPEDecl
                            , const bool            isIgnored)
{
    //
    //  If we have a DTD handler, and this entity is not ignored, and
    //  its an unparsed entity, then send this one, else if we have a Decl
    //  handler then send this one.
    //
    if (!isIgnored) {

        if (entityDecl.isUnparsed()) {

            if (fDTDHandler) {
                fDTDHandler->unparsedEntityDecl
                (
                    entityDecl.getName()
                    , entityDecl.getPublicId()
                    , entityDecl.getSystemId()
                    , entityDecl.getNotationName()
                );
            }
        }
        else if (fDeclHandler) {

            const XMLCh* entityName = entityDecl.getName();
            ArrayJanitor<XMLCh> tmpNameJan(0);

            if (isPEDecl) {

                unsigned int nameLen = XMLString::stringLen(entityName);
                XMLCh* tmpName = (XMLCh*) fMemoryManager->allocate
                (
                    (nameLen + 2) * sizeof(XMLCh)
                );//new XMLCh[nameLen + 2];

                tmpNameJan.reset(tmpName, fMemoryManager);
                tmpName[0] = chPercent;
                XMLString::copyString(tmpName + 1, entityName);
                entityName = tmpName;
            }

            if (entityDecl.isExternal()) {
                fDeclHandler->externalEntityDecl
                (
                    entityName
                    , entityDecl.getPublicId()
                    , entityDecl.getSystemId()
                );
            }
            else {
                fDeclHandler->internalEntityDecl
                (
                    entityName
                    , entityDecl.getValue()
                );
            }
        }
    }
}


void SAX2XMLReaderImpl::resetDocType()
{
    fHasExternalSubset = false;
    // Just map to the DTD handler
    if (fDTDHandler)
        fDTDHandler->resetDocType();
}


void SAX2XMLReaderImpl::notationDecl(   const   XMLNotationDecl&    notDecl
                                , const bool                isIgnored)
{
    if (fDTDHandler && !isIgnored)
    {
        fDTDHandler->notationDecl
        (
            notDecl.getName()
            , notDecl.getPublicId()
            , notDecl.getSystemId()
        );
    }
}


void SAX2XMLReaderImpl::startAttList(const DTDElementDecl&)
{
    // Unused by SAX DTDHandler interface at this time
}


void SAX2XMLReaderImpl::startIntSubset()
{
    // Unused by SAX DTDHandler interface at this time
}


void SAX2XMLReaderImpl::startExtSubset()
{
    if (fLexicalHandler)
        fLexicalHandler->startEntity(gDTDEntityStr);
}


void SAX2XMLReaderImpl::TextDecl(   const  XMLCh* const
                            , const XMLCh* const)
{
    // Unused by SAX DTDHandler interface at this time
}


// ---------------------------------------------------------------------------
//  SAX2XMLReaderImpl: Handlers for the XMLEntityHandler interface
// ---------------------------------------------------------------------------
void SAX2XMLReaderImpl::endInputSource(const InputSource&)
{
}

bool SAX2XMLReaderImpl::expandSystemId(const XMLCh* const, XMLBuffer&)
{
    return false;
}


void SAX2XMLReaderImpl::resetEntities()
{
    // Nothing to do for this one
}


InputSource* SAX2XMLReaderImpl::resolveEntity(   const   XMLCh* const    publicId
                                               , const   XMLCh* const    systemId
                                               , const   XMLCh* const)
{
    // Just map to the SAX entity resolver handler
    if (fEntityResolver)
        return fEntityResolver->resolveEntity(publicId, systemId);
    return 0;
}

InputSource* SAX2XMLReaderImpl::resolveEntity(XMLResourceIdentifier* resourceIdentifier)
{
    //
    //  Just map it to the SAX entity resolver. If there is not one installed,
    //  return a null pointer to cause the default resolution.
    //
    if (fEntityResolver)
        return fEntityResolver->resolveEntity(resourceIdentifier->getPublicId(), 
                                                resourceIdentifier->getSystemId());
    if (fXMLEntityResolver)
        return fXMLEntityResolver->resolveEntity(resourceIdentifier);

    return 0;
}
 
void SAX2XMLReaderImpl::startInputSource(const InputSource&)
{
    // Nothing to do for this one
}

// ---------------------------------------------------------------------------
//  SAX2XMLReaderImpl: Overrides of the XMLErrorReporter interface
// ---------------------------------------------------------------------------
void SAX2XMLReaderImpl::resetErrors()
{
    if (fErrorHandler)
        fErrorHandler->resetErrors();
}


void SAX2XMLReaderImpl::error(  const   unsigned int
                        , const XMLCh* const
                        , const XMLErrorReporter::ErrTypes  errType
                        , const XMLCh* const                errorText
                        , const XMLCh* const                systemId
                        , const XMLCh* const                publicId
                        , const XMLSSize_t                  lineNum
                        , const XMLSSize_t                  colNum)
{
    SAXParseException toThrow = SAXParseException
    (
        errorText
        , publicId
        , systemId
        , lineNum
        , colNum
        , fMemoryManager
    );

    if (!fErrorHandler)
    {
        if (errType == XMLErrorReporter::ErrType_Fatal)
            throw toThrow;
        else
            return;
    }

    if (errType == XMLErrorReporter::ErrType_Warning)
        fErrorHandler->warning(toThrow);
    else if (errType == XMLErrorReporter::ErrType_Fatal)
        fErrorHandler->fatalError(toThrow);
    else
        fErrorHandler->error(toThrow);
}


// ---------------------------------------------------------------------------
//  SAX2XMLReaderImpl: Features and Properties
// ---------------------------------------------------------------------------

void SAX2XMLReaderImpl::setFeature(const XMLCh* const name, const bool value)
{

    if (fParseInProgress)
        throw SAXNotSupportedException("Feature modification is not supported during parse.", fMemoryManager);
	
    if (XMLString::compareIStringASCII(name, XMLUni::fgSAX2CoreNameSpaces) == 0)
    {
        setDoNamespaces(value);
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgSAX2CoreValidation) == 0)
    {
        fValidation = value;
        if (fValidation)
            if (fAutoValidation)
                setValidationScheme(Val_Auto);
            else
                setValidationScheme(Val_Always);
        else
            setValidationScheme(Val_Never);
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgSAX2CoreNameSpacePrefixes) == 0)
    {
        fNamespacePrefix = value;
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgXercesDynamic) == 0)
    {
        fAutoValidation = value;
        // for auto validation, the sax2 core validation feature must also be enabled.
        if (fValidation)
            if (fAutoValidation)
                setValidationScheme(Val_Auto);
            else
                setValidationScheme(Val_Always);
        else
            setValidationScheme(Val_Never);
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgXercesSchema) == 0)
    {
        setDoSchema(value);
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgXercesSchemaFullChecking) == 0)
    {
        fScanner->setValidationSchemaFullChecking(value);
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgXercesIdentityConstraintChecking) == 0)
    {
        fScanner->setIdentityConstraintChecking(value);
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgXercesLoadExternalDTD) == 0)
    {
        fScanner->setLoadExternalDTD(value);
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgXercesContinueAfterFatalError) == 0)
    {
        fScanner->setExitOnFirstFatal(!value);
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgXercesValidationErrorAsFatal) == 0)
    {
        fScanner->setValidationConstraintFatal(value);
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgXercesCacheGrammarFromParse) == 0)
    {
        fScanner->cacheGrammarFromParse(value);

        if (value)
            fScanner->useCachedGrammarInParse(value);
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgXercesUseCachedGrammarInParse) == 0)
    {
        if (value || !fScanner->isCachingGrammarFromParse())
            fScanner->useCachedGrammarInParse(value);
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgXercesCalculateSrcOfs) == 0)
    {
        fScanner->setCalculateSrcOfs(value);
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgXercesStandardUriConformant) == 0)
    {
        fScanner->setStandardUriConformant(value);
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgXercesGenerateSyntheticAnnotations) == 0)
    {
        fScanner->setGenerateSyntheticAnnotations(value);
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgXercesValidateAnnotations) == 0)
    {
        fScanner->setValidateAnnotations(value);
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgXercesIgnoreCachedDTD) == 0)
    {
        fScanner->setIgnoredCachedDTD(value);
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgXercesIgnoreAnnotations) == 0)
    {
        fScanner->setIgnoreAnnotations(value);
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgXercesDisableDefaultEntityResolution) == 0)
    {
        fScanner->setDisableDefaultEntityResolution(value);
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgXercesSkipDTDValidation) == 0)
    {
        fScanner->setSkipDTDValidation(value);
    }
    else
       throw SAXNotRecognizedException("Unknown Feature", fMemoryManager);
}

bool SAX2XMLReaderImpl::getFeature(const XMLCh* const name) const
{
    if (XMLString::compareIStringASCII(name, XMLUni::fgSAX2CoreNameSpaces) == 0)
        return getDoNamespaces();
    else if (XMLString::compareIStringASCII(name, XMLUni::fgSAX2CoreValidation) == 0)
        return fValidation;
    else if (XMLString::compareIStringASCII(name, XMLUni::fgSAX2CoreNameSpacePrefixes) == 0)
        return fNamespacePrefix;
    else if (XMLString::compareIStringASCII(name, XMLUni::fgXercesDynamic) == 0)
        return fAutoValidation;
    else if (XMLString::compareIStringASCII(name, XMLUni::fgXercesSchema) == 0)
        return getDoSchema();
    else if (XMLString::compareIStringASCII(name, XMLUni::fgXercesSchemaFullChecking) == 0)
        return fScanner->getValidationSchemaFullChecking();
    else if (XMLString::compareIStringASCII(name, XMLUni::fgXercesIdentityConstraintChecking) == 0)
        return fScanner->getIdentityConstraintChecking();
    else if (XMLString::compareIStringASCII(name, XMLUni::fgXercesLoadExternalDTD) == 0)
        return fScanner->getLoadExternalDTD();
    else if (XMLString::compareIStringASCII(name, XMLUni::fgXercesContinueAfterFatalError) == 0)
        return !fScanner->getExitOnFirstFatal();
    else if (XMLString::compareIStringASCII(name, XMLUni::fgXercesValidationErrorAsFatal) == 0)
        return fScanner->getValidationConstraintFatal();
    else if (XMLString::compareIStringASCII(name, XMLUni::fgXercesCacheGrammarFromParse) == 0)
        return fScanner->isCachingGrammarFromParse();
    else if (XMLString::compareIStringASCII(name, XMLUni::fgXercesUseCachedGrammarInParse) == 0)
        return fScanner->isUsingCachedGrammarInParse();
    else if (XMLString::compareIStringASCII(name, XMLUni::fgXercesCalculateSrcOfs) == 0)
        return fScanner->getCalculateSrcOfs();
    else if (XMLString::compareIStringASCII(name, XMLUni::fgXercesStandardUriConformant) == 0)
        return fScanner->getStandardUriConformant();
    else if (XMLString::compareIStringASCII(name, XMLUni::fgXercesGenerateSyntheticAnnotations) == 0)
        return fScanner->getGenerateSyntheticAnnotations();
    else if (XMLString::compareIStringASCII(name, XMLUni::fgXercesValidateAnnotations) == 0)
        return fScanner->getValidateAnnotations();
    else if (XMLString::compareIStringASCII(name, XMLUni::fgXercesIgnoreCachedDTD) == 0)
        return fScanner->getIgnoreCachedDTD();
    else if (XMLString::compareIStringASCII(name, XMLUni::fgXercesIgnoreAnnotations) == 0)
        return fScanner->getIgnoreAnnotations();
    else if (XMLString::compareIStringASCII(name, XMLUni::fgXercesDisableDefaultEntityResolution) == 0)
        return fScanner->getDisableDefaultEntityResolution();
    else if (XMLString::compareIStringASCII(name, XMLUni::fgXercesSkipDTDValidation) == 0)
        return fScanner->getSkipDTDValidation();
    else
       throw SAXNotRecognizedException("Unknown Feature", fMemoryManager);

    return false;
}

void SAX2XMLReaderImpl::setProperty(const XMLCh* const name, void* value)
{
	if (fParseInProgress)
		throw SAXNotSupportedException("Property modification is not supported during parse.", fMemoryManager);

	if (XMLString::compareIStringASCII(name, XMLUni::fgXercesSchemaExternalSchemaLocation) == 0)
	{
		fScanner->setExternalSchemaLocation((XMLCh*)value);
	}
	else if (XMLString::compareIStringASCII(name, XMLUni::fgXercesSchemaExternalNoNameSpaceSchemaLocation) == 0)
	{
		fScanner->setExternalNoNamespaceSchemaLocation((XMLCh*)value);
	}
	else if (XMLString::compareIStringASCII(name, XMLUni::fgXercesSecurityManager) == 0)
	{
		fScanner->setSecurityManager((SecurityManager*)value);
	}
    else if (XMLString::equals(name, XMLUni::fgXercesScannerName))
    {
        XMLScanner* tempScanner = XMLScannerResolver::resolveScanner
        (
            (const XMLCh*) value
            , fValidator
            , fGrammarResolver
            , fMemoryManager
        );

        if (tempScanner) {

            tempScanner->setParseSettings(fScanner);
            tempScanner->setURIStringPool(fURIStringPool);
            delete fScanner;
            fScanner = tempScanner;
        }
    }
    else
       throw SAXNotRecognizedException("Unknown Property", fMemoryManager);
}


void* SAX2XMLReaderImpl::getProperty(const XMLCh* const name) const
{
    if (XMLString::compareIStringASCII(name, XMLUni::fgXercesSchemaExternalSchemaLocation) == 0)
        return (void*)fScanner->getExternalSchemaLocation();
    else if (XMLString::compareIStringASCII(name, XMLUni::fgXercesSchemaExternalNoNameSpaceSchemaLocation) == 0)
        return (void*)fScanner->getExternalNoNamespaceSchemaLocation();
    else if (XMLString::compareIStringASCII(name, XMLUni::fgXercesSecurityManager) == 0)
        return (void*)fScanner->getSecurityManager();
    else if (XMLString::equals(name, XMLUni::fgXercesScannerName))
        return (void*)fScanner->getName();
    else
        throw SAXNotRecognizedException("Unknown Property", fMemoryManager);
    return 0;
}


// ---------------------------------------------------------------------------
//  SAX2XMLReaderImpl: Private getters and setters for conveniences
// ---------------------------------------------------------------------------

void SAX2XMLReaderImpl::setValidationScheme(const ValSchemes newScheme)
{
    if (newScheme == Val_Never)
        fScanner->setValidationScheme(XMLScanner::Val_Never);
    else if (newScheme == Val_Always)
        fScanner->setValidationScheme(XMLScanner::Val_Always);
    else
        fScanner->setValidationScheme(XMLScanner::Val_Auto);
}

void SAX2XMLReaderImpl::setDoNamespaces(const bool newState)
{
    fScanner->setDoNamespaces(newState);
}

bool SAX2XMLReaderImpl::getDoNamespaces() const
{
    return fScanner->getDoNamespaces();
}

void SAX2XMLReaderImpl::setDoSchema(const bool newState)
{
    fScanner->setDoSchema(newState);
}

bool SAX2XMLReaderImpl::getDoSchema() const
{
    return fScanner->getDoSchema();
}


// ---------------------------------------------------------------------------
//  SAX2XMLReaderImpl: Grammar preparsing
// ---------------------------------------------------------------------------
Grammar* SAX2XMLReaderImpl::loadGrammar(const char* const systemId,
                                        const short grammarType,
                                        const bool toCache)
{
    // Avoid multiple entrance
    if (fParseInProgress)
        ThrowXMLwithMemMgr(IOException, XMLExcepts::Gen_ParseInProgress, fMemoryManager);

    Grammar* grammar = 0;
    try
    {
        fParseInProgress = true;
        grammar = fScanner->loadGrammar(systemId, grammarType, toCache);
        fParseInProgress = false;
    }
    catch(const OutOfMemoryException&)
    {
        throw;
    }
    catch(...)
    {
        fParseInProgress = false;
        throw;
    }

    return grammar;
}

Grammar* SAX2XMLReaderImpl::loadGrammar(const XMLCh* const systemId,
                                        const short grammarType,
                                        const bool toCache)
{
    // Avoid multiple entrance
    if (fParseInProgress)
        ThrowXMLwithMemMgr(IOException, XMLExcepts::Gen_ParseInProgress, fMemoryManager);

    Grammar* grammar = 0;
    try
    {
        fParseInProgress = true;
        grammar = fScanner->loadGrammar(systemId, grammarType, toCache);
        fParseInProgress = false;
    }
    catch(const OutOfMemoryException&)
    {
        throw;
    }
    catch(...)
    {
        fParseInProgress = false;
        throw;
    }

    return grammar;
}

Grammar* SAX2XMLReaderImpl::loadGrammar(const InputSource& source,
                                        const short grammarType,
                                        const bool toCache)
{
    // Avoid multiple entrance
    if (fParseInProgress)
        ThrowXMLwithMemMgr(IOException, XMLExcepts::Gen_ParseInProgress, fMemoryManager);

    Grammar* grammar = 0;
    try
    {
        fParseInProgress = true;
        grammar = fScanner->loadGrammar(source, grammarType, toCache);
        fParseInProgress = false;
    }
    catch(const OutOfMemoryException&)
    {
        throw;
    }
    catch(...)
    {
        fParseInProgress = false;
        throw;
    }

    return grammar;
}

void SAX2XMLReaderImpl::resetCachedGrammarPool()
{
    fGrammarResolver->resetCachedGrammar();
}

void SAX2XMLReaderImpl::setInputBufferSize(const size_t bufferSize)
{
    fScanner->setInputBufferSize(bufferSize);
}

Grammar* SAX2XMLReaderImpl::getGrammar(const XMLCh* const nameSpaceKey)
{
    return fGrammarResolver->getGrammar(nameSpaceKey);
}


XERCES_CPP_NAMESPACE_END

