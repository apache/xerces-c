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
 * Revision 1.6  2005/04/01 17:36:25  dbertoni
 * Fix for Jira issue XERCESC-1389.
 *
 * Revision 1.5  2004/09/08 13:56:47  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.4  2003/05/16 00:03:10  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.3  2003/05/15 18:42:55  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.2  2002/11/04 15:17:00  tng
 * C++ Namespace Support.
 *
 * Revision 1.1.1.1  2002/02/01 22:22:31  peiyongz
 * sane_include
 *
 * Revision 1.3  2001/06/07 20:55:38  tng
 * Fix no newline at the end warning.  By Pei Yong Zhang.
 *
 * Revision 1.2  2001/05/11 13:26:49  tng
 * Copyright update.
 *
 * Revision 1.1  2001/03/02 19:22:56  knoaman
 * Schema: Regular expression handling part I
 *
 */

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/util/regx/RegxUtil.hpp>
#include <xercesc/util/XMLString.hpp>

XERCES_CPP_NAMESPACE_BEGIN

XMLCh* RegxUtil::decomposeToSurrogates(XMLInt32 ch,
                                       MemoryManager* const manager) {

	XMLCh* pszStr = (XMLCh*) manager->allocate(3 *  sizeof(XMLCh));//new XMLCh[3];

	ch -= 0x10000;
	pszStr[0] = XMLCh((ch >> 10) + 0xD800);
	pszStr[1] = XMLCh((ch & 0x03FF) + 0xDC00);
	pszStr[2] = chNull;

	return pszStr;
}


XMLCh* RegxUtil::stripExtendedComment(const XMLCh* const expression,
                                      MemoryManager* const manager) {

    XMLCh* buffer = (manager) ? XMLString::replicate(expression, manager)
                              : XMLString::replicate(expression);

    if (buffer)
    {
        const XMLCh* inPtr = expression;
        XMLCh* outPtr = buffer;

        while (*inPtr) {

            XMLCh ch = *inPtr++;

            if (ch == chFF || ch == chCR || ch == chLF
                || ch == chSpace || ch == chHTab) {
                continue;
            }

		    // Skips chracters between '#' and a line end.
		    if (ch == chPound) {

                while (*inPtr) {

                    ch = *inPtr++;
                    if (ch == chLF || ch == chCR)
                        break;
                }

                continue;
            }

            if (ch == chBackSlash && *inPtr) {

			    if ((ch = *inPtr++) == chPound || ch == chHTab || ch == chLF
                    || ch == chFF || ch == chCR || ch == chSpace) {
                    *outPtr++ = ch;
                }
                else { // Other escaped character.

                    *outPtr++ = chBackSlash;
                    *outPtr++ = ch;
                }
            }
            else { // As is.
                *outPtr++ = ch;
            }
        }

        *outPtr = chNull; // null terminate
    }

    return buffer;
}

XERCES_CPP_NAMESPACE_END

/**
  * End of file RegxUtil.cpp
  */
