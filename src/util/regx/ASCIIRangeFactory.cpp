/*
 * The Apache Software License, Version 1.1
 * 
 * Copyright (c) 1999-2001 The Apache Software Foundation.  All rights
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
 * $Log$
 * Revision 1.1  2001/03/02 19:26:39  knoaman
 * Schema: Regular expression handling part II
 *
 */

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <util/regx/ASCIIRangeFactory.hpp>
#include <util/regx/RegxDefs.hpp>
#include <util/regx/TokenFactory.hpp>
#include <util/regx/RangeToken.hpp>


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

    // Create space ranges
    RangeToken* tok = TokenFactory::instance()->createRange();
    tok->addRange(chHTab, chHTab);
	tok->addRange(chLF, chLF);
	tok->addRange(chFF, chFF);
	tok->addRange(chCR, chCR);
	tok->addRange(chSpace, chSpace);
    setRangeToken(fgASCIISpace, tok);

    // Create digits ranges
    tok = TokenFactory::instance()->createRange();
    tok->addRange(chDigit_0, chDigit_9);
    setRangeToken(fgASCIIDigit, tok);

    // Create word ranges
    tok = TokenFactory::instance()->createRange();
    tok->addRange(chDigit_0, chDigit_9);
	tok->addRange(chLatin_A, chLatin_Z);
    tok->addRange(chUnderscore, chUnderscore);
	tok->addRange(chLatin_a, chLatin_z);
    setRangeToken(fgASCIIWord, tok);

    // Create xdigit ranges
    tok = TokenFactory::instance()->createRange();
    tok->addRange(chDigit_0, chDigit_9);
    tok->addRange(chLatin_A, chLatin_F);
    tok->addRange(chLatin_a, chLatin_a);
    setRangeToken(fgASCIIXDigit, tok);

    // Create ascii ranges
    tok = TokenFactory::instance()->createRange();
    tok->addRange(0x00, 0x7F);
    setRangeToken(fgASCII, tok);

    fRangesCreated = true;
}

// ---------------------------------------------------------------------------
//  ASCIIRangeFactory: Range creation methods
// ---------------------------------------------------------------------------
void ASCIIRangeFactory::initializeKeywordMap() {

    if (fKeywordsInitialized)
        return;

    addKeywordMap(fgASCIISpace, fgASCIICategory);
    addKeywordMap(fgASCIIDigit, fgASCIICategory);
    addKeywordMap(fgASCIIWord, fgASCIICategory);
    addKeywordMap(fgASCIIXDigit, fgASCIICategory);
    addKeywordMap(fgASCII, fgASCIICategory);

    fKeywordsInitialized = true;
}

/**
  * End of file ASCIIRangeFactory.cpp
  */
