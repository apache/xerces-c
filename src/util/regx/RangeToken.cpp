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
 * Revision 1.4  2001/05/29 19:39:33  knoaman
 * Typo fix
 *
 * Revision 1.3  2001/05/11 13:26:45  tng
 * Copyright update.
 *
 * Revision 1.2  2001/05/03 18:17:37  knoaman
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
 * Revision 1.1  2001/03/02 19:26:46  knoaman
 * Schema: Regular expression handling part II
 *
 */

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <util/regx/RangeToken.hpp>
#include <util/regx/TokenFactory.hpp>
#include <util/XMLExceptMsgs.hpp>

// ---------------------------------------------------------------------------
//  Static member data initialization
// ---------------------------------------------------------------------------
const int RangeToken::MAPSIZE = 256;
const unsigned int RangeToken::INITIALSIZE = 16;

// ---------------------------------------------------------------------------
//  RangeToken: Constructors and Destructors
// ---------------------------------------------------------------------------
RangeToken::RangeToken(const unsigned short tokType) : Token(tokType)
    , fSorted(false)
    , fCompacted(false)
    , fNonMapIndex(0)
    , fElemCount(0)
    , fMaxCount(0)
    , fMap(0)
    , fRanges(0)
    , fCaseIToken(0)
{

}

RangeToken::~RangeToken() {

    delete fMap;
    delete[] fRanges;
}


// ---------------------------------------------------------------------------
//  RangeToken: Getter methods
// ---------------------------------------------------------------------------
RangeToken* RangeToken::getCaseInsensitiveToken(TokenFactory* const tokFactory) {

    // REVIST
    // We will not build a token with case insenstive ranges
    // For now we will return a copy of ourselves.
    if (fCaseIToken == 0 && tokFactory) {

        bool isNRange = (getTokenType() == NRANGE) ? true : false;
        RangeToken* lwrToken = tokFactory->createRange(isNRange);

        lwrToken->mergeRanges(this);
        fCaseIToken = lwrToken;
    }

    return fCaseIToken;
}

// ---------------------------------------------------------------------------
//  RangeToken: Range manipulation methods
// ---------------------------------------------------------------------------
void RangeToken::addRange(const XMLInt32 start, const XMLInt32 end) {

    XMLInt32 val1, val2;

    fCaseIToken = 0;

    if (start <= end) {

        val1 = start;
        val2 = end;
    }
    else {

        val1 = end;
        val2 = start;
    }

    if (fRanges == 0) {

        fMaxCount = INITIALSIZE;
        fRanges = new XMLInt32[fMaxCount];
        fRanges[0] = val1;
        fRanges[1] = val2;
        fElemCount = 2;
        fSorted = true;
    }
    else {

        if (fRanges[fElemCount-1] + 1 == val1) {

            fRanges[fElemCount-1] = val2;
            return;
        }

        if (fElemCount + 2 >= fMaxCount) {
            expand(2);
        }

        if (fRanges[fElemCount-1] >= val1)
            fSorted = false;

        fRanges[fElemCount++] = val1;
        fRanges[fElemCount++] = val2;

        if (!fSorted) {
            sortRanges();
        }
    }
}

void RangeToken::sortRanges() {

    if (fSorted || fRanges == 0)
        return;

    for (int i = fElemCount - 4; i >= 0; i -= 2) {

        for (int j = 0; j <= i; j +=2) {

            if (fRanges[j] > fRanges[j + 2]
                || (fRanges[j]==fRanges[j+2] && fRanges[j+1] > fRanges[j+3])) {

                XMLInt32 tmpVal = fRanges[j+2];
                fRanges[j+2] = fRanges[j];
                fRanges[j] = tmpVal;
                tmpVal = fRanges[j+3];
                fRanges[j+3] = fRanges[j+1];
                fRanges[j+1] = tmpVal;
            }
        }
    }

    fSorted = true;
}

