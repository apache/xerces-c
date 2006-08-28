//
// These methods are defined in the API, but they are currently unimplemented
//   and throw exceptions if called 
//
%ignore createXMLReader(const XMLCh *);


// Operators we don't want to wrap
%ignore operator =;
%ignore operator new;
%ignore operator delete;
%ignore operator <<;


// both of these static variables cause trouble
// the transcoding service is only useful to C++ anyway.
%ignore XERCES_CPP_NAMESPACE::XMLPlatformUtils::fgTransService;
%ignore XERCES_CPP_NAMESPACE::XMLPlatformUtils::fgNetAccessor;

// these are other static variables that are useless to Perl
%ignore XERCES_CPP_NAMESPACE::XMLPlatformUtils::fgUserPanicHandler;
%ignore XERCES_CPP_NAMESPACE::XMLPlatformUtils::fgDefaultPanicHandler;
%ignore XERCES_CPP_NAMESPACE::XMLPlatformUtils::fgMemoryManager;
%ignore XERCES_CPP_NAMESPACE::XMLPlatformUtils::fgDefaulPanicHandler;
%ignore XERCES_CPP_NAMESPACE::XMLPlatformUtils::fgArrayMemoryManager;
%ignore XERCES_CPP_NAMESPACE::XMLPlatformUtils::fgAtomicMutex;

// these are methods that are useless in Perl
%ignore XERCES_CPP_NAMESPACE::XMLPlatformUtils::curFilePos;
%ignore XERCES_CPP_NAMESPACE::XMLPlatformUtils::closeFile;
%ignore XERCES_CPP_NAMESPACE::XMLPlatformUtils::fileSize;
%ignore XERCES_CPP_NAMESPACE::XMLPlatformUtils::openFile;
%ignore XERCES_CPP_NAMESPACE::XMLPlatformUtils::openFileToWrite;
%ignore XERCES_CPP_NAMESPACE::XMLPlatformUtils::openStdInHandle;
%ignore XERCES_CPP_NAMESPACE::XMLPlatformUtils::readFileBuffer;
%ignore XERCES_CPP_NAMESPACE::XMLPlatformUtils::writeBufferToFile;
%ignore XERCES_CPP_NAMESPACE::XMLPlatformUtils::resetFile;
%ignore XERCES_CPP_NAMESPACE::XMLPlatformUtils::getFullPath;
%ignore XERCES_CPP_NAMESPACE::XMLPlatformUtils::getCurrentDirectory;
%ignore XERCES_CPP_NAMESPACE::XMLPlatformUtils::isAnySlash;
%ignore XERCES_CPP_NAMESPACE::XMLPlatformUtils::removeDotSlash;
%ignore XERCES_CPP_NAMESPACE::XMLPlatformUtils::removeDotDotSlash;
%ignore XERCES_CPP_NAMESPACE::XMLPlatformUtils::isRelative;
%ignore XERCES_CPP_NAMESPACE::XMLPlatformUtils::weavePaths;
%ignore XERCES_CPP_NAMESPACE::XMLPlatformUtils::getCurrentMillis;
%ignore XERCES_CPP_NAMESPACE::XMLPlatformUtils::closeMutex;
%ignore XERCES_CPP_NAMESPACE::XMLPlatformUtils::lockMutex;
%ignore XERCES_CPP_NAMESPACE::XMLPlatformUtils::makeMutex;
%ignore XERCES_CPP_NAMESPACE::XMLPlatformUtils::unlockMutex;
%ignore XERCES_CPP_NAMESPACE::XMLPlatformUtils::loadMsgSet;
%ignore XERCES_CPP_NAMESPACE::XMLPlatformUtils::compareAndSwap;
%ignore XERCES_CPP_NAMESPACE::XMLPlatformUtils::atomicIncrement;
%ignore XERCES_CPP_NAMESPACE::XMLPlatformUtils::atomicDecrement;
%ignore XERCES_CPP_NAMESPACE::XMLPlatformUtils::recognizeNEL;
%ignore XERCES_CPP_NAMESPACE::XMLPlatformUtils::isNELRecognized;
%ignore XERCES_CPP_NAMESPACE::XMLPlatformUtils::strictIANAEncoding;
%ignore XERCES_CPP_NAMESPACE::XMLPlatformUtils::isStrictIANAEncoding;
%ignore XERCES_CPP_NAMESPACE::XMLPlatformUtils::alignPointerForNewBlockAllocation;

%ignore PerlErrorCallbackHandler::warning(const SAXParseException&);
%ignore PerlErrorCallbackHandler::error(const SAXParseException&);
%ignore PerlErrorCallbackHandler::fatalError(const SAXParseException&);

