/*
 * Copyright 1999-2000,2004 The Apache Software Foundation.
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
 * Revision 1.7  2004/09/08 13:56:23  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.6  2004/01/29 11:48:46  cargilld
 * Code cleanup changes to get rid of various compiler diagnostic messages.
 *
 * Revision 1.5  2003/05/16 06:01:52  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.4  2003/05/15 19:07:46  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.3  2002/11/04 15:22:05  tng
 * C++ Namespace Support.
 *
 * Revision 1.2  2002/08/21 17:45:00  tng
 * [Bug 7087] compiler warnings when using gcc.
 *
 * Revision 1.1.1.1  2002/02/01 22:22:13  peiyongz
 * sane_include
 *
 * Revision 1.4  2000/03/02 19:54:47  roddey
 * This checkin includes many changes done while waiting for the
 * 1.1.0 code to be finished. I can't list them all here, but a list is
 * available elsewhere.
 *
 * Revision 1.3  2000/02/24 20:05:26  abagchi
 * Swat for removing Log from API docs
 *
 * Revision 1.2  2000/02/06 07:48:05  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.1.1.1  1999/11/09 01:05:27  twl
 * Initial checkin
 *
 * Revision 1.2  1999/11/08 20:45:18  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */


#if !defined(VALUEARRAY_HPP)
#define VALUEARRAY_HPP

#include <xercesc/util/XMLEnumerator.hpp>
#include <xercesc/util/ArrayIndexOutOfBoundsException.hpp>
#include <xercesc/util/IllegalArgumentException.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/framework/MemoryManager.hpp>

XERCES_CPP_NAMESPACE_BEGIN

template <class TElem> class ValueArrayOf : public XMemory
{
public :
    // -----------------------------------------------------------------------
    //  Contructors and Destructor
    // -----------------------------------------------------------------------
    ValueArrayOf
    (
           const unsigned int   size
         , MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager
    );
	ValueArrayOf
    (
          const TElem*         values
        , const unsigned int   size
        , MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager
    );
	ValueArrayOf(const ValueArrayOf<TElem>& source);
	~ValueArrayOf();


    // -----------------------------------------------------------------------
    //  Public operators
    // -----------------------------------------------------------------------
	TElem& operator[](const unsigned int index);
	const TElem& operator[](const unsigned int index) const;
	ValueArrayOf<TElem>& operator=(const ValueArrayOf<TElem>& toAssign);
	bool operator==(const ValueArrayOf<TElem>& toCompare) const;
	bool operator!=(const ValueArrayOf<TElem>& toCompare) const;


    // -----------------------------------------------------------------------
    //  Copy operations
    // -----------------------------------------------------------------------
    unsigned int copyFrom(const ValueArrayOf<TElem>& srcArray);


    // -----------------------------------------------------------------------
    //  Getter methods
    // -----------------------------------------------------------------------
	unsigned int length() const;
	TElem* rawData() const;


    // -----------------------------------------------------------------------
    //  Miscellaneous methods
    // -----------------------------------------------------------------------
    void resize(const unsigned int newSize);


private :
    // -----------------------------------------------------------------------
    //  Data members
    // -----------------------------------------------------------------------
	unsigned int    fSize;
	TElem*          fArray;
    MemoryManager*  fMemoryManager;
};


//
//  An enumerator for a value array. It derives from the basic enumerator
//  class, so that value vectors can be generically enumerated.
//
template <class TElem> class ValueArrayEnumerator : public XMLEnumerator<TElem>, public XMemory
{
public :
    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    ValueArrayEnumerator
    (
                ValueArrayOf<TElem>* const toEnum
        , const bool                       adopt = false
    );
    virtual ~ValueArrayEnumerator();


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
    ValueArrayEnumerator(const ValueArrayEnumerator<TElem>&);
    ValueArrayEnumerator<TElem>& operator=(const ValueArrayEnumerator<TElem>&);

    // -----------------------------------------------------------------------
    //  Data Members
    //
    //  fAdopted
    //      Indicates whether we have adopted the passed vector. If so then
    //      we delete the vector when we are destroyed.
    //
    //  fCurIndex
    //      This is the current index into the vector.
    //
    //  fToEnum
    //      The value array being enumerated.
    // -----------------------------------------------------------------------
    bool                    fAdopted;
    unsigned int            fCurIndex;
    ValueArrayOf<TElem>*    fToEnum;
};

XERCES_CPP_NAMESPACE_END

#if !defined(XERCES_TMPLSINC)
#include <xercesc/util/ValueArrayOf.c>
#endif

#endif
