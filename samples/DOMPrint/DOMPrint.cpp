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
//  This sample program which invokes the DOMParser to build a DOM tree for
//  the specified input file. It then walks the tree, and prints out the data
//  as an XML file.
//
//  The parameters are:
//
//      [-?]            - Show usage and exit
//      [-v]            - Do validation
//      [-n]            - Enable namespace processing
//      [-e]            - Expand Entity References
//      [-x]            - Create XMLDecl type nodes      
//      [-NoEscape]     - Don't escape characters like '<' or '&'.
//      filename        - The path to the XML file to parse
//
//  These are non-case sensitive
//
//   Limitations:
//      1.  The encoding="xxx" clause in the XML header should reflect
//          the system local code page, but does not.
//      2.  Cases where the XML data contains characters that can not
//          be represented in the system local code page are not handled.
//      3.  Enabled namespace processing won't affect the output, since
//          DOM doesn't do namespace yet. But it will confirm that all
//          prefixes are correctly mapped, else you'll get errors.
// ---------------------------------------------------------------------------


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <util/PlatformUtils.hpp>
#include <parsers/DOMParser.hpp>
#include <dom/DOM_Node.hpp>
#include <dom/DOM_NamedNodeMap.hpp>
#include <dom/DomMemDebug.hpp>
#include "DOMTreeErrorReporter.hpp"
#include <string.h>
#include <stdlib.h>



// ---------------------------------------------------------------------------
//  Local data
//
//  xmlFile
//      The path to the file to parser. Set via command line.
//
//  doEscapes
//      Indicates whether special chars should be escaped in the output.
//      Defaults to doing escapes, -NoEscape overrides.
//
//  doNamespaces
//      Indicates whether namespace processing should be done.
//
//  doExpand
//      Indicates whether entity references needs to be expanded or not
//      Defaults to false
//
//  doValidation
//      Indicates whether validation should be done. The default is not
//      to validate, but -v overrides that.
// ---------------------------------------------------------------------------
static char*	xmlFile         = 0;
static bool     doEscapes       = true;
static bool     doNamespaces    = false;
static bool     doValidation    = false;
static bool     doExpand        = false;
static bool     doCreateXMLDecl = false;


// ---------------------------------------------------------------------------
//  Forward references
// ---------------------------------------------------------------------------
void     outputContent(ostream& target, const DOMString &s);
void     usage();
ostream& operator<<(ostream& target, const DOMString& toWrite);
ostream& operator<<(ostream& target, DOM_Node& toWrite);



