
#include "stdafx.h"
#include "xml4com.h"
#include "XMLDOMProcessingInstruction.h"

// IXMLDOMProcessingInstruction methods
STDMETHODIMP CXMLDOMProcessingInstruction::get_target(BSTR  *pVal)
{
	ATLTRACE(_T("CXMLDOMProcessingInstruction::get_target\n"));

	if (NULL == pVal)
		return E_POINTER;

	*pVal = NULL;

	try
	{
		*pVal = SysAllocString(processingInstruction.getTarget().rawBuffer());
	}
	catch(...)
	{
		return E_FAIL;
	}

	return S_OK;
}

STDMETHODIMP CXMLDOMProcessingInstruction::get_data(BSTR  *pVal)
{
	ATLTRACE(_T("CXMLDOMProcessingInstruction::get_data\n"));

	if (NULL == pVal)
		return E_POINTER;

	*pVal = NULL;

	try
	{
		*pVal = SysAllocString(processingInstruction.getData().rawBuffer());
	}
	catch(...)
	{
		return E_FAIL;
	}

	return S_OK;

}

STDMETHODIMP CXMLDOMProcessingInstruction::put_data(BSTR newVal)
{
	ATLTRACE(_T("CXMLDOMProcessingInstruction::put_data\n"));

	try
	{
		processingInstruction.setData(newVal);
	}
	catch(...)
	{
		return E_FAIL;
	}
	
	return S_OK;
}
