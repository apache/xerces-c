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

/***********/
/*         */
/* XMLCh * */
/*         */
/***********/

/************************************************************************/
/*                                                                      */
/* FOR FUNCTIONS TAKING XMLCh * (I.E AN XMLCh STRING) AS AN ARGUMENT -- */
/* NOW YOU CAN JUST SUPPLY A STRING.  THIS TYPEMAP CONVERTS             */
/* PERL-STRINGS TO XMLCh STRINGS AUTOMATICALLY                          */
/*                                                                      */
/************************************************************************/

/************************************************************************/
/*                                                                      */
/* CAVEAT:                                                              */
/* TO CONVERT STRINGS TO XMLCh STRINGS, A TEMPORARY POINTER MUST BE     */
/* CREATED IN THE in TYPEMAP TO POINT TO MEMORY THAT HOLDS THE          */
/* CONVERSION.  THE MEMORY IS DYNAMIC, SO IT MUST BE FREED AFTER THE C  */
/* FUNCTION THAT USES IT IS CALLED.  THIS IS DONE VIA A "freearg"       */
/* TYPEMAP.                                                             */
/*                                                                      */
/************************************************************************/

// in typemap
%typemap(in) XMLCh * %{
  if ($input == &PL_sv_undef) {
    SWIG_Perl_NullRef("perl-string",$argnum,"$symname");
    goto fail;
  } else {
    // we convert *everything* into a string that isn't undef
    $1 = Perl2XMLString($input);
  }
%}

%typemap(freearg) XMLCh * %{
  delete[] $1;
%}

// out typemap
%typemap(out) XMLCh * %{
  $result = XMLString2Perl($1);
  ++argvi;
%}

// varout typemap (for global variables)
%typemap(varout) XMLCh[] %{
  sv_setsv((SV*)$result, XMLString2Perl($1));
%}

