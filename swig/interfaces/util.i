// we remove this macro for PlatformUtils and XMLURL
#define MakeXMLException(theType, expKeyword)

%include "util/PlatformUtils.i"

%import "xercesc/util/XMemory.hpp"

%include "xercesc/util/XMLURL.hpp"

#define DECL_XSERIALIZABLE(XMLUri)
%include "xercesc/util/XMLUri.hpp"

// this macro will get redefined and SWIG 1.3 thinks that's an error
#undef MakeXMLException
%include "xercesc/util/XMLExceptMsgs.hpp"
%include "xercesc/util/XMLException.hpp"

%include "xercesc/util/QName.hpp"

%include "xercesc/util/SecurityManager.hpp"
