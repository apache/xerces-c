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
 * originally based on software copyright (c) 1999, International
 * Business Machines, Inc., http://www.ibm.com .  For more information
 * on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 */

/*
 * $Id$
 * $Log$
 * Revision 1.6  2003/05/15 18:53:26  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.5  2003/01/06 19:40:05  knoaman
 * Throw an invalid datatype value exception if fEntityDeclPool is NULL.
 *
 * Revision 1.4  2002/12/18 14:17:55  gareth
 * Fix to bug #13438. When you eant a vector that calls delete[] on its members you should use RefArrayVectorOf.
 *
 * Revision 1.3  2002/11/04 14:53:28  tng
 * C++ Namespace Support.
 *
 * Revision 1.2  2002/09/24 19:44:40  tng
 * Performance: use XMLString::equals instead of XMLString::compareString
 *
 * Revision 1.1.1.1  2002/02/01 22:22:41  peiyongz
 * sane_include
 *
 * Revision 1.7  2001/11/13 13:21:59  tng
 * Schema fix: Entities should be unparsed.
 *
 * Revision 1.6  2001/10/09 20:50:27  peiyongz
 * init(): take 1 arg
 *
 * Revision 1.5  2001/09/27 13:51:25  peiyongz
 * DTV Reorganization: ctor/init created to be used by derived class
 *
 * Revision 1.4  2001/09/24 21:39:29  peiyongz
 * DTV Reorganization: checkValueSpace()
 *
 * Revision 1.3  2001/08/14 22:11:56  peiyongz
 * new exception message added
 *
 * Revision 1.2  2001/07/24 21:23:39  tng
 * Schema: Use DatatypeValidator for ID/IDREF/ENTITY/ENTITIES/NOTATION.
 *
 * Revision 1.1  2001/07/06 20:19:52  peiyongz
 * ENTITYDTV
 *
 */

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/validators/datatype/ENTITYDatatypeValidator.hpp>
#include <xercesc/validators/datatype/InvalidDatatypeValueException.hpp>
#include <xercesc/framework/XMLEntityDecl.hpp>

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  Constructors and Destructor
// ---------------------------------------------------------------------------
ENTITYDatatypeValidator::ENTITYDatatypeValidator(MemoryManager* const manager)
:StringDatatypeValidator(0, 0, 0, DatatypeValidator::ENTITY, manager)
,fEntityDeclPool(0)
{}

ENTITYDatatypeValidator::ENTITYDatatypeValidator(
                          DatatypeValidator*            const baseValidator
                        , RefHashTableOf<KVStringPair>* const facets
                        , RefArrayVectorOf<XMLCh>*      const enums
                        , const int                           finalSet
                        , MemoryManager* const                manager)
:StringDatatypeValidator(baseValidator, facets, finalSet, DatatypeValidator::ENTITY, manager)
,fEntityDeclPool(0)
{
    init(enums);
}

ENTITYDatatypeValidator::~ENTITYDatatypeValidator()
{}

DatatypeValidator* ENTITYDatatypeValidator::newInstance
(
      RefHashTableOf<KVStringPair>* const facets
    , RefArrayVectorOf<XMLCh>* const      enums
    , const int                           finalSet
    , MemoryManager* const                manager
)
{
    return (DatatypeValidator*) new (manager) ENTITYDatatypeValidator(this, facets, enums, finalSet, manager);
}

// -----------------------------------------------------------------------
// Compare methods
// -----------------------------------------------------------------------
int ENTITYDatatypeValidator::compare(const XMLCh* const lValue
                                   , const XMLCh* const rValue)
{
    return ( XMLString::equals(lValue, rValue)? 0 : -1);
}

void ENTITYDatatypeValidator::validate(const XMLCh* const content)
{
    // use StringDatatypeValidator (which in turn, invoke
    // the baseValidator) to validate content against
    // facets if any.
    //
    StringDatatypeValidator::validate(content);

    //
    // parse the entity iff an EntityDeclPool is provided
    //
    if (fEntityDeclPool)
    {
        DTDEntityDecl* decl = fEntityDeclPool->getByKey(content);

        if (!decl                ||
            (!decl->isUnparsed())  )
        {
            ThrowXML1(InvalidDatatypeValueException
                    , XMLExcepts::VALUE_ENTITY_Invalid
                    , content);
        }

    }
    else {
        ThrowXML1
        (
            InvalidDatatypeValueException
            , XMLExcepts::VALUE_ENTITY_Invalid
            , content
        );
    }

    return;
}

void ENTITYDatatypeValidator::checkValueSpace(const XMLCh* const content)
{
    //
    // 3.3.11 check must: "NCName"
    //
    if ( !XMLString::isValidNCName(content))
    {
        ThrowXML1(InvalidDatatypeValueException
                , XMLExcepts::VALUE_Invalid_NCName
                , content);
    }

}

XERCES_CPP_NAMESPACE_END

 /**
  * End of file ENTITYDatatypeValidator.cpp
  */
