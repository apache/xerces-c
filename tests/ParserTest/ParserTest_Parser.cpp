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
 * Revision 1.1  1999/11/09 01:02:22  twl
 * Initial revision
 *
 * Revision 1.3  1999/11/08 20:42:25  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <stdlib.h>
#include <util/RefVectorOf.hpp>
#include <util/XMLString.hpp>
#include <util/XMLUni.hpp>
#include <internal/XMLReader.hpp>
#include <internal/XMLScanner.hpp>
#include <framework/XMLAttr.hpp>
#include <framework/XMLNotationDecl.hpp>
#include <framework/XMLValidator.hpp>
#include <validators/DTD/DTDAttDef.hpp>
#include <validators/DTD/DTDElementDecl.hpp>
#include <validators/DTD/DTDEntityDecl.hpp>
#include "ParserTest.hpp"

#include <util/BitOps.hpp>

// ---------------------------------------------------------------------------
//  Local functions
// ---------------------------------------------------------------------------
static int attrComp(const void* elemOne, const void* elemTwo)
{
    return XMLString::compareString
    (
        (*(XMLAttr**)elemOne)->getName()
        , (*(XMLAttr**)elemTwo)->getName()
    );
}


// ---------------------------------------------------------------------------
//  TestParser: Constructors and Destructor
// ---------------------------------------------------------------------------
TestParser::TestParser() :

    fDoNamespaces(false)
    , fInExtSubset(false)
    , fInsideRoot(false)
    , fIntDTDOutput(false)
    , fNestingLevel(0)
    , fOutputType(OutputType_None)
    , fShowErrLoc(false)
    , fShowWarnings(false)
    , fSurrogate(0)
{
}

TestParser::~TestParser()
{
}


// ---------------------------------------------------------------------------
//  TestParser: The document handler interfaces
// ---------------------------------------------------------------------------
void TestParser::docCharacters( const   XMLCh* const    chars
                                , const unsigned int    length
                                , const bool            cdataSection)
{
    if (fOutputType == OutputType_Debug)
    {
        outStrm << "Got CHARS:\n    Bytes: "
                << length << ", CDATA?: "
                << (cdataSection ? "Yes" : "No")
                << "\n" << EndLn;
    }
     else if ((fOutputType == OutputType_JCCanon)
          ||  (fOutputType == OutputType_SunCanon))
    {
        showChars(chars, length);
    }
     else if (fOutputType == OutputType_XML)
    {
        if (cdataSection)
            outStrm << "<![CDATA[";
        showChars(chars, length);
        if (cdataSection)
            outStrm << "]]>";
    }
}

void TestParser::docComment(const XMLCh* const comment)
{
    if (fOutputType == OutputType_Debug)
    {
        outStrm << "Got document COMMENT:\n    "
                << "Text: \"" << comment << "\"\n"
                << EndLn;
    }
     else if (fOutputType == OutputType_XML)
    {
        outStrm << "<!--";
        showString(comment);
        outStrm << "-->";
    }
}

void TestParser::docPI( const   XMLCh* const    target
                        , const XMLCh* const    data)
{
    if (fOutputType == OutputType_Debug)
    {
        outStrm << "Got document PI:\n     "
                << "Target: \"" << target << '"';

        if (XMLString::stringLen(data))
            outStrm << ", Data: \"" << data << '"';

        outStrm << "\n" << EndLn;
    }
     else if ((fOutputType == OutputType_XML)
          ||  (fOutputType == OutputType_JCCanon)
          ||  (fOutputType == OutputType_SunCanon))
    {
        outStrm << "<?";
        showString(target);
        outStrm << " ";
        if (XMLString::stringLen(data))
            outStrm << data;
        outStrm << "?>";
    }
}


void TestParser::endDocument()
{
    if (fOutputType == OutputType_Debug)
    {
        outStrm << "Got ENDDOCUMENT:\n" << EndLn;
    }
     else if (fOutputType == OutputType_SunCanon)
    {
        outStrm << "\r\n";
    }
}

