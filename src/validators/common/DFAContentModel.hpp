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
 * Revision 1.2  2001/02/27 14:48:52  tng
 * Schema: Add CMAny and ContentLeafNameTypeVector, by Pei Yong Zhang
 *
 * Revision 1.1  2001/02/16 14:17:29  tng
 * Schema: Move the common Content Model files that are shared by DTD
 * and schema from 'DTD' folder to 'common' folder.  By Pei Yong Zhang.
 *
 * Revision 1.4  2000/03/02 19:55:38  roddey
 * This checkin includes many changes done while waiting for the
 * 1.1.0 code to be finished. I can't list them all here, but a list is
 * available elsewhere.
 *
 * Revision 1.3  2000/02/24 20:16:48  abagchi
 * Swat for removing Log from API docs
 *
 * Revision 1.2  2000/02/09 21:42:37  abagchi
 * Copyright swat
 *
 * Revision 1.1.1.1  1999/11/09 01:03:19  twl
 * Initial checkin
 *
 * Revision 1.2  1999/11/08 20:45:38  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */

#if !defined(DFACONTENTMODEL_HPP)
#define DFACONTENTMODEL_HPP

#include <util/XercesDefs.hpp>
#include <framework/XMLContentModel.hpp>
#include <validators/common/ContentLeafNameTypeVector.hpp>

class ContentSpecNode;
class CMLeaf;
class CMNode;
class CMStateSet;
class DTDElementDecl;

//
//  DFAContentModel is the heavy weight derivative of ContentModel that does
//  all of the non-trivial element content validation. This guy does the full
//  bore regular expression to DFA conversion to create a DFA that it then
//  uses in its validation algorithm.
//
//  NOTE:   Upstream work insures that this guy will never see a content model
//          with PCDATA in it. Any model with PCDATA is 'mixed' and is handled
//          via the MixedContentModel class, since mixed models are very
//          constrained in form and easily handled via a special case. This
//          also makes our life much easier here.
//
class DFAContentModel : public XMLContentModel
{
public:
    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    DFAContentModel(const DTDElementDecl& elemDecl);
    virtual ~DFAContentModel();


    // -----------------------------------------------------------------------
    //  Implementation of the virtual content model interface
    // -----------------------------------------------------------------------
    virtual bool getIsAmbiguous() const;
	virtual int validateContent
    (
        const   unsigned int*   childIds
        , const unsigned int    childCount
    )   const;

	virtual int validateContentSpecial
    (
        const   unsigned int*   childIds
        , const unsigned int    childCount
	) const;

    virtual ContentLeafNameTypeVector* getContentLeafNameTypeVector() const ;

//  Onhold, until EquivClassComparator is defined
//  void setEquivClassComparator(EquivClassComparator comparator);

private :
    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    DFAContentModel();
    DFAContentModel(const DFAContentModel&);
    void operator=(const DFAContentModel&);


    // -----------------------------------------------------------------------
    //  Private helper methods
    // -----------------------------------------------------------------------
    void buildDFA();
    CMNode* buildSyntaxTree(const ContentSpecNode* const curNode);
    void calcFollowList(CMNode* const curNode);
    bool isAmbiguous() const;
    unsigned int* makeDefStateList() const;
    int postTreeBuildInit
    (
                CMNode* const   nodeCur
        , const unsigned int    curIndex
    );


    // -----------------------------------------------------------------------
    //  Private data members
    //
    //  fElemDecl
    //      The element decl object of the element that this is the content
    //      model for. Its just stored here to avoid passing it around all
    //      over the place while building up the DFA.
    //
    //  fElemMap
    //  fElemMapSize
    //      This is the map of unique input symbol elements to indices into
    //      each state's per-input symbol transition table entry. This is part
    //      of the built DFA information that must be kept around to do the
    //      actual validation.
    //
    //  fEmptyOk
    //      This is an optimization. While building the transition table we
    //      can see whether this content model would approve of an empty
    //      content (which could happen if everything was optional.) So we
    //      set this flag and short circuit that check, which would otherwise
    //      be ugly and time consuming if we tried to determine it at each
    //      validation call.
    //
    //  fEOCPos
    //      The NFA position of the special EOC (end of content) node. This
    //      is saved away since its used during the DFA build.
    //
    //  fFinalStateFlags
    //      This is an array of booleans, one per state (there are
    //      fTransTableSize states in the DFA) that indicates whether that
    //      state is a final state.
    //
    //  fFollowList
    //      The list of follow positions for each NFA position (i.e. for each
    //      non-epsilon leaf node.) This is only used during the building of
    //      the DFA, and is let go afterwards.
    //
    //  fHeadNode
    //      This is the head node of our intermediate representation. It is
    //      only non-null during the building of the DFA (just so that it
    //      does not have to be passed all around.) Once the DFA is built,
    //      this is no longer required so its deleted.
    //
    //  fIsAmbiguous
    //      This flag is set during construction if the content model is
    //      ambiguous. This will be used by the scanner to report an error
    //      and is available for later perusal if required.
    //
    //  fLeafCount
    //      The count of leaf nodes. This is an important number that set some
    //      limits on the sizes of data structures in the DFA process.
    //
    //  fLeafList
    //      An array of non-epsilon leaf nodes, which is used during the DFA
    //      build operation, then dropped. These are just references to nodes
    //      pointed to by fHeadNode, so we don't have to clean them up, just
    //      the actually leaf list array itself needs cleanup.
    //
    //  fSpecNode
    //      The content spec node for the element that this object represents
    //      the content of. This info is needed a good bit so we get it once
    //      and keep it. We don't own it, we just reference it.
    //
    //  fTransTable
    //  fTransTableSize
    //      This is the transition table that is the main by product of all
    //      of the effort here. It is an array of arrays of ints. The first
    //      dimension is the number of states we end up with in the DFA. The
    //      second dimensions is the number of unique elements in the content
    //      model (fElemMapSize). Each entry in the second dimension indicates
    //      the new state given that input for the first dimension's start
    //      state.
    //
    //      The fElemMap array handles mapping from element indexes to
    //      positions in the second dimension of the transition table.
    //
    //      fTransTableSize is the number of valid entries in the transition
    //      table, and in the other related tables such as fFinalStateFlags.
    // -----------------------------------------------------------------------
    const DTDElementDecl&   fElemDecl;
    unsigned int*           fElemMap;
    unsigned int            fElemMapSize;
    bool                    fEmptyOk;
    unsigned int            fEOCPos;
    bool*                   fFinalStateFlags;
    CMStateSet**            fFollowList;
    CMNode*                 fHeadNode;
    bool                    fIsAmbiguous;
    unsigned int            fLeafCount;
    CMLeaf**                fLeafList;
    const ContentSpecNode*  fSpecNode;
    unsigned int**          fTransTable;
    unsigned int            fTransTableSize;
};


inline bool DFAContentModel::getIsAmbiguous() const
{
    return fIsAmbiguous;
}

#endif
