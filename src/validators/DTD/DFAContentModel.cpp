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
 * Revision 1.3  2000/03/02 19:55:38  roddey
 * This checkin includes many changes done while waiting for the
 * 1.1.0 code to be finished. I can't list them all here, but a list is
 * available elsewhere.
 *
 * Revision 1.2  2000/02/09 21:42:37  abagchi
 * Copyright swatswat
 *
 * Revision 1.1.1.1  1999/11/09 01:03:17  twl
 * Initial checkin
 *
 * Revision 1.2  1999/11/08 20:45:38  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <util/RuntimeException.hpp>
#include <framework/XMLValidator.hpp>
#include <validators/DTD/CMBinaryOp.hpp>
#include <validators/DTD/CMLeaf.hpp>
#include <validators/DTD/CMUnaryOp.hpp>
#include <validators/DTD/DFAContentModel.hpp>
#include <validators/DTD/ContentSpecNode.hpp>
#include <validators/DTD/DTDElementDecl.hpp>


// ---------------------------------------------------------------------------
//  Local static data
//
//  gEOCFakeId
//  gEpsilonFakeId
//      We have to put in a couple of special CMLeaf nodes to represent
//      special values, using fake element ids that we know won't conflict
//      with real element ids.
// ---------------------------------------------------------------------------
static const unsigned int   gEOCFakeId      = 0xFFFFFFF1;
static const unsigned int   gEpsilonFakeId  = 0xFFFFFFF2;


// ---------------------------------------------------------------------------
//  DFAContentModel: Constructors and Destructor
// ---------------------------------------------------------------------------
DFAContentModel::DFAContentModel(const DTDElementDecl& elemDecl) :

    fElemDecl(elemDecl)
    , fElemMap(0)
    , fElemMapSize(0)
    , fEmptyOk(false)
    , fEOCPos(0)
    , fFinalStateFlags(0)
    , fFollowList(0)
    , fHeadNode(0)
    , fLeafCount(0)
    , fLeafList(0)
    , fSpecNode(0)
    , fTransTable(0)
    , fTransTableSize(0)
{
    //
    //  Store away our content spec node. This is used all over the place
    //  here so its easier to store a member pointer than to pass it around.
    //
    fSpecNode = elemDecl.getContentSpec();

    // And build the DFA data structures
    buildDFA();
}

DFAContentModel::~DFAContentModel()
{
    //
    //  Clean up all the stuff that is not just temporary representation
    //  data that was cleaned up after building the DFA.
    //
    delete [] fFinalStateFlags;

    unsigned index;
    for (index = 0; index < fTransTableSize; index++)
        delete [] fTransTable[index];
    delete [] fTransTable;

    delete [] fElemMap;
}


// ---------------------------------------------------------------------------
//  DFAContentModel: Implementation of the ContentModel virtual interface
// ---------------------------------------------------------------------------
int
DFAContentModel::validateContent(   const   unsigned int*   childIds
                                    , const unsigned int    childCount) const
{
    //
    //  If there are no children, then either we fail on the 0th element
    //  or we return success. It depends upon whether this content model
    //  accepts empty content, which we determined earlier.
    //
    if (!childCount)
    {
        if (fEmptyOk)
            return -1;
        return 0;
    }

    //
    //  Lets loop through the children in the array and move our way
    //  through the states. Note that we use the fElemMap array to map
    //  an element index to a state index.
    //
    unsigned int curState = 0;
    unsigned int childIndex = 0;
    for (; childIndex < childCount; childIndex++)
    {
        // Get the current element index out
        const unsigned int curElem = childIds[childIndex];

        // Look up this child in our element map
        unsigned int elemIndex = 0;
        for (; elemIndex < fElemMapSize; elemIndex++)
        {
            if (fElemMap[elemIndex] == curElem)
                break;
        }

        // If we didn't find it, then obviously not valid
        if (elemIndex == fElemMapSize)
            return childIndex;

        //
        //  Look up the next state for this input symbol when in the
        //  current state.
        //
        curState = fTransTable[curState][elemIndex];

        //  If its not a legal transition then we failed.
        if (curState == -1)
            return childIndex;
    }

    //
    //  We transitioned all the way through the input list. However, that
    //  does not mean that we ended in a final state. So check whether
    //  our ending state is a final state.
    //
    if (!fFinalStateFlags[curState])
        return childIndex;

    return XMLValidator::Success;
}


