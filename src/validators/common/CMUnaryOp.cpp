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
 * Revision 1.3  2000/03/02 19:55:37  roddey
 * This checkin includes many changes done while waiting for the
 * 1.1.0 code to be finished. I can't list them all here, but a list is
 * available elsewhere.
 *
 * Revision 1.2  2000/02/09 21:42:37  abagchi
 * Copyright swatswat
 *
 * Revision 1.1.1.1  1999/11/09 01:03:08  twl
 * Initial checkin
 *
 * Revision 1.2  1999/11/08 20:45:37  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <util/XercesDefs.hpp>
#include <util/RuntimeException.hpp>
#include <validators/DTD/CMStateSet.hpp>
#include <validators/DTD/CMUnaryOp.hpp>


// ---------------------------------------------------------------------------
//  CMUnaryOp: Constructors and Destructor
// ---------------------------------------------------------------------------
CMUnaryOp::CMUnaryOp(   const   ContentSpecNode::NodeTypes  type
                        ,       CMNode* const               nodeToAdopt) :
    CMNode(type)
    , fChild(nodeToAdopt)
{
    // Insure that its one of the types we require
    if ((type != ContentSpecNode::ZeroOrOne)
    &&  (type != ContentSpecNode::ZeroOrMore)
    &&  (type != ContentSpecNode::OneOrMore))
    {
        ThrowXML(RuntimeException, XMLExcepts::CM_UnaryOpHadBinType);
    }
}

CMUnaryOp::~CMUnaryOp()
{
    delete fChild;
}


// ---------------------------------------------------------------------------
//  CMUnaryOp: Getter methods
// ---------------------------------------------------------------------------
const CMNode* CMUnaryOp::getChild() const
{
    return fChild;
}

CMNode* CMUnaryOp::getChild()
{
    return fChild;
}


// ---------------------------------------------------------------------------
//  CMUnaryOp: Implementation of the public CMNode virtual interface
// ---------------------------------------------------------------------------
bool CMUnaryOp::isNullable() const
{
    // Repetition operations are always nullable
    return true;
}


// ---------------------------------------------------------------------------
//  CMUnaryOp: Implementation of the protected CMNode virtual interface
// ---------------------------------------------------------------------------
void CMUnaryOp::calcFirstPos(CMStateSet& toSet) const
{
    // Its just based on our child node's first pos
    toSet = fChild->getFirstPos();
}

void CMUnaryOp::calcLastPos(CMStateSet& toSet) const
{
    // Its just based on our child node's last pos
    toSet = fChild->getLastPos();
}
