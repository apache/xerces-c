#ifndef ___xmldomtext_h___
#define ___xmldomtext_h___

#include <dom/DOM_Text.hpp>
#include "IXMLDOMTextImpl.h"

class ATL_NO_VTABLE CXMLDOMText : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public IXMLDOMTextImpl<IXMLDOMText, &IID_IXMLDOMText, &LIBID_IBMXMLLib>
{
public:
	CXMLDOMText()
	{}
	
	void	FinalRelease()
	{
		ReleaseOwnerDoc();
	}

	virtual DOM_Text& get_DOM_Text()		 	 { return text;} 
	virtual DOMNodeType get_DOMNodeType() const  { return NODE_TEXT; }

DECLARE_NOT_AGGREGATABLE(CXMLDOMText)
DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CXMLDOMText)
	COM_INTERFACE_ENTRY(IXMLDOMText)
	COM_INTERFACE_ENTRY(IXMLDOMCharacterData)
	COM_INTERFACE_ENTRY(IXMLDOMNode)
	COM_INTERFACE_ENTRY(IIBMXMLDOMNodeIdentity)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()
	
	DOM_Text text;
};

typedef CComObject<CXMLDOMText> CXMLDOMTextObj;

#endif // ___xmldomtext_h___