// ---------------------------------------------------------------------------
//
//  main
//
// ---------------------------------------------------------------------------
int main(int argC, char* argV[])
{
    // Initialize the XML4C2 system
    try
    {
        XMLPlatformUtils::Initialize();
    }

    catch(const XMLException& toCatch)
    {
        cerr << "Error during Xerces-c Initialization.\n"
             << "  Exception message:"
             << DOMString(toCatch.getMessage()) << endl;
        return 1;
    }

    // Check command line and extract arguments.
    if (argC < 2)
    {
        usage();
        XMLPlatformUtils::Terminate();
        return 1;
    }

    // Watch for special case help request
    if (!strcmp(argV[1], "-?"))
    {
        usage();
        XMLPlatformUtils::Terminate();
        return 2;
    }

    // See if non validating dom parser configuration is requested.
    int parmInd;
    for (parmInd = 1; parmInd < argC; parmInd++)
    {
        // Break out on first parm not starting with a dash
        if (argV[parmInd][0] != '-')
            break;

        if (!strcmp(argV[parmInd], "-v")
        ||  !strcmp(argV[parmInd], "-V"))
        {
            doValidation = true;
        }
         else if (!strcmp(argV[parmInd], "-n")
              ||  !strcmp(argV[parmInd], "-N"))
        {
            doNamespaces = true;
        }
         else if (!strcmp(argV[parmInd], "-e")
              ||  !strcmp(argV[parmInd], "-E"))
        {
            doExpand = true;
        }
         else if (!strcmp(argV[parmInd], "-x")
              ||  !strcmp(argV[parmInd], "-X"))
        {
            doCreateXMLDecl = true;
        }
         else if (!strcmp(argV[parmInd], "-NoEscape"))
        {
            doEscapes = false;
        }
         else
        {
            usage();
            XMLPlatformUtils::Terminate();
            return 1;
        }
    }

    //
    //  And now we have to have only one parameter left and it must be
    //  the file name.
    //
    if (parmInd + 1 != argC)
    {
        usage();
        XMLPlatformUtils::Terminate();
        return 1;
    }
    xmlFile = argV[parmInd];

    //
    //  Create our parser, then attach an error handler to the parser.
    //  The parser will call back to methods of the ErrorHandler if it
    //  discovers errors during the course of parsing the XML document.
    //
    DOMParser *parser = new DOMParser;
    parser->setDoValidation(doValidation);
    parser->setDoNamespaces(doNamespaces);
    ErrorHandler *errReporter = new DOMTreeErrorReporter();
    parser->setErrorHandler(errReporter);
	parser->setExpandEntityReferences(doExpand);
    parser->setToCreateXMLDeclTypeNode(doCreateXMLDecl);
    //
    //  Parse the XML file, catching any XML exceptions that might propogate
    //  out of it.
    //
    bool errorsOccured = false;
    try
    {
        parser->parse(xmlFile);
    }

    catch (const XMLException& e)
    {
        cerr << "An error occured during parsing\n   Message: "
             << DOMString(e.getMessage()) << endl;
        errorsOccured = true;
    }

    // If the parse was successful, output the document data from the DOM tree
    if (!errorsOccured)
    {
        DOM_Node doc = parser->getDocument();
		cout << doc << endl;
    }

    //
    //  Clean up the error handler. The parser does not adopt handlers
    //  since they could be many objects or one object installed for multiple
    //  handlers.
    //
    delete errReporter;

    //
    //  Delete the parser itself.  Must be done prior to calling Terminate, below.
    //
    delete parser;

    // And call the termination method
    XMLPlatformUtils::Terminate();
    // DomMemDebug().print();

    //
	//  The DOM document and its contents are reference counted, and need
	//	no explicit deletion.
    //
    return 0;
}



// ---------------------------------------------------------------------------
//
//  Usage()
//
// ---------------------------------------------------------------------------
void usage()
{
    cout << "\nUsage: DOMPrint [options] file\n\n"
            "   This program invokes the XML4C DOM parser and \n"
            "   builds the DOM tree. It then traverses the DOM \n"
            "   tree and prints the contents of the tree\n"
            "   Options are case insensitive\n\n"
            "   Options:\n"
            "     -v          Validate the data according to the DTD\n"
            "     -n          Enable namespace processing\n"
            "     -NoEscape   Don't escape special characters in output\n"
            "     -?          Show this help (must be the only parameter)\n"
          <<  endl;
}




