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
#include <memory.h>
#include <string.h>
#include <util/RuntimeException.hpp>
#include <framework/XMLElementDecl.hpp>
#include <framework/XMLValidator.hpp>
#include <validators/DTD/ContentSpecNode.hpp>
#include <validators/DTD/DTDElementDecl.hpp>
#include <validators/DTD/MixedContentModel.hpp>
#include <validators/DTD/CMStateSet.hpp>


// ---------------------------------------------------------------------------
//  MixedContentModel: Constructors and Destructor
// ---------------------------------------------------------------------------
MixedContentModel::MixedContentModel(const DTDElementDecl& parentElem)
{
    //
    //  Create a vector of unsigned ints that will be filled in with the
    //  ids of the child nodes. It will be expanded as needed but we give
    //  it an initial capacity of 64 which should be more than enough for
    //  99% of the scenarios.
    //
    ValueVectorOf<unsigned int> childIds(64);

    //
    //  Get the parent element's content spec. This is the head of the tree
    //  of nodes that describes the content model. We will iterate this
    //  tree.
    //
    const ContentSpecNode* curNode = parentElem.getContentSpec();
    if (!curNode)
        ThrowXML(RuntimeException, XMLExcepts::CM_NoParentCSN);

    // And now call the private recursive method that iterates the tree
    buildChildList(*curNode, childIds);

    //
    //  And now we know how many elements we need in our member list. So
    //  fill them in.
    //
    fCount = childIds.size();
    fChildIds = new unsigned int[fCount];
    memcpy(fChildIds, childIds.rawData(), fCount * sizeof(unsigned int));
}


MixedContentModel::~MixedContentModel()
{
    delete [] fChildIds;
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

int
MixedContentModel::validateContent( const   unsigned int*   childIds
                                    , const unsigned int    childCount) const
{
    // Use an outer index to search the passed list of children
    for (unsigned int outIndex = 0; outIndex < childCount; outIndex++)
    {
        // Get the current child out of the source index
        const unsigned int curChild = childIds[outIndex];

        // If its PCDATA, then we just accept that
        if (curChild == XMLElementDecl::fgPCDataElemId)
            continue;

        // Otherwise  try to find it in our list using an inner index
        unsigned int inIndex = 0;
        for (; inIndex < fCount; inIndex++)
        {
            if (curChild == fChildIds[inIndex])
                break;
        }

        //
        //  We did not find this one, so the validation failed. We return
        //  the outer index, which is the index of the child that failed.
        //
        if (inIndex == fCount)
            return outIndex;
    }

    // Return success
    return XMLValidator::Success;
}


// ---------------------------------------------------------------------------
//  MixedContentModel: Private helper methods
// ---------------------------------------------------------------------------
void
MixedContentModel::buildChildList(  const   ContentSpecNode&             curNode
                                    ,       ValueVectorOf<unsigned int>& toFill)
{
    // Get the type of spec node our current node is
    const ContentSpecNode::NodeTypes curType = curNode.getType();

    // If its a leaf, then store its id in the target list
    if (curType == ContentSpecNode::Leaf)
    {
        toFill.addElement(curNode.getElemId());
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
        buildChildList(*leftNode, toFill);

        // The last node of a choice or sequence has a null right
        if (rightNode)
            buildChildList(*rightNode, toFill);
    }
     else if ((curType == ContentSpecNode::OneOrMore)
          ||  (curType == ContentSpecNode::ZeroOrOne)
          ||  (curType == ContentSpecNode::ZeroOrMore))
    {
        // Just do the left node on this one
        buildChildList(*leftNode, toFill);
    }
}
