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

#ifndef __PERLCALLBACKHANDLER
#define __PERLCALLBACKHANDLER

#ifdef __cplusplus
/* Needed on some windows machines---since MS plays funny
   games with the header files under C++ */
#include <math.h>
#include <stdlib.h>
extern "C" {
#endif
#include "EXTERN.h"
#include "perl.h"
#include "XSUB.h"

/* Get rid of free and malloc defined by perl */
#undef free
#undef malloc

#include <string.h>
#ifdef __cplusplus
}
#endif

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
#define PERLCALLBACKHANDLER_CONTENT_TYPE       3
#define PERLCALLBACKHANDLER_DOCUMENT_TYPE      4

#include "xercesc/util/TransService.hpp"
#include "xercesc/util/XMLString.hpp"
#include "xercesc/util/PlatformUtils.hpp"

XERCES_CPP_NAMESPACE_USE

class PerlCallbackHandler {

private:

    PerlCallbackHandler(PerlCallbackHandler*);

protected:

    SV *callbackObj;

public:

    PerlCallbackHandler();
    PerlCallbackHandler(SV*);
    virtual ~PerlCallbackHandler();
    virtual int type() {return PERLCALLBACKHANDLER_BASE_TYPE;}

    SV* set_callback_obj(SV*);
    SV* XMLString2Perl(const XMLCh*);
};

#endif /* __PERLCALLBACKHANDLER */
