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
 * $Log$
 * Revision 1.2  2000/03/30 02:00:13  abagchi
 * Initial checkin of working code with Copyright Notice
 *
 */

// CBindStatCallback.cpp : Implementation of BindStatusCallback
#include "stdafx.h"
#include "resource.h"
#include "BindStatusCallback.h"
#include "xml4com.h"
#include "XMLDOMDocument.h"

/////////////////////////////////////////////////////////////////////////////
// CBindStatCallback

STDMETHODIMP CBindStatCallback::OnStartBinding(DWORD dwReserved, IBinding *pBinding)
{
	ATLTRACE(_T("CBindStatCallback::OnStartBinding\n"));
	return S_OK;
}

STDMETHODIMP CBindStatCallback::GetPriority(LONG *pnPriority)
{
	ATLTRACE(_T("CBindStatCallback::GetPriority\n"));
	return E_NOTIMPL;
}

STDMETHODIMP CBindStatCallback::OnLowResource(DWORD reserved)
{
	ATLTRACE(_T("CBindStatCallback::OnLowResource\n"));
	return E_NOTIMPL;
}

STDMETHODIMP CBindStatCallback::OnProgress(ULONG ulProgress, ULONG ulProgressMax, ULONG ulStatusCode, LPCWSTR szStatusText)
{
	ATLTRACE(_T("CBindStatCallback::OnProgress %d %d\n"),ulProgress,ulProgressMax);
	if (m_pDoc->IsAbort())
		return E_ABORT; 

	_bstr_t text(szStatusText);
	switch (ulStatusCode) {
		case BINDSTATUS_FINDINGRESOURCE:
			text = _T("Finding resource ") + text;
			break;
		case BINDSTATUS_CONNECTING:
			text = _T("Connecting ") + text;
			break;
		case BINDSTATUS_REDIRECTING:
			text = _T("Redirecting ") + text;
			break;
		case BINDSTATUS_BEGINDOWNLOADDATA:
			text = _T("Begin to download data ") + text;
			break;
		case BINDSTATUS_DOWNLOADINGDATA:
			text = _T("Downloading data ") + text;
			break;
		case BINDSTATUS_ENDDOWNLOADDATA:
			text = _T("End of downloading data ") + text;
			break;
		case BINDSTATUS_BEGINDOWNLOADCOMPONENTS:
			text = _T("Downloading components ") + text;
			break;
	    case BINDSTATUS_INSTALLINGCOMPONENTS:
			text = _T("Installing components ") + text;
			break;
	    case BINDSTATUS_ENDDOWNLOADCOMPONENTS:
			text = _T("End of downloading components ") + text;
			break;
	    case BINDSTATUS_USINGCACHEDCOPY:
			text = _T("Using cached copy ") + text;
			break;
	    case BINDSTATUS_SENDINGREQUEST:
			text = _T("Sending request ") + text;
			break;
	    case BINDSTATUS_CLASSIDAVAILABLE:
			text = _T("Classid available ") + text;
			break;
	    case BINDSTATUS_MIMETYPEAVAILABLE:
			text = _T("Mime type available ") + text;
			break;
	    case BINDSTATUS_CACHEFILENAMEAVAILABLE:
			text = _T("Cache file name available ") + text;
			break;
	    case BINDSTATUS_BEGINSYNCOPERATION:
			text = _T("Begin sync operation ") + text;
			break;
	    case BINDSTATUS_ENDSYNCOPERATION:
			text = _T("End of sync operation ") + text;
			break;
	    case BINDSTATUS_BEGINUPLOADDATA:
			text = _T("Begin uploading data ") + text;
			break;
	    case BINDSTATUS_UPLOADINGDATA:
			text = _T("Uploading data ") + text;
			break;
	    case BINDSTATUS_PROTOCOLCLASSID:
			text = _T("Protocol classid ") + text;
			break;
		case BINDSTATUS_ENCODING:
			text = _T("Encoding ") + text;
			break;
	    case BINDSTATUS_CLASSINSTALLLOCATION:
			text = _T("Class intall location ") + text;
			break;
	    case BINDSTATUS_DECODING:
			text = _T("Decoding ") + text;
			break;
		default:
			break;
	}

	ATLTRACE(_T("CBindStatCallback::OnProgress %s\n"),text);
	
	return S_OK;
}

STDMETHODIMP CBindStatCallback::OnStopBinding(HRESULT hresult, LPCWSTR szError)
{
	ATLTRACE(_T("CBindStatCallback::OnStopBinding\n"));
	return S_OK;
}

STDMETHODIMP CBindStatCallback::GetBindInfo(DWORD *pgrfBINDF, BINDINFO *pbindInfo)
{
	ATLTRACE(_T("CBindStatCallback::GetBindInfo\n"));
	return E_NOTIMPL;
}

STDMETHODIMP CBindStatCallback::OnDataAvailable(DWORD grfBSCF, DWORD dwSize, FORMATETC *pformatetc, STGMEDIUM *pstgmed)
{
	ATLTRACE(_T("CBindStatCallback::OnDataAvailable\n"));
	return E_NOTIMPL;
}

STDMETHODIMP CBindStatCallback::OnObjectAvailable(REFIID riid, IUnknown *punk)
{
	ATLTRACE(_T("CBindStatCallback::OnObjectAvailable\n"));
	return E_NOTIMPL;
}
