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
 * Revision 1.2  2000/01/19 00:59:21  roddey
 * Get rid of dependence on old utils output streams.
 *
 * Revision 1.1.1.1  1999/11/09 01:02:22  twl
 * Initial checkin
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
        std::wcout  << L"Got CHARS:\n    Bytes: "
                    << length << L", CDATA?: "
                    << (cdataSection ? L"Yes" : L"No")
                    << L"\n" << std::endl;
    }
     else if ((fOutputType == OutputType_JCCanon)
          ||  (fOutputType == OutputType_SunCanon))
    {
        showChars(chars, length);
    }
     else if (fOutputType == OutputType_XML)
    {
        if (cdataSection)
            std::wcout << L"<![CDATA[";
        showChars(chars, length);
        if (cdataSection)
            std::wcout << L"]]>";
    }
}

void TestParser::docComment(const XMLCh* const comment)
{
    if (fOutputType == OutputType_Debug)
    {
        std::wcout  << L"Got document COMMENT:\n    "
                    << L"Text: \"" << comment << L"\"\n"
                    << std::endl;
    }
     else if (fOutputType == OutputType_XML)
    {
        std::wcout << L"<!--";
        showString(comment);
        std::wcout << L"-->";
    }
}

void TestParser::docPI( const   XMLCh* const    target
                        , const XMLCh* const    data)
{
    if (fOutputType == OutputType_Debug)
    {
        std::wcout  << L"Got document PI:\n     "
                    << L"Target: \"" << target << '"';

        if (XMLString::stringLen(data))
            std::wcout << L", Data: \"" << data << '"';

        std::wcout << L"\n" << std::endl;
    }
     else if ((fOutputType == OutputType_XML)
          ||  (fOutputType == OutputType_JCCanon)
          ||  (fOutputType == OutputType_SunCanon))
    {
        std::wcout << L"<?";
        showString(target);
        std::wcout << L" ";
        if (XMLString::stringLen(data))
            std::wcout << data;
        std::wcout << L"?>";
    }
}


void TestParser::endDocument()
{
    if (fOutputType == OutputType_Debug)
    {
        std::wcout << L"Got ENDDOCUMENT:\n" << std::endl;
    }
     else if (fOutputType == OutputType_SunCanon)
    {
        std::wcout << L"\r\n";
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

            std::wcout  << L"Got ENDELEMENT:\n    Name: "
                        << L"Name: {" << bufURI.getRawBuffer() << L"}"
                        << elemDecl.getBaseName()
                        << std::endl;
        }
         else
        {
            std::wcout  << L"Got ENDELEMENT:\n    Name: "
                        << elemDecl.getFullName() << std::endl;
        }

        std::wcout << std::endl;
    }
     else if ((fOutputType == OutputType_XML)
          ||  (fOutputType == OutputType_JCCanon)
          ||  (fOutputType == OutputType_SunCanon))
    {
        std::wcout << L"</";
        showString(elemDecl.getFullName());
        std::wcout << L">";
    }

    // Clear the flag that says we're now inside the root
    if (isRoot)
        fInsideRoot = false;
}

void TestParser::endEntityReference(const XMLEntityDecl& entDecl)
{
    if (fOutputType == OutputType_Debug)
    {
        std::wcout  << L"Got ENDENTITYREF:\n    "
                    << L"Name: " << entDecl.getName() << L"\n"
                    << std::endl;
    }
}

