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
 * $Id$
 */

#include "stdafx.h"
#include "xml4com.h"
#include "XMLDOMAttribute.h"

// IXMLDOMAttribute methods
STDMETHODIMP CXMLDOMAttribute::get_name(BSTR  *pVal)
{
	ATLTRACE(_T("CXMLDOMAttribute::get_name\n"));

	if (NULL == pVal)
		return E_POINTER;

	try
	{
		DOMString val = attr.getName();
		*pVal = SysAllocStringLen(val.rawBuffer(),val.length());
	}
	catch(DOM_DOMException& ex) {
		return MakeHRESULT(ex);
	}
	catch(...)
	{
		return E_FAIL;
	}
	

	return S_OK;
}

STDMETHODIMP CXMLDOMAttribute::get_value(VARIANT  *pVal)
{
	ATLTRACE(_T("CXMLDOMAttribute::get_value\n"));

	if (NULL == pVal)
		return E_POINTER;

	::VariantInit(pVal);

	try
	{
		DOMString val = attr.getValue();
		V_VT(pVal)   = VT_BSTR;
		V_BSTR(pVal) = SysAllocStringLen(val.rawBuffer(),val.length());
	}
	catch(DOM_DOMException& ex) {
		return MakeHRESULT(ex);
	}
	catch(...)
	{
		return E_FAIL;
	}


	return S_OK;
}

STDMETHODIMP CXMLDOMAttribute::put_value(VARIANT newVal)
{
	ATLTRACE(_T("CXMLDOMAttribute::put_value\n"));

	try
	{
		if(V_VT(&newVal) == VT_BSTR) {
			attr.setValue(V_BSTR(&newVal));
		}
		else {
			attr.setValue((BSTR) (_bstr_t) newVal);
		}

		attr.setValue(V_BSTR(&newVal));
	}
	catch(DOM_DOMException& ex) {
		return MakeHRESULT(ex);
	}
	catch(...)
	{
		return E_FAIL;
	}
	
	return S_OK;
}

	//  IXMLDOMNode method
STDMETHODIMP CXMLDOMAttribute::get_specified(VARIANT_BOOL  *pVal)
{
	ATLTRACE(_T("CXMLDOMAttribute::get_specified\n"));

	try
	{
		*pVal = attr.getSpecified() ? VARIANT_TRUE : VARIANT_FALSE;
	}
	catch(DOM_DOMException& ex) {
		return MakeHRESULT(ex);
	}
	catch(...)
	{
		return E_FAIL;
	}
	
	return S_OK;
}


STDMETHODIMP CXMLDOMAttribute::get_nodeValue(VARIANT  *pVal)
{
	ATLTRACE(_T("CXMLDOMAttribute::get_nodeValue\n"));

	if (NULL == pVal)
		return E_POINTER;

	::VariantInit(pVal);

	try
	{
		DOMString val = attr.getValue();
		V_VT(pVal)   = VT_BSTR;
		V_BSTR(pVal) = SysAllocStringLen(val.rawBuffer(),val.length());
	}
	catch(DOM_DOMException& ex) {
		return MakeHRESULT(ex);
	}
	catch(...)
	{
		return E_FAIL;
	}


	return S_OK;
}


