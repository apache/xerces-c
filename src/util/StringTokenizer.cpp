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
 * Revision 1.2  2001/05/11 13:26:29  tng
 * Copyright update.
 *
 * Revision 1.1  2001/04/19 17:47:58  knoaman
 * A string tokenizer utility.
 *
 */

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <util/StringTokenizer.hpp>
#include <util/XMLUniDefs.hpp>


// ---------------------------------------------------------------------------
//  StringTokenizer: Local const data
// ---------------------------------------------------------------------------
const XMLCh fgDelimeters[] =
{
    chSpace, chHTab, chCR, chLF, chNull
};

// ---------------------------------------------------------------------------
//  StringTokenizer: Constructors and Destructor
// ---------------------------------------------------------------------------
StringTokenizer::StringTokenizer(const XMLCh* const srcStr)
    : fOffset(0)
    , fStringLen(XMLString::stringLen(srcStr))
    , fString(XMLString::replicate(srcStr))
    , fTokens(0)
{
	try {

        fDelimeters = XMLString::replicate(fgDelimeters);

        if (fStringLen > 0) {
            fTokens = new RefVectorOf<XMLCh>(4, true);
        }
    }
    catch(...) {
        cleanUp();
    }
}

StringTokenizer::StringTokenizer(const XMLCh* const srcStr,
                                 const XMLCh* const delim)
    : fOffset(0)
    , fStringLen(XMLString::stringLen(srcStr))
    , fString(XMLString::replicate(srcStr))
    , fTokens(0)
{
	try {

        fDelimeters = XMLString::replicate(delim);

        if (fStringLen > 0) {
            fTokens = new RefVectorOf<XMLCh>(4, true);
        }
    }
    catch(...) {
        cleanUp();
    }
}

StringTokenizer::~StringTokenizer()
{
	cleanUp();
}


// ---------------------------------------------------------------------------
//  StringTokenizer: Management methods
// ---------------------------------------------------------------------------
XMLCh* StringTokenizer::nextToken() {

    if (fOffset >= fStringLen) {
        return 0;
    }

    bool tokFound = false;
    int  startIndex = fOffset;
    int  endIndex = fOffset;

    for (; endIndex < fStringLen; endIndex++) {

        if (isDelimeter(fString[endIndex])) {

			if (tokFound) {
                break;
            }

			startIndex++;
			continue;
        }

        tokFound = true;
    }

    fOffset = endIndex;

    if (tokFound) {

        XMLCh* tokStr = new XMLCh[(endIndex - startIndex) + 1];

        XMLString::subString(tokStr, fString, startIndex, endIndex);
        fTokens->addElement(tokStr);

        return tokStr;
	}

    return 0;
}


bool StringTokenizer::hasMoreTokens() {

    if (countTokens() > 0)
        return true;

	return false;
}


/**
  * End of file StringTokenizer.cpp
  */

