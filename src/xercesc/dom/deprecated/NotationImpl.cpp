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

#include "NotationImpl.hpp"
#include "DOM_DOMException.hpp"
#include "DOM_Node.hpp"


XERCES_CPP_NAMESPACE_BEGIN


/**
* Notations are how the Document Type Description (DTD) records hints
* about the format of an XML "unparsed entity" -- in other words,
* non-XML data bound to this document type, which some applications
* may wish to consult when manipulating the document. A Notation
* represents a name-value pair, with its nodeName being set to the
* declared name of the notation.
* <P>
* Notations are also used to formally declare the "targets" of
* Processing Instructions.
* <P>
* Note that the Notation's data is non-DOM information; the DOM only
* records what and where it is.
* <P>
* See the XML 1.0 spec, sections 4.7 and 2.6, for more info.
* <P>
* Level 1 of the DOM does not support editing Notation contents.
*
* @author Rania Y. Khalaf
* @author Joseph Kesselman
* @since  PR-DOM-Level-1-19980818.
*/

NotationImpl::NotationImpl(DocumentImpl *ownerDoc, const DOMString &nName)
    : NodeImpl(ownerDoc)
{
    name = nName.clone();
};

NotationImpl::NotationImpl(const NotationImpl &other, bool deep)
    : NodeImpl(other)
{
    name = other.name.clone();
};


NotationImpl::~NotationImpl()
{
};


NodeImpl *NotationImpl::cloneNode(bool deep)
{
    return new NotationImpl(*this, deep);
};


DOMString NotationImpl::getNodeName() {
    return name;
};


short NotationImpl::getNodeType() {
    return DOM_Node::NOTATION_NODE;
};


// Notation nodes do not have a parent
NodeImpl * NotationImpl::getParentNode()
{
    return 0;
};


DOMString NotationImpl::getPublicId()
{
    return publicId.clone();
};


DOMString NotationImpl::getSystemId()
{
    return systemId.clone();
};


void NotationImpl::setPublicId(const DOMString &arg)
{
    if(isReadOnly())
        throw DOM_DOMException(
        DOM_DOMException::NO_MODIFICATION_ALLOWED_ERR,null);

    publicId = arg.clone();
};


void NotationImpl::setSystemId(const DOMString &arg)
{
    if(isReadOnly())
        throw DOM_DOMException(
        DOM_DOMException::NO_MODIFICATION_ALLOWED_ERR,null);

    systemId = arg.clone();
};

XERCES_CPP_NAMESPACE_END

