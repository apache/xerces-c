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
#include <util/regx/RangeToken.hpp>
#include <util/regx/ModifierToken.hpp>
#include <util/regx/RegularExpression.hpp>
#include <util/regx/RegxUtil.hpp>

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
Token::Token(const unsigned short tokType) : fTokenType(tokType) {

}


Token::~Token() {

}

// ---------------------------------------------------------------------------
//  Token: Getter mthods
// ---------------------------------------------------------------------------
int Token::getMinLength() const {

	switch (fTokenType) {

	case CONCAT:
        {
            int sum = 0;
            unsigned int childSize = size();

            for (unsigned int i=0; i<childSize; i++) {
                sum += getChild(i)->getMinLength();
            }
            return sum;
        }
	case CONDITION:
	case UNION:
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
	case CLOSURE:
	case NONGREEDYCLOSURE:
        if (getMin() >= 0)
            return getMin() * getChild(0)->getMinLength();

		return 0;
	case EMPTY:
	case ANCHOR:
		return 0;
	case DOT:
	case CHAR:
	case RANGE:
	case NRANGE:
		return 1;
	case INDEPENDENT:
	case PAREN:
	case MODIFIERGROUP:
		return getChild(0)->getMinLength();
	case BACKREFERENCE:
		return 0; // *****  - REVISIT
	case STRING:
		return XMLString::stringLen(getString());
	case LOOKAHEAD:
	case NEGATIVELOOKAHEAD:
	case LOOKBEHIND:
	case NEGATIVELOOKBEHIND:
		return 0; // *****  - REVIST
//	default:
//		throw;
	}

	// We should not get here, but we have it to make some compilers happy
	return -1;
}


int Token::getMaxLength() const {

	switch (fTokenType) {

	case CONCAT:
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
    case CONDITION:
    case UNION:
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
    case CLOSURE:
    case NONGREEDYCLOSURE:
        if (getMax() >= 0) {
            return getMax() * getChild(0)->getMaxLength();
		}
		return -1;
    case EMPTY:
    case ANCHOR:
        return 0;
    case CHAR:
        return 1;
    case DOT:
    case RANGE:
    case NRANGE:
        return 2;
    case INDEPENDENT:
    case PAREN:
    case MODIFIERGROUP:
        return getChild(0)->getMaxLength();
    case BACKREFERENCE:
		return -1; // REVISIT
    case STRING:
        return XMLString::stringLen(getString());
    case LOOKAHEAD:
    case NEGATIVELOOKAHEAD:
    case LOOKBEHIND:
    case NEGATIVELOOKBEHIND:
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
	case CONCAT:
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
	case UNION:
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
	case CONDITION:
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
	case CLOSURE:
	case NONGREEDYCLOSURE:
		{
			Token* tok = getChild(0);
			if (tok)
				tok->analyzeFirstCharacter(rangeTok, options, tokFactory);
			return FC_CONTINUE;
		}
	case DOT:
	case EMPTY:
	case ANCHOR:
		return FC_CONTINUE;
	case CHAR:
		{
            XMLInt32 ch = getChar();
			rangeTok->addRange(ch, ch);
			if (ch < 0x1000 && isSet(options,RegularExpression::IGNORE_CASE)) {
				//REVISIT
			}
		}
		return FC_TERMINAL;
	case RANGE:
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
	case NRANGE:
		{
			if (isSet(options, RegularExpression::IGNORE_CASE)) {

				RangeToken* caseITok = (((RangeToken*)
					                       this)->getCaseInsensitiveToken(tokFactory));
				rangeTok->mergeRanges(RangeToken::complementRanges(caseITok, tokFactory));
			}
			else {
				rangeTok->mergeRanges(
					RangeToken::complementRanges((RangeToken*) this, tokFactory));
			}
		}
	case INDEPENDENT:
	case PAREN:
		{
			Token* tok = getChild(0);
			if (tok)
				return tok->analyzeFirstCharacter(rangeTok,options, tokFactory);
		}
	case MODIFIERGROUP:
	case BACKREFERENCE:
		rangeTok->addRange(0, UTF16_MAX);
		return FC_ANY;
	case STRING:
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
	case LOOKAHEAD:
	case NEGATIVELOOKAHEAD:
	case LOOKBEHIND:
	case NEGATIVELOOKBEHIND:
		FC_CONTINUE;
//	default:
//		throw;
	}

	return 0;
}


Token* Token::findFixedString(int options, int& outOptions) {

    switch(fTokenType) {

    case CHAR:
		return 0;
    case STRING:
		outOptions = options;
		return this;
    case UNION:
    case CLOSURE:
    case NONGREEDYCLOSURE:
    case EMPTY:
    case ANCHOR:
    case RANGE:
    case NRANGE:
    case DOT:
    case BACKREFERENCE:
    case LOOKAHEAD:
    case NEGATIVELOOKAHEAD:
    case LOOKBEHIND:
    case NEGATIVELOOKBEHIND:
    case CONDITION:
		return 0;
    case INDEPENDENT:
    case PAREN:
		return getChild(0)->findFixedString(options, outOptions);
    case CONCAT:
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
    case MODIFIERGROUP:
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

	if (getTokenType() != STRING && tok->getTokenType() != STRING)
		return false; //Should we throw an exception?

    int length = XMLString::stringLen(getString());
    int tokLength = XMLString::stringLen(tok->getString());

	return length < tokLength;
}

/**
  *	End of file Token.cpp
  */
