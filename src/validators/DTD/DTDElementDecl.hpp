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
 * Revision 1.1  1999/11/09 01:03:32  twl
 * Initial revision
 *
 * Revision 1.3  1999/11/08 20:45:40  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */


#if !defined(DTDELEMENTDECL_HPP)
#define DTDELEMENTDECL_HPP

#include <util/RefHashTableOf.hpp>
#include <framework/XMLElementDecl.hpp>
#include <validators/DTD/DTDAttDef.hpp>

class ContentSpecNode;
class DTDAttDefList;


//
//  This class is a derivative of the basic element decl. This one implements
//  the virtuals so that they work for a DTD. THe big difference is that
//  they don't live in any URL in the DTD. The names are just stored as full
//  QNames, so they are not split out and element decls don't live within
//  URL namespaces or anything like that.
//
class VALIDATORS_EXPORT DTDElementDecl : public XMLElementDecl
{
public :
    // -----------------------------------------------------------------------
    //  Class specific types
    //
    //  ModelTypes
    //      Indicates the type of content model that an element has. This
    //      indicates how the content model is represented and validated.
    // -----------------------------------------------------------------------
    enum ModelTypes
    {
        Empty
        , Any
        , Mixed
        , Children

        , ModelTypes_Count
    };


    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    DTDElementDecl();

    DTDElementDecl
    (
        const   XMLCh* const    elemQName
        , const ModelTypes      modelType = Any
    );

    ~DTDElementDecl();


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
    )   const;
    virtual XMLAttDefList& getAttDefList() const;
    virtual const XMLCh* getBaseName() const;
    virtual CharDataOpts getCharDataOpts() const;
    virtual const XMLCh* getFullName() const;
    virtual bool hasAttDefs() const;
    virtual bool resetDefs();


    // -----------------------------------------------------------------------
    //  Getter methods
    // -----------------------------------------------------------------------
    const DTDAttDef* getAttDef(const XMLCh* const attName) const;
    DTDAttDef* getAttDef(const XMLCh* const attName);
    ContentSpecNode* getContentSpec();
    const ContentSpecNode* getContentSpec() const;
    DTDElementDecl::ModelTypes getModelType() const;


    // -----------------------------------------------------------------------
    //  Setter methods
    // -----------------------------------------------------------------------
    void addAttDef(DTDAttDef* const toAdd);
    void setContentSpec(ContentSpecNode* toAdopt);
    void setModelType(const DTDElementDecl::ModelTypes toSet);
    void setName(const XMLCh* const newName);


protected :
    // -----------------------------------------------------------------------
    //  Protected, virtual methods
    // -----------------------------------------------------------------------
    virtual XMLContentModel* makeContentModel() const;
    virtual XMLCh* formatContentModel
    (
        const   XMLValidator&   validator
    )   const;


private :
    // -----------------------------------------------------------------------
    //  Private helper methods
    // -----------------------------------------------------------------------
    XMLContentModel* createChildModel() const;
    void faultInAttDefList() const;


    // -----------------------------------------------------------------------
    //  Private data members
    //
    //  fAttDefs
    //      The list of attributes that are defined for this element. Each
    //      element is its own little 'namespace' for attributes, so each
    //      element maintains its own list of owned attribute defs. It is
    //      faulted in when an attribute is actually added.
    //
    //  fAttList
    //      We have to return a view of our att defs via the abstract view
    //      that the scanner understands. It may or may not ever be asked
    //      for so we fault it in as needed.
    //
    //  fBaseName
    //      This is faulted in upon demand from the fQName field.
    //
    //  fContentSpec
    //      This is the content spec for the node. It contains the original
    //      content spec that was read from the DTD, as a tree of nodes. This
    //      one is always set up, and is used to build the fContentModel
    //      version if we are validating.
    //
    //  fQName
    //      This is the name of the element decl. Its just a QName that may
    //      or may not have a prefix: part. We don't care if it does or not.
    //      DTDs have no means of assigning namespaces to decls, so we don't
    //      have any concerns about that stuff at all.
    //
    //  fModelType
    //      The content model type of this element. This tells us what kind
    //      of content model to create.
    // -----------------------------------------------------------------------
    RefHashTableOf<DTDAttDef>*  fAttDefs;
    DTDAttDefList*              fAttList;
    XMLCh*                      fBaseName;
    ContentSpecNode*            fContentSpec;
    XMLCh*                      fQName;
    ModelTypes                  fModelType;
};


// ---------------------------------------------------------------------------
//  DTDElementDecl: Constructors and Destructor
// ---------------------------------------------------------------------------
inline DTDElementDecl::DTDElementDecl() :

    fAttDefs(0)
    , fAttList(0)
    , fBaseName(0)
    , fContentSpec(0)
    , fQName(0)
    , fModelType(Any)
{
}

inline
DTDElementDecl::DTDElementDecl( const   XMLCh* const                elemName
                                , const DTDElementDecl::ModelTypes  type) :
    fAttDefs(0)
    , fAttList(0)
    , fBaseName(0)
    , fContentSpec(0)
    , fQName(XMLString::replicate(elemName))
    , fModelType(type)
{
}


// ---------------------------------------------------------------------------
//  DTDElementDecl: XMLElementDecl virtual interface implementation
// ---------------------------------------------------------------------------
inline const XMLCh* DTDElementDecl::getFullName() const
{
    // Just return our QName
    return fQName;
}


// ---------------------------------------------------------------------------
//  DTDElementDecl: Getter methods
// ---------------------------------------------------------------------------
inline ContentSpecNode* DTDElementDecl::getContentSpec()
{
    return fContentSpec;
}

inline const ContentSpecNode* DTDElementDecl::getContentSpec() const
{
    return fContentSpec;
}

inline DTDElementDecl::ModelTypes DTDElementDecl::getModelType() const
{
    return fModelType;
}


// ---------------------------------------------------------------------------
//  DTDElementDecl: Setter methods
// ---------------------------------------------------------------------------
inline void
DTDElementDecl::setModelType(const DTDElementDecl::ModelTypes toSet)
{
    fModelType = toSet;
}

inline void DTDElementDecl::setName(const XMLCh* const newName)
{
    delete [] fQName;
    fQName = XMLString::replicate(newName);
}

#endif
