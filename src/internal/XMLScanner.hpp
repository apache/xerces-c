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
 * $Log$
 * Revision 1.30  2001/08/02 16:54:39  tng
 * Reset some Scanner flags in scanReset().
 *
 * Revision 1.29  2001/08/01 19:11:01  tng
 * Add full schema constraint checking flag to the samples and the parser.
 *
 * Revision 1.28  2001/07/24 21:23:39  tng
 * Schema: Use DatatypeValidator for ID/IDREF/ENTITY/ENTITIES/NOTATION.
 *
 * Revision 1.27  2001/07/13 16:56:48  tng
 * ScanId fix.
 *
 * Revision 1.26  2001/07/12 18:50:17  tng
 * Some performance modification regarding standalone check and xml decl check.
 *
 * Revision 1.25  2001/07/10 21:09:31  tng
 * Give proper error messsage when scanning external id.
 *
 * Revision 1.24  2001/07/09 13:42:08  tng
 * Partial Markup in Parameter Entity is validity constraint and thus should be just error, not fatal error.
 *
 * Revision 1.23  2001/07/05 13:12:11  tng
 * Standalone checking is validity constraint and thus should be just error, not fatal error:
 *
 * Revision 1.22  2001/06/22 12:42:33  tng
 * [Bug 2257] 1.5 thinks a <?xml-stylesheet ...> tag is a <?xml ...> tag
 *
 * Revision 1.21  2001/06/04 20:59:29  jberry
 * Add method incrementErrorCount for use by validator. Make sure to reset error count in _both_ the scanReset methods.
 *
 * Revision 1.20  2001/06/03 19:21:40  jberry
 * Add support for tracking error count during parse; enables simple parse without requiring error handler.
 *
 * Revision 1.19  2001/05/28 20:55:02  tng
 * Schema: allocate a fDTDValidator, fSchemaValidator explicitly to avoid wrong cast
 *
 * Revision 1.18  2001/05/11 15:17:28  tng
 * Schema: Nillable fixes.
 *
 * Revision 1.17  2001/05/11 13:26:17  tng
 * Copyright update.
 *
 * Revision 1.16  2001/05/03 20:34:29  tng
 * Schema: SchemaValidator update
 *
 * Revision 1.15  2001/05/03 19:09:09  knoaman
 * Support Warning/Error/FatalError messaging.
 * Validity constraints errors are treated as errors, with the ability by user to set
 * validity constraints as fatal errors.
 *
 * Revision 1.14  2001/04/19 18:16:59  tng
 * Schema: SchemaValidator update, and use QName in Content Model
 *
 * Revision 1.13  2001/03/30 16:46:56  tng
 * Schema: Use setDoSchema instead of setSchemaValidation which makes more sense.
 *
 * Revision 1.12  2001/03/30 16:35:06  tng
 * Schema: Whitespace normalization.
 *
 * Revision 1.11  2001/03/21 21:56:05  tng
 * Schema: Add Schema Grammar, Schema Validator, and split the DTDValidator into DTDValidator, DTDScanner, and DTDGrammar.
 *
 * Revision 1.10  2001/02/15 15:56:27  tng
 * Schema: Add setSchemaValidation and getSchemaValidation for DOMParser and SAXParser.
 * Add feature "http://apache.org/xml/features/validation/schema" for SAX2XMLReader.
 * New data field  fSchemaValidation in XMLScanner as the flag.
 *
 * Revision 1.9  2000/04/12 22:58:28  roddey
 * Added support for 'auto validate' mode.
 *
 * Revision 1.8  2000/03/03 01:29:32  roddey
 * Added a scanReset()/parseReset() method to the scanner and
 * parsers, to allow for reset after early exit from a progressive parse.
 * Added calls to new Terminate() call to all of the samples. Improved
 * documentation in SAX and DOM parsers.
 *
 * Revision 1.7  2000/03/02 19:54:30  roddey
 * This checkin includes many changes done while waiting for the
 * 1.1.0 code to be finished. I can't list them all here, but a list is
 * available elsewhere.
 *
 * Revision 1.6  2000/02/24 20:18:07  abagchi
 * Swat for removing Log from API docs
 *
 * Revision 1.5  2000/02/06 07:47:54  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.4  2000/01/24 20:40:43  roddey
 * Exposed the APIs to get to the byte offset in the source XML buffer. This stuff
 * is not tested yet, but I wanted to get the API changes in now so that the API
 * can be stablized.
 *
 * Revision 1.3  2000/01/12 23:52:46  roddey
 * These are trivial changes required to get the C++ and Java versions
 * of error messages more into sync. Mostly it was where the Java version
 * was passing out one or more parameter than the C++ version was. In
 * some cases the change just required an extra parameter to get the
 * needed info to the place where the error was issued.
 *
 * Revision 1.2  2000/01/12 00:15:04  roddey
 * Changes to deal with multiply nested, relative pathed, entities and to deal
 * with the new URL class changes.
 *
 * Revision 1.1.1.1  1999/11/09 01:08:23  twl
 * Initial checkin
 *
 * Revision 1.4  1999/11/08 20:44:52  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */


#if !defined(XMLSCANNER_HPP)
#define XMLSCANNER_HPP

#include <util/KVStringPair.hpp>
#include <util/RefVectorOf.hpp>
#include <util/XMLString.hpp>
#include <framework/XMLAttr.hpp>
#include <framework/XMLBufferMgr.hpp>
#include <framework/XMLErrorCodes.hpp>
#include <framework/XMLRefInfo.hpp>
#include <framework/XMLPScanToken.hpp>
#include <internal/ElemStack.hpp>
#include <internal/ReaderMgr.hpp>
#include <validators/DTD/DTDEntityDecl.hpp>
#include <validators/DTD/DTDValidator.hpp>
#include <validators/schema/SchemaValidator.hpp>

class InputSource;
class XMLDocumentHandler;
class DocTypeHandler;
class XMLElementDecl;
class XMLEntityHandler;
class EntityResolver;
class XMLErrorReporter;
class ErrorHandler;
class XMLMsgLoader;
class XMLValidator;


//
//  This is the mondo scanner class, which does the vast majority of the
//  work of parsing. It handles reading in input and spitting out events
//  to installed handlers.
//
class XMLPARSER_EXPORT XMLScanner
{
public :
    // -----------------------------------------------------------------------
    //  Public class types
    //
    //  NOTE: These should really be private, but some of the compilers we
    //  have to deal with are too stupid to understand this.
    //
    //  DeclTypes
    //      Used by scanXMLDecl() to know what type of decl it should scan.
    //      Text decls have slightly different rules from XMLDecls.
    //
    //  EntityExpRes
    //      These are the values returned from the entity expansion method,
    //      to indicate how it went.
    //
    //  XMLTokens
    //      These represent the possible types of input we can get while
    //      scanning content.
    //
    //  ValScheme
    //      This indicates what the scanner should do in terms of validation.
    //      'Auto' means if there is any int/ext subset, then validate. Else,
    //      don't.
    // -----------------------------------------------------------------------
    enum DeclTypes
    {
        Decl_Text
        , Decl_XML
    };

    enum EntityExpRes
    {
        EntityExp_Pushed
        , EntityExp_Returned
        , EntityExp_Failed
    };

    enum XMLTokens
    {
        Token_CData
        , Token_CharData
        , Token_Comment
        , Token_EndTag
        , Token_EOF
        , Token_PI
        , Token_StartTag
        , Token_Unknown
    };

    enum ValSchemes
    {
        Val_Never
        , Val_Always
        , Val_Auto
    };


    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    XMLScanner
    (
        XMLValidator* const valToAdopt
    );
    XMLScanner
    (
        XMLDocumentHandler* const  docHandler
        , DocTypeHandler* const    docTypeHandler
        , XMLEntityHandler* const  entityHandler
        , XMLErrorReporter* const  errReporter
        , XMLValidator* const      valToAdopt
    );
    ~XMLScanner();


    // -----------------------------------------------------------------------
    //  Error emitter methods
    // -----------------------------------------------------------------------
    void emitError(const XMLErrs::Codes toEmit);
    void emitError
    (
        const   XMLErrs::Codes    toEmit
        , const XMLCh* const        text1
        , const XMLCh* const        text2 = 0
        , const XMLCh* const        text3 = 0
        , const XMLCh* const        text4 = 0
    );
    void emitError
    (
        const   XMLErrs::Codes    toEmit
        , const char* const         text1
        , const char* const         text2 = 0
        , const char* const         text3 = 0
        , const char* const         text4 = 0
    );


