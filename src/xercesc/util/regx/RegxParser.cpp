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
 * Revision 1.4  2003/01/13 19:02:23  knoaman
 * [Bug 14390] C++ Indentifier collision with Python.
 *
 * Revision 1.3  2002/11/04 15:17:00  tng
 * C++ Namespace Support.
 *
 * Revision 1.2  2002/03/18 19:29:53  knoaman
 * Change constant names to eliminate possible conflict with user defined ones.
 *
 * Revision 1.1.1.1  2002/02/01 22:22:30  peiyongz
 * sane_include
 *
 * Revision 1.10  2001/11/20 20:48:10  knoaman
 * Fix for invalid repeating quantifier check.
 *
 * Revision 1.9  2001/11/16 15:56:37  knoaman
 * Add check for invalid repeating quantifier.
 *
 * Revision 1.8  2001/09/20 13:11:42  knoaman
 * Regx  + misc. fixes
 *
 * Revision 1.7  2001/08/31 16:53:41  knoaman
 * Misc. fixes.
 *
 * Revision 1.6  2001/07/26 12:46:48  knoaman
 * Fix for bug 2815.
 *
 * Revision 1.5  2001/06/06 13:49:27  jberry
 * Fix two improper NULL tests
 *
 * Revision 1.4  2001/05/11 21:51:01  knoaman
 * Schema updates and fixes.
 *
 * Revision 1.3  2001/05/11 13:26:48  tng
 * Copyright update.
 *
 * Revision 1.2  2001/05/03 18:17:45  knoaman
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
 * Revision 1.1  2001/03/02 19:22:54  knoaman
 * Schema: Regular expression handling part I
 *
 */

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/util/regx/RegxParser.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/regx/RegularExpression.hpp>
#include <xercesc/util/regx/RegxUtil.hpp>
#include <xercesc/util/regx/RegxDefs.hpp>
#include <xercesc/util/regx/TokenInc.hpp>
#include <xercesc/framework/XMLErrorCodes.hpp>
#include <xercesc/util/ParseException.hpp>

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  Static member data initialization
// ---------------------------------------------------------------------------
const unsigned short RegxParser::S_NORMAL		= 0;
const unsigned short RegxParser::S_INBRACKETS	= 1;
const unsigned short RegxParser::S_INXBRACKETS	= 2;

// ---------------------------------------------------------------------------
//  RegxParser::ReferencePostion: Constructors and Destructor
// ---------------------------------------------------------------------------
RegxParser::ReferencePosition::ReferencePosition(const int refNo,
												 const int position)
	:fReferenceNo(refNo)
	, fPosition(position)
{

}

// ---------------------------------------------------------------------------
//  RegxParser: Constructors and Destructors
// ---------------------------------------------------------------------------
RegxParser::RegxParser()
    :fHasBackReferences(false),
     fOptions(0),
     fOffset(0),
     fNoGroups(1),
     fParseContext(S_NORMAL),
     fStringLen(0),
     fState(0),
     fCharData(0),
     fString(0),
     fReferences(0),
     fTokenFactory(0)
{
}

RegxParser::~RegxParser() {

	delete [] fString;
	delete fReferences;
}

// ---------------------------------------------------------------------------
//  RegxParser: Parsing methods
// ---------------------------------------------------------------------------
Token* RegxParser::parse(const XMLCh* const regxStr, const int options) {

    // if TokenFactory is not set do nothing.
    // REVISIT - should we throw an exception
    if (fTokenFactory == 0) {
        return 0;
    }

	XMLMutexLock lockInit(&fMutex);

	fOptions = options;
	fOffset = 0;
	fNoGroups = 1;
	fHasBackReferences = false;
	setParseContext(S_NORMAL);
	delete [] fString;
	fString = XMLString::replicate(regxStr);

	if (isSet(RegularExpression::EXTENDED_COMMENT)) {

        delete [] fString;
		fString = RegxUtil::stripExtendedComment(regxStr);
    }

    fStringLen = XMLString::stringLen(fString);
    processNext();

    Token* retTok = parseRegx();

	if (fOffset != fStringLen) {
        ThrowXML(ParseException,XMLExcepts::Parser_Parse1);
    }

    if (fReferences != 0) {

		unsigned int refSize = fReferences->size();
        for (unsigned int i = 0; i < refSize; i++) {

			if (fNoGroups <= fReferences->elementAt(i)->fReferenceNo) {
                ThrowXML(ParseException,XMLExcepts::Parser_Parse2);
            }
        }

		fReferences->removeAllElements();
    }

    return retTok;
}


