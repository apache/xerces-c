/*
 * The Apache Software License, Version 1.1
 * 
 * Copyright (c) 1999 The Apache Software Foundation.  All rights 
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
 * $Log$
 * Revision 1.1  1999/11/09 01:08:31  twl
 * Initial revision
 *
 * Revision 1.2  1999/11/08 20:44:37  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <util/XMLUni.hpp>
#include <framework/XMLElementDecl.hpp>


// ---------------------------------------------------------------------------
//  XMLElementDecl: Public, static data
// ---------------------------------------------------------------------------
const unsigned int  XMLElementDecl::fgInvalidElemId  = 0xFFFFFFFE;
const unsigned int  XMLElementDecl::fgPCDataElemId   = 0xFFFFFFFF;
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
    delete fContentModel;
    delete [] fFormattedModel;
}


// ---------------------------------------------------------------------------
//  ElementDecl: Miscellaneous
// ---------------------------------------------------------------------------
const XMLCh*
XMLElementDecl::getFormattedContentModel(const XMLValidator& validator) const
{
    //
    //  If its not already built, then call the protected virtual method
    //  to allow the derived class to build it (since only it knows.)
    //  Otherwise, just return the previously formatted methods.
    //
    //  Since we are faulting this in, within a const getter, we have to
    //  cast off the const-ness.
    //
    if (!fFormattedModel)
        ((XMLElementDecl*)this)->fFormattedModel = formatContentModel(validator);

    return fFormattedModel;
}


// ---------------------------------------------------------------------------
//  ElementDecl: Hidden constructors
// ---------------------------------------------------------------------------
XMLElementDecl::XMLElementDecl() :

    fContentModel(0)
    , fCreateReason(XMLElementDecl::NoReason)
    , fFormattedModel(0)
    , fId(XMLElementDecl::fgInvalidElemId)
{
}
