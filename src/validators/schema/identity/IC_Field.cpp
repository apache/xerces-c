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
#include <validators/schema/identity/IC_Field.hpp>
#include <validators/schema/identity/ValueStore.hpp>
#include <validators/schema/identity/XercesXPath.hpp>


// ---------------------------------------------------------------------------
//  FieldMatcher: Constructors and Destructor
// ---------------------------------------------------------------------------
FieldMatcher::FieldMatcher(XercesXPath* const xpath, IC_Field* const aField,
                           ValueStore* const valueStore)
    : XPathMatcher(xpath, true, 0)
    , fField(aField)
    , fValueStore(valueStore)
{
}

// ---------------------------------------------------------------------------
//  FieldMatcher: Match methods
// ---------------------------------------------------------------------------
void FieldMatcher::matched(const XMLCh* const content,
                           DatatypeValidator* const dv, 
                           const bool isNil) {

    if(isNil) {
        fValueStore->reportNilError(fField->getIdentityConstraint());
    }

    fValueStore->addValue(fField, dv, content);

    // once we've stored the value for this field, we set the mayMatch
    // member to false so that, in the same scope, we don't match any more
    // values (and throw an error instead).
    fField->setMayMatch(false);
}

// ---------------------------------------------------------------------------
//  IC_Field: Constructors and Destructor
// ---------------------------------------------------------------------------
IC_Field::IC_Field(XercesXPath* const xpath,
                   IdentityConstraint* const identityConstraint)
    : fXPath(xpath)
    , fIdentityConstraint(identityConstraint)
{
}


IC_Field::~IC_Field()
{
    delete fXPath;
}

// ---------------------------------------------------------------------------
//  IC_Field: operators
// ---------------------------------------------------------------------------
bool IC_Field::operator== (const IC_Field& other) const {

    return (*fXPath == *(other.fXPath));
}

bool IC_Field::operator!= (const IC_Field& other) const {

    return !operator==(other);
}

// ---------------------------------------------------------------------------
//  IC_Field: Factory methods
// ---------------------------------------------------------------------------
XPathMatcher* IC_Field::createMatcher(ValueStore* const valueStore) {

    return new FieldMatcher(fXPath, this, valueStore);
}

/**
  * End of file IC_Field.cpp
  */

