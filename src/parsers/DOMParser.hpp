/*
 * The Apache Software License, Version 1.1
 * 
 * Copyright (c) 1999 The Apache Software Foundation.  All rights 
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

/**
 * $Log$
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
 *  <p>This configuration of IBM XML4C builds a DOM for the input file.
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
    DOMParser(XMLValidator* const valToAdopt = 0);
    ~DOMParser();
    void reset();


    // -----------------------------------------------------------------------
    //  Getter methods
    // -----------------------------------------------------------------------
    DOM_Document getDocument();
    ErrorHandler* getErrorHandler();
    const ErrorHandler* getErrorHandler() const;
    EntityResolver* getEntityResolver();
    const EntityResolver* getEntityResolver() const;
    bool getExpandEntityReferences() const;
    const XMLScanner& getScanner() const;
    const XMLValidator& getValidator() const;


    // -----------------------------------------------------------------------
    //  Setter methods
    // -----------------------------------------------------------------------
    void setDoNamespaces(const bool newState);
    void setDoValidation(const bool newState);
    void setExpandEntityReferences(const bool expand);
    void setErrorHandler(ErrorHandler* const handler);
    void setEntityResolver(EntityResolver* const handler);
    void setExitOnFirstFatalError(const bool newState);


    // -----------------------------------------------------------------------
    //  Parsing methods
    // -----------------------------------------------------------------------
    void parse(const InputSource& source, const bool reuseValidator = false);
    void parse(const XMLCh* const systemId, const bool reuseValidator = false);
    void parse(const char* const systemId, const bool reuseValidator = false);
    bool parseFirst
    (
        const   XMLCh* const    systemId
        ,       XMLPScanToken&  toFill
        , const bool            reuseValidator = false
    );
    bool parseFirst
    (
        const   char* const     systemId
        ,       XMLPScanToken&  toFill
        , const bool            reuseValidator = false
    );
    bool parseFirst
    (
        const   InputSource&    source
        ,       XMLPScanToken&  toFill
        , const bool            reuseValidator = false
    );
    bool parseNext(XMLPScanToken& token);



    // -----------------------------------------------------------------------
    //  Implementation of the XMLErrorReporter interface.
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
    //  Implementation of the XMLEntityHandler interface.
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



    // -----------------------------------------------------------------------
    //  Implementation of the XMLDocumentHandler interface.
    // -----------------------------------------------------------------------
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
        , const XMLCh* const    actualEncStr
    );


protected :
    // -----------------------------------------------------------------------
    //  Protected getter methods
    // -----------------------------------------------------------------------
    DOM_Node getCurrentNode();


    // -----------------------------------------------------------------------
    //  Protected setter methods
    // -----------------------------------------------------------------------
    void setCurrentNode(DOM_Node toSet);
    void setDocument(DOM_Document toSet);


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
    // The SAX entity resolver doesn't handle this
}

inline bool DOMParser::expandSystemId(const XMLCh* const, XMLBuffer&)
{
    // The SAX entity resolver doesn't handle this
    return false;
}

inline void DOMParser::resetEntities()
{
    // Nothing to do on this one
}

inline void DOMParser::startInputSource(const InputSource&)
{
    // The SAX entity resolver doesn't handle this
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
