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
 * Revision 1.4  2000/03/02 19:55:37  roddey
 * This checkin includes many changes done while waiting for the
 * 1.1.0 code to be finished. I can't list them all here, but a list is
 * available elsewhere.
 *
 * Revision 1.3  2000/02/24 20:16:47  abagchi
 * Swat for removing Log from API docs
 *
 * Revision 1.2  2000/02/09 21:42:36  abagchi
 * Copyright swat
 *
 * Revision 1.1.1.1  1999/11/09 01:03:04  twl
 * Initial checkin
 *
 * Revision 1.2  1999/11/08 20:45:36  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */

#if !defined(CMLEAF_HPP)
#define CMLEAF_HPP

#include <util/XercesDefs.hpp>
#include <util/RuntimeException.hpp>
#include <validators/DTD/ContentSpecNode.hpp>
#include <validators/DTD/CMNode.hpp>
#include <validators/DTD/CMStateSet.hpp>

//
//  This class represents a leaf in the content spec node tree of an
//  element's content model. It just has an element id and a position value,
//  the latter of which is used during the building of a DFA.
//
class CMLeaf : public CMNode
{
public :
    // -----------------------------------------------------------------------
    //  Constructors
    // -----------------------------------------------------------------------
    CMLeaf
    (
        const   unsigned int    elemId
        , const unsigned int    position = (unsigned int)-1
    );
    ~CMLeaf();


    // -----------------------------------------------------------------------
    //  Getter methods
    // -----------------------------------------------------------------------
    unsigned int getId() const;
    unsigned int getPosition() const;


    // -----------------------------------------------------------------------
    //  Setter methods
    // -----------------------------------------------------------------------
    void setPosition(const unsigned int newPosition);


    // -----------------------------------------------------------------------
    //  Implementation of public CMNode virtual interface
    // -----------------------------------------------------------------------
    bool isNullable() const;


protected :
    // -----------------------------------------------------------------------
    //  Implementation of protected CMNode virtual interface
    // -----------------------------------------------------------------------
    void calcFirstPos(CMStateSet& toSet) const;
    void calcLastPos(CMStateSet& toSet) const;


private :
    // -----------------------------------------------------------------------
    //  Private data members
    //
    //  fElemId
    //      This is the element id of the element that this leaf represents.
    //
    //  fPosition
    //      Part of the algorithm to convert a regex directly to a DFA
    //      numbers each leaf sequentially. If its -1, that means its an
    //      epsilon node. All others are non-epsilon positions.
    // -----------------------------------------------------------------------
    unsigned int    fElemId;
    unsigned int    fPosition;
};


// -----------------------------------------------------------------------
//  Constructors
// -----------------------------------------------------------------------
inline CMLeaf::CMLeaf(  const   unsigned int    elemId
                        , const unsigned int    position) :
    CMNode(ContentSpecNode::Leaf)
    , fElemId(elemId)
    , fPosition(position)
{
}

inline CMLeaf::~CMLeaf()
{
}


// ---------------------------------------------------------------------------
//  Getter methods
// ---------------------------------------------------------------------------
inline unsigned int CMLeaf::getId() const
{
    return fElemId;
}

inline unsigned int CMLeaf::getPosition() const
{
    return fPosition;
}


// ---------------------------------------------------------------------------
//  Setter methods
// ---------------------------------------------------------------------------
inline void CMLeaf::setPosition(const unsigned int newPosition)
{
    fPosition = newPosition;
}


// ---------------------------------------------------------------------------
//  Implementation of public CMNode virtual interface
// ---------------------------------------------------------------------------
inline bool CMLeaf::isNullable() const
{
    // Leaf nodes are never nullable unless its an epsilon node
    return (fPosition == -1);
}


// ---------------------------------------------------------------------------
//  Implementation of protected CMNode virtual interface
// ---------------------------------------------------------------------------
inline void CMLeaf::calcFirstPos(CMStateSet& toSet) const
{
    // If we are an epsilon node, then the first pos is an empty set
    if (fPosition == -1)
    {
        toSet.zeroBits();
        return;
    }

    // Otherwise, its just the one bit of our position
    toSet.setBit(fPosition);
}

inline void CMLeaf::calcLastPos(CMStateSet& toSet) const
{
    // If we are an epsilon node, then the last pos is an empty set
    if (fPosition == -1)
    {
        toSet.zeroBits();
        return;
    }

    // Otherwise, its just the one bit of our position
    toSet.setBit(fPosition);
}

#endif
