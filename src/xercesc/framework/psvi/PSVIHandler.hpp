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
  * Revision 1.2  2003/09/22 15:03:06  neilg
  * clearly the local name of an element should be a string, not an XMLElementDecl...
  *
  * Revision 1.1  2003/09/16 14:33:36  neilg
  * PSVI/schema component model classes, with Makefile/configuration changes necessary to build them
  *
  */


#if !defined(PSVIHANDLER_HPP)
#define PSVIHANDLER_HPP

#include <xercesc/framework/psvi/PSVIElement.hpp>
#include <xercesc/framework/psvi/PSVIAttribute.hpp>

XERCES_CPP_NAMESPACE_BEGIN


/**
  * This abstract class provides the interface for the scanner to return
  * PSVI information to the application.
  *
  */
class XMLPARSER_EXPORT PSVIHandler
{
public:
    // -----------------------------------------------------------------------
    //  Constructors are hidden, just the virtual destructor is exposed
    // -----------------------------------------------------------------------
    /** @name Destructor */
    //@{
    virtual ~PSVIHandler()
    {
    }
    //@}

    /** @name The PSVI handler interface */
    //@{
    /** Receive notification of the PSVI properties of an element.
      * The scanner will issue this call after the XMLDocumentHandler
      * endElement call.  Since the scanner will issue the psviAttributes
      * call immediately after reading the start tag of an element, all element
      * content will be effectively bracketed by these two calls.
      * @param  localName The name of the element whose end tag was just
      *                     parsed.
      * @param  uri       The namespace to which the element is bound
      * @param  prefix  The string representing the prefix name
      * @param  elementInfo    Object containing the element's PSVI properties
      */
    virtual void handleElementPSVI
    (
        const   XMLCh *                 localName 
        , const XMLCh* const            uri
        , const XMLCh* const            prefix
        , const PSVIElement *           elementInfo
    ) = 0;

    /**
      * Enables PSVI information about attributes to be passed back to the
      * application.  This callback will be made on *all*
      * elements; on elements with no attributes, the final parameter will
      * be null.
      * @param  localName The name of the element upon which start tag 
      *          these attributes were encountered.
      * @param  uri       The namespace to which the element is bound
      * @param  prefix  The string representing the prefix name
      * @param  psviAttributes   Object containing the attributes' PSVI properties
      *          with information to identify them.
      */
    virtual void handleAttributesPSVI
    (
        const   XMLCh *                 localName 
        , const XMLCh* const            uri
        , const XMLCh* const            prefix
        , const PSVIAttributeList *     psviAttributes
    ) = 0;


    //@}



protected :
    // -----------------------------------------------------------------------
    //  Hidden Constructors
    // -----------------------------------------------------------------------
    PSVIHandler()
    {
    }


private:
    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    PSVIHandler(const PSVIHandler&);
    PSVIHandler& operator=(const PSVIHandler&);
};

XERCES_CPP_NAMESPACE_END

#endif
