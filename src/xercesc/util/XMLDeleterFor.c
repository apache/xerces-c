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
 * Revision 1.3  2004/09/08 13:56:24  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.2  2002/11/04 15:22:05  tng
 * C++ Namespace Support.
 *
 * Revision 1.1.1.1  2002/02/01 22:22:14  peiyongz
 * sane_include
 *
 * Revision 1.1  2000/03/02 19:54:48  roddey
 * This checkin includes many changes done while waiting for the
 * 1.1.0 code to be finished. I can't list them all here, but a list is
 * available elsewhere.
 *
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#if defined(XERCES_TMPLSINC)
#include <xercesc/util/XMLDeleterFor.hpp>
#endif

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  XMLDeleterFor: Constructors and Destructor
// ---------------------------------------------------------------------------
template <class T> XMLDeleterFor<T>::XMLDeleterFor(T* const toDelete) :

    fToDelete(toDelete)
{
}

template <class T> XMLDeleterFor<T>::~XMLDeleterFor()
{
   delete fToDelete;
}

XERCES_CPP_NAMESPACE_END