void TestParser::endElement(const   XMLElementDecl& elemDecl
                            , const unsigned int    uriId
                            , const bool            isRoot)
{
    if (fOutputType == OutputType_Debug)
    {
        if (fDoNamespaces)
        {
            XMLBuffer bufURI;
            fScanner->getValidator()->getURIText(uriId, bufURI);

            outStrm << "Got ENDELEMENT:\n    Name: "
                    << "Name: {" << bufURI.getRawBuffer() << "}"
                    << elemDecl.getBaseName()
                    << EndLn;
        }
         else
        {
            outStrm << "Got ENDELEMENT:\n    Name: "
                    << elemDecl.getFullName() << EndLn;
        }

        outStrm << EndLn;
    }
     else if ((fOutputType == OutputType_XML)
          ||  (fOutputType == OutputType_JCCanon)
          ||  (fOutputType == OutputType_SunCanon))
    {
        outStrm << "</";
        showString(elemDecl.getFullName());
        outStrm << ">";
    }

    // Clear the flag that says we're now inside the root
    if (isRoot)
        fInsideRoot = false;
}

void TestParser::endEntityReference(const XMLEntityDecl& entDecl)
{
    if (fOutputType == OutputType_Debug)
    {
        outStrm << "Got ENDENTITYREF:\n    "
                << "Name: " << entDecl.getName() << "\n" << EndLn;
    }
}

void TestParser::ignorableWhitespace(const  XMLCh* const    chars
                                    , const unsigned int    length
                                    , const bool            cdataSection)
{
    if (fOutputType == OutputType_Debug)
    {
        outStrm << "Got WHITESPACE:\n    Bytes: "
                << length << ", CDATA?: "
                << (cdataSection ? "Yes" : "No")
                << "\n" << EndLn;
    }
     else if (fOutputType == OutputType_XML)
    {
        if (cdataSection)
            outStrm << "<![CDATA[";
        showChars(chars, length);
        if (cdataSection)
            outStrm << "]]>";
    }
     else if ((fOutputType == OutputType_JCCanon)
          ||  (fOutputType == OutputType_SunCanon))
    {
        if (!fInsideRoot)
            return;
        showChars(chars, length);
    }
}

void TestParser::resetDocument()
{
    if (fOutputType == OutputType_Debug)
        outStrm << "Got RESETDOCUMENT:\n" << EndLn;
}

void TestParser::startDocument()
{
    if (fOutputType == OutputType_Debug)
        outStrm << "Got STARTDOCUMENT:\n" << EndLn;
}

