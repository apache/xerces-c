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
  * Revision 1.8  2000/12/14 18:49:58  tng
  * Fix API document generation warning: "Warning: end of member group without matching begin"
  *
  * Revision 1.7  2000/08/09 22:09:09  jpolast
  * added const XMLCh* getURIText()
  * allows parsers to use const URIs instead of appending
  * to a XMLBuffer.
  *
  * Revision 1.6  2000/03/02 19:54:25  roddey
  * This checkin includes many changes done while waiting for the
  * 1.1.0 code to be finished. I can't list them all here, but a list is
  * available elsewhere.
  *
  * Revision 1.5  2000/02/24 20:00:24  abagchi
  * Swat for removing Log from API docs
  *
  * Revision 1.4  2000/02/15 23:59:07  roddey
  * More updated documentation of Framework classes.
  *
  * Revision 1.3  2000/02/15 01:21:31  roddey
  * Some initial documentation improvements. More to come...
  *
  * Revision 1.2  2000/02/06 07:47:49  rahulj
  * Year 2K copyright swat.
  *
  * Revision 1.1.1.1  1999/11/09 01:08:38  twl
  * Initial checkin
  *
  * Revision 1.4  1999/11/08 20:44:41  rahul
  * Swat for adding in Product name and CVS comment log variable.
  *
  */


#if !defined(XMLVALIDATOR_HPP)
#define XMLVALIDATOR_HPP

#include <util/XercesDefs.hpp>
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


