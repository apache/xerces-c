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
 * Revision 1.1  1999/11/09 01:08:32  twl
 * Initial revision
 *
 * Revision 1.2  1999/11/08 20:44:38  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */

#if !defined(XMLELEMENTDECL_HPP)
#define XMLELEMENTDECL_HPP

#include <util/XMLString.hpp>
#include <framework/XMLAttr.hpp>
#include <framework/XMLAttDefList.hpp>
#include <framework/XMLContentModel.hpp>

class XMLValidator;

//
//  This class defines the core information of an element declaration. Each
//  validator (DTD, Schema, etc...) will have its own information that it
//  associations with the declaration of an element, but they must all share
//  at least this core information, i.e. they must all derive from this
//  class. The set of info enforced at this level is driven by the needs of
//  XML 1.0 spec validation and well formedness checks.
//
//  This class supports keyed collection semantics by providing the getKey()
//  method, which extracts the key field. getKey() just calls getFullName()
//  which is virtual so that each type of validator can have its own version
//  of what a full name is. For the DTD validator, its element decl
//  derivatives will return QNames in form pre:name. A Schema validator
//  would return expanded names in the form {uri}name.
//
//  This class defines some special element id values for invalid elements
//  and PCDATA elements, as well as a string for the special PCDATA element
//  name. All validators must honor these special values in order to allow
//  content models to work generically (i.e. to let code know when its dealing
//  with invalid or PCDATA element ids without having to know what type of
//  validator its messing with.)
//
class XMLPARSER_EXPORT XMLElementDecl
{
 public:
    // -----------------------------------------------------------------------
    //  Class specific types
    //
    //  CreateReasons
    //      This type is used to store how an element declaration got into
    //      the validator's element pool. They are faulted in for various
    //      reasons.
    //
    //  LookupOpts
    //      These are the values used by the attribute lookup methods.
    //
    //  CharDataOpts
    //      This is used to indicate how this type of element reacts to
    //      character data as content.
    // -----------------------------------------------------------------------
    enum CreateReasons
    {
        NoReason
        , Declared
        , AttList
        , InContentModel
        , AsRootElem
    };

    enum LookupOpts
    {
        AddIfNotFound
        , FailIfNotFound
    };

    enum CharDataOpts
    {
        NoCharData
        , SpacesOk
        , AllCharData
    };


    // -----------------------------------------------------------------------
    //  Public static data
    //
    //  fgInvalidElemId
    //      A value to represent an invalid element node id.
    //
    //  fgPCDataElemId
    //      This is the value to use to represent a PCDATA node when an
    //      element id is required.
    //
    //  fgPCDataElemName
    //      This is the value to use to represent a PCDATA node when an
    //      element name is required.
    // -----------------------------------------------------------------------
    static const unsigned int   fgInvalidElemId;
    static const unsigned int   fgPCDataElemId;
    static const XMLCh          fgPCDataElemName[];



    // -----------------------------------------------------------------------
    //  Destructor
    // -----------------------------------------------------------------------
    virtual ~XMLElementDecl();


    // -----------------------------------------------------------------------
    //  The virtual element decl interface
    // -----------------------------------------------------------------------
    virtual XMLAttDef* findAttr
    (
        const   XMLCh* const    qName
        , const unsigned int    uriId
        , const XMLCh* const    baseName
        , const LookupOpts      options
        ,       bool&           wasAdded
    )   const = 0;
    virtual XMLAttDefList& getAttDefList() const = 0;
    virtual const XMLCh* getBaseName() const = 0;
    virtual CharDataOpts getCharDataOpts() const = 0;
    virtual const XMLCh* getFullName() const = 0;
    virtual bool hasAttDefs() const = 0;
    virtual bool resetDefs() = 0;


    // -----------------------------------------------------------------------
    //  Getter methods
    // -----------------------------------------------------------------------
    XMLContentModel* getContentModel();
    const XMLContentModel* getContentModel() const;
    CreateReasons getCreateReason() const;
    unsigned int getId() const;
    bool isDeclared() const;


