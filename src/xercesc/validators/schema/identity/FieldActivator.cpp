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
 * Revision 1.10  2005/01/07 15:28:49  amassari
 * Removed warnings
 *
 * Revision 1.9  2004/09/08 13:56:59  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.8  2003/12/17 01:13:10  cargilld
 * Fix memhandlertest failure (memory not deleted).
 *
 * Revision 1.7  2003/12/17 00:18:41  cargilld
 * Update to memory management so that the static memory manager (one used to call Initialize) is only for static data.
 *
 * Revision 1.6  2003/12/16 18:41:15  knoaman
 * Make IC_Field stateless
 *
 * Revision 1.5  2003/05/22 02:10:52  knoaman
 * Default the memory manager.
 *
 * Revision 1.4  2003/05/15 18:59:34  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.3  2002/11/04 14:47:41  tng
 * C++ Namespace Support.
 *
 * Revision 1.2  2002/08/27 05:56:19  knoaman
 * Identity Constraint: handle case of recursive elements.
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
#include <xercesc/validators/schema/identity/FieldActivator.hpp>
#include <xercesc/validators/schema/identity/ValueStore.hpp>
#include <xercesc/validators/schema/identity/ValueStoreCache.hpp>
#include <xercesc/validators/schema/identity/XPathMatcherStack.hpp>
#include <xercesc/util/HashPtr.hpp>

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  FieldActivator: Constructors and Destructor
// ---------------------------------------------------------------------------
FieldActivator::FieldActivator(ValueStoreCache* const valueStoreCache,
                               XPathMatcherStack* const matcherStack,
                               MemoryManager* const manager)
    : fValueStoreCache(valueStoreCache)
    , fMatcherStack(matcherStack)
    , fMayMatch(0)
    , fMemoryManager(manager)
{
    fMayMatch = new (manager) ValueHashTableOf<bool>(29, new (manager) HashPtr(), manager);
}

FieldActivator::FieldActivator(const FieldActivator& other)
    : XMemory(other)
    , fValueStoreCache(other.fValueStoreCache)
    , fMatcherStack(other.fMatcherStack)
    , fMayMatch(0)
    , fMemoryManager(other.fMemoryManager)
{
    fMayMatch = new (fMemoryManager) ValueHashTableOf<bool>(29, new (fMemoryManager) HashPtr(), fMemoryManager);
    ValueHashTableOfEnumerator<bool> mayMatchEnum(other.fMayMatch, false, fMemoryManager);

    // Build key set
    while (mayMatchEnum.hasMoreElements())
    {
        IC_Field* field = (IC_Field*) mayMatchEnum.nextElementKey();
        fMayMatch->put(field, other.fMayMatch->get(field));
    }
}


FieldActivator::~FieldActivator()
{
    delete fMayMatch;
}

// ---------------------------------------------------------------------------
//  FieldActivator: Operator methods
// ---------------------------------------------------------------------------
FieldActivator& FieldActivator::operator =(const FieldActivator& other) {

    if (this == &other) {
        return *this;
    }

    fValueStoreCache = other.fValueStoreCache;
    fMatcherStack = other.fMatcherStack;
    return *this;
}

// ---------------------------------------------------------------------------
//  FieldActivator: Operator methods
// ---------------------------------------------------------------------------
XPathMatcher* FieldActivator::activateField(IC_Field* const field, const int initialDepth) {

    ValueStore* valueStore = fValueStoreCache->getValueStoreFor(field, initialDepth);
    XPathMatcher* matcher = field->createMatcher(this, valueStore, fMemoryManager);

    setMayMatch(field, true);
    fMatcherStack->addMatcher(matcher);
    matcher->startDocumentFragment();

    return matcher;
}

void FieldActivator::startValueScopeFor(const IdentityConstraint* const ic,
                                        const int initialDepth) {

    unsigned int fieldCount = ic->getFieldCount();

    for(unsigned int i=0; i<fieldCount; i++) {

        const IC_Field* field = ic->getFieldAt(i);
        ValueStore* valueStore = fValueStoreCache->getValueStoreFor(field, initialDepth);

        valueStore->startValueScope();
    }
}

void FieldActivator::endValueScopeFor(const IdentityConstraint* const ic, const int initialDepth) {

    ValueStore* valueStore = fValueStoreCache->getValueStoreFor(ic, initialDepth);

    valueStore->endValueScope();
}

XERCES_CPP_NAMESPACE_END

/**
  * End of file FieldActivator.cpp
  */

