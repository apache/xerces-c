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
 * Revision 1.8  2004/09/08 13:56:22  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.7  2004/06/23 22:19:35  neilg
 * fix for compilation under gcc 3.5
 *
 * Revision 1.6  2004/01/29 11:48:46  cargilld
 * Code cleanup changes to get rid of various compiler diagnostic messages.
 *
 * Revision 1.5  2003/05/16 06:01:52  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.4  2003/05/15 19:04:35  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.3  2002/11/04 15:22:04  tng
 * C++ Namespace Support.
 *
 * Revision 1.2  2002/08/21 17:44:59  tng
 * [Bug 7087] compiler warnings when using gcc.
 *
 * Revision 1.1.1.1  2002/02/01 22:22:11  peiyongz
 * sane_include
 *
 * Revision 1.4  2000/03/02 19:54:44  roddey
 * This checkin includes many changes done while waiting for the
 * 1.1.0 code to be finished. I can't list them all here, but a list is
 * available elsewhere.
 *
 * Revision 1.3  2000/02/24 20:05:25  abagchi
 * Swat for removing Log from API docs
 *
 * Revision 1.2  2000/02/06 07:48:03  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.1.1.1  1999/11/09 01:04:57  twl
 * Initial checkin
 *
 * Revision 1.2  1999/11/08 20:45:12  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */


#if !defined(REFARRAY_HPP)
#define REFARRAY_HPP

#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/ArrayIndexOutOfBoundsException.hpp>
#include <xercesc/util/IllegalArgumentException.hpp>
#include <xercesc/util/XMLEnumerator.hpp>
#include <xercesc/framework/MemoryManager.hpp>

XERCES_CPP_NAMESPACE_BEGIN

template <class TElem> class RefArrayOf : public XMemory
{
public :
    // -----------------------------------------------------------------------
    //  Contructors and Destructor
    // -----------------------------------------------------------------------
    RefArrayOf
    (
          const unsigned int size
        , MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);
	RefArrayOf
    (
          TElem* values[]
        , const unsigned int size
        , MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);
	RefArrayOf(const RefArrayOf<TElem>& source);
	~RefArrayOf();


    // -----------------------------------------------------------------------
    //  Public operators
    // -----------------------------------------------------------------------
	TElem*& operator[](const unsigned int index);
	const TElem* operator[](const unsigned int index) const;
	RefArrayOf<TElem>& operator=(const RefArrayOf<TElem>& toAssign);
	bool operator==(const RefArrayOf<TElem>& toCompare) const;
	bool operator!=(const RefArrayOf<TElem>& toCompare) const;


    // -----------------------------------------------------------------------
    //  Copy operations
    // -----------------------------------------------------------------------
    unsigned int copyFrom(const RefArrayOf<TElem>& srcArray);


    // -----------------------------------------------------------------------
    //  Getter methods
    // -----------------------------------------------------------------------
	unsigned int length() const;
	TElem** rawData() const;


    // -----------------------------------------------------------------------
    //  Element management methods
    // -----------------------------------------------------------------------
    void deleteAt(const unsigned int index);
    void deleteAllElements();
    void resize(const unsigned int newSize);


private :
    // -----------------------------------------------------------------------
    //  Data members
    // -----------------------------------------------------------------------
	unsigned int    fSize;
	TElem**         fArray;
    MemoryManager*  fMemoryManager;
};


//
//  An enumerator for a reference array. It derives from the basic enumerator
//  class, so that value vectors can be generically enumerated.
//
template <class TElem> class RefArrayEnumerator : public XMLEnumerator<TElem>, public XMemory
{
public :
    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    RefArrayEnumerator
    (
                RefArrayOf<TElem>* const    toEnum
        , const bool                        adopt = false
    );
    virtual ~RefArrayEnumerator();


    // -----------------------------------------------------------------------
    //  Enum interface
    // -----------------------------------------------------------------------
    bool hasMoreElements() const;
    TElem& nextElement();
    void Reset();


private :
    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    RefArrayEnumerator(const RefArrayEnumerator<TElem>&);
    RefArrayEnumerator<TElem>& operator=(const RefArrayEnumerator<TElem>&);

    // -----------------------------------------------------------------------
    //  Data Members
    //
    //  fAdopted
    //      Indicates whether we have adopted the passed array. If so then
    //      we delete it when we are destroyed.
    //
    //  fCurIndex
    //      This is the current index into the array.
    //
    //  fToEnum
    //      The reference array being enumerated.
    // -----------------------------------------------------------------------
    bool                fAdopted;
    unsigned int        fCurIndex;
    RefArrayOf<TElem>*  fToEnum;
};

XERCES_CPP_NAMESPACE_END

#if !defined(XERCES_TMPLSINC)
#include <xercesc/util/RefArrayOf.c>
#endif

#endif
