
#include <dom/DOM_Node.hpp>
#include "XMLDOMNodeList.h"
#include "XMLDOMNamedNodeMap.h"
#include "XMLDOMUtil.h"

template <class T, const IID* piid, const GUID* plibid, WORD wMajor,WORD wMinor, class tihclass>
STDMETHODIMP
IXMLDOMNodeImpl<T,piid,plibid,wMajor,wMinor,tihclass>::get_nodeName(BSTR *pVal)
{
	ATLTRACE(_T("IXMLDOMNodeImpl::get_nodeName\n"));

	if (NULL == pVal)
		return E_POINTER;

	*pVal = NULL;

	try
	{
		*pVal = SysAllocString(get_DOM_Node().getNodeName().rawBuffer());
	}
	catch(...)
	{
		return E_FAIL;
	}
	
	return S_OK;
}

template <class T, const IID* piid, const GUID* plibid, WORD wMajor,WORD wMinor, class tihclass>
STDMETHODIMP
IXMLDOMNodeImpl<T,piid,plibid,wMajor,wMinor,tihclass>::get_nodeValue(VARIANT *pVal)
{
	ATLTRACE(_T("IXMLDOMNodeImpl::get_nodeValue\n"));

	if (NULL == pVal)
		return E_POINTER;

	::VariantInit(pVal);

	try
	{
		V_VT(pVal)   = VT_BSTR;
		V_BSTR(pVal) = SysAllocString(get_DOM_Node().getNodeValue().rawBuffer());
	}
	catch(...)
	{
		return E_FAIL;
	}
	
	return S_OK;
}

template <class T, const IID* piid, const GUID* plibid, WORD wMajor,WORD wMinor, class tihclass>
STDMETHODIMP
IXMLDOMNodeImpl<T,piid,plibid,wMajor,wMinor,tihclass>::put_nodeValue(VARIANT newVal)
{
	ATLTRACE(_T("IXMLDOMNodeImpl::put_nodeValue\n"));

	if (V_VT(&newVal) != VT_BSTR)
		return E_INVALIDARG;

	try
	{
		get_DOM_Node().setNodeValue(V_BSTR(&newVal));
	}
	catch(...)
	{
		return E_FAIL;
	}
	

	return S_OK;
}

template <class T, const IID* piid, const GUID* plibid, WORD wMajor,WORD wMinor, class tihclass>
STDMETHODIMP
IXMLDOMNodeImpl<T,piid,plibid,wMajor,wMinor,tihclass>::get_nodeType(DOMNodeType *pVal)
{
	ATLTRACE(_T("IXMLDOMNodeImpl::get_nodeType\n"));

	if (NULL == pVal)
		return E_POINTER;

	*pVal = get_DOMNodeType();

	return S_OK;
}

template <class T, const IID* piid, const GUID* plibid, WORD wMajor,WORD wMinor, class tihclass>
STDMETHODIMP
IXMLDOMNodeImpl<T,piid,plibid,wMajor,wMinor,tihclass>::get_parentNode(IXMLDOMNode **pVal)
{	
	ATLTRACE(_T("IXMLDOMNodeImpl::get_parentNode\n"));
	
	if (NULL == pVal)
		return E_POINTER;

	*pVal = NULL;
	HRESULT hr = S_OK;

	try
	{
		hr = wrapNode(m_pIXMLDOMDocument, get_DOM_Node().getParentNode(),IID_IXMLDOMNode,reinterpret_cast<LPVOID *> (pVal));
	}
	catch(...)
	{
		return E_FAIL;
	}
	

	return hr;
}

