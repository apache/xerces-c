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

#ifndef __PERLERRORCALLBACKHANDLER
#define __PERLERRORCALLBACKHANDLER

#include "PerlCallbackHandler.hpp"
#include "xercesc/sax/ErrorHandler.hpp"

XERCES_CPP_NAMESPACE_USE

class PerlErrorCallbackHandler : public ErrorHandler
			       , public PerlCallbackHandler 
{

protected:

public:

    PerlErrorCallbackHandler();
    PerlErrorCallbackHandler(SV *obj);
    ~PerlErrorCallbackHandler();

    int type() {return PERLCALLBACKHANDLER_ERROR_TYPE;}

	// the ErrorHandler interface
    void warning(const SAXParseException& exception);
    void error(const SAXParseException& exception);
    void fatalError(const SAXParseException& exception);
    void resetErrors();
};

#endif /* __PERLERRORCALLBACKHANDLER */
