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
 * Revision 1.1  2001/02/27 14:48:45  tng
 * Schema: Add CMAny and ContentLeafNameTypeVector, by Pei Yong Zhang
 *
 */

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <util/XercesDefs.hpp>
#include <util/RuntimeException.hpp>
#include <validators/common/CMStateSet.hpp>
#include <validators/common/CMAny.hpp>

// ---------------------------------------------------------------------------
//  CMUnaryOp: Constructors and Destructor
// ---------------------------------------------------------------------------
CMAny::CMAny( const   ContentSpecNode::NodeTypes  type
                     , const unsigned int         URI
                     , const unsigned int         position ) :
       CMNode(type)
     , fURI(URI)
     , fPosition(position)
{
    if ((type != ContentSpecNode::Any)
    &&  (type != ContentSpecNode::Any_Other)
    &&  (type != ContentSpecNode::Any_Local))
    {
        ThrowXML(RuntimeException, XMLExcepts::CM_BinOpHadUnaryType);
		/***
		ThrowXML1(RuntimeException,
		          XMLExcepts::CM_NotValidSpecTypeForNode,
				  "CMAny");
        ***/
    }

}

CMAny::~CMAny()
{
}

// ---------------------------------------------------------------------------
//  Getter methods
// ---------------------------------------------------------------------------
unsigned int CMAny::getURI() const
{
	return fURI;
}

unsigned int CMAny::getPosition() const
{
    return fPosition;
}

// ---------------------------------------------------------------------------
//  Setter methods
// ---------------------------------------------------------------------------
void CMAny::setPosition(const unsigned int newPosition)
{
    fPosition = newPosition;
}

// ---------------------------------------------------------------------------
//  Implementation of public CMNode virtual interface
// ---------------------------------------------------------------------------
bool CMAny::isNullable() const
{
    // Leaf nodes are never nullable unless its an epsilon node
    return (fPosition == -1);
}

// ---------------------------------------------------------------------------
//  Implementation of protected CMNode virtual interface
// ---------------------------------------------------------------------------
void CMAny::calcFirstPos(CMStateSet& toSet) const
{
    // If we are an epsilon node, then the first pos is an empty set
    if (fPosition == -1)
        toSet.zeroBits();
    else
    // Otherwise, its just the one bit of our position
        toSet.setBit(fPosition);

	return;
}

void CMAny::calcLastPos(CMStateSet& toSet) const
{
    // If we are an epsilon node, then the last pos is an empty set
    if (fPosition == -1)
        toSet.zeroBits();
    // Otherwise, its just the one bit of our position
    else
        toSet.setBit(fPosition);

	return;
}