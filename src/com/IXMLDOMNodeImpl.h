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
 * $Log$
 * Revision 1.2  2000/03/30 02:00:12  abagchi
 * Initial checkin of working code with Copyright Notice
 *
 */

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