// ---------------------------------------------------------------------------
//  DFAContentModel: Private helper methods
// ---------------------------------------------------------------------------
void DFAContentModel::buildDFA()
{
    unsigned int index;


    //
    //  The first step we need to take is to rewrite the content model using
    //  our CMNode objects, and in the process get rid of any repetition short
    //  cuts, converting them into '*' style repetitions or getting rid of
    //  repetitions altogether.
    //
    //  The conversions done are:
    //
    //  x+ -> (x|x*)
    //  x? -> (x|epsilon)
    //
    //  This is a relatively complex scenario. What is happening is that we
    //  create a top level binary node of which the special EOC value is set
    //  as the right side node. The the left side is set to the rewritten
    //  syntax tree. The source is the original content model info from the
    //  decl pool. The rewrite is done by buildSyntaxTree() which recurses the
    //  decl pool's content of the element and builds a new tree in the
    //  process.
    //
    //  Note that, during this operation, we set each non-epsilon leaf node's
    //  DFA state position and count the number of such leafs, which is left
    //  in the fLeafCount member.
    //
    CMLeaf* nodeEOC = new CMLeaf(gEOCFakeId);
    CMNode* nodeOrgContent = buildSyntaxTree(fElemDecl.getContentSpec());
    fHeadNode = new CMBinaryOp
    (
        ContentSpecNode::Sequence
        , nodeOrgContent
        , nodeEOC
    );

    //
    //  And handle specially the EOC node, which also must be numbered and
    //  counted as a non-epsilon leaf node. It could not be handled in the
    //  above tree build because it was created before all that started. We
    //  save the EOC position since its used during the DFA building loop.
    //
    fEOCPos = fLeafCount;
    nodeEOC->setPosition(fLeafCount++);

    //
    //  Ok, so now we have to iterate the new tree and do a little more work
    //  now that we know the leaf count. One thing we need to do is to
    //  calculate the first and last position sets of each node. This is
    //  cached away in each of the nodes.
    //
    //  Along the way we also set the leaf count in each node as the maximum
    //  state count. They must know this in order to create their first/last
    //  position sets.
    //
    //  We also need to build an array of references to the non-epsilon
    //  leaf nodes. Since we iterate here the same way as we did during the
    //  initial tree build (which built their position numbers, we will put
    //  them in the array according to their position values.
    //
    fLeafList = new CMLeaf*[fLeafCount];
    postTreeBuildInit(fHeadNode, 0);

    //
    //  And, moving onward... We now need to build the follow position sets
    //  for all the nodes. So we allocate an array of pointers to state sets,
    //  one for each leaf node (i.e. each significant DFA position.)
    //
    fFollowList = new CMStateSet*[fLeafCount];
    for (index = 0; index < fLeafCount; index++)
        fFollowList[index] = new CMStateSet(fLeafCount);
    calcFollowList(fHeadNode);

    //
    //  Check the DFA for ambiguity. If it is ambiguous, then set the
    //  ambiguous flag member and keep going.
    //
    fIsAmbiguous = isAmbiguous();

    //
    //  Check to see whether this content model can handle an empty content,
    //  which is something we need to optimize by looking now before we
    //  throw away the info that would tell us that.
    //
    //  If the left node of the head (the top level of the original content)
    //  is nullable, then its true.
    //
    fEmptyOk = nodeOrgContent->isNullable();

    //
    //  And finally the big push... Now we build the DFA using all the states
    //  and the tree we've built up. First we set up the various data
    //  structures we are going to use while we do this.
    //
    //  First of all we need an array of unique element ids in our content
    //  model. For each transition table entry, we need a set of contiguous
    //  indices to represent the transitions for a particular input element.
    //  So we need to a zero based range of indexes that map to element types.
    //  This element map provides that mapping.
    //
    fElemMap = new unsigned int[fLeafCount];
    fElemMapSize = 0;
    for (unsigned int outIndex = 0; outIndex < fLeafCount; outIndex++)
    {
        // Get the current leaf's element index
        const unsigned int elemId = fLeafList[outIndex]->getId();

        // See if the current leaf node's element index is in the list
        unsigned int inIndex = 0;
        for (; inIndex < fElemMapSize; inIndex++)
        {
            if (fElemMap[inIndex] == elemId)
                break;
        }

        // If it was not in the list, then add it and bump the map size
        if (inIndex == fElemMapSize)
            fElemMap[fElemMapSize++] = elemId;
    }

    //
    //  Next lets create some arrays, some that that hold transient info
    //  during the DFA build and some that are permament. These are kind of
    //  sticky since we cannot know how big they will get, but we don't want
    //  to use any collection type classes because of performance.
    //
    //  Basically they will probably be about fLeafCount*2 on average, but can
    //  be as large as 2^(fLeafCount*2), worst case. So we start with
    //  fLeafCount*4 as a middle ground. This will be very unlikely to ever
    //  have to expand though, it if does, the overhead will be somewhat ugly.
    //
    unsigned int curArraySize = fLeafCount * 4;
    const CMStateSet** statesToDo = new const CMStateSet*[curArraySize];
    fFinalStateFlags = new bool[curArraySize];
    fTransTable = new unsigned int*[curArraySize];

    //
    //  Ok we start with the initial set as the first pos set of the head node
    //  (which is the seq node that holds the content model and the EOC node.)
    //
    const CMStateSet* setT = new CMStateSet(fHeadNode->getFirstPos());

    //
    //  Init our two state flags. Basically the unmarked state counter is
    //  always chasing the current state counter. When it catches up, that
    //  means we made a pass through that did not add any new states to the
    //  lists, at which time we are done. We could have used a expanding array
    //  of flags which we used to mark off states as we complete them, but
    //  this is easier though less readable maybe.
    //
    unsigned int unmarkedState = 0;
    unsigned int curState = 0;

    //
    //  Init the first transition table entry, and put the initial state
    //  into the states to do list, then bump the current state.
    //
    fTransTable[curState] = makeDefStateList();
    statesToDo[curState] = setT;
    curState++;

    //
    //  Ok, almost done with the algorithm from hell... We now enter the
    //  loop where we go until the states done counter catches up with
    //  the states to do counter.
    //
    CMStateSet* newSet = 0;
    while (unmarkedState < curState)
    {
        //
        //  Get the next unmarked state out of the list of states to do.
        //  And get the associated transition table entry.
        //
        setT = statesToDo[unmarkedState];
        unsigned int* transEntry = fTransTable[unmarkedState];

        // Mark this one final if it contains the EOC state
        fFinalStateFlags[unmarkedState] = setT->getBit(fEOCPos);

        // Bump up the unmarked state count, marking this state done
        unmarkedState++;

        // Loop through each possible input symbol in the element map
        for (unsigned int elemIndex = 0; elemIndex < fElemMapSize; elemIndex++)
        {
            //
            //  Build up a set of states which is the union of all of the
            //  follow sets of DFA positions that are in the current state. If
            //  we gave away the new set last time through then create a new
            //  one. Otherwise, zero out the existing one.
            //
            if (!newSet)
                newSet = new CMStateSet(fLeafCount);
            else
                newSet->zeroBits();

            for (unsigned int leafIndex = 0; leafIndex < fLeafCount; leafIndex++)
            {
                // If this leaf index (DFA position) is in the current set...
                if (setT->getBit(leafIndex))
                {
                    //
                    //  If this leaf is the current input symbol, then we want
                    //  to add its follow list to the set of states to transition
                    //  to from the current state.
                    //
                    if (fLeafList[leafIndex]->getId() == fElemMap[elemIndex])
                        *newSet |= *fFollowList[leafIndex];
                }
            }

            //
            //  If this new set is not empty, then see if its in the list
            //  of states to do. If not, then add it.
            //
            if (!newSet->isEmpty())
            {
                //
                //  Search the 'states to do' list to see if this new
                //  state set is already in there.
                //
                unsigned int stateIndex = 0;
                for (; stateIndex < curState; stateIndex++)
                {
                    if (*statesToDo[stateIndex] == *newSet)
                        break;
                }

                // If we did not find it, then add it
                if (stateIndex == curState)
                {
                    //
                    //  Put this new state into the states to do and init
                    //  a new entry at the same index in the transition
                    //  table.
                    //
                    statesToDo[curState] = newSet;
                    fTransTable[curState] = makeDefStateList();

                    // We now have a new state to do so bump the count
                    curState++;

                    //
                    //  Null out the new set to indicate we adopted it. This
                    //  will cause the creation of a new set on the next time
                    //  around the loop.
                    //
                    newSet = 0;
                }

                //
                //  Now set this state in the transition table's entry for this
                //  element (using its index), with the DFA state we will move
                //  to from the current state when we see this input element.
                //
                transEntry[elemIndex] = stateIndex;

                // Expand the arrays if we're full
                if (curState == curArraySize)
                {
                    //
                    //  Yikes, we overflowed the initial array size, so we've
                    //  got to expand all of these arrays. So adjust up the
                    //  size by 50% and allocate new arrays.
                    //
                    const unsigned int newSize = (unsigned int)(curArraySize * 1.5);
                    const CMStateSet** newToDo = new const CMStateSet*[newSize];
                    bool* newFinalFlags = new bool[newSize];
                    unsigned int** newTransTable = new unsigned int*[newSize];

                    // Copy over all of the existing content
                    for (unsigned int expIndex = 0; expIndex < curArraySize; expIndex++)
                    {
                        newToDo[expIndex] = statesToDo[expIndex];
                        newFinalFlags[expIndex] = fFinalStateFlags[expIndex];
                        newTransTable[expIndex] = fTransTable[expIndex];
                    }

                    // Clean up the old stuff
                    delete [] statesToDo;
                    delete [] fFinalStateFlags;
                    delete [] fTransTable;

                    // Store the new array size and pointers
                    curArraySize = newSize;
                    statesToDo = newToDo;
                    fFinalStateFlags = newFinalFlags;
                    fTransTable = newTransTable;
                }
            }
        }
    }

    // Store the current state count in the trans table size
    fTransTableSize = curState;

    // If the last temp set was not stored, then clean it up
    if (newSet)
        delete newSet;

    //
    //  Now we can clean up all of the temporary data that was needed during
    //  DFA build.
    //
    delete fHeadNode;
    fHeadNode = 0;

    for (index = 0; index < fLeafCount; index++)
        delete fFollowList[index];

    for (index = 0; index < curState; index++)
        delete (CMStateSet*)statesToDo[index];

    delete [] fLeafList;
    delete [] fFollowList;
    delete [] statesToDo;
}


