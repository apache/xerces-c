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
 * Revision 1.7  2004/09/08 13:55:35  peiyongz
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
 * Revision 1.1.1.1  2002/02/01 22:21:41  peiyongz
 * sane_include
 *
 * Revision 1.3  2000/06/03 00:28:59  andyh
 * COM Wrapper changes from Curt Arnold
 *
 * Revision 1.2  2000/03/30 02:00:11  abagchi
 * Initial checkin of working code with Copyright Notice
 *
 */

#ifndef ___xmldomentity_h___
#define ___xmldomentity_h___

#include <xercesc/dom/DOMEntity.hpp>
#include "IXMLDOMNodeImpl.h"

XERCES_CPP_NAMESPACE_USE

class ATL_NO_VTABLE CXMLDOMEntity : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public IXMLDOMNodeImpl<IXMLDOMEntity, &IID_IXMLDOMEntity>
{
public:
	CXMLDOMEntity()
	{}

	void	FinalRelease()
	{
		ReleaseOwnerDoc();
	}

	virtual DOMNode* get_DOMNode()			 { return entity;} 
	virtual DOMNodeType get_DOMNodeType() const  { return NODE_ENTITY; }

DECLARE_NOT_AGGREGATABLE(CXMLDOMEntity)
DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CXMLDOMEntity)
	COM_INTERFACE_ENTRY(IXMLDOMEntity)
	COM_INTERFACE_ENTRY(IXMLDOMNode)
	COM_INTERFACE_ENTRY(IIBMXMLDOMNodeIdentity)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

	// IXMLDOMEntity methods
	STDMETHOD(get_publicId)(VARIANT  *pVal);
	STDMETHOD(get_systemId)(VARIANT  *pVal);
	STDMETHOD(get_notationName)(BSTR  *pVal);

	DOMEntity* entity;
};

typedef CComObject<CXMLDOMEntity> CXMLDOMEntityObj;

#endif // ___xmldomentity_h___