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
 * originally based on software copyright (c) 2001, International
 * Business Machines, Inc., http://www.ibm.com .  For more information
 * on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 */

/*
 * $Log$
 * Revision 1.3  2001/07/16 21:28:25  knoaman
 * fix bug - no delete for the static instance in destructor.
 *
 * Revision 1.2  2001/05/11 13:26:45  tng
 * Copyright update.
 *
 * Revision 1.1  2001/05/03 18:17:40  knoaman
 * Some design changes:
 * o Changed the TokenFactory from a single static instance, to a
 *    normal class. Each RegularExpression object will have its own
 *    instance of TokenFactory, and that instance will be passed to
 *    other classes that need to use a TokenFactory to create Token
 *    objects (with the exception of RangeTokenMap).
 * o Added a new class RangeTokenMap to map a the different ranges
 *    in a given category to a specific RangeFactory object. In the old
 *    design RangeFactory had dual functionality (act as a Map, and as
 *    a factory for creating RangeToken(s)). The RangeTokenMap will
 *    have its own copy of the TokenFactory. There will be only one
 *    instance of the RangeTokenMap class, and that instance will be
 *    lazily deleted when XPlatformUtils::Terminate is called.
 *
 */

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <util/regx/RangeTokenMap.hpp>
#include <util/regx/RangeToken.hpp>
#include <util/regx/RegxDefs.hpp>
#include <util/regx/TokenFactory.hpp>
#include <util/regx/RangeFactory.hpp>
#include <util/PlatformUtils.hpp>
#include <util/XMLDeleterFor.hpp>
#include <util/XMLExceptMsgs.hpp>

// ---------------------------------------------------------------------------
//  Static member data initialization
// ---------------------------------------------------------------------------
RangeTokenMap* RangeTokenMap::fInstance = 0;


// ---------------------------------------------------------------------------
//  RangeTokenElemMap: Constructors and Destructor
// ---------------------------------------------------------------------------
RangeTokenElemMap::RangeTokenElemMap(unsigned int categoryId) :
    fCategoryId(categoryId)
    , fRange(0)
    , fNRange(0)
{

}

RangeTokenElemMap::~RangeTokenElemMap()
{

}

// ---------------------------------------------------------------------------
//  RangeTokenMap: Constructors and Destructor
// ---------------------------------------------------------------------------
RangeTokenMap::RangeTokenMap() :
    fRegistryInitialized(0)
    , fTokenRegistry(0)
    , fRangeMap(0)
    , fCategories(0)
    , fTokenFactory(0) {

}

RangeTokenMap::~RangeTokenMap() {

    delete fTokenRegistry;
    fTokenRegistry = 0;

    delete fRangeMap;
    fRangeMap = 0;

    delete fCategories;
    fCategories = 0;

    delete fTokenFactory;
    fTokenFactory = 0;

    fInstance = 0;
}

// ---------------------------------------------------------------------------
//  RangeTokenMap: Getter methods
// ---------------------------------------------------------------------------
RangeToken* RangeTokenMap::getRange(const XMLCh* const keyword,
								   const bool complement) {

	if (fTokenRegistry == 0 || fRangeMap == 0 || fCategories == 0)
		return 0;

    if (!fTokenRegistry->containsKey(keyword))
		return 0;


	RangeTokenElemMap* elemMap = 0;

	// Use a faux scope to synchronize while we do this
    {
        XMLMutexLock lockInit(&fMutex);

		elemMap = fTokenRegistry->get(keyword);
		RangeToken* rangeTok = 0;

		if (elemMap->getRangeToken() == 0) {
		
			unsigned int categId = elemMap->getCategoryId();
			const XMLCh* categName = fCategories->getValueForId(categId);
			RangeFactory* rangeFactory = fRangeMap->get(categName);

			if (rangeFactory == 0)
				return 0;

			rangeFactory->buildRanges();
		}

		if (complement && ((rangeTok = elemMap->getRangeToken()) != 0)) {
			elemMap->setRangeToken((RangeToken*)
									RangeToken::complementRanges(rangeTok, fTokenFactory),
									complement);
		}
    }

	return (elemMap == 0) ? 0 : elemMap->getRangeToken(complement);
}


// ---------------------------------------------------------------------------
//  RangeTokenMap: Putter methods
// ---------------------------------------------------------------------------
void RangeTokenMap::addCategory(const XMLCh* const categoryName) {

    if (fCategories)
	    fCategories->addOrFind(categoryName);
}

void RangeTokenMap::addRangeMap(const XMLCh* const categoryName,
                                RangeFactory* const rangeFactory) {

    if (fRangeMap)
	    fRangeMap->put((void*)categoryName, rangeFactory);
}

void RangeTokenMap::addKeywordMap(const XMLCh* const keyword,
                                 const XMLCh* const categoryName) {

    if (fCategories == 0 || fTokenRegistry == 0)
        return;

	unsigned int categId = fCategories->getId(categoryName);

	if (categId == 0) {
		ThrowXML1(RuntimeException, XMLExcepts::Regex_InvalidCategoryName, categoryName);
	}

    if (fTokenRegistry->containsKey(keyword)) {

        RangeTokenElemMap* elemMap = fTokenRegistry->get(keyword);

		if (elemMap->getCategoryId() != categId)
			elemMap->setCategoryId(categId);

		return;
	}

	fTokenRegistry->put((void*) keyword, new RangeTokenElemMap(categId));
}

// ---------------------------------------------------------------------------
//  RangeTokenMap: Setter methods
// ---------------------------------------------------------------------------
void RangeTokenMap::setRangeToken(const XMLCh* const keyword,
                                  RangeToken* const tok,const bool complement) {

    if (fTokenRegistry == 0)
		return;

	if (fTokenRegistry->containsKey(keyword)) {
        fTokenRegistry->get(keyword)->setRangeToken(tok, complement);
    }
    else {
		ThrowXML1(RuntimeException, XMLExcepts::Regex_KeywordNotFound, keyword);
	}
}


// ---------------------------------------------------------------------------
//  RangeTokenMap: Initialization methods
// ---------------------------------------------------------------------------
void RangeTokenMap::initializeRegistry() {

	XMLMutexLock lockInit(&fMutex);

	if (fRegistryInitialized)
		return;

    fTokenFactory = new TokenFactory();
    fTokenRegistry = new RefHashTableOf<RangeTokenElemMap>(109);
    fRangeMap = new RefHashTableOf<RangeFactory>(29);
	fCategories = new XMLStringPool();
	fRegistryInitialized = true;
}


// ---------------------------------------------------------------------------
//  RangeTokenMap: Instance methods
// ---------------------------------------------------------------------------
RangeTokenMap* RangeTokenMap::instance() {

	if (!fInstance) {

		fInstance = new RangeTokenMap();
        XMLPlatformUtils::registerLazyData
        (
            new XMLDeleterFor<RangeTokenMap>(fInstance)
        );
	}
	
    return (fInstance);
}

/**
  * End of file RangeTokenMap.cpp
  */
