#ifndef ___xmldomnamednodemap_h___
#define ___xmldomnamednodemap_h___

#include <dom/DOM_NamedNodeMap.hpp>
#include "NodeContainerImpl.h"

class ATL_NO_VTABLE CXMLDOMNamedNodeMap : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public IDispatchImpl<IXMLDOMNamedNodeMap, &IID_IXMLDOMNamedNodeMap, &LIBID_IBMXMLLib>,
	public NodeContainerImpl<DOM_NamedNodeMap>
{
public:
	CXMLDOMNamedNodeMap()
	{}

	void FinalRelease()
	{
		ReleaseOwnerDoc();
	}

DECLARE_NOT_AGGREGATABLE(CXMLDOMNamedNodeMap)
DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CXMLDOMNamedNodeMap)
	COM_INTERFACE_ENTRY(IXMLDOMNamedNodeMap)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()
  
	// IXMLDOMNamedNodeMap methods
	STDMETHOD(getNamedItem)(BSTR name, IXMLDOMNode  * *namedItem);
	STDMETHOD(setNamedItem)(IXMLDOMNode  *newItem, IXMLDOMNode  * *nameItem);
	STDMETHOD(removeNamedItem)(BSTR name, IXMLDOMNode  * *namedItem);
	STDMETHOD(get_item)(long index, IXMLDOMNode  * *pVal);
	STDMETHOD(get_length)(long  *pVal);
	STDMETHOD(getQualifiedItem)(BSTR baseName, BSTR namespaceURI, IXMLDOMNode  * *node);
	STDMETHOD(removeQualifiedItem)(BSTR baseName, BSTR namespaceURI, IXMLDOMNode  * *node);
	STDMETHOD(nextNode)(IXMLDOMNode  * *nextItem);
	STDMETHOD(reset)();
	STDMETHOD(get__newEnum)(IUnknown  * *pVal);
};

typedef CComObject<CXMLDOMNamedNodeMap> CXMLDOMNamedNodeMapObj;

#endif // ___xmldomnamednodemap_h___