CMNode* DFAContentModel::buildSyntaxTree(const ContentSpecNode* const curNode)
{
    // Initialize a return node pointer
    CMNode* retNode = 0;

    // Get the spec type of the passed node
    const ContentSpecNode::NodeTypes curType = curNode->getType();

    if (curType == ContentSpecNode::Leaf)
    {
        //
        //  Create a new leaf node, and pass it the current leaf count, which
        //  is its DFA state position. Bump the leaf count after storing it.
        //  This makes the positions zero based since we store first and then
        //  increment.
        //
        retNode = new CMLeaf(curNode->getElemId(), fLeafCount++);
    }
     else
    {
        //
        //  Its not a leaf, so we have to recurse its left and maybe right
        //  nodes. Save both values before we recurse and trash the node.
        //
        const ContentSpecNode* leftNode = curNode->getFirst();
        const ContentSpecNode* rightNode = curNode->getSecond();

        if ((curType == ContentSpecNode::Choice)
        ||   (curType == ContentSpecNode::Sequence))
        {
            //
            //  Recurse on both children, and return a binary op node with the
            //  two created sub nodes as its children. The node type is the
            //  same type as the source.
            //
            CMNode* newLeft = buildSyntaxTree(leftNode);
            CMNode* newRight = buildSyntaxTree(rightNode);
            retNode = new CMBinaryOp(curType, newLeft, newRight);
        }
         else if (curType == ContentSpecNode::ZeroOrMore)
        {
            // This one is fine as is, just change to our form
            retNode = new CMUnaryOp(curType, buildSyntaxTree(leftNode));
        }
         else if (curType == ContentSpecNode::ZeroOrOne)
        {
            // Convert to (x|epsilon)
            CMNode* newLeft = buildSyntaxTree(leftNode);
            retNode = new CMBinaryOp
            (
                ContentSpecNode::Choice
                , newLeft
                , new CMLeaf(gEpsilonFakeId)
            );
        }
         else if (curType == ContentSpecNode::OneOrMore)
        {
            // Convert to (x,x*)
            CMNode* newLeft = buildSyntaxTree(leftNode);
            CMNode* newLeft2 = buildSyntaxTree(leftNode);
            retNode = new CMBinaryOp
            (
                ContentSpecNode::Sequence
                , newLeft
                , new CMUnaryOp(ContentSpecNode::ZeroOrMore, newLeft2)
            );
        }
         else
        {
            ThrowXML(RuntimeException, XMLExcepts::CM_UnknownCMSpecType);
        }
    }
    return retNode;
}


