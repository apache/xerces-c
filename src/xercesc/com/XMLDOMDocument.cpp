/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 1999-2001 The Apache Software Foundation.  All rights
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

#include "stdafx.h"

#include <xercesc/dom/deprecated/DOMParser.hpp>
#include <xercesc/sax/SAXParseException.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>

#include "xml4com.h"
#include "XMLDOMDocument.h"
#include "XMLDOMUtil.h"
#include "XMLDOMNodeList.h"
#include "XMLDOMNamedNodeMap.h"
#include "XMLDOMDocumentType.h"
#include "XMLDOMImplementation.h"
#include "XMLDOMElement.h"
#include "XMLDOMDocumentFragment.h"
#include "XMLDOMText.h"
#include "XMLDOMComment.h"
#include "XMLDOMCDATASection.h"
#include "XMLDOMProcessingInstruction.h"
#include "XMLDOMAttribute.h"
#include "XMLDOMEntityReference.h"
#include "BindStatusCallback.h"
#include <xercesc/dom/deprecated/DocumentImpl.hpp>

// I need to make sure the file is registered with long filenames
HRESULT WINAPI CXMLDOMDocument::UpdateRegistry(BOOL bRegister)
{
	USES_CONVERSION;

	TCHAR file[MAX_PATH];
	if (::GetModuleFileName(_Module.m_hInst, file, MAX_PATH)) {
		WIN32_FIND_DATA d;
		memset(&d,0,sizeof(WIN32_FIND_DATA));
		HANDLE h = FindFirstFile(file,&d);
		if (h != INVALID_HANDLE_VALUE) {
			TCHAR  *name = _tcsrchr(file,_T('\\'));
			TCHAR newFile[MAX_PATH] = _T("");
			_tcsncpy(newFile,file,name-file);
			_tcscat(newFile,_T("\\"));
			_tcscat(newFile,d.cFileName);
			FindClose(h);
			
			_ATL_REGMAP_ENTRY regmap[2] = {{NULL,NULL},{NULL,NULL}};
			regmap[0].szKey  = OLESTR("XMLMODULE");
			regmap[0].szData = T2OLE(newFile);
			return _Module.UpdateRegistryFromResource((UINT) IDR_XMLDOCUMENT, bRegister,regmap);
		}
	}
	return E_FAIL;
}

CXMLDOMDocument::CXMLDOMDocument()
	:m_Document					()
	,m_bValidate				(false)
	,m_lReadyState				(0)
	,m_url						(_T(""))
	,m_pParseError				(NULL)
	,m_bAsync					(false)
	,m_bAbort					(false)
	,m_hParseThread				(NULL)
	,m_pOnReadyStateChange		(NULL)
	,m_pOnDataAvailable			(NULL)
	,m_pOnTransformNode			(NULL)
	,m_FileName					(_T(""))
	,m_xml						(_T(""))
	,m_bParseError				(false)
	,m_bThreadValidate			(false)
	,m_bPreserveWhiteSpace      (false)
{
}

HRESULT CXMLDOMDocument::FinalConstruct()
{
	// create monitor window
	RECT rc;
    memset(&rc,0,sizeof(RECT));
	if (NULL == Create(NULL, rc, _T("XML Parse Monitor Window"), 0))
		return E_FAIL;

	HRESULT hr = CXMLDOMParseErrorObj::CreateInstance(&m_pParseError);
	if (S_OK != hr)
		return hr;
	
	m_pParseError->AddRef();
	m_pIXMLDOMDocument = this;

	m_Document = DOM_Document::createDocument();

	return hr;
}

void CXMLDOMDocument::FinalRelease()
{
	if (NULL != m_hParseThread) {
		m_bAbort = true;
		::WaitForSingleObject(m_hParseThread, INFINITE);
		::CloseHandle(m_hParseThread);
		m_hParseThread = NULL;
	}

	if (m_pParseError != NULL) {
		m_pParseError->Release();
		m_pParseError = NULL;
	}
	if (m_pOnReadyStateChange != NULL) {
		m_pOnReadyStateChange->Release();
		m_pOnReadyStateChange = NULL;
	}
	if (m_pOnDataAvailable != NULL) {
		m_pOnDataAvailable->Release();
		m_pOnDataAvailable = NULL;
	}
	if (m_pOnTransformNode != NULL) {
		m_pOnTransformNode->Release();
		m_pOnTransformNode = NULL;
	}

	DestroyWindow();
}

void CXMLDOMDocument::warning(const SAXParseException& exc)
{
	// ignore warnings
}

void CXMLDOMDocument::error(const SAXParseException& exc)
{
	m_pParseError->SetData(1,
						   exc.getSystemId(),
						   exc.getMessage(),
						   _T(""),
						   exc.getLineNumber(),
						   exc.getColumnNumber(),
						   0);
	m_bParseError = true;
}

void CXMLDOMDocument::fatalError(const SAXParseException& exc)
{
	m_pParseError->SetData(1,
						   exc.getSystemId(),
						   exc.getMessage(),
						   _T(""),
						   exc.getLineNumber(),
						   exc.getColumnNumber(),
						   0);
	m_bParseError = true;
}

LRESULT CXMLDOMDocument::OnReadyStateChange(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	ATLTRACE(_T("CXMLDOMDocument::OnReadyStateChange\n"));

	bHandled = TRUE;

	m_lReadyState = wParam;
	Fire_onreadystatechange();
	if (NULL != m_pOnReadyStateChange) {
		CComVariant varResult;
		DISPPARAMS disp = { NULL, NULL, 0, 0 };
		m_pOnReadyStateChange->Invoke(DISPID_VALUE, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL);
	}
	if (m_lReadyState != 4)
		return 1L;

	m_Document = m_TmpDocument;
	m_TmpDocument = 0;
	
	m_url = m_FileName;

	if (!m_bParseError) {
		Fire_ondataavailable();
		if (NULL != m_pOnDataAvailable) {
			CComVariant varResult;
			DISPPARAMS disp = { NULL, NULL, 0, 0 };
			m_pOnDataAvailable->Invoke(DISPID_VALUE, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL);
		}
	}

	return 1L;
}

