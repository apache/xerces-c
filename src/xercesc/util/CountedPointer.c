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
 * Revision 1.5  2005/01/12 13:29:06  amassari
 * Macro having 3 arguments was invoked with only 2
 *
 * Revision 1.4  2004/09/08 13:56:21  peiyongz
 * Apache License Version 2.0
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
 * Revision 1.3  2000/03/02 19:54:38  roddey
 * This checkin includes many changes done while waiting for the
 * 1.1.0 code to be finished. I can't list them all here, but a list is
 * available elsewhere.
 *
 * Revision 1.2  2000/02/06 07:48:01  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.1.1.1  1999/11/09 01:04:12  twl
 * Initial checkin
 *
 * Revision 1.2  1999/11/08 20:45:05  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#if defined(XERCES_TMPLSINC)
#include <xercesc/util/CountedPointer.hpp>
#endif

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  CountedPointerTo: Constructors and Destructor
// ---------------------------------------------------------------------------
template <class T> CountedPointerTo<T>::
CountedPointerTo(const CountedPointerTo<T>& toCopy) :

    fPtr(toCopy.fPtr)
{
    if (fPtr)
        fPtr->addRef();
}

template <class T> CountedPointerTo<T>::CountedPointerTo(T* p) :

    fPtr(p)
{
    if (fPtr)
        fPtr->addRef();
}

template <class T> CountedPointerTo<T>::~CountedPointerTo()
{
    if (fPtr)
        fPtr->removeRef();
}


// ---------------------------------------------------------------------------
//  CountedPointerTo: Operators
// ---------------------------------------------------------------------------
template <class T> CountedPointerTo<T>&
CountedPointerTo<T>::operator=(const CountedPointerTo<T>& other)
{
    if (this == &other)
        return *this;

    if (other.fPtr)
        other.fPtr->addRef();

    if (fPtr)
        fPtr->removeRef();

    fPtr = other.fPtr;
    return *this;
}

template <class T> CountedPointerTo<T>::operator T*()
{
    return fPtr;
}

template <class T> const T* CountedPointerTo<T>::operator->() const
{
    return fPtr;
}

template <class T> T* CountedPointerTo<T>::operator->()
{
    return fPtr;
}

template <class T> const T& CountedPointerTo<T>::operator*() const
{
    if (!fPtr)
        ThrowXMLwithMemMgr(NullPointerException, XMLExcepts::CPtr_PointerIsZero, 0);
    return *fPtr;
}

template <class T> T& CountedPointerTo<T>::operator*()
{
    if (!fPtr)
        ThrowXMLwithMemMgr(NullPointerException, XMLExcepts::CPtr_PointerIsZero, 0);
    return *fPtr;
}

XERCES_CPP_NAMESPACE_END
