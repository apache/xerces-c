#ifndef ___xmldomdocumentfragment_h___
#define ___xmldomdocumentfragment_h___

#include <dom/DOM_DocumentFragment.hpp>
#include "IXMLDOMNodeImpl.h"

class ATL_NO_VTABLE CXMLDOMDocumentFragment : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public IXMLDOMNodeImpl<IXMLDOMDocumentFragment, &IID_IXMLDOMDocumentFragment, &LIBID_IBMXMLLib>
{
public:
	CXMLDOMDocumentFragment()
	{}

	void	FinalRelease()
	{
		ReleaseOwnerDoc();
	}
	
	virtual DOM_Node& get_DOM_Node()			 { return documentFragment;} 
	virtual DOMNodeType get_DOMNodeType() const  { return NODE_DOCUMENT_FRAGMENT; }

DECLARE_NOT_AGGREGATABLE(CXMLDOMDocumentFragment)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CXMLDOMDocumentFragment)
	COM_INTERFACE_ENTRY(IXMLDOMDocumentFragment)
	COM_INTERFACE_ENTRY(IXMLDOMNode)
	COM_INTERFACE_ENTRY(IIBMXMLDOMNodeIdentity)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

	DOM_DocumentFragment documentFragment;
};

typedef CComObject<CXMLDOMDocumentFragment> CXMLDOMDocumentFragmentObj;

#endif // ___xmldomdocumentfragment_h___