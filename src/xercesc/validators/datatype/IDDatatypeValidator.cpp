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
 * $Id$
 * $Log$
 * Revision 1.1  2002/02/01 22:22:41  peiyongz
 * Initial revision
 *
 * Revision 1.6  2001/10/09 20:50:02  peiyongz
 * init(): take 1 arg
 *
 * Revision 1.5  2001/09/27 13:51:25  peiyongz
 * DTV Reorganization: ctor/init created to be used by derived class
 *
 * Revision 1.4  2001/09/25 14:23:42  peiyongz
 * DTV Reorganization: checkValueSpace()
 *
 * Revision 1.3  2001/08/14 22:11:56  peiyongz
 * new exception message added
 *
 * Revision 1.2  2001/07/24 21:23:40  tng
 * Schema: Use DatatypeValidator for ID/IDREF/ENTITY/ENTITIES/NOTATION.
 *
 * Revision 1.1  2001/07/04 14:38:25  peiyongz
 * IDDatatypeValidator: created
 * DatatypeValidatorFactory: IDDTV enabled
 * XMLString:isValidName(): to validate Name (XML [4][5])
 *
 */

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/validators/datatype/IDDatatypeValidator.hpp>
#include <xercesc/validators/datatype/InvalidDatatypeValueException.hpp>

// ---------------------------------------------------------------------------
//  Constructors and Destructor
// ---------------------------------------------------------------------------
IDDatatypeValidator::IDDatatypeValidator()
:StringDatatypeValidator(0, 0, 0, DatatypeValidator::ID)
,fIDRefList(0)
{}

IDDatatypeValidator::IDDatatypeValidator(
                          DatatypeValidator*            const baseValidator
                        , RefHashTableOf<KVStringPair>* const facets
                        , RefVectorOf<XMLCh>*           const enums
                        , const int                           finalSet)
:StringDatatypeValidator(baseValidator, facets, finalSet, DatatypeValidator::ID)
,fIDRefList(0)
{
    init(enums);
}

IDDatatypeValidator::~IDDatatypeValidator()
{}

DatatypeValidator* IDDatatypeValidator::newInstance(
                                      RefHashTableOf<KVStringPair>* const facets
                                    , RefVectorOf<XMLCh>*           const enums
                                    , const int                           finalSet)
{
    return (DatatypeValidator*) new IDDatatypeValidator(this, facets, enums, finalSet);
}

IDDatatypeValidator::IDDatatypeValidator(
                          DatatypeValidator*            const baseValidator
                        , RefHashTableOf<KVStringPair>* const facets
                        , const int                           finalSet
                        , const ValidatorType                 type)
:StringDatatypeValidator(baseValidator, facets, finalSet, type)
,fIDRefList(0)
{
    // do not invoke init() here!!!
}

void IDDatatypeValidator::validate(const XMLCh* const content)
{
    // use StringDatatypeValidator (which in turn, invoke
    // the baseValidator) to validate content against
    // facets if any.
    //
    StringDatatypeValidator::validate(content);

    // storing IDs to the global ID table
    if (fIDRefList)
        addId(content);
}

void IDDatatypeValidator::addId(const XMLCh * const content)
{
    XMLRefInfo* find = fIDRefList->get(content);
    if (find)
    {
        if (find->getDeclared())
        {
            ThrowXML1(InvalidDatatypeValueException
                    , XMLExcepts::VALUE_ID_Not_Unique
                    , content);
        }
    }
     else
    {
        find = new XMLRefInfo(content);
        fIDRefList->put((void*)find->getRefName(), find);
    }

    //
    //  Mark it declared
    //
    find->setDeclared(true);
}

void IDDatatypeValidator::checkValueSpace(const XMLCh* const content)
{
    //
    // 3.3.8 check must: "NCName"
    //
    if ( !XMLString::isValidNCName(content))
    {
        ThrowXML1(InvalidDatatypeValueException
                , XMLExcepts::VALUE_Invalid_NCName
                , content);
    }

}

/**
  * End of file IDDatatypeValidator.cpp
  */
