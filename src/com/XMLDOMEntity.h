#ifndef ___xmldomentity_h___
#define ___xmldomentity_h___

#include <dom/DOM_Entity.hpp>
#include "IXMLDOMNodeImpl.h"

class ATL_NO_VTABLE CXMLDOMEntity : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public IXMLDOMNodeImpl<IXMLDOMEntity, &IID_IXMLDOMEntity, &LIBID_IBMXMLLib>
{
public:
	CXMLDOMEntity()
	{}

	void	FinalRelease()
	{
		ReleaseOwnerDoc();
	}

	virtual DOM_Node& get_DOM_Node()			 { return entity;} 
	virtual DOMNodeType get_DOMNodeType() const  { return NODE_ENTITY; }

DECLARE_NOT_AGGREGATABLE(CXMLDOMEntity)
DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CXMLDOMEntity)
	COM_INTERFACE_ENTRY(IXMLDOMEntity)
	COM_INTERFACE_ENTRY(IXMLDOMNode)
	COM_INTERFACE_ENTRY(IIBMXMLDOMNodeIdentity)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

	// IXMLDOMEntity methods
	STDMETHOD(get_publicId)(VARIANT  *pVal);
	STDMETHOD(get_systemId)(VARIANT  *pVal);
	STDMETHOD(get_notationName)(BSTR  *pVal);

	DOM_Entity entity;
};

typedef CComObject<CXMLDOMEntity> CXMLDOMEntityObj;

#endif // ___xmldomentity_h___