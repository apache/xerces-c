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
 * Revision 1.5  2000/02/17 03:53:50  rahulj
 * Added some new getters to query the parser state and
 * Finished documenting the public and protected methods.
 *
 * Revision 1.4  2000/02/06 07:47:56  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.3  2000/01/20 19:10:08  roddey
 * Added protected get/set methods for doc and cur node members. Some people
 * had been writing DOMParser derivatives and touching these directly, so when they
 * were made private this broke some people's code.
 *
 * Revision 1.2  1999/12/15 19:57:48  roddey
 * Got rid of redundant 'const' on boolean return value. Some compilers choke
 * on this and its useless.
 *
 * Revision 1.1.1.1  1999/11/09 01:07:49  twl
 * Initial checkin
 *
 * Revision 1.5  1999/11/08 20:44:53  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */

#if !defined(DOMPARSER_HPP)
#define DOMPARSER_HPP


#include <dom/DOM_Document.hpp>
#include <framework/XMLDocumentHandler.hpp>
#include <framework/XMLErrorReporter.hpp>
#include <framework/XMLEntityHandler.hpp>
#include <util/ValueStackOf.hpp>

class EntityResolver;
class ErrorHandler;
class XMLPScanToken;
class XMLScanner;
class XMLValidator;


/**
  * This class implements the Document Object Model (DOM) interface.
  * It should be used by applications which choose to parse and
  * process the XML document using the DOM api's. This implementation
  * also allows the applications to install an error and an entitty
  * handler (useful extensions to the DOM specification).
  *
  * <p>It can be used to instantiate a validating or non-validating
  * parser, by setting a member flag.</p>
  */