// ignore the char* versions of ALL constructors
%ignore XERCES_CPP_NAMESPACE::DOMLSParser::parseURI(const char *const );
%ignore MemBufInputSource(const XMLByte* const, const unsigned int, const char* const,
	const bool foo=false,
        MemoryManager* const foo=XMLPlatformUtils::fgMemoryManager);

%ignore XMLURL(const XMLURL&,const char* const,
        MemoryManager* const foo=XMLPlatformUtils::fgMemoryManager);
%ignore XMLURL(const char* const,
        MemoryManager* const foo=XMLPlatformUtils::fgMemoryManager);
%ignore XMLURL(const XMLCh* const, const char* const,
        MemoryManager* const foo=XMLPlatformUtils::fgMemoryManager);
%ignore URLInputSource(const XMLCh* const, const char* const,
        MemoryManager* const foo=XMLPlatformUtils::fgMemoryManager);
%ignore URLInputSource(const XMLCh* const, const char* const, const char* const,
        MemoryManager* const foo=XMLPlatformUtils::fgMemoryManager);
%ignore XMLFormatter(
        const   char* const             outEncoding
        , const char* const             docVersion
        ,       XMLFormatTarget* const  target
        , const EscapeFlags             escapeFlags = NoEscapes
        , const UnRepFlags              unrepFlags = UnRep_Fail
        ,       MemoryManager* const    manager = XMLPlatformUtils::fgMemoryManager);

%ignore XMLFormatter(
        const   char* const             outEncoding
        ,       XMLFormatTarget* const  target
        , const EscapeFlags             escapeFlags = NoEscapes
        , const UnRepFlags              unrepFlags = UnRep_Fail
        ,       MemoryManager* const    manager = XMLPlatformUtils::fgMemoryManager);

%ignore XERCES_CPP_NAMESPACE::AttributeList::getValue(const char* const) const;
%ignore setExternalSchemaLocation(const char* const);
%ignore setExternalNoNamespaceSchemaLocation(const char* const);

/* 
 * Ignore these char* methods for all Parsers
 */
%ignore loadGrammar(const char* const systemId,
                                 const short grammarType,
                                 const bool toCache = false);


%ignore parse(const char* const);
%ignore parseFirst(const char *const,XMLPScanToken&,const bool);

/*
 * We need these constructors to always adopt the input strings
 * unfortunately, the default is set to FALSE, so we must ignore
 * all constructors that use the default, and explicitly set it
 * in a typemap. While we are at it, we also ignore the constructor
 * that sets the MemoryManager
 */
%ignore MemBufInputSource(const XMLByte* const, const unsigned int, const XMLCh* const);
%ignore MemBufInputSource(const XMLByte* const, const unsigned int, const XMLCh* const, 
                          const bool, MemoryManager *const);
%ignore Wrapper4InputSource(InputSource* const);
%ignore Wrapper4InputSource(InputSource* const, const bool 
        , MemoryManager* const);
%ignore Wrapper4DOMLSInput(
        DOMLSInput* const
	, DOMLSResourceResolver*
        , const bool 
        , MemoryManager* const);
%ignore Wrapper4DOMLSInput(
        DOMLSInput* const
	, DOMLSResourceResolver*);

// These are odd cases. We want access to skip the method with the
// memory manager argument, but we want access to the optional argument
// *after* the memory manager. So we just ignore the method that doesn't 
// include the memory manager - since our typemap defaults that argument
// they are redundant
%ignore createLSParser(short const,const XMLCh* const);
%ignore SAXParser(XMLValidator*   const);

// FIXME SWIG-1.3.30 broke this
// %ignore createXMLReader();

/*
 * ignore the constructors which set the MemoryManager
 */
%ignore createDocument(const XMLCh *,const XMLCh *,DOMDocumentType *,MemoryManager *const);
%ignore createDocument(MemoryManager *const);
%ignore createLSSerializer(MemoryManager *const);
%ignore XMLFormatter(
        const   XMLCh* const           
        , const XMLCh* const           
        ,       XMLFormatTarget* const 
        , const EscapeFlags
        , const UnRepFlags 
        ,       MemoryManager* const);
%ignore XMLFormatter(
        const   XMLCh* const
        ,       XMLFormatTarget* const
        , const EscapeFlags
        , const UnRepFlags 
        ,       MemoryManager* const);
%ignore LocalFileFormatTarget(const XMLCh* const, MemoryManager* const);
%ignore LocalFileFormatTarget(const char* const, 
            MemoryManager* const  manager = XMLPlatformUtils::fgMemoryManager);
