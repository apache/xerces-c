/*
 * Copyright 2001,2004 The Apache Software Foundation.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *      http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * $Log$
 * Revision 1.9  2004/11/25 14:36:40  knoaman
 * Fix problem with an All content model with minOccurs of 0.
 *
 * Revision 1.8  2004/09/16 13:32:03  amassari
 * Updated error message for UPA to also state the complex type that is failing the test
 *
 * Revision 1.7  2004/09/08 13:56:51  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.6  2004/01/29 11:51:21  cargilld
 * Code cleanup changes to get rid of various compiler diagnostic messages.
 *
 * Revision 1.5  2003/05/16 21:43:20  knoaman
 * Memory manager implementation: Modify constructors to pass in the memory manager.
 *
 * Revision 1.4  2003/05/15 18:48:27  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.3  2003/03/07 18:16:57  tng
 * Return a reference instead of void for operator=
 *
 * Revision 1.2  2002/11/04 14:54:58  tng
 * C++ Namespace Support.
 *
 * Revision 1.1.1.1  2002/02/01 22:22:37  peiyongz
 * sane_include
 *
 * Revision 1.2  2001/11/21 14:30:13  knoaman
 * Fix for UPA checking.
 *
 * Revision 1.1  2001/08/24 12:48:48  tng
 * Schema: AllContentModel
 *
 */


#if !defined(ALLCONTENTMODEL_HPP)
#define ALLCONTENTMODEL_HPP

#include <xercesc/framework/XMLContentModel.hpp>
#include <xercesc/util/ValueVectorOf.hpp>
#include <xercesc/validators/common/ContentLeafNameTypeVector.hpp>

XERCES_CPP_NAMESPACE_BEGIN

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
          ContentSpecNode* const parentContentSpec
		, const bool             isMixed
        , MemoryManager* const   manager = XMLPlatformUtils::fgMemoryManager
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
      , const XMLCh*            pComplexTypeName = 0
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
    AllContentModel& operator=(const AllContentModel&);


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
    MemoryManager* fMemoryManager;
    unsigned int    fCount;
    QName**         fChildren;
    bool*           fChildOptional;
    unsigned int    fNumRequired;
    bool            fIsMixed;
    bool            fHasOptionalContent;
};

inline ContentLeafNameTypeVector* AllContentModel::getContentLeafNameTypeVector() const
{
	return 0;
}

inline unsigned int
AllContentModel::getNextState(const unsigned int,
                              const unsigned int) const {

    return XMLContentModel::gInvalidTrans;
}

XERCES_CPP_NAMESPACE_END

#endif

