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
 * originally based on software copyright (c) 1999, International
 * Business Machines, Inc., http://www.ibm.com .  For more information
 * on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 */

/*
 * $Id$
 * $Log$
 * Revision 1.1  2002/02/01 22:22:40  peiyongz
 * Initial revision
 *
 * Revision 1.8  2001/11/22 20:23:20  peiyongz
 * _declspec(dllimport) and inline warning C4273
 *
 * Revision 1.7  2001/09/24 15:33:15  peiyongz
 * DTV Reorganization: virtual methods moved to *.cpp
 *
 * Revision 1.6  2001/09/19 18:49:17  peiyongz
 * DTV reorganization: move inline to class declaration to avoid inline
 * function interdependency.
 *
 * Revision 1.5  2001/09/18 20:38:02  peiyongz
 * DTV reorganization: inherit from AbstractStringValidator.
 *
 * Revision 1.4  2001/08/24 17:12:01  knoaman
 * Add support for anySimpleType.
 * Remove parameter 'baseValidator' from the virtual method 'newInstance'.
 *
 * Revision 1.3  2001/08/21 18:42:53  peiyongz
 * Bugzilla# 2816: cleanUp() declared with external linkage and called
 *                          before defined as inline
 *
 * Revision 1.2  2001/08/10 16:21:19  peiyongz
 * use XMLUri instead of XMLURL
 *
 * Revision 1.1  2001/08/01 18:49:16  peiyongz
 * AnyRUIDatatypeValidator
 *
 *
 */

#if !defined(ANYURI_DATATYPEVALIDATOR_HPP)
#define ANYURI_DATATYPEVALIDATOR_HPP

#include <xercesc/validators/datatype/AbstractStringValidator.hpp>
#include <xercesc/util/XMLUri.hpp>

class VALIDATORS_EXPORT AnyURIDatatypeValidator : public AbstractStringValidator
{
public:

    // -----------------------------------------------------------------------
    //  Public ctor/dtor
    // -----------------------------------------------------------------------
	/** @name Constructor. */
    //@{

    AnyURIDatatypeValidator();

    AnyURIDatatypeValidator(DatatypeValidator*            const baseValidator
                          , RefHashTableOf<KVStringPair>* const facets 
                          , RefVectorOf<XMLCh>*           const enums                          
                          , const int                           finalSet);

    virtual ~AnyURIDatatypeValidator();

	//@}

    /**
      * Returns an instance of the base datatype validator class
	  * Used by the DatatypeValidatorFactory.
      */
    virtual DatatypeValidator* newInstance(RefHashTableOf<KVStringPair>* const facets
                                         , RefVectorOf<XMLCh>*           const enums
                                         , const int                           finalSet);

protected:

    virtual void assignAdditionalFacet(const XMLCh* const key
                                     , const XMLCh* const value);

    virtual void inheritAdditionalFacet();

    virtual void checkAdditionalFacetConstraints() const;

    virtual void checkAdditionalFacet(const XMLCh* const content) const;

    virtual void checkValueSpace(const XMLCh* const content);

    virtual int  getLength(const XMLCh* const content) const;

private:

    inline void cleanUp();

    // -----------------------------------------------------------------------
    //  Private data members
    //
    //  fTempURI
    //      to support relative URI, such as the urispec= "\sample"
	//		
    // -----------------------------------------------------------------------    

    XMLUri       *fTempURI;   

};

inline void AnyURIDatatypeValidator::cleanUp()
{
    if (fTempURI)
    {
        delete fTempURI;
        fTempURI = 0;
    }
}

/**
  * End of file AnyURIDatatypeValidator.hpp
  */
#endif
