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

/**
 * $Log$
 * Revision 1.6  2004/09/08 13:56:23  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.5  2003/12/17 00:18:35  cargilld
 * Update to memory management so that the static memory manager (one used to call Initialize) is only for static data.
 *
 * Revision 1.4  2003/05/29 13:26:44  knoaman
 * Fix memory leak when using deprecated dom.
 *
 * Revision 1.3  2003/05/16 06:01:52  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.2  2002/11/04 15:22:05  tng
 * C++ Namespace Support.
 *
 * Revision 1.1.1.1  2002/02/01 22:22:13  peiyongz
 * sane_include
 *
 * Revision 1.3  2000/03/02 19:54:47  roddey
 * This checkin includes many changes done while waiting for the
 * 1.1.0 code to be finished. I can't list them all here, but a list is
 * available elsewhere.
 *
 * Revision 1.2  2000/02/06 07:48:05  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.1.1.1  1999/11/09 01:05:29  twl
 * Initial checkin
 *
 * Revision 1.2  1999/11/08 20:45:18  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#if defined(XERCES_TMPLSINC)
#include <xercesc/util/ValueStackOf.hpp>
#endif

XERCES_CPP_NAMESPACE_BEGIN


// ---------------------------------------------------------------------------
//  ValueStackOf: Constructors and Destructor
// ---------------------------------------------------------------------------
template <class TElem>
ValueStackOf<TElem>::ValueStackOf(const unsigned int fInitCapacity,
                                  MemoryManager* const manager,
                                  const bool toCallDestructor) :

    fVector(fInitCapacity, manager, toCallDestructor)
{
}

template <class TElem> ValueStackOf<TElem>::~ValueStackOf()
{
}


// ---------------------------------------------------------------------------
//  ValueStackOf: Element management methods
// ---------------------------------------------------------------------------
template <class TElem> void ValueStackOf<TElem>::push(const TElem& toPush)
{
    fVector.addElement(toPush);
}

template <class TElem> const TElem& ValueStackOf<TElem>::peek() const
{
    const int curSize = fVector.size();
    if (curSize == 0)
        ThrowXMLwithMemMgr(EmptyStackException, XMLExcepts::Stack_EmptyStack, fVector.getMemoryManager());

    return fVector.elementAt(curSize-1);
}

template <class TElem> TElem ValueStackOf<TElem>::pop()
{
    const int curSize = fVector.size();
    if (curSize == 0)
        ThrowXMLwithMemMgr(EmptyStackException, XMLExcepts::Stack_EmptyStack, fVector.getMemoryManager());

    TElem retVal = fVector.elementAt(curSize-1);
    fVector.removeElementAt(curSize-1);
    return retVal;
}

template <class TElem> void ValueStackOf<TElem>::removeAllElements()
{
    fVector.removeAllElements();
}


// ---------------------------------------------------------------------------
//  ValueStackOf: Getter methods
// ---------------------------------------------------------------------------
template <class TElem> bool ValueStackOf<TElem>::empty()
{
    return (fVector.size() == 0);
}

template <class TElem> unsigned int ValueStackOf<TElem>::curCapacity()
{
    return fVector.curCapacity();
}

template <class TElem> unsigned int ValueStackOf<TElem>::size()
{
    return fVector.size();
}




// ---------------------------------------------------------------------------
//  ValueStackEnumerator: Constructors and Destructor
// ---------------------------------------------------------------------------
template <class TElem> ValueStackEnumerator<TElem>::
ValueStackEnumerator(       ValueStackOf<TElem>* const  toEnum
                    , const bool                        adopt) :

    fAdopted(adopt)
    , fCurIndex(0)
    , fToEnum(toEnum)
    , fVector(&toEnum->fVector)
{
}

template <class TElem> ValueStackEnumerator<TElem>::~ValueStackEnumerator()
{
    if (fAdopted)
        delete fToEnum;
}


// ---------------------------------------------------------------------------
//  ValueStackEnumerator: Enum interface
// ---------------------------------------------------------------------------
template <class TElem> bool ValueStackEnumerator<TElem>::hasMoreElements() const
{
    if (fCurIndex >= fVector->size())
        return false;
    return true;
}

template <class TElem> TElem& ValueStackEnumerator<TElem>::nextElement()
{
    return fVector->elementAt(fCurIndex++);
}

template <class TElem> void ValueStackEnumerator<TElem>::Reset()
{
    fCurIndex = 0;
}

XERCES_CPP_NAMESPACE_END