class PARSERS_EXPORT DOMParser :

    public XMLDocumentHandler
    , public XMLErrorReporter
    , public XMLEntityHandler
{
public :
    // -----------------------------------------------------------------------
    //  Constructors and Detructor
    // -----------------------------------------------------------------------

    /** @name Constructors and Destructor */
    //@{
    /** Constructor with an instance of validator class to use for
      * validation.
      * @param valToAdopt Pointer to the validator instance to use. The
      *                   parser is responsible for freeing the memory.
      */
    DOMParser(XMLValidator* const valToAdopt = 0);

    /**
      * Destructor
      */
    ~DOMParser();
    //@}

    /** This method resets the state of the DOM driver and makes
      * it ready for a fresh parse run.
      */
    void reset();


    // -----------------------------------------------------------------------
    //  Getter methods
    // -----------------------------------------------------------------------
 
    /** @name Getter methods */
    //@{
    /**
      * This method returns the DOM_Document object representing the
      * root of the document tree. This object provides the primary
      * access to the document's data.
      *
      * @return The DOM_Document object which represents the entire
      *         XML document.
      */
    DOM_Document getDocument();

    /**
      * This method returns the installed error handler. Suitable
      * for 'lvalue' usages.
      *
      * @return The pointer to the installed error handler object.
      */
    ErrorHandler* getErrorHandler();

    /**
      * This method returns the installed error handler. Suitable
      * for 'rvalue' usages.
      *
      * @return A const pointer to the installed error handler object.
      */
    const ErrorHandler* getErrorHandler() const;

    /**
      * This method returns the installed entity resolver. Suitable
      * for 'lvalue' usages.
      *
      * @return The pointer to the installed entity resolver object.
      */
    EntityResolver* getEntityResolver();

    /**
      * This method returns the installed entity resolver. Suitable
      * for 'rvalue' usages.
      *
      * @return A const pointer to the installed entity resolver object.
      */
    const EntityResolver* getEntityResolver() const;

    /**
      * This method returns a reference to the underlying scanner object.
      * It allows read only access to data maintained in the scanner.
      *
      * @return A const reference to the underlying scanner object.
      */
    const XMLScanner& getScanner() const;

    /**
      * This method returns a reference to the parser's installed
      * validator.
      *
      * @return A const reference to the installed validator object.
      */
    const XMLValidator& getValidator() const;

    /**
      * This method returns the state of the parser's namespace
      * handling capability.
      *
      * @return true, if the parser is currently configured to
      *         understand namespaces, false otherwise.
      *
      * @see #setDoNamespaces
      */
    bool getDoNamespaces() const;

    /**
      * This method returns the state of the parser's validation
      * handling flag which controls whether validation checks
      * are enforced or not.
      *
      * @return true, if the parser is currently configured to
      *         do validation, false otherwise.
      *
      * @see #setDoValidation
      */
    bool getDoValidation() const;

    /**
      * This method returns the state of the parser's
      * exit-on-First-Fatal-Error flag.
      *
      * @return true, if the parser is currently configured to
      *         exit on the first fatal error, false otherwise.
      *
      * @see #setExitOnFirstFatalError
      */
    bool getExitOnFirstFatalError() const;

    /**
      * This method returns the state of the parser's expand entity
      * references flag.
      *
      * @return 'true' if the expand entity reference flag is set on
      *         the parser, 'false' otherwise.
      *
      * @see #setExpandEntityReferences
      */
    bool getExpandEntityReferences() const;
    //@}


    // -----------------------------------------------------------------------
    //  Setter methods
    // -----------------------------------------------------------------------

    /** @name Setter methods */
    //@{
    /**
      * This method allows applications to install their own error
      * handler to trap error and warning messages.
      *
      * @param handler  A const pointer to the user supplied error
      *                 handler.
      *
      * @see #getErrorHandler
      */
    void setErrorHandler(ErrorHandler* const handler);

    /**
      * This method allows applications to install their own entity
      * resolver. By installing an entity resolver, the applications
      * can trap and potentially redirect references to external
      * entities.
      *
      * @param handler  A const pointer to the user supplied entity
      *                 resolver.
      *
      * @see #getEntityResolver
      */
    void setEntityResolver(EntityResolver* const handler);

    /**
      * This method allows users to enable or disable the parser's
      * namespace processing. When set to true, parser starts enforcing
      * all the constraints / rules specified by the NameSpace
      * specification.
      *
      * <p>The parser's default state is: false.</p>
      *
      * <p>This flag is ignored by the underlying scanner if the installed
      * validator indicates that namespace constraints should be
      * enforced.</p>
      *
      * @param newState The value specifying whether NameSpace rules should
      *                 be enforced or not.
      *
      * @see #getDoNamespaces
      */
    void setDoNamespaces(const bool newState);

    /**
      * This method allows users to enable or disable the parser's validation
      * checks.
      *
      * <p>By default, the parser does not to any validation. The default
      * value is false.</p>
      *
      * @param newState The value specifying whether the parser should
      *                 do validity checks or not against the DTD in the
      *                 input XML document.
      *
      * @see #getDoValidation
      */
    void setDoValidation(const bool newState);

    /**
      * This method allows users to set the parser's behaviour when it
      * encounters the first fatal error. If set to true, the parser
      * will exit at the first fatal error. If false, then it will
      * report the error and continue processing.
      *
      * <p>The default value is 'true' and the parser exits on the
      * first fatal error.</p>
      *
      * @param newState The value specifying whether the parser should
      *                 continue or exit when it encounters the first
      *                 fatal error.
      *
      * @see #getExitOnFirstFatalError
      */
    void setExitOnFirstFatalError(const bool newState);

    /**
      * This method allows the user to specify whether the parser
      * should expand all entity reference nodes. When the
      * 'do expansion' flag is true, the DOM tree does not have
      * any entity reference nodes. Is is replaced by the sub-tree
      * representing the replacement text  of the entity. When
      * the 'do expansion' flag is false, the DOM tree contains an
      * extra entity reference node, whose children is the
      * sub tree of the replacement text.
      *
      * @param expand The new state of the expand entity reference
      *               flag.
      */
    void setExpandEntityReferences(const bool expand);
    //@}


    // -----------------------------------------------------------------------
    //  Parsing methods
    // -----------------------------------------------------------------------

    /** @name Parsing methods */
    //@{
    /**
      * This method invokes the parsing process on the XML file specified
      * by the InputSource parameter. This API is borrowed from the
      * SAX Parser interface.
      *
      * @param source A const reference to the InputSource object which
      *               points to the XML file to be parsed.
      * @param reuseValidator The flag indicating whether the existing
      *                       validator should be reused or not for this
      *                       parsing run.
      * @see Parser#parse(InputSource)
      */
    void parse(const InputSource& source, const bool reuseValidator = false);

    /**
      * This method invokes the parsing process on the XML file specified by
      * the Unicode string parameter 'systemId'. This method is borrowed
      * from the SAX Parser interface.
      *
      * @param systemId A const XMLCh pointer to the Unicode string which
      *                 contains the path to the XML file to be parsed.
      * @param reuseValidator The flag indicating whether the existing
      *                       validator should be reused or not for this
      *                       parsing run.
      *
      * @see Parser#parse(XMLCh*)
      */
    void parse(const XMLCh* const systemId, const bool reuseValidator = false);

    /**
      * This method invokes the parsing process on the XML file specified by
      * the native char* string parameter 'systemId'.
      *
      * @param systemId A const char pointer to a native string which
      *                 contains the path to the XML file to be parsed.
      * @param reuseValidator The flag indicating whether the existing
      *                       validator should be reused or not for this
      *                       parsing run.
      */
    void parse(const char* const systemId, const bool reuseValidator = false);

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
      *                       should be reused or not for this parsing
      *                       process.
      * @return 'true', if successful in parsing the prolog. It indicates the
      *         user can go ahead with parsing the rest of the file. It returns
      *         'false' to indicate that the parser could not find a proper
      *         prolog definition.
      *
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
      *                       should be reused or not for this parsing
      *                       run.
      *
      * @return 'true', if successful in parsing the prolog. It indicates the
      *         user can go ahead with parsing the rest of the file. It returns
      *         'false' to indicate that the parser could not find a proper
      *         prolog definition.
      *
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
      *                       should be reused or not for this parsing
      *                       process.
      *
      * @return 'true', if successful in parsing the prolog. It indicates the
      *         user can go ahead with parsing the rest of the file. It returns
      *         'false' to indicate that the parser could not find a proper
      *         prolog definition.
      *
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
      * a XML token (as defined in the XML specification).</p>
      *
      * @param token A token maintaing state information to maintain
      *              internal consistency between invocation of 'parseNext'
      *              calls.
      *
      * @return 'true', if successful in parsing the next XML token.
      *         It indicates the user can go ahead with parsing the rest
      *         of the file. It returns 'false' to indicate that the parser
      *         could not find next token as per the XML specification
      *         production rule.
      *
      * @see #parseFirst(XMLCh*,...)
      * @see #parseFirst(char*,...)
      * @see #parseFirst(InputSource&,...)
      */
    bool parseNext(XMLPScanToken& token);
    //@}



    // -----------------------------------------------------------------------
    //  Implementation of the XMLErrorReporter interface.
    // -----------------------------------------------------------------------

    /** @name Implementation of the XMLErrorReporter interface. */
    //@{
    /**
      * This method is used to report back errors found while parsing the
      * XML file. This method is also borrowed from the SAX specification.
      * It calls the corresponding user installed Error Handler method:
      * 'fatal', 'error', 'warning' depending on the severity of the error.
      * This classification is defined by the XML specification.
      *
      * @param errCode An integer code for the error.
      * @param msgDomain A const pointer to an Unicode string representing
      *                  the message domain to use.
      * @param errType An enumeration classifying the severity of the error.
      * @param errorText A const pointer to an Unicode string representing
      *                  the text of the error message.
      * @param systemId  A const pointer to an Unicode string representing
      *                  the system id of the XML file where this error
      *                  was discovered.
      * @param publicId  A const pointer to an Unicode string representing
      *                  the public id of the XML file where this error
      *                  was discovered.
      * @param lineNum   The line number where the error occurred.
      * @param colNum    The column number where the error occurred.
      * @see ErrorHandler
      */
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

    /**
      * This method allows the user installed Error Handler
      * callback to 'reset' itself.
      *
      * <b><font color="#FF0000">This method is a no-op for this DOM
      * implementation.</font></b>
      *
      */
    virtual void resetErrors();
    //@}


    // -----------------------------------------------------------------------
    //  Implementation of the XMLEntityHandler interface.
    // -----------------------------------------------------------------------

    /** @name Implementation of the XMLEntityHandler interface. */
    //@{
    /**
      * This method is used to indicate the end of parsing of an external
      * entity file.
      *
      * <b><font color="#FF0000">This method is a no-op for this DOM
      * implementation.</font></b>
      *
      * @param inputSource A const reference to the InputSource object
      *                    which points to the XML file being parsed.
      * @see InputSource
      */
    virtual void endInputSource(const InputSource& inputSource);

    /**
      * This method allows an installed XMLEntityHandler to further
      * process any system id's of enternal entities encountered in
      * the XML file being parsed, such as redirection etc.
      *
      * <b><font color="#FF0000">This method always returns 'false'
      * for this DOM implementation.</font></b>
      *
      * @param systemId  A const pointer to an Unicode string representing
      *                  the system id scanned by the parser.
      * @param toFill    A pointer to a buffer in which the application
      *                  processed system id is stored.
      * @return 'true', if any processing is done, 'false' otherwise. 
      */
    virtual bool expandSystemId
    (
        const   XMLCh* const    systemId
        ,       XMLBuffer&      toFill
    );

    /**
      * This method allows the installed XMLEntityHandler to reset
      * itself.
      *
      * <b><font color="#FF0000">This method is a no-op for this DOM
      * implementation.</font></b>
      */
    virtual void resetEntities();

    /**
      * This method allows a user installed entity handler to further
      * process any pointers to external entities. The applications
      * can implement 'redirection' via this callback. This method
      * is also borrowed from the SAX specification.
      *
      * @param publicId A const pointer to a Unicode string representing the
      *                 public id of the entity just parsed.
      * @param systemId A const pointer to a Unicode string representing the
      *                 system id of the entity just parsed.
      * @return The value returned by the user installed resolveEntity
      *         method or NULL otherwise to indicate no processing was done.
      * @see EntityResolver
      */
    virtual InputSource* resolveEntity
    (
        const   XMLCh* const    publicId
        , const XMLCh* const    systemId
    );

    /**
      * This method is used to indicate the start of parsing an
      * external entity file.
      *
      * <b><font color="#FF0000">This method is a no-op for this SAX driver
      * implementation.</font></b>
      *
      * @param inputSource A const reference to the InputSource object
      *                    which points to the external entity 
      *                    being parsed.
      */
    virtual void startInputSource(const InputSource& inputSource);
    //@}



    // -----------------------------------------------------------------------
    //  Implementation of the XMLDocumentHandler interface.
    // -----------------------------------------------------------------------

    /** @name Implementation of the XMLDocumentHandler interface. */
    //@{
    /**
      * This method is used to report all the characters scanned
      * by the parser. This DOM implementation stores this data
      * in the appropriate DOM node, creating one if necessary.
      *
      * @param chars   A const pointer to a Unicode string representing the
      *                character data.
      * @param length  The length of the Unicode string returned in 'chars'.
      * @param cdataSection  A flag indicating if the characters represent
      *                      content from the CDATA section.
      */
    virtual void docCharacters
    (
        const   XMLCh* const    chars
        , const unsigned int    length
        , const bool            cdataSection
    );

    /**
      * This method is used to report any comments scanned by the parser.
      * A new comment node is created which stores this data.
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
      * A new PI node is created and appended as a child of the
      * current node in the tree.
      *
      * @param target A const pointer to a Unicode string representing the
      *               target of the PI declaration.
      * @param data   A const pointer to a Unicode string representing the
      *               data of the PI declaration. See the PI production rule
      *               in the XML specification for details.
      */
    virtual void docPI
    (
        const   XMLCh* const    target
        , const XMLCh* const    data
    );

    /**
      * This method is used to indicate the end of root element
      * was just scanned by the parser.
      */
    virtual void endDocument();

    /**
      * This method is used to indicate the end tag of an element.
      *
      * @param elemDecl A const reference to the object containing element
      *                 declaration information.
      * @param urlId    An id referring to the namespace prefix, if
      *                 namespaces setting is switched on.
      * @param isRoot   A flag indicating whether this element was the
      *                 root element.
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
      *
      * <p>Any whitespace before content is ignored. If the current
      * node is already of type DOM_Node::TEXT_NODE, then these
      * whitespaces are appended, otherwise a new Text node is
      * created which stores this data. Essentially all ignoreable
      * characters are collected in one node.</p>
      *
      * @param chars   A const pointer to a Unicode string representing the
      *                ignorable whitespace character data.
      * @param length  The length of the Unicode string 'chars'.
      * @param cdataSection  A flag indicating if the characters represent
      *                      content from the CDATA section.
      */
    virtual void ignorableWhitespace
    (
        const   XMLCh* const    chars
        , const unsigned int    length
        , const bool            cdataSection
    );

    /**
      * This method allows the user installed Document Handler
      * to 'reset' itself, freeing all the memory resources. The
      * scanner calls this method before starting a new parse
      * event.
      */
    virtual void resetDocument();

    /**
      * This method is used to report the start of the parsing process.
      */
    virtual void startDocument();

    /**
      * This method is used to report the start of an element. It is
      * called at the end of the element, by which time all attributes
      * specified are also parsed. A new DOM Element node is created
      * along with as many attribute nodes as required. This new
      * element is added appended as a child of the current node in
      * the tree.
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
      * If the expand entity reference flag is true, then a new
      * DOM Entity reference node is created.
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
      * <b><font color="#FF0000">This method is a no-op for this DOM
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
        , const XMLCh* const    actualEncStr
    );
    //@}


protected :
    // -----------------------------------------------------------------------
    //  Protected getter methods
    // -----------------------------------------------------------------------

    /** @name Protected getter methods */
    //@{
    /**
      */
    DOM_Node getCurrentNode();
    //@}


    // -----------------------------------------------------------------------
    //  Protected setter methods
    // -----------------------------------------------------------------------

    /** @name Protected setter methods */
    //@{
    /**
      * This method sets the current node maintained inside the
      * parser to the one specified.
      *
      * @param toSet The DOM node which will be the current node.
      */
    void setCurrentNode(DOM_Node toSet);

    /**
      * This method sets the DOM Document node to the one specified.
      *
      * @param toSet The new DOM Document node for this XML document.
      */
    void setDocument(DOM_Document toSet);
    //@}


private :
    // -----------------------------------------------------------------------
    //  Private data members
    //
    //  fCurrentNode
    //  fCurrentParent
    //      Used to track the current node during nested element events. Since
    //      the tree must be built from a set of disjoint callbacks, we need
    //      these to keep up with where we currently are.
    //
    //  fDocument
    //      The root document object, filled with the document contents.
    //
    //  fEntityResolver
    //      The installed SAX entity resolver, if any. Null if none.
    //
    //  fErrorHandler
    //      The installed SAX error handler, if any. Null if none.
    //
    //  fExpandEntityReference
    //      Indicates whether entity reference nodes should be expanded to
    //      its constituent text nodes or just created a single (end result)
    //      text node.
    //
    //  fNodeStack
    //      Used to track previous parent nodes during nested element events.
    //
    //  fParseInProgress
    //      Used to prevent multiple entrance to the parser while its doing
    //      a parse.
    //
    //  fScanner
    //      The scanner used for this parser. This is created during the
    //      constructor.
    //
    //  fValidator
    //      The validator that is installed. If none is provided, we will
    //      create and install a DTD validator. We install this on the
    //      scanner we create, which it will use to do validation. We set
    //      ourself on it as the error reporter for validity errors.
    //
    //  fWithinElement
    //      A flag to indicate that the parser is within at least one level
    //      of element processing.
    // -----------------------------------------------------------------------
    DOM_Node                fCurrentParent;
    DOM_Node                fCurrentNode;
    DOM_Document            fDocument;
    EntityResolver*         fEntityResolver;
    ErrorHandler*           fErrorHandler;
    bool                    fExpandEntityReferences;
    ValueStackOf<DOM_Node>* fNodeStack;
    bool                    fParseInProgress;
    XMLScanner*             fScanner;
    XMLValidator*           fValidator;
    bool                    fWithinElement;
};



