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
 * Revision 1.5  2004/01/29 11:52:31  cargilld
 * Code cleanup changes to get rid of various compiler diagnostic messages.
 *
 * Revision 1.4  2002/11/04 14:49:42  tng
 * C++ Namespace Support.
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

#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMNamedNodeMap.hpp>
#include <xercesc/dom/DOMNode.hpp>

XERCES_CPP_NAMESPACE_BEGIN

class DOMNode;
class DOMElement;

/**
 * Some useful utility methods.
 */
class VALIDATORS_EXPORT XUtil
{
public:

    // Finds and returns the first child element node.
    static DOMElement* getFirstChildElement(const DOMNode* const parent);

    // Finds and returns the first child node with the given qualifiedname.
    static DOMElement* getFirstChildElementNS(const DOMNode* const parent
                                              , const XMLCh** const elemNames
                                              , const XMLCh* const uriStr
                                              , unsigned int       length);

    // Finds and returns the next sibling element node.
    static DOMElement* getNextSiblingElement(const DOMNode* const node);

    static DOMElement* getNextSiblingElementNS(const DOMNode* const node
                                               , const XMLCh** const elemNames
                                               , const XMLCh* const uriStr
                                               , unsigned int        length);

private:
    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------

	// This class cannot be instantiated.
     XUtil() {};
	~XUtil() {};
};

XERCES_CPP_NAMESPACE_END

#endif
