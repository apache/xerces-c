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
 * Revision 1.1  2002/02/01 22:21:39  peiyongz
 * Initial revision
 *
 * Revision 1.3  2000/07/07 00:12:51  jpolast
 * bug fixes for non-null terminated strings
 *
 * Revision 1.2  2000/03/30 02:00:13  abagchi
 * Initial checkin of working code with Copyright Notice
 *
 */

#ifndef ___ixmldomcharacterdataimpl_h___
#define ___ixmldomcharacterdataimpl_h___

#include "IXMLDOMNodeImpl.h"

template <class T, const IID* piid, const GUID* plibid = &CComModule::m_libid, WORD wMajor = 1,
WORD wMinor = 0, class tihclass = CComTypeInfoHolder>
class ATL_NO_VTABLE IXMLDOMCharacterDataImpl: public IXMLDOMNodeImpl<T,piid,plibid,wMajor,wMinor,tihclass>
{
private:
	XMLCh* DOMStringToXMLCh(DOMString str)
	{
		XMLCh* cdata = new XMLCh[str.length()+1];
		for (unsigned int i=0; i < str.length(); i++)
			cdata[i] = str.charAt(i);
		cdata[str.length()] = 0;
		return cdata;
	}

public:

	virtual DOM_CharacterData& get_DOM_CharacterData() = 0; 
	virtual DOM_Node& get_DOM_Node() { return get_DOM_CharacterData(); } 

	// IXMLDOMCharacterData 

STDMETHOD(get_data)(BSTR  *pVal)
{
	ATLTRACE(_T("IXMLDOMCharacterDataImpl::get_data\n"));

	if (NULL == pVal)
		return E_POINTER;

	*pVal = NULL;
	
	try
	{
		//*pVal = SysAllocString(get_DOM_CharacterData().getData().rawBuffer());
		*pVal = SysAllocString(DOMStringToXMLCh(get_DOM_CharacterData().getData()));
	}
	catch(...)
	{
		return E_FAIL;
	}
	

	return S_OK;
}

STDMETHOD(put_data)(BSTR newVal)
{
	ATLTRACE(_T("IXMLDOMCharacterDataImpl::put_data\n"));

	try
	{
		get_DOM_CharacterData().setData(newVal);
	}
	catch(...)
	{
		return E_FAIL;
	}
	

	return S_OK;
}

STDMETHOD(get_length)(long  *pVal)
{
	ATLTRACE(_T("IXMLDOMCharacterDataImpl::get_length\n"));

	if (NULL == pVal)
		return E_POINTER;

	*pVal = 0;

	try
	{
		*pVal = get_DOM_CharacterData().getLength();
	}
	catch(...)
	{
		return E_FAIL;
	}

	return S_OK;
}

STDMETHOD(substringData)(long offset, long count, BSTR  *data)
{
	ATLTRACE(_T("IXMLDOMCharacterDataImpl::substringData\n"));

	if (NULL == data)
		return E_POINTER;

	*data = NULL;

	try
	{
		// need to copy the string to a new buffer since DOMString doesn't null terminate the substring.
		*data = SysAllocString(DOMStringToXMLCh(get_DOM_CharacterData().substringData(offset, count)));
	}
	catch(...)
	{
		return E_FAIL;
	}
	
	return S_OK;
}

STDMETHOD(appendData)(BSTR data)
{
	ATLTRACE(_T("IXMLDOMCharacterDataImpl::appendData\n"));

	try
	{
		get_DOM_CharacterData().appendData(data);
	}
	catch(...)
	{
		return E_FAIL;
	}
	

	return S_OK;
}

STDMETHOD(insertData)(long offset, BSTR data)
{
	ATLTRACE(_T("IXMLDOMCharacterDataImpl::insertData\n"));

	try
	{
		get_DOM_CharacterData().insertData(offset, data);
	}
	catch(...)
	{
		return E_FAIL;
	}
	

	return S_OK;
}

STDMETHOD(deleteData)(long offset, long count)
{
	ATLTRACE(_T("IXMLDOMCharacterDataImpl::deleteData\n"));

	try
	{
		get_DOM_CharacterData().deleteData(offset, count);
	}
	catch(...)
	{
		return E_FAIL;
	}
	

	return S_OK;
}

STDMETHOD(replaceData)(long offset, long count, BSTR data)
{
	ATLTRACE(_T("IXMLDOMCharacterDataImpl::replaceData\n"));

	try
	{
		get_DOM_CharacterData().replaceData(offset, count, data);
	}
	catch(...)
	{
		return E_FAIL;
	}
	
	return S_OK;
}

};

#endif // ___ixmldomcharacterdataimpl_h___