void RegxParser::processNext() {

    if (fOffset >= fStringLen) {

        fCharData = -1;
        fState = REGX_T_EOF;
        return;
	}

    unsigned short nextState;
	XMLCh ch = fString[fOffset++];
	fCharData = ch;

    if (fParseContext == S_INBRACKETS) {

		switch (ch) {
        case chBackSlash:
            nextState = REGX_T_BACKSOLIDUS;

			if (fOffset >= fStringLen) {
				ThrowXML(ParseException,XMLExcepts::Parser_Next1);
			}

			fCharData = fString[fOffset++];
			break;
		case chDash:
            if (isSet(RegularExpression::XMLSCHEMA_MODE)
                && fOffset < fStringLen && fString[fOffset] == chOpenSquare) {

                fOffset++;
                nextState = REGX_T_XMLSCHEMA_CC_SUBTRACTION;
            }
            else {
                nextState = REGX_T_CHAR;
            }
            break;
        case chOpenSquare:
            if (!isSet(RegularExpression::XMLSCHEMA_MODE)
                && fOffset < fStringLen && fString[fOffset] == chColon) {

                fOffset++;
                nextState = REGX_T_POSIX_CHARCLASS_START;
                break;
			} // Through down
        default:
            if (RegxUtil::isHighSurrogate(ch) && fOffset < fStringLen) {

                XMLCh lowCh = fString[fOffset];
                if (RegxUtil::isLowSurrogate(lowCh)) {
                    fCharData = RegxUtil::composeFromSurrogate(ch, lowCh);
					fOffset++;
                }
				else {
                    throw XMLErrs::Expected2ndSurrogateChar;
                }
            }

			nextState = REGX_T_CHAR;
        }

        fState = nextState;
        return;
    }

    switch (ch) {

    case chPipe:
        nextState = REGX_T_OR;
        break;
    case chAsterisk:
        nextState = REGX_T_STAR;
        break;
    case chPlus:
        nextState = REGX_T_PLUS;
        break;
    case chQuestion:
		nextState = REGX_T_QUESTION;
		break;
    case chCloseParen:
        nextState = REGX_T_RPAREN;
        break;
    case chPeriod:
		nextState = REGX_T_DOT;
		break;
    case chOpenSquare:
        nextState = REGX_T_LBRACKET;
        break;
    case chCaret:
        nextState = REGX_T_CARET;
        break;
    case chDollarSign:
		nextState = REGX_T_DOLLAR;
		break;
	case chOpenParen:
        {
		    nextState = REGX_T_LPAREN;
            if (fOffset >= fStringLen)
                break;

			if (fString[fOffset] != chQuestion)
                break;

            if (++fOffset >= fStringLen)
                ThrowXML(ParseException,XMLExcepts::Parser_Next2);

            ch = fString[fOffset++];

            switch (ch) {
            case chColon:
                nextState = REGX_T_LPAREN2;
                break;
			case chEqual:
                nextState = REGX_T_LOOKAHEAD;
                break;
            case chBang:
                nextState = REGX_T_NEGATIVELOOKAHEAD;
                break;
            case chOpenSquare:
                nextState = REGX_T_SET_OPERATIONS;
                break;
            case chCloseAngle:
                nextState = REGX_T_INDEPENDENT;
				break;
            case chOpenAngle:
				if (fOffset >= fStringLen)
					ThrowXML(ParseException,XMLExcepts::Parser_Next2);

				ch = fString[fOffset++];

				if (ch == chEqual) {
					nextState = REGX_T_LOOKBEHIND;
				}
				else if (ch == chBang) {
					nextState = REGX_T_NEGATIVELOOKBEHIND;
				}
				else {
					ThrowXML(ParseException,XMLExcepts::Parser_Next3);
				}
				break;
            case chPound:
				while (fOffset < fStringLen) {

					ch = fString[fOffset++];
					if (ch == chCloseParen)
						break;
				}

				if (ch != chCloseParen)
					ThrowXML(ParseException,XMLExcepts::Parser_Next4);

				nextState = REGX_T_COMMENT;
				break;
            default:
				if (ch == chDash || chLatin_a <= ch && ch <= chLatin_z
                    || chLatin_A <= ch && ch <= chLatin_Z) { // Options

                    fOffset--;
                    nextState = REGX_T_MODIFIERS;
                    break;
                }
                else if (ch == chOpenParen) {
                    nextState = REGX_T_CONDITION;
                    break;
                }
                ThrowXML(ParseException,XMLExcepts::Parser_Next2);
            }
        }
		break;
	case chBackSlash:
        nextState = REGX_T_BACKSOLIDUS;
        if (fOffset >= fStringLen) {
			ThrowXML(ParseException,XMLExcepts::Parser_Next1);
        }

        fCharData = fString[fOffset++];
        break;
	default:
		nextState = REGX_T_CHAR;
		if (RegxUtil::isHighSurrogate(ch) && fOffset < fStringLen) {

                XMLCh lowCh = fString[fOffset];
                if (RegxUtil::isLowSurrogate(lowCh)) {
                    fCharData = RegxUtil::composeFromSurrogate(ch, lowCh);
					fOffset++;
                }
				else {
                    throw XMLErrs::Expected2ndSurrogateChar;
                }
            }
	}

	fState = nextState;
}


