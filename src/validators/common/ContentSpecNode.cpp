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
 * $Id$
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <util/Janitor.hpp>
#include <util/XMLUniDefs.hpp>
#include <util/XMLUni.hpp>
#include <framework/XMLNotationDecl.hpp>
#include <framework/XMLBuffer.hpp>
#include <validators/common/ContentSpecNode.hpp>
#include <validators/DTD/DTDValidator.hpp>



// ---------------------------------------------------------------------------
//  Local methods
// ---------------------------------------------------------------------------
static void formatNode( const   ContentSpecNode* const      curNode
                        , const ContentSpecNode::NodeTypes  parentType
                        , const Grammar&                    grammar
                        ,       XMLBuffer&                  bufToFill)
{
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
    unsigned int tmpVal;
    switch(curType)
    {
        case ContentSpecNode::Leaf :
            tmpVal = curNode->getElemId();
            if (tmpVal == XMLElementDecl::fgPCDataElemId)
                bufToFill.append(XMLElementDecl::fgPCDataElemName);
            else
                bufToFill.append(grammar.getElemDecl(tmpVal)->getFullName());
            break;

        case ContentSpecNode::ZeroOrOne :
            if (doRepParens)
                bufToFill.append(chOpenParen);
            formatNode(first, curType, grammar, bufToFill);
            if (doRepParens)
                bufToFill.append(chCloseParen);
            bufToFill.append(chQuestion);
            break;

        case ContentSpecNode::ZeroOrMore :
            if (doRepParens)
                bufToFill.append(chOpenParen);
            formatNode(first, curType, grammar, bufToFill);
            if (doRepParens)
                bufToFill.append(chCloseParen);
            bufToFill.append(chAsterisk);
            break;

        case ContentSpecNode::OneOrMore :
            if (doRepParens)
                bufToFill.append(chOpenParen);
            formatNode(first, curType, grammar, bufToFill);
            if (doRepParens)
                bufToFill.append(chCloseParen);
            bufToFill.append(chPlus);
            break;

        case ContentSpecNode::Choice :
            if (parentType != curType)
                bufToFill.append(chOpenParen);
            formatNode(first, curType, grammar, bufToFill);
            bufToFill.append(chPipe);
            formatNode(second, curType, grammar, bufToFill);
            if (parentType != curType)
                bufToFill.append(chCloseParen);
            break;

        case ContentSpecNode::Sequence :
            if (parentType != curType)
                bufToFill.append(chOpenParen);
            formatNode(first, curType, grammar, bufToFill);
            bufToFill.append(chComma);
            formatNode(second, curType, grammar, bufToFill);
            if (parentType != curType)
                bufToFill.append(chCloseParen);
            break;
    }
}


// ---------------------------------------------------------------------------
//  ContentSpecNode: Miscellaneous
// ---------------------------------------------------------------------------
void ContentSpecNode::formatSpec(const  Grammar&   grammar
                                ,       XMLBuffer&      bufToFill) const
{
    // Clean out the buffer first
    bufToFill.reset();

    if (fType == ContentSpecNode::Leaf)
        bufToFill.append(chOpenParen);
    formatNode
    (
        this
        , UnknownType
        , grammar
        , bufToFill
    );
    if (fType == ContentSpecNode::Leaf)
        bufToFill.append(chCloseParen);
}
