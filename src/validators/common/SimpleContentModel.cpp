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
 * Revision 1.4  2000/03/03 22:33:00  roddey
 * Fixed a bug in SimpleContentModel that allowed an <a/> to be taken
 * as valid for a content model of (a,b).
 *
 * Revision 1.3  2000/03/02 19:55:40  roddey
 * This checkin includes many changes done while waiting for the
 * 1.1.0 code to be finished. I can't list them all here, but a list is
 * available elsewhere.
 *
 * Revision 1.2  2000/02/09 21:42:39  abagchi
 * Copyright swatswat
 *
 * Revision 1.1.1.1  1999/11/09 01:03:46  twl
 * Initial checkin
 *
 * Revision 1.2  1999/11/08 20:45:44  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <util/RuntimeException.hpp>
#include <validators/common/SimpleContentModel.hpp>


// ---------------------------------------------------------------------------
//  SimpleContentModel: Implementation of the ContentModel virtual interface
// ---------------------------------------------------------------------------

//
//  For this content model, the only way it can be ambiguous is if it is a
//  choice and both sides are the same.
//
bool SimpleContentModel::getIsAmbiguous() const
{
    if (fOp != ContentSpecNode::Choice)
        return false;

    return fFirstChild == fSecondChild;
}


//
//  This method is called to validate our content. For this one, its just a
//  pretty simple 'bull your way through it' test according to what kind of
//  operation it is for.
//
int
SimpleContentModel::validateContent(const   unsigned int*   childIds
                                    , const unsigned int    childCount) const
{
    //
    //  According to the type of operation, we do the correct type of
    //  content check.
    //
    unsigned int index;
    switch(fOp)
    {
        case ContentSpecNode::Leaf :
            //
            //  There can only be one child and it has to be of the
            //  element type we stored.
            //
            if (!childCount)
                return 0;

            if (childIds[0] != fFirstChild)
                return 0;

            if (childCount > 1)
                return 1;
            break;

        case ContentSpecNode::ZeroOrOne :
            //
            //  If the child count is greater than one, then obviously
            //  bad. Otherwise, if its one, then the one child must be
            //  of the type we stored.
            //
            if ((childCount == 1) && (childIds[0] != fFirstChild))
                return 0;

            if (childCount > 1)
                return 1;
            break;

        case ContentSpecNode::ZeroOrMore :
            //
            //  If the child count is zero, that's fine. If its more than
            //  zero, then make sure that all children are of the element
            //  type that we stored.
            //
            if (childCount > 0)
            {
                for (index = 0; index < childCount; index++)
                {
                    if (childIds[index] != fFirstChild)
                        return index;
                }
            }
            break;

        case ContentSpecNode::OneOrMore :
            //
            //  If the child count is zero, that's an error. If its more
            //  than zero, then make sure that all children are of the
            //  element type that we stored.
            //
            if (childCount == 0)
                return 0;

            for (index = 0; index < childCount; index++)
            {
                if (childIds[index] != fFirstChild)
                    return index;
            }
            break;

        case ContentSpecNode::Choice :
            //
            //  There can only be one child, and it must be one of the
            //  two types we stored.
            //
            if (!childCount)
                return 0;

            if ((childIds[0] != fFirstChild) && (childIds[0] != fSecondChild))
                return 0;

            if (childCount > 1)
                return 1;
            break;

        case ContentSpecNode::Sequence :
            //
            //  There must be two children and they must be the two values
            //  we stored, in the stored order. So first check the obvious
            //  problem of an empty content, which would never be valid
            //  in this content mode.
            //
            if (!childCount)
                return 0;

            // If we have at least one child, its got to match our first
            if ((childCount >= 1) && (childIds[0] != fFirstChild))
                return 0;

            // If we hvae at least two children, its got to match our second
            if ((childCount >= 2) && (childIds[1] != fSecondChild))
                return 1;

            // If we only had one (and it was valid), then too few children
            if (childCount == 1)
                return 1;

            // And finally check for too many children
            if (childCount > 2)
                return 2;

            break;

        default :
            ThrowXML(RuntimeException, XMLExcepts::CM_UnknownCMSpecType);
            break;
    }
    return -1;
}

int SimpleContentModel::validateContentSpecial(  const   unsigned int*   childIds
                                            , const unsigned int    childCount) const
{
	return 0;
};

ContentLeafNameTypeVector* SimpleContentModel::getContentLeafNameTypeVector() const
{
    return 0;
};
