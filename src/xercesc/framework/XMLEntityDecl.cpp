/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 1999-2003 The Apache Software Foundation.  All rights
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
 * $Id$
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/framework/XMLEntityDecl.hpp>
#include <xercesc/util/XMLUniDefs.hpp>
#include <xercesc/util/OutOfMemoryException.hpp>

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  XMLEntityDecl: Constructors and Destructor
// ---------------------------------------------------------------------------
XMLEntityDecl::XMLEntityDecl(MemoryManager* const manager) :

    fId(0)
    , fValueLen(0)
    , fValue(0)
    , fName(0)
    , fNotationName(0)
    , fPublicId(0)
    , fSystemId(0)
    , fBaseURI(0)
    , fMemoryManager(manager)
{
}

XMLEntityDecl::XMLEntityDecl(const XMLCh* const entName,
                             MemoryManager* const manager) :

    fId(0)
    , fValueLen(0)
    , fValue(0)
    , fName(0)
    , fNotationName(0)
    , fPublicId(0)
    , fSystemId(0)
    , fBaseURI(0)
    , fMemoryManager(manager)
{
    fName = XMLString::replicate(entName, fMemoryManager);
}

XMLEntityDecl::XMLEntityDecl(const  XMLCh* const   entName
                            , const XMLCh* const   value
                            , MemoryManager* const manager) :
    fId(0)
    , fValueLen(XMLString::stringLen(value))
    , fValue(0)
    , fName(0)
    , fNotationName(0)
    , fPublicId(0)
    , fSystemId(0)
    , fBaseURI(0)
    , fMemoryManager(manager)
{
    try
    {
        fValue = XMLString::replicate(value, fMemoryManager);
        fName = XMLString::replicate(entName, fMemoryManager);
    }
    catch(const OutOfMemoryException&)
    {
        throw;
    }
    catch(...)
    {
        cleanUp();
    }
}

XMLEntityDecl::XMLEntityDecl(const  XMLCh* const   entName
                            , const XMLCh          value
                            , MemoryManager* const manager) :
    fId(0)
    , fValueLen(1)
    , fValue(0)
    , fName(0)
    , fNotationName(0)
    , fPublicId(0)
    , fSystemId(0)
    , fBaseURI(0)
    , fMemoryManager(manager)
{
    try
    {
        XMLCh dummy[2] = { chNull, chNull };
        dummy[0] = value;
        fValue = XMLString::replicate(dummy, fMemoryManager);
        fName = XMLString::replicate(entName, fMemoryManager);
    }
    catch(const OutOfMemoryException&)
    {
        throw;
    }
    catch(...)
    {
        cleanUp();
    }
}

XMLEntityDecl::~XMLEntityDecl()
{
    cleanUp();
}


// ---------------------------------------------------------------------------
//  XMLEntityDecl: Setter methods
// ---------------------------------------------------------------------------
void XMLEntityDecl::setName(const XMLCh* const entName)
{
    // Clean up the current name stuff
    if (fName)
       fMemoryManager->deallocate(fName);

    fName = XMLString::replicate(entName, fMemoryManager);
}


// ---------------------------------------------------------------------------
//  XMLEntityDecl: Private helper methods
// ---------------------------------------------------------------------------
void XMLEntityDecl::cleanUp()
{
    fMemoryManager->deallocate(fName);
    fMemoryManager->deallocate(fNotationName);
    fMemoryManager->deallocate(fValue);
    fMemoryManager->deallocate(fPublicId);
    fMemoryManager->deallocate(fSystemId);
    fMemoryManager->deallocate(fBaseURI);
}

/***
 * Support for Serialization/De-serialization
 ***/

IMPL_XSERIALIZABLE_NOCREATE(XMLEntityDecl)

void XMLEntityDecl::serialize(XSerializeEngine& serEng)
{

    if (serEng.isStoring())
    {
        serEng<<fId;
        serEng<<fValueLen;
        serEng.writeString(fValue);
        serEng.writeString(fName);
        serEng.writeString(fNotationName);
        serEng.writeString(fPublicId);
        serEng.writeString(fSystemId);
        serEng.writeString(fBaseURI);
    }
    else
    {
        serEng>>fId;
        serEng>>fValueLen;
        serEng.readString(fValue);
        serEng.readString(fName);
        serEng.readString(fNotationName);
        serEng.readString(fPublicId);
        serEng.readString(fSystemId);
        serEng.readString(fBaseURI);
    }
}

XERCES_CPP_NAMESPACE_END

