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
 * Revision 1.7  2004/09/14 19:47:30  amassari
 * The signature for implementation for ArrayJanitor::reset was not aligned with the declaration (jira#1261)
 *
 * Revision 1.6  2004/09/08 13:56:22  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.5  2003/11/06 19:28:11  knoaman
 * PSVI support for annotations.
 *
 * Revision 1.4  2003/05/21 03:34:52  jberry
 * Cast away CodeWarrior complaint of casting away const if we're holding a const obj
 *
 * Revision 1.3  2003/05/15 19:04:35  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.2  2002/11/04 15:22:04  tng
 * C++ Namespace Support.
 *
 * Revision 1.1.1.1  2002/02/01 22:22:10  peiyongz
 * sane_include
 *
 * Revision 1.6  2000/10/13 22:45:11  andyh
 * Complete removal of ArrayJanitory::operator->().  Was just commented out earlier.
 *
 * Revision 1.5  2000/10/10 23:52:10  andyh
 * From Janitor, remove the addition that is having compile problems in MSVC.
 *
 * Revision 1.4  2000/10/09 18:32:31  jberry
 * Add some auto_ptr functionality to allow modification of monitored
 * pointer value. This eases use of Janitor in some situations.
 *
 * Revision 1.3  2000/03/02 19:54:40  roddey
 * This checkin includes many changes done while waiting for the
 * 1.1.0 code to be finished. I can't list them all here, but a list is
 * available elsewhere.
 *
 * Revision 1.2  2000/02/06 07:48:02  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.1.1.1  1999/11/09 01:04:26  twl
 * Initial checkin
 *
 * Revision 1.2  1999/11/08 20:45:08  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#if defined(XERCES_TMPLSINC)
#include <xercesc/util/Janitor.hpp>
#endif

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  Janitor: Constructors and Destructor
// ---------------------------------------------------------------------------
template <class T> Janitor<T>::Janitor(T* const toDelete) :
    fData(toDelete)
{
}


template <class T> Janitor<T>::~Janitor()
{
    reset();
}


// ---------------------------------------------------------------------------
//  Janitor: Public, non-virtual methods
// ---------------------------------------------------------------------------
template <class T> void
Janitor<T>::orphan()
{
   release();
}


template <class T> T&
Janitor<T>::operator*() const
{
	return *fData;
}


template <class T> T*
Janitor<T>::operator->() const
{
	return fData;
}


template <class T> T*
Janitor<T>::get() const
{
	return fData;
}


template <class T> T*
Janitor<T>::release()
{
	T* p = fData;
	fData = 0;
	return p;
}


template <class T> void Janitor<T>::reset(T* p)
{
    if (fData)
        delete fData;

    fData = p;
}

template <class T> bool Janitor<T>::isDataNull()
{
    return (fData == 0);
}


// -----------------------------------------------------------------------
//  ArrayJanitor: Constructors and Destructor
// -----------------------------------------------------------------------
template <class T> ArrayJanitor<T>::ArrayJanitor(T* const toDelete) :
    fData(toDelete)
    , fMemoryManager(0)
{
}

template <class T>
ArrayJanitor<T>::ArrayJanitor(T* const toDelete,
                              MemoryManager* const manager) :
    fData(toDelete)
    , fMemoryManager(manager)
{
}


template <class T> ArrayJanitor<T>::~ArrayJanitor()
{
	reset();
}


// -----------------------------------------------------------------------
//  ArrayJanitor: Public, non-virtual methods
// -----------------------------------------------------------------------
template <class T> void
ArrayJanitor<T>::orphan()
{
   release();
}


//	Look, Ma! No hands! Don't call this with null data!
template <class T> T&
ArrayJanitor<T>::operator[](int index) const
{
	//	TODO: Add appropriate exception
	return fData[index];
}


template <class T> T*
ArrayJanitor<T>::get() const
{
	return fData;
}


template <class T> T*
ArrayJanitor<T>::release()
{
	T* p = fData;
	fData = 0;
	return p;
}


template <class T> void
ArrayJanitor<T>::reset(T* p)
{
	if (fData) {

		if (fMemoryManager)
            fMemoryManager->deallocate((void*)fData);
        else
            delete [] fData;
    }

	fData = p;
    fMemoryManager = 0;
}

template <class T> void
ArrayJanitor<T>::reset(T* p, MemoryManager* const manager)
{
	if (fData) {

		if (fMemoryManager)
            fMemoryManager->deallocate((void*)fData);
        else
            delete [] fData;
    }

	fData = p;
    fMemoryManager = manager;
}

XERCES_CPP_NAMESPACE_END

