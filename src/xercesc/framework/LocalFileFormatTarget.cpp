/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 2002 The Apache Software Foundation.  All rights
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
 * $Log$
 * Revision 1.4  2003/01/09 20:59:44  tng
 * [Bug 15928] Output with LocalFileFormatTarget fails silently.
 *
 * Revision 1.3  2002/11/27 18:09:25  tng
 * [Bug 13447] Performance: Using LocalFileFormatTarget with DOMWriter is very slow.
 *
 * Revision 1.2  2002/11/04 15:00:21  tng
 * C++ Namespace Support.
 *
 * Revision 1.1  2002/06/19 21:59:26  peiyongz
 * DOM3:DOMSave Interface support: LocalFileFormatTarget
 *
 *
 */

#include <xercesc/framework/LocalFileFormatTarget.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/IOException.hpp>
#include <string.h>

XERCES_CPP_NAMESPACE_BEGIN

LocalFileFormatTarget::LocalFileFormatTarget(const XMLCh* const fileName)
: fSource(0)
, fDataBuf(0)
, fIndex(0)
, fCapacity(1023)
{
    fSource = XMLPlatformUtils::openFileToWrite(fileName);

    if (!fSource)
        ThrowXML1(IOException, XMLExcepts::File_CouldNotOpenFile, fileName);

    // Buffer is one larger than capacity, to allow for zero term
    fDataBuf = new XMLByte[fCapacity+4];

    // Keep it null terminated
    fDataBuf[0] = XMLByte(0);

}

LocalFileFormatTarget::LocalFileFormatTarget(const char* const fileName)
: fSource(0)
, fDataBuf(0)
, fIndex(0)
, fCapacity(1023)
{
    fSource = XMLPlatformUtils::openFileToWrite(fileName);

    if (!fSource)
        ThrowXML1(IOException, XMLExcepts::File_CouldNotOpenFile, fileName);

    // Buffer is one larger than capacity, to allow for zero term
    fDataBuf = new XMLByte[fCapacity+4];

    // Keep it null terminated
    fDataBuf[0] = XMLByte(0);
}

LocalFileFormatTarget::~LocalFileFormatTarget()
{
    // flush remaining buffer before destroy
    flushBuffer();

    if (fSource)
        XMLPlatformUtils::closeFile(fSource);

    delete [] fDataBuf;
}

void LocalFileFormatTarget::writeChars(const XMLByte* const toWrite
                                     , const unsigned int   count
                                     , XMLFormatter * const        )
{
    if (count) {
        insureCapacity(count);
        memcpy(&fDataBuf[fIndex], toWrite, count * sizeof(XMLByte));
        fIndex += count;
    }

    return;
}

void LocalFileFormatTarget::flushBuffer()
{
    // Exception thrown in writeBufferToFile, if any, will be propagated to
    // the XMLFormatter and then to the DOMWriter, which may notify
    // application through DOMErrorHandler, if any.
    XMLPlatformUtils::writeBufferToFile(fSource, (long) fIndex, fDataBuf);
    fIndex = 0;
    fDataBuf[0] = 0;
    fDataBuf[fIndex + 1] = 0;
    fDataBuf[fIndex + 2] = 0;
    fDataBuf[fIndex + 3] = 0;
}

void LocalFileFormatTarget::insureCapacity(const unsigned int extraNeeded)
{
    // If we can handle it, do nothing yet
    if (fIndex + extraNeeded < fCapacity)
        return;

    // Oops, not enough room. Calc new capacity and allocate new buffer
    const unsigned int newCap = (unsigned int)((fIndex + extraNeeded) * 2);
    XMLByte* newBuf = new XMLByte[newCap+4];

    // Copy over the old stuff
    memcpy(newBuf, fDataBuf, fCapacity * sizeof(XMLByte) + 4);

    // Clean up old buffer and store new stuff
    delete [] fDataBuf;
    fDataBuf = newBuf;
    fCapacity = newCap;

    // flush the buffer too
    flushBuffer();
}

XERCES_CPP_NAMESPACE_END


