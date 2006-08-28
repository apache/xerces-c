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
 *    By making these macros, it reduces the code *file* size dramatically
 *    (but doesn't reduce the compiled file size at all...)
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
%include "dom/DOMException.i"
%include "dom/DOMNode.i"
%include "dom/DOMAttr.i"
%include "dom/DOMElement.i"
%include "dom/DOMEntity.i"
%include "dom/DOMDocumentType.i"
%include "dom/DOMCharacterData.i"
%include "dom/DOMComment.i"
%include "dom/DOMText.i"
%include "dom/DOMCDATASection.i"
%include "dom/DOMNodeList.i"
%include "dom/DOMNamedNodeMap.i"
%include "dom/DOMDocumentFragment.i"
%include "dom/DOMEntityReference.i"
%include "dom/DOMNotation.i"
%include "dom/DOMProcessingInstruction.i"

// type info
%include "dom/DOMTypeInfo.i"
%include "dom/DOMPSVITypeInfo.i"

// XPath
%include "dom/DOMXPathEvaluator.i"
%include "dom/DOMXPathException.i"
%include "dom/DOMXPathExpression.i"
%include "dom/DOMXPathNamespace.i"
%include "dom/DOMXPathNSResolver.i"
%include "dom/DOMXPathResult.i"

// Introduced in DOM Level 2
%include "dom/DOMDocumentRange.i"
%include "dom/DOMDocumentTraversal.i"
%include "dom/DOMNodeIterator.i"
%include "dom/DOMNodeFilter.i"
%include "dom/DOMRange.i"
%include "dom/DOMRangeException.i"
%include "dom/DOMTreeWalker.i"

/*
 * Introduced in DOM Level 3
 */
%include "dom/DOMUserDataHandler.i"
%include "dom/DOMConfiguration.i"
%include "dom/DOMStringList.i"
%include "dom/DOMImplementationLS.i"
%include "dom/DOMImplementation.i"
%include "dom/DOMImplementationList.i"
%include "dom/DOMImplementationSource.i"
%include "dom/DOMImplementationRegistry.i"

%include "dom/DOMError.i"
%include "dom/DOMErrorHandler.i"
%include "dom/DOMDocument.i"
%include "dom/DOMLocator.i"
%include "dom/DOMLSResourceResolver.i"

// DOMLSParser
%include "dom/DOMLSInput.i"
%include "framework/Wrapper4InputSource.i"
%include "framework/Wrapper4DOMLSInput.i"
%import  "dom/DOMLSParserFilter.i"
%include "dom/DOMLSParser.i"

// DOMWriter
%include "dom/DOMLSOutput.i"
%include "framework/XMLFormatter.i"
%include "framework/StdOutFormatTarget.i"
%include "framework/LocalFileFormatTarget.i"
%include "framework/MemBufFormatTarget.i"

%include "dom/DOMLSException.i"
%include "dom/DOMLSSerializer.i"
%include "dom/DOMLSSerializerFilter.i"
