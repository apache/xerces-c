#ifndef ___ixmldomtextimpl_h___
#define ___ixmldomtextimpl_h___

#include "XMLDOMUtil.h"
#include "IXMLDOMCharacterDataImpl.h"

template <class T, const IID* piid, const GUID* plibid = &CComModule::m_libid, WORD wMajor = 1,
WORD wMinor = 0, class tihclass = CComTypeInfoHolder>
class ATL_NO_VTABLE IXMLDOMTextImpl: public IXMLDOMCharacterDataImpl<T,piid,plibid,wMajor,wMinor,tihclass>
{
public:

	virtual DOM_Text& get_DOM_Text() = 0; 
	virtual DOM_CharacterData& get_DOM_CharacterData() { return get_DOM_Text(); } 

	// IXMLDOMText

STDMETHOD(splitText)(long offset, IXMLDOMText  **rightHandTextNode)
{
	ATLTRACE(_T("IXMLDOMTextImpl::splitText\n"));

	if (NULL == rightHandTextNode)
		return E_POINTER;

	*rightHandTextNode = NULL;

	HRESULT hr = S_OK;

	try
	{
		hr = wrapNode(m_pIXMLDOMDocument,get_DOM_Text().splitText(offset),IID_IXMLDOMText, reinterpret_cast<LPVOID *> (rightHandTextNode));
	}
	catch(...)
	{
		return E_FAIL;
	}
	
	return hr;
}

};

#endif // ___ixmldomtextimpl_h___