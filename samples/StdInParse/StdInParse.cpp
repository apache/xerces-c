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
 * Revision 1.2  2000/01/12 00:27:01  roddey
 * Updates to work with the new URL and input source scheme.
 *
 * Revision 1.1.1.1  1999/11/09 01:09:27  twl
 * Initial checkin
 *
 * Revision 1.5  1999/11/08 20:43:43  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <framework/StdInInputSource.hpp>
#include <parsers/SAXParser.hpp>
#include "StdInParse.hpp"


// ---------------------------------------------------------------------------
//  Local data
//
//  doNamespaces
//      Indicates whether namespace processing should be enabled or not.
//      The default is no, but -ns overrides that.
//
//  doValidation
//      Indicates whether validation should be done. The default is not
//      to validate, but -v overrides that.
// ---------------------------------------------------------------------------
static bool     doNamespaces    = false;
static bool     doValidation    = false;



// ---------------------------------------------------------------------------
//  Local helper methods
// ---------------------------------------------------------------------------
void usage()
{
    cout << "\nUsage:\n"
         << "    StdInParse [options]\n"
         << "    -v  Do a validating parse. Defaults to non-validating.\n"
         << "    -ns Enable namespace processing.\n\n"
         << "This program allows you to redirect a file into the program\n"
         << "to be parsed. It will count the elements, characters, and \n"
         << "spaces and display these stats at the end\n"
         << endl;
}


// ---------------------------------------------------------------------------
//  Program entry point
// ---------------------------------------------------------------------------
int main(int argC, char* argV[])
{
    // Initialize the XML4C system
    try
    {
         XMLPlatformUtils::Initialize();
    }

    catch (const XMLException& toCatch)
    {
         cerr << "Error during initialization! Message:\n"
              << StrX(toCatch.getMessage()) << endl;
         return 1;
    }

    unsigned int parmInd;
    for (parmInd = 1; parmInd < (unsigned int)argC; parmInd++)
    {
        if (!strcmp(argV[parmInd], "-v")
        ||  !strcmp(argV[parmInd], "-V"))
        {
            doValidation = true;
        }
         else if (!strcmp(argV[parmInd], "-NS")
              ||  !strcmp(argV[parmInd], "-ns"))
        {
            doNamespaces = true;
        }
         else
        {
            usage();
            return 1;
        }
    }

    //
    //  Create a SAX parser object. Then, according to what we were told on
    //  the command line, set the options.
    //
    SAXParser parser;
    parser.setDoValidation(doValidation);
    parser.setDoNamespaces(doNamespaces);


    //
    //  Create our SAX handler object and install it on the parser, as the
    //  document and error handler. We are responsible for cleaning them
    //  up, but since its just stack based here, there's nothing special
    //  to do.
    //
    StdInParseHandlers handler;
    parser.setDocumentHandler(&handler);
    parser.setErrorHandler(&handler);

    //
    //  Kick off the parse and catch any exceptions. Create a standard
    //  input input source and tell the parser to parse from that.
    //
    try
    {
        StdInInputSource src;
        parser.parse(src);
    }

    catch (const XMLException& e)
    {
        cerr << "\nError during parsing: \n"
             << StrX(e.getMessage())
             << "\n" << endl;
        return -1;
    }

    // Print out the stats that we collected and time taken
    cout << handler.getElementCount() << " elems, "
         << handler.getAttrCount() << " attrs, "
         << handler.getSpaceCount() << " spaces, "
         << handler.getCharacterCount() << " chars)" << endl;

    return 0;
}



// ---------------------------------------------------------------------------
//  StrX: Private helper methods
// ---------------------------------------------------------------------------
void StrX::transcode(const XMLCh* const toTranscode, const unsigned int len)
{
    // Short circuit if its a null pointer
    if (!toTranscode || (!toTranscode[0]))
    {
        fLocalForm = new char[1];
        fLocalForm[0] = 0;
        return;
	}

    // See if our XMLCh and wchar_t as the same on this platform
    const bool isSameSize = (sizeof(XMLCh) == sizeof(wchar_t));

    //
    //  Get the actual number of chars. If the passed len is zero, its null
    //  terminated. Else we have to use the len.
    //
    wchar_t realLen = (wchar_t)len;
    if (!realLen)
    {
        //
        //  We cannot just assume we can use wcslen() because we don't know
        //  if our XMLCh is the same as wchar_t on this platform.
        //
        const XMLCh* tmpPtr = toTranscode;
        while (*(tmpPtr++))
            realLen++;
    }

    //
    //  If either the passed length was non-zero or our char sizes are not 
    //  same, we have to use a temp buffer. Since this is common in these
    //  samples, we just do it anyway.
    //
    wchar_t* tmpSource = new wchar_t[realLen + 1];
    if (isSameSize)
    {
        memcpy(tmpSource, toTranscode, realLen * sizeof(wchar_t));
    }
     else
    {
        for (unsigned int index = 0; index < realLen; index++)
            tmpSource[index] = (wchar_t)toTranscode[index];
    }
    tmpSource[realLen] = 0;

    // See now many chars we need to transcode this guy
    const unsigned int targetLen = ::wcstombs(0, tmpSource, 0);

    // Allocate out storage member
    fLocalForm = new char[targetLen + 1];

    //
    //  And transcode our temp source buffer to the local buffer. Cap it
    //  off since the converter won't do it (because the null is beyond
    //  where the target will fill up.)
    //
    ::wcstombs(fLocalForm, tmpSource, targetLen);
    fLocalForm[targetLen] = 0;

    // Don't forget to delete our temp buffer
    delete [] tmpSource;
}
