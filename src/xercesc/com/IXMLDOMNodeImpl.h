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
 * Revision 1.7  2004/10/01 08:10:38  amassari
 * Updated version numbers to 2.6; use version macros when possible
 *
 * Revision 1.6  2004/09/08 13:55:35  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.5  2004/02/25 18:38:33  amassari
 * The COM wrapper doesn't use the deprecated DOM anymore
 *
 * Revision 1.4  2004/02/12 13:49:43  amassari
 * Updated version to 2.5
 *
 * Revision 1.3  2003/11/21 12:05:48  amassari
 * Updated version to 2.4
 *
 * Revision 1.2  2003/03/14 12:44:49  tng
 * [Bug 17147] C++ namespace breaks build of XercesCOM DLL
 *
 * Revision 1.1.1.1  2002/02/01 22:21:39  peiyongz
 * sane_include
 *
 * Revision 1.6  2001/05/11 13:25:00  tng
 * Copyright update.
 *
 * Revision 1.5  2001/01/19 15:17:54  tng
 * COM Updates by Curt Arnold: changed 1.3 to 1.4, updated the GUID's so
 * both can coexist and fixed a new minor bugs.  Most of the changes involved
 * error reporting, now a DOM defined error will return an HRESULT of
 * 0x80040600 + code and will set an error description to the error name.
 *
 * Revision 1.4  2000/06/19 20:05:56  rahulj
 * Changes for increased conformance and stability. Submitted by
 * Curt.Arnold@hyprotech.com. Verified by Joe Polastre.
 *
 * Revision 1.3  2000/06/03 00:28:54  andyh
 * COM Wrapper changes from Curt Arnold
 *
 * Revision 1.2  2000/03/30 02:00:12  abagchi
 * Initial checkin of working code with Copyright Notice
 *
 */

#ifndef ___ixmldomnodeimpl_h___
#define ___ixmldomnodeimpl_h___

#include <xercesc/util/XercesDefs.hpp>
#include <xercesc/dom/DOMException.hpp>
XERCES_CPP_NAMESPACE_USE


//
//   This macro is defined in MSXML.H's compatible with IE5
//      and not defined in those from IE4.
//
//   To correct, install a IE5 or later version of the Microsoft Platform SDK
//      and add \Program Files\Microsoft Platform SDK\Include as the first entry
//      on the Directories tab on the dialog displayed after selecting Tools Options
//      from the Visual Studio IDE.
//
//   See http://xml.apache.org/xerces-c/build.html#BuildCOM
#ifndef __IXMLDOMNode_INTERFACE_DEFINED__
#error "xerces-dom requires an MSXML.H compatible with IE5 or later.  See http://xml.apache.org/xerces-c/build.html#BuildCOM for directions to correct this problem."
#endif


template <class T, const IID* piid, class tihclass = CComTypeInfoHolder>
class ATL_NO_VTABLE IXMLDOMNodeImpl:
	public IDispatchImpl<T,piid,&LIBID_Xerces, XERCES_VERSION_MAJOR, INVK_CAT2_RAW_NUMERIC(XERCES_VERSION_MINOR,XERCES_VERSION_REVISION),tihclass>,
	public IIBMXMLDOMNodeIdentity,
	public ISupportErrorInfo
{
public:

	IXMLDOMNodeImpl()
		:m_pIXMLDOMDocument(NULL)
	{}

	virtual DOMNode* get_DOMNode()		    = 0;
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

		*pVal = reinterpret_cast<long> (get_DOMNode());
		return S_OK;
	}

	//   ISupportErrorInfo
	HRESULT STDMETHODCALLTYPE InterfaceSupportsErrorInfo(REFIID iid);
	

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

HRESULT MakeHRESULT(DOMException& ex);

#include "IXMLDOMNodeImpl.inl"

#endif // ___ixmldomnodeimpl_h___
