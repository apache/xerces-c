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
 * $Id$
 * $Log$
 * Revision 1.5  2004/09/08 13:56:21  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.4  2004/01/29 11:48:46  cargilld
 * Code cleanup changes to get rid of various compiler diagnostic messages.
 *
 * Revision 1.3  2003/12/17 00:18:35  cargilld
 * Update to memory management so that the static memory manager (one used to call Initialize) is only for static data.
 *
 * Revision 1.2  2002/11/04 15:22:03  tng
 * C++ Namespace Support.
 *
 * Revision 1.1.1.1  2002/02/01 22:22:10  peiyongz
 * sane_include
 *
 * Revision 1.2  2001/11/22 20:23:00  peiyongz
 * _declspec(dllimport) and inline warning C4273
 *
 * Revision 1.1  2001/08/16 21:54:01  peiyongz
 * new class creation
 *
 */

#if !defined(HASH_CMSTATESET_HPP)
#define HASH_CMSTATESET_HPP

#include <xercesc/util/HashBase.hpp>
#include <xercesc/validators/common/CMStateSet.hpp>

XERCES_CPP_NAMESPACE_BEGIN

/**
 * The <code>HashCMStateSet</code> class inherits from <code>HashBase</code>.
 * This is a CMStateSet specific hasher class designed to hash the values
 * of CMStateSet.
 *
 * See <code>HashBase</code> for more information.
 */

class XMLUTIL_EXPORT HashCMStateSet : public HashBase
{
public:
	HashCMStateSet();
	virtual ~HashCMStateSet();
	virtual unsigned int getHashVal(const void *const key, unsigned int mod
        , MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);
	virtual bool equals(const void *const key1, const void *const key2);
private:
    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    HashCMStateSet(const HashCMStateSet&);
    HashCMStateSet& operator=(const HashCMStateSet&);
};

inline HashCMStateSet::HashCMStateSet()
{
}

inline HashCMStateSet::~HashCMStateSet()
{
}

inline unsigned int HashCMStateSet::getHashVal(const void *const key, unsigned int mod
                                               , MemoryManager* const)
{
    const CMStateSet* const pkey = (const CMStateSet* const) key;
	return ((pkey->hashCode()) % mod);
}

inline bool HashCMStateSet::equals(const void *const key1, const void *const key2)
{
    const CMStateSet* const pkey1 = (const CMStateSet* const) key1;
    const CMStateSet* const pkey2 = (const CMStateSet* const) key2;

	return (*pkey1==*pkey2);
}

XERCES_CPP_NAMESPACE_END

#endif
