/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 2001-2002 The Apache Software Foundation.  All rights
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
 * $Id$
 */

#include "DOMAttrMapImpl.hpp"
#include "DOMNamedNodeMapImpl.hpp"
#include "DOMNodeImpl.hpp"
#include "DOMElementImpl.hpp"
#include <xercesc/dom/DOMAttr.hpp>
#include "DOMCasts.hpp"
#include "DOMDocumentImpl.hpp"

DOMAttrMapImpl::DOMAttrMapImpl(DOMNode *ownerNod)
	: DOMNamedNodeMapImpl(ownerNod)
{
	hasDefaults(false);
}

DOMAttrMapImpl::DOMAttrMapImpl(DOMNode *ownerNod, const DOMNamedNodeMapImpl *defaults)
	: DOMNamedNodeMapImpl(ownerNod)
{
	hasDefaults(false);
	if (defaults != 0)
	{
		if (defaults->getLength() > 0)
		{
			hasDefaults(true);
			cloneContent(defaults);
		}
	}
}

DOMAttrMapImpl::~DOMAttrMapImpl()
{
}

DOMAttrMapImpl *DOMAttrMapImpl::cloneAttrMap(DOMNode *ownerNode_p)
{
	DOMAttrMapImpl *newmap = new (castToNodeImpl(ownerNode_p)->getOwnerDocument()) DOMAttrMapImpl(ownerNode_p);
	newmap->cloneContent(this);
	// newmap->attrDefaults = this->attrDefaults;  // revisit
	return newmap;
}

DOMNode *DOMAttrMapImpl::removeNamedItem(const XMLCh *name)
{
	DOMNode* removed = DOMNamedNodeMapImpl::removeNamedItem(name);

	// Replace it if it had a default value
	// (DOM spec level 1 - Element Interface)
	if (hasDefaults() && (removed != 0))
	{
		DOMAttrMapImpl* defAttrs = ((DOMElementImpl*)fOwnerNode)->getDefaultAttributes();
		DOMAttr* attr = (DOMAttr*)(defAttrs->getNamedItem(name));
		if (attr != 0)
		{
			DOMAttr* newAttr = (DOMAttr*)attr->cloneNode(true);
			setNamedItem(newAttr);
		}
	}

	return removed;
}

DOMNode *DOMAttrMapImpl::removeNamedItemNS(const XMLCh *namespaceURI, const XMLCh *localName)
{
	DOMNode* removed = DOMNamedNodeMapImpl::removeNamedItemNS(namespaceURI, localName);

	// Replace it if it had a default value
	// (DOM spec level 2 - Element Interface)
	if (hasDefaults() && (removed != 0))
	{
		DOMAttrMapImpl* defAttrs = ((DOMElementImpl*)fOwnerNode)->getDefaultAttributes();
		DOMAttr* attr = (DOMAttr*)(defAttrs->getNamedItemNS(namespaceURI, localName));
		if (attr != 0)
		{
			DOMAttr* newAttr = (DOMAttr*)attr->cloneNode(true);
			setNamedItem(newAttr);
		}
	}

	return removed;
}
