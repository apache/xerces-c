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
 * Revision 1.5  2004/09/08 13:56:24  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.4  2004/01/29 11:48:47  cargilld
 * Code cleanup changes to get rid of various compiler diagnostic messages.
 *
 * Revision 1.3  2003/03/07 18:11:55  tng
 * Return a reference instead of void for operator=
 *
 * Revision 1.2  2002/11/04 15:22:05  tng
 * C++ Namespace Support.
 *
 * Revision 1.1.1.1  2002/02/01 22:22:14  peiyongz
 * sane_include
 *
 * Revision 1.2  2000/03/09 22:38:08  abagchi
 * Changed copy constructor to make it work on SunOS 5.7
 *
 * Revision 1.1  2000/03/02 19:54:48  roddey
 * This checkin includes many changes done while waiting for the
 * 1.1.0 code to be finished. I can't list them all here, but a list is
 * available elsewhere.
 *
 */


#if !defined(XMLDELETERFOR_HPP)
#define XMLDELETERFOR_HPP

#include <xercesc/util/XercesDefs.hpp>
#include <xercesc/util/PlatformUtils.hpp>

XERCES_CPP_NAMESPACE_BEGIN

//
//  For internal use only.
//
//  This class is used by the platform utilities class to support cleanup
//  of global/static data which is lazily created. Since that data is
//  widely spread out, and in higher level DLLs, the platform utilities
//  class cannot know about them directly. So, the code that creates such
//  objects creates an registers a deleter for the object. The platform
//  termination call will iterate the list and delete the objects.
//
template <class T> class XMLDeleterFor : public XMLDeleter
{
public :
    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    XMLDeleterFor(T* const toDelete);
    ~XMLDeleterFor();


private :
    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    XMLDeleterFor();
    XMLDeleterFor(const XMLDeleterFor<T>&);
    XMLDeleterFor<T>& operator=(const XMLDeleterFor<T>&);


    // -----------------------------------------------------------------------
    //  Private data members
    //
    //  fToDelete
    //      This is a pointer to the data to destroy
    // -----------------------------------------------------------------------
    T*  fToDelete;
};

XERCES_CPP_NAMESPACE_END

#if !defined(XERCES_TMPLSINC)
#include <xercesc/util/XMLDeleterFor.c>
#endif

#endif
