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
 * $Id
 */

// ---------------------------------------------------------------------------
//  This program tests the XMLPlatformUtils::Initialize/Terminate() pair
//  by calling the pair a number of times.
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <util/PlatformUtils.hpp>
#include <sax/SAXException.hpp>
#include <sax/SAXParseException.hpp>

#include <parsers/DOMParser.hpp>
#include <parsers/IDOMParser.hpp>
#include <parsers/SAXParser.hpp>
#include <sax2/SAX2XMLReader.hpp>
#include <sax2/XMLReaderFactory.hpp>

#include <dom/DOM_DOMException.hpp>
#include <idom/IDOM_DOMException.hpp>
#include "InitTermTest.hpp"

#include <string.h>
#include <stdlib.h>
#include <fstream.h>
#include <limits.h>


// ---------------------------------------------------------------------------
//  Local Enum to switch theState
// ---------------------------------------------------------------------------
enum Teststate {
    Once,
    Multiple,
    UnEven,
    Limit,
    ExceedLimit
};

// ---------------------------------------------------------------------------
//  Declare functions
// ---------------------------------------------------------------------------
int TestInit4DOM(const char* xmlFile, Teststate theState);
int TestInit4DOM(const char* xmlFile, Teststate theState);
int TestInit4SAX(const char* xmlFile, Teststate theState);
int TestInit4SAX2(const char* xmlFile, Teststate theState);

// ---------------------------------------------------------------------------
//  Define macro
// ---------------------------------------------------------------------------
#define TESTINITPRE                                               \
    long times = 1;                                               \
    switch (theState) {                                           \
        case Multiple:                                            \
        case UnEven:                                              \
            times = 100;                                           \
            break;                                                \
        case Limit:                                               \
        case ExceedLimit:                                         \
            times = LONG_MAX;                                     \
            break;                                                \
        case Once:                                                \
        default:                                                  \
            times = 1;                                            \
    }                                                             \
    long i = 0;                                                   \
    for (i = 0; i < times; i++) {                                 \
        try                                                       \
        {                                                         \
            XMLPlatformUtils::Initialize();                       \
        }                                                         \
                                                                  \
        catch (const XMLException& toCatch)                       \
        {                                                         \
             cerr << "Error during initialization! :\n"           \
                  << StrX(toCatch.getMessage()) << endl;          \
             return 1;                                            \
        }                                                         \
    }                                                             \
                                                                  \
    if (theState == ExceedLimit) {                                \
        try                                                       \
        {                                                         \
            XMLPlatformUtils::Initialize();                       \
        }                                                         \
                                                                  \
        catch (const XMLException& toCatch)                       \
        {                                                         \
             cerr << "Error during initialization! :\n"           \
                  << StrX(toCatch.getMessage()) << endl;          \
             return 1;                                            \
        }                                                         \
    }


#define TESTINITPOST                                              \
    InitTermTestErrorHandler errorHandler;                        \
    parser->setErrorHandler(&errorHandler);                       \
    bool   errorOccurred = false;                                 \
                                                                  \
    errorHandler.resetErrors();                                   \
                                                                  \
    try                                                           \
    {                                                             \
        parser->parse(xmlFile);                                   \
    }                                                             \
                                                                  \
    catch (const XMLException& toCatch)                           \
    {                                                             \
        cerr << "\nError during parsing: '" << xmlFile << "'\n"   \
             << "Exception message is:  \n"                       \
             << StrX(toCatch.getMessage()) << "\n" << endl;       \
        errorOccurred = true;                                     \
    }                                                             \
    catch (const DOM_DOMException& toCatch)                       \
    {                                                             \
        cerr << "\nDOM Error during parsing: '" << xmlFile        \
             << "\nDOMException code is:  \n"                     \
             << toCatch.code << "\n" << endl;                     \
        errorOccurred = true;                                     \
    }                                                             \
    catch (...)                                                   \
    {                                                             \
        cerr << "\nUnexpected exception during parsing: '"        \
        << xmlFile << "'\n";                                      \
        errorOccurred = true;                                     \
    }                                                             \
                                                                  \
    if (errorHandler.getSawErrors())                              \
    {                                                             \
        cout << "\nErrors occured, no output available\n" << endl;\
        errorOccurred = true;                                     \
    }                                                             \
                                                                  \
    delete parser;                                                \
                                                                  \
    for (i = 0; i < times; i++) {                                 \
        XMLPlatformUtils::Terminate();                            \
    }                                                             \
                                                                  \
    if (theState == ExceedLimit || theState == UnEven) {          \
        XMLPlatformUtils::Terminate();                            \
    }                                                             \
                                                                  \
    if (errorOccurred)                                            \
        return 4;                                                 \
    else                                                          \
        return 0;

// ---------------------------------------------------------------------------
//  DOM Parser
// ---------------------------------------------------------------------------
int TestInit4DOM(const char* xmlFile, Teststate theState)
{
    TESTINITPRE;
    DOMParser* parser = new DOMParser;
    TESTINITPOST;
}

