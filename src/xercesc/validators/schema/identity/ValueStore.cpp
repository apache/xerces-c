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
 * Revision 1.9  2003/12/17 00:18:41  cargilld
 * Update to memory management so that the static memory manager (one used to call Initialize) is only for static data.
 *
 * Revision 1.8  2003/12/16 18:41:15  knoaman
 * Make IC_Field stateless
 *
 * Revision 1.7  2003/05/18 14:02:09  knoaman
 * Memory manager implementation: pass per instance manager.
 *
 * Revision 1.6  2003/05/15 18:59:34  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.5  2002/12/04 02:32:43  knoaman
 * #include cleanup.
 *
 * Revision 1.4  2002/11/04 14:47:41  tng
 * C++ Namespace Support.
 *
 * Revision 1.3  2002/09/24 19:57:20  tng
 * Performance: use XMLString::equals instead of XMLString::compareString
 *
 * Revision 1.2  2002/02/18 06:26:50  jberry
 * Quiet codewarrior compiler warnings
 *
 * Revision 1.1.1.1  2002/02/01 22:22:51  peiyongz
 * sane_include
 *
 * Revision 1.3  2001/12/06 17:03:37  knoaman
 * Identity Constraint: fix for xpath expressions containing union operator(s).
 *
 * Revision 1.2  2001/11/20 20:32:52  knoaman
 * Bypass validating element's simple content if it's empty and element is nillable.
 *
 * Revision 1.1  2001/11/02 14:08:40  knoaman
 * Add support for identity constraints.
 *
 */

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/internal/XMLScanner.hpp>
#include <xercesc/framework/XMLValidator.hpp>
#include <xercesc/validators/datatype/DatatypeValidator.hpp>
#include <xercesc/validators/schema/identity/FieldActivator.hpp>
#include <xercesc/validators/schema/identity/ValueStore.hpp>
#include <xercesc/validators/schema/identity/IC_Field.hpp>
#include <xercesc/validators/schema/identity/IC_KeyRef.hpp>
#include <xercesc/validators/schema/identity/ValueStoreCache.hpp>

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  ValueStore: Constructors and Destructor
// ---------------------------------------------------------------------------
ValueStore::ValueStore(IdentityConstraint* const ic,
                       XMLScanner* const scanner,
                       MemoryManager* const manager)
    : fDoReportError(false)
    , fValuesCount(0)
    , fIdentityConstraint(ic)
    , fValues(manager)
    , fValueTuples(0)
    , fKeyValueStore(0)
    , fScanner(scanner)
    , fMemoryManager(manager)
{
	fDoReportError = (scanner && scanner->getDoValidation());
}


ValueStore::~ValueStore()
{
    delete fValueTuples;
}

// ---------------------------------------------------------------------------
//  ValueStore: Helper methods
// ---------------------------------------------------------------------------
void ValueStore::addValue(IC_Field* const,
                          DatatypeValidator* const,
                          const XMLCh* const) {

}

void ValueStore::addValue(FieldActivator* const fieldActivator,
                          IC_Field* const field,
                          DatatypeValidator* const dv,
                          const XMLCh* const value) {

    if (!fieldActivator->getMayMatch(field) && fDoReportError) {
        fScanner->getValidator()->emitError(XMLValid::IC_FieldMultipleMatch);
    }

    // do we even know this field?
    int index = fValues.indexOf(field);

    if (index == -1) {

        if (fDoReportError) {
           fScanner->getValidator()->emitError(XMLValid::IC_UnknownField);
        }

        return;
    }

    // store value
    if (!fValues.getDatatypeValidatorAt(index) &&
        !fValues.getValueAt(index)) {
        fValuesCount++;
    }

    fValues.put(field, dv, value);

    if (fValuesCount == (int) fValues.size()) {

        // is this value as a group duplicated?
        if (contains(&fValues)) {
            duplicateValue();
        }

        // store values
        if (!fValueTuples) {
            fValueTuples = new (fMemoryManager) RefVectorOf<FieldValueMap>(4, true, fMemoryManager);
        }

        fValueTuples->addElement(new (fMemoryManager) FieldValueMap(fValues));
    }
}

void ValueStore::append(const ValueStore* const other) {

    if (!other->fValueTuples) {
        return;
    }

    unsigned int tupleSize = other->fValueTuples->size();

    for (unsigned int i=0; i<tupleSize; i++) {

	    FieldValueMap* valueMap = other->fValueTuples->elementAt(i);

        if (!contains(valueMap)) {

            if (!fValueTuples) {
                fValueTuples = new (fMemoryManager) RefVectorOf<FieldValueMap>(4, true, fMemoryManager);
            }

            fValueTuples->addElement(new (fMemoryManager) FieldValueMap(*valueMap));
        }
    }
}

void ValueStore::startValueScope() {

    fValuesCount = 0;

    int count = fIdentityConstraint->getFieldCount();

    for (int i = 0; i < count; i++) {
        fValues.put(fIdentityConstraint->getFieldAt(i), 0, 0);
    }
}

