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
 * Revision 1.11  2004/09/08 13:56:59  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.10  2004/01/29 11:52:32  cargilld
 * Code cleanup changes to get rid of various compiler diagnostic messages.
 *
 * Revision 1.9  2003/11/14 22:35:09  neilg
 * changes in support of second phase of XSModel implementation; thanks to David Cargill
 *
 * Revision 1.8  2003/11/13 23:21:41  peiyongz
 * initSize
 *
 * Revision 1.7  2003/10/17 21:18:04  peiyongz
 * using XTemplateSerializer
 *
 * Revision 1.6  2003/10/14 15:24:23  peiyongz
 * Implementation of Serialization/Deserialization
 *
 * Revision 1.5  2003/10/01 16:32:42  neilg
 * improve handling of out of memory conditions, bug #23415.  Thanks to David Cargill.
 *
 * Revision 1.4  2003/05/15 18:59:34  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.3  2002/11/04 14:47:41  tng
 * C++ Namespace Support.
 *
 * Revision 1.2  2002/09/24 19:57:20  tng
 * Performance: use XMLString::equals instead of XMLString::compareString
 *
 * Revision 1.1.1.1  2002/02/01 22:22:50  peiyongz
 * sane_include
 *
 * Revision 1.2  2001/11/15 17:10:19  knoaman
 * Particle derivation checking support.
 *
 * Revision 1.1  2001/11/02 14:08:40  knoaman
 * Add support for identity constraints.
 *
 */

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/validators/schema/identity/IdentityConstraint.hpp>
#include <xercesc/validators/schema/identity/IC_Selector.hpp>
#include <xercesc/validators/schema/identity/IC_Field.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/OutOfMemoryException.hpp>

//since we need to dynamically created each and every derivatives 
//during deserialization by XSerializeEngine>>Derivative, we got
//to include all hpp
#include <xercesc/validators/schema/identity/IC_Unique.hpp>
#include <xercesc/validators/schema/identity/IC_Key.hpp>
#include <xercesc/validators/schema/identity/IC_KeyRef.hpp>

#include <xercesc/internal/XTemplateSerializer.hpp>

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  IdentityConstraint: Constructors and Destructor
// ---------------------------------------------------------------------------
IdentityConstraint::IdentityConstraint(const XMLCh* const identityConstraintName,
                                       const XMLCh* const elemName,
                                       MemoryManager* const manager)
    : fIdentityConstraintName(0)
    , fElemName(0)
    , fSelector(0)
    , fFields(0)
    , fMemoryManager(manager)
    , fNamespaceURI(-1)
{
    try {
        fIdentityConstraintName = XMLString::replicate(identityConstraintName, fMemoryManager);
        fElemName = XMLString::replicate(elemName, fMemoryManager);
    }
    catch(const OutOfMemoryException&)
    {
        throw;
    }
    catch(...) {

        cleanUp();
        throw;
    }
}


IdentityConstraint::~IdentityConstraint()
{
    cleanUp();
}

// ---------------------------------------------------------------------------
//  IdentityConstraint: operators
// ---------------------------------------------------------------------------
bool IdentityConstraint::operator ==(const IdentityConstraint& other) const {

    if (getType() != other.getType())
        return false;

    if (!XMLString::equals(fIdentityConstraintName, other.fIdentityConstraintName))
        return false;

    if (*fSelector != *(other.fSelector))
        return false;

    unsigned int fieldCount = fFields->size();

    if (fieldCount != other.fFields->size())
        return false;

    for (unsigned int i = 0; i < fieldCount; i++) {
        if (*(fFields->elementAt(i)) != *(other.fFields->elementAt(i)))
            return false;
    }

    return true;
}

bool IdentityConstraint::operator !=(const IdentityConstraint& other) const {

    return !operator==(other);
}

// ---------------------------------------------------------------------------
//  IdentityConstraint: Setter methods
// ---------------------------------------------------------------------------
void IdentityConstraint::setSelector(IC_Selector* const selector) {

    if (fSelector) {
        delete fSelector;
    }

    fSelector = selector;
}


// ---------------------------------------------------------------------------
//  IdentityConstraint: cleanUp methods
// ---------------------------------------------------------------------------
void IdentityConstraint::cleanUp() {

    fMemoryManager->deallocate(fIdentityConstraintName);//delete [] fIdentityConstraintName;
    fMemoryManager->deallocate(fElemName);//delete [] fElemName;
    delete fFields;
    delete fSelector;
}

/***
 * Support for Serialization/De-serialization
 ***/

IMPL_XSERIALIZABLE_NOCREATE(IdentityConstraint)

void IdentityConstraint::serialize(XSerializeEngine& serEng)
{

    if (serEng.isStoring())
    {
        serEng.writeString(fIdentityConstraintName);
        serEng.writeString(fElemName);

        serEng<<fSelector;
        serEng<<fNamespaceURI;
        /***
         *
         * Serialize RefVectorOf<IC_Field>* fFields;
         *
         ***/
        XTemplateSerializer::storeObject(fFields, serEng);
    
    }
    else
    {

        serEng.readString(fIdentityConstraintName);
        serEng.readString(fElemName);

        serEng>>fSelector;
        serEng>>fNamespaceURI;
        /***
         *
         * Deserialize RefVectorOf<IC_Field>* fFields;
         *
         ***/
        XTemplateSerializer::loadObject(&fFields, 4, true, serEng);

    }

}  

void IdentityConstraint::storeIC(XSerializeEngine&         serEng
                               , IdentityConstraint* const ic)
{
    if (ic)
    {
        serEng<<(int) ic->getType();
        serEng<<ic;
    }
    else
    {
        serEng<<(int) UNKNOWN;
    }

}

IdentityConstraint* IdentityConstraint::loadIC(XSerializeEngine& serEng)
{

    int type;
    serEng>>type;

    switch((ICType)type)
    {
    case UNIQUE: 
        IC_Unique* ic_unique;
        serEng>>ic_unique;
        return ic_unique;      
    case KEY:
        IC_Key* ic_key;
        serEng>>ic_key;
        return ic_key;
    case KEYREF: 
        IC_KeyRef* ic_keyref;
        serEng>>ic_keyref;
        return ic_keyref;
    case UNKNOWN:
        return 0;
    default: //we treat this same as UnKnown
        return 0;
    }

}

XERCES_CPP_NAMESPACE_END

/**
  * End of file IdentityConstraint.cpp
  */

