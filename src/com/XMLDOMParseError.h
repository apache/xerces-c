#ifndef ___xmldomparseerror_h___
#define ___xmldomparseerror_h___

class ATL_NO_VTABLE CXMLDOMParseError : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public IDispatchImpl<IXMLDOMParseError, &IID_IXMLDOMParseError, &LIBID_IBMXMLLib>
{
public:
	CXMLDOMParseError()
		:m_Code		 (0)
		,m_url		 (_T(""))
		,m_Reason	 (_T(""))
		,m_Source	 (_T(""))
		,m_LineNumber(0)
		,m_LinePos	 (0)
		,m_FilePos	 (0)
	{}

	HRESULT FinalConstruct();
	void	FinalRelease();	

DECLARE_NOT_AGGREGATABLE(CXMLDOMParseError)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CXMLDOMParseError)
	COM_INTERFACE_ENTRY(IXMLDOMParseError)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

	// IXMLDOMParseError methods
	STDMETHOD(get_errorCode)(/* [out][retval] */ long  *errorCode);
    STDMETHOD(get_url)(/* [out][retval] */ BSTR  *urlString);
    STDMETHOD(get_reason)(/* [out][retval] */ BSTR  *reasonString);
    STDMETHOD(get_srcText)(/* [out][retval] */ BSTR  *sourceString);
    STDMETHOD(get_line)(/* [out][retval] */ long  *lineNumber);
    STDMETHOD(get_linepos)(/* [out][retval] */ long  *linePosition);
    STDMETHOD(get_filepos)(/* [out][retval] */ long  *filePosition);

	void SetData(long code,
				 const _bstr_t &url,
				 const _bstr_t &reason,
				 const _bstr_t &source,
				 long  lineNumber,
				 long  linePos,
				 long  filePos);
	void Reset();

private:

	long	m_Code;
	_bstr_t m_url;
	_bstr_t m_Reason;
	_bstr_t m_Source;
	long	m_LineNumber;
	long	m_LinePos;
	long	m_FilePos;

	CComCriticalSection	m_CS;
};

typedef CComObject<CXMLDOMParseError> CXMLDOMParseErrorObj;

#endif // ___xmldomparseerror_h___