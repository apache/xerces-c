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
 * Revision 1.3  2000/01/12 23:52:46  roddey
 * These are trivial changes required to get the C++ and Java versions
 * of error messages more into sync. Mostly it was where the Java version
 * was passing out one or more parameter than the C++ version was. In
 * some cases the change just required an extra parameter to get the
 * needed info to the place where the error was issued.
 *
 * Revision 1.2  2000/01/12 00:15:04  roddey
 * Changes to deal with multiply nested, relative pathed, entities and to deal
 * with the new URL class changes.
 *
 * Revision 1.1.1.1  1999/11/09 01:08:23  twl
 * Initial checkin
 *
 * Revision 1.4  1999/11/08 20:44:52  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */


#if !defined(XMLSCANNER_HPP)
#define XMLSCANNER_HPP

#include <util/KVStringPair.hpp>
#include <util/RefVectorOf.hpp>
#include <util/XMLString.hpp>
#include <framework/XMLAttr.hpp>
#include <framework/XMLBufferMgr.hpp>
#include <framework/XMLErrorCodes.hpp>
#include <framework/XMLRefInfo.hpp>
#include <framework/XMLPScanToken.hpp>
#include <internal/ElemStack.hpp>
#include <internal/ReaderMgr.hpp>

class InputSource;
class XMLDocumentHandler;
class XMLDocumentTypeHandler;
class XMLElementDecl;
class XMLEntityHandler;
class XMLErrorReporter;
class XMLMsgLoader;
class XMLValidator;


//
//  This is the mondo scanner class, which does the vast majority of the
//  work of parsing. It handles reading in input and spitting out events
//  to installed handlers.
//
class XMLPARSER_EXPORT XMLScanner
{
public :
    // -----------------------------------------------------------------------
    //  Public class types
    //
    //  NOTE: These should really be private, but some of the compilers we
    //  have to deal with are too stupid to understand this.
    //
    //  DeclTypes
    //      Used by scanXMLDecl() to know what type of decl it should scan.
    //      Text decls have slightly different rules from XMLDecls.
    //
    //  EntityExpRes
    //      These are the values returned from the entity expansion method,
    //      to indicate how it went.
    //
    //  XMLTokens
    //      These represent the possible types of input we can get while
    //      scanning content.
    // -----------------------------------------------------------------------
    enum DeclTypes
    {
        Decl_Text
        , Decl_XML
    };

    enum EntityExpRes
    {
        EntityExp_Pushed
        , EntityExp_Returned
        , EntityExp_Failed
    };

    enum XMLTokens 
    {
        Token_CData
        , Token_CharData
        , Token_Comment
        , Token_EndTag
        , Token_EOF
        , Token_PI
        , Token_StartTag
        , Token_Unknown
    };


    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    XMLScanner
    (
        XMLValidator* const validator
    );
    XMLScanner
    (
        XMLDocumentHandler* const   docHandler
        , XMLEntityHandler* const   entityHandler
        , XMLErrorReporter* const   errReporter
        , XMLValidator* const       validator
    );
    ~XMLScanner();


    // -----------------------------------------------------------------------
    //  Error emitter methods
    // -----------------------------------------------------------------------
    void emitError(const XML4CErrs::Codes toEmit);
    void emitError
    (
        const   XML4CErrs::Codes    toEmit
        , const XMLCh* const        text1
        , const XMLCh* const        text2 = 0
        , const XMLCh* const        text3 = 0
        , const XMLCh* const        text4 = 0
    );
    void emitError
    (
        const   XML4CErrs::Codes    toEmit
        , const char* const         text1
        , const char* const         text2 = 0
        , const char* const         text3 = 0
        , const char* const         text4 = 0
    );


    // -----------------------------------------------------------------------
    //  Getter methods
    // -----------------------------------------------------------------------
    const XMLDocumentHandler* getDocHandler() const;
    XMLDocumentHandler* getDocHandler();
    bool getDoNamespaces() const;
    bool getDoValidation() const;
    const XMLEntityHandler* getEntityHandler() const;
    XMLEntityHandler* getEntityHandler();
    const XMLErrorReporter* getErrorReporter() const;
    XMLErrorReporter* getErrorReporter();
    bool getExitOnFirstFatal() const;
    RefHashTableOf<XMLRefInfo>& getIDRefList();
    bool getInException() const;
    const RefHashTableOf<XMLRefInfo>& getIDRefList() const;
    bool getLastExtLocation
    (
                XMLCh* const    sysIdToFill
        , const unsigned int    maxSysIdChars
        ,       XMLCh* const    pubIdToFill
        , const unsigned int    maxPubIdChars
        ,       unsigned int&   lineToFill
        ,       unsigned int&   colToFill
    );
    bool getStandalone() const;
    const XMLValidator* getValidator() const;
    XMLValidator* getValidator();