Token* RegxParser::parseRegx() {

    Token* tok = parseTerm();
    Token* parentTok = 0;

    while (fState == REGX_T_OR) {

        processNext();
        if (parentTok == 0) {

            parentTok = fTokenFactory->createUnion();
		    parentTok->addChild(tok, fTokenFactory);
            tok = parentTok;
        }

        tok->addChild(parseTerm(), fTokenFactory);
    }

    return tok;
}


Token* RegxParser::parseTerm() {

    unsigned short state = fState;

    if (state == REGX_T_OR || state == REGX_T_RPAREN || state == REGX_T_EOF) {
        return fTokenFactory->createToken(Token::T_EMPTY);
    }
    else {

		Token* tok = parseFactor();
		Token* concatTok = 0;

		while ((state = fState) != REGX_T_OR && state != REGX_T_RPAREN && state != REGX_T_EOF)
        {
            if (concatTok == 0) {

                concatTok = fTokenFactory->createUnion(true);
                concatTok->addChild(tok, fTokenFactory);
                tok = concatTok;
            }
            concatTok->addChild(parseFactor(), fTokenFactory);
        }

        return tok;
    }
}


Token* RegxParser::processCaret() {

    processNext();
	return fTokenFactory->getLineBegin();
}


Token* RegxParser::processDollar() {

    processNext();
    return fTokenFactory->getLineEnd();
}


Token* RegxParser::processLook(const unsigned short tokType) {

    processNext();

	Token* tok = fTokenFactory->createLook(tokType, parseRegx());

    if (fState != REGX_T_RPAREN) {
        ThrowXML(ParseException,XMLExcepts::Parser_Factor1);
    }

    processNext();

    return tok;
}


Token* RegxParser::processBacksolidus_A() {

    processNext();
	return fTokenFactory->getStringBegin();
}


Token* RegxParser::processBacksolidus_Z() {

    processNext();
    return fTokenFactory->getStringEnd2();
}


Token* RegxParser::processBacksolidus_z() {

    processNext();
    return fTokenFactory->getStringEnd();
}


Token* RegxParser::processBacksolidus_b() {

    processNext();
    return fTokenFactory->getWordEdge();
}


Token* RegxParser::processBacksolidus_B() {

    processNext();
    return fTokenFactory->getNotWordEdge();
}


Token* RegxParser::processBacksolidus_lt() {

    processNext();
    return fTokenFactory->getWordBegin();
}


Token* RegxParser::processBacksolidus_gt() {

    processNext();
    return fTokenFactory->getWordEnd();
}


Token* RegxParser::processStar(Token* const tok) {

    processNext();

    if (fState == REGX_T_QUESTION) {

        processNext();
        return fTokenFactory->createClosure(tok, true);
    }

    return fTokenFactory->createClosure(tok);
}


Token* RegxParser::processPlus(Token* const tok) {

    processNext();

    if (fState == REGX_T_QUESTION) {

		processNext();
		return fTokenFactory->createConcat(tok,
			               fTokenFactory->createClosure(tok,true));
    }

    return fTokenFactory->createConcat(tok,
		                        fTokenFactory->createClosure(tok));
}


Token* RegxParser::processQuestion(Token* const tok) {

    processNext();

    Token* parentTok = fTokenFactory->createUnion();

    if (fState == REGX_T_QUESTION) {

        processNext();
        parentTok->addChild(fTokenFactory->createToken(Token::T_EMPTY), fTokenFactory);
        parentTok->addChild(tok, fTokenFactory);
    }
    else {

        parentTok->addChild(tok, fTokenFactory);
        parentTok->addChild(fTokenFactory->createToken(Token::T_EMPTY), fTokenFactory);
    }

    return parentTok;
}


Token* RegxParser::processParen() {

    processNext();
    int num = fNoGroups++;
    Token* tok = fTokenFactory->createParenthesis(parseRegx(),num);

    if (fState != REGX_T_RPAREN)
        ThrowXML(ParseException,XMLExcepts::Parser_Factor1);

    processNext();
    return tok;
}


Token* RegxParser::processParen2() {

    processNext();
    Token* tok = fTokenFactory->createParenthesis(parseRegx(), 0);

    if (fState != REGX_T_RPAREN)
        ThrowXML(ParseException,XMLExcepts::Parser_Factor1);

    processNext();
    return tok;
}


