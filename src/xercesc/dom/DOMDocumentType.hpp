#ifndef DOMDocumentType_HEADER_GUARD_
#define DOMDocumentType_HEADER_GUARD_


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

#include <xercesc/util/XercesDefs.hpp>
#include "DOMNode.hpp"

class DOMNamedNodeMap;

/**
 * Each <code>Document</code> has a <code>doctype</code> whose value
 * is either <code>null</code> or a <code>DocumentType</code> object.
 *
 * The <code>DOMDocumentType</code> class provides access
 *  to the list of entities and notations that are defined for the document.
 * <p>The DOM Level 1 doesn't support editing <code>DocumentType</code> nodes.
 */
class CDOM_EXPORT DOMDocumentType: public DOMNode {
protected:
    DOMDocumentType() {};
    DOMDocumentType(const DOMDocumentType &other) {};
    DOMDocumentType & operator = (const DOMDocumentType &other) {return *this;};

public:

    virtual ~DOMDocumentType() {};



    /** @name Getter functions. */
    //@{
  /**
   * The name of DTD; i.e., the name immediately following the
   * <code>DOCTYPE</code> keyword in an XML source document.
   */
  virtual const XMLCh *       getName() const = 0;

  /**
   * This function returns a  <code>NamedNodeMap</code> containing the general entities, both
   * external and internal, declared in the DTD. Parameter entities are not contained.
   * Duplicates are discarded.
   * <p>
   * Note: this functionality is not implemented in the initial release
   * of the parser, and the returned NamedNodeMap will be empty.
   */
  virtual DOMNamedNodeMap *getEntities() const = 0;


  /**
   * This function returns a named node map containing an entry for
   * each notation declared in a document's DTD.  Duplicates are discarded.
   *
   * <p>
   * Note: this functionality is not implemented in the initial release
   * of the parser, and the returned NamedNodeMap will be empty.
   */
  virtual DOMNamedNodeMap *getNotations() const = 0;
  //@}

    /** @name Functions introduced in DOM Level 2. */
    //@{
    /**
     * Get the public identifier of the external subset.
     *
     * @return The public identifier of the external subset.
     */
    virtual const XMLCh *     getPublicId() const = 0;

    /**
     * Get the system identifier of the external subset.
     *
     * @return The system identifier of the external subset.
     */
    virtual const XMLCh *     getSystemId() const = 0;

    /**
     * Get the internal subset as a string.
     *
     * @return The internal subset as a string.
     */
    virtual const XMLCh *     getInternalSubset() const = 0;
    //@}

};

#endif


