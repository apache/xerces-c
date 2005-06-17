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
 * $Id$
 */

#ifndef ___xmldomentityreference_h___
#define ___xmldomentityreference_h___

#include <xercesc/dom/DOMEntityReference.hpp>
#include "IXMLDOMNodeImpl.h"

XERCES_CPP_NAMESPACE_USE

class ATL_NO_VTABLE CXMLDOMEntityReference :
	public CComObjectRootEx<CComSingleThreadModel>,
	public IXMLDOMNodeImpl<IXMLDOMEntityReference, &IID_IXMLDOMEntityReference>
{
public:
	CXMLDOMEntityReference()
	{}

	void	FinalRelease()
	{
		ReleaseOwnerDoc();
	}

	virtual DOMNode* get_DOMNode()			 { return entityReference;}
	virtual DOMNodeType get_DOMNodeType() const  { return NODE_ENTITY_REFERENCE; }

DECLARE_NOT_AGGREGATABLE(CXMLDOMEntityReference)
DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CXMLDOMEntityReference)
	COM_INTERFACE_ENTRY(IXMLDOMEntityReference)
	COM_INTERFACE_ENTRY(IXMLDOMNode)
	COM_INTERFACE_ENTRY(IIBMXMLDOMNodeIdentity)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()

	DOMEntityReference* entityReference;
};

typedef CComObject<CXMLDOMEntityReference> CXMLDOMEntityReferenceObj;

#endif // ___xmldomentityreference_h___