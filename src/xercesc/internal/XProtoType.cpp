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
 * $Id$
 * $Log$
 * Revision 1.3  2003/12/24 15:24:13  cargilld
 * More updates to memory management so that the static memory manager.
 *
 * Revision 1.2  2003/12/17 00:18:34  cargilld
 * Update to memory management so that the static memory manager (one used to call Initialize) is only for static data.
 *
 * Revision 1.1  2003/09/18 18:31:24  peiyongz
 * OSU: Object Serialization Utilities
 *
 *
 */

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/internal/XProtoType.hpp>
#include <xercesc/internal/XSerializeEngine.hpp>
#include <xercesc/util/XMLString.hpp>

XERCES_CPP_NAMESPACE_BEGIN

/***
 *
 *  write the length of the class name
 *  write the class name
 *
 ***/
void XProtoType::store(XSerializeEngine& serEng) const
{
    int strLen = XMLString::stringLen((char*)fClassName);
	serEng << strLen;
	serEng.write(fClassName, strLen * sizeof(XMLByte));
}

/***
 *
 *  To verify that the content in the binary stream
 *  is the same as this class
 *
 ***/
void XProtoType::load(XSerializeEngine& serEng
                    , XMLByte* const    inName
                    , MemoryManager* const manager)
{
    if (!inName)
    {       
        ThrowXMLwithMemMgr(XSerializationException
               , XMLExcepts::XSer_ProtoType_Null_ClassName, manager);
    }

    // read and check class name length
    int      inNameLen = XMLString::stringLen((char*)inName); 
    int      classNameLen = 0;
    serEng >> classNameLen;

	if (classNameLen != inNameLen)
    {
        XMLCh value1[16];
        XMLCh value2[16];
        XMLString::binToText(inNameLen,    value1, 16, 10, manager);
        XMLString::binToText(classNameLen, value2, 16, 10, manager);

        ThrowXMLwithMemMgr2(XSerializationException
                , XMLExcepts::XSer_ProtoType_NameLen_Dif
                , value1
                , value2
                , manager);  
    }

    // read and check class name
	XMLByte  className[256];
    serEng.read(className, classNameLen*sizeof(XMLByte));
    className[classNameLen] = '\0';

    if ( !XMLString::equals((char*)className, (char*)inName))
    {
        //we don't have class name exceed this length in xerces
        XMLCh name1[256];
        XMLCh name2[256];
        XMLString::transcode((char*)inName,    name1, 255, manager);
        XMLString::transcode((char*)className, name2, 255, manager);

        ThrowXMLwithMemMgr2(XSerializationException
                , XMLExcepts::XSer_ProtoType_Name_Dif
                , name1
                , name2
                , manager);  
    }

    return;

}

XERCES_CPP_NAMESPACE_END