void TestParser::ignorableWhitespace(const  XMLCh* const    chars
                                    , const unsigned int    length
                                    , const bool            cdataSection)
{
    if (fOutputType == OutputType_Debug)
    {
        std::wcout  << L"Got WHITESPACE:\n    Bytes: "
                    << length << L", CDATA?: "
                    << (cdataSection ? L"Yes" : L"No")
                    << L"\n" << std::endl;
    }
     else if (fOutputType == OutputType_XML)
    {
        if (cdataSection)
            std::wcout << L"<![CDATA[";
        showChars(chars, length);
        if (cdataSection)
            std::wcout << L"]]>";
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
        std::wcout << L"Got RESETDOCUMENT:\n" << std::endl;
}

void TestParser::startDocument()
{
    if (fOutputType == OutputType_Debug)
        std::wcout << L"Got STARTDOCUMENT:\n" << std::endl;
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
            std::wcout  << L"Got STARTELEMENT:\n    "
                        << L" Name: {" << bufURI.getRawBuffer()
                        << L"}" << elemDecl.getBaseName()
                        << L", AttCount: " << attCount
                        << L", Empty?: "
                        << (isEmpty ? L"yes" : L"no")
                        << L"\n";
        }
         else
        {
            std::wcout  << L"Got STARTELEMENT:\n    "
                        << L" Name: " << elemDecl.getFullName()
                        << L", AttCount: " << attCount
                        << L", Empty?: "
                        << (isEmpty ? L"yes" : L"no")
                        << L"\n";
        }

        // If any attributes, then show them
        if (attCount)
        {
            std::wcout << L"    Attrs: ";
            for (unsigned int attInd = 0; attInd < attCount; attInd++)
            {
                const XMLAttr* curAttr = attrList.elementAt(attInd);

                if (fDoNamespaces)
                {
                    fScanner->getValidator()->getURIText
                    (
                        curAttr->getURIId()
                        , bufURI
                    );
                    std::wcout  << L"{" << bufURI.getRawBuffer()
                                << L"}" << curAttr->getName() << L"="
                                << curAttr->getValue() << L"\n";
                }
                 else
                {
                    std::wcout  << curAttr->getQName() << L"="
                                << curAttr->getValue() << L"\n";
                }

                std::wcout << L"           ";
            }
        }

        std::wcout << std::endl;
    }
     else if (fOutputType == OutputType_XML)
    {
        std::wcout << L"<";
        showString(elemDecl.getFullName());

        if (attCount)
        {
            std::wcout << L" ";

            for (unsigned int index = 0; index < attCount; index++)
            {
                const XMLAttr* curAttr = attrList.elementAt(index);
                showString(curAttr->getQName());
                std::wcout << L"=\"";
                showString(curAttr->getValue());
                std::wcout << L"\"";

                if (index < attCount-1)
                    std::wcout << L" ";
            }
        }

        if (isEmpty)
            std::wcout << L"/>";
        else
            std::wcout << L">";
    }
     else if ((fOutputType == OutputType_JCCanon)
          ||  (fOutputType == OutputType_SunCanon))
    {
        std::wcout << L"<";
        showString(elemDecl.getFullName());

        if (attCount)
        {
            std::wcout << L" ";

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
                std::wcout << L"=\"";
                showString(curAttr->getValue());
                std::wcout << L"\"";

                if (index < attCount-1)
                    std::wcout << L" ";
            }
            delete [] attrTmp;
        }

        if (isEmpty)
        {
            std::wcout << L"></";
            showString(elemDecl.getFullName());
            std::wcout << L">";
        }
         else
        {
            std::wcout << L">";
        }
    }
}

void TestParser::startEntityReference(const XMLEntityDecl& entDecl)
{
    if (fOutputType == OutputType_Debug)
    {
        std::wcout  << L"Got STARTENTITY:\n    "
                    << L"Name: " << entDecl.getName() << L"\n" << std::endl;
    }
}

