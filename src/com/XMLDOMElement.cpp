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
 * $Log$
 * Revision 1.4  2000/07/07 00:14:12  jpolast
 * bug fix
 *
 * Revision 1.3  2000/06/19 20:05:58  rahulj
 * Changes for increased conformance and stability. Submitted by
 * Curt.Arnold@hyprotech.com. Verified by Joe Polastre.
 *
 * Revision 1.2  2000/03/30 02:00:11  abagchi
 * Initial checkin of working code with Copyright Notice
 *
 */

#include "stdafx.h"
#include "xml4com.h"
#include "XMLDOMElement.h"
#include "XMLDOMAttribute.h"
#include "XMLDOMNodeList.h"

// IXMLDOMElement methods
STDMETHODIMP CXMLDOMElement::get_tagName(BSTR  *pVal)
{
	ATLTRACE(_T("CXMLDOMElement::get_tagName\n"));

	if (NULL == pVal)
		return E_POINTER;

	*pVal = NULL;

	try
	{
		*pVal = SysAllocString(element.getTagName().rawBuffer());
	}
	catch(...)
	{
		return E_FAIL;
	}
	
	return S_OK;
}

STDMETHODIMP CXMLDOMElement::getAttribute(BSTR name, VARIANT  *pVal)
{
	ATLTRACE(_T("CXMLDOMElement::getAttribute\n"));

	if (NULL == pVal)
		return E_POINTER;

	::VariantInit(pVal);
	V_VT(pVal) = VT_EMPTY;

	DOMString a = element.getAttribute(name);

	try {
		V_VT(pVal)   = VT_BSTR;
		V_BSTR(pVal) = SysAllocString(a.rawBuffer());
	}
	catch(...) {
		return E_FAIL;
	}
	
	return S_OK;
}

STDMETHODIMP CXMLDOMElement::setAttribute(BSTR name, VARIANT value)
{
	ATLTRACE(_T("CXMLDOMElement::setAttribute\n"));

	try
	{
		if (V_VT(&value) != VT_BSTR)
			return E_INVALIDARG;

		element.setAttribute(name, value.bstrVal);
	}
	catch(...)
	{
		return E_FAIL;
	}
	

	return S_OK;
}

STDMETHODIMP CXMLDOMElement::removeAttribute(BSTR name)
{
	ATLTRACE(_T("CXMLDOMElement::removeAttribute\n"));

	try
	{
		element.removeAttribute(name);
	}
	catch(...)
	{
		return E_FAIL;
	}
	
	return S_OK;
}

STDMETHODIMP CXMLDOMElement::getAttributeNode(BSTR name, IXMLDOMAttribute  **attr)
{
	ATLTRACE(_T("CXMLDOMElement::getAttributeNode\n"));

	if (NULL == attr)
		return E_POINTER;

	if(*attr) (*attr)->Release();
	*attr = NULL;
	DOM_Attr attrNode(element.getAttributeNode(name));
	if(attrNode.isNull())
		return S_OK;
	

	CXMLDOMAttributeObj *pObj = NULL;
	HRESULT hr = CXMLDOMAttributeObj::CreateInstance(&pObj);
	if (S_OK != hr) 
		return hr;

	pObj->AddRef();
	pObj->SetOwnerDoc(m_pIXMLDOMDocument);

	try
	{
		pObj->attr = attrNode;
	}
	catch(...)
	{
		pObj->Release(); 
		return E_FAIL;
	}
	
	hr = pObj->QueryInterface(IID_IXMLDOMAttribute, reinterpret_cast<LPVOID*> (attr));
	if (S_OK != hr) 
		*attr = NULL;

	pObj->Release();
	return hr;
}

