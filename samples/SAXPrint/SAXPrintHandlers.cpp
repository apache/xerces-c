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
 * $Log$
 * Revision 1.5  2000/03/28 19:43:11  roddey
 * Fixes for signed/unsigned warnings. New work for two way transcoding
 * stuff.
 *
 * Revision 1.4  2000/03/02 19:53:49  roddey
 * This checkin includes many changes done while waiting for the
 * 1.1.0 code to be finished. I can't list them all here, but a list is
 * available elsewhere.
 *
 * Revision 1.3  2000/02/11 03:05:35  abagchi
 * Removed second parameter from call to StrX constructor
 *
 * Revision 1.2  2000/02/06 07:47:24  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.1.1.1  1999/11/09 01:09:29  twl
 * Initial checkin
 *
 * Revision 1.11  1999/11/08 20:43:42  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */



// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <util/XMLUni.hpp>
#include <sax/AttributeList.hpp>
#include "SAXPrint.hpp"


// ---------------------------------------------------------------------------
//  SAXPrintHandlers: Constructors and Destructor
// ---------------------------------------------------------------------------
SAXPrintHandlers::SAXPrintHandlers( const   char* const encodingName
                                    , const bool        doEscapes) :
    fFormatter
    (
        encodingName
        , (doEscapes ? XMLFormatter::StdEscapes : XMLFormatter::NoEscapes)
        , this
    )
{
}

SAXPrintHandlers::~SAXPrintHandlers()
{
}


// ---------------------------------------------------------------------------
//  SAXPrintHandlers: Overrides of the output formatter target interface
// ---------------------------------------------------------------------------
void SAXPrintHandlers::writeChars(const XMLByte* const toWrite)
{
    // For this one, just dump them to the standard output
    cout << toWrite;
}


// ---------------------------------------------------------------------------
//  SAXPrintHandlers: Overrides of the SAX ErrorHandler interface
// ---------------------------------------------------------------------------
void SAXPrintHandlers::error(const SAXParseException& e)
{
    cerr << "\nError at file " << StrX(e.getSystemId())
		 << ", line " << e.getLineNumber()
		 << ", char " << e.getColumnNumber()
         << "\n  Message: " << StrX(e.getMessage()) << endl;
}

void SAXPrintHandlers::fatalError(const SAXParseException& e)
{
    cerr << "\nFatal Error at file " << StrX(e.getSystemId())
		 << ", line " << e.getLineNumber()
		 << ", char " << e.getColumnNumber()
         << "\n  Message: " << StrX(e.getMessage()) << endl;
}

void SAXPrintHandlers::warning(const SAXParseException& e)
{
    cerr << "\nWarning at file " << StrX(e.getSystemId())
		 << ", line " << e.getLineNumber()
		 << ", char " << e.getColumnNumber()
         << "\n  Message: " << StrX(e.getMessage()) << endl;
}


// ---------------------------------------------------------------------------
//  SAXPrintHandlers: Overrides of the SAX DTDHandler interface
// ---------------------------------------------------------------------------
void SAXPrintHandlers::unparsedEntityDecl(const     XMLCh* const name
                                          , const   XMLCh* const publicId
                                          , const   XMLCh* const systemId
                                          , const   XMLCh* const notationName)
{
    // Not used at this time
}


void SAXPrintHandlers::notationDecl(                                               const   XMLCh* const name
                                    , const XMLCh* const publicId
                                    , const XMLCh* const systemId)
{
    // Not used at this time
}


// ---------------------------------------------------------------------------
//  SAXPrintHandlers: Overrides of the SAX DocumentHandler interface
// ---------------------------------------------------------------------------
void SAXPrintHandlers::characters(const     XMLCh* const    chars
                                  , const   unsigned int    length)
{
    fFormatter.formatBuf(chars, length);
}


void SAXPrintHandlers::endDocument()
{
}

void SAXPrintHandlers::endElement(const XMLCh* const name)
{
    fFormatter << "</" << name << ">";
}

void SAXPrintHandlers::ignorableWhitespace( const   XMLCh* const chars
                                            ,const  unsigned int length)
{
    fFormatter.formatBuf(chars, length);
}

void SAXPrintHandlers::processingInstruction(const  XMLCh* const target
                                            , const XMLCh* const data)
{
    fFormatter << "<?" << target;
    if (data)
        fFormatter << " " << data;
    fFormatter << "?>\n";
}

void SAXPrintHandlers::startDocument()
{
}

void SAXPrintHandlers::startElement(const   XMLCh* const    name
                                    ,       AttributeList&  attributes)
{
    fFormatter << "<" << name;
    unsigned int len = attributes.getLength();

    for (unsigned int index = 0; index < len; index++)
    {
        fFormatter << " " << attributes.getName(index) << "=\""
		           << attributes.getValue(index) << "\"";
    }
    fFormatter << ">";
}