    // -----------------------------------------------------------------------
    //  Getter methods
    // -----------------------------------------------------------------------
    const XMLDocumentHandler* getDocHandler() const;
    XMLDocumentHandler* getDocHandler();
    const DocTypeHandler* getDocTypeHandler() const;
    DocTypeHandler* getDocTypeHandler();
    bool getDoNamespaces() const;
    ValSchemes getValidationScheme() const;
    bool getDoSchema() const;
    bool getValidationSchemaFullChecking() const;
    const XMLEntityHandler* getEntityHandler() const;
    XMLEntityHandler* getEntityHandler();
    const XMLErrorReporter* getErrorReporter() const;
    XMLErrorReporter* getErrorReporter();
    bool getExitOnFirstFatal() const;
    bool getValidationConstraintFatal() const;
    RefHashTableOf<XMLRefInfo>* getIDRefList();
    const RefHashTableOf<XMLRefInfo>* getIDRefList() const;
    bool getInException() const;
    bool getLastExtLocation
    (
                XMLCh* const    sysIdToFill
        , const unsigned int    maxSysIdChars
        ,       XMLCh* const    pubIdToFill
        , const unsigned int    maxPubIdChars
        ,       unsigned int&   lineToFill
        ,       unsigned int&   colToFill
    );
    const Locator* getLocator() const;
    unsigned int getSrcOffset() const;
    bool getStandalone() const;
    const XMLValidator* getValidator() const;
    XMLValidator* getValidator();
    int getErrorCount();
    const DTDEntityDecl* getEntityDecl
    (
        const   XMLCh* const    entName
    )   const;

    DTDEntityDecl* getEntityDecl
    (
        const   XMLCh* const    entName
    );
    NameIdPoolEnumerator<DTDEntityDecl> getEntityEnumerator() const;
    NameIdPool<DTDEntityDecl>* getEntityDeclPool();
    const NameIdPool<DTDEntityDecl>* getEntityDeclPool() const;
    const XMLStringPool* getURIStringPool() const;
    XMLStringPool* getURIStringPool();
    bool getHasNoDTD() const;

    // -----------------------------------------------------------------------
    //  Getter methods
    // -----------------------------------------------------------------------

    /**
      * When an attribute name has no prefix, unlike elements, it is not mapped
      * to the global namespace. So, in order to have something to map it to
      * for practical purposes, a id for an empty URL is created and used for
      * such names.
      *
      * @return The URL pool id of the URL for an empty URL "".
      */
    unsigned int getEmptyNamespaceId() const;

    /**
      * When a prefix is found that has not been mapped, an error is issued.
      * However, if the parser has been instructed not to stop on the first
      * fatal error, it needs to be able to continue. To do so, it will map
      * that prefix tot his magic unknown namespace id.
      *
      * @return The URL pool id of the URL for the unknown prefix
      *         namespace.
      */
    unsigned int getUnknownNamespaceId() const;

    /**
      * The prefix 'xml' is a magic prefix, defined by the XML spec and
      * requiring no prior definition. This method returns the id for the
      * intrinsically defined URL for this prefix.
      *
      * @return The URL pool id of the URL for the 'xml' prefix.
      */
    unsigned int getXMLNamespaceId() const;

    /**
      * The prefix 'xmlns' is a magic prefix, defined by the namespace spec
      * and requiring no prior definition. This method returns the id for the
      * intrinsically defined URL for this prefix.
      *
      * @return The URL pool id of the URL for the 'xmlns' prefix.
      */
    unsigned int getXMLNSNamespaceId() const;

    /**
      * This method find the passed URI id in its URI pool and
      * copy the text of that URI into the passed buffer.
      */
    bool getURIText
    (
        const   unsigned int    uriId
        ,       XMLBuffer&      uriBufToFill
    )   const;

    const XMLCh* getURIText(const   unsigned int    uriId) const;

    /**
      * This method separate the passed QName into prefix
      * and local part, and then return the URI id by resolving
      * the prefix.
      *
      * mode: Indicate if this QName comes from an Element or Attribute
      */
    unsigned int resolveQName
    (
        const   XMLCh* const        qName
        ,       XMLBuffer&          nameBufToFill
        ,       XMLBuffer&          prefixBufToFill
        , const ElemStack::MapModes mode
    );

    /* tell if the validator comes from user */
    bool isValidatorFromUser();


