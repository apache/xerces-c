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
 * Revision 1.5  2000/02/06 07:48:37  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.4  2000/01/24 20:38:56  roddey
 * Fixed some small bugs introduced in the recent move away from the util/* streams.
 *
 * Revision 1.3  2000/01/21 23:58:06  roddey
 * Initial move away from util streams was bad. Wide char APIs didn't allow enough
 * control to do canonical output, so changed to use std short char APIs.
 *
 * Revision 1.1  1999/11/09 01:02:23  twl
 * Initial revision
 *
 * Revision 1.3  1999/11/08 20:42:25  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include    <framework/XMLDocumentHandler.hpp>
#include    <framework/XMLErrorReporter.hpp>
#include    <validators/DTD/DocTypeHandler.hpp>


class TestParser :

    public XMLDocumentHandler, public XMLErrorReporter, public DocTypeHandler
{
public :
    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    TestParser();
    ~TestParser();


    // -----------------------------------------------------------------------
    //  Setter methods
    // -----------------------------------------------------------------------
    void setDoNamespaces(const bool state);
    void setScanner(XMLScanner* const state);
    void setOutputType(const OutputTypes outType);
    void setShowErrLoc(const bool state);
    void setShowIntDTD(const bool state);
    void setShowWarnings(const bool state);


    // -----------------------------------------------------------------------
    //  The XMLDocumentHandler interface
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
        , const unsigned int    uriId
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
        , const unsigned int            elemURIId
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
        , const XMLCh* const    autoEncStr
    );


    // -----------------------------------------------------------------------
    //  Implementation of the XMLErrorReporter interface
    // -----------------------------------------------------------------------
    virtual void error
    (
        const   unsigned int                errCode
        , const XMLCh* const                msgDomain
        , const XMLErrorReporter::ErrTypes  type
        , const XMLCh* const                text
        , const XMLCh* const                systemId
        , const XMLCh* const                publicId
        , const unsigned int                lineNum
        , const unsigned int                colNum
    );

    virtual void resetErrors();


    // -----------------------------------------------------------------------
    //  The document type handler virtual handler interface
    // -----------------------------------------------------------------------
    virtual void attDef
    (
        const   DTDElementDecl&     elemDecl
        , const DTDAttDef&          attDef
        , const bool                ignoring
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


private :
    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    TestParser(const TestParser&);
    void operator=(const TestParser&);


    // -----------------------------------------------------------------------
    //  Private helper methods
    // -----------------------------------------------------------------------
    void showChar(const char toShow, const bool doCan);
    void showChars
    (
        const   XMLCh* const    chars
        , const unsigned int    length
    );
    void showIds(const XMLCh* const publicId, const XMLCh* const systemId);
    void showString(const XMLCh* const toShow);


    // -----------------------------------------------------------------------
    //  Data members
    //
    //  fDoNamespaces
    //      Indicates whether the user wanted us to be namespace aware or
    //      not.
    //
    //  fInsideRoot
    //      This is set once the first start element event is seen, and cleared
    //      when the root element ends. Its used to support the canonical
    //      format used by the Bosak tests. Basically it allows us to supress
    //      the whitespace outside the internal subset but before the root
    //      element.
    //
    //  fInExtSubSet
    //      Indicates when we are in the external subset, so that we don't
    //      display that part.
    //
    //  fIntDTDOutput
    //      Indicates whether the data from the internal DTD subset is
    //      output or not (only used if fXMLOutput is true.)
    //
    //  fNestingLevel
    //      This is used to handle tabbing over nested elements. Each start
    //      element bumps it up, and each end element bumps it down.
    //
    //  fOutputType
    //      This flag controls the primary style of output used. It can
    //      be set to do James Clark type canonical output, Sun style
    //      canonical output, debug output, regular XML output, or none.
    //
    //  fScanner
    //      The scanner we created to do the scanning.
    //
    //  fShowErrLoc
    //      This flag turns on the special display mode that is used for
    //      negative test testing. It puts out a special, condensed display
    //      of error info that can be compared in subsequent runs to check
    //      for changes. If its turned on, it forces the output type to
    //      'none'.
    //
    //  fShowWarnings
    //      Indicates whether warning messages should be displayed or not.
    //
    //  fSurrogate
    //      Indicates that we got a surrogate char, so we have to wait for
    //      the next char before we can output it. Its zero when not.
    // -----------------------------------------------------------------------
    bool            fDoNamespaces;
    bool            fInExtSubset;
    bool            fInsideRoot;
    bool            fIntDTDOutput;
    unsigned int    fNestingLevel;
    OutputTypes     fOutputType;
    XMLScanner*     fScanner;
    bool            fShowErrLoc;
    bool            fShowWarnings;
    XMLCh           fSurrogate;
};


// ---------------------------------------------------------------------------
//  TestParser: Setter Methods
// ---------------------------------------------------------------------------
inline void TestParser::setDoNamespaces(const bool state)
{
    fDoNamespaces = state;
}

inline void TestParser::setScanner(XMLScanner* const ourScanner)
{
    fScanner = ourScanner;
}

inline void TestParser::setOutputType(const OutputTypes outType)
{
    fOutputType = outType;
}

inline void TestParser::setShowIntDTD(const bool state)
{
    fIntDTDOutput = state;
}

inline void TestParser::setShowWarnings(const bool state)
{
    fShowWarnings = state;
}

inline void TestParser::setShowErrLoc(const bool state)
{
    fShowErrLoc = state;
}
