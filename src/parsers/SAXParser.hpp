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

/**
 * $Log$
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
 * <p>This is the Simple API for XML (SAX) 1.0 driver for IBM XML4C. It
 *  implements the SAX interface and allows the client program to install
 *  SAX handlers for event callback. This class implements the internal
 *  event interfaces, whose event callbacks it maps to SAX event callbacks.</p>
 *
 *  This class can be validating or non-validating, by setting a member
 *  flag.
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
    SAXParser(XMLValidator* const valToAdopt = 0);
    ~SAXParser();


    // -----------------------------------------------------------------------
    //  Getter methods
    // -----------------------------------------------------------------------
    DocumentHandler* getDocumentHandler();
    const DocumentHandler* getDocumentHandler() const;
    EntityResolver* getEntityResolver();
    const EntityResolver* getEntityResolver() const;
    ErrorHandler* getErrorHandler();
    const ErrorHandler* getErrorHandler() const;
    const XMLScanner& getScanner() const;
    const XMLValidator& getValidator() const;
    bool getDoNamespaces() const;


    // -----------------------------------------------------------------------
    //  Setter methods
    // -----------------------------------------------------------------------
    void setDoNamespaces(const bool newState);
    void setDoValidation(const bool newState);
    void setExitOnFirstFatalError(const bool newState);


    // -----------------------------------------------------------------------
    //  Advanced document handler list maintenance methods
    // -----------------------------------------------------------------------
    void installAdvDocHandler(XMLDocumentHandler* const toInstall);
    bool removeAdvDocHandler(XMLDocumentHandler* const toRemove);


    // -----------------------------------------------------------------------
    //  Implementation of the SAXParser interface
    // -----------------------------------------------------------------------
    virtual void parse(const InputSource& source, const bool reuseValidator = false);
    virtual void parse(const XMLCh* const systemId, const bool reuseValidator = false);
    virtual void parse(const char* const systemId, const bool reuseValidator = false);
    virtual void setDocumentHandler(DocumentHandler* const handler);
    virtual void setDTDHandler(DTDHandler* const handler);
    virtual void setErrorHandler(ErrorHandler* const handler);
    virtual void setEntityResolver(EntityResolver* const resolver);


    // -----------------------------------------------------------------------
    //  Progressive scan methods
    // -----------------------------------------------------------------------
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
    //  Implementation of the DocTypeHandler
    // -----------------------------------------------------------------------
    virtual void attDef
    (
        const   DTDElementDecl& elemDecl
        , const DTDAttDef&      attDef
        , const bool            ignoring
    );

    virtual void doctypeComment
    (
        const   XMLCh* const    comment
    );

    virtual void doctypeDecl
    (
        const   DTDElementDecl& elemDecl
        , const XMLCh* const    publicId
        , const XMLCh* const    systemId
        , const bool            hasIntSubset
    );

    virtual void doctypePI
    (
        const   XMLCh* const    target
        , const XMLCh* const    data
    );

    virtual void doctypeWhitespace
    (
        const   XMLCh* const    chars
        , const unsigned int    length
    );

    virtual void elementDecl
    (
        const   DTDElementDecl& decl
        , const bool            isIgnored
    );

    virtual void endAttList
    (
        const   DTDElementDecl& elemDecl
    );

    virtual void endIntSubset();

    virtual void endExtSubset();

    virtual void entityDecl
    (
        const   DTDEntityDecl&  entityDecl
        , const bool            isPEDecl
        , const bool            isIgnored
    );

    virtual void resetDocType();

    virtual void notationDecl
    (
        const   XMLNotationDecl&    notDecl
        , const bool                isIgnored
    );

    virtual void startAttList
    (
        const   DTDElementDecl& elemDecl
    );

    virtual void startIntSubset();

    virtual void startExtSubset();

    virtual void TextDecl
    (
        const   XMLCh* const    versionStr
        , const XMLCh* const    encodingStr
    );


    // -----------------------------------------------------------------------
    //  Implementation of the XMLDocumentHandler interface
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
        , const XMLCh* const    actualEncodingStr
    );


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
