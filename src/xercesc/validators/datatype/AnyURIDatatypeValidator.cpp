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
 * Revision 1.12  2004/09/08 13:56:52  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.11  2004/01/12 23:02:18  neilg
 * remove unused static member
 *
 * Revision 1.10  2003/12/17 00:18:38  cargilld
 * Update to memory management so that the static memory manager (one used to call Initialize) is only for static data.
 *
 * Revision 1.9  2003/10/01 16:32:41  neilg
 * improve handling of out of memory conditions, bug #23415.  Thanks to David Cargill.
 *
 * Revision 1.8  2003/10/01 00:27:12  knoaman
 * Performance: call a static method to check the validity of URI instead of
 * creating/deleting local objects.
 *
 * Revision 1.7  2003/09/30 21:31:30  peiyongz
 * Implementation of Serialization/Deserialization
 *
 * Revision 1.6  2003/05/18 14:02:07  knoaman
 * Memory manager implementation: pass per instance manager.
 *
 * Revision 1.5  2003/05/16 06:01:57  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.4  2003/05/15 18:53:26  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.3  2002/12/18 14:17:55  gareth
 * Fix to bug #13438. When you eant a vector that calls delete[] on its members you should use RefArrayVectorOf.
 *
 * Revision 1.2  2002/11/04 14:53:27  tng
 * C++ Namespace Support.
 *
 * Revision 1.1.1.1  2002/02/01 22:22:40  peiyongz
 * sane_include
 *
 * Revision 1.10  2001/10/10 14:18:26  peiyongz
 * no message
 *
 * Revision 1.9  2001/10/09 20:53:58  peiyongz
 * init(): take 1 arg.
 *
 * Revision 1.8  2001/10/02 18:59:29  peiyongz
 * Invalid_Facet_Tag to display the tag name
 *
 * Revision 1.7  2001/09/24 15:33:15  peiyongz
 * DTV Reorganization: virtual methods moved to *.cpp
 *
 * Revision 1.6  2001/09/19 18:49:17  peiyongz
 * DTV reorganization: move inline to class declaration to avoid inline
 * function interdependency.
 *
 * Revision 1.5  2001/09/18 20:38:03  peiyongz
 * DTV reorganization: inherit from AbstractStringValidator.
 *
 * Revision 1.4  2001/08/21 18:42:53  peiyongz
 * Bugzilla# 2816: cleanUp() declared with external linkage and called
 *                          before defined as inline
 *
 * Revision 1.3  2001/08/14 22:11:56  peiyongz
 * new exception message added
 *
 * Revision 1.2  2001/08/10 16:21:19  peiyongz
 * use XMLUri instead of XMLURL
 *
 * Revision 1.1  2001/08/01 18:49:16  peiyongz
 * AnyRUIDatatypeValidator
 *
 *
 */

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/validators/datatype/AnyURIDatatypeValidator.hpp>
#include <xercesc/validators/datatype/InvalidDatatypeFacetException.hpp>
#include <xercesc/validators/datatype/InvalidDatatypeValueException.hpp>
#include <xercesc/util/OutOfMemoryException.hpp>

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  Constructors and Destructor
// ---------------------------------------------------------------------------
AnyURIDatatypeValidator::AnyURIDatatypeValidator(MemoryManager* const manager)
:AbstractStringValidator(0, 0, 0, DatatypeValidator::AnyURI, manager)
{}

AnyURIDatatypeValidator::~AnyURIDatatypeValidator()
{  
}

AnyURIDatatypeValidator::AnyURIDatatypeValidator(
                          DatatypeValidator*            const baseValidator
                        , RefHashTableOf<KVStringPair>* const facets
                        , RefArrayVectorOf<XMLCh>*      const enums
                        , const int                           finalSet
                        , MemoryManager* const manager)
:AbstractStringValidator(baseValidator, facets, finalSet, DatatypeValidator::AnyURI, manager)
{
    try
    {
        init(enums, manager);
    }
    catch(const OutOfMemoryException&)
    {
        throw;
    }
    catch (...)
    {        
        throw;
    }
}

DatatypeValidator* AnyURIDatatypeValidator::newInstance(
                                      RefHashTableOf<KVStringPair>* const facets
                                    , RefArrayVectorOf<XMLCh>*           const enums
                                    , const int                           finalSet
                                    , MemoryManager* const manager)
{
    return (DatatypeValidator*) new (manager) AnyURIDatatypeValidator(this, facets, enums, finalSet, manager);
}

// ---------------------------------------------------------------------------
//  Utilities
// ---------------------------------------------------------------------------

void AnyURIDatatypeValidator::checkValueSpace(const XMLCh* const content
                                              , MemoryManager* const manager)
{

    // check 3.2.17.c0 must: URI (rfc 2396/2723)
    try
    {
        // Support for relative URLs
        // According to Java 1.1: URLs may also be specified with a
        // String and the URL object that it is related to.
        //
        if (XMLString::stringLen(content))
        {          
              if (!XMLUri::isValidURI(true, content))
                ThrowXMLwithMemMgr1(InvalidDatatypeValueException
                    , XMLExcepts::VALUE_URI_Malformed
                    , content
                    , manager);
        }
    }
    catch(const OutOfMemoryException&)
    {
        throw;
    }
    catch (...)
    {
        ThrowXMLwithMemMgr1(InvalidDatatypeValueException
                , XMLExcepts::VALUE_URI_Malformed
                , content
                , manager);
    }

}

/***
 * Support for Serialization/De-serialization
 ***/

IMPL_XSERIALIZABLE_TOCREATE(AnyURIDatatypeValidator)

void AnyURIDatatypeValidator::serialize(XSerializeEngine& serEng)
{
    AbstractStringValidator::serialize(serEng);
}

XERCES_CPP_NAMESPACE_END

/**
  * End of file AnyURIDatatypeValidator.cpp
  */
