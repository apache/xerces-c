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
 * Revision 1.1  2002/02/01 22:22:34  peiyongz
 * Initial revision
 *
 * Revision 1.4  2001/10/15 20:18:32  knoaman
 * Fix for bug 4177.
 *
 * Revision 1.3  2001/05/11 13:26:52  tng
 * Copyright update.
 *
 * Revision 1.2  2001/05/03 18:18:02  knoaman
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
 * Revision 1.1  2001/03/02 19:26:50  knoaman
 * Schema: Regular expression handling part II
 *
 */

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/util/regx/XMLRangeFactory.hpp>
#include <xercesc/internal/CharTypeTables.hpp>
#include <xercesc/util/regx/RegxDefs.hpp>
#include <xercesc/util/regx/TokenFactory.hpp>
#include <xercesc/util/regx/RangeToken.hpp>
#include <xercesc/util/regx/RangeTokenMap.hpp>


// ---------------------------------------------------------------------------
//  Static member data initialization
// ---------------------------------------------------------------------------
bool XMLRangeFactory::fRangesCreated = false;
bool XMLRangeFactory::fKeywordsInitialized = false;

// ---------------------------------------------------------------------------
//  Local static functions
// ---------------------------------------------------------------------------
static void setupRange(RangeToken* const rangeTok,
                       const XMLCh* const theTable) {

    const XMLCh* pchCur = theTable;

    // Do the ranges first
    while (*pchCur)
    {
        const XMLCh chLow = *pchCur++;
        const XMLCh chHigh = *pchCur++;

		rangeTok->addRange(chLow, chHigh);
    }

    // Skip the range terminator
    pchCur++;

    // And then the singles until we hit its terminator
    while (*pchCur) {
        const XMLCh chSingle = *pchCur++;
        rangeTok->addRange(chSingle, chSingle);
    }
}

// ---------------------------------------------------------------------------
//  XMLRangeFactory: Constructors and Destructor
// ---------------------------------------------------------------------------
XMLRangeFactory::XMLRangeFactory() {

}

XMLRangeFactory::~XMLRangeFactory() {

}

// ---------------------------------------------------------------------------
//  XMLRangeFactory: Range creation methods
// ---------------------------------------------------------------------------
void XMLRangeFactory::buildRanges() {

    if (fRangesCreated)
        return;

    if (!fKeywordsInitialized) {
        initializeKeywordMap();
    }

    RangeTokenMap* rangeTokMap = RangeTokenMap::instance();
    TokenFactory* tokFactory = rangeTokMap->getTokenFactory();

    // Create space ranges
    RangeToken* tok = tokFactory->createRange();
    setupRange(tok, gWhitespaceChars);
    rangeTokMap->setRangeToken(fgXMLSpace, tok);

    // Create digits ranges
    tok = tokFactory->createRange();
    setupRange(tok, gDigitChars);
    tok->sortRanges();
    tok->compactRanges();
    rangeTokMap->setRangeToken(fgXMLDigit, tok);

    // Create word ranges
    tok = tokFactory->createRange();
    setupRange(tok, gBaseChars);
    setupRange(tok, gXMLChars);
    setupRange(tok, gDigitChars);
    tok->sortRanges();
    tok->compactRanges();
    rangeTokMap->setRangeToken(fgXMLWord, tok);

    // Create NameChar ranges
    tok = tokFactory->createRange();
    setupRange(tok, gBaseChars);
    setupRange(tok, gCombiningChars);
    setupRange(tok, gExtenderChars);
    setupRange(tok, gIdeographicChars);
    setupRange(tok, gDigitChars);
    tok->addRange(chDash, chDash);
    tok->addRange(chColon, chColon);
    tok->addRange(chPeriod, chPeriod);
    tok->addRange(chUnderscore, chUnderscore);
    tok->sortRanges();
    tok->compactRanges();
    rangeTokMap->setRangeToken(fgXMLNameChar, tok);

    // Create initialNameChar ranges
    tok = tokFactory->createRange();
    setupRange(tok, gBaseChars);
    setupRange(tok, gXMLChars);
    tok->addRange(chColon, chColon);
    tok->addRange(chUnderscore, chUnderscore);
    tok->sortRanges();
    tok->compactRanges();
    rangeTokMap->setRangeToken(fgXMLInitialNameChar, tok);

    fRangesCreated = true;
}

// ---------------------------------------------------------------------------
//  XMLRangeFactory: Range creation methods
// ---------------------------------------------------------------------------
void XMLRangeFactory::initializeKeywordMap() {

    if (fKeywordsInitialized)
        return;

    RangeTokenMap* rangeTokMap = RangeTokenMap::instance();

    rangeTokMap->addKeywordMap(fgXMLSpace, fgXMLCategory);
    rangeTokMap->addKeywordMap(fgXMLDigit, fgXMLCategory);
    rangeTokMap->addKeywordMap(fgXMLWord, fgXMLCategory);
    rangeTokMap->addKeywordMap(fgXMLNameChar, fgXMLCategory);
    rangeTokMap->addKeywordMap(fgXMLInitialNameChar, fgXMLCategory);

    fKeywordsInitialized = true;
}

/**
  * End of file XMLRangeFactory.cpp
  */
