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
  * Revision 1.1  1999/11/09 01:08:38  twl
  * Initial revision
  *
  * Revision 1.4  1999/11/08 20:44:41  rahul
  * Swat for adding in Product name and CVS comment log variable.
  *
  */


#if !defined(XMLVALIDATOR_HPP)
#define XMLVALIDATOR_HPP

#include <util/XML4CDefs.hpp>
#include <util/XMLEnumerator.hpp>
#include <util/RefHashTableOf.hpp>
#include <framework/XMLAttr.hpp>
#include <framework/XMLValidityCodes.hpp>
#include <framework/XMLRefInfo.hpp>

class ReaderMgr;
class XMLBuffer;
class XMLBufferMgr;
class XMLElementDecl;
class XMLEntityDecl;
class XMLEntityHandler;
class XMLErrorReporter;
class XMLNotationDecl;
class XMLMsgLoader;
class XMLScanner;


//
//  This abstract class provides the interface for all validators. This is
//  the simple amount of API that all validators must honor, in order for
//  the scanner to use them to do validation. All validators will actually
//  contain much more functionality than is accessible via this common API,
//  but that functionality requires that you know what type of validator you
//  are dealing with.
//
//  Basically, at this level, the primary concern is to be able to query
//  core information about elements and attributes. Adding decls to the
//  validator requires that you go through the derived interface because they
//  all have their own decl types. At this level, we can return information
//  via the base decl classes, from which each validator derives its own
//  decl classes.
//

class XMLPARSER_EXPORT XMLValidator
{
public:
    // -----------------------------------------------------------------------
    //  Class specific types
    // -----------------------------------------------------------------------
    enum Constants
    {
        Success     = -1
        , BadParent = -2
        , BadChild  = -3
    };


    enum LookupOpts
    {
        AddIfNotFound
        , FailIfNotFound
    };


    // -----------------------------------------------------------------------
    //  Constructors are hidden, just the virtual destructor is exposed
    // -----------------------------------------------------------------------
    virtual ~XMLValidator()
    {
    }


    // -----------------------------------------------------------------------
    //  Getter methods
    // -----------------------------------------------------------------------
    unsigned int getEmptyNamespaceId() const;
    unsigned int getGlobalNamespaceId() const;
    unsigned int getUnknownNamespaceId() const;
    unsigned int getXMLNamespaceId() const;
    unsigned int getXMLNSNamespaceId() const;


    // -----------------------------------------------------------------------
    //  Setter methods
    //
    //  setScannerInfo() is called by the scanner to tell the validator
    //  about the stuff it needs to have access to.
    // -----------------------------------------------------------------------
    void setScannerInfo
    (
        XMLScanner* const           owningScanner
        , ReaderMgr* const          readerMgr
        , XMLBufferMgr* const       bufMgr
    );

    void setErrorReporter
    (
        XMLErrorReporter* const errorReporter
    );


    // -----------------------------------------------------------------------
    //  The virtual validator interface
    // -----------------------------------------------------------------------
    virtual int addOrFindNSId
    (
        const   XMLCh* const    uriText
    ) = 0;

    virtual int checkContent
    (
        const   unsigned int    elemId
        , const unsigned int*   childIds
        , const unsigned int    childCount
    ) = 0;

    virtual bool checkRootElement
    (
        const   unsigned int    elemId
    ) = 0;

    virtual void faultInAttr
    (
                XMLAttr&    toFill
        , const XMLAttDef&  attDef
    )   const = 0;

    virtual const XMLElementDecl* findElemDecl
    (
        const   unsigned int    uriId
        , const XMLCh* const    baseName
        , const XMLCh* const    qName
        , const LookupOpts      options
        ,       bool&           wasAdded
    )   const = 0;

    virtual XMLElementDecl* findElemDecl
    (
        const   unsigned int    uriId
        , const XMLCh* const    baseName
        , const XMLCh* const    qName
        , const LookupOpts      options
        ,       bool&           wasAdded
    ) = 0;

