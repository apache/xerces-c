/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 2001 The Apache Software Foundation.  All rights
 * reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. The end-user documentation included with the redistribution,
 *    if any, must include the following acknowledgment:
 *       "This product includes software developed by the
 *        Apache Software Foundation (http://www.apache.org/)."
 *    Alternately, this acknowledgment may appear in the software itself,
 *    if and wherever such third-party acknowledgments normally appear.
 *
 * 4. The names "Xerces" and "Apache Software Foundation" must
 *    not be used to endorse or promote products derived from this
 *    software without prior written permission. For written
 *    permission, please contact apache\@apache.org.
 *
 * 5. Products derived from this software may not be called "Apache",
 *    nor may "Apache" appear in their name, without prior written
 *    permission of the Apache Software Foundation.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE APACHE SOFTWARE FOUNDATION OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * ====================================================================
 *
 * This software consists of voluntary contributions made by many
 * individuals on behalf of the Apache Software Foundation, and was
 * originally based on software copyright (c) 2001, International
 * Business Machines, Inc., http://www.ibm.com .  For more information
 * on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 */

/*
 * $Log$
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
