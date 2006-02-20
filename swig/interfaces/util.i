// we remove this macro for PlatformUtils and XMLURL
#define MakeXMLException(theType, expKeyword)

%include "xercesc/util/PlatformUtils.hpp"
%import "xercesc/util/XMemory.hpp"
%include "xercesc/util/XMLURL.hpp"

#define DECL_XSERIALIZABLE(XMLUri)
%include "xercesc/util/XMLUri.hpp"

// this macro will get redefined and SWIG 1.3 thinks that's an error
#undef MakeXMLException
%include "xercesc/util/XMLExceptMsgs.hpp"
%include "xercesc/util/XMLException.hpp"

// Unicode string constants for XML Formatter
%include "xercesc/util/XMLUni.hpp"

%include "xercesc/util/QName.hpp"

