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
 * Revision 1.8  2003/10/01 00:27:12  knoaman
 * Performance: call a static method to check the validity of URI instead of
 * creating/deleting local objects.
 *
 * Revision 1.7  2003/09/30 21:31:30  peiyongz
 * Implementation of Serialization/Deserialization
 *
 * Revision 1.6  2003/05/18 14:02:07  knoaman
 * Memory manager implementation: pass per instance manager.
 *
 * Revision 1.5  2003/05/16 06:01:57  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.4  2003/05/15 18:53:26  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.3  2002/12/18 14:17:55  gareth
 * Fix to bug #13438. When you eant a vector that calls delete[] on its members you should use RefArrayVectorOf.
 *
 * Revision 1.2  2002/11/04 14:53:27  tng
 * C++ Namespace Support.
 *
 * Revision 1.1.1.1  2002/02/01 22:22:40  peiyongz
 * sane_include
 *
 * Revision 1.10  2001/10/10 14:18:26  peiyongz
 * no message
 *
 * Revision 1.9  2001/10/09 20:53:58  peiyongz
 * init(): take 1 arg.
 *
 * Revision 1.8  2001/10/02 18:59:29  peiyongz
 * Invalid_Facet_Tag to display the tag name
 *
 * Revision 1.7  2001/09/24 15:33:15  peiyongz
 * DTV Reorganization: virtual methods moved to *.cpp
 *
 * Revision 1.6  2001/09/19 18:49:17  peiyongz
 * DTV reorganization: move inline to class declaration to avoid inline
 * function interdependency.
 *
 * Revision 1.5  2001/09/18 20:38:03  peiyongz
 * DTV reorganization: inherit from AbstractStringValidator.
 *
 * Revision 1.4  2001/08/21 18:42:53  peiyongz
 * Bugzilla# 2816: cleanUp() declared with external linkage and called
 *                          before defined as inline
 *
 * Revision 1.3  2001/08/14 22:11:56  peiyongz
 * new exception message added
 *
 * Revision 1.2  2001/08/10 16:21:19  peiyongz
 * use XMLUri instead of XMLURL
 *
 * Revision 1.1  2001/08/01 18:49:16  peiyongz
 * AnyRUIDatatypeValidator
 *
 *
 */

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/validators/datatype/AnyURIDatatypeValidator.hpp>
#include <xercesc/validators/datatype/InvalidDatatypeFacetException.hpp>
#include <xercesc/validators/datatype/InvalidDatatypeValueException.hpp>

XERCES_CPP_NAMESPACE_BEGIN

//
//http://www.template.com
//
static const XMLCh BASE_URI[] =
{
    chLatin_h, chLatin_t, chLatin_t, chLatin_p,
    chColon, chForwardSlash, chForwardSlash,
    chLatin_w, chLatin_w, chLatin_w, chPeriod,
    chLatin_t, chLatin_e, chLatin_m, chLatin_p, chLatin_l,
    chLatin_a, chLatin_t, chLatin_e, chPeriod,
    chLatin_c, chLatin_o, chLatin_m, chNull
};

// ---------------------------------------------------------------------------
//  Constructors and Destructor
// ---------------------------------------------------------------------------
AnyURIDatatypeValidator::AnyURIDatatypeValidator(MemoryManager* const manager)
:AbstractStringValidator(0, 0, 0, DatatypeValidator::AnyURI, manager)
,fTempURI(0)
{}

AnyURIDatatypeValidator::~AnyURIDatatypeValidator()
{
    cleanUp();
}

AnyURIDatatypeValidator::AnyURIDatatypeValidator(
                          DatatypeValidator*            const baseValidator
                        , RefHashTableOf<KVStringPair>* const facets
                        , RefArrayVectorOf<XMLCh>*           const enums
                        , const int                           finalSet
                        , MemoryManager* const manager)
:AbstractStringValidator(baseValidator, facets, finalSet, DatatypeValidator::AnyURI, manager)
,fTempURI(0)
{
    try
    {
        init(enums);
    }
    catch (...)
    {
        cleanUp();
        throw;
    }
}

DatatypeValidator* AnyURIDatatypeValidator::newInstance(
                                      RefHashTableOf<KVStringPair>* const facets
                                    , RefArrayVectorOf<XMLCh>*           const enums
                                    , const int                           finalSet
                                    , MemoryManager* const manager)
{
    return (DatatypeValidator*) new AnyURIDatatypeValidator(this, facets, enums, finalSet, manager);
}

// ---------------------------------------------------------------------------
//  Utilities
// ---------------------------------------------------------------------------

void AnyURIDatatypeValidator::assignAdditionalFacet( const XMLCh* const key
                                                   , const XMLCh* const)
{
    ThrowXML1(InvalidDatatypeFacetException
            , XMLExcepts::FACET_Invalid_Tag
            , key);
}

void AnyURIDatatypeValidator::inheritAdditionalFacet()
{}

void AnyURIDatatypeValidator::checkAdditionalFacetConstraints() const
{}

void AnyURIDatatypeValidator::checkAdditionalFacet(const XMLCh* const) const
{}

int AnyURIDatatypeValidator::getLength(const XMLCh* const content) const
{
    return XMLString::stringLen(content);
}

void AnyURIDatatypeValidator::checkValueSpace(const XMLCh* const content)
{

    // check 3.2.17.c0 must: URI (rfc 2396/2723)
    try
    {
        if (!fTempURI)
            fTempURI = new (fMemoryManager) XMLUri(BASE_URI, fMemoryManager);

        // Support for relative URLs
        // According to Java 1.1: URLs may also be specified with a
        // String and the URL object that it is related to.
        //
        if (XMLString::stringLen(content))
        {
            if (!XMLUri::isValidURI(fTempURI, content))
                ThrowXML1(InvalidDatatypeValueException
                    , XMLExcepts::VALUE_URI_Malformed
                    , content);
        }
    }
    catch (...)
    {
        ThrowXML1(InvalidDatatypeValueException
                , XMLExcepts::VALUE_URI_Malformed
                , content);
    }

}

/***
 * Support for Serialization/De-serialization
 ***/

IMPL_XSERIALIZABLE_TOCREATE(AnyURIDatatypeValidator)

void AnyURIDatatypeValidator::serialize(XSerializeEngine& serEng)
{
    AbstractStringValidator::serialize(serEng);

    //don't serialize fTempURI
}

XERCES_CPP_NAMESPACE_END

/**
  * End of file AnyURIDatatypeValidator.cpp
  */
