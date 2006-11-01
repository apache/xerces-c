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

#ifndef __PERLNODEFILTERCALLBACKHANDLER
#define __PERLNODEFILTERCALLBACKHANDLER

#include "PerlCallbackHandler.hpp"
#include "xercesc/dom/DOMLSParserFilter.hpp"
#include "xercesc/dom/DOMLSSerializerFilter.hpp"

XERCES_CPP_NAMESPACE_BEGIN

class PerlNodeFilterCallbackHandler : public DOMLSSerializerFilter
				    , public DOMLSParserFilter
				    , public PerlCallbackHandler
{

protected:

public:

  PerlNodeFilterCallbackHandler();
  PerlNodeFilterCallbackHandler(SV *obj);
  ~PerlNodeFilterCallbackHandler();

  int type() {return PERLCALLBACKHANDLER_NODE_TYPE;}

  // The DOMNodeFilter interface - inherited via DOMLSSerializerFilter
  // short acceptNode (const DOMNode* node) const;


  // The DOMLSSerializerFilter interface
  short acceptNode (const DOMNode* node) const;
  unsigned long getWhatToShow() const;

  // The DOMLSParserFilter interface
  // unsigned long getWhatToShow() const;
  short acceptNode (DOMNode* node);
  short startElement(DOMElement* node);
};

XERCES_CPP_NAMESPACE_END

#endif /* __PERLNODEFILTERCALLBACKHANDLER */