void RangeToken::compactRanges() {

    if (fCompacted || fRanges == 0 || fElemCount <= 2)
        return;

    unsigned int base = 0;
    unsigned int target = 0;

    while (target < fElemCount) {

        if (base != target) {

            fRanges[base] = fRanges[target++];
            fRanges[base+1] = fRanges[target++];
        }
        else
            target += 2;

        XMLInt32 baseEnd = fRanges[base + 1];

        while (target < fElemCount) {

            XMLInt32 startRange = fRanges[target];

            if (baseEnd + 1 < startRange)
                break;

            XMLInt32 endRange = fRanges[target + 1];

            if (baseEnd + 1 == startRange || baseEnd < endRange) {

                baseEnd = endRange;
                fRanges[base+1] = baseEnd;
                target += 2;
            }
            else if (baseEnd >= endRange) {
                target += 2;
            }
            else {
                ThrowXML(RuntimeException, XMLExcepts::Regex_CompactRangesError);
            }
        } // inner while

        base += 2;
    }

    if (base != fElemCount) {

        while (fElemCount > base) {
            fRanges[fElemCount--] = 0;
        }
    }

    fCompacted = true;
}

void RangeToken::mergeRanges(const Token *const tok) {


    if (tok->getTokenType() != this->getTokenType())
        ThrowXML(IllegalArgumentException, XMLExcepts::Regex_MergeRangesTypeMismatch);

    RangeToken* rangeTok = (RangeToken *) tok;

    if (rangeTok->fRanges == 0)
        return;

    fCaseIToken = 0;
    sortRanges();
    rangeTok->sortRanges();

    if (fRanges == 0) {

        fMaxCount = rangeTok->fMaxCount;
        fRanges = new XMLInt32[fMaxCount];
        for (unsigned int index = 0; index < rangeTok->fElemCount; index++) {
            fRanges[index] = rangeTok->fRanges[index];
        }

        fElemCount = rangeTok->fElemCount;
        return;
    }

    unsigned int newMaxCount = (fElemCount + rangeTok->fElemCount >= fMaxCount)
                                 ? fMaxCount + rangeTok->fMaxCount : fMaxCount;
    XMLInt32* result = new XMLInt32[newMaxCount];

    for (unsigned int i=0, j=0, k=0; i < fElemCount || j < rangeTok->fElemCount;) {

        if (i >= fElemCount) {

            for (int count = 0; count < 2; count++) {
                result[k++] = rangeTok->fRanges[j++];
            }
        }
        else if (j >= rangeTok->fElemCount) {

            for (int count = 0; count < 2; count++) {
                result[k++] = fRanges[i++];
            }
        }
        else if (rangeTok->fRanges[j] < fRanges[i]
                 || (rangeTok->fRanges[j] == fRanges[i]
                     && rangeTok->fRanges[j+1] < fRanges[i+1])) {

            for (int count = 0; count < 2; count++) {
                result[k++] = rangeTok->fRanges[j++];
            }
        }
        else {

            for (int count = 0; count < 2; count++) {

                result[k++] = fRanges[i++];
            }
        }
    }

    delete [] fRanges;
    fElemCount += rangeTok->fElemCount;
    fRanges = result;
    fMaxCount = newMaxCount;
}