    // -----------------------------------------------------------------------
    //  Setter methods
    // -----------------------------------------------------------------------
    void setDocHandler(XMLDocumentHandler* const docHandler);
    void setDoNamespaces(const bool doNamespaces);
    void setDoValidation(const bool validate);
    void setEntityHandler(XMLEntityHandler* const docTypeHandler);
    void setErrorReporter(XMLErrorReporter* const errHandler);
    void setExitOnFirstFatal(const bool newValue);
    void setValidator(XMLValidator* const validator);


    // -----------------------------------------------------------------------
    //  Document scanning methods
    //
    //  scanDocument() does the entire source document. scanFirst() and
    //  scanNext() support a progressive parse.
    // -----------------------------------------------------------------------
    void scanDocument
    (
        const   InputSource&    src
        , const bool            reuseValidator = false
    );
    void scanDocument
    (
        const   XMLCh* const    systemId
        , const bool            reuseValidator = false
    );
    void scanDocument
    (
        const   char* const     systemId
        , const bool            reuseValidator = false
    );

    bool scanFirst
    (
        const   InputSource&    src
        ,       XMLPScanToken&  toFill
        , const bool            reuseValidator = false
    );
    bool scanFirst
    (
        const   XMLCh* const    systemId
        ,       XMLPScanToken&  toFill
        , const bool            reuseValidator = false
    );
    bool scanFirst
    (
        const   char* const     systemId
        ,       XMLPScanToken&  toFill
        , const bool            reuseValidator = false
    );

    bool scanNext(XMLPScanToken& toFill);


private :
    // -----------------------------------------------------------------------
    //  Private class types
    // -----------------------------------------------------------------------
    enum IDTypes
    {
        IDType_Public
        , IDType_External
        , IDType_Either
    };

    enum DTDSubsets
    {
        Subset_Internal
        , Subset_External
    };


    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    XMLScanner();
    XMLScanner(const XMLScanner&);
    void operator=(const XMLScanner&);


    // -----------------------------------------------------------------------
    //  Private helper methods
    // -----------------------------------------------------------------------
    void commonInit();


    // -----------------------------------------------------------------------
    //  Private helper methods
    //
    //  These are implemented in XMLScanner2.cpp, to keep the main file from
    //  becoming so bloated. We can't have any bloated files.
    // -----------------------------------------------------------------------
    unsigned int buildAttList
    (
        const   RefVectorOf<KVStringPair>&  providedAttrs
        , const unsigned int                attCount
        ,       XMLElementDecl&             elemDecl
        ,       RefVectorOf<XMLAttr>&       toFill
    );
    void checkIDRefs();
    bool isLegalToken(const XMLPScanToken& toCheck);
    bool normalizeAttValue
    (
        const   XMLCh* const        attrName
        , const XMLCh* const        value
        , const XMLAttDef::AttTypes type
        ,       XMLBuffer&          toFill
    );
    unsigned int resolveQName
    (
        const   XMLCh* const        qName
        ,       XMLBuffer&          nameBufToFill
        ,       XMLBuffer&          prefixBufToFill
        , const ElemStack::MapModes mode
    );
    unsigned int resolvePrefix
    (
        const   XMLCh* const        prefix
        , const ElemStack::MapModes mode
    );
    unsigned int resolvePrefix
    (
        const   XMLCh* const        prefix
        ,       XMLBuffer&          uriBufToFill
        , const ElemStack::MapModes mode
    );
    void scanReset(const InputSource& src);
    void sendCharData(XMLBuffer& toSend);
    XMLTokens senseNextToken(unsigned int& orgReader);
    void updateNSMap
    (
        const   XMLCh* const    attrName
        , const XMLCh* const    attrValue
    );
    void validateAttrValue
    (
        const   XMLCh* const            valueText
        , const XMLAttDef::AttTypes     type
        , const XMLAttDef::DefAttTypes  defType
        , const XMLCh* const            defText
        , const XMLCh* const            fullName
        , const XMLCh* const            enumList
    );


