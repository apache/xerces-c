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
 * Revision 1.1.1.1  1999/11/09 01:03:14  twl
 * Initial checkin
 *
 * Revision 1.2  1999/11/08 20:45:38  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */


#if !defined(CONTENTSPECNODE_HPP)
#define CONTENTSPECNODE_HPP

#include <framework/XMLElementDecl.hpp>
#include <util/XercesDefs.hpp>

class XMLBuffer;
class XMLValidator;


class ContentSpecNode
{
public :
    // -----------------------------------------------------------------------
    //  Class specific types
    // -----------------------------------------------------------------------
    enum NodeTypes
    {
        Leaf
        , ZeroOrOne
        , ZeroOrMore
        , OneOrMore
        , Choice
        , Sequence

        , UnknownType = -1
    };


    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    ContentSpecNode();
    ContentSpecNode(const unsigned int elemId);
    ContentSpecNode
    (
        const   NodeTypes               type
        ,       ContentSpecNode* const  firstToAdopt
        ,       ContentSpecNode* const  secondToAdopt
    );
    ~ContentSpecNode();


    // -----------------------------------------------------------------------
    //  Getter methods
    // -----------------------------------------------------------------------
    unsigned int getElemId() const;
    ContentSpecNode* getFirst();
    const ContentSpecNode* getFirst() const;
    ContentSpecNode* getSecond();
    const ContentSpecNode* getSecond() const;
    NodeTypes getType() const;
    bool isPCData() const;
    ContentSpecNode* orphanFirst();
    ContentSpecNode* orphanSecond();


    // -----------------------------------------------------------------------
    //  Setter methods
    // -----------------------------------------------------------------------
    void setElemId(const unsigned int elemId);
    void setFirst(ContentSpecNode* const toAdopt);
    void setSecond(ContentSpecNode* const toAdopt);
    void setType(const NodeTypes type);


    // -----------------------------------------------------------------------
    //  Miscellaneous
    // -----------------------------------------------------------------------
    void formatSpec
    (
        const   XMLValidator&   validator
        ,       XMLBuffer&      bufToFill
    )   const;


private :
    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    ContentSpecNode(const ContentSpecNode&);
    void operator=(const ContentSpecNode&);


    // -----------------------------------------------------------------------
    //  Private Data Members
    //
    //  fElemId
    //      If the type is Leaf, then this is the id of the element. If its
    //      fgPCDataElemId, then its a PCData node.
    //
    //  fFirst
    //  fSecond
    //      The optional first and second nodes. The fType field indicates
    //      which of these are valid. The validaty constraints are:
    //
    //          Leaf = Neither valid
    //          ZeroOrOne, ZeroOrMore = First
    //          Choice, Sequence = First and Second
    //
    //  fType
    //      The type of node. This controls how many of the child node fields
    //      are used.
    // -----------------------------------------------------------------------
    unsigned int        fElemId;
    ContentSpecNode*    fFirst;
    ContentSpecNode*    fSecond;
    NodeTypes           fType;
};


// ---------------------------------------------------------------------------
//  ContentSpecNode: Constructors and Destructor
// ---------------------------------------------------------------------------
inline ContentSpecNode::ContentSpecNode() :

    fElemId(XMLElementDecl::fgInvalidElemId)
    , fFirst(0)
    , fSecond(0)
    , fType(ContentSpecNode::Leaf)
{
}

inline
ContentSpecNode::ContentSpecNode(const unsigned int elemId) :

    fElemId(elemId)
    , fFirst(0)
    , fSecond(0)
    , fType(ContentSpecNode::Leaf)
{
}

inline
ContentSpecNode::ContentSpecNode(const  NodeTypes               type
                                ,       ContentSpecNode* const  firstToAdopt
                                ,       ContentSpecNode* const  secondToAdopt) :

    fElemId(XMLElementDecl::fgInvalidElemId)
    , fFirst(firstToAdopt)
    , fSecond(secondToAdopt)
    , fType(type)
{
}

inline ContentSpecNode::~ContentSpecNode()
{
    // Delete our children, which cause recursive cleanup
    delete fFirst;
    delete fSecond;
}


// ---------------------------------------------------------------------------
//  ContentSpecNode: Getter methods
// ---------------------------------------------------------------------------
inline unsigned int ContentSpecNode::getElemId() const
{
    return fElemId;
}

inline ContentSpecNode* ContentSpecNode::getFirst()
{
    return fFirst;
}

inline const ContentSpecNode* ContentSpecNode::getFirst() const
{
    return fFirst;
}

inline ContentSpecNode* ContentSpecNode::getSecond()
{
    return fSecond;
}

inline const ContentSpecNode* ContentSpecNode::getSecond() const
{
    return fSecond;
}

inline ContentSpecNode::NodeTypes ContentSpecNode::getType() const
{
    return fType;
}

inline bool ContentSpecNode::isPCData() const
{
    return ((fType == Leaf) && (fElemId == XMLElementDecl::fgPCDataElemId));
}

inline ContentSpecNode* ContentSpecNode::orphanFirst()
{
    ContentSpecNode* retNode = fFirst;
    fFirst = 0;
    return retNode;
}

inline ContentSpecNode* ContentSpecNode::orphanSecond()
{
    ContentSpecNode* retNode = fSecond;
    fSecond = 0;
    return retNode;
}


// ---------------------------------------------------------------------------
//  ContentSpecType: Setter methods
// ---------------------------------------------------------------------------
inline void ContentSpecNode::setElemId(const unsigned int newId)
{
    fElemId = newId;
}

inline void ContentSpecNode::setFirst(ContentSpecNode* const toAdopt)
{
    delete fFirst;
    fFirst = toAdopt;
}

inline void ContentSpecNode::setSecond(ContentSpecNode* const toAdopt)
{
    delete fSecond;
    fSecond = toAdopt;
}

inline void ContentSpecNode::setType(const NodeTypes type)
{
    fType = type;
}

#endif
