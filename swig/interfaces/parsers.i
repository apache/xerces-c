// scan token helper class for progressive parsing
%include "xercesc/framework/XMLPScanToken.hpp"
%include "xercesc/framework/psvi/PSVIHandler.hpp"

%include "xercesc/sax/Parser.hpp"
%include "xercesc/parsers/AbstractDOMParser.hpp"
%include "xercesc/parsers/XercesDOMParser.hpp"
%include "xercesc/parsers/SAXParser.hpp"

/*
 * the SAX2XMLReader methods gets a special exception handler
 *    'goto fail' must be called - either explicitly, or via SWIG_croak()
 *    to ensure that any variable cleanup is done - to avoid memory leaks.
 *    We make this a macro to be similar to the other exception handlers.
 */
%{
#define CATCH_SAX_EXCEPTION                           \
    catch (const XMLException& e)                     \
        {                                             \
	    makeXMLException(e);                      \
            goto fail;                                \
        }                                             \
    catch (const SAXNotSupportedException& e)         \
	{                                             \
	    makeSAXNotSupportedException(e);          \
            goto fail;                                \
	}                                             \
    catch (const SAXNotRecognizedException& e)        \
	{                                             \
	    makeSAXNotRecognizedException(e);         \
            goto fail;                                \
	}                                             \
    catch (...)                                       \
        {                                             \
            SWIG_croak("Handling Unknown exception"); \
            goto fail;                                \
        }
%}

%define SAXEXCEPTION(method)
%exception method {
    try {
        $action
    } 
    CATCH_SAX_EXCEPTION
}
%enddef

SAXEXCEPTION(XERCES_CPP_NAMESPACE::SAX2XMLReader::getFeature)
SAXEXCEPTION(XERCES_CPP_NAMESPACE::SAX2XMLReader::setFeature)
SAXEXCEPTION(XERCES_CPP_NAMESPACE::SAX2XMLReader::setProperty)
SAXEXCEPTION(XERCES_CPP_NAMESPACE::SAX2XMLReader::getProperty)

%include "xercesc/sax2/SAX2XMLReader.hpp"
%include "xercesc/sax2/XMLReaderFactory.hpp"

