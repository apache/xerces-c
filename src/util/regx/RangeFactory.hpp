/*
 * The Apache Software License, Version 1.1
 * 
 * Copyright (c) 1999-2001 The Apache Software Foundation.  All rights
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
 */

#if !defined(RANGEFACTORY_HPP)
#define RANGEFACTORY_HPP

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <util/StringPool.hpp>
#include <util/Mutexes.hpp>
#include <stdlib.h>

// ---------------------------------------------------------------------------
//  Forward Declaration
// ---------------------------------------------------------------------------
class RangeToken;
class TokenFactory;


class XMLUTIL_EXPORT RangeTokenElemMap {

public:
    RangeTokenElemMap(unsigned int categoryId);
    ~RangeTokenElemMap();

    // -----------------------------------------------------------------------
    //  Getter methods
    // -----------------------------------------------------------------------
	unsigned int getCategoryId() const;
	RangeToken*  getRangeToken(const bool complement = false) const;

	// -----------------------------------------------------------------------
    //  Setter methods
    // -----------------------------------------------------------------------
	void setRangeToken(RangeToken* const tok, const bool complement = false);
	void setCategoryId(const unsigned int categId);

private:
    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    RangeTokenElemMap(const RangeTokenElemMap&);
    void operator=(const RangeTokenElemMap&);

    // Data members
    unsigned int fCategoryId;
    RangeToken*  fRange;
    RangeToken*  fNRange;
};


class XMLUTIL_EXPORT RangeFactory {

public:
	// -----------------------------------------------------------------------
    //  Constructors and destructors
    // -----------------------------------------------------------------------
    virtual ~RangeFactory();

    // -----------------------------------------------------------------------
    //  Putter methods
    // -----------------------------------------------------------------------
    void addCategory(const XMLCh* const categoryName);
	void addRangeMap(const XMLCh* const categoryName,
                     RangeFactory* const rangeFactory);
    void addKeywordMap(const XMLCh* const keyword,
                       const XMLCh* const categoryName);

    // -----------------------------------------------------------------------
    //  Instance methods
    // -----------------------------------------------------------------------
	static RangeFactory* instance();

	//-----------------------------------------------------------------------
    //  Initialization methods
    // -----------------------------------------------------------------------
	virtual void initializeKeywordMap();

    // -----------------------------------------------------------------------
    //  CleanUp methods
    // -----------------------------------------------------------------------
    static void cleanUp();

protected:
    // -----------------------------------------------------------------------
    //  Constructor and destructors
    // -----------------------------------------------------------------------
    RangeFactory();

    // -----------------------------------------------------------------------
    //  Setter methods
    // -----------------------------------------------------------------------
	void setRangeToken(const XMLCh* const keyword, RangeToken* const tok,
                       const bool complement = false);

    // -----------------------------------------------------------------------
    //  Getter methods
    // -----------------------------------------------------------------------
    /*
     *  Gets a commonly used RangeToken from the token registry based on the
     *  range name - Called by TokenFactory.
     */
	 RangeToken* getRange(const XMLCh* const name,
                          const bool complement = false);

     RefHashTableOf<RangeTokenElemMap>* getTokenRegistry() const;
     RefHashTableOf<RangeFactory>* getRangeMap() const;
	 XMLStringPool* getCategories() const;

	 /*
	  *	Used by children to build commonly used ranges
	  */
     virtual void buildRanges();

private:
	// -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    RangeFactory(const RangeFactory&);
    void operator=(const RangeFactory&);

    // -----------------------------------------------------------------------
    //  Private Helpers methods
    // -----------------------------------------------------------------------
    /*
     *  Initializes the registry with a set of commonly used RangeToken
     *  objects.
     */
     void initializeRegistry();
	 friend class TokenFactory; 

    // -----------------------------------------------------------------------
    //  Private data members
    //
    //  fTokenRegistry
    //      Contains a set of commonly used tokens
	//
    //  fRangeMap
    //      Contains a map between a category name and a RangeFactory object.
    //
    //  fCategories
    //      Contains range categories names
    // -----------------------------------------------------------------------
    static bool                               fRegistryInitialized;	
    static RefHashTableOf<RangeTokenElemMap>* fTokenRegistry;
    static RefHashTableOf<RangeFactory>*      fRangeMap;
	static XMLStringPool*                     fCategories;
	static RangeFactory*                      fInstance;
    XMLMutex                                  fMutex;
};

// ---------------------------------------------------------------------------
//  RangeTokenElemMap: Getter methods
// ---------------------------------------------------------------------------
inline unsigned int RangeTokenElemMap::getCategoryId() const {

    return fCategoryId;
}

inline RangeToken* RangeTokenElemMap::getRangeToken(const bool complement) const {

	return complement ? fNRange : fRange;
}

// ---------------------------------------------------------------------------
//  RangeTokenElemMap: Setter methods
// ---------------------------------------------------------------------------
inline void RangeTokenElemMap::setCategoryId(const unsigned int categId) {

    fCategoryId = categId;
}

inline void RangeTokenElemMap::setRangeToken(RangeToken* const tok,
                                      const bool complement) {

    if (complement)
        fNRange = tok;
	else
        fRange = tok;
}

// ---------------------------------------------------------------------------
//  RangeFactory: Getter methods
// ---------------------------------------------------------------------------
inline RefHashTableOf<RangeTokenElemMap>* RangeFactory::getTokenRegistry() const {

    return fTokenRegistry;
}

inline RefHashTableOf<RangeFactory>* RangeFactory::getRangeMap() const {

    return fRangeMap;
}

inline XMLStringPool* RangeFactory::getCategories() const {

    return fCategories;
}

// ---------------------------------------------------------------------------
//  RangeFactory: Instance methods
// ---------------------------------------------------------------------------
inline RangeFactory* RangeFactory::instance() {

	if (!fInstance) {

		fInstance = new RangeFactory();
		atexit(cleanUp);
	}
	
    return (fInstance);
}

// ---------------------------------------------------------------------------
//  RangeFactory: cleanUp methods
// ---------------------------------------------------------------------------
inline void RangeFactory::cleanUp() {

    delete fTokenRegistry;
    fTokenRegistry = 0;

    delete fRangeMap;
    fRangeMap = 0;

    delete fCategories;
    fCategories = 0;

    delete fInstance;
    fInstance = 0;
}

#endif

/**
  *	End file RangeFactory.hpp
  */
