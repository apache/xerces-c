/*
 * Copyright 2004,2004 The Apache Software Foundation.
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
 * Revision 1.4  2004/09/08 13:56:46  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.3  2004/08/20 10:16:11  cargilld
 * Fix Jira bug XERCESC-1257, incorrect header guard.
 *
 * Revision 1.2  2004/04/22 22:46:46  neilg
 * not all 390 processors support the new transcoding instructions; this patch makes Xerces work there as well.  Thanks to Steve Dulin
 *
 * Revision 1.1  2004/02/06 15:02:11  cargilld
 * Intrinsic transcoding support for 390.
 *
 */

#ifndef XMLWIN1252TRANSCODER390_HPP
#define XMLWIN1252TRANSCODER390_HPP

#include <xercesc/util/XercesDefs.hpp>
#include <xercesc/util/Transcoders/Uniconv390/XML256TableTranscoder390.hpp>

XERCES_CPP_NAMESPACE_BEGIN


//
//  This class provides an implementation of the XMLTranscoder interface
//  for the Windows variant of Latin1, called Windows-1252. Its close to
//  Latin1, but is somewhat different.
//
class XMLUTIL_EXPORT XMLWin1252Transcoder390 : public XML256TableTranscoder390
{
public :
    // -----------------------------------------------------------------------
    //  Public constructors and destructor
    // -----------------------------------------------------------------------
    XMLWin1252Transcoder390
    (
        const   XMLCh* const    encodingName
        , const unsigned int    blockSize
        , MemoryManager* const  manager = XMLPlatformUtils::fgMemoryManager
    );

    virtual ~XMLWin1252Transcoder390();


private :
    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    XMLWin1252Transcoder390();
    XMLWin1252Transcoder390(const XMLWin1252Transcoder390&);
    XMLWin1252Transcoder390& operator=(const XMLWin1252Transcoder390&);
};

XERCES_CPP_NAMESPACE_END

#endif