template <class T, const IID* piid, const GUID* plibid, WORD wMajor,WORD wMinor, class tihclass>
STDMETHODIMP
IXMLDOMNodeImpl<T,piid,plibid,wMajor,wMinor,tihclass>::get_childNodes(IXMLDOMNodeList * *pVal)
{
	ATLTRACE(_T("IXMLDOMNodeImpl::get_childNodes\n"));
	
	if (NULL == pVal)
		return E_POINTER;

	*pVal = NULL;

	CXMLDOMNodeListObj *pObj = NULL;
	HRESULT hr = CXMLDOMNodeListObj::CreateInstance(&pObj);
	if (S_OK != hr) 
		return hr;
	
	pObj->AddRef();
	pObj->SetOwnerDoc(m_pIXMLDOMDocument);

	try
	{
		pObj->m_container = get_DOM_Node().getChildNodes();
	}
	catch(...)
	{
		pObj->Release(); 
		return E_FAIL;
	}
	
	hr = pObj->QueryInterface(IID_IXMLDOMNodeList, reinterpret_cast<LPVOID*> (pVal));
	if (S_OK != hr) 
		*pVal = NULL;

	pObj->Release();
	return hr;
}

template <class T, const IID* piid, const GUID* plibid, WORD wMajor,WORD wMinor, class tihclass>
STDMETHODIMP
IXMLDOMNodeImpl<T,piid,plibid,wMajor,wMinor,tihclass>::get_firstChild(IXMLDOMNode **pVal)
{
	ATLTRACE(_T("IXMLDOMNodeImpl::get_firstChild\n"));

	if (NULL == pVal)
		return E_POINTER;

	*pVal = NULL;
	HRESULT hr = S_OK;

	try
	{
		hr = wrapNode(m_pIXMLDOMDocument,get_DOM_Node().getFirstChild(),IID_IXMLDOMNode, reinterpret_cast<LPVOID *> (pVal));
	}
	catch(...)
	{
		return E_FAIL;
	}
	

	return hr;
}

template <class T, const IID* piid, const GUID* plibid, WORD wMajor,WORD wMinor, class tihclass>
STDMETHODIMP
IXMLDOMNodeImpl<T,piid,plibid,wMajor,wMinor,tihclass>::get_lastChild(IXMLDOMNode **pVal)
{
	ATLTRACE(_T("IXMLDOMNodeImpl::get_lastChild\n"));

	if (NULL == pVal)
		return E_POINTER;

	*pVal = NULL;
	HRESULT hr = S_OK;

	try
	{
		hr = wrapNode(m_pIXMLDOMDocument,get_DOM_Node().getLastChild(),IID_IXMLDOMNode, reinterpret_cast<LPVOID *> (pVal));
	}
	catch(...)
	{
		return E_FAIL;
	}
	

	return hr;
}

template <class T, const IID* piid, const GUID* plibid, WORD wMajor,WORD wMinor, class tihclass>
STDMETHODIMP
IXMLDOMNodeImpl<T,piid,plibid,wMajor,wMinor,tihclass>::get_previousSibling(IXMLDOMNode * *pVal)
{
	ATLTRACE(_T("IXMLDOMNodeImpl::get_previousSibling\n"));

	if (NULL == pVal)
		return E_POINTER;

	*pVal = NULL;
	HRESULT hr = S_OK;

	try
	{
		hr = wrapNode(m_pIXMLDOMDocument,get_DOM_Node().getPreviousSibling(),IID_IXMLDOMNode, reinterpret_cast<LPVOID *> (pVal));
	}
	catch(...)
	{
		return E_FAIL;
	}
	

	return hr;
}

template <class T, const IID* piid, const GUID* plibid, WORD wMajor,WORD wMinor, class tihclass>
STDMETHODIMP
IXMLDOMNodeImpl<T,piid,plibid,wMajor,wMinor,tihclass>::get_nextSibling(IXMLDOMNode * *pVal)
{
	ATLTRACE(_T("IXMLDOMNodeImpl::get_nextSibling\n"));

	if (NULL == pVal)
		return E_POINTER;

	*pVal = NULL;
	HRESULT hr = S_OK;

	try
	{
		hr = wrapNode(m_pIXMLDOMDocument,get_DOM_Node().getNextSibling(),IID_IXMLDOMNode, reinterpret_cast<LPVOID *> (pVal));
	}
	catch(...)
	{
		return E_FAIL;
	}
	
	return hr;
}

