/*
 * Copyright 2002,2004 The Apache Software Foundation.
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

#ifndef __XMLEXCEPTIONHANDLER
#define __XMLEXCEPTIONHANDLER

#include "xercesc/util/ArrayIndexOutOfBoundsException.hpp"
#include "xercesc/util/EmptyStackException.hpp"
#include "xercesc/util/IllegalArgumentException.hpp"
#include "xercesc/util/InvalidCastException.hpp"
#include "xercesc/util/IOException.hpp"
#include "xercesc/util/NoSuchElementException.hpp"
#include "xercesc/util/NullPointerException.hpp"
#include "xercesc/util/NumberFormatException.hpp"
#include "xercesc/util/ParseException.hpp"
#include "xercesc/util/RuntimeException.hpp"
#include "xercesc/util/SchemaDateTimeException.hpp"
#include "xercesc/util/TranscodingException.hpp"
#include "xercesc/util/UnexpectedEOFException.hpp"
#include "xercesc/util/UnsupportedEncodingException.hpp"
#include "xercesc/util/UTFDataFormatException.hpp"
#include "xercesc/util/XMLNetAccessor.hpp"
#include "xercesc/internal/XSerializationException.hpp"
#include "xercesc/validators/datatype/InvalidDatatypeFacetException.hpp"
#include "xercesc/validators/datatype/InvalidDatatypeValueException.hpp"
#include "xercesc/validators/schema/identity/XPathException.hpp"

XERCES_CPP_NAMESPACE_BEGIN

class XMLExceptionHandler {

protected:

  XMLExceptionHandler();
  static XMLExceptionHandler* _instance;

public:

  static XMLExceptionHandler* getInstance();

  ~XMLExceptionHandler();
  XMLException* copyXMLException(const XMLException& e);
};

XERCES_CPP_NAMESPACE_END

#endif /* __XMLEXCEPTIONHANDLER */
