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
 * Revision 1.9  2004/09/08 13:56:24  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.8  2003/12/17 00:18:35  cargilld
 * Update to memory management so that the static memory manager (one used to call Initialize) is only for static data.
 *
 * Revision 1.7  2003/12/01 23:23:27  neilg
 * fix for bug 25118; thanks to Jeroen Witmond
 *
 * Revision 1.6  2003/09/23 18:16:07  peiyongz
 * Inplementation for Serialization/Deserialization
 *
 * Revision 1.5  2003/05/16 06:01:53  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.4  2002/12/11 00:20:02  peiyongz
 * Doing businesss in value space. Converting out-of-bound value into special values.
 *
 * Revision 1.3  2002/11/04 15:22:05  tng
 * C++ Namespace Support.
 *
 * Revision 1.2  2002/02/20 18:17:02  tng
 * [Bug 5977] Warnings on generating apiDocs.
 *
 * Revision 1.1.1.1  2002/02/01 22:22:15  peiyongz
 * sane_include
 *
 * Revision 1.11  2001/11/28 15:39:26  peiyongz
 * return Type& for operator=
 *
 * Revision 1.10  2001/11/22 20:23:00  peiyongz
 * _declspec(dllimport) and inline warning C4273
 *
 * Revision 1.9  2001/11/19 21:33:42  peiyongz
 * Reorganization: Double/Float
 *
 * Revision 1.8  2001/10/26 16:37:46  peiyongz
 * Add thread safe code
 *
 * Revision 1.6  2001/09/27 14:54:20  peiyongz
 * DTV Reorganization: derived from XMLAbstractDoubleFloat
 *
 * Revision 1.5  2001/08/29 19:03:03  peiyongz
 * Bugzilla# 2816:on AIX 4.2, xlC 3 r ev.1, Compilation error on inline method
 *
 * Revision 1.4  2001/07/31 13:48:29  peiyongz
 * fValue removed
 *
 * Revision 1.3  2001/07/26 18:21:15  peiyongz
 * Boundary Checking
 *
 * Revision 1.2  2001/07/24 21:52:27  peiyongz
 * XMLDouble: move fg...String to XMLUni
 *
 * Revision 1.1  2001/07/24 13:58:11  peiyongz
 * XMLDouble and related supporting methods from XMLBigInteger/XMLBigDecimal
 *
 */

#ifndef XML_DOUBLE_HPP
#define XML_DOUBLE_HPP

#include <xercesc/util/XMLAbstractDoubleFloat.hpp>

XERCES_CPP_NAMESPACE_BEGIN

class XMLUTIL_EXPORT XMLDouble : public XMLAbstractDoubleFloat
{
public:

	/**
	 * Constructs a newly allocated <code>XMLDouble</code> object that
	 * represents the value represented by the string.
	 *
	 * @param      strValue the <code>String</code> to be converted to an
	 *                 <code>XMLDouble</code>.
         * @param manager    Pointer to the memory manager to be used to
         *                   allocate objects.
	 * @exception  NumberFormatException  if the <code>String</code> does not
	 *               contain a parsable XMLDouble.
	 */

    XMLDouble(const XMLCh* const strValue,
              MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);

    ~XMLDouble();

	/**
	 * Compares this object to the specified object.
	 * The result is <code>true</code> if and only if the argument is not
	 * <code>null</code> and is an <code>XMLDouble</code> object that contains
	 * the same <code>int</code> value as this object.
	 *
	 * @param   lValue the object to compare with.
	 * @param   rValue the object to compare against.
	 * @return  <code>true</code> if the objects are the same;
	 *          <code>false</code> otherwise.
	 */

    inline static int            compareValues(const XMLDouble* const lValue
                                             , const XMLDouble* const rValue);

    /***
     * Support for Serialization/De-serialization
     ***/
    DECL_XSERIALIZABLE(XMLDouble)

    XMLDouble(MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);

protected:

    void                  checkBoundary(const XMLCh* const strValue);

private:
    //
    // Unimplemented
    //
    // copy ctor
    // assignment ctor
    //
     XMLDouble(const XMLDouble& toCopy);
     XMLDouble& operator=(const XMLDouble& toAssign);

};

inline int XMLDouble::compareValues(const XMLDouble* const lValue
                                  , const XMLDouble* const rValue)
{
    return XMLAbstractDoubleFloat::compareValues((const XMLAbstractDoubleFloat* const) lValue,
                                                 (const XMLAbstractDoubleFloat* const) rValue 
                                                 , ((XMLAbstractDoubleFloat*)lValue)->getMemoryManager());
}

XERCES_CPP_NAMESPACE_END

#endif

