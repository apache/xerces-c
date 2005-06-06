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
 * Revision 1.9  2005/01/07 15:28:49  amassari
 * Removed warnings
 *
 * Revision 1.8  2004/09/08 13:56:59  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.7  2003/10/01 16:32:42  neilg
 * improve handling of out of memory conditions, bug #23415.  Thanks to David Cargill.
 *
 * Revision 1.6  2003/05/22 02:10:52  knoaman
 * Default the memory manager.
 *
 * Revision 1.5  2003/05/18 14:02:09  knoaman
 * Memory manager implementation: pass per instance manager.
 *
 * Revision 1.4  2003/05/15 18:59:34  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.3  2002/12/18 14:17:59  gareth
 * Fix to bug #13438. When you eant a vector that calls delete[] on its members you should use RefArrayVectorOf.
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
#include <xercesc/validators/schema/identity/FieldValueMap.hpp>
#include <xercesc/util/Janitor.hpp>
#include <xercesc/util/OutOfMemoryException.hpp>

XERCES_CPP_NAMESPACE_BEGIN

typedef JanitorMemFunCall<FieldValueMap>    CleanupType;

// ---------------------------------------------------------------------------
//  FieldValueMap: Constructors and Destructor
// ---------------------------------------------------------------------------
FieldValueMap::FieldValueMap(MemoryManager* const manager)
    : fFields(0)
    , fValidators(0)
    , fValues(0)
    , fMemoryManager(manager)
{
}

FieldValueMap::FieldValueMap(const FieldValueMap& other)
    : XMemory(other)
    , fFields(0)
    , fValidators(0)
    , fValues(0)
    , fMemoryManager(other.fMemoryManager)
{
    if (other.fFields) {
        CleanupType cleanup(this, &FieldValueMap::cleanUp);

        try {

                unsigned int valuesSize = other.fValues->size();

                fFields = new (fMemoryManager) ValueVectorOf<IC_Field*>(*(other.fFields));
                fValidators = new (fMemoryManager) ValueVectorOf<DatatypeValidator*>(*(other.fValidators));
                fValues = new (fMemoryManager) RefArrayVectorOf<XMLCh>(other.fFields->curCapacity(), true, fMemoryManager);

                for (unsigned int i=0; i<valuesSize; i++) {
                    fValues->addElement(XMLString::replicate(other.fValues->elementAt(i), fMemoryManager));
                }
        }
        catch(const OutOfMemoryException&)
        {
            cleanup.release();

            throw;
        }

        cleanup.release();
    }
}

FieldValueMap::~FieldValueMap()
{
    cleanUp();
}

// ---------------------------------------------------------------------------
//  FieldValueMap: Private helper methods.
// ---------------------------------------------------------------------------
void FieldValueMap::cleanUp()
{
    delete fFields;
    delete fValidators;
    delete fValues;
}

// ---------------------------------------------------------------------------
//  FieldValueMap: Helper methods
// ---------------------------------------------------------------------------
int FieldValueMap::indexOf(const IC_Field* const key) const {

    if (fFields) {

        unsigned int fieldSize = fFields->size();

        for (unsigned int i=0; i < fieldSize; i++) {
            if (fFields->elementAt(i) == key) {
                return i;
            }
        }
    }

    return -1;
}

XERCES_CPP_NAMESPACE_END

/**
  * End of file FieldValueMap.cpp
  */

