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
 * Revision 1.10  2004/09/16 13:32:04  amassari
 * Updated error message for UPA to also state the complex type that is failing the test
 *
 * Revision 1.9  2004/09/08 13:56:51  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.8  2004/01/29 11:51:21  cargilld
 * Code cleanup changes to get rid of various compiler diagnostic messages.
 *
 * Revision 1.7  2003/12/17 00:18:38  cargilld
 * Update to memory management so that the static memory manager (one used to call Initialize) is only for static data.
 *
 * Revision 1.6  2003/11/20 18:12:20  knoaman
 * Use a bitwise operation to check the node type.
 *
 * Revision 1.5  2003/11/07 17:08:11  knoaman
 * For PSVI support, distinguish wildcard elements with namespace lists.
 *
 * Revision 1.4  2002/11/04 14:54:58  tng
 * C++ Namespace Support.
 *
 * Revision 1.3  2002/09/24 19:48:39  tng
 * Performance: use XMLString::equals instead of XMLString::compareString
 *
 * Revision 1.2  2002/02/25 21:18:53  tng
 * Schema Fix: Ensure no invalid uri index for UPA checking.
 *
 * Revision 1.1.1.1  2002/02/01 22:22:39  peiyongz
 * sane_include
 *
 * Revision 1.11  2001/11/28 16:46:20  tng
 * Schema fix: Check for invalid URI index first.
 *
 * Revision 1.10  2001/11/21 14:30:13  knoaman
 * Fix for UPA checking.
 *
 * Revision 1.9  2001/08/21 16:06:11  tng
 * Schema: Unique Particle Attribution Constraint Checking.
 *
 * Revision 1.8  2001/05/11 13:27:19  tng
 * Copyright update.
 *
 * Revision 1.7  2001/05/03 21:02:32  tng
 * Schema: Add SubstitutionGroupComparator and update exception messages.  By Pei Yong Zhang.
 *
 * Revision 1.6  2001/04/19 18:17:33  tng
 * Schema: SchemaValidator update, and use QName in Content Model
 *
 * Revision 1.5  2001/03/21 21:56:29  tng
 * Schema: Add Schema Grammar, Schema Validator, and split the DTDValidator into DTDValidator, DTDScanner, and DTDGrammar.
 *
 * Revision 1.4  2001/03/21 19:30:00  tng
 * Schema: Content Model Updates, by Pei Yong Zhang.
 *
 * Revision 1.3  2001/02/27 14:48:57  tng
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
 * Revision 1.4  2000/03/03 22:33:00  roddey
 * Fixed a bug in SimpleContentModel that allowed an <a/> to be taken
 * as valid for a content model of (a,b).
 *
 * Revision 1.3  2000/03/02 19:55:40  roddey
 * This checkin includes many changes done while waiting for the
 * 1.1.0 code to be finished. I can't list them all here, but a list is
 * available elsewhere.
 *
 * Revision 1.2  2000/02/09 21:42:39  abagchi
 * Copyright swatswat
 *
 * Revision 1.1.1.1  1999/11/09 01:03:46  twl
 * Initial checkin
 *
 * Revision 1.2  1999/11/08 20:45:44  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/util/RuntimeException.hpp>
