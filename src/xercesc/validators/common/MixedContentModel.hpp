/*
 * Copyright 1999-2001,2004 The Apache Software Foundation.
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
 * Revision 1.10  2005/01/06 21:39:44  amassari
 * Removed warnings
 *
 * Revision 1.9  2004/09/16 13:32:04  amassari
 * Updated error message for UPA to also state the complex type that is failing the test
 *
 * Revision 1.8  2004/09/08 13:56:51  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.7  2004/01/29 11:51:21  cargilld
 * Code cleanup changes to get rid of various compiler diagnostic messages.
 *
 * Revision 1.6  2003/05/16 21:43:20  knoaman
 * Memory manager implementation: Modify constructors to pass in the memory manager.
 *
 * Revision 1.5  2003/05/15 18:48:27  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.4  2003/03/07 18:16:57  tng
 * Return a reference instead of void for operator=
 *
 * Revision 1.3  2002/11/04 14:54:58  tng
 * C++ Namespace Support.
 *
 * Revision 1.2  2002/02/25 21:18:53  tng
 * Schema Fix: Ensure no invalid uri index for UPA checking.
 *
 * Revision 1.1.1.1  2002/02/01 22:22:39  peiyongz
 * sane_include
 *
 * Revision 1.12  2001/11/28 16:46:20  tng
 * Schema fix: Check for invalid URI index first.
 *
 * Revision 1.11  2001/11/21 14:30:13  knoaman
 * Fix for UPA checking.
 *
 * Revision 1.10  2001/08/21 16:06:11  tng
 * Schema: Unique Particle Attribution Constraint Checking.
 *
 * Revision 1.9  2001/08/13 15:06:39  knoaman
 * update <any> validation.
 *
 * Revision 1.8  2001/05/11 13:27:19  tng
 * Copyright update.
 *
 * Revision 1.7  2001/05/03 21:02:31  tng
 * Schema: Add SubstitutionGroupComparator and update exception messages.  By Pei Yong Zhang.
 *
 * Revision 1.6  2001/04/19 18:17:33  tng
 * Schema: SchemaValidator update, and use QName in Content Model
 *
 * Revision 1.5  2001/03/21 21:56:28  tng
 * Schema: Add Schema Grammar, Schema Validator, and split the DTDValidator into DTDValidator, DTDScanner, and DTDGrammar.
 *
 * Revision 1.4  2001/03/21 19:29:58  tng
 * Schema: Content Model Updates, by Pei Yong Zhang.
 *
 * Revision 1.3  2001/02/27 18:32:33  tng
 * Schema: Use XMLElementDecl instead of DTDElementDecl in Content Model.
 *
 * Revision 1.2  2001/02/27 14:48:55  tng
 * Schema: Add CMAny and ContentLeafNameTypeVector, by Pei Yong Zhang
 *
 * Revision 1.1  2001/02/16 14:17:29  tng
 * Schema: Move the common Content Model files that are shared by DTD
 * and schema from 'DTD' folder to 'common' folder.  By Pei Yong Zhang.
 *
 * Revision 1.3  2000/02/24 20:16:49  abagchi
 * Swat for removing Log from API docs
 *
 * Revision 1.2  2000/02/09 21:42:39  abagchi
 * Copyright swat
 *
 * Revision 1.1.1.1  1999/11/09 01:03:45  twl
 * Initial checkin
 *
 * Revision 1.3  1999/11/08 20:45:43  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */


#if !defined(MIXEDCONTENTMODEL_HPP)
#define MIXEDCONTENTMODEL_HPP

#include <xercesc/util/ValueVectorOf.hpp>
#include <xercesc/framework/XMLContentModel.hpp>
#include <xercesc/validators/common/ContentLeafNameTypeVector.hpp>

XERCES_CPP_NAMESPACE_BEGIN

class ContentSpecNode;

//
//  MixedContentModel is a derivative of the abstract content model base
//  class that handles the special case of mixed model elements. If an element
//  is mixed model, it has PCDATA as its first possible content, followed
//  by an alternation of the possible children. The children cannot have any
//  numeration or order, so it must look like this:
//
//  <!ELEMENT Foo ((#PCDATA|a|b|c|)*)>
//
//  So, all we have to do is to keep an array of the possible children and
//  validate by just looking up each child being validated by looking it up
//  in the list.
//
class MixedContentModel : public XMLContentModel
{
public :
    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    MixedContentModel
    (
        const bool                dtd
        , ContentSpecNode* const  parentContentSpec
		, const bool              ordered = false
        , MemoryManager* const    manager = XMLPlatformUtils::fgMemoryManager
    );

    ~MixedContentModel();


    // -----------------------------------------------------------------------
    //  Getter methods
    // -----------------------------------------------------------------------
    bool hasDups() const;

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
      , ValueVectorOf<ContentSpecNode::NodeTypes>& toType
    );

    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    MixedContentModel();
    MixedContentModel(const MixedContentModel&);
    MixedContentModel& operator=(const MixedContentModel&);


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
    //  fChildTypes
    //      The type of the children to support ANY.
    //
    //  fOrdered
    //      True if mixed content model is ordered. DTD mixed content models
    //      are <em>always</em> unordered.
    //
    //  fDTD
    //      Boolean to allow DTDs to validate even with namespace support.
    //
    // -----------------------------------------------------------------------
    unsigned int                fCount;
    QName**                     fChildren;
    ContentSpecNode::NodeTypes* fChildTypes;
    bool                        fOrdered;
    bool                        fDTD;
    MemoryManager*              fMemoryManager;
};

inline ContentLeafNameTypeVector* MixedContentModel::getContentLeafNameTypeVector() const
{
	return 0;
}

inline unsigned int
MixedContentModel::getNextState(const unsigned int,
                                const unsigned int) const {

    return XMLContentModel::gInvalidTrans;
}

inline void MixedContentModel::checkUniqueParticleAttribution
    (
        SchemaGrammar*    const 
      , GrammarResolver*  const 
      , XMLStringPool*    const 
      , XMLValidator*     const
      , unsigned int*     const pContentSpecOrgURI
      , const XMLCh*            /*pComplexTypeName*/ /*= 0*/
    )
{
    // rename back
    unsigned int i = 0;
    for (i = 0; i < fCount; i++) {
        unsigned int orgURIIndex = fChildren[i]->getURI();
        if ((orgURIIndex != XMLContentModel::gEOCFakeId) &&
            (orgURIIndex != XMLElementDecl::fgInvalidElemId) &&
            (orgURIIndex != XMLElementDecl::fgPCDataElemId))
            fChildren[i]->setURI(pContentSpecOrgURI[orgURIIndex]);
    }

    // for mixed content model, it's only a sequence
    // UPA checking is not necessary
}

XERCES_CPP_NAMESPACE_END

#endif
