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
#include <xercesc/framework/XMLElementDecl.hpp>
#include <xercesc/util/XMLUniDefs.hpp>
#include <xercesc/util/XMLUni.hpp>

#include <xercesc/validators/schema/SchemaElementDecl.hpp>
#include <xercesc/validators/DTD/DTDElementDecl.hpp>

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  XMLElementDecl: Public, static data
// ---------------------------------------------------------------------------
const unsigned int  XMLElementDecl::fgInvalidElemId    = 0xFFFFFFFE;
const unsigned int  XMLElementDecl::fgPCDataElemId     = 0xFFFFFFFF;
const XMLCh         XMLElementDecl::fgPCDataElemName[] =
{
        chPound, chLatin_P, chLatin_C, chLatin_D, chLatin_A
    ,   chLatin_T, chLatin_A, chNull
};



// ---------------------------------------------------------------------------
//  XMLElementDecl: Destructor
// ---------------------------------------------------------------------------
XMLElementDecl::~XMLElementDecl()
{
    delete fElementName;
}

// ---------------------------------------------------------------------------
//  XMLElementDecl: Setter Methods
// ---------------------------------------------------------------------------
void
XMLElementDecl::setElementName(const XMLCh* const       prefix
                            , const XMLCh* const        localPart
                            , const int                 uriId )
{
    if (fElementName)
        fElementName->setName(prefix, localPart, uriId);
    else
        fElementName = new (fMemoryManager) QName(prefix, localPart, uriId, fMemoryManager);
}

void
XMLElementDecl::setElementName(const XMLCh* const       rawName
                            , const int                 uriId )
{
    if (fElementName)
        fElementName->setName(rawName, uriId);
    else
        fElementName = new (fMemoryManager) QName(rawName, uriId, fMemoryManager);
}

void
XMLElementDecl::setElementName(const QName* const    elementName)
{
    if (fElementName)
        fElementName->setValues(*elementName);
    else
        fElementName = new (fMemoryManager) QName(*elementName);
}

// ---------------------------------------------------------------------------
//  ElementDecl: Hidden constructors
// ---------------------------------------------------------------------------
XMLElementDecl::XMLElementDecl(MemoryManager* const manager) :

    fMemoryManager(manager)
    , fElementName(0)
    , fCreateReason(XMLElementDecl::NoReason)
    , fId(XMLElementDecl::fgInvalidElemId)
    , fExternalElement(false)
{
}

/***
 * Support for Serialization/De-serialization
 ***/

IMPL_XSERIALIZABLE_NOCREATE(XMLElementDecl)

void XMLElementDecl::serialize(XSerializeEngine& serEng)
{

    if (serEng.isStoring())
    {
        serEng<<fElementName;
        serEng<<(int) fCreateReason;
        serEng<<fId;
        serEng<<fExternalElement;
    }
    else
    {
        serEng>>fElementName;

        int i;
        serEng>>i;
        fCreateReason=(CreateReasons)i;

        serEng>>fId;
        serEng>>fExternalElement;
    }

}

void 
XMLElementDecl::storeElementDecl(XSerializeEngine&        serEng
                               , XMLElementDecl*    const element)
{
    if (element)
    {
        serEng<<(int) element->getObjectType();
        serEng<<element;
    }
    else
    {
        serEng<<(int) UnKnown;
    }
}

XMLElementDecl* 
XMLElementDecl::loadElementDecl(XSerializeEngine& serEng)
{
    int type;
    serEng>>type;

    switch((XMLElementDecl::objectType)type)
    {
    case Schema:
        SchemaElementDecl* schemaElementDecl;
        serEng>>schemaElementDecl;
        return schemaElementDecl;
    case DTD:
        DTDElementDecl* dtdElementDecl;
        serEng>>dtdElementDecl;
        return dtdElementDecl;
    case UnKnown:
         //fall through
    default:
        return 0;
    }
}

XERCES_CPP_NAMESPACE_END

