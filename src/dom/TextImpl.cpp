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
 * $Id$
 */

#include "DocumentImpl.hpp"
#include "DOM_DOMException.hpp"
#include "DOM_Node.hpp"
#include "TextImpl.hpp"
#include "CharacterDataImpl.hpp"
#include "DStringPool.hpp"

static DOMString *gText;   // will be lazily initialized to point to "#text"


TextImpl::TextImpl(DocumentImpl *ownerDoc, const DOMString &data)
    : CharacterDataImpl(ownerDoc, data),
    fIgnorableWhitespace(false)
{
};

TextImpl::TextImpl(const TextImpl &other, bool deep)
    : CharacterDataImpl(other, deep)
{
};

TextImpl::~TextImpl()
{
};


bool TextImpl::isTextImpl() 
{
    return true;
};


NodeImpl *TextImpl::cloneNode(bool deep)
{
    return ownerDocument->createTextNode(value);
};


DOMString TextImpl::getNodeName() {
    return DStringPool::getStaticString("#text", &gText);
}

short TextImpl::getNodeType() {
    return DOM_Node::TEXT_NODE;
};


TextImpl *TextImpl::splitText(unsigned int offset)
{
    if (readOnly)
    {
        throw DOM_DOMException(
            DOM_DOMException::NO_MODIFICATION_ALLOWED_ERR, null);
    }
	unsigned int len = value.length();
    if (offset >= len)
        throw DOM_DOMException(DOM_DOMException::INDEX_SIZE_ERR, null);
                
    TextImpl *newText = 
                (TextImpl *) ownerDocument->createTextNode(
                        value.substringData(offset, value.length() - offset));
    NodeImpl *parent = getParentNode();
    if (parent != null)
        parent->insertBefore(newText, getNextSibling());

    value = value.substringData(0, offset);
    return newText;
};


bool TextImpl::isIgnorableWhitespace()
{
    return fIgnorableWhitespace;
}


void TextImpl::setIgnorableWhitespace(bool ignorable)
{
    fIgnorableWhitespace = ignorable;
}
