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
 * Revision 1.2  2001/04/19 18:17:22  tng
 * Schema: SchemaValidator update, and use QName in Content Model
 *
 * Revision 1.1  2001/03/21 21:56:20  tng
 * Schema: Add Schema Grammar, Schema Validator, and split the DTDValidator into DTDValidator, DTDScanner, and DTDGrammar.
 *
 */



#if !defined(DTDSCANNER_HPP)
#define DTDSCANNER_HPP

#include <validators/DTD/DTDGrammar.hpp>
#include <validators/DTD/DTDEntityDecl.hpp>

/*
 * Default implementation of an XML DTD scanner.
 */
class DocTypeHandler;

class VALIDATORS_EXPORT DTDScanner
{
public:
    // -----------------------------------------------------------------------
    //  Class specific types
    //
    //  NOTE: This should really be private, but some of the compilers we
    //  have to support cannot understand that.
    //
    //  EntityExpRes
    //      Returned from scanEntityRef() to indicate how the expanded text
    //      was treated.
    // -----------------------------------------------------------------------
    enum EntityExpRes
    {
        EntityExp_Failed
        , EntityExp_Pushed
        , EntityExp_Returned
    };


    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    DTDScanner(DTDGrammar* dtdGrammar, NameIdPool<DTDEntityDecl>* entityDeclPool, DocTypeHandler* const    docTypeHandler);
    virtual ~DTDScanner();

    // -----------------------------------------------------------------------
    //  Getter methods
    // -----------------------------------------------------------------------
    DocTypeHandler* getDocTypeHandler();
    const DocTypeHandler* getDocTypeHandler() const;

    // -----------------------------------------------------------------------
    //  Setter methods
    //
    //  setScannerInfo() is called by the scanner to tell the DTDScanner
    //  about the stuff it needs to have access to.
    // -----------------------------------------------------------------------
    void setScannerInfo
    (
        XMLScanner* const           owningScanner
        , ReaderMgr* const          readerMgr
        , XMLBufferMgr* const       bufMgr
    );

    void setDocTypeHandler
    (
            DocTypeHandler* const handlerToSet
    );

    void scanDocTypeDecl(const bool reuseGrammar);

private:
    // -----------------------------------------------------------------------
    //  Private class types
    // -----------------------------------------------------------------------
    enum IDTypes
    {
        IDType_Public
        , IDType_External
        , IDType_Either
    };


    // -----------------------------------------------------------------------
    //  Private DTD scanning methods. These are all in XMLValidator2.cpp
    // -----------------------------------------------------------------------
    bool checkForPERef
    (
        const   bool    spaceRequired
        , const bool    inLiteral
        , const bool    inMarkup
        , const bool    throwEndOfExt = false
    );
    bool expandPERef
    (
        const   bool    scanExternal
        , const bool    inLiteral
        , const bool    inMarkup
        , const bool    throwEndOfExt = false
    );
    bool getQuotedString(XMLBuffer& toFill);
    XMLAttDef* scanAttDef(DTDElementDecl& elemDecl, XMLBuffer& bufToUse);
    bool scanAttValue
    (
        const   XMLCh* const        attrName
        ,       XMLBuffer&          toFill
        , const XMLAttDef::AttTypes type
    );
    void scanAttListDecl();
    ContentSpecNode* scanChildren
    (
        const   DTDElementDecl&     elemDecl
        ,       XMLBuffer&          bufToUse
    );
    bool scanCharRef(XMLCh& toFill, XMLCh& second);
    void scanComment();
    bool scanContentSpec(DTDElementDecl& toFill);
    void scanDefaultDecl(DTDAttDef& toFill);
    void scanElementDecl();
    void scanEntityDecl();
    bool scanEntityDef();
    bool scanEntityLiteral(XMLBuffer& toFill, const bool isPE);
    bool scanEntityDef(DTDEntityDecl& decl, const bool isPEDecl);
    EntityExpRes scanEntityRef(XMLCh& firstCh, XMLCh& secondCh, bool& escaped);
    bool scanEnumeration
    (
        const   DTDAttDef&  attDef
        ,       XMLBuffer&  toFill
        , const bool        notation
    );
    bool scanEq();
    void scanExtSubsetDecl(const bool inIncludeSect);
    bool scanId
    (
                XMLBuffer&  pubIdToFill
        ,       XMLBuffer&  sysIdToFill
        , const IDTypes     whatKind
    );
    void scanIgnoredSection();
    bool scanInternalSubset();
    void scanMarkupDecl(const bool parseTextDecl);
    bool scanMixed(DTDElementDecl& toFill);
    void scanNotationDecl();
    void scanPI();
    bool scanPublicLiteral(XMLBuffer& toFill);
    bool scanSystemLiteral(XMLBuffer& toFill);
    void scanTextDecl();


