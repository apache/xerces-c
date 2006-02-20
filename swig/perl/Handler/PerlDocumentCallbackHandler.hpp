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

#ifndef __PERLDOCUMENTCALLBACKHANDLER
#define __PERLDOCUMENTCALLBACKHANDLER

#include "PerlCallbackHandler.hpp"
#include "xercesc/sax/DocumentHandler.hpp"
#include "xercesc/util/XMLString.hpp"

XERCES_CPP_NAMESPACE_USE


class PerlDocumentCallbackHandler : public DocumentHandler
				  , public PerlCallbackHandler 
{

protected:

public:

    PerlDocumentCallbackHandler();
    PerlDocumentCallbackHandler(SV *obj);
    ~PerlDocumentCallbackHandler();

    int type() {return PERLCALLBACKHANDLER_DOCUMENT_TYPE;}

	// The DocumentHandler interface
    void startElement(const XMLCh* const name, 
		      AttributeList& attributes);
    void characters(const XMLCh* const chars, 
		       const unsigned int length);
    void ignorableWhitespace(const XMLCh* const chars, 
				const unsigned int length);
    void endElement(const XMLCh* const name);
    void resetDocument();
    void startDocument();
    void endDocument();
    void processingInstruction (const XMLCh* const target,
				const XMLCh* const data);
    void setDocumentLocator(const Locator* const locator);

};

#endif /*  __PERLDOCUMENTCALLBACKHANDLER */
