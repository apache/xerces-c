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
 * $Log$
 * Revision 1.5  2004/09/08 13:56:59  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.4  2003/10/14 15:24:23  peiyongz
 * Implementation of Serialization/Deserialization
 *
 * Revision 1.3  2003/05/15 18:59:34  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.2  2002/11/04 14:47:41  tng
 * C++ Namespace Support.
 *
 * Revision 1.1.1.1  2002/02/01 22:22:50  peiyongz
 * sane_include
 *
 * Revision 1.1  2001/11/02 14:08:40  knoaman
 * Add support for identity constraints.
 *
 */

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/validators/schema/identity/IC_KeyRef.hpp>

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  IC_KeyRef: Constructors and Destructor
// ---------------------------------------------------------------------------
IC_KeyRef::IC_KeyRef(const XMLCh* const identityConstraintName,
                     const XMLCh* const elemName,
                     IdentityConstraint* const icKey,
                     MemoryManager* const manager)
    : IdentityConstraint(identityConstraintName, elemName, manager)
    , fKey(icKey)
{
}


IC_KeyRef::~IC_KeyRef()
{
}

/***
 * Support for Serialization/De-serialization
 ***/

IMPL_XSERIALIZABLE_TOCREATE(IC_KeyRef)

void IC_KeyRef::serialize(XSerializeEngine& serEng)
{
    IdentityConstraint::serialize(serEng);

    if (serEng.isStoring())
    {
        IdentityConstraint::storeIC(serEng, fKey);
    }
    else
    {
        fKey = IdentityConstraint::loadIC(serEng);
    }

}

IC_KeyRef::IC_KeyRef(MemoryManager* const manager)
:IdentityConstraint(0, 0, manager)
,fKey(0)
{
}

XERCES_CPP_NAMESPACE_END

/**
  * End of file IC_KeyRef.cpp
  */


