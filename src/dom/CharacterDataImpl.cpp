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

#include "CharacterDataImpl.hpp"
#include "DOM_DOMException.hpp"


CharacterDataImpl::CharacterDataImpl(DocumentImpl *ownerDoc,
                                     const DOMString &data)
    : NodeImpl(ownerDoc, data)
{
};

CharacterDataImpl::CharacterDataImpl(const CharacterDataImpl &other, bool deep)
    : NodeImpl(other)
{
};


CharacterDataImpl::~CharacterDataImpl() {
};



void CharacterDataImpl::appendData(const DOMString &data)
{
    if(readOnly)
        throw DOM_DOMException(
        DOM_DOMException::NO_MODIFICATION_ALLOWED_ERR, null);
    
    value.appendData(data);
};


void CharacterDataImpl::deleteData(unsigned int offset, unsigned int count)
{
    if (readOnly)
        throw DOM_DOMException(
        DOM_DOMException::NO_MODIFICATION_ALLOWED_ERR, null);

    // Note: the C++ DOMString operation throws the correct DOMExceptions
    //       when parameter values are bad.
    //  
    value.deleteData(offset, count);
};



DOMString &CharacterDataImpl::getData()
{
    return value; 
};


//
//  getCharDataLength - return the length of the character data string.
//                      Note:  in the public DOM API, the name of this method
//                      is getLength(), but has been renamed here to avoid a
//                      conflict with NodeListImpl::getLength().  The conflict
//                      occurs because NodeListImpl is a base class of us.
//                      DOM_CharData::getLength() delegates to this method, so
//                      all of the names are correct from an external API
//                      point of view.
//
unsigned int CharacterDataImpl::getCharDataLength()
{
    return value.length();  
};



void CharacterDataImpl::insertData(unsigned int offset, const DOMString &data) 
{
    
    if (readOnly)
        throw DOM_DOMException(
        DOM_DOMException::NO_MODIFICATION_ALLOWED_ERR, null);
    
    // Note: the C++ DOMString operation throws the correct DOMExceptions
    //       when parameter values are bad.
    //  
    value.insertData(offset, data);
}



void CharacterDataImpl::replaceData(unsigned int offset, unsigned int count, const DOMString &data)
{
    if (readOnly)
        throw DOM_DOMException(
        DOM_DOMException::NO_MODIFICATION_ALLOWED_ERR, null);
    deleteData(offset, count);
    insertData(offset, data);
};




void CharacterDataImpl::setData(const DOMString &arg)
{
    if (readOnly)
        throw DOM_DOMException(DOM_DOMException::NO_MODIFICATION_ALLOWED_ERR, null);
    value = arg.clone();
};





DOMString CharacterDataImpl::substringData(unsigned int offset, unsigned int count)
{

    // Note: the C++ DOMString operation throws the correct DOMExceptions
    //       when parameter values are bad.
    //  
    return value.substringData(offset, count);
};