void RangeToken::subtractRanges(RangeToken* const tok) {

    if (fRanges == 0 || tok->fRanges == 0)
        return;

    if (tok->getTokenType() == NRANGE) {

        intersectRanges(tok);
        return;
    }

    fCaseIToken = 0;
    sortRanges();
    compactRanges();
    tok->sortRanges();
    tok->compactRanges();

    unsigned int newMax = (fElemCount + tok->fElemCount >= fMaxCount)
                             ? fMaxCount + tok->fMaxCount : fMaxCount;
    XMLInt32* result = new XMLInt32[newMax];
    unsigned int newElemCount = 0;
    unsigned int srcCount = 0;
    unsigned int subCount = 0;

    while (srcCount < fElemCount && subCount < tok->fElemCount) {

        XMLInt32 srcBegin = fRanges[srcCount];
        XMLInt32 srcEnd = fRanges[srcCount + 1];
        XMLInt32 subBegin = tok->fRanges[subCount];
        XMLInt32 subEnd = tok->fRanges[subCount + 1];

        if (srcEnd < subBegin) { // no overlap

            result[newElemCount++] = fRanges[srcCount++];
            result[newElemCount++] = fRanges[srcCount++];
        }
        else if (srcEnd >= subBegin && srcBegin <= subEnd) {

            if (subBegin <= srcBegin && srcEnd <= subEnd) {
                srcCount += 2;
            }
            else if (subBegin <= srcBegin) {

                fRanges[srcCount] = subEnd + 1;
                subCount += 2;
            }
            else if (srcEnd <= subEnd) {

                result[newElemCount++] = srcBegin;
                result[newElemCount++] = subBegin - 1;
                srcCount += 2;
            }
            else {

                result[newElemCount++] = srcBegin;
                result[newElemCount++] = subBegin - 1;
                fRanges[srcCount] = subEnd + 1;
                subCount += 2;
            }
        }
        else if (subEnd < srcBegin) {
            subCount += 2;
        }
        else {
            delete [] result;
            ThrowXML(RuntimeException, XMLExcepts::Regex_SubtractRangesError);
        }
    } //end while

    while (srcCount < fElemCount) {

        result[newElemCount++] = fRanges[srcCount++];
        result[newElemCount++] = fRanges[srcCount++];
    }

    delete [] fRanges;
    fRanges = result;
    fElemCount = newElemCount;
    fMaxCount = newMax;
}

/**
  * Ignore whether 'tok' is NRANGE or not.
  */
void RangeToken::intersectRanges(RangeToken* const tok) {

    if (fRanges == 0 || tok->fRanges == 0)
        return;

    fCaseIToken = 0;
    sortRanges();
    compactRanges();
    tok->sortRanges();
    tok->compactRanges();

    unsigned int newMax = (fElemCount + tok->fElemCount >= fMaxCount)
                             ? fMaxCount + tok->fMaxCount : fMaxCount;
    XMLInt32* result = new XMLInt32[newMax];
    unsigned int newElemCount = 0;
    unsigned int srcCount = 0;
    unsigned int tokCount = 0;

    while (srcCount < fElemCount && tokCount < tok->fElemCount) {

        XMLInt32 srcBegin = fRanges[srcCount];
        XMLInt32 srcEnd = fRanges[srcCount + 1];
        XMLInt32 tokBegin = tok->fRanges[tokCount];
        XMLInt32 tokEnd = tok->fRanges[tokCount + 1];

        if (srcEnd < tokBegin) {
            srcCount += 2;
        }
        else if (srcEnd >= tokBegin && srcBegin <= tokEnd) {

            if (tokBegin <= srcBegin && srcEnd <= tokEnd) {

                result[newElemCount++] = srcBegin;
                result[newElemCount++] = srcEnd;
                srcCount += 2;
            }
            else if (tokBegin <= srcBegin) {

                result[newElemCount++] = srcBegin;
                result[newElemCount++] = tokEnd;
                tokCount += 2;

                if (tokCount < tok->fElemCount)
                    fRanges[srcCount] = tokEnd + 1;
                else
                    srcCount += 2;
            }
            else if (srcEnd <= tokEnd) {

                result[newElemCount++] = tokBegin;
                result[newElemCount++] = srcEnd;
                srcCount += 2;
            }
            else {

                result[newElemCount++] = tokBegin;
                result[newElemCount++] = tokEnd;
                tokCount += 2;

                if (tokCount < tok->fElemCount)
                    fRanges[srcCount] = tokEnd + 1;
                else
                    srcCount += 2;
            }
        }
        else if (tokEnd < srcBegin) {
            tokCount += 2;

            if (tokCount >= tok->fElemCount)
                srcCount += 2;
        }
        else {

            delete [] result;
            ThrowXML(RuntimeException, XMLExcepts::Regex_IntersectRangesError);
        }
    } //end while

    delete [] fRanges;
    fRanges = result;
    fElemCount = newElemCount;
    fMaxCount = newMax;
}