// ---------------------------------------------------------------------------
//  DOMParser: Handlers for the XMLEntityHandler interface
// ---------------------------------------------------------------------------
inline void DOMParser::endInputSource(const InputSource&)
{
    // The DOM entity resolver doesn't handle this
}

inline bool DOMParser::expandSystemId(const XMLCh* const, XMLBuffer&)
{
    // The DOM entity resolver doesn't handle this
    return false;
}

inline void DOMParser::resetEntities()
{
    // Nothing to do on this one
}

inline void DOMParser::startInputSource(const InputSource&)
{
    // The DOM entity resolver doesn't handle this
}


// ---------------------------------------------------------------------------
//  DOMParser: Handlers for the XMLDocumentHandler interface
// ---------------------------------------------------------------------------
inline void DOMParser::endDocument()
{
    // Not used in DOM at this time
}



// ---------------------------------------------------------------------------
//  DOMParser: Getter methods
// ---------------------------------------------------------------------------
inline DOM_Document DOMParser::getDocument()
{
    return fDocument;
}

inline ErrorHandler* DOMParser::getErrorHandler()
{
    return fErrorHandler;
}

inline const ErrorHandler* DOMParser::getErrorHandler() const
{
    return fErrorHandler;
}

inline EntityResolver* DOMParser::getEntityResolver()
{
    return fEntityResolver;
}

inline const EntityResolver* DOMParser::getEntityResolver() const
{
    return fEntityResolver;
}

inline bool DOMParser::getExpandEntityReferences() const
{
    return fExpandEntityReferences;
}

inline const XMLScanner& DOMParser::getScanner() const
{
    return *fScanner;
}


// ---------------------------------------------------------------------------
//  DOMParser: Setter methods
// ---------------------------------------------------------------------------
inline void DOMParser::setExpandEntityReferences(const bool expand)
{
    fExpandEntityReferences = expand;
}


// ---------------------------------------------------------------------------
//  DOMParser: Protected getter methods
// ---------------------------------------------------------------------------
inline DOM_Node DOMParser::getCurrentNode()
{
    return fCurrentNode;
}


// ---------------------------------------------------------------------------
//  DOMParser: Protected setter methods
// ---------------------------------------------------------------------------
inline void DOMParser::setCurrentNode(DOM_Node toSet)
{
    fCurrentNode = toSet;
}

inline void DOMParser::setDocument(DOM_Document toSet)
{
    fDocument = toSet;
}

#endif
