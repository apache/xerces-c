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
 * Revision 1.7  2000/03/28 19:43:14  roddey
 * Fixes for signed/unsigned warnings. New work for two way transcoding
 * stuff.
 *
 * Revision 1.6  2000/03/24 01:30:50  chchou
 * Fix bug #8 to support ignorable whitespace text nodes
 *
 * Revision 1.5  2000/03/02 19:54:05  roddey
 * This checkin includes many changes done while waiting for the
 * 1.1.0 code to be finished. I can't list them all here, but a list is
 * available elsewhere.
 *
 * Revision 1.4  2000/02/06 07:47:34  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.3  2000/02/04 01:49:25  aruna1
 * TreeWalker and NodeIterator changes
 *
 * Revision 1.2  1999/11/30 21:16:26  roddey
 * Changes to add the transcode() method to DOMString, which returns a transcoded
 * version (to local code page) of the DOM string contents. And I changed all of the
 * exception 'throw by pointer' to 'throw by value' style.
 *
 * Revision 1.1.1.1  1999/11/09 01:09:19  twl
 * Initial checkin
 *
 * Revision 1.2  1999/11/08 20:44:33  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */

#include "DocumentImpl.hpp"
#include "DOM_DOMException.hpp"
#include "DOM_Node.hpp"
#include "TextImpl.hpp"
#include "CharacterDataImpl.hpp"
#include "DStringPool.hpp"

static DOMString *gText;   // will be lazily initialized to point to "#text"


TextImpl::TextImpl(DocumentImpl *ownerDoc, const DOMString &data) :
CharacterDataImpl(ownerDoc, DStringPool::getStaticString("#text", &gText), 
                  DOM_Node::TEXT_NODE, data)
, fIgnorableWhitespace(false)
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