STDMETHODIMP CXMLDOMDocument::InterfaceSupportsErrorInfo(REFIID riid)
{
	if (IsEqualGUID(IID_IXMLDOMDocument,riid))
		return S_OK;
	return S_FALSE;
}

STDMETHODIMP CXMLDOMDocument::get_doctype(IXMLDOMDocumentType  **pVal)
{
	ATLTRACE(_T("CXMLDOMDocument::get_doctype\n"));

	if (NULL == pVal)
		return E_POINTER;

	*pVal = NULL;
	DOM_DocumentType doctype = m_Document.getDoctype();

	//
	//   if the document had no doctype then return a null object
	//
	if(doctype.isNull())
		return S_OK;

	CXMLDOMDocumentTypeObj *pObj = NULL;
	HRESULT hr = CXMLDOMDocumentTypeObj::CreateInstance(&pObj);
	if (S_OK != hr)
		return hr;
	
	pObj->AddRef();
	pObj->SetOwnerDoc(this);

	try
	{
		pObj->documentType = doctype;
	}
	catch(...)
	{
		pObj->Release();
		return E_FAIL;
	}
	
	hr = pObj->QueryInterface(IID_IXMLDOMDocumentType, reinterpret_cast<LPVOID*> (pVal));
	if (S_OK != hr)
		*pVal = NULL;

	pObj->Release();
	return hr;
}

STDMETHODIMP CXMLDOMDocument::get_implementation(IXMLDOMImplementation  **pVal)
{
	ATLTRACE(_T("CXMLDOMDocument::get_implementation\n"));

	if (NULL == pVal)
		return E_POINTER;

	*pVal = NULL;

	CXMLDOMImplementationObj *pObj = NULL;
	HRESULT hr = CXMLDOMImplementationObj::CreateInstance(&pObj);
	if (S_OK != hr)
		return hr;
	
	pObj->AddRef();

	hr = pObj->QueryInterface(IID_IXMLDOMImplementation, reinterpret_cast<LPVOID*> (pVal));
	if (S_OK != hr)
		*pVal = NULL;

	pObj->Release();
	return hr;
}

STDMETHODIMP CXMLDOMDocument::get_documentElement(IXMLDOMElement  * *pVal)
{
	ATLTRACE(_T("CXMLDOMDocument::get_documentElement\n"));

	if (NULL == pVal)
		return E_POINTER;

	*pVal = NULL;

	CXMLDOMElementObj *pObj = NULL;
	HRESULT hr = CXMLDOMElementObj::CreateInstance(&pObj);
	if (S_OK != hr)
		return S_OK;
	
	pObj->AddRef();
	pObj->SetOwnerDoc(this);

	try
	{
		pObj->element = m_Document.getDocumentElement();
	}
	catch(DOM_DOMException& ex)
	{
		pObj->Release();
		return MakeHRESULT(ex);
	}
	catch(...)
	{
		pObj->Release();
		return S_OK;
	}
	
	hr = pObj->QueryInterface(IID_IXMLDOMElement, reinterpret_cast<LPVOID*> (pVal));
	if (S_OK != hr)
		*pVal = NULL;

	pObj->Release();
	return S_OK;
}

