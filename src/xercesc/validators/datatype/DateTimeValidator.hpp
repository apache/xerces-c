/*
 * Copyright 2001,2004 The Apache Software Foundation.
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
 * Revision 1.10  2004/09/08 13:56:52  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.9  2004/01/29 11:51:22  cargilld
 * Code cleanup changes to get rid of various compiler diagnostic messages.
 *
 * Revision 1.8  2003/12/17 00:18:38  cargilld
 * Update to memory management so that the static memory manager (one used to call Initialize) is only for static data.
 *
 * Revision 1.7  2003/12/11 21:40:24  peiyongz
 * support for Canonical Representation for Datatype
 *
 * Revision 1.6  2003/11/12 20:32:03  peiyongz
 * Statless Grammar: ValidationContext
 *
 * Revision 1.5  2003/10/02 19:21:06  peiyongz
 * Implementation of Serialization/Deserialization
 *
 * Revision 1.4  2003/08/14 03:00:11  knoaman
 * Code refactoring to improve performance of validation.
 *
 * Revision 1.3  2003/05/15 18:53:26  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.2  2002/11/04 14:53:28  tng
 * C++ Namespace Support.
 *
 * Revision 1.1.1.1  2002/02/01 22:22:41  peiyongz
 * sane_include
 *
 * Revision 1.2  2001/11/12 20:37:57  peiyongz
 * SchemaDateTimeException defined
 *
 * Revision 1.1  2001/11/07 19:18:52  peiyongz
 * DateTime Port
 *
 */

#if !defined(DATETIME_VALIDATOR_HPP)
#define DATETIME_VALIDATOR_HPP

#include <xercesc/validators/datatype/AbstractNumericFacetValidator.hpp>
#include <xercesc/util/XMLDateTime.hpp>

XERCES_CPP_NAMESPACE_BEGIN

class VALIDATORS_EXPORT DateTimeValidator : public AbstractNumericFacetValidator
{
public:

    // -----------------------------------------------------------------------
    //  Public dtor
    // -----------------------------------------------------------------------
	/** @name Constructor. */
    //@{

    virtual ~DateTimeValidator();

	//@}

	virtual void validate
                 (
                  const XMLCh*             const content
                ,       ValidationContext* const context = 0
                ,       MemoryManager*     const manager = XMLPlatformUtils::fgMemoryManager
                  );

    virtual int  compare(const XMLCh* const value1
                       , const XMLCh* const value2
                       , MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager
                       );

    /***
     * Support for Serialization/De-serialization
     ***/
    DECL_XSERIALIZABLE(DateTimeValidator)

protected:

    // -----------------------------------------------------------------------
    //  ctor used by derived class
    // -----------------------------------------------------------------------
    DateTimeValidator
    (
        DatatypeValidator* const baseValidator
        , RefHashTableOf<KVStringPair>* const facets
        , const int finalSet
        , const ValidatorType type
        , MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager
    );

    //
    // Abstract interface
    //

    virtual int  compareValues(const XMLNumber* const lValue
                             , const XMLNumber* const rValue);

    virtual void checkContent(const XMLCh*             const content
                            ,       ValidationContext* const context
                            , bool                           asBase
                            ,       MemoryManager*     const manager);

    virtual void  setMaxInclusive(const XMLCh* const);

    virtual void  setMaxExclusive(const XMLCh* const);

    virtual void  setMinInclusive(const XMLCh* const);

    virtual void  setMinExclusive(const XMLCh* const);

    virtual void  setEnumeration(MemoryManager* const manager);

protected:

    // -----------------------------------------------------------------------
    //  helper interface: to be implemented/overwritten by derived class
    // -----------------------------------------------------------------------
    virtual XMLDateTime*   parse(const XMLCh* const, MemoryManager* const manager) = 0;
    virtual void parse(XMLDateTime* const) = 0;

    // to be overwritten by duration
    virtual int            compareDates(const XMLDateTime* const lValue
                                      , const XMLDateTime* const rValue
                                      , bool strict);

private:
    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    DateTimeValidator(const DateTimeValidator&);
    DateTimeValidator& operator=(const DateTimeValidator&);
};

XERCES_CPP_NAMESPACE_END

#endif

/**
  * End of file DateTimeValidator.hpp
  */


