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
 * Revision 1.3  2000/02/24 20:16:49  abagchi
 * Swat for removing Log from API docs
 *
 * Revision 1.2  2000/02/09 21:42:39  abagchi
 * Copyright swat
 *
 * Revision 1.1.1.1  1999/11/09 01:03:48  twl
 * Initial checkin
 *
 * Revision 1.2  1999/11/08 20:45:44  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */


#if !defined(SIMPLECONTENTMODEL_HPP)
#define SIMPLECONTENTMODEL_HPP

#include <framework/XMLContentModel.hpp>
#include <validators/common/ContentSpecNode.hpp>
#include <validators/common/ContentLeafNameTypeVector.hpp>


//
//  SimpleContentModel is a derivative of the abstract content model base
//  class that handles a small set of simple content models that are just
//  way overkill to give the DFA treatment.
//
//  DESCRIPTION:
//
//  This guy handles the following scenarios:
//
//      a
//      a?
//      a*
//      a+
//      a,b
//      a|b
//
//  These all involve a unary operation with one element type, or a binary
//  operation with two elements. These are very simple and can be checked
//  in a simple way without a DFA and without the overhead of setting up a
//  DFA for such a simple check.
//
//  NOTE:   Pass the XMLElementDecl::fgPCDataElemId value to represent a
//          PCData node. Pass XMLElementDecl::fgInvalidElemId for unused ids.
//
class SimpleContentModel : public XMLContentModel
{
public :
    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    SimpleContentModel
    (
        const   unsigned int                firstChildId
        , const unsigned int                secondChildId
        , const ContentSpecNode::NodeTypes  cmOp
    );

    ~SimpleContentModel();


    // -----------------------------------------------------------------------
    //  Implementation of the ContentModel virtual interface
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

    virtual ContentLeafNameTypeVector* getContentLeafNameTypeVector() const;

//  Onhold, until EquivClassComparator is defined
//  void setEquivClassComparator(EquivClassComparator comparator);

private :
    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    SimpleContentModel();
    SimpleContentModel(const SimpleContentModel&);
    void operator=(const SimpleContentModel&);


    // -----------------------------------------------------------------------
    //  Private data members
    //
    //  fFirstChild
    //  fSecondChild
    //      The element idsof the first (and optional second) child node. The
    //      operation code tells us whether the second child is used or not.
    //
    //  fOp
    //      The operation that this object represents. Since this class only
    //      does simple contents, there is only ever a single operation
    //      involved (i.e. the children of the operation are always one or
    //      two leafs.)
    // -----------------------------------------------------------------------
    unsigned int                fFirstChild;
    unsigned int                fSecondChild;
    ContentSpecNode::NodeTypes  fOp;
};


// ---------------------------------------------------------------------------
//  SimpleContentModel: Constructors and Destructor
// ---------------------------------------------------------------------------
inline
SimpleContentModel::SimpleContentModel( const   unsigned int                firstChildId
                                        , const unsigned int                secondChildId
                                        , const ContentSpecNode::NodeTypes  cmOp) :
    fOp(cmOp)
    , fFirstChild(firstChildId)
    , fSecondChild(secondChildId)
{
}

inline SimpleContentModel::~SimpleContentModel()
{
}

#endif