STDMETHODIMP CXMLDOMElement::setAttributeNode(IXMLDOMAttribute  *attr, IXMLDOMAttribute  **attributeNode)
{
	ATLTRACE(_T("CXMLDOMElement::setAttributeNode\n"));

	if (NULL == attr || NULL == attributeNode)
		return E_POINTER;

	if(*attributeNode) (*attributeNode)->Release();
	*attributeNode = NULL;

	long id = 0;
	IIBMXMLDOMNodeIdentity* nodeID = NULL;
	if(SUCCEEDED(attr->QueryInterface(IID_IIBMXMLDOMNodeIdentity,(void**) &nodeID))) {
		nodeID->get_NodeId(&id);
		nodeID->Release();
	}

	DOM_Attr attrNode;
	try {
		attrNode = (element.setAttributeNode(*((DOM_Attr*) id)));
		if(attrNode.isNull())
			return S_OK;
	}
	catch(...) {
		return E_FAIL;
	}



	CXMLDOMAttributeObj *pObj = NULL;
	HRESULT hr = CXMLDOMAttributeObj::CreateInstance(&pObj);
	if (S_OK != hr) 
		return hr;
	
	pObj->AddRef();
	pObj->SetOwnerDoc(m_pIXMLDOMDocument);

	try
	{
		pObj->attr = element.setAttributeNode(attrNode);
	}
	catch(...)
	{
		pObj->Release(); 
		return E_FAIL;
	}
	
	hr = pObj->QueryInterface(IID_IXMLDOMAttribute, reinterpret_cast<LPVOID*> (attributeNode));
	if (S_OK != hr) 
		*attributeNode = NULL;

	pObj->Release();
	return hr;
}

STDMETHODIMP CXMLDOMElement::removeAttributeNode(IXMLDOMAttribute  *attr, IXMLDOMAttribute  * *attributeNode)
{
	ATLTRACE(_T("CXMLDOMElement::removeAttributeNode\n"));

	if (NULL == attr || NULL == attributeNode)
		return E_POINTER;

	*attributeNode = NULL;

	CXMLDOMAttributeObj *pObj = NULL;
	HRESULT hr = CXMLDOMAttributeObj::CreateInstance(&pObj);
	if (S_OK != hr) 
		return hr;
	
	pObj->AddRef();
	pObj->SetOwnerDoc(m_pIXMLDOMDocument);

	try
	{
		long id = 0;
		IIBMXMLDOMNodeIdentity* nodeID = NULL;
		if(SUCCEEDED(attr->QueryInterface(IID_IIBMXMLDOMNodeIdentity,(void**) &nodeID))) {
			nodeID->get_NodeId(&id);
			nodeID->Release();
		}
		pObj->attr = element.removeAttributeNode(*((DOM_Attr*) id));
	}
	catch(...)
	{
		pObj->Release(); 
		return E_FAIL;
	}
	
	hr = pObj->QueryInterface(IID_IXMLDOMAttribute, reinterpret_cast<LPVOID*> (attributeNode));
	if (S_OK != hr) 
		*attributeNode = NULL;

	pObj->Release();
	return hr;
}

STDMETHODIMP CXMLDOMElement::getElementsByTagName(BSTR tagName, IXMLDOMNodeList  **pVal)
{
	ATLTRACE(_T("CXMLDOMElement::getElementsByTagName\n"));

	if (NULL == pVal)
		return E_POINTER;

	*pVal = NULL;

	CXMLDOMNodeListObj *pObj = NULL;
	HRESULT hr = CXMLDOMNodeListObj::CreateInstance(&pObj);
	if (S_OK != hr) 
		return hr;
	
	pObj->AddRef();
	pObj->SetOwnerDoc(m_pIXMLDOMDocument);

	try
	{
		pObj->m_container = element.getElementsByTagName(tagName);
	}
	catch(...)
	{
		pObj->Release(); 
		return E_FAIL;
	}
	
	hr = pObj->QueryInterface(IID_IXMLDOMNodeList, reinterpret_cast<LPVOID*> (pVal));
	if (S_OK != hr) 
		*pVal = NULL;

	pObj->Release();
	return hr;
}

STDMETHODIMP CXMLDOMElement::normalize(void)
{
	ATLTRACE(_T("CXMLDOMElement::normalize\n"));

	try
	{
		element.normalize();
	}
	catch(...)
	{
		return E_FAIL;
	}	

	return S_OK;
}
