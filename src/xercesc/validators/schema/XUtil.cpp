/*
 * Copyright 2001,2004 The Apache Software Foundation.
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
 * $Log$
 * Revision 1.6  2004/09/08 13:56:58  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.5  2002/11/04 14:49:42  tng
 * C++ Namespace Support.
 *
 * Revision 1.4  2002/09/24 20:12:48  tng
 * Performance: use XMLString::equals instead of XMLString::compareString
 *
 * Revision 1.3  2002/05/21 19:31:45  tng
 * DOM Reorganization: modify to use the new DOM interface and remove obsolete code in XUtil.
 *
 * Revision 1.2  2002/02/06 22:21:49  knoaman
 * Use IDOM for schema processing.
 *
 * Revision 1.1.1.1  2002/02/01 22:22:50  peiyongz
 * sane_include
 *
 * Revision 1.6  2001/12/05 20:12:30  knoaman
 * Use getLocalName instead of getNodeName.
 *
 * Revision 1.5  2001/11/02 14:13:45  knoaman
 * Add support for identity constraints.
 *
 * Revision 1.4  2001/05/11 13:27:39  tng
 * Copyright update.
 *
 * Revision 1.3  2001/05/03 21:02:40  tng
 * Schema: Add SubstitutionGroupComparator and update exception messages.  By Pei Yong Zhang.
 *
 * Revision 1.2  2001/04/04 18:02:04  tng
 * Schema: include failure on Unix for XUtil.cpp.  Fixed by Pei Yong Zhang.
 *
 * Revision 1.1  2001/03/30 16:06:00  tng
 * Schema: XUtil, added by Pei Yong Zhang
 *
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/validators/schema/XUtil.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/framework/XMLBuffer.hpp>
#include <xercesc/util/IllegalArgumentException.hpp>
#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMNamedNodeMap.hpp>
#include <xercesc/dom/DOMNode.hpp>

XERCES_CPP_NAMESPACE_BEGIN

// Finds and returns the first child element node.
DOMElement* XUtil::getFirstChildElement(const DOMNode* const parent)
{
    // search for node
    DOMNode* child = parent->getFirstChild();

    while (child != 0)
	{
        if (child->getNodeType() == DOMNode::ELEMENT_NODE)
            return (DOMElement*)child;

        child = child->getNextSibling();
    }

    // not found
    return 0;
}

// Finds and returns the first child node with the given name.
DOMElement* XUtil::getFirstChildElementNS(const DOMNode* const parent
                                          , const XMLCh** const elemNames
                                          , const XMLCh* const uriStr
                                          , unsigned int        length)
{
    // search for node
    DOMNode* child = parent->getFirstChild();
    while (child != 0)
	{
        if (child->getNodeType() == DOMNode::ELEMENT_NODE)
		{
            for (unsigned int i = 0; i < length; i++)
			{
                if (XMLString::equals(child->getNamespaceURI(), uriStr) &&
                    XMLString::equals(child->getLocalName(), elemNames[i]))
                    return (DOMElement*)child;
			}
		}
        child = child->getNextSibling();
    }

    // not found
    return 0;
}

// Finds and returns the last child element node.
DOMElement* XUtil::getNextSiblingElement(const DOMNode* const node)
{
    // search for node
    DOMNode* sibling = node->getNextSibling();

    while (sibling != 0)
	{
        if (sibling->getNodeType() == DOMNode::ELEMENT_NODE)
            return (DOMElement*)sibling;

        sibling = sibling->getNextSibling();
    }

    // not found
    return 0;
}

// Finds and returns the next sibling element node with the give name.
DOMElement* XUtil::getNextSiblingElementNS(const DOMNode* const node
                                           , const XMLCh** const elemNames
                                           , const XMLCh* const uriStr
									       , unsigned int        length)
{
    // search for node
    DOMNode* sibling = node->getNextSibling();
    while (sibling != 0)
	{
        if (sibling->getNodeType() == DOMNode::ELEMENT_NODE)
		{
            for (unsigned int i = 0; i < length; i++)
			{
                if (XMLString::equals(sibling->getNamespaceURI(), uriStr) &&
                    XMLString::equals(sibling->getLocalName(), elemNames[i]))
                    return (DOMElement*)sibling;
			}
		}
        sibling = sibling->getNextSibling();
    }

    // not found
    return 0;
}

XERCES_CPP_NAMESPACE_END