#include <xercesc/framework/XMLValidator.hpp>
#include <xercesc/validators/common/SimpleContentModel.hpp>
#include <xercesc/validators/schema/SubstitutionGroupComparator.hpp>
#include <xercesc/validators/schema/XercesElementWildcard.hpp>

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  SimpleContentModel: Implementation of the ContentModel virtual interface
// ---------------------------------------------------------------------------
//
//  This method is called to validate our content. For this one, its just a
//  pretty simple 'bull your way through it' test according to what kind of
//  operation it is for.
//
int
SimpleContentModel::validateContent(QName** const       children
                                  , const unsigned int  childCount
                                  , const unsigned int) const
{
    //
    //  According to the type of operation, we do the correct type of
    //  content check.
    //
    unsigned int index;
    switch(fOp & 0x0f)
    {
        case ContentSpecNode::Leaf :
            //
            //  There can only be one child and it has to be of the
            //  element type we stored.
            //
            if (!childCount)
                return 0;

            // If the 0th child is not the right kind, report an error at 0
            if (fDTD) {
                if (!XMLString::equals(children[0]->getRawName(), fFirstChild->getRawName())) {
                    return 0;
                }
            }
            else {
                if ((children[0]->getURI() != fFirstChild->getURI()) ||
                    !XMLString::equals(children[0]->getLocalPart(), fFirstChild->getLocalPart())) {
                    return 0;
                }
            }

            if (childCount > 1)
                return 1;
            break;

        case ContentSpecNode::ZeroOrOne :
            //
            //  If the child count is greater than one, then obviously
            //  bad. Otherwise, if its one, then the one child must be
            //  of the type we stored.
            //
            if (childCount == 1) {
                if (fDTD) {
                    if (!XMLString::equals(children[0]->getRawName(), fFirstChild->getRawName())) {
                            return 0;
                    }
                }
                else {
                    if ((children[0]->getURI() != fFirstChild->getURI()) ||
                        (!XMLString::equals(children[0]->getLocalPart(), fFirstChild->getLocalPart()))) {
                        return 0;
                    }
                }
            }


            if (childCount > 1)
                return 1;
            break;

        case ContentSpecNode::ZeroOrMore :
            //
            //  If the child count is zero, that's fine. If its more than
            //  zero, then make sure that all children are of the element
            //  type that we stored.
            //
            if (childCount > 0)
            {
                if (fDTD) {
                    for (index = 0; index < childCount; index++) {
                        if (!XMLString::equals(children[index]->getRawName(), fFirstChild->getRawName())) {
                            return index;
                        }
                    }
                }
                else {
                    for (index = 0; index < childCount; index++) {
                        if ((children[index]->getURI() != fFirstChild->getURI()) ||
                            !XMLString::equals(children[index]->getLocalPart(), fFirstChild->getLocalPart())) {
                            return index;
                        }
                    }
                }
            }
            break;

        case ContentSpecNode::OneOrMore :
            //
            //  If the child count is zero, that's an error. If its more
            //  than zero, then make sure that all children are of the
            //  element type that we stored.
            //
            if (childCount == 0)
                return 0;

            if (fDTD) {
                for (index = 0; index < childCount; index++) {
                    if (!XMLString::equals(children[index]->getRawName(), fFirstChild->getRawName())) {
                        return index;
                    }
                }
            }
            else {
                for (index = 0; index < childCount; index++) {
                    if ((children[index]->getURI() != fFirstChild->getURI()) ||
                        !XMLString::equals(children[index]->getLocalPart(), fFirstChild->getLocalPart())) {
                        return index;
                    }
                }
            }
            break;

        case ContentSpecNode::Choice :
            //
            //  There can only be one child, and it must be one of the
            //  two types we stored.
            //
            if (!childCount)
                return 0;

            if (fDTD) {
                if (!XMLString::equals(children[0]->getRawName(), fFirstChild->getRawName()) &&
                    !XMLString::equals(children[0]->getRawName(), fSecondChild->getRawName())) {
                    return 0;
                }
            }
            else {
                if (((children[0]->getURI() != fFirstChild->getURI()) ||
                     !XMLString::equals(children[0]->getLocalPart(), fFirstChild->getLocalPart())) &&
                    ((children[0]->getURI() != fSecondChild->getURI()) ||
                     !XMLString::equals(children[0]->getLocalPart(), fSecondChild->getLocalPart()))) {
                    return 0;
                }
            }

            if (childCount > 1)
                return 1;
            break;

        case ContentSpecNode::Sequence :
            //
            //  There must be two children and they must be the two values
            //  we stored, in the stored order. So first check the obvious
            //  problem of an empty content, which would never be valid
            //  in this content mode.
            //
            if (!childCount)
                return 0;

            if (childCount == 2) {
                if (fDTD) {
                    if (!XMLString::equals(children[0]->getRawName(), fFirstChild->getRawName())) {
                        return 0;
                    }
                    if (!XMLString::equals(children[1]->getRawName(), fSecondChild->getRawName())) {
                        return 1;
                    }
                }
                else {
                    if ((children[0]->getURI() != fFirstChild->getURI()) ||
                        !XMLString::equals(children[0]->getLocalPart(), fFirstChild->getLocalPart())) {
                        return 0;
                    }

                    if ((children[1]->getURI() != fSecondChild->getURI()) ||
                        !XMLString::equals(children[1]->getLocalPart(), fSecondChild->getLocalPart())) {
                        return 1;
                    }
                }
            }
            else {
                if (childCount > 2) {
                    return 2;
                }

                return childCount;
            }
            break;

        default :
            ThrowXMLwithMemMgr(RuntimeException, XMLExcepts::CM_UnknownCMSpecType, fMemoryManager);
            break;
    }
    return -1;
}

