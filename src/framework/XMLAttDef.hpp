/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 1999-2001 The Apache Software Foundation.  All rights
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
 * Revision 1.10  2001/05/11 13:25:31  tng
 * Copyright update.
 *
 * Revision 1.9  2001/02/27 18:48:20  tng
 * Schema: Add SchemaAttDef, SchemaElementDecl, SchemaAttDefList.
 *
 * Revision 1.8  2000/12/14 18:49:56  tng
 * Fix API document generation warning: "Warning: end of member group without matching begin"
 *
 * Revision 1.7  2000/11/30 18:22:38  andyh
 * reuseValidator - fix bugs (spurious errors) that occured on reuse due to
 * pools already containing some items.  Fixed by Tinny Ng.
 *
 * Revision 1.6  2000/07/07 22:23:38  jpolast
 * remove useless getKey() functions.
 *
 * Revision 1.5  2000/02/24 20:00:22  abagchi
 * Swat for removing Log from API docs
 *
 * Revision 1.4  2000/02/16 23:03:48  roddey
 * More documentation updates
 *
 * Revision 1.3  2000/02/15 01:21:30  roddey
 * Some initial documentation improvements. More to come...
 *
 * Revision 1.2  2000/02/06 07:47:46  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.1.1.1  1999/11/09 01:08:27  twl
 * Initial checkin
 *
 * Revision 1.2  1999/11/08 20:44:34  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */

#if !defined(ATTDEF_HPP)
#define ATTDEF_HPP

#include <util/XMLString.hpp>

class XMLAttr;

/** Represents the core information of an atribute definition
 *
 *  This class defines the basic characteristics of an attribute, no matter
 *  what type of validator is used. If a particular schema associates more
 *  information with an attribute it will create a derivative of this class.
 *  So this class provides an abstract way to get basic information on
 *  attributes from any type of validator.
 *
 *  This class supports keyed collection semantics on the fully qualified
 *  attribute name, by providing a getKey() method to extract the key string.
 *  getKey(), in this case, just calls the virtual method getFullName() to
 *  get the fully qualified name, as defined by the derived class.
 *
 *  Note that the 'value' of an attribute type definition is the default or
 *  of fixed value given to it in its definition. If the attribute is of the
 *  enumerated or notation type, it will have an 'enumeration value' as well
 *  which is a space separated list of its possible vlaues.
 */
class XMLPARSER_EXPORT XMLAttDef
{
public:
    // -----------------------------------------------------------------------
    //  Class specific types
    //
    //  AttTypes
    //      The list of possible types that an attribute can have, according
    //      to the XML 1.0 spec and schema.
    //
    //  DefAttTypes
    //      The modifiers that an attribute decl can have, which indicates
    //      whether instances of that attributes are required, implied, etc..
    //
    //  CreateReasons
    //      This type is used to store how an attribute declaration got into
    //      the elementdecl's attribute pool.
    //
    // -----------------------------------------------------------------------
	enum AttTypes
    {
        CData               = 0
        , ID                = 1
        , IDRef             = 2
        , IDRefs            = 3
        , Entity            = 4
        , Entities          = 5
        , NmToken           = 6
        , NmTokens          = 7
        , Notation          = 8
        , Enumeration       = 9
        , Simple            = 10
        , Any_Any           = 11
        , Any_Other         = 12
        , Any_Local         = 13
        , Any_List          = 14

        , AttTypes_Count
        , AttTypes_Min      = 0
        , AttTypes_Max      = 14
        , AttTypes_Unknown  = -1
	};

    enum DefAttTypes
    {
        Default                  = 0
        , Required               = 1
        , Implied                = 2
        , Fixed                  = 3
        , Prohibited             = 4
        , Required_And_Fixed     = 5
        , ProcessContents_Strict = 6
        , ProcessContents_Lax    = 7
        , ProcessContents_Skip   = 8

        , DefAttTypes_Count
        , DefAttTypes_Min   = 0
        , DefAttTypes_Max   = 8
        , DefAttTypes_Unknown = -1
	};

    enum CreateReasons
    {
        NoReason
        , JustFaultIn
    };

    // -----------------------------------------------------------------------
    //  Public static data members
    // -----------------------------------------------------------------------
    static const unsigned int fgInvalidAttrId;


    // -----------------------------------------------------------------------
    //  Public, static methods
    // -----------------------------------------------------------------------

    /** @name Public, static methods */
    //@{

    /** Get a string representation of the passed attribute type enum
      *
      * This method allows you to get a textual representation of an attriubte
      * type, mostly for debug or display.
      *
      * @param attrType The attribute type value to get the string for.
      *
      * @return A const pointer to the static string that holds the text
      *         description of the passed type.
      */
    static const XMLCh* getAttTypeString(const AttTypes attrType);