template <class T, const IID* piid, const GUID* plibid, WORD wMajor,WORD wMinor, class tihclass>
STDMETHODIMP
IXMLDOMNodeImpl<T,piid,plibid,wMajor,wMinor,tihclass>::get_attributes(IXMLDOMNamedNodeMap * *pVal)
{
	ATLTRACE(_T("IXMLDOMNodeImpl::get_attributes\n"));

	if (NULL == pVal)
		return E_POINTER;

	*pVal = NULL;

	DOM_NamedNodeMap map;
	try
	{
		map = get_DOM_Node().getAttributes();
	}
	catch(...)
	{
		return E_FAIL;
	}
	
	if (map == 0 && 
		NODE_ELEMENT  != get_DOMNodeType() &&
		NODE_ENTITY   != get_DOMNodeType() &&
		NODE_NOTATION != get_DOMNodeType()) 
		return S_OK;
	
	CXMLDOMNamedNodeMapObj *pObj = NULL;
	HRESULT hr = CXMLDOMNamedNodeMapObj::CreateInstance(&pObj);
	if (S_OK != hr) 
		return hr;
	
	pObj->AddRef();
	pObj->SetOwnerDoc(m_pIXMLDOMDocument);
	pObj->m_container = map;
	
	hr = pObj->QueryInterface(IID_IXMLDOMNamedNodeMap, reinterpret_cast<LPVOID*> (pVal));
	if (S_OK != hr) 
		*pVal = NULL;

	pObj->Release();
	return hr;
}

template <class T, const IID* piid, const GUID* plibid, WORD wMajor,WORD wMinor, class tihclass>
STDMETHODIMP
IXMLDOMNodeImpl<T,piid,plibid,wMajor,wMinor,tihclass>::insertBefore(IXMLDOMNode *newChild, VARIANT refChild, IXMLDOMNode **outNewChild)
{
	ATLTRACE(_T("IXMLDOMNodeImpl::insertBefore\n"));

	if (NULL == outNewChild)
		return E_POINTER;

	*outNewChild = NULL;

	if (NULL == newChild)
		return E_INVALIDARG;

	if (V_VT(&refChild) != VT_DISPATCH && V_VT(&refChild) != VT_NULL)
		return E_INVALIDARG;

	if (V_VT(&refChild) == VT_NULL) 
		return appendChild(newChild,outNewChild); 
	
	CComQIPtr<IIBMXMLDOMNodeIdentity,&IID_IIBMXMLDOMNodeIdentity> pNewChild(newChild);
	if (!pNewChild)
		return E_NOINTERFACE;

	long id = 0;
	HRESULT hr = pNewChild->get_NodeId(&id);
	if (S_OK != hr)
		return hr;

	DOM_Node *pNewChildNode = reinterpret_cast<DOM_Node*> (id);
	if (NULL == pNewChildNode)
		return E_INVALIDARG;

	CComQIPtr<IIBMXMLDOMNodeIdentity,&IID_IIBMXMLDOMNodeIdentity> pRefChild(V_DISPATCH(&refChild));
	if (!pRefChild)
		return E_NOINTERFACE;

	id = 0;
	hr = pRefChild->get_NodeId(&id);
	if (S_OK != hr)
		return hr;

	DOM_Node *pRefChildNode = reinterpret_cast<DOM_Node*> (id);
	if (NULL == pRefChildNode)
		return E_INVALIDARG;

	try
	{
		DOM_Node n = get_DOM_Node().insertBefore(*pNewChildNode, *pRefChildNode);
		hr = wrapNode(m_pIXMLDOMDocument,n,IID_IXMLDOMNode, reinterpret_cast<LPVOID *> (outNewChild));
	}
	catch(...)
	{
		return E_FAIL;
	}
	
	return hr;
}