/**
 *  This abstract class provides the interface for all validators. This is
 *  the simple amount of API that all validators must honor, in order for
 *  the scanner to use them to do validation. All validators will actually
 *  contain much more functionality than is accessible via this common API,
 *  but that functionality requires that you know what type of validator you
 *  are dealing with.
 *
 *  Basically, at this level, the primary concern is to be able to query
 *  core information about elements and attributes. Adding decls to the
 *  validator requires that you go through the derived interface because they
 *  all have their own decl types. At this level, we can return information
 *  via the base decl classes, from which each validator derives its own
 *  decl classes.
 */
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

    /** @name Destructor */
    //@{

    /**
     *  The derived class should clean up its allocated data, then this class
     *  will do the same for data allocated at this level.
     */
    virtual ~XMLValidator()
    {
    }
    //@}


    // -----------------------------------------------------------------------
    //  Getter methods
    // -----------------------------------------------------------------------

    /** @name Getter Methods */
    //@{

    /**
      * When an attribute name has no prefix, unlike elements, it is not mapped
      * to the global namespace. So, in order to have something to map it to
      * for practical purposes, a id for an empty URL is created and used for
      * such names.
      *
      * @return The URL pool id of the URL for an empty URL "".
      */
    unsigned int getEmptyNamespaceId() const;

    /**
      * When namespaces are enabled, any elements whose names have no prefix
      * are mapped to a global namespace. This is the URL id for the URL
      * to which those names are mapped. It has no official standard text,
      * but the parser must use some id here.
      *
      * @return The URL pool id of the URL for the global namespace.
      */
    unsigned int getGlobalNamespaceId() const;

    /**
      * When a prefix is found that has not been mapped, an error is issued.
      * However, if the parser has been instructed not to stop on the first
      * fatal error, it needs to be able to continue. To do so, it will map
      * that prefix tot his magic unknown namespace id.
      *
      * @return The URL pool id of the URL for the unknown prefix
      *         namespace.
      */
    unsigned int getUnknownNamespaceId() const;

    /**
      * The prefix 'xml' is a magic prefix, defined by the XML spec and 
      * requiring no prior definition. This method returns the id for the
      * intrinsically defined URL for this prefix.
      *
      * @return The URL pool id of the URL for the 'xml' prefix.
      */
    unsigned int getXMLNamespaceId() const;

    /**
      * The prefix 'xmlns' is a magic prefix, defined by the namespace spec
      * and requiring no prior definition. This method returns the id for the
      * intrinsically defined URL for this prefix.
      *
      * @return The URL pool id of the URL for the 'xmlns' prefix.
      */
    unsigned int getXMLNSNamespaceId() const;

    //@}


    // -----------------------------------------------------------------------
    //  Setter methods
    //
    //  setScannerInfo() is called by the scanner to tell the validator
    //  about the stuff it needs to have access to.
    // -----------------------------------------------------------------------

    /** @name Setter methods */
    //@{

    /**
      * @param  owningScanner   This is a pointer to the scanner to which the
      *                         validator belongs. The validator will often
      *                         need to query state data from the scanner.
      *
      * @param  readerMgr       This is a pointer to the reader manager that is
      *                         being used by the scanner.
      *
      * @param  bufMgr          This is the buffer manager of the scanner. This
      *                         is provided as a convenience so that the validator
      *                         doesn't have to create its own buffer manager
      *                         during the parse process.
      */
    void setScannerInfo
    (
        XMLScanner* const           owningScanner
        , ReaderMgr* const          readerMgr
        , XMLBufferMgr* const       bufMgr
    );

    /**
      * This method is called to set an error reporter on the validator via
      * which it will report any errors it sees during parsing or validation.
      * This is generally called by the owning scanner.
      *
      * @param  errorReporter   A pointer to the error reporter to use. This
      *                         is not adopted, just referenced so the caller
      *                         remains responsible for its cleanup, if any.
      */
    void setErrorReporter
    (
        XMLErrorReporter* const errorReporter
    );

    //@}


    // -----------------------------------------------------------------------
    //  The virtual validator interface
    // -----------------------------------------------------------------------

    /** @name Virtual validator interface */
    //@{

    /**
      * The derived class should look for the passed URI (case sensitive) in
      * its URI pool. If the URI does not exist, then it should be added to the
      * pool. The new or existing pool id should be returned.
      */
    virtual int addOrFindNSId
    (
        const   XMLCh* const    uriText
    ) = 0;

    /**
      * The derived class should look up its declaration of the passed element
      * from its element pool. It should then use the content model description
      * contained in that element declaration to validate that the passed list
      * of child elements are valid for that content model. The count can be
      * zero, indicating no child elements.
      *
      * Note that whitespace and text content are not validated here. Those are
      * handled by the scanner. So only element ids are provided here.
      *
      * @param  elemId      The pool id of the element whose content is to be
      *                     checked.
      *
      * @param  childIds    An array of element ids which represent the elements
      *                     found within the parent element, i.e. the content
      *                     to be validated.
      *
      * @param  childCount  The number of elements in the childIds array. It can
      *                     be zero if the element had none.
      */
    virtual int checkContent
    (
        const   unsigned int    elemId
        , const unsigned int*   childIds
        , const unsigned int    childCount
    ) = 0;

    /**
      * For those validators that contrain the possible root elements of a
      * document to only particular elements, they should use this call to
      * validate that the passed root element id is a legal root element.
      */
    virtual bool checkRootElement
    (
        const   unsigned int    elemId
    ) = 0;

    /**
      * The derived class should fault in the passed XMLAttr value. It should
      * use the passeed attribute definition (which is passed via the base
      * type so it must often be downcast to the appropriate type for the
      * derived validator class), to fill in the passed attribute. This is done
      * as a performance enhancement since the derived class has more direct
      * access to the information.
      */
    virtual void faultInAttr
    (
                XMLAttr&    toFill
        , const XMLAttDef&  attDef
    )   const = 0;

    /**
      * The derived class should look up an element in its element declaration
      * pool and return a pointer to it. The name should be looked up either via
      * the qName field (if namespaces are off) or via the uriID/baseName combo
      * (if namespaces are on.)
      *
      * The options allow the caller to indicate how the validator should react
      * if the element exists or not. If the element does not exist, the option
      * can indicate that it should be faulted it. If it does not exist and
      * faulting in is not requested, a null pointer should be returned. The
      * wasAdded flag should be set if the declaration was faulted in, else it
      * should be cleared.
      */
    virtual const XMLElementDecl* findElemDecl
    (
        const   unsigned int    uriId
        , const XMLCh* const    baseName
        , const XMLCh* const    qName
        , const LookupOpts      options
        ,       bool&           wasAdded
    )   const = 0;

    /*
     *  This method is identical to the previous one, except that it is a non-
     *  const version.
     */
    virtual XMLElementDecl* findElemDecl
    (
        const   unsigned int    uriId
        , const XMLCh* const    baseName
        , const XMLCh* const    qName
        , const LookupOpts      options
        ,       bool&           wasAdded
    ) = 0;

    /**
      * The derived class should look the passed entity declaration name in its
      * entity pool and return a pointer to the declaration object. If the
      * entity is not in the pool, it should return a null pointer.
      *
      * @param  entName     The name of the entity to look up. Entity names can
      *                     not be namespace based, so its always just a single
      *                     unqualified name.
      *
      * @param  isPE        Indicates whether the name represents a parameter
      *                     entity or a general entity. The validator should
      *                     look at the correct pool for the entity type.
      */
    virtual const XMLEntityDecl* findEntityDecl
    (
        const   XMLCh* const    entName
        , const bool            isPE
    )   const = 0;

    /**
      * This method is identical to the previous one, except that it is non-
      * const.
      *
      * @param  entName     The name of the entity to look up. Entity names can
      *                     not be namespace based, so its always just a single
      *                     unqualified name.
      *
      * @param  isPE        Indicates whether the name represents a parameter
      *                     entity or a general entity. The validator should
      *                     look at the correct pool for the entity type.
      */
    virtual XMLEntityDecl* findEntityDecl
    (
        const   XMLCh* const    entName
        , const bool            isPE
    ) = 0;

    /**
      * The derived class should find the indicate element in its element
      * declaration pool. If namespaces are on, then it should use the uriId
      * and base name. Otherwise, it should use the qName. It should return
      * the element decl pool id for the decl found. If the element is not
      * not found, it should return the value XMLElementDecl::fgInvalidElemId;
      */
    virtual unsigned int findElemId
    (
        const   unsigned int    uriId
        , const XMLCh* const    baseName
        , const XMLCh* const    qName
    )   const = 0;

    /**
      * The derived class should look up the passed notation name in its
      * notation decl pool. If the name is not found, it should return a null
      * pointer. If it is found, it should return a pointer to the declaration
      * object.
      */
    virtual const XMLNotationDecl* findNotationDecl
    (
        const   XMLCh* const    notName
    )   const = 0;

    /**
      * This method is identical to the previous method except that it is non-
      * const.
      */
    virtual XMLNotationDecl* findNotationDecl
    (
        const   XMLCh* const    notName
    ) = 0;

    /**
      * The derived class should look up the passed namespace name (prefix) in
      * its namespace name pool and return the id. If not found, it should
      * return zero, which is never a valid pool id.
      */
    virtual unsigned int findNSId
    (
        const   XMLCh* const    nsName
    )   const = 0;

    /**
      * The derived class should return the element from its element decl pool
      * which has the indicated element id. Given that these ids are gotten from
      * the validator itself, it should always be valid. If not, then a major
      * internal error has occured.
      */
    virtual const XMLElementDecl* getElemDecl
    (
        const   unsigned int    elemId
    )   const = 0;

    /**
      * This method is identical to the previous method, except that it is a
      * non-const version.
      */
    virtual XMLElementDecl* getElemDecl
    (
        const   unsigned int    elemId
    ) = 0;

    /**
      * The derived class should find the passed URI id in its URI pool and
      * copy the text of that URI into the passed buffer. Since these ids are
      * obtained from the validator itself, they should always be valid. If
      * not, then a major internal error has occured.
      */
    virtual bool getURIText
    (
        const   unsigned int    uriId
        ,       XMLBuffer&      uriBufToFill
    )   const = 0;

	virtual const XMLCh* getURIText
	(   
	    const   unsigned int    uriId
    )   const = 0;

    /**
      * This method is called by the scanner after the parse has completed. It
      * gives the validator a chance to check certain things that can only be
      * checked after the whole document has been parsed, such as referential
      * integrity of ID/IDREF pairs and so forth. The validator should just
      * issue errors for any problems it finds.
      */
    virtual void postParseValidation() = 0;

    /**
      * This method is called by the scanner before a new document is about
      * to start. It gives the validator a change to reset itself in preperation
      * for another validation pass.
      */
    virtual void reset() = 0;

    /**
      * The derived class should return a boolean that indicates whether it
      * requires namespace processing or not. Some do and some allow it to be
      * optional. This flag is used to control whether the client code's
      * requests to disable namespace processing can be honored or not.
      */
    virtual bool requiresNamespaces() const = 0;

    /**
      * The derived class should apply any rules to the passed attribute value
      * that are above and beyond those defined by XML 1.0. The scanner itself
      * will impose XML 1.0 rules, based on the type of the attribute. This
      * will generally be used to check things such as range checks and other
      * datatype related validation.
      *
      * If the value breaks any rules as defined by the derived class, it
      * should just issue errors as usual.
      */
    virtual void validateAttrValue
    (
        const   XMLAttDef&                  attDef
        , const XMLCh* const                attrValue
    ) = 0;

    //@}

    // -----------------------------------------------------------------------
    //  Virtual DTD handler interface. If handlesDTD() returns true, then
    //  scanDTD() will be called when a DOCTYPE is seen.
    // -----------------------------------------------------------------------

    /** @name Virtual DTD handler interface */
    //@{

    /**
      * This method allows the scanner to ask the validator if it handles
      * DTDs or not.
      */
    virtual bool handlesDTD() const = 0;

    /**
      * This method is called by the scanner when it is time for the validator
      * to parse the DTD. At the time of this call, the parser has parsed up to
      * the [ of the DOCTYPE line. This call should parse to the ]> that ends
      * the DOCTYPE line, and any external subset referenced.
      *
      * @param  reuseValidator  Indicates whether the current state of the
      *                         validator should be kept. If the DTD validator
      *                         sees any internal subset, it should consider
      *                         it an error for this to be true. Otherwise, it
      *                         just ignores any external subset and returns
      *                         with the state unchanged.
      */
    virtual void scanDTD(const bool reuseValidator) = 0;

    //@}


    // -----------------------------------------------------------------------
    //  Error emitter methods
    // -----------------------------------------------------------------------

    /** @name Error emittor methods */
    //@{

    /**
     *  This call is a convenience by which validators can emit errors. Most
     *  of the grunt work of loading the text, getting the current source
     *  location, ect... is handled here.
     *
     *  If the loaded text has replacement parameters, then text strings can be
     *  passed. These will be used to replace the tokens {0}, {1}, {2}, and {3}
     *  in the order passed. So text1 will replace {0}, text2 will replace {1},
     *  and so forth.
     *
     *  @param toEmit   The error code to emit. it must be one of the defined
     *                  validator error codes.
     *
     *  @param  textX   Up to four replacement parameters. They can be provided
     *                  as either XMLCh strings, or local code page strings which
     *                  will be transcoded internally.
     */
    void emitError(const XMLValid::Codes toEmit);
    void emitError
    (
        const   XMLValid::Codes toEmit
        , const XMLCh* const    text1
        , const XMLCh* const    text2 = 0
        , const XMLCh* const    text3 = 0
        , const XMLCh* const    text4 = 0
    );
    void emitError
    (
        const   XMLValid::Codes toEmit
        , const char* const     text1
        , const char* const     text2 = 0
        , const char* const     text3 = 0
        , const char* const     text4 = 0
    );

    //@}


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
