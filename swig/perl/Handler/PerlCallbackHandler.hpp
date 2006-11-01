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

#ifndef __PERLCALLBACKHANDLER
#define __PERLCALLBACKHANDLER


/* Needed on some windows machines---since MS plays funny
   games with the header files under C++ */
#include <math.h>
#include <stdlib.h>

extern "C" {

#include "EXTERN.h"
#include "perl.h"
#include "XSUB.h"

/* Get rid of free and malloc defined by perl */
#undef free
#undef malloc

#include <string.h>
}

#if !defined(PERL_REVISION) || ((PERL_REVISION >= 5) && ((PERL_VERSION < 5) || ((PERL_VERSION == 5) && (PERL_SUBVERSION < 50))))
#ifndef PL_sv_yes
#define PL_sv_yes PL_sv_yes
#endif
#ifndef PL_sv_undef
#define PL_sv_undef PL_sv_undef
#endif
#ifndef PL_na
#define PL_na PL_na
#endif
#endif

#define PERLCALLBACKHANDLER_BASE_TYPE         -1
#define PERLCALLBACKHANDLER_ERROR_TYPE         0
#define PERLCALLBACKHANDLER_ENTITY_TYPE        1
#define PERLCALLBACKHANDLER_NODE_TYPE          2
#define PERLCALLBACKHANDLER_SAX_TYPE           3
#define PERLCALLBACKHANDLER_DOMERROR_TYPE      4

#include "xercesc/util/PlatformUtils.hpp"
#include "../Transcoder/Transcoder.hpp"

XERCES_CPP_NAMESPACE_BEGIN

class PerlCallbackHandler {

private:

    PerlCallbackHandler(PerlCallbackHandler*);

protected:

  SV *callbackObj;
  Transcoder* UTF8_TRANSCODER;

public:

    PerlCallbackHandler();
    PerlCallbackHandler(SV*);
    virtual ~PerlCallbackHandler();
    virtual int type() {return PERLCALLBACKHANDLER_BASE_TYPE;}

    SV* set_callback_obj(SV*);
};

XERCES_CPP_NAMESPACE_END

#endif /* __PERLCALLBACKHANDLER */
