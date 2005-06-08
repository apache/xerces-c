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
 * $Log$
 * Revision 1.15  2004/09/09 20:09:30  peiyongz
 * getDataOverflowed()
 *
 * Revision 1.14  2004/09/08 13:56:24  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.13  2003/12/17 00:18:35  cargilld
 * Update to memory management so that the static memory manager (one used to call Initialize) is only for static data.
 *
 * Revision 1.12  2003/10/15 14:50:01  peiyongz
 * Bugzilla#22821: locale-sensitive function used to validate 'double' type, patch
 * from jsweeney@spss.com (Jeff Sweeney)
 *
 * Revision 1.11  2003/09/23 18:16:07  peiyongz
 * Inplementation for Serialization/Deserialization
 *
 * Revision 1.10  2003/05/16 06:01:53  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.9  2003/05/16 03:11:22  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.8  2003/03/10 20:55:58  peiyongz
 * Schema Errata E2-40 double/float
 *
 * Revision 1.7  2003/02/02 23:54:43  peiyongz
 * getFormattedString() added to return original and converted value.
 *
 * Revision 1.6  2003/01/30 19:14:43  tng
 * On some platforms like Solaris strtod will return -0.0.   So need to consider this scenario as well.
 *
 * Revision 1.5  2002/12/11 00:20:02  peiyongz
 * Doing businesss in value space. Converting out-of-bound value into special values.
 *
 * Revision 1.4  2002/11/04 15:22:05  tng
 * C++ Namespace Support.
 *
 * Revision 1.3  2002/09/24 19:51:24  tng
 * Performance: use XMLString::equals instead of XMLString::compareString
 *
 * Revision 1.2  2002/05/03 16:05:45  peiyongz
 * Bug 7341: Missing newline at end of util and DOM source files,
 * patch from Martin Kalen.
 *
 * Revision 1.1.1.1  2002/02/01 22:22:15  peiyongz
 * sane_include
 *
 * Revision 1.16  2001/11/19 21:33:42  peiyongz
 * Reorganization: Double/Float
 *
 * Revision 1.15  2001/11/19 17:27:55  peiyongz
 * Boundary Values updated
 *
 * Revision 1.14  2001/10/26 16:37:46  peiyongz
 * Add thread safe code
 *
 * Revision 1.12  2001/09/20 13:11:41  knoaman
 * Regx  + misc. fixes
 *
 * Revision 1.11  2001/09/14 13:57:59  peiyongz
 * exponent is a must if 'E' or 'e' is present.
 *
 * Revision 1.10  2001/08/23 11:54:26  tng
 * Add newline at the end and various typo fixes.
 *
 * Revision 1.9  2001/08/21 15:10:15  peiyongz
 * Bugzilla# 3017: MSVC5.0: C2202: 'compareSpecial' : not all
 * control paths return a value
 *
 * Revision 1.8  2001/08/14 22:10:20  peiyongz
 * new exception message added
 *
 * Revision 1.7  2001/07/31 17:38:16  peiyongz
 * Fix: memory leak by static (boundry) objects
 *
 * Revision 1.6  2001/07/31 13:48:29  peiyongz
 * fValue removed
 *
 * Revision 1.5  2001/07/27 20:43:53  peiyongz
 * copy ctor: to check for special types.
 *
 * Revision 1.4  2001/07/26 18:21:15  peiyongz
 * Boundary Checking
 *
 * Revision 1.3  2001/07/25 19:06:56  peiyongz
 * Fix to Linux compilation error: XMLDouble::LiteralType' is not an aggregate type
 *
 * Revision 1.2  2001/07/24 21:52:27  peiyongz
 * XMLDouble: move fg...String to XMLUni
 *
 * Revision 1.1  2001/07/24 13:58:11  peiyongz
 * XMLDouble and related supporting methods from XMLBigInteger/XMLBigDecimal
 *
 */

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/util/XMLDouble.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/NumberFormatException.hpp>
#include <xercesc/util/Janitor.hpp>

#include <string.h>
#include <stdlib.h>

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  ctor/dtor
// ---------------------------------------------------------------------------
XMLDouble::XMLDouble(const XMLCh* const strValue,
                     MemoryManager* const manager)
:XMLAbstractDoubleFloat(manager)
{
    init(strValue);
}

XMLDouble::~XMLDouble()
{
}

void XMLDouble::checkBoundary(char* const strValue)
{
    convert(strValue);
}

/***
 * Support for Serialization/De-serialization
 ***/

IMPL_XSERIALIZABLE_TOCREATE(XMLDouble)

XMLDouble::XMLDouble(MemoryManager* const manager)
:XMLAbstractDoubleFloat(manager)
{
}

void XMLDouble::serialize(XSerializeEngine& serEng)
{
    XMLAbstractDoubleFloat::serialize(serEng);
}

XERCES_CPP_NAMESPACE_END