STDMETHODIMP CXMLDOMDocument::putref_documentElement(IXMLDOMElement  *newVal)
{
	ATLTRACE(_T("CXMLDOMDocument::putref_documentElement\n"));

	DOM_Element elem;

	try
	{
		elem = m_Document.getDocumentElement();
		if (NULL == newVal && !elem.isNull()) {
			m_Document.removeChild(elem);
			return S_OK;
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
		
	CComQIPtr<IIBMXMLDOMNodeIdentity,&IID_IIBMXMLDOMNodeIdentity> pNewVal(newVal);
	if (!pNewVal)
		return E_NOINTERFACE;

	long id = 0;
	HRESULT hr = pNewVal->get_NodeId(&id);
	if (S_OK != hr)
		return hr;

	DOM_Node *pNewValNode = reinterpret_cast<DOM_Node*> (id);
	if (NULL == pNewValNode)
		return E_INVALIDARG;

	try
	{
		if(elem.isNull())
			m_Document.appendChild(*pNewValNode);
		else
			m_Document.replaceChild(*pNewValNode, elem);
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

STDMETHODIMP CXMLDOMDocument::createElement(BSTR tagName, IXMLDOMElement  **pVal)
{
	ATLTRACE(_T("CXMLDOMDocument::createElement\n"));

	if (NULL == pVal)
		return E_POINTER;

	*pVal = NULL;

	CXMLDOMElementObj *pObj = NULL;
	HRESULT hr = CXMLDOMElementObj::CreateInstance(&pObj);
	if (S_OK != hr)
		return hr;
	
	pObj->AddRef();
	pObj->SetOwnerDoc(this);

	try
	{
		pObj->element = m_Document.createElement(tagName);
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
	
	hr = pObj->QueryInterface(IID_IXMLDOMElement, reinterpret_cast<LPVOID*> (pVal));
	if (S_OK != hr)
		*pVal = NULL;

	pObj->Release();
	return hr;
}

STDMETHODIMP CXMLDOMDocument::createDocumentFragment(IXMLDOMDocumentFragment  **docFrag)
{
	ATLTRACE(_T("CXMLDOMDocument::createDocumentFragment\n"));

	if (NULL == docFrag)
		return E_POINTER;

	*docFrag = NULL;

	CXMLDOMDocumentFragmentObj *pObj = NULL;
	HRESULT hr = CXMLDOMDocumentFragmentObj::CreateInstance(&pObj);
	if (S_OK != hr)
		return hr;
	
	pObj->AddRef();
	pObj->SetOwnerDoc(this);

	try
	{
		pObj->documentFragment = m_Document.createDocumentFragment();
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
	
	hr = pObj->QueryInterface(IID_IXMLDOMDocumentFragment, reinterpret_cast<LPVOID*> (docFrag));
	if (S_OK != hr)
		*docFrag = NULL;

	pObj->Release();
	return hr;
}

STDMETHODIMP CXMLDOMDocument::createTextNode(BSTR data, IXMLDOMText  **pVal)
{
	ATLTRACE(_T("CXMLDOMDocument::createTextNode\n"));

	if (NULL == pVal)
		return E_POINTER;

	*pVal = NULL;

	CXMLDOMTextObj *pObj = NULL;
	HRESULT hr = CXMLDOMTextObj::CreateInstance(&pObj);
	if (S_OK != hr)
		return hr;
	
	pObj->AddRef();
	pObj->SetOwnerDoc(this);

	try
	{
		pObj->text = m_Document.createTextNode(data);
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
	
	hr = pObj->QueryInterface(IID_IXMLDOMText, reinterpret_cast<LPVOID*> (pVal));
	if (S_OK != hr)
		*pVal = NULL;

	pObj->Release();
	return hr;
}

STDMETHODIMP CXMLDOMDocument::createComment(BSTR data, IXMLDOMComment  **comment)
{
	ATLTRACE(_T("CXMLDOMDocument::createComment\n"));

	if (NULL == comment)
		return E_POINTER;

	*comment = NULL;

	CXMLDOMCommentObj *pObj = NULL;
	HRESULT hr = CXMLDOMCommentObj::CreateInstance(&pObj);
	if (S_OK != hr)
		return hr;
	
	pObj->AddRef();
	pObj->SetOwnerDoc(this);

	try
	{
		pObj->comment = m_Document.createComment(data);
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
	
	hr = pObj->QueryInterface(IID_IXMLDOMComment, reinterpret_cast<LPVOID*> (comment));
	if (S_OK != hr)
		*comment = NULL;

	pObj->Release();
	return hr;
}

STDMETHODIMP CXMLDOMDocument::createCDATASection(BSTR data, IXMLDOMCDATASection  **cdata)
{
	ATLTRACE(_T("CXMLDOMDocument::createCDATASection\n"));

	if (NULL == cdata)
		return E_POINTER;

	*cdata = NULL;

	CXMLDOMCDATASectionObj *pObj = NULL;
	HRESULT hr = CXMLDOMCDATASectionObj::CreateInstance(&pObj);
	if (S_OK != hr)
		return hr;
	
	pObj->AddRef();
	pObj->SetOwnerDoc(this);

	try
	{
		pObj->cdataSection = m_Document.createCDATASection(data);
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
	
	hr = pObj->QueryInterface(IID_IXMLDOMCDATASection, reinterpret_cast<LPVOID*> (cdata));
	if (S_OK != hr)
		*cdata = NULL;

	pObj->Release();
	return hr;
}

STDMETHODIMP CXMLDOMDocument::createProcessingInstruction(BSTR target, BSTR data, IXMLDOMProcessingInstruction  **pVal)
{
	ATLTRACE(_T("CXMLDOMDocument::createProcessingInstruction\n"));

	if (NULL == pVal)
		return E_POINTER;

	*pVal = NULL;

	CXMLDOMProcessingInstructionObj *pObj = NULL;
	HRESULT hr = CXMLDOMProcessingInstructionObj::CreateInstance(&pObj);
	if (S_OK != hr)
		return hr;
	
	pObj->AddRef();
	pObj->SetOwnerDoc(this);

	try
	{
		pObj->processingInstruction = m_Document.createProcessingInstruction(target, data);
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
	
	hr = pObj->QueryInterface(IID_IXMLDOMProcessingInstruction, reinterpret_cast<LPVOID*> (pVal));
	if (S_OK != hr)
		*pVal = NULL;

	pObj->Release();
	return hr;
}

STDMETHODIMP CXMLDOMDocument::createAttribute(BSTR name, IXMLDOMAttribute  **attr)
{
	ATLTRACE(_T("CXMLDOMDocument::createAttribute\n"));

	if (NULL == attr)
		return E_POINTER;

	*attr = NULL;

	CXMLDOMAttributeObj *pObj = NULL;
	HRESULT hr = CXMLDOMAttributeObj::CreateInstance(&pObj);
	if (S_OK != hr)
		return hr;
	
	pObj->AddRef();
	pObj->SetOwnerDoc(this);

	try
	{
		pObj->attr = m_Document.createAttribute(name);
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
	
	hr = pObj->QueryInterface(IID_IXMLDOMAttribute, reinterpret_cast<LPVOID*> (attr));
	if (S_OK != hr)
		*attr = NULL;

	pObj->Release();
	return hr;
}

STDMETHODIMP CXMLDOMDocument::createEntityReference(BSTR name, IXMLDOMEntityReference  **entityRef)
{
	ATLTRACE(_T("CXMLDOMDocument::createEntityReference\n"));

	if (NULL == entityRef)
		return E_POINTER;

	*entityRef = NULL;

	CXMLDOMEntityReferenceObj *pObj = NULL;
	HRESULT hr = CXMLDOMEntityReferenceObj::CreateInstance(&pObj);
	if (S_OK != hr)
		return hr;
	
	pObj->AddRef();
	pObj->SetOwnerDoc(this);

	try
	{
		pObj->entityReference = m_Document.createEntityReference(name);
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
	
	hr = pObj->QueryInterface(IID_IXMLDOMEntityReference, reinterpret_cast<LPVOID*> (entityRef));
	if (S_OK != hr)
		*entityRef = NULL;

	pObj->Release();
	return hr;
}

STDMETHODIMP CXMLDOMDocument::getElementsByTagName(BSTR tagName, IXMLDOMNodeList  **pVal)
{
	ATLTRACE(_T("CXMLDOMDocument::getElementsByTagName\n"));

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
		pObj->m_container = m_Document.getElementsByTagName(tagName);
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

STDMETHODIMP CXMLDOMDocument::createNode(VARIANT TYPE, BSTR name, BSTR namespaceURI, IXMLDOMNode  **pVal)
{
	ATLTRACE(_T("CXMLDOMDocument::createNode\n"));

	HRESULT hr = S_OK;
	if (NULL == pVal)
		return E_POINTER;

	*pVal = NULL;

	VARIANT type;
	::VariantInit(&type);
	if (V_VT(&TYPE) != VT_I4 && V_VT(&TYPE) != VT_BSTR)
		hr = ::VariantChangeType(&type,&TYPE,0,VT_I4); 	
	else
		hr = ::VariantCopy(&type,&TYPE);
	
	if (S_OK != hr)
		return hr;

	DOMNodeType nodeType = NODE_INVALID;
	if (VT_I4 == V_VT(&type))
		nodeType = static_cast<DOMNodeType> (V_I4(&type));
	else {
		OLECHAR* str = V_BSTR(&type);
		for (int i = 0; i < g_DomNodeNameSize; ++i) {
			if (0 == _wcsicmp(str,g_DomNodeName[i])) {
				nodeType = static_cast<DOMNodeType> (i);
				break;
			}
		}
	}

	::VariantClear(&type);
	
	if (NODE_INVALID		== nodeType ||
		NODE_DOCUMENT		== nodeType	||
		NODE_DOCUMENT_TYPE	== nodeType	||
		NODE_ENTITY			== nodeType ||
		NODE_NOTATION		== nodeType)
		return E_FAIL;

	try
	{
	switch(nodeType) {
		case NODE_ELEMENT:
		{
			DOM_Element node;
			if (SysStringLen(namespaceURI) > 0)
				node = m_Document.createElementNS(namespaceURI,name);
			else
				node = m_Document.createElement(name);
			
			hr = wrapNode(m_pIXMLDOMDocument,node,IID_IXMLDOMNode,reinterpret_cast<LPVOID *> (pVal));
			break;
		}
		case NODE_ATTRIBUTE:
		{
			DOM_Attr node;
			if (SysStringLen(namespaceURI) > 0)
				node = m_Document.createAttributeNS(namespaceURI,name);
			else
				node = m_Document.createAttribute(name);

			hr = wrapNode(m_pIXMLDOMDocument,node,IID_IXMLDOMNode,reinterpret_cast<LPVOID *> (pVal));
			break;
		}
		case NODE_TEXT:
		{
			DOM_Text node = m_Document.createTextNode(OLESTR(""));
			hr = wrapNode(m_pIXMLDOMDocument,node,IID_IXMLDOMNode,reinterpret_cast<LPVOID *> (pVal));
			break;
		}
		case NODE_CDATA_SECTION:
		{
			DOM_CDATASection node = m_Document.createCDATASection(OLESTR(""));
			hr = wrapNode(m_pIXMLDOMDocument,node,IID_IXMLDOMNode,reinterpret_cast<LPVOID *> (pVal));
			break;
		}
		case NODE_ENTITY_REFERENCE:
		{
			DOM_EntityReference node = m_Document.createEntityReference(name);
			hr = wrapNode(m_pIXMLDOMDocument,node,IID_IXMLDOMNode,reinterpret_cast<LPVOID *> (pVal));
			break;
		}
		case NODE_PROCESSING_INSTRUCTION:
		{
			DOM_ProcessingInstruction node = m_Document.createProcessingInstruction(name,OLESTR(""));
			hr = wrapNode(m_pIXMLDOMDocument,node,IID_IXMLDOMNode,reinterpret_cast<LPVOID *> (pVal));
			break;
		}
		case NODE_COMMENT:
		{
			DOM_Comment node = m_Document.createComment(OLESTR(""));
			hr = wrapNode(m_pIXMLDOMDocument,node,IID_IXMLDOMNode,reinterpret_cast<LPVOID *> (pVal));
			break;
		}
		case NODE_DOCUMENT_FRAGMENT:
		{
			DOM_DocumentFragment node = m_Document.createDocumentFragment();
			hr = wrapNode(m_pIXMLDOMDocument,node,IID_IXMLDOMNode,reinterpret_cast<LPVOID *> (pVal));
			break;
		}
		default:
			hr = E_FAIL;
			break;
	}
	}
	catch(DOM_DOMException& ex)
	{
		hr = MakeHRESULT(ex);
	}
	catch (...)
	{
		hr = E_FAIL;
	}
	
	return hr;
}

STDMETHODIMP CXMLDOMDocument::nodeFromID(BSTR idString, IXMLDOMNode  **pVal)
{
	ATLTRACE(_T("CXMLDOMDocument::nodeFromID\n"));

	if (NULL == pVal)
		return E_POINTER;

	*pVal = NULL;
	HRESULT hr = S_OK;

	try
	{
		hr = wrapNode(m_pIXMLDOMDocument, m_Document.getElementById(idString),IID_IXMLDOMNode,reinterpret_cast<LPVOID *> (pVal));
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

STDMETHODIMP CXMLDOMDocument::load(VARIANT xmlSource, VARIANT_BOOL  *isSuccessful)
{
	ATLTRACE(_T("CXMLDOMDocument::load\n"));

	if (NULL == isSuccessful)
		return E_POINTER;

	*isSuccessful = VARIANT_FALSE;

	if (V_VT(&xmlSource) != VT_BSTR					&&
		V_VT(&xmlSource) != VT_DISPATCH				&&
		V_VT(&xmlSource) != (VT_ARRAY | VT_VARIANT)	&&
		V_VT(&xmlSource) != (VT_ARRAY | VT_UI1)		&&
		V_VT(&xmlSource) != VT_UNKNOWN)
		return E_INVALIDARG;

	// do not start another thread if there is another active
	if (NULL != m_hParseThread) {
		DWORD exitCode = 0;
		BOOL rc = ::GetExitCodeThread(m_hParseThread, &exitCode);
		if (!rc || STILL_ACTIVE == exitCode)
			return S_OK;
		
		::CloseHandle(m_hParseThread);
		m_hParseThread = NULL;
	}

	HRESULT hr = S_OK;
	m_bAbort = false;
	
	m_FileName = _T("");
	m_xml = _T("");
	m_TmpDocument = 0;
	m_bThreadValidate = m_bValidate;
	
	if (V_VT(&xmlSource) == VT_BSTR) {
		m_FileName = V_BSTR(&xmlSource);
		if (0 == m_FileName.length())
			return E_INVALIDARG;

		// see if the file is relative path
		if (!PathIsURL(m_FileName) && PathIsRelative(m_FileName)) {
			// try appending baseurl if exists
			_bstr_t baseURL;
			if (S_OK == GetBaseURL(baseURL)) {
				// change any backslashes to slashes
				LPTSTR loc = _tcschr(m_FileName,_T('\\'));
				while (loc != NULL) {
					*loc = _T('/');
					loc = _tcschr(m_FileName,_T('\\'));
				}
				m_FileName = baseURL + _T("/") + m_FileName;
			}
			else {
				TCHAR szCurDir[MAX_PATH];
				GetCurrentDirectory(MAX_PATH,szCurDir);
				m_FileName=_bstr_t(szCurDir) + _T("\\") + m_FileName;
			}
		}
	}
	else
	if (V_VT(&xmlSource) == VT_UNKNOWN) {
		CComQIPtr<IStream,&IID_IStream> pS(V_UNKNOWN(&xmlSource));
		if (!pS)
			return E_INVALIDARG;

		CComBSTR b;
		hr = b.ReadFromStream(pS);
		if (S_OK != hr)
			return hr;

		m_xml = b;
		if (0 == m_xml.length())
			return E_INVALIDARG;
	}
	else
	if (V_VT(&xmlSource) == VT_DISPATCH) {
		CComQIPtr<IXMLDOMDocument,&IID_IXMLDOMDocument> pDoc(V_DISPATCH(&xmlSource));
		if (!pDoc)
			return E_INVALIDARG;
		
		BSTR b = NULL;
		hr = pDoc->get_xml(&b);
		if (S_OK != hr)
			return hr;

		m_xml = b;
		::SysFreeString(b);

		if (0 == m_xml.length())
			return E_INVALIDARG;
	}
	else
	if (V_VT(&xmlSource) == (VT_ARRAY | VT_VARIANT)) {
		SAFEARRAY *pArray = reinterpret_cast<SAFEARRAY *> (xmlSource.byref);
		if (NULL == pArray)
			return E_INVALIDARG;

		long lLBoundVar = 0;
		long lUBoundVar = 0;
	
		UINT dims = ::SafeArrayGetDim(pArray);
		if (dims == 0)
			return E_INVALIDARG;
	
		hr = ::SafeArrayGetLBound(pArray, dims, &lLBoundVar);
		if (S_OK != hr)
			return hr;

		hr = ::SafeArrayGetUBound(pArray, dims, &lUBoundVar);
		if (S_OK != hr)
			return hr;

		if (lUBoundVar >= lLBoundVar) {
			VARIANT *pIndex = NULL;
			hr = ::SafeArrayAccessData(pArray, reinterpret_cast<void **> (&pIndex));
			if (S_OK != hr)
				return hr;

			int length = lUBoundVar-lLBoundVar+2;
			BYTE *body = new BYTE[length];
			for (long i = 0; i <= lUBoundVar-lLBoundVar; ++i) {	
				VARIANT var = pIndex[i];
				if (V_VT(&var) != VT_UI1) {
					hr = E_INVALIDARG;
					break;
				}
				body[i] = V_UI1(&var);
			}
			body[length-1] = 0;
					
			::SafeArrayUnaccessData(pArray);
			if (S_OK != hr) {
				delete [] body;
				return hr;
			}
			m_xml = reinterpret_cast<char*> (body);
			delete [] body;
			if (0 == m_xml.length())
				return E_INVALIDARG;
		}
	}	
	else
	if (V_VT(&xmlSource) == (VT_ARRAY | VT_UI1)) {
		SAFEARRAY *pArray = reinterpret_cast<SAFEARRAY *> (xmlSource.byref);
		if (NULL == pArray)
			return E_INVALIDARG;

		long lLBoundVar = 0;
		long lUBoundVar = 0;
	
		UINT dims = ::SafeArrayGetDim(pArray);
		if (dims == 0)
			return E_INVALIDARG;
	
		hr = ::SafeArrayGetLBound(pArray, dims, &lLBoundVar);
		if (S_OK != hr)
			return hr;

		hr = ::SafeArrayGetUBound(pArray, dims, &lUBoundVar);
		if (S_OK != hr)
			return hr;

		if (lUBoundVar >= lLBoundVar) {
			BYTE *pIndex = NULL;
			hr = ::SafeArrayAccessData(pArray, reinterpret_cast<void **> (&pIndex));
			if (S_OK != hr)
				return hr;

			int length = lUBoundVar-lLBoundVar+2;
			BYTE *body = new BYTE[length];
			for (long i = 0; i <= lUBoundVar-lLBoundVar; ++i)	
				body[i] = pIndex[i];
			
			body[length-1] = 0;
			::SafeArrayUnaccessData(pArray);
			m_xml = reinterpret_cast<char*> (body);
			delete [] body;
			if (0 == m_xml.length())
				return E_INVALIDARG;
		}
	}	

	UINT nthreadID = 0;
	m_hParseThread = reinterpret_cast<HANDLE> (_beginthreadex(NULL,
												 0,
											     CXMLDOMDocument::ParseThread,
												 (void *) this,
												 0,
												 &nthreadID));
	if (NULL == m_hParseThread)
		return S_OK;
	
	if (m_bAsync) {
		*isSuccessful = VARIANT_TRUE;
		return S_OK;
	}

	bool bWait = true;
	while (bWait) {
		DWORD dwEvt = MsgWaitForMultipleObjects(1,&m_hParseThread,FALSE,INFINITE,QS_ALLINPUT);
		switch(dwEvt) {
			case WAIT_OBJECT_0:
				bWait = false;
				break;
			case WAIT_OBJECT_0 + 1:
			{
				MSG msg;
				while(::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {
					if (WM_CLOSE == msg.message || WM_QUIT == msg.message) {
						 bWait = false;
						 m_bAbort = true;
						 break;
					}
					else {
						PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
						TranslateMessage(&msg);
						DispatchMessage(&msg);
					}
				}
				break;
			}
			default:
				m_bAbort = true;
				bWait = false;
				break;
		}
	}

	if (m_bAbort)
		return S_OK;

	if (m_bParseError)
		return hr;

	m_Document = m_TmpDocument;
	m_TmpDocument = 0;
	
	m_url = m_FileName;
	*isSuccessful = VARIANT_TRUE;
	
	return hr;
}

HRESULT CXMLDOMDocument::GetBaseURL(_bstr_t &baseURL)
{
	baseURL = _T("");
	CComPtr<IServiceProvider> pSP;
	HRESULT hr = GetSite(IID_IServiceProvider, reinterpret_cast<LPVOID *> (&pSP));
    if (S_OK != hr)
		return hr;

	CComPtr<IWebBrowser2> pWB;
    hr = pSP->QueryService(SID_SWebBrowserApp,IID_IWebBrowser2,
                                 reinterpret_cast<LPVOID *> (&pWB));
	if (S_OK != hr)
		return hr;

	BSTR b = NULL;
	hr = pWB->get_LocationURL(&b);
	if (S_OK != hr)
		return hr;

	TCHAR url[MAX_PATH] = _T("");
	lstrcat(url,_bstr_t(b));

	// look for last slash and remove the filename after it
	LPTSTR loc = _tcsrchr(url, _T('/'));
	if (loc != NULL) {
		TCHAR str[MAX_PATH] = _T("");
		_tcsncpy(str, url, loc - url);
		lstrcpy(url,str);
	}

	baseURL = url;
	return hr;
}

STDMETHODIMP CXMLDOMDocument::get_readyState(long  *pVal)
{
	ATLTRACE(_T("CXMLDOMDocument::get_readyState\n"));

	if (NULL == pVal)
		return E_POINTER;

	*pVal = m_lReadyState;

	return S_OK;
}

STDMETHODIMP CXMLDOMDocument::get_parseError(IXMLDOMParseError  **pVal)
{
	ATLTRACE(_T("CXMLDOMDocument::get_parseError\n"));

	if (NULL == pVal)
		return E_POINTER;

	return m_pParseError->QueryInterface(IID_IXMLDOMParseError, reinterpret_cast<LPVOID*> (pVal));
}

STDMETHODIMP CXMLDOMDocument::get_url(BSTR  *pVal)
{
	ATLTRACE(_T("CXMLDOMDocument::get_url\n"));

	if (NULL == pVal)
		return E_POINTER;

	*pVal = m_url.copy();
	return S_OK;
}

STDMETHODIMP CXMLDOMDocument::get_async(VARIANT_BOOL  *pVal)
{
	ATLTRACE(_T("CXMLDOMDocument::get_async\n"));
	
	if (NULL == pVal)
		return E_POINTER;

	*pVal = m_bAsync ? VARIANT_TRUE : VARIANT_FALSE;

	return S_OK;
}

STDMETHODIMP CXMLDOMDocument::put_async(VARIANT_BOOL newVal)
{
	ATLTRACE(_T("CXMLDOMDocument::put_async\n"));

	m_bAsync = (VARIANT_TRUE == newVal) ? true : false;

	return S_OK;
}

STDMETHODIMP CXMLDOMDocument::abort(void)
{
	ATLTRACE(_T("CXMLDOMDocument::abort\n"));
	m_bAbort = true;
	return S_OK;
}

STDMETHODIMP CXMLDOMDocument::loadXML(BSTR bstrXML, VARIANT_BOOL  *isSuccessful)
{
	ATLTRACE(_T("CXMLDOMDocument::loadXML\n"));

	if (NULL == isSuccessful)
		return E_POINTER;

	*isSuccessful = VARIANT_FALSE;

	// do not start another thread if there is another active
	if (NULL != m_hParseThread) {
		DWORD exitCode = 0;
		BOOL rc = ::GetExitCodeThread(m_hParseThread, &exitCode);
		if (!rc || STILL_ACTIVE == exitCode)
			return S_OK;
		
		::CloseHandle(m_hParseThread);
		m_hParseThread = NULL;
	}

	HRESULT hr = S_OK;
	m_bAbort = false;
	
	m_FileName = _T("");
	m_TmpDocument = 0;
	m_bThreadValidate = m_bValidate;
	
	m_xml = bstrXML;
	if (0 == m_xml.length())
		return E_INVALIDARG;

	UINT nthreadID = 0;
	m_hParseThread = reinterpret_cast<HANDLE> (_beginthreadex(NULL,
												 0,
											     CXMLDOMDocument::ParseThread,
												 (void *) this,
												 0,
												 &nthreadID));
	if (NULL == m_hParseThread)
		return S_OK;
	
	if (m_bAsync) {
		*isSuccessful = VARIANT_TRUE;
		return S_OK;
	}

	bool bWait = true;
	while (bWait) {
		DWORD dwEvt = MsgWaitForMultipleObjects(1,&m_hParseThread,FALSE,INFINITE,QS_ALLINPUT);
		switch(dwEvt) {
			case WAIT_OBJECT_0:
				bWait = false;
				break;
			case WAIT_OBJECT_0 + 1:
			{
				MSG msg;
				while(::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {
					if (WM_CLOSE == msg.message || WM_QUIT == msg.message) {
						 bWait = false;
						 m_bAbort = true;
						 break;
					}
					else {
						PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
						TranslateMessage(&msg);
						DispatchMessage(&msg);
					}
				}
				break;
			}
			default:
				m_bAbort = true;
				bWait = false;
				break;
		}
	}

	if (m_bAbort)
		return S_OK;

	if (m_bParseError)
		return hr;

	m_Document = m_TmpDocument;
	m_TmpDocument = 0;
	
	m_url = m_FileName;
	*isSuccessful = VARIANT_TRUE;
	
	return hr;
}

UINT APIENTRY CXMLDOMDocument::ParseThread(void *pParm)
{
	ATLTRACE(_T("CXMLDOMDocument::ParseThread\n"));

	CXMLDOMDocument *pThis = reinterpret_cast<CXMLDOMDocument *> (pParm);
	if (NULL == pThis)
		return 0;

	if (!pThis->m_bAbort && pThis->m_FileName.length() > 0) {
		CBindStatCallbackObj *pCallbackObj = NULL;
		HRESULT hr = CBindStatCallbackObj::CreateInstance(&pCallbackObj);
		if (S_OK != hr)
			pCallbackObj = NULL;

		if (pCallbackObj != NULL) {
			pCallbackObj->AddRef();
			pCallbackObj->m_pDoc = pThis;	
		}
		TCHAR name[MAX_PATH] = _T("");
		if (pThis->m_bAsync)
			pThis->PostMessage(MSG_READY_STATE_CHANGE,1);
		
		hr = URLDownloadToCacheFile(NULL,pThis->m_FileName,name,URLOSTRM_GETNEWESTVERSION,0,pCallbackObj);
		if (pCallbackObj != NULL)
			pCallbackObj->Release();

		if (E_ABORT == hr)
			pThis->m_bAbort = true;
		else {	
			if (S_OK != hr) {
				_bstr_t error = _T("Failed to download ") + pThis->m_FileName + _T(": ");
				_com_error comError(hr);
				error += comError.ErrorMessage();
				pThis->m_pParseError->SetData(1,pThis->m_FileName,error,_T(""),0,0,0);
				pThis->m_bParseError = true;
			}
		}

		if (S_OK == hr) {
			pThis->m_FileName = name;
			if (pThis->m_bAsync)
				pThis->PostMessage(MSG_READY_STATE_CHANGE,2);
		}
	}

	DOMParser parser;

	//
	//   If set to true then an node supporting IXMLDOMProcessingInstruction
	//     is added for the XML declaration.
	//
	//   Setting to true in a custom DLL will better mimic
	//      MSXML.DLL but at a cost of conformance errors
	//      using David Brownell's suite
	parser.setToCreateXMLDeclTypeNode(false);

	parser.setIncludeIgnorableWhitespace(pThis->m_bPreserveWhiteSpace);



	if (!pThis->m_bParseError && !pThis->m_bAbort) {
		parser.setDoValidation(pThis->m_bThreadValidate);
		//
		//   this brings the COM component into better mimicry to MSXML
		//      by not throwing a validation error when there is no DOCTYPE
		//
		parser.setValidationScheme(pThis->m_bThreadValidate ? DOMParser::Val_Auto : DOMParser::Val_Never);
	}

	if (!pThis->m_bParseError && !pThis->m_bAbort)
		parser.setErrorHandler(pThis);

	if (!pThis->m_bParseError && !pThis->m_bAbort)
		pThis->m_pParseError->Reset();
	
	if (!pThis->m_bParseError && !pThis->m_bAbort)
		pThis->m_bParseError = false;

	try
	{
		if (!pThis->m_bParseError && !pThis->m_bAbort) {
			if (pThis->m_FileName.length() > 0)
				parser.parse(static_cast<LPCTSTR> (pThis->m_FileName));
			else {
				XMLByte *pXMLByte =  reinterpret_cast<XMLByte*> (static_cast<XMLCh*>(pThis->m_xml));
				MemBufInputSource memBufIS(pXMLByte,pThis->m_xml.length()*sizeof(XMLCh),OLESTR("IBMXMLParser"),false);
				memBufIS.setEncoding(OLESTR("UTF-16LE"));
				if (!pThis->m_bParseError && !pThis->m_bAbort)
					parser.parse(memBufIS);
			}
		}

	}
	catch(...)
	{
		pThis->m_bParseError = true;
		return 0;
	}

	if (!pThis->m_bParseError && !pThis->m_bAbort)
		pThis->m_TmpDocument = parser.getDocument();

	if (!pThis->m_bParseError && !pThis->m_bAbort && pThis->m_bAsync)
		pThis->PostMessage(MSG_READY_STATE_CHANGE,4);
  	
	return 0;
}

STDMETHODIMP CXMLDOMDocument::save(VARIANT location)
{
	ATLTRACE(_T("CXMLDOMDocument::save\n"));

	if (V_VT(&location) != VT_BSTR)
		return E_INVALIDARG;

	_bstr_t file(V_BSTR(&location));
	if (0 == file.length())
		return E_INVALIDARG;

	_bstr_t text;
	
	try {
		DOM_NodeList childs = m_Document.getChildNodes();
		int length = childs.getLength();
		for (int i=0; i < length; ++i) {
			DOM_Node child = childs.item(i);
			_bstr_t nodeText;
			GetXML(child,nodeText);
			text += nodeText;
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

	FILE* fp;
	if ((fp = _tfopen(file, "wt")) == NULL) {
		return E_FAIL;
	}

	if(text.length()>0)
		_fputts(text, fp);
	
	fclose(fp);

	return S_OK;
}

STDMETHODIMP CXMLDOMDocument::get_validateOnParse(VARIANT_BOOL  *pVal)
{
	ATLTRACE(_T("CXMLDOMDocument::get_validateOnParse\n"));

	if (NULL == pVal)
		return E_POINTER;

	*pVal = (m_bValidate) ? VARIANT_TRUE : VARIANT_FALSE;
	return S_OK;
}

STDMETHODIMP CXMLDOMDocument::put_validateOnParse(VARIANT_BOOL newVal)
{
	ATLTRACE(_T("CXMLDOMDocument::put_validateOnParse\n"));
	m_bValidate = (VARIANT_TRUE == newVal) ? true : false;
	return S_OK;
}

STDMETHODIMP CXMLDOMDocument::get_resolveExternals(VARIANT_BOOL  *pVal)
{
	ATLTRACE(_T("CXMLDOMDocument::get_resolveExternals\n"));

	if (NULL == pVal)
		return E_POINTER;

	return E_NOTIMPL;
}

STDMETHODIMP CXMLDOMDocument::put_resolveExternals(VARIANT_BOOL newVal)
{
	ATLTRACE(_T("CXMLDOMDocument::put_resolveExternals\n"));
	return E_NOTIMPL;
}

STDMETHODIMP CXMLDOMDocument::get_preserveWhiteSpace(VARIANT_BOOL  *pVal)
{
	ATLTRACE(_T("CXMLDOMDocument::get_preserveWhiteSpace\n"));
	if (NULL == pVal)
		return E_POINTER;

	*pVal = (m_bPreserveWhiteSpace) ? VARIANT_TRUE : VARIANT_FALSE;

	return S_OK;
}

STDMETHODIMP CXMLDOMDocument::put_preserveWhiteSpace(VARIANT_BOOL newVal)
{	
	ATLTRACE(_T("CXMLDOMDocument::put_preserveWhiteSpace\n"));
	m_bPreserveWhiteSpace = (VARIANT_TRUE == newVal) ? true : false;
	return S_OK;

}

STDMETHODIMP CXMLDOMDocument::put_onreadystatechange(VARIANT newVal)
{
	ATLTRACE(_T("CXMLDOMDocument::put_onreadystatechange\n"));

	if (VT_DISPATCH != V_VT(&newVal) &&
		VT_NULL		!= V_VT(&newVal))
		return E_INVALIDARG;

	LPDISPATCH pDisp = NULL;
	if (VT_DISPATCH == V_VT(&newVal))
		pDisp = V_DISPATCH(&newVal);	

	if (m_pOnReadyStateChange != NULL) {
		m_pOnReadyStateChange->Release();
		m_pOnReadyStateChange = NULL;
	}
	m_pOnReadyStateChange = pDisp;
	if (m_pOnReadyStateChange != NULL)
		m_pOnReadyStateChange->AddRef();

	return S_OK;
}

STDMETHODIMP CXMLDOMDocument::put_ondataavailable(VARIANT newVal)
{
	ATLTRACE(_T("CXMLDOMDocument::put_ondataavailable\n"));
	
	if (VT_DISPATCH != V_VT(&newVal) &&
		VT_NULL		!= V_VT(&newVal))
		return E_INVALIDARG;

	LPDISPATCH pDisp = NULL;
	if (VT_DISPATCH == V_VT(&newVal))
		pDisp = V_DISPATCH(&newVal);

	if (m_pOnDataAvailable != NULL) {
		m_pOnDataAvailable->Release();
		m_pOnDataAvailable = NULL;
	}
	m_pOnDataAvailable = pDisp;
	if (m_pOnDataAvailable != NULL)
		m_pOnDataAvailable->AddRef();
	
	return S_OK;
}

STDMETHODIMP CXMLDOMDocument::put_ontransformnode(VARIANT newVal)
{
	ATLTRACE(_T("CXMLDOMDocument::put_ontransformnode\n"));

	if (VT_DISPATCH != V_VT(&newVal) &&
		VT_NULL		!= V_VT(&newVal))
		return E_INVALIDARG;

	LPDISPATCH pDisp = NULL;
	if (VT_DISPATCH == V_VT(&newVal))
		pDisp = V_DISPATCH(&newVal);

	if (m_pOnTransformNode != NULL) {
		m_pOnTransformNode->Release();
		m_pOnTransformNode = NULL;
	}
	m_pOnTransformNode = pDisp;
	if (m_pOnTransformNode != NULL)
		m_pOnTransformNode->AddRef();

	return S_OK;
}

