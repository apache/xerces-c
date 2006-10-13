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
 * includes.i - all the #includes needed for Xerces.cpp
 */

%{
#include "xercesc/sax/SAXException.hpp"
#include "xercesc/sax/SAXParseException.hpp"
#include "xercesc/sax/EntityResolver.hpp"

#include "xercesc/util/PlatformUtils.hpp"
#include "xercesc/util/XMLString.hpp"
#include "xercesc/util/XMLUri.hpp"
#include "xercesc/util/QName.hpp"
#include "xercesc/util/HexBin.hpp"
#include "xercesc/util/Base64.hpp"
#include "xercesc/util/NameIdPool.hpp"
#include "xercesc/util/XMLEnumerator.hpp"
#include "xercesc/util/SecurityManager.hpp"
#include "xercesc/util/XMLException.hpp"
#include "xercesc/util/XMLEntityResolver.hpp"

#include "xercesc/framework/XMLValidator.hpp"
#include "xercesc/framework/XMLDocumentHandler.hpp"
#include "xercesc/framework/XMLPScanToken.hpp"

#include "xercesc/framework/psvi/PSVIHandler.hpp"
#include "xercesc/framework/psvi/PSVIItem.hpp"
#include "xercesc/framework/psvi/PSVIElement.hpp"
#include "xercesc/framework/psvi/PSVIAttribute.hpp"
#include "xercesc/framework/psvi/PSVIAttributeList.hpp"

#include "xercesc/framework/XMLGrammarDescription.hpp"
#include "xercesc/framework/XMLDTDDescription.hpp"
#include "xercesc/framework/XMLSchemaDescription.hpp"

#include "xercesc/validators/common/Grammar.hpp"
#include "xercesc/validators/DTD/DTDAttDef.hpp"
#include "xercesc/validators/DTD/DTDAttDefList.hpp"
#include "xercesc/validators/DTD/DTDGrammar.hpp"
#include "xercesc/validators/DTD/DTDValidator.hpp"
#include "xercesc/validators/schema/SchemaGrammar.hpp"
#include "xercesc/validators/schema/SchemaValidator.hpp"
#include "xercesc/validators/schema/SchemaAttDefList.hpp"
#include "xercesc/validators/schema/SchemaAttDef.hpp"

#include "PSVIWriter/PSVIUni.hpp"
%}

