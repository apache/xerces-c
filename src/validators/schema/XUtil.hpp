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
 * Revision 1.3  2001/11/02 14:13:45  knoaman
 * Add support for identity constraints.
 *
 * Revision 1.2  2001/05/11 13:27:39  tng
 * Copyright update.
 *
 * Revision 1.1  2001/03/30 16:06:00  tng
 * Schema: XUtil, added by Pei Yong Zhang
 *
 */

#if !defined(XUTIL_HPP)
#define XUTIL_HPP

#include <dom/DOM_Element.hpp>
#include <dom/DOM_Document.hpp>
#include <dom/DOM_NamedNodeMap.hpp>
#include <dom/DOM_Node.hpp>

/**
 * Some useful utility methods.
 */
class VALIDATORS_EXPORT XUtil
{
public:

    //
    // Copies the source tree into the specified place in a destination
    // tree. The source node and its children are appended as children
    // of the destination node.
    // <p>
    // <em>Note:</em> This is an iterative implementation.
    //
    static void copyInto(const DOM_Node &src, DOM_Node &dest);

    /*
     * Returns the concatenated child text of the specified node.
     * This method only looks at the immediate children of type
     * <code>Node.TEXT_NODE</code> or the children of any child
     * node that is of type <code>Node.CDATA_SECTION_NODE</code>
     * for the concatenation.
     *
     * @param node The node to look at.
     */
    static DOMString getChildText(const DOM_Node &node);

    // Finds and returns the first child element node.
    static DOM_Element getFirstChildElement(const DOM_Node &parent);
    // Finds and returns the first child element node with the given name.
    static DOM_Element getFirstChildElement(const DOM_Node    &parent
		                                  , const XMLCh* const elemName);
    // Finds and returns the first child node with the given name.
    static DOM_Element getFirstChildElement(const DOM_Node     &parent
		                                  , const XMLCh** const elemNames
										  , unsigned int        length);
    // Finds and returns the first child node with the given name and
    // attribute name, value pair.
    static DOM_Element getFirstChildElement(const DOM_Node    &parent
		                                  , const XMLCh* const elemName
                                          , const XMLCh* const attrName
                                          , const XMLCh* const attrValue);

    // Finds and returns the first child node with the given qualifiedname.
    static DOM_Element getFirstChildElementNS(const DOM_Node     &parent
                                            , const XMLCh** const elemNames
                                            , const XMLCh* const uriStr
                                            , unsigned int        length);

    // Finds and returns the last child element node.
    static DOM_Element getLastChildElement(const DOM_Node &parent);
    // Finds and returns the last child element node with the given name.
    static DOM_Element getLastChildElement(const DOM_Node    &parent
                                         , const XMLCh* const elemName);
    // Finds and returns the last child node with the given name.
    static DOM_Element getLastChildElement(const DOM_Node     &parent
                                         , const XMLCh** const elemNames
	                                     , unsigned int        length);
    // Finds and returns the last child node with the given name and
    // attribute name, value pair.
    static DOM_Element getLastChildElement(const DOM_Node    &parent
                                         , const XMLCh* const elemName
                                         , const XMLCh* const attrName
                                         , const XMLCh* const attrValue);

    // Finds and returns the next sibling element node.
    static DOM_Element getNextSiblingElement(const DOM_Node &node);
    // Finds and returns the next sibling element node with the given name.
    static DOM_Element getNextSiblingElement(const DOM_Node    &node
		                                   , const XMLCh* const elemName);
    // Finds and returns the next sibling node with the given name.
    static DOM_Element getNextSiblingElement(const DOM_Node     &node
		                                   , const XMLCh** const elemNames
										   , unsigned int        length);
    // Finds and returns the next sibling node with the given name and
    // attribute name, value pair. Since only elements have attributes,
    // the node returned will be of type Node.ELEMENT_NODE.
    static DOM_Element getNextSiblingElement(const DOM_Node    &parent
		                                   , const XMLCh* const elemName
                                           , const XMLCh* const attrName
                                           , const XMLCh* const attrValue);

    // Finds and returns the next sibling node with the given qualified name.
    static DOM_Element getNextSiblingElementNS(const DOM_Node     &node
                                             , const XMLCh** const elemNames
                                             , const XMLCh* const uriStr
                                             , unsigned int        length);

protected:
    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------

	// This class cannot be instantiated.
     XUtil() {};
	~XUtil() {};

};

#endif
