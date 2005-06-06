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
 * $Log$
 * Revision 1.10  2004/09/08 13:56:24  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.9  2004/02/05 18:08:38  cargilld
 * Code cleanup changes to get rid of various compiler diagnostic messages.
 *
 * Revision 1.8  2004/01/29 11:48:47  cargilld
 * Code cleanup changes to get rid of various compiler diagnostic messages.
 *
 * Revision 1.7  2003/12/17 00:18:35  cargilld
 * Update to memory management so that the static memory manager (one used to call Initialize) is only for static data.
 *
 * Revision 1.6  2003/10/01 16:32:39  neilg
 * improve handling of out of memory conditions, bug #23415.  Thanks to David Cargill.
 *
 * Revision 1.5  2003/05/18 14:02:05  knoaman
 * Memory manager implementation: pass per instance manager.
 *
 * Revision 1.4  2003/05/16 03:11:22  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.3  2002/12/18 14:17:54  gareth
 * Fix to bug #13438. When you eant a vector that calls delete[] on its members you should use RefArrayVectorOf.
 *
 * Revision 1.2  2002/11/04 15:22:05  tng
 * C++ Namespace Support.
 *
 * Revision 1.1.1.1  2002/02/01 22:22:16  peiyongz
 * sane_include
 *
 * Revision 1.1  2001/06/11 15:15:46  knoaman
 * StringTokenizer renamed to XMLStringTokenizer.
 *
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

