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
 * Revision 1.4  2001/10/19 18:52:04  tng
 * Since PParse can take any XML file as input file, it shouldn't hardcode to expect 16 elements.
 * Change it to work similar to SAXCount which just prints the number of elements, characters, attributes ... etc.
 * And other modification for consistent help display and return code across samples.
 *
 * Revision 1.3  2000/03/02 19:53:44  roddey
 * This checkin includes many changes done while waiting for the
 * 1.1.0 code to be finished. I can't list them all here, but a list is
 * available elsewhere.
 *
 * Revision 1.2  2000/02/06 07:47:21  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.1.1.1  1999/11/09 01:09:46  twl
 * Initial checkin
 *
 * Revision 1.4  1999/11/08 20:43:38  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */


#include    <sax/HandlerBase.hpp>

class PParseHandlers : public HandlerBase
{
public :
    // -----------------------------------------------------------------------
    //  Constructors
    // -----------------------------------------------------------------------
    PParseHandlers();
    ~PParseHandlers();

    // -----------------------------------------------------------------------
    //  Getter methods
    // -----------------------------------------------------------------------
    unsigned int getElementCount() const
    {
        return fElementCount;
    }

    unsigned int getAttrCount() const
    {
        return fAttrCount;
    }

    unsigned int getCharacterCount() const
    {
        return fCharacterCount;
    }

    bool getSawErrors() const
    {
        return fSawErrors;
    }

    unsigned int getSpaceCount() const
    {
        return fSpaceCount;
    }


    // -----------------------------------------------------------------------
    //  Handlers for the SAX DocumentHandler interface
    // -----------------------------------------------------------------------
    void startElement(const XMLCh* const name, AttributeList& attributes);
    void characters(const XMLCh* const chars, const unsigned int length);
    void ignorableWhitespace(const XMLCh* const chars, const unsigned int length);
    void resetDocument();


    // -----------------------------------------------------------------------
    //  Implementations of the SAX ErrorHandler interface
    // -----------------------------------------------------------------------
    void warning(const SAXParseException& exception);
    void error(const SAXParseException& exception);
    void fatalError(const SAXParseException& exception);


    private:
    // -----------------------------------------------------------------------
    //  Private data members
    //
    //  fAttrCount
    //  fCharacterCount
    //  fElementCount
    //  fSpaceCount
    //      These are just counters that are run upwards based on the input
    //      from the document handlers.
    //
    //  fSawErrors
    //      This is set by the error handlers, and is queryable later to
    //      see if any errors occured.
    // -----------------------------------------------------------------------
    unsigned int    fAttrCount;
    unsigned int    fCharacterCount;
    unsigned int    fElementCount;
    unsigned int    fSpaceCount;
    bool            fSawErrors;
};