// ---------------------------------------------------------------------------
//  ostream << DOM_Node   
//
//  Stream out a DOM node, and, recursively, all of its children. This
//  function is the heart of writing a DOM tree out as XML source. Give it
//  a document node and it will do the whole thing.
// ---------------------------------------------------------------------------
ostream& operator<<(ostream& target, DOM_Node& toWrite)
{
    // Get the name and value out for convenience
    DOMString   nodeName = toWrite.getNodeName();
    DOMString   nodeValue = toWrite.getNodeValue();

	switch (toWrite.getNodeType())
    {
		case DOM_Node::TEXT_NODE:
        {
            outputContent(target, nodeValue);
            break;
        }

        case DOM_Node::PROCESSING_INSTRUCTION_NODE :
        {
            target  << "<?"
                    << nodeName
                    << ' '
                    << nodeValue
                    << "?>";
            break;
        }

        case DOM_Node::DOCUMENT_NODE :
        {
            //
            //  Bug here:  we need to find a way to get the encoding name
            //  for the default code page on the system where the program
            //  is running, and plug that in for the encoding name.  
            //
            //target << "<?xml version='1.0' encoding='ISO-8859-1' ?>\n";
            DOM_Node child = toWrite.getFirstChild();
            while( child != 0)
            {
                target << child << endl;
                child = child.getNextSibling();
            }
            break;
        }

        case DOM_Node::ELEMENT_NODE :
        {
            // Output the element start tag.
            target << '<' << nodeName;

            // Output any attributes on this element
            DOM_NamedNodeMap attributes = toWrite.getAttributes();
            int attrCount = attributes.getLength();
            for (int i = 0; i < attrCount; i++)
            {
                DOM_Node  attribute = attributes.item(i);

                target  << ' ' << attribute.getNodeName()
                        << " = \"";
                        //  Note that "<" must be escaped in attribute values.
                        outputContent(target, attribute.getNodeValue());
                        target << '"';
            }

            //
            //  Test for the presence of children, which includes both
            //  text content and nested elements.
            //
            DOM_Node child = toWrite.getFirstChild();
            if (child != 0)
            {
                // There are children. Close start-tag, and output children.
                target << ">";
                while( child != 0)
                {
                    target << child;
                    child = child.getNextSibling();
                }

                // Done with children.  Output the end tag.
                target << "</" << nodeName << ">";
            }
            else
            {
                //
                //  There were no children. Output the short form close of
                //  the element start tag, making it an empty-element tag.
                //
                target << "/>";
            }
            break;
        }

        case DOM_Node::ENTITY_REFERENCE_NODE:
        {
            DOM_Node child;
            for (child = toWrite.getFirstChild(); child != 0; child = child.getNextSibling())
                target << child;
            break;
        }

        case DOM_Node::CDATA_SECTION_NODE:
        {
            target << "<![CDATA[" << nodeValue << "]]>";
            break;
        }

        case DOM_Node::COMMENT_NODE:
        {
            target << "<!--" << nodeValue << "-->";
            break;
        }

        case DOM_Node::DOCUMENT_TYPE_NODE:
        {
			DOM_DocumentType doctype = (DOM_DocumentType &)toWrite;;

			target << "<!DOCTYPE " << nodeName ;
			DOMString id = doctype.getPublicId();
			if (id != 0)
				target << " PUBLIC \"" << id << "\"";
			id = doctype.getSystemId();
			if (id != 0)
				target << " SYSTEM \"" << id << "\"";
			id = doctype.getInternalSubset(); 
			if (id !=0)
				target << " [ " << id  << "]";
			target  << ">" << endl;
            break;
        }
		case DOM_Node::ENTITY_NODE:
        {
			target << "<!ENTITY " << nodeName;
			DOMString id = ((DOM_Entity &)toWrite).getPublicId();
			if (id != 0)
				target << "PUBLIC \"" << id << "\"";
			id = ((DOM_Entity &)toWrite).getSystemId();
			if (id != 0)
				target << "SYSTEM \"" << id << "\"";
			id = ((DOM_Entity &)toWrite).getNotationName();
			if (id != 0)
				target << "NDATA \"" << id << "\"";
			target << endl;

            break;
        }
        case DOM_Node::XML_DECL_NODE:
        {
            target << "<?xml version=" << ((DOM_XMLDecl &)toWrite).getVersion();
            DOMString str = ((DOM_XMLDecl &)toWrite).getEncoding();
            if (str != 0)
                target << " encoding=" << str;
            str = ((DOM_XMLDecl &)toWrite).getStandalone();
            if (str != 0)
                target << " standalone=" << str;
            target << "?>" << endl;
            break;
        }
        default:
            cerr << "Unrecognized node type = "
                 << (long)toWrite.getNodeType() << endl;
    }
	return target;
}


// ---------------------------------------------------------------------------
//  outputContent
//
//  Write document content from a DOMString to a C++ ostream. Escape the
//  XML special characters (<, &, etc.) unless this is suppressed by the
//  command line option.
// ---------------------------------------------------------------------------
void outputContent(ostream& target, const DOMString &toWrite)
{
    
    if (doEscapes == false)
    {
        target << toWrite;
    }
     else
    {
        int            length = toWrite.length();
        const XMLCh*   chars  = toWrite.rawBuffer();
        
        int index;
        for (index = 0; index < length; index++)
        {
            switch (chars[index])
            {
            case chAmpersand :
                target << "&amp;";
                break;
                
            case chOpenAngle :
                target << "&lt;";
                break;
                
            case chCloseAngle:
                target << "&gt;";
                break;
                
            case chDoubleQuote :
                target << "&quot;";
                break;
                
            default:
                // If it is none of the special characters, print it as such
                target << toWrite.substringData(index, 1);
                break;
            }
        }
    }

    return;
}


// ---------------------------------------------------------------------------
//  ostream << DOMString
//
//  Stream out a DOM string. Doing this requires that we first transcode
//  to char * form in the default code page for the system
// ---------------------------------------------------------------------------
ostream& operator<<(ostream& target, const DOMString& s)
{
    char *p = s.transcode();
    target << p;
    delete [] p;
    return target;
}
