/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 1999-2000 The Apache Software Foundation.  All rights
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
 */



// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/util/XMLUniDefs.hpp>
#include <xercesc/util/XMLUni.hpp>
#include <xercesc/sax/AttributeList.hpp>
#include <xercesc/sax/SAXParseException.hpp>
#include <xercesc/sax/SAXException.hpp>
#include <xercesc/framework/LocalFileInputSource.hpp>
#include "Redirect.hpp"
#include <string.h>


// ---------------------------------------------------------------------------
//  Local constant data
//
//  gFileToTrap
//      This is the file that we are looking for in the entity handler, to
//      redirect to another file.
//
//  gRedirectToFile
//      This is the file that we are going to redirect the parser to.
// ---------------------------------------------------------------------------
static const XMLCh  gFileToTrap[] =
{
        chLatin_p, chLatin_e, chLatin_r, chLatin_s, chLatin_o, chLatin_n
    ,   chLatin_a, chLatin_l, chPeriod,  chLatin_d, chLatin_t, chLatin_d, chNull
};

static const XMLCh  gRedirectToFile[] =
{
        chLatin_r, chLatin_e, chLatin_d, chLatin_i, chLatin_r, chLatin_e
    ,   chLatin_c, chLatin_t, chPeriod,  chLatin_d, chLatin_t, chLatin_d, chNull
};


// ---------------------------------------------------------------------------
//  RedirectHandlers: Constructors and Destructor
// ---------------------------------------------------------------------------
RedirectHandlers::RedirectHandlers() :

    fElementCount(0)
    , fAttrCount(0)
    , fCharacterCount(0)
    , fSpaceCount(0)
{
}

RedirectHandlers::~RedirectHandlers()
{
}


// ---------------------------------------------------------------------------
//  RedirectHandlers: Implementation of the SAX DocumentHandler interface
// ---------------------------------------------------------------------------
void RedirectHandlers::startElement(const   XMLCh* const    name
                                    ,       AttributeList&  attributes)
{
    fElementCount++;
    fAttrCount += attributes.getLength();
}

void RedirectHandlers::characters(  const   XMLCh* const    chars
                                    , const unsigned int    length)
{
    fCharacterCount += length;
}

void RedirectHandlers::ignorableWhitespace( const   XMLCh* const chars
                                            , const unsigned int length)
{
    fSpaceCount += length;
}

void RedirectHandlers::resetDocument()
{
    fAttrCount = 0;
    fCharacterCount = 0;
    fElementCount = 0;
    fSpaceCount = 0;
}



// ---------------------------------------------------------------------------
//  RedirectHandlers: Overrides of the SAX ErrorHandler interface
// ---------------------------------------------------------------------------
void RedirectHandlers::error(const SAXParseException& e)
{
    XERCES_STD_QUALIFIER cerr << "\nError at (file " << StrX(e.getSystemId())
		 << ", line " << e.getLineNumber()
		 << ", char " << e.getColumnNumber()
         << "): " << StrX(e.getMessage()) << XERCES_STD_QUALIFIER endl;
}

void RedirectHandlers::fatalError(const SAXParseException& e)
{
    XERCES_STD_QUALIFIER cerr << "\nFatal Error at (file " << StrX(e.getSystemId())
		 << ", line " << e.getLineNumber()
		 << ", char " << e.getColumnNumber()
         << "): " << StrX(e.getMessage()) << XERCES_STD_QUALIFIER endl;
}

void RedirectHandlers::warning(const SAXParseException& e)
{
    XERCES_STD_QUALIFIER cerr << "\nWarning at (file " << StrX(e.getSystemId())
		 << ", line " << e.getLineNumber()
		 << ", char " << e.getColumnNumber()
         << "): " << StrX(e.getMessage()) << XERCES_STD_QUALIFIER endl;
}


// -----------------------------------------------------------------------
//  Handlers for the SAX EntityResolver interface
// -----------------------------------------------------------------------
InputSource* RedirectHandlers::resolveEntity(const   XMLCh* const    publicId
                                             , const XMLCh* const    systemId)
{
    //
    //  If its our file, then create a new URL input source for the file that
    //  we want to really be used. Otherwise, just return zero to let the
    //  default action occur.
    //
    //  We cannot assume that the XMLCh type is ok to pass to wcscmp(), so
    //  just do a comparison ourselves.
    //
    const XMLCh* s1 = gFileToTrap;
    const XMLCh* s2 = systemId;
    while (true)
    {
        // Break out on any difference
        if (*s1 != *s2)
            return 0;

        // If one is null, then both were null, so they are equal
        if (!*s1)
            break;

        // Else get the next char
        s1++;
        s2++;
    }

    // They were equal, so redirect to our other file
    return new LocalFileInputSource(gRedirectToFile);
}