    // -----------------------------------------------------------------------
    //  Setter methods
    // -----------------------------------------------------------------------
    void setDocHandler(XMLDocumentHandler* const docHandler);
    void setDocTypeHandler(DocTypeHandler* const docTypeHandler);
    void setDoNamespaces(const bool doNamespaces);
    void setEntityHandler(XMLEntityHandler* const docTypeHandler);
    void setEntityResolver(EntityResolver* const handler);
    void setErrorReporter(XMLErrorReporter* const errHandler);
    void setErrorHandler(ErrorHandler* const handler);
    void setExitOnFirstFatal(const bool newValue);
    void setValidationConstraintFatal(const bool newValue);
    void setValidationScheme(const ValSchemes newScheme);
    void setValidator(XMLValidator* const valToAdopt);
    void setDoSchema(const bool doSchema);
    void setValidationSchemaFullChecking(const bool schemaFullChecking);
    void setHasNoDTD(const bool hasNoDTD);

    // -----------------------------------------------------------------------
    //  Mutator methods
    // -----------------------------------------------------------------------
    void incrementErrorCount(void);			// For use by XMLValidator

    // -----------------------------------------------------------------------
    //  Deprecated methods as of 3.2.0. Use getValidationScheme() and
    //  setValidationScheme() instead.
    // -----------------------------------------------------------------------
    bool getDoValidation() const;
    void setDoValidation(const bool validate, const bool setValScheme = true);


    // -----------------------------------------------------------------------
    //  Document scanning methods
    //
    //  scanDocument() does the entire source document. scanFirst(),
    //  scanNext(), and scanReset() support a progressive parse.
    // -----------------------------------------------------------------------
    void scanDocument
    (
        const   InputSource&    src
        , const bool            reuseGrammar = false
    );
    void scanDocument
    (
        const   XMLCh* const    systemId
        , const bool            reuseGrammar = false
    );
    void scanDocument
    (
        const   char* const     systemId
        , const bool            reuseGrammar = false
    );

    bool scanFirst
    (
        const   InputSource&    src
        ,       XMLPScanToken&  toFill
        , const bool            reuseGrammar = false
    );
    bool scanFirst
    (
        const   XMLCh* const    systemId
        ,       XMLPScanToken&  toFill
        , const bool            reuseGrammar = false
    );
    bool scanFirst
    (
        const   char* const     systemId
        ,       XMLPScanToken&  toFill
        , const bool            reuseGrammar = false
    );

    bool scanNext(XMLPScanToken& toFill);

    void scanReset(XMLPScanToken& toFill);

    bool checkXMLDecl(bool startWithAngle);

private :
    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    XMLScanner();
    XMLScanner(const XMLScanner&);
    void operator=(const XMLScanner&);


    // -----------------------------------------------------------------------
    //  Private helper methods
    // -----------------------------------------------------------------------
    void commonInit();
    void initValidator(XMLValidator* theValidator);
    void resetEntityDeclPool();
    void resetURIStringPool();


    // -----------------------------------------------------------------------
    //  Private helper methods
    //
    //  These are implemented in XMLScanner2.cpp, to keep the main file from
    //  becoming so bloated. We can't have any bloated files.
    // -----------------------------------------------------------------------
    unsigned int buildAttList
    (
        const   RefVectorOf<KVStringPair>&  providedAttrs
        , const unsigned int                attCount
        ,       XMLElementDecl&             elemDecl
        ,       RefVectorOf<XMLAttr>&       toFill
    );
    void checkIDRefs();
    bool isLegalToken(const XMLPScanToken& toCheck);
    bool normalizeAttValue
    (
        const   XMLAttDef* const    attDef
        , const XMLCh* const        value
        ,       XMLBuffer&          toFill
    );
    bool normalizeAttRawValue
    (
        const   XMLCh* const        attrName
        , const XMLCh* const        value
        ,       XMLBuffer&          toFill
    );

    unsigned int resolvePrefix
    (
        const   XMLCh* const        prefix
        , const ElemStack::MapModes mode
    );
    unsigned int resolvePrefix
    (
        const   XMLCh* const        prefix
        ,       XMLBuffer&          uriBufToFill
        , const ElemStack::MapModes mode
    );
    void scanReset(const InputSource& src);
    void sendCharData(XMLBuffer& toSend);
    XMLTokens senseNextToken(unsigned int& orgReader);
    void updateNSMap
    (
        const   XMLCh* const    attrName
        , const XMLCh* const    attrValue
    );
    void scanRawAttrListforNameSpaces(const RefVectorOf<KVStringPair>* theRawAttrList, int attCount);
    void parseSchemaLocation(const XMLCh* const schemaLocationStr);
    void resolveSchemaGrammar(const XMLCh* const loc, const XMLCh* const uri);
    bool switchGrammar(int newGrammarNameSpaceIndex);
    bool switchGrammar(const XMLCh* const newGrammarNameSpace);
    bool laxElementValidation(QName* element, ContentLeafNameTypeVector* cv);

