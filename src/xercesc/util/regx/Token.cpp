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
 * Revision 1.7  2004/09/08 13:56:47  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.6  2003/12/17 00:18:37  cargilld
 * Update to memory management so that the static memory manager (one used to call Initialize) is only for static data.
 *
 * Revision 1.5  2002/11/21 14:56:35  gareth
 * Fixed bug in Token::analyzeFirstCharacter so that . matches new line with head character optimisation enabled. As per discussion Jennifer Schachter had with Khaled.
 *
 * Revision 1.4  2002/11/04 15:17:00  tng
 * C++ Namespace Support.
 *
 * Revision 1.3  2002/10/15 18:11:02  knoaman
 * [Bug 13489]: missing 'return' in Token.cpp
 *
 * Revision 1.2  2002/03/18 19:29:53  knoaman
 * Change constant names to eliminate possible conflict with user defined ones.
 *
 * Revision 1.1.1.1  2002/02/01 22:22:31  peiyongz
 * sane_include
 *
 * Revision 1.3  2001/05/11 13:26:50  tng
 * Copyright update.
 *
 * Revision 1.2  2001/05/03 18:17:49  knoaman
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
 * Revision 1.1  2001/03/02 19:22:58  knoaman
 * Schema: Regular expression handling part I
 *
 */

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/util/regx/RangeToken.hpp>
#include <xercesc/util/regx/ModifierToken.hpp>
#include <xercesc/util/regx/RegularExpression.hpp>
#include <xercesc/util/regx/RegxUtil.hpp>

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  Static member data initialization
// ---------------------------------------------------------------------------
const XMLInt32		 Token::UTF16_MAX = 0x10FFFF;
const unsigned short Token::FC_CONTINUE = 0;
const unsigned short Token::FC_TERMINAL = 1;
const unsigned short Token::FC_ANY = 2;

// ---------------------------------------------------------------------------
//  Token: Constructors and Destructors
// ---------------------------------------------------------------------------
Token::Token(const unsigned short tokType
             , MemoryManager* const manager) 
             : fTokenType(tokType) 
             , fMemoryManager(manager)
{

}


Token::~Token() {

}

// ---------------------------------------------------------------------------
//  Token: Getter mthods
// ---------------------------------------------------------------------------
int Token::getMinLength() const {

	switch (fTokenType) {

	case T_CONCAT:
        {
            int sum = 0;
            unsigned int childSize = size();

            for (unsigned int i=0; i<childSize; i++) {
                sum += getChild(i)->getMinLength();
            }
            return sum;
        }
	case T_CONDITION:
	case T_UNION:
        {
			unsigned int childSize = size();

            if (childSize == 0) {
                return 0;
            }
			int ret = getChild(0)->getMinLength();

            for (unsigned int i=1; i < childSize; i++) {

                int min = getChild(i)->getMinLength();
                if (min < ret)
                    ret = min;
            }
			return ret;
        }
	case T_CLOSURE:
	case T_NONGREEDYCLOSURE:
        if (getMin() >= 0)
            return getMin() * getChild(0)->getMinLength();

		return 0;
	case T_EMPTY:
	case T_ANCHOR:
		return 0;
	case T_DOT:
	case T_CHAR:
	case T_RANGE:
	case T_NRANGE:
		return 1;
	case T_INDEPENDENT:
	case T_PAREN:
	case T_MODIFIERGROUP:
		return getChild(0)->getMinLength();
	case T_BACKREFERENCE:
		return 0; // *****  - REVISIT
	case T_STRING:
		return XMLString::stringLen(getString());
	case T_LOOKAHEAD:
	case T_NEGATIVELOOKAHEAD:
	case T_LOOKBEHIND:
	case T_NEGATIVELOOKBEHIND:
		return 0; // *****  - REVIST
//	default:
//		throw;
	}

	// We should not get here, but we have it to make some compilers happy
	return -1;
}


