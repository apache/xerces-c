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
#include "XMLDOMNamedNodeMap.h"
#include "XMLDOMUtil.h"
#include "IXMLDOMNodeImpl.h"

typedef CComEnumOnSTL<IEnumVARIANT, &IID_IEnumVARIANT, VARIANT, _Copy<VARIANT>, NodeContainerImpl<DOM_NamedNodeMap> >
		CComEnumUnknownOnNamedNodeContainer;

STDMETHODIMP CXMLDOMNamedNodeMap::getNamedItem(BSTR name, IXMLDOMNode  **pVal)
{
	ATLTRACE(_T("CXMLDOMNamedNodeMap::getNamedItem\n"));

	if (NULL == pVal)
		return E_POINTER;

	*pVal = NULL;
	HRESULT hr = S_OK;

	if (m_container == 0)
		return S_OK;

	try
	{
		DOM_Node n = m_container.getNamedItem(name);
		if(!n.isNull())
			hr = wrapNode(m_pIXMLDOMDocument,n,IID_IXMLDOMNode, reinterpret_cast<LPVOID *> (pVal));
	}
	catch(DOM_DOMException& ex)
	{
		return MakeHRESULT(ex);
	}
	catch(...)
	{
		return E_FAIL;
	}
	
	return hr;
}

STDMETHODIMP CXMLDOMNamedNodeMap::setNamedItem(IXMLDOMNode  *newItem, IXMLDOMNode  **pVal)
{
	ATLTRACE(_T("CXMLDOMNamedNodeMap::setNamedItem\n"));

	if (NULL == pVal)
		return E_POINTER;

	*pVal = NULL;

	if (m_container == 0)
		return S_OK;

	if (NULL == newItem)
		return E_INVALIDARG;

	CComQIPtr<IIBMXMLDOMNodeIdentity,&IID_IIBMXMLDOMNodeIdentity> pNewItem(newItem);
	if (!pNewItem)
		return E_NOINTERFACE;

	long id = 0;
	HRESULT hr = pNewItem->get_NodeId(&id);
	if (S_OK != hr)
		return hr;

	DOM_Node *pNewItemNode = reinterpret_cast<DOM_Node*> (id);
	if (NULL == pNewItemNode)
		return E_INVALIDARG;

	try
	{
		DOMString  name = pNewItemNode->getNodeName();
		//
		//  returns old node
		//
		DOM_Node n = m_container.setNamedItem(*pNewItemNode);
		if(!n.isNull())
			hr = wrapNode(m_pIXMLDOMDocument,n,IID_IXMLDOMNode, reinterpret_cast<LPVOID *> (pVal));
	}
	catch(DOM_DOMException& ex)
	{
		return MakeHRESULT(ex);
	}
	catch(...)
	{
		return E_FAIL;
	}

	return hr;
}

STDMETHODIMP CXMLDOMNamedNodeMap::removeNamedItem(BSTR name, IXMLDOMNode  **pVal)
{
	ATLTRACE(_T("CXMLDOMNamedNodeMap::removeNamedItem\n"));
	
	if (NULL == pVal)
		return E_POINTER;

	*pVal = NULL;
	HRESULT hr = S_OK;

	if (m_container == 0)
		return S_OK;

	try
	{
		DOM_Node n = m_container.removeNamedItem(name);
		if(!n.isNull())
			hr = wrapNode(m_pIXMLDOMDocument,n,IID_IXMLDOMNode, reinterpret_cast<LPVOID *> (pVal));
	}
	//
	//   if we had a failure, return success anyway
	//
	catch(...)
	{
	}
	
	return hr;
}

STDMETHODIMP CXMLDOMNamedNodeMap::get_item(long index, IXMLDOMNode  **pVal)
{
	ATLTRACE(_T("CXMLDOMNamedNodeMap::get_item\n"));

	if (NULL == pVal)
		return E_POINTER;

	*pVal = NULL;
	HRESULT hr = S_OK;
	
	try
	{
		if (m_container == 0 || index < 0)
			return E_INVALIDARG;

		long length =  m_container.getLength();
		if (index < length)
			hr = wrapNode(m_pIXMLDOMDocument,m_container.item(index),IID_IXMLDOMNode, reinterpret_cast<LPVOID *> (pVal));
	}
	catch(DOM_DOMException& ex)
	{
		return MakeHRESULT(ex);
	}
	catch(...)
	{
		return E_FAIL;
	}
	
	return hr;
}

