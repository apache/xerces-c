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
 * Revision 1.7  2004/09/08 13:55:36  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.6  2004/02/25 18:38:33  amassari
 * The COM wrapper doesn't use the deprecated DOM anymore
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
 * Revision 1.1.1.1  2002/02/01 22:21:42  peiyongz
 * sane_include
 *
 * Revision 1.5  2001/05/11 13:25:07  tng
 * Copyright update.
 *
 * Revision 1.4  2001/01/19 15:18:34  tng
 * COM Updates by Curt Arnold: changed 1.3 to 1.4, updated the GUID's so
 * both can coexist and fixed a new minor bugs.  Most of the changes involved
 * error reporting, now a DOM defined error will return an HRESULT of
 * 0x80040600 + code and will set an error description to the error name.
 *
 * Revision 1.3  2000/06/03 00:29:01  andyh
 * COM Wrapper changes from Curt Arnold
 *
 * Revision 1.2  2000/03/30 02:00:09  abagchi
 * Initial checkin of working code with Copyright Notice
 *
 */

#ifndef ___xmldomtext_h___
#define ___xmldomtext_h___

#include <xercesc/dom/DOMText.hpp>
#include "IXMLDOMTextImpl.h"

XERCES_CPP_NAMESPACE_USE

class ATL_NO_VTABLE CXMLDOMText :
	public CComObjectRootEx<CComSingleThreadModel>,
	public IXMLDOMTextImpl<IXMLDOMText, &IID_IXMLDOMText>
{
public:
	CXMLDOMText()
	{}
	
	void	FinalRelease()
	{
		ReleaseOwnerDoc();
	}

	virtual DOMText* get_DOMText()		 	 { return text;}
	virtual DOMNodeType get_DOMNodeType() const  { return NODE_TEXT; }

DECLARE_NOT_AGGREGATABLE(CXMLDOMText)
DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CXMLDOMText)
	COM_INTERFACE_ENTRY(IXMLDOMText)
	COM_INTERFACE_ENTRY(IXMLDOMCharacterData)
	COM_INTERFACE_ENTRY(IXMLDOMNode)
	COM_INTERFACE_ENTRY(IIBMXMLDOMNodeIdentity)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()
	
	DOMText* text;
};

typedef CComObject<CXMLDOMText> CXMLDOMTextObj;

#endif // ___xmldomtext_h___