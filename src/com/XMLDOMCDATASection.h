#ifndef ___xmldomcdatasection_h___
#define ___xmldomcdatasection_h___

#include <dom/DOM_CDATASection.hpp>
#include "IXMLDOMTextImpl.h"

class ATL_NO_VTABLE CXMLDOMCDATASection : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public IXMLDOMTextImpl<IXMLDOMCDATASection, &IID_IXMLDOMCDATASection, &LIBID_IBMXMLLib>
{
public:
	CXMLDOMCDATASection()
	{}

	void	FinalRelease()
	{
		ReleaseOwnerDoc();
	}

	virtual DOM_Text& get_DOM_Text()			 { return cdataSection;} 
	virtual DOMNodeType get_DOMNodeType() const  { return NODE_CDATA_SECTION; }

DECLARE_NOT_AGGREGATABLE(CXMLDOMCDATASection)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CXMLDOMCDATASection)
	COM_INTERFACE_ENTRY(IXMLDOMCDATASection)
	COM_INTERFACE_ENTRY(IXMLDOMText)
	COM_INTERFACE_ENTRY(IXMLDOMCharacterData)
	COM_INTERFACE_ENTRY(IXMLDOMNode)
	COM_INTERFACE_ENTRY(IIBMXMLDOMNodeIdentity)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

	DOM_CDATASection cdataSection;
};

typedef CComObject<CXMLDOMCDATASection> CXMLDOMCDATASectionObj;

#endif // ___xmldomcdatasection_h___