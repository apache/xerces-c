/*
 * Copyright 2001,2004 The Apache Software Foundation.
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
 * $Id$
 * $Log$
 * Revision 1.5  2004/09/08 13:56:24  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.4  2004/01/29 11:48:47  cargilld
 * Code cleanup changes to get rid of various compiler diagnostic messages.
 *
 * Revision 1.3  2003/05/15 19:07:46  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.2  2002/11/04 15:22:05  tng
 * C++ Namespace Support.
 *
 * Revision 1.1.1.1  2002/02/01 22:22:15  peiyongz
 * sane_include
 *
 * Revision 1.1  2001/08/16 21:53:52  peiyongz
 * new class creation
 *
 */

#ifndef XML_INTEGER_HPP
#define XML_INTEGER_HPP

#include <xercesc/util/XMemory.hpp>

XERCES_CPP_NAMESPACE_BEGIN

class XMLUTIL_EXPORT XMLInteger : public XMemory
{
public:

	/**
	 * Constructs a newly allocated <code>XMLInteger</code> object
     *
	 * @param      intVal   the <code>integer</code>
	 */

	XMLInteger(const int intVal);

	~XMLInteger();

	/**
	 * Returns the built in integer value.
	 */
    int intValue() const;

private:
    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    XMLInteger(const XMLInteger&);
    XMLInteger& operator=(const XMLInteger&);

    // -----------------------------------------------------------------------
    //  Private data members
    //
	//  fData
    //     the value
    //
    // -----------------------------------------------------------------------
    int         fData;

};

inline XMLInteger::XMLInteger(const int intVal)
:fData(intVal)
{
}

inline XMLInteger::~XMLInteger()
{
}

inline int XMLInteger::intValue() const
{
    return fData;
}

XERCES_CPP_NAMESPACE_END

#endif
