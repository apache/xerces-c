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
 * Revision 1.1  2002/02/01 22:21:51  peiyongz
 * Initial revision
 *
 * Revision 1.6  2000/02/24 20:00:23  abagchi
 * Swat for removing Log from API docs
 *
 * Revision 1.5  2000/02/16 23:03:48  roddey
 * More documentation updates
 *
 * Revision 1.4  2000/02/16 21:42:58  aruna1
 * API Doc++ summary changes in
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

#if !defined(XMLENTITYDECL_HPP)
#define XMLENTITYDECL_HPP

#include <xercesc/util/XMLString.hpp>

/**
 *  This class defines that core information that defines an XML entity, no
 *  matter what validator is used. Each validator will create a derivative
 *  of this class which adds any extra information it requires.
 *
 *  This class supports keyed collection semantics via the getKey() method
 *  which extracts the key field, the entity name in this case. The name will
 *  have whatever form is deemed appropriate for the type of validator in
 *  use.
 *
 *  When setting the fields of this class, you must make sure that you do
 *  not set conflicting values. For instance, an internal entity cannot have
 *  a notation name. And an external entity cannot have a value string.
 *  These rules are defined by the XML specification. In most cases, these
 *  objects are created by validator objects as they parse a DTD or Schema
 *  or whatever, at which time they confirm the correctness of the data before
 *  creating the entity decl object.
 */
class XMLPARSER_EXPORT XMLEntityDecl
{
public:
    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------

    /** @name Constructors */
    //@{
    
    /**
      *  Deafult Constructor
      */
    XMLEntityDecl();

    /** Constructor with a const entity name
      *
      * @param  entName The new name to give to this entity.
      */
    XMLEntityDecl
    (
        const   XMLCh* const    entName
    );
    
    /**
      * Constructor with a const entity name and value
      *
      * @param  entName The new name to give to this entity.
      * @param  value   The new value to give to this entity name.
      */
    XMLEntityDecl
    (
        const   XMLCh* const    entName
        , const XMLCh* const    value
    );

    /**
      * Constructor with a const entity name and single XMLCh value
      *
      * @param  entName The new name to give to this entity.
      * @param  value   The new value to give to this entity name.
      */
    XMLEntityDecl
    (
        const   XMLCh* const    entName
        , const XMLCh           value
    );
    //@}

    /** @name Destructor */
    //@{

    /**
      *  Default destructor
      */
    virtual ~XMLEntityDecl();

    //@}


    // -----------------------------------------------------------------------
    //  Virtual entity decl interface
    // -----------------------------------------------------------------------

    /** @name The pure virtual methods in this interface. */
    //@{

    /** Get the 'declared in internal subset' flag
      *
      * Gets the state of the flag which indicates whether the entity was
      * declared in the internal or external subset. Some structural 
      * description languages might not have an internal subset concept, in
      * which case this will always return false.
      */
    virtual bool getDeclaredInIntSubset() const = 0;

    /** Get the 'is parameter entity' flag
      *
      * Gets the state of the flag which indicates whether this entity is
      * a parameter entity. If not, then its a general entity.
      */
    virtual bool getIsParameter() const = 0;

    /** Get the 'is special char entity' flag
      *
      * Gets the state of the flag that indicates whether this entity is
      * one of the special, intrinsically supported character entities.
      */
    virtual bool getIsSpecialChar() const = 0;

    //@}


    // -----------------------------------------------------------------------
    //  Getter methods
    // -----------------------------------------------------------------------

    /** @name Getter methods */
    //@{

    /**
      * Gets the pool id of this entity. Validators maintain all decls in 
      * pools, from which they can be quickly extracted via id.
      */
    unsigned int getId() const;

    /**
      * Returns a const pointer to the name of this entity decl. This name
      * will be in whatever format is appropriate for the type of validator
      * in use.
      */
    const XMLCh* getName() const;

    /**
      * Gets the notation name, if any, declared for this entity. If this
      * entity is not a notation type entity, it will be a null pointer.
      */
    const XMLCh* getNotationName() const;

    /**
      * Gets the public id declared for this entity. Public ids are optional
      * so it can be a null pointer.
      */
    const XMLCh* getPublicId() const;

    /**
      * Gets the system id declared for this entity. The system id is required
      * so this method should never return a null pointers.
      */
    const XMLCh* getSystemId() const;

    /**
      * This method returns the value of an internal entity. If this is not
      * an internal entity (i.e. its external), then this will be a null
      * pointer.
      */
    const XMLCh* getValue() const;

    /**
     *  This method returns the number of characters in the value returned
     *  by getValue(). If this entity is external, this will be zero since
     *  an external entity has no internal value.
     */
    unsigned int getValueLen() const;

    /**
      * Indicates that this entity is an external entity. If not, then it is
      * assumed to be an internal entity, suprise.
      */
    bool isExternal() const;

    /**
      * Indicates whether this entity is unparsed. This is meaningless for
      * internal entities. Some external entities are unparsed in that they
      * refer to something other than XML source.
      */
    bool isUnparsed() const;

    //@}


    // -----------------------------------------------------------------------
    //  Setter methods
    // -----------------------------------------------------------------------

    /** @name Setter methods */
    //@{

