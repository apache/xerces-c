/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 2004 The Apache Software Foundation.  All rights
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
 * $Id$
 * $Log$
 * Revision 1.1  2004/04/13 16:41:16  peiyongz
 * IdentityConstraintHandler
 *
 *
 */

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include "IdentityConstraintHandler.hpp"

#include <xercesc/validators/schema/SchemaElementDecl.hpp>

#include <xercesc/validators/schema/identity/FieldActivator.hpp>
#include <xercesc/validators/schema/identity/ValueStore.hpp>
#include <xercesc/validators/schema/identity/IC_Selector.hpp>

#include <xercesc/util/OutOfMemoryException.hpp>

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  IdentityConstraintHandler: Constructors and Destructor
// ---------------------------------------------------------------------------
IdentityConstraintHandler::IdentityConstraintHandler(XMLScanner*    const scanner
                     , MemoryManager* const manager)
: fScanner(scanner)
, fMemoryManager(manager)
, fMatcherStack(0)
, fValueStoreCache(0)
, fFieldActivator(0)
{

    try {

        fMatcherStack    = new (fMemoryManager) XPathMatcherStack(fMemoryManager);
        fValueStoreCache = new (fMemoryManager) ValueStoreCache(fMemoryManager);
        fFieldActivator  = new (fMemoryManager) FieldActivator(fValueStoreCache, fMatcherStack, fMemoryManager);

        fValueStoreCache->setScanner(scanner);
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

IdentityConstraintHandler::~IdentityConstraintHandler()
{
    cleanUp();
}

// ---------------------------------------------------------------------------
//  IdentityConstraintHandler:  methods
// ---------------------------------------------------------------------------
void IdentityConstraintHandler::deactivateContext(      SchemaElementDecl* const elem
                                                , const XMLCh*             const content)
{

    int oldCount = fMatcherStack->getMatcherCount();

    if (oldCount || elem->getIdentityConstraintCount()) 
    {

        for (int i = oldCount - 1; i >= 0; i--) 
        {
            XPathMatcher* matcher = fMatcherStack->getMatcherAt(i);
            matcher->endElement(*(elem), content);
        }

        if (fMatcherStack->size() > 0) 
        {
            fMatcherStack->popContext();
        }

        // handle everything *but* keyref's.
        int newCount = fMatcherStack->getMatcherCount();

        for (int j = oldCount - 1; j >= newCount; j--) 
        {
            XPathMatcher* matcher = fMatcherStack->getMatcherAt(j);
            IdentityConstraint* ic = matcher->getIdentityConstraint();

            if (ic  && (ic->getType() != IdentityConstraint::KEYREF))
                fValueStoreCache->transplant(ic, matcher->getInitialDepth());
        }

        // now handle keyref's...
        for (int k = oldCount - 1; k >= newCount; k--) 
        {
            XPathMatcher* matcher = fMatcherStack->getMatcherAt(k);
            IdentityConstraint* ic = matcher->getIdentityConstraint();

            if (ic && (ic->getType() == IdentityConstraint::KEYREF)) 
            {
                ValueStore* values = fValueStoreCache->getValueStoreFor(ic, matcher->getInitialDepth());

                if (values) { // nothing to do if nothing matched!
                    values->endDcocumentFragment(fValueStoreCache);
                }
            }
        }

        fValueStoreCache->endElement();

    }
}

void IdentityConstraintHandler::activateIdentityConstraint
                     (      
                             SchemaElementDecl* const     elem
                     ,       int                          elemDepth
                     , const unsigned int                 uriId
                     , const XMLCh*                 const elemPrefix
                     , const RefVectorOf<XMLAttr>&        attrList
                     , const unsigned int                 attrCount
                      )
{

    unsigned int count = elem->getIdentityConstraintCount();

    if (count || fMatcherStack->getMatcherCount()) 
    {

        fValueStoreCache->startElement();
        fMatcherStack->pushContext();
        fValueStoreCache->initValueStoresFor( elem, elemDepth);

        for (unsigned int i = 0; i < count; i++) 
        {
            activateSelectorFor(elem->getIdentityConstraintAt(i), elemDepth);
        }

        // call all active identity constraints
        count = fMatcherStack->getMatcherCount();

        for (unsigned int j = 0; j < count; j++) 
        {
            XPathMatcher* matcher = fMatcherStack->getMatcherAt(j);
            matcher->startElement(*elem, uriId, elemPrefix, attrList, attrCount);
        }
    }
}

void IdentityConstraintHandler::activateSelectorFor(      IdentityConstraint* const ic
                                   , const int                       initialDepth) 
{

    IC_Selector* selector = ic->getSelector();

    if (!selector)
        return;

    XPathMatcher* matcher = selector->createMatcher(fFieldActivator, initialDepth, fMemoryManager);

    fMatcherStack->addMatcher(matcher);
    matcher->startDocumentFragment();
}

// ---------------------------------------------------------------------------
//  IdentityConstraintHandler:  Getter methods
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
//  IdentityConstraintHandler: cleanUp methods
// ---------------------------------------------------------------------------
void IdentityConstraintHandler::cleanUp() 
{
    if (fMatcherStack)
        delete fMatcherStack;

    if (fValueStoreCache)
        delete fValueStoreCache;

    if (fFieldActivator)
        delete fFieldActivator;

}

void IdentityConstraintHandler::reset()
{
    fValueStoreCache->startDocument();
    fMatcherStack->clear();
}

XERCES_CPP_NAMESPACE_END

/**
  * End of file IdentityConstraintHandler.cpp
  */