void ValueStore::endValueScope() {

    if (fValuesCount == 0) {

        if (fIdentityConstraint->getType() == IdentityConstraint::KEY && fDoReportError) {
            fScanner->getValidator()->emitError(XMLValid::IC_AbsentKeyValue,
                fIdentityConstraint->getElementName());
        }

        return;
    }

    // do we have enough values?
    if ((fValuesCount != fIdentityConstraint->getFieldCount()) && fDoReportError) {

        switch (fIdentityConstraint->getType()) {
        case IdentityConstraint::UNIQUE:
            {
				fScanner->getValidator()->emitError(XMLValid::IC_UniqueNotEnoughValues,
                    fIdentityConstraint->getElementName());
                break;
            }
        case IdentityConstraint::KEY:
            {
				fScanner->getValidator()->emitError(XMLValid::IC_KeyNotEnoughValues,
                    fIdentityConstraint->getElementName(), fIdentityConstraint->getIdentityConstraintName());
                break;
            }
        case IdentityConstraint::KEYREF:
            {
				fScanner->getValidator()->emitError(XMLValid::IC_KeyRefNotEnoughValues,
                    fIdentityConstraint->getElementName(), fIdentityConstraint->getIdentityConstraintName());
                break;
            }
        }
    }
}

bool ValueStore::contains(const FieldValueMap* const other) {

    if (fValueTuples) {

        unsigned int otherSize = other->size();
        unsigned int tupleSize = fValueTuples->size();

        for (unsigned int i=0; i<tupleSize; i++) {

            FieldValueMap* valueMap = fValueTuples->elementAt(i);

            if (otherSize == valueMap->size()) {

                bool matchFound = true;

                for (unsigned int j=0; j<otherSize; j++) {
                    if (!isDuplicateOf(valueMap->getDatatypeValidatorAt(j), valueMap->getValueAt(j),
                                       other->getDatatypeValidatorAt(j), other->getValueAt(j))) {
                        matchFound = false;
                        break;
                    }
                }

                if (matchFound) { // found it
                    return true;
                }
            }
        }
    }

    return false;
}

bool ValueStore::isDuplicateOf(DatatypeValidator* const dv1, const XMLCh* const val1,
                               DatatypeValidator* const dv2, const XMLCh* const val2) {

    // if either validator's null, fall back on string comparison
    if(!dv1 || !dv2) {
        return (XMLString::equals(val1, val2));
    }

    unsigned int val1Len = XMLString::stringLen(val1);
    unsigned int val2Len = XMLString::stringLen(val2);

    if (!val1Len && !val2Len) {

        if (dv1 == dv2) {
            return true;
        }

        return false;
    }

    if (!val1Len || !val2Len) {
        return false;
    }

    // are the validators equal?
    // As always we are obliged to compare by reference...
    if (dv1 == dv2) {
        return ((dv1->compare(val1, val2, fMemoryManager)) == 0);
    }

    // see if this.fValidator is derived from value.fValidator:
    DatatypeValidator* tempVal = dv1;
    for(; !tempVal || tempVal == dv2; tempVal = tempVal->getBaseValidator()) ;

    if (tempVal) { // was derived!
        return ((dv2->compare(val1, val2, fMemoryManager)) == 0);
    }

    // see if value.fValidator is derived from this.fValidator:
    for(tempVal = dv2; !tempVal || tempVal == dv1; tempVal = tempVal->getBaseValidator()) ;

    if(tempVal) { // was derived!
        return ((dv1->compare(val1, val2, fMemoryManager)) == 0);
    }

    // if we're here it means the types weren't related.  Must fall back to strings:
    return (XMLString::equals(val1, val2));
}


// ---------------------------------------------------------------------------
//  ValueStore: Docuement handling methods
// ---------------------------------------------------------------------------
void ValueStore::endDcocumentFragment(ValueStoreCache* const valueStoreCache) {

    if (fIdentityConstraint->getType() == IdentityConstraint::KEYREF) {

        // verify references
        // get the key store corresponding (if it exists):
        fKeyValueStore = valueStoreCache->getGlobalValueStoreFor(((IC_KeyRef*) fIdentityConstraint)->getKey());

        if (!fKeyValueStore) {

            if (fDoReportError) {
                fScanner->getValidator()->emitError(XMLValid::IC_KeyRefOutOfScope,
                    fIdentityConstraint->getIdentityConstraintName());
            }

            return;
        }

        unsigned int count = (fValueTuples) ? fValueTuples->size() : 0;

        for (unsigned int i = 0; i < count; i++) {

            FieldValueMap* valueMap = fValueTuples->elementAt(i);

            if (!fKeyValueStore->contains(valueMap) && fDoReportError) {

                fScanner->getValidator()->emitError(XMLValid::IC_KeyNotFound,
                    fIdentityConstraint->getElementName());
            }
        }
    }
}

// ---------------------------------------------------------------------------
//  ValueStore: Error reporting methods
// ---------------------------------------------------------------------------
void ValueStore::reportNilError(IdentityConstraint* const ic) {

    if (fDoReportError && ic->getType() == IdentityConstraint::KEY) {
        fScanner->getValidator()->emitError(XMLValid::IC_KeyMatchesNillable,
                                            ic->getElementName());
    }
}

void ValueStore::duplicateValue() {

    if (fDoReportError) {

        switch (fIdentityConstraint->getType()) {
        case IdentityConstraint::UNIQUE:
            {
                fScanner->getValidator()->emitError(XMLValid::IC_DuplicateUnique,
                    fIdentityConstraint->getElementName());
                break;
            }
        case IdentityConstraint::KEY:
            {
                fScanner->getValidator()->emitError(XMLValid::IC_DuplicateKey,
                    fIdentityConstraint->getElementName());
                break;
            }
        }
    }
}

XERCES_CPP_NAMESPACE_END

/**
  * End of file ValueStore.cpp
  */

