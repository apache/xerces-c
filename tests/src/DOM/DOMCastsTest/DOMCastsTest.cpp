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

// Verify DOM cast helpers, including checked fallback and rejection paths.
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/dom/impl/DOMCasts.hpp>
#include <iostream>
#include <stdexcept>
#include <vector>

using namespace xercesc;

static void require(bool condition) {
    if (!condition) throw std::runtime_error("DOM cast result differs from the checked interface");
}

// Check mutable and const helpers against the original interface cross-cast.
#define CHECK_CAST(Interface, Getter, Cast) \
    { \
        Interface* expected = dynamic_cast<Interface*>(node); \
        auto* member = expected ? expected->Getter() : nullptr; \
        try { require(Cast(node) == member && member); } \
        catch (const DOMException& e) { require(!member && e.code == DOMException::INVALID_STATE_ERR); } \
        const DOMNode* immutable = node; \
        const Interface* expected_const = dynamic_cast<const Interface*>(immutable); \
        const auto* const_member = expected_const ? expected_const->Getter() : nullptr; \
        try { require(Cast(immutable) == const_member && const_member); } \
        catch (const DOMException& e) { require(!const_member && e.code == DOMException::INVALID_STATE_ERR); } \
    }

static void verify(DOMNode* node) {
    CHECK_CAST(HasDOMNodeImpl, getNodeImpl, castToNodeImpl);
    CHECK_CAST(HasDOMParentImpl, getParentNodeImpl, castToParentImpl);
    CHECK_CAST(HasDOMChildImpl, getChildNodeImpl, castToChildImpl);
}

// A node type hint must not replace checking the actual implementation type.
class UnusualElement : public DOMElementImpl {
public:
    UnusualElement(DOMDocument* doc, const XMLCh* name) : DOMElementImpl(doc, name) {}
    NodeType getNodeType() const override { return DOMNode::TEXT_NODE; }
};

// Existing null-member rejection must also apply through the optimized path.
class MissingMemberElement : public DOMElementImpl {
public:
    MissingMemberElement(DOMDocument* doc, const XMLCh* name) : DOMElementImpl(doc, name) {}
    DOMNodeImpl* getNodeImpl() override { return nullptr; }
    const DOMNodeImpl* getNodeImpl() const override { return nullptr; }
    DOMParentNode* getParentNodeImpl() override { return nullptr; }
    const DOMParentNode* getParentNodeImpl() const override { return nullptr; }
    DOMChildNode* getChildNodeImpl() override { return nullptr; }
    const DOMChildNode* getChildNodeImpl() const override { return nullptr; }
};

int main() {
    XMLPlatformUtils::Initialize();
    int status = 0;
    try {
        XercesDOMParser parser;
        parser.setDoNamespaces(true);
        parser.setValidationScheme(XercesDOMParser::Val_Never);
        parser.setLoadExternalDTD(false);
        parser.setDisableDefaultEntityResolution(true);
        const char xml[] = "<!DOCTYPE root [<!ENTITY value 'hello'><!NOTATION note SYSTEM 'note.bin'>]><root>&value;</root>";
        MemBufInputSource input(reinterpret_cast<const XMLByte*>(xml), sizeof(xml) - 1, "memory");
        parser.parse(input);
        require(parser.getErrorCount() == 0);
        DOMDocument* doc = parser.getDocument();
        const XMLCh* name = doc->getDocumentElement()->getNodeName();
        std::vector<DOMNode*> nodes = {
            nullptr, doc, doc->getDocumentElement(), doc->createElement(name),
            doc->createTextNode(name), doc->createCDATASection(name),
            doc->createAttribute(name), doc->createComment(name),
            doc->createDocumentFragment(), doc->createProcessingInstruction(name, name),
            doc->getDoctype(), doc->getDoctype()->getEntities()->item(0),
            doc->getDoctype()->getNotations()->item(0), doc->createEntityReference(name)
        };
        for (DOMNode* node : nodes) verify(node);
        UnusualElement unusual(doc, name);
        MissingMemberElement missing(doc, name);
        verify(&unusual);
        verify(&missing);
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        status = 1;
    }
    catch (...) {
        std::cerr << "Unexpected exception in DOM cast regression checks" << std::endl;
        status = 1;
    }
    XMLPlatformUtils::Terminate();
    if (status == 0) std::cout << "DOM cast regression checks passed\n";
    return status;
}