    // -----------------------------------------------------------------------
    //  Private scanning methods
    // -----------------------------------------------------------------------
    bool basicAttrValueScan
    (
        const   XMLCh* const    attrName
        ,       XMLBuffer&      toFill
    );
    bool getQuotedString(XMLBuffer& toFill);
    unsigned int rawAttrScan
    (
        const   XMLCh* const                elemName
        ,       RefVectorOf<KVStringPair>&  toFill
        ,       bool&                       isEmpty
    );
    bool scanAttValue
    (
        const   XMLCh* const        attrName
        ,       XMLBuffer&          toFill
        , const XMLAttDef::AttTypes type
    );
    void scanCDSection();
    void scanCharData(XMLBuffer& toToUse);
    bool scanCharRef(XMLCh& toFill, XMLCh& second);
    void scanComment();
    bool scanContent(const bool extEntity);
    void scanDocTypeDecl();
    void scanEndTag(bool& gotData);
    EntityExpRes scanEntityRef
    (
        const   bool    inAttVal
        ,       XMLCh&  firstCh
        ,       XMLCh&  secondCh
        ,       bool&   escaped
    );
    bool scanEq();
    bool scanId
    (
                XMLBuffer&  pubIdToFill
        ,       XMLBuffer&  sysIdToFill
        , const IDTypes     whatKind
    );
    void scanIgnoredSection();
    bool scanInternalSubset();
    void scanMiscellaneous();
    void scanPI();
    void scanProlog();
    bool scanPublicLiteral(XMLBuffer& toFill);
    bool scanStartTag(bool& gotData);
    bool scanStartTagNS(bool& gotData);
    bool scanSystemLiteral(XMLBuffer& toFill);
    void scanXMLDecl(const DeclTypes type);
    unsigned int scanUpToWSOr
    (
                XMLBuffer&  toFill
        , const XMLCh       chEndChar
    );



    // -----------------------------------------------------------------------
    //  Data members
    //
    //  fAttrList
    //      Every time we get a new element start tag, we have to pass to
    //      the document handler the attributes found. To make it more
    //      efficient we keep this ref vector of XMLAttr objects around. We
    //      just reuse it over and over, allowing it to grow to meet the
    //      peek need.
    //
    //  fBaseDir
    //      This is the base directory, from which the initial XML file
    //      was loaded. It is set after the file is successfully opened,
    //      so we know it to be valid. If the initial file had no path
    //      component, then this is left null. It is used to handle relative
    //      paths of DTD and external entity system ids.
    //
    //  fBufMgr
    //      This is a manager for temporary buffers used during scanning.
    //      For efficiency we must use a set of static buffers, but we have
    //      to insure that they are not incorrectly reused. So this manager
    //      provides the smarts to hand out buffers as required.
    //
    //  fDocHandler
    //      The client code's document handler. If zero, then no document
    //      handler callouts are done. We don't adopt it.
    //
    //  fDoNamespaces
    //      This flag indicates whether the client code wants us to do
    //      namespaces or not. If the installed validator indicates that it
    //      has to do namespaces, then this is ignored.
    //
    //  fDoValidation
    //      Indicates whether any validation should be done. A validator
    //      can still be installed and it will build up all the data
    //      structures for the DTD/Schema declarations, but the scanner will
    //      not ask it to validate anything.
    //
    //  fElemStack
    //      This is the element stack that is used to track the elements that
    //      are currently being worked on.
    //
    //  fEntityHandler
    //      The client code's entity handler. If zero, then no entity handler
    //      callouts are done. We don't adopt it.
    //
    //  fErrorReporter
    //      The client code's error handler. If zero, then no error handler
    //      callouts are done. We don't adopt it.
    //
    //  fExitOnFirstFatal
    //      This indicates whether we bail out on the first fatal XML error
    //      or not. It defaults to true, which is the strict XML way, but it
    //      can be changed.
    //
    //  fIDRefList
    //      This is a list of XMLRefInfo objects. This member lets us do all
    //      needed ID-IDREF balancing checks.
    //
    //  fInException
    //      To avoid a circular freakout when we catch an exception and emit
    //      it, which would normally throw again if the 'fail on first error'
    //      flag is one.
    //
    //  fRawAttrList
    //      During the initial scan of the attributes we can only do a raw
    //      scan for key/value pairs. So this vector is used to store them
    //      until they can be processed (and put into fAttrList.)
    //
    //  fReaderMgr
    //      This is the reader manager, from which we get characters. It
    //      manages the reader stack for us, and provides a lot of convenience
    //      methods to do specialized checking for chars, sequences of chars,
    //      skipping chars, etc...
    //
    //  fReuseValidator
    //      This flag is set on a per-scan basis. So its provided in the
    //      scanDocument() and scanFirst() methods, and applies for that
    //      one pass. It indicates that the validator should not be reused
    //      and that any external structural description should be ignored.
    //      There cannot be any internal subset.
    //
    //  fScannerId
    //  fSequenceId
    //      These are used for progressive parsing, to make sure that the
    //      client code does the right thing at the right time.
    //
    //  fStandalone
    //      Indicates whether the document is standalone or not. Defaults to
    //      no, but can be overridden in the XMLDecl.
    //
    //  fValidator
    //      The installed validator. We look at them via the abstract
    //      validator interface, and don't know what it actual is.
    //
    //
    //  fAttName
    //  fAttValue
    //  fCDataBuf
    //  fNameBuf
    //  fQNameBuf
    //  fPrefixBuf
    //      For the most part, buffers are obtained from the fBufMgr object
    //      on the fly. However, for the start tag scan, we have a set of
    //      fixed buffers for performance reasons. These are used a lot and
    //      there are a number of them, so asking the buffer manager each
    //      time for new buffers is a bit too much overhead.
    // -----------------------------------------------------------------------
    RefVectorOf<XMLAttr>*       fAttrList;
    XMLCh*                      fBaseDir;
    XMLBufferMgr                fBufMgr;
    XMLDocumentHandler*         fDocHandler;
    bool                        fDoNamespaces;
    bool                        fDoValidation;
    ElemStack                   fElemStack;
    XMLEntityHandler*           fEntityHandler;
    XMLErrorReporter*           fErrorReporter;
    bool                        fExitOnFirstFatal;
    RefHashTableOf<XMLRefInfo>* fIDRefList;
    bool                        fInException;
    RefVectorOf<KVStringPair>*  fRawAttrList;
    ReaderMgr                   fReaderMgr;
    bool                        fReuseValidator;
    XMLUInt32                   fScannerId;
    XMLUInt32                   fSequenceId;
    bool                        fStandalone;
    XMLValidator*               fValidator;

