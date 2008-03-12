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

#include <stdlib.h>
#include <stdio.h>
#include "XMLExceptionHandler.hpp"

XERCES_CPP_NAMESPACE_USE

XMLExceptionHandler* XMLExceptionHandler::_instance = NULL;

XMLExceptionHandler*
XMLExceptionHandler::getInstance() {
  if (_instance == NULL) {
    _instance = new XMLExceptionHandler();
  }
  return _instance;
}

XMLExceptionHandler::XMLExceptionHandler() {}

XMLException*
XMLExceptionHandler::copyXMLException(const XMLException& e)
{
    if(e.getType() == XMLUni::fgArrayIndexOutOfBoundsException_Name)
    {
        return (XMLException*)((ArrayIndexOutOfBoundsException*)&e)->duplicate();
    }
    else if(e.getType() == XMLUni::fgEmptyStackException_Name)
    {
        return (XMLException*)((EmptyStackException*)&e)->duplicate();
    }
    else if(e.getType() == XMLUni::fgIllegalArgumentException_Name)
    {
        return (XMLException*)((IllegalArgumentException*)&e)->duplicate();
    }
    else if(e.getType() == XMLUni::fgInvalidCastException_Name)
    {
        return (XMLException*)((InvalidCastException*)&e)->duplicate();
    }
    else if(e.getType() == XMLUni::fgIOException_Name)
    {
        return (XMLException*)((IOException*)&e)->duplicate();
    }
    else if(e.getType() == XMLUni::fgNoSuchElementException_Name)
    {
        return (XMLException*)((NoSuchElementException*)&e)->duplicate();
    }
    else if(e.getType() == XMLUni::fgNullPointerException_Name)
    {
        return (XMLException*)((NullPointerException*)&e)->duplicate();
    }
    else if(e.getType() == XMLUni::fgNumberFormatException_Name)
    {
        return (XMLException*)((NumberFormatException*)&e)->duplicate();
    }
    else if(e.getType() == XMLUni::fgParseException_Name)
    {
        return (XMLException*)((ParseException*)&e)->duplicate();
    }
    else if(e.getType() == XMLUni::fgXMLPlatformUtilsException_Name)
    {
        return (XMLException*)((XMLPlatformUtilsException*)&e)->duplicate();
    }
    else if(e.getType() == XMLUni::fgRuntimeException_Name)
    {
        return (XMLException*)((RuntimeException*)&e)->duplicate();
    }
    else if(e.getType() == XMLUni::fgSchemaDateTimeException_Name)
    {
        return (XMLException*)((SchemaDateTimeException*)&e)->duplicate();
    }
    else if(e.getType() == XMLUni::fgTranscodingException_Name)
    {
        return (XMLException*)((TranscodingException*)&e)->duplicate();
    }
    else if(e.getType() == XMLUni::fgUnexpectedEOFException_Name)
    {
        return (XMLException*)((UnexpectedEOFException*)&e)->duplicate();
    }
    else if(e.getType() == XMLUni::fgUnsupportedEncodingException_Name)
    {
        return (XMLException*)((UnsupportedEncodingException*)&e)->duplicate();
    }
    else if(e.getType() == XMLUni::fgUTFDataFormatException_Name)
    {
        return (XMLException*)((UTFDataFormatException*)&e)->duplicate();
    }
    else if(e.getType() == XMLUni::fgNetAccessorException_Name)
    {
        return (XMLException*)((NetAccessorException*)&e)->duplicate();
    }
    else if(e.getType() == XMLUni::fgMalformedURLException_Name)
    {
        return (XMLException*)((MalformedURLException*)&e)->duplicate();
    }
    else if(e.getType() == XMLUni::fgXSerializationException_Name)
    {
        return (XMLException*)((XSerializationException*)&e)->duplicate();
    }
    else if(e.getType() == XMLUni::fgInvalidDatatypeFacetException_Name)
    {
        return (XMLException*)((InvalidDatatypeFacetException*)&e)->duplicate();
    }
    else if(e.getType() == XMLUni::fgInvalidDatatypeValueException_Name)
    {
        return (XMLException*)((InvalidDatatypeValueException*)&e)->duplicate();
    }
    else if(e.getType() == XMLUni::fgXPathException_Name)
    {
        return (XMLException*)((XPathException*)&e)->duplicate();
    }
    else
    {
        fprintf(stderr,"Unknown Exception type: %d", e.getType());
    }
}
