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
#include <xercesc/framework/XMLAttDef.hpp>
#include <xercesc/util/ArrayIndexOutOfBoundsException.hpp>
#include <xercesc/util/XMLUni.hpp>
#include <xercesc/util/OutOfMemoryException.hpp>

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  Local const data
//
//  gAttTypeStrings
//      A list of strings which are used to map attribute type numbers to
//      attribute type names.
//
//  gDefAttTypesStrings
//      A list of strings which are used to map default attribute type
//      numbers to default attribute type names.
// ---------------------------------------------------------------------------
const XMLCh* const gAttTypeStrings[XMLAttDef::AttTypes_Count] =
{
    XMLUni::fgCDATAString
    , XMLUni::fgIDString
    , XMLUni::fgIDRefString
    , XMLUni::fgIDRefsString
    , XMLUni::fgEntityString
    , XMLUni::fgEntitiesString
    , XMLUni::fgNmTokenString
    , XMLUni::fgNmTokensString
    , XMLUni::fgNotationString
    , XMLUni::fgEnumerationString
    , XMLUni::fgCDATAString
    , XMLUni::fgCDATAString
    , XMLUni::fgCDATAString
    , XMLUni::fgCDATAString

};

const XMLCh* const gDefAttTypeStrings[XMLAttDef::DefAttTypes_Count] =
{
    XMLUni::fgDefaultString
    , XMLUni::fgFixedString
    , XMLUni::fgRequiredString
    , XMLUni::fgImpliedString
    , XMLUni::fgImpliedString
    , XMLUni::fgImpliedString
    , XMLUni::fgImpliedString
    , XMLUni::fgImpliedString
    , XMLUni::fgImpliedString
};



// ---------------------------------------------------------------------------
//  XMLAttDef: Public, static data members
// ---------------------------------------------------------------------------
const unsigned int XMLAttDef::fgInvalidAttrId = 0xFFFFFFFE;


// ---------------------------------------------------------------------------
//  XMLAttDef: Public, static methods
// ---------------------------------------------------------------------------
const XMLCh* XMLAttDef::getAttTypeString(const XMLAttDef::AttTypes attrType
                                         , MemoryManager* const manager)
{
    // Check for an invalid attribute type and return a null
    if ((attrType < AttTypes_Min) || (attrType > AttTypes_Max))
        ThrowXMLwithMemMgr(ArrayIndexOutOfBoundsException, XMLExcepts::AttDef_BadAttType, manager);
    return gAttTypeStrings[attrType];
}

const XMLCh* XMLAttDef::getDefAttTypeString(const XMLAttDef::DefAttTypes attrType
                                            , MemoryManager* const manager)
{
    // Check for an invalid attribute type and return a null
    if ((attrType < DefAttTypes_Min) || (attrType > DefAttTypes_Max))
        ThrowXMLwithMemMgr(ArrayIndexOutOfBoundsException, XMLExcepts::AttDef_BadDefAttType, manager);
    return gDefAttTypeStrings[attrType];
}


// ---------------------------------------------------------------------------
//  XMLAttDef: Destructor
// ---------------------------------------------------------------------------
XMLAttDef::~XMLAttDef()
{
    cleanUp();
}


// ---------------------------------------------------------------------------
//  XMLAttDef: Hidden constructors
// ---------------------------------------------------------------------------
XMLAttDef::XMLAttDef( const XMLAttDef::AttTypes    type
                    , const XMLAttDef::DefAttTypes defType
                    , MemoryManager* const         manager) :

    fDefaultType(defType)
    , fType(type)
    , fCreateReason(XMLAttDef::NoReason)
    , fProvided(false)
    , fExternalAttribute(false)
    , fId(XMLAttDef::fgInvalidAttrId)
    , fValue(0)
    , fEnumeration(0)
    , fMemoryManager(manager)
{
}

XMLAttDef::XMLAttDef( const XMLCh* const           attrValue
                    , const XMLAttDef::AttTypes    type
                    , const XMLAttDef::DefAttTypes defType
                    , const XMLCh* const           enumValues
                    , MemoryManager* const         manager) :

    fDefaultType(defType)
    , fType(type)
    , fCreateReason(XMLAttDef::NoReason)
    , fProvided(false)
    , fExternalAttribute(false)
    , fId(XMLAttDef::fgInvalidAttrId)
    , fValue(0)
    , fEnumeration(0)
    , fMemoryManager(manager)
{
    try
    {
        fValue = XMLString::replicate(attrValue, fMemoryManager);
        fEnumeration = XMLString::replicate(enumValues, fMemoryManager);
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
//  XMLAttDef: Private helper methods
// ---------------------------------------------------------------------------
void XMLAttDef::cleanUp()
{
    if (fEnumeration)
        fMemoryManager->deallocate(fEnumeration);

    if (fValue)
        fMemoryManager->deallocate(fValue);
}

/***
 * Support for Serialization/De-serialization
 ***/

IMPL_XSERIALIZABLE_NOCREATE(XMLAttDef)

void XMLAttDef::serialize(XSerializeEngine& serEng)
{

    if (serEng.isStoring())
    {
        serEng<<(int)fDefaultType;
        serEng<<(int)fType;
        serEng<<(int)fCreateReason;
        serEng<<fProvided;
        serEng<<fExternalAttribute;
        serEng<<fId;

        serEng.writeString(fValue);
        serEng.writeString(fEnumeration);
    }
    else
    {
        int i;
        serEng>>i;
        fDefaultType = (DefAttTypes) i;

        serEng>>i;
        fType = (AttTypes)i;

        serEng>>i;
        fCreateReason = (CreateReasons)i;

        serEng>>fProvided;
        serEng>>fExternalAttribute;
        serEng>>fId;

        serEng.readString(fValue);
        serEng.readString(fEnumeration);
    }
}

XERCES_CPP_NAMESPACE_END

