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
 * Revision 1.1  2002/02/01 22:22:28  peiyongz
 * Initial revision
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


// ---------------------------------------------------------------------------
//  Static member data initialization
// ---------------------------------------------------------------------------
bool ASCIIRangeFactory::fRangesCreated = false;
bool ASCIIRangeFactory::fKeywordsInitialized = false;

// ---------------------------------------------------------------------------
//  ASCIIRangeFactory: Constructors and Destructor
// ---------------------------------------------------------------------------
ASCIIRangeFactory::ASCIIRangeFactory() {

}

ASCIIRangeFactory::~ASCIIRangeFactory() {

}

// ---------------------------------------------------------------------------
//  ASCIIRangeFactory: Range creation methods
// ---------------------------------------------------------------------------
void ASCIIRangeFactory::buildRanges() {

    if (fRangesCreated)
        return;

    if (!fKeywordsInitialized) {
        initializeKeywordMap();
    }

    RangeTokenMap* rangeTokMap = RangeTokenMap::instance();
    TokenFactory* tokFactory = rangeTokMap->getTokenFactory();

    // Create space ranges
    RangeToken* tok = tokFactory->createRange();
    tok->addRange(chHTab, chHTab);
	tok->addRange(chLF, chLF);
	tok->addRange(chFF, chFF);
	tok->addRange(chCR, chCR);
	tok->addRange(chSpace, chSpace);
	rangeTokMap->setRangeToken(fgASCIISpace, tok);

    // Create digits ranges
    tok = tokFactory->createRange();
    tok->addRange(chDigit_0, chDigit_9);
    rangeTokMap->setRangeToken(fgASCIIDigit, tok);

    // Create word ranges
    tok = tokFactory->createRange();
    tok->addRange(chDigit_0, chDigit_9);
	tok->addRange(chLatin_A, chLatin_Z);
    tok->addRange(chUnderscore, chUnderscore);
	tok->addRange(chLatin_a, chLatin_z);
    rangeTokMap->setRangeToken(fgASCIIWord, tok);

    // Create xdigit ranges
    tok = tokFactory->createRange();
    tok->addRange(chDigit_0, chDigit_9);
    tok->addRange(chLatin_A, chLatin_F);
    tok->addRange(chLatin_a, chLatin_a);
    rangeTokMap->setRangeToken(fgASCIIXDigit, tok);

    // Create ascii ranges
    tok = tokFactory->createRange();
    tok->addRange(0x00, 0x7F);
    rangeTokMap->setRangeToken(fgASCII, tok);

    fRangesCreated = true;
}

// ---------------------------------------------------------------------------
//  ASCIIRangeFactory: Range creation methods
// ---------------------------------------------------------------------------
void ASCIIRangeFactory::initializeKeywordMap() {

    if (fKeywordsInitialized)
        return;

	RangeTokenMap* rangeTokMap = RangeTokenMap::instance();

    rangeTokMap->addKeywordMap(fgASCIISpace, fgASCIICategory);
    rangeTokMap->addKeywordMap(fgASCIIDigit, fgASCIICategory);
    rangeTokMap->addKeywordMap(fgASCIIWord, fgASCIICategory);
    rangeTokMap->addKeywordMap(fgASCIIXDigit, fgASCIICategory);
    rangeTokMap->addKeywordMap(fgASCII, fgASCIICategory);

    fKeywordsInitialized = true;
}

/**
  * End of file ASCIIRangeFactory.cpp
  */
