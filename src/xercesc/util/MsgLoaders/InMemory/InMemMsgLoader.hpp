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
 * Revision 1.6  2004/09/08 13:56:33  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.5  2003/12/24 15:24:13  cargilld
 * More updates to memory management so that the static memory manager.
 *
 * Revision 1.4  2003/12/17 03:56:15  neilg
 * add default memory manager parameter to loadMsg method that uses char * parameters
 *
 * Revision 1.3  2003/03/07 18:15:44  tng
 * Return a reference instead of void for operator=
 *
 * Revision 1.2  2002/11/04 15:10:40  tng
 * C++ Namespace Support.
 *
 * Revision 1.1.1.1  2002/02/01 22:22:21  peiyongz
 * sane_include
 *
 * Revision 1.4  2000/03/28 19:43:21  roddey
 * Fixes for signed/unsigned warnings. New work for two way transcoding
 * stuff.
 *
 * Revision 1.3  2000/03/02 19:55:15  roddey
 * This checkin includes many changes done while waiting for the
 * 1.1.0 code to be finished. I can't list them all here, but a list is
 * available elsewhere.
 *
 * Revision 1.2  2000/02/06 07:48:22  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.1.1.1  1999/11/09 01:07:20  twl
 * Initial checkin
 *
 * Revision 1.2  1999/11/08 20:45:27  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */


#if !defined(INMEMMSGLOADER_HPP)
#define INMEMMSGLOADER_HPP

#include <xercesc/util/XercesDefs.hpp>
#include <xercesc/util/XMLMsgLoader.hpp>

XERCES_CPP_NAMESPACE_BEGIN

//
//  This is a simple in memory message loader implementation. For those
//  folks who just want a single language and want something very fast and
//  efficient, can basically just provide a couple of arrays of Unicode
//  strings that can be looked up by the message id.
//
class XMLUTIL_EXPORT InMemMsgLoader : public XMLMsgLoader
{
public :
    // -----------------------------------------------------------------------
    //  Public Constructors and Destructor
    // -----------------------------------------------------------------------
    InMemMsgLoader(const XMLCh* const msgDomain);
    ~InMemMsgLoader();


    // -----------------------------------------------------------------------
    //  Implementation of the virtual message loader API
    // -----------------------------------------------------------------------
    virtual bool loadMsg
    (
        const   XMLMsgLoader::XMLMsgId  msgToLoad
        ,       XMLCh* const            toFill
        , const unsigned int            maxChars
    );

    virtual bool loadMsg
    (
        const   XMLMsgLoader::XMLMsgId  msgToLoad
        ,       XMLCh* const            toFill
        , const unsigned int            maxChars
        , const XMLCh* const            repText1
        , const XMLCh* const            repText2 = 0
        , const XMLCh* const            repText3 = 0
        , const XMLCh* const            repText4 = 0
        , MemoryManager* const          manager  = XMLPlatformUtils::fgMemoryManager
    );

    virtual bool loadMsg
    (
        const   XMLMsgLoader::XMLMsgId  msgToLoad
        ,       XMLCh* const            toFill
        , const unsigned int            maxChars
        , const char* const             repText1
        , const char* const             repText2 = 0
        , const char* const             repText3 = 0
        , const char* const             repText4 = 0
        , MemoryManager * const         manager  = XMLPlatformUtils::fgMemoryManager
    );


private :
    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    InMemMsgLoader();
    InMemMsgLoader(const InMemMsgLoader&);
    InMemMsgLoader& operator=(const InMemMsgLoader&);


    // -----------------------------------------------------------------------
    //  Private data members
    //
    //  fMsgDomain
    //      This is the message domain that we are for loading message from.
    // -----------------------------------------------------------------------
    XMLCh*  fMsgDomain;
};

XERCES_CPP_NAMESPACE_END

#endif