// ---------------------------------------------------------------------------
//  IDOM Parser
// ---------------------------------------------------------------------------
int TestInit4IDOM(const char* xmlFile, Teststate theState)
{
    TESTINITPRE;
    IDOMParser* parser = new IDOMParser;
    TESTINITPOST;
}

// ---------------------------------------------------------------------------
//  SAX Parser
// ---------------------------------------------------------------------------
int TestInit4SAX(const char* xmlFile, Teststate theState)
{
    TESTINITPRE;
    SAXParser* parser = new SAXParser;
    TESTINITPOST;
}

// ---------------------------------------------------------------------------
//  SAX2 XML Reader
// ---------------------------------------------------------------------------
int TestInit4SAX2(const char* xmlFile, Teststate theState)
{
    TESTINITPRE;
    SAX2XMLReader* parser = XMLReaderFactory::createXMLReader();
    TESTINITPOST;
}

// ---------------------------------------------------------------------------
//  Main
// ---------------------------------------------------------------------------
int main(int argC, char* argV[]) {
    // Check command line and extract arguments.
    if (argC != 2)
    {
        cout << "\nUsage:\n"
                "    InitTermTest <XML file>\n\n"
                "This program tests the XMLPlatformUtils::Initialize()/Terminate()\n"
                "pair by calling it a number of times."
             << endl;
        return 1;
    }

    char* xmlFile = argV[1];
    bool error = false;

    //
    // Calling Init/Term pair a number of times
    //
    unsigned int i = 0;
    for (i=0; i < 100; i++) {
        if (error) {
            cout << "Test Failed" << endl;
            return 4;
        }

        if (TestInit4DOM(xmlFile, Once))
            error = true;
        if (TestInit4IDOM(xmlFile, Once))
            error = true;
        if (TestInit4SAX(xmlFile, Once))
            error = true;
        if (TestInit4SAX2(xmlFile, Once))
            error = true;
    }

    if (error || TestInit4DOM(xmlFile, Multiple))
        error = true;
    if (error || TestInit4IDOM(xmlFile, Multiple))
        error = true;
    if (error || TestInit4SAX(xmlFile, Multiple))
        error = true;
    if (error || TestInit4SAX2(xmlFile, Multiple))
        error = true;
/*
 * The following Limit test is a stress test that can run a long time
 * Commented out for regular sanity test
 */
/*
    if (error || TestInit4DOM(xmlFile, Limit))
        error = true;
    if (error || TestInit4IDOM(xmlFile, Limit))
        error = true;
    if (error || TestInit4SAX(xmlFile, Limit))
        error = true;
    if (error || TestInit4SAX2(xmlFile, Limit))
        error = true;

    if (error || TestInit4DOM(xmlFile, ExceedLimit))
        error = true;
    if (error || TestInit4IDOM(xmlFile, ExceedLimit))
        error = true;
    if (error || TestInit4SAX(xmlFile, ExceedLimit))
        error = true;
    if (error || TestInit4SAX2(xmlFile, ExceedLimit))
        error = true;
*/

    if (error || TestInit4DOM(xmlFile, UnEven))
        error = true;
    if (error || TestInit4IDOM(xmlFile, UnEven))
        error = true;
    if (error || TestInit4SAX(xmlFile, UnEven))
        error = true;
    if (error || TestInit4SAX2(xmlFile, UnEven))
        error = true;

    if (error) {
        cout << "Test Failed" << endl;
        return 4;
    }

    cout << "Test Run Successfully" << endl;
    return 0;
}

// ---------------------------------------------------------------------------
//  InitTermTestErrorHandler
// ---------------------------------------------------------------------------
InitTermTestErrorHandler::InitTermTestErrorHandler() :

    fSawErrors(false)
{
}

InitTermTestErrorHandler::~InitTermTestErrorHandler()
{
}

void InitTermTestErrorHandler::error(const SAXParseException& e)
{
    fSawErrors = true;
    cerr << "\nError at file " << StrX(e.getSystemId())
         << ", line " << e.getLineNumber()
         << ", char " << e.getColumnNumber()
         << "\n  Message: " << StrX(e.getMessage()) << endl;
}

void InitTermTestErrorHandler::fatalError(const SAXParseException& e)
{
    fSawErrors = true;
    cerr << "\nFatal Error at file " << StrX(e.getSystemId())
         << ", line " << e.getLineNumber()
         << ", char " << e.getColumnNumber()
         << "\n  Message: " << StrX(e.getMessage()) << endl;
}

void InitTermTestErrorHandler::warning(const SAXParseException& e)
{
    cerr << "\nWarning at file " << StrX(e.getSystemId())
         << ", line " << e.getLineNumber()
         << ", char " << e.getColumnNumber()
         << "\n  Message: " << StrX(e.getMessage()) << endl;
}

void InitTermTestErrorHandler::resetErrors()
{
    fSawErrors = false;
}