Token* RegxParser::processCondition() {

    if (fOffset + 1 >= fStringLen)
		ThrowXML(ParseException,XMLExcepts::Parser_Factor4);

    int refNo = -1;
	Token* conditionTok = 0;
	XMLInt32 ch = fString[fOffset];

    if (chDigit_1 <= ch && ch <= chDigit_9) {

        refNo = ch - chDigit_0;
        fHasBackReferences =  true;

        if (fReferences == 0) {
            this->fReferences = new RefVectorOf<ReferencePosition>(8, true);
        }

        fReferences->addElement(new ReferencePosition(refNo, fOffset));
        fOffset++;

        if (fString[fOffset] != chCloseParen)
            ThrowXML(ParseException,XMLExcepts::Parser_Factor1);

        fOffset++;
    }
    else {

        if (ch == chQuestion) {
            fOffset--;
        }

        processNext();
        conditionTok = parseFactor();
        switch(conditionTok->getTokenType()) {
        case Token::T_LOOKAHEAD:
        case Token::T_NEGATIVELOOKAHEAD:
        case Token::T_LOOKBEHIND:
        case Token::T_NEGATIVELOOKBEHIND:
            break;
        case Token::T_ANCHOR:
            if (fState != REGX_T_RPAREN)
				ThrowXML(ParseException,XMLExcepts::Parser_Factor1);
			break;
        default:
			ThrowXML(ParseException,XMLExcepts::Parser_Factor5);
        }
    }

    processNext();
    Token* yesPattern = parseRegx();
    Token* noPattern = 0;

    if (yesPattern->getTokenType() == Token::T_UNION) {

        if (yesPattern->size() != 2)
            ThrowXML(ParseException,XMLExcepts::Parser_Factor6);

        noPattern = yesPattern->getChild(1);
        yesPattern = yesPattern->getChild(0);
    }

    if (fState != REGX_T_RPAREN)
        ThrowXML(ParseException,XMLExcepts::Parser_Factor1);

	processNext();
	return fTokenFactory->createCondition(refNo,conditionTok,
		                                             yesPattern,noPattern);
}


Token* RegxParser::processModifiers() {

    // fOffset points to the next '?'.
	// modifiers ::= [imsw]* ('-' [imsw]*)? ':'
    int add = 0;
    int mask = 0;
    XMLInt32 ch = -1;

    while (fOffset < fStringLen) {

        int v = RegularExpression::getOptionValue(fString[fOffset]);

        ch = fString[fOffset];
        if (v == 0)
            break;

        add |= v;
        fOffset++;
    } // end while

    if (fOffset >= fStringLen)
        ThrowXML(ParseException,XMLExcepts::Parser_Factor2);

    if (ch == chDash) {

        fOffset++;
        while(fOffset < fStringLen) {
            int v = RegularExpression::getOptionValue(fString[fOffset]);

            ch = fString[fOffset];
            if (v == 0)
                break;

            mask |= v;
            fOffset++;
        }

        if (fOffset >= fStringLen)
            ThrowXML(ParseException,XMLExcepts::Parser_Factor2);
    }

    Token* tok = 0;

    if (ch == chColon) {

        fOffset++;
		processNext();
        tok = fTokenFactory->createModifierGroup(parseRegx(),add,mask);

        if (fState != REGX_T_RPAREN)
            ThrowXML(ParseException,XMLExcepts::Parser_Factor1);

        processNext();
    }
    else if (ch == chCloseParen) {

        fOffset++;
        processNext();
        tok = fTokenFactory->createModifierGroup(parseRegx(),add,mask);
    }
    else {
        ThrowXML(ParseException,XMLExcepts::Parser_Factor3);
	}

	return tok;
}


Token* RegxParser::processIndependent() {

    processNext();

	Token* tok = fTokenFactory->createLook(Token::T_INDEPENDENT, parseRegx());

	if (fState != REGX_T_RPAREN)
		ThrowXML(ParseException,XMLExcepts::Parser_Factor1);

    processNext();
    return tok;
}


Token* RegxParser::processBacksolidus_c() {

    XMLCh ch; //Must be in 0x0040-0x005F

    if (fOffset >= fStringLen
        || ((ch = fString[fOffset++]) & 0xFFE0) != 0x0040)
        ThrowXML(ParseException,XMLExcepts::Parser_Atom1);

    processNext();
	return fTokenFactory->createChar(ch - 0x40);
}


Token* RegxParser::processBacksolidus_C() {

	// REVISIT - Do we throw an exception - we do not want to throw too
	// many exceptions
    return 0;
}

Token* RegxParser::processBacksolidus_i() {

    processNext();
	return fTokenFactory->createChar(chLatin_i);
}


Token* RegxParser::processBacksolidus_I() {

	//Ditto
    return 0;
}


Token* RegxParser::processBacksolidus_g() {

    processNext();
    return fTokenFactory->getGraphemePattern();
}


Token* RegxParser::processBacksolidus_X() {

    processNext();
    return fTokenFactory->getCombiningCharacterSequence();
}

