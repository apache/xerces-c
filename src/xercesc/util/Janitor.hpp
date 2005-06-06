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
 * Revision 1.7  2005/04/26 17:58:12  cargilld
 * Fix for xercesc-1413: invalid operator equal template signature
 *
 * Revision 1.6  2004/09/08 13:56:22  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.5  2004/01/29 11:48:46  cargilld
 * Code cleanup changes to get rid of various compiler diagnostic messages.
 *
 * Revision 1.4  2003/11/06 19:28:11  knoaman
 * PSVI support for annotations.
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
 * Revision 1.7  2000/10/13 22:45:12  andyh
 * Complete removal of ArrayJanitory::operator->().  Was just commented out earlier.
 *
 * Revision 1.6  2000/10/10 23:52:11  andyh
 * From Janitor, remove the addition that is having compile problems in MSVC.
 *
 * Revision 1.5  2000/10/09 18:32:31  jberry
 * Add some auto_ptr functionality to allow modification of monitored
 * pointer value. This eases use of Janitor in some situations.
 *
 * Revision 1.4  2000/03/02 19:54:40  roddey
 * This checkin includes many changes done while waiting for the
 * 1.1.0 code to be finished. I can't list them all here, but a list is
 * available elsewhere.
 *
 * Revision 1.3  2000/02/24 20:05:24  abagchi
 * Swat for removing Log from API docs
 *
 * Revision 1.2  2000/02/06 07:48:02  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.1.1.1  1999/11/09 01:04:27  twl
 * Initial checkin
 *
 * Revision 1.2  1999/11/08 20:45:08  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */


#if !defined(JANITOR_HPP)
#define JANITOR_HPP

#include <xercesc/util/XMemory.hpp>
#include <xercesc/framework/MemoryManager.hpp>

XERCES_CPP_NAMESPACE_BEGIN

template <class T> class Janitor : public XMemory
{
public  :
    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    Janitor(T* const toDelete);
    ~Janitor();

    // -----------------------------------------------------------------------
    //  Public, non-virtual methods
    // -----------------------------------------------------------------------
    void orphan();

    //  small amount of auto_ptr compatibility
    T& operator*() const;
    T* operator->() const;
    T* get() const;
    T* release();
    void reset(T* p = 0);
    bool isDataNull();

private :
    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    Janitor();
    Janitor(const Janitor<T>&);
    Janitor<T>& operator=(const Janitor<T>&);

    // -----------------------------------------------------------------------
    //  Private data members
    //
    //  fData
    //      This is the pointer to the object or structure that must be
    //      destroyed when this object is destroyed.
    // -----------------------------------------------------------------------
    T*  fData;
};



template <class T> class ArrayJanitor : public XMemory
{
public  :
    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    ArrayJanitor(T* const toDelete);
    ArrayJanitor(T* const toDelete, MemoryManager* const manager);
    ~ArrayJanitor();


    // -----------------------------------------------------------------------
    //  Public, non-virtual methods
    // -----------------------------------------------------------------------
    void orphan();

	//	small amount of auto_ptr compatibility
	T&	operator[](int index) const;
	T*	get() const;
	T*	release();
	void reset(T* p = 0);
	void reset(T* p, MemoryManager* const manager);

private :
    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
	ArrayJanitor();
    ArrayJanitor(const ArrayJanitor<T>& copy);
    ArrayJanitor<T>& operator=(const ArrayJanitor<T>& copy);    

    // -----------------------------------------------------------------------
    //  Private data members
    //
    //  fData
    //      This is the pointer to the object or structure that must be
    //      destroyed when this object is destroyed.
    // -----------------------------------------------------------------------
    T*  fData;
    MemoryManager* fMemoryManager;
};



template <class T> class JanitorMemFunCall
{
public  :

    typedef void (T::*MFPT) ();

    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    JanitorMemFunCall(
        T*      object,
        MFPT    toCall);

    ~JanitorMemFunCall();

    // -----------------------------------------------------------------------
    //  Public, non-virtual methods
    // -----------------------------------------------------------------------
	void release();

private :
    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    JanitorMemFunCall();
    JanitorMemFunCall(const JanitorMemFunCall<T>&);
    JanitorMemFunCall<T>& operator=(const JanitorMemFunCall<T>&);

    // -----------------------------------------------------------------------
    //  Private data members
    //
    //  fObject
    //      This is the pointer to the object for which we will call the
    //      member function when this object is destroyed.
    // -----------------------------------------------------------------------
    T*      fObject;
    MFPT    fToCall;
};




XERCES_CPP_NAMESPACE_END

#if !defined(XERCES_TMPLSINC)
#include <xercesc/util/Janitor.c>
#endif

#endif