    virtual const XMLEntityDecl* findEntityDecl
    (
        const   XMLCh* const    entName
        , const bool            isPE
    )   const = 0;

    virtual XMLEntityDecl* findEntityDecl
    (
        const   XMLCh* const    entName
        , const bool            isPE
    ) = 0;

    virtual unsigned int findElemId
    (
        const   unsigned int    uriId
        , const XMLCh* const    baseName
        , const XMLCh* const    qName
    )   const = 0;

    virtual const XMLNotationDecl* findNotationDecl
    (
        const   XMLCh* const    notName
    )   const = 0;

    virtual XMLNotationDecl* findNotationDecl
    (
        const   XMLCh* const    notName
    ) = 0;

    virtual unsigned int findNSId
    (
        const   XMLCh* const    nsName
    )   const = 0;

    virtual const XMLElementDecl* getElemDecl
    (
        const   unsigned int    elemId
    )   const = 0;

    virtual XMLElementDecl* getElemDecl
    (
        const   unsigned int    elemId
    ) = 0;

    virtual bool getURIText
    (
        const   unsigned int    uriId
        ,       XMLBuffer&      uriBufToFill
    )   const = 0;

    virtual void postParseValidation() = 0;

    virtual void reset() = 0;

    virtual bool requiresNamespaces() const = 0;

    virtual void validateAttrValue
    (
        const   XMLAttDef&                  attDef
        , const XMLCh* const                attrValue
    ) = 0;


    // -----------------------------------------------------------------------
    //  Virtual DTD handler interface. If handlesDTD() returns true, then
    //  scanDTD() will be called when a DOCTYPE is seen.
    // -----------------------------------------------------------------------
    virtual bool handlesDTD() const = 0;
    virtual void scanDTD(const bool reuseValidator) = 0;


    // -----------------------------------------------------------------------
    //  Error emitter methods
    // -----------------------------------------------------------------------
    void emitError(const XML4CValid::Codes toEmit);
    void emitError
    (
        const   XML4CValid::Codes   toEmit
        , const XMLCh* const        text1
        , const XMLCh* const        text2 = 0
        , const XMLCh* const        text3 = 0
        , const XMLCh* const        text4 = 0
    );
    void emitError
    (
        const   XML4CValid::Codes   toEmit
        , const char* const         text1
        , const char* const         text2 = 0
        , const char* const         text3 = 0
        , const char* const         text4 = 0
    );



protected :
    // -----------------------------------------------------------------------
    //  Hidden constructors
    // -----------------------------------------------------------------------
    XMLValidator
    (
        XMLErrorReporter* const errReporter = 0
    );


    // -----------------------------------------------------------------------
    //  Protected getters
    // -----------------------------------------------------------------------
    const XMLBufferMgr* getBufMgr() const;
    XMLBufferMgr* getBufMgr();
    const ReaderMgr* getReaderMgr() const;
    ReaderMgr* getReaderMgr();
    const XMLScanner* getScanner() const;
    XMLScanner* getScanner();


    // -----------------------------------------------------------------------
    //  Protected methods
    // -----------------------------------------------------------------------
    void setBaseFields
    (
        const   unsigned int    emptyNamespaceId
        , const unsigned int    globalNamespaceId
        , const unsigned int    unknownNamespaceId
        , const unsigned int    xmlNamespaceId
        , const unsigned int    xmlNSNamespaceId
    );


private :
    // -----------------------------------------------------------------------
    //  Unimplemented Constructors and Operators
    // -----------------------------------------------------------------------
    XMLValidator(const XMLValidator&);
    void operator=(const XMLValidator&);


