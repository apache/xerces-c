/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 2001 The Apache Software Foundation.  All rights
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
 * originally based on software copyright (c) 2001, International
 * Business Machines, Inc., http://www.ibm.com .  For more information
 * on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 */

/*
 * $Log$
 * Revision 1.2  2002/03/18 19:29:53  knoaman
 * Change constant names to eliminate possible conflict with user defined ones.
 *
 * Revision 1.1.1.1  2002/02/01 22:22:34  peiyongz
 * sane_include
 *
 * Revision 1.4  2001/06/05 14:50:32  knoaman
 * Fixes to regular expression.
 *
 * Revision 1.3  2001/05/11 13:26:52  tng
 * Copyright update.
 *
 * Revision 1.2  2001/05/03 18:17:59  knoaman
 * Some design changes:
 * o Changed the TokenFactory from a single static instance, to a
 *    normal class. Each RegularExpression object will have its own
 *    instance of TokenFactory, and that instance will be passed to
 *    other classes that need to use a TokenFactory to create Token
 *    objects (with the exception of RangeTokenMap).
 * o Added a new class RangeTokenMap to map a the different ranges
 *    in a given category to a specific RangeFactory object. In the old
 *    design RangeFactory had dual functionality (act as a Map, and as
 *    a factory for creating RangeToken(s)). The RangeTokenMap will
 *    have its own copy of the TokenFactory. There will be only one
 *    instance of the RangeTokenMap class, and that instance will be
 *    lazily deleted when XPlatformUtils::Terminate is called.
 *
 * Revision 1.1  2001/03/02 19:23:02  knoaman
 * Schema: Regular expression handling part I
 *
 */

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/util/regx/UnionToken.hpp>
#include <xercesc/framework/XMLBuffer.hpp>
#include <xercesc/util/regx/RegxUtil.hpp>
#include <xercesc/util/regx/TokenFactory.hpp>
#include <xercesc/util/regx/StringToken.hpp>

// ---------------------------------------------------------------------------
//  Static member data initialization
// ---------------------------------------------------------------------------
const unsigned short UnionToken::INITIALSIZE = 8;

// ---------------------------------------------------------------------------
//  UnionToken: Constructors and Destructors
// ---------------------------------------------------------------------------
UnionToken::UnionToken(const unsigned short tokType)
    : Token(tokType)
    , fChildren(0)
{

}

UnionToken::~UnionToken() {

    delete fChildren;
}


// ---------------------------------------------------------------------------
//  UnionToken: Children manipulation methods
// ---------------------------------------------------------------------------
void UnionToken::addChild(Token* const child, TokenFactory* const tokFactory) {

    if (child == 0)
        return;

    if (fChildren == 0)
        fChildren = new RefVectorOf<Token>(INITIALSIZE, false);

    if (getTokenType() == T_UNION) {

        fChildren->addElement(child);
        return;
    }

    unsigned short childType = child->getTokenType();
    unsigned int   childSize = child->size();

    if (childType == T_CONCAT) {

        for (unsigned int i = 0; i < childSize; i++) {

            addChild(child->getChild(i), tokFactory);
        }

        return;
    }

    unsigned int childrenSize = fChildren->size();
    if (childrenSize == 0) {

        fChildren->addElement(child);
        return;
    }

    Token* previousTok = fChildren->elementAt(childrenSize - 1);
    unsigned short previousType = previousTok->getTokenType();

    if (!((previousType == T_CHAR || previousType == T_STRING)
          && (childType == T_CHAR || childType == T_STRING))) {

        fChildren->addElement(child);
        return;
    }

    // Continue
    XMLBuffer stringBuf;

    if (previousType == T_CHAR) {

        XMLInt32 ch = previousTok->getChar();

        if (ch >= 0x10000) {

            XMLCh* chSurrogate = RegxUtil::decomposeToSurrogates(ch);
            stringBuf.append(chSurrogate);
            delete [] chSurrogate;
        }
        else {
            stringBuf.append((XMLCh) ch);
        }

        previousTok = tokFactory->createString(0);
        fChildren->setElementAt(previousTok, childrenSize - 1);
    }
    else {
        stringBuf.append(previousTok->getString());
    }

    if (childType == T_CHAR) {

        XMLInt32 ch = child->getChar();

        if (ch >= 0x10000) {

            XMLCh* chSurrogate = RegxUtil::decomposeToSurrogates(ch);
            stringBuf.append(chSurrogate);
            delete [] chSurrogate;
        }
        else {
            stringBuf.append((XMLCh) ch);
        }
    }
    else {
        stringBuf.append(child->getString());
    }

    ((StringToken*) previousTok)->setString(stringBuf.getRawBuffer());
}

/**
  * End of file UnionToken.cpp
  */
