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
 * Revision 1.12  2004/09/08 13:56:51  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.11  2004/08/11 21:08:31  knoaman
 * Fixing Xerces-C crash when creating a MixedContentModel. Patch by  Andrew Fang.
 *
 * Revision 1.10  2004/01/29 11:51:21  cargilld
 * Code cleanup changes to get rid of various compiler diagnostic messages.
 *
 * Revision 1.9  2003/12/17 00:18:38  cargilld
 * Update to memory management so that the static memory manager (one used to call Initialize) is only for static data.
 *
 * Revision 1.8  2003/11/20 18:12:20  knoaman
 * Use a bitwise operation to check the node type.
 *
 * Revision 1.7  2003/11/07 17:08:11  knoaman
 * For PSVI support, distinguish wildcard elements with namespace lists.
 *
 * Revision 1.6  2003/05/18 14:02:06  knoaman
 * Memory manager implementation: pass per instance manager.
 *
 * Revision 1.5  2003/05/16 21:43:20  knoaman
 * Memory manager implementation: Modify constructors to pass in the memory manager.
 *
 * Revision 1.4  2003/05/15 18:48:27  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.3  2002/11/04 14:54:58  tng
 * C++ Namespace Support.
 *
 * Revision 1.2  2002/09/24 19:48:39  tng
 * Performance: use XMLString::equals instead of XMLString::compareString
 *
 * Revision 1.1.1.1  2002/02/01 22:22:39  peiyongz
 * sane_include
 *
 * Revision 1.13  2001/08/24 12:48:48  tng
 * Schema: AllContentModel
 *
 * Revision 1.12  2001/08/21 16:06:11  tng
 * Schema: Unique Particle Attribution Constraint Checking.
 *
 * Revision 1.11  2001/07/09 15:22:37  knoaman
 * complete <any> declaration.
 *
 * Revision 1.10  2001/06/12 22:13:33  peiyongz
 * validateContentSpecial() : apply SubstitutionGroupComparator.isEquivalentTo()
 *
 * Revision 1.9  2001/05/11 13:27:19  tng
 * Copyright update.
 *
 * Revision 1.8  2001/05/03 21:02:30  tng
 * Schema: Add SubstitutionGroupComparator and update exception messages.  By Pei Yong Zhang.
 *
 * Revision 1.7  2001/04/19 18:17:32  tng
 * Schema: SchemaValidator update, and use QName in Content Model
 *
 * Revision 1.6  2001/03/21 21:56:28  tng
 * Schema: Add Schema Grammar, Schema Validator, and split the DTDValidator into DTDValidator, DTDScanner, and DTDGrammar.
 *
 * Revision 1.5  2001/03/21 19:29:57  tng
 * Schema: Content Model Updates, by Pei Yong Zhang.
 *
 * Revision 1.4  2001/02/27 18:32:32  tng
 * Schema: Use XMLElementDecl instead of DTDElementDecl in Content Model.
 *
 * Revision 1.3  2001/02/27 14:48:54  tng
 * Schema: Add CMAny and ContentLeafNameTypeVector, by Pei Yong Zhang
 *
 * Revision 1.2  2001/02/16 14:58:57  tng
 * Schema: Update Makefile, configure files, project files, and include path in
 * certain cpp files because of the move of the common Content Model files.  By Pei Yong Zhang.
 *
 * Revision 1.1  2001/02/16 14:17:29  tng
 * Schema: Move the common Content Model files that are shared by DTD
 * and schema from 'DTD' folder to 'common' folder.  By Pei Yong Zhang.
 *
 * Revision 1.5  2000/05/15 22:31:32  andyh
 * Replace #include<memory.h> with <string.h> everywhere.
 *
 * Revision 1.4  2000/03/18 00:00:05  roddey
 * Initial updates for two way transcoding support
 *
 * Revision 1.3  2000/03/02 19:55:39  roddey
 * This checkin includes many changes done while waiting for the
 * 1.1.0 code to be finished. I can't list them all here, but a list is
 * available elsewhere.
 *
 * Revision 1.2  2000/02/09 21:42:39  abagchi
 * Copyright swatswat
 *
 * Revision 1.1.1.1  1999/11/09 01:03:43  twl
 * Initial checkin
 *
 * Revision 1.3  1999/11/08 20:45:43  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <string.h>
