#ifndef DOMTextImpl_HEADER_GUARD_
#define DOMTextImpl_HEADER_GUARD_

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

//
//  This file is part of the internal implementation of the C++ XML DOM.
//  It should NOT be included or used directly by application programs.
//
//  Applications should include the file <xercesc/dom/DOM.hpp> for the entire
//  DOM API, or xercesc/dom/DOM*.hpp for individual DOM classes, where the class
//  name is substituded for the *.
//



#include <xercesc/util/XercesDefs.hpp>
#include <xercesc/dom/DOMText.hpp>

#include "DOMChildNode.hpp"
#include "DOMNodeImpl.hpp"
#include "DOMCharacterDataImpl.hpp"

XERCES_CPP_NAMESPACE_BEGIN


class CDOM_EXPORT DOMTextImpl: public DOMText {
public:
    DOMNodeImpl             fNode;
    DOMChildNode            fChild;
    DOMCharacterDataImpl    fCharacterData;

public:
    DOMTextImpl(DOMDocument* ownerDoc, const XMLCh* data);
    DOMTextImpl(const DOMTextImpl& other, bool deep=false);

    virtual                ~DOMTextImpl();
    virtual DOMText*        splitText(XMLSize_t offset);
    // DOM Level 3
    virtual bool            getIsWhitespaceInElementContent() const;
    virtual const XMLCh*    getWholeText();
    virtual DOMText*        replaceWholeText(const XMLCh* content);

    // non-standard extension
    virtual bool            isIgnorableWhitespace() const;

    // Declare the functions coming from DOMNode.
    DOMNODE_FUNCTIONS;


    // All of the functions coming from DOMCharacterData
    virtual const XMLCh*    getData() const;
    virtual XMLSize_t       getLength() const;
    virtual const XMLCh*    substringData(XMLSize_t offset,
                                          XMLSize_t count) const;
    virtual void            appendData(const XMLCh *arg);
    virtual void            insertData(XMLSize_t offset, const  XMLCh *arg);
    virtual void            deleteData(XMLSize_t offset,
                                       XMLSize_t count);
    virtual void            replaceData(XMLSize_t offset,
                                        XMLSize_t count,
                                        const XMLCh *arg);
    virtual void            setData(const XMLCh *data);

protected:
    virtual void            setIgnorableWhitespace(bool ignorable);
    friend class            AbstractDOMParser;

private:
    // -----------------------------------------------------------------------
    // Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    DOMTextImpl & operator = (const DOMTextImpl &);
};

XERCES_CPP_NAMESPACE_END

#endif

