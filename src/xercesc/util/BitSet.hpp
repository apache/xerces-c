/*
 * Copyright 1999-2004 The Apache Software Foundation.
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
 * Revision 1.5  2004/09/08 13:56:21  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.4  2004/01/29 11:48:46  cargilld
 * Code cleanup changes to get rid of various compiler diagnostic messages.
 *
 * Revision 1.3  2003/05/15 19:04:35  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.2  2002/11/04 15:22:03  tng
 * C++ Namespace Support.
 *
 * Revision 1.1.1.1  2002/02/01 22:22:10  peiyongz
 * sane_include
 *
 * Revision 1.4  2000/03/02 19:54:38  roddey
 * This checkin includes many changes done while waiting for the
 * 1.1.0 code to be finished. I can't list them all here, but a list is
 * available elsewhere.
 *
 * Revision 1.3  2000/02/24 20:05:24  abagchi
 * Swat for removing Log from API docs
 *
 * Revision 1.2  2000/02/06 07:48:01  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.1.1.1  1999/11/09 01:04:11  twl
 * Initial checkin
 *
 * Revision 1.2  1999/11/08 20:45:05  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */

#if !defined(BITSET_HPP)
#define BITSET_HPP

#include <xercesc/util/XMemory.hpp>
#include <xercesc/util/PlatformUtils.hpp>

XERCES_CPP_NAMESPACE_BEGIN

class XMLUTIL_EXPORT BitSet : public XMemory
{
public:
    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    BitSet( const unsigned int size
          , MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);
    BitSet(const BitSet& toCopy);
    ~BitSet();


    // -----------------------------------------------------------------------
    //  Equality methods
    // -----------------------------------------------------------------------
    bool equals(const BitSet& other) const;


    // -----------------------------------------------------------------------
    //  Getter methods
    // -----------------------------------------------------------------------
    bool allAreCleared() const;
    bool allAreSet() const;
    unsigned int size() const;
    bool get(const unsigned int index) const;


    // -----------------------------------------------------------------------
    //  Setter methods
    // -----------------------------------------------------------------------
    void clear(const unsigned int index);
    void clearAll();
    void set(const unsigned int index);


    // -----------------------------------------------------------------------
    //  Bitwise logical operations
    // -----------------------------------------------------------------------
    void andWith(const BitSet& other);
    void orWith(const BitSet& other);
    void xorWith(const BitSet& other);


    // -----------------------------------------------------------------------
    //  Miscellaneous
    // -----------------------------------------------------------------------
    unsigned int hash(const unsigned int hashModulus) const;


private :
    // -----------------------------------------------------------------------
    //  Unimplemented constructors
    // -----------------------------------------------------------------------
    BitSet();    
    BitSet& operator=(const BitSet&);
    // -----------------------------------------------------------------------
    //  Private methods
    // -----------------------------------------------------------------------
    void ensureCapacity(const unsigned int bits);


    // -----------------------------------------------------------------------
    //  Data members
    //
    //  fBits
    //      The array of unsigned longs used to store the bits.
    //
    //  fUnitLen
    //      The length of the storage array, in storage units not bits.
    // -----------------------------------------------------------------------
    MemoryManager*  fMemoryManager;
    unsigned long*  fBits;
    unsigned int    fUnitLen;
};

XERCES_CPP_NAMESPACE_END

#endif
