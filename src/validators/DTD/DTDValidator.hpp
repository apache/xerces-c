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
 * Revision 1.7  2001/02/26 19:22:08  tng
 * Schema: add parameter prefix in findElem and findAttr.
 *
 * Revision 1.6  2000/08/09 22:08:48  jpolast
 * added const XMLCh* getURIText() as per XMLValidator.
 * allows parsers to use const URIs instead of appending
 * to a XMLBuffer.
 *
 * Revision 1.5  2000/04/06 19:00:07  roddey
 * Added a getter for the doc type handler.
 *
 * Revision 1.4  2000/02/24 20:16:49  abagchi
 * Swat for removing Log from API docs
 *
 * Revision 1.3  2000/02/09 21:42:38  abagchi
 * Copyright swat
 *
 * Revision 1.2  2000/01/12 23:52:48  roddey
 * These are trivial changes required to get the C++ and Java versions
 * of error messages more into sync. Mostly it was where the Java version
 * was passing out one or more parameter than the C++ version was. In
 * some cases the change just required an extra parameter to get the
 * needed info to the place where the error was issued.
 *
 * Revision 1.1.1.1  1999/11/09 01:03:36  twl
 * Initial checkin
 *
 * Revision 1.5  1999/11/08 20:45:41  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */



#if !defined(DTDVALIDATOR_HPP)
#define DTDVALIDATOR_HPP

#include <util/RefHashTableOf.hpp>
#include <util/NameIdPool.hpp>
#include <util/StringPool.hpp>
#include <framework/XMLValidator.hpp>
#include <validators/DTD/DTDElementDecl.hpp>
#include <validators/DTD/DTDEntityDecl.hpp>

class ContentSpecNode;
class DocTypeHandler;
class XMLMsgLoader;


//
//  This is a derivative of the abstract validator interface. This class
//  implements a validator that supports standard XML 1.0 DTD semantics.
//  This class handles scanning the internal and external subsets of the
//  DTD, and provides the standard validation services against the DTD info
//  it found.
//
//  NOTE: DTDs are not namespace aware, so we just use regular NameIdPool
//  data structures to store element and attribute decls. They are all set
//  to be in the global namespace and the full QName is used as the base name
//  of the decl. This means that all the URI parameters below are expected
//  to be null pointers (and anything else will cause an exception.)
//
class VALIDATORS_EXPORT DTDValidator : public XMLValidator
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
    DTDValidator(XMLErrorReporter* const errReporter = 0);
    virtual ~DTDValidator();


    // -----------------------------------------------------------------------
    //  Getter methods
    // -----------------------------------------------------------------------
    NameIdPoolEnumerator<DTDElementDecl> getElemEnumerator() const;
    NameIdPoolEnumerator<DTDEntityDecl> getEntityEnumerator() const;
    NameIdPoolEnumerator<XMLNotationDecl> getNotationEnumerator() const;
    unsigned int getRootElemId() const;
    DocTypeHandler* getDocTypeHandler();
    const DocTypeHandler* getDocTypeHandler() const;


    // -----------------------------------------------------------------------
    //  Setter methods
    // -----------------------------------------------------------------------
    void setDocTypeHandler
    (
            DocTypeHandler* const handlerToSet
    );


    // -----------------------------------------------------------------------
    //  Implementation of the XMLValidator interface
    // -----------------------------------------------------------------------
    virtual int addOrFindNSId
    (
        const   XMLCh* const    uriText
    );

    virtual int checkContent
    (
        const   unsigned int    elemId
        , const unsigned int*   childIds
        , const unsigned int    childCount
    );

    virtual bool checkRootElement
    (
        const   unsigned int    elemId
    );

    virtual void faultInAttr
    (
                XMLAttr&    toFill
        , const XMLAttDef&  attDef
    )   const;

    virtual const XMLElementDecl* findElemDecl
    (
        const   unsigned int    uriId
        , const XMLCh* const    baseName
        , const XMLCh* const    prefix
        , const XMLCh* const    qName
        , const LookupOpts      options
        ,       bool&           wasAdded
    )   const;

    virtual XMLElementDecl* findElemDecl
    (
        const   unsigned int    uriId
        , const XMLCh* const    baseName
        , const XMLCh* const    prefix
        , const XMLCh* const    qName
        , const LookupOpts      options
        ,       bool&           wasAdded
    );

    virtual const XMLEntityDecl* findEntityDecl
    (
        const   XMLCh* const    entName
        , const bool            isPE
    )   const;

    virtual XMLEntityDecl* findEntityDecl
    (
        const   XMLCh* const    entName
        , const bool            isPE
    );

    virtual unsigned int findElemId
    (
        const   unsigned int    uriId
        , const XMLCh* const    baseName
        , const XMLCh* const    prefix
        , const XMLCh* const    qName
    )   const;

    virtual const XMLNotationDecl* findNotationDecl
    (
        const   XMLCh* const    notName
    )   const;

    virtual XMLNotationDecl* findNotationDecl
    (
        const   XMLCh* const    notName
    );

    virtual unsigned int findNSId
    (
        const   XMLCh* const    nsName
    )   const;

    virtual const XMLElementDecl* getElemDecl
    (
        const   unsigned int    elemId
    )   const;

    virtual XMLElementDecl* getElemDecl
    (
        const   unsigned int    elemId
    );

    virtual bool getURIText
    (
        const   unsigned int    uriId
        ,       XMLBuffer&      uriBufToFill
    )   const;

	virtual const XMLCh* getURIText
	(
	    const   unsigned int    uriId
    ) const;


    virtual void postParseValidation();

    virtual void reset();

    virtual bool requiresNamespaces() const;

    virtual void validateAttrValue
    (
        const   XMLAttDef&                  attDef
        , const XMLCh* const                attrValue
    );


    // -----------------------------------------------------------------------
    //  DTD specific pool methods. Many of the virtual interface methods
    //  above just call this one, passing along the qName (which is all that
    //  we deal with in this validator.
    // -----------------------------------------------------------------------
    unsigned int findElemId
    (
        const   XMLCh* const    qName
    )   const;



    // -----------------------------------------------------------------------
    //  Virtual DTD handler interface. If handlesDTD() returns true, then
    //  scanDocTypeDecl() will be called when a DOCTYPE is seen. The passed
    //  message loader and reader manager are really internal stuff but they
    //  are required to process the DTD external and internal subset.
    // -----------------------------------------------------------------------
    virtual bool handlesDTD() const;
    virtual void scanDTD(const bool reuseValidator);


