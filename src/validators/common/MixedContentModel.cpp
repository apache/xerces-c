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
#include <util/RuntimeException.hpp>
#include <framework/XMLElementDecl.hpp>
#include <framework/XMLValidator.hpp>
#include <validators/common/ContentSpecNode.hpp>
#include <validators/common/MixedContentModel.hpp>
#include <validators/common/CMStateSet.hpp>


// ---------------------------------------------------------------------------
//  MixedContentModel: Constructors and Destructor
// ---------------------------------------------------------------------------
MixedContentModel::MixedContentModel(const XMLElementDecl& parentElem
									 ,const bool           ordered
									 ,const bool           dtd)
:fCount(0),
 fChildIds(0),
 fChildTypes(0),
 fOrdered(ordered),
 fDTD(dtd)
{
    //
    //  Create a vector of unsigned ints that will be filled in with the
    //  ids of the child nodes. It will be expanded as needed but we give
    //  it an initial capacity of 64 which should be more than enough for
    //  99% of the scenarios.
    //

    ValueVectorOf<unsigned int> childs(64);
    ValueVectorOf<ContentSpecNode::NodeTypes> childTypes(64);

    //
    //  Get the parent element's content spec. This is the head of the tree
    //  of nodes that describes the content model. We will iterate this
    //  tree.
    //
    const ContentSpecNode* curNode = parentElem.getContentSpec();
    if (!curNode)
        ThrowXML(RuntimeException, XMLExcepts::CM_NoParentCSN);

    // And now call the private recursive method that iterates the tree
    buildChildList(*curNode, childs, childTypes);

	//
   	//  And now we know how many elements we need in our member list. So
	//  fill them in.
	//
    fCount = childs.size();
    fChildIds = new unsigned int[fCount];
    memcpy(fChildIds, childs.rawData(), fCount * sizeof(unsigned int));
	fChildTypes = new ContentSpecNode::NodeTypes [fCount];
    memcpy(fChildTypes, childTypes.rawData(), fCount * sizeof(ContentSpecNode::NodeTypes));

}

MixedContentModel::~MixedContentModel()
{
	delete [] fChildIds;
   	delete [] fChildTypes;
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
        const unsigned int curVal = fChildIds[index];
        for (unsigned int iIndex = 0; iIndex < fCount; iIndex++)
        {
            if (iIndex == index)
                continue;

            if (curVal == fChildIds[iIndex])
                return true;
        }
    }
    return false;
}


// ---------------------------------------------------------------------------
//  MixedContentModel: Implementation of the ContentModel virtual interface
// ---------------------------------------------------------------------------
bool MixedContentModel::getIsAmbiguous() const
{
    if (fCount < 2)
        return false;

    unsigned int index;

    //
    //  Run through the children and remember the min and max element ids.
    //  These will be used to create a bit set that will map to element ids
    //  (adjusted by the min value.)
    //
    //  Note that we skip the 0th element because its the PCDATA one.
    //
    unsigned int minId = 0xFFFFFFFF;
    unsigned int maxId = 0;
    for (index = 1; index < fCount; index++)
    {
        const unsigned int curId = fChildIds[index];
        if (curId < minId)
            minId = curId;

        if (curId > maxId)
            maxId = curId;
    }

    //
    //  Ok, now we can create a range value that represents the spread
    //  between the min/max element value.
    //
    //  With this number we can create a state set that has a bit per
    //  possible entry in the leaf array.
    //
    const unsigned int idRange = (maxId - minId) + 1;
    CMStateSet idSet(idRange);

    for (index = 1; index < fCount; index++)
    {
        const unsigned int adjustedId = fChildIds[index] - minId;
        if (idSet.getBit(adjustedId))
            return true;
        idSet.setBit(adjustedId);
    }
    return false;
}

