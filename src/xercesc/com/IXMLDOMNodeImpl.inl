/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 1999-2000 The Apache Software Foundation.  All rights
 * reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. The end-user documentation included with the redistribution,
 *    if any, must include the following acknowledgment:
 *       "This product includes software developed by the
 *        Apache Software Foundation (http://www.apache.org/)."
 *    Alternately, this acknowledgment may appear in the software itself,
 *    if and wherever such third-party acknowledgments normally appear.
 *
 * 4. The names "Xerces" and "Apache Software Foundation" must
 *    not be used to endorse or promote products derived from this
 *    software without prior written permission. For written
 *    permission, please contact apache\@apache.org.
 *
 * 5. Products derived from this software may not be called "Apache",
 *    nor may "Apache" appear in their name, without prior written
 *    permission of the Apache Software Foundation.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE APACHE SOFTWARE FOUNDATION OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * ====================================================================
 *
 * This software consists of voluntary contributions made by many
 * individuals on behalf of the Apache Software Foundation, and was
 * originally based on software copyright (c) 1999, International
 * Business Machines, Inc., http://www.ibm.com .  For more information
 * on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 */

/*
 * $Id$
 */


#include <xercesc/dom/deprecated/DOM_Node.hpp>
#include <xercesc/dom/deprecated/DOM_Document.hpp>
#include "XMLDOMNodeList.h"
#include "XMLDOMNamedNodeMap.h"
#include "XMLDOMUtil.h"
#include <xercesc/dom/deprecated/DOM_DOMException.hpp>

XERCES_CPP_NAMESPACE_USE

template <class T, const IID* piid, class tihclass>
HRESULT STDMETHODCALLTYPE IXMLDOMNodeImpl<T,piid,tihclass>::InterfaceSupportsErrorInfo(REFIID riid)
{
	if(riid == *piid) return S_OK;
	return S_FALSE;
}

template <class T, const IID* piid, class tihclass>
STDMETHODIMP
IXMLDOMNodeImpl<T,piid,tihclass>::get_nodeName(BSTR *pVal)
{
	ATLTRACE(_T("IXMLDOMNodeImpl::get_nodeName\n"));

	if (NULL == pVal)
		return E_POINTER;

	*pVal = NULL;

	try
	{
		DOMString val = get_DOM_Node().getNodeName();
		*pVal = SysAllocStringLen(val.rawBuffer(),val.length());
	}
	catch(DOM_DOMException& ex)
	{
		return MakeHRESULT(ex);
	}
	catch(...)
	{
		return E_FAIL;
	}
	
	return S_OK;
}

template <class T, const IID* piid, class tihclass>
STDMETHODIMP
IXMLDOMNodeImpl<T,piid,tihclass>::get_nodeValue(VARIANT *pVal)
{
	ATLTRACE(_T("IXMLDOMNodeImpl::get_nodeValue\n"));

	if (NULL == pVal)
		return E_POINTER;

	::VariantInit(pVal);


	try
	{
		DOMString val = get_DOM_Node().getNodeValue();
		if (val != 0)
		{
			V_VT(pVal)   = VT_BSTR;

			V_BSTR(pVal) = SysAllocStringLen(val.rawBuffer(),val.length());
		}
		else
			V_VT(pVal) = VT_NULL;
	}
	catch(DOM_DOMException& ex)
	{
		return MakeHRESULT(ex);
	}
	catch(...)
	{
		return E_FAIL;
	}
	
	return S_OK;
}

template <class T, const IID* piid, class tihclass>
STDMETHODIMP
IXMLDOMNodeImpl<T,piid,tihclass>::put_nodeValue(VARIANT newVal)
{
	ATLTRACE(_T("IXMLDOMNodeImpl::put_nodeValue\n"));

	try
	{
		if(V_VT(&newVal) == VT_BSTR)
		{
			get_DOM_Node().setNodeValue(V_BSTR(&newVal));
		}
		//
		//   coerce to BSTR or throw error
		//
		else
		{
			get_DOM_Node().setNodeValue((BSTR) (_bstr_t) newVal);
		}
	}
	catch(DOM_DOMException& ex)
	{
		return MakeHRESULT(ex);
	}
	catch(...)
	{
		return E_FAIL;
	}
	

	return S_OK;
}

template <class T, const IID* piid, class tihclass>
STDMETHODIMP
IXMLDOMNodeImpl<T,piid,tihclass>::get_nodeType(DOMNodeType *pVal)
{
	ATLTRACE(_T("IXMLDOMNodeImpl::get_nodeType\n"));

	if (NULL == pVal)
		return E_POINTER;

	*pVal = get_DOMNodeType();

	return S_OK;
}

template <class T, const IID* piid, class tihclass>
STDMETHODIMP
IXMLDOMNodeImpl<T,piid,tihclass>::get_parentNode(IXMLDOMNode **pVal)
{	
	ATLTRACE(_T("IXMLDOMNodeImpl::get_parentNode\n"));
	
	if (NULL == pVal)
		return E_POINTER;

	*pVal = NULL;
	HRESULT hr = S_OK;

	try
	{
		DOM_Node n = get_DOM_Node().getParentNode();
		if(!n.isNull()) {
			hr = wrapNode(m_pIXMLDOMDocument, n,IID_IXMLDOMNode,reinterpret_cast<LPVOID *> (pVal));
		}
	}
	catch(DOM_DOMException& ex)
	{
		return MakeHRESULT(ex);
	}
	catch(...)
	{
		return E_FAIL;
	}
	

	return hr;
}

template <class T, const IID* piid, class tihclass>
STDMETHODIMP
IXMLDOMNodeImpl<T,piid,tihclass>::get_childNodes(IXMLDOMNodeList * *pVal)
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
	catch(DOM_DOMException& ex)
	{
		pObj->Release();
		return MakeHRESULT(ex);
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

template <class T, const IID* piid, class tihclass>
STDMETHODIMP
IXMLDOMNodeImpl<T,piid,tihclass>::get_firstChild(IXMLDOMNode **pVal)
{
	ATLTRACE(_T("IXMLDOMNodeImpl::get_firstChild\n"));

	if (NULL == pVal)
		return E_POINTER;

	*pVal = NULL;

	HRESULT hr = S_OK;

	try
	{
		DOM_Node n = get_DOM_Node().getFirstChild();
		//
		//   returns Nothing if no children
		//
		if(!n.isNull())
			hr = wrapNode(m_pIXMLDOMDocument,n,IID_IXMLDOMNode, reinterpret_cast<LPVOID *> (pVal));
	}
	catch(DOM_DOMException& ex)
	{
		return MakeHRESULT(ex);
	}
	catch(...)
	{
		return E_FAIL;
	}
	

	return hr;
}

template <class T, const IID* piid, class tihclass>
STDMETHODIMP
IXMLDOMNodeImpl<T,piid,tihclass>::get_lastChild(IXMLDOMNode **pVal)
{
	ATLTRACE(_T("IXMLDOMNodeImpl::get_lastChild\n"));

	if (NULL == pVal)
		return E_POINTER;

	*pVal = NULL;

	HRESULT hr = S_OK;

	try
	{
		DOM_Node n = get_DOM_Node().getLastChild();
		if(!n.isNull())
			hr = wrapNode(m_pIXMLDOMDocument,n,IID_IXMLDOMNode, reinterpret_cast<LPVOID *> (pVal));
	}
	catch(DOM_DOMException& ex)
	{
		return MakeHRESULT(ex);
	}
	catch(...)
	{
		return E_FAIL;
	}
	

	return hr;
}

template <class T, const IID* piid, class tihclass>
STDMETHODIMP
IXMLDOMNodeImpl<T,piid,tihclass>::get_previousSibling(IXMLDOMNode * *pVal)
{
	ATLTRACE(_T("IXMLDOMNodeImpl::get_previousSibling\n"));

	if (NULL == pVal)
		return E_POINTER;

	*pVal = NULL;
	HRESULT hr = S_OK;

	try
	{
		DOM_Node n = get_DOM_Node().getPreviousSibling();
		if(!n.isNull())
			hr = wrapNode(m_pIXMLDOMDocument,n,IID_IXMLDOMNode, reinterpret_cast<LPVOID *> (pVal));
	}
	catch(DOM_DOMException& ex)
	{
		return MakeHRESULT(ex);
	}
	catch(...)
	{
		return E_FAIL;
	}
	

	return hr;
}

template <class T, const IID* piid, class tihclass>
STDMETHODIMP
IXMLDOMNodeImpl<T,piid,tihclass>::get_nextSibling(IXMLDOMNode * *pVal)
{
	ATLTRACE(_T("IXMLDOMNodeImpl::get_nextSibling\n"));

	if (NULL == pVal)
		return E_POINTER;

	*pVal = NULL;

	HRESULT hr = S_OK;

	try
	{
		DOM_Node n = get_DOM_Node().getNextSibling();
		if(!n.isNull())
			hr = wrapNode(m_pIXMLDOMDocument,n,IID_IXMLDOMNode, reinterpret_cast<LPVOID *> (pVal));
	}
	catch(DOM_DOMException& ex)
	{
		return MakeHRESULT(ex);
	}
	catch(...)
	{
		return E_FAIL;
	}
	
	return hr;
}

template <class T, const IID* piid, class tihclass>
STDMETHODIMP
IXMLDOMNodeImpl<T,piid,tihclass>::get_attributes(IXMLDOMNamedNodeMap * *pVal)
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
	catch(DOM_DOMException& ex)
	{
		return MakeHRESULT(ex);
	}
	catch(...)
	{
		return E_FAIL;
	}
	
	if ((map == 0) || (NODE_ELEMENT  != get_DOMNodeType()))
		//&&
		//NODE_ENTITY   != get_DOMNodeType() &&
		//NODE_NOTATION != get_DOMNodeType())
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

template <class T, const IID* piid, class tihclass>
STDMETHODIMP
IXMLDOMNodeImpl<T,piid,tihclass>::insertBefore(IXMLDOMNode *newChild, VARIANT refChild, IXMLDOMNode **outNewChild)
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
	catch(DOM_DOMException& ex)
	{
		return MakeHRESULT(ex);
	}
	catch(...)
	{
		return E_FAIL;
	}
	
	return hr;
}

template <class T, const IID* piid, class tihclass>
STDMETHODIMP
IXMLDOMNodeImpl<T,piid,tihclass>::replaceChild(IXMLDOMNode *newChild, IXMLDOMNode *oldChild, IXMLDOMNode * *outNewChild)
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
	catch(DOM_DOMException& ex)
	{
		return MakeHRESULT(ex);
	}
	catch(...)
	{
		return E_FAIL;
	}
	
	return S_OK;
}

