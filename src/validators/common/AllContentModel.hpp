/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 2001 The Apache Software Foundation.  All rights
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
 * originally based on software copyright (c) 2001, International
 * Business Machines, Inc., http://www.ibm.com .  For more information
 * on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 */

/*
 * $Log$
 * Revision 1.2  2001/11/21 14:30:13  knoaman
 * Fix for UPA checking.
 *
 * Revision 1.1  2001/08/24 12:48:48  tng
 * Schema: AllContentModel
 *
 */


#if !defined(ALLCONTENTMODEL_HPP)
#define ALLCONTENTMODEL_HPP

#include <util/ValueVectorOf.hpp>
#include <framework/XMLContentModel.hpp>
#include <validators/common/ContentLeafNameTypeVector.hpp>

class ContentSpecNode;

//
//  AllContentModel is a derivative of the abstract content model base
//  class that handles the special case of <all> feature in schema. If a model
//  is <all>, all non-optional children must appear
//
//  So, all we have to do is to keep an array of the possible children and
//  validate by just looking up each child being validated by looking it up
//  in the list, and make sure all non-optional children appear.
//
class AllContentModel : public XMLContentModel
{
public :
    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    AllContentModel
    (
      ContentSpecNode* const  parentContentSpec
		, const bool            isMixed
    );

    ~AllContentModel();

    // -----------------------------------------------------------------------
    //  Implementation of the ContentModel virtual interface
    // -----------------------------------------------------------------------
    virtual int validateContent
    (
        QName** const         children
      , const unsigned int    childCount
      , const unsigned int    emptyNamespaceId
    )   const;

	virtual int validateContentSpecial
    (
        QName** const         children
      , const unsigned int    childCount
      , const unsigned int    emptyNamespaceId
      , GrammarResolver*  const pGrammarResolver
      , XMLStringPool*    const pStringPool
    ) const;

    virtual ContentLeafNameTypeVector* getContentLeafNameTypeVector() const ;

    virtual unsigned int getNextState(const unsigned int currentState,
                                      const unsigned int elementIndex) const;

    virtual void checkUniqueParticleAttribution
    (
        SchemaGrammar*    const pGrammar
      , GrammarResolver*  const pGrammarResolver
      , XMLStringPool*    const pStringPool
      , XMLValidator*     const pValidator
      , unsigned int*     const pContentSpecOrgURI
    ) ;

private :
    // -----------------------------------------------------------------------
    //  Private helper methods
    // -----------------------------------------------------------------------
    void buildChildList
    (
        ContentSpecNode* const                     curNode
      , ValueVectorOf<QName*>&                     toFill
      , ValueVectorOf<bool>&                       toType
    );

    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    AllContentModel();
    AllContentModel(const AllContentModel&);
    void operator=(const AllContentModel&);


    // -----------------------------------------------------------------------
    //  Private data members
    //
    //  fCount
    //      The count of possible children in the fChildren member.
    //
    //  fChildren
    //      The list of possible children that we have to accept. This array
    //      is allocated as large as needed in the constructor.
    //
    //  fChildOptional
    //      The corresponding list of optional state of each child in fChildren
    //      True if the child is optional (i.e. minOccurs = 0).
    //
    //  fNumRequired
    //      The number of required children in <all> (i.e. minOccurs = 1)
    //
    //  fIsMixed
    //      AllContentModel with mixed PCDATA.
    // -----------------------------------------------------------------------
    unsigned int    fCount;
    QName**         fChildren;
    bool*           fChildOptional;
    unsigned int    fNumRequired;
    bool            fIsMixed;

};

inline ContentLeafNameTypeVector* AllContentModel::getContentLeafNameTypeVector() const
{
	return 0;
}

inline unsigned int
AllContentModel::getNextState(const unsigned int currentState,
                              const unsigned int elementIndex) const {

    return XMLContentModel::gInvalidTrans;
}

#endif

