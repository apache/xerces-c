/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 2000 The Apache Software Foundation.  All rights
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
 *    permission, please contact apache@apache.org.
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
 * individuals on behalf of the Apache Software Foundation and was
 * originally based on software copyright (c) 1999, International
 * Business Machines, Inc., http://www.apache.org.  For more
 * information on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 */

/*
 * $Log$
 * Revision 1.1  2001/03/21 21:56:28  tng
 * Schema: Add Schema Grammar, Schema Validator, and split the DTDValidator into DTDValidator, DTDScanner, and DTDGrammar.
 *
 */

#include <validators/common/GrammarResolver.hpp>
#include <validators/datatype/DatatypeValidatorFactory.hpp>

// ---------------------------------------------------------------------------
//  GrammarResolver: Constructor and Destructor
// ---------------------------------------------------------------------------
GrammarResolver::GrammarResolver() :
    fGrammarRegistry(0)
    , fDataTypeReg(0)
{
   fGrammarRegistry = new RefHashTableOf<Grammar>(29, true);
}

GrammarResolver::~GrammarResolver()
{
   delete fGrammarRegistry;
   if (fDataTypeReg)
      delete fDataTypeReg;
}

// ---------------------------------------------------------------------------
//  GrammarResolver: Getter methods
// ---------------------------------------------------------------------------

DatatypeValidatorFactory* GrammarResolver::getDatatypeRegistry()
{
   if (!fDataTypeReg) {
      fDataTypeReg = new DatatypeValidatorFactory();
   }
   return fDataTypeReg;
}

Grammar* GrammarResolver::getGrammar( const XMLCh* const nameSpaceKey )
{
   return fGrammarRegistry->get( nameSpaceKey );
}

RefHashTableOfEnumerator<Grammar>
GrammarResolver::getGrammarEnumerator() const
{
    return RefHashTableOfEnumerator<Grammar>(fGrammarRegistry);
}

bool GrammarResolver::containsNameSpace( const XMLCh* const nameSpaceKey )
{
   return fGrammarRegistry->containsKey( nameSpaceKey );
}

void GrammarResolver::putGrammar( const XMLCh* const nameSpaceKey, Grammar* const grammarToAdopt ){
   fGrammarRegistry->put( (void*) nameSpaceKey, grammarToAdopt );
}

void GrammarResolver::removeGrammar( const XMLCh* const nameSpaceKey ) {
   if ( containsNameSpace( nameSpaceKey ) )
          fGrammarRegistry->removeKey( nameSpaceKey );
}

void GrammarResolver::reset() {
   fGrammarRegistry->removeAll();
   if (fDataTypeReg) {
      fDataTypeReg->resetRegistry();
   }
}