int Token::getMaxLength() const {

	switch (fTokenType) {

	case T_CONCAT:
        {
            int sum = 0;
            unsigned int childSize = size();

            for (unsigned int i=0; i<childSize; i++) {

                int val = getChild(i)->getMaxLength();

				if (val < 0){
					return -1;
				}
                sum += val;
            }
            return sum;
        }
    case T_CONDITION:
    case T_UNION:
        {
            unsigned int childSize = size();

			if (childSize == 0)
                return 0;

            int ret = getChild(0)->getMaxLength();

            for (unsigned i = 1; ret > 0 && i < childSize; i++) {

                int max = getChild(i)->getMaxLength();

                if (max < 0) {

                    ret = -1;
                    break;
                }

                if (max > ret)
                    ret = max;
            }
			return ret;
        }
    case T_CLOSURE:
    case T_NONGREEDYCLOSURE:
        if (getMax() >= 0) {
            return getMax() * getChild(0)->getMaxLength();
		}
		return -1;
    case T_EMPTY:
    case T_ANCHOR:
        return 0;
    case T_CHAR:
        return 1;
    case T_DOT:
    case T_RANGE:
    case T_NRANGE:
        return 2;
    case T_INDEPENDENT:
    case T_PAREN:
    case T_MODIFIERGROUP:
        return getChild(0)->getMaxLength();
    case T_BACKREFERENCE:
		return -1; // REVISIT
    case T_STRING:
        return XMLString::stringLen(getString());
    case T_LOOKAHEAD:
    case T_NEGATIVELOOKAHEAD:
    case T_LOOKBEHIND:
    case T_NEGATIVELOOKBEHIND:
		return 0; // REVISIT
//    default:
//		throw; //ThrowXML(RuntimeException, ...)
    } // end switch

	return -1;
}

// ---------------------------------------------------------------------------
//  Token: Helper mthods
// ---------------------------------------------------------------------------
int Token::analyzeFirstCharacter(RangeToken* const rangeTok,
								 const int options,
                                 TokenFactory* const tokFactory)
{
	switch(fTokenType) {
	case T_CONCAT:
		{
			int ret = FC_CONTINUE;
			for (int i=0; i<size(); i++) {

				Token* tok = getChild(i);
				if (tok
					&& (ret=tok->analyzeFirstCharacter(rangeTok,
                                    options, tokFactory))!= FC_CONTINUE)
					break;
			}
			return ret;
		}
	case T_UNION:
		{
			unsigned int childSize = size();
            if (childSize == 0)
                return FC_CONTINUE;

            int ret = FC_CONTINUE;
			bool hasEmpty = false;

			for (unsigned int i=0; i < childSize; i++) {

                ret = getChild(i)->analyzeFirstCharacter(rangeTok, options, tokFactory);

				if (ret == FC_ANY)
					break;
				else
					hasEmpty = true;
			}
			return hasEmpty ? FC_CONTINUE : ret;
		}
	case T_CONDITION:
		{
            int ret1 = getChild(0)->analyzeFirstCharacter(rangeTok, options, tokFactory);

            if (size() == 1)
                return FC_CONTINUE;

			int ret2;
			if (ret1 != FC_ANY) {
			    ret2 = getChild(1)->analyzeFirstCharacter(rangeTok, options, tokFactory);
			}

			if (ret1 == FC_ANY || ret2 == FC_ANY)
				return FC_ANY;

			if (ret1 == FC_CONTINUE || ret2 == FC_CONTINUE)
				return FC_CONTINUE;

			return FC_TERMINAL;
		}
	case T_CLOSURE:
	case T_NONGREEDYCLOSURE:
		{
			Token* tok = getChild(0);
			if (tok)
				tok->analyzeFirstCharacter(rangeTok, options, tokFactory);
			return FC_CONTINUE;
		}
	case T_DOT:
    return FC_ANY;
	case T_EMPTY:
	case T_ANCHOR:
		return FC_CONTINUE;
	case T_CHAR:
		{
            XMLInt32 ch = getChar();
			rangeTok->addRange(ch, ch);
			if (ch < 0x1000 && isSet(options,RegularExpression::IGNORE_CASE)) {
				//REVISIT
			}
		}
		return FC_TERMINAL;
	case T_RANGE:
		{
			if (isSet(options, RegularExpression::IGNORE_CASE)) {
                rangeTok->mergeRanges(((RangeToken*)
                                         this)->getCaseInsensitiveToken(tokFactory));
			}
			else {
				rangeTok->mergeRanges(this);
			}
			return FC_TERMINAL;
		}
	case T_NRANGE:
		{
			if (isSet(options, RegularExpression::IGNORE_CASE)) {

				RangeToken* caseITok = (((RangeToken*)
					                       this)->getCaseInsensitiveToken(tokFactory));
				rangeTok->mergeRanges(RangeToken::complementRanges(caseITok, tokFactory, fMemoryManager));
			}
			else {
				rangeTok->mergeRanges(
					RangeToken::complementRanges((RangeToken*) this, tokFactory, fMemoryManager));
			}
		}
	case T_INDEPENDENT:
	case T_PAREN:
		{
			Token* tok = getChild(0);
			if (tok)
				return tok->analyzeFirstCharacter(rangeTok,options, tokFactory);
		}
	case T_MODIFIERGROUP:
	case T_BACKREFERENCE:
		rangeTok->addRange(0, UTF16_MAX);
		return FC_ANY;
	case T_STRING:
		{
			const XMLCh* str = getString();
            XMLInt32 ch = str[0];

			if (RegxUtil::isHighSurrogate((XMLCh) ch)) {
			}

		    rangeTok->addRange(ch, ch);
		    if (ch<0x10000 && isSet(options,RegularExpression::IGNORE_CASE)) {
                //REVISIT
            }
		}
		return FC_TERMINAL;
	case T_LOOKAHEAD:
	case T_NEGATIVELOOKAHEAD:
	case T_LOOKBEHIND:
	case T_NEGATIVELOOKBEHIND:
		return FC_CONTINUE;
//	default:
//		throw;
	}

	return 0;
}


