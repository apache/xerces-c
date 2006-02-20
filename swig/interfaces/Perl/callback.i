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
 * Perl/callback.i - an experimental typecheck for returning callback subclasses
 *
 */ 

/*
 * PerlCallbackHandler* - this is broken and useless
 */

%typemap(out) XERCES_CPP_NAMESPACE::PerlCallbackHandler * = SWIGTYPE *DYNAMIC;

DYNAMIC_CAST(SWIGTYPE_p_PerlCallbackHandler, PerlCallbackHandler_dynamic_cast);

%{

static swig_type_info *
PerlCallbackHandler_dynamic_cast(void **ptr) {
   PerlCallbackHandler **nptr = (PerlCallbackHandler **) ptr;
   if (*nptr == NULL) {
       return NULL;
   }
   short int type = (*nptr)->type();
   if (type == PERLCALLBACKHANDLER_BASE_TYPE) {
      die("Can't cast a PerlCallbackHandler base type node\n");
   }
   if (type == PERLCALLBACKHANDLER_ERROR_TYPE) {
      return SWIGTYPE_p_PerlErrorCallbackHandler;
   }
   if (type == PERLCALLBACKHANDLER_ENTITY_TYPE) {
      return SWIGTYPE_p_PerlEntityResolverHandler;
   }
   if (type == PERLCALLBACKHANDLER_CONTENT_TYPE) {
      return SWIGTYPE_p_PerlContentCallbackHandler;
   }
   if (type == PERLCALLBACKHANDLER_DOCUMENT_TYPE) {
      return SWIGTYPE_p_PerlDocumentCallbackHandler;
   }
   if (type == PERLCALLBACKHANDLER_NODE_TYPE) {
      return SWIGTYPE_p_PerlNodeFilterCallbackHandler;
   }
   return NULL;
}

%}