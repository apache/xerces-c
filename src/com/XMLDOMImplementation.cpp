
#include "stdafx.h"
#include "xml4com.h"
#include "XMLDOMImplementation.h"

STDMETHODIMP CXMLDOMImplementation::hasFeature(BSTR feature, BSTR ver, VARIANT_BOOL  *pVal)
{
	ATLTRACE(_T("CXMLDOMImplementation::hasFeature\n"));

	if (NULL == pVal)
		return E_POINTER;

	*pVal = VARIANT_FALSE;

	try
	{
		*pVal = (implementation.hasFeature(feature, ver) ? VARIANT_TRUE : VARIANT_FALSE);
	}
	catch(...)
	{
		return E_FAIL;
	}
	

	return S_OK;
}