Token* RegxParser::processBackReference() {

    int refNo = fCharData - chDigit_0;
    Token* tok = fTokenFactory->createBackReference(refNo);

    fHasBackReferences = true;
    if (fReferences == 0) {
        fReferences = new RefVectorOf<ReferencePosition>(8, true);
    }

    fReferences->addElement(new ReferencePosition(refNo, fOffset - 2));
    processNext();
    return tok;
}


Token* RegxParser::parseFactor() {

    switch (fState) {

    case REGX_T_CARET:
        return processCaret();
    case REGX_T_DOLLAR:
        return processDollar();
    case REGX_T_LOOKAHEAD:
        return processLook(Token::T_LOOKAHEAD);
    case REGX_T_NEGATIVELOOKAHEAD:
        return processLook(Token::T_NEGATIVELOOKAHEAD);
    case REGX_T_LOOKBEHIND:
        return processLook(Token::T_LOOKBEHIND);
    case REGX_T_NEGATIVELOOKBEHIND:
        return processLook(Token::T_NEGATIVELOOKBEHIND);
    case REGX_T_COMMENT:
        processNext();
        return fTokenFactory->createToken(Token::T_EMPTY);
    case REGX_T_BACKSOLIDUS:
        switch(fCharData) {
        case chLatin_A:
            return processBacksolidus_A();
        case chLatin_Z:
            return processBacksolidus_Z();
        case chLatin_z:
            return processBacksolidus_z();
        case chLatin_b:
            return processBacksolidus_B();
        case chLatin_B:
            return processBacksolidus_B();
        case chOpenAngle:
            return processBacksolidus_lt();
        case chCloseAngle:
            return processBacksolidus_gt();
		}
    }

	Token* tok = parseAtom();

	switch(fState) {

    case REGX_T_STAR:
        return processStar(tok);
    case REGX_T_PLUS:
        return processPlus(tok);
    case REGX_T_QUESTION:
        return processQuestion(tok);
    case REGX_T_CHAR:
		if (fCharData == chOpenCurly) {

            int offset = fOffset;
            int min = 0;
            int max = -1;
            bool minExist = false;

            if (offset >= fStringLen)
                break;

            XMLInt32 ch = fString[offset++];

            if (ch != chComma && (ch < chDigit_0 || ch > chDigit_9))
                ThrowXML1(ParseException, XMLExcepts::Regex_InvalidQuantifier, fString);

            if (ch != chComma) {
                minExist = true;
                min = ch - chDigit_0;
                while (offset < fStringLen
                       && (ch = fString[offset++]) >= chDigit_0
                       && ch <= chDigit_9) {

                    min = min*10 + ch - chDigit_0;
                    ch = -1;
                }
            }

            max = min;

            if (ch != chCloseCurly && ch != chComma)  {
                ThrowXML1(ParseException, XMLExcepts::Regex_InvalidQuantifier, fString);
            }

            if (ch == chComma) {

                if (offset >= fStringLen)
                    break;

                if (((ch = fString[offset++]) < chDigit_0 || ch > chDigit_9)
                    && ch != chCloseCurly)
                    ThrowXML1(ParseException, XMLExcepts::Regex_InvalidQuantifier, fString);

                if (ch == chCloseCurly) {
                    if (minExist)
                        max = -1;
                    else
                        ThrowXML1(ParseException, XMLExcepts::Regex_InvalidQuantifier, fString);
                }
                else {
                    max = ch - chDigit_0;
                    while (offset < fStringLen
                           && (ch = fString[offset++]) >= chDigit_0
                           && ch <= chDigit_9) {

                        max = max*10 + ch - chDigit_0;
                        ch = -1;
                    }

                    if (ch != chCloseCurly)  {
                        ThrowXML1(ParseException, XMLExcepts::Regex_InvalidQuantifier, fString);
                    }
                }
            } // end if ch = chComma

            if (checkQuestion(offset)) {

                tok = fTokenFactory->createClosure(tok, true);
                fOffset = offset + 1;
            }
            else {

                tok = fTokenFactory->createClosure(tok);
                fOffset = offset;
            }

            tok->setMin(min);
            tok->setMax(max);
            processNext();
		}
	}

	return tok;
}