int SimpleContentModel::validateContentSpecial(QName** const          children
                                            , const unsigned int      childCount
                                            , const unsigned int
                                            , GrammarResolver*  const pGrammarResolver
                                            , XMLStringPool*    const pStringPool) const
{

    SubstitutionGroupComparator comparator(pGrammarResolver, pStringPool);

    //
    //  According to the type of operation, we do the correct type of
    //  content check.
    //
    unsigned int index;
    switch(fOp & 0x0f)
    {
        case ContentSpecNode::Leaf :
            //
            //  There can only be one child and it has to be of the
            //  element type we stored.
            //
            if (!childCount)
                return 0;

            if ((children[0]->getURI() != fFirstChild->getURI()) ||
                !XMLString::equals(children[0]->getLocalPart(), fFirstChild->getLocalPart()))
            {
                if (!comparator.isEquivalentTo(children[0], fFirstChild))
                   return 0;
            }

            if (childCount > 1)
                return 1;
            break;

        case ContentSpecNode::ZeroOrOne :
            //
            //  If the child count is greater than one, then obviously
            //  bad. Otherwise, if its one, then the one child must be
            //  of the type we stored.
            //
            if ((childCount == 1) &&
               ((children[0]->getURI() != fFirstChild->getURI()) ||
                !XMLString::equals(children[0]->getLocalPart(), fFirstChild->getLocalPart())))
            {
                if(!comparator.isEquivalentTo(children[0], fFirstChild))
                    return 0;
            }

            if (childCount > 1)
                return 1;
            break;

        case ContentSpecNode::ZeroOrMore :
            //
            //  If the child count is zero, that's fine. If its more than
            //  zero, then make sure that all children are of the element
            //  type that we stored.
            //
            if (childCount > 0)
            {
                for (index = 0; index < childCount; index++)
                {
                    if ((children[index]->getURI() != fFirstChild->getURI()) ||
                        !XMLString::equals(children[index]->getLocalPart(), fFirstChild->getLocalPart()))
                    {
    				    if (!comparator.isEquivalentTo(children[index], fFirstChild))
                            return index;
                    }
                }
            }
            break;

        case ContentSpecNode::OneOrMore :
            //
            //  If the child count is zero, that's an error. If its more
            //  than zero, then make sure that all children are of the
            //  element type that we stored.
            //
            if (childCount == 0)
                return 0;

            for (index = 0; index < childCount; index++)
            {
                if ((children[index]->getURI() != fFirstChild->getURI()) ||
                    !XMLString::equals(children[index]->getLocalPart(), fFirstChild->getLocalPart()))
                {
    			    if (!comparator.isEquivalentTo(children[index], fFirstChild))
                        return index;
                }
            }
            break;

        case ContentSpecNode::Choice :
            //
            //  There can only be one child, and it must be one of the
            //  two types we stored.
            //
            if (!childCount)
                return 0;

            if (((children[0]->getURI() != fFirstChild->getURI()) ||
                 !XMLString::equals(children[0]->getLocalPart(), fFirstChild->getLocalPart())) &&
                ((children[0]->getURI() != fSecondChild->getURI()) ||
                 !XMLString::equals(children[0]->getLocalPart(), fSecondChild->getLocalPart())))
            {

                 if (!comparator.isEquivalentTo(children[0], fFirstChild) &&
                     !comparator.isEquivalentTo(children[0], fSecondChild) )
                     return 0;
            }

            if (childCount > 1)
                return 1;
            break;

        case ContentSpecNode::Sequence :
            //
            //  There must be two children and they must be the two values
            //  we stored, in the stored order. So first check the obvious
            //  problem of an empty content, which would never be valid
            //  in this content mode.
            //
            if (!childCount)
                return 0;

            if (childCount == 2)
            {
                if ((children[0]->getURI() != fFirstChild->getURI()) ||
                    !XMLString::equals(children[0]->getLocalPart(), fFirstChild->getLocalPart()))
                {
                    if(!comparator.isEquivalentTo(children[0], fFirstChild))
                        return 0;
                }

                if ((children[1]->getURI() != fSecondChild->getURI()) ||
                    !XMLString::equals(children[1]->getLocalPart(), fSecondChild->getLocalPart()))
                {
                    if (!comparator.isEquivalentTo(children[1], fSecondChild))
                        return 1;
                }
            }
            else
            {
                if (childCount > 2)
                {
                    return 2;
                }

                return childCount;
            }
            break;

        default :
            ThrowXMLwithMemMgr(RuntimeException, XMLExcepts::CM_UnknownCMSpecType, fMemoryManager);
            break;
    }
    return -1;
}