void
TestParser::startElement(const  XMLElementDecl&         elemDecl
                        , const unsigned int            uriId
                        , const XMLCh* const            prefixName
                        , const RefVectorOf<XMLAttr>&   attrList
                        , const unsigned int            attCount
                        , const bool                    isEmpty
                        , const bool                    isRoot)
{
    // Set the flag that says we're now inside the root, if its not empty
    if (isRoot && !isEmpty)
        fInsideRoot = true;

    if (fOutputType == OutputType_Debug)
    {
        XMLBuffer bufURI;
        if (fDoNamespaces)
        {
            fScanner->getValidator()->getURIText(uriId, bufURI);
            outStrm << "Got STARTELEMENT:\n    "
                    << " Name: {" << bufURI.getRawBuffer() << "}" << elemDecl.getBaseName()
                    << ", AttCount: " << attCount
                    << ", Empty?: "
                    << (isEmpty ? "yes" : "no")
                    << "\n";
        }
         else
        {
            outStrm << "Got STARTELEMENT:\n    "
                    << " Name: " << elemDecl.getFullName()
                    << ", AttCount: " << attCount
                    << ", Empty?: "
                    << (isEmpty ? "yes" : "no")
                    << "\n";
        }

        // If any attributes, then show them
        if (attCount)
        {
            outStrm << "    Attrs: ";
            for (unsigned int attInd = 0; attInd < attCount; attInd++)
            {
                const XMLAttr* curAttr = attrList.elementAt(attInd);

                if (fDoNamespaces)
                {
                    fScanner->getValidator()->getURIText(curAttr->getURIId(), bufURI);
                    outStrm << "{" << bufURI.getRawBuffer()
                            << "}" << curAttr->getName() << "="
                            << curAttr->getValue() << "\n";
                }
                 else
                {
                    outStrm << curAttr->getQName() << "="
                            << curAttr->getValue() << "\n";
                }

                outStrm << "           ";
            }
        }

        outStrm << EndLn;
    }
     else if (fOutputType == OutputType_XML)
    {
        outStrm << "<";
        showString(elemDecl.getFullName());

        if (attCount)
        {
            outStrm << " ";

            for (unsigned int index = 0; index < attCount; index++)
            {
                const XMLAttr* curAttr = attrList.elementAt(index);
                showString(curAttr->getQName());
                outStrm << "=\"";
                showString(curAttr->getValue());
                outStrm << "\"";

                if (index < attCount-1)
                    outStrm << " ";
            }
        }

        if (isEmpty)
            outStrm << "/>";
        else
            outStrm << ">";
    }
     else if ((fOutputType == OutputType_JCCanon)
          ||  (fOutputType == OutputType_SunCanon))
    {
        outStrm << "<";
        showString(elemDecl.getFullName());

        if (attCount)
        {
            outStrm << " ";

            //
            //  Get a list of attribute pointers. The canonical output
            //  format requires sorted attributes. If we aren't doing
            //  canonical output, then we don't sort it, but we still use
            //  the array.
            //
            const XMLAttr** attrTmp = new const XMLAttr*[attCount];
            unsigned int index;
            for (index = 0; index < attCount; index++)
                attrTmp[index] = attrList.elementAt(index);

            if (attCount > 1)
                qsort(attrTmp, attCount, sizeof(XMLAttr*), attrComp);

            for (index = 0; index < attCount; index++)
            {
                const XMLAttr* curAttr = attrTmp[index];
                showString(curAttr->getQName());
                outStrm << "=\"";
                showString(curAttr->getValue());
                outStrm << "\"";

                if (index < attCount-1)
                    outStrm << " ";
            }
            delete [] attrTmp;
        }

        if (isEmpty)
        {
            outStrm << "></";
            showString(elemDecl.getFullName());
            outStrm << ">";
        }
         else
        {
            outStrm << ">";
        }
    }
}

void TestParser::startEntityReference(const XMLEntityDecl& entDecl)
{
    if (fOutputType == OutputType_Debug)
    {
        outStrm << "Got STARTENTITY:\n    "
                << "Name: " << entDecl.getName() << "\n" << EndLn;
    }
}

void TestParser::XMLDecl(const  XMLCh* const    versionStr
                        , const XMLCh* const    encodingStr
                        , const XMLCh* const    standaloneStr
                        , const XMLCh* const    autoEncStr)
{
    if (fOutputType == OutputType_Debug)
    {
        outStrm << "Got XMLDECL:\n    "
                << "Version:\"" << versionStr << "\""
                << " Encoding:\"" << encodingStr << "\""
                << " Standalone:\"" << standaloneStr << "\""
                << " Auto Encoding:\"" << autoEncStr << "\""
                << "\n"
                << EndLn;
    }
     else if (fOutputType == OutputType_XML)
    {
        outStrm << "<?xml";

        if (XMLString::stringLen(versionStr))
            outStrm << " version=\"" << versionStr << '"';

        if (XMLString::stringLen(encodingStr))
            outStrm << " encoding=\"" << encodingStr << '"';

        if (XMLString::stringLen(standaloneStr))
            outStrm  << " standlone=\"" << standaloneStr << '"';

        outStrm << " ?>";
    }
}