STDMETHODIMP CXMLDOMNamedNodeMap::get_length(long  *pVal)
{
	ATLTRACE(_T("CXMLDOMNamedNodeMap::get_length\n"));

	if (NULL == pVal)
		return E_POINTER;

	*pVal = 0;

	if (m_container == 0)
		return S_OK;

	try
	{
		*pVal = m_container.getLength();
	}
	catch(DOM_DOMException& ex)
	{
		return MakeHRESULT(ex);
	}
	catch(...)
	{
		return E_FAIL;
	}
	
	return S_OK;
}

STDMETHODIMP CXMLDOMNamedNodeMap::getQualifiedItem(BSTR baseName, BSTR namespaceURI, IXMLDOMNode  **pVal)
{
	ATLTRACE(_T("CXMLDOMNamedNodeMap::getQualifiedItem\n"));

	if (NULL == pVal)
		return E_POINTER;

	*pVal = NULL;
	
	if (m_container == 0)
		return S_OK;
	
	HRESULT hr = S_OK;

	try
	{
		DOM_Node n = m_container.getNamedItemNS(namespaceURI,baseName);
		if(!n.isNull())
			hr = wrapNode(m_pIXMLDOMDocument,n,IID_IXMLDOMNode, reinterpret_cast<LPVOID *> (pVal));
	}
	catch(DOM_DOMException& ex)
	{
		return MakeHRESULT(ex);
	}
	catch(...)
	{
		return E_FAIL;
	}

	return S_OK;
}

STDMETHODIMP CXMLDOMNamedNodeMap::removeQualifiedItem(BSTR baseName, BSTR namespaceURI, IXMLDOMNode  **pVal)
{
	ATLTRACE(_T("CXMLDOMNamedNodeMap::removeQualifiedItem\n"));

	if (NULL == pVal)
		return E_POINTER;

	*pVal = NULL;
	
	if (m_container == 0)
		return S_OK;
	
	HRESULT hr = S_OK;

	try
	{
		DOM_Node n = m_container.removeNamedItemNS(namespaceURI,baseName);
		if(!n.isNull())
			hr = wrapNode(m_pIXMLDOMDocument,n,IID_IXMLDOMNode, reinterpret_cast<LPVOID *> (pVal));
	}
	catch(DOM_DOMException& ex)
	{
		return MakeHRESULT(ex);
	}
	catch(...)
	{
		return E_FAIL;
	}

	return S_OK;
}

STDMETHODIMP CXMLDOMNamedNodeMap::nextNode(IXMLDOMNode  **pVal)
{
	ATLTRACE(_T("CXMLDOMNamedNodeMap::nextNode\n"));
	if (NULL == pVal)
		return E_POINTER;

	*pVal = NULL;

	if (m_container == 0)
		return S_OK;
	
	int length = m_container.getLength();
	if (0 == length)
		return S_OK;

	if (m_NextNodeIndex >= length)
		return S_OK;
	
	HRESULT hr = S_OK;

	try
	{
		DOM_Node n = m_container.item(m_NextNodeIndex);
		if(!n.isNull())
			hr = wrapNode(m_pIXMLDOMDocument,n,IID_IXMLDOMNode, reinterpret_cast<LPVOID *> (pVal));
	}
	catch(DOM_DOMException& ex)
	{
		return MakeHRESULT(ex);
	}
	catch(...)
	{
		return E_FAIL;
	}

	++m_NextNodeIndex;
	
	return hr;
}

STDMETHODIMP CXMLDOMNamedNodeMap::reset()
{
	ATLTRACE(_T("CXMLDOMNamedNodeMap::reset\n"));

	m_NextNodeIndex = 0;
	
	return S_OK;
}

STDMETHODIMP CXMLDOMNamedNodeMap::get__newEnum(IUnknown  **pVal)
{
	ATLTRACE(_T("CXMLDOMNamedNodeMap::get__newEnum\n"));

	if (NULL == pVal)
		return E_POINTER;

	*pVal = NULL;

	CComObject<CComEnumUnknownOnNamedNodeContainer> *pe = NULL;
	HRESULT hr = CComObject<CComEnumUnknownOnNamedNodeContainer>::CreateInstance(&pe);
	if (S_OK != hr)
		return hr;

	pe->AddRef();

	hr = pe->Init(GetUnknown(),*this);
	if (S_OK == hr)
		hr = pe->QueryInterface(pVal);

	pe->Release();

	return hr;
}

HRESULT CXMLDOMNamedNodeMap::InterfaceSupportsErrorInfo(REFIID riid)
{
	if(riid == IID_IXMLDOMNamedNodeMap) return S_OK;
	return S_FALSE;
}