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
 * Revision 1.8  2004/09/08 13:56:23  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.7  2004/01/29 11:48:46  cargilld
 * Code cleanup changes to get rid of various compiler diagnostic messages.
 *
 * Revision 1.6  2003/05/29 13:26:44  knoaman
 * Fix memory leak when using deprecated dom.
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
 * Revision 1.1.1.1  1999/11/09 01:05:30  twl
 * Initial checkin
 *
 * Revision 1.2  1999/11/08 20:45:18  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */

#if !defined(VALUESTACKOF_HPP)
#define VALUESTACKOF_HPP

#include <xercesc/util/EmptyStackException.hpp>
#include <xercesc/util/ValueVectorOf.hpp>

XERCES_CPP_NAMESPACE_BEGIN

//
//  Forward declare the enumerator so he can be our friend. Can you say
//  friend? Sure...
//
template <class TElem> class ValueStackEnumerator;


template <class TElem> class ValueStackOf : public XMemory
{
public :
    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    ValueStackOf
    (
          const unsigned int fInitCapacity
          , MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager
          , const bool toCallDestructor = false
    );
    ~ValueStackOf();


    // -----------------------------------------------------------------------
    //  Element management methods
    // -----------------------------------------------------------------------
    void push(const TElem& toPush);
    const TElem& peek() const;
    TElem pop();
    void removeAllElements();


    // -----------------------------------------------------------------------
    //  Getter methods
    // -----------------------------------------------------------------------
    bool empty();
    unsigned int curCapacity();
    unsigned int size();


private :
    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------    
    ValueStackOf(const ValueStackOf<TElem>&);
    ValueStackOf<TElem>& operator=(const ValueStackOf<TElem>&);

    // -----------------------------------------------------------------------
    //  Declare our friends
    // -----------------------------------------------------------------------
    friend class ValueStackEnumerator<TElem>;


    // -----------------------------------------------------------------------
    //  Data Members
    //
    //  fVector
    //      The vector that is used as the backing data structure for the
    //      stack.
    // -----------------------------------------------------------------------
    ValueVectorOf<TElem>    fVector;
};



//
//  An enumerator for a value stack. It derives from the basic enumerator
//  class, so that value stacks can be generically enumerated.
//
template <class TElem> class ValueStackEnumerator : public XMLEnumerator<TElem>, public XMemory
{
public :
    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    ValueStackEnumerator
    (
                ValueStackOf<TElem>* const  toEnum
        , const bool                        adopt = false
    );
    virtual ~ValueStackEnumerator();


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
    ValueStackEnumerator(const ValueStackEnumerator<TElem>&);
    ValueStackEnumerator<TElem>& operator=(const ValueStackEnumerator<TElem>&);

    // -----------------------------------------------------------------------
    //  Data Members
    //
    //  fAdopted
    //      Indicates whether we have adopted the passed stack. If so then
    //      we delete the stack when we are destroyed.
    //
    //  fCurIndex
    //      This is the current index into the vector inside the stack being
    //      enumerated.
    //
    //  fToEnum
    //      The stack that is being enumerated. This is just kept for
    //      adoption purposes, since we really are enumerating the vector
    //      inside of it.
    // -----------------------------------------------------------------------
    bool                    fAdopted;
    unsigned int            fCurIndex;
    ValueVectorOf<TElem>*   fVector;
    ValueStackOf<TElem>*    fToEnum;
};

XERCES_CPP_NAMESPACE_END

#if !defined(XERCES_TMPLSINC)
#include <xercesc/util/ValueStackOf.c>
#endif

#endif
