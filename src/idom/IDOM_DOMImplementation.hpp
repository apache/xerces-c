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
 * $Log$
 * Revision 1.2  2001/05/11 13:25:50  tng
 * Copyright update.
 *
 * Revision 1.1.1.1  2001/04/03 00:14:29  andyh
 * IDOM
 *
 */

#ifndef IDOM_DOMImplementation_HEADER_GUARD_
#define IDOM_DOMImplementation_HEADER_GUARD_

#include <util/XercesDefs.hpp>

class IDOM_Document;
class IDOM_DocumentType;

/**
 *   This class provides a way to query the capabilities of an implementation
 *   of the DOM
 */


class CDOM_EXPORT IDOM_DOMImplementation {
 protected:
     IDOM_DOMImplementation() {};                                      // no plain constructor
     IDOM_DOMImplementation(const IDOM_DOMImplementation &other) {};   // no copy construtor.
     IDOM_DOMImplementation & operator = (const IDOM_DOMImplementation &other) {return *this;};  // No Assignment


 public:

     //  Factory method for getting a DOMImplementation object.
     //     The DOM implementation retains ownership of the returned object.
     //     Application code should NOT delete it.
     //
 static IDOM_DOMImplementation *getImplementation();



 virtual ~IDOM_DOMImplementation() {};

virtual bool  hasFeature(const XMLCh *feature,  const XMLCh *version) = 0;



//  Create a new DocumentType.
//     Initially the application owns the returned DocumentType object and is responsible
//        for deleting it.  If the DocumentType is subsequently associated with a Document,
//        that document becomes the owner of the storage and will delete the document type
//        when the document is deleted.
virtual  IDOM_DocumentType *createDocumentType(const XMLCh *qualifiedName,
                                               const XMLCh *publicId, const XMLCh *systemId) = 0;


virtual IDOM_Document *createDocument(const XMLCh *namespaceURI,
                              const XMLCh *qualifiedName, IDOM_DocumentType *doctype) = 0;

// Non-standard extension.  Create a completely empty document that has neither a root
//          element or a doctype node.
//
virtual IDOM_Document *createDocument() = 0;
};

#endif
