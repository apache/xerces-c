/*
 * Copyright 1999-2001,2004 The Apache Software Foundation.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *      http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * $Log$
 * Revision 1.10  2004/10/01 08:10:38  amassari
 * Updated version numbers to 2.6; use version macros when possible
 *
 * Revision 1.9  2004/09/08 13:55:35  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.8  2004/02/25 18:38:33  amassari
 * The COM wrapper doesn't use the deprecated DOM anymore
 *
 * Revision 1.7  2004/02/12 13:49:43  amassari
 * Updated version to 2.5
 *
 * Revision 1.6  2003/12/02 14:33:13  amassari
 * Don't use the word "exception" as a variable name, as VC 7.1 complains about it
 *
 * Revision 1.5  2003/11/21 12:05:48  amassari
 * Updated version to 2.4
 *
 * Revision 1.4  2003/10/21 21:21:32  amassari
 * When the COM object is loaded by a late-binding engine (like WSH, or
 * Visual Basic when the type library is not preloaded in the editor), the type
 * library version stored in the resource must match the version specified in the
 * IDispatchImpl template (defaulted to 1.0), or trying to invoke a method will fail
 *
 * Revision 1.3  2003/03/14 12:44:49  tng
 * [Bug 17147] C++ namespace breaks build of XercesCOM DLL
 *
 * Revision 1.2  2002/05/21 19:53:53  tng
 * DOM Reorganization: update include path for the old DOM interface in COM files
 *
 * Revision 1.1.1.1  2002/02/01 22:21:40  peiyongz
 * sane_include
 *
 * Revision 1.6  2001/05/11 13:25:03  tng
 * Copyright update.
 *
 * Revision 1.5  2001/01/19 15:18:10  tng
 * COM Updates by Curt Arnold: changed 1.3 to 1.4, updated the GUID's so
 * both can coexist and fixed a new minor bugs.  Most of the changes involved
 * error reporting, now a DOM defined error will return an HRESULT of
 * 0x80040600 + code and will set an error description to the error name.
 *
 * Revision 1.4  2000/06/19 20:05:58  rahulj
 * Changes for increased conformance and stability. Submitted by
 * Curt.Arnold@hyprotech.com. Verified by Joe Polastre.
 *
 * Revision 1.3  2000/06/03 00:28:57  andyh
 * COM Wrapper changes from Curt Arnold
 *
 * Revision 1.2  2000/03/30 02:00:11  abagchi
 * Initial checkin of working code with Copyright Notice
 *
 */

#ifndef ___xmldomdocument_h___
#define ___xmldomdocument_h___

#include <xercesc/dom/DOMDocument.hpp>
#include "IXMLDOMNodeImpl.h"

#include "resource.h"       // main symbols
#include "XMLDOMParseError.h"

#include <xercesc/sax/ErrorHandler.hpp>
#include "xml4comCP.h"

XERCES_CPP_NAMESPACE_USE

class XERCES_CPP_NAMESPACE_QUALIFIER SAXParseException;

class ATL_NO_VTABLE CXMLDOMDocument :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CXMLDOMDocument, &CLSID_DOMDocument>,
	public IObjectSafetyImpl<CXMLDOMDocument, INTERFACESAFE_FOR_UNTRUSTED_CALLER>,
	public IXMLDOMNodeImpl<IXMLDOMDocument, &IID_IXMLDOMDocument>,
	public IObjectWithSiteImpl<CXMLDOMDocument>,
	public CProxyXMLDOMDocumentEvents< CXMLDOMDocument >,
	public IConnectionPointContainerImpl<CXMLDOMDocument>,
	public IProvideClassInfo2Impl<&CLSID_DOMDocument, &DIID_XMLDOMDocumentEvents, &LIBID_Xerces, XERCES_VERSION_MAJOR, INVK_CAT2_RAW_NUMERIC(XERCES_VERSION_MINOR,XERCES_VERSION_REVISION)>,
	public CWindowImpl<CXMLDOMDocument, CWindow, CWinTraits<0,0> >,
	ErrorHandler
{
public:
	CXMLDOMDocument();

	HRESULT FinalConstruct();
	void	FinalRelease();

	virtual DOMNode*        get_DOMNode()		    { return m_Document;}
    virtual DOMNodeType     get_DOMNodeType() const { return NODE_DOCUMENT; }

	virtual void resetErrors() {};

	//DECLARE_REGISTRY_RESOURCEID(IDR_XMLDOCUMENT)
	static HRESULT WINAPI UpdateRegistry(BOOL bRegister);

DECLARE_NOT_AGGREGATABLE(CXMLDOMDocument)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CXMLDOMDocument)
	COM_INTERFACE_ENTRY(IXMLDOMDocument)
	COM_INTERFACE_ENTRY(IXMLDOMNode)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IIBMXMLDOMNodeIdentity)
	COM_INTERFACE_ENTRY(IObjectSafety)
	COM_INTERFACE_ENTRY(IObjectWithSite)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY_IMPL(IConnectionPointContainer)
	COM_INTERFACE_ENTRY(IProvideClassInfo)
	COM_INTERFACE_ENTRY(IProvideClassInfo2)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()

