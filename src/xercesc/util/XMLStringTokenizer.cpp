/*
 * Copyright 2001,2004 The Apache Software Foundation.
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
 * $Id$
 */

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/util/Janitor.hpp>
#include <xercesc/util/XMLStringTokenizer.hpp>
#include <xercesc/util/XMLUniDefs.hpp>
#include <xercesc/util/OutOfMemoryException.hpp>

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  XMLStringTokenizer: Local const data
// ---------------------------------------------------------------------------
const XMLCh fgDelimeters[] =
{
    chSpace, chHTab, chCR, chLF, chNull
};

// ---------------------------------------------------------------------------
//  XMLStringTokenizer: Constructors and Destructor
// ---------------------------------------------------------------------------

typedef JanitorMemFunCall<XMLStringTokenizer>   CleanupType;

XMLStringTokenizer::XMLStringTokenizer( const XMLCh* const srcStr
                                      , MemoryManager* const manager)
    : fOffset(0)
    , fStringLen(XMLString::stringLen(srcStr))
    , fString(XMLString::replicate(srcStr, manager))
    , fDelimeters(XMLString::replicate(fgDelimeters, manager))
    , fTokens(0)
    , fMemoryManager(manager)
{
    CleanupType cleanup(this, &XMLStringTokenizer::cleanUp);

	try {
        if (fStringLen > 0) {
            fTokens = new (fMemoryManager) RefArrayVectorOf<XMLCh>(4, true, fMemoryManager);
        }
    }
    catch(const OutOfMemoryException&)
    {
        cleanup.release();

        throw;
    }

    cleanup.release();
}

XMLStringTokenizer::XMLStringTokenizer(const XMLCh* const srcStr,
                                       const XMLCh* const delim,
                                       MemoryManager* const manager)
    : fOffset(0)
    , fStringLen(XMLString::stringLen(srcStr))
    , fString(XMLString::replicate(srcStr, manager))
    , fDelimeters(XMLString::replicate(delim, manager))
    , fTokens(0)
    , fMemoryManager(manager)
{
    CleanupType cleanup(this, &XMLStringTokenizer::cleanUp);

	try {

        

        if (fStringLen > 0) {
            fTokens = new (fMemoryManager) RefArrayVectorOf<XMLCh>(4, true, fMemoryManager);
        }
    }
    catch(const OutOfMemoryException&)
    {
        cleanup.release();

        throw;
    }

    cleanup.release();
}

XMLStringTokenizer::~XMLStringTokenizer()
{
	cleanUp();
}


// ---------------------------------------------------------------------------
//  XMLStringTokenizer: Management methods
// ---------------------------------------------------------------------------
XMLCh* XMLStringTokenizer::nextToken() {

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

        XMLCh* tokStr = (XMLCh*) fMemoryManager->allocate
        (
            (endIndex - startIndex + 1) * sizeof(XMLCh)
        );//new XMLCh[(endIndex - startIndex) + 1];

        XMLString::subString(tokStr, fString, startIndex, endIndex, fMemoryManager);
        fTokens->addElement(tokStr);

        return tokStr;
	}

    return 0;
}


bool XMLStringTokenizer::hasMoreTokens() {

    if (countTokens() > 0)
        return true;

	return false;
}

XERCES_CPP_NAMESPACE_END

/**
  * End of file XMLStringTokenizer.cpp
  */

