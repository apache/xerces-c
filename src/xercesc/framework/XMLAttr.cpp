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
#include <xercesc/framework/XMLAttr.hpp>
#include <xercesc/framework/MemoryManager.hpp>
#include <xercesc/util/OutOfMemoryException.hpp>

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  XMLAttr: Constructors and Destructor
// ---------------------------------------------------------------------------
XMLAttr::XMLAttr(MemoryManager* const manager) :

      fSpecified(false)
    , fType(XMLAttDef::CData)
    , fValueBufSz(0)
    , fValue(0)
    , fAttName(0)
    , fMemoryManager(manager)
    , fDatatypeValidator(0)
    , fIsSchemaValidated(false)
{
    fAttName = new (fMemoryManager) QName(fMemoryManager);
}

XMLAttr::XMLAttr(   const   unsigned int        uriId
                    , const XMLCh* const        attrName
                    , const XMLCh* const        attrPrefix
                    , const XMLCh* const        attrValue
                    , const XMLAttDef::AttTypes type
                    , const bool                specified
                    , MemoryManager* const      manager 
                    , DatatypeValidator * datatypeValidator
                    , const bool isSchema ):

      fSpecified(specified)
    , fType(type)
    , fValueBufSz(0)
    , fValue(0)
    , fAttName(0)
    , fMemoryManager(manager)
    , fDatatypeValidator(datatypeValidator)
    , fIsSchemaValidated(isSchema)
{
    try
    {
        //
        //  Just call the local setters to set up everything. Too much
        //  work is required to replicate that functionality here.
        //
        fAttName = new (fMemoryManager) QName(attrPrefix, attrName, uriId, fMemoryManager);
        setValue(attrValue);
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

XMLAttr::XMLAttr(   const   unsigned int        uriId
                    , const XMLCh* const        rawName
                    , const XMLCh* const        attrValue
                    , const XMLAttDef::AttTypes type
                    , const bool                specified
                    , MemoryManager* const      manager 
                    , DatatypeValidator * datatypeValidator
                    , const bool isSchema ):

      fSpecified(specified)
    , fType(type)
    , fValueBufSz(0)
    , fValue(0)
    , fAttName(0)
    , fMemoryManager(manager)
    , fDatatypeValidator(datatypeValidator)
    , fIsSchemaValidated(isSchema)
{
    try
    {
        //  Just call the local setters to set up everything. Too much
        //  work is required to replicate that functionality here.
        fAttName = new (fMemoryManager) QName(rawName, uriId, fMemoryManager);
        setValue(attrValue);
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


// ---------------------------------------------------------------------------
//  XMLAttr: Getter methods
// ---------------------------------------------------------------------------
const XMLCh* XMLAttr::getQName() const
{
    return fAttName->getRawName();
}


// ---------------------------------------------------------------------------
//  XMLAttr: Setter methods
// ---------------------------------------------------------------------------
void XMLAttr::setName(  const   unsigned int    uriId
                        , const XMLCh* const    attrName
                        , const XMLCh* const    attrPrefix)
{
    fAttName->setName(attrPrefix, attrName, uriId);
}


void XMLAttr::setURIId(const unsigned int uriId)
{
    fAttName->setURI(uriId);
}


void XMLAttr::setValue(const XMLCh* const newValue)
{
    const unsigned int newLen = XMLString::stringLen(newValue);
    if (!fValueBufSz || (newLen > fValueBufSz))
    {
        fMemoryManager->deallocate(fValue); //delete [] fValue;
        fValueBufSz = newLen + 8;
        fValue = (XMLCh*) fMemoryManager->allocate((fValueBufSz+1) * sizeof(XMLCh)); //new XMLCh[fValueBufSz + 1];
    }
    XMLString::moveChars(fValue, newValue, newLen + 1);
}


// ---------------------------------------------------------------------------
//  XMLAttr: Private, helper methods
// ---------------------------------------------------------------------------
void XMLAttr::cleanUp()
{
    delete fAttName;
    fMemoryManager->deallocate(fValue); //delete [] fValue;
}

XERCES_CPP_NAMESPACE_END

