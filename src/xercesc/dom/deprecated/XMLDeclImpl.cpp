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

#include "XMLDeclImpl.hpp"
#include <xercesc/util/XMLUni.hpp>
#include "DOM_Node.hpp"
#include "DStringPool.hpp"
#include "DocumentImpl.hpp"

XERCES_CPP_NAMESPACE_BEGIN


static DOMString *gNam = 0;  // will be lazily initialized to "#xmldecl"
static XMLRegisterCleanup gNamCleanup;

XMLDeclImpl::XMLDeclImpl(DocumentImpl *ownerDoc)
    : ChildNode(ownerDoc),
    version(DOMString(XMLUni::fgVersion1_0)),
    encoding (DOMString(XMLUni::fgUTF8EncodingString)),
    standalone (DOMString(XMLUni::fgNoString))
{
};


//Introduced in DOM Level 2
XMLDeclImpl::XMLDeclImpl(DocumentImpl *ownerDoc, const DOMString &ver,
                         const DOMString &enc, const DOMString &isStd)
	: ChildNode(ownerDoc),
    version ( ver.clone() ),
    encoding ( enc.clone() ),
    standalone ( isStd.clone() )
{
};


XMLDeclImpl::XMLDeclImpl(const XMLDeclImpl &other, bool deep)
    : ChildNode(other)
{
    version     = other.version.clone();
    encoding    = other.encoding.clone();
    standalone  = other.standalone.clone();
};


XMLDeclImpl::~XMLDeclImpl()
{
};

NodeImpl * XMLDeclImpl::cloneNode(bool deep)
{
    return new (getOwnerDocument()->getMemoryManager()) XMLDeclImpl(*this, deep);
}

DOMString XMLDeclImpl::getNodeName()
{

    return DStringPool::getStaticString("#xmldecl"
                                       , &gNam
                                       , reinitXMLDeclImpl
                                       , gNamCleanup
                                       );
}

short XMLDeclImpl::getNodeType()
{
    return DOM_Node::XML_DECL_NODE;
}

DOMString XMLDeclImpl::getVersion() const
{
    return version;
}

DOMString XMLDeclImpl::getEncoding() const
{
    return encoding;
}

DOMString XMLDeclImpl::getStandalone() const
{
    return standalone;
}

void XMLDeclImpl::setVersion(const DOMString &data)
{
    version = data.clone();
}

void XMLDeclImpl::setEncoding(const DOMString &data)
{
    encoding = data.clone();
}

void XMLDeclImpl::setStandalone(const DOMString &data)
{
    standalone = data.clone();
}

// -----------------------------------------------------------------------
//  Notification that lazy data has been deleted
// -----------------------------------------------------------------------
void XMLDeclImpl::reinitXMLDeclImpl() {

    delete gNam;
    gNam = 0;

}

XERCES_CPP_NAMESPACE_END

