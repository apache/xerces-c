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
 * Revision 1.10  2004/09/08 13:56:23  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.9  2004/01/29 11:48:46  cargilld
 * Code cleanup changes to get rid of various compiler diagnostic messages.
 *
 * Revision 1.8  2003/05/16 06:01:52  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.7  2003/05/15 19:07:45  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.6  2002/12/17 17:17:58  gareth
 * added abstract base class BaseRefVectorOf from which both RefVectorOf and RefArrayVectorOf inherit
 * the new RefArrayVectorOf has proper destructor for array deletion
 *
 * Revision 1.5  2002/12/04 02:32:43  knoaman
 * #include cleanup.
 *
 * Revision 1.4  2002/11/04 15:22:04  tng
 * C++ Namespace Support.
 *
 * Revision 1.3  2002/08/21 17:45:00  tng
 * [Bug 7087] compiler warnings when using gcc.
 *
 * Revision 1.2  2002/02/05 13:11:06  tng
 * [Bug 6114] Memory leaks on iDOM getElementsByTagName().
 *
 * Revision 1.1.1.1  2002/02/01 22:22:12  peiyongz
 * sane_include
 *
 * Revision 1.5  2001/06/25 13:01:52  knoaman
 * Add constraint checking on elements in complex types to prevent same
 * element names from having different definitions - use substitueGroups.
 *
 * Revision 1.4  2000/03/02 19:54:45  roddey
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
 * Revision 1.1.1.1  1999/11/09 01:05:05  twl
 * Initial checkin
 *
 * Revision 1.2  1999/11/08 20:45:13  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */

#if !defined(REFVECTOROF_HPP)
#define REFVECTOROF_HPP

#include <xercesc/util/BaseRefVectorOf.hpp>

XERCES_CPP_NAMESPACE_BEGIN

/** 
 * Class with implementation for vectors of References - implements from the 
 * Abstract class Vector
 */ 
template <class TElem> class RefVectorOf : public BaseRefVectorOf<TElem>
{
public :
    // -----------------------------------------------------------------------
    //  Constructor
    // -----------------------------------------------------------------------
    RefVectorOf(const unsigned int maxElems,
                const bool adoptElems = true,
                MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);

    // -----------------------------------------------------------------------
    //  Destructor
    // -----------------------------------------------------------------------
    ~RefVectorOf();

private:
    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    RefVectorOf(const RefVectorOf<TElem>&);
    RefVectorOf<TElem>& operator=(const RefVectorOf<TElem>&);
};

XERCES_CPP_NAMESPACE_END

#if !defined(XERCES_TMPLSINC)
#include <xercesc/util/RefVectorOf.c>
#endif

#endif
