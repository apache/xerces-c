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
 * Revision 1.5  2000/03/02 19:53:39  roddey
 * This checkin includes many changes done while waiting for the
 * 1.1.0 code to be finished. I can't list them all here, but a list is
 * available elsewhere.
 *
 * Revision 1.4  2000/02/06 07:47:17  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.3  2000/01/19 00:17:50  rahulj
 * Added makefile for unix builds. Fixed the comments and usage
 * string.
 *
 * Revision 1.2  2000/01/18 23:57:35  rahulj
 * Now exploting C++ features to compact the sample code.
 *
 * Revision 1.1  2000/01/18 23:22:18  rahulj
 * Added new sample to illustrate how to create a DOM tree in
 * memory.
 *
 */

/*
 * This sample illustrates how you can create a DOM tree in memory.
 * It then prints the count of elements in the tree.
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <util/PlatformUtils.hpp>
#include <util/XMLString.hpp>
#include <dom/DOM.hpp>
#include <iostream.h>



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
        cerr << "Error during Xerces-c Initialization.\n"
             << "  Exception message:"
             << pMsg;
        delete pMsg;
        return 1;
    }

    // Watch for special case help request
    if (argC > 1)
    {
        cout << "\nUsage: CreateDOMDocument\n"
            "   This program creates a new DOM document from scratch in memory.\n"
            "   It then prints the count of elements in the tree.\n"
            <<  endl;
        return 0;
    }

    //  The tree we create below is the same that the DOMParser would
    //  have created, except that no whitespace text nodes would be created.

    // <company>
    //     <product>Xerces-C</product>
    //     <category idea='great'>XML Parsing Tools</category>
    //     <developedBy>Apache Software Foundation</developedBy>
    // </company>

    DOM_DOMImplementation impl;
    
    DOM_Document doc = impl.createDocument(
                0,                    // root element namespace URI.
                "company",            // root element name
                DOM_DocumentType());  // document type object (DTD).

    DOM_Element rootElem = doc.getDocumentElement();

    DOM_Element  prodElem = doc.createElement("product");
    rootElem.appendChild(prodElem);

    DOM_Text    prodDataVal = doc.createTextNode("Xerces-C");
    prodElem.appendChild(prodDataVal);

    DOM_Element  catElem = doc.createElement("category");
    rootElem.appendChild(catElem);
    catElem.setAttribute("idea", "great");

    DOM_Text    catDataVal = doc.createTextNode("XML Parsing Tools");
    catElem.appendChild(catDataVal);

    DOM_Element  devByElem = doc.createElement("developedBy");
    rootElem.appendChild(devByElem);
    
    DOM_Text    devByDataVal = doc.createTextNode("Apache Software Foundation");
    devByElem.appendChild(devByDataVal);

    //
    // Now count the number of elements in the above DOM tree.
    //

    unsigned int elementCount = doc.getElementsByTagName("*").getLength();
    cout << "The tree just created contains: " << elementCount
         << " elements." << endl;

    //
    //  The DOM document and its contents are reference counted, and need
    //  no explicit deletion.
    //
    return 0;
}