// -----------------------------------------------------------------------
//  TestParser: The DocTypeHandler interface
// -----------------------------------------------------------------------
void TestParser::attDef(const   DTDElementDecl& elemDecl
                        , const DTDAttDef&      attDef
                        , const bool            ignoring)
{
    if (fOutputType == OutputType_Debug)
    {
        outStrm << "Got ATTDEF:\n    "
                << "Name: " << attDef.getFullName()
                << ", Type: "
                << XMLAttDef::getAttTypeString(attDef.getType())
                << ", DefType: "
                << XMLAttDef::getDefAttTypeString(attDef.getDefaultType());

        if (XMLString::stringLen(attDef.getValue()))
            outStrm << ", Value: \"" << attDef.getValue() << '"';

        outStrm << "\n" << EndLn;
    }
     else if (fOutputType != OutputType_None)
    {
        if (fInExtSubset)
            return;

        if (fIntDTDOutput)
        {
            outStrm << attDef.getFullName() << " ";
            if (attDef.getType() == XMLAttDef::Enumeration)
            {
                outStrm << chOpenParen;
                const XMLCh* curCh = attDef.getEnumeration();
                while (*curCh)
                {
                    while (!XMLReader::isWhitespace(*curCh))
                        outStrm << *curCh++;
                    curCh++;
                    if (*curCh)
                        outStrm << chPipe;
                }
                outStrm << chCloseParen;
            }
             else
            {
                outStrm << XMLAttDef::getAttTypeString(attDef.getType());
            }

            if (XMLString::stringLen(attDef.getValue()))
                outStrm << " \"" << attDef.getValue() << '"';

            if (attDef.getDefaultType() != XMLAttDef::Default)
            {
                outStrm << " "
                        << XMLAttDef::getDefAttTypeString(attDef.getDefaultType());
            }
        }
    }
}

void TestParser::doctypeComment(const XMLCh* const comment)
{
    if (fOutputType == OutputType_Debug)
    {
        outStrm << "Got DTD COMMENT:\n    "
                << "Text: \"" << comment << "\"\n"
                << EndLn;
    }
     else if (fOutputType != OutputType_None)
    {
        if (fInExtSubset)
            return;

        if (fIntDTDOutput)
            outStrm << "<!--" << comment << "-->";
    }
}

void TestParser::doctypeDecl(const  DTDElementDecl& elemDecl
                            , const XMLCh* const    publicId
                            , const XMLCh* const    systemId
                            , const bool            hasIntSubset)
{
    if (fOutputType == OutputType_Debug)
    {
        outStrm << "Got DOCTYPE:\n    "
                << "Root: " << elemDecl.getFullName();

        if (XMLString::stringLen(publicId))
            outStrm << ", PUBLIC: " << publicId;

        if (XMLString::stringLen(systemId))
            outStrm << ", SYSTEM: " << systemId;

        outStrm << "\n" << EndLn;
    }
     else if (fOutputType != OutputType_None)
    {
        if (fIntDTDOutput)
        {
            outStrm << "<!DOCTYPE " << elemDecl.getFullName();
            showIds(publicId, systemId);

            if (!hasIntSubset)
                outStrm << ">";
        }
    }
}

void TestParser::doctypePI( const   XMLCh* const    target
                            , const XMLCh* const    data)
{
    if (fOutputType == OutputType_Debug)
    {
        outStrm << "Got DTD PI:\n     "
                << "Target: \"" << target << '"';

        if (XMLString::stringLen(data))
            outStrm << ", Data: \"" << data << '"';
        outStrm << "\n" << EndLn;
    }
     else if (fOutputType != OutputType_None)
    {
        if (fInExtSubset)
            return;

        if (fIntDTDOutput)
        {
            outStrm << "<?" << target;
            if (XMLString::stringLen(data))
                outStrm << " " << data;
            outStrm << "?>";
        }
    }
}

void TestParser::doctypeWhitespace( const   XMLCh* const    chars
                                    , const unsigned int    length)
{
    if (fOutputType == OutputType_Debug)
    {
        outStrm << "Got DTD Spaces:\n    Bytes: "
                << length << "\n" << EndLn;
    }
     else if (fOutputType != OutputType_None)
    {
        if (fInExtSubset)
            return;

        if (fIntDTDOutput)
            showChars(chars, length);
    }
}

