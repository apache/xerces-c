/*
 * Copyright 2001-2002,2004 The Apache Software Foundation.
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
 * Revision 1.9  2005/05/05 01:12:24  dbertoni
 * Fix for Jira issue XERCESC-1391.
 *
 * Revision 1.8  2004/11/12 23:24:58  knoaman
 * Fix multi threading problem.
 *
 * Revision 1.7  2004/10/20 15:18:49  knoaman
 * Allow option of initializing static data in XMLPlatformUtils::Initialize
 *
 * Revision 1.6  2004/09/08 13:56:47  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.5  2003/05/18 14:02:06  knoaman
 * Memory manager implementation: pass per instance manager.
 *
 * Revision 1.4  2002/11/04 15:17:01  tng
 * C++ Namespace Support.
 *
 * Revision 1.3  2002/05/24 16:42:20  knoaman
 * Performance fixes: eliminate mulitple calls to addRange and sort.
 *
 * Revision 1.2  2002/02/05 13:20:06  tng
 * [Bug 5716] Can't parse with Validation more than one file.
 *
 * Revision 1.1.1.1  2002/02/01 22:22:34  peiyongz
 * sane_include
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
#include <xercesc/util/Janitor.hpp>
#include <string.h>

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  Local static functions
// ---------------------------------------------------------------------------
static void setupRange(XMLInt32* const rangeMap,
                       const XMLCh* const theTable,
                       unsigned int startingIndex) {

    const XMLCh* pchCur = theTable;

    // Do the ranges first
    while (*pchCur)
    {
        rangeMap[startingIndex++] = *pchCur++;
    }

    // Skip the range terminator
    pchCur++;

    // And then the singles until we hit its terminator
    while (*pchCur) {

        const XMLCh chSingle = *pchCur++;
        rangeMap[startingIndex++] = chSingle;
        rangeMap[startingIndex++] = chSingle;
    }
}

static unsigned int getTableLen(const XMLCh* const theTable) {

    unsigned int rangeLen = XMLString::stringLen(theTable);

    return rangeLen + 2*XMLString::stringLen(theTable + rangeLen + 1);
}

// ---------------------------------------------------------------------------
//  XMLRangeFactory: Constructors and Destructor
// ---------------------------------------------------------------------------
XMLRangeFactory::XMLRangeFactory()
{

}

XMLRangeFactory::~XMLRangeFactory() {

}

// ---------------------------------------------------------------------------
//  XMLRangeFactory: Range creation methods
// ---------------------------------------------------------------------------
void XMLRangeFactory::buildRanges(RangeTokenMap *rangeTokMap) {

    if (fRangesCreated)
        return;

    if (!fKeywordsInitialized) {
        initializeKeywordMap(rangeTokMap);
    }

    TokenFactory* tokFactory = rangeTokMap->getTokenFactory();

    // Create space ranges
    unsigned int wsTblLen = getTableLen(gWhitespaceChars);
    RangeToken* tok = tokFactory->createRange();
    XMLInt32* wsRange = (XMLInt32*) XMLPlatformUtils::fgMemoryManager->allocate
    (
        wsTblLen * sizeof(XMLInt32)
    );//new XMLInt32[wsTblLen];

    tok->setRangeValues(wsRange, wsTblLen);
    setupRange(wsRange, gWhitespaceChars, 0);
    // Build the internal map.
    tok->createMap();
    rangeTokMap->setRangeToken(fgXMLSpace, tok);

    tok = (RangeToken*) RangeToken::complementRanges(tok, tokFactory);
    // Build the internal map.
    tok->createMap();
    rangeTokMap->setRangeToken(fgXMLSpace, tok , true);

    // Create digits ranges
    tok = tokFactory->createRange();
    unsigned int digitTblLen = getTableLen(gDigitChars);
    XMLInt32* digitRange = (XMLInt32*) XMLPlatformUtils::fgMemoryManager->allocate
    (
        digitTblLen * sizeof(XMLInt32)
    );//new XMLInt32[digitTblLen];

    tok->setRangeValues(digitRange, digitTblLen);
    setupRange(digitRange, gDigitChars, 0);
    // Build the internal map.
    tok->createMap();
    rangeTokMap->setRangeToken(fgXMLDigit, tok);

    tok = (RangeToken*) RangeToken::complementRanges(tok, tokFactory);
    // Build the internal map.
    tok->createMap();
    rangeTokMap->setRangeToken(fgXMLDigit, tok , true);

    // Build word ranges
    unsigned int baseTblLen = getTableLen(gBaseChars);
    unsigned int ideoTblLen = getTableLen(gIdeographicChars);
    unsigned int wordRangeLen = baseTblLen + ideoTblLen + digitTblLen;
    XMLInt32* wordRange = (XMLInt32*) XMLPlatformUtils::fgMemoryManager->allocate
    (
        wordRangeLen * sizeof(XMLInt32)
    );//new XMLInt32[wordRangeLen];
    ArrayJanitor<XMLInt32> janWordRange(wordRange, XMLPlatformUtils::fgMemoryManager);

    setupRange(wordRange, gBaseChars, 0);
    setupRange(wordRange, gIdeographicChars, baseTblLen);
    memcpy(wordRange + baseTblLen + ideoTblLen, digitRange, digitTblLen * sizeof(XMLInt32));

    // Create NameChar ranges
    tok = tokFactory->createRange();
    unsigned int combTblLen = getTableLen(gCombiningChars);
    unsigned int extTblLen = getTableLen(gExtenderChars);
    unsigned int nameTblLen = wordRangeLen + combTblLen + extTblLen;
    XMLInt32* nameRange = (XMLInt32*) XMLPlatformUtils::fgMemoryManager->allocate
    (
        (nameTblLen + 8) * sizeof(XMLInt32)
    );//new XMLInt32[nameTblLen + 8];

    tok->setRangeValues(nameRange, nameTblLen + 8);
    memcpy(nameRange, wordRange, wordRangeLen * sizeof(XMLInt32));
    setupRange(nameRange, gCombiningChars, wordRangeLen);
    setupRange(nameRange, gExtenderChars, wordRangeLen + combTblLen);
    nameRange[nameTblLen++] = chDash;
    nameRange[nameTblLen++] = chDash;
    nameRange[nameTblLen++] = chColon;
    nameRange[nameTblLen++] = chColon;
    nameRange[nameTblLen++] = chPeriod;
    nameRange[nameTblLen++] = chPeriod;
    nameRange[nameTblLen++] = chUnderscore;
    nameRange[nameTblLen++] = chUnderscore;
    tok->sortRanges();
    tok->compactRanges();
    // Build the internal map.
    tok->createMap();
    rangeTokMap->setRangeToken(fgXMLNameChar, tok);

    tok = (RangeToken*) RangeToken::complementRanges(tok, tokFactory);
    // Build the internal map.
    tok->createMap();
    rangeTokMap->setRangeToken(fgXMLNameChar, tok , true);

    // Create initialNameChar ranges
    tok = tokFactory->createRange();
    unsigned int initialNameTblLen = baseTblLen + ideoTblLen;
    XMLInt32* initialNameRange = (XMLInt32*) XMLPlatformUtils::fgMemoryManager->allocate
    (
        (initialNameTblLen + 4) * sizeof(XMLInt32)
    );//new XMLInt32[initialNameTblLen + 4];

    tok->setRangeValues(initialNameRange, initialNameTblLen + 4);
    memcpy(initialNameRange, wordRange, initialNameTblLen * sizeof(XMLInt32));
    initialNameRange[initialNameTblLen++] = chColon;
    initialNameRange[initialNameTblLen++] = chColon;
    initialNameRange[initialNameTblLen++] = chUnderscore;
    initialNameRange[initialNameTblLen++] = chUnderscore;
    tok->sortRanges();
    tok->compactRanges();
    // Build the internal map.
    tok->createMap();
    rangeTokMap->setRangeToken(fgXMLInitialNameChar, tok);

    tok = (RangeToken*) RangeToken::complementRanges(tok, tokFactory);
    // Build the internal map.
    tok->createMap();
    rangeTokMap->setRangeToken(fgXMLInitialNameChar, tok , true);

    // Create word range
    tok = tokFactory->createRange();
    tok->setRangeValues(wordRange, wordRangeLen);
    janWordRange.orphan();
    tok->sortRanges();
    tok->compactRanges();
    // Build the internal map.
    tok->createMap();
    rangeTokMap->setRangeToken(fgXMLWord, tok);

    tok = (RangeToken*) RangeToken::complementRanges(tok, tokFactory);
    // Build the internal map.
    tok->createMap();
    rangeTokMap->setRangeToken(fgXMLWord, tok , true);

    fRangesCreated = true;
}

// ---------------------------------------------------------------------------
//  XMLRangeFactory: Range creation methods
// ---------------------------------------------------------------------------
void XMLRangeFactory::initializeKeywordMap(RangeTokenMap *rangeTokMap) {

    if (fKeywordsInitialized)
        return;

    rangeTokMap->addKeywordMap(fgXMLSpace, fgXMLCategory);
    rangeTokMap->addKeywordMap(fgXMLDigit, fgXMLCategory);
    rangeTokMap->addKeywordMap(fgXMLWord, fgXMLCategory);
    rangeTokMap->addKeywordMap(fgXMLNameChar, fgXMLCategory);
    rangeTokMap->addKeywordMap(fgXMLInitialNameChar, fgXMLCategory);

    fKeywordsInitialized = true;
}

XERCES_CPP_NAMESPACE_END

/**
  * End of file XMLRangeFactory.cpp
  */
