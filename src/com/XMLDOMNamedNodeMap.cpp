
#include "stdafx.h"
#include "xml4com.h"
#include "XMLDOMNamedNodeMap.h"
#include "XMLDOMUtil.h"

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
		hr = wrapNode(m_pIXMLDOMDocument,m_container.getNamedItem(name),IID_IXMLDOMNode, reinterpret_cast<LPVOID *> (pVal));
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
		m_container.setNamedItem(*pNewItemNode);
		hr = wrapNode(m_pIXMLDOMDocument,m_container.getNamedItem(name),IID_IXMLDOMNode, reinterpret_cast<LPVOID *> (pVal));
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
		hr = wrapNode(m_pIXMLDOMDocument,m_container.removeNamedItem(name),IID_IXMLDOMNode, reinterpret_cast<LPVOID *> (pVal));
	}
	catch(...)
	{
		return E_FAIL;
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
		hr = wrapNode(m_pIXMLDOMDocument,m_container.getNamedItemNS(namespaceURI,baseName),IID_IXMLDOMNode, reinterpret_cast<LPVOID *> (pVal));
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
		hr = wrapNode(m_pIXMLDOMDocument,m_container.removeNamedItemNS(namespaceURI,baseName),IID_IXMLDOMNode, reinterpret_cast<LPVOID *> (pVal));
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
		hr = wrapNode(m_pIXMLDOMDocument,m_container.item(m_NextNodeIndex),IID_IXMLDOMNode, reinterpret_cast<LPVOID *> (pVal));
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