template <class T, const IID* piid, const GUID* plibid, WORD wMajor,WORD wMinor, class tihclass>
STDMETHODIMP
IXMLDOMNodeImpl<T,piid,plibid,wMajor,wMinor,tihclass>::replaceChild(IXMLDOMNode *newChild, IXMLDOMNode *oldChild, IXMLDOMNode * *outNewChild)
{
	ATLTRACE(_T("IXMLDOMNodeImpl::replaceChild\n"));

	if (NULL == outNewChild)
		return E_POINTER;

	*outNewChild = NULL;

	if (NULL == oldChild)
		return E_INVALIDARG;

	if (NULL == newChild)
		return removeChild(oldChild,outNewChild);

		
	CComQIPtr<IIBMXMLDOMNodeIdentity,&IID_IIBMXMLDOMNodeIdentity> pNewChild(newChild);
	if (!pNewChild)
		return E_NOINTERFACE;

	long id = 0;
	HRESULT hr = pNewChild->get_NodeId(&id);
	if (S_OK != hr)
		return hr;

	DOM_Node *pNewChildNode = reinterpret_cast<DOM_Node*> (id);
	if (NULL == pNewChildNode)
		return E_INVALIDARG;

	CComQIPtr<IIBMXMLDOMNodeIdentity,&IID_IIBMXMLDOMNodeIdentity> pOldChild(oldChild);
	if (!pOldChild)
		return E_NOINTERFACE;

	id = 0;
	hr = pOldChild->get_NodeId(&id);
	if (S_OK != hr)
		return hr;

	DOM_Node *pOldChildNode = reinterpret_cast<DOM_Node*> (id);
	if (NULL == pOldChildNode)
		return E_INVALIDARG;

	try
	{
		DOM_Node n = get_DOM_Node().replaceChild(*pNewChildNode, *pOldChildNode);
		hr = wrapNode(m_pIXMLDOMDocument,n,IID_IXMLDOMNode, reinterpret_cast<LPVOID *> (outNewChild));
	}
	catch(...)
	{
		return E_FAIL;
	}
	
	return S_OK;
}

template <class T, const IID* piid, const GUID* plibid, WORD wMajor,WORD wMinor, class tihclass>
STDMETHODIMP
IXMLDOMNodeImpl<T,piid,plibid,wMajor,wMinor,tihclass>::removeChild(IXMLDOMNode *child, IXMLDOMNode * *oldChild)
{
	ATLTRACE(_T("IXMLDOMNodeImpl::removeChild\n"));

	if (NULL == oldChild)
		return E_POINTER;

	*oldChild = NULL;

	if (NULL == child)
		return E_INVALIDARG;
		
	CComQIPtr<IIBMXMLDOMNodeIdentity,&IID_IIBMXMLDOMNodeIdentity> pChild(child);
	if (!pChild)
		return E_NOINTERFACE;

	long id = 0;
	HRESULT hr = pChild->get_NodeId(&id);
	if (S_OK != hr)
		return hr;

	DOM_Node *pChildNode = reinterpret_cast<DOM_Node*> (id);
	if (NULL == pChildNode)
		return E_INVALIDARG;

	try
	{
		DOM_Node n = get_DOM_Node().removeChild(*pChildNode);
		hr = wrapNode(m_pIXMLDOMDocument,n,IID_IXMLDOMNode, reinterpret_cast<LPVOID *> (oldChild));
	}
	catch(...)
	{
		return E_FAIL;
	}
	
	return S_OK;
}

template <class T, const IID* piid, const GUID* plibid, WORD wMajor,WORD wMinor, class tihclass>
STDMETHODIMP
IXMLDOMNodeImpl<T,piid,plibid,wMajor,wMinor,tihclass>::appendChild(IXMLDOMNode *newChild, IXMLDOMNode * *outNewChild)
{
	ATLTRACE(_T("IXMLDOMNodeImpl::appendChild\n"));
	
	if (NULL == outNewChild)
		return E_POINTER;

	*outNewChild = NULL;

	if (NULL == newChild)
		return E_INVALIDARG;
		
	CComQIPtr<IIBMXMLDOMNodeIdentity,&IID_IIBMXMLDOMNodeIdentity> pNewChild(newChild);
	if (!pNewChild)
		return E_NOINTERFACE;

	long id = 0;
	HRESULT hr = pNewChild->get_NodeId(&id);
	if (S_OK != hr)
		return hr;

	DOM_Node *pNewChildNode = reinterpret_cast<DOM_Node*> (id);
	if (NULL == pNewChildNode)
		return E_INVALIDARG;

	try
	{
		DOM_Node n = get_DOM_Node().appendChild(*pNewChildNode);
		hr = wrapNode(m_pIXMLDOMDocument,n,IID_IXMLDOMNode, reinterpret_cast<LPVOID *> (outNewChild));
	}
	catch(...)
	{
		return E_FAIL;
	}
	
	return S_OK;
}

