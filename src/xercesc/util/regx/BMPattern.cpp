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
 * Revision 1.2  2001/05/11 13:26:40  tng
 * Copyright update.
 *
 * Revision 1.1  2001/03/02 19:22:31  knoaman
 * Schema: Regular expression handling part I
 *
 */

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/util/regx/BMPattern.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/Janitor.hpp>

// ---------------------------------------------------------------------------
//  BMPattern: Constructors
// ---------------------------------------------------------------------------
BMPattern::BMPattern(const XMLCh* const pattern, bool ignoreCase)
	:fPattern(XMLString::replicate(pattern)),
	 fUppercasePattern(0),
	 fIgnoreCase(ignoreCase),
	 fShiftTable(0),
	 fShiftTableLen(256) {

	try {
		initialize();
	}
	catch(...) {

		cleanUp();
		throw;
	}
}

BMPattern::BMPattern(const XMLCh* const pattern, int tableSize, bool ignoreCase)
	:fPattern(XMLString::replicate(pattern)),
	 fUppercasePattern(0),
	 fIgnoreCase(ignoreCase),
	 fShiftTable(0),
	 fShiftTableLen(tableSize) {
	
	try {
		initialize();
	}
	catch(...) {

		cleanUp();
		throw;
	}
}

BMPattern::~BMPattern() {

	cleanUp();
}

// ---------------------------------------------------------------------------
//  BMPattern: matches methods
// ---------------------------------------------------------------------------
int BMPattern::matches(const XMLCh* const content, int start, int limit) {

	const unsigned int	patternLen = XMLString::stringLen(fPattern);
	// Uppercase Content
	XMLCh* ucContent = 0;

	if (patternLen == 0)
		return start;

	if (fIgnoreCase) {
		
		ucContent = XMLString::replicate(content);
		XMLString::upperCase(ucContent);
	}

	ArrayJanitor<XMLCh> janUCContent(ucContent);

	int index = start + patternLen;

	while (index <= limit) {

		int patternIndex = patternLen;
		int nIndex = index + 1;
		XMLCh ch;

		while (patternIndex > 0) {

			ch = content[--index];

			if (ch != fPattern[--patternIndex]) {

				// No match, so we will break. But first we have
				// to check the ignore case flag. If it is set, then
				// we try to match with the case ignored
				if (!fIgnoreCase ||
					(fUppercasePattern[patternIndex] != ucContent[index]))
					break;
			}

			if (patternIndex == 0)
				return index;
		}

		index += fShiftTable[ch % fShiftTableLen] + 1;

		if (index < nIndex)
			index = nIndex;
	}

	return -1;
}

// ---------------------------------------------------------------------------
//  BMPattern: private helpers methods
// ---------------------------------------------------------------------------
void BMPattern::initialize() {

	const unsigned int	patternLen = XMLString::stringLen(fPattern);
	XMLCh* lowercasePattern = 0;

	fShiftTable = new int[fShiftTableLen];

	if (fIgnoreCase) {

		fUppercasePattern = XMLString::replicate(fPattern);
		lowercasePattern = XMLString::replicate(fPattern);
		XMLString::upperCase(fUppercasePattern);
		XMLString::lowerCase(lowercasePattern);
	}

	ArrayJanitor<XMLCh> janLowercase(lowercasePattern);

	for (unsigned int i=0; i< fShiftTableLen; i++)
		fShiftTable[i] = patternLen;

	for (unsigned int k=0; k< patternLen; k++) {

		XMLCh	ch = fPattern[k];
		int		diff = patternLen - k - 1;
		int		index = ch % fShiftTableLen;

		if (diff < fShiftTable[index])
			fShiftTable[index] = diff;

		if (fIgnoreCase) {

			for (int j=0; j< 2; j++) {

				ch = (j == 0) ? fUppercasePattern[k] : lowercasePattern[k];
				index = ch % fShiftTableLen;

				if (diff < fShiftTable[index])
					fShiftTable[index] = diff;
			}
		}
	}
}

/**
  *	End of file BMPattern.cpp
  */
