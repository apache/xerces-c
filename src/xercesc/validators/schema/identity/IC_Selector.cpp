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
 * Revision 1.1  2002/02/01 22:22:50  peiyongz
 * Initial revision
 *
 * Revision 1.3  2001/11/23 18:35:33  tng
 * Eliminate Warning from AIX xlC 3.6:1540-399: (W) "XMLAttr" is undefined.  The delete operator will not call a destructor.
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
#include <xercesc/framework/XMLAttr.hpp>
#include <xercesc/validators/schema/identity/IC_Selector.hpp>
#include <xercesc/validators/schema/identity/XercesXPath.hpp>
#include <xercesc/validators/schema/identity/IdentityConstraint.hpp>
#include <xercesc/validators/schema/identity/FieldActivator.hpp>

// ---------------------------------------------------------------------------
//  SelectorMatcher: Constructors and Destructor
// ---------------------------------------------------------------------------
SelectorMatcher::SelectorMatcher(XercesXPath* const xpath,
                                 IC_Selector* const selector,
                                 FieldActivator* const fieldActivator)
    : XPathMatcher(xpath, false, selector->getIdentityConstraint())
    , fElementDepth(0)
    , fMatchedDepth(-1)
    , fSelector(selector)
    , fFieldActivator(fieldActivator)
{
}

// ---------------------------------------------------------------------------
//  FieldMatcher: XMLDocumentHandler methods
// ---------------------------------------------------------------------------
void SelectorMatcher::startDocumentFragment() {

    XPathMatcher::startDocumentFragment();
    fElementDepth = 0;
    fMatchedDepth = -1;
}

void SelectorMatcher::startElement(const XMLElementDecl& elemDecl,
                                   const unsigned int urlId,
                                   const XMLCh* const elemPrefix,
                                   const RefVectorOf<XMLAttr>& attrList,
                                   const unsigned int attrCount) {

    XPathMatcher::startElement(elemDecl, urlId, elemPrefix, attrList, attrCount);
    fElementDepth++;

    // activate the fields, if selector is matched
    if (fMatchedDepth == -1 && isMatched()) {

        IdentityConstraint* ic = fSelector->getIdentityConstraint();
        int count = ic->getFieldCount();

        fMatchedDepth = fElementDepth;
        fFieldActivator->startValueScopeFor(ic);

        for (int i = 0; i < count; i++) {

            IC_Field* field = ic->getFieldAt(i);
            XPathMatcher* matcher = fFieldActivator->activateField(field);

            matcher->startElement(elemDecl, urlId, elemPrefix, attrList, attrCount);
        }
    }
}

void SelectorMatcher::endElement(const XMLElementDecl& elemDecl) {

    XPathMatcher::endElement(elemDecl);

    if (fElementDepth-- == fMatchedDepth) {

        fMatchedDepth = -1;
        fFieldActivator->endValueScopeFor(fSelector->getIdentityConstraint());
    }
}

// ---------------------------------------------------------------------------
//  IC_Selector: Constructors and Destructor
// ---------------------------------------------------------------------------
IC_Selector::IC_Selector(XercesXPath* const xpath,
                         IdentityConstraint* const identityConstraint)
    : fXPath(xpath)
    , fIdentityConstraint(identityConstraint)
{
}


IC_Selector::~IC_Selector()
{
    delete fXPath;
}

// ---------------------------------------------------------------------------
//  IC_Selector: operators
// ---------------------------------------------------------------------------
bool IC_Selector::operator ==(const IC_Selector& other) const {

    return (*fXPath == *(other.fXPath));
}

bool IC_Selector::operator !=(const IC_Selector& other) const {

    return !operator==(other);
}

// ---------------------------------------------------------------------------
//  IC_Selector: Factory methods
// ---------------------------------------------------------------------------
XPathMatcher* IC_Selector::createMatcher(FieldActivator* const fieldActivator) {

    return new SelectorMatcher(fXPath, this, fieldActivator);
}

/**
  * End of file IC_Selector.cpp
  */

