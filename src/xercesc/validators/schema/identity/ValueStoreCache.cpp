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
 * $Log$
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

// ---------------------------------------------------------------------------
//  ValueStoreCache: Constructors and Destructor
// ---------------------------------------------------------------------------
ValueStoreCache::ValueStoreCache()
    : fValueStores(0)
    , fGlobalICMap(0)
    , fIC2ValueStoreMap(0)
    , fGlobalMapStack(0)
{
    try {
        init();
    }
    catch(...) {
        cleanUp();
        throw;
    }
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
    fGlobalICMap = new RefHashTableOf<ValueStore>(13, false, new HashPtr());
}

void ValueStoreCache::endElement() {

    if (fGlobalMapStack->empty()) {
        return; // must be an invalid doc!
    }

    RefHashTableOf<ValueStore>* oldMap = fGlobalMapStack->pop();
    RefHashTableOfEnumerator<ValueStore> mapEnum(oldMap);
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

    fValueStores = new RefVectorOf<ValueStore>(8);
    fGlobalICMap = new RefHashTableOf<ValueStore>(13, false, new HashPtr());
    fIC2ValueStoreMap = new RefHashTableOf<ValueStore>(13, false, new HashPtr());
    fGlobalMapStack = new RefStackOf<RefHashTableOf<ValueStore> >(8);
}

void ValueStoreCache::initValueStoresFor(SchemaElementDecl* const elemDecl) {

    // initialize value stores for unique fields
    unsigned int icCount = elemDecl->getIdentityConstraintCount();

    for (unsigned int i=0; i<icCount; i++) {

        IdentityConstraint* ic = elemDecl->getIdentityConstraintAt(i);
        ValueStore* valueStore = fIC2ValueStoreMap->get(ic);

        if (valueStore && ic->getType() != IdentityConstraint::KEYREF) {
            continue;
        }

        valueStore = new ValueStore(ic, fScanner);
        fValueStores->addElement(valueStore);
        fIC2ValueStoreMap->put(ic, valueStore);
    }
}

void ValueStoreCache::transplant(IdentityConstraint* const ic) {

    if (ic->getType() == IdentityConstraint::KEYREF) {
        return;
    }

    ValueStore* newVals = fIC2ValueStoreMap->get(ic);
    ValueStore* currVals = fGlobalICMap->get(ic);

    fIC2ValueStoreMap->removeKey(ic);

    if (currVals) {
        currVals->append(newVals);
    } else {
        fGlobalICMap->put(ic, newVals);
    }

    ValueStore* valueStore = new ValueStore(ic, fScanner);
    fValueStores->addElement(valueStore);
    fIC2ValueStoreMap->put(ic, valueStore);
}

/**
  * End of file ValueStoreCache.cpp
  */