    // -----------------------------------------------------------------------
    //  Private scanning methods
    // -----------------------------------------------------------------------
    bool basicAttrValueScan
    (
        const   XMLCh* const    attrName
        ,       XMLBuffer&      toFill
    );
    bool getQuotedString(XMLBuffer& toFill);
    unsigned int rawAttrScan
    (
        const   XMLCh* const                elemName
        ,       RefVectorOf<KVStringPair>&  toFill
        ,       bool&                       isEmpty
    );
    bool scanAttValue
    (
        const   XMLAttDef* const    attDef
        ,       XMLBuffer&          toFill
    );
    void scanCDSection();
    void scanCharData(XMLBuffer& toToUse);
    bool scanCharRef(XMLCh& toFill, XMLCh& second);
    void scanComment();
    bool scanContent(const bool extEntity);
    void scanEndTag(bool& gotData);
    EntityExpRes scanEntityRef
    (
        const   bool    inAttVal
        ,       XMLCh&  firstCh
        ,       XMLCh&  secondCh
        ,       bool&   escaped
    );
    bool scanEq();
    void scanMiscellaneous();
    void scanPI();
    void scanProlog();
    bool scanStartTag(bool& gotData);
    bool scanStartTagNS(bool& gotData);
    void scanXMLDecl(const DeclTypes type);
    unsigned int scanUpToWSOr
    (
                XMLBuffer&  toFill
        , const XMLCh       chEndChar
    );



