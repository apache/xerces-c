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

/**
 * $Log$
 * Revision 1.2  2000/02/06 07:47:57  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.1.1.1  1999/11/09 01:07:44  twl
 * Initial checkin
 *
 * Revision 1.2  1999/11/08 20:44:54  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */


#ifndef DTDHANDLER_HPP
#define DTDHANDLER_HPP

/**
  * Receive notification of basic DTD-related events.
  *
  * <p>If a SAX application needs information about notations and
  * unparsed entities, then the application implements this 
  * interface and registers an instance with the SAX parser using 
  * the parser's setDTDHandler method.  The parser uses the 
  * instance to report notation and unparsed entity declarations to 
  * the application.</p>
  *
  * <p>The SAX parser may report these events in any order, regardless
  * of the order in which the notations and unparsed entities were
  * declared; however, all DTD events must be reported after the
  * document handler's startDocument event, and before the first
  * startElement event.</p>
  *
  * <p>It is up to the application to store the information for 
  * future use (perhaps in a hash table or object tree).
  * If the application encounters attributes of type "NOTATION",
  * "ENTITY", or "ENTITIES", it can use the information that it
  * obtained through this interface to find the entity and/or
  * notation corresponding with the attribute value.</p>
  *
  * <p>The HandlerBase class provides a default implementation
  * of this interface, which simply ignores the events.</p>
  *
  * $Log$
  * Revision 1.2  2000/02/06 07:47:57  rahulj
  * Year 2K copyright swat.
  *
  * Revision 1.1.1.1  1999/11/09 01:07:44  twl
  * Initial checkin
  *
  * Revision 1.2  1999/11/08 20:44:54  rahul
  * Swat for adding in Product name and CVS comment log variable.
  *
  * @see Parser#setDTDHandler
  * @see HandlerBase#HandlerBase
  */

class SAX_EXPORT DTDHandler
{
public:
    /** @name Constructors and Destructor */
    //@{
    /** Default Constructor */
    DTDHandler()
    {
    }
    
    /** Destructor */
    virtual ~DTDHandler()
    {
    }

    //@}

    /** @name The DTD handler interface */
    //@{
  /**
    * Receive notification of a notation declaration event.
    *
    * <p>It is up to the application to record the notation for later
    * reference, if necessary.</p>
    *
    * <p>If a system identifier is present, and it is a URL, the SAX
    * parser must resolve it fully before passing it to the
    * application.</p>
    *
    * @param name The notation name.
    * @param publicId The notation's public identifier, or null if
    *        none was given.
    * @param systemId The notation's system identifier, or null if
    *        none was given.
    * @exception SAXException Any SAX exception, possibly
    *            wrapping another exception.
    * @see #unparsedEntityDecl
    * @see AttributeList#AttributeList
    */
	virtual void notationDecl
    (
        const   XMLCh* const    name
        , const XMLCh* const    publicId
        , const XMLCh* const    systemId
    ) = 0;

  /**
    * Receive notification of an unparsed entity declaration event.
    *
    * <p>Note that the notation name corresponds to a notation
    * reported by the notationDecl() event.  It is up to the
    * application to record the entity for later reference, if
    * necessary.</p>
    *
    * <p>If the system identifier is a URL, the parser must resolve it
    * fully before passing it to the application.</p>
    *
    * @exception SAXException Any SAX exception, possibly
    *            wrapping another exception.
    * @param name The unparsed entity's name.
    * @param publicId The entity's public identifier, or null if none
    *        was given.
    * @param systemId The entity's system identifier (it must always
    *        have one).
    * @param notation name The name of the associated notation.
    * @see #notationDecl
    * @see AttributeList#AttributeList
    */
    virtual void unparsedEntityDecl
    (
        const   XMLCh* const    name
        , const XMLCh* const    publicId
        , const XMLCh* const    systemId
        , const XMLCh* const    notationName
    ) = 0;

    virtual void resetDocType() = 0;

    //@}

private :
    /** @name Unimplemented constructors and operators */
    //@{
    /** Copy constructor */
    DTDHandler(const DTDHandler&);

    /** Assignment operator */
    void operator=(const DTDHandler&);
    //@}
};

#endif
