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
