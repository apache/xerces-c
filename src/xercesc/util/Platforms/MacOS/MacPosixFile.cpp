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

#include <xercesc/util/XercesDefs.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/Janitor.hpp>

#include <limits.h>
#include <stdio.h>

#include <xercesc/util/Platforms/MacOS/MacPosixFile.hpp>


XERCES_CPP_NAMESPACE_BEGIN

//----------------------------------------------------------------------------
// XMLMacPosixFile methods
//----------------------------------------------------------------------------

XMLMacPosixFile::XMLMacPosixFile()
  : mFile(NULL)
{
}


XMLMacPosixFile::~XMLMacPosixFile()
{
	if (mFile)
		close();
}


unsigned int
XMLMacPosixFile::currPos()
{
    if (!mFile)
		ThrowXML(XMLPlatformUtilsException, XMLExcepts::CPtr_PointerIsZero);
		 
    long curPos = ftell(mFile);
	
    if (curPos == -1)
        ThrowXML(XMLPlatformUtilsException, XMLExcepts::File_CouldNotGetSize);

    return (unsigned int)curPos;
}


void
XMLMacPosixFile::close()
{
    if (!mFile)
		ThrowXML(XMLPlatformUtilsException, XMLExcepts::CPtr_PointerIsZero);
		
    if (fclose(mFile))
        ThrowXML(XMLPlatformUtilsException, XMLExcepts::File_CouldNotCloseFile);
		
	mFile = NULL;
}


unsigned int
XMLMacPosixFile::size()
{
    if (mFile == NULL)
		ThrowXML(XMLPlatformUtilsException, XMLExcepts::CPtr_PointerIsZero);
		
    // Get the current position
    long curPos = ftell(mFile);
    if (curPos == -1)
        ThrowXML(XMLPlatformUtilsException, XMLExcepts::File_CouldNotGetCurPos);

    // Seek to the end and save that value for return
    if (fseek(mFile, 0, SEEK_END))
        ThrowXML(XMLPlatformUtilsException, XMLExcepts::File_CouldNotSeekToEnd);

    long retVal = ftell(mFile);
    if (retVal == -1)
        ThrowXML(XMLPlatformUtilsException, XMLExcepts::File_CouldNotSeekToEnd);

    // And put the pointer back
    if (fseek(mFile, curPos, SEEK_SET))
        ThrowXML(XMLPlatformUtilsException, XMLExcepts::File_CouldNotSeekToPos);

    return (unsigned int)retVal;
}


bool
XMLMacPosixFile::open(const XMLCh* const path, bool toWrite)
{
    if (!path)
		ThrowXML(XMLPlatformUtilsException, XMLExcepts::CPtr_PointerIsZero);

	//	Transcode the unicode path to UTF8, which is what the Mac posix routines want
	char tmpPath[kMaxMacStaticPathChars];
	std::size_t len = TranscodeUniCharsToUTF8(path, tmpPath, XMLString::stringLen(path), kMaxMacStaticPathChars-1);
	tmpPath[len] = 0;
	
	//	Call through to the char version to do the work
	return open(tmpPath, toWrite);
}


bool
XMLMacPosixFile::open(const char* const path, bool toWrite)
{
    if (!path)
		ThrowXML(XMLPlatformUtilsException, XMLExcepts::CPtr_PointerIsZero);

	const char* perms = (toWrite) ? "w" : "r";
	mFile = fopen(path, perms);
	
	return (mFile != NULL);
}


unsigned int
XMLMacPosixFile::read(const unsigned int byteCount, XMLByte* const buffer)
{
    if (!mFile || !buffer)
		ThrowXML(XMLPlatformUtilsException, XMLExcepts::CPtr_PointerIsZero);
		
    size_t bytesRead = 0;
	if (byteCount > 0)
	{
    	bytesRead = fread((void*)buffer, 1, byteCount, mFile);

		if (ferror(mFile))
			ThrowXML(XMLPlatformUtilsException, XMLExcepts::File_CouldNotReadFromFile);
	}
	
    return (unsigned int)bytesRead;
}


void
XMLMacPosixFile::write(long byteCount, const XMLByte* buffer)
{
    if (!mFile || !buffer)
		ThrowXML(XMLPlatformUtilsException, XMLExcepts::CPtr_PointerIsZero);

    while (byteCount > 0)
    {
        size_t bytesWritten = fwrite(buffer, sizeof(XMLByte), byteCount, mFile);

        if (ferror(mFile))
			ThrowXML(XMLPlatformUtilsException, XMLExcepts::File_CouldNotWriteToFile);

		buffer		+= bytesWritten;
		byteCount	-= bytesWritten;
    }
}


void
XMLMacPosixFile::reset()
{
    if (!mFile)
		ThrowXML(XMLPlatformUtilsException, XMLExcepts::CPtr_PointerIsZero);
		
    // Seek to the start of the file
    if (fseek(mFile, 0, SEEK_SET))
        ThrowXML(XMLPlatformUtilsException, XMLExcepts::File_CouldNotResetFile);
}


XERCES_CPP_NAMESPACE_END
