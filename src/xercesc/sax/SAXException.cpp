/*
 * Copyright 1999-2000,2004 The Apache Software Foundation.
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
 * Revision 1.4  2004/09/08 13:56:19  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.3  2003/08/13 15:43:24  knoaman
 * Use memory manager when creating SAX exceptions.
 *
 * Revision 1.2  2002/11/04 14:56:26  tng
 * C++ Namespace Support.
 *
 * Revision 1.1.1.1  2002/02/01 22:22:08  peiyongz
 * sane_include
 *
 * Revision 1.1  2000/08/09 22:06:04  jpolast
 * more functionality to SAXException and its children.
 * msgs are now functional for SAXNotSupportedEx and
 * SAXNotRecognizedEx
 *
 *
 */

#include "SAXException.hpp"

XERCES_CPP_NAMESPACE_BEGIN


// SAXNotSupportedException Constructors
SAXNotSupportedException::SAXNotSupportedException(MemoryManager* const manager)
	: SAXException(manager)
{
}

SAXNotSupportedException::SAXNotSupportedException(const XMLCh* const msg,
                                                   MemoryManager* const manager)
	: SAXException(msg, manager)
{
}

SAXNotSupportedException::SAXNotSupportedException(const char* const msg,
                                                   MemoryManager* const manager)
	: SAXException(msg, manager)
{
}

SAXNotSupportedException::SAXNotSupportedException(const SAXException& toCopy)
  : SAXException(toCopy)
{
}

// SAXNotRecognizedException Constructors
SAXNotRecognizedException::SAXNotRecognizedException(MemoryManager* const manager)
	: SAXException(manager)
{
}

SAXNotRecognizedException::SAXNotRecognizedException(const XMLCh* const msg,
                                                     MemoryManager* const manager)
	: SAXException(msg, manager)
{
}

SAXNotRecognizedException::SAXNotRecognizedException(const char* const msg,
                                                     MemoryManager* const manager)
	: SAXException(msg, manager)
{
}

SAXNotRecognizedException::SAXNotRecognizedException(const SAXException& toCopy)
  : SAXException(toCopy)
{
}

XERCES_CPP_NAMESPACE_END