#include <xercesc/util/RuntimeException.hpp>
#include <xercesc/framework/XMLElementDecl.hpp>
#include <xercesc/validators/common/ContentSpecNode.hpp>
#include <xercesc/validators/common/MixedContentModel.hpp>
#include <xercesc/validators/common/CMStateSet.hpp>
#include <xercesc/validators/common/Grammar.hpp>
#include <xercesc/validators/schema/SubstitutionGroupComparator.hpp>

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  MixedContentModel: Constructors and Destructor
// ---------------------------------------------------------------------------
MixedContentModel::MixedContentModel(const bool             dtd
                                   , ContentSpecNode* const parentContentSpec
                                   , const bool             ordered
                                   , MemoryManager* const   manager) :
   fCount(0)
 , fChildren(0)
 , fChildTypes(0)
 , fOrdered(ordered)
 , fDTD(dtd)
 , fMemoryManager(manager)
{
    //
    //  Create a vector of unsigned ints that will be filled in with the
    //  ids of the child nodes. It will be expanded as needed but we give
    //  it an initial capacity of 64 which should be more than enough for
    //  99% of the scenarios.
    //
    ValueVectorOf<QName*> children(64, fMemoryManager);
    ValueVectorOf<ContentSpecNode::NodeTypes> childTypes(64, fMemoryManager);

    //
    //  Get the parent element's content spec. This is the head of the tree
    //  of nodes that describes the content model. We will iterate this
    //  tree.
    //
    ContentSpecNode* curNode = parentContentSpec;
    if (!curNode)
        ThrowXMLwithMemMgr(RuntimeException, XMLExcepts::CM_NoParentCSN, fMemoryManager);

    // And now call the private recursive method that iterates the tree
    buildChildList(curNode, children, childTypes);

    //
    //  And now we know how many elements we need in our member list. So
    //  fill them in.
    //
    fCount = children.size();
    fChildren = (QName**) fMemoryManager->allocate(fCount * sizeof(QName*)); //new QName*[fCount];
    fChildTypes = (ContentSpecNode::NodeTypes*) fMemoryManager->allocate
    (
        fCount * sizeof(ContentSpecNode::NodeTypes)
    ); //new ContentSpecNode::NodeTypes[fCount];
    for (unsigned int index = 0; index < fCount; index++) {
        fChildren[index] = new (fMemoryManager) QName(*children.elementAt(index));
        fChildTypes[index] = childTypes.elementAt(index);
    }
}

MixedContentModel::~MixedContentModel()
{
    for (unsigned int index = 0; index < fCount; index++) {
        delete fChildren[index];
    }
    fMemoryManager->deallocate(fChildren); //delete [] fChildren;
    fMemoryManager->deallocate(fChildTypes); //delete [] fChildTypes;
}


// ---------------------------------------------------------------------------
//  MixedContentModel: Getter methods
// ---------------------------------------------------------------------------
bool MixedContentModel::hasDups() const
{
    // Can't have dups if only one child
    if (fCount == 1)
        return false;

    for (unsigned int index = 0; index < fCount; index++)
    {
        const QName* curVal = fChildren[index];
        for (unsigned int iIndex = 0; iIndex < fCount; iIndex++)
        {
            if (iIndex == index)
                continue;

            if (fDTD) {
                if (XMLString::equals(curVal->getRawName(), fChildren[iIndex]->getRawName())) {
                    return true;
                }
            }
            else {
                if ((curVal->getURI() == fChildren[iIndex]->getURI()) &&
                    (XMLString::equals(curVal->getLocalPart(), fChildren[iIndex]->getLocalPart()))) {
                    return true;
                }
            }
        }
    }
    return false;
}