private:
    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    DTDValidator(const DTDValidator&);
    void operator=(const DTDValidator&);


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
    void scanDocTypeDecl(const bool reuseValidator);
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
    //  fElemDeclPool
    //      This is the element decl pool. It contains all of the elements
    //      declared in the DTD (and their associated attributes.) When in
    //      non-validating mode, its just populated as new elements are seen
    //      and they are given default characteristics.
    //
    //  fEntityDeclPool
    //      This is a pool of EntityDecl objects, which contains all of the
    //      general entities that are declared in the DTD subsets.
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
    //  fNotationDeclPool
    //      This is a pool of NotationDecl objects, which contains all of the
    //      notations declared in the DTD subsets.
    //
    //  fPEntityDeclPool
    //      This is a pool of EntityDecl objects, which contains all of the
    //      parameter entities that are declared in the DTD subsets.
    //
    //  fRootElemId
    //      The id of the root element that we found in the DOCTYPE statement.
    //      Its initialized to ContentModel::fgInvalidElemId, so that its
    //      invalid unless we have a DOCTYPE.
    //
    //  fURIStringPool
    //      Each validator is required to maintain a pool for URIs. It has
    //      to be able to assign unique ids for URIs. We use a standard string
    //      pool class.
    // -----------------------------------------------------------------------
    DocTypeHandler*                 fDocTypeHandler;
    DTDAttDef*                      fDumAttDef;
    DTDElementDecl*                 fDumElemDecl;
    DTDEntityDecl*                  fDumEntityDecl;
    NameIdPool<DTDElementDecl>*     fElemDeclPool;
    NameIdPool<DTDEntityDecl>*      fEntityDeclPool;
    bool                            fInternalSubset;
    unsigned int                    fNextAttrId;
    NameIdPool<XMLNotationDecl>*    fNotationDeclPool;
    NameIdPool<DTDEntityDecl>*      fPEntityDeclPool;
    unsigned int                    fRootElemId;
    XMLStringPool                   fURIStringPool;
};


// ---------------------------------------------------------------------------
//  DTDValidator: Getter methods
// ---------------------------------------------------------------------------
inline NameIdPoolEnumerator<DTDElementDecl>
DTDValidator::getElemEnumerator() const
{
    return NameIdPoolEnumerator<DTDElementDecl>(fElemDeclPool);
}

inline NameIdPoolEnumerator<DTDEntityDecl>
DTDValidator::getEntityEnumerator() const
{
    return NameIdPoolEnumerator<DTDEntityDecl>(fEntityDeclPool);
}

inline NameIdPoolEnumerator<XMLNotationDecl>
DTDValidator::getNotationEnumerator() const
{
    return NameIdPoolEnumerator<XMLNotationDecl>(fNotationDeclPool);
}

inline unsigned int DTDValidator::getRootElemId() const
{
    return fRootElemId;
}

inline DocTypeHandler* DTDValidator::getDocTypeHandler()
{
    return fDocTypeHandler;
}

inline const DocTypeHandler* DTDValidator::getDocTypeHandler() const
{
    return fDocTypeHandler;
}


// ---------------------------------------------------------------------------
//  DTDValidator: Setter methods
// ---------------------------------------------------------------------------
inline void DTDValidator::setDocTypeHandler(DocTypeHandler* const handlerToSet)
{
    fDocTypeHandler = handlerToSet;
}


// ---------------------------------------------------------------------------
//  DTDValidator: DTD handler interface
// ---------------------------------------------------------------------------
inline bool DTDValidator::handlesDTD() const
{
    // We definitely want to handle DTD scanning
    return true;
}

#endif
