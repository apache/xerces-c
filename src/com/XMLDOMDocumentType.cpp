
#include "stdafx.h"
#include "xml4com.h"
#include "XMLDOMDocumentType.h"
#include "XMLDOMNamedNodeMap.h"

// IXMLDOMDocumentType methods
STDMETHODIMP CXMLDOMDocumentType::get_name(BSTR  *pVal)
{
	ATLTRACE(_T("CXMLDOMDocumentType::get_name\n"));

	if (NULL == pVal)
		return E_POINTER;

	*pVal = NULL;

	try
	{
		*pVal = SysAllocString(documentType.getName().rawBuffer());
	}
	catch(...)
	{
		return E_FAIL;
	}
	

	return S_OK;
}

STDMETHODIMP CXMLDOMDocumentType::get_entities(IXMLDOMNamedNodeMap  **pVal)
{
	ATLTRACE(_T("CXMLDOMDocumentType::get_entities\n"));

	if (NULL == pVal)
		return E_POINTER;

	*pVal = NULL;

	CXMLDOMNamedNodeMapObj *pObj = NULL;
	HRESULT hr = CXMLDOMNamedNodeMapObj::CreateInstance(&pObj);
	if (S_OK != hr) 
		return hr;
	
	pObj->AddRef();
	pObj->SetOwnerDoc(m_pIXMLDOMDocument);

	try
	{
		pObj->m_container = documentType.getEntities();
	}
	catch(...)
	{
		pObj->Release(); 
		return E_FAIL;
	}
	
	hr = pObj->QueryInterface(IID_IXMLDOMNamedNodeMap, reinterpret_cast<LPVOID*> (pVal));
	if (S_OK != hr) 
		*pVal = NULL;

	pObj->Release();
	return hr;
}

STDMETHODIMP CXMLDOMDocumentType::get_notations(IXMLDOMNamedNodeMap  **pVal)
{
	ATLTRACE(_T("CXMLDOMDocumentType::get_notations\n"));

	if (NULL == pVal)
		return E_POINTER;

	*pVal = NULL;

	CXMLDOMNamedNodeMapObj *pObj = NULL;
	HRESULT hr = CXMLDOMNamedNodeMapObj::CreateInstance(&pObj);
	if (S_OK != hr) 
		return hr;
	
	pObj->AddRef();
	pObj->SetOwnerDoc(m_pIXMLDOMDocument);

	try
	{
		pObj->m_container = documentType.getNotations();
	}
	catch(...)
	{
		pObj->Release(); 
		return E_FAIL;
	}
	
	hr = pObj->QueryInterface(IID_IXMLDOMNamedNodeMap, reinterpret_cast<LPVOID*> (pVal));
	if (S_OK != hr) 
		*pVal = NULL;

	pObj->Release();
	return hr;

}