    // -----------------------------------------------------------------------
    //  Private data members
    //
    //  fEmptyNamespaceId
    //      This is the id of the empty namespace URI. This is a special one
    //      because of the xmlns="" type of deal. We have to quickly sense
    //      that its the empty namespace.
    //
    //  fErrorReporter
    //      The error reporter we are to use, if any.
    //
    //  fGlobalNamespaceId
    //      This is the id of the namespace URI which is assigned to the
    //      global namespace. Its for debug purposes only, since there is no
    //      real global namespace URI. Its set by the derived class.
    //
    //  fUnknownNamespaceId
    //      This is the id of the namespace URI which is assigned to the
    //      global namespace. Its for debug purposes only, since there is no
    //      real global namespace URI. Its set by the derived class.
    //
    //  fXMLNamespaceId
    //  fXMLNSNamespaceId
    //      These are the ids of the namespace URIs which are assigned to the
    //      'xml' and 'xmlns' special prefixes. The former is officially
    //      defined but the latter is not, so we just provide one for debug
    //      purposes.
    // -----------------------------------------------------------------------
    XMLBufferMgr*       fBufMgr;
    unsigned int        fEmptyNamespaceId;
    XMLErrorReporter*   fErrorReporter;
    unsigned int        fGlobalNamespaceId;
    ReaderMgr*          fReaderMgr;
    XMLScanner*         fScanner;
    unsigned int        fUnknownNamespaceId;
    unsigned int        fXMLNamespaceId;
    unsigned int        fXMLNSNamespaceId;

};


// ---------------------------------------------------------------------------
//  XMLValidator: Getter methods
// ---------------------------------------------------------------------------
inline unsigned int XMLValidator::getEmptyNamespaceId() const
{
    return fEmptyNamespaceId;
}

inline unsigned int XMLValidator::getGlobalNamespaceId() const
{
    return fGlobalNamespaceId;
}

inline unsigned int XMLValidator::getUnknownNamespaceId() const
{
    return fUnknownNamespaceId;
}

inline unsigned int XMLValidator::getXMLNamespaceId() const
{
    return fXMLNamespaceId;
}

inline unsigned int XMLValidator::getXMLNSNamespaceId() const
{
    return fXMLNSNamespaceId;
}


// -----------------------------------------------------------------------
//  Setter methods
// -----------------------------------------------------------------------
inline void
XMLValidator::setScannerInfo(XMLScanner* const      owningScanner
                            , ReaderMgr* const      readerMgr
                            , XMLBufferMgr* const   bufMgr)
{
    // We don't own any of these, we just reference them
    fScanner = owningScanner;
    fReaderMgr = readerMgr;
    fBufMgr = bufMgr;
}

inline void
XMLValidator::setErrorReporter(XMLErrorReporter* const errorReporter)
{
    fErrorReporter = errorReporter;
}


// ---------------------------------------------------------------------------
//  XMLValidator: Protected getter
// ---------------------------------------------------------------------------
inline const XMLBufferMgr* XMLValidator::getBufMgr() const
{
    return fBufMgr;
}

inline XMLBufferMgr* XMLValidator::getBufMgr()
{
    return fBufMgr;
}

inline const ReaderMgr* XMLValidator::getReaderMgr() const
{
    return fReaderMgr;
}

inline ReaderMgr* XMLValidator::getReaderMgr()
{
    return fReaderMgr;
}

inline const XMLScanner* XMLValidator::getScanner() const
{
    return fScanner;
}

inline XMLScanner* XMLValidator::getScanner()
{
    return fScanner;
}


// ---------------------------------------------------------------------------
//  XMLValidator: Protected methods
// ---------------------------------------------------------------------------
inline void
XMLValidator::setBaseFields(const   unsigned int    emptyNamespaceId
                            , const unsigned int    globalNamespaceId
                            , const unsigned int    unknownNamespaceId
                            , const unsigned int    xmlNamespaceId
                            , const unsigned int    xmlNSNamespaceId)
{
    fEmptyNamespaceId   = emptyNamespaceId;
    fGlobalNamespaceId  = globalNamespaceId;
    fUnknownNamespaceId = unknownNamespaceId;
    fXMLNamespaceId     = xmlNamespaceId;
    fXMLNSNamespaceId   = xmlNSNamespaceId;
}

#endif
