/*
 * DOM_Node*
 */

%typemap(out) XERCES_CPP_NAMESPACE::DOMNode * = SWIGTYPE *DYNAMIC;

DYNAMIC_CAST(SWIGTYPE_p_XERCES_CPP_NAMESPACE__DOMNode, DOMNode_dynamic_cast);

%{
static swig_type_info *
DOMNode_dynamic_cast(void **ptr) {
   DOMNode **nptr = (DOMNode **) ptr;
   if (*nptr == NULL) {
       return NULL;
   }
   short int type = (*nptr)->getNodeType();
   if (type == DOMNode::TEXT_NODE) {
      return SWIGTYPE_p_XERCES_CPP_NAMESPACE__DOMText;
   }
   if (type == DOMNode::PROCESSING_INSTRUCTION_NODE) {
      return SWIGTYPE_p_XERCES_CPP_NAMESPACE__DOMProcessingInstruction;
   }
   if (type == DOMNode::DOCUMENT_NODE) {
      return SWIGTYPE_p_XERCES_CPP_NAMESPACE__DOMDocument;
   }
   if (type == DOMNode::ELEMENT_NODE) {
      return SWIGTYPE_p_XERCES_CPP_NAMESPACE__DOMElement;
   }
   if (type == DOMNode::ENTITY_REFERENCE_NODE) {
      return SWIGTYPE_p_XERCES_CPP_NAMESPACE__DOMEntityReference;
   }
   if (type == DOMNode::CDATA_SECTION_NODE) {
      return SWIGTYPE_p_XERCES_CPP_NAMESPACE__DOMCDATASection;
   }
   if (type == DOMNode::CDATA_SECTION_NODE) {
      return SWIGTYPE_p_XERCES_CPP_NAMESPACE__DOMCDATASection;
   }
   if (type == DOMNode::COMMENT_NODE) {
      return SWIGTYPE_p_XERCES_CPP_NAMESPACE__DOMComment;
   }
   if (type == DOMNode::DOCUMENT_TYPE_NODE) {
      return SWIGTYPE_p_XERCES_CPP_NAMESPACE__DOMDocumentType;
   }
   if (type == DOMNode::ENTITY_NODE) {
      return SWIGTYPE_p_XERCES_CPP_NAMESPACE__DOMEntity;
   }
   if (type == DOMNode::ATTRIBUTE_NODE) {
      return SWIGTYPE_p_XERCES_CPP_NAMESPACE__DOMAttr;
   }
   if (type == DOMNode::NOTATION_NODE) {
      return SWIGTYPE_p_XERCES_CPP_NAMESPACE__DOMNotation;
   }
   return NULL;
}
%}
