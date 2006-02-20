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

/*
 * defines.i - This file contains all directives included
 *   verbatim into the C++ output file
 */

%{
#include "xercesc/sax/InputSource.hpp"
#include "xercesc/sax/SAXException.hpp"
#include "xercesc/sax/SAXParseException.hpp"
#include "xercesc/sax/Locator.hpp"
#include "xercesc/sax/HandlerBase.hpp"
#include "xercesc/sax2/Attributes.hpp"
#include "xercesc/sax2/ContentHandler.hpp"
#include "xercesc/sax2/LexicalHandler.hpp"
#include "xercesc/sax2/DefaultHandler.hpp"
#include "xercesc/sax2/SAX2XMLReader.hpp"
#include "xercesc/sax2/XMLReaderFactory.hpp"
#include "xercesc/util/PlatformUtils.hpp"
#include "xercesc/util/TransService.hpp"
#include "xercesc/util/XMLString.hpp"
#include "xercesc/util/XMLUri.hpp"
#include "xercesc/util/QName.hpp"
#include "xercesc/util/HexBin.hpp"
#include "xercesc/util/Base64.hpp"
#include "xercesc/parsers/AbstractDOMParser.hpp"
#include "xercesc/parsers/XercesDOMParser.hpp"
#include "xercesc/parsers/SAXParser.hpp"
#include "xercesc/dom/DOM.hpp"
#include "xercesc/framework/LocalFileInputSource.hpp"
#include "xercesc/framework/MemBufInputSource.hpp"
#include "xercesc/framework/StdInInputSource.hpp"
#include "xercesc/framework/URLInputSource.hpp"
#include "xercesc/framework/XMLGrammarDescription.hpp"
#include "xercesc/framework/XMLDTDDescription.hpp"
#include "xercesc/framework/XMLSchemaDescription.hpp"
#include "xercesc/util/NameIdPool.hpp"
#include "xercesc/util/XMLEnumerator.hpp"
#include "xercesc/framework/XMLValidator.hpp"
#include "xercesc/validators/common/Grammar.hpp"
#include "xercesc/validators/DTD/DTDAttDef.hpp"
#include "xercesc/validators/DTD/DTDAttDefList.hpp"
#include "xercesc/validators/DTD/DTDGrammar.hpp"
#include "xercesc/validators/DTD/DTDValidator.hpp"
#include "xercesc/validators/schema/SchemaGrammar.hpp"
#include "xercesc/validators/schema/SchemaValidator.hpp"
#include "xercesc/validators/schema/SchemaAttDefList.hpp"
#include "xercesc/validators/schema/SchemaAttDef.hpp"
#include "xercesc/framework/XMLFormatter.hpp"
#include "xercesc/framework/MemBufFormatTarget.hpp"
#include "xercesc/framework/LocalFileFormatTarget.hpp"
#include "xercesc/framework/StdOutFormatTarget.hpp"
#include "xercesc/framework/Wrapper4InputSource.hpp"
#include "xercesc/framework/Wrapper4DOMLSInput.hpp"
#include "xercesc/framework/psvi/PSVIHandler.hpp"

// for resolving XMLExceptions
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

XERCES_CPP_NAMESPACE_USE

// we initialize the static UTF-8 transcoding info
// these are used by the typemaps to convert between
// Xerces internal UTF-16 and Perl's internal UTF-8
static XMLCh* UTF8_ENCODING = NULL; 
static XMLTranscoder* UTF8_TRANSCODER  = NULL;

static bool DEBUG_UTF8_OUT = 0;
static bool DEBUG_UTF8_IN = 0;

%}

/*
 * Import the language specific includes
 */

#ifdef SWIGPERL
%include "Perl/defines.i"
%include "Perl/transcode.i"
%include "Perl/errors.i"
#endif


// These get wrapped by SWIG so that we can modify them from the scripting language
bool DEBUG_UTF8_OUT;
bool DEBUG_UTF8_IN;