template <class T, const IID* piid, class tihclass>
STDMETHODIMP
IXMLDOMNodeImpl<T,piid,tihclass>::removeChild(IXMLDOMNode *child, IXMLDOMNode * *oldChild)
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
		if(!n.isNull())
			hr = wrapNode(m_pIXMLDOMDocument,n,IID_IXMLDOMNode, reinterpret_cast<LPVOID *> (oldChild));
	}
	catch(DOM_DOMException& ex)
	{
		return MakeHRESULT(ex);
	}
	catch(...)
	{
		return E_FAIL;
	}
	
	return S_OK;
}

template <class T, const IID* piid, class tihclass>
STDMETHODIMP
IXMLDOMNodeImpl<T,piid,tihclass>::appendChild(IXMLDOMNode *newChild, IXMLDOMNode * *outNewChild)
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
	catch(DOM_DOMException& ex)
	{
		return MakeHRESULT(ex);
	}
	catch(...)
	{
		return E_FAIL;
	}
	
	return S_OK;
}

template <class T, const IID* piid, class tihclass>
STDMETHODIMP
IXMLDOMNodeImpl<T,piid,tihclass>::hasChildNodes(VARIANT_BOOL *hasChild)
{
	ATLTRACE(_T("IXMLDOMNodeImpl::hasChildNodes\n"));

	if (NULL == hasChild)
		return E_POINTER;

	*hasChild = VARIANT_FALSE;

	try
	{
		*hasChild = (get_DOM_Node().hasChildNodes()) ? VARIANT_TRUE : VARIANT_FALSE;
	}
	catch(DOM_DOMException& ex)
	{
		return MakeHRESULT(ex);
	}
	catch(...)
	{
		return E_FAIL;
	}
	
	return S_OK;
}

