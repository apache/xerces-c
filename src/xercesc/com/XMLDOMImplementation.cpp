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
 * Revision 1.3  2004/09/08 13:55:35  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.2  2004/02/25 18:38:33  amassari
 * The COM wrapper doesn't use the deprecated DOM anymore
 *
 * Revision 1.1.1.1  2002/02/01 22:21:41  peiyongz
 * sane_include
 *
 * Revision 1.2  2000/03/30 02:00:10  abagchi
 * Initial checkin of working code with Copyright Notice
 *
 */

#include "stdafx.h"
#include "xml4com.h"
#include "XMLDOMImplementation.h"

STDMETHODIMP CXMLDOMImplementation::hasFeature(BSTR feature, BSTR ver, VARIANT_BOOL  *pVal)
{
	ATLTRACE(_T("CXMLDOMImplementation::hasFeature\n"));

	if (NULL == pVal)
		return E_POINTER;

	*pVal = VARIANT_FALSE;

	try
	{
		*pVal = (implementation->hasFeature(feature, ver) ? VARIANT_TRUE : VARIANT_FALSE);
	}
	catch(...)
	{
		return E_FAIL;
	}
	

	return S_OK;
}