    XMLBuffer                   fAttNameBuf;
    XMLBuffer                   fAttValueBuf;
    XMLBuffer                   fCDataBuf;
    XMLBuffer                   fNameBuf;
    XMLBuffer                   fQNameBuf;
    XMLBuffer                   fPrefixBuf;
    XMLBuffer                   fURIBuf;
};



// ---------------------------------------------------------------------------
//  XMLScanner: Getter methods
// ---------------------------------------------------------------------------
inline const XMLDocumentHandler* XMLScanner::getDocHandler() const
{
    return fDocHandler;
}

inline XMLDocumentHandler* XMLScanner::getDocHandler()
{
    return fDocHandler;
}

inline bool XMLScanner::getDoNamespaces() const
{
    return fDoNamespaces;
}

inline bool XMLScanner::getDoValidation() const
{
    return fDoValidation;
}

inline const XMLEntityHandler* XMLScanner::getEntityHandler() const
{
    return fEntityHandler;
}

inline XMLEntityHandler* XMLScanner::getEntityHandler()
{
    return fEntityHandler;
}

inline const XMLErrorReporter* XMLScanner::getErrorReporter() const
{
    return fErrorReporter;
}

inline XMLErrorReporter* XMLScanner::getErrorReporter()
{
    return fErrorReporter;
}

inline bool XMLScanner::getExitOnFirstFatal() const
{
    return fExitOnFirstFatal;
}

inline RefHashTableOf<XMLRefInfo>& XMLScanner::getIDRefList()
{
    return *fIDRefList;
}

inline bool XMLScanner::getInException() const
{
    return fInException;
}

inline const RefHashTableOf<XMLRefInfo>& XMLScanner::getIDRefList() const
{
    return *fIDRefList;
}

inline bool XMLScanner::getStandalone() const
{
    return fStandalone;
}

inline const XMLValidator* XMLScanner::getValidator() const
{
    return fValidator;
}

inline XMLValidator* XMLScanner::getValidator()
{
    return fValidator;
}


// ---------------------------------------------------------------------------
//  XMLScanner: Setter methods
// ---------------------------------------------------------------------------
inline void XMLScanner::setDoNamespaces(const bool doNamespaces)
{
    fDoNamespaces = doNamespaces;
}

inline void XMLScanner::setDocHandler(XMLDocumentHandler* const docHandler)
{
    fDocHandler = docHandler;
}

inline void XMLScanner::setErrorReporter(XMLErrorReporter* const errHandler)
{
    fErrorReporter = errHandler;
}

inline void XMLScanner::setEntityHandler(XMLEntityHandler* const entityHandler)
{
    fEntityHandler = entityHandler;
    fReaderMgr.setEntityHandler(entityHandler);
}

inline void XMLScanner::setExitOnFirstFatal(const bool newValue)
{
    fExitOnFirstFatal = newValue;
}

#endif