ContentLeafNameTypeVector* SimpleContentModel::getContentLeafNameTypeVector() const
{
    return 0;
}

void SimpleContentModel::checkUniqueParticleAttribution
    (
        SchemaGrammar*    const pGrammar
      , GrammarResolver*  const pGrammarResolver
      , XMLStringPool*    const pStringPool
      , XMLValidator*     const pValidator
      , unsigned int*     const pContentSpecOrgURI
      , const XMLCh*            pComplexTypeName /*= 0*/
    )
{
    // rename back
    unsigned int orgURIIndex = 0;

    orgURIIndex = fFirstChild->getURI();
    if ((orgURIIndex != XMLContentModel::gEOCFakeId) &&
        (orgURIIndex != XMLElementDecl::fgInvalidElemId) &&
        (orgURIIndex != XMLElementDecl::fgPCDataElemId))
        fFirstChild->setURI(pContentSpecOrgURI[orgURIIndex]);

    orgURIIndex = fSecondChild->getURI();
    if ((orgURIIndex != XMLContentModel::gEOCFakeId) &&
        (orgURIIndex != XMLElementDecl::fgInvalidElemId) &&
        (orgURIIndex != XMLElementDecl::fgPCDataElemId))
        fSecondChild->setURI(pContentSpecOrgURI[orgURIIndex]);

    // only possible violation is when it's a choice
    if ((fOp & 0x0f) == ContentSpecNode::Choice) {

        SubstitutionGroupComparator comparator(pGrammarResolver, pStringPool);

        if (XercesElementWildcard::conflict(pGrammar,
                                            ContentSpecNode::Leaf,
                                            fFirstChild,
                                            ContentSpecNode::Leaf,
                                            fSecondChild,
                                            &comparator))

            pValidator->emitError(XMLValid::UniqueParticleAttributionFail,
                                  pComplexTypeName,
                                  fFirstChild->getRawName(),
                                  fSecondChild->getRawName());
    }
}

XERCES_CPP_NAMESPACE_END

