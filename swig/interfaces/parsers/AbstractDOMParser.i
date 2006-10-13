%ignore XERCES_CPP_NAMESPACE::AbstractDOMParser::parseFirst(
        const   char* const
        ,       XMLPScanToken&);
%ignore XERCES_CPP_NAMESPACE::AbstractDOMParser::setExternalSchemaLocation(const char* const);
%ignore XERCES_CPP_NAMESPACE::AbstractDOMParser::setExternalNoNamespaceSchemaLocation(const char* const);
%ignore XERCES_CPP_NAMESPACE::AbstractDOMParser::parse(const char* const);

%include "xercesc/parsers/AbstractDOMParser.hpp"