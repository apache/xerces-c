/*
 * The Apache Software License, Version 1.1
 * 
 * Copyright (c) 1999-2000 The Apache Software Foundation.  All rights
 * reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer. 
 * 
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 
 * 3. The end-user documentation included with the redistribution,
 *    if any, must include the following acknowledgment:  
 *       "This product includes software developed by the
 *        Apache Software Foundation (http://www.apache.org/)."
 *    Alternately, this acknowledgment may appear in the software itself,
 *    if and wherever such third-party acknowledgments normally appear.
 * 
 * 4. The names "Xerces" and "Apache Software Foundation" must
 *    not be used to endorse or promote products derived from this
 *    software without prior written permission. For written 
 *    permission, please contact apache\@apache.org.
 * 
 * 5. Products derived from this software may not be called "Apache",
 *    nor may "Apache" appear in their name, without prior written
 *    permission of the Apache Software Foundation.
 * 
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE APACHE SOFTWARE FOUNDATION OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * ====================================================================
 * 
 * This software consists of voluntary contributions made by many
 * individuals on behalf of the Apache Software Foundation, and was
 * originally based on software copyright (c) 1999, International
 * Business Machines, Inc., http://www.ibm.com .  For more information
 * on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 */

/**
 * $Log$
 * Revision 1.2  2000/02/06 07:48:05  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.1.1.1  1999/11/09 01:05:47  twl
 * Initial checkin
 *
 * Revision 1.2  1999/11/08 20:45:20  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */


#if !defined(XMLMSGLOADER_HPP)
#define XMLMSGLOADER_HPP

#include <util/XML4CDefs.hpp>

//
//  This header defines an abstract message loading API. This is the API via
//  which the parser system loads translatable text, and there can be multiple
//  actual implementations of this mechanism. The API is very simple because
//  there can be many kinds of underlying systems on which implementations are
//  based and we don't want to get into portability trouble by being overly
//  smart.
//
//  Each instance of the message loader loads a file of messages, which are
//  accessed by key and which are associated with a particular language. The
//  actual source information may be in many forms, but by the time it is
//  extracted for use it will be in Unicode format. The language is always
//  the default language for the local machine.
//
//  Msg loader derivatives are not required to be thread safe. The parser will
//  never use a single instance in more than one thread.
//
class XMLUTIL_EXPORT XMLMsgLoader
{
public :
    // -----------------------------------------------------------------------
    //  Class specific types
    //
    //  XMLMsgId
    //      A simple typedef to give us flexibility about the representation
    //      of a message id.
    // -----------------------------------------------------------------------
    typedef unsigned int    XMLMsgId;


    // -----------------------------------------------------------------------
    //  Public Constructors and Destructor
    // -----------------------------------------------------------------------
    virtual ~XMLMsgLoader();


    // -----------------------------------------------------------------------
    //  The virtual message loader API
    // -----------------------------------------------------------------------
    virtual bool loadMsg
    (
        const   XMLMsgId            msgToLoad
        ,       XMLCh* const        toFill
        , const unsigned long       maxChars
    ) = 0;

    virtual bool loadMsg
    (
        const   XMLMsgId            msgToLoad
        ,       XMLCh* const        toFill
        , const unsigned long       maxChars
        , const XMLCh* const        repText1
        , const XMLCh* const        repText2 = 0
        , const XMLCh* const        repText3 = 0
        , const XMLCh* const        repText4 = 0
    ) = 0;

    virtual bool loadMsg
    (
        const   XMLMsgId            msgToLoad
        ,       XMLCh* const        toFill
        , const unsigned long       maxChars
        , const char* const         repText1
        , const char* const         repText2 = 0
        , const char* const         repText3 = 0
        , const char* const         repText4 = 0
    ) = 0;


    // -----------------------------------------------------------------------
    //  Getter methods
    // -----------------------------------------------------------------------
    const XMLCh* getLanguageName() const;


protected :
    // -----------------------------------------------------------------------
    //  Hidden Constructors
    // -----------------------------------------------------------------------
    XMLMsgLoader();


    // -----------------------------------------------------------------------
    //  Protected helper methods
    // -----------------------------------------------------------------------
    void setLanguageName(XMLCh* const nameToAdopt);


private :
    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    XMLMsgLoader(const XMLMsgLoader&);
    void operator=(const XMLMsgLoader&);


    // -----------------------------------------------------------------------
    //  Private data members
    //
    //  fLanguage
    //      This is the language that is represented by this instance of the
    //      loader. The derived class must pass this through for storage and
    //      we adopt it and delete it when we go.
    // -----------------------------------------------------------------------
    XMLCh*  fLanguage;
};


// ---------------------------------------------------------------------------
//  XMLMsgLoader: Public Constructors and Destructor
// ---------------------------------------------------------------------------
inline XMLMsgLoader::~XMLMsgLoader()
{
    delete [] fLanguage;
}


// ---------------------------------------------------------------------------
//  XMLMsgLoader: Hidden Constructors
// ---------------------------------------------------------------------------
inline XMLMsgLoader::XMLMsgLoader() :

    fLanguage(0)
{
}


// ---------------------------------------------------------------------------
//  XMLMsgLoader: Getter methods
// ---------------------------------------------------------------------------
inline const XMLCh* XMLMsgLoader::getLanguageName() const
{
    return fLanguage;
}


// ---------------------------------------------------------------------------
//  XMLMsgLoader: Protected helper methods
// ---------------------------------------------------------------------------
inline void XMLMsgLoader::setLanguageName(XMLCh* const nameToAdopt)
{
    delete [] fLanguage;
    fLanguage = nameToAdopt;
}

#endif
