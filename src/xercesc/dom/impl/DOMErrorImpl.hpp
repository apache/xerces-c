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
 * Revision 1.5  2003/03/07 18:07:17  tng
 * Return a reference instead of void for operator=
 *
 * Revision 1.4  2002/11/04 15:07:34  tng
 * C++ Namespace Support.
 *
 * Revision 1.3  2002/07/15 19:25:49  tng
 * DOM L3:  declare a dummy DOMError::set/getRelatedException
 *
 * Revision 1.2  2002/05/30 19:26:00  knoaman
 * documentation update
 *
 * Revision 1.1  2002/05/23 15:47:24  knoaman
 * DOM L3 core - support for DOMError, DOMErrorHandler and DOMLocator
 *
 */


#ifndef DOMERRORIMPL_HPP
#define DOMERRORIMPL_HPP

#include <xercesc/dom/DOMError.hpp>

XERCES_CPP_NAMESPACE_BEGIN


/**
  * Introduced in DOM Level 3
  * Implementation of a DOMError interface.
  *
  * @see DOMError#DOMError
  */

class CDOM_EXPORT DOMErrorImpl : public DOMError
{
public:
    /** @name Constructors and Destructor */
    //@{

    /** Constructors */
    DOMErrorImpl(const short severity);

    DOMErrorImpl
    (
        const short severity
        , const XMLCh* const message
        , DOMLocator* const location
    );

    /** Desctructor */
    virtual ~DOMErrorImpl();

    //@}

    /** @name Get function */
    //@{

   /**
    * <p><b>"Experimental - subject to change"</b></p>
    *
    * Get the severity of the error
    */
    virtual short getSeverity() const;

   /**
    * <p><b>"Experimental - subject to change"</b></p>
    *
    * Get the message describing the error that occured.
    */
    virtual const XMLCh* getMessage() const;

   /**
    * <p><b>"Experimental - subject to change"</b></p>
    *
    * Get the location of the error
    */
    virtual DOMLocator* getLocation() const;

    /**
     * The related platform dependent exception if any.
     *
     * <p><b>"Experimental - subject to change"</b></p>
     *
     * @see   setRelatedException
     * @since DOM Level 3
     */
    virtual void* getRelatedException() const;

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
    virtual void setSeverity(const short severity);

   /**
    * <p><b>"Experimental - subject to change"</b></p>
    *
    * Set the error message
    *
    * @param message the error message to set.
    */
    virtual void setMessage(const XMLCh* const message);

   /**
    * <p><b>"Experimental - subject to change"</b></p>
    *
    * Set the location of the error
    *
    * @param location the location of the error to set.
    */
    virtual void setLocation(DOMLocator* const location);

   /**
    * <p><b>"Experimental - subject to change"</b></p>
    *
    * Set whether the location is owned by DOMError or not
    *
    * @param value <code>true</code> if DOMLocator is owned and should be
    *              deleted, <code>false</code> otherwise.
    */
    void setAdoptLocation(const bool value);

    /**
     * The related platform dependent exception if any.
     *
     * <p><b>"Experimental - subject to change"</b></p>
     *
     * @param exception the related exception to set.
     * @see   getRelatedException
     * @since DOM Level 3
     */
    virtual void setRelatedException(void* exception) const;

    //@}


private :
    /* Unimplemented constructors and operators */

    /* Copy constructor */
    DOMErrorImpl(const DOMErrorImpl&);

    /* Assignment operator */
    DOMErrorImpl& operator=(const DOMErrorImpl&);

    // -----------------------------------------------------------------------
    //  Private data members
    //
    //  fAdoptLocation
    //      Indicates whether we own the DOMLocator object or not.
    //
    //  fSeverity
    //      The type of the error.
    //
    //  fMessage
    //      The error message
    //
    //  fLocation
    //      The location info of the error
    // -----------------------------------------------------------------------
    bool         fAdoptLocation;
    short        fSeverity;
    const XMLCh* fMessage;
    DOMLocator*  fLocation;
};

// ---------------------------------------------------------------------------
//  DOMErrorImpl: Getter methods
// ---------------------------------------------------------------------------
inline short DOMErrorImpl::getSeverity() const
{
    return fSeverity;
}

inline const XMLCh* DOMErrorImpl::getMessage() const
{
    return fMessage;
}

inline DOMLocator* DOMErrorImpl::getLocation() const
{
    return fLocation;
}

inline void* DOMErrorImpl::getRelatedException() const
{
    return 0;
}

// ---------------------------------------------------------------------------
//  DOMLocatorImpl: Setter methods
// ---------------------------------------------------------------------------
inline void DOMErrorImpl::setSeverity(const short severity)
{
    fSeverity = severity;
}

inline void DOMErrorImpl::setMessage(const XMLCh* const message)
{
    fMessage = message;
}

inline void DOMErrorImpl::setAdoptLocation(const bool value)
{
    fAdoptLocation = value;
}


XERCES_CPP_NAMESPACE_END

#endif
