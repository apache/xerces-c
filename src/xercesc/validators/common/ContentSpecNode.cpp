/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 1999-2001 The Apache Software Foundation.  All rights
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
 * $Id$
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/framework/XMLBuffer.hpp>
#include <xercesc/validators/common/ContentSpecNode.hpp>
#include <xercesc/validators/schema/SchemaSymbols.hpp>

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  ContentSpecNode: Copy Constructor
//
//  Note: this copy constructor has dependency on various get*() methods
//        and shall be placed after those method's declaration.
//        aka inline function compilation error on AIX 4.2, xlC 3 r ev.1
// ---------------------------------------------------------------------------

ContentSpecNode::ContentSpecNode(const ContentSpecNode& toCopy) :
    fMemoryManager(toCopy.fMemoryManager)
    , fElement(0)
    , fElementDecl(toCopy.fElementDecl)
    , fFirst(0)
    , fSecond(0)
    , fType(toCopy.fType)
    , fAdoptFirst(true)
    , fAdoptSecond(true)
    , fMinOccurs(toCopy.fMinOccurs)
    , fMaxOccurs(toCopy.fMaxOccurs)
{
    const QName* tempElement = toCopy.getElement();
    if (tempElement)
        fElement = new (fMemoryManager) QName(*tempElement);

    const ContentSpecNode *tmp = toCopy.getFirst();
    if (tmp)
        fFirst = new (fMemoryManager) ContentSpecNode(*tmp);

    tmp = toCopy.getSecond();
    if (tmp)
        fSecond = new (fMemoryManager) ContentSpecNode(*tmp);
}

// ---------------------------------------------------------------------------
//  Local methods
// ---------------------------------------------------------------------------
static void formatNode( const   ContentSpecNode* const      curNode
                        , const ContentSpecNode::NodeTypes  parentType
                        ,       XMLBuffer&                  bufToFill)
{
    if (!curNode)
        return;

    const ContentSpecNode* first = curNode->getFirst();
    const ContentSpecNode* second = curNode->getSecond();
    const ContentSpecNode::NodeTypes curType = curNode->getType();

    // Get the type of the first node
    const ContentSpecNode::NodeTypes firstType = first ?
                                                 first->getType() :
                                                 ContentSpecNode::Leaf;

    // Calculate the parens flag for the rep nodes
    bool doRepParens = false;
    if (((firstType != ContentSpecNode::Leaf)
            && (parentType != ContentSpecNode::UnknownType))
    ||  ((firstType == ContentSpecNode::Leaf)
            && (parentType == ContentSpecNode::UnknownType)))
    {
        doRepParens = true;
    }

    // Now handle our type
    switch(curType & 0x0f)
    {
        case ContentSpecNode::Leaf :
            if (curNode->getElement()->getURI() == XMLElementDecl::fgPCDataElemId)
                bufToFill.append(XMLElementDecl::fgPCDataElemName);
            else
                bufToFill.append(curNode->getElement()->getRawName());
            break;

        case ContentSpecNode::ZeroOrOne :
            if (doRepParens)
                bufToFill.append(chOpenParen);
            formatNode(first, curType, bufToFill);
            if (doRepParens)
                bufToFill.append(chCloseParen);
            bufToFill.append(chQuestion);
            break;

        case ContentSpecNode::ZeroOrMore :
            if (doRepParens)
                bufToFill.append(chOpenParen);
            formatNode(first, curType, bufToFill);
            if (doRepParens)
                bufToFill.append(chCloseParen);
            bufToFill.append(chAsterisk);
            break;

        case ContentSpecNode::OneOrMore :
            if (doRepParens)
                bufToFill.append(chOpenParen);
            formatNode(first, curType, bufToFill);
            if (doRepParens)
                bufToFill.append(chCloseParen);
            bufToFill.append(chPlus);
            break;

        case ContentSpecNode::Choice :
            if (parentType != curType)
                bufToFill.append(chOpenParen);
            formatNode(first, curType, bufToFill);
            bufToFill.append(chPipe);
            formatNode(second, curType, bufToFill);
            if (parentType != curType)
                bufToFill.append(chCloseParen);
            break;

        case ContentSpecNode::Sequence :
            if (parentType != curType)
                bufToFill.append(chOpenParen);
            formatNode(first, curType, bufToFill);
            bufToFill.append(chComma);
            formatNode(second, curType, bufToFill);
            if (parentType != curType)
                bufToFill.append(chCloseParen);
            break;

        case ContentSpecNode::All :
            if (parentType != curType)
			{
                bufToFill.append(chLatin_A);
                bufToFill.append(chLatin_l);
                bufToFill.append(chLatin_l);
                bufToFill.append(chOpenParen);
			}
            formatNode(first, curType, bufToFill);
            bufToFill.append(chComma);
            formatNode(second, curType, bufToFill);
            if (parentType != curType)
                bufToFill.append(chCloseParen);
            break;
    }
}


