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
 * Revision 1.2  2000/02/06 07:47:47  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.1.1.1  1999/11/09 01:08:28  twl
 * Initial checkin
 *
 * Revision 1.2  1999/11/08 20:44:35  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */

#if !defined(XMLATTR_HPP)
#define XMLATTR_HPP

#include <util/XMLString.hpp>
#include <framework/XMLAttDef.hpp>


//
//  This class defines the information about an attribute that will come out
//  of the scanner during parsing. This information does not depend upon the
//  type of validator because it is not tied to any scheme/DTD type info. Its
//  just the raw XML 1.0 information that will be reported about an attribute
//  in the startElement() callback method of the XMLDocumentHandler class.
//  Hence it is not intended to be extended or derived from. Its designed to
//  be used as is.
//
//  The 'specified' field of this class indicates whether the attribute was
//  actually present or whether it was faulted in because it had a fixed or
//  default value.
//
//  The code receiving this information can ask its validator for more info
//  about the attribute, i.e. get its declaration from the DTD/Schema info.
//
class XMLPARSER_EXPORT XMLAttr
{
public:
    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    XMLAttr();
    XMLAttr
    (
        const   unsigned int        uriId
        , const XMLCh* const        attrName
        , const XMLCh* const        attrPrefix
        , const XMLCh* const        attrValue
        , const XMLAttDef::AttTypes type = XMLAttDef::CData
        , const bool                specified = true
    );
    ~XMLAttr();


    // -----------------------------------------------------------------------
    //  Getter methods
    // -----------------------------------------------------------------------
    const XMLCh* getName() const;
    const XMLCh* getPrefix() const;
    const XMLCh* getQName() const;
    bool getSpecified() const;
    XMLAttDef::AttTypes getType() const;
    const XMLCh* getValue() const;
    unsigned int getURIId() const;


    // -----------------------------------------------------------------------
    //  Setter methods
    // -----------------------------------------------------------------------
    void set
    (
        const   unsigned int        uriId
        , const XMLCh* const        attrName
        , const XMLCh* const        attrPrefix
        , const XMLCh* const        attrValue
        , const XMLAttDef::AttTypes type = XMLAttDef::CData
    );
    void setName
    (
        const   unsigned int        uriId
        , const XMLCh* const        attrName
        , const XMLCh* const        attrPrefix
    );
    void setSpecified(const bool newValue);
    void setType(const XMLAttDef::AttTypes newValue);
    void setURIId(const unsigned int uriId);
    void setValue(const XMLCh* const newValue);


private :
    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    XMLAttr(const XMLAttr&);
    XMLAttr& operator=(const XMLAttr&);


    // -----------------------------------------------------------------------
    //  Private, helper methods
    // -----------------------------------------------------------------------
    void cleanUp();


    // -----------------------------------------------------------------------
    //  Private instance variables
    //
    //  fName
    //      The base part of the name of the attribute.
    //
    //  fPrefix
    //      The prefix that was applied to this attribute's name. It really
    //      does not matter technically but it might be required for pratical
    //      reasons, to recreate the original document for instance.
    //
    //  fQName
    //      This is the QName form of the name, which is faulted in (from the
    //      prefix and name) upon request.
    //
    //  fSpecified
    //      True if this attribute appeared in the element; else, false if
    //      it was defaulted from an AttDef.
    //
    //  fType
    //      The attribute type enum value for this attribute. Indicates what
    //      type of attribute it was.
    //
    //  fValue
    //      The attribute value that was given in the attribute instance.
    //
    //  fURIId
    //      The id of the URI that this attribute belongs to.
    // -----------------------------------------------------------------------
    XMLCh*              fName;
    XMLCh*              fPrefix;
    XMLCh*              fQName;
    bool                fSpecified;
    XMLAttDef::AttTypes fType;
    XMLCh*              fValue;
    unsigned int        fURIId;
};

// ---------------------------------------------------------------------------
//  XMLAttr: Constructors and Destructor
// ---------------------------------------------------------------------------
inline XMLAttr::~XMLAttr()
{
    cleanUp();
}


// ---------------------------------------------------------------------------
//  XMLAttr: Getter methods
// ---------------------------------------------------------------------------
inline const XMLCh* XMLAttr::getName() const
{
    return fName;
}

inline const XMLCh* XMLAttr::getPrefix() const
{
    return fPrefix;
}

inline bool XMLAttr::getSpecified() const
{
    return fSpecified;
}

inline XMLAttDef::AttTypes XMLAttr::getType() const
{
    return fType;
}

inline const XMLCh* XMLAttr::getValue() const
{
    return fValue;
}

inline unsigned int XMLAttr::getURIId() const
{
    return fURIId;
}


// ---------------------------------------------------------------------------
//  XMLAttr: Setter methods
// ---------------------------------------------------------------------------
inline void XMLAttr::setType(const XMLAttDef::AttTypes newValue)
{
    fType = newValue;
}

inline void XMLAttr::setSpecified(const bool newValue)
{
    fSpecified = newValue;
}

#endif