    /**
     *  This method will set the entity name. The format of this name is
     *  defined by the particular validator in use, since it will be the
     *  one who creates entity definitions as it parses the DTD, Schema,
     *  ect...
     *
     *  @param  entName   The new name to give to this entity.
     */
    void setName
    (
        const   XMLCh* const    entName
    );

    /**
     *  This method will set the notation name for this entity. By setting
     *  this, you are indicating that this is an unparsed external entity.
     *
     *  @param  newName   The new notation name to give to this entity.
     */
    void setNotationName(const XMLCh* const newName);

    /**
     *  This method will set a new public id on this entity. The public id
     *  has no particular form and is purely for client consumption.
     *
     *  @param  newId     The new public id to give to this entity.
     */
    void setPublicId(const XMLCh* const newId);

    /**
     *  This method will set a new sysetm id on this entity. This will
     *  then control where the source for this entity lives. If it is
     *  an internal entity, then the system id is only for bookkeeping
     *  purposes, and to allow any external entities referenced from
     *  within the entity to be correctly resolved.
     *
     *  @param  newId     The new system id to give to the entity.
     */
    void setSystemId(const XMLCh* const newId);

    /**
     *  This method will set a new value for this entity. This is only
     *  valid if the entity is to be an internal entity. By setting this
     *  field, you are indicating that the entity is internal.
     *
     *  @param  newValue  The new value to give to this entity.
     */
    void setValue(const XMLCh* const newValue);

    //@}

    /* For internal use only */
    void setId(const unsigned int newId);


    // -----------------------------------------------------------------------
    //  Support named pool syntax
    // -----------------------------------------------------------------------

    /** @name Setter methods */
    //@{

    /**
      * This method allows objects of this class to be used within a standard
      * keyed collection used commonly within the parser system. The collection
      * calls this method to get the key (usually to hash it) by which the
      * object is to be stored.
      */
    const XMLCh* getKey() const;

    //@}


private :
    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    XMLEntityDecl(const XMLEntityDecl&);
    void operator=(XMLEntityDecl&);


    // -----------------------------------------------------------------------
    //  XMLEntityDecl: Private helper methods
    // -----------------------------------------------------------------------
    void cleanUp();


    // -----------------------------------------------------------------------
    //  Private data members
    //
    //  fId
    //      This is the unique id given to this entity decl.
    //
    //  fName
    //      The name of the enitity. Entity names are never namespace based.
    //
    //  fNotationName
    //      The optional notation of the entity. If there was none, then its
    //      empty.
    //
    //  fPublicId
    //      The public id of the entity, which can be empty.
    //
    //  fSystemId
    //      The system id of the entity.
    //
    //  fValue
    //  fValueLen
    //      The entity's value and length, which is only valid if its an
    //      internal style entity.
    // -----------------------------------------------------------------------
    unsigned int    fId;
	XMLCh*          fName;
    XMLCh*          fNotationName;
    XMLCh*          fPublicId;
    XMLCh*          fSystemId;
    XMLCh*          fValue;
    unsigned int    fValueLen;
};


// ---------------------------------------------------------------------------
//  XMLEntityDecl: Getter methods
// ---------------------------------------------------------------------------
inline unsigned int XMLEntityDecl::getId() const
{
    return fId;
}

inline const XMLCh* XMLEntityDecl::getName() const
{
    return fName;
}

inline const XMLCh* XMLEntityDecl::getNotationName() const
{
    return fNotationName;
}

inline const XMLCh* XMLEntityDecl::getPublicId() const
{
    return fPublicId;
}

inline const XMLCh* XMLEntityDecl::getSystemId() const
{
    return fSystemId;
}

inline const XMLCh* XMLEntityDecl::getValue() const
{
    return fValue;
}

inline unsigned int XMLEntityDecl::getValueLen() const
{
    return fValueLen;
}

inline bool XMLEntityDecl::isExternal() const
{
    // If it has a system or public id, its external
    return ((fPublicId != 0) || (fSystemId != 0));
}

inline bool XMLEntityDecl::isUnparsed() const
{
    // If it has a notation, its unparsed
    return (fNotationName != 0);
}


// ---------------------------------------------------------------------------
//  XMLEntityDecl: Setter methods
// ---------------------------------------------------------------------------
inline void XMLEntityDecl::setId(const unsigned int newId)
{
    fId = newId;
}

inline void XMLEntityDecl::setNotationName(const XMLCh* const newName)
{
    delete [] fNotationName;
    fNotationName = XMLString::replicate(newName);
}

inline void XMLEntityDecl::setPublicId(const XMLCh* const newId)
{
    delete [] fPublicId;
    fPublicId = XMLString::replicate(newId);
}

inline void XMLEntityDecl::setSystemId(const XMLCh* const newId)
{
    delete [] fSystemId;
    fSystemId = XMLString::replicate(newId);
}

inline void XMLEntityDecl::setValue(const XMLCh* const newValue)
{
    delete [] fValue;
    fValue = XMLString::replicate(newValue);
    fValueLen = XMLString::stringLen(newValue);
}


// ---------------------------------------------------------------------------
//  XMLEntityDecl: Support named pool syntax
// ---------------------------------------------------------------------------
inline const XMLCh* XMLEntityDecl::getKey() const
{
    return fName;
}

#endif
