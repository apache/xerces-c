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

/*
 * $Id$
 * $Log$
 * Revision 1.5  2004/09/08 13:55:58  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.4  2003/01/24 20:20:22  tng
 * Add method flush to XMLFormatTarget
 *
 * Revision 1.3  2002/11/04 15:00:21  tng
 * C++ Namespace Support.
 *
 * Revision 1.2  2002/06/05 15:47:29  peiyongz
 * operator = modified
 *
 * Revision 1.1  2002/05/28 22:40:46  peiyongz
 * DOM3 Save Interface: DOMWriter/DOMWriterFilter
 *
 */

#ifndef StdOutFormatTarget_HEADER_GUARD_
#define StdOutFormatTarget_HEADER_GUARD_

#include <xercesc/framework/XMLFormatter.hpp>

XERCES_CPP_NAMESPACE_BEGIN

class XMLPARSER_EXPORT StdOutFormatTarget : public XMLFormatTarget {
public:

    /** @name constructors and destructor */
    //@{
    StdOutFormatTarget() ;
    ~StdOutFormatTarget();
    //@}

    // -----------------------------------------------------------------------
    //  Implementations of the format target interface
    // -----------------------------------------------------------------------
    virtual void writeChars(const XMLByte* const toWrite
                          , const unsigned int   count
                          , XMLFormatter* const  formatter);

    virtual void flush();

private:
    // -----------------------------------------------------------------------
    //  Unimplemented methods.
    // -----------------------------------------------------------------------
    StdOutFormatTarget(const StdOutFormatTarget&);
    StdOutFormatTarget& operator=(const StdOutFormatTarget&);
};

XERCES_CPP_NAMESPACE_END

#endif
