#ifndef IDOM_Entity_HEADER_GUARD_
#define IDOM_Entity_HEADER_GUARD_

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
 * Revision 1.1  2002/02/01 22:21:56  peiyongz
 * Initial revision
 *
 * Revision 1.2  2001/05/11 13:25:52  tng
 * Copyright update.
 *
 * Revision 1.1.1.1  2001/04/03 00:14:30  andyh
 * IDOM
 *
 */


#include <xercesc/util/XercesDefs.hpp>
#include "IDOM_Node.hpp"


/**
 * This interface represents an entity, either parsed or unparsed, in an XML
 * document.
 *
 * Note that this models the entity itself not the entity
 * declaration. <code>Entity</code> declaration modeling has been left for a
 * later Level of the DOM specification.
 * <p>The <code>nodeName</code> attribute that is inherited from
 * <code>Node</code> contains the name of the entity.
 * <p>An XML processor may choose to completely expand entities before  the
 * structure model is passed to the DOM; in this case there will be no
 * <code>EntityReference</code> nodes in the document tree.
 *
 * <p>Note: the first release of this parser does not create entity
 *    nodes when reading an XML document.  Entities may be
 *    programatically created using DOM_Document::createEntity().
 */
class CDOM_EXPORT IDOM_Entity: public IDOM_Node {
protected:
    IDOM_Entity() {};
    IDOM_Entity(const IDOM_Entity &other) {};
    IDOM_Entity & operator = (const IDOM_Entity &other) {return *this;};

public:
    virtual ~IDOM_Entity() {};

    /** @name Get functions. */
    //@{
  /**
   * The public identifier associated with the entity, if specified.
   *
   * If the public identifier was not specified, this is <code>null</code>.
   */
  virtual const XMLCh *        getPublicId() const = 0;

  /**
   * The system identifier associated with the entity, if specified.
   *
   * If the system identifier was not specified, this is <code>null</code>.
   */
  virtual const XMLCh *        getSystemId() const = 0;

  /**
   * For unparsed entities, the name of the notation for the entity.
   *
   * For parsed entities, this is <code>null</code>.
   */
  virtual const XMLCh *        getNotationName() const = 0;

  //@}

};

#endif