template <class T, const IID* piid, class tihclass>
STDMETHODIMP
IXMLDOMNodeImpl<T,piid,tihclass>::get_ownerDocument(IXMLDOMDocument **pVal)
{
	ATLTRACE(_T("IXMLDOMNodeImpl::get_ownerDocument\n"));

	if (NULL == pVal)
		return E_POINTER;
	*pVal = NULL;

	if (get_DOMNodeType() != NODE_DOCUMENT)
	{
		*pVal = m_pIXMLDOMDocument;
		if (*pVal != NULL)
			(*pVal)->AddRef();
	}
	return S_OK;
}

template <class T, const IID* piid, class tihclass>
STDMETHODIMP
IXMLDOMNodeImpl<T,piid,tihclass>::cloneNode(VARIANT_BOOL deep, IXMLDOMNode **pVal)
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
	catch(DOM_DOMException& ex)
	{
		return MakeHRESULT(ex);
	}
	catch(...)
	{
		return E_FAIL;
	}
	

	return hr;
}

template <class T, const IID* piid, class tihclass>
STDMETHODIMP
IXMLDOMNodeImpl<T,piid,tihclass>::get_nodeTypeString(BSTR *pVal)
{
	ATLTRACE(_T("IXMLDOMNodeImpl::get_nodeTypeString\n"));

	USES_CONVERSION;

	if (NULL == pVal)
		return E_POINTER;

	*pVal = ::SysAllocString(g_DomNodeName[get_DOMNodeType()]);
	
	return S_OK;
}

