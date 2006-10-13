// The API currently blocks these
%ignore XERCES_CPP_NAMESPACE::Wrapper4InputSource::setStringData;
%ignore XERCES_CPP_NAMESPACE::Wrapper4InputSource::setByteStream;

%include "xercesc/framework/Wrapper4InputSource.hpp"