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
 * The interface files that are common to all the Xerces top-level modules
 */

%include "includes.i"

/*
 * Platform and compiler specific items
 */

%include "platform.i"

/*
 * Import the language specific macros - not namespace dependent
 *   we place these first so that they can define any master
 *   macros needed by the language-independent interface files
 */

#ifdef SWIGPERL
%include "Perl/shadow.i"
%include "Perl/ignore.i"
%include "Perl/errors.i"
%include "Perl/defines.i"
%include "Perl/includes.i"
#endif

/* 
 * External objects
 */
%include "transcoder.i"
%include "make-xml-exception.i"

/* 
 * Common typemaps
 */
%include "typemaps.i"

/* 
 * Include extra verbatim C code in the initialization function
 */
%include "init.i"

/* 
 * The general %ignore directives
 */

%include "ignore.i"

/*
 * Operator support
 */

%include "operator.i"

