#ifndef ___xmldomentityreference_h___
#define ___xmldomentityreference_h___

#include <dom/DOM_EntityReference.hpp>
#include "IXMLDOMNodeImpl.h"

class ATL_NO_VTABLE CXMLDOMEntityReference : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public IXMLDOMNodeImpl<IXMLDOMEntityReference, &IID_IXMLDOMEntityReference, &LIBID_IBMXMLLib>
{
public:
	CXMLDOMEntityReference()
	{}

	void	FinalRelease()
	{
		ReleaseOwnerDoc();
	}

	virtual DOM_Node& get_DOM_Node()			 { return entityReference;} 
	virtual DOMNodeType get_DOMNodeType() const  { return NODE_ENTITY_REFERENCE; }

DECLARE_NOT_AGGREGATABLE(CXMLDOMEntityReference)
DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CXMLDOMEntityReference)
	COM_INTERFACE_ENTRY(IXMLDOMEntityReference)
	COM_INTERFACE_ENTRY(IXMLDOMNode)
	COM_INTERFACE_ENTRY(IIBMXMLDOMNodeIdentity)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

	DOM_EntityReference entityReference;
};

typedef CComObject<CXMLDOMEntityReference> CXMLDOMEntityReferenceObj;

#endif // ___xmldomentityreference_h___