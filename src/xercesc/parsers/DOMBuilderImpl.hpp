/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 2002 The Apache Software Foundation.  All rights
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
 * originally based on software copyright (c) 2001, International
 * Business Machines, Inc., http://www.ibm.com .  For more information
 * on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 */

/*
 * $Id$
 *
 */

#if !defined(DOMBUILDERIMPL_HPP)
#define DOMBUILDERIMPL_HPP


#include <xercesc/parsers/AbstractDOMParser.hpp>
#include <xercesc/dom/DOMBuilder.hpp>

 /**
  * DOMBuilderImpl provides an implementation of a DOMBuilder interface.
  * A DOMBuilder instance is obtained from the DOMImplementationLS interface
  * by invoking its createDOMBuilder method.
  */
class PARSERS_EXPORT DOMBuilderImpl : public AbstractDOMParser,
                                      public DOMBuilder
{
public :
    // -----------------------------------------------------------------------
    //  Constructors and Detructor
    // -----------------------------------------------------------------------

    /** @name Constructors and Destructor */
    //@{
    /** Construct a DOMBuilderImpl, with an optional validator
      *
      * Constructor with an instance of validator class to use for
      * validation. If you don't provide a validator, a default one will
      * be created for you in the scanner.
      *
      * @param valToAdopt Pointer to the validator instance to use. The
      *                   parser is responsible for freeing the memory.
      */
    DOMBuilderImpl(XMLValidator* const valToAdopt = 0);

    /**
      * Destructor
      */
    virtual ~DOMBuilderImpl();

    //@}


    // -----------------------------------------------------------------------
    //  Getter methods
    // -----------------------------------------------------------------------

    /** @name Getter methods */
    //@{

    /** Get a pointer to the error handler
      *
      * This method returns the installed error handler. If no handler
      * has been installed, then it will be a zero pointer.
      *
      * @return The pointer to the installed error handler object.
      */
    DOMErrorHandler* getErrorHandler();

    /** Get a const pointer to the error handler
      *
      * This method returns the installed error handler.  If no handler
      * has been installed, then it will be a zero pointer.
      *
      * @return A const pointer to the installed error handler object.
      */
    const DOMErrorHandler* getErrorHandler() const;

    /** Get a pointer to the entity resolver
      *
      * This method returns the installed entity resolver.  If no resolver
      * has been installed, then it will be a zero pointer.
      *
      * @return The pointer to the installed entity resolver object.
      */
    DOMEntityResolver* getEntityResolver();

    /** Get a const pointer to the entity resolver
      *
      * This method returns the installed entity resolver. If no resolver
      * has been installed, then it will be a zero pointer.
      *
      * @return A const pointer to the installed entity resolver object.
      */
    const DOMEntityResolver* getEntityResolver() const;

    /** Get a pointer to the application filter
      *
      * This method returns the installed application filter. If no filter
      * has been installed, then it will be a zero pointer.
      *
      * @return The pointer to the installed application filter.
      */
    DOMBuilderFilter* getFilter();

    /** Get a const pointer to the application filter
      *
      * This method returns the installed application filter. If no filter
      * has been installed, then it will be a zero pointer.
      *
      * @return A const pointer to the installed application filter
      */
    const DOMBuilderFilter* getFilter() const;

    //@}


    // -----------------------------------------------------------------------
    //  Setter methods
    // -----------------------------------------------------------------------

    /** @name Setter methods */
    //@{

    /** Set the error handler
      *
      * This method allows applications to install their own error handler
      * to trap error and warning messages.
      *
      * <i>Any previously set handler is merely dropped, since the parser
      * does not own them.</i>
      *
      * @param handler  A const pointer to the user supplied error
      *                 handler.
      *
      * @see #getErrorHandler
      */
    void setErrorHandler(DOMErrorHandler* const handler);

    /** Set the entity resolver
      *
      * This method allows applications to install their own entity
      * resolver. By installing an entity resolver, the applications
      * can trap and potentially redirect references to external
      * entities.
      *
      * <i>Any previously set resolver is merely dropped, since the parser
      * does not own them.</i>
      *
      * @param handler  A const pointer to the user supplied entity
      *                 resolver.
      *
      * @see #getEntityResolver
      */
    void setEntityResolver(DOMEntityResolver* const handler);

    /** Set the application filter
      *
      * When the application provides a filter, the parser will call out to
      * the filter at the completion of the construction of each Element node.
      * The filter implementation can choose to remove the element from the
      * document being constructed (unless the element is the document element)
      * or to terminate the parse early. If the document is being validated
      * when it's loaded the validation happens before the filter is called.
      *
      * <i>Any previously set filter is merely dropped, since the parser
      * does not own them.</i>
      *
      * @param filter  A const pointer to the user supplied application
      *                filter.
      *
      * @see #getFilter
      */
    void setFilter(DOMBuilderFilter* const filter);

    //@}


    // -----------------------------------------------------------------------
    //  Feature methods
    // -----------------------------------------------------------------------
    /** @name Feature methods */
    //@{

    /** Set the state of a feature
      *
      * It is possible for a DOMBuilder to recognize a feature name but to be
      * unable to set its value.
      *
      * @param name  The feature name.
      * @param state The requested state of the feature (true or false).
      * @exception DOMException
      *     NOT_SUPPORTED_ERR: Raised when the DOMBuilder recognizes the
      *     feature name but cannot set the requested value.
      *     <br>NOT_FOUND_ERR: Raised when the DOMBuilder does not recognize
      *     the feature name.
      *
      * @see #setFeature
      * @see #canSetFeature
      */
    void setFeature(const XMLCh* const name, const bool state);

    /** Look up the value of a feature.
      *
      * @param name The feature name.
      * @return The current state of the feature (true or false)
      * @exception DOMException
      *     NOT_FOUND_ERR: Raised when the DOMBuilder does not recognize
      *     the feature name. 
      *
      * @see #getFeature
      * @see #canSetFeature
      */
    bool getFeature(const XMLCh* const name);

    /** Query whether setting a feature to a specific value is supported.
      *
      * @param name  The feature name.
      * @param state The requested state of the feature (true or false).
      * @return <code>true</code> if the feature could be successfully set
      *     to the specified value, or <code>false</code> if the feature
      *     is not recognized or the requested value is not supported. The
      *     value of the feature itself is not changed.
      *
      * @see #getFeature
      * @see #setFeature
      */
    bool canSetFeature(const XMLCh* const name, const bool state);

    //@}

    // -----------------------------------------------------------------------
    //  Parsing methods
    // -----------------------------------------------------------------------
    /** @name Parsing methods */
    //@{

    /** Parse via an input source object
      *
      * This method invokes the parsing process on the XML file specified
      * by the DOMInputSource parameter. This API is borrowed from the
      * SAX Parser interface.
      *
      * @param source A const reference to the DOMInputSource object which
      *               points to the XML file to be parsed.
      * @param reuseGrammar The flag indicating whether the existing Grammar
      *                     should be reused or not for this parsing run.
      *                     If true, there cannot be any internal subset.
      * @return If the DOMBuilder is a synchronous DOMBuilder the newly created
      *         and populated Document is returned. If the DOMBuilder is
      *         asynchronous then <code>null</code> is returned since the
      *         document object is not yet parsed when this method returns.
      * @exception SAXException Any SAX exception, possibly
      *            wrapping another exception.
      * @exception XMLException An exception from the parser or client
      *            handler code.
      * @exception DOMException A DOM exception as per DOM spec.
      *
      * @see DOMInputSource#DOMInputSource
      * @see #setEntityResolver
      * @see #setErrorHandler
      */
    DOMDocument* parse(const DOMInputSource& source, const bool reuseGrammar = false);

    /** Parse via a file path or URL
      *
      * This method invokes the parsing process on the XML file specified by
      * the Unicode string parameter 'systemId'.
      *
      * @param systemId A const XMLCh pointer to the Unicode string which
      *                 contains the path to the XML file to be parsed.
      * @param reuseGrammar The flag indicating whether the existing Grammar
      *                     should be reused or not for this parsing run.
      *                     If true, there cannot be any internal subset.
      * @return If the DOMBuilder is a synchronous DOMBuilder the newly created
      *         and populated Document is returned. If the DOMBuilder is
      *         asynchronous then <code>null</code> is returned since the
      *         document object is not yet parsed when this method returns.
      * @exception SAXException Any SAX exception, possibly
      *            wrapping another exception.
      * @exception XMLException An exception from the parser or client
      *            handler code.
      * @exception DOM_DOMException A DOM exception as per DOM spec.
      *
      * @see #parse(DOMInputSource,...)
      */
    DOMDocument* parseURI(const XMLCh* const systemId, const bool reuseGrammar = false);

    /** Parse via a file path or URL (in the local code page)
      *
      * This method invokes the parsing process on the XML file specified by
      * the native char* string parameter 'systemId'.
      *
      * @param systemId A const char pointer to a native string which
      *                 contains the path to the XML file to be parsed.
      * @param reuseGrammar The flag indicating whether the existing Grammar
      *                     should be reused or not for this parsing run.
      *                     If true, there cannot be any internal subset.
      * @return If the DOMBuilder is a synchronous DOMBuilder the newly created
      *         and populated Document is returned. If the DOMBuilder is
      *         asynchronous then <code>null</code> is returned since the
      *         document object is not yet parsed when this method returns.
      * @exception SAXException Any SAX exception, possibly
      *            wrapping another exception.
      * @exception XMLException An exception from the parser or client
      *            handler code.
      * @exception DOM_DOMException A DOM exception as per DOM spec.
      *
      * @see #parse(DOMInputSource,...)
      */
    DOMDocument* parseURI(const char* const systemId, const bool reuseGrammar = false);

    /** Parse via an input source object
      *
      * This method invokes the parsing process on the XML file specified
      * by the DOMInputSource parameter, and inserts the content into an
      * existing document at the position specified with the contextNode
      * and action arguments. When parsing the input stream the context node
      * is used for resolving unbound namespace prefixes.
      *
      * @param source A const reference to the DOMInputSource object which
      *               points to the XML file to be parsed.
      * @param contextNode The node that is used as the context for the data
      *                    that is being parsed. This node must be a Document
      *                    node, a DocumentFragment node, or a node of a type
      *                    that is allowed as a child of an element, e.g. it
      *                    can not be an attribute node.
      * @param action This parameter describes which action should be taken
      *               between the new set of node being inserted and the
      *               existing children of the context node.
      * @exception DOMException
      *     NOT_SUPPORTED_ERR: Raised when the DOMBuilder doesn't support
      *     this method.
      *     <br>NO_MODIFICATION_ALLOWED_ERR: Raised if the context node is
      *     readonly.
      */
    virtual void parseWithContext
    (
        const   DOMInputSource& source
        ,       DOMNode* const contextNode
        , const short action
    );


    // -----------------------------------------------------------------------
    //  Implementation of the XMLErrorReporter interface.
    // -----------------------------------------------------------------------

    /** @name Implementation of the XMLErrorReporter interface. */
    //@{

    /** Handle errors reported from the parser
      *
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
      * @see DOMErrorHandler
      */
    virtual void error
    (
        const   unsigned int                errCode
        , const XMLCh* const                msgDomain
        , const XMLErrorReporter::ErrTypes  errType
        , const XMLCh* const                errorText
        , const XMLCh* const                systemId
        , const XMLCh* const                publicId
        , const XMLSSize_t                  lineNum
        , const XMLSSize_t                  colNum
    );

    /** Reset any error data before a new parse
     *
      * This method allows the user installed Error Handler callback to
      * 'reset' itself.
      *
      * <b><font color="#FF0000">This method is a no-op for this DOM
      * implementation.</font></b>
      */
    virtual void resetErrors();
    //@}


    // -----------------------------------------------------------------------
    //  Implementation of the XMLEntityHandler interface.
    // -----------------------------------------------------------------------

    /** @name Implementation of the XMLEntityHandler interface. */
    //@{

    /** Handle an end of input source event
      *
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

    /** Expand a system id
      *
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

    /** Reset any entity handler information
      *
      * This method allows the installed XMLEntityHandler to reset
      * itself.
      *
      * <b><font color="#FF0000">This method is a no-op for this DOM
      * implementation.</font></b>
      */
    virtual void resetEntities();

    /** Resolve a public/system id
      *
      * This method allows a user installed entity handler to further
      * process any pointers to external entities. The applications can
      * implement 'redirection' via this callback. This method is also
      * borrowed from the SAX specification.
      *
      * @param publicId A const pointer to a Unicode string representing the
      *                 public id of the entity just parsed.
      * @param systemId A const pointer to a Unicode string representing the
      *                 system id of the entity just parsed.
      * @return The value returned by the user installed resolveEntity
      *         method or NULL otherwise to indicate no processing was done.
      * @see DOMEntityResolver
      */
    virtual InputSource* resolveEntity
    (
        const   XMLCh* const    publicId
        , const XMLCh* const    systemId
        , const XMLCh* const    baseURI = 0
    );

    /** Handle a 'start input source' event
      *
      * This method is used to indicate the start of parsing an external
      * entity file.
      *
      * <b><font color="#FF0000">This method is a no-op for this DOM parse
      * implementation.</font></b>
      *
      * @param inputSource A const reference to the InputSource object
      *                    which points to the external entity
      *                    being parsed.
      */
    virtual void startInputSource(const InputSource& inputSource);

    //@}


private :
    // -----------------------------------------------------------------------
    //  Private data members
    //
    //  fEntityResolver
    //      The installed DOM entity resolver, if any. Null if none.
    //
    //  fErrorHandler
    //      The installed DOM error handler, if any. Null if none.
    //
    //  fFilter
    //      The installed application filter, if any. Null if none.
    //-----------------------------------------------------------------------
    bool                        fAutoValidation;
    bool                        fValidation;
    DOMEntityResolver*          fEntityResolver;
    DOMErrorHandler*            fErrorHandler;
    DOMBuilderFilter*           fFilter;
};



