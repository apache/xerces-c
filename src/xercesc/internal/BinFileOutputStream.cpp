/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 2003 The Apache Software Foundation.  All rights
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
 * Revision 1.1  2003/09/18 18:39:12  peiyongz
 * Binary File Output Stream:
 *
 * $Id$
 *
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/internal/BinFileOutputStream.hpp>
#include <xercesc/util/Janitor.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLExceptMsgs.hpp>
#include <xercesc/util/XMLString.hpp>

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  BinFileOutputStream: Constructors and Destructor
// ---------------------------------------------------------------------------
BinFileOutputStream::~BinFileOutputStream()
{
    if (fSource)
        XMLPlatformUtils::closeFile(fSource);
}

BinFileOutputStream::BinFileOutputStream(const XMLCh*   const fileName)

:fSource(0)
{
    fSource = XMLPlatformUtils::openFileToWrite(fileName);
}

BinFileOutputStream::BinFileOutputStream(const char*    const fileName
                                       , MemoryManager* const manager)
:fSource(0)
{
    // Transcode the file name and put a janitor on the temp buffer
    XMLCh* realName = XMLString::transcode(fileName, manager);
    ArrayJanitor<XMLCh> janName(realName, manager);

    // Try to open the file
    fSource = XMLPlatformUtils::openFileToWrite(realName);
}

// ---------------------------------------------------------------------------
//  BinFileOutputStream: Getter methods
// ---------------------------------------------------------------------------
unsigned int BinFileOutputStream::getSize() const
{
    return XMLPlatformUtils::fileSize(fSource);
}


// ---------------------------------------------------------------------------
//  BinFileOutputStream: Stream management methods
// ---------------------------------------------------------------------------
void BinFileOutputStream::reset()
{
    XMLPlatformUtils::resetFile(fSource);
}


// ---------------------------------------------------------------------------
//  BinFileOutputStream: Implementation of the input stream interface
// ---------------------------------------------------------------------------
unsigned int BinFileOutputStream::curPos() const
{
    return XMLPlatformUtils::curFilePos(fSource);
}


void BinFileOutputStream::writeBytes(       XMLByte* const  toGo
                                    , const unsigned int    maxToWrite)
{
    //
    //  Write up to the maximum bytes requested. 
    //  

    XMLPlatformUtils::writeBufferToFile(fSource, maxToWrite, toGo);
}

XERCES_CPP_NAMESPACE_END
