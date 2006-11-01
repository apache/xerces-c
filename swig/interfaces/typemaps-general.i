/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 * 
 *      http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// this is new for Xerces-C-3.0
%apply size_t { ssize_t };

/*
 * Scripting languages are not going to change the default memory manager
 *   so we always default this argument
 */
%typemap(default,numinputs=0) XERCES_CPP_NAMESPACE::MemoryManager* const  manager "$1 = XERCES_CPP_NAMESPACE::XMLPlatformUtils::fgMemoryManager;"

/*
 * Dynamic Casts
 * 
 *   This very cool SWIG feature enables use to return the correct object
 *   type to the scripting language when a C++ method is defined as
 *   returning a base class object. We define a method that tells what
 *   type subclass the object is in.
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

%typemap(out) XERCES_CPP_NAMESPACE::XMLGrammarDescription * = SWIGTYPE *DYNAMIC;

DYNAMIC_CAST(SWIGTYPE_p_XERCES_CPP_NAMESPACE__XMLGrammarDescription, XMLGrammarDescription_dynamic_cast);

%{
static swig_type_info *
XMLGrammarDescription_dynamic_cast(void **ptr) {
   XMLGrammarDescription **nptr = (XMLGrammarDescription **) ptr;
   if (*nptr == NULL) {
       return NULL;
   }
   short int type = (*nptr)->getGrammarType();
   if (type == Grammar::DTDGrammarType) {
      return SWIGTYPE_p_XERCES_CPP_NAMESPACE__XMLDTDDescription;
   }
   if (type == Grammar::SchemaGrammarType) {
      return SWIGTYPE_p_XERCES_CPP_NAMESPACE__XMLSchemaDescription;
   }
   return NULL;
}
%}

