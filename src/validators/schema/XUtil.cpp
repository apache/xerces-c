/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 1999-2000 The Apache Software Foundation.  All rights
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
 * originally based on software copyright (c) 1999, International
 * Business Machines, Inc., http://www.ibm.com .  For more information
 * on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 */

/*
 * $Log$
 * Revision 1.1  2001/03/30 16:06:00  tng
 * Schema: XUtil, added by Pei Yong Zhang
 *
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <validators/Schema/XUtil.hpp>
#include <util/XMLString.hpp>
#include <framework/XMLBuffer.hpp>
#include <dom/AttrImpl.hpp>
#include <dom/ElementImpl.hpp>
#include <dom/DocumentImpl.hpp>
#include <util/IllegalArgumentException.hpp>

void XUtil::copyInto(const DOM_Node &src, DOM_Node &dest)
{
    // get node factory
    DOM_Document factory = dest.getOwnerDocument();

    // placement variables
    DOM_Node start  = src;
    DOM_Node parent = src;
    DOM_Node place  = src;

    // traverse source tree
    while (place != 0)
	{
        // copy this node
        DOM_Node node;
        short  type = place.getNodeType();
        switch (type) 
		{
		case DOM_Node::CDATA_SECTION_NODE: 
			{
                node = factory.createCDATASection(place.getNodeValue());
                break;
			}
		case DOM_Node::COMMENT_NODE: 
			{               
				node = factory.createComment(place.getNodeValue());            
				break;
			}
		case DOM_Node::ELEMENT_NODE: 
			{
				DOM_Element element = factory.createElement(place.getNodeName());
				node = element;
			    ElementImpl *elemImpl = (ElementImpl*)element.fImpl;
				DOM_NamedNodeMap attrs  = place.getAttributes();
                unsigned int attrCount = attrs.getLength();
				for (unsigned int i = 0; i < attrCount; i++) 
				{
                    if (attrs.item(i).getNodeType() == DOM_Node::ATTRIBUTE_NODE)
                    {
    				    DOM_Node tmpNode = attrs.item(i);
					    DOM_Attr attr = (DOM_Attr&)tmpNode;
					    AttrImpl *attrImpl = elemImpl->setAttribute(attr.getNodeName(), attr.getNodeValue());
                        if ((factory.getNodeType() == DOM_Node::DOCUMENT_NODE) && !attr.getSpecified()) 
                            attrImpl->setSpecified(false);
                    }
				}//for
				break;
			}
		case DOM_Node::ENTITY_REFERENCE_NODE: 
			{
				node = factory.createEntityReference(place.getNodeName());
				break;
			}
		case DOM_Node::PROCESSING_INSTRUCTION_NODE: 
			{
				node = factory.createProcessingInstruction(place.getNodeName(), place.getNodeValue());
				break;
			}
		case DOM_Node::TEXT_NODE: 
			{
				node = factory.createTextNode(place.getNodeValue());
				break;
			}
		default: 
			{
                ThrowXML(IllegalArgumentException, XMLExcepts::Str_ZeroSizedTargetBuf);
                // later on, 
                /***
				   ThrowXML2(IllegalArgumentException
				           , XMLExcepts::Str_ZeroSizedTargetBuf
                           , "can't copy node type"
						   , node.getNodeName());
                ***/
			}
		}//switch

		dest.appendChild(node);

        // iterate over children
        if (place.hasChildNodes()) 
		{
			parent = place;
			place  = place.getFirstChild();
			dest   = node;
		}
            // advance
        else 
		{
			place = place.getNextSibling();
			while (place == null && parent != start) 
			{
				place  = parent.getNextSibling();
				parent = parent.getParentNode();
				dest   = dest.getParentNode();
			}
		}
	}//while

} 


/*
     * Returns the concatenated child text of the specified node.
     * This method only looks at the immediate children of type
     * <code>Node.TEXT_NODE</code> or the children of any child
     * node that is of type <code>Node.CDATA_SECTION_NODE</code> 
     * for the concatenation.
     *
     * @param node The node to look at.
*/