    /** Get a string representation of the passed def attribute type enum
      *
      * This method allows you to get a textual representation of an default
      * attributetype, mostly for debug or display.
      *
      * @param attrType The default attribute type value to get the string for.
      *
      * @return A const pointer to the static string that holds the text
      *         description of the passed default type.
      */
    static const XMLCh* getDefAttTypeString(const DefAttTypes attrType);

    //@}


    // -----------------------------------------------------------------------
    //  Destructor
    // -----------------------------------------------------------------------

    /** @name Destructor */
    //@{

    /**
      *  Destructor
      */
    virtual ~XMLAttDef();
    //@}


    // -----------------------------------------------------------------------
    //  The virtual attribute def interface
    // -----------------------------------------------------------------------

    /** @name Virtual interface */
    //@{

    /** Get the full name of this attribute type
      *
      * The derived class should return a const pointer to the full name of
      * this attribute. This will vary depending on the type of validator in
      * use.
      *
      * @return A const pointer to the full name of this attribute type.
      */
    virtual const XMLCh* getFullName() const = 0;

    //@}


    // -----------------------------------------------------------------------
    //  Getter methods
    // -----------------------------------------------------------------------

    /** @name Getter methods */
    //@{

    /** Get the default type of this attribute type
      *
      * This method returns the 'default type' of the attribute. Default
      * type in this case refers to the XML concept of a default type for
      * an attribute, i.e. #FIXED, #IMPLIED, etc...
      *
      * @return The default type enum for this attribute type.
      */
    DefAttTypes getDefaultType() const;

    /** Get the enumeration value (if any) of this attribute type
      *
      * If the attribute is of an enumeration or notatin type, then this
      * method will return a const reference to a string that contains the
      * space separated values that can the attribute can have.
      *
      * @return A const pointer to a string that contains the space separated
      *         legal values for this attribute.
      */
    const XMLCh* getEnumeration() const;

    /** Get the pool id of this attribute type
      *
      * This method will return the id of this attribute in the validator's
      * attribute pool. It was set by the validator when this attribute was
      * created.
      *
      * @return The pool id of this attribute type.
      */
    unsigned int getId() const;

    /** Query whether the attribute was explicitly provided.
      *
      * When the scanner scans a start tag, it will ask the element decl
      * object of the element type of that start tag to clear the 'provided'
      * flag on all its attributes. As the scanner sees explicitly provided
      * attributes, its turns on this flag to indicate that this attribute
      * has been provided. In this way, the scanner can catch duplicated
      * attributes and required attributes that aren't provided, and default
      * in fixed/default valued attributes that are not explicitly provided.
      *
      * @return Returns a boolean value that indicates whether this attribute
      *         was explicitly provided.
      */
    bool getProvided() const;

    /** Get the type of this attribute
      *
      * Gets the type of this attribute. This type is represented by an enum
      * that convers the types of attributes allowed by XML, e.g. CDATA, NMTOKEN,
      * NOTATION, etc...
      *
      * @return The attribute type enumeration value for this type of
      *         attribute.
      */
    AttTypes getType() const;

    /** Get the default/fixed value of this attribute (if any.)
      *
      * If the attribute defined a default/fixed value, then it is stored
      * and this method will retrieve it. If it has non, then a null pointer
      * is returned.
      *
      * @return A const pointer to the default/fixed value for this attribute
      *         type.
      */
    const XMLCh* getValue() const;

    /** Get the create reason for this attribute
      *
      * This method returns an enumeration which indicates why this attribute
      * declaration exists.
      *
      * @return An enumerated value that indicates the reason why this attribute
      * was added to the attribute table.
      */
    CreateReasons getCreateReason() const;

    //@}


    // -----------------------------------------------------------------------
    //  Setter methods
    // -----------------------------------------------------------------------

    /** @name Setter methods */
    //@{

    /** Set the default attribute type
      *
      * This method sets the default attribute type for this attribute.
      * This setting controls whether the attribute is required, fixed,
      * implied, etc...
      *
      * @param  The new default attribute to set
      */
    void setDefaultType(const XMLAttDef::DefAttTypes newValue);

    /** Set the pool id for this attribute type.
      *
      * This method sets the pool id of this attribute type. This is usually
      * called by the validator that creates the actual instance (which is of
      * a derived type known only by the validator.)
      *
      * @param  The new pool id to set.
      */
    void setId(const unsigned int newId);

    /** Set or clear the 'provided' flag.
      *
      * This method will set or clear the 'provided' flag. This is called
      * by the scanner as it is scanning a start tag and marking off the
      * attributes that have been explicitly provided.
      *
      * @param  The new provided state to set
      */
    void setProvided(const bool newValue);

    /** Set the type of this attribute type.
      *
      * This method will set the type of the attribute. The type of an attribute
      * controls how it is normalized and what kinds of characters it can hold.
      *
      * @param  The new attribute type to set
      */
    void setType(const XMLAttDef::AttTypes newValue);