BEGIN_CONNECTION_POINT_MAP(CXMLDOMDocument)
	CONNECTION_POINT_ENTRY(DIID_XMLDOMDocumentEvents)
END_CONNECTION_POINT_MAP()

	DECLARE_WND_CLASS(_T("XMLParseMonitor"))

BEGIN_MSG_MAP(CMonitorWnd)
	MESSAGE_HANDLER(MSG_READY_STATE_CHANGE,	OnReadyStateChange)
END_MSG_MAP()

	LRESULT OnReadyStateChange(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

	// IXMLDOMDocument methods
	STDMETHOD(get_doctype)(IXMLDOMDocumentType  * *pVal);
	STDMETHOD(get_implementation)(IXMLDOMImplementation  * *pVal);
	STDMETHOD(get_documentElement)(IXMLDOMElement  * *pVal);
	STDMETHOD(putref_documentElement)(IXMLDOMElement  *newVal);
	STDMETHOD(createElement)(BSTR tagName, IXMLDOMElement  * *element);
	STDMETHOD(createDocumentFragment)(IXMLDOMDocumentFragment  * *docFrag);
	STDMETHOD(createTextNode)(BSTR data, IXMLDOMText  * *text);
	STDMETHOD(createComment)(BSTR data, IXMLDOMComment  * *comment);
	STDMETHOD(createCDATASection)(BSTR data, IXMLDOMCDATASection  * *cdata);
	STDMETHOD(createProcessingInstruction)(BSTR target, BSTR data, IXMLDOMProcessingInstruction  * *pVal);
	STDMETHOD(createAttribute)(BSTR name, IXMLDOMAttribute  * *attr);
	STDMETHOD(createEntityReference)(BSTR name, IXMLDOMEntityReference  * *entityRef);
	STDMETHOD(getElementsByTagName)(BSTR tagName, IXMLDOMNodeList  * *resultList);
	STDMETHOD(createNode)(VARIANT type, BSTR name, BSTR namespaceURI, IXMLDOMNode  * *node);
	STDMETHOD(nodeFromID)(BSTR idString, IXMLDOMNode  * *node);
	STDMETHOD(load)(VARIANT xmlSource, VARIANT_BOOL  *isSuccessful);
	STDMETHOD(get_readyState)(long  *pVal);
	STDMETHOD(get_parseError)(IXMLDOMParseError  * *pVal);
	STDMETHOD(get_url)(BSTR  *pVal);
	STDMETHOD(get_async)(VARIANT_BOOL  *pVal);
	STDMETHOD(put_async)(VARIANT_BOOL newVal);
	STDMETHOD(abort)(void);
	STDMETHOD(loadXML)(BSTR bstrXML, VARIANT_BOOL  *success);
	STDMETHOD(save)(VARIANT location);
	STDMETHOD(get_validateOnParse)(VARIANT_BOOL  *pVal);
	STDMETHOD(put_validateOnParse)(VARIANT_BOOL newVal);
	STDMETHOD(get_resolveExternals)(VARIANT_BOOL  *pVal);
	STDMETHOD(put_resolveExternals)(VARIANT_BOOL newVal);
	STDMETHOD(get_preserveWhiteSpace)(VARIANT_BOOL  *pVal);
	STDMETHOD(put_preserveWhiteSpace)(VARIANT_BOOL newVal);
	STDMETHOD(put_onreadystatechange)(VARIANT newVal);
	STDMETHOD(put_ondataavailable)(VARIANT newVal);
	STDMETHOD(put_ontransformnode)(VARIANT newVal);

	// Error handling
	virtual void warning(const SAXParseException& exc);
	virtual void error(const SAXParseException& exc);
    virtual void fatalError(const SAXParseException& exc);

	bool IsAbort() const { return m_bAbort; }

	XERCES_CPP_NAMESPACE_QUALIFIER DOMDocument*		  m_Document;

private:

	bool				  m_bValidate;
	long				  m_lReadyState;
	_bstr_t			      m_url;
	CXMLDOMParseErrorObj *m_pParseError;
	bool			      m_bAsync;	
	bool				  m_bAbort;
	HANDLE				  m_hParseThread;
	LPDISPATCH			  m_pOnReadyStateChange;
	LPDISPATCH			  m_pOnDataAvailable;
	LPDISPATCH			  m_pOnTransformNode;

	// thread data
	_bstr_t				  m_FileName;
	_bstr_t				  m_xml;
	XERCES_CPP_NAMESPACE_QUALIFIER DOMDocument*		  m_TmpDocument;
	bool			      m_bParseError;
	bool				  m_bThreadValidate;
	bool                  m_bPreserveWhiteSpace;
	
	HRESULT GetBaseURL(_bstr_t &baseURL);
	
	static UINT APIENTRY ParseThread(void *pParm);
};

typedef CComObject<CXMLDOMDocument> CXMLDOMDocumentObj;

#endif // ___xmldomdocument_h___