void DFAContentModel::calcFollowList(CMNode* const curNode)
{
    // Get the spec type of the passed node
    const ContentSpecNode::NodeTypes curType = curNode->getType();

    if (curType == ContentSpecNode::Choice)
    {
        // Just recurse
        calcFollowList(((CMBinaryOp*)curNode)->getLeft());
        calcFollowList(((CMBinaryOp*)curNode)->getRight());
    }
     else if (curType == ContentSpecNode::Sequence)
    {
        // Recurse before we process this node
        calcFollowList(((CMBinaryOp*)curNode)->getLeft());
        calcFollowList(((CMBinaryOp*)curNode)->getRight());

        //
        //  Now handle our level. We use our left child's last pos set and our
        //  right child's first pos set, so get them now for convenience.
        //
        const CMStateSet& last  = ((CMBinaryOp*)curNode)->getLeft()->getLastPos();
        const CMStateSet& first = ((CMBinaryOp*)curNode)->getRight()->getFirstPos();

        //
        //  Now, for every position which is in our left child's last set
        //  add all of the states in our right child's first set to the
        //  follow set for that position.
        //
        for (unsigned int index = 0; index < fLeafCount; index++)
        {
            if (last.getBit(index))
                *fFollowList[index] |= first;
        }
    }
     else if (curType == ContentSpecNode::ZeroOrMore)
    {
        // Recurse first
        calcFollowList(((CMUnaryOp*)curNode)->getChild());

        //
        //  Now handle our level. We use our own first and last position
        //  sets, so get them up front.
        //
        const CMStateSet& first = curNode->getFirstPos();
        const CMStateSet& last  = curNode->getLastPos();

        //
        //  For every position which is in our last position set, add all
        //  of our first position states to the follow set for that
        //  position.
        //
        for (unsigned int index = 0; index < fLeafCount; index++)
        {
            if (last.getBit(index))
                *fFollowList[index] |= first;
        }
    }
     else if ((curType == ContentSpecNode::OneOrMore)
          ||  (curType == ContentSpecNode::ZeroOrOne))
    {
        ThrowXML1(RuntimeException, XMLExcepts::CM_NotValidForSpecType, "CalcFollowList");
    }
}


