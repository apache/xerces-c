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
 * Revision 1.4  2000/02/16 19:48:56  roddey
 * More documentation updates
 *
 * Revision 1.3  2000/02/15 01:21:30  roddey
 * Some initial documentation improvements. More to come...
 *
 * Revision 1.2  2000/02/06 07:47:48  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.1.1.1  1999/11/09 01:08:32  twl
 * Initial checkin
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

/**
 *  This class defines the core information of an element declaration. Each
 *  validator (DTD, Schema, etc...) will have its own information that it
 *  associations with the declaration of an element, but they must all share
 *  at least this core information, i.e. they must all derive from this
 *  class. The set of info enforced at this level is driven by the needs of
 *  XML 1.0 spec validation and well formedness checks.
 *
 *  This class supports keyed collection semantics by providing the getKey()
 *  method, which extracts the key field. getKey() just calls getFullName()
 *  which is virtual so that each type of validator can have its own version
 *  of what a full name is. For the DTD validator, its element decl
 *  derivatives will return QNames in form pre:name. A Schema validator
 *  would return expanded names in the form {uri}name.
 *
 *  This class defines some special element id values for invalid elements
 *  and PCDATA elements, as well as a string for the special PCDATA element
 *  name. All validators must honor these special values in order to allow
 *  content models to work generically (i.e. to let code know when its dealing
 *  with invalid or PCDATA element ids without having to know what type of
 *  validator its messing with.)
 */
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
    /** @name Destructor */
    //@{
    virtual ~XMLElementDecl();
    //@}


    // -----------------------------------------------------------------------
    //  The virtual element decl interface
    // -----------------------------------------------------------------------

    /** @name Virual ElementDecl interface */
    //@{

    /** Find an attribute by name or optionally fault it in.
      *
      * The derived class should look up the passed attribute in the list of
      * of attributes for this element. If namespaces are enabled, then it
      * should use the uriId/baseName pair, else it should use the qName. The
      * options allow the caller to indicate whether the attribute should be
      * defaulted in if not found. If it is defaulted in, then wasAdded should
      * be set, else it should be cleared. If its not found and the caller does
      * not want defaulting, then return a null pointer.
      *
      * @param  qName       This is the qName of the attribute, i.e. the actual
      *                     lexical name found.
      *
      * @param  uriId       This is the id of the URI of the namespace to which
      *                     this attribute mapped. Only valid if namespaces are
      *                     enabled.
      *
      * @param  baseName    This is the base part of the name, i.e. after any
      *                     prefix.
      *
      * @param  options     Indicates the lookup options.
      *
      * @param  wasAdded    Should be set if the attribute is faulted in, else
      *                     cleared.
      */
    virtual XMLAttDef* findAttr
    (
        const   XMLCh* const    qName
        , const unsigned int    uriId
        , const XMLCh* const    baseName
        , const LookupOpts      options
        ,       bool&           wasAdded
    )   const = 0;

    /** Get a list of attributes defined for this element.
      *
      * The derived class should return a reference to some member object which
      * implements the XMLAttDefList interface. This object gives the scanner the
      * ability to look through the attributes defined for this element.
      *
      * It is done this way for efficiency, though of course this is not thread
      * safe. The scanner guarantees that it won't ever call this method in any
      * nested way, but the outside world must be careful about when it calls
      * this method, and optimally never would.
      */
    virtual XMLAttDefList& getAttDefList() const = 0;

    /** Get the base name of this element type.
      *
      * The derived class should return the base name part of the element's
      * name. This is the same regardless of whether namespaces are enabled or
      * not.
      *
      * @return A const pointer to the base name of the element decl.
      */
    virtual const XMLCh* getBaseName() const = 0;

    /** The character data options for this element type
      *
      * The derived class should return an appropriate character data opts value
      * which correctly represents its tolerance towards whitespace or character
      * data inside of its instances. This allows the scanner to do all of the
      * validation of character data.
      */
    virtual CharDataOpts getCharDataOpts() const = 0;

    /** Get the full name of this element type.
      *
      * The derived class should reutrn the full name of the element. If namespaces
      * are not enabled, then this is the qName. Else it is the {uri}baseName
      * form. For those validators that always require namespace processing, it
      * will always be in the latter form because namespace processing will always
      * be on.
      */
    virtual const XMLCh* getFullName() const = 0;

    /** Indicate whether this element type defined any attributes
      *
      * The derived class should return a boolean that indicates whether this
      * element has any attributes defined for it or not. This is an optimization
      * that allows the scanner to skip some work if no attributes exist.
      */
    virtual bool hasAttDefs() const = 0;

    /** Reset the flags on the attribute definitions.
      *
      * This method is called by the scanner at the beginning of each scan
      * of a start tag, asking this element decl to reset the 'declared' flag
      * of each of its attribute defs. This allows the scanner to mark each
      * one as declared yet or not.
      */
    virtual bool resetDefs() = 0;

    //@}


    // -----------------------------------------------------------------------
    //  Getter methods
    // -----------------------------------------------------------------------

    /** @name Getter methods */
    //{@

    /** Get a pointer to the abstract content model
      *
      * This method will return a const pointer to the content model object
      * of this element. This class is a simple abstraction that allows an
      * element to define and use multiple, specialized content model types
      * internally but still allow the outside world to do simple stuff with
      * them.
      *
      * @return A const pointer to the element's content model, via the basic
      * abstract content model type.
      */
    const XMLContentModel* getContentModel() const;

    /** Get a pointer to the abstract content model
      *
      * This method is identical to the previous one, except that it is non
      * const.
      */
    XMLContentModel* getContentModel();

    /** Get the create reason for this element type
      *
      * This method returns an enumeration which indicates why this element
      * declaration exists. Elements can be used before they are actually
      * declared, so they will often be faulted into the pool and marked as
      * to why they are there.
      *
      * @return An enumerated value that indicates the reason why this element
      * was added to the element decl pool.
      */
    CreateReasons getCreateReason() const;

    /** Get the element decl pool id for this element type
      *
      * This method will return the element decl pool id of this element
      * declaration. This uniquely identifies this element type within the
      * parse event that it is declared within. This value is assigned by the
      * validator whose decl pool this object belongs to.
      *
      * @return The element decl id of this element declaration.
      */
    unsigned int getId() const;

    /** Indicate whether this element type has been declared yet
      *
      * This method returns a boolean that indicates whether this element
      * has been declared yet. There are a number of reasons why an element
      * declaration can be faulted in, but eventually it must be declared or
      * its an error. See the CreateReasons enumeration.
      *
      * @return true if this element has been declared, else false.
      */
    bool isDeclared() const;

    //@}


    // -----------------------------------------------------------------------
    //  Setter methods
    // -----------------------------------------------------------------------

    /** @name Setter methods */
    //{@

    /** Set the content model object for this element type
      *
      * This method will adopt the based content model object. This is called
      * by the actual validator which is parsing its DTD or Schema or whatever
      * a creating an element decl. It will build what it feels is the correct
      * content model type object and store it on the element decl object via
      * this method.
      *
      * @param  newModelToAdopt This method will adop the passed content model
      *         object. Any previous object is destroyed.
      */
    void setContentModel(XMLContentModel* const newModelToAdopt);

    /** Update the create reason for this element type.
      *
      * This method will update the 'create reason' field for this element
      * decl object. As the validator parses its DTD, Schema, etc... it will
      * encounter various references to an element declaration, which will
      * cause the element declaration to either be declared or to be faulted
      * into the pool in preperation for some future declaration. As it does
      * so,it will update this field to indicate the current satus of the
      * decl object.
      */
    void setCreateReason(const CreateReasons newReason);

    /** Set the element decl pool id for this element type
      *
      * This method will set the pool id of this element decl. This is called
      * by the validator which created this object, and will provide this
      * decl object with a unique id within the parse event that created it.
      */
    void setId(const unsigned int newId);

    //@}


    // -----------------------------------------------------------------------
    //  Miscellaneous methods
    // -----------------------------------------------------------------------

    /** @name Miscellenous methods */
    //{@

    /** Geta formatted string of the content model
      *
      * This method is a convenience method which will create a formatted
      * representation of the content model of the element. It will not always
      * exactly recreate the original model, since some normalization or
      * or reformatting may occur. But, it will be a technically accurate
      * representation of the original content model.
      *
      * The format depends upon the validator, since content models are
      * expressed differently in different structural description languages.
      *
      * @param  validator   The validator which owns this object, and which
      *                     therefore has the information required to format
      *                     the content model.
      *
      * @return A pointer to an internal buffer which contains the formatted
      *         content model. The caller does not own this buffer and should
      *         copy it if it needs to be kept around.
      */
    const XMLCh* getFormattedContentModel
    (
        const   XMLValidator&   validator
    )   const;

    /** Support keyed collections
      *
      * This method allows objects of this type be placed into one of the
      * standard keyed collections. This method will return the full name of
      * the element, which will vary depending upon the type of the validator.
      *
      * @return A const pointer to teh full name of this element type.
      */
    const XMLCh* getKey() const;

    //@}


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
