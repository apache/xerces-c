/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 1999-2002 The Apache Software Foundation.  All rights
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
#include "RangeImpl.hpp"

XERCES_CPP_NAMESPACE_BEGIN


static DOMString *gText = 0;   // will be lazily initialized to point to "#text"
static XMLRegisterCleanup gTextCleanup;

TextImpl::TextImpl(DocumentImpl *ownerDoc, const DOMString &dat)
    : CharacterDataImpl(ownerDoc, dat)
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
    return new (getOwnerDocument()->getMemoryManager()) TextImpl(*this, deep);
};


DOMString TextImpl::getNodeName() {
    return DStringPool::getStaticString("#text"
                                      , &gText
                                      , reinitTextImpl
                                      , gTextCleanup
                                      );
}

short TextImpl::getNodeType() {
    return DOM_Node::TEXT_NODE;
};


TextImpl *TextImpl::splitText(unsigned int offset)
{
    if (isReadOnly())
    {
        throw DOM_DOMException(
            DOM_DOMException::NO_MODIFICATION_ALLOWED_ERR, null);
    }
	unsigned int len = data.length();
    if (offset > len || offset < 0)
        throw DOM_DOMException(DOM_DOMException::INDEX_SIZE_ERR, null);

    TextImpl *newText =
                (TextImpl *) getOwnerDocument()->createTextNode(
                        data.substringData(offset, data.length() - offset));

    NodeImpl *parent = getParentNode();
    if (parent != null)
        parent->insertBefore(newText, getNextSibling());

    data = data.substringData(0, offset);

    if (this->getOwnerDocument() != null) {
        typedef RefVectorOf<RangeImpl> RangeImpls;
        RangeImpls* ranges = this->getOwnerDocument()->getRanges();
        if (ranges != null) {
            unsigned int sz = ranges->size();
            if (sz != 0) {
                for (unsigned int i =0; i<sz; i++) {
                    ranges->elementAt(i)->updateSplitInfo( this, newText, offset);
                }
            }
        }
    }

    return newText;
};


bool TextImpl::isIgnorableWhitespace()
{
    return ignorableWhitespace();
}


void TextImpl::setIgnorableWhitespace(bool ignorable)
{
    ignorableWhitespace(ignorable);
}

// -----------------------------------------------------------------------
//  Notification that lazy data has been deleted
// -----------------------------------------------------------------------
void TextImpl::reinitTextImpl() {

    delete gText;
    gText = 0;

}

XERCES_CPP_NAMESPACE_END

