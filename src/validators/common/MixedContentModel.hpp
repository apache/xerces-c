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

#include <util/ValueVectorOf.hpp>
#include <framework/XMLContentModel.hpp>
#include <validators/common/ContentLeafNameTypeVector.hpp>

class ContentSpecNode;
class XMLElementDecl;


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
        const   XMLElementDecl& parentElem
		,const bool             ordered = false
		,const bool             dtd = true
    );

    ~MixedContentModel();


    // -----------------------------------------------------------------------
    //  Getter methods
    // -----------------------------------------------------------------------
    bool hasDups() const;


    // -----------------------------------------------------------------------
    //  Implementation of the ContentModel virtual interface
    // -----------------------------------------------------------------------
    virtual bool getIsAmbiguous() const;
	
	virtual int validateContent
    (
        const   unsigned int*   childIds
        , const unsigned int    childCount
		, const XMLValidator   *pValidator = 0
    )   const;

	virtual int validateContentSpecial
    (
        const   unsigned int*   childIds
        , const unsigned int    childCount
		, const XMLValidator   *pValidator = 0
	) const;

    virtual ContentLeafNameTypeVector* getContentLeafNameTypeVector() const ;

//  Onhold, until EquivClassComparator is defined
//  void setEquivClassComparator(EquivClassComparator comparator);

private :
    // -----------------------------------------------------------------------
    //  Private helper methods
    // -----------------------------------------------------------------------
    void buildChildList
    (
        const   ContentSpecNode&                           curNode
        ,       ValueVectorOf<unsigned int>&               toFill
		,       ValueVectorOf<ContentSpecNode::NodeTypes>& toType
	);

    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    MixedContentModel();
    MixedContentModel(const MixedContentModel&);
    void operator=(const MixedContentModel&);


    // -----------------------------------------------------------------------
    //  Private data members
    //
    //  fCount
    //      The count of possible children in the fChildIds member.
    //
    //  fChildIds
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
    //  fComparator
    //      this is the EquivClassComparator object
    // -----------------------------------------------------------------------
    unsigned int    fCount;
    unsigned int   *fChildIds;
	ContentSpecNode::NodeTypes  *fChildTypes;
    bool            fOrdered;
    bool            fDTD;
#ifdef _feat_1530
	EquivClassComparator        comparator;
#endif
};

#endif