DOMString XUtil::getChildText(const DOM_Node &node)
{
    // is there anything to do?
    if (node == 0)
        return 0;

    // concatenate children text
    DOMString bufToFill;
    DOM_Node child = node.getFirstChild();
    while (child != 0) 
	{
		short type = child.getNodeType();
        if (type == DOM_Node::TEXT_NODE)
            bufToFill.appendData(child.getNodeValue());
        else if (type == DOM_Node::CDATA_SECTION_NODE)
            bufToFill.appendData(getChildText(child));

        child = child.getNextSibling();
    }

    // return text value
    return bufToFill;
}

// Finds and returns the first child element node.
DOM_Element XUtil::getFirstChildElement(const DOM_Node &parent)
{
    // search for node
    DOM_Node child = parent.getFirstChild();
    while (child != 0) 
	{
        if (child.getNodeType() == DOM_Node::ELEMENT_NODE) 
            return (DOM_Element&)child;

        child = child.getNextSibling();
    }

    // not found
    return DOM_Element();
}

// Finds and returns the first child node with the given name.
DOM_Element XUtil::getFirstChildElement(const DOM_Node    &parent
                                      , const XMLCh* const elemName)
{
    // search for node
    DOM_Node child = parent.getFirstChild();
    while (child != 0) 
	{
        if ((child.getNodeType() == DOM_Node::ELEMENT_NODE) &&
            (XMLString::compareString(child.getNodeName().rawBuffer(), elemName) ==0))
            return (DOM_Element&)child;

        child = child.getNextSibling();
    }

    // not found
    return DOM_Element();
}

// Finds and returns the first child node with the given name.
DOM_Element XUtil::getFirstChildElement(const DOM_Node     &parent
                                      , const XMLCh** const elemNames
									  , unsigned int        length)
{
    // search for node
    DOM_Node child = parent.getFirstChild();
    while (child != 0) 
	{
        if (child.getNodeType() == DOM_Node::ELEMENT_NODE)
		{
            for (unsigned int i = 0; i < length; i++)
			{
                if (XMLString::compareString(child.getNodeName().rawBuffer(), elemNames[i]) ==0)
                    return (DOM_Element&)child;
			}
		}
        child = child.getNextSibling();
    }

    // not found
    return DOM_Element();
}

// Finds and returns the first child node with the given name and
// attribute name, value pair.
DOM_Element XUtil::getFirstChildElement(const DOM_Node    &parent
                                      , const XMLCh* const elemName
                                      , const XMLCh* const attrName
                                      , const XMLCh* const attrValue)
{
    // search for node
    DOM_Node child = parent.getFirstChild();
    while (child != 0) 
	{
        if (child.getNodeType() == DOM_Node::ELEMENT_NODE) 
		{
			DOM_Element element = (DOM_Element&)child;
            if ((XMLString::compareString(element.getNodeName().rawBuffer(), elemName) ==0) &&
                (XMLString::compareString(element.getAttribute(attrName).rawBuffer(), attrValue) ==0))
				return element;
        }
        child = child.getNextSibling();
    }

    // not found
    return DOM_Element();
}

// Finds and returns the last child element node.
DOM_Element XUtil::getLastChildElement(const DOM_Node &parent) {

    // search for node
    DOM_Node child = parent.getLastChild();
    while (child != 0) 
	{
        if (child.getNodeType() == DOM_Node::ELEMENT_NODE)
            return (DOM_Element&)child;

        child = child.getPreviousSibling();
    }

    // not found
    return DOM_Element();

} // getLastChildElement(Node):Element

// Finds and returns the last child node with the given name.
DOM_Element XUtil::getLastChildElement(const DOM_Node    &parent
                                     , const XMLCh* const elemName)
{
    // search for node
    DOM_Node child = parent.getLastChild();
    while (child != 0) 
	{
        if ((child.getNodeType() == DOM_Node::ELEMENT_NODE) &&
            (XMLString::compareString(child.getNodeName().rawBuffer(), elemName) ==0))
                return (DOM_Element&)child;

        child = child.getPreviousSibling();
    }

    // not found
    return DOM_Element();
}

