/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 1999-2003 The Apache Software Foundation.  All rights
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

/*
 * This sample illustrates how you can create a DOM tree in memory.
 * It then prints the count of elements in the tree.
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/dom/DOM.hpp>
#if defined(XERCES_NEW_IOSTREAMS)
#include <iostream>
#else
#include <iostream.h>
#endif

XERCES_CPP_NAMESPACE_USE

// ---------------------------------------------------------------------------
//  This is a simple class that lets us do easy (though not terribly efficient)
//  trancoding of char* data to XMLCh data.
// ---------------------------------------------------------------------------
class XStr
{
public :
    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    XStr(const char* const toTranscode)
    {
        // Call the private transcoding method
        fUnicodeForm = XMLString::transcode(toTranscode);
    }

    ~XStr()
    {
        XMLString::release(&fUnicodeForm);
    }


    // -----------------------------------------------------------------------
    //  Getter methods
    // -----------------------------------------------------------------------
    const XMLCh* unicodeForm() const
    {
        return fUnicodeForm;
    }

private :
    // -----------------------------------------------------------------------
    //  Private data members
    //
    //  fUnicodeForm
    //      This is the Unicode XMLCh format of the string.
    // -----------------------------------------------------------------------
    XMLCh*   fUnicodeForm;
};

#define X(str) XStr(str).unicodeForm()


// ---------------------------------------------------------------------------
//  main
// ---------------------------------------------------------------------------

int main(int argC, char* argV[])
{
    // Initialize the XML4C2 system.
    try
    {
        XMLPlatformUtils::Initialize();
    }

    catch(const XMLException& toCatch)
    {
        char *pMsg = XMLString::transcode(toCatch.getMessage());
        XERCES_STD_QUALIFIER cerr << "Error during Xerces-c Initialization.\n"
             << "  Exception message:"
             << pMsg;
        XMLString::release(&pMsg);
        return 1;
    }

    // Watch for special case help request
    int errorCode = 0;
    if (argC > 1)
    {
        XERCES_STD_QUALIFIER cout << "\nUsage:\n"
                "    CreateDOMDocument\n\n"
                "This program creates a new DOM document from scratch in memory.\n"
                "It then prints the count of elements in the tree.\n"
             << XERCES_STD_QUALIFIER endl;
        errorCode = 1;
    }
    if(errorCode) {
        XMLPlatformUtils::Terminate();
        return errorCode;
    }

   {
       //  Nest entire test in an inner block.
       //  The tree we create below is the same that the XercesDOMParser would
       //  have created, except that no whitespace text nodes would be created.

       // <company>
       //     <product>Xerces-C</product>
       //     <category idea='great'>XML Parsing Tools</category>
       //     <developedBy>Apache Software Foundation</developedBy>
       // </company>

       DOMImplementation* impl =  DOMImplementationRegistry::getDOMImplementation(X("Core"));

       if (impl != NULL)
       {
           try
           {
               DOMDocument* doc = impl->createDocument(
                           0,                    // root element namespace URI.
                           X("company"),         // root element name
                           0);                   // document type object (DTD).

               DOMElement* rootElem = doc->getDocumentElement();

               DOMElement*  prodElem = doc->createElement(X("product"));
               rootElem->appendChild(prodElem);

               DOMText*    prodDataVal = doc->createTextNode(X("Xerces-C"));
               prodElem->appendChild(prodDataVal);

               DOMElement*  catElem = doc->createElement(X("category"));
               rootElem->appendChild(catElem);

               catElem->setAttribute(X("idea"), X("great"));

               DOMText*    catDataVal = doc->createTextNode(X("XML Parsing Tools"));
               catElem->appendChild(catDataVal);

               DOMElement*  devByElem = doc->createElement(X("developedBy"));
               rootElem->appendChild(devByElem);

               DOMText*    devByDataVal = doc->createTextNode(X("Apache Software Foundation"));
               devByElem->appendChild(devByDataVal);

               //
               // Now count the number of elements in the above DOM tree.
               //

               unsigned int elementCount = doc->getElementsByTagName(X("*"))->getLength();
               XERCES_STD_QUALIFIER cout << "The tree just created contains: " << elementCount
                    << " elements." << XERCES_STD_QUALIFIER endl;

               doc->release();
           }

           catch (const DOMException& e)
           {
               XERCES_STD_QUALIFIER cerr << "DOMException code is:  " << e.code << XERCES_STD_QUALIFIER endl;
               errorCode = 2;
           }
           catch (...)
           {
               XERCES_STD_QUALIFIER cerr << "An error occurred creating the document" << XERCES_STD_QUALIFIER endl;
               errorCode = 3;
           }
       }  // (inpl != NULL)
       else
       {
           XERCES_STD_QUALIFIER cerr << "Requested implementation is not supported" << XERCES_STD_QUALIFIER endl;
           errorCode = 4;
       }
   }

   XMLPlatformUtils::Terminate();
   return errorCode;
}

