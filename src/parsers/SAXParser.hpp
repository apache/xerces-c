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
 * Revision 1.4  2000/02/15 04:47:37  rahulj
 * Documenting the SAXParser framework. Not done yet.
 *
 * Revision 1.3  2000/02/06 07:47:56  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.2  1999/12/15 19:57:48  roddey
 * Got rid of redundant 'const' on boolean return value. Some compilers choke
 * on this and its useless.
 *
 * Revision 1.1.1.1  1999/11/09 01:07:51  twl
 * Initial checkin
 *
 * Revision 1.6  1999/11/08 20:44:54  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */

#if !defined(SAXPARSER_HPP)
#define SAXPARSER_HPP

#include <sax/Parser.hpp>
#include <internal/VecAttrListImpl.hpp>
#include <framework/XMLDocumentHandler.hpp>
#include <framework/XMLElementDecl.hpp>
#include <framework/XMLEntityHandler.hpp>
#include <framework/XMLErrorReporter.hpp>
#include <validators/DTD/DocTypeHandler.hpp>

class DocumentHandler;
class EntityResolver;
class XMLPScanToken;
class XMLScanner;
class XMLValidator;


/**
  * <p>This is the Simple API for XML (SAX) 1_0 driver for Xerces-C. It
  * implements the SAX interface and allows the client program to install
  * SAX handlers for event callbacks.</p>
  *
  * <p>This class uses the parsers internal event interfaces and event
  * callbacks and maps them to the SAX event callbacks.  It can be
  * used to instantiate a validating or non-validating parser, by
  * setting a member flag.</p>
  */

