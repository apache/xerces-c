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
 * Revision 1.1  2002/02/01 22:22:31  peiyongz
 * Initial revision
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
#include <xercesc/framework/XMLBuffer.hpp>

XMLCh* RegxUtil::decomposeToSurrogates(XMLInt32 ch) {

	XMLCh* pszStr = new XMLCh[3];

	ch -= 0x10000;
	pszStr[0] = XMLCh((ch >> 10) + 0xD800);
	pszStr[1] = XMLCh((ch & 0x03FF) + 0xDC00);
	pszStr[2] = chNull;

	return pszStr;
}


XMLCh* RegxUtil::stripExtendedComment(const XMLCh* const expression) {

	unsigned int strLen = XMLString::stringLen(expression);

	if (strLen == 0)
		return 0;

	XMLBuffer buffer;
	unsigned int offset = 0;

	while (offset < strLen) {

		XMLCh ch = expression[offset++];

		if (ch == chFF || ch == chCR || ch == chLF
			|| ch == chSpace || ch == chHTab) {
			continue;
		}

		// Skips chracters between '#' and a line end.
		if (ch == chPound) {

			while (offset < strLen) {

				ch = expression[offset++];
				if (ch == chLF || ch == chCR)
					break;
			}
			continue;
		}

		XMLCh next;
		if (ch == chBackSlash && offset < strLen) {

			if ((next = expression[offset]) == chPound
				|| next == chHTab || next == chLF || next == chFF
				|| next == chCR || next == chSpace) {

				buffer.append(next);
				offset++;
			} else {                         // Other escaped character.

				buffer.append(chBackSlash);
				buffer.append(next);
				offset++;
			}
		}
		else {                             // As is.
			buffer.append(ch);
		}
	}

	return XMLString::replicate(buffer.getRawBuffer());
}
/**
  * End of file RegxUtil.cpp
  */
