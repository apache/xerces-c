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
 * Revision 1.5  2004/09/08 13:56:35  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.4  2004/05/11 13:39:35  amassari
 * The net accessor input source now can be used to get data using PUT or POST, as well as GET
 *
 * Revision 1.3  2003/03/07 18:15:49  tng
 * Return a reference instead of void for operator=
 *
 * Revision 1.2  2002/11/04 15:11:38  tng
 * C++ Namespace Support.
 *
 * Revision 1.1.1.1  2002/02/01 22:22:23  peiyongz
 * sane_include
 *
 * Revision 1.1  2000/03/20 23:48:51  rahulj
 * Added Socket based NetAccessor. This will enable one to
 * use HTTP URL's for system id's. Default build options do
 * not use this NetAccessor. Specify the '-n socket' option
 * to 'runConfigure' to configure Xerces-C to use this new
 * feature. The code works under Solaris 2.6, Linux, AIX
 * and HPUX 11 with aCC.
 * Todo's: enable proper error handling.
 *
 */


#if !defined(SOCKETNETACCESSOR_HPP)
#define SOCKETNETACCESSOR_HPP


#include <xercesc/util/XercesDefs.hpp>
#include <xercesc/util/XMLURL.hpp>
#include <xercesc/util/BinInputStream.hpp>
#include <xercesc/util/XMLNetAccessor.hpp>

XERCES_CPP_NAMESPACE_BEGIN

//
// This class is the wrapper for the socket based code which
// provides the ability to fetch a resource specified using
// a HTTP or FTP URL.
//

class XMLUTIL_EXPORT SocketNetAccessor : public XMLNetAccessor
{
public :
    SocketNetAccessor();
    ~SocketNetAccessor();

    virtual BinInputStream* makeNew(const XMLURL&  urlSource, const XMLNetHTTPInfo* httpInfo=0);
    virtual const XMLCh* getId() const;

private :
    static const XMLCh fgMyName[];

    SocketNetAccessor(const SocketNetAccessor&);
    SocketNetAccessor& operator=(const SocketNetAccessor&);

}; // SocketNetAccessor

inline const XMLCh* SocketNetAccessor::getId() const
{
    return fgMyName;
}


XERCES_CPP_NAMESPACE_END

#endif // SOCKETNETACCESSOR_HPP