// ---------------------------------------------------------------------------
//  MixedContentModel: Implementation of the ContentModel virtual interface
// ---------------------------------------------------------------------------
//
//Under the XML Schema mixed model,
//the order and number of child elements appearing in an instance
//must agree with
//the order and number of child elements specified in the model.
//
int
MixedContentModel::validateContent( QName** const         children
                                  , const unsigned int    childCount
                                  , const unsigned int) const
{
    // must match order
    if (fOrdered) {
        unsigned int inIndex = 0;
        for (unsigned int outIndex = 0; outIndex < childCount; outIndex++) {

            // Get the current child out of the source index
            const QName* curChild = children[outIndex];

            // If its PCDATA, then we just accept that
            if (curChild->getURI() == XMLElementDecl::fgPCDataElemId)
                continue;

            ContentSpecNode::NodeTypes type = fChildTypes[inIndex];
            const QName* inChild = fChildren[inIndex];

            if (type == ContentSpecNode::Leaf) {
                if (fDTD) {
                    if (!XMLString::equals(inChild->getRawName(), curChild->getRawName())) {
                        return outIndex;
                    }
                }
                else {
                    if ((inChild->getURI() != curChild->getURI()) ||
                        (!XMLString::equals(inChild->getLocalPart(), curChild->getLocalPart()))) {
                        return outIndex;
                    }
                }
            }
            else if (type == ContentSpecNode::Any) {
            }
            else if (type == ContentSpecNode::Any_NS) {
                if (inChild->getURI() != curChild->getURI())
                    return outIndex;
            }
            else if (type == ContentSpecNode::Any_Other) {
                if (inChild->getURI() == curChild->getURI())
                    return outIndex;
            }

            // advance index
            inIndex++;
        }
    }

    // can appear in any order
    else {
        for (unsigned int outIndex = 0; outIndex < childCount; outIndex++) {
            // Get the current child out of the source index
            const QName* curChild = children[outIndex];

            // If its PCDATA, then we just accept that
            if (curChild->getURI() == XMLElementDecl::fgPCDataElemId)
                continue;

            // And try to find it in our list
            unsigned int inIndex = 0;
            for (; inIndex < fCount; inIndex++)
            {
                ContentSpecNode::NodeTypes type = fChildTypes[inIndex];
                const QName* inChild = fChildren[inIndex];

                if (type == ContentSpecNode::Leaf) {
                    if (fDTD) {
                        if (XMLString::equals(inChild->getRawName(), curChild->getRawName())) {
                            break;
                        }
                    }
                    else {
                        if ((inChild->getURI() == curChild->getURI()) &&
                            (XMLString::equals(inChild->getLocalPart(), curChild->getLocalPart()))) {
                            break;
                        }
                    }
                }
                else if (type == ContentSpecNode::Any) {
                    break;
                }
                else if (type == ContentSpecNode::Any_NS) {
                    if (inChild->getURI() == curChild->getURI())
                        break;
                }
                else if (type == ContentSpecNode::Any_Other) {
                    if (inChild->getURI() != curChild->getURI())
                        break;
                }

                // REVISIT: What about checking for multiple ANY matches?
                //          The content model ambiguity *could* be checked
                //          by the caller before constructing the mixed
                //          content model.
            }
            // We did not find this one, so the validation failed
            if (inIndex == fCount)
                return outIndex;
        }
    }

    // Everything seems to be in order, so return success
    // success
    return -1;
}