bool DFAContentModel::isAmbiguous() const
{
    unsigned int index;

    //
    //  Run through the current leaves and remember the min and max element
    //  ids. These will be used to create a bit set that will map to element
    //  ids (adjusted by the min value.)
    //
    //  Do fLeafCount - 1 because we don't want to pick up the 'EOC' node,
    //  which has a very large dummy value in it.
    //
    unsigned int minId = 0xFFFFFFFF;
    unsigned int maxId = 0;
    for (index = 0; index < fLeafCount - 1; index++)
    {
        const unsigned int curId = fLeafList[index]->getId();
        if (curId < minId)
            minId = curId;

        if (curId > maxId)
            maxId = curId;
    }

    //
    //  Ok, now we can create a range value that represents the spread
    //  between the min/max element value. The range can never be larger
    //  than the number of elements in the content model, so it would
    //  never be outrageously large unless the content model was just
    //  totally pathological.
    //
    //  With this number we can create a state set that has a bit per
    //  possible entry in the leaf array.
    //
    const unsigned int idRange = (maxId - minId) + 1;
    CMStateSet idSet(idRange);

    // Check each follow list
    for (index = 0; index < fLeafCount - 1; index++)
    {
        // Get the current follow list set
        const CMStateSet* curSet = fFollowList[index];

        //
        //  For each possible leaf, we now go through and get the element
        //  id, adjust it to zero base it, then check it in the idSet. If
        //  that bit is already on, its ambiguous. Else set that bit and
        //  keep going.
        //
        for (unsigned int inner = 0; inner < fLeafCount - 1; inner++)
        {
            // If this bit is not on in the follow set, skip to next
            if (!curSet->getBit(inner))
                continue;

            // Get the adjusted id value for the element in the follow list
            const unsigned int adjustedId = fLeafList[inner]->getId() - minId;

            if (idSet.getBit(adjustedId))
                return true;
            idSet.setBit(adjustedId);
        }

        // Zero out the bitset for the next round
        idSet.zeroBits();
    }
    return false;
}


