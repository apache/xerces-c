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
 * Revision 1.3  2004/09/08 13:56:23  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.2  2002/11/04 15:22:04  tng
 * C++ Namespace Support.
 *
 * Revision 1.1.1.1  2002/02/01 22:22:12  peiyongz
 * sane_include
 *
 * Revision 1.1  2001/11/12 20:36:54  peiyongz
 * SchemaDateTimeException defined
 *
 *
 */


#if !defined(SCHEMA_DATETIME_EXCEPTION_HPP)
#define SCHEMA_DATETIME_EXCEPTION_HPP

#include <xercesc/util/XercesDefs.hpp>
#include <xercesc/util/XMLException.hpp>

XERCES_CPP_NAMESPACE_BEGIN

MakeXMLException(SchemaDateTimeException, XMLUTIL_EXPORT)

XERCES_CPP_NAMESPACE_END

#endif
