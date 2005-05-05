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
 * Revision 1.7  2005/05/05 01:12:24  dbertoni
 * Fix for Jira issue XERCESC-1391.
 *
 * Revision 1.6  2004/11/12 23:24:58  knoaman
 * Fix multi threading problem.
 *
 * Revision 1.5  2004/10/20 15:18:49  knoaman
 * Allow option of initializing static data in XMLPlatformUtils::Initialize
 *
 * Revision 1.4  2004/09/08 13:56:47  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.3  2002/11/04 15:17:00  tng
 * C++ Namespace Support.
 *
 * Revision 1.2  2002/02/05 13:20:06  tng
 * [Bug 5716] Can't parse with Validation more than one file.
 *
 * Revision 1.1.1.1  2002/02/01 22:22:28  peiyongz
 * sane_include
 *
 * Revision 1.3  2001/05/11 13:26:39  tng
 * Copyright update.
 *
 * Revision 1.2  2001/05/03 18:17:18  knoaman
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
 * Revision 1.1  2001/03/02 19:26:39  knoaman
 * Schema: Regular expression handling part II
 *
 */

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/util/regx/ASCIIRangeFactory.hpp>
#include <xercesc/util/regx/RegxDefs.hpp>
#include <xercesc/util/regx/TokenFactory.hpp>
#include <xercesc/util/regx/RangeToken.hpp>
#include <xercesc/util/regx/RangeTokenMap.hpp>

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  ASCIIRangeFactory: Constructors and Destructor
// ---------------------------------------------------------------------------
ASCIIRangeFactory::ASCIIRangeFactory()
{
}

ASCIIRangeFactory::~ASCIIRangeFactory() {

}

// ---------------------------------------------------------------------------
//  ASCIIRangeFactory: Range creation methods
// ---------------------------------------------------------------------------
void ASCIIRangeFactory::buildRanges(RangeTokenMap *rangeTokMap) {

    if (fRangesCreated)
        return;

    if (!fKeywordsInitialized) {
        initializeKeywordMap(rangeTokMap);
    }

    TokenFactory* tokFactory = rangeTokMap->getTokenFactory();

    // Create space ranges
    RangeToken* tok = tokFactory->createRange();
    tok->addRange(chHTab, chHTab);
    tok->addRange(chLF, chLF);
    tok->addRange(chFF, chFF);
    tok->addRange(chCR, chCR);
    tok->addRange(chSpace, chSpace);

    // Build the internal map.
    tok->createMap();

    rangeTokMap->setRangeToken(fgASCIISpace, tok);

    tok = (RangeToken*) RangeToken::complementRanges(tok, tokFactory);

    // Build the internal map.
    tok->createMap();

    rangeTokMap->setRangeToken(fgASCIISpace, tok , true);

    // Create digits ranges
    tok = tokFactory->createRange();
    tok->addRange(chDigit_0, chDigit_9);

    // Build the internal map.
    tok->createMap();

    rangeTokMap->setRangeToken(fgASCIIDigit, tok);

    tok = (RangeToken*) RangeToken::complementRanges(tok, tokFactory);

    // Build the internal map.
    tok->createMap();

    rangeTokMap->setRangeToken(fgASCIIDigit, tok , true);

    // Create word ranges
    tok = tokFactory->createRange();
    tok->addRange(chDigit_0, chDigit_9);
    tok->addRange(chLatin_A, chLatin_Z);
    tok->addRange(chUnderscore, chUnderscore);
    tok->addRange(chLatin_a, chLatin_z);
    // Build the internal map.
    tok->createMap();
    rangeTokMap->setRangeToken(fgASCIIWord, tok);

    tok = (RangeToken*) RangeToken::complementRanges(tok, tokFactory);
    // Build the internal map.
    tok->createMap();
    rangeTokMap->setRangeToken(fgASCIIWord, tok , true);

    // Create xdigit ranges
    tok = tokFactory->createRange();
    tok->addRange(chDigit_0, chDigit_9);
    tok->addRange(chLatin_A, chLatin_F);
    tok->addRange(chLatin_a, chLatin_a);
    // Build the internal map.
    tok->createMap();

    rangeTokMap->setRangeToken(fgASCIIXDigit, tok);

    tok = (RangeToken*) RangeToken::complementRanges(tok, tokFactory);
    // Build the internal map.
    tok->createMap();
    rangeTokMap->setRangeToken(fgASCIIXDigit, tok , true);

    // Create ascii ranges
    tok = tokFactory->createRange();
    tok->addRange(0x00, 0x7F);
    // Build the internal map.
    tok->createMap();
    rangeTokMap->setRangeToken(fgASCII, tok);

    tok = (RangeToken*) RangeToken::complementRanges(tok, tokFactory);
    // Build the internal map.
    tok->createMap();
    rangeTokMap->setRangeToken(fgASCII, tok , true);

    fRangesCreated = true;
}

// ---------------------------------------------------------------------------
//  ASCIIRangeFactory: Range creation methods
// ---------------------------------------------------------------------------
void ASCIIRangeFactory::initializeKeywordMap(RangeTokenMap *rangeTokMap) {

    if (fKeywordsInitialized)
        return;

    rangeTokMap->addKeywordMap(fgASCIISpace, fgASCIICategory);
    rangeTokMap->addKeywordMap(fgASCIIDigit, fgASCIICategory);
    rangeTokMap->addKeywordMap(fgASCIIWord, fgASCIICategory);
    rangeTokMap->addKeywordMap(fgASCIIXDigit, fgASCIICategory);
    rangeTokMap->addKeywordMap(fgASCII, fgASCIICategory);

    fKeywordsInitialized = true;
}

XERCES_CPP_NAMESPACE_END

/**
  * End of file ASCIIRangeFactory.cpp
  */
