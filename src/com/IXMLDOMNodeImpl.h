#ifndef ___ixmldomnodeimpl_h___
#define ___ixmldomnodeimpl_h___

template <class T, const IID* piid, const GUID* plibid = &CComModule::m_libid, WORD wMajor = 1,
WORD wMinor = 0, class tihclass = CComTypeInfoHolder>
class ATL_NO_VTABLE IXMLDOMNodeImpl: 
	public IDispatchImpl<T,piid,plibid,wMajor,wMinor,tihclass>,
	public IIBMXMLDOMNodeIdentity
{
public:

	IXMLDOMNodeImpl()
		:m_pIXMLDOMDocument(NULL)
	{}

	virtual DOM_Node& get_DOM_Node()		    = 0; 
	virtual DOMNodeType get_DOMNodeType() const = 0;

	void	SetOwnerDoc(IXMLDOMDocument	*p)
	{
		m_pIXMLDOMDocument = p;
		if (m_pIXMLDOMDocument != NULL)
			m_pIXMLDOMDocument->AddRef();
	}

	// IIBMXMLDOMNodeIdentity 
	STDMETHOD(get_NodeId)(long *pVal)
	{
		ATLTRACE(_T("IXMLDOMNodeImpl::get_NodeId\n"));

		if (NULL == pVal)
			return E_POINTER;

		*pVal = reinterpret_cast<long> (&get_DOM_Node());
		return S_OK;
	}
	
	// IXMLDOMNode 
    STDMETHOD(get_nodeName)(BSTR  *pVal);
	STDMETHOD(get_nodeValue)(VARIANT  *pVal);
	STDMETHOD(put_nodeValue)(VARIANT newVal);
	STDMETHOD(get_nodeType)(DOMNodeType  *pVal);
	STDMETHOD(get_parentNode)(IXMLDOMNode  * *pVal);
	STDMETHOD(get_childNodes)(IXMLDOMNodeList  * *pVal);
	STDMETHOD(get_firstChild)(IXMLDOMNode  * *pVal);
	STDMETHOD(get_lastChild)(IXMLDOMNode  * *pVal);
	STDMETHOD(get_previousSibling)(IXMLDOMNode  * *pVal);
	STDMETHOD(get_nextSibling)(IXMLDOMNode  * *pVal);
	STDMETHOD(get_attributes)(IXMLDOMNamedNodeMap  * *pVal);
	STDMETHOD(insertBefore)(IXMLDOMNode  *newChild, VARIANT refChild, IXMLDOMNode  * *outNewChild);
	STDMETHOD(replaceChild)(IXMLDOMNode  *newChild, IXMLDOMNode  *oldChild, IXMLDOMNode  * *outNewChild);
	STDMETHOD(removeChild)(IXMLDOMNode  *childNode, IXMLDOMNode  * *oldChild);
	STDMETHOD(appendChild)(IXMLDOMNode  *newChild, IXMLDOMNode  * *outNewChild);
	STDMETHOD(hasChildNodes)(VARIANT_BOOL  *hasChild);
	STDMETHOD(get_ownerDocument)(IXMLDOMDocument  * *pVal);
	STDMETHOD(cloneNode)(VARIANT_BOOL deep, IXMLDOMNode  * *cloneRoot);
	STDMETHOD(get_nodeTypeString)(BSTR  *pVal);
	STDMETHOD(get_text)(BSTR  *pVal);
	STDMETHOD(put_text)(BSTR newVal);
	STDMETHOD(get_specified)(VARIANT_BOOL  *pVal);
	STDMETHOD(get_definition)(IXMLDOMNode  * *pVal);
	STDMETHOD(get_nodeTypedValue)(VARIANT  *pVal);
	STDMETHOD(put_nodeTypedValue)(VARIANT newVal);
	STDMETHOD(get_dataType)(VARIANT  *pVal);
	STDMETHOD(put_dataType)(BSTR dataTypeName);
	STDMETHOD(get_xml)(BSTR  *pVal);
	STDMETHOD(transformNode)(IXMLDOMNode  *stylesheet, BSTR  *xmlString);
	STDMETHOD(selectNodes)(BSTR queryString, IXMLDOMNodeList  * *resultList);
	STDMETHOD(selectSingleNode)(BSTR queryString, IXMLDOMNode  * *resultNode);
	STDMETHOD(get_parsed)(VARIANT_BOOL  *pVal);
	STDMETHOD(get_namespaceURI)(BSTR  *pVal);
	STDMETHOD(get_prefix)(BSTR  *pVal);
	STDMETHOD(get_baseName)(BSTR  *pVal);
	STDMETHOD(transformNodeToObject)(IXMLDOMNode  *stylesheet, VARIANT outputObject);

protected:

	IXMLDOMDocument	*m_pIXMLDOMDocument;
	
	void	ReleaseOwnerDoc()
	{
		if (m_pIXMLDOMDocument != NULL) {
			m_pIXMLDOMDocument->Release();
			m_pIXMLDOMDocument = NULL;
		}
	}

};

#include "IXMLDOMNodeImpl.inl"

#endif // ___ixmldomnodeimpl_h___
