/*
 * The Apache Software License, Version 1.1
 * 
 * Copyright (c) 2002 The Apache Software Foundation.  All rights
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
 * Revision 1.2  2002/05/30 19:24:48  knoaman
 * documentation update
 *
 * Revision 1.1  2002/05/23 15:47:24  knoaman
 * DOM L3 core - support for DOMError, DOMErrorHandler and DOMLocator
 *
 */


#ifndef DOMERROR_HPP
#define DOMERROR_HPP

#include <xercesc/util/XercesDefs.hpp>

class DOMLocator;


/**
  * Introduced in DOM Level 3
  *
  * DOMError is an interface that describes an error.
  *
  * @see DOMErrorHandler#handleError
  */

class CDOM_EXPORT DOMError
{
protected:
    /** @name Constructors */
    //@{

    /** Default constructor */
    DOMError() {};

    //@}

public:

    // -----------------------------------------------------------------------
    //  Class types
    // -----------------------------------------------------------------------
    enum ErrorSeverity
    {
        SEVERITY_WARNING     = 0,
        SEVERITY_ERROR       = 1,
        SEVERITY_FATAL_ERROR = 2
    };


    /** @name Destructor */
    //@{

    /** Desctructor */
    virtual ~DOMError()
    {
    }

    //@}

    /** @name Get function */
    //@{

   /**
    * <p><b>"Experimental - subject to change"</b></p>
    *
    * Get the severity of the error
    */
    virtual short getSeverity() const = 0;

   /**
    * <p><b>"Experimental - subject to change"</b></p>
    *
    * Get the message describing the error that occured.
    */
    virtual const XMLCh* getMessage() const = 0;

   /**
    * <p><b>"Experimental - subject to change"</b></p>
    *
    * Get the location of the error
    */
    virtual DOMLocator* getLocation() const = 0;

    //@}


   /** @name Set function */
    //@{

   /**
    * <p><b>"Experimental - subject to change"</b></p>
    *
    * Set the severity of the error
    *
    * @param severity the type of the error to set
    */
    virtual void setSeverity(const short severity) = 0;

   /**
    * <p><b>"Experimental - subject to change"</b></p>
    *
    * Set the error message
    *
    * @param message the error message to set.
    */
    virtual void setMessage(const XMLCh* const message) = 0;

   /**
    * <p><b>"Experimental - subject to change"</b></p>
    *
    * Set the location of the error
    *
    * @param location the location of the error to set.
    */
    virtual void setLocation(DOMLocator* const location) = 0;

    //@}


private :
    /* Unimplemented constructors and operators */
    
    /* Copy constructor */
    DOMError(const DOMError&);

    /* Assignment operator */
    void operator=(const DOMError&);
    
};

#endif
