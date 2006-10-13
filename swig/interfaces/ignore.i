//
// These methods are defined in the API, but they are currently unimplemented
//   and throw exceptions if called 
//
%ignore createXMLReader(const XMLCh *);


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


// this constructor has a single XMLCh that interfers with another bool constructor
%ignore DTDEntityDecl(
        const   XMLCh* const
        , const XMLCh       
        , const bool            fromIntSubset = false
        , const bool            specialChar = false);

/*
 * Scripting languages won't need to create exceptions -- FIXME -- not true
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
