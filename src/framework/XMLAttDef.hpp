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

/**
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
 */
class XMLPARSER_EXPORT XMLAttDef
{
public:
    // -----------------------------------------------------------------------
    //  Class specific types
    //
    //  AttTypes
    //      The list of possible types that an attribute can have, according
    //      to the XML 1.0 spec.
    //
    //  DefAttTypes
    //      The modifiers that an attribute decl can have, which indicates
    //      whether instances of that attributes are required, implied, etc..
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

        , AttTypes_Count
        , AttTypes_Min      = 0
        , AttTypes_Max      = 9
        , AttTypes_Unknown  = -1
	};

    enum DefAttTypes
    {
        Default             = 0
        , Required          = 1
        , Implied           = 2
        , Fixed             = 3

        , DefAttTypes_Count
        , DefAttTypes_Min   = 0
        , DefAttTypes_Max   = 3
        , DefAttTypes_Unknown = -1
	};


    // -----------------------------------------------------------------------
    //  Public static data members
    // -----------------------------------------------------------------------
    static const unsigned int fgInvalidAttrId;


    // -----------------------------------------------------------------------
    //  Public, static methods
    // -----------------------------------------------------------------------
    static const XMLCh* getAttTypeString(const AttTypes attrType);
    static const XMLCh* getDefAttTypeString(const DefAttTypes attrType);


    // -----------------------------------------------------------------------
    //  Destructor
    // -----------------------------------------------------------------------

    /** @name Destructor */
    //@{
    virtual ~XMLAttDef();
    //@}


    // -----------------------------------------------------------------------
    //  Support named pool element semantics
    // -----------------------------------------------------------------------
    const XMLCh* getKey() const;


    // -----------------------------------------------------------------------
    //  The virtual attribute def interface
    // -----------------------------------------------------------------------
    virtual const XMLCh* getFullName() const = 0;


    // -----------------------------------------------------------------------
    //  Getter methods
    // -----------------------------------------------------------------------
    DefAttTypes getDefaultType() const;
    const XMLCh* getEnumeration() const;
    unsigned int getId() const;
    bool getProvided() const;
    AttTypes getType() const;
    const XMLCh* getValue() const;


    // -----------------------------------------------------------------------
    //  Setter methods
    // -----------------------------------------------------------------------
    void setDefaultType(const XMLAttDef::DefAttTypes newValue);
    void setId(const unsigned int newId);
    void setProvided(const bool newValue);
    void setType(const XMLAttDef::AttTypes newValue);
    void setValue(const XMLCh* const newValue);
    void setEnumeration(const XMLCh* const newValue);


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
    // -----------------------------------------------------------------------
    DefAttTypes     fDefaultType;
    XMLCh*          fEnumeration;
    unsigned int    fId;
    bool            fProvided;
    AttTypes        fType;
    XMLCh*          fValue;
};


// ---------------------------------------------------------------------------
//  Support named pool element semantics
// ---------------------------------------------------------------------------
inline const XMLCh* XMLAttDef::getKey() const
{
    //
    //  Just call the virtual methods since only the validator specific
    //  derived class knows what the full name is.
    //
    return getFullName();
}


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

#endif
