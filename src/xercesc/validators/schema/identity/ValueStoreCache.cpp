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
 * Revision 1.10  2003/12/17 00:18:41  cargilld
 * Update to memory management so that the static memory manager (one used to call Initialize) is only for static data.
 *
 * Revision 1.9  2003/10/01 16:32:42  neilg
 * improve handling of out of memory conditions, bug #23415.  Thanks to David Cargill.
 *
 * Revision 1.8  2003/05/26 22:05:01  knoaman
 * Pass the memory manager to XMLString::replicate.
 *
 * Revision 1.7  2003/05/18 14:02:09  knoaman
 * Memory manager implementation: pass per instance manager.
 *
 * Revision 1.6  2003/05/15 18:59:34  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.5  2002/11/04 14:47:41  tng
 * C++ Namespace Support.
 *
 * Revision 1.4  2002/08/28 13:41:20  knoaman
 * missing type info.
 *
 * Revision 1.3  2002/08/27 05:56:19  knoaman
 * Identity Constraint: handle case of recursive elements.
 *
 * Revision 1.2  2002/08/26 23:48:09  knoaman
 * Fix for ValueStore's null pointer.
 *
 * Revision 1.1.1.1  2002/02/01 22:22:51  peiyongz
 * sane_include
 *
 * Revision 1.1  2001/11/02 14:08:40  knoaman
 * Add support for identity constraints.
 *
 */

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/validators/schema/identity/ValueStoreCache.hpp>
#include <xercesc/validators/schema/identity/ValueStore.hpp>
#include <xercesc/validators/schema/SchemaElementDecl.hpp>
#include <xercesc/util/HashPtr.hpp>
#include <xercesc/util/OutOfMemoryException.hpp>

XERCES_CPP_NAMESPACE_BEGIN

typedef JanitorMemFunCall<ValueStoreCache>    CleanupType;

// ---------------------------------------------------------------------------
//  ValueStoreCache: Constructors and Destructor
// ---------------------------------------------------------------------------
ValueStoreCache::ValueStoreCache(MemoryManager* const manager)
    : fValueStores(0)
    , fGlobalICMap(0)
    , fIC2ValueStoreMap(0)
    , fGlobalMapStack(0)
    , fScanner(0)
    , fMemoryManager(manager)
{
    CleanupType cleanup(this, &ValueStoreCache::cleanUp);

    try {
        init();
    }
    catch(const OutOfMemoryException&)
    {
        cleanup.release();

        throw;
    }

    cleanup.release();
}


ValueStoreCache::~ValueStoreCache()
{
    cleanUp();
}

// ---------------------------------------------------------------------------
//  ValueStoreCache: Document handling methods
// ---------------------------------------------------------------------------
void ValueStoreCache::startDocument() {

    fIC2ValueStoreMap->removeAll();
    fGlobalICMap->removeAll();
    fValueStores->removeAllElements();
    fGlobalMapStack->removeAllElements();
}

void ValueStoreCache::startElement() {

    fGlobalMapStack->push(fGlobalICMap);
    fGlobalICMap = new (fMemoryManager) RefHashTableOf<ValueStore>
    (
        13
        , false
        , new (fMemoryManager) HashPtr()
        , fMemoryManager
    );
}

void ValueStoreCache::endElement() {

    if (fGlobalMapStack->empty()) {
        return; // must be an invalid doc!
    }

    RefHashTableOf<ValueStore>* oldMap = fGlobalMapStack->pop();
    RefHashTableOfEnumerator<ValueStore> mapEnum(oldMap, false, fMemoryManager);
//    Janitor<RefHashTableOf<ValueStore> > janMap(oldMap);

    while (mapEnum.hasMoreElements()) {

        ValueStore& oldVal = mapEnum.nextElement();
        IdentityConstraint* ic = oldVal.getIdentityConstraint();
        ValueStore* currVal = fGlobalICMap->get(ic);

        if (!currVal) {
            fGlobalICMap->put(ic, &oldVal);
        }
        else {
            currVal->append(&oldVal);
        }
    }
    delete oldMap;
}

// ---------------------------------------------------------------------------
//  ValueStoreCache: Helper methods
// ---------------------------------------------------------------------------
void ValueStoreCache::cleanUp() {

    delete fIC2ValueStoreMap;
    delete fGlobalICMap;
    delete fGlobalMapStack;
    delete fValueStores;
}

void ValueStoreCache::init() {

    fValueStores = new (fMemoryManager) RefVectorOf<ValueStore>(8, true, fMemoryManager);
    fGlobalICMap = new (fMemoryManager) RefHashTableOf<ValueStore>
    (
        13
        , false
        , new (fMemoryManager) HashPtr()
        , fMemoryManager
    );
    fIC2ValueStoreMap = new (fMemoryManager) RefHash2KeysTableOf<ValueStore>
    (
        13
        , false
        , new (fMemoryManager) HashPtr()
        , fMemoryManager
    );
    fGlobalMapStack = new (fMemoryManager) RefStackOf<RefHashTableOf<ValueStore> >(8, true, fMemoryManager);
}

void ValueStoreCache::initValueStoresFor(SchemaElementDecl* const elemDecl,
                                         const int initialDepth) {

    // initialize value stores for unique fields
    unsigned int icCount = elemDecl->getIdentityConstraintCount();

    for (unsigned int i=0; i<icCount; i++) {

        IdentityConstraint* ic = elemDecl->getIdentityConstraintAt(i);
        ValueStore* valueStore = new (fMemoryManager) ValueStore(ic, fScanner, fMemoryManager);
        fValueStores->addElement(valueStore);
        fIC2ValueStoreMap->put(ic, initialDepth, valueStore);
    }
}

void ValueStoreCache::transplant(IdentityConstraint* const ic, const int initialDepth) {

    if (ic->getType() == IdentityConstraint::KEYREF) {
        return;
    }

    ValueStore* newVals = fIC2ValueStoreMap->get(ic, initialDepth);
    ValueStore* currVals = fGlobalICMap->get(ic);

    if (currVals) {
        currVals->append(newVals);
    } else {
        ValueStore* valueStore = new (fMemoryManager) ValueStore(ic, fScanner, fMemoryManager);
        fValueStores->addElement(valueStore);
        valueStore->append(newVals);
        fGlobalICMap->put(ic, valueStore);
    }
}

XERCES_CPP_NAMESPACE_END

/**
  * End of file ValueStoreCache.cpp
  */

