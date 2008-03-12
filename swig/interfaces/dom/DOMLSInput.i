%exception XERCES_CPP_NAMESPACE::DOMLSInput::~DOMLSInput %{
    try 
    {
	XMLCh *current = (XMLCh *)(arg1)->getStringData();
	if (current != NULL) {
	  delete [] current;
	}
        $action
    } 
    CATCH_XML_EXCEPTION
%}

%exception XERCES_CPP_NAMESPACE::DOMLSInput::setStringData %{
    try 
    {
	XMLCh *current = (XMLCh *)(arg1)->getStringData();
	if (current != NULL) {
	  delete [] current;
	}
        $action
    } 
    CATCH_XML_EXCEPTION
%}

%extend XERCES_CPP_NAMESPACE::DOMLSInput {
%typemap(freearg) const XMLCh* data ""
}

%include "xercesc/dom/DOMLSInput.hpp"