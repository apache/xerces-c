#ifndef ___nodecontainerimpl_h___
#define ___nodecontainerimpl_h___

template <class T>
class NodeContainerImpl
{
public:

	class iterator 
	{
	public:
		iterator()
			:m_container()
			,m_NextNodeIndex(0)
			,m_pIXMLDOMDocument(NULL)
		{
			VariantInit(&m_NextVar);
			V_VT(&m_NextVar) = VT_NULL;
		}	

		iterator(const T& container,int idx,IXMLDOMDocument *p)
			:m_container(m_container)
			,m_NextNodeIndex(idx)
			,m_pIXMLDOMDocument(p)
		{
			VariantInit(&m_NextVar);
			V_VT(&m_NextVar) = VT_NULL;
			if (m_pIXMLDOMDocument != NULL)
				m_pIXMLDOMDocument->AddRef();
		}

		~iterator()
		{
			VariantClear(&m_NextVar);
			if (m_pIXMLDOMDocument != NULL)
				m_pIXMLDOMDocument->Release();
		}
		
		bool operator !=(const iterator& rhs)
		{
			return (m_NextNodeIndex != rhs.m_NextNodeIndex);
		}
		
		VARIANT& operator*()
		{
			if (m_container == 0)
				return m_NextVar;

			int length = m_container.getLength(); 
			if (m_NextNodeIndex >= length)
				return m_NextVar;
			
			CComPtr<IXMLDOMNode> pNode;
			HRESULT hr = wrapNode(m_pIXMLDOMDocument,m_container.item(m_NextNodeIndex),IID_IXMLDOMNode, reinterpret_cast<LPVOID *> (&pNode));
			if (S_OK == hr) {
				CComQIPtr<IDispatch,&IID_IDispatch> pDisp(pNode);
				if (pNode) {
					VariantClear(&m_NextVar);
					V_VT(&m_NextVar)	   = VT_DISPATCH;
					V_DISPATCH(&m_NextVar) = pDisp.Detach();
				}
			}

			return m_NextVar;
		}
		
		iterator operator++(int)
		{
			return iterator(m_container,m_NextNodeIndex++,m_pIXMLDOMDocument);		
		}
		
	private:

		T				 m_container;	
		int			     m_NextNodeIndex;
		IXMLDOMDocument	*m_pIXMLDOMDocument;
		VARIANT			 m_NextVar;
	};

	NodeContainerImpl()
		:m_NextNodeIndex(0)
		,m_pIXMLDOMDocument(NULL)
	{}

	iterator begin()
	{
		return iterator(m_container,0,m_pIXMLDOMDocument);
	}
	
	iterator end()
	{
		if (m_container == 0)
			return iterator(m_container,0,m_pIXMLDOMDocument);
		else	
			return iterator(m_container,m_container.getLength(),m_pIXMLDOMDocument);
	}
	
	void	SetOwnerDoc(IXMLDOMDocument	*p)
	{
		m_pIXMLDOMDocument = p;
		if (m_pIXMLDOMDocument != NULL)
			m_pIXMLDOMDocument->AddRef();
	}

	T				 m_container;

protected:

	int				 m_NextNodeIndex;
	IXMLDOMDocument	*m_pIXMLDOMDocument;

	void	ReleaseOwnerDoc()
	{
		if (m_pIXMLDOMDocument != NULL) {
			m_pIXMLDOMDocument->Release();
			m_pIXMLDOMDocument = NULL;
		}
	}
};

#endif // ___nodecontainerimpl_h___
