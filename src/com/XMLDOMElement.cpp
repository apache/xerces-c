
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

	try
	{
		V_VT(pVal)   = VT_BSTR;
		V_BSTR(pVal) = SysAllocString(element.getAttribute(name).rawBuffer());
	}
	catch(...)
	{
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

	*attr = NULL;

	CXMLDOMAttributeObj *pObj = NULL;
	HRESULT hr = CXMLDOMAttributeObj::CreateInstance(&pObj);
	if (S_OK != hr) 
		return hr;
	
	pObj->AddRef();
	pObj->SetOwnerDoc(m_pIXMLDOMDocument);

	try
	{
		pObj->attr = element.getAttributeNode(name);
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

	*attributeNode = NULL;

	CXMLDOMAttributeObj *pObj = NULL;
	HRESULT hr = CXMLDOMAttributeObj::CreateInstance(&pObj);
	if (S_OK != hr) 
		return hr;
	
	pObj->AddRef();
	pObj->SetOwnerDoc(m_pIXMLDOMDocument);

	try
	{
		pObj->attr = element.setAttributeNode(((CXMLDOMAttribute*) attr)->attr);
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
		pObj->attr = element.removeAttributeNode(((CXMLDOMAttribute*) attr)->attr);
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
