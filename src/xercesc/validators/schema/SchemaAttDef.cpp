/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 2001 The Apache Software Foundation.  All rights
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
 * originally based on software copyright (c) 2001, International
 * Business Machines, Inc., http://www.ibm.com .  For more information
 * on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 */

/*
 * $Log$
 * Revision 1.1  2002/02/01 22:22:45  peiyongz
 * Initial revision
 *
 * Revision 1.4  2001/08/09 15:23:16  knoaman
 * add support for <anyAttribute> declaration.
 *
 * Revision 1.3  2001/07/31 15:26:54  knoaman
 * Added support for <attributeGroup>.
 *
 * Revision 1.2  2001/05/11 13:27:33  tng
 * Copyright update.
 *
 * Revision 1.1  2001/02/27 18:48:22  tng
 * Schema: Add SchemaAttDef, SchemaElementDecl, SchemaAttDefList.
 *
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/framework/XMLElementDecl.hpp>
#include <xercesc/validators/schema/SchemaAttDef.hpp>

// ---------------------------------------------------------------------------
//  SchemaAttDef: Implementation of the XMLAttDef interface
// ---------------------------------------------------------------------------
const XMLCh* SchemaAttDef::getFullName() const
{
   return fAttName->getRawName();
}

// ---------------------------------------------------------------------------
//  SchemaAttDef: Constructors and Destructor
// ---------------------------------------------------------------------------
SchemaAttDef::SchemaAttDef() :
    fElemId(XMLElementDecl::fgInvalidElemId)
    , fAttName(0)
    , fDatatypeValidator(0)
    , fNamespaceList(0)
{
}

SchemaAttDef::SchemaAttDef( const XMLCh* const                     prefix
                          , const XMLCh* const                     localPart
                          , const int                              uriId
                          , const XMLAttDef::AttTypes              type
                          , const XMLAttDef::DefAttTypes           defType) :
    XMLAttDef(type, defType)
    , fElemId(XMLElementDecl::fgInvalidElemId)
    , fDatatypeValidator(0)
    , fNamespaceList(0)
{
    fAttName = new QName(prefix, localPart, uriId);
}

SchemaAttDef::SchemaAttDef( const XMLCh* const                     prefix
                          , const XMLCh* const                     localPart
                          , const int                              uriId
                          , const XMLCh* const                     attValue
                          , const XMLAttDef::AttTypes              type
                          , const XMLAttDef::DefAttTypes           defType
                          , const XMLCh* const                     enumValues) :

    XMLAttDef(attValue, type, defType, enumValues)
    , fElemId(XMLElementDecl::fgInvalidElemId)
    , fDatatypeValidator(0)
    , fNamespaceList(0)
{
    fAttName = new QName(prefix, localPart, uriId);
}

SchemaAttDef::SchemaAttDef(const SchemaAttDef* other) :

    XMLAttDef(other->getValue(), other->getType(),
              other->getDefaultType(), other->getEnumeration())
    , fElemId(XMLElementDecl::fgInvalidElemId)
    , fAttName(0)
    , fDatatypeValidator(other->fDatatypeValidator)
    , fNamespaceList(0)
{
    QName* otherName = other->getAttName();
    fAttName = new QName(otherName->getPrefix(), 
                         otherName->getLocalPart(), otherName->getURI());

    if (other->fNamespaceList && other->fNamespaceList->size()) {
        fNamespaceList = new ValueVectorOf<unsigned int>(*(other->fNamespaceList));
    }
}

SchemaAttDef::~SchemaAttDef()
{
   delete fAttName;
   delete fNamespaceList;
}


// ---------------------------------------------------------------------------
//  SchemaAttDef: Setter methods
// ---------------------------------------------------------------------------
void SchemaAttDef::setAttName(const XMLCh* const        prefix
                            , const XMLCh* const        localPart
                            , const int                 uriId )
{
   fAttName->setName(prefix, localPart, uriId);
}