template <class T, const IID* piid, class tihclass>
STDMETHODIMP
IXMLDOMNodeImpl<T,piid,tihclass>::get_text(BSTR *pVal)
{
	ATLTRACE(_T("IXMLDOMNodeImpl::get_text\n"));

	if (NULL == pVal)
		return E_POINTER;

	DOMString val = GetText(get_DOM_Node());
	*pVal = SysAllocStringLen(val.rawBuffer(),val.length());

	return S_OK;
}

template <class T, const IID* piid, class tihclass>
STDMETHODIMP
IXMLDOMNodeImpl<T,piid,tihclass>::put_text(BSTR newVal)
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
		if(NODE_ELEMENT == type)
		{
			//
			//   remove all child elements
			//
			DOM_Node elem = get_DOM_Node();
			DOM_Node child = elem.getLastChild();
			while(!child.isNull())
			{
				elem.removeChild(child);
				child = elem.getLastChild();
			}
			
			DOM_Document doc = elem.getOwnerDocument();
			elem.appendChild(doc.createTextNode(newVal));
		}
		else
			get_DOM_Node().setNodeValue(newVal);
	}
	catch(DOM_DOMException& ex)
	{
		return MakeHRESULT(ex);
	}
	catch(...)
	{
		return E_FAIL;
	}

	return S_OK;
}


template <class T, const IID* piid, class tihclass>
STDMETHODIMP
IXMLDOMNodeImpl<T,piid,tihclass>::get_specified(VARIANT_BOOL *pVal)
{
	ATLTRACE(_T("IXMLDOMNodeImpl::get_specified\n"));

	*pVal = VARIANT_TRUE;
	return S_OK;
}

template <class T, const IID* piid, class tihclass>
STDMETHODIMP
IXMLDOMNodeImpl<T,piid,tihclass>::get_definition(IXMLDOMNode * *pVal)
{	
	ATLTRACE(_T("IXMLDOMNodeImpl::get_definition\n"));

	return E_NOTIMPL;
}

template <class T, const IID* piid, class tihclass>
STDMETHODIMP
IXMLDOMNodeImpl<T,piid,tihclass>::get_nodeTypedValue(VARIANT *pVal)
{
	ATLTRACE(_T("IXMLDOMNodeImpl::get_nodeTypedValue\n"));

	return get_nodeValue(pVal);
}

template <class T, const IID* piid, class tihclass>
STDMETHODIMP
IXMLDOMNodeImpl<T,piid,tihclass>::put_nodeTypedValue(VARIANT newVal)
{	
	ATLTRACE(_T("IXMLDOMNodeImpl::put_nodeTypedValue\n"));

	return put_nodeValue(newVal);
}

template <class T, const IID* piid, class tihclass>
STDMETHODIMP
IXMLDOMNodeImpl<T,piid,tihclass>::get_dataType(VARIANT *pVal)
{
	ATLTRACE(_T("IXMLDOMNodeImpl::get_dataType\n"));

	return E_NOTIMPL;
}

template <class T, const IID* piid, class tihclass>
STDMETHODIMP
IXMLDOMNodeImpl<T,piid,tihclass>::put_dataType(BSTR dataTypeName)
{
	ATLTRACE(_T("IXMLDOMNodeImpl::put_dataType\n"));

	return E_NOTIMPL;
}