%{
XMLException*
copyXMLException(const XMLException& e)
{
    if(e.getType() == XERCES_CPP_NAMESPACE::XMLUni::fgArrayIndexOutOfBoundsException_Name)
    {
        return (XMLException*)((ArrayIndexOutOfBoundsException*)&e)->duplicate();
    }
    else if(e.getType() == XERCES_CPP_NAMESPACE::XMLUni::fgEmptyStackException_Name)
    {
        return (XMLException*)((EmptyStackException*)&e)->duplicate();
    }
    else if(e.getType() == XERCES_CPP_NAMESPACE::XMLUni::fgIllegalArgumentException_Name)
    {
        return (XMLException*)((IllegalArgumentException*)&e)->duplicate();
    }
    else if(e.getType() == XERCES_CPP_NAMESPACE::XMLUni::fgInvalidCastException_Name)
    {
        return (XMLException*)((InvalidCastException*)&e)->duplicate();
    }
    else if(e.getType() == XERCES_CPP_NAMESPACE::XMLUni::fgIOException_Name)
    {
        return (XMLException*)((IOException*)&e)->duplicate();
    }
    else if(e.getType() == XERCES_CPP_NAMESPACE::XMLUni::fgNoSuchElementException_Name)
    {
        return (XMLException*)((NoSuchElementException*)&e)->duplicate();
    }
    else if(e.getType() == XERCES_CPP_NAMESPACE::XMLUni::fgNullPointerException_Name)
    {
        return (XMLException*)((NullPointerException*)&e)->duplicate();
    }
    else if(e.getType() == XERCES_CPP_NAMESPACE::XMLUni::fgNumberFormatException_Name)
    {
        return (XMLException*)((NumberFormatException*)&e)->duplicate();
    }
    else if(e.getType() == XERCES_CPP_NAMESPACE::XMLUni::fgParseException_Name)
    {
        return (XMLException*)((ParseException*)&e)->duplicate();
    }
    else if(e.getType() == XERCES_CPP_NAMESPACE::XMLUni::fgXMLPlatformUtilsException_Name)
    {
        return (XMLException*)((XMLPlatformUtilsException*)&e)->duplicate();
    }
    else if(e.getType() == XERCES_CPP_NAMESPACE::XMLUni::fgRuntimeException_Name)
    {
        return (XMLException*)((RuntimeException*)&e)->duplicate();
    }
    else if(e.getType() == XERCES_CPP_NAMESPACE::XMLUni::fgSchemaDateTimeException_Name)
    {
        return (XMLException*)((SchemaDateTimeException*)&e)->duplicate();
    }
    else if(e.getType() == XERCES_CPP_NAMESPACE::XMLUni::fgTranscodingException_Name)
    {
        return (XMLException*)((TranscodingException*)&e)->duplicate();
    }
    else if(e.getType() == XERCES_CPP_NAMESPACE::XMLUni::fgUnexpectedEOFException_Name)
    {
        return (XMLException*)((UnexpectedEOFException*)&e)->duplicate();
    }
    else if(e.getType() == XERCES_CPP_NAMESPACE::XMLUni::fgUnsupportedEncodingException_Name)
    {
        return (XMLException*)((UnsupportedEncodingException*)&e)->duplicate();
    }
    else if(e.getType() == XERCES_CPP_NAMESPACE::XMLUni::fgUTFDataFormatException_Name)
    {
        return (XMLException*)((UTFDataFormatException*)&e)->duplicate();
    }
    else if(e.getType() == XERCES_CPP_NAMESPACE::XMLUni::fgNetAccessorException_Name)
    {
        return (XMLException*)((NetAccessorException*)&e)->duplicate();
    }
    else if(e.getType() == XERCES_CPP_NAMESPACE::XMLUni::fgMalformedURLException_Name)
    {
        return (XMLException*)((MalformedURLException*)&e)->duplicate();
    }
    else if(e.getType() == XERCES_CPP_NAMESPACE::XMLUni::fgXSerializationException_Name)
    {
        return (XMLException*)((XSerializationException*)&e)->duplicate();
    }
    else if(e.getType() == XERCES_CPP_NAMESPACE::XMLUni::fgInvalidDatatypeFacetException_Name)
    {
        return (XMLException*)((InvalidDatatypeFacetException*)&e)->duplicate();
    }
    else if(e.getType() == XERCES_CPP_NAMESPACE::XMLUni::fgInvalidDatatypeValueException_Name)
    {
        return (XMLException*)((InvalidDatatypeValueException*)&e)->duplicate();
    }
    else if(e.getType() == XERCES_CPP_NAMESPACE::XMLUni::fgXPathException_Name)
    {
        return (XMLException*)((XPathException*)&e)->duplicate();
    }
    else
    {
        SWIG_Perl_SetErrorf("Unknown Exception type: %d", e.getType());
    }
}

void
makeXMLException(const XMLException& e){
    SV *error = ERRSV;
    SWIG_MakePtr(error, (void *) copyXMLException(e), SWIGTYPE_p_XERCES_CPP_NAMESPACE__XMLException, SWIG_SHADOW|0);
}

void
makeDOMException(const DOMException& e){
    SV *error = ERRSV;
    SWIG_MakePtr(error, (void *) new DOMException(e), SWIGTYPE_p_XERCES_CPP_NAMESPACE__DOMException, SWIG_SHADOW|0);
}

void
makeSAXNotRecognizedException(const SAXNotRecognizedException& e){
    SV *error = ERRSV;
    SWIG_MakePtr(error, (void *) new SAXNotRecognizedException(e), SWIGTYPE_p_XERCES_CPP_NAMESPACE__SAXNotRecognizedException, SWIG_SHADOW|0);
}

void
makeSAXNotSupportedException(const SAXNotSupportedException& e){
    SV *error = ERRSV;
    SWIG_MakePtr(error, (void *) new SAXNotSupportedException(e), SWIGTYPE_p_XERCES_CPP_NAMESPACE__SAXNotSupportedException, SWIG_SHADOW|0);
}

%}