void TestParser::elementDecl(const  DTDElementDecl&     decl
                            , const bool                isIgnored)
{
    if (fOutputType == OutputType_Debug)
    {
        outStrm << "Got ELEMENT DECL:\n    "
                << "Name: " << decl.getFullName();

        if (isIgnored)
            outStrm << " (Ignored)";

        outStrm << ", Content: ";
        outStrm << decl.getFormattedContentModel(*fScanner->getValidator());
        outStrm << "\n" << EndLn;
    }
     else if (fOutputType != OutputType_None)
    {
        if (fInExtSubset)
            return;

        if (fIntDTDOutput)
        {
            outStrm << "<!ELEMENT " << decl.getFullName() << " ";
            outStrm << decl.getFormattedContentModel(*fScanner->getValidator());
            outStrm << ">";
        }
    }
}

void TestParser::endAttList(const DTDElementDecl& elemDecl)
{
    if (fOutputType == OutputType_Debug)
    {
        outStrm << "Got ENDATTLIST:\n    "
                << "Name: " << elemDecl.getFullName() << "\n"
                << EndLn;
    }
     else if (fOutputType != OutputType_None)
    {
        if (fInExtSubset)
            return;

        if (fIntDTDOutput)
            outStrm << ">";
    }
}

void TestParser::endIntSubset()
{
    if (fOutputType == OutputType_Debug)
    {
        outStrm << "Got ENDINTSUBSET\n" << EndLn;
    }
     else if (fOutputType != OutputType_None)
    {
        if (fIntDTDOutput)
            outStrm << "]>";
    }
}

void TestParser::endExtSubset()
{
    fInExtSubset = false;
    if (fOutputType == OutputType_Debug)
        outStrm << "Got ENDEXTSUBSET\n" << EndLn;
}

void TestParser::entityDecl(const   DTDEntityDecl&  entityDecl
                            , const bool            isPEDecl
                            , const bool            isIgnored)
{
    if (fOutputType == OutputType_Debug)
    {
        outStrm << "Got ENTITYDECL:\n    "
                << "Name: " << entityDecl.getName()
                << (isPEDecl ? " [Parameter Entity]" : " [General Entity]")
                << "\n" << EndLn;
    }
     else if (fOutputType != OutputType_None)
    {
        if (fInExtSubset)
            return;

        if (fIntDTDOutput)
        {
            outStrm << "<!ENTITY ";
            if (isPEDecl)
                outStrm << "% ";
            outStrm << entityDecl.getName();
        
            if (entityDecl.isExternal())
                showIds(entityDecl.getPublicId(), entityDecl.getSystemId());
             else
                outStrm << " \"" << entityDecl.getValue() << "\"";

            outStrm << ">";
        }
    }
}

void TestParser::resetDocType()
{
    if (fOutputType == OutputType_Debug)
        outStrm << "Got RESETDOCTYPE:\n" << EndLn;
}

void TestParser::notationDecl(  const   XMLNotationDecl&    notDecl
                                , const bool                isIgnored)
{
    if (fOutputType == OutputType_Debug)
    {
        outStrm << "Got NOTATIONDECL:\n    "
                << "Name: " << notDecl.getName()
                << EndLn;
    }
     else if (fOutputType != OutputType_None)
    {
        if (fInExtSubset)
            return;

        if (fIntDTDOutput)
        {
            outStrm << "<!NOTATION " << notDecl.getName() << " ";

            if (!XMLString::stringLen(notDecl.getSystemId()))
                outStrm << "PUBLIC ";
            else
                outStrm << "SYSTEM ";

            if (XMLString::stringLen(notDecl.getPublicId()))
                outStrm << "\"" << notDecl.getPublicId() << '"';

            if (XMLString::stringLen(notDecl.getSystemId()))
                outStrm << " \"" << notDecl.getSystemId() << '"';

            outStrm << ">";
        }
    }
}