template <class T, const IID* piid, class tihclass>
STDMETHODIMP
IXMLDOMNodeImpl<T,piid,tihclass>::get_xml(BSTR *pVal)
{
	ATLTRACE(_T("IXMLDOMNodeImpl::get_xml\n"));

	if (NULL == pVal)
		return E_POINTER;

	*pVal = NULL;

	_bstr_t text;
	
	try {
		GetXML(get_DOM_Node(),text);
	}
	catch(DOM_DOMException& ex)
	{
		return MakeHRESULT(ex);
	}
	catch(...)
	{
		return E_FAIL;
	}

	*pVal = text.copy();
	
	return S_OK;
}

template <class T, const IID* piid, class tihclass>
STDMETHODIMP
IXMLDOMNodeImpl<T,piid,tihclass>::transformNode(IXMLDOMNode *stylesheet, BSTR *xmlString)
{
	ATLTRACE(_T("IXMLDOMNodeImpl::transformNode\n"));

	return E_NOTIMPL;
}

template <class T, const IID* piid, class tihclass>
STDMETHODIMP
IXMLDOMNodeImpl<T,piid,tihclass>::selectNodes(BSTR queryString, IXMLDOMNodeList * *resultList)
{
	ATLTRACE(_T("IXMLDOMNodeImpl::selectNodes\n"));

	return E_NOTIMPL;
}

template <class T, const IID* piid, class tihclass>
STDMETHODIMP
IXMLDOMNodeImpl<T,piid,tihclass>::selectSingleNode(BSTR queryString, IXMLDOMNode * *resultNode)
{
	ATLTRACE(_T("IXMLDOMNodeImpl::selectSingleNode\n"));

	return E_NOTIMPL;
}

template <class T, const IID* piid, class tihclass>
STDMETHODIMP
IXMLDOMNodeImpl<T,piid,tihclass>::get_parsed(VARIANT_BOOL *pVal)
{
	ATLTRACE(_T("IXMLDOMNodeImpl::get_parsed\n"));

	if (NULL == pVal)
		return E_POINTER;

	*pVal = VARIANT_TRUE;
	return S_OK;
}

template <class T, const IID* piid, class tihclass>
STDMETHODIMP
IXMLDOMNodeImpl<T,piid,tihclass>::get_namespaceURI(BSTR *pVal)
{
	ATLTRACE(_T("IXMLDOMNodeImpl::get_namespaceURI\n"));

	if (NULL == pVal)
		return E_POINTER;

	*pVal = NULL;

	try {
		DOMString val = get_DOM_Node().getNamespaceURI();
		*pVal = SysAllocStringLen(val.rawBuffer(),val.length());
	}
	catch(DOM_DOMException& ex)
	{
		return MakeHRESULT(ex);
	}
	catch(...)
	{
		return E_FAIL;
	}

	return S_OK;
}

template <class T, const IID* piid, class tihclass>
STDMETHODIMP
IXMLDOMNodeImpl<T,piid,tihclass>::get_prefix(BSTR *pVal)
{
	ATLTRACE(_T("IXMLDOMNodeImpl::get_prefix\n"));
	
	if (NULL == pVal)
		return E_POINTER;

	*pVal = NULL;

	try {
		DOMString val = get_DOM_Node().getPrefix();
		*pVal = SysAllocStringLen(val.rawBuffer(),val.length());
	}
	catch(DOM_DOMException& ex)
	{
		return MakeHRESULT(ex);
	}
	catch(...)
	{
		return E_FAIL;
	}

	return S_OK;
}

template <class T, const IID* piid, class tihclass>
STDMETHODIMP
IXMLDOMNodeImpl<T,piid,tihclass>::get_baseName(BSTR *pVal)
{
	ATLTRACE(_T("IXMLDOMNodeImpl::get_baseName\n"));

	if (NULL == pVal)
		return E_POINTER;

	*pVal = NULL;

	try {
		DOMString val = get_DOM_Node().getLocalName();
		*pVal = SysAllocStringLen(val.rawBuffer(),val.length());
	}
	catch(DOM_DOMException& ex)
	{
		return MakeHRESULT(ex);
	}
	catch(...)
	{
		return E_FAIL;
	}

	return S_OK;
}

template <class T, const IID* piid, class tihclass>
STDMETHODIMP
IXMLDOMNodeImpl<T,piid,tihclass>::transformNodeToObject(IXMLDOMNode *stylesheet, VARIANT outputObject)
{
	ATLTRACE(_T("IXMLDOMNodeImpl::transformNodeToObject\n"));

	return E_NOTIMPL;
}

