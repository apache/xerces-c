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
 * Revision 1.3  2000/02/06 07:47:58  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.2  1999/12/18 00:21:23  roddey
 * Fixed a small reported memory leak
 *
 * Revision 1.1.1.1  1999/11/09 01:07:47  twl
 * Initial checkin
 *
 * Revision 1.2  1999/11/08 20:45:02  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */


#ifndef SAXEXCEPTION_HPP
#define SAXEXCEPTION_HPP

#include <util/XMLString.hpp>
#include <util/XMLUni.hpp>


/**
  * Encapsulate a general SAX error or warning.
  *
  * <p>This class can contain basic error or warning information from
  * either the XML SAX parser or the application: a parser writer or
  * application writer can subclass it to provide additional
  * functionality.  SAX handlers may throw this exception or
  * any exception subclassed from it.</p>
  *
  * <p>If the application needs to pass through other types of
  * exceptions, it must wrap those exceptions in a SAXException
  * or an exception derived from a SAXException.</p>
  *
  * <p>If the parser or application needs to include information
  * about a specific location in an XML document, it should use the
  * SAXParseException subclass.</p>
  *
  * $Log$
  * Revision 1.3  2000/02/06 07:47:58  rahulj
  * Year 2K copyright swat.
  *
  * Revision 1.2  1999/12/18 00:21:23  roddey
  * Fixed a small reported memory leak
  *
  * Revision 1.1.1.1  1999/11/09 01:07:47  twl
  * Initial checkin
  *
  * Revision 1.2  1999/11/08 20:45:02  rahul
  * Swat for adding in Product name and CVS comment log variable.
  *
  * @see SAXParseException#SAXParseException
  */
class SAX_EXPORT SAXException
{
public:
    /** @name Constructors and Destructor */
    //@{
    /** Default constructor 
     */
    SAXException() :

        fMsg(XMLString::replicate(XMLUni::fgZeroLenString))
    {
    }

  /**
    * Create a new SAXException.
    *
    * @param msg The error or warning message.
    */
    SAXException(const XMLCh* const msg) :

        fMsg(XMLString::replicate(msg))
    {
    }

    SAXException(const char* const msg) :

        fMsg(XMLString::transcode(msg))
    {
    }

    SAXException(const SAXException& toCopy) :

        fMsg(XMLString::replicate(toCopy.fMsg))
    {
    }

    /** Destructor */
    virtual ~SAXException()
    {
        delete [] fMsg;
    }

    //@}


    /** @name Public Operators */
    SAXException& operator=(const SAXException& toCopy)
    {
        if (this == &toCopy)
            return *this;

        delete [] fMsg;
        fMsg = XMLString::replicate(toCopy.fMsg);
        return *this;
    }

    /** @name Getter Methods */
    const XMLCh* getMessage() const
    {
        return fMsg;
    }


private :
    // -----------------------------------------------------------------------
    //  Private data members
    //
    //  fMsg
    //      This is the text of the error that is being thrown.
    // -----------------------------------------------------------------------
    XMLCh*  fMsg;
};

#endif
