#ifndef ___wrapnode_h___
#define ___wrapnode_h___

class DOM_Node;

extern const TCHAR* g_DomNodeName[];
extern const int	g_DomNodeNameSize;

void GetXML (const DOM_Node &node, _bstr_t &text);
void GetText(const DOM_Node& node, _bstr_t &text);
HRESULT wrapNode(IXMLDOMDocument *pDoc, DOM_Node& node, REFIID iid, LPVOID *pVal);

#endif // ___wrapnode_h___