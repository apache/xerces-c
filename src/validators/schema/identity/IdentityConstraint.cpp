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
#include <validators/schema/identity/IdentityConstraint.hpp>
#include <validators/schema/identity/IC_Selector.hpp>
#include <validators/schema/identity/IC_Field.hpp>
#include <util/XMLString.hpp>

// ---------------------------------------------------------------------------
//  IdentityConstraint: Constructors and Destructor
// ---------------------------------------------------------------------------
IdentityConstraint::IdentityConstraint(const XMLCh* const identityConstraintName,
                                       const XMLCh* const elemName)
    : fIdentityConstraintName(XMLString::replicate(identityConstraintName))
    , fElemName(0)
    , fSelector(0)
    , fFields(0)
{
    try {
        fElemName = XMLString::replicate(elemName);
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

    if (XMLString::compareString(fIdentityConstraintName, other.fIdentityConstraintName))
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

    delete [] fIdentityConstraintName;
    delete [] fElemName;
    delete fFields;
    delete fSelector;
}



/**
  * End of file IdentityConstraint.cpp
  */