// ---------------------------------------------------------------------------
//  ContentSpecNode: Miscellaneous
// ---------------------------------------------------------------------------
void ContentSpecNode::formatSpec(XMLBuffer&      bufToFill) const
{
    // Clean out the buffer first
    bufToFill.reset();

    if (fType == ContentSpecNode::Leaf)
        bufToFill.append(chOpenParen);
    formatNode
    (
        this
        , UnknownType
        , bufToFill
    );
    if (fType == ContentSpecNode::Leaf)
        bufToFill.append(chCloseParen);
}

int ContentSpecNode::getMinTotalRange() const {

    int min = fMinOccurs;

    if ((fType & 0x0f) == ContentSpecNode::Sequence
        || fType == ContentSpecNode::All
        || (fType & 0x0f) == ContentSpecNode::Choice) {

        int minFirst = fFirst->getMinTotalRange();

        if (fSecond) {

            int minSecond = fSecond->getMinTotalRange();

            if ((fType & 0x0f) == ContentSpecNode::Choice) {
                min = min * ((minFirst < minSecond)? minFirst : minSecond);
            }
            else {
                min = min * (minFirst + minSecond);
            }
        }
        else
            min = min * minFirst;
    }

    return min;
}

int ContentSpecNode::getMaxTotalRange() const {

    int max = fMaxOccurs;

    if (max == SchemaSymbols::XSD_UNBOUNDED) {
         return SchemaSymbols::XSD_UNBOUNDED;
    }

    if ((fType & 0x0f) == ContentSpecNode::Sequence
        || fType == ContentSpecNode::All
        || (fType & 0x0f) == ContentSpecNode::Choice) {

        int maxFirst = fFirst->getMaxTotalRange();

        if (maxFirst == SchemaSymbols::XSD_UNBOUNDED) {
             return SchemaSymbols::XSD_UNBOUNDED;
        }

        if (fSecond) {

            int maxSecond = fSecond->getMaxTotalRange();

            if (maxSecond == SchemaSymbols::XSD_UNBOUNDED) {
                return SchemaSymbols::XSD_UNBOUNDED;
            }
            else {

                if ((fType & 0x0f) == ContentSpecNode::Choice) {
                    max = max * (maxFirst > maxSecond) ? maxFirst : maxSecond;
                }
                else {
                    max = max * (maxFirst + maxSecond);
                }
            }
        }
        else {
            max = max * maxFirst;
        }
    }

    return max;
}

/***
 * Support for Serialization/De-serialization
 ***/

IMPL_XSERIALIZABLE_TOCREATE(ContentSpecNode)

void ContentSpecNode::serialize(XSerializeEngine& serEng)
{
    /***
     *  Since fElement, fFirst, fSecond are NOT created by the default 
     *  constructor, we need to create them dynamically.
     ***/

    if (serEng.isStoring())
    {
        serEng<<fElement;
        XMLElementDecl::storeElementDecl(serEng, fElementDecl);
        serEng<<fFirst;
        serEng<<fSecond;

        serEng<<(int)fType;
        serEng<<fAdoptFirst;
        serEng<<fAdoptSecond;
        serEng<<fMinOccurs;
        serEng<<fMaxOccurs;
    }
    else
    {
        serEng>>fElement;
        fElementDecl = XMLElementDecl::loadElementDecl(serEng);
        serEng>>fFirst;
        serEng>>fSecond;

        int type;
        serEng>>type;
        fType = (NodeTypes)type;

        serEng>>fAdoptFirst;
        serEng>>fAdoptSecond;
        serEng>>fMinOccurs;
        serEng>>fMaxOccurs;
    }

}

XERCES_CPP_NAMESPACE_END

