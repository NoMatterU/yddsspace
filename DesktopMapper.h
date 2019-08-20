
// MFCApplication1.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CMFCApplication1App:
// See MFCApplication1.cpp for the implementation of this class
//

class CMFCApplication1App : public CWinApp
{
private:
	ULONG_PTR m_gdiplusToken;

public:
	CMFCApplication1App();

// Overrides
public:
	virtual BOOL InitInstance();

	int ExitInstance();

	BOOL PreTranslateMessage(MSG * pMsg);

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CMFCApplication1App theApp;