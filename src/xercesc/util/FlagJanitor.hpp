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
 * Revision 1.4  2004/09/08 13:56:21  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.3  2003/03/07 18:11:54  tng
 * Return a reference instead of void for operator=
 *
 * Revision 1.2  2002/11/04 15:22:03  tng
 * C++ Namespace Support.
 *
 * Revision 1.1.1.1  2002/02/01 22:22:10  peiyongz
 * sane_include
 *
 * Revision 1.4  2000/03/02 19:54:39  roddey
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
 * Revision 1.1.1.1  1999/11/09 01:04:17  twl
 * Initial checkin
 *
 * Revision 1.2  1999/11/08 20:45:06  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */


#if !defined(FLAGJANITOR_HPP)
#define FLAGJANITOR_HPP

#include <xercesc/util/XercesDefs.hpp>

XERCES_CPP_NAMESPACE_BEGIN

template <class T> class FlagJanitor
{
public :
    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    FlagJanitor(T* const valPtr, const T newVal);
    ~FlagJanitor();


    // -----------------------------------------------------------------------
    //  Value management methods
    // -----------------------------------------------------------------------
    void release();


private :
    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    FlagJanitor();
    FlagJanitor(const FlagJanitor<T>&);
    FlagJanitor<T>& operator=(const FlagJanitor<T>&);


    // -----------------------------------------------------------------------
    //  Private data members
    //
    //  fOldVal
    //      The old value that was in the flag when we were constructed.
    //
    //  fValPtr
    //      A pointer to the flag that we are to restore the value of
    // -----------------------------------------------------------------------
    T   fOldVal;
    T*  fValPtr;
};

XERCES_CPP_NAMESPACE_END

#if !defined(XERCES_TMPLSINC)
#include <xercesc/util/FlagJanitor.c>
#endif

#endif
