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
 * Revision 1.5  2005/05/19 15:46:32  cargilld
 * 390 update: use ICU table which is present with the uniconv390.
 *
 * Revision 1.4  2004/09/08 13:56:47  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.3  2002/12/24 17:59:07  tng
 * Build with ICU 2.4
 *
 * Revision 1.2  2002/11/04 15:17:01  tng
 * C++ Namespace Support.
 *
 * Revision 1.1.1.1  2002/02/01 22:22:34  peiyongz
 * sane_include
 *
 * Revision 1.2  2001/05/11 13:26:52  tng
 * Copyright update.
 *
 * Revision 1.1  2001/03/02 19:26:51  knoaman
 * Schema: Regular expression handling part II
 *
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/util/regx/XMLUniCharacter.hpp>

#if defined (XML_USE_ICU_TRANSCODER) || defined (XML_USE_UNICONV390_TRANSCODER)
   #include <unicode/uchar.h>
#else
   #include <xercesc/util/regx/UniCharTable.hpp>
#endif

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  XMLUniCharacter: Public static methods
// ---------------------------------------------------------------------------
unsigned short XMLUniCharacter::getType(const XMLCh ch) {

#if defined (XML_USE_ICU_TRANSCODER) || defined (XML_USE_UNICONV390_TRANSCODER)
	return (unsigned short) u_charType(ch);
#else
	return (unsigned short) fgUniCharsTable[ch];
#endif
}

XERCES_CPP_NAMESPACE_END

/**
  * End of file XMLUniCharacter.cpp
  */

