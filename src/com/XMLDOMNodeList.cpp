
#include "stdafx.h"
#include "xml4com.h"
#include "XMLDOMNodeList.h"
#include "XMLDOMUtil.h"

typedef CComEnumOnSTL<IEnumVARIANT, &IID_IEnumVARIANT, VARIANT, _Copy<VARIANT>,NodeContainerImpl<DOM_NodeList> >
		CComEnumUnknownOnNodeContainer;

STDMETHODIMP CXMLDOMNodeList::get_item(long index, IXMLDOMNode  **pVal)
{
	ATLTRACE(_T("CXMLDOMNodeList::get_item\n"));

	if (NULL == pVal)
		return E_POINTER;

	*pVal = NULL;
	HRESULT hr = S_OK;

	try
	{
		if (m_container == 0 || index < 0)
			return E_INVALIDARG;

		long length = m_container.getLength(); 
		if (index >= length)
			return E_INVALIDARG;

		hr = wrapNode(m_pIXMLDOMDocument,m_container.item(index),IID_IXMLDOMNode, reinterpret_cast<LPVOID *> (pVal));
	}
	catch(...)
	{
		return E_FAIL;
	}
	
	return hr;
}

STDMETHODIMP CXMLDOMNodeList::get_length(long  *pVal)
{
	ATLTRACE(_T("CXMLDOMNodeList::get_length\n"));

	if (NULL == pVal)
		return E_POINTER;

	*pVal = 0;

	if (m_container == 0)
		return S_OK;

	try
	{
		*pVal = m_container.getLength();
	}
	catch(...)
	{
		return E_FAIL;
	}
	
	return S_OK;
}

STDMETHODIMP CXMLDOMNodeList::nextNode(IXMLDOMNode  **pVal)
{
	ATLTRACE(_T("CXMLDOMNodeList::nextNode\n"));

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
		hr = wrapNode(m_pIXMLDOMDocument,m_container.item(m_NextNodeIndex),IID_IXMLDOMNode, reinterpret_cast<LPVOID *> (pVal));
	}
	catch(...)
	{
		return E_FAIL;
	}

	++m_NextNodeIndex;
	
	return hr;
}

STDMETHODIMP CXMLDOMNodeList::reset()
{
	ATLTRACE(_T("CXMLDOMNodeList::reset\n"));
	
	m_NextNodeIndex = 0;
	
	return S_OK;
}

STDMETHODIMP CXMLDOMNodeList::get__newEnum(IUnknown  **pVal)
{
	ATLTRACE(_T("CXMLDOMNodeList::get__newEnum\n"));

	if (NULL == pVal)
		return E_POINTER;

	*pVal = NULL;

	CComObject<CComEnumUnknownOnNodeContainer> *pe = NULL;
	HRESULT hr = CComObject<CComEnumUnknownOnNodeContainer>::CreateInstance(&pe);
	if (S_OK != hr)
		return hr;

	pe->AddRef();

	hr = pe->Init(GetUnknown(),*this);
	if (S_OK == hr)
		hr = pe->QueryInterface(pVal);

	pe->Release();

	return hr;
}
