/*
 * Copyright 1999-2000,2004 The Apache Software Foundation.
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
 * Revision 1.4  2004/09/08 13:55:35  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.3  2004/02/25 18:38:33  amassari
 * The COM wrapper doesn't use the deprecated DOM anymore
 *
 * Revision 1.2  2003/11/21 12:05:48  amassari
 * Updated version to 2.4
 *
 * Revision 1.1.1.1  2002/02/01 22:21:40  peiyongz
 * sane_include
 *
 * Revision 1.2  2000/03/30 02:00:12  abagchi
 * Initial checkin of working code with Copyright Notice
 *
 */

#ifndef ___ixmldomtextimpl_h___
#define ___ixmldomtextimpl_h___

#include "XMLDOMUtil.h"
#include "IXMLDOMCharacterDataImpl.h"

template <class T, const IID* piid, class tihclass = CComTypeInfoHolder>
class ATL_NO_VTABLE IXMLDOMTextImpl: public IXMLDOMCharacterDataImpl<T,piid,tihclass>
{
public:

	virtual DOMText* get_DOMText() = 0; 
	virtual DOMCharacterData* get_DOMCharacterData() { return get_DOMText(); } 

	// IXMLDOMText

STDMETHOD(splitText)(long offset, IXMLDOMText  **rightHandTextNode)
{
	ATLTRACE(_T("IXMLDOMTextImpl::splitText\n"));

	if (NULL == rightHandTextNode)
		return E_POINTER;

	*rightHandTextNode = NULL;

	HRESULT hr = S_OK;

	try
	{
		hr = wrapNode(m_pIXMLDOMDocument,get_DOMText()->splitText(offset),IID_IXMLDOMText, reinterpret_cast<LPVOID *> (rightHandTextNode));
	}
	catch(...)
	{
		return E_FAIL;
	}
	
	return hr;
}

};

#endif // ___ixmldomtextimpl_h___