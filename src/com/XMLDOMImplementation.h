#ifndef ___xmldomimplementation_h___
#define ___xmldomimplementation_h___

#include <dom/DOM_DOMImplementation.hpp>

class ATL_NO_VTABLE CXMLDOMImplementation : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public IDispatchImpl<IXMLDOMImplementation, &IID_IXMLDOMImplementation, &LIBID_IBMXMLLib>
{
public:
	CXMLDOMImplementation()
	{}

DECLARE_NOT_AGGREGATABLE(CXMLDOMImplementation)
DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CXMLDOMImplementation)
	COM_INTERFACE_ENTRY(IXMLDOMImplementation)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

	// IXMLDOMImplementation methods
	STDMETHOD(hasFeature)(BSTR feature, BSTR ver, VARIANT_BOOL  *pVal);

	DOM_DOMImplementation implementation;
};

typedef CComObject<CXMLDOMImplementation> CXMLDOMImplementationObj;

#endif // ___xmldomimplemenation_h___
