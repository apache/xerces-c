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

/*
 * $Log$
 * Revision 1.1  2000/08/02 18:04:41  jpolast
 * initial checkin of sax2 implemenation
 * submitted by Simon Fell (simon@fell.com)
 * and Joe Polastre (jpolast@apache.org)
 *
 *
 */

#if !defined(SAX2XMLReaderImpl_HPP)
#define SAX2XMLReaderImpl_HPP

#include <parsers/SAXParser.hpp>
#include <sax/Parser.hpp>
#include <framework/xmlbuffer.hpp>
#include <internal/VecAttributesImpl.hpp>
#include <sax2/SAX2XMLReader.hpp>
#include <util/RefStackOf.hpp>
#include <util/ValueStackOf.hpp>
#include <framework/XMLBufferMgr.hpp>

class ContentHandler;

/**
  * This class implements the SAX2 'XMLReader' interface and should be
  * used by applications wishing to parse the XML files using SAX2.
  * It allows the client program to install SAX2 handlers for event
  * callbacks.
  *
  * <p>It can be used to instantiate a validating or non-validating
  * parser, by setting a member flag.</p>
  *
  * we basically re-use the existing SAX1 parser code, but provide a 
  * new implementation of XMLContentHandler that raises the new
  * SAX2 style events
  *
  */

