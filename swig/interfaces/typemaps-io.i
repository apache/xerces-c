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

/*
 *  MemBufInputSource::MemBufInputSource()
 * 
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

%typemap(out) XERCES_CPP_NAMESPACE::InputSource * = SWIGTYPE *DYNAMIC;

DYNAMIC_CAST(SWIGTYPE_p_XERCES_CPP_NAMESPACE__InputSource, InputSource_dynamic_cast);

%{
static swig_type_info *
InputSource_dynamic_cast(void **ptr) {
   InputSource **nptr = (InputSource **) ptr;
   if (*nptr == NULL) {
       return NULL;
   }

   if (dynamic_cast< XERCES_CPP_NAMESPACE::Wrapper4DOMLSInput * >(*nptr)) {
      return SWIGTYPE_p_XERCES_CPP_NAMESPACE__Wrapper4DOMLSInput;
   }
   if (dynamic_cast< XERCES_CPP_NAMESPACE::MemBufInputSource * >(*nptr)) {
      return SWIGTYPE_p_XERCES_CPP_NAMESPACE__MemBufInputSource;
   }
   if (dynamic_cast< XERCES_CPP_NAMESPACE::LocalFileInputSource * >(*nptr)) {
      return SWIGTYPE_p_XERCES_CPP_NAMESPACE__LocalFileInputSource;
   }
   if (dynamic_cast< XERCES_CPP_NAMESPACE::URLInputSource * >(*nptr)) {
      return SWIGTYPE_p_XERCES_CPP_NAMESPACE__URLInputSource;
   }
   if (dynamic_cast< XERCES_CPP_NAMESPACE::StdInInputSource * >(*nptr)) {
      return SWIGTYPE_p_XERCES_CPP_NAMESPACE__StdInInputSource;
   }
   return NULL;
}
%}

