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
 * Revision 1.1  1999/11/09 01:08:35  twl
 * Initial revision
 *
 * Revision 1.2  1999/11/08 20:44:39  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */

#if !defined(XMLNOTATIONDECL_HPP)
#define XMLNOTATIONDECL_HPP

#include <util/XML4CDefs.hpp>
#include <util/XMLString.hpp>

//
//  This class represents the core information about a notation declaration
//  that all validators must at least support. Each validator will create a
//  derivative of this class which adds any information it requires for its
//  own extra needs.
//
class XMLPARSER_EXPORT XMLNotationDecl
{
public:
    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    XMLNotationDecl();
    XMLNotationDecl
    (
        const   XMLCh* const    notName
        , const XMLCh* const    pubId
        , const XMLCh* const    sysId
    );
    ~XMLNotationDecl();


    // -----------------------------------------------------------------------
    //  Getter methods
    // -----------------------------------------------------------------------
    unsigned int getId() const;
    const XMLCh* getName() const;
    const XMLCh* getPublicId() const;
    const XMLCh* getSystemId() const;


    // -----------------------------------------------------------------------
    //  Setter methods
    // -----------------------------------------------------------------------
    void setId(const unsigned int newId);
    void setName
    (
        const   XMLCh* const    notName
    );
    void setPublicId(const XMLCh* const newId);
    void setSystemId(const XMLCh* const newId);


    // -----------------------------------------------------------------------
    //  Support named collection element semantics
    // -----------------------------------------------------------------------
    const XMLCh* getKey() const;


private :
    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    XMLNotationDecl(const XMLNotationDecl&);
    void operator=(const XMLNotationDecl&);


    // -----------------------------------------------------------------------
    //  XMLNotationDecl: Private helper methods
    // -----------------------------------------------------------------------
    void cleanUp();


    // -----------------------------------------------------------------------
    //  Private data members
    //
    //  fId
    //      This is the unique id given to this notation decl.
    //
    //  fName
    //      The notation's name, which identifies the type of notation it
    //      applies to.
    //
    //  fPublicId
    //      The text of the notation's public id, if any.
    //
    //  fSystemId
    //      The text of the notation's system id, if any.
    // -----------------------------------------------------------------------
    unsigned int    fId;
	XMLCh*          fName;
    XMLCh*          fPublicId;
    XMLCh*          fSystemId;
};


// -----------------------------------------------------------------------
//  Getter methods
// -----------------------------------------------------------------------
inline unsigned int XMLNotationDecl::getId() const
{
    return fId;
}

inline const XMLCh* XMLNotationDecl::getName() const
{
    return fName;
}

inline const XMLCh* XMLNotationDecl::getPublicId() const
{
    return fPublicId;
}

inline const XMLCh* XMLNotationDecl::getSystemId() const
{
    return fSystemId;
}


// -----------------------------------------------------------------------
//  Setter methods
// -----------------------------------------------------------------------
inline void XMLNotationDecl::setId(const unsigned int newId)
{
    fId = newId;
}

inline void XMLNotationDecl::setPublicId(const XMLCh* const newId)
{
    delete [] fPublicId;
    fPublicId = XMLString::replicate(newId);
}

inline void XMLNotationDecl::setSystemId(const XMLCh* const newId)
{
    delete [] fSystemId;
    fSystemId = XMLString::replicate(newId);
}


// ---------------------------------------------------------------------------
//  XMLNotationDecl: Support named pool element semantics
// ---------------------------------------------------------------------------
inline const XMLCh* XMLNotationDecl::getKey() const
{
    return fName;
}

#endif
