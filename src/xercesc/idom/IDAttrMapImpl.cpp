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
 * $Id$
 */

#include "IDAttrMapImpl.hpp"
#include "IDNamedNodeMapImpl.hpp"
#include "IDNodeImpl.hpp"
#include "IDElementImpl.hpp"
#include "IDOM_Attr.hpp"
#include "IDCasts.hpp"
#include "IDDocumentImpl.hpp"

IDAttrMapImpl::IDAttrMapImpl(IDOM_Node *ownerNod)
	: IDNamedNodeMapImpl(ownerNod)
{
	hasDefaults(false);
}

IDAttrMapImpl::IDAttrMapImpl(IDOM_Node *ownerNod, const IDNamedNodeMapImpl *defaults)
	: IDNamedNodeMapImpl(ownerNod)
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

IDAttrMapImpl::~IDAttrMapImpl()
{
}

IDAttrMapImpl *IDAttrMapImpl::cloneAttrMap(IDOM_Node *ownerNode_p)
{
	IDAttrMapImpl *newmap = new (castToNodeImpl(ownerNode_p)->getOwnerDocument()) IDAttrMapImpl(ownerNode_p);
	newmap->cloneContent(this);
	// newmap->attrDefaults = this->attrDefaults;  // idom_revisit
	return newmap;
}

IDOM_Node *IDAttrMapImpl::removeNamedItem(const XMLCh *name)
{
	IDOM_Node* removed = IDNamedNodeMapImpl::removeNamedItem(name);

	// Replace it if it had a default value
	// (DOM spec level 1 - Element Interface)
	if (hasDefaults() && (removed != 0))
	{
		IDAttrMapImpl* defAttrs = ((IDElementImpl*)fOwnerNode)->getDefaultAttributes();
		IDOM_Attr* attr = (IDOM_Attr*)(defAttrs->getNamedItem(name));
		if (attr != 0)
		{
			IDOM_Attr* newAttr = (IDOM_Attr*)attr->cloneNode(true);
			setNamedItem(newAttr);
		}
	}

	return removed;
}

IDOM_Node *IDAttrMapImpl::removeNamedItemNS(const XMLCh *namespaceURI, const XMLCh *localName)
{
	IDOM_Node* removed = IDNamedNodeMapImpl::removeNamedItemNS(namespaceURI, localName);

	// Replace it if it had a default value
	// (DOM spec level 2 - Element Interface)
	if (hasDefaults() && (removed != 0))
	{
		IDAttrMapImpl* defAttrs = ((IDElementImpl*)fOwnerNode)->getDefaultAttributes();
		IDOM_Attr* attr = (IDOM_Attr*)(defAttrs->getNamedItemNS(namespaceURI, localName));
		if (attr != 0)
		{
			IDOM_Attr* newAttr = (IDOM_Attr*)attr->cloneNode(true);
			setNamedItem(newAttr);
		}
	}

	return removed;
}
