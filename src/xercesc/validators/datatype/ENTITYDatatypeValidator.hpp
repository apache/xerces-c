/*
 * Copyright 2001-2004 The Apache Software Foundation.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *      http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * $Id$
 * $Log$
 * Revision 1.9  2004/09/08 13:56:53  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.8  2004/01/29 11:51:22  cargilld
 * Code cleanup changes to get rid of various compiler diagnostic messages.
 *
 * Revision 1.7  2003/12/17 00:18:39  cargilld
 * Update to memory management so that the static memory manager (one used to call Initialize) is only for static data.
 *
 * Revision 1.6  2003/11/12 20:32:03  peiyongz
 * Statless Grammar: ValidationContext
 *
 * Revision 1.5  2003/09/30 18:17:53  peiyongz
 * Implementation of Serialization/Deserialization
 *
 * Revision 1.4  2003/05/15 18:53:26  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.3  2002/12/18 14:17:55  gareth
 * Fix to bug #13438. When you eant a vector that calls delete[] on its members you should use RefArrayVectorOf.
 *
 * Revision 1.2  2002/11/04 14:53:28  tng
 * C++ Namespace Support.
 *
 * Revision 1.1.1.1  2002/02/01 22:22:41  peiyongz
 * sane_include
 *
 * Revision 1.7  2001/11/22 20:23:20  peiyongz
 * _declspec(dllimport) and inline warning C4273
 *
 * Revision 1.6  2001/10/09 20:50:27  peiyongz
 * init(): take 1 arg
 *
 * Revision 1.5  2001/09/27 13:51:25  peiyongz
 * DTV Reorganization: ctor/init created to be used by derived class
 *
 * Revision 1.4  2001/09/24 21:39:29  peiyongz
 * DTV Reorganization: checkValueSpace()
 *
 * Revision 1.3  2001/08/24 17:12:01  knoaman
 * Add support for anySimpleType.
 * Remove parameter 'baseValidator' from the virtual method 'newInstance'.
 *
 * Revision 1.2  2001/07/24 21:23:40  tng
 * Schema: Use DatatypeValidator for ID/IDREF/ENTITY/ENTITIES/NOTATION.
 *
 * Revision 1.1  2001/07/06 20:19:52  peiyongz
 * ENTITYDTV
 *
 */

#if !defined(ENTITY_DATATYPEVALIDATOR_HPP)
#define ENTITY_DATATYPEVALIDATOR_HPP

#include <xercesc/validators/datatype/StringDatatypeValidator.hpp>
#include <xercesc/util/NameIdPool.hpp>
#include <xercesc/validators/DTD/DTDEntityDecl.hpp>

XERCES_CPP_NAMESPACE_BEGIN

class VALIDATORS_EXPORT ENTITYDatatypeValidator : public StringDatatypeValidator
{
public:

    // -----------------------------------------------------------------------
    //  Public ctor/dtor
    // -----------------------------------------------------------------------
	/** @name Constructors and Destructor. */
    //@{

    ENTITYDatatypeValidator
    (
        MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager
    );
    ENTITYDatatypeValidator
    (
        DatatypeValidator* const baseValidator
        , RefHashTableOf<KVStringPair>* const facets
        , RefArrayVectorOf<XMLCh>* const enums
        , const int finalSet
        , MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager
    );

    virtual ~ENTITYDatatypeValidator();

	//@}

    // -----------------------------------------------------------------------
    // Validation methods
    // -----------------------------------------------------------------------
    /** @name Validation Function */
    //@{

    /**
     * validate that a string matches the boolean datatype
     * @param content A string containing the content to be validated
     *
     * @exception throws InvalidDatatypeException if the content is
     * is not valid.
     */

	virtual void validate
                 (
                  const XMLCh*             const content
                ,       ValidationContext* const context = 0
                ,       MemoryManager*     const manager = XMLPlatformUtils::fgMemoryManager
                  );

    //@}

    // -----------------------------------------------------------------------
    // Compare methods
    // -----------------------------------------------------------------------
    /** @name Compare Function */
    //@{

    /**
     * Compare two boolean data types
     *
     * @param content1
     * @param content2
     * @return
     */
    virtual int compare(const XMLCh* const, const XMLCh* const
        ,       MemoryManager*     const manager = XMLPlatformUtils::fgMemoryManager);

    //@}

    /**
      * Returns an instance of the base datatype validator class
	  * Used by the DatatypeValidatorFactory.
      */
    virtual DatatypeValidator* newInstance
    (
        RefHashTableOf<KVStringPair>* const facets
        , RefArrayVectorOf<XMLCh>* const enums
        , const int finalSet
        , MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager
    );

// -----------------------------------------------------------------------
// Setter methods
// -----------------------------------------------------------------------
    /** @name Setter Functions */
    //@{

    //deprecated
    inline void    setEntityDeclPool(NameIdPool<DTDEntityDecl>* const entityDeclPool);

    //@}

    /***
     * Support for Serialization/De-serialization
     ***/
    DECL_XSERIALIZABLE(ENTITYDatatypeValidator)

protected:

    //
    // ctor provided to be used by derived classes
    //
    ENTITYDatatypeValidator(DatatypeValidator*            const baseValidator
                          , RefHashTableOf<KVStringPair>* const facets
                          , const int                           finalSet
                          , const ValidatorType                 type);

    virtual void checkValueSpace(const XMLCh* const content
                                , MemoryManager* const manager);

private:
    // -----------------------------------------------------------------------
    // Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    ENTITYDatatypeValidator(const ENTITYDatatypeValidator&);
    ENTITYDatatypeValidator& operator = (const  ENTITYDatatypeValidator&);

};

// -----------------------------------------------------------------------
// Setter methods
// -----------------------------------------------------------------------
inline void ENTITYDatatypeValidator::setEntityDeclPool(NameIdPool<DTDEntityDecl>* const )
{
}

XERCES_CPP_NAMESPACE_END

#endif

/**
  * End of file ENTITYDatatypeValidator.hpp
  */