Token* RegxParser::parseAtom() {

    Token* tok = 0;

    switch(fState) {

    case REGX_T_LPAREN:
        return processParen();
    case REGX_T_LPAREN2:
        return processParen2();
    case REGX_T_CONDITION:
        return processCondition();
    case REGX_T_MODIFIERS:
        return processModifiers();
    case REGX_T_INDEPENDENT:
        return processIndependent();
    case REGX_T_DOT:
        processNext();
        tok = fTokenFactory->getDot();
        break;
    case REGX_T_LBRACKET:
        return parseCharacterClass(true);
    case REGX_T_SET_OPERATIONS:
        return parseSetOperations();
    case REGX_T_BACKSOLIDUS:
		switch(fCharData) {

        case chLatin_d:
        case chLatin_D:
        case chLatin_w:
        case chLatin_W:
        case chLatin_s:
        case chLatin_S:
            tok = getTokenForShorthand(fCharData);
            processNext();
            return tok;
        case chLatin_e:
        case chLatin_f:
        case chLatin_n:
        case chLatin_r:
        case chLatin_t:
        case chLatin_u:
        case chLatin_v:
        case chLatin_x:
			{
                XMLInt32 ch = decodeEscaped();
                if (ch < 0x10000) {
                    tok = fTokenFactory->createChar(ch);
                }
                else {

                    XMLCh* surrogateStr = RegxUtil::decomposeToSurrogates(ch);
				    ArrayJanitor<XMLCh> janSurrogate(surrogateStr);
				    tok = fTokenFactory->createString(surrogateStr);
                }
            }
			break;
        case chLatin_c:
            return processBacksolidus_c();
        case chLatin_C:
            return processBacksolidus_C();
        case chLatin_i:
            return processBacksolidus_i();
        case chLatin_I:
            return processBacksolidus_I();
        case chLatin_g:
            return processBacksolidus_g();
        case chLatin_X:
            return processBacksolidus_X();
        case chDigit_0:
        case chDigit_1:
        case chDigit_2:
        case chDigit_3:
        case chDigit_4:
        case chDigit_5:
        case chDigit_6:
        case chDigit_7:
        case chDigit_8:
        case chDigit_9:
            return processBackReference();
        case chLatin_p:
        case chLatin_P:
			{
				int start = fOffset;
				tok = processBacksolidus_pP(fCharData);
				if (tok == 0) {
					ThrowXML(ParseException,XMLExcepts::Parser_Atom5);
				}
			}
            break;
        default:
            tok = fTokenFactory->createChar(fCharData);
		} // end switch

        processNext();
        break;
    case REGX_T_CHAR:
        tok = fTokenFactory->createChar(fCharData);
        processNext();
        break;
    default:
        ThrowXML(ParseException,XMLExcepts::Parser_Atom4);
    } //end switch

    return tok;
}


RangeToken* RegxParser::processBacksolidus_pP(const XMLInt32 ch) {

    bool positive = (ch == chLatin_p);

    processNext();
	if (fState != REGX_T_CHAR)
		ThrowXML(ParseException,XMLExcepts::Parser_Atom2);

    RangeToken* tok = 0;

	switch(fCharData) {

    case chLatin_L:
		tok = fTokenFactory->getRange(fgUniLetter, !positive);
		break;
    case chLatin_M:
		tok = fTokenFactory->getRange(fgUniMark, !positive);
		break;
    case chLatin_N:
		tok = fTokenFactory->getRange(fgUniNumber, !positive);
		break;
    case chLatin_Z:
		tok = fTokenFactory->getRange(fgUniSeparator, !positive);
		break;
    case chLatin_C:
		tok = fTokenFactory->getRange(fgUniControl, !positive);
		break;
    case chLatin_P:
		tok = fTokenFactory->getRange(fgUniPunctuation, !positive);
		break;
    case chLatin_S:
		tok = fTokenFactory->getRange(fgUniSymbol, !positive);
		break;
    case chOpenCurly:
		{
			int nameStart = fOffset;
			int nameEnd = XMLString::indexOf(fString,chCloseCurly,nameStart);

			if (nameEnd < 0)
				ThrowXML(ParseException,XMLExcepts::Parser_Atom3);

			fOffset = nameEnd + 1;
			XMLCh* rangeName = new XMLCh[(nameEnd - nameStart) + 1];
			ArrayJanitor<XMLCh> janRangeName(rangeName);
			XMLString::subString(rangeName, fString, nameStart, nameEnd);
			tok = fTokenFactory->getRange(rangeName, !positive);
		}
		break;
	default:
		ThrowXML(ParseException,XMLExcepts::Parser_Atom2);
	}

    return tok;
}


XMLInt32 RegxParser::processCInCharacterClass(RangeToken* const tok,
                                              const XMLInt32 ch) {

	return decodeEscaped();
}


