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
 * $Log$
 * Revision 1.5  2001/05/11 13:24:57  tng
 * Copyright update.
 *
 * Revision 1.4  2001/05/03 16:00:03  tng
 * Schema: samples update with schema
 *
 * Revision 1.3  2000/10/10 23:55:53  andyh
 * XMLFormatter patch, contributed by Bill Schindler.  Fix problems with
 * output to multi-byte encodings.
 *
 * Revision 1.2  2000/08/09 22:20:38  jpolast
 * updates for changes to sax2 core functionality.
 *
 * Revision 1.1  2000/08/02 19:16:14  jpolast
 * initial checkin of SAX2Print
 *
 *
 */



// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <util/XMLUniDefs.hpp>
#include <sax2/Attributes.hpp>
#include "SAX2Print.hpp"


// ---------------------------------------------------------------------------
//  Local const data
//
//  Note: This is the 'safe' way to do these strings. If you compiler supports
//        L"" style strings, and portability is not a concern, you can use
//        those types constants directly.
// ---------------------------------------------------------------------------
static const XMLCh  gEndElement[] = { chOpenAngle, chForwardSlash, chNull };
static const XMLCh  gEndPI[] = { chQuestion, chCloseAngle, chNull };
static const XMLCh  gStartPI[] = { chOpenAngle, chQuestion, chNull };
static const XMLCh  gXMLDecl1[] =
{
        chOpenAngle, chQuestion, chLatin_x, chLatin_m, chLatin_l
    ,   chSpace, chLatin_v, chLatin_e, chLatin_r, chLatin_s, chLatin_i
    ,   chLatin_o, chLatin_n, chEqual, chDoubleQuote, chDigit_1, chPeriod
    ,   chDigit_0, chDoubleQuote, chSpace, chLatin_e, chLatin_n, chLatin_c
    ,   chLatin_o, chLatin_d, chLatin_i, chLatin_n, chLatin_g, chEqual
    ,   chDoubleQuote, chNull
};

static const XMLCh  gXMLDecl2[] =
{
        chDoubleQuote, chQuestion, chCloseAngle
    ,   chLF, chNull
};




// ---------------------------------------------------------------------------
//  SAX2PrintHandlers: Constructors and Destructor
// ---------------------------------------------------------------------------
SAX2PrintHandlers::SAX2PrintHandlers( const   char* const              encodingName
                                    , const XMLFormatter::UnRepFlags unRepFlags
									, const bool					 expandNamespaces) :

    fFormatter
    (
        encodingName
        , this
        , XMLFormatter::NoEscapes
        , unRepFlags
    ),
	fExpandNS ( expandNamespaces )
{
    //
    //  Go ahead and output an XML Decl with our known encoding. This
    //  is not the best answer, but its the best we can do until we
    //  have SAX2 support.
    //
    fFormatter << gXMLDecl1 << fFormatter.getEncodingName() << gXMLDecl2;
}

SAX2PrintHandlers::~SAX2PrintHandlers()
{
}


// ---------------------------------------------------------------------------
//  SAX2PrintHandlers: Overrides of the output formatter target interface
// ---------------------------------------------------------------------------
void SAX2PrintHandlers::writeChars(const XMLByte* const toWrite)
{
}

void SAX2PrintHandlers::writeChars(const XMLByte* const toWrite,
                                   const unsigned int count,
                                   XMLFormatter* const formatter)
{
    // For this one, just dump them to the standard output
    // Surprisingly, Solaris was the only platform on which
    // required the char* cast to print out the string correctly.
    // Without the cast, it was printing the pointer value in hex.
    // Quite annoying, considering every other platform printed
    // the string with the explicit cast to char* below.
  cout.write((char *) toWrite, (int) count);
	cout.flush();
}


// ---------------------------------------------------------------------------
//  SAX2PrintHandlers: Overrides of the SAX ErrorHandler interface
// ---------------------------------------------------------------------------
void SAX2PrintHandlers::error(const SAXParseException& e)
{
    cerr << "\nError at file " << StrX(e.getSystemId())
		 << ", line " << e.getLineNumber()
		 << ", char " << e.getColumnNumber()
         << "\n  Message: " << StrX(e.getMessage()) << endl;
}