//
//Under the XML Schema mixed model,
//the order and number of child elements appearing in an instance
//must agree with
//the order and number of child elements specified in the model.
//
int
MixedContentModel::validateContent( const unsigned int*   childIds
                                  , const unsigned int    childCount
								  , const XMLValidator   *pValidator) const
{
	// must match order
	if (fOrdered) {

		unsigned int inIndex = 0;
		for (unsigned int outIndex = 0; outIndex < childCount; outIndex++) {

            // Get the current child out of the source index
            const unsigned int curChild = childIds[outIndex];

            // If its PCDATA, then we just accept that
            if (curChild == XMLElementDecl::fgPCDataElemId)
                continue;

			ContentSpecNode::NodeTypes type = fChildTypes[inIndex];
            const unsigned int inChild = fChildIds[inIndex];

			if (type == ContentSpecNode::Leaf) {
				if (curChild != inChild)
    				return outIndex;
			}
			else if (type == ContentSpecNode::Any) {

				const XMLElementDecl* elemDecl = pValidator->getElemDecl(inChild);
				const int uri = elemDecl->getURI();
				const XMLElementDecl* elemDecl2 = pValidator->getElemDecl(curChild);		
				if ((uri != -1 ) &&
					(uri != elemDecl2->getURI()))
				{
					return outIndex;
				}
			}
			else if (type == ContentSpecNode::Any_Local) {

				const XMLElementDecl* elemDecl = pValidator->getElemDecl(inChild);
				if (elemDecl->getURI() != -1) {
					return outIndex;
				}
			}
			else if (type == ContentSpecNode::Any_Other) {

				const XMLElementDecl* elemDecl = pValidator->getElemDecl(inChild);
				const XMLElementDecl* elemDecl2 = pValidator->getElemDecl(curChild);
				if (elemDecl->getURI() == elemDecl2->getURI()) {
					return outIndex;
				}
			}
			
			// advance index
			inIndex++;
		}

		/***checking the number of childs
		if (inIndex != fCount)
			return outIndex;
		***/
	}

	// can appear in any order
	else {
		for (unsigned int outIndex = 0; outIndex < childCount; outIndex++)
		{
            // Get the current child out of the source index
            const unsigned int curChild = childIds[outIndex];

            // If its PCDATA, then we just accept that
            if (curChild == XMLElementDecl::fgPCDataElemId)
                continue;

			// And try to find it in our list
			unsigned int inIndex = 0;
			for (; inIndex < fCount; inIndex++)
			{

			    ContentSpecNode::NodeTypes type = fChildTypes[inIndex];

				if (type == ContentSpecNode::Leaf) {
	    			if (curChild == fChildIds[inIndex])
            			break;
				}
				else if (type == ContentSpecNode::Any) {

				    const XMLElementDecl* elemDecl = pValidator->getElemDecl(fChildIds[inIndex]);
				    const int uri = elemDecl->getURI();
				    const XMLElementDecl* elemDecl2 = pValidator->getElemDecl(curChild);

					if (uri == -1 || uri == elemDecl2->getURI()) {
						break;
					}
				}
				else if (type == ContentSpecNode::Any_Local) {

				    const XMLElementDecl* elemDecl = pValidator->getElemDecl(fChildIds[inIndex]);
					if (elemDecl->getURI() == -1) {
						break;
					}
				}
				else if (type == ContentSpecNode::Any_Other) {

				    const XMLElementDecl* elemDecl = pValidator->getElemDecl(fChildIds[inIndex]);
				    const XMLElementDecl* elemDecl2 = pValidator->getElemDecl(curChild);
					if ( elemDecl->getURI() != elemDecl2->getURI()) {
						break;
					}
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
    return XMLValidator::Success;

}


// ---------------------------------------------------------------------------
//  MixedContentModel: Private helper methods
// ---------------------------------------------------------------------------
void
MixedContentModel::buildChildList(  const   ContentSpecNode&             curNode
                                    ,       ValueVectorOf<unsigned int>& toFill
									,       ValueVectorOf<ContentSpecNode::NodeTypes>& toType)
{
    // Get the type of spec node our current node is
    const ContentSpecNode::NodeTypes curType = curNode.getType();

    // If its a leaf, then store its id in the target list
    if ((curType == ContentSpecNode::Leaf)      ||
        (curType == ContentSpecNode::Any)       ||
        (curType == ContentSpecNode::Any_Other) ||
        (curType == ContentSpecNode::Any_Local)   )
    {
        toFill.addElement(curNode.getElemId());
		toType.addElement(curType);
        return;
    }

    // Get both the child node pointers
    const ContentSpecNode* leftNode = curNode.getFirst();
    const ContentSpecNode* rightNode = curNode.getSecond();

    // And recurse according to the type of node
    if ((curType == ContentSpecNode::Choice)
    ||  (curType == ContentSpecNode::Sequence))
    {
        // Recurse on the left and right nodes
        buildChildList(*leftNode, toFill, toType);

        // The last node of a choice or sequence has a null right
        if (rightNode)
            buildChildList(*rightNode, toFill, toType);
    }
     else if ((curType == ContentSpecNode::OneOrMore)
          ||  (curType == ContentSpecNode::ZeroOrOne)
          ||  (curType == ContentSpecNode::ZeroOrMore))
    {
        // Just do the left node on this one
        buildChildList(*leftNode, toFill, toType);
    }
}

int MixedContentModel::validateContentSpecial(  const   unsigned int*   childIds
                                               , const unsigned int    childCount
								               , const XMLValidator   *pValidator) const
{
	return validateContent(childIds, childCount, pValidator);
};

ContentLeafNameTypeVector* MixedContentModel::getContentLeafNameTypeVector() const
{
	return 0;
};