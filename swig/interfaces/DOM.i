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
 * All DOM classes and their related SWIG alterations go in this interface file
 */

/*
 * the DOM classes gets a special exception handler
 *    'goto fail' must be called - either explicitly, or via SWIG_croak()
 *    to ensure that any variable cleanup is done - to avoid memory leaks.
 *    By making these macros, it reduces the code size dramatically
 */
%{
#define CATCH_DOM_EXCEPTION         \
    catch (const XMLException& e)   \
    {                               \
        makeXMLException(e);        \
	goto fail;                  \
    }                               \
    catch (const DOMException& e)   \
    {                               \
	makeDOMException(e);        \
	goto fail;                  \
    }                               \
    catch (...)                     \
    {                               \
        SWIG_croak("Handling Unknown exception"); \
        goto fail;                  \
    }
%}

%exception {
    try 
    {
        $action
    } 
    CATCH_DOM_EXCEPTION
}

// Introduced in DOM Level 1
%include "xercesc/dom/DOMException.hpp"
%include "xercesc/dom/DOMNode.hpp"
%include "xercesc/dom/DOMAttr.hpp"
%include "xercesc/dom/DOMElement.hpp"
%include "xercesc/dom/DOMEntity.hpp"
%include "xercesc/dom/DOMDocumentType.hpp"
%include "xercesc/dom/DOMCharacterData.hpp"
%include "xercesc/dom/DOMComment.hpp"
%include "xercesc/dom/DOMText.hpp"
%include "xercesc/dom/DOMCDATASection.hpp"
%include "xercesc/dom/DOMNodeList.hpp"
%include "xercesc/dom/DOMNamedNodeMap.hpp"
%include "xercesc/dom/DOMDocumentFragment.hpp"
%include "xercesc/dom/DOMEntityReference.hpp"
%include "xercesc/dom/DOMNotation.hpp"
%include "xercesc/dom/DOMProcessingInstruction.hpp"

// Introduced in DOM Level 2
%include "xercesc/dom/DOMDocumentRange.hpp"
%include "xercesc/dom/DOMDocumentTraversal.hpp"
%include "xercesc/dom/DOMNodeIterator.hpp"
%include "xercesc/dom/DOMNodeFilter.hpp"
%include "xercesc/dom/DOMRange.hpp"
%include "xercesc/dom/DOMRangeException.hpp"
%include "xercesc/dom/DOMTreeWalker.hpp"
%include "xercesc/dom/DOMDocumentTraversal.hpp"

/*
 * Introduced in DOM Level 3
 */
%include "xercesc/dom/DOMConfiguration.hpp"
%include "xercesc/dom/DOMImplementationLS.hpp"
%include "xercesc/dom/DOMImplementation.hpp"
%include "xercesc/dom/DOMImplementationSource.hpp"
%include "xercesc/dom/DOMImplementationRegistry.hpp"

%include "xercesc/dom/DOMErrorHandler.hpp"
%include "xercesc/dom/DOMDocument.hpp"
%include "xercesc/dom/DOMLocator.hpp"

%include "xercesc/dom/DOMLSResourceResolver.hpp"

// DOMLSParser
%include "xercesc/dom/DOMLSInput.hpp"
%include "xercesc/framework/Wrapper4InputSource.hpp"
%include "xercesc/framework/Wrapper4DOMLSInput.hpp"
%import  "xercesc/dom/DOMLSParserFilter.hpp"
%include "xercesc/dom/DOMLSParser.hpp"

// DOMWriter
%include "xercesc/dom/DOMLSOutput.hpp"
%import "xercesc/framework/XMLFormatter.hpp"
%include "xercesc/framework/StdOutFormatTarget.hpp"
%include "xercesc/framework/LocalFileFormatTarget.hpp"
%include "xercesc/framework/MemBufFormatTarget.hpp"

%include "xercesc/dom/DOMLSSerializer.hpp"
%include "xercesc/dom/DOMLSSerializerFilter.hpp"

%extend XERCES_CPP_NAMESPACE::DOMNode {
   bool operator==(const DOMNode *other) {
       return self->isSameNode(other);
   }
   bool operator!=(const DOMNode *other) {
       return !self->isSameNode(other);
   }
};