class PARSERS_EXPORT SAX2XMLReaderImpl :

	public SAX2XMLReader
    , public SAXParser
{
public :

	static const XMLCh SAX_CORE_VALIDATION[];
	static const XMLCh SAX_CORE_NAMESPACES[];
	static const XMLCh SAX_CORE_NAMESPACES_PREFIXES[];
	static const XMLCh SAX_XERCES_DYNAMIC[];
	
    enum ValSchemes
    {
        Val_Never
        , Val_Always
        , Val_Auto
    };

	SAX2XMLReaderImpl() ;
	~SAX2XMLReaderImpl() ;

	// -----------------------------------------------------------------------
    //  Implementation of the XMLDocumentHandler interface
    // -----------------------------------------------------------------------

    /** @name Implementation of the XMLDocumentHandler Interface. */
    //@{
    /**
      * This method is used to report all the characters scanned
      * by the parser. The driver will invoke the 'characters'
      * method of the user installed SAX Document Handler.
      *
      * <p>If any advanced callback handlers are installed, the
      * corresponding 'docCharacters' method will also be invoked.</p>
      *
      * @param chars   A const pointer to a Unicode string representing the
      *                character data.
      * @param length  The length of the Unicode string returned in 'chars'.
      * @param cdataSection  A flag indicating if the characters represent
      *                      content from the CDATA section.
      * @see DocumentHandler#characters
      */
    virtual void docCharacters
    (
        const   XMLCh* const    chars
        , const unsigned int    length
        , const bool            cdataSection
    );

    /**
      * This method is used to report any comments scanned by the parser.
      * This method is a no-op unless, unless an advanced callback handler
      * is installed, in which case the corresponding 'docComment' method
      * is invoked.
      *
      * @param comment A const pointer to a null terminated Unicode
      *                string representing the comment text.
      */
    virtual void docComment
    (
        const   XMLCh* const    comment
    );

    /**
      * This method is used to report any PI scanned by the parser.
      *
      * <p>Any PI's occurring before any 'content' are not reported
      * to any SAX handler as per the specification. However, all
      * PI's within content are reported via the SAX Document Handler's
      * 'processingInstruction' method.
      *
      * <p>If any advanced callback handlers are installed, the
      * corresponding 'docPI' method will be invoked.</p>
      *
      * @param target A const pointer to a Unicode string representing the
      *               target of the PI declaration.
      * @param data   A const pointer to a Unicode string representing the
      *               data of the PI declaration. See the PI production rule
      *               in the XML specification for details.
      *
      * @see DocumentHandler#processingInstruction
      */
    virtual void docPI
    (
        const   XMLCh* const    target
        , const XMLCh* const    data
    );

    /**
      * This method is used to indicate the end of root element
      * was just scanned by the parser. Corresponding 'endDocument'
      * method of the user installed SAX Document Handler will also
      * be invoked.
      *
      * <p>In addition, if any advanced callback handlers are installed,
      * the corresponding 'endDocument' method is invoked.</p>
      *
      * @see DocumentHandler#endDocument
      */
    virtual void endDocument();

    /**
      * This method is used to indicate the end tag of an element.
      * The driver will invoke the corresponding 'endElement' method of
      * the SAX Document Handler interface.
      *
      * <p>If any advanced callback handlers are installed, the
      * corresponding 'endElement' method is also invoked.</p>
      *
      * @param elemDecl A const reference to the object containing element
      *                 declaration information.
      * @param urlId    An id referring to the namespace prefix, if
      *                 namespaces setting is switched on.
      * @param isRoot   A flag indicating whether this element was the
      *                 root element.
      * @see DocumentHandler#endElement
      */
    virtual void endElement
    (
        const   XMLElementDecl& elemDecl
        , const unsigned int    urlId
        , const bool            isRoot
    );

    /**
      * This method is used to indicate that an end of an entity reference
      * was just scanned.
      *
      * <p>If any advanced callback handlers are installed, the
      * corresponding 'endEnityReference' method is invoked.</p>
      *
      * @param entDecl A const reference to the object containing the
      *                entity declaration information.
      */
    virtual void endEntityReference
    (
        const   XMLEntityDecl&  entDecl
    );

    /**
      * This method is used to report all the whitespace characters,
      * which are determined to be 'ignorable'. This distinction
      * between characters is only made, if validation is enabled.
      * Corresponding 'ignorableWhitespace' method of the user installed
      * SAX Document Handler interface is called.
      *
      * <p>Any whitespace before content is not reported to the SAX
      * Document Handler method, as per the SAX specification.
      * However, if any advanced callback handlers are installed, the
      * corresponding 'ignorableWhitespace' method is invoked.</p>
      *
      * @param chars   A const pointer to a Unicode string representing the
      *                ignorable whitespace character data.
      * @param length  The length of the Unicode string 'chars'.
      * @param cdataSection  A flag indicating if the characters represent
      *                      content from the CDATA section.
      * @see DocumentHandler#ignorableWhitespace
      */
    virtual void ignorableWhitespace
    (
        const   XMLCh* const    chars
        , const unsigned int    length
        , const bool            cdataSection
    );

    /**
      * This method allows the user installed Document Handler and
      * any advanced callback handlers to 'reset' themselves.
      */
    virtual void resetDocument();

    /**
      * This method is used to report the start of the parsing process.
      * The corresponding user installed SAX Document Handler's method
      * 'startDocument' is invoked.
      * 
      * <p>If any advanced callback handlers are installed, then the
      * corresponding 'startDocument' method is also called.</p>
      *
      * @see DocumentHandler#startDocument
      */
    virtual void startDocument();

    /**
      * This method is used to report the start of an element. It is
      * called at the end of the element, by which time all attributes
      * specified are also parsed. The corresponding user installed
      * SAX Document Handler's method 'startElement' is invoked.
      * 
      * <p>If any advanced callback handlers are installed, then the
      * corresponding 'startElement' method is also called.</p>
      *
      * @param elemDecl A const reference to the object containing element
      *                 declaration information.
      * @param urlId    An id referring to the namespace prefix, if
      *                 namespaces setting is switched on.
      * @param elemPrefix A const pointer to a Unicode string containing
      *                   the namespace prefix for this element. Applicable
      *                   only when namespace processing is enabled.
      * @param attrList  A const reference to the object containing the
      *                  list of attributes just scanned for this element.
      * @param attrCount A count of number of attributes in the list
      *                  specified by the parameter 'attrList'.
      * @param isEmpty  A flag indicating whether this is an empty element
      *                 or not.
      * @param isRoot   A flag indicating whether this element was the
      *                 root element.
      * @see DocumentHandler#startElement
      */
    virtual void startElement
    (
        const   XMLElementDecl&         elemDecl
        , const unsigned int            urlId
        , const XMLCh* const            elemPrefix
        , const RefVectorOf<XMLAttr>&   attrList
        , const unsigned int            attrCount
        , const bool                    isEmpty
        , const bool                    isRoot
    );

    /**
      * This method is used to indicate the start of an entity reference.
      *
      * <p>If any advanced callback handlers are installed, the
      * corresponding 'endEnityReference' method is invoked.</p>
      *
      * @param entDecl A const reference to the object containing the
      *                entity declaration information.
      */
    virtual void startEntityReference
    (
        const   XMLEntityDecl&  entDecl
    );

    /**
      * This method is used to report the XML decl scanned by the parser.
      * Refer to the XML specification to see the meaning of parameters.
      *
      * <b><font color="#FF0000">This method is a no-op for this SAX driver
      * implementation.</font></b>
      *
      * @param versionStr A const pointer to a Unicode string representing
      *                   version string value.
      * @param encodingStr A const pointer to a Unicode string representing
      *                    the encoding string value.
      * @param standaloneStr A const pointer to a Unicode string
      *                      representing the standalone string value.
      * @param actualEncodingStr A const pointer to a Unicode string
      *                          representing the actual encoding string
      *                          value.
      */
    virtual void XMLDecl
    (
        const   XMLCh* const    versionStr
        , const XMLCh* const    encodingStr
        , const XMLCh* const    standaloneStr
        , const XMLCh* const    actualEncodingStr
    );
    //@}

	// -----------------------------------------------------------------------
    //  Implementation of the XMLReader interface
    // -----------------------------------------------------------------------

    /** @name Getter methods */
    //@{
    /**
      * This method returns the installed document handler. Suitable
      * for 'lvalue' usages.
      *
      * @return The pointer to the installed document handler object.
      */
    virtual ContentHandler* getContentHandler();

    /**
      * This method returns the installed document handler. Suitable
      * only for 'rvalue' usages.
      *
      * @return A const pointer to the installed document handler object.
      */
    virtual const ContentHandler* getContentHandler() const;

    virtual DTDHandler* getDTDHandler() ;

    virtual const DTDHandler* getDTDHandler() const ;

    /**
      * This method returns the installed entity resolver. Suitable
      * for 'lvalue' usages.
      *
      * @return The pointer to the installed entity resolver object.
      */
    virtual EntityResolver* getEntityResolver()  ;

    /**
      * This method returns the installed entity resolver. Suitable
      * for 'rvalue' usages.
      *
      * @return A const pointer to the installed entity resolver object.
      */
    virtual const EntityResolver* getEntityResolver() const  ;

    /**
      * This method returns the installed error handler. Suitable
      * for 'lvalue' usages.
      *
      * @return The pointer to the installed error handler object.
      */
    virtual ErrorHandler* getErrorHandler() ;

    /**
      * This method returns the installed error handler. Suitable
      * for 'rvalue' usages.
      *
      * @return A const pointer to the installed error handler object.
      */
    virtual const ErrorHandler* getErrorHandler() const ;

  /**
    * Parse an XML document.
    *
    * The application can use this method to instruct the SAX parser
    * to begin parsing an XML document from any valid input
    * source (a character stream, a byte stream, or a URI).
    *
    * Applications may not invoke this method while a parse is in
    * progress (they should create a new Parser instead for each
    * additional XML document).  Once a parse is complete, an
    * application may reuse the same Parser object, possibly with a
    * different input source.
    *
    * @param source The input source for the top-level of the
    *               XML document.
    * @param reuseValidator Indicates whether the validator should be
    *               reused, ignoring any external subset. If true, there
    *               cannot be any internal subset.
    * @exception SAXException Any SAX exception, possibly
    *            wrapping another exception.
    * @exception XMLException An exception from the parser or client
    *            handler code.
    * @see InputSource#InputSource
    * @see #setEntityResolver
    * @see #setDTDHandler
    * @see #setDocumentHandler
    * @see #setErrorHandler
    */
    virtual void parse
    (
        const   InputSource&    source
        , const bool            reuseValidator = false
    ) ;

  /**
    * Parse an XML document from a system identifier (URI).
    *
    * This method is a shortcut for the common case of reading a
    * document from a system identifier.  It is the exact equivalent
    * of the following:
    *
    * parse(new URLInputSource(systemId));
    *
    * If the system identifier is a URL, it must be fully resolved
    * by the application before it is passed to the parser.
    *
    * @param systemId The system identifier (URI).
    * @param reuseValidator Indicates whether the validator should be
    *               reused, ignoring any external subset. If true, there
    *               cannot be any internal subset.
    * @exception SAXException Any SAX exception, possibly
    *            wrapping another exception.
    * @exception XMLException An exception from the parser or client
    *            handler code.
    * @see #parse(InputSource)
    */
    virtual void parse
    (
        const   XMLCh* const    systemId
        , const bool            reuseValidator = false
    ) ;

  /**
    * Parse an XML document from a system identifier (URI).
    *
    * This method is a shortcut for the common case of reading a
    * document from a system identifier.  It is the exact equivalent
    * of the following:
    *
    * parse(new URLInputSource(systemId));
    *
    * If the system identifier is a URL, it must be fully resolved
    * by the application before it is passed to the parser.
    *
    * @param systemId The system identifier (URI).
    * @param reuseValidator Indicates whether the validator should be
    *               reused, ignoring any external subset. If true, there
    *               cannot be any internal subset.
    * @exception SAXException Any SAX exception, possibly
    *            wrapping another exception.
    * @exception XMLException An exception from the parser or client
    *            handler code.
    * @see #parse(InputSource)
    */
    virtual void parse
    (
        const   char* const     systemId
        , const bool            reuseValidator = false
    ) ;

    /**
      * This method installs the user specified SAX Document Handler
      * callback function on parser.
      *
      * @param handler A pointer to the document handler to be called
      *                when the parser comes across 'document' events
      *                as per the SAX specification.
      *
      * @see Parser#parse(char*)
      */
    virtual void setContentHandler(ContentHandler* const handler);

  /**
    * Allow an application to register a DTD event handler.
    *
    * If the application does not register a DTD handler, all DTD
    * events reported by the SAX parser will be silently ignored (this
    * is the default behaviour implemented by HandlerBase).
    *
    * Applications may register a new or different handler in the middle
    * of a parse, and the SAX parser must begin using the new handler
    * immediately.
    *
    * @param handler The DTD handler.
    * @see DTDHandler#DTDHandler
    * @see HandlerBase#HandlerBase
    */
    virtual void setDTDHandler(DTDHandler* const handler) ;

  /**
    * Allow an application to register a custom entity resolver.
    *
    * If the application does not register an entity resolver, the
    * SAX parser will resolve system identifiers and open connections
    * to entities itself (this is the default behaviour implemented in
    * DefaultHandler).
    *
    * Applications may register a new or different entity resolver
    * in the middle of a parse, and the SAX parser must begin using
    * the new resolver immediately.
    *
    * @param resolver The object for resolving entities.
    * @see EntityResolver#EntityResolver
    * @see DefaultHandler#DefaultHandler
    */
    virtual void setEntityResolver(EntityResolver* const resolver) ;
    
  /**
    * Allow an application to register an error event handler.
    *
    * If the application does not register an error event handler,
    * all error events reported by the SAX parser will be silently
    * ignored, except for fatalError, which will throw a SAXException
    * (this is the default behaviour implemented by HandlerBase).
    *
    * Applications may register a new or different handler in the
    * middle of a parse, and the SAX parser must begin using the new
    * handler immediately.
    *
    * @param handler The error handler.
    * @see ErrorHandler#ErrorHandler
    * @see SAXException#SAXException
    * @see HandlerBase#HandlerBase
    */
    virtual void setErrorHandler(ErrorHandler* const handler) ;

	/**
	  * This method sets a Feature as per the SAX2 spec (such as
	  * do validation)
	  *
	  * @param name The name of the feature to be set
	  * @param value true if the feature should be enabled.
	  */
	virtual void setFeature(const XMLCh* const name, const bool value);

	/**
	  * This method gets a Feature as per the SAX2 spec (such as
	  * do validation)
	  *
	  * @param name The name of the feature to be set
	  * @param value true if the feature should be enabled.
	  */
	virtual const bool getFeature(const XMLCh* const name);

	/**
	  * Set the value of a property.
	  *
	  * @param name The property name, which is any fully-qualified URI.
	  * @param value The requested value for the property.
	  */
	virtual void setProperty(const XMLCh* const name, const void* const value);

	/**
	  * Gets the value of a property.
	  *
	  * @param name The property name, which is any fully-qualified URI.
	  */
	virtual const void* const getProperty(const XMLCh* const name);


private :
    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    SAX2XMLReaderImpl(const SAX2XMLReaderImpl&);
    void operator=(const SAX2XMLReaderImpl&);

    // -----------------------------------------------------------------------
    //  Private data members
    //
    //  fAttrList
    //      A temporary implementation of the basic SAX2 Attributes
    //      interface. We use this one over and over on each startElement
    //      event to allow SAX-like access to the element attributes.
    //
    //  fDocHandler
    //      The installed SAX doc handler, if any. Null if none.
    //
	//  fnamespacePrefix
	//      Indicates whether the namespace-prefix feature is on or off.
	//
	//	fStringBuffers
	//		Any temporary strings we need are pulled out of this pool
	//
	//	fPrefixes 
	//		A Stack of the current namespace prefixes that need calls to
	//		endPrefixMapping
	//
	//	fPrefixCounts
	//		A Stack of the number of prefixes that need endPrefixMapping
	//		calls for that element
    // -----------------------------------------------------------------------
	VecAttributesImpl		   fAttrList ;
	ContentHandler*			   fDocHandler ;
	RefVectorOf<XMLAttr>*      tempAttrVec ;

	bool                       fnamespacePrefix;
	bool                       fautoValidation;
	bool                       fValidation;

	XMLBufferMgr			   fStringBuffers ;
	RefStackOf<XMLBuffer> *    fPrefixes ;
	ValueStackOf<unsigned int> * prefixCounts ;

	// internal function used to set the state of validation: always, never, or auto
	virtual void setValidationScheme(const ValSchemes newScheme);

};


// ---------------------------------------------------------------------------
//  SAX2XMLReader: Getter methods
// ---------------------------------------------------------------------------
inline ContentHandler* SAX2XMLReaderImpl::getContentHandler()
{
    return fDocHandler;
}

inline const ContentHandler* SAX2XMLReaderImpl::getContentHandler() const
{
    return fDocHandler;
}

inline DTDHandler* SAX2XMLReaderImpl::getDTDHandler()
{
	return fDTDHandler ;
}

inline const DTDHandler* SAX2XMLReaderImpl::getDTDHandler() const 
{
	return fDTDHandler ;
}

inline EntityResolver* SAX2XMLReaderImpl::getEntityResolver() 
{
	return SAXParser::getEntityResolver() ;
}

inline const EntityResolver* SAX2XMLReaderImpl::getEntityResolver() const  
{
	return SAXParser::getEntityResolver() ;
}

inline ErrorHandler* SAX2XMLReaderImpl::getErrorHandler() 
{
	return SAXParser::getErrorHandler() ;
}

inline const ErrorHandler* SAX2XMLReaderImpl::getErrorHandler() const
{
	return SAXParser::getErrorHandler() ;
}

#endif 
