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
 * $Id$
 */

#if !defined(DATATYPEVALIDATOR_HPP)
#define DATATYPEVALIDATOR_HPP

#include <util/RefHashTableOf.hpp>
#include <util/KVStringPair.hpp>

/**
  * DataTypeValidator defines the interface that data type validators must 
  * obey. These validators can be supplied by the application writer and may
  * be useful as standalone code as well as plugins to the validator 
  * architecture.
  *
  * Notice:
  * The datatype validator will own the facets hashtable passed to it during
  * construction, which means that the datatype validator will be responsible
  * for the deletion. The facets hashtable will be created during parsing and
  * passed to the appropriate datatype validator which in turn will delete it
  * upon its destruction.
  *
  */

class VALIDATORS_EXPORT DatatypeValidator
{
public:
    // -----------------------------------------------------------------------
    // Constant data
    // -----------------------------------------------------------------------
	//facets
	enum {
        FACET_LENGTH       = 1,
        FACET_MINLENGTH    = 1<<1,
        FACET_MAXLENGTH    = 1<<2,
        FACET_PATTERN      = 1<<3, 
        FACET_ENUMERATION  = 1<<4,
        FACET_MAXINCLUSIVE = 1<<5,
        FACET_MAXEXCLUSIVE = 1<<6,
        FACET_MININCLUSIVE = 1<<7,
        FACET_MINEXCLUSIVE = 1<<8,
        FACET_PRECISSION   = 1<<9,
        FACET_SCALE        = 1<<10,
        FACET_ENCODING     = 1<<11,
        FACET_DURATION     = 1<<12,
        FACET_PERIOD       = 1<<13,
        FACET_WHITESPACE   = 1<<14
    };

    //2.4.2.6 whiteSpace - Datatypes 
	enum {
        PRESERVE = 0,
        REPLACE  = 1,
        COLLAPSE = 2
    };

	// -----------------------------------------------------------------------
    //  Public Destructor
    // -----------------------------------------------------------------------
	/** @name Destructor. */
    //@{

    virtual ~DatatypeValidator();

	//@}

    // -----------------------------------------------------------------------
    // Getter methods
    // -----------------------------------------------------------------------
    /** @name Getter Functions */
    //@{

    /**
      * Returns the datatype facet if any is set.
	  */
	RefHashTableOf<KVStringPair>* getFacets() const;

    /**
      * Returns default value (collapse) for whiteSpace facet.
      * This function is overwritten in StringDatatypeValidator.
      */
    virtual short getWSFacet () const;

    /**
      * Returns the base datatype validator if set.
      */
    DatatypeValidator* getBaseValidator() const;

    //@}

    // -----------------------------------------------------------------------
    // Validation methods
    // -----------------------------------------------------------------------
    /** @name Validation Function */
    //@{

     /** 
	   * Checks that the "content" string is valid datatype.
       * If invalid, a Datatype validation exception is thrown.
	   *
	   * @param  content   A string containing the content to be validated
	   *
	   */
	virtual void validate(const XMLCh* const content) = 0;

	 //@}

    // -----------------------------------------------------------------------
    // Compare methods
    // -----------------------------------------------------------------------
    /** @name Compare Function */
    //@{

    /**
      * Compares content in the Domain value vs. lexical value.
      * 
      * e.g. If type is a float then 1.0 may be equivalent to 1 even though
      * both are lexically different.
      *
      * @param  value1    string to compare
      *
      * @param  value2    string to compare
      *
      * We will provide a default behavior that should be redefined at the
      * children level, if necessary (i.e. boolean case).
      */
    int compare(const XMLCh* const value1, const XMLCh* const value2);

    //@}

protected:
    // -----------------------------------------------------------------------
    //  Protected Constructors
    // -----------------------------------------------------------------------
    /** @name Constructors */
    //@{

	/**
      *
      * @param  baseValidator  The base datatype validator for derived
      *                        validators. Null if native validator.
      *
      * @param  facets         A hashtable of datatype facets.
	  *
      */
	DatatypeValidator(DatatypeValidator* const baseValidator,
                      RefHashTableOf<KVStringPair>* const facets);

    //@}

    /**
      * Returns an instance of the base datatype validator class
	  * Used by the DatatypeValidatorFactory.
      */
	virtual DatatypeValidator* newInstance(DatatypeValidator* const,
                                           RefHashTableOf<KVStringPair>* const) = 0;

	friend class DatatypeValidatorFactory;

	/**
	  * Process the WHITESPACE facet if passed by user.
      * For all datatypes (exception of string), we don't need to pass 
	  * the WHITESPACE facet as its value is always 'collapse' and cannot
	  * be reset by user.
	  *
	  * To be overwritten in the StringDatatypeValidator
      */
	virtual void processWhiteSpace(RefHashTableOf<KVStringPair>* const facets);

private:
    // -----------------------------------------------------------------------
    //  CleanUp methods
    // -----------------------------------------------------------------------
	void cleanUp();

    // -----------------------------------------------------------------------
    //  Private data members
    //
    //  fBaseValidator
    //      This is a pointer to a base datatype validator. If value is null,
	//      it means we have a native datatype validator not a derived one.
	//		
    //  fFacets
    //      This is a hashtable of dataype facets.
    // -----------------------------------------------------------------------
	DatatypeValidator*            fBaseValidator;
	RefHashTableOf<KVStringPair>* fFacets;
};


// ---------------------------------------------------------------------------
//  DatatypeValidator: Getters
// ---------------------------------------------------------------------------
inline RefHashTableOf<KVStringPair>* DatatypeValidator::getFacets() const {

    return fFacets;
}

inline DatatypeValidator* DatatypeValidator::getBaseValidator() const {

	return fBaseValidator;
}

inline short DatatypeValidator::getWSFacet() const {

    return COLLAPSE;
}


// ---------------------------------------------------------------------------
//  DatatypeValidator: CleanUp methods
// ---------------------------------------------------------------------------
inline void DatatypeValidator::cleanUp() {

	delete fFacets;
}


// ---------------------------------------------------------------------------
//  DatatypeValidators: Compare methods
// ---------------------------------------------------------------------------
inline int DatatypeValidator::compare(const XMLCh* const value1, 
                                      const XMLCh* const value2)
{
    return XMLString::compareString(value1, value2);
}

#endif

/**
  * End of file DatatypeValidator.hpp
  */

