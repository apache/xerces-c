/*
 * Copyright 2001,2004 The Apache Software Foundation.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *      http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * $Log$
 * Revision 1.9  2004/09/08 13:56:47  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.8  2003/12/24 15:24:15  cargilld
 * More updates to memory management so that the static memory manager.
 *
 * Revision 1.7  2003/12/17 00:18:37  cargilld
 * Update to memory management so that the static memory manager (one used to call Initialize) is only for static data.
 *
 * Revision 1.6  2003/05/18 14:02:06  knoaman
 * Memory manager implementation: pass per instance manager.
 *
 * Revision 1.5  2003/05/16 21:37:00  knoaman
 * Memory manager implementation: Modify constructors to pass in the memory manager.
 *
 * Revision 1.4  2003/05/16 00:03:10  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.3  2002/11/04 15:17:01  tng
 * C++ Namespace Support.
 *
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

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  Static member data initialization
// ---------------------------------------------------------------------------
const unsigned short UnionToken::INITIALSIZE = 8;

// ---------------------------------------------------------------------------
//  UnionToken: Constructors and Destructors
// ---------------------------------------------------------------------------
UnionToken::UnionToken(const unsigned short tokType, MemoryManager* const manager)
    : Token(tokType, manager)
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
        fChildren = new (tokFactory->getMemoryManager()) RefVectorOf<Token>(INITIALSIZE, false, tokFactory->getMemoryManager());

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
    XMLBuffer stringBuf(1023, tokFactory->getMemoryManager());

    if (previousType == T_CHAR) {

        XMLInt32 ch = previousTok->getChar();

        if (ch >= 0x10000) {

            XMLCh* chSurrogate = RegxUtil::decomposeToSurrogates(ch, tokFactory->getMemoryManager());
            stringBuf.append(chSurrogate);
            tokFactory->getMemoryManager()->deallocate(chSurrogate);//delete [] chSurrogate;
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

            XMLCh* chSurrogate = RegxUtil::decomposeToSurrogates(ch, tokFactory->getMemoryManager());
            stringBuf.append(chSurrogate);
            tokFactory->getMemoryManager()->deallocate(chSurrogate);//delete [] chSurrogate;
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

XERCES_CPP_NAMESPACE_END

/**
  * End of file UnionToken.cpp
  */
