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

#if !defined(XMLENTITYDECL_HPP)
#define XMLENTITYDECL_HPP

#include <util/XMLString.hpp>

//
//  This class defines that core information that defines an entity, no
//  matter what validator is used. Each validator will create a derivative
//  of this class which adds any extra information it requires.
//
//  This class supportes keyed collection semantics via the getKey() method
//  which extracts the key field, the entity name in this case.
//
class XMLPARSER_EXPORT XMLEntityDecl
{
public:
    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    XMLEntityDecl();
    XMLEntityDecl
    (
        const   XMLCh* const    entName
    );
    XMLEntityDecl
    (
        const   XMLCh* const    entName
        , const XMLCh* const    value
    );
    XMLEntityDecl
    (
        const   XMLCh* const    entName
        , const XMLCh           value
    );
    virtual ~XMLEntityDecl();


    // -----------------------------------------------------------------------
    //  Virtual entity decl interface
    // -----------------------------------------------------------------------
    virtual bool getDeclaredInIntSubset() const = 0;
    virtual bool getIsParameter() const = 0;
    virtual bool getIsSpecialChar() const = 0;


    // -----------------------------------------------------------------------
    //  Getter methods
    // -----------------------------------------------------------------------
    unsigned int getId() const;
    const XMLCh* getName() const;
    const XMLCh* getNotationName() const;
    const XMLCh* getPublicId() const;
    const XMLCh* getSystemId() const;
    const XMLCh* getValue() const;
    unsigned int getValueLen() const;
    bool isExternal() const;
    bool isUnparsed() const;


    // -----------------------------------------------------------------------
    //  Setter methods
    // -----------------------------------------------------------------------
    void setId(const unsigned int newId);
    void setName
    (
        const   XMLCh* const    entName
    );
    void setNotationName(const XMLCh* const newName);
    void setPublicId(const XMLCh* const newId);
    void setSystemId(const XMLCh* const newId);
    void setValue(const XMLCh* const newValue);


    // -----------------------------------------------------------------------
    //  Support named pool syntax
    // -----------------------------------------------------------------------
    const XMLCh* getKey() const;


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
