/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 2002 The Apache Software Foundation.  All rights
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
 *    nor may "Apache" appear in their featName, without prior written
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
 * $Id$
 * $Log$
 * Revision 1.2  2002/06/03 22:40:07  peiyongz
 * *** empty log message ***
 *
 * Revision 1.1  2002/05/29 13:33:32  peiyongz
 * DOM3 Save Interface: DOMWriter/DOMWriterFilter
 *
 */

#include "DOMPrintFilter.hpp"
#include <xercesc/util/XMLUniDefs.hpp>
#include <xercesc/util/XMLString.hpp>

static const XMLCh  element_person[]=
{
	chLatin_p, chLatin_e, chLatin_r, chLatin_s, chLatin_o, chLatin_n, chNull 
};

static const XMLCh  element_link[]=
{
	chLatin_l, chLatin_i, chLatin_n, chLatin_k, chNull 
};

DOMPrintFilter::DOMPrintFilter(unsigned long whatToShow)
:DOMWriterFilter(whatToShow)
{}

short DOMPrintFilter::acceptNode(const DOMNode* node) const
{
	//
	// The DOMWriter shall call getWhatToShow() before calling 
	// acceptNode(), to show nodes which are supposed to be 
	// shown to this filter.
	// 
	// REVISIT: In case the DOMWriter does not follow the protocol, 
	//          Shall the filter honour, or NOT, what it claimes
	//         (when it is constructed/setWhatToShow()) 
	//          it is interested in ?
	// 
	// The DOMLS specs does not specify that acceptNode() shall do
	// this way, or not, so it is up the implementation,
	// to skip the code below for the sake of performance ...
	//
	if ((getWhatToShow() & (1 << (node->getNodeType() - 1))) == 0)
		return DOMNodeFilter::FILTER_ACCEPT;

	switch (node->getNodeType())
	{
	case DOMNode::ELEMENT_NODE:
		{
			// for element whose name is "person", skip it
			if (XMLString::compareString(node->getNodeName(), element_person)==0)
				return DOMNodeFilter::FILTER_SKIP;
			// for element whose name is "line", reject it
			if (XMLString::compareString(node->getNodeName(), element_link)==0)
				return DOMNodeFilter::FILTER_REJECT;
			// for rest, accept it
			return DOMNodeFilter::FILTER_ACCEPT;

			break;
		}
	case DOMNode::COMMENT_NODE:
		{
			// the WhatToShow will make this no effect
			return DOMNodeFilter::FILTER_REJECT;
			break;
		}
	case DOMNode::TEXT_NODE:
		{
			// the WhatToShow will make this no effect
			return DOMNodeFilter::FILTER_REJECT;
			break;
		}
	case DOMNode::DOCUMENT_TYPE_NODE:
		{
			// even we say we are going to process document type,
			// we are not able be to see this node since
			// DOMWriterImpl (a XercesC's default implementation
			// of DOMWriter) will not pass DocumentType node to
			// this filter.
			//
			return DOMNodeFilter::FILTER_REJECT;  // no effect
			break;
		}
	case DOMNode::DOCUMENT_NODE:
		{
			// same as DOCUMENT_NODE
			return DOMNodeFilter::FILTER_REJECT;  // no effect
			break;
		}
	default :
		{
			return DOMNodeFilter::FILTER_ACCEPT;
			break;
		}
	}

	return DOMNodeFilter::FILTER_ACCEPT;
}