void TestParser::XMLDecl(const  XMLCh* const    versionStr
                        , const XMLCh* const    encodingStr
                        , const XMLCh* const    standaloneStr
                        , const XMLCh* const    autoEncStr)
{
    if (fOutputType == OutputType_Debug)
    {
        std::wcout  << L"Got XMLDECL:\n    "
                    << L"Version:\"" << versionStr << L"\""
                    << L" Encoding:\"" << encodingStr << L"\""
                    << L" Standalone:\"" << standaloneStr << L"\""
                    << L" Auto Encoding:\"" << autoEncStr << L"\""
                    << L"\n"
                    << std::endl;
    }
     else if (fOutputType == OutputType_XML)
    {
        std::wcout << L"<?xml";

        if (XMLString::stringLen(versionStr))
            std::wcout << L" version=\"" << versionStr << '"';

        if (XMLString::stringLen(encodingStr))
            std::wcout << L" encoding=\"" << encodingStr << '"';

        if (XMLString::stringLen(standaloneStr))
            std::wcout  << L" standlone=\"" << standaloneStr << '"';

        std::wcout << L" ?>";
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
        std::wcout  << L"Got ATTDEF:\n    "
                    << L"Name: " << attDef.getFullName()
                    << L", Type: "
                    << XMLAttDef::getAttTypeString(attDef.getType())
                    << L", DefType: "
                    << XMLAttDef::getDefAttTypeString(attDef.getDefaultType());

        if (XMLString::stringLen(attDef.getValue()))
            std::wcout << L", Value: \"" << attDef.getValue() << '"';

        std::wcout << L"\n" << std::endl;
    }
     else if (fOutputType != OutputType_None)
    {
        if (fInExtSubset)
            return;

        if (fIntDTDOutput)
        {
            std::wcout << attDef.getFullName() << L" ";
            if (attDef.getType() == XMLAttDef::Enumeration)
            {
                std::wcout << chOpenParen;
                const XMLCh* curCh = attDef.getEnumeration();
                while (*curCh)
                {
                    while (!XMLReader::isWhitespace(*curCh))
                        std::wcout << *curCh++;
                    curCh++;
                    if (*curCh)
                        std::wcout << chPipe;
                }
                std::wcout << chCloseParen;
            }
             else
            {
                std::wcout << XMLAttDef::getAttTypeString(attDef.getType());
            }

            if (XMLString::stringLen(attDef.getValue()))
                std::wcout << L" \"" << attDef.getValue() << '"';

            if (attDef.getDefaultType() != XMLAttDef::Default)
            {
                std::wcout  << L" "
                            << XMLAttDef::getDefAttTypeString(attDef.getDefaultType());
            }
        }
    }
}

void TestParser::doctypeComment(const XMLCh* const comment)
{
    if (fOutputType == OutputType_Debug)
    {
        std::wcout  << L"Got DTD COMMENT:\n    "
                    << L"Text: \"" << comment << L"\"\n"
                    << std::endl;
    }
     else if (fOutputType != OutputType_None)
    {
        if (fInExtSubset)
            return;

        if (fIntDTDOutput)
            std::wcout << L"<!--" << comment << L"-->";
    }
}

void TestParser::doctypeDecl(const  DTDElementDecl& elemDecl
                            , const XMLCh* const    publicId
                            , const XMLCh* const    systemId
                            , const bool            hasIntSubset)
{
    if (fOutputType == OutputType_Debug)
    {
        std::wcout  << L"Got DOCTYPE:\n    "
                    << L"Root: " << elemDecl.getFullName();

        if (XMLString::stringLen(publicId))
            std::wcout << L", PUBLIC: " << publicId;

        if (XMLString::stringLen(systemId))
            std::wcout << L", SYSTEM: " << systemId;

        std::wcout << L"\n" << std::endl;
    }
     else if (fOutputType != OutputType_None)
    {
        if (fIntDTDOutput)
        {
            std::wcout << L"<!DOCTYPE " << elemDecl.getFullName();
            showIds(publicId, systemId);

            if (!hasIntSubset)
                std::wcout << L">";
        }
    }
}

void TestParser::doctypePI( const   XMLCh* const    target
                            , const XMLCh* const    data)
{
    if (fOutputType == OutputType_Debug)
    {
        std::wcout  << L"Got DTD PI:\n     "
                    << L"Target: \"" << target << '"';

        if (XMLString::stringLen(data))
            std::wcout << L", Data: \"" << data << '"';
        std::wcout << L"\n" << std::endl;
    }
     else if (fOutputType != OutputType_None)
    {
        if (fInExtSubset)
            return;

        if (fIntDTDOutput)
        {
            std::wcout << L"<?" << target;
            if (XMLString::stringLen(data))
                std::wcout << L" " << data;
            std::wcout << L"?>";
        }
    }
}

