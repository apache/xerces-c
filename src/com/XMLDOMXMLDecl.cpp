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


#include "stdafx.h"
#include "xml4com.h"
#include "XMLDOMXMLDecl.h"

// IXMLDOMProcessingInstruction methods
STDMETHODIMP CXMLDOMXMLDecl::get_target(BSTR  *pVal)
{
	ATLTRACE(_T("CXMLDOMXMLDecl::get_target\n"));

	if (NULL == pVal)
		return E_POINTER;

	*pVal = NULL;

	try
	{
		*pVal = SysAllocString(OLESTR("xml"));
	}
	catch(...)
	{
		return E_FAIL;
	}

	return S_OK;
}

STDMETHODIMP CXMLDOMXMLDecl::get_data(BSTR  *pVal)
{
	ATLTRACE(_T("CXMLDOMXMLDecl::get_data\n"));

	if (NULL == pVal)
		return E_POINTER;

	*pVal = NULL;

	try
	{
		unsigned int len= 0;
		unsigned int totalLen = 0;
		DOMString version(xmlDecl.getVersion());
		len = version.length();
		if(len > 0) totalLen = len + 10;

		DOMString standalone(xmlDecl.getStandalone());
		len = standalone.length();
		if(len > 0) {
			if(totalLen > 0) 
				totalLen += len + 14;
			else
				totalLen = len + 13;
		}

		DOMString encoding(xmlDecl.getEncoding());
		len = encoding.length();
		if(len > 0) {
			if(totalLen > 0)
				totalLen += len + 13;
			else
				totalLen = len + 12;
		}

		*pVal = SysAllocStringLen(NULL,totalLen+1);
		**pVal = 0;

		len = version.length();
		totalLen = len;
		if(len > 0) {
			wcscpy(*pVal,OLESTR("version=\""));
			wcscat(*pVal,version.rawBuffer());
			wcscat(*pVal,OLESTR("\""));
		}

		len = standalone.length();
		if(len > 0) {
			if(totalLen > 0)
				wcscat(*pVal,OLESTR(" standalone=\""));
			else
				wcscat(*pVal,OLESTR("standalone=\""));
			wcscat(*pVal,standalone.rawBuffer());
			wcscat(*pVal,OLESTR("\""));
			totalLen += len;
		}

		len = encoding.length();
		if(len > 0) {
			if(totalLen > 0)
				wcscat(*pVal,OLESTR(" encoding=\""));
			else
				wcscat(*pVal,OLESTR("encoding=\""));
			wcscat(*pVal,encoding.rawBuffer());
			wcscat(*pVal,OLESTR("\""));
		}

	}
	catch(...)
	{
		return E_FAIL;
	}

	return S_OK;

}

STDMETHODIMP CXMLDOMXMLDecl::put_data(BSTR newVal)
{
	ATLTRACE(_T("CXMLDOMXMLDecl::put_data\n"));

	return E_NOTIMPL;
}