Token* Token::findFixedString(int options, int& outOptions) {

    switch(fTokenType) {

    case T_CHAR:
		return 0;
    case T_STRING:
		outOptions = options;
		return this;
    case T_UNION:
    case T_CLOSURE:
    case T_NONGREEDYCLOSURE:
    case T_EMPTY:
    case T_ANCHOR:
    case T_RANGE:
    case T_NRANGE:
    case T_DOT:
    case T_BACKREFERENCE:
    case T_LOOKAHEAD:
    case T_NEGATIVELOOKAHEAD:
    case T_LOOKBEHIND:
    case T_NEGATIVELOOKBEHIND:
    case T_CONDITION:
		return 0;
    case T_INDEPENDENT:
    case T_PAREN:
		return getChild(0)->findFixedString(options, outOptions);
    case T_CONCAT:
        {
            Token* prevTok = 0;
            int prevOptions = 0;

            for (int i=0; i<size(); i++) {

                Token* tok = getChild(i)->findFixedString(options, outOptions);

                if (prevTok == 0 || prevTok->isShorterThan(tok)) {

                    prevTok = tok;
					prevOptions = outOptions;
                }
            }

			outOptions = prevOptions;
            return prevTok;
        }
    case T_MODIFIERGROUP:
        {
            options |= ((ModifierToken *) this)->getOptions();
            options &= ~((ModifierToken *) this)->getOptionsMask();
            return getChild(0)->findFixedString(options, outOptions);
        }
    } // end switch

    return 0;
}


bool Token::isShorterThan(Token* const tok) {

	if (tok == 0)
		return false;

	if (getTokenType() != T_STRING && tok->getTokenType() != T_STRING)
		return false; //Should we throw an exception?

    int length = XMLString::stringLen(getString());
    int tokLength = XMLString::stringLen(tok->getString());

	return length < tokLength;
}

XERCES_CPP_NAMESPACE_END

/**
  *	End of file Token.cpp
  */
