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
 * Revision 1.4  2003/05/15 18:53:26  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.3  2002/12/18 14:17:55  gareth
 * Fix to bug #13438. When you eant a vector that calls delete[] on its members you should use RefArrayVectorOf.
 *
 * Revision 1.2  2002/11/04 14:53:28  tng
 * C++ Namespace Support.
 *
 * Revision 1.1.1.1  2002/02/01 22:22:42  peiyongz
 * sane_include
 *
 * Revision 1.10  2001/10/10 15:11:21  peiyongz
 * extract/validate URI iff present
 *
 * Revision 1.9  2001/10/10 14:17:50  peiyongz
 * <URI>:<LocalPart> where <URI> can be optional.
 *
 * Revision 1.8  2001/10/09 20:46:19  peiyongz
 * NOTATION: checkContent(): <URI>:<localPart>
 *
 * Revision 1.7  2001/10/02 18:59:29  peiyongz
 * Invalid_Facet_Tag to display the tag name
 *
 * Revision 1.6  2001/09/24 15:33:15  peiyongz
 * DTV Reorganization: virtual methods moved to *.cpp
 *
 * Revision 1.5  2001/09/20 15:14:47  peiyongz
 * DTV reorganization: inherit from AbstractStringVaildator
 *
 * Revision 1.3  2001/08/21 18:42:53  peiyongz
 * Bugzilla# 2816: cleanUp() declared with external linkage and called
 *                          before defined as inline
 *
 * Revision 1.2  2001/08/14 22:11:56  peiyongz
 * new exception message added
 *
 * Revision 1.1  2001/07/05 20:15:27  peiyongz
 * NOTATIONDatatypeValidator
 *
 */

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/validators/datatype/NOTATIONDatatypeValidator.hpp>
#include <xercesc/util/XMLUri.hpp>
#include <xercesc/validators/datatype/InvalidDatatypeFacetException.hpp>
#include <xercesc/validators/datatype/InvalidDatatypeValueException.hpp>

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  Constructors and Destructor
// ---------------------------------------------------------------------------
NOTATIONDatatypeValidator::NOTATIONDatatypeValidator(MemoryManager* const manager)
:AbstractStringValidator(0, 0, 0, DatatypeValidator::NOTATION, manager)
{}

NOTATIONDatatypeValidator::~NOTATIONDatatypeValidator()
{}

NOTATIONDatatypeValidator::NOTATIONDatatypeValidator(
                          DatatypeValidator*            const baseValidator
                        , RefHashTableOf<KVStringPair>* const facets
                        , RefArrayVectorOf<XMLCh>*           const enums
                        , const int                           finalSet
                        , MemoryManager* const manager)
:AbstractStringValidator(baseValidator, facets, finalSet, DatatypeValidator::NOTATION, manager)
{
    init(enums);
}

DatatypeValidator* NOTATIONDatatypeValidator::newInstance
(
      RefHashTableOf<KVStringPair>* const facets
    , RefArrayVectorOf<XMLCh>* const      enums
    , const int                           finalSet
    , MemoryManager* const                manager
)
{
    return (DatatypeValidator*) new (manager) NOTATIONDatatypeValidator(this, facets, enums, finalSet, manager);
}

// ---------------------------------------------------------------------------
//  Utilities
// ---------------------------------------------------------------------------
void NOTATIONDatatypeValidator::assignAdditionalFacet( const XMLCh* const key
                                                     , const XMLCh* const)
{
    ThrowXML1(InvalidDatatypeFacetException
            , XMLExcepts::FACET_Invalid_Tag
            , key);
}

void NOTATIONDatatypeValidator::inheritAdditionalFacet()
{}

void NOTATIONDatatypeValidator::checkAdditionalFacetConstraints() const
{}

void NOTATIONDatatypeValidator::checkAdditionalFacet(const XMLCh* const) const
{}

void NOTATIONDatatypeValidator::checkValueSpace(const XMLCh* const content)
{
    //
    //  NOTATATION: <URI>:<localPart>
    //  where URI is optional
    //        ':' and localPart must be present
    //
    int contentLength = XMLString::stringLen(content);
    int colonPosition = XMLString::lastIndexOf(content, chColon);

    if ((colonPosition == -1)                ||  // no ':'
        (colonPosition == contentLength - 1)  )  // <URI>':'
        ThrowXML1(InvalidDatatypeValueException
                , XMLExcepts::VALUE_NOTATION_Invalid
                , content);

    if (colonPosition > 0)
    {
        // Extract URI
        XMLCh* uriPart = new XMLCh[colonPosition + 1];
        ArrayJanitor<XMLCh> jan1(uriPart);
        XMLString::subString(uriPart, content, 0, colonPosition);

        try
        {
            // no relative uri support here
            XMLUri  newURI(uriPart);
        }
        catch (...)
        {
            ThrowXML1(InvalidDatatypeValueException
                    , XMLExcepts::VALUE_NOTATION_Invalid
                    , content);
        }
    }

    // Extract localpart
    XMLCh* localPart = new XMLCh[contentLength - colonPosition];
    ArrayJanitor<XMLCh> jan2(localPart);
    XMLString::subString(localPart, content, colonPosition + 1, contentLength);

    if ( !XMLString::isValidNCName(localPart))
    {
        ThrowXML1(InvalidDatatypeValueException
                , XMLExcepts::VALUE_NOTATION_Invalid
                , content);
    }

}

int NOTATIONDatatypeValidator::getLength(const XMLCh* const content) const
{
    return XMLString::stringLen(content);
}

XERCES_CPP_NAMESPACE_END

/**
  * End of file NOTATIONDatatypeValidator.cpp
  */
