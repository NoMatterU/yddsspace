
// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit

// turns off MFC's hiding of some common and often safely ignored warning messages
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions





#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC support for Internet Explorer 4 Common Controls
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#define WM_SHOWTASK (WM_USER + 10)
#define WM_LOSEFOCUS (WM_USER + 11)
#define WM_DELLISTITEM (WM_USER + 12)

#define DIALOGWIDTH 600
#define DIALOGHEIGTH 620
#define LISTCTRLWIDTH 510
#define LISTCTRLHEIGHT 280
#define MAXLISTITEM 30

#include <afxcontrolbars.h>     // MFC support for ribbons and control bars
#include <VersionHelpers.h>
#include <gdiplus.h>             //GDI+声明，可以GDI/GDI+混合使用

#pragma comment(lib, "gdiplus.lib")
using namespace Gdiplus;







