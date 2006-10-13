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

#ifndef __PERLSAXCALLBACKHANDLER
#define __PERLSAXCALLBACKHANDLER

#include "PerlCallbackHandler.hpp"
#include "xercesc/sax/DocumentHandler.hpp"
#include "xercesc/sax2/ContentHandler.hpp"
#include "xercesc/util/XMLString.hpp"

XERCES_CPP_NAMESPACE_BEGIN

class PerlSAXCallbackHandler  : public ContentHandler
			      , public DocumentHandler
			      , public PerlCallbackHandler 
{

protected:

public:

    PerlSAXCallbackHandler();
    PerlSAXCallbackHandler(SV *obj);
    ~PerlSAXCallbackHandler();

    int type() {return PERLCALLBACKHANDLER_SAX_TYPE;}

	// The DocumentHandler interface
    void characters(const XMLCh* const chars, 
		    const unsigned int length);
    void ignorableWhitespace(const XMLCh* const chars, 
			     const unsigned int length);
    void startElement(const XMLCh* const, AttributeList&);
    void endElement(const XMLCh* const);

    void resetDocument();
    void startDocument();
    void endDocument();
    void processingInstruction (const XMLCh* const target,
				const XMLCh* const data);
    void setDocumentLocator(const Locator* const locator);

	// The ContentHandler interface
    void startElement(const   XMLCh* const    uri,
		      const   XMLCh* const    localname,
		      const   XMLCh* const    qname,
		      const   Attributes&     attrs);
    void endElement(const   XMLCh* const    uri,
		    const   XMLCh* const    localname,
		    const   XMLCh* const    qname);
    void startPrefixMapping (const XMLCh* const prefix,
			     const XMLCh* const uri);
    void endPrefixMapping (const XMLCh* const prefix);
    void skippedEntity (const XMLCh* const name);
};

XERCES_CPP_NAMESPACE_END

#endif /* __PERLSAXCALLBACKHANDLER */
