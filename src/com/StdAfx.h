// stdafx.h : include file for standard system include files,
//      or project specific include files that are used frequently,
//      but are changed infrequently

#if !defined(AFX_STDAFX_H__975CB5CC_234E_4CA0_940E_DA102634BF51__INCLUDED_)
#define AFX_STDAFX_H__975CB5CC_234E_4CA0_940E_DA102634BF51__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define STRICT
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0400
#endif
#define _ATL_APARTMENT_THREADED

#include <atlbase.h>
//You may derive a class from CComModule and use it if you want to override
//something, but do not change the name of _Module
extern CComModule _Module;
#include <atlcom.h>
#include <atlctl.h>
#include <comdef.h>
#include <ExDisp.h>
#include <ShlGuid.h>
#include <stdio.h>
#include <process.h>
#include <Wininet.h>

const long	MSG_READY_STATE_CHANGE = WM_APP + 0x0001;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__975CB5CC_234E_4CA0_940E_DA102634BF51__INCLUDED)