    /** Set the default/fixed value of this attribute type.
      *
      * This method set the fixed/default value for the attribute. This value
      * will be used when instances of this attribute type are faulted in. It
      * <b>must</b> be a valid value for the type set by setType(). If the
      * type is enumeration or notation, this must be one of the valid values
      * set in the setEnumeration() call.
      *
      * @param  The new fixed/default value to set.
      */
    void setValue(const XMLCh* const newValue);

    /** Set the enumerated value of this attribute type.
      *
      * This method sets the enumerated/notation value list for this attribute
      * type. It is a space separated set of possible values. These values must
      * meet the constrains of the XML spec for such values of this type of
      * attribute. This should only be set if the setType() method is used to
      * set the type to the enumeration or notation types.
      *
      * @param  The new enumerated/notation value list to set.
      */
    void setEnumeration(const XMLCh* const newValue);

    /** Update the create reason for this attribute type.
      *
      * This method will update the 'create reason' field for this attribute
      * decl object.
      */
    void setCreateReason(const CreateReasons newReason);

    //@}

protected :
    // -----------------------------------------------------------------------
    //  Hidden constructors
    // -----------------------------------------------------------------------
    XMLAttDef
    (
        const   AttTypes            type = CData
        , const DefAttTypes         defType = Implied
    );
    XMLAttDef
    (
        const   XMLCh* const        attValue
        , const AttTypes            type
        , const DefAttTypes         defType
        , const XMLCh* const        enumValues = 0
    );


private :
    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    XMLAttDef(const XMLAttDef&);
    void operator=(const XMLAttDef&);


    // -----------------------------------------------------------------------
    //  Private helper methods
    // -----------------------------------------------------------------------
    void cleanUp();


    // -----------------------------------------------------------------------
    //  Private data members
    //
    //  fDefaultType
    //      Indicates what, if any, default stuff this attribute has.
    //
    //  fEnumeration
    //      If its an enumeration, this is the list of values as space
    //      separated values.
    //
    //  fId
    //      This is the unique id of this attribute, given to it when its put
    //      into the validator's attribute decl pool. It defaults to the
    //      special value XMLAttrDef::fgInvalidAttrId.
    //
    //  fProvided
    //      This field is really for use by the scanner. It is used to track
    //      which of the attributes of an element were provided. Any marked
    //      as not provided (after scanning the start tag) and having a
    //      default type of Required, is in error.
    //
    //  fType
    //      The type of attribute, which is one of the AttTypes values.
    //
    //  fValue
    //      This is the value of the attribute, which is the default value
    //      given in the attribute declaration.
    //
    //  fCreateReason
    //      This flag tells us how this attribute got created.  Sometimes even
    //      the attribute was not declared for the element, we want to fault
    //      fault it into the pool to avoid lots of redundant errors.
    // -----------------------------------------------------------------------
    DefAttTypes     fDefaultType;
    XMLCh*          fEnumeration;
    unsigned int    fId;
    bool            fProvided;
    AttTypes        fType;
    XMLCh*          fValue;
    CreateReasons       fCreateReason;
};



// ---------------------------------------------------------------------------
//  Getter methods
// ---------------------------------------------------------------------------
inline XMLAttDef::DefAttTypes XMLAttDef::getDefaultType() const
{
    return fDefaultType;
}

inline const XMLCh* XMLAttDef::getEnumeration() const
{
    return fEnumeration;
}

inline unsigned int XMLAttDef::getId() const
{
    return fId;
}

inline bool XMLAttDef::getProvided() const
{
    return fProvided;
}

inline XMLAttDef::AttTypes XMLAttDef::getType() const
{
    return fType;
}

inline const XMLCh* XMLAttDef::getValue() const
{
    return fValue;
}

inline XMLAttDef::CreateReasons XMLAttDef::getCreateReason() const
{
    return fCreateReason;
}


// ---------------------------------------------------------------------------
//  XMLAttDef: Setter methods
// ---------------------------------------------------------------------------
inline void XMLAttDef::setDefaultType(const XMLAttDef::DefAttTypes newValue)
{
    fDefaultType = newValue;
}

inline void XMLAttDef::setEnumeration(const XMLCh* const newValue)
{
    delete [] fEnumeration;
    fEnumeration = XMLString::replicate(newValue);
}

inline void XMLAttDef::setId(const unsigned int newId)
{
    fId = newId;
}

inline void XMLAttDef::setProvided(const bool newValue)
{
    fProvided = newValue;
}

inline void XMLAttDef::setType(const XMLAttDef::AttTypes newValue)
{
    fType = newValue;
}

inline void XMLAttDef::setValue(const XMLCh* const newValue)
{
    delete [] fValue;
    fValue = XMLString::replicate(newValue);
}

inline void
XMLAttDef::setCreateReason(const XMLAttDef::CreateReasons newReason)
{
    fCreateReason = newReason;
}

#endif
