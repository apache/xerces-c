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

#ifndef __PERLDOMCALLBACKHANDLER
#define __PERLDOMCALLBACKHANDLER

#include "PerlCallbackHandler.hpp"
#include "xercesc/dom/DOMErrorHandler.hpp"
#include "xercesc/dom/DOMLSResourceResolver.hpp"

XERCES_CPP_NAMESPACE_BEGIN

class DOMLSInput;

class PerlDOMCallbackHandler : public DOMErrorHandler
			       , public DOMLSResourceResolver
			       , public PerlCallbackHandler 
{

protected:

public:

    PerlDOMCallbackHandler();
    PerlDOMCallbackHandler(SV *obj);
    ~PerlDOMCallbackHandler();

    int type() {return PERLCALLBACKHANDLER_DOMERROR_TYPE;}

	// the DOMErrorHandler interface
    bool handleError(const DOMError& domError);

	// The DOMLSResourceResolver interface
    DOMLSInput* resolveResource(  const XMLCh* const
				 , const XMLCh* const
				 , const XMLCh* const
				 , const XMLCh* const
				 , const XMLCh* const);
};

XERCES_CPP_NAMESPACE_END

#endif /* __PERLDOMCALLBACKHANDLER */
