/*
 * Copyright 1999-2002,2004 The Apache Software Foundation.
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
 * Revision 1.12  2004/09/08 13:56:23  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.11  2003/09/10 14:56:31  neilg
 * fix compiler warnings on ISeries; add Apache copyright notice
 *
 * Revision 1.10  2003/05/30 16:11:45  gareth
 * Fixes so we compile under VC7.1. Patch by Alberto Massari.
 *
 * Revision 1.9  2003/05/16 06:01:52  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.8  2003/05/15 19:07:45  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.7  2003/02/06 16:11:30  peiyongz
 * Bug#16826: RefVectorOf.c has errors in strict ANSI mode,  patch from
 * David Bertoni (David_N_Bertoni@lotus.com )
 *
 * Revision 1.6  2002/12/17 21:06:02  gareth
 * Removed defaulting from parameters in c files as windows and gcc 3.2 say this is wrong.
 *
 * Revision 1.5  2002/12/17 17:17:58  gareth
 * added abstract base class BaseRefVectorOf from which both RefVectorOf and RefArrayVectorOf inherit
 * the new RefArrayVectorOf has proper destructor for array deletion
 *
 * Revision 1.4  2002/11/04 15:22:04  tng
 * C++ Namespace Support.
 *
 * Revision 1.3  2002/02/05 15:38:14  tng
 * [Bug 6114] Memory leaks on iDOM getElementsByTagName().
 *
 * Revision 1.2  2002/02/05 13:11:06  tng
 * [Bug 6114] Memory leaks on iDOM getElementsByTagName().
 *
 * Revision 1.1.1.1  2002/02/01 22:22:12  peiyongz
 * sane_include
 *
 * Revision 1.5  2001/06/25 13:01:49  knoaman
 * Add constraint checking on elements in complex types to prevent same
 * element names from having different definitions - use substitueGroups.
 *
 * Revision 1.4  2000/07/31 19:18:25  jpolast
 * bug fix in removeAll() to zero out all the pointers.
 *
 * Revision 1.3  2000/03/02 19:54:45  roddey
 * This checkin includes many changes done while waiting for the
 * 1.1.0 code to be finished. I can't list them all here, but a list is
 * available elsewhere.
 *
 * Revision 1.2  2000/02/06 07:48:03  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.1.1.1  1999/11/09 01:05:04  twl
 * Initial checkin
 *
 * Revision 1.2  1999/11/08 20:45:13  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#if defined(XERCES_TMPLSINC)
#include <xercesc/util/RefVectorOf.hpp>
#endif

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  RefVectorOf: Constructors and Destructor
// ---------------------------------------------------------------------------
template <class TElem>
RefVectorOf<TElem>::RefVectorOf(const unsigned int maxElems,
                                const bool adoptElems,
                                MemoryManager* const manager)
    : BaseRefVectorOf<TElem>(maxElems, adoptElems, manager)
{
}

template <class TElem> RefVectorOf<TElem>::~RefVectorOf()
{
    if (this->fAdoptedElems)
    {
       for (unsigned int index = 0; index < this->fCurCount; index++)
        delete this->fElemList[index];
    }
    this->fMemoryManager->deallocate(this->fElemList);//delete [] this->fElemList;
}


XERCES_CPP_NAMESPACE_END
