#ifndef ___xmldomcomment_h___
#define ___xmldomcomment_h___

#include <dom/DOM_Comment.hpp>
#include "IXMLDOMCharacterDataImpl.h"

class ATL_NO_VTABLE CXMLDOMComment : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public IXMLDOMCharacterDataImpl<IXMLDOMComment, &IID_IXMLDOMComment, &LIBID_IBMXMLLib>
{
public:
	CXMLDOMComment()
	{}

	void	FinalRelease()
	{
		ReleaseOwnerDoc();
	}

	virtual DOM_CharacterData& get_DOM_CharacterData()  { return comment;} 
	virtual DOMNodeType get_DOMNodeType() const			{ return NODE_COMMENT; }
	
DECLARE_NOT_AGGREGATABLE(CXMLDOMComment)
DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CXMLDOMComment)
	COM_INTERFACE_ENTRY(IXMLDOMComment)
	COM_INTERFACE_ENTRY(IXMLDOMCharacterData)
	COM_INTERFACE_ENTRY(IXMLDOMNode)
	COM_INTERFACE_ENTRY(IIBMXMLDOMNodeIdentity)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

	DOM_Comment comment;
};

typedef CComObject<CXMLDOMComment> CXMLDOMCommentObj;

#endif // ___xmldomcomment_h___