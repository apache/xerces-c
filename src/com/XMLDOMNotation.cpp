
#include "stdafx.h"
#include "xml4com.h"
#include "XMLDOMNotation.h"

STDMETHODIMP CXMLDOMNotation::get_publicId(VARIANT  *pVal)
{
	ATLTRACE(_T("CXMLDOMNotation::get_publicId\n"));

	if (NULL == pVal)
		return E_POINTER;

	::VariantInit(pVal);

	try
	{
		V_VT(pVal) = VT_BSTR;
		V_BSTR(pVal) = SysAllocString(notation.getPublicId().rawBuffer());
	}
	catch(...)
	{
		return E_FAIL;
	}
	

	return S_OK;
}

STDMETHODIMP CXMLDOMNotation::get_systemId(VARIANT  *pVal)
{
	ATLTRACE(_T("CXMLDOMNotation::get_systemId\n"));

	if (NULL == pVal)
		return E_POINTER;

	::VariantInit(pVal);

	try
	{
		V_VT(pVal) = VT_BSTR;
		V_BSTR(pVal) = SysAllocString(notation.getSystemId().rawBuffer());
	}
	catch(...)
	{
		return E_FAIL;
	}
	

	return S_OK;
}