template <class T, const IID* piid, const GUID* plibid, WORD wMajor,WORD wMinor, class tihclass>
STDMETHODIMP
IXMLDOMNodeImpl<T,piid,plibid,wMajor,wMinor,tihclass>::hasChildNodes(VARIANT_BOOL *hasChild)
{
	ATLTRACE(_T("IXMLDOMNodeImpl::hasChildNodes\n"));

	if (NULL == hasChild)
		return E_POINTER;

	*hasChild = VARIANT_FALSE;

	try
	{
		*hasChild = (get_DOM_Node().hasChildNodes()) ? VARIANT_TRUE : VARIANT_FALSE;
	}
	catch(...)
	{
		return E_FAIL;
	}
	
	return S_OK;
}

template <class T, const IID* piid, const GUID* plibid, WORD wMajor,WORD wMinor, class tihclass>
STDMETHODIMP
IXMLDOMNodeImpl<T,piid,plibid,wMajor,wMinor,tihclass>::get_ownerDocument(IXMLDOMDocument **pVal)
{
	ATLTRACE(_T("IXMLDOMNodeImpl::get_ownerDocument\n"));

	if (NULL == pVal)
		return E_POINTER;

	*pVal = m_pIXMLDOMDocument;
	if (*pVal != NULL)
		(*pVal)->AddRef();

	return S_OK;
}

template <class T, const IID* piid, const GUID* plibid, WORD wMajor,WORD wMinor, class tihclass>
STDMETHODIMP
IXMLDOMNodeImpl<T,piid,plibid,wMajor,wMinor,tihclass>::cloneNode(VARIANT_BOOL deep, IXMLDOMNode **pVal)
{
	ATLTRACE(_T("IXMLDOMNodeImpl::cloneNode\n"));

	if (NULL == pVal)
		return E_POINTER;

	*pVal = NULL;
	HRESULT hr = S_OK;

	try
	{
		hr = wrapNode(m_pIXMLDOMDocument,get_DOM_Node().cloneNode((VARIANT_TRUE == deep) ? true : false),IID_IXMLDOMNode, reinterpret_cast<LPVOID *> (pVal));
	}
	catch(...)
	{
		return E_FAIL;
	}
	

	return hr;
}

template <class T, const IID* piid, const GUID* plibid, WORD wMajor,WORD wMinor, class tihclass>
STDMETHODIMP
IXMLDOMNodeImpl<T,piid,plibid,wMajor,wMinor,tihclass>::get_nodeTypeString(BSTR *pVal)
{
	ATLTRACE(_T("IXMLDOMNodeImpl::get_nodeTypeString\n"));

	USES_CONVERSION;

	if (NULL == pVal)
		return E_POINTER;

	*pVal = ::SysAllocString(T2OLE(g_DomNodeName[get_DOMNodeType()]));
	
	return S_OK;
}

template <class T, const IID* piid, const GUID* plibid, WORD wMajor,WORD wMinor, class tihclass>
STDMETHODIMP
IXMLDOMNodeImpl<T,piid,plibid,wMajor,wMinor,tihclass>::get_text(BSTR *pVal)
{
	ATLTRACE(_T("IXMLDOMNodeImpl::get_text\n"));

	if (NULL == pVal)
		return E_POINTER;

	*pVal = NULL;

	_bstr_t text;
	GetText(get_DOM_Node(), text);

	*pVal = text.copy();

	return S_OK;
}

