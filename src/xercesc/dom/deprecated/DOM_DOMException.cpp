/*
 * Copyright 1999-2002,2004 The Apache Software Foundation.
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

#include "DOM_DOMException.hpp"
#include "DOMString.hpp"

XERCES_CPP_NAMESPACE_BEGIN


DOM_DOMException::DOM_DOMException()
: msg(0)
{
        code = (ExceptionCode) 0;
};


DOM_DOMException::DOM_DOMException(short exCode, const DOMString &message)
: msg(message)
{
   code = (ExceptionCode) exCode;
};


DOM_DOMException::DOM_DOMException(const DOM_DOMException &other)
: msg(other.msg)
{
        code = other.code;
};


DOM_DOMException::~DOM_DOMException()
{
};

XERCES_CPP_NAMESPACE_END

