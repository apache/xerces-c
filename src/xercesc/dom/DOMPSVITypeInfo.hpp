/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 2003 The Apache Software Foundation.  All rights
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

#if !defined(DOMPSVITYPEINFO_HPP)
#define DOMPSVITYPEINFO_HPP

//------------------------------------------------------------------------------------
//  Includes
//------------------------------------------------------------------------------------
#include <xercesc/util/XMLString.hpp>


XERCES_CPP_NAMESPACE_BEGIN

/**
  * The <code>DOMPSVITypeInfo</code> interface represent the PSVI info used by 
  * <code>DOMElement</code> or <code>DOMAttr</code> nodes, specified in the 
  * schemas associated with the document. 
  */
class CDOM_EXPORT DOMPSVITypeInfo
{
protected:
    // -----------------------------------------------------------------------
    //  Hidden constructors
    // -----------------------------------------------------------------------
    /** @name Hidden constructors */
    //@{
    DOMPSVITypeInfo() {};
    //@}

private:
    // -----------------------------------------------------------------------
    // Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    /** @name Unimplemented constructors and operators */
    //@{
    DOMPSVITypeInfo(const DOMPSVITypeInfo &);
    DOMPSVITypeInfo & operator = (const DOMPSVITypeInfo &);
    //@}

public:

    enum PSVIProperty
    {
        PSVI_Validity
        , PSVI_Validitation_Attempted
        , PSVI_Type_Definition_Type
        , PSVI_Type_Definition_Name
        , PSVI_Type_Definition_Namespace
        , PSVI_Type_Definition_Anonymous
        , PSVI_Nil
        , PSVI_Member_Type_Definition_Name
        , PSVI_Member_Type_Definition_Namespace
        , PSVI_Member_Type_Definition_Anonymous
        , PSVI_Schema_Default
        , PSVI_Schema_Normalized_Value
        , PSVI_Schema_Specified
    };

    // -----------------------------------------------------------------------
    //  All constructors are hidden, just the destructor is available
    // -----------------------------------------------------------------------
    /** @name Destructor */
    //@{
    /**
     * Destructor
     *
     */
    virtual ~DOMPSVITypeInfo() {};
    //@}

    //@{
    // -----------------------------------------------------------------------
    //  Getter methods
    // -----------------------------------------------------------------------
    /**
     * Returns the string value of the specified PSVI property associated to a 
     * <code>DOMElement</code> or <code>DOMAttr</code>, or null if not available.
     *
     * <p><b>"Experimental - subject to change"</b></p>
     *
     * @return the string value of the specified PSVI property associated to a 
     * <code>DOMElement</code> or <code>DOMAttr</code>, or null if not available.
     */
    virtual const XMLCh* getStringProperty(PSVIProperty prop) const = 0;

    /**
     * Returns the numeric value of the specified PSVI property associated to a 
     * <code>DOMElement</code> or <code>DOMAttr</code>, or null if not available.
     *
     * <p><b>"Experimental - subject to change"</b></p>
     *
     * @return the numeric value of the specified PSVI property associated to a 
     * <code>DOMElement</code> or <code>DOMAttr</code>, or null if not available.
     */
    virtual int getNumericProperty(PSVIProperty prop) const = 0;
    //@}
};

XERCES_CPP_NAMESPACE_END

#endif

/**
 * End of file DOMPSVITypeInfo.hpp
 */