    // -----------------------------------------------------------------------
    //  Data members
    //
    //  fAttrList
    //      Every time we get a new element start tag, we have to pass to
    //      the document handler the attributes found. To make it more
    //      efficient we keep this ref vector of XMLAttr objects around. We
    //      just reuse it over and over, allowing it to grow to meet the
    //      peek need.
    //
    //  fBufMgr
    //      This is a manager for temporary buffers used during scanning.
    //      For efficiency we must use a set of static buffers, but we have
    //      to insure that they are not incorrectly reused. So this manager
    //      provides the smarts to hand out buffers as required.
    //
    //  fDocHandler
    //      The client code's document handler. If zero, then no document
    //      handler callouts are done. We don't adopt it.
    //
    //  fDocTypeHandler
    //      The client code's document type handler (used by DTD Validator).
    //
    //  fDoNamespaces
    //      This flag indicates whether the client code wants us to do
    //      namespaces or not. If the installed validator indicates that it
    //      has to do namespaces, then this is ignored.
    //
    //  fElemStack
    //      This is the element stack that is used to track the elements that
    //      are currently being worked on.
    //
    //  fEntityHandler
    //      The client code's entity handler. If zero, then no entity handler
    //      callouts are done. We don't adopt it.
    //
    //  fEntityResolver
    //      The client code's entity resolver.  Need to store this info for
    //      Schema parse entity resolving.
    //
    //  fErrorReporter
    //      The client code's error reporter. If zero, then no error reporter
    //      callouts are done. We don't adopt it.
    //
    //  fErrorHandler
    //      The client code's error handler.  Need to store this info for
    //      Schema parse error handling.
    //
    //  fExitOnFirstFatal
    //      This indicates whether we bail out on the first fatal XML error
    //      or not. It defaults to true, which is the strict XML way, but it
    //      can be changed.
    //
    //  fValidationConstraintFatal
    //      This indicates whether we treat validation constraint errors as
    //      fatal errors or not. It defaults to false, but it can be changed.
    //
    //  fIDRefList
    //      This is a list of XMLRefInfo objects. This member lets us do all
    //      needed ID-IDREF balancing checks.
    //
    //  fInException
    //      To avoid a circular freakout when we catch an exception and emit
    //      it, which would normally throw again if the 'fail on first error'
    //      flag is one.
    //
    //  fRawAttrList
    //      During the initial scan of the attributes we can only do a raw
    //      scan for key/value pairs. So this vector is used to store them
    //      until they can be processed (and put into fAttrList.)
    //
    //  fReaderMgr
    //      This is the reader manager, from which we get characters. It
    //      manages the reader stack for us, and provides a lot of convenience
    //      methods to do specialized checking for chars, sequences of chars,
    //      skipping chars, etc...
    //
    //  fReuseGrammar
    //      This flag is set on a per-scan basis. So its provided in the
    //      scanDocument() and scanFirst() methods, and applies for that
    //      one pass. It indicates if the Grammar should be reused or not.
    //      If so, then all the Grammar will be ignored.
    //      There cannot be any internal subset.
    //
    //  fScannerId
    //  fSequenceId
    //      These are used for progressive parsing, to make sure that the
    //      client code does the right thing at the right time.
    //
    //  fStandalone
    //      Indicates whether the document is standalone or not. Defaults to
    //      no, but can be overridden in the XMLDecl.
    //
    //  fHasNoDTD
    //      Indicates the document has no DTD or has only an internal DTD subset
    //      which contains no parameter entity references.
    //
    //  fValidate
    //      Indicates whether any validation should be done. This is defined
    //      by the existence of a Grammar together with fValScheme.
    //
    //  fValidator
    //      The installed validator. We look at them via the abstract
    //      validator interface, and don't know what it actual is.
    //      Either point to user's installed validator, or fDTDValidator
    //      or fSchemaValidator.
    //
    //  fDTDValidator
    //      The DTD validator instance.
    //
    //  fSchemaValidator
    //      The Schema validator instance.
    //
    //  fValidatorFromUser
    //      This flag indicates whether the validator was installed from
    //      user.  If false, then the validator was created by the Scanner.
    //
    //  fValScheme
    //      This is the currently set validation scheme. It defaults to
    //      'never', but can be set by the client.
    //
    //  fErrorCount
    //		The number of errors we've encountered.
    //
    //  fDoSchema
    //      This flag indicates whether the client code wants Schema to
    //      be processed or not.
    //
    //  fSchemaFullChecking
    //      This flag indicates whether the client code wants full Schema
    //      constraint checking.
    //
    //  fSeeXsi
    //      This flag indicates a schema has been seen.
    //
    //  fAttName
    //  fAttValue
    //  fCDataBuf
    //  fNameBuf
    //  fQNameBuf
    //  fPrefixBuf
    //      For the most part, buffers are obtained from the fBufMgr object
    //      on the fly. However, for the start tag scan, we have a set of
    //      fixed buffers for performance reasons. These are used a lot and
    //      there are a number of them, so asking the buffer manager each
    //      time for new buffers is a bit too much overhead.
    //
    //  fEmptyNamespaceId
    //      This is the id of the empty namespace URI. This is a special one
    //      because of the xmlns="" type of deal. We have to quickly sense
    //      that its the empty namespace.
    //
    //  fUnknownNamespaceId
    //      This is the id of the namespace URI which is assigned to the
    //      global namespace. Its for debug purposes only, since there is no
    //      real global namespace URI. Its set by the derived class.
    //
    //  fXMLNamespaceId
    //  fXMLNSNamespaceId
    //      These are the ids of the namespace URIs which are assigned to the
    //      'xml' and 'xmlns' special prefixes. The former is officially
    //      defined but the latter is not, so we just provide one for debug
    //      purposes.
    //
    //  fSchemaNamespaceId
    //      This is the id of the schema namespace URI.
    //
    //  fGrammarResolver
    //      Grammar Pool that stores all the Grammar
    //
    //  fGrammar
    //      Current Grammar used by the Scanner and Validator
    //
    //  fEntityDeclPool
    //      This is a pool of EntityDecl objects, which contains all of the
    //      general entities that are declared in the DTD subsets, plus the
    //      default entities (such as &gt; &lt; ...) defined by the XML Standard.
    //
    //  fURIStringPool
    //      This is a pool for URIs with unique ids assigned. We use a standard
    //      string pool class.  This pool is going to be shared by all Grammar.
    //      Use only if namespace is turned on.
    //
    // -----------------------------------------------------------------------
    RefVectorOf<XMLAttr>*       fAttrList;
    XMLBufferMgr                fBufMgr;
    XMLDocumentHandler*         fDocHandler;
    DocTypeHandler*             fDocTypeHandler;
    bool                        fDoNamespaces;
    ElemStack                   fElemStack;
    XMLEntityHandler*           fEntityHandler;
    EntityResolver*             fEntityResolver;
    XMLErrorReporter*           fErrorReporter;
    ErrorHandler*               fErrorHandler;
    bool                        fExitOnFirstFatal;
    bool                        fValidationConstraintFatal;
    RefHashTableOf<XMLRefInfo>* fIDRefList;
    bool                        fInException;
    RefVectorOf<KVStringPair>*  fRawAttrList;
    ReaderMgr                   fReaderMgr;
    bool                        fReuseGrammar;
    XMLUInt32                   fScannerId;
    XMLUInt32                   fSequenceId;
    bool                        fStandalone;
    bool                        fHasNoDTD;
    bool                        fValidate;
    XMLValidator*               fValidator;
    DTDValidator*               fDTDValidator;
    SchemaValidator*            fSchemaValidator;
    bool                        fValidatorFromUser;
    ValSchemes                  fValScheme;
    int                         fErrorCount;
    bool                        fDoSchema;
    bool                        fSchemaFullChecking;
    bool                        fSeeXsi;

