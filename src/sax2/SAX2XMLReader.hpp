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
 * Revision 1.1  2000/08/02 18:02:35  jpolast
 * initial checkin of sax2 implementation
 * submitted by Simon Fell (simon@fell.com)
 * and Joe Polastre (jpolast@apache.org)
 *
 *
 */

#ifndef SAX2XMLReader_HPP
#define SAX2XMLReader_HPP

#include <util/XercesDefs.hpp>
#include <util/XMLUniDefs.hpp>

class ContentHandler ;
class DTDHandler;
class EntityResolver;
class ErrorHandler;
class InputSource;

class SAX_EXPORT SAX2XMLReader
{
public:


    /** @name Constructors and Destructor */
    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    //@{
    /** The default constructor */
    SAX2XMLReader() 
    {
    }
    /** The destructor */
    virtual ~SAX2XMLReader() 
    {
    }
    //@}

    //-----------------------------------------------------------------------
    // The XMLReader interface
    //-----------------------------------------------------------------------
    /** @name The XMLReader interfaces */
    //@{

	/**
	  * This method returns the installed content handler. Suitable
	  * for 'lvalue' usages.
	  *
	  * @return The pointer to the installed content handler object.
	  */
    virtual ContentHandler* getContentHandler() = 0 ;

    /**
      * This method returns the installed content handler. Suitable
      * only for 'rvalue' usages.
      *
      * @return A const pointer to the installed content handler object.
      */
    virtual const ContentHandler* getContentHandler() const = 0 ;

    virtual DTDHandler* getDTDHandler() = 0;

    virtual const DTDHandler* getDTDHandler() const = 0;

    /**
      * This method returns the installed entity resolver. Suitable
      * for 'lvalue' usages.
      *
      * @return The pointer to the installed entity resolver object.
      */
    virtual EntityResolver* getEntityResolver() = 0 ;

    /**
      * This method returns the installed entity resolver. Suitable
      * for 'rvalue' usages.
      *
      * @return A const pointer to the installed entity resolver object.
      */
    virtual const EntityResolver* getEntityResolver() const = 0 ;

    /**
      * This method returns the installed error handler. Suitable
      * for 'lvalue' usages.
      *
      * @return The pointer to the installed error handler object.
      */
    virtual ErrorHandler* getErrorHandler() = 0 ;

    /**
      * This method returns the installed error handler. Suitable
      * for 'rvalue' usages.
      *
      * @return A const pointer to the installed error handler object.
      */
    virtual const ErrorHandler* getErrorHandler() const = 0 ;

	/**
	  * This method gets a Feature as per the SAX2 spec (such as
      * do validation)
	  *
	  * @param name The name of the feature to be set
	  * @param value true if the feature should be enabled.
	  */
	virtual const bool getFeature(const XMLCh* const name) = 0 ;

	/**
      * Gets the value of a property.
      *
      * @param name The property name, which is any fully-qualified URI.
      */
	virtual const void* const getProperty(const XMLCh* const name) = 0 ;


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
    ) = 0;

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
    ) = 0;

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
    ) = 0;
	
  /**
    * Allow an application to register a document event handler.
    *
    * If the application does not register a document handler, all
    * document events reported by the SAX parser will be silently
    * ignored (this is the default behaviour implemented by
    * HandlerBase).
    *
    * Applications may register a new or different handler in the
    * middle of a parse, and the SAX parser must begin using the new
    * handler immediately.
    *
    * @param handler The document handler.
    * @see DocumentHandler#DocumentHandler
    * @see HandlerBase#HandlerBase
    */
    virtual void setContentHandler(ContentHandler* const handler) = 0;

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
    virtual void setDTDHandler(DTDHandler* const handler) = 0;

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
    virtual void setEntityResolver(EntityResolver* const resolver) = 0;
    
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
    virtual void setErrorHandler(ErrorHandler* const handler) = 0;

  /**
	* This method sets a Feature as per the SAX2 spec (such as
	* do validation)
	*
	* @param name The name of the feature to be set
	* @param value true if the feature should be enabled.
	*/
	virtual void setFeature(const XMLCh* const name, const bool value) = 0;

  /**
	* Set the value of a property.
	*
	* @param name The property name, which is any fully-qualified URI.
	* @param value The requested value for the property.
	*/
	virtual void setProperty(const XMLCh* const name, const void* const value) = 0 ;

    //@}


private :
    /* The copy constructor, you cannot call this directly */
    SAX2XMLReader(const SAX2XMLReader&);

    /* The assignment operator, you cannot call this directly */
    void operator=(const SAX2XMLReader&);

};

#endif