    // -----------------------------------------------------------------------
    //  Private data members
    //
    //  fDocTypeHandler
    //      This holds the optional doc type handler that can be installed
    //      and used to call back for all markup events. It is DTD specific.
    //
    //  fDumAttDef
    //  fDumElemDecl
    //  fDumEntityDecl
    //      These are dummy objects into which mark decls are parsed when
    //      they are just overrides of previously declared markup decls. In
    //      such situations, the first one wins but we need to have somewhere
    //      to parse them into. So these are lazily created and used as needed
    //      when such markup decls are seen.
    //
    //  fInternalSubset
    //      This is used to track whether we are in the internal subset or not,
    //      in which case we are in the external subset.
    //
    //  fNextAttrId
    //      Since att defs are per-element, we don't have a validator wide
    //      attribute def pool. So we use a simpler data structure in each
    //      element decl to store its att defs, and we use this simple counter
    //      to apply a unique id to each new attribute.
    //
    //  fDTDGrammar
    //      The DTD information we scanned like element decl, attribute decl
    //      are stored in this Grammar.
    //
    //  fBufMgr
    //      This is the buffer manager of the scanner. This is provided as a
    //      convenience so that the DTDScanner doesn't have to create its own
    //      buffer manager during the parse process.
    //
    //  fReaderMgr
    //      This is a pointer to the reader manager that is being used by the scanner.
    //
    //  fScanner
    //      The pointer to the scanner to which this DTDScanner belongs
    //
    //  fPEntityDeclPool
    //      This is a pool of EntityDecl objects, which contains all of the
    //      parameter entities that are declared in the DTD subsets.
    //
    //  fEntityDeclPool
    //      This is a pool of EntityDecl objects, which contains all of the
    //      general entities that are declared in the DTD subsets.  It is
    //      owned by the Scanner as Schema Grammar may also need access to
    //      this pool for entity reference.
    //
    //  fEmptyNamespaceId
    //      The uri for all DTD decls
    // -----------------------------------------------------------------------
    DocTypeHandler*                 fDocTypeHandler;
    DTDAttDef*                      fDumAttDef;
    DTDElementDecl*                 fDumElemDecl;
    DTDEntityDecl*                  fDumEntityDecl;
    bool                            fInternalSubset;
    unsigned int                    fNextAttrId;
    DTDGrammar*                     fDTDGrammar;
    XMLBufferMgr*                   fBufMgr;
    ReaderMgr*                      fReaderMgr;
    XMLScanner*                     fScanner;
    NameIdPool<DTDEntityDecl>*      fPEntityDeclPool;
    NameIdPool<DTDEntityDecl>*      fEntityDeclPool;
    unsigned int                    fEmptyNamespaceId;
};


// ---------------------------------------------------------------------------
//  DTDScanner: Getter methods
// ---------------------------------------------------------------------------
inline DocTypeHandler* DTDScanner::getDocTypeHandler()
{
    return fDocTypeHandler;
}

inline const DocTypeHandler* DTDScanner::getDocTypeHandler() const
{
    return fDocTypeHandler;
}


// ---------------------------------------------------------------------------
//  DTDScanner: Setter methods
// ---------------------------------------------------------------------------
inline void DTDScanner::setDocTypeHandler(DocTypeHandler* const handlerToSet)
{
    fDocTypeHandler = handlerToSet;
}

// -----------------------------------------------------------------------
//  Setter methods
// -----------------------------------------------------------------------
inline void DTDScanner::setScannerInfo(XMLScanner* const      owningScanner
                            , ReaderMgr* const      readerMgr
                            , XMLBufferMgr* const   bufMgr)
{
    // We don't own any of these, we just reference them
    fScanner = owningScanner;
    fReaderMgr = readerMgr;
    fBufMgr = bufMgr;

    if (fScanner->getDoNamespaces())
        fEmptyNamespaceId = fScanner->getEmptyNamespaceId();
    else
        fEmptyNamespaceId = 0;

}
#endif