int MixedContentModel::validateContentSpecial(QName** const           children
                                            , const unsigned int      childCount
                                            , const unsigned int
                                            , GrammarResolver*  const pGrammarResolver
                                            , XMLStringPool*    const pStringPool) const
{

    SubstitutionGroupComparator comparator(pGrammarResolver, pStringPool);

    // must match order
    if (fOrdered) {
        unsigned int inIndex = 0;
        for (unsigned int outIndex = 0; outIndex < childCount; outIndex++) {

            // Get the current child out of the source index
            QName* curChild = children[outIndex];

            // If its PCDATA, then we just accept that
            if (curChild->getURI() == XMLElementDecl::fgPCDataElemId)
                continue;

            ContentSpecNode::NodeTypes type = fChildTypes[inIndex];
            QName* inChild = fChildren[inIndex];

            if (type == ContentSpecNode::Leaf) {
                if ( !comparator.isEquivalentTo(curChild, inChild))
                    return outIndex;
            }
            else if (type == ContentSpecNode::Any) {
            }
            else if (type == ContentSpecNode::Any_NS) {
                if (inChild->getURI() != curChild->getURI())
                    return outIndex;
            }
            else if (type == ContentSpecNode::Any_Other) {
                if (inChild->getURI() == curChild->getURI())
                    return outIndex;
            }

            // advance index
            inIndex++;
        }
    }

    // can appear in any order
    else {
        for (unsigned int outIndex = 0; outIndex < childCount; outIndex++) {
            // Get the current child out of the source index
            QName* curChild = children[outIndex];

            // If its PCDATA, then we just accept that
            if (curChild->getURI() == XMLElementDecl::fgPCDataElemId)
                continue;

            // And try to find it in our list
            unsigned int inIndex = 0;
            for (; inIndex < fCount; inIndex++)
            {
                ContentSpecNode::NodeTypes type = fChildTypes[inIndex];
                QName* inChild = fChildren[inIndex];

                if (type == ContentSpecNode::Leaf) {
                    if ( comparator.isEquivalentTo(curChild, inChild))
                        break;
                }
                else if (type == ContentSpecNode::Any) {
                    break;
                }
                else if (type == ContentSpecNode::Any_NS) {
                    if (inChild->getURI() == curChild->getURI())
                        break;
                }
                else if (type == ContentSpecNode::Any_Other) {
                    if (inChild->getURI() != curChild->getURI())
                        break;
                }

                // REVISIT: What about checking for multiple ANY matches?
                //          The content model ambiguity *could* be checked
                //          by the caller before constructing the mixed
                //          content model.
            }
            // We did not find this one, so the validation failed
            if (inIndex == fCount)
                return outIndex;
        }
    }

    // Everything seems to be in order, so return success
    // success
    return -1;
}

// ---------------------------------------------------------------------------
//  MixedContentModel: Private helper methods
// ---------------------------------------------------------------------------
void
MixedContentModel::buildChildList(  ContentSpecNode* const       curNode
                                  , ValueVectorOf<QName*>&       toFill
                                  , ValueVectorOf<ContentSpecNode::NodeTypes>& toType)
{
    // Get the type of spec node our current node is
    const ContentSpecNode::NodeTypes curType = curNode->getType();

    // If its a leaf, then store its id in the target list
    if ((curType == ContentSpecNode::Leaf)      ||
        (curType == ContentSpecNode::Any)       ||
        (curType == ContentSpecNode::Any_Other) ||
        (curType == ContentSpecNode::Any_NS)   )
    {
        toFill.addElement(curNode->getElement());
        toType.addElement(curType);
        return;
    }

    // Get both the child node pointers
    ContentSpecNode* leftNode = curNode->getFirst();
    ContentSpecNode* rightNode = curNode->getSecond();

    // And recurse according to the type of node
    if (((curType & 0x0f) == ContentSpecNode::Choice)
    ||  ((curType & 0x0f) == ContentSpecNode::Sequence))
    {
        // Recurse on the left and right nodes
        buildChildList(leftNode, toFill, toType);

        // The last node of a choice or sequence has a null right
        if (rightNode)
            buildChildList(rightNode, toFill, toType);
    }
    else if ((curType == ContentSpecNode::OneOrMore)
         ||  (curType == ContentSpecNode::ZeroOrOne)
         ||  (curType == ContentSpecNode::ZeroOrMore))
    {
        // Just do the left node on this one
        buildChildList(leftNode, toFill, toType);
    }
}

XERCES_CPP_NAMESPACE_END