    // -----------------------------------------------------------------------
    //  Setter methods
    // -----------------------------------------------------------------------
    void setContentModel(XMLContentModel* const newModelToAdopt);
    void setCreateReason(const CreateReasons newReason);
    void setId(const unsigned int newId);


    // -----------------------------------------------------------------------
    //  Miscellaneous methods
    // -----------------------------------------------------------------------
    const XMLCh* getFormattedContentModel
    (
        const   XMLValidator&   validator
    )   const;
    const XMLCh* getKey() const;


protected :
    // -----------------------------------------------------------------------
    //  Hidden constructors
    // -----------------------------------------------------------------------
    XMLElementDecl();


    // -----------------------------------------------------------------------
    //  Protected, virtual methods
    // -----------------------------------------------------------------------
    virtual XMLContentModel* makeContentModel() const = 0;
    virtual XMLCh* formatContentModel
    (
        const   XMLValidator&   validator
    )   const = 0;


private :
    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    XMLElementDecl(const XMLElementDecl&);
    void operator=(const XMLElementDecl&);


    // -----------------------------------------------------------------------
    //  Data members
    //
    //  fContentModel
    //      The content model object for this element. It is stored here via
    //      its abstract interface.
    //
    //  fCreateReason
    //      We sometimes have to put an element decl object into the elem
    //      decl pool before the element's declaration is seen, such as when
    //      its used in another element's content model or an att list is
    //      seen for it. This flag tells us whether its been declared, and
    //      if not why it had to be created.
    //
    //  fFormattedModel
    //      This is a faulted in member. When the outside world asks for
    //      our content model as a string, we format it and fault it into
    //      this field (to avoid doing the formatted over and over.)
    //
    //  fId
    //      The unique id of this element. This is created by the derived
    //      class, or more accurately the validator that owns the objects
    //      of the derived types. But, since they all have to have them, we
    //      let them all store the id here. It is defaulted to have the
    //      value fgInvalidElem until explicitly set.
    // -----------------------------------------------------------------------
    XMLContentModel*    fContentModel;
    CreateReasons       fCreateReason;
    XMLCh*              fFormattedModel;
    unsigned int        fId;
};


// ---------------------------------------------------------------------------
//  XMLElementDecl: Miscellaneous methods
// ---------------------------------------------------------------------------
inline const XMLCh* XMLElementDecl::getKey() const
{
    //
    //  We just call the virtual method, since only the derived class knows
    //  what form the full name should take.
    //
    return getFullName();
}


// ---------------------------------------------------------------------------
//  XMLElementDecl: Getter methods
// ---------------------------------------------------------------------------
inline XMLContentModel* XMLElementDecl::getContentModel()
{
    if (!fContentModel)
        fContentModel = makeContentModel();
    return fContentModel;
}

inline const XMLContentModel* XMLElementDecl::getContentModel() const
{
    // Fault in the content model (which requires a cast off of const)
    if (!fContentModel)
        ((XMLElementDecl*)this)->fContentModel = makeContentModel();
    return fContentModel;
}

inline XMLElementDecl::CreateReasons XMLElementDecl::getCreateReason() const
{
    return fCreateReason;
}

inline unsigned int XMLElementDecl::getId() const
{
    return fId;
}

inline bool XMLElementDecl::isDeclared() const
{
    return (fCreateReason == Declared);
}


// ---------------------------------------------------------------------------
//  XMLElementDecl: Setter methods
// ---------------------------------------------------------------------------
inline void
XMLElementDecl::setContentModel(XMLContentModel* const newModelToAdopt)
{
    delete fContentModel;
    fContentModel = newModelToAdopt;
}

inline void
XMLElementDecl::setCreateReason(const XMLElementDecl::CreateReasons newReason)
{
    fCreateReason = newReason;
}

inline void XMLElementDecl::setId(const unsigned int newId)
{
    fId = newId;
}

#endif
