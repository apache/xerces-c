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
 * $Id$
 */

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
			:m_container(container)
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
		
		iterator& operator=(const iterator& rhs)
		{
			if (this != &rhs) {
				if (m_pIXMLDOMDocument != NULL) {
					m_pIXMLDOMDocument->Release() ;
					m_pIXMLDOMDocument = NULL ;
				}
				m_container = rhs.m_container ;
				m_NextNodeIndex = rhs.m_NextNodeIndex ;
				m_NextVar = rhs.m_NextVar ;
				m_pIXMLDOMDocument = rhs.m_pIXMLDOMDocument ;
				if (m_pIXMLDOMDocument != NULL) {
					m_pIXMLDOMDocument->AddRef() ;
				}
			}
			return *this ;
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
