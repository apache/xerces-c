#ifndef DOMNotationImpl_HEADER_GUARD_
#define DOMNotationImpl_HEADER_GUARD_
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
#include <xercesc/dom/DOMNotation.hpp>

XERCES_CPP_NAMESPACE_BEGIN


#include "DOMNodeImpl.hpp"

class DOMDocument;


class CDOM_EXPORT DOMNotationImpl: public DOMNotation {
public:
    DOMNodeImpl      fNode;

    const XMLCh * fName;
    const XMLCh * fPublicId;
    const XMLCh * fSystemId;
    const XMLCh * fBaseURI;

public:
    DOMNotationImpl(DOMDocument *ownerDoc, const XMLCh *);
    DOMNotationImpl(const DOMNotationImpl &other, bool deep=false);

    virtual ~DOMNotationImpl();

     // Declare all of the functions from DOMNode.
    DOMNODE_FUNCTIONS;

    //
    // The Public Identifier for this Notation. If no public identifier
    // was specified, this will be null.
    virtual const XMLCh * getPublicId() const;

    // The System Identifier for this Notation. If no system identifier
    // was specified, this will be null.
    virtual const XMLCh * getSystemId() const;

    // NON-DOM: The Public Identifier for this Notation. If no public
    // identifier was specified, this will be null.
    virtual void setPublicId(const XMLCh *arg);


    // NON-DOM: The System Identifier for this Notation. If no system
    // identifier was specified, this will be null.
    virtual void setSystemId(const XMLCh *arg);

    // NON-DOM: set base uri
    virtual void setBaseURI(const XMLCh *arg);

private:
    // unimplemented    
    DOMNotationImpl& operator= (const DOMNotationImpl& other);
};

XERCES_CPP_NAMESPACE_END

#endif