    XMLBuffer                   fAttNameBuf;
    XMLBuffer                   fAttValueBuf;
    XMLBuffer                   fCDataBuf;
    XMLBuffer                   fNameBuf;
    XMLBuffer                   fQNameBuf;
    XMLBuffer                   fPrefixBuf;
    XMLBuffer                   fURIBuf;

    unsigned int                fEmptyNamespaceId;
    unsigned int                fUnknownNamespaceId;
    unsigned int                fXMLNamespaceId;
    unsigned int                fXMLNSNamespaceId;
    unsigned int                fSchemaNamespaceId;

    GrammarResolver*            fGrammarResolver;
    Grammar*                    fGrammar;
    NameIdPool<DTDEntityDecl>*  fEntityDeclPool;
    XMLStringPool*              fURIStringPool;
};



// ---------------------------------------------------------------------------
//  XMLScanner: Getter methods
// ---------------------------------------------------------------------------
inline const XMLDocumentHandler* XMLScanner::getDocHandler() const
{
    return fDocHandler;
}

inline XMLDocumentHandler* XMLScanner::getDocHandler()
{
    return fDocHandler;
}

inline const DocTypeHandler* XMLScanner::getDocTypeHandler() const
{
    return fDocTypeHandler;
}

inline DocTypeHandler* XMLScanner::getDocTypeHandler()
{
    return fDocTypeHandler;
}

inline bool XMLScanner::getDoNamespaces() const
{
    return fDoNamespaces;
}

inline const XMLEntityHandler* XMLScanner::getEntityHandler() const
{
    return fEntityHandler;
}

inline XMLEntityHandler* XMLScanner::getEntityHandler()
{
    return fEntityHandler;
}

inline const XMLErrorReporter* XMLScanner::getErrorReporter() const
{
    return fErrorReporter;
}

inline XMLErrorReporter* XMLScanner::getErrorReporter()
{
    return fErrorReporter;
}

inline bool XMLScanner::getExitOnFirstFatal() const
{
    return fExitOnFirstFatal;
}

inline bool XMLScanner::getValidationConstraintFatal() const
{
    return fValidationConstraintFatal;
}

inline RefHashTableOf<XMLRefInfo>* XMLScanner::getIDRefList()
{
    return fIDRefList;
}

inline bool XMLScanner::getInException() const
{
    return fInException;
}

inline const RefHashTableOf<XMLRefInfo>* XMLScanner::getIDRefList() const
{
    return fIDRefList;
}

inline const Locator* XMLScanner::getLocator() const
{
    return &fReaderMgr;
}

inline unsigned int XMLScanner::getSrcOffset() const
{
    return fReaderMgr.getSrcOffset();
}

inline bool XMLScanner::getStandalone() const
{
    return fStandalone;
}

inline XMLScanner::ValSchemes XMLScanner::getValidationScheme() const
{
    return fValScheme;
}

inline const XMLValidator* XMLScanner::getValidator() const
{
    return fValidator;
}

inline XMLValidator* XMLScanner::getValidator()
{
    return fValidator;
}

inline bool XMLScanner::getDoSchema() const
{
    return fDoSchema;
}

inline bool XMLScanner::getValidationSchemaFullChecking() const
{
    return fSchemaFullChecking;
}

inline int XMLScanner::getErrorCount()
{
    return fErrorCount;
}

inline bool XMLScanner::isValidatorFromUser()
{
    return fValidatorFromUser;
}

inline unsigned int XMLScanner::getEmptyNamespaceId() const
{
    return fEmptyNamespaceId;
}

inline unsigned int XMLScanner::getUnknownNamespaceId() const
{
    return fUnknownNamespaceId;
}

inline unsigned int XMLScanner::getXMLNamespaceId() const
{
    return fXMLNamespaceId;
}

inline unsigned int XMLScanner::getXMLNSNamespaceId() const
{
    return fXMLNSNamespaceId;
}

