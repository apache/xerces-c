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
 * Scripting languages are not going to change the default memory manager
 *   so we always default this argument
 */
// %typemap(default,numinputs=0) XERCES_CPP_NAMESPACE::MemoryManager* const  manager "$1 = XERCES_CPP_NAMESPACE::XMLPlatformUtils::fgMemoryManager;"

/*
 *  MemBufInputSource::MemBufInputSource()
 * 
 * ALWAYS ADOPT BUFFER (I.E. MAKE A COPY OF IT) SINCE IT IS TAKEN FROM
 * THE SCRIPTING LANGUAGE, AND WHO KNOWS WHAT WILL HAPPEN TO IT AFTER
 * IT IS GIVEN TO THE
 * CONSTRUCTOR
 */

// %typemap(in,numinputs=0) (unsigned int byteCount) "$1 = 0;" 

/*
 * These arguments are used to indicate that Xerces-C should
 *    adopt a resource being passed as an argument. We should
 *    *always* tell Xerces-C to adopt.
 */
%typemap(in,numinputs=0) const bool adoptFlag "$1 = true;"   // for Wrapper4InputSource
                                                             // and Wrapper4DOMInputSource
%typemap(in,numinputs=0) const bool adoptBuffer "$1 = true;" // for MemBufInputSource

// SAX2XMLReader::setProperty() should refuse the option to set the
// security manager
%extend XERCES_CPP_NAMESPACE::SAX2XMLReader {
%typemap(check) (const XMLCh* const name, void* value) {
  if (XMLString::compareIStringASCII($1, XMLUni::fgXercesSecurityManager) == 0) {
    makeSAXNotSupportedException(SAXNotSupportedException("Setting security manageer not supported"));
    goto fail;
  }
}

/*
 * Enable conversion of void* => XMLCh* in setProperty()
 *
 * The in typemap converts the void* to an XMLCh*
 *
 * The freearg typemap deletes the transcoded string
 *
 */
%typemap(in) (void* value) {
  // now check the value
  if ($input == &PL_sv_undef) {
    SWIG_Perl_NullRef("perl-string",$argnum,"$symname");
    goto fail;
  } else {
    // we convert *everything* into a string that isn't undef
    $1 = Perl2XMLString($input);
  }
}
%typemap(freearg) void * %{
  delete[] $1;
%}

/*
 * Dynamic Casts
 * 
 *   This very cool SWIG feature enables use to return the correct object
 *   type to Perl when a C++ method is defined as returning a base class
 *   object. We define a method that tells what type subclass the object
 *   is in.
 */

/*
 * Grammar*
 */

%typemap(out) XERCES_CPP_NAMESPACE::Grammar * = SWIGTYPE *DYNAMIC;

DYNAMIC_CAST(SWIGTYPE_p_XERCES_CPP_NAMESPACE__Grammar, Grammar_dynamic_cast);

%{
static swig_type_info *
Grammar_dynamic_cast(void **ptr) {
   Grammar **nptr = (Grammar **) ptr;
   if (*nptr == NULL) {
       return NULL;
   }
   short int type = (*nptr)->getGrammarType();
   if (type == Grammar::DTDGrammarType) {
      return SWIGTYPE_p_XERCES_CPP_NAMESPACE__DTDGrammar;
   }
   if (type == Grammar::SchemaGrammarType) {
      return SWIGTYPE_p_XERCES_CPP_NAMESPACE__SchemaGrammar;
   }
   return NULL;
}
%}


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

