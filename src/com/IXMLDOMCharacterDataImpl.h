#ifndef ___ixmldomcharacterdataimpl_h___
#define ___ixmldomcharacterdataimpl_h___

#include "IXMLDOMNodeImpl.h"

template <class T, const IID* piid, const GUID* plibid = &CComModule::m_libid, WORD wMajor = 1,
WORD wMinor = 0, class tihclass = CComTypeInfoHolder>
class ATL_NO_VTABLE IXMLDOMCharacterDataImpl: public IXMLDOMNodeImpl<T,piid,plibid,wMajor,wMinor,tihclass>
{
public:

	virtual DOM_CharacterData& get_DOM_CharacterData() = 0; 
	virtual DOM_Node& get_DOM_Node() { return get_DOM_CharacterData(); } 

	// IXMLDOMCharacterData 

STDMETHOD(get_data)(BSTR  *pVal)
{
	ATLTRACE(_T("IXMLDOMCharacterDataImpl::get_data\n"));

	if (NULL == pVal)
		return E_POINTER;

	*pVal = NULL;
	
	try
	{
		*pVal = SysAllocString(get_DOM_CharacterData().getData().rawBuffer());
	}
	catch(...)
	{
		return E_FAIL;
	}
	

	return S_OK;
}

STDMETHOD(put_data)(BSTR newVal)
{
	ATLTRACE(_T("IXMLDOMCharacterDataImpl::put_data\n"));

	try
	{
		get_DOM_CharacterData().setData(newVal);
	}
	catch(...)
	{
		return E_FAIL;
	}
	

	return S_OK;
}

STDMETHOD(get_length)(long  *pVal)
{
	ATLTRACE(_T("IXMLDOMCharacterDataImpl::get_length\n"));

	if (NULL == pVal)
		return E_POINTER;

	*pVal = 0;

	try
	{
		*pVal = get_DOM_CharacterData().getLength();
	}
	catch(...)
	{
		return E_FAIL;
	}

	return S_OK;
}

STDMETHOD(substringData)(long offset, long count, BSTR  *data)
{
	ATLTRACE(_T("IXMLDOMCharacterDataImpl::substringData\n"));

	if (NULL == data)
		return E_POINTER;

	*data = NULL;

	try
	{
		*data = SysAllocString(get_DOM_CharacterData().substringData(offset, count).rawBuffer());
	}
	catch(...)
	{
		return E_FAIL;
	}
	
	return S_OK;
}

STDMETHOD(appendData)(BSTR data)
{
	ATLTRACE(_T("IXMLDOMCharacterDataImpl::appendData\n"));

	try
	{
		get_DOM_CharacterData().appendData(data);
	}
	catch(...)
	{
		return E_FAIL;
	}
	

	return S_OK;
}

STDMETHOD(insertData)(long offset, BSTR data)
{
	ATLTRACE(_T("IXMLDOMCharacterDataImpl::insertData\n"));

	try
	{
		get_DOM_CharacterData().insertData(offset, data);
	}
	catch(...)
	{
		return E_FAIL;
	}
	

	return S_OK;
}

STDMETHOD(deleteData)(long offset, long count)
{
	ATLTRACE(_T("IXMLDOMCharacterDataImpl::deleteData\n"));

	try
	{
		get_DOM_CharacterData().deleteData(offset, count);
	}
	catch(...)
	{
		return E_FAIL;
	}
	

	return S_OK;
}

STDMETHOD(replaceData)(long offset, long count, BSTR data)
{
	ATLTRACE(_T("IXMLDOMCharacterDataImpl::replaceData\n"));

	try
	{
		get_DOM_CharacterData().replaceData(offset, count, data);
	}
	catch(...)
	{
		return E_FAIL;
	}
	
	return S_OK;
}

};

#endif // ___ixmldomcharacterdataimpl_h___