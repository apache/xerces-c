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
 * Perl/defines.i - includes all code included verbatim in Xerces.cpp
 *
 */ 

%{

#include "PerlCallbackHandler.hpp"
#include "PerlErrorCallbackHandler.hpp"
#include "PerlDocumentCallbackHandler.hpp"
#include "PerlContentCallbackHandler.hpp"
#include "PerlEntityResolverHandler.i"
#include "PerlNodeFilterCallbackHandler.i"

static void
SWIG_Disown(SV *sv) {
  
  if (SvGMAGICAL(sv)) {
      mg_get(sv);
  }
    

  /* Check to see if this is an object */
  if (sv_isobject(sv)) {
    SV *tsv = (SV*) SvRV(sv);
    if ((SvTYPE(tsv) == SVt_PVHV)) {
      MAGIC *mg;
      if (SvMAGICAL(tsv)) {
        mg = mg_find(tsv,'P');
        if (mg) {
          sv = mg->mg_obj;
          if (sv_isobject(sv)) {
              fprintf(stderr,"Found object for disown\n");

	      HV *stash;
	      stash=SvSTASH(SvRV(sv));

	      if (stash != 0)
              {
                  fprintf(stderr,"Found stash: %x\n",stash);
                  HV *hv;
                  GV *gv=*(GV**)hv_fetch(stash, "OWNER", 5, TRUE);
                  if (isGV(gv))
                  {
                      fprintf(stderr,"Found OWNER glob: %x\n",gv);
                      fprintf(stderr,"Found object: %x\n",sv);
                      fprintf(stderr,"Found ref object: %x\n",SvRV(sv));
                      hv=GvHVn(gv);

                      HE *entry;
                      char *key;
                      I32 klen;
                      SV *hval;
	    
                      (void)hv_iterinit(hv);
                      while ((entry = hv_iternext(hv)))  {
                          key = hv_iterkey(entry, &klen);
                          fprintf(stderr,"Found hash entry: %s\n",key);
                      }
            
		      if (hv_exists_ent(hv, sv, 0))
                      {
                          fprintf(stderr,"Found key in OWNER hash!\n");
                          if (hv_delete_ent(hv,sv,0,0))
                          {
                              fprintf(stderr,"Deleted key in OWNER hash!\n");
                          }
                          else 
                          {
                              SWIG_SetError("Couldn't delete key from OWNER hash :-<\n");
                          }
                      }
                      else
                      {
                          SWIG_SetError("no key in OWNER hash :-<\n");
                      }
                  }
                  else
                  {
                      SWIG_SetError("no GV\n");
                  }
              }
              else
              {
                  SWIG_SetError("no stash found\n");
              }
          } else {
              SWIG_SetError("SWIG_Disown: mg is not object");
          }
        } else {
            SWIG_SetError("SWIG_Disown: no mg");
        }
      } else {
          SWIG_SetError("SWIG_Disown: not magical");
      }
    } else {
      SWIG_SetError("SWIG_Disown: not PVHV");
    }
  } else {            /* not an object */
      SWIG_SetError("SWIG_Disown: not an object");
  }
}

%}
