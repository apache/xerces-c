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
 * Revision 1.39  2005/03/30 00:55:14  cargilld
 * Begin work on adding some new features by checking in the feature handling support.
 *
 * Revision 1.38  2004/12/30 15:23:41  amassari
 * Notify advanced handlers of the whitespace before and after the root document element (jira# 729)
 *
 * Revision 1.37  2004/12/07 19:45:43  knoaman
 * An option to ignore a cached DTD grammar when a document contains an
 * internal and external subset.
 *
 * Revision 1.36  2004/09/29 19:00:29  peiyongz
 * [jira1207] --patch from Dan Rosen
 *
 * Revision 1.35  2004/09/28 02:14:14  cargilld
 * Add support for validating annotations.
 *
 * Revision 1.34  2004/09/23 01:09:55  cargilld
 * Add support for generating synthetic XSAnnotations.  When a schema component has non-schema attributes and no child attributes create a synthetic XSAnnotation (under feature control) so the non-schema attributes can be recovered under PSVI.
 *
 * Revision 1.33  2004/09/08 13:56:17  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.32  2004/04/13 16:53:26  peiyongz
 * get/setIdentityConstraintChecking
 *
 * Revision 1.31  2004/01/29 11:46:32  cargilld
 * Code cleanup changes to get rid of various compiler diagnostic messages.
 *
 * Revision 1.30  2003/12/17 00:18:35  cargilld
 * Update to memory management so that the static memory manager (one used to call Initialize) is only for static data.
 *
 * Revision 1.29  2003/11/21 22:38:50  neilg
 * Enable grammar pools and grammar resolvers to manufacture
 * XSModels.  This also cleans up handling in the
 * parser classes by eliminating the need to tell
 * the grammar pool that schema compoments need to be produced.
 * Thanks to David Cargill.
 *
 * Revision 1.28  2003/11/06 15:30:07  neilg
 * first part of PSVI/schema component model implementation, thanks to David Cargill.  This covers setting the PSVIHandler on parser objects, as well as implementing XSNotation, XSSimpleTypeDefinition, XSIDCDefinition, and most of XSWildcard, XSComplexTypeDefinition, XSElementDeclaration, XSAttributeDeclaration and XSAttributeUse.
 *
 * Revision 1.27  2003/10/30 21:37:31  knoaman
 * Enhanced Entity Resolver Support. Thanks to David Cargill.
 *
 * Revision 1.26  2003/10/01 16:32:38  neilg
 * improve handling of out of memory conditions, bug #23415.  Thanks to David Cargill.
 *
 * Revision 1.25  2003/09/16 18:30:54  neilg
 * make Grammar pool be responsible for creating and owning URI string pools.  This is one more step towards having grammars be independent of the parsers involved in their creation
 *
 * Revision 1.24  2003/08/13 15:43:24  knoaman
 * Use memory manager when creating SAX exceptions.
 *
 * Revision 1.23  2003/07/31 17:05:48  peiyongz
 * using getGrammar(URI)
 *
 * Revision 1.22  2003/07/10 19:48:24  peiyongz
 * Stateless Grammar: Initialize scanner with grammarResolver,
 *
 * Revision 1.21  2003/06/25 22:36:46  peiyongz
 * to use new GrammarResolver::getGrammar()
 *
 * Revision 1.20  2003/06/20 18:55:54  peiyongz
 * Stateless Grammar Pool :: Part I
 *
 * Revision 1.19  2003/05/18 14:02:05  knoaman
 * Memory manager implementation: pass per instance manager.
 *
 * Revision 1.18  2003/05/16 21:36:59  knoaman
 * Memory manager implementation: Modify constructors to pass in the memory manager.
 *
 * Revision 1.17  2003/05/15 18:26:50  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.16  2003/04/17 21:58:50  neilg
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
 * add security manager
 * 
 * Revision 1.15  2003/02/04 19:27:43  knoaman
 * Performance: use global buffer to eliminate repetitive memory creation/deletion.
 *
 * Revision 1.14  2003/01/09 19:07:08  tng
 * [Bug 15802] Add "const" qualifier to getURIText.
 *
 * Revision 1.13  2003/01/03 20:09:36  tng
 * New feature StandardUriConformant to force strict standard uri conformance.
 *
 * Revision 1.12  2002/12/27 16:16:51  knoaman
 * Set scanner options and handlers.
 *
 * Revision 1.11  2002/12/23 15:23:18  knoaman
 * Added a public api to various parsers to return the src offset within the input
 * source.
 *
 * Revision 1.10  2002/12/04 01:57:09  knoaman
 * Scanner re-organization.
 *
 * Revision 1.9  2002/11/04 14:57:03  tng
 * C++ Namespace Support.
 *
 * Revision 1.8  2002/08/14 15:20:38  knoaman
 * [Bug 3111] Problem with LexicalHandler::startDTD() and LexicalHandler::endDTD().
 *
 * Revision 1.7  2002/07/11 18:27:04  knoaman
 * Grammar caching/preparsing - initial implementation.
 *
 * Revision 1.6  2002/05/30 16:20:09  tng
 * Add feature to optionally ignore external DTD.
 *
 * Revision 1.5  2002/05/29 21:37:47  knoaman
 * Add baseURI to resolveEntity to support DOMInputSource.
 *
 * Revision 1.4  2002/05/28 20:44:14  tng
 * [Bug 9104] prefixes dissapearing when schema validation turned on.
 *
 * Revision 1.3  2002/05/27 18:39:21  tng
 * To get ready for 64 bit large file, use XMLSSize_t to represent line and column number.
 *
 * Revision 1.2  2002/05/22 20:53:41  knoaman
 * Prepare for DOM L3 :
 * - Make use of the XMLEntityHandler/XMLErrorReporter interfaces, instead of using
 * EntityHandler/ErrorHandler directly.
 * - Add 'AbstractDOMParser' class to provide common functionality for XercesDOMParser
 * and DOMBuilder.
 *
 * Revision 1.1.1.1  2002/02/01 22:22:07  peiyongz
 * sane_include
 *
 * Revision 1.23  2001/11/20 18:51:44  tng
 * Schema: schemaLocation and noNamespaceSchemaLocation to be specified outside the instance document.  New methods setExternalSchemaLocation and setExternalNoNamespaceSchemaLocation are added (for SAX2, two new properties are added).
 *
 * Revision 1.22  2001/10/25 19:46:15  tng
 * Comment outside root element should also be reported.
 *
 * Revision 1.21  2001/08/01 19:11:02  tng
 * Add full schema constraint checking flag to the samples and the parser.
 *
 * Revision 1.20  2001/06/03 19:26:20  jberry
 * Add support for querying error count following parse; enables simple parse without requiring error handler.
 *
 * Revision 1.19  2001/05/11 13:26:22  tng
 * Copyright update.
 *
 * Revision 1.18  2001/05/03 19:09:23  knoaman
 * Support Warning/Error/FatalError messaging.
 * Validity constraints errors are treated as errors, with the ability by user to set
 * validity constraints as fatal errors.
 *
 * Revision 1.17  2001/03/30 16:46:57  tng
 * Schema: Use setDoSchema instead of setSchemaValidation which makes more sense.
 *
 * Revision 1.16  2001/03/21 21:56:08  tng
 * Schema: Add Schema Grammar, Schema Validator, and split the DTDValidator into DTDValidator, DTDScanner, and DTDGrammar.
 *
 * Revision 1.15  2001/02/15 15:56:29  tng
 * Schema: Add setSchemaValidation and getSchemaValidation for DOMParser and SAXParser.
 * Add feature "http://apache.org/xml/features/validation/schema" for SAX2XMLReader.
 * New data field  fSchemaValidation in XMLScanner as the flag.
 *
 * Revision 1.14  2000/09/05 23:38:26  andyh
 * Added advanced callback support for XMLDecl()
 *
 * Revision 1.13  2000/06/19 18:12:56  rahulj
 * Suppress the comments, characters, ignoreableWhitespaces before
 * root element. Only allow the PI's to get through. Still need to come
 * to a consensus on this.
 *
 * Revision 1.12  2000/06/17 02:00:55  rahulj
 * Also pass any PI's, comment's, character's occuring before root
 * element to the registered document Handler. Defect identified
 * by John Smirl and Rich Taylor.
 *
 * Revision 1.11  2000/05/15 22:31:18  andyh
 * Replace #include<memory.h> with <string.h> everywhere.
 *
 * Revision 1.10  2000/04/12 22:58:30  roddey
 * Added support for 'auto validate' mode.
 *
 * Revision 1.9  2000/04/11 19:17:58  roddey
 * If a SAX error handler is installed, then the resetErrors() event handler
 * should call the one on the installed SAX error handler.
 *
 * Revision 1.8  2000/04/05 18:56:17  roddey
 * Init the fDTDHandler member. Enable installation of DTDHandler
 * on SAX parser.
 *
 * Revision 1.7  2000/03/03 01:29:34  roddey
 * Added a scanReset()/parseReset() method to the scanner and
 * parsers, to allow for reset after early exit from a progressive parse.
 * Added calls to new Terminate() call to all of the samples. Improved
 * documentation in SAX and DOM parsers.
 *
 * Revision 1.6  2000/03/02 19:54:33  roddey
 * This checkin includes many changes done while waiting for the
 * 1.1.0 code to be finished. I can't list them all here, but a list is
 * available elsewhere.
 *
 * Revision 1.5  2000/02/17 03:54:26  rahulj
 * Added some new getters to query the parser state and
 * clarified the documentation.
 *
 * Revision 1.4  2000/02/06 07:47:56  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.3  2000/01/12 00:15:22  roddey
 * Changes to deal with multiply nested, relative pathed, entities and to deal
 * with the new URL class changes.
 *
 * Revision 1.2  1999/12/15 19:57:48  roddey
 * Got rid of redundant 'const' on boolean return value. Some compilers choke
 * on this and its useless.
 *
 * Revision 1.1.1.1  1999/11/09 01:07:50  twl
 * Initial checkin
 *
 * Revision 1.6  1999/11/08 20:44:53  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/parsers/SAXParser.hpp>
#include <xercesc/internal/XMLScannerResolver.hpp>
#include <xercesc/framework/XMLValidator.hpp>
#include <xercesc/util/IOException.hpp>
#include <xercesc/sax/DocumentHandler.hpp>
#include <xercesc/sax/DTDHandler.hpp>
#include <xercesc/sax/ErrorHandler.hpp>
#include <xercesc/sax/EntityResolver.hpp>
#include <xercesc/sax/SAXParseException.hpp>
#include <xercesc/validators/common/GrammarResolver.hpp>
#include <xercesc/framework/XMLGrammarPool.hpp>
#include <xercesc/framework/XMLSchemaDescription.hpp>
#include <xercesc/util/Janitor.hpp>
#include <xercesc/util/OutOfMemoryException.hpp>
#include <xercesc/util/XMLEntityResolver.hpp>
#include <string.h>

XERCES_CPP_NAMESPACE_BEGIN


// ---------------------------------------------------------------------------
//  SAXParser: Constructors and Destructor
// ---------------------------------------------------------------------------
SAXParser::SAXParser( XMLValidator* const   valToAdopt
                    , MemoryManager* const  manager
                    , XMLGrammarPool* const gramPool):

    fParseInProgress(false)
    , fElemDepth(0)
    , fAdvDHCount(0)
    , fAdvDHListSize(32)
    , fDocHandler(0)
    , fDTDHandler(0)
    , fEntityResolver(0)
    , fXMLEntityResolver(0)
    , fErrorHandler(0)
    , fPSVIHandler(0)
    , fAdvDHList(0)
    , fScanner(0)
    , fGrammarResolver(0)
    , fURIStringPool(0)
    , fValidator(valToAdopt)
    , fMemoryManager(manager)
    , fGrammarPool(gramPool)
    , fElemQNameBuf(1023, manager)
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


SAXParser::~SAXParser()
{
    cleanUp();
}

// ---------------------------------------------------------------------------
//  SAXParser: Initialize/CleanUp methods
// ---------------------------------------------------------------------------
void SAXParser::initialize()
{
    // Create grammar resolver and string pool to pass to scanner
    fGrammarResolver = new (fMemoryManager) GrammarResolver(fGrammarPool, fMemoryManager);
    fURIStringPool = fGrammarResolver->getStringPool();

    // Create our scanner and tell it what validator to use
    fScanner = XMLScannerResolver::getDefaultScanner(fValidator, fGrammarResolver, fMemoryManager);
    fScanner->setURIStringPool(fURIStringPool);

    // Create the initial advanced handler list array and zero it out
    fAdvDHList = (XMLDocumentHandler**) fMemoryManager->allocate
    (
        fAdvDHListSize * sizeof(XMLDocumentHandler*)
    );//new XMLDocumentHandler*[fAdvDHListSize];
    memset(fAdvDHList, 0, sizeof(void*) * fAdvDHListSize);
}

void SAXParser::cleanUp()
{
    fMemoryManager->deallocate(fAdvDHList);//delete [] fAdvDHList;
    delete fScanner;
    delete fGrammarResolver;
    // grammar pool must do this
    //delete fURIStringPool;

    if (fValidator)
        delete fValidator;
}


// ---------------------------------------------------------------------------
//  SAXParser: Advanced document handler list maintenance methods
// ---------------------------------------------------------------------------
void SAXParser::installAdvDocHandler(XMLDocumentHandler* const toInstall)
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


bool SAXParser::removeAdvDocHandler(XMLDocumentHandler* const toRemove)
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
//  SAXParser: Getter methods
// ---------------------------------------------------------------------------
const XMLValidator& SAXParser::getValidator() const
{
    return *fScanner->getValidator();
}


bool SAXParser::getDoNamespaces() const
{
    return fScanner->getDoNamespaces();
}

bool SAXParser::getGenerateSyntheticAnnotations() const
{
    return fScanner->getGenerateSyntheticAnnotations();
}

bool SAXParser::getValidateAnnotations() const
{
    return fScanner->getValidateAnnotations();
}

bool SAXParser::getExitOnFirstFatalError() const
{
    return fScanner->getExitOnFirstFatal();
}

bool SAXParser::getValidationConstraintFatal() const
{
    return fScanner->getValidationConstraintFatal();
}


SAXParser::ValSchemes SAXParser::getValidationScheme() const
{
    const XMLScanner::ValSchemes scheme = fScanner->getValidationScheme();

    if (scheme == XMLScanner::Val_Always)
        return Val_Always;
    else if (scheme == XMLScanner::Val_Never)
        return Val_Never;

    return Val_Auto;
}

bool SAXParser::getDoSchema() const
{
    return fScanner->getDoSchema();
}

bool SAXParser::getValidationSchemaFullChecking() const
{
    return fScanner->getValidationSchemaFullChecking();
}

bool SAXParser::getIdentityConstraintChecking() const
{
    return fScanner->getIdentityConstraintChecking();
}

int SAXParser::getErrorCount() const
{
    return fScanner->getErrorCount();
}

XMLCh* SAXParser::getExternalSchemaLocation() const
{
    return fScanner->getExternalSchemaLocation();
}

XMLCh* SAXParser::getExternalNoNamespaceSchemaLocation() const
{
    return fScanner->getExternalNoNamespaceSchemaLocation();
}

SecurityManager* SAXParser::getSecurityManager() const
{
    return fScanner->getSecurityManager();
}

bool SAXParser::getLoadExternalDTD() const
{
    return fScanner->getLoadExternalDTD();
}

bool SAXParser::isCachingGrammarFromParse() const
{
    return fScanner->isCachingGrammarFromParse();
}

bool SAXParser::isUsingCachedGrammarInParse() const
{
    return fScanner->isUsingCachedGrammarInParse();
}

bool SAXParser::getCalculateSrcOfs() const
{
    return fScanner->getCalculateSrcOfs();
}

bool SAXParser::getStandardUriConformant() const
{
    return fScanner->getStandardUriConformant();
}

Grammar* SAXParser::getGrammar(const XMLCh* const nameSpaceKey)
{
    return fGrammarResolver->getGrammar(nameSpaceKey);
}

Grammar* SAXParser::getRootGrammar()
{
    return fScanner->getRootGrammar();
}

const XMLCh* SAXParser::getURIText(unsigned int uriId) const
{
    return fScanner->getURIText(uriId);
}

unsigned int SAXParser::getSrcOffset() const
{
    return fScanner->getSrcOffset();
}

bool SAXParser::getIgnoreCachedDTD() const
{
    return fScanner->getIgnoreCachedDTD();
}

bool SAXParser::getIgnoreAnnotations() const
{
    return fScanner->getIgnoreAnnotations();
}

bool SAXParser::getDisableDefaultEntityResolution() const
{
    return fScanner->getDisableDefaultEntityResolution();
}

bool SAXParser::getSkipDTDValidation() const
{
    return fScanner->getSkipDTDValidation();
}

// ---------------------------------------------------------------------------
//  SAXParser: Setter methods
// ---------------------------------------------------------------------------
void SAXParser::setDoNamespaces(const bool newState)
{
    fScanner->setDoNamespaces(newState);
}

void SAXParser::setGenerateSyntheticAnnotations(const bool newState)
{
    fScanner->setGenerateSyntheticAnnotations(newState);
}

void SAXParser::setValidateAnnotations(const bool newState)
{
    fScanner->setValidateAnnotations(newState);
}

void SAXParser::setExitOnFirstFatalError(const bool newState)
{
    fScanner->setExitOnFirstFatal(newState);
}


void SAXParser::setValidationConstraintFatal(const bool newState)
{
    fScanner->setValidationConstraintFatal(newState);
}


void SAXParser::setValidationScheme(const ValSchemes newScheme)
{
    if (newScheme == Val_Never)
        fScanner->setValidationScheme(XMLScanner::Val_Never);
    else if (newScheme == Val_Always)
        fScanner->setValidationScheme(XMLScanner::Val_Always);
    else
        fScanner->setValidationScheme(XMLScanner::Val_Auto);
}

void SAXParser::setDoSchema(const bool newState)
{
    fScanner->setDoSchema(newState);
}

void SAXParser::setValidationSchemaFullChecking(const bool schemaFullChecking)
{
    fScanner->setValidationSchemaFullChecking(schemaFullChecking);
}

void SAXParser::setIdentityConstraintChecking(const bool identityConstraintChecking)
{
    fScanner->setIdentityConstraintChecking(identityConstraintChecking);
}

void SAXParser::setExternalSchemaLocation(const XMLCh* const schemaLocation)
{
    fScanner->setExternalSchemaLocation(schemaLocation);
}
void SAXParser::setExternalNoNamespaceSchemaLocation(const XMLCh* const noNamespaceSchemaLocation)
{
    fScanner->setExternalNoNamespaceSchemaLocation(noNamespaceSchemaLocation);
}

void SAXParser::setExternalSchemaLocation(const char* const schemaLocation)
{
    fScanner->setExternalSchemaLocation(schemaLocation);
}
void SAXParser::setExternalNoNamespaceSchemaLocation(const char* const noNamespaceSchemaLocation)
{
    fScanner->setExternalNoNamespaceSchemaLocation(noNamespaceSchemaLocation);
}

void SAXParser::setSecurityManager(SecurityManager* const securityManager)
{
    // since this could impact various components, don't permit it to change
    // during a parse
    if (fParseInProgress)
        ThrowXMLwithMemMgr(IOException, XMLExcepts::Gen_ParseInProgress, fMemoryManager);

    fScanner->setSecurityManager(securityManager);
}

void SAXParser::setLoadExternalDTD(const bool newState)
{
    fScanner->setLoadExternalDTD(newState);
}

void SAXParser::cacheGrammarFromParse(const bool newState)
{
    fScanner->cacheGrammarFromParse(newState);

    if (newState)
        fScanner->useCachedGrammarInParse(newState);
}

void SAXParser::useCachedGrammarInParse(const bool newState)
{
    if (newState || !fScanner->isCachingGrammarFromParse())
        fScanner->useCachedGrammarInParse(newState);
}

void SAXParser::setCalculateSrcOfs(const bool newState)
{
    fScanner->setCalculateSrcOfs(newState);
}

void SAXParser::setStandardUriConformant(const bool newState)
{
    fScanner->setStandardUriConformant(newState);
}

void SAXParser::useScanner(const XMLCh* const scannerName)
{
    XMLScanner* tempScanner = XMLScannerResolver::resolveScanner
    (
        scannerName
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

void SAXParser::setInputBufferSize(const size_t bufferSize)
{
    fScanner->setInputBufferSize(bufferSize);
}

void SAXParser::setIgnoreCachedDTD(const bool newValue)
{
    fScanner->setIgnoredCachedDTD(newValue);
}

void SAXParser::setIgnoreAnnotations(const bool newValue)
{
    fScanner->setIgnoreAnnotations(newValue);
}

void SAXParser::setDisableDefaultEntityResolution(const bool newValue)
{
    fScanner->setDisableDefaultEntityResolution(newValue);
}

void SAXParser::setSkipDTDValidation(const bool newValue)
{
    fScanner->setSkipDTDValidation(newValue);
}

// ---------------------------------------------------------------------------
//  SAXParser: Overrides of the SAX Parser interface
// ---------------------------------------------------------------------------
void SAXParser::parse(const InputSource& source)
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

void SAXParser::parse(const XMLCh* const systemId)
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

void SAXParser::parse(const char* const systemId)
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

void SAXParser::setDocumentHandler(DocumentHandler* const handler)
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


void SAXParser::setDTDHandler(DTDHandler* const handler)
{
    fDTDHandler = handler;
    if (fDTDHandler)
        fScanner->setDocTypeHandler(this);
    else
        fScanner->setDocTypeHandler(0);
}


void SAXParser::setErrorHandler(ErrorHandler* const handler)
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


void SAXParser::setPSVIHandler(PSVIHandler* const handler)
{
    fPSVIHandler = handler;
    if (fPSVIHandler) {
        fScanner->setPSVIHandler(fPSVIHandler);      
    }
    else {
        fScanner->setPSVIHandler(0);       
    }
}

void SAXParser::setEntityResolver(EntityResolver* const resolver)
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

void SAXParser::setXMLEntityResolver(XMLEntityResolver* const resolver)
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

// ---------------------------------------------------------------------------
//  SAXParser: Progressive parse methods
// ---------------------------------------------------------------------------
bool SAXParser::parseFirst( const   XMLCh* const    systemId
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

bool SAXParser::parseFirst( const   char* const     systemId
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

bool SAXParser::parseFirst( const   InputSource&    source
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

bool SAXParser::parseNext(XMLPScanToken& token)
{
    return fScanner->scanNext(token);
}

void SAXParser::parseReset(XMLPScanToken& token)
{
    // Reset the scanner
    fScanner->scanReset(token);
}


// ---------------------------------------------------------------------------
//  SAXParser: Overrides of the XMLDocumentHandler interface
// ---------------------------------------------------------------------------
void SAXParser::docCharacters(  const   XMLCh* const    chars
                                , const unsigned int    length
                                , const bool            cdataSection)
{
    // Suppress the chars before the root element.
    if (fElemDepth)
    {
        // Just map to the SAX document handler
        if (fDocHandler)
            fDocHandler->characters(chars, length);
    }

    //
    //  If there are any installed advanced handlers, then lets call them
    //  with this info.
    //
    for (unsigned int index = 0; index < fAdvDHCount; index++)
        fAdvDHList[index]->docCharacters(chars, length, cdataSection);
}


void SAXParser::docComment(const XMLCh* const commentText)
{
    //
    //  SAX has no way to report this. But, if there are any installed
    //  advanced handlers, then lets call them with this info.
    //
    for (unsigned int index = 0; index < fAdvDHCount; index++)
        fAdvDHList[index]->docComment(commentText);
}


void SAXParser::XMLDecl( const  XMLCh* const    versionStr
                        , const XMLCh* const    encodingStr
                        , const XMLCh* const    standaloneStr
                        , const XMLCh* const    actualEncodingStr
                        )
{
    //
    //  SAX has no way to report this. But, if there are any installed
    //  advanced handlers, then lets call them with this info.
    //
    for (unsigned int index = 0; index < fAdvDHCount; index++)
        fAdvDHList[index]->XMLDecl( versionStr,
                                    encodingStr,
                                    standaloneStr,
                                    actualEncodingStr );
}


void SAXParser::docPI(  const   XMLCh* const    target
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


void SAXParser::endDocument()
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


void SAXParser::endElement( const   XMLElementDecl& elemDecl
                            , const unsigned int    uriId
                            , const bool            isRoot
                            , const XMLCh* const    elemPrefix)
{
    // Just map to the SAX document handler
    if (fDocHandler) {
        if (fScanner->getDoNamespaces()) {

            if (elemPrefix && *elemPrefix) {

                fElemQNameBuf.set(elemPrefix);
                fElemQNameBuf.append(chColon);
                fElemQNameBuf.append(elemDecl.getBaseName());
                fDocHandler->endElement(fElemQNameBuf.getRawBuffer());
            }
            else {
                fDocHandler->endElement(elemDecl.getBaseName());
            }
        }
        else
            fDocHandler->endElement(elemDecl.getFullName());

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


void SAXParser::endEntityReference(const XMLEntityDecl& entityDecl)
{
    //
    //  SAX has no way to report this event. But, if there are any installed
    //  advanced handlers, then lets call them with this info.
    //
    for (unsigned int index = 0; index < fAdvDHCount; index++)
        fAdvDHList[index]->endEntityReference(entityDecl);
}


void SAXParser::ignorableWhitespace(const   XMLCh* const    chars
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


void SAXParser::resetDocument()
{
    // Just map to the SAX document handler
    if (fDocHandler)
        fDocHandler->resetDocument();

    //
    //  If there are any installed advanced handlers, then lets call them
    //  with this info.
    //
    for (unsigned int index = 0; index < fAdvDHCount; index++)
        fAdvDHList[index]->resetDocument();

    // Make sure our element depth flag gets set back to zero
    fElemDepth = 0;
}


void SAXParser::startDocument()
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


void SAXParser::
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
        fAttrList.setVector(&attrList, attrCount);
        if (fScanner->getDoNamespaces()) {

            if (elemPrefix && *elemPrefix) {

                fElemQNameBuf.set(elemPrefix);
                fElemQNameBuf.append(chColon);
                fElemQNameBuf.append(elemDecl.getBaseName());
                fDocHandler->startElement(fElemQNameBuf.getRawBuffer(), fAttrList);

                // If its empty, send the end tag event now
                if (isEmpty)
                    fDocHandler->endElement(fElemQNameBuf.getRawBuffer());
            }
            else {

                fDocHandler->startElement(elemDecl.getBaseName(), fAttrList);

                // If its empty, send the end tag event now
                if (isEmpty)
                    fDocHandler->endElement(elemDecl.getBaseName());
            }
        }
        else {
            fDocHandler->startElement(elemDecl.getFullName(), fAttrList);

            // If its empty, send the end tag event now
            if (isEmpty)
                fDocHandler->endElement(elemDecl.getFullName());
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


void SAXParser::startEntityReference(const XMLEntityDecl& entityDecl)
{
    //
    //  SAX has no way to report this. But, If there are any installed
    //  advanced handlers, then lets call them with this info.
    //
    for (unsigned int index = 0; index < fAdvDHCount; index++)
        fAdvDHList[index]->startEntityReference(entityDecl);
}



// ---------------------------------------------------------------------------
//  SAXParser: Overrides of the DocTypeHandler interface
// ---------------------------------------------------------------------------
void SAXParser::attDef( const   DTDElementDecl&
                        , const DTDAttDef&
                        , const bool)
{
    // Unused by SAX DTDHandler interface at this time
}


void SAXParser::doctypeComment(const XMLCh* const)
{
    // Unused by SAX DTDHandler interface at this time
}


void SAXParser::doctypeDecl(const   DTDElementDecl&
                            , const XMLCh* const
                            , const XMLCh* const
                            , const bool
                            , const bool)
{
    // Unused by SAX DTDHandler interface at this time
}


void SAXParser::doctypePI(  const   XMLCh* const
                            , const XMLCh* const)
{
    // Unused by SAX DTDHandler interface at this time
}


void SAXParser::doctypeWhitespace(  const   XMLCh* const
                                    , const unsigned int)
{
    // Unused by SAX DTDHandler interface at this time
}


void SAXParser::elementDecl(const DTDElementDecl&, const bool)
{
    // Unused by SAX DTDHandler interface at this time
}


void SAXParser::endAttList(const DTDElementDecl&)
{
    // Unused by SAX DTDHandler interface at this time
}


void SAXParser::endIntSubset()
{
    // Unused by SAX DTDHandler interface at this time
}


void SAXParser::endExtSubset()
{
    // Unused by SAX DTDHandler interface at this time
}


void SAXParser::entityDecl( const   DTDEntityDecl&  entityDecl
                            , const bool
                            , const bool            isIgnored)
{
    //
    //  If we have a DTD handler, and this entity is not ignored, and
    //  its an unparsed entity, then send this one.
    //
    if (fDTDHandler && !isIgnored)
    {
        if (entityDecl.isUnparsed())
        {
            fDTDHandler->unparsedEntityDecl
            (
                entityDecl.getName()
                , entityDecl.getPublicId()
                , entityDecl.getSystemId()
                , entityDecl.getNotationName()
            );
        }
    }
}


void SAXParser::resetDocType()
{
    // Just map to the DTD handler
    if (fDTDHandler)
        fDTDHandler->resetDocType();
}


void SAXParser::notationDecl(   const   XMLNotationDecl&    notDecl
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


void SAXParser::startAttList(const DTDElementDecl&)
{
    // Unused by SAX DTDHandler interface at this time
}


void SAXParser::startIntSubset()
{
    // Unused by SAX DTDHandler interface at this time
}


void SAXParser::startExtSubset()
{
    // Unused by SAX DTDHandler interface at this time
}


void SAXParser::TextDecl(   const  XMLCh* const
                            , const XMLCh* const)
{
    // Unused by SAX DTDHandler interface at this time
}


// ---------------------------------------------------------------------------
//  SAXParser: Overrides of the XMLErrorReporter interface
// ---------------------------------------------------------------------------
void SAXParser::resetErrors()
{
    if (fErrorHandler)
        fErrorHandler->resetErrors();
}


void SAXParser::error(  const   unsigned int
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
//  SAXParser: Handlers for the XMLEntityHandler interface
// ---------------------------------------------------------------------------
void SAXParser::endInputSource(const InputSource&)
{
}

bool SAXParser::expandSystemId(const XMLCh* const, XMLBuffer&)
{
    return false;
}


void SAXParser::resetEntities()
{
    // Nothing to do for this one
}


InputSource*
SAXParser::resolveEntity(   const   XMLCh* const    publicId
                            , const XMLCh* const    systemId
                            , const XMLCh* const)
{
    // Just map to the SAX entity resolver handler
    if (fEntityResolver)
        return fEntityResolver->resolveEntity(publicId, systemId);
    return 0;
}


InputSource*
SAXParser::resolveEntity(  XMLResourceIdentifier* resourceIdentifier )
{
    // Just map to the SAX entity resolver handler
    if (fEntityResolver)
        return fEntityResolver->resolveEntity(resourceIdentifier->getPublicId(), 
                                                resourceIdentifier->getSystemId());
    if (fXMLEntityResolver)
        return fXMLEntityResolver->resolveEntity(resourceIdentifier);
    return 0;
}


void SAXParser::startInputSource(const InputSource&)
{
    // Nothing to do for this one
}



// ---------------------------------------------------------------------------
//  SAXParser: Deprecated methods
// ---------------------------------------------------------------------------
bool SAXParser::getDoValidation() const
{
    //
    //  We don't want to tie the public parser classes to the enum used
    //  by the scanner, so we use a separate one and map.
    //
    //  DON'T mix the new and old methods!!
    //
    const XMLScanner::ValSchemes scheme = fScanner->getValidationScheme();
    if (scheme == XMLScanner::Val_Always)
        return true;
    return false;
}

void SAXParser::setDoValidation(const bool newState)
{
    fScanner->setDoValidation
    (
        newState ? XMLScanner::Val_Always : XMLScanner::Val_Never
    );
}


// ---------------------------------------------------------------------------
//  SAXParser: Grammar preparsing methods
// ---------------------------------------------------------------------------
Grammar* SAXParser::loadGrammar(const char* const systemId,
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

Grammar* SAXParser::loadGrammar(const XMLCh* const systemId,
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

Grammar* SAXParser::loadGrammar(const InputSource& source,
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

void SAXParser::resetCachedGrammarPool()
{
    fGrammarResolver->resetCachedGrammar();
}

XERCES_CPP_NAMESPACE_END

