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


#include <stdio.h>
#include <string.h>
#include <dom/DOM.hpp>
#include <dom/DomMemDebug.hpp>
#include <util/PlatformUtils.hpp>
#include <util/XMLException.hpp>
#include <util/XMLString.hpp>
#include <parsers/DOMParser.hpp>
#include <framework/MemBufInputSource.hpp>
#include <sax/ErrorHandler.hpp>
#include <sax/SAXException.hpp>
#include <sax/SAXParseException.hpp>

//-------------------------------------------------------------
//
//   Macros of general use in tests.
//
//-------------------------------------------------------------
#define TASSERT(c) tassert((c), __FILE__, __LINE__)

void tassert(bool c, char *file, int line)
{
    if (!c)
        printf("Failure.  Line %d,   file %s\n", line, file);
};


#define TESTPROLOG entryMemState = DomMemDebug();

#define TESTEPILOG \
    exitMemState = DomMemDebug(); \
    if (entryMemState != exitMemState) { \
        printf(" Memory leak at line %d, file %s:  ", __LINE__, __FILE__);  \
        exitMemState.printDifference(entryMemState); \
    }


#define EXCEPTION_TEST(operation, expected_exception)               \
{                                                                   \
    try {                                                           \
    operation;                                                      \
    printf(" Error: no exception thrown at line %d\n", __LINE__);   \
}                                                                   \
    catch (DOM_DOMException &e) {                                       \
    if (e.code != expected_exception)                       \
    printf(" Wrong exception code: %d at line %d\n", e.code, __LINE__); \
}                                                                 \
    catch (...)   {                                                 \
    printf(" Wrong exception thrown at line %d\n", __LINE__);       \
}                                                                   \
}




// ---------------------------------------------------------------------------
//
//  Simple error handler to install on parser
//      With no error handler errors are silently ignored.
//      This test program does not normally produce XML errors, but having
//       an error handler is useful for debugging.
//
// ---------------------------------------------------------------------------
class SimpleErrorHandler : public ErrorHandler
{
public:
    SimpleErrorHandler() {};
    virtual ~SimpleErrorHandler() {};

    void warning(const SAXParseException& e) {printError(e);};
    void error(const SAXParseException& e){printError(e);};
    void fatalError(const SAXParseException& e){printError(e);};
    void resetErrors() {};
    void printError(const SAXParseException &e) { fprintf(stderr, "\n Error in parsing.\n"
                                                            " line: %d,   col: %d \n"
                                                            " Message: %s\n",
                                                            e.getLineNumber(),
                                                            e.getColumnNumber(),
                                                            XMLString::transcode(e.getMessage()));};
private :
    SimpleErrorHandler(const SimpleErrorHandler&);
    void operator=(const SimpleErrorHandler&);
};



// ---------------------------------------------------------------------------
//
//  Small in-memory test document
//
// ---------------------------------------------------------------------------
    static const char*  TestDoc1 =
"<?xml version='1.0' encoding='ascii'?>     \n\
<!DOCTYPE doc [                             \n\
<!ELEMENT doc  (elA | elB)*>                \n\
<!ELEMENT elA             (#PCDATA)>        \n\
<!ELEMENT elB             (#PCDATA)>        \n\
<!ATTLIST elA    id ID    #IMPLIED>         \n\
<!ATTLIST elB    id CDATA #IMPLIED>         \n\
]>                                          \n\
                                            \n\
<doc>                                       \n\
    <elA id='a001'/>                        \n\
    <elB id='a002'/>                        \n\
    <elA id='a003'/>                        \n\
</doc>                                      \n\
";


int main()
{
    // Initialize the XML4C2 system
    try
    {
         XMLPlatformUtils::Initialize();
    }
    catch (const XMLException& toCatch)
    {
         fprintf(stderr, "Error during initialization! Message: \n%s\n", 
              XMLString::transcode(toCatch.getMessage()));
         return 1;
    }

    bool doValidation    = true;
    bool doNamespaces    = false;

    DOMParser *parser = new DOMParser;
    parser->setDoValidation(doValidation);
    parser->setDoNamespaces(doNamespaces);

    ErrorHandler *ehandler = new SimpleErrorHandler();
    parser->setErrorHandler(ehandler);

    MemBufInputSource* memBufIS = new MemBufInputSource (
        (const XMLByte*)TestDoc1,
        strlen(TestDoc1),
        "TestDoc1",
        false
        );
    parser->parse(*memBufIS);

    DOM_Document doc = parser->getDocument();


    DomMemDebug     entryMemState, exitMemState;

    TESTPROLOG;
    {
        DOM_Element elA = doc.getElementById("a001");
        TASSERT(elA != 0);
        
        DOM_Element elB = doc.getElementById("a002");
        TASSERT(elB == 0);
        
        DOM_Element elC = doc.getElementById("a003");
        TASSERT(elC != 0);
        TASSERT(elC != elA);
        
        DOMString s = elA.getAttribute("id");
        TASSERT(s.equals("a001"));
        
        s = elC.getAttribute("id");
        TASSERT(s.equals("a003"));
        
    }
    TESTEPILOG;
        
    doc = 0;
    delete parser;
    delete memBufIS;
    delete ehandler;

    //
    //  Print Final allocation stats for full set of tests
    //
    XMLPlatformUtils::Terminate();
    DomMemDebug().print();
    return 0;


return 0;
}

