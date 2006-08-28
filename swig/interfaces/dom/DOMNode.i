%include "xercesc/dom/DOMNode.hpp"

%extend XERCES_CPP_NAMESPACE::DOMNode {
   bool operator==(const DOMNode *other) {
       return self->isSameNode(other);
   }
   bool operator!=(const DOMNode *other) {
       return !self->isSameNode(other);
   }
};