#ifndef ___xmldomattribute_h___
#define ___xmldomattribute_h___

#include <dom/DOM_Attr.hpp>
#include "IXMLDOMNodeImpl.h"

class ATL_NO_VTABLE CXMLDOMAttribute : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public IXMLDOMNodeImpl<IXMLDOMAttribute, &IID_IXMLDOMAttribute, &LIBID_IBMXMLLib>
{
public:
	CXMLDOMAttribute()
	{}

	void	FinalRelease()
	{
		ReleaseOwnerDoc();
	}

	virtual DOM_Node& get_DOM_Node()			 { return attr;} 
	virtual DOMNodeType get_DOMNodeType() const  { return NODE_ATTRIBUTE; }

DECLARE_NOT_AGGREGATABLE(CXMLDOMAttribute)
DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CXMLDOMAttribute)
	COM_INTERFACE_ENTRY(IXMLDOMAttribute)
	COM_INTERFACE_ENTRY(IXMLDOMNode)
	COM_INTERFACE_ENTRY(IIBMXMLDOMNodeIdentity)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()
	
	// IXMLDOMAttribute methods
	STDMETHOD(get_name)(BSTR  *pVal);
	STDMETHOD(get_value)(VARIANT  *pVal);
	STDMETHOD(put_value)(VARIANT newVal);

	DOM_Attr attr;
};

typedef CComObject<CXMLDOMAttribute> CXMLDOMAttributeObj;

#endif // ___xmldomattribute_h___