RangeToken* RegxParser::parseCharacterClass(const bool useNRange) {

    setParseContext(S_INBRACKETS);
	processNext();

    RangeToken* base = 0;
    RangeToken* tok = 0;
    bool nRange = false;

	if (fState == REGX_T_CHAR && fCharData == chCaret) {

        nRange = true;
        processNext();

		if (useNRange) {
            tok = fTokenFactory->createRange(true);
        }
        else {

			base = fTokenFactory->createRange();
            base->addRange(0, Token::UTF16_MAX);
            tok = fTokenFactory->createRange();
        }
    }
    else {
        tok = fTokenFactory->createRange();
    }

    bool firstLoop = true;

    while (fState != REGX_T_EOF) {

        if (fState == REGX_T_CHAR && fCharData == chCloseSquare && !firstLoop)
			break;

        bool end = false;
        XMLInt32 ch = fCharData;

        firstLoop = false;
        if (fState == REGX_T_BACKSOLIDUS) {

            switch(ch) {
            case chLatin_d:
            case chLatin_D:
            case chLatin_w:
            case chLatin_W:
            case chLatin_s:
            case chLatin_S:
                tok->mergeRanges(getTokenForShorthand(ch));
                end = true;
				break;
            case chLatin_i:
            case chLatin_I:
            case chLatin_c:
            case chLatin_C:
                ch = processCInCharacterClass(tok, ch);
				if (ch < 0){
				    end = true;
                }
                break;
            case chLatin_p:
            case chLatin_P:
				{
					int pStart = fOffset;
					RangeToken* tok2 = processBacksolidus_pP(ch);

					if (tok2 == 0) {
						ThrowXML(ParseException,XMLExcepts::Parser_Atom5);
					}

					tok->mergeRanges(tok2);
					end = true;
				}
                break;
            default:
                ch = decodeEscaped();
			}
        } // end if REGX_T_BACKSOLIDUS
        else if (fState == REGX_T_POSIX_CHARCLASS_START) {

            int nameEnd = XMLString::indexOf(fString, chColon, fOffset);

            if (nameEnd < 0) {
				ThrowXML(ParseException,XMLExcepts::Parser_CC1);
			}

            bool positive = true;

            if (fString[fOffset] == chCaret) {

                fOffset++;
                positive = false;
            }

			XMLCh* name = new XMLCh[(nameEnd - fOffset) + 1];
			ArrayJanitor<XMLCh> janName(name);

			XMLString::subString(name, fString, fOffset, nameEnd);
            RangeToken* rangeTok = fTokenFactory->getRange(name, !positive);

            if (rangeTok == 0) {
				ThrowXML(ParseException,XMLExcepts::Parser_CC3);
            }

			tok->mergeRanges(rangeTok);
			end = true;

			if (nameEnd+1 >= fStringLen || fString[nameEnd+1] != chCloseSquare) {
				ThrowXML(ParseException,XMLExcepts::Parser_CC1);
			}

			fOffset = nameEnd + 2;
        }

        processNext();
		if (!end) {

            if (fState != REGX_T_CHAR || fCharData != chDash) {
                tok->addRange(ch, ch);
            }
            else {

                processNext();

                if (fState == REGX_T_EOF)
                    ThrowXML(ParseException,XMLExcepts::Parser_CC2);

                if (fState == REGX_T_CHAR && fCharData == chCloseSquare) {

                    tok->addRange(ch, ch);
                    tok->addRange(chDash, chDash);
                }
                else {

                    XMLInt32 rangeEnd = fCharData;

                    if (fState == REGX_T_BACKSOLIDUS) {
                        rangeEnd = decodeEscaped();
                    }

                    processNext();
                    tok->addRange(ch, rangeEnd);
                }
            }
        }

        if (isSet(RegularExpression::SPECIAL_COMMA)
            && fState == REGX_T_CHAR && fCharData == chComma) {
            processNext();
        }
    } // end while fState

	if (fState == REGX_T_EOF) {
        ThrowXML(ParseException,XMLExcepts::Parser_CC2);
	}

    if (!useNRange && nRange) {

        base->subtractRanges(tok);
        tok = base;
    }

    tok->sortRanges();
    tok->compactRanges();
    setParseContext(S_NORMAL);
    processNext();

    return tok;
}


RangeToken* RegxParser::parseSetOperations() {

    RangeToken* tok = parseCharacterClass(false);

    while (fState != REGX_T_RPAREN) {

		if (fState == REGX_T_CHAR
            && (fCharData == chDash || fCharData == chAmpersand)
            || fState == REGX_T_PLUS) {

            processNext();
            if (fState != REGX_T_LBRACKET)
                ThrowXML(ParseException,XMLExcepts::Parser_Ope1);

            RangeToken* tok2 = parseCharacterClass(false);

            if (fState == REGX_T_PLUS) {
                tok->mergeRanges(tok2);
            }
            else if (fCharData == chDash) {
                tok->subtractRanges(tok2);
            }
            else if (fCharData == chAmpersand) {
                tok->intersectRanges(tok2);
            }
            else {
                throw 0; // ThrowXML(RuntimeException, "ASSERT")
            }
        }
        else {
			ThrowXML(ParseException,XMLExcepts::Parser_Ope2);
		}
    }

    processNext();
    return tok;
}