template <class T, const IID* piid, const GUID* plibid, WORD wMajor,WORD wMinor, class tihclass>
STDMETHODIMP
IXMLDOMNodeImpl<T,piid,plibid,wMajor,wMinor,tihclass>::put_text(BSTR newVal)
{
	ATLTRACE(_T("IXMLDOMNodeImpl::put_text\n"));

	DOMNodeType type = get_DOMNodeType();
	if (NODE_DOCUMENT_FRAGMENT == type ||
		NODE_DOCUMENT_TYPE	   == type ||
		NODE_ENTITY			   == type ||
		NODE_ENTITY_REFERENCE  == type ||
		NODE_NOTATION		   == type)
		return E_ACCESSDENIED;

	try
	{
		get_DOM_Node().setNodeValue(newVal);
	}
	catch(...)
	{
		return E_FAIL;
	}

	return S_OK;
}

template <class T, const IID* piid, const GUID* plibid, WORD wMajor,WORD wMinor, class tihclass>
STDMETHODIMP
IXMLDOMNodeImpl<T,piid,plibid,wMajor,wMinor,tihclass>::get_specified(VARIANT_BOOL *pVal)
{
	ATLTRACE(_T("IXMLDOMNodeImpl::get_specified\n"));

	return E_NOTIMPL;
}

template <class T, const IID* piid, const GUID* plibid, WORD wMajor,WORD wMinor, class tihclass>
STDMETHODIMP
IXMLDOMNodeImpl<T,piid,plibid,wMajor,wMinor,tihclass>::get_definition(IXMLDOMNode * *pVal)
{	
	ATLTRACE(_T("IXMLDOMNodeImpl::get_definition\n"));

	return E_NOTIMPL;
}

template <class T, const IID* piid, const GUID* plibid, WORD wMajor,WORD wMinor, class tihclass>
STDMETHODIMP
IXMLDOMNodeImpl<T,piid,plibid,wMajor,wMinor,tihclass>::get_nodeTypedValue(VARIANT *pVal)
{
	ATLTRACE(_T("IXMLDOMNodeImpl::get_nodeTypedValue\n"));

	return get_nodeValue(pVal);
}

template <class T, const IID* piid, const GUID* plibid, WORD wMajor,WORD wMinor, class tihclass>
STDMETHODIMP
IXMLDOMNodeImpl<T,piid,plibid,wMajor,wMinor,tihclass>::put_nodeTypedValue(VARIANT newVal)
{	
	ATLTRACE(_T("IXMLDOMNodeImpl::put_nodeTypedValue\n"));

	return put_nodeValue(newVal);
}

template <class T, const IID* piid, const GUID* plibid, WORD wMajor,WORD wMinor, class tihclass>
STDMETHODIMP
IXMLDOMNodeImpl<T,piid,plibid,wMajor,wMinor,tihclass>::get_dataType(VARIANT *pVal)
{
	ATLTRACE(_T("IXMLDOMNodeImpl::get_dataType\n"));

	return E_NOTIMPL;
}

template <class T, const IID* piid, const GUID* plibid, WORD wMajor,WORD wMinor, class tihclass>
STDMETHODIMP
IXMLDOMNodeImpl<T,piid,plibid,wMajor,wMinor,tihclass>::put_dataType(BSTR dataTypeName)
{
	ATLTRACE(_T("IXMLDOMNodeImpl::put_dataType\n"));

	return E_NOTIMPL;
}

template <class T, const IID* piid, const GUID* plibid, WORD wMajor,WORD wMinor, class tihclass>
STDMETHODIMP
IXMLDOMNodeImpl<T,piid,plibid,wMajor,wMinor,tihclass>::get_xml(BSTR *pVal)
{
	ATLTRACE(_T("IXMLDOMNodeImpl::get_xml\n"));

	if (NULL == pVal)
		return E_POINTER;

	*pVal = NULL;

	_bstr_t text;
	
	try {
		GetXML(get_DOM_Node(),text);
	}
	catch(...)
	{
		return E_FAIL;
	}

	*pVal = text.copy();
	
	return S_OK;
}

