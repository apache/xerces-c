/*
 * Copyright 1999-2002,2004 The Apache Software Foundation.
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
    if (offset > len)
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

