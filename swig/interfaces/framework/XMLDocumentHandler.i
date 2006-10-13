%import "xercesc/util/BaseRefVectorOf.hpp";
%import "xercesc/util/RefVectorOf.hpp";
namespace XERCES_CPP_NAMESPACE {
  %template(BaseXMLAttrVector)  BaseRefVectorOf<XMLAttr>;
  %template(XMLAttrVector)  RefVectorOf<XMLAttr>;
}
%include "framework/XMLAttr.i"
// advanced document handler interface
%include "xercesc/framework/XMLDocumentHandler.hpp"

// Now, we ignore these methods (without class names)!!!
// Many of the parser implement these methods, and they are not
// useful as part of the parser API - i.e. an application will not
// invoke these methods directly on the parser object
%ignore docCharacters;
%ignore docComment;
%ignore docPI;
%ignore endDocument;
%ignore endElement;
%ignore endEntityReference;
%ignore ignorableWhitespace;
%ignore resetDocument;
%ignore startDocument;
%ignore startElement;
%ignore startEntityReference;
%ignore XMLDecl;
