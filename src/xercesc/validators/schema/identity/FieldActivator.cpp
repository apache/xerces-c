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

// ---------------------------------------------------------------------------
//  FieldActivator: Constructors and Destructor
// ---------------------------------------------------------------------------
FieldActivator::FieldActivator(ValueStoreCache* const valueStoreCache,
                               XPathMatcherStack* const matcherStack)
    : fValueStoreCache(valueStoreCache)
    , fMatcherStack(matcherStack)
{
}

FieldActivator::FieldActivator(const FieldActivator& other)
    : fValueStoreCache(other.fValueStoreCache)
    , fMatcherStack(other.fMatcherStack)
{
}


FieldActivator::~FieldActivator()
{
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
    XPathMatcher* matcher = field->createMatcher(valueStore);

    field->setMayMatch(true);
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

/**
  * End of file FieldActivator.cpp
  */