void TestParser::startAttList(const DTDElementDecl& elemDecl)
{
    if (fOutputType == OutputType_Debug)
    {
        outStrm << "Got STARTATTLIST:\n    "
                << "Name: " << elemDecl.getFullName() << "\n"
                << EndLn;
    }
     else if (fOutputType != OutputType_None)
    {
        if (fInExtSubset)
            return;

        if (fIntDTDOutput)
            outStrm << "<!ATTLIST " << elemDecl.getFullName();
    }
}

void TestParser::startIntSubset()
{
    if (fOutputType == OutputType_Debug)
    {
        outStrm << "Got STARTINTSUBSET\n" << EndLn;
    }
     else if (fOutputType != OutputType_None)
    {
        if (fIntDTDOutput)
            outStrm << " [";
    }
}

void TestParser::startExtSubset()
{
    fInExtSubset = true;
    if (fOutputType == OutputType_Debug)
        outStrm << "Got STARTEXTSUBSET\n" << EndLn;
}

void TestParser::TextDecl(  const   XMLCh* const    versionStr
                            , const XMLCh* const    encodingStr)
{
    if (fOutputType == OutputType_Debug)
    {
        outStrm << "Got TEXTDECL:\n    ";

        if (XMLString::stringLen(versionStr))
            outStrm << "Version: " << versionStr;

        if (XMLString::stringLen(encodingStr))
            outStrm << "Encoding: " << encodingStr;

        outStrm << "\n" << EndLn;
    }
}


// ---------------------------------------------------------------------------
//  TestParser: Implementation of the XMLErrorReporter interface
// ---------------------------------------------------------------------------
void TestParser::error( const   unsigned int                errCode
                        , const XMLCh* const                msgDomain
                        , const XMLErrorReporter::ErrTypes  type
                        , const XMLCh* const                text
                        , const XMLCh* const                systemId
                        , const XMLCh* const                publicId
                        , const unsigned int                lineNum
                        , const unsigned int                colNum)
{
    //
    //  If we are in 'show error loc' mode, then we do a special, condensed
    //  display of error location info. Else we fall through and do the
    //  normal one for human consumption.
    //
    if (fShowErrLoc)
    {
        // We only do fatal and validity errors in this case
        if (type == XMLErrorReporter::ErrType_Warning)
            return;

        //
        //  We want to display the entity name, but not the whole path, since
        //  this output is for regression testing and has to be compared
        //  against previous runs potentitally on other machines.
        //
        const XMLCh* entName = systemId;
        int ofs = XMLString::lastIndexOf(systemId, chForwardSlash);
        if (ofs == -1)
            ofs = XMLString::lastIndexOf(systemId, chBackSlash);
        if (ofs != -1)
            entName = &systemId[ofs + 1];

        outStrm << lineNum << "/" << colNum
                << L":" << entName
                << L" - " << text
                << EndLn;
        return;
    }

    // If its a warning and we are not showing warnings, then get out
    if ((type == XMLErrorReporter::ErrType_Warning) && !fShowWarnings)
        return;

    const char* typeStr = "?";
    if (type == XMLErrorReporter::ErrType_Fatal)
        typeStr = "ERROR";
    else if (type == XMLErrorReporter::ErrType_Warning)
        typeStr = "WARNING";
    else if (type == XMLErrorReporter::ErrType_Invalid)
        typeStr = "VALIDITY";

    // Save the stream flags so that we can restore them
    StreamJanitor strmJan(&errStrm);

    // Output the error heading and the error type string
    outStrm << "\nError: (" << typeStr;

    // If we have either id, display them
    if (XMLString::stringLen(systemId))
         outStrm << ", System Id: " << systemId;

    if (XMLString::stringLen(publicId))
        outStrm << ", Public Id: " << publicId;

    // Display the position information
    outStrm << DecimalRadix
            << ", Line/Col: " << lineNum << "/" << colNum
            << ")\n";

    // And finally the error text
    outStrm << text << EndLn;
}

void TestParser::resetErrors()
{
    if (fOutputType == OutputType_Debug)
        outStrm << "Got RESETERRORS:\n" << EndLn;
}