void TestParser::doctypeWhitespace( const   XMLCh* const    chars
                                    , const unsigned int    length)
{
    if (fOutputType == OutputType_Debug)
    {
        std::wcout  << L"Got DTD Spaces:\n    Bytes: "
                    << length << L"\n" << std::endl;
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
        std::wcout  << L"Got ELEMENT DECL:\n    "
                    << L"Name: " << decl.getFullName();

        if (isIgnored)
            std::wcout << L" (Ignored)";

        std::wcout << L", Content: ";
        std::wcout << decl.getFormattedContentModel(*fScanner->getValidator());
        std::wcout << L"\n" << std::endl;
    }
     else if (fOutputType != OutputType_None)
    {
        if (fInExtSubset)
            return;

        if (fIntDTDOutput)
        {
            std::wcout << L"<!ELEMENT " << decl.getFullName() << L" ";
            std::wcout << decl.getFormattedContentModel(*fScanner->getValidator());
            std::wcout << L">";
        }
    }
}

void TestParser::endAttList(const DTDElementDecl& elemDecl)
{
    if (fOutputType == OutputType_Debug)
    {
        std::wcout  << L"Got ENDATTLIST:\n    "
                    << L"Name: " << elemDecl.getFullName() << L"\n"
                    << std::endl;
    }
     else if (fOutputType != OutputType_None)
    {
        if (fInExtSubset)
            return;

        if (fIntDTDOutput)
            std::wcout << L">";
    }
}

void TestParser::endIntSubset()
{
    if (fOutputType == OutputType_Debug)
    {
        std::wcout << L"Got ENDINTSUBSET\n" << std::endl;
    }
     else if (fOutputType != OutputType_None)
    {
        if (fIntDTDOutput)
            std::wcout << L"]>";
    }
}

void TestParser::endExtSubset()
{
    fInExtSubset = false;
    if (fOutputType == OutputType_Debug)
        std::wcout << L"Got ENDEXTSUBSET\n" << std::endl;
}

void TestParser::entityDecl(const   DTDEntityDecl&  entityDecl
                            , const bool            isPEDecl
                            , const bool            isIgnored)
{
    if (fOutputType == OutputType_Debug)
    {
        std::wcout  << L"Got ENTITYDECL:\n    "
                    << L"Name: " << entityDecl.getName()
                    << (isPEDecl ? L" [Parameter Entity]" : L" [General Entity]")
                    << L"\n" << std::endl;
    }
     else if (fOutputType != OutputType_None)
    {
        if (fInExtSubset)
            return;

        if (fIntDTDOutput)
        {
            std::wcout << L"<!ENTITY ";
            if (isPEDecl)
                std::wcout << L"% ";
            std::wcout << entityDecl.getName();
        
            if (entityDecl.isExternal())
                showIds(entityDecl.getPublicId(), entityDecl.getSystemId());
             else
                std::wcout << L" \"" << entityDecl.getValue() << L"\"";

            std::wcout << L">";
        }
    }
}

void TestParser::resetDocType()
{
    if (fOutputType == OutputType_Debug)
        std::wcout << L"Got RESETDOCTYPE:\n" << std::endl;
}

void TestParser::notationDecl(  const   XMLNotationDecl&    notDecl
                                , const bool                isIgnored)
{
    if (fOutputType == OutputType_Debug)
    {
        std::wcout  << L"Got NOTATIONDECL:\n    "
                    << L"Name: " << notDecl.getName()
                    << std::endl;
    }
     else if (fOutputType != OutputType_None)
    {
        if (fInExtSubset)
            return;

        if (fIntDTDOutput)
        {
            std::wcout << L"<!NOTATION " << notDecl.getName() << L" ";

            if (!XMLString::stringLen(notDecl.getSystemId()))
                std::wcout << L"PUBLIC ";
            else
                std::wcout << L"SYSTEM ";

            if (XMLString::stringLen(notDecl.getPublicId()))
                std::wcout << L"\"" << notDecl.getPublicId() << '"';

            if (XMLString::stringLen(notDecl.getSystemId()))
                std::wcout << L" \"" << notDecl.getSystemId() << '"';

            std::wcout << L">";
        }
    }
}

void TestParser::startAttList(const DTDElementDecl& elemDecl)
{
    if (fOutputType == OutputType_Debug)
    {
        std::wcout  << L"Got STARTATTLIST:\n    "
                    << L"Name: " << elemDecl.getFullName() << L"\n"
                    << std::endl;
    }
     else if (fOutputType != OutputType_None)
    {
        if (fInExtSubset)
            return;

        if (fIntDTDOutput)
            std::wcout << "<!ATTLIST " << elemDecl.getFullName();
    }
}

