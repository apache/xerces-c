
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
		*pVal = SysAllocString(attr.getName().rawBuffer());
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
		V_VT(pVal)   = VT_BSTR;
		V_BSTR(pVal) = SysAllocString(attr.getValue().rawBuffer());
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
		if (V_VT(&newVal) != VT_BSTR)
			return E_FAIL;

		attr.setValue(V_BSTR(&newVal));
	}
	catch(...)
	{
		return E_FAIL;
	}
	
	return S_OK;
}

