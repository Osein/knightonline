
// UIEditor.h : main header file for the UIEditor application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CUIEditorApp:
// See UIEditor.cpp for the implementation of this class
//

class CUIEditorApp : public CWinAppEx
{
public:
	CUIEditorApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();

	DECLARE_MESSAGE_MAP()
};

extern CUIEditorApp theApp;
