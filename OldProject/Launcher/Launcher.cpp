// Launcher.cpp : Defines the class behaviors for the application.
//

#include "pch.h"
#include "framework.h"
#include "Launcher.h"
#include "LauncherDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CLauncherApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

CLauncherApp::CLauncherApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

CLauncherApp theApp;

BOOL CLauncherApp::InitInstance()
{
	CWinApp::InitInstance();
	AfxEnableControlContainer();

	// Activate "Windows Native" visual manager for enabling themes in MFC controls
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	CLauncherDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();

#if !defined(_AFXDLL) && !defined(_AFX_NO_MFC_CONTROLS_IN_DIALOGS)
	ControlBarCleanUp();
#endif

	//exit(0);

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
