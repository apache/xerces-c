#ifndef ___xmldomnodelist_h___
#define ___xmldomnodelist_h___

#include <dom/DOM_NodeList.hpp>
#include "NodeContainerImpl.h"

class ATL_NO_VTABLE CXMLDOMNodeList : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public IDispatchImpl<IXMLDOMNodeList, &IID_IXMLDOMNodeList, &LIBID_IBMXMLLib>,
	public NodeContainerImpl<DOM_NodeList>
{
public:
	CXMLDOMNodeList()
	{}

	void FinalRelease()
	{
		ReleaseOwnerDoc();
	}

DECLARE_NOT_AGGREGATABLE(CXMLDOMNodeList)
DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CXMLDOMNodeList)
	COM_INTERFACE_ENTRY(IXMLDOMNodeList)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

	// IXMLDOMNodeList
	STDMETHOD(get_item)(long index, IXMLDOMNode  **pVal);
	STDMETHOD(get_length)(long  *pVal);
	STDMETHOD(nextNode)(IXMLDOMNode  **nextItem);
	STDMETHOD(reset)();
	STDMETHOD(get__newEnum)(IUnknown  **pVal);
};
typedef CComObject<CXMLDOMNodeList> CXMLDOMNodeListObj;

#endif // ___xmldomnodelist_h___