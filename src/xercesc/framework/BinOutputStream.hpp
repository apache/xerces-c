/*
 * Copyright 2003,2004 The Apache Software Foundation.
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
 * Revision 1.4  2004/09/08 13:55:57  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.3  2004/02/16 04:02:34  neilg
 * fix for bug 26936
 *
 * Revision 1.2  2003/12/16 17:17:25  peiyongz
 * typo fixed
 *
 * Revision 1.1  2003/09/18 18:29:58  peiyongz
 * Interface: Binary Output Stream
 *
 * $Id$
 *
 */

#if !defined(BIN_OUTPUT_STREAM_HPP)
#define BIN_OUTPUT_STREAM_HPP

#include <xercesc/util/XMemory.hpp>

XERCES_CPP_NAMESPACE_BEGIN

class XMLUTIL_EXPORT BinOutputStream : public XMemory
{
public :
    // -----------------------------------------------------------------------
    //  Virtual destructor for derived classes
    // -----------------------------------------------------------------------
    virtual ~BinOutputStream();

    // -----------------------------------------------------------------------
    //  The virtual output stream interface
    // -----------------------------------------------------------------------
    virtual unsigned int curPos() const = 0;

    virtual void writeBytes
    (
          const XMLByte* const      toGo
        , const unsigned int        maxToWrite
    ) = 0;

protected :
    // -----------------------------------------------------------------------
    //  Hidden Constructors
    // -----------------------------------------------------------------------
    BinOutputStream();


private :
    // -----------------------------------------------------------------------
    //  Unimplemented Constructors
    // -----------------------------------------------------------------------
    BinOutputStream(const BinOutputStream&);
    BinOutputStream& operator=(const BinOutputStream&);
};

XERCES_CPP_NAMESPACE_END

#endif