void SAX2PrintHandlers::fatalError(const SAXParseException& e)
{
    cerr << "\nFatal Error at file " << StrX(e.getSystemId())
		 << ", line " << e.getLineNumber()
		 << ", char " << e.getColumnNumber()
         << "\n  Message: " << StrX(e.getMessage()) << endl;
}

void SAX2PrintHandlers::warning(const SAXParseException& e)
{
    cerr << "\nWarning at file " << StrX(e.getSystemId())
		 << ", line " << e.getLineNumber()
		 << ", char " << e.getColumnNumber()
         << "\n  Message: " << StrX(e.getMessage()) << endl;
}


// ---------------------------------------------------------------------------
//  SAX2PrintHandlers: Overrides of the SAX DTDHandler interface
// ---------------------------------------------------------------------------
void SAX2PrintHandlers::unparsedEntityDecl(const     XMLCh* const name
                                          , const   XMLCh* const publicId
                                          , const   XMLCh* const systemId
                                          , const   XMLCh* const notationName)
{
    // Not used at this time
}


void SAX2PrintHandlers::notationDecl(const   XMLCh* const name
                                    , const XMLCh* const publicId
                                    , const XMLCh* const systemId)
{
    // Not used at this time
}


// ---------------------------------------------------------------------------
//  SAX2PrintHandlers: Overrides of the SAX DocumentHandler interface
// ---------------------------------------------------------------------------
void SAX2PrintHandlers::characters(const     XMLCh* const    chars
                                  , const   unsigned int    length)
{
    fFormatter.formatBuf(chars, length, XMLFormatter::CharEscapes);
}


void SAX2PrintHandlers::endDocument()
{
}


void SAX2PrintHandlers::endElement(const XMLCh* const uri,
								 const XMLCh* const localname,
								 const XMLCh* const qname)
{
    // No escapes are legal here
    fFormatter << XMLFormatter::NoEscapes << gEndElement ;
	if ( fExpandNS )
	{
		if (XMLString::compareIString(uri,XMLUni::fgEmptyString) != 0)
				fFormatter  << uri << chColon;
		fFormatter << localname << chCloseAngle;
	}
	else
		fFormatter << qname << chCloseAngle;
}


void SAX2PrintHandlers::ignorableWhitespace( const   XMLCh* const chars
                                            ,const  unsigned int length)
{
    fFormatter.formatBuf(chars, length, XMLFormatter::NoEscapes);
}


void SAX2PrintHandlers::processingInstruction(const  XMLCh* const target
                                            , const XMLCh* const data)
{
    fFormatter << XMLFormatter::NoEscapes << gStartPI  << target;
    if (data)
        fFormatter << chSpace << data;
    fFormatter << XMLFormatter::NoEscapes << gEndPI;
}


void SAX2PrintHandlers::startDocument()
{
}


void SAX2PrintHandlers::startElement(const   XMLCh* const    uri,
									const   XMLCh* const    localname,
									const   XMLCh* const    qname,
                                    const   Attributes&		attributes)
{
    // The name has to be representable without any escapes
    fFormatter  << XMLFormatter::NoEscapes << chOpenAngle ;
	if ( fExpandNS )
	{
		if (XMLString::compareIString(uri,XMLUni::fgEmptyString) != 0)
				fFormatter  << uri << chColon;
		fFormatter << localname ;
	}
	else
		fFormatter << qname ;

    unsigned int len = attributes.getLength();
    for (unsigned int index = 0; index < len; index++)
    {
        //
        //  Again the name has to be completely representable. But the
        //  attribute can have refs and requires the attribute style
        //  escaping.
        //
        fFormatter  << XMLFormatter::NoEscapes << chSpace ;
		if ( fExpandNS )
		{
			if (XMLString::compareIString(attributes.getURI(index),XMLUni::fgEmptyString) != 0)
				fFormatter  << attributes.getURI(index) << chColon;
			fFormatter  << attributes.getLocalName(index) ;
		}
		else
			fFormatter  << attributes.getQName(index) ;

		fFormatter  << chEqual << chDoubleQuote
                    << XMLFormatter::AttrEscapes
		            << attributes.getValue(index)
                    << XMLFormatter::NoEscapes
                    << chDoubleQuote;
    }
    fFormatter << chCloseAngle;
}
