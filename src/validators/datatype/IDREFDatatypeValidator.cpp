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

/*
 * $Id$
 * $Log$
 * Revision 1.2  2001/07/24 21:23:40  tng
 * Schema: Use DatatypeValidator for ID/IDREF/ENTITY/ENTITIES/NOTATION.
 *
 * Revision 1.1  2001/07/04 20:16:59  peiyongz
 * IDREFDatatypeValidator
 *
 */

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <validators/datatype/IDREFDatatypeValidator.hpp>
#include <validators/datatype/InvalidDatatypeFacetException.hpp>
#include <validators/datatype/InvalidDatatypeValueException.hpp>

IDREFDatatypeValidator::IDREFDatatypeValidator(
                                           DatatypeValidator*            const baseValidator
                                         , RefHashTableOf<KVStringPair>* const facets
                                         , RefVectorOf<XMLCh>*           const enums
                                         , const int                           finalSet)
:StringDatatypeValidator(baseValidator, facets, enums, finalSet)
,fIDRefList(0)
{

    //
    // the StringDatatypeValidator has the same set of
    // constrainning facets as the IDREFDatatypeValidator
    // and the StringDatatypeValidator(...) would do
    // all the jobs there.

    //
    // if enumeration is provided, make sure that they
    // are all valid Name(s).
    //
    if (enums)
    {
        int enumLength = enums->size();
        for ( int i = 0; i < enumLength; i++)
        {
            if ( XMLString::isValidName(enums->elementAt(i)) == false)
                ThrowXML(InvalidDatatypeFacetException, XMLExcepts::FACET_Len_minLen);
                //("Value '"+content+"' is not a valid NCName");
        }
    }

    DatatypeValidator::setType(DatatypeValidator::IDREF);
}

void IDREFDatatypeValidator::validate(const XMLCh* const content)
{
    // use StringDatatypeValidator (which in turn, invoke
    // the baseValidator) to validate content against
    // facets if any.
    //
    StringDatatypeValidator::validate(content);

    // check 3.3.1.constrain must: "Name"
    //
    if ( XMLString::isValidName(content) == false)
        ThrowXML(InvalidDatatypeValueException, XMLExcepts::FACET_Len_minLen);
        //("Value '"+content+"' is not a valid QName");

    // this is different from java, since we always add, while
    // in java, it is done as told. REVISIT.
    //
    if (fIDRefList)
        addIdRef(content);
}

//
// Add an IDREF to the fIDRefList
//
void IDREFDatatypeValidator::addIdRef(const XMLCh* const content)
{
    XMLRefInfo* find = fIDRefList->get(content);
    if (!find)
    {
        find = new XMLRefInfo(content);
        fIDRefList->put((void*)find->getRefName(), find);
    }

    //
    //  Mark it used
    //
    find->setUsed(true);
}

/**
  * End of file IDREFDatatypeValidator.cpp
  */
