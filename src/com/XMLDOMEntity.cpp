
#include "stdafx.h"
#include "xml4com.h"
#include "XMLDOMEntity.h"

// IXMLDOMEntity methods
STDMETHODIMP CXMLDOMEntity::get_publicId(VARIANT  *pVal)
{
	ATLTRACE(_T("CXMLDOMEntity::get_publicId\n"));

	if (NULL == pVal)
		return E_POINTER;

	::VariantInit(pVal);

	try
	{
		V_VT(pVal)   = VT_BSTR;
		V_BSTR(pVal) = SysAllocString(entity.getPublicId().rawBuffer());
	}
	catch(...)
	{
		return E_FAIL;
	}
	
	return S_OK;
}

STDMETHODIMP CXMLDOMEntity::get_systemId(VARIANT  *pVal)
{
	ATLTRACE(_T("CXMLDOMEntity::get_systemId\n"));

	if (NULL == pVal)
		return E_POINTER;

	::VariantInit(pVal);

	try
	{
		V_VT(pVal)   = VT_BSTR;
		V_BSTR(pVal) = SysAllocString(entity.getSystemId().rawBuffer());
	}
	catch(...)
	{
		return E_FAIL;
	}
	
	return S_OK;
}

STDMETHODIMP CXMLDOMEntity::get_notationName(BSTR  *pVal)
{
	ATLTRACE(_T("CXMLDOMEntity::get_notationName\n"));

	if (NULL == pVal)
		return E_POINTER;

	*pVal = NULL; 

	try
	{
		*pVal = SysAllocString(entity.getNotationName().rawBuffer());
	}
	catch(...)
	{
		return E_FAIL;
	}

	return S_OK;
}
