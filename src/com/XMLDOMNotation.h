#ifndef ___xmldomnotation_h___
#define ___xmldomnotation_h___

#include <dom/DOM_Notation.hpp>
#include "IXMLDOMNodeImpl.h"

class ATL_NO_VTABLE CXMLDOMNotation : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public IXMLDOMNodeImpl<IXMLDOMNotation, &IID_IXMLDOMNotation, &LIBID_IBMXMLLib>
{
public:
	CXMLDOMNotation()
	{}
	
	void	FinalRelease()
	{
		ReleaseOwnerDoc();
	}

	virtual DOM_Node& get_DOM_Node()			 { return notation;} 
	virtual DOMNodeType get_DOMNodeType() const  { return NODE_NOTATION; }

DECLARE_NOT_AGGREGATABLE(CXMLDOMNotation)
DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CXMLDOMNotation)
	COM_INTERFACE_ENTRY(IXMLDOMNotation)
	COM_INTERFACE_ENTRY(IXMLDOMNode)
	COM_INTERFACE_ENTRY(IIBMXMLDOMNodeIdentity)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

	// IXMLDOMNotation methods
	STDMETHOD(get_publicId)(VARIANT  *pVal);
	STDMETHOD(get_systemId)(VARIANT  *pVal);

	DOM_Notation notation;
};

typedef CComObject<CXMLDOMNotation> CXMLDOMNotationObj;

#endif // ___xmldomnotation_h___