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
 * Revision 1.7  2003/10/01 01:09:35  knoaman
 * Refactoring of some code to improve performance.
 *
 * Revision 1.6  2003/05/15 18:53:26  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.5  2003/01/13 20:16:51  knoaman
 * [Bug 16024] SchemaSymbols.hpp conflicts C++ Builder 6 dir.h
 *
 * Revision 1.4  2002/12/18 14:17:55  gareth
 * Fix to bug #13438. When you eant a vector that calls delete[] on its members you should use RefArrayVectorOf.
 *
 * Revision 1.3  2002/11/04 14:53:27  tng
 * C++ Namespace Support.
 *
 * Revision 1.2  2002/02/14 15:17:31  peiyongz
 * getEnumString()
 *
 * Revision 1.1.1.1  2002/02/01 22:22:40  peiyongz
 * sane_include
 *
 * Revision 1.1  2001/08/24 17:12:01  knoaman
 * Add support for anySimpleType.
 * Remove parameter 'baseValidator' from the virtual method 'newInstance'.
 *
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/validators/datatype/AnySimpleTypeDatatypeValidator.hpp>
#include <xercesc/util/RuntimeException.hpp>

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  AnySimpleTypeDatatypeValidator: Constructors and Destructor
// ---------------------------------------------------------------------------
AnySimpleTypeDatatypeValidator::AnySimpleTypeDatatypeValidator(MemoryManager* const manager)
    : DatatypeValidator(0, 0, SchemaSymbols::XSD_RESTRICTION, DatatypeValidator::AnySimpleType, manager)
{
    setWhiteSpace(DatatypeValidator::PRESERVE);
}

AnySimpleTypeDatatypeValidator::~AnySimpleTypeDatatypeValidator()
{

}

// ---------------------------------------------------------------------------
//  AnySimpleTypeDatatypeValidator: Instance methods
// ---------------------------------------------------------------------------
DatatypeValidator* AnySimpleTypeDatatypeValidator::newInstance
(
      RefHashTableOf<KVStringPair>* const facets
    , RefArrayVectorOf<XMLCh>* const      enums
    , const int                           finalSet
    , MemoryManager* const                manager
)
{
    // We own them, so we will delete them first
    delete facets;
    delete enums;

    ThrowXML(RuntimeException, XMLExcepts::DV_InvalidOperation);

    // to satisfy some compilers
    return 0;
}

const RefArrayVectorOf<XMLCh>* AnySimpleTypeDatatypeValidator::getEnumString() const
{
	return 0;
}

XERCES_CPP_NAMESPACE_END

/**
  * End of file AnySimpleTypeDatatypeValidator.cpp
  */