%ignore MemBufFormatTarget(int, MemoryManager* const);
%ignore XercesDOMParser(XMLValidator* const, MemoryManager* const);
%ignore URLInputSource(const XMLURL&, MemoryManager *const);
%ignore URLInputSource(const XMLCh* const, MemoryManager *const);
%ignore URLInputSource(const XMLCh* const, const XMLCh* const,MemoryManager *const);
%ignore URLInputSource(const XMLCh* const, const XMLCh* const, const XMLCh* const,MemoryManager *const);
%ignore StdInInputSource(MemoryManager* const);
%ignore LocalFileInputSource(const XMLCh* const,MemoryManager* const);
%ignore LocalFileInputSource(const XMLCh* const,const XMLCh* const,MemoryManager* const);
%ignore XMLUri(MemoryManager* const);
%ignore XMLUri(const XMLCh* const,MemoryManager* const);
%ignore XMLUri(const XMLUri* const,const XMLCh* const,MemoryManager* const);
%ignore XMLURL(MemoryManager* const);
%ignore XMLURL(const XMLCh* const,MemoryManager* const);
%ignore XMLURL(const XMLCh* const,const XMLCh* const,MemoryManager* const);
%ignore    XMLNotationDecl(MemoryManager* const);
%ignore    XMLNotationDecl(
        const   XMLCh* const
        , const XMLCh* const
        , const XMLCh* const
        , const XMLCh* const
        , MemoryManager* const
    );
%ignore getAttTypeString(const AttTypes
        , MemoryManager* const);
%ignore getDefAttTypeString(const DefAttTypes
        , MemoryManager* const);

%ignore DTDGrammar(MemoryManager *const);
%ignore SchemaValidator(XMLErrorReporter *const,MemoryManager *const);
%ignore SchemaGrammar(MemoryManager *const);
%ignore    QName(MemoryManager* const);
%ignore    QName
    (
          const XMLCh* const   prefix
        , const XMLCh* const   localPart
	    , const unsigned int   uriId
        , MemoryManager* const
    );
%ignore    QName
    (
          const XMLCh* const   rawName
	    , const unsigned int   uriId
        , MemoryManager* const
    );


// changing the locale and the NLS home is probably useful
// but changing the panic manager and memory manager is not
%ignore Initialize(const char*          const
                         , const char*          const
                         ,       PanicHandler*  const
                         ,       MemoryManager* const);
%ignore Initialize(const char*          const
                         , const char*          const
                         ,       PanicHandler*  const);

// this constructor has a single XMLCh that interfers with another bool constructor
%ignore DTDEntityDecl(
        const   XMLCh* const
        , const XMLCh       
        , const bool            fromIntSubset = false
        , const bool            specialChar = false);

%ignore XERCES_CPP_NAMESPACE::DOMImplementation::loadDOMExceptionMsg;

/*
 * Perl won't need to create exceptions -- FIXME -- not true
 *   callbacks should be able to call exceptions
 */

%ignore    DOMException();
%ignore    DOMException(      
                       short                 code
               , const XMLCh*                message
               ,       MemoryManager* const  memoryManager = XMLPlatformUtils::fgMemoryManager
                );

%ignore    DOMException(const DOMException &other);

%ignore    DOMRangeException();
%ignore    DOMRangeException(
                            RangeExceptionCode       code
                    , const XMLCh*                   message
                    ,       MemoryManager*     const memoryManager
                     );
%ignore    DOMRangeException(const DOMRangeException &other);

%ignore    SAXException(MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager) ;
%ignore    SAXException(const XMLCh* const msg,
                 MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager) ;
%ignore    SAXException(const char* const msg,
                 MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager) ;
%ignore    SAXException(const SAXException& toCopy) ;

%ignore    SAXNotSupportedException(MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager) ;
%ignore    SAXNotSupportedException(const XMLCh* const msg,
                 MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager) ;
%ignore    SAXNotSupportedException(const char* const msg,
                 MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager) ;

%ignore    SAXNotRecognizedException(MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager) ;
%ignore    SAXNotRecognizedException(const XMLCh* const msg,
                 MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager) ;
%ignore    SAXNotRecognizedException(const char* const msg,
                 MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager) ;

%ignore    SAXParseException(const XMLCh* const message, const Locator& locator,
                      MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);
%ignore    SAXParseException
    (
        const   XMLCh* const    message
        , const XMLCh* const    publicId
        , const XMLCh* const    systemId
        , const XMLSSize_t      lineNumber
        , const XMLSSize_t      columnNumber
        , MemoryManager* const  manager = XMLPlatformUtils::fgMemoryManager
    );
%ignore    SAXParseException(const SAXParseException& toCopy);

/*
 * methods not needed by the XMLValidator interfaces
 */