void TestParser::startIntSubset()
{
    if (fOutputType == OutputType_Debug)
    {
        std::wcout << L"Got STARTINTSUBSET\n" << std::endl;
    }
     else if (fOutputType != OutputType_None)
    {
        if (fIntDTDOutput)
            std::wcout << L" [";
    }
}

void TestParser::startExtSubset()
{
    fInExtSubset = true;
    if (fOutputType == OutputType_Debug)
        std::wcout << L"Got STARTEXTSUBSET\n" << std::endl;
}

void TestParser::TextDecl(  const   XMLCh* const    versionStr
                            , const XMLCh* const    encodingStr)
{
    if (fOutputType == OutputType_Debug)
    {
        std::wcout << L"Got TEXTDECL:\n    ";

        if (XMLString::stringLen(versionStr))
            std::wcout << L"Version: " << versionStr;

        if (XMLString::stringLen(encodingStr))
            std::wcout << L"Encoding: " << encodingStr;

        std::wcout << L"\n" << std::endl;
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

        std::wcout  << lineNum << L"/" << colNum
                    << L":" << entName
                    << L" - " << text
                    << std::endl;
        return;
    }

    // If its a warning and we are not showing warnings, then get out
    if ((type == XMLErrorReporter::ErrType_Warning) && !fShowWarnings)
        return;

    const XMLCh* typeStr = L"?";
    if (type == XMLErrorReporter::ErrType_Fatal)
        typeStr = L"ERROR";
    else if (type == XMLErrorReporter::ErrType_Warning)
        typeStr = L"WARNING";
    else if (type == XMLErrorReporter::ErrType_Invalid)
        typeStr = L"VALIDITY";

    // Output the error heading and the error type string
    std::wcout << L"\nError: (" << typeStr;

    // If we have either id, display them
    if (XMLString::stringLen(systemId))
         std::wcout << L", System Id: " << systemId;

    if (XMLString::stringLen(publicId))
        std::wcout << L", Public Id: " << publicId;

    // Display the position information
    std::wcout  << std::hex
                << L", Line/Col: " << lineNum << L"/" << colNum
                << std::dec << L")\n";

    // And finally the error text
    std::wcout << text << std::endl;
}

void TestParser::resetErrors()
{
    if (fOutputType == OutputType_Debug)
        std::wcout << L"Got RESETERRORS:\n" << std::endl;
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
                std::wcerr << L"Missing trailing surrogate\n" << std::endl;
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
            std::wcout << L"&#10;";
        else if (toShow == chHTab)
            std::wcout << L"&#9;";
        else if (toShow == chCR)
            std::wcout << L"&#13;";
        else if (toShow == chSingleQuote)
            std::wcout << L"&apos;";
        else if (toShow == chAmpersand)
            std::wcout << L"&amp;";
        else if (toShow == chDoubleQuote)
            std::wcout << L"&quot;";
        else if (toShow == chOpenAngle)
            std::wcout << L"&lt;";
        else if (toShow == chCloseAngle)
            std::wcout << L"&gt;";
        else
        {
            wchar_t tmpStr[2];
            tmpStr[0] = toShow;
            tmpStr[1] = 0;
            std::wcout << tmpStr;
        }
    }
     else
    {
        wchar_t tmpStr[2];
        tmpStr[0] = toShow;
        tmpStr[1] = 0;
        std::wcout << tmpStr;
    }
}

void
TestParser::showIds(const XMLCh* const publicId, const XMLCh* const systemId)
{
    if (XMLString::stringLen(publicId) || XMLString::stringLen(systemId))
    {
        if (!XMLString::stringLen(publicId))
        {
            std::wcout << L" SYSTEM '" << systemId << L"'";
        }
         else
        {
            std::wcout << L" PUBLIC '" << publicId << L"'";
            if (systemId)
                std::wcout << L" '" << systemId << L"'";
        }
    }
}


inline void TestParser::showString(const XMLCh* const toShow)
{
    showChars(toShow, XMLString::stringLen(toShow));
}
