#ifndef ___xmldomdocumenttype_h___
#define ___xmldomdocumenttype_h___

#include <dom/DOM_DocumentType.hpp>
#include "IXMLDOMNodeImpl.h"

class ATL_NO_VTABLE CXMLDOMDocumentType : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public IXMLDOMNodeImpl<IXMLDOMDocumentType, &IID_IXMLDOMDocumentType, &LIBID_IBMXMLLib>
{
public:
	CXMLDOMDocumentType()
	{}

	void	FinalRelease()
	{
		ReleaseOwnerDoc();
	}

	virtual DOM_Node& get_DOM_Node()	  { return documentType;} 
	virtual DOMNodeType get_DOMNodeType() const  { return NODE_DOCUMENT_TYPE; }

DECLARE_NOT_AGGREGATABLE(CXMLDOMDocumentType)
DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CXMLDOMDocumentType)
	COM_INTERFACE_ENTRY(IXMLDOMDocumentType)
	COM_INTERFACE_ENTRY(IXMLDOMNode)
	COM_INTERFACE_ENTRY(IIBMXMLDOMNodeIdentity)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

	// IXMLDOMDocumentType methods
	STDMETHOD(get_name)(BSTR  *pVal);
	STDMETHOD(get_entities)(IXMLDOMNamedNodeMap  * *pVal);
	STDMETHOD(get_notations)(IXMLDOMNamedNodeMap  * *pVal);

	DOM_DocumentType documentType;
};

typedef CComObject<CXMLDOMDocumentType> CXMLDOMDocumentTypeObj;

#endif // ___xmldomdocumenttype_h___