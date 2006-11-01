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

// in typemap
%typemap(in) XMLCh * %{
    // we convert *everything* into a string *including* undef
    $1 = UTF8_TRANSCODER->Local2XMLString($input);
%}

%typemap(freearg) XMLCh * %{
  delete[] $1;
%}

%exception XERCES_CPP_NAMESPACE::DOMLSInput::~DOMLSInput %{
    try 
    {
	XMLCh *current = (XMLCh *)(arg1)->getStringData();
	if (current != NULL) {
	  delete [] current;
	}
        $action
    } 
    CATCH_DOM_EXCEPTION
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
    CATCH_DOM_EXCEPTION
%}

%extend XERCES_CPP_NAMESPACE::DOMLSInput {
%typemap(freearg) const XMLCh* data ""
}

// out typemap
%typemap(out) XMLCh * %{
  $result = UTF8_TRANSCODER->XMLString2Local($1);
  ++argvi;
%}

// %typemap(typecheck, precedence=70)
// we must set a *really* low precedence on this because it will return
// true for *everything* 
%typecheck(70)
XMLCh*, const XMLCh* 
{
  $1 = 1;
}

// for XMLSchemaDescription
%apply XMLCh* { uint16_t* };

/*
 * Enable conversion of void* => XMLCh* in setProperty()
 *
 * The in typemap converts the void* to an XMLCh*
 *
 * The freearg typemap deletes the transcoded string
 *
 */
%typemap(in) (void* value) {
    // we convert *everything* into a string *including* undef
    $1 = UTF8_TRANSCODER->Local2XMLString($input);
}
%typemap(freearg) void * %{
  delete[] $1;
%}

