/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 1999-2001 The Apache Software Foundation.  All rights
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
#include <xercesc/util/XMLUniDefs.hpp>
#include <xercesc/util/XMLUni.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/framework/XMLAttr.hpp>

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  XMLAttr: Constructors and Destructor
// ---------------------------------------------------------------------------
XMLAttr::XMLAttr() :
      fType(XMLAttDef::CData)
    , fValue(0)
    , fValueBufSz(0)
    , fSpecified(false)
    , fAttName(0)
{
    fAttName = new QName();
}

XMLAttr::XMLAttr(   const   unsigned int        uriId
                    , const XMLCh* const        attrName
                    , const XMLCh* const        attrPrefix
                    , const XMLCh* const        attrValue
                    , const XMLAttDef::AttTypes type
                    , const bool                specified) :

      fType(type)
    , fValue(0)
    , fValueBufSz(0)
    , fSpecified(specified)
{
    try
    {
        //
        //  Just call the local setters to set up everything. Too much
        //  work is required to replicate that functionality here.
        //
        fAttName = new QName(attrPrefix, attrName, uriId);
        setValue(attrValue);
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
        delete [] fValue;
        fValueBufSz = newLen + 8;
        fValue = new XMLCh[fValueBufSz + 1];
    }
    XMLString::moveChars(fValue, newValue, newLen + 1);
}


// ---------------------------------------------------------------------------
//  XMLAttr: Private, helper methods
// ---------------------------------------------------------------------------
void XMLAttr::cleanUp()
{
    delete fAttName;
    delete [] fValue;
}

XERCES_CPP_NAMESPACE_END

