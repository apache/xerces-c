#ifndef ___xmldomelement_h___
#define ___xmldomelement_h___

#include <dom/DOM_Element.hpp>
#include "IXMLDOMNodeImpl.h"

class ATL_NO_VTABLE CXMLDOMElement : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public IXMLDOMNodeImpl<IXMLDOMElement, &IID_IXMLDOMElement, &LIBID_IBMXMLLib>
{
public:
	CXMLDOMElement()
	{}

	void	FinalRelease()
	{
		ReleaseOwnerDoc();
	}

	virtual DOM_Node& get_DOM_Node()	  { return element;} 
	virtual DOMNodeType get_DOMNodeType() const  { return NODE_ELEMENT; }

DECLARE_NOT_AGGREGATABLE(CXMLDOMElement)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CXMLDOMElement)
	COM_INTERFACE_ENTRY(IXMLDOMElement)
	COM_INTERFACE_ENTRY(IXMLDOMNode)
	COM_INTERFACE_ENTRY(IIBMXMLDOMNodeIdentity)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

	// IXMLDOMElement methods
	STDMETHOD(get_tagName)(BSTR  *pVal);
	STDMETHOD(getAttribute)(BSTR name, VARIANT  *value);
	STDMETHOD(setAttribute)(BSTR name, VARIANT value);
	STDMETHOD(removeAttribute)(BSTR name);
	STDMETHOD(getAttributeNode)(BSTR name, IXMLDOMAttribute  * *attr);
	STDMETHOD(setAttributeNode)(IXMLDOMAttribute  *attr, IXMLDOMAttribute  * *attributeNode);
	STDMETHOD(removeAttributeNode)(IXMLDOMAttribute  *attr, IXMLDOMAttribute  * *attributeNode);
	STDMETHOD(getElementsByTagName)(BSTR tagName, IXMLDOMNodeList  * *resultList);
	STDMETHOD(normalize)(void);

	DOM_Element element;
};

typedef CComObject<CXMLDOMElement> CXMLDOMElementObj;

#endif // ___xmldomelement_h___