// Finds and returns the last child node with the given name.
DOM_Element XUtil::getLastChildElement(const DOM_Node     &parent
                                     , const XMLCh** const elemNames
									 , unsigned int        length)
{
    // search for node
    DOM_Node child = parent.getLastChild();
    while (child != 0) 
	{
        if (child.getNodeType() == DOM_Node::ELEMENT_NODE)
		{
            for (unsigned int i = 0; i < length; i++)
			{
                if (XMLString::compareString(child.getNodeName().rawBuffer(), elemNames[i]) ==0)
                    return (DOM_Element&)child;
			}
		}
        child = child.getPreviousSibling();
    }

    // not found
    return DOM_Element();
}

// Finds and returns the last child node with the given name and
// attribute name, value pair.
DOM_Element XUtil::getLastChildElement(const DOM_Node    &parent
                                     , const XMLCh* const elemName
                                     , const XMLCh* const attrName
                                     , const XMLCh* const attrValue)
{
    // search for node
    DOM_Node child = parent.getLastChild();
    while (child != 0) 
	{
        if (child.getNodeType() == DOM_Node::ELEMENT_NODE) 
		{
			DOM_Element element = (DOM_Element&)child;
            if ((XMLString::compareString(element.getNodeName().rawBuffer(), elemName) ==0) &&
                (XMLString::compareString(element.getAttribute(attrName).rawBuffer(), attrValue) ==0))
				return element;
        }
        child = child.getPreviousSibling();
    }

    // not found
    return DOM_Element();
}

// Finds and returns the next sibling element node.
DOM_Element XUtil::getNextSiblingElement(const DOM_Node &node) 
{
    // search for node
    DOM_Node sibling = node.getNextSibling();
    while (sibling != 0) 
	{
        if (sibling.getNodeType() == DOM_Node::ELEMENT_NODE)
            return (DOM_Element&)sibling;

        sibling = sibling.getNextSibling();
    }

    // not found
    return DOM_Element();
}

// Finds and returns the next sibling element node with the give name.
DOM_Element XUtil::getNextSiblingElement(const DOM_Node    &node
                                       , const XMLCh* const elemName) 
{
    // search for node
    DOM_Node sibling = node.getNextSibling();
    while (sibling != 0) 
	{
        if ((sibling.getNodeType() == DOM_Node::ELEMENT_NODE) &&
            (XMLString::compareString(sibling.getNodeName().rawBuffer(), elemName) ==0))
            return (DOM_Element&)sibling;

        sibling = sibling.getNextSibling();
    }

    // not found
    return DOM_Element();
}

// Finds and returns the next sibling element node with the give name.
DOM_Element XUtil::getNextSiblingElement(const DOM_Node     &node
                                       , const XMLCh** const elemNames
									   , unsigned int        length) 
{
    // search for node
    DOM_Node sibling = node.getNextSibling();
    while (sibling != 0) 
	{
        if (sibling.getNodeType() == DOM_Node::ELEMENT_NODE)
		{
            for (unsigned int i = 0; i < length; i++)
			{
                if (XMLString::compareString(sibling.getNodeName().rawBuffer(), elemNames[i]) ==0)
                    return (DOM_Element&)sibling;
			}
		}
        sibling = sibling.getNextSibling();
    }

    // not found
    return DOM_Element();
}

// Finds and returns the next sibling element node with the given name and
// attribute name, value pair.
DOM_Element XUtil::getNextSiblingElement(const DOM_Node    &node
                                       , const XMLCh* const elemName
                                       , const XMLCh* const attrName
                                       , const XMLCh* const attrValue)
{
    // search for node
    DOM_Node sibling = node.getNextSibling();
    while (sibling != 0) 
	{
        if (sibling.getNodeType() == DOM_Node::ELEMENT_NODE) 
		{
			DOM_Element element = (DOM_Element&)sibling;
            if ((XMLString::compareString(element.getNodeName().rawBuffer(), elemName) ==0) &&
                (XMLString::compareString(element.getAttribute(attrName).rawBuffer(), attrValue) ==0))
				return element;
        }
        sibling = sibling.getNextSibling();
    }

    // not found
    return DOM_Element();
}