//
//  -1 is used to represent bad transitions in the transition table entry for
//  each state. So each entry is initialized to an all -1 array. This method
//  creates a new entry and initializes it.
//
unsigned int* DFAContentModel::makeDefStateList() const
{
    unsigned int* retArray = new unsigned int[fElemMapSize];
    for (unsigned int index = 0; index < fElemMapSize; index++)
        retArray[index] = -1;
    return retArray;
}


int DFAContentModel::postTreeBuildInit(         CMNode* const   nodeCur
                                        , const unsigned int    curIndex)
{
    // Set the maximum states on this node
    nodeCur->setMaxStates(fLeafCount);

    // Get the spec type of the passed node
    const ContentSpecNode::NodeTypes curType = nodeCur->getType();

    // Get a copy of the index we can modify
    unsigned int newIndex = curIndex;

    // Recurse as required
    if ((curType == ContentSpecNode::Choice)
    ||  (curType == ContentSpecNode::Sequence))
    {
        newIndex = postTreeBuildInit(((CMBinaryOp*)nodeCur)->getLeft(), newIndex);
        newIndex = postTreeBuildInit(((CMBinaryOp*)nodeCur)->getRight(), newIndex);
    }
     else if (curType == ContentSpecNode::ZeroOrMore)
    {
        newIndex = postTreeBuildInit(((CMUnaryOp*)nodeCur)->getChild(), newIndex);
    }
     else if (curType == ContentSpecNode::Leaf)
    {
        //
        //  Put this node in the leaf list at the current index if its
        //  a non-epsilon leaf.
        //
        if (((CMLeaf*)nodeCur)->getId() != gEpsilonFakeId)
            fLeafList[newIndex++] = (CMLeaf*)nodeCur;
    }
     else
    {
        ThrowXML(RuntimeException, XMLExcepts::CM_UnknownCMSpecType);
    }
    return newIndex;
}