class PARSERS_EXPORT SAXParser :

    public Parser
    , public XMLDocumentHandler
    , public XMLErrorReporter
    , public XMLEntityHandler
    , public DocTypeHandler
{
public :
    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------

    /** @name Constructors and Destructor */
    //@{
    /** Constructor with an instance of validator class to use for
      * validation.
      * @param valToAdopt Pointer to the validator instance to use. The
      *                   parser is responsible for freeing the memory.
      */
    SAXParser(XMLValidator* const valToAdopt = 0);

    /**
      * Destructor
      */
    ~SAXParser();
    //@}


    /** @name Getter methods */
    //@{
    /**
      * This method returns the installed document handler. Suitable
      * for 'lvalue' usages.
      * @return The pointer to the installed document handler object.
      */
    DocumentHandler* getDocumentHandler();

    /**
      * This method returns the installed document handler. Suitable
      * only for 'rvalue' usages.
      * @return A const pointer to the installed document handler object.
      */
    const DocumentHandler* getDocumentHandler() const;

    /**
      * This method returns the installed entity resolver. Suitable
      * for 'lvalue' usages.
      * @return The pointer to the installed entity resolver object.
      */
    EntityResolver* getEntityResolver();

    /**
      * This method returns the installed entity resolver. Suitable
      * for 'rvalue' usages.
      * @return A const pointer to the installed entity resolver object.
      */
    const EntityResolver* getEntityResolver() const;

    /**
      * This method returns the installed error handler. Suitable
      * for 'lvalue' usages.
      * @return The pointer to the installed error handler object.
      */
    ErrorHandler* getErrorHandler();

    /**
      * This method returns the installed error handler. Suitable
      * for 'rvalue' usages.
      * @return A const pointer to the installed error handler object.
      */
    const ErrorHandler* getErrorHandler() const;

    /**
      * This method returns a reference to the underlying scanner object.
      * It allows read only access to data maintained in the scanner.
      * @return A const reference to the underlying scanner object.
      */
    const XMLScanner& getScanner() const;

    /**
      * This method returns a reference to the parser's installed
      * validator.  
      * @return A const reference to the installed validator object.
      */
    const XMLValidator& getValidator() const;

    /**
      * This method returns the state of the parser's namespace
      * handling capability.
      * @return true, if the parser is currently configured to
      *         understand namespaces, false otherwise.
      */
    bool getDoNamespaces() const;
    //@}


    // -----------------------------------------------------------------------
    //  Setter methods
    // -----------------------------------------------------------------------

    /** @name Setter methods */
    //@{
    /**
      * This method allows one to set the parser's namespace handling
      * capability on or off. When set to true, parser starts enforcing
      * all the constraints / rules specified by the NameSpace
      * specification.
      * @param newState The value specifying whether NameSpace rules should
      *                 be enforced or not.
      */
    void setDoNamespaces(const bool newState);

    /**
      * This method allows one to set the parser's validation
      * capability on or off.  
      * @param newState The value specifying whether the parser should
      *                 do validity checks or not against the DTD in the
      *                 input XML document.
      */
    void setDoValidation(const bool newState);

    /**
      * This method allows one to set the parser's behaviour when it
      * encounters the first fatal error. If set to true, the parser
      * will exit at the first fatal error. If false, then it will
      * report the error and continue processing.
      * @param newState The value specifying whether the parser should
      *                 continue or exit when it encounters the first
      *                 fatal error.
      */
    void setExitOnFirstFatalError(const bool newState);
    //@}


    // -----------------------------------------------------------------------
    //  Advanced document handler list maintenance methods
    // -----------------------------------------------------------------------

    /** @name Advanced document handler list maintenance methods */
    //@{
    /**
      * This method installs the specified 'advanced' document callback
      * handler, thereby allowing the user to customize the processing,
      * if they choose to do so. Any number of advanced callback handlers
      * maybe installed.
      * @param toInstall A pointer to the users advanced callback handler.
      */
    void installAdvDocHandler(XMLDocumentHandler* const toInstall);

    /**
      * This method removes the 'advanced' document handler callback from
      * the underlying parser scanner. If no handler is installed, advanced
      * callbacks are not reported by the scanner.
      * @param toRemove A pointer to the users advanced callback handler which
      *                 should be removed.
      */
    bool removeAdvDocHandler(XMLDocumentHandler* const toRemove);
    //@}


    // -----------------------------------------------------------------------
    //  Implementation of the SAXParser interface
    // -----------------------------------------------------------------------

    /** @name Implementation of SAX 1.0 interface's. */
    //@{
    /**
      * This method invokes the parsing process on the XML file specified
      * by the InputSource parameter.
      * @param source A const reference to the InputSource object which points
      *               to the XML file to be parsed.
      * @param reuseValidator The flag indicating whether the existing validator
      *                       should be reused or removed for this parsing
      *                       process.
	  * @see Parser#parse(InputSource)
      */
    virtual void parse(const InputSource& source, const bool reuseValidator = false);

    /**
      * This method invokes the parsing process on the XML file specified by
      * the Unicode string parameter 'systemId'.
      * @param systemId A const XMLCh pointer to the Unicode string which
      *                 contains the path to the XML file to be parsed.
      * @param reuseValidator The flag indicating whether the existing validator
      *                       should be reused or removed for this parsing
      *                       process.
	  * @see Parser#parse(XMLCh*)
      */
    virtual void parse(const XMLCh* const systemId, const bool reuseValidator = false);

    /**
      * This method invokes the parsing process on the XML file specified by
      * the native char* string parameter 'systemId'.
      * @param systemId A const char pointer to a native string which
      *                 contains the path to the XML file to be parsed.
      * @param reuseValidator The flag indicating whether the existing validator
      *                       should be reused or removed for this parsing
      *                       process.
      */
    virtual void parse(const char* const systemId, const bool reuseValidator = false);

    /**
      * This method installs the document handler callback function on
      * parser.
      * @param handler A pointer to the document handler to be called
      *                when the parser comes across 'document' events
      *                as per the SAX specification.
	  * @see Parser#parse(char*)
      */
    virtual void setDocumentHandler(DocumentHandler* const handler);

    /**
      * This method installs the specified DTD handler on the parser.
      * @param handler A pointer to the DTD handler to be called
      *                when the parser comes across 'DTD' events
      *                as per the SAX specification.
	  * @see Parser#setDTDHandler
      */
    virtual void setDTDHandler(DTDHandler* const handler);

    /**
      * This method installs the specified error handler on the parser.
      * @param handler A pointer to the error handler to be called
      *                when the parser comes across 'error' events
      *                as per the SAX specification.
	  * @see Parser#setErrorHandler
      */
    virtual void setErrorHandler(ErrorHandler* const handler);

    /**
      * This method installs the specified entity resolver on the
      * parser. It allows applications to trap and redirect calls
      * to external entities.
      * @param handler A pointer to the entity resolver to be called
      *                when the parser comes across references to
      *                entities in the XML file.
	  * @see Parser#setEntityResolver
      */
    virtual void setEntityResolver(EntityResolver* const resolver);
    //@}


    // -----------------------------------------------------------------------
    //  Progressive scan methods
    // -----------------------------------------------------------------------

    /** @name Progressive scan methods */
    //@{
    /**
      * <p>This method is used to start a progressive parse on a XML file.
      * To continue parsing, subsequent calls must be to the parseNext
      * method.<p>
	  *
	  * <p>It scans through the prolog and returns a token to be used on
      * subsequent scanNext() calls. If the return value is true, then the
	  * token is legal and ready for further use. If it returns false, then
	  * the scan of the prolog failed and the token is not going to work on
	  * subsequent scanNext() calls.</p>
	  *
      * @param systemId A pointer to a Unicode string represting the path
      *                 to the XML file to be parsed.
	  * @param toFill A token maintaing state information to maintain
	  *               internal consistency between invocation of 'parseNext'
	  *               calls.
      * @param reuseValidator The flag indicating whether the existing validator
      *                       should be reused or removed for this parsing
      *                       process.
	  * @return 'true', if successful in parsing the prolog. It indicates the
	  *         user can go ahead with parsing the rest of the file. It returns
	  *         'false' to indicate that the parser could not find a proper
	  *         prolog definition.
	  * @see #parseNext
	  * @see #parseFirst(char*,...)
	  * @see #parseFirst(InputSource&,...)
      */
    bool parseFirst
    (
        const   XMLCh* const    systemId
        ,       XMLPScanToken&  toFill
        , const bool            reuseValidator = false
    );

    /**
      * <p>This method is used to start a progressive parse on a XML file.
      * To continue parsing, subsequent calls must be to the parseNext
      * method.<p>
	  *
	  * <p>It scans through the prolog and returns a token to be used on
      * subsequent scanNext() calls. If the return value is true, then the
	  * token is legal and ready for further use. If it returns false, then
	  * the scan of the prolog failed and the token is not going to work on
	  * subsequent scanNext() calls.</p>
	  *
      * @param systemId A pointer to a regular native string represting
      *                 the path to the XML file to be parsed.
	  * @param toFill A token maintaing state information to maintain
	  *               internal consistency between invocation of 'parseNext'
	  *               calls.
      * @param reuseValidator The flag indicating whether the existing validator
      *                       should be reused or removed for this parsing
      *                       process.
	  * @return 'true', if successful in parsing the prolog. It indicates the
	  *         user can go ahead with parsing the rest of the file. It returns
	  *         'false' to indicate that the parser could not find a proper
	  *         prolog definition.
	  * @see #parseNext
	  * @see #parseFirst(XMLCh*,...)
	  * @see #parseFirst(InputSource&,...)
      */
    bool parseFirst
    (
        const   char* const     systemId
        ,       XMLPScanToken&  toFill
        , const bool            reuseValidator = false
    );

    /**
      * <p>This method is used to start a progressive parse on a XML file.
      * To continue parsing, subsequent calls must be to the parseNext
      * method.<p>
	  *
	  * <p>It scans through the prolog and returns a token to be used on
      * subsequent scanNext() calls. If the return value is true, then the
	  * token is legal and ready for further use. If it returns false, then
	  * the scan of the prolog failed and the token is not going to work on
	  * subsequent scanNext() calls.</p>
	  *
      * @param source A const reference to the InputSource object which
      *                 points to the XML file to be parsed.
	  * @param toFill A token maintaing state information to maintain
	  *               internal consistency between invocation of 'parseNext'
	  *               calls.
      * @param reuseValidator The flag indicating whether the existing validator
      *                       should be reused or removed for this parsing
      *                       process.
	  * @return 'true', if successful in parsing the prolog. It indicates the
	  *         user can go ahead with parsing the rest of the file. It returns
	  *         'false' to indicate that the parser could not find a proper
	  *         prolog definition.
	  * @see #parseNext
	  * @see #parseFirst(XMLCh*,...)
	  * @see #parseFirst(char*,...)
      */
    bool parseFirst
    (
        const   InputSource&    source
        ,       XMLPScanToken&  toFill
        , const bool            reuseValidator = false
    );

    /**
      * <p>This method is used to continue with progressive parsing of
      * XML files started by a call to 'parseFirst' method.<p>
	  *
	  * <p>It parses the XML file and stops as soon as it comes across
      * a XML token (as defined in the XML specification). Relevant
	  * callback handlers are invoked as required by the SAX
	  * specification.</p>
	  *
	  * @param token A token maintaing state information to maintain
	  *              internal consistency between invocation of 'parseNext'
	  *              calls.
	  * @return 'true', if successful in parsing the next XML token.
	  *         It indicates the user can go ahead with parsing the rest
	  *         of the file. It returns 'false' to indicate that the parser
	  *         could not find next token as per the XML specification
	  *         production rule.
	  * @see #parseFirst(XMLCh*,...)
	  * @see #parseFirst(char*,...)
	  * @see #parseFirst(InputSource&,...)
      */
    bool parseNext(XMLPScanToken& token);
    //@}



    // -----------------------------------------------------------------------
    //  Implementation of the DocTypeHandler Interface
    // -----------------------------------------------------------------------

    /** @name Implementation of the DocTypeHandler Interface */
    //@{
    /**
      * This method is used to report back to the user, the attribute
	  * definition just scanned successfully by the parser.
      *
      * <b><font color="#FF0000">This method is not implemented by this SAX
      * driver at this time.</font></b>
	  *
      * @param elemDecl A const reference to the object containing information
	  *                 about the element whose attribute definition was just
	  *                 parsed.
	  * @param attDef   A const reference to the object containing information
	  *                 attribute definition.
      * @param ignore   The flag indicating whether this attribute definition
	  *                 was ignored by the parser or not.
      */
    virtual void attDef
    (
        const   DTDElementDecl& elemDecl
        , const DTDAttDef&      attDef
        , const bool            ignoring
    );

    /**
      * This method is used to report back to the user, the comment occurring
	  * within the DTD which was just scanned successfully by the parser.
      *
      * <b><font color="#FF0000">This method is no-op for this SAX driver
      * implementation.</font></b>
	  *
      * @param comment  A const pointer to a Unicode string representing the
	  *                 text of the comment just parsed.
      */
    virtual void doctypeComment
    (
        const   XMLCh* const    comment
    );

    /**
      * This method is used to report back to the user, the DOCTYPE declaration
	  * just scanned successfully by the parser.
      *
      * <b><font color="#FF0000">This method is no-op for this SAX driver
      * implementation.</font></b>
	  *
      * @param elemDecl A const reference to the object containing information
	  *                 about the root element definition declaration of the
	  *                 XML document being parsed.
      * @param publicId A const pointer to a Unicode string representing the
	  *                 public id of the DTD file.
      * @param systemId A const pointer to a Unicode string representing the
	  *                 system id of the DTD file.
      * @param hasIntSubset A flag indicating if this XML file contains any
	  *                     internal subset.
      */
    virtual void doctypeDecl
    (
        const   DTDElementDecl& elemDecl
        , const XMLCh* const    publicId
        , const XMLCh* const    systemId
        , const bool            hasIntSubset
    );

    /**
      * This method is used to report back to the user, any PI declarations
	  * occurring inside the DTD definition block.
      *
      * <b><font color="#FF0000">This method is no-op for this SAX driver
      * implementation.</font></b>
	  *
      * @param target A const pointer to a Unicode string representing the
	  *               target of the PI declaration.
      * @param data   A const pointer to a Unicode string representing the
	  *               data of the PI declaration. See the PI production rule
      *               in the XML specification for details.
      */
    virtual void doctypePI
    (
        const   XMLCh* const    target
        , const XMLCh* const    data
    );

    /**
      * This method is used to report back to the user, any whitespaces
	  * occurring inside the DTD definition block.
      *
      * <b><font color="#FF0000">This method is no-op for this SAX driver
      * implementation.</font></b>
	  *
      * @param chars  A const pointer to a Unicode string representing the
	  *               whitespace characters.
      * @param length The length of the whitespace Unicode string.
      */
    virtual void doctypeWhitespace
    (
        const   XMLCh* const    chars
        , const unsigned int    length
    );

    /**
      * This method is used to report back to the user, any element declarations
	  * successfully scanned by the parser.
      *
      * <b><font color="#FF0000">This method is no-op for this SAX driver
      * implementation.</font></b>
	  *
      * @param decl   A const reference to the object containing element
	  *               declaration information.
      * @param isIgnored The flag indicating whether this definition was ignored
	  *                  by the parser or not.
      */
    virtual void elementDecl
    (
        const   DTDElementDecl& decl
        , const bool            isIgnored
    );

    /**
      * This method is used to report back to the user, the end of an attribute
	  * list for an element, just scanned by the parser.
      *
      * <b><font color="#FF0000">This method is no-op for this SAX driver
      * implementation.</font></b>
	  *
      * @param elemDecl A const reference to the object containing element
	  *                 declaration information.
      */
    virtual void endAttList
    (
        const   DTDElementDecl& elemDecl
    );

    /**
      * This method is used to report back to the user, the end of the internal
	  * subset that the parser just finished scanning.
      *
      * <b><font color="#FF0000">This method is no-op for this SAX driver
      * implementation.</font></b>
      */
    virtual void endIntSubset();

    /**
      * This method is used to report back to the user, the end of the external
	  * subset that the parser just finished scanning.
      *
      * <b><font color="#FF0000">This method is no-op for this SAX driver
      * implementation.</font></b>
      */
    virtual void endExtSubset();

    /**
      * This method is used to report back to the user, any entity declarations
	  * successfully scanned by the parser. For unparsed entities, the parser
      * will invoke the SAX DTDHandler::unparsedEntityDecl callback.
      *
      * @param entityDecl A const reference to the object containing the entity
	  *                   declaration information.
      * @param isPEDecl  The flag indicating whether this was a parameter entity
	  *                  declaration or not.
      * @param isIgnored The flag indicating whether this definition was ignored
	  *                  by the parser or not.
	  * @see DTDHandler#unparsedEntityDecl
      */
    virtual void entityDecl
    (
        const   DTDEntityDecl&  entityDecl
        , const bool            isPEDecl
        , const bool            isIgnored
    );

    /**
      * This method allows the user installed DTD handler to reset itself.
      */
    virtual void resetDocType();

    /**
      * This method is used to report back to the user, any notation
	  * declarations successfully scanned by the parser. If there is
      * a user installed  DTDHandler method, then the parser will
      * invoke the SAX DTDHandler::notationDecl callback.
      *
      * @param notDecl A const reference to the object containing the notation
	  *                declaration information.
      * @param isIgnored The flag indicating whether this definition was ignored
	  *                  by the parser or not.
	  * @see DTDHandler#notationDecl
      */
    virtual void notationDecl
    (
        const   XMLNotationDecl&    notDecl
        , const bool                isIgnored
    );

    /**
      * This method is used to indicate the start of an element's attribute
      * list declaration.
      *
      * <b><font color="#FF0000">This method is no-op for this SAX driver
      * implementation.</font></b>
	  *
      * @param elemDecl A const reference to the object containing element
	  *                 declaration information.
      */
    virtual void startAttList
    (
        const   DTDElementDecl& elemDecl
    );

    /**
      * This method is used indicate the start of the internal subset
	  * just seen by the parser.
      *
      * <b><font color="#FF0000">This method is no-op for this SAX driver
      * implementation.</font></b>
      */
    virtual void startIntSubset();

    /**
      * This method is used indicate the start of the external subset
	  * just scanned by the parser.
      *
      * <b><font color="#FF0000">This method is no-op for this SAX driver
      * implementation.</font></b>
      */
    virtual void startExtSubset();

    /**
      * This method is used to report back to the user, the TextDecl
	  * just scanned successfully by the parser. Look at the XML
	  * specification for the syntax of a TextDecl.
      *
      * <b><font color="#FF0000">This method is no-op for this SAX driver
      * implementation.</font></b>
	  *
      * @param versionStr A const pointer to a Unicode string representing the
	  *                   version number of the 'version' clause.
      * @param encodingStr A const pointer to a Unicode string representing the
	  *                    encoding name of the 'encoding' clause.
      */
    virtual void TextDecl
    (
        const   XMLCh* const    versionStr
        , const XMLCh* const    encodingStr
    );
    //@}


    // -----------------------------------------------------------------------
    //  Implementation of the XMLDocumentHandler interface
    // -----------------------------------------------------------------------

    /** @name Implementation of the XMLDocumentHandler Interface. */
    //@{
    /**
      */
    virtual void docCharacters
    (
        const   XMLCh* const    chars
        , const unsigned int    length
        , const bool            cdataSection
    );

    virtual void docComment
    (
        const   XMLCh* const    comment
    );

    virtual void docPI
    (
        const   XMLCh* const    target
        , const XMLCh* const    data
    );

    virtual void endDocument();

    virtual void endElement
    (
        const   XMLElementDecl& elemDecl
        , const unsigned int    urlId
        , const bool            isRoot
    );

    virtual void endEntityReference
    (
        const   XMLEntityDecl&  entDecl
    );

    virtual void ignorableWhitespace
    (
        const   XMLCh* const    chars
        , const unsigned int    length
        , const bool            cdataSection
    );

    virtual void resetDocument();

    virtual void startDocument();

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

    virtual void startEntityReference
    (
        const   XMLEntityDecl&  entDecl
    );

    virtual void XMLDecl
    (
        const   XMLCh* const    versionStr
        , const XMLCh* const    encodingStr
        , const XMLCh* const    standaloneStr
        , const XMLCh* const    actualEncodingStr
    );
    //@}


    // -----------------------------------------------------------------------
    //  Implementation of the XMLErrorReporter interface
    // -----------------------------------------------------------------------
    virtual void error
    (
        const   unsigned int                errCode
        , const XMLCh* const                msgDomain
        , const XMLErrorReporter::ErrTypes  errType
        , const XMLCh* const                errorText
        , const XMLCh* const                systemId
        , const XMLCh* const                publicId
        , const unsigned int                lineNum
        , const unsigned int                colNum
    );

    virtual void resetErrors();


    // -----------------------------------------------------------------------
    //  Implementation of the XMLEntityHandler interface
    // -----------------------------------------------------------------------
    virtual void endInputSource(const InputSource& inputSource);

    virtual bool expandSystemId
    (
        const   XMLCh* const    systemId
        ,       XMLBuffer&      toFill
    );

    virtual void resetEntities();

    virtual InputSource* resolveEntity
    (
        const   XMLCh* const    publicId
        , const XMLCh* const    systemId
    );

    virtual void startInputSource(const InputSource& inputSource);


private :
    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    SAXParser(const SAXParser&);
    void operator=(const SAXParser&);


    // -----------------------------------------------------------------------
    //  Private data members
    //
    //  fAttrList
    //      A temporary implementation of the basic SAX attribute list
    //      interface. We use this one over and over on each startElement
    //      event to allow SAX-like access to the element attributes.
    //
    //  fDocHandler
    //      The installed SAX doc handler, if any. Null if none.
    //
    //  fDTDHandler
    //      The installed SAX DTD handler, if any. Null if none.
    //
    //  fElemDepth
    //      This is used to track the element nesting depth, so that we can
    //      know when we are inside content. This is so we can ignore char
    //      data outside of content.
    //
    //  fEntityResolver
    //      The installed SAX entity handler, if any. Null if none.
    //
    //  fErrorHandler
    //      The installed SAX error handler, if any. Null if none.
    //
    //  fAdvDHCount
    //  fAdvDHList
    //  fAdvDHListSize
    //      This is an array of pointers to XMLDocumentHandlers, which is
    //      how we see installed advanced document handlers. There will
    //      usually not be very many at all, so a simple array is used
    //      instead of a collection, for performance. It will grow if needed,
    //      but that is unlikely.
    //
    //      The count is how many handlers are currently installed. The size
    //      is how big the array itself is (for expansion purposes.) When
    //      count == size, is time to expand.
    //
    //  fParseInProgress
    //      This flag is set once a parse starts. It is used to prevent
    //      multiple entrance or reentrance of the parser.
    //
    //  fScanner
    //      The scanner being used by this parser. It is created internally
    //      during construction.
    //
    //  fValidator
    //      The validator that is installed. If none is provided, we will
    //      create and install a DTD validator. We install this on the
    //      scanner we create, which it will use to do validation. We set
    //      ourself on it as the error reporter for validity errors.
    // -----------------------------------------------------------------------
    VecAttrListImpl         fAttrList;
    DocumentHandler*        fDocHandler;
    DTDHandler*             fDTDHandler;
    unsigned int            fElemDepth;
    EntityResolver*         fEntityResolver;
    ErrorHandler*           fErrorHandler;
    unsigned int            fAdvDHCount;
    XMLDocumentHandler**    fAdvDHList;
    unsigned int            fAdvDHListSize;
    bool                    fParseInProgress;
    XMLScanner*             fScanner;
    XMLValidator*           fValidator;
};


// ---------------------------------------------------------------------------
//  SAXParser: Getter methods
// ---------------------------------------------------------------------------
inline DocumentHandler* SAXParser::getDocumentHandler()
{
    return fDocHandler;
}

inline const DocumentHandler* SAXParser::getDocumentHandler() const
{
    return fDocHandler;
}

inline EntityResolver* SAXParser::getEntityResolver()
{
    return fEntityResolver;
}

inline const EntityResolver* SAXParser::getEntityResolver() const
{
    return fEntityResolver;
}

inline ErrorHandler* SAXParser::getErrorHandler()
{
    return fErrorHandler;
}

inline const ErrorHandler* SAXParser::getErrorHandler() const
{
    return fErrorHandler;
}

inline const XMLScanner& SAXParser::getScanner() const
{
    return *fScanner;
}

#endif