inline NameIdPoolEnumerator<DTDEntityDecl>
XMLScanner::getEntityEnumerator() const
{
    return NameIdPoolEnumerator<DTDEntityDecl>(fEntityDeclPool);
}

inline const DTDEntityDecl* XMLScanner::getEntityDecl(const  XMLCh* const    entName) const
{
    return fEntityDeclPool->getByKey(entName);
}

inline DTDEntityDecl* XMLScanner::getEntityDecl(const XMLCh* const entName)
{
    return fEntityDeclPool->getByKey(entName);
}

inline NameIdPool<DTDEntityDecl>* XMLScanner::getEntityDeclPool()
{
    return fEntityDeclPool;
}

inline const NameIdPool<DTDEntityDecl>* XMLScanner::getEntityDeclPool() const
{
    return fEntityDeclPool;
}

inline const XMLStringPool* XMLScanner::getURIStringPool() const
{
    return fURIStringPool;
}

inline XMLStringPool* XMLScanner::getURIStringPool()
{
    return fURIStringPool;
}

inline bool XMLScanner::getHasNoDTD() const
{
    return fHasNoDTD;
}

// ---------------------------------------------------------------------------
//  XMLScanner: Setter methods
// ---------------------------------------------------------------------------
inline void XMLScanner::setDocHandler(XMLDocumentHandler* const docHandler)
{
    fDocHandler = docHandler;
}

inline void XMLScanner::setDocTypeHandler(DocTypeHandler* const docTypeHandler)
{
    fDocTypeHandler = docTypeHandler;
}

inline void XMLScanner::setDoNamespaces(const bool doNamespaces)
{
    fDoNamespaces = doNamespaces;
    if (fDoNamespaces) {
        if (!fURIStringPool) {
            fURIStringPool = new XMLStringPool();
            resetURIStringPool();
        }
    }
}

inline void XMLScanner::setErrorReporter(XMLErrorReporter* const errHandler)
{
    fErrorReporter = errHandler;
    fDTDValidator->setErrorReporter(fErrorReporter);
    fSchemaValidator->setErrorReporter(fErrorReporter);
}

inline void XMLScanner::setErrorHandler(ErrorHandler* const handler)
{
    fErrorHandler = handler;
}

inline void XMLScanner::setEntityHandler(XMLEntityHandler* const entityHandler)
{
    fEntityHandler = entityHandler;
    fReaderMgr.setEntityHandler(entityHandler);
}

inline void XMLScanner::setEntityResolver(EntityResolver* const handler)
{
    fEntityResolver = handler;
}

inline void XMLScanner::setExitOnFirstFatal(const bool newValue)
{
    fExitOnFirstFatal = newValue;
}

inline void XMLScanner::setValidationConstraintFatal(const bool newValue)
{
    fValidationConstraintFatal = newValue;
}

inline void XMLScanner::setValidationScheme(const ValSchemes newScheme)
{
    fValScheme = newScheme;

    // validation flag for Val_Auto is set to false by default,
    //   and will be turned to true if a grammar is seen
    if (fValScheme == Val_Always)
        fValidate = true;
    else
        fValidate = false;
}

inline void XMLScanner::setValidator(XMLValidator* const valToAdopt)
{
    if (fValidatorFromUser)
        delete fValidator;
    fValidator = valToAdopt;
    fValidatorFromUser = true;
    initValidator(fValidator);
}

inline void XMLScanner::setDoSchema(const bool doSchema)
{
    fDoSchema = doSchema;
}

inline void XMLScanner::setValidationSchemaFullChecking(const bool schemaFullChecking)
{
    fSchemaFullChecking = schemaFullChecking;
}

inline void XMLScanner::setHasNoDTD(const bool hasNoDTD)
{
    fHasNoDTD = hasNoDTD;
}

// ---------------------------------------------------------------------------
//  XMLScanner: Mutator methods
// ---------------------------------------------------------------------------
inline void XMLScanner::incrementErrorCount()
{
    ++fErrorCount;
}


// ---------------------------------------------------------------------------
//  XMLScanner: Deprecated methods
// ---------------------------------------------------------------------------
inline bool XMLScanner::getDoValidation() const
{
    return fValidate;
}

inline void XMLScanner::setDoValidation(const bool validate, const bool setValScheme)
{
    fValidate = validate;
    if (setValScheme) {
        if (fValidate)
            fValScheme = Val_Always;
        else
            fValScheme = Val_Never;
    }
}

#endif