template <class T, const IID* piid, const GUID* plibid, WORD wMajor,WORD wMinor, class tihclass>
STDMETHODIMP
IXMLDOMNodeImpl<T,piid,plibid,wMajor,wMinor,tihclass>::transformNode(IXMLDOMNode *stylesheet, BSTR *xmlString)
{
	ATLTRACE(_T("IXMLDOMNodeImpl::transformNode\n"));

	return E_NOTIMPL;
}

template <class T, const IID* piid, const GUID* plibid, WORD wMajor,WORD wMinor, class tihclass>
STDMETHODIMP
IXMLDOMNodeImpl<T,piid,plibid,wMajor,wMinor,tihclass>::selectNodes(BSTR queryString, IXMLDOMNodeList * *resultList)
{
	ATLTRACE(_T("IXMLDOMNodeImpl::selectNodes\n"));

	return E_NOTIMPL;
}

template <class T, const IID* piid, const GUID* plibid, WORD wMajor,WORD wMinor, class tihclass>
STDMETHODIMP
IXMLDOMNodeImpl<T,piid,plibid,wMajor,wMinor,tihclass>::selectSingleNode(BSTR queryString, IXMLDOMNode * *resultNode)
{
	ATLTRACE(_T("IXMLDOMNodeImpl::selectSingleNode\n"));

	return E_NOTIMPL;
}

template <class T, const IID* piid, const GUID* plibid, WORD wMajor,WORD wMinor, class tihclass>
STDMETHODIMP
IXMLDOMNodeImpl<T,piid,plibid,wMajor,wMinor,tihclass>::get_parsed(VARIANT_BOOL *pVal)
{
	ATLTRACE(_T("IXMLDOMNodeImpl::get_parsed\n"));

	if (NULL == pVal)
		return E_POINTER;

	*pVal = VARIANT_TRUE;
	return S_OK;
}

template <class T, const IID* piid, const GUID* plibid, WORD wMajor,WORD wMinor, class tihclass>
STDMETHODIMP
IXMLDOMNodeImpl<T,piid,plibid,wMajor,wMinor,tihclass>::get_namespaceURI(BSTR *pVal)
{
	ATLTRACE(_T("IXMLDOMNodeImpl::get_namespaceURI\n"));

	if (NULL == pVal)
		return E_POINTER;

	*pVal = NULL;

	try {
		*pVal = SysAllocString(get_DOM_Node().getNamespaceURI().rawBuffer());
	}
	catch(...)
	{
		return E_FAIL;
	}

	return S_OK;
}

template <class T, const IID* piid, const GUID* plibid, WORD wMajor,WORD wMinor, class tihclass>
STDMETHODIMP
IXMLDOMNodeImpl<T,piid,plibid,wMajor,wMinor,tihclass>::get_prefix(BSTR *pVal)
{
	ATLTRACE(_T("IXMLDOMNodeImpl::get_prefix\n"));
	
	if (NULL == pVal)
		return E_POINTER;

	*pVal = NULL;

	try {
		*pVal = SysAllocString(get_DOM_Node().getPrefix().rawBuffer());
	}
	catch(...)
	{
		return E_FAIL;
	}

	return S_OK;
}

template <class T, const IID* piid, const GUID* plibid, WORD wMajor,WORD wMinor, class tihclass>
STDMETHODIMP
IXMLDOMNodeImpl<T,piid,plibid,wMajor,wMinor,tihclass>::get_baseName(BSTR *pVal)
{
	ATLTRACE(_T("IXMLDOMNodeImpl::get_baseName\n"));

	if (NULL == pVal)
		return E_POINTER;

	*pVal = NULL;

	try {
		*pVal = SysAllocString(get_DOM_Node().getLocalName().rawBuffer());
	}
	catch(...)
	{
		return E_FAIL;
	}

	return S_OK;
}

template <class T, const IID* piid, const GUID* plibid, WORD wMajor,WORD wMinor, class tihclass>
STDMETHODIMP
IXMLDOMNodeImpl<T,piid,plibid,wMajor,wMinor,tihclass>::transformNodeToObject(IXMLDOMNode *stylesheet, VARIANT outputObject)
{
	ATLTRACE(_T("IXMLDOMNodeImpl::transformNodeToObject\n"));

	return E_NOTIMPL;
}