// ---------------------------------------------------------------------------
//  DOMBuilderImpl: Handlers for the XMLEntityHandler interface
// ---------------------------------------------------------------------------
inline void DOMBuilderImpl::endInputSource(const InputSource&)
{
    // The DOM entity resolver doesn't handle this
}

inline bool DOMBuilderImpl::expandSystemId(const XMLCh* const, XMLBuffer&)
{
    // The DOM entity resolver doesn't handle this
    return false;
}

inline void DOMBuilderImpl::resetEntities()
{
    // Nothing to do on this one
}

inline void DOMBuilderImpl::startInputSource(const InputSource&)
{
    // The DOM entity resolver doesn't handle this
}


// ---------------------------------------------------------------------------
//  DOMBuilderImpl: Getter methods
// ---------------------------------------------------------------------------
inline DOMErrorHandler* DOMBuilderImpl::getErrorHandler()
{
    return fErrorHandler;
}

inline const DOMErrorHandler* DOMBuilderImpl::getErrorHandler() const
{
    return fErrorHandler;
}

inline DOMEntityResolver* DOMBuilderImpl::getEntityResolver()
{
    return fEntityResolver;
}

inline const DOMEntityResolver* DOMBuilderImpl::getEntityResolver() const
{
    return fEntityResolver;
}

inline DOMBuilderFilter* DOMBuilderImpl::getFilter()
{
    return fFilter;
}

inline const DOMBuilderFilter* DOMBuilderImpl::getFilter() const
{
    return fFilter;
}


#endif