%ignore XERCES_CPP_NAMESPACE::XMLValidator::emitError;
%ignore XERCES_CPP_NAMESPACE::XMLValidator::checkRootElement;
%ignore XERCES_CPP_NAMESPACE::XMLValidator::reinitMsgMutex;
%ignore XERCES_CPP_NAMESPACE::XMLValidator::reinitMsgLoader;
%ignore XERCES_CPP_NAMESPACE::XMLValidator::setErrorReporter;
%ignore XERCES_CPP_NAMESPACE::XMLValidator::setScannerInfo;
%ignore XERCES_CPP_NAMESPACE::XMLValidator::handlesSchema;
%ignore XERCES_CPP_NAMESPACE::XMLValidator::handlesDTD;
%ignore XERCES_CPP_NAMESPACE::XMLValidator::validateElement;
%ignore XERCES_CPP_NAMESPACE::XMLValidator::validateAttrValue;
%ignore XERCES_CPP_NAMESPACE::XMLValidator::requiresNamespaces;
%ignore XERCES_CPP_NAMESPACE::XMLValidator::reset;
%ignore XERCES_CPP_NAMESPACE::XMLValidator::postParseValidation;
%ignore XERCES_CPP_NAMESPACE::XMLValidator::preContentValidation;
%ignore XERCES_CPP_NAMESPACE::XMLValidator::faultInAttr;
%ignore XERCES_CPP_NAMESPACE::XMLValidator::checkContent;

// ignore all the constructors for the Grammar components
%ignore XERCES_CPP_NAMESPACE::DTDAttDef::DTDAttDef;
%ignore XERCES_CPP_NAMESPACE::DTDElementDecl::DTDElementDecl;
%ignore XERCES_CPP_NAMESPACE::DTDEntityDecl::DTDEntityDecl;
%ignore XERCES_CPP_NAMESPACE::SchemaElementDecl::SchemaElementDecl;
%ignore XERCES_CPP_NAMESPACE::SchemaAttDef::SchemaAttDef;
%ignore	XERCES_CPP_NAMESPACE::DTDAttDefList::DTDAttDefList;
%ignore	XERCES_CPP_NAMESPACE::SchemaAttDefList::SchemaAttDefList;

/*
 * methods not needed by the public Parser interfaces
 *
 *   this is probably because I'm not using AdvDocHandlers and things
 *   that want to control the parsing process, but until someone asks
 *   for them, I'm going to leave them out.
 */

// XMLEntityHandler interface
%ignore endInputSource;
%ignore expandSystemId;
%ignore resetEntities;
%ignore resolveEntity;
%ignore startInputSource;

// XMLDocumentHandler interface.
%ignore docCharacters;
%ignore docComment;
%ignore docPI;
%ignore endDocument;
%ignore endElement;
%ignore endEntityReference;
%ignore ignorableWhitespace;
%ignore resetDocument;
%ignore startDocument;
%ignore startElement;
%ignore startEntityReference;
%ignore XMLDecl;

// depricated methods - don't ask me to include these
%ignore getDoValidation;
%ignore setDoValidation;
%ignore attDef;
%ignore doctypeComment;
%ignore doctypeDecl;
%ignore doctypePI;
%ignore doctypeWhitespace;
%ignore elementDecl;
%ignore endAttList;
%ignore endIntSubset;
%ignore endExtSubset;
%ignore entityDecl;
%ignore resetDocType;
%ignore notationDecl;
%ignore startAttList;
%ignore startIntSubset;
%ignore startExtSubset;
%ignore TextDecl;

// const methods
%ignore getDocumentHandler() const;
%ignore getErrorHandler() const;
%ignore getEntityResolver() const;
%ignore getXMLEntityResolver() const;
%ignore getPSVIHandler() const;

%ignore getContentSpec() const;
%ignore getBaseName() const;
%ignore getElementName() const;
%ignore findAttDef(unsigned long const,XMLCh const *const) const;
%ignore findAttDef(XMLCh const *const,XMLCh const *const) const;
%ignore getAttDef(unsigned int) const;
%ignore getAttDef(XMLCh const *const) const;
%ignore getAttDef(XMLCh const *const,int const) const;
%ignore getElemDecl(unsigned int const) const;
%ignore getElemDecl(unsigned int const,XMLCh const *const,XMLCh const *const,unsigned int) const;
%ignore getNotationDecl(XMLCh const *const) const;
%ignore getEntityDecl(XMLCh const *const) const;
%ignore getEntityDeclPool() const;
%ignore getAttWildCard() const;
%ignore getAnnotation() const;
%ignore getAnnotation(void const *const) const;
%ignore getAnnotations() const;
%ignore findAttDefLocalPart(unsigned long const,XMLCh const *const) const;
%ignore getBaseAttDecl() const;
%ignore getFilter() const;
%ignore getPrefix() const;
%ignore getLocalPart() const;
%ignore getRawName() const;