/**
  * for RANGE: Creates complement.
  * for NRANGE: Creates the same meaning RANGE.
  */
Token* RangeToken::complementRanges(RangeToken* const tok,
                                    TokenFactory* const tokFactory) {

    if (tok->getTokenType() != RANGE && tok->getTokenType() != NRANGE)
        ThrowXML(IllegalArgumentException, XMLExcepts::Regex_ComplementRangesInvalidArg);

    tok->sortRanges();
    tok->compactRanges();

    XMLInt32 lastElem = tok->fRanges[tok->fElemCount - 1];
    RangeToken* rangeTok = tokFactory->createRange();

    if (tok->fRanges[0] > 0) {
        rangeTok->addRange(0, tok->fRanges[0] - 1);
    }

    for (unsigned int i= 1; i< tok->fElemCount - 2; i += 2) {
        rangeTok->addRange(tok->fRanges[i] + 1, tok->fRanges[i+1] - 1);
    }

    if (lastElem != UTF16_MAX) {
        rangeTok->addRange(lastElem + 1, UTF16_MAX);
    }

    rangeTok->fCompacted = true;

    return rangeTok;
}


// ---------------------------------------------------------------------------
//  RangeToken: Match methods
// ---------------------------------------------------------------------------
bool RangeToken::match(const XMLInt32 ch) {

    if (fMap == 0)
        createMap();

    bool ret;

    if (getTokenType() == RANGE) {

        if (ch < MAPSIZE)
            return ((fMap[ch/32] & (1<<(ch&0x1f))) != 0);

        ret = false;

        for (unsigned int i= fNonMapIndex; i< fElemCount; i +=2) {

            if (fRanges[i] <= ch && ch <= fRanges[i+1])
                return true;
        }
    }
    else {

        if (ch < MAPSIZE)
            return ((fMap[ch/32] & (1<<(ch&0x1f))) == 0);

        ret = true;

        for (unsigned int i= fNonMapIndex; i< fElemCount; i += 2) {

            if (fRanges[i] <= ch && ch <= fRanges[i+1])
                return false;
        }
    }

    return ret;
}

// ---------------------------------------------------------------------------
//  RangeToken: Private helpers methods
// ---------------------------------------------------------------------------
void RangeToken::expand(const unsigned int length) {

    unsigned int newMax = fElemCount + length;

    // Avoid too many reallocations by expanding by a percentage
    unsigned int minNewMax = (unsigned int)((double)fElemCount * 1.25);
    if (newMax < minNewMax)
        newMax = minNewMax;

    XMLInt32* newList = new XMLInt32[newMax];
    for (unsigned int index = 0; index < fElemCount; index++)
        newList[index] = fRanges[index];

    delete [] fRanges;
    fRanges = newList;
    fMaxCount = newMax;
}

void RangeToken::createMap() {

    int asize = MAPSIZE/32;
    fMap = new int[asize];
    fNonMapIndex = fElemCount;

    for (int i = 0; i < asize; i++) {
        fMap[i] = 0;
    }

    for (unsigned int j= 0; j < fElemCount; j += 2) {

        XMLInt32 begin = fRanges[j];
        XMLInt32 end = fRanges[j+1];

        if (begin < MAPSIZE) {

            for (int k = begin; k <= end && k < MAPSIZE; k++) {
                fMap[k/32] |= 1<<(k&0x1F);
            }
        }
        else {

            fNonMapIndex = j;
            break;
        }

        if (end >= MAPSIZE) {

            fNonMapIndex = j;
            break;
        }
    }
}
/**
  * End of file RangeToken.cpp
  */
