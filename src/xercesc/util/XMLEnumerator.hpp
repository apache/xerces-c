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
 * Revision 1.6  2005/01/07 15:12:10  amassari
 * Removed warnings
 *
 * Revision 1.5  2004/09/08 13:56:24  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.4  2004/01/29 11:48:47  cargilld
 * Code cleanup changes to get rid of various compiler diagnostic messages.
 *
 * Revision 1.3  2002/11/04 15:22:05  tng
 * C++ Namespace Support.
 *
 * Revision 1.2  2002/08/21 17:45:00  tng
 * [Bug 7087] compiler warnings when using gcc.
 *
 * Revision 1.1.1.1  2002/02/01 22:22:15  peiyongz
 * sane_include
 *
 * Revision 1.4  2000/03/02 19:54:48  roddey
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
 * Revision 1.1.1.1  1999/11/09 01:05:37  twl
 * Initial checkin
 *
 * Revision 1.2  1999/11/08 20:45:19  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */


#if !defined(XMLENUMERATOR_HPP)
#define XMLENUMERATOR_HPP

#include <xercesc/util/XercesDefs.hpp>

XERCES_CPP_NAMESPACE_BEGIN

template <class TElem> class XMLEnumerator
{
public :
    // -----------------------------------------------------------------------
    //  Destructor
    // -----------------------------------------------------------------------
    virtual ~XMLEnumerator() {};

    // -----------------------------------------------------------------------
    //  XMLEnumerator interface
    // -----------------------------------------------------------------------
    virtual bool hasMoreElements() const = 0;
    virtual TElem& nextElement() = 0;
    virtual void Reset() = 0;

    XMLEnumerator() {}
    XMLEnumerator(const XMLEnumerator<TElem>&) {}

private:
	// -----------------------------------------------------------------------
    //  Unimplemented operators
    // -----------------------------------------------------------------------
    XMLEnumerator<TElem>& operator=(const XMLEnumerator<TElem>&);    
};

XERCES_CPP_NAMESPACE_END

#endif
