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

/**
 * $Log$
 * Revision 1.5  2004/09/08 13:56:36  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.4  2004/05/11 13:39:35  amassari
 * The net accessor input source now can be used to get data using PUT or POST, as well as GET
 *
 * Revision 1.3  2003/03/07 18:15:50  tng
 * Return a reference instead of void for operator=
 *
 * Revision 1.2  2002/11/04 15:11:39  tng
 * C++ Namespace Support.
 *
 * Revision 1.1.1.1  2002/02/01 22:22:23  peiyongz
 * sane_include
 *
 * Revision 1.2  2001/03/02 14:39:27  tng
 * Enabling libWWW NetAccessor support under UNIX. Tested with latest tarball of libWWW
 * (w3c-libwww-5.3.2) under RedHat Linux 6.1.  Added by Martin Kalen.
 *
 * There is one MAJOR problem with the use of libwww and the patches
 * below, which someone with knowledge of libwww filters etc. might want
 * to look into. Default behavior for content-type text/xml is to consume
 * all xml data before it reaches the simple HTML presenter. Hence, only
 * files with content-type text/html will actually reach the xerces-c
 * library. If you have a *.xml file on the webbserver, processing of the
 * file will throw an exception stating "The main XML document cannot be
 * empty" (correct in a xerces point of view since if you enable debug
 * build you will see that libwww "eats" all text/xml).
 *
 * See "Diffs for enabling libWWW NetAccessor support under UNIX" posted in March 1, 2001
 * in the xerces-c-dev mailing list for further information.
 *
 * Revision 1.1  2000/02/17 22:06:19  rahulj
 * Moved the four LibWWW files to its own sub-directory in the
 * NetAccessor directory.
 *
 *
 * Revision 1.1  2000/01/15 01:08:04  rahulj
 * Added support for HTTP to the parser.
 * Error handling is not very good. Also cannot guarantee that
 * there are no memory leaks.
 * Only tested under NT 4.0 SP 5 using libWWW 5.2.8.
 *
 */


#if !defined(LIBWWWNETACCESSOR_HPP)
#define LIBWWWNETACCESSOR_HPP


#include <xercesc/util/XercesDefs.hpp>
#include <xercesc/util/XMLURL.hpp>
#include <xercesc/util/BinInputStream.hpp>
#include <xercesc/util/XMLNetAccessor.hpp>

XERCES_CPP_NAMESPACE_BEGIN

//
// This class is the wrapper for the libWWW library which provides
// support for HTTP and other network protocols, so that URL's using
// these protocols can be used in system id's in the XML decl clauses.
//

class XMLUTIL_EXPORT LibWWWNetAccessor : public XMLNetAccessor
{
public :
    LibWWWNetAccessor();
    ~LibWWWNetAccessor();

    BinInputStream* makeNew(const XMLURL&  urlSource, const XMLNetHTTPInfo* httpInfo=0);
    const XMLCh* getId() const;

private :
    static const XMLCh fgMyName[];

    LibWWWNetAccessor(const LibWWWNetAccessor&);
    LibWWWNetAccessor& operator=(const LibWWWNetAccessor&);

}; // LibWWWNetAccessor

inline const XMLCh* LibWWWNetAccessor::getId() const
{
    return fgMyName;
}

XERCES_CPP_NAMESPACE_END

#endif // LIBWWWNETACCESSOR_HPP