// ---------------------------------------------------------------------------
//  TestParser: Private helpers
// ---------------------------------------------------------------------------
void TestParser::showChars( const   XMLCh* const    chars
                            , const unsigned int    length)
{
    static const XMLByte FirstByteMark[7] =
    {
        0x00, 0x00, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC
    };

    const bool doCan = ((fOutputType == OutputType_SunCanon)
                        || (fOutputType == OutputType_JCCanon));

    unsigned int index = 0;
    while (index < length)
    {
        //
        //  Convert the char to UTF-8 format. This will generate multiple
        //  short chars to display. For each one, call the showChar()
        //  method to display it.
        //
        XMLUInt32 tmpVal = chars[index++];

        if ((tmpVal >= 0xD800) && (tmpVal <= 0xDBFF))
        {
            if (index == length)
            {
                errStrm << "Missing trailing surrogate\n" << EndLn;
                break;
            }

            tmpVal = ((tmpVal - 0xD800) << 10)
                     + ((chars[index] - 0xDC00) + 0x10000);
            index++;
        }

        // Figure out how many bytes we'll kick out
        unsigned int outBytes;
        if (tmpVal < 0x80)
            outBytes = 1;
        else if (tmpVal < 0x800)
            outBytes = 2;
        else if (tmpVal < 0x10000)
            outBytes = 3;
        else if (tmpVal < 0x200000)
            outBytes = 4;
        else if (tmpVal < 0x4000000)
            outBytes = 5;
        else
        {
            outBytes = 6;
            if (tmpVal & 0x80000000)
            {
                outBytes = 2;
                tmpVal = 0xFFFD;
            }
        }

        // Get the chars into a temp buffer in the right order
        char tmpOutChars[6];
        unsigned int outIndex = outBytes;
        switch(outBytes)
        {
            case 6 : tmpOutChars[--outIndex] = char((tmpVal | 0x80) & 0xBF);
                     tmpVal >>= 6;
            case 5 : tmpOutChars[--outIndex] = char((tmpVal | 0x80) & 0xBF);
                     tmpVal >>= 6;
            case 4 : tmpOutChars[--outIndex] = char((tmpVal | 0x80) & 0xBF);
                     tmpVal >>= 6;
            case 3 : tmpOutChars[--outIndex] = char((tmpVal | 0x80) & 0xBF);
                     tmpVal >>= 6;
            case 2 : tmpOutChars[--outIndex] = char((tmpVal | 0x80) & 0xBF);
                     tmpVal >>= 6;
            case 1 : tmpOutChars[--outIndex] = char(tmpVal | FirstByteMark[outBytes]);
        }

        // And spit them out
        for (outIndex = 0; outIndex < outBytes; outIndex++)
            showChar(tmpOutChars[outIndex], doCan);
    }
}


void TestParser::showChar(const char toShow, const bool doCan)
{
    if (doCan)
    {
        if (toShow == chLF)
            outStrm << "&#10;";
        else if (toShow == chHTab)
            outStrm << "&#9;";
        else if (toShow == chCR)
            outStrm << "&#13;";
        else if (toShow == chSingleQuote)
            outStrm << "&apos;";
        else if (toShow == chAmpersand)
            outStrm << "&amp;";
        else if (toShow == chDoubleQuote)
            outStrm << "&quot;";
        else if (toShow == chOpenAngle)
            outStrm << "&lt;";
        else if (toShow == chCloseAngle)
            outStrm << "&gt;";
        else
            outStrm << toShow;
    }
     else
    {
        outStrm << toShow;
    }
}

void
TestParser::showIds(const XMLCh* const publicId, const XMLCh* const systemId)
{
    if (XMLString::stringLen(publicId) || XMLString::stringLen(systemId))
    {
        if (!XMLString::stringLen(publicId))
        {
            outStrm << " SYSTEM '" << systemId << "'";
        }
         else
        {
            outStrm << " PUBLIC '" << publicId << "'";
            if (systemId)
                outStrm << " '" << systemId << "'";
        }
    }
}