Token* RegxParser::getTokenForShorthand(const XMLInt32 ch) {

    Token* tok = 0;
    bool useUnicode = isSet(RegularExpression::USE_UNICODE_CATEGORY);

	switch (ch) {
	case chLatin_d:
		tok = useUnicode ? fTokenFactory->getRange(fgUniDecimalDigit)
						 : fTokenFactory->getRange(fgASCIIDigit);
		break;
	case chLatin_D:
		tok = useUnicode ? fTokenFactory->getRange(fgUniDecimalDigit, true)
						 : fTokenFactory->getRange(fgASCIIDigit, true);
		break;
	case chLatin_w:
		tok = useUnicode ? fTokenFactory->getRange(fgUniIsWord)
						 : fTokenFactory->getRange(fgASCIIWord);
		break;
	case chLatin_W:
		tok = useUnicode ? fTokenFactory->getRange(fgUniIsWord, true)
						 : fTokenFactory->getRange(fgASCIIWord, true);
		break;
	case chLatin_s:
		tok = useUnicode ? fTokenFactory->getRange(fgUniIsSpace)
						 : fTokenFactory->getRange(fgASCIISpace);
		break;
	case chLatin_S:
		tok = useUnicode ? fTokenFactory->getRange(fgUniIsSpace, true)
						 : fTokenFactory->getRange(fgASCIISpace, true);
//	default:
//		ThrowXML(RuntimeException, "Invalid shorthand {0}", chAsString)
	}

    return tok;
}


XMLInt32 RegxParser::decodeEscaped() {

    if (fState != REGX_T_BACKSOLIDUS)
		ThrowXML(ParseException,XMLExcepts::Parser_Next1);

    XMLInt32 ch = fCharData;

	switch (ch) {
	case chLatin_e:
		ch = 0x1B; // Escape
		break;
	case chLatin_f:
		ch = chFF;
		break;
	case chLatin_n:
		ch = chLF;
		break;
	case chLatin_r:
		ch = chCR;
		break;
	case chLatin_t:
		ch = chHTab;
		break;
	case chLatin_x:
		{
			processNext();
			if (fState != REGX_T_CHAR) {
				ThrowXML(ParseException,XMLExcepts::Parser_Descape1);
			}
			if (fCharData == chOpenCurly) {

				int v1 = 0;
				XMLInt32 uv = 0;

				do {
					processNext();
					if (fState != REGX_T_CHAR)
						ThrowXML(ParseException,XMLExcepts::Parser_Descape1);

					if ((v1 = hexChar(fCharData)) < 0)
						break;

					uv = uv*16 + v1;
				} while (true);

				if (fCharData != chCloseCurly)
					ThrowXML(ParseException,XMLExcepts::Parser_Descape3);

				if (uv > Token::UTF16_MAX)
					ThrowXML(ParseException,XMLExcepts::Parser_Descape4);

				ch = uv;
			}
			else {
				int v1 = 0;
				if (fState != REGX_T_CHAR || (v1 = hexChar(fCharData)) < 0)
					ThrowXML(ParseException,XMLExcepts::Parser_Descape1);

				int uv = v1;

				processNext();
				if (fState != REGX_T_CHAR || (v1 = hexChar(fCharData)) < 0)
					ThrowXML(ParseException,XMLExcepts::Parser_Descape1);

				ch = uv*16 + v1;
			}
		}
		break;
	case chLatin_u:
		{
			int v1 = 0;
			int uv = 0;

			for (int i=0; i< 4; i++) {

				processNext();
				if (fState != REGX_T_CHAR || (v1 = hexChar(fCharData)) < 0)
					ThrowXML(ParseException,XMLExcepts::Parser_Descape1);

				uv = (i == 0) ? v1 : uv*16 + v1;
			}

			ch = uv;
		}
		break;
	case chLatin_v:
		{
			int v1 = 0;
			int uv = 0;

			for (int i=0; i< 6; i++) {

				processNext();
				if (fState != REGX_T_CHAR || (v1 = hexChar(fCharData)) < 0)
					ThrowXML(ParseException,XMLExcepts::Parser_Descape1);

				uv = (i == 0) ? v1 : uv*16 + v1;
			}

			if (uv > Token::UTF16_MAX)
				ThrowXML(ParseException,XMLExcepts::Parser_Descape1);

			ch = uv;
		}
		break;
	case chLatin_A:
	case chLatin_Z:
	case chLatin_z:
		ThrowXML(ParseException,XMLExcepts::Parser_Descape5);
	} // end switch

    return ch;
}

// ---------------------------------------------------------------------------
//  RegxParser: Helper Methods
// ---------------------------------------------------------------------------
bool RegxParser::checkQuestion(const int off) {

    return ((off < fStringLen) && fString[off] == chQuestion);
}

XERCES_CPP_NAMESPACE_END

/**
  *	End file RegxParser.cpp
  */
