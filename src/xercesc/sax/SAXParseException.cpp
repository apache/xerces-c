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
 * $Log$
 * Revision 1.5  2003/08/13 15:43:24  knoaman
 * Use memory manager when creating SAX exceptions.
 *
 * Revision 1.4  2003/04/21 21:07:38  knoaman
 * Use XMLString::release to prepare for configurable memory manager.
 *
 * Revision 1.3  2002/11/04 14:56:26  tng
 * C++ Namespace Support.
 *
 * Revision 1.2  2002/05/27 18:33:07  tng
 * To get ready for 64 bit large file, use XMLSSize_t to represent line and column number.
 *
 * Revision 1.1.1.1  2002/02/01 22:22:08  peiyongz
 * sane_include
 *
 * Revision 1.4  2000/09/07 23:55:02  andyh
 * Fix SAXException assignment operator.  Now non-virtual, and
 * SAXParseException invokes base class operator.
 *
 * Revision 1.3  2000/03/02 19:54:35  roddey
 * This checkin includes many changes done while waiting for the
 * 1.1.0 code to be finished. I can't list them all here, but a list is
 * available elsewhere.
 *
 * Revision 1.2  2000/02/06 07:47:58  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.1.1.1  1999/11/09 01:07:47  twl
 * Initial checkin
 *
 * Revision 1.2  1999/11/08 20:45:02  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include    <xercesc/util/XMLString.hpp>
#include    <xercesc/sax/Locator.hpp>
#include    <xercesc/sax/SAXParseException.hpp>

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  SAXParseException: Constructors and Destructor
// ---------------------------------------------------------------------------
SAXParseException::SAXParseException(const  XMLCh* const    message
                                    , const Locator&        locator
                                    , MemoryManager* const  manager) :
    SAXException(message, manager)
    , fColumnNumber(locator.getColumnNumber())
    , fLineNumber(locator.getLineNumber())
    , fPublicId(XMLString::replicate(locator.getPublicId(), manager))
    , fSystemId(XMLString::replicate(locator.getSystemId(), manager))
{
}

SAXParseException::SAXParseException(const  XMLCh* const    message
                                    , const XMLCh* const    publicId
                                    , const XMLCh* const    systemId
                                    , const XMLSSize_t      lineNumber
                                    , const XMLSSize_t      columnNumber
                                    , MemoryManager* const  manager) :
    SAXException(message, manager)
    , fColumnNumber(columnNumber)
    , fLineNumber(lineNumber)
    , fPublicId(XMLString::replicate(publicId, manager))
    , fSystemId(XMLString::replicate(systemId, manager))
{
}

SAXParseException::SAXParseException(const SAXParseException& toCopy) :

    SAXException(toCopy)
    , fColumnNumber(toCopy.fColumnNumber)
    , fLineNumber(toCopy.fLineNumber)
    , fPublicId(0)
    , fSystemId(0)
{
    fPublicId = XMLString::replicate(toCopy.fPublicId, toCopy.fMemoryManager);
    fSystemId = XMLString::replicate(toCopy.fSystemId, toCopy.fMemoryManager);
}

SAXParseException::~SAXParseException()
{
    fMemoryManager->deallocate(fPublicId);//XMLString::release(&fPublicId);
    fMemoryManager->deallocate(fSystemId);//XMLString::release(&fSystemId);
}


// ---------------------------------------------------------------------------
//  SAXParseException: Public operators
// ---------------------------------------------------------------------------
SAXParseException&
SAXParseException::operator=(const SAXParseException& toAssign)
{
    if (this == &toAssign)
        return *this;

    fMemoryManager->deallocate(fPublicId);//XMLString::release(&fPublicId);
    fMemoryManager->deallocate(fSystemId);//XMLString::release(&fSystemId);

    this->SAXException::operator =(toAssign);
    fColumnNumber = toAssign.fColumnNumber;
    fLineNumber = toAssign.fLineNumber;

    fPublicId = XMLString::replicate(toAssign.fPublicId, fMemoryManager);
    fSystemId = XMLString::replicate(toAssign.fSystemId, fMemoryManager);

    return *this;
}


// ---------------------------------------------------------------------------
//  SAXParseException: Getter methods
// ---------------------------------------------------------------------------
const XMLCh* SAXParseException::getPublicId() const
{
    return fPublicId;
}

const XMLCh* SAXParseException::getSystemId() const
{
    return fSystemId;
}

XMLSSize_t SAXParseException::getLineNumber() const
{
    return fLineNumber;
}


XMLSSize_t SAXParseException::getColumnNumber() const
{
    return fColumnNumber;
}

XERCES_CPP_NAMESPACE_END

