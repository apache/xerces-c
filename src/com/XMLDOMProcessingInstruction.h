#ifndef ___xmldomprocessinginstruction_h___
#define ___xmldomprocessinginstruction_h___

#include <dom/DOM_ProcessingInstruction.hpp>
#include "IXMLDOMNodeImpl.h"

class ATL_NO_VTABLE CXMLDOMProcessingInstruction : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public IXMLDOMNodeImpl<IXMLDOMProcessingInstruction, &IID_IXMLDOMProcessingInstruction, &LIBID_IBMXMLLib>
{
public:
	CXMLDOMProcessingInstruction()
	{}
	
	void	FinalRelease()
	{
		ReleaseOwnerDoc();
	}

	virtual DOM_Node& get_DOM_Node()			 { return processingInstruction;} 
	virtual DOMNodeType get_DOMNodeType() const  { return NODE_PROCESSING_INSTRUCTION; }

DECLARE_NOT_AGGREGATABLE(CXMLDOMProcessingInstruction)
DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CXMLDOMProcessingInstruction)
	COM_INTERFACE_ENTRY(IXMLDOMProcessingInstruction)
	COM_INTERFACE_ENTRY(IXMLDOMNode)
	COM_INTERFACE_ENTRY(IIBMXMLDOMNodeIdentity)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

	// IXMLDOMProcessingInstruction methods
	STDMETHOD(get_target)(BSTR  *pVal);
	STDMETHOD(get_data)(BSTR  *pVal);
	STDMETHOD(put_data)(BSTR newVal);

	DOM_ProcessingInstruction processingInstruction;
};

typedef CComObject<CXMLDOMProcessingInstruction> CXMLDOMProcessingInstructionObj;

#endif // ___xmldomprocessinginstruction_h___