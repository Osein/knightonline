#pragma once

// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated
// by Microsoft Visual C++, your modifications will be overwritten.

/////////////////////////////////////////////////////////////////////////////

#include "afxwin.h"

class CWEBBROWSER2 : public CWnd
{
protected:
	DECLARE_DYNCREATE(CWEBBROWSER2)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= {0x8856f961,0x340a,0x11d0,{0xa9,0x6b,0x00,0xc0,0x4f,0xd7,0x05,0xa2}};
		return clsid;
	}
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle,
						const RECT& rect, CWnd* pParentWnd, UINT nID, 
						CCreateContext* pContext = nullptr)
	{ 
		return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID);
	}

	BOOL Create(LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd,
				UINT nID, CFile* pPersist = nullptr, BOOL bStorage = FALSE,
				BSTR bstrLicKey = nullptr)
	{ 
		return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID,
		pPersist, bStorage, bstrLicKey); 
	}

// Attributes
public:
enum
{
	CSC_UPDATECOMMANDS = -1,
	CSC_NAVIGATEFORWARD = 1,
	CSC_NAVIGATEBACK = 2
}CommandStateChangeConstants;

enum
{
	OLECMDID_OPEN = 1,
	OLECMDID_NEW = 2,
	OLECMDID_SAVE = 3,
	OLECMDID_SAVEAS = 4,
	OLECMDID_SAVECOPYAS = 5,
	OLECMDID_PRINT = 6,
	OLECMDID_PRINTPREVIEW = 7,
	OLECMDID_PAGESETUP = 8,
	OLECMDID_SPELL = 9,
	OLECMDID_PROPERTIES = 10,
	OLECMDID_CUT = 11,
	OLECMDID_COPY = 12,
	OLECMDID_PASTE = 13,
	OLECMDID_PASTESPECIAL = 14,
	OLECMDID_UNDO = 15,
	OLECMDID_REDO = 16,
	OLECMDID_SELECTALL = 17,
	OLECMDID_CLEARSELECTION = 18,
	OLECMDID_ZOOM = 19,
	OLECMDID_GETZOOMRANGE = 20,
	OLECMDID_UPDATECOMMANDS = 21,
	OLECMDID_REFRESH = 22,
	OLECMDID_STOP = 23,
	OLECMDID_HIDETOOLBARS = 24,
	OLECMDID_SETPROGRESSMAX = 25,
	OLECMDID_SETPROGRESSPOS = 26,
	OLECMDID_SETPROGRESSTEXT = 27,
	OLECMDID_SETTITLE = 28,
	OLECMDID_SETDOWNLOADSTATE = 29,
	OLECMDID_STOPDOWNLOAD = 30,
	OLECMDID_ONTOOLBARACTIVATED = 31,
	OLECMDID_FIND = 32,
	OLECMDID_DELETE = 33,
	OLECMDID_HTTPEQUIV = 34,
	OLECMDID_HTTPEQUIV_DONE = 35,
	OLECMDID_ENABLE_INTERACTION = 36,
	OLECMDID_ONUNLOAD = 37,
	OLECMDID_PROPERTYBAG2 = 38,
	OLECMDID_PREREFRESH = 39,
	OLECMDID_SHOWSCRIPTERROR = 40,
	OLECMDID_SHOWMESSAGE = 41,
	OLECMDID_SHOWFIND = 42,
	OLECMDID_SHOWPAGESETUP = 43,
	OLECMDID_SHOWPRINT = 44,
	OLECMDID_CLOSE = 45,
	OLECMDID_ALLOWUILESSSAVEAS = 46,
	OLECMDID_DONTDOWNLOADCSS = 47,
	OLECMDID_UPDATEPAGESTATUS = 48,
	OLECMDID_PRINT2 = 49,
	OLECMDID_PRINTPREVIEW2 = 50,
	OLECMDID_SETPRINTTEMPLATE = 51,
	OLECMDID_GETPRINTTEMPLATE = 52,
	OLECMDID_PAGEACTIONBLOCKED = 55,
	OLECMDID_PAGEACTIONUIQUERY = 56,
	OLECMDID_FOCUSVIEWCONTROLS = 57,
	OLECMDID_FOCUSVIEWCONTROLSQUERY = 58,
	OLECMDID_SHOWPAGEACTIONMENU = 59,
	OLECMDID_ADDTRAVELENTRY = 60,
	OLECMDID_UPDATETRAVELENTRY = 61,
	OLECMDID_UPDATEBACKFORWARDSTATE = 62,
	OLECMDID_OPTICAL_ZOOM = 63,
	OLECMDID_OPTICAL_GETZOOMRANGE = 64,
	OLECMDID_WINDOWSTATECHANGED = 65,
	OLECMDID_ACTIVEXINSTALLSCOPE = 66,
	OLECMDID_UPDATETRAVELENTRY_DATARECOVERY = 67,
	OLECMDID_SHOWTASKDLG = 68,
	OLECMDID_POPSTATEEVENT = 69,
	OLECMDID_VIEWPORT_MODE = 70,
	OLECMDID_LAYOUT_VIEWPORT_WIDTH = 71,
	OLECMDID_VISUAL_VIEWPORT_EXCLUDE_BOTTOM = 72,
	OLECMDID_USER_OPTICAL_ZOOM = 73,
	OLECMDID_PAGEAVAILABLE = 74,
	OLECMDID_GETUSERSCALABLE = 75,
	OLECMDID_UPDATE_CARET = 76,
	OLECMDID_ENABLE_VISIBILITY = 77,
	OLECMDID_MEDIA_PLAYBACK = 78,
	OLECMDID_SETFAVICON = 79,
	OLECMDID_SET_HOST_FULLSCREENMODE = 80,
	OLECMDID_EXITFULLSCREEN = 81,
	OLECMDID_SCROLLCOMPLETE = 82,
	OLECMDID_ONBEFOREUNLOAD = 83,
	OLECMDID_SHOWMESSAGE_BLOCKABLE = 84,
	OLECMDID_SHOWTASKDLG_BLOCKABLE = 85
}OLECMDID;

enum
{
	OLECMDF_SUPPORTED = 1,
	OLECMDF_ENABLED = 2,
	OLECMDF_LATCHED = 4,
	OLECMDF_NINCHED = 8,
	OLECMDF_INVISIBLE = 16,
	OLECMDF_DEFHIDEONCTXTMENU = 32
}OLECMDF;

enum
{
	OLECMDEXECOPT_DODEFAULT = 0,
	OLECMDEXECOPT_PROMPTUSER = 1,
	OLECMDEXECOPT_DONTPROMPTUSER = 2,
	OLECMDEXECOPT_SHOWHELP = 3
}OLECMDEXECOPT;

enum
{
	READYSTATE_UNINITIALIZED = 0,
	READYSTATE_LOADING = 1,
	READYSTATE_LOADED = 2,
	READYSTATE_INTERACTIVE = 3,
	READYSTATE_COMPLETE = 4
}tagREADYSTATE;

enum
{
	secureLockIconUnsecure = 0,
	secureLockIconMixed = 1,
	secureLockIconSecureUnknownBits = 2,
	secureLockIconSecure40Bit = 3,
	secureLockIconSecure56Bit = 4,
	secureLockIconSecureFortezza = 5,
	secureLockIconSecure128Bit = 6
}SecureLockIconConstants;

enum
{
	ProtectedModeRedirect = 1
}NewProcessCauseConstants;

enum
{
	SWC_EXPLORER = 0,
	SWC_BROWSER = 1,
	SWC_3RDPARTY = 2,
	SWC_CALLBACK = 4,
	SWC_DESKTOP = 8
}ShellWindowTypeConstants;

enum
{
	SWFO_NEEDDISPATCH = 1,
	SWFO_INCLUDEPENDING = 2,
	SWFO_COOKIEPASSED = 4
}ShellWindowFindWindowOptions;

// Operations
public:
// IWebBrowser2

// Functions
//

	void GoBack()
	{
		InvokeHelper(0x64, DISPATCH_METHOD, VT_EMPTY, nullptr, nullptr);
	}
	void GoForward()
	{
		InvokeHelper(0x65, DISPATCH_METHOD, VT_EMPTY, nullptr, nullptr);
	}
	void GoHome()
	{
		InvokeHelper(0x66, DISPATCH_METHOD, VT_EMPTY, nullptr, nullptr);
	}
	void GoSearch()
	{
		InvokeHelper(0x67, DISPATCH_METHOD, VT_EMPTY, nullptr, nullptr);
	}
	void Navigate(LPCTSTR URL, VARIANT * Flags, VARIANT * TargetFrameName, VARIANT * PostData, VARIANT * Headers)
	{
		static BYTE parms[] = VTS_BSTR VTS_UNKNOWN VTS_UNKNOWN VTS_UNKNOWN VTS_UNKNOWN;
		InvokeHelper(0x68, DISPATCH_METHOD, VT_EMPTY, nullptr, parms, URL, Flags, TargetFrameName, PostData, Headers);
	}
	void Refresh()
	{
		InvokeHelper(0xFFFFFDDA, DISPATCH_METHOD, VT_EMPTY, nullptr, nullptr);
	}
	void Refresh2(VARIANT * Level)
	{
		static BYTE parms[] = VTS_UNKNOWN;
		InvokeHelper(0x69, DISPATCH_METHOD, VT_EMPTY, nullptr, parms, Level);
	}
	void Stop()
	{
		InvokeHelper(0x6A, DISPATCH_METHOD, VT_EMPTY, nullptr, nullptr);
	}
	LPDISPATCH get_Application()
	{
		LPDISPATCH result;
		InvokeHelper(0xC8, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, nullptr);
		return result;
	}
	LPDISPATCH get_Parent()
	{
		LPDISPATCH result;
		InvokeHelper(0xC9, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, nullptr);
		return result;
	}
	LPDISPATCH get_Container()
	{
		LPDISPATCH result;
		InvokeHelper(0xCA, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, nullptr);
		return result;
	}
	LPDISPATCH get_Document()
	{
		LPDISPATCH result;
		InvokeHelper(0xCB, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, nullptr);
		return result;
	}
	BOOL get_TopLevelContainer()
	{
		BOOL result;
		InvokeHelper(0xCC, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, nullptr);
		return result;
	}
	CString get_Type()
	{
		CString result;
		InvokeHelper(0xCD, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, nullptr);
		return result;
	}
	long get_Left()
	{
		long result;
		InvokeHelper(0xCE, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, nullptr);
		return result;
	}
	void put_Left(long newValue)
	{
		static BYTE parms[] = VTS_I4;
		InvokeHelper(0xCE, DISPATCH_PROPERTYPUT, VT_EMPTY, nullptr, parms, newValue);
	}
	long get_Top()
	{
		long result;
		InvokeHelper(0xCF, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, nullptr);
		return result;
	}
	void put_Top(long newValue)
	{
		static BYTE parms[] = VTS_I4;
		InvokeHelper(0xCF, DISPATCH_PROPERTYPUT, VT_EMPTY, nullptr, parms, newValue);
	}
	long get_Width()
	{
		long result;
		InvokeHelper(0xD0, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, nullptr);
		return result;
	}
	void put_Width(long newValue)
	{
		static BYTE parms[] = VTS_I4;
		InvokeHelper(0xD0, DISPATCH_PROPERTYPUT, VT_EMPTY, nullptr, parms, newValue);
	}
	long get_Height()
	{
		long result;
		InvokeHelper(0xD1, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, nullptr);
		return result;
	}
	void put_Height(long newValue)
	{
		static BYTE parms[] = VTS_I4;
		InvokeHelper(0xD1, DISPATCH_PROPERTYPUT, VT_EMPTY, nullptr, parms, newValue);
	}
	CString get_LocationName()
	{
		CString result;
		InvokeHelper(0xD2, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, nullptr);
		return result;
	}
	CString get_LocationURL()
	{
		CString result;
		InvokeHelper(0xD3, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, nullptr);
		return result;
	}
	BOOL get_Busy()
	{
		BOOL result;
		InvokeHelper(0xD4, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, nullptr);
		return result;
	}
	void Quit()
	{
		InvokeHelper(0x12C, DISPATCH_METHOD, VT_EMPTY, nullptr, nullptr);
	}
	void ClientToWindow(int * pcx, int * pcy)
	{
		static BYTE parms[] = VTS_UNKNOWN VTS_UNKNOWN;
		InvokeHelper(0x12D, DISPATCH_METHOD, VT_EMPTY, nullptr, parms, pcx, pcy);
	}
	void PutProperty(LPCTSTR Property, VARIANT vtValue)
	{
		static BYTE parms[] = VTS_BSTR VTS_VARIANT;
		InvokeHelper(0x12E, DISPATCH_METHOD, VT_EMPTY, nullptr, parms, Property, &vtValue);
	}
	VARIANT GetProperty(LPCTSTR Property)
	{
		VARIANT result;
		static BYTE parms[] = VTS_BSTR;
		InvokeHelper(0x12F, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, Property);
		return result;
	}
	CString get_Name()
	{
		CString result;
		InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, nullptr);
		return result;
	}
	long get_HWND()
	{
		long result;
		InvokeHelper(0xFFFFFDFD, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, nullptr);
		return result;
	}
	CString get_FullName()
	{
		CString result;
		InvokeHelper(0x190, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, nullptr);
		return result;
	}
	CString get_Path()
	{
		CString result;
		InvokeHelper(0x191, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, nullptr);
		return result;
	}
	BOOL get_Visible()
	{
		BOOL result;
		InvokeHelper(0x192, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, nullptr);
		return result;
	}
	void put_Visible(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL;
		InvokeHelper(0x192, DISPATCH_PROPERTYPUT, VT_EMPTY, nullptr, parms, newValue);
	}
	BOOL get_StatusBar()
	{
		BOOL result;
		InvokeHelper(0x193, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, nullptr);
		return result;
	}
	void put_StatusBar(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL;
		InvokeHelper(0x193, DISPATCH_PROPERTYPUT, VT_EMPTY, nullptr, parms, newValue);
	}
	CString get_StatusText()
	{
		CString result;
		InvokeHelper(0x194, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, nullptr);
		return result;
	}
	void put_StatusText(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR;
		InvokeHelper(0x194, DISPATCH_PROPERTYPUT, VT_EMPTY, nullptr, parms, newValue);
	}
	long get_ToolBar()
	{
		long result;
		InvokeHelper(0x195, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, nullptr);
		return result;
	}
	void put_ToolBar(long newValue)
	{
		static BYTE parms[] = VTS_I4;
		InvokeHelper(0x195, DISPATCH_PROPERTYPUT, VT_EMPTY, nullptr, parms, newValue);
	}
	BOOL get_MenuBar()
	{
		BOOL result;
		InvokeHelper(0x196, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, nullptr);
		return result;
	}
	void put_MenuBar(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL;
		InvokeHelper(0x196, DISPATCH_PROPERTYPUT, VT_EMPTY, nullptr, parms, newValue);
	}
	BOOL get_FullScreen()
	{
		BOOL result;
		InvokeHelper(0x197, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, nullptr);
		return result;
	}
	void put_FullScreen(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL;
		InvokeHelper(0x197, DISPATCH_PROPERTYPUT, VT_EMPTY, nullptr, parms, newValue);
	}
	void Navigate2(VARIANT * URL, VARIANT * Flags, VARIANT * TargetFrameName, VARIANT * PostData, VARIANT * Headers)
	{
		static BYTE parms[] = VTS_UNKNOWN VTS_UNKNOWN VTS_UNKNOWN VTS_UNKNOWN VTS_UNKNOWN;
		InvokeHelper(0x1F4, DISPATCH_METHOD, VT_EMPTY, nullptr, parms, URL, Flags, TargetFrameName, PostData, Headers);
	}
	long QueryStatusWB(long cmdID)
	{
		long result;
		static BYTE parms[] = VTS_I4;
		InvokeHelper(0x1F5, DISPATCH_METHOD, VT_I4, (void*)&result, parms, cmdID);
		return result;
	}
	void ExecWB(long cmdID, long cmdexecopt, VARIANT * pvaIn, VARIANT * pvaOut)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_UNKNOWN VTS_UNKNOWN;
		InvokeHelper(0x1F6, DISPATCH_METHOD, VT_EMPTY, nullptr, parms, cmdID, cmdexecopt, pvaIn, pvaOut);
	}
	void ShowBrowserBar(VARIANT * pvaClsid, VARIANT * pvarShow, VARIANT * pvarSize)
	{
		static BYTE parms[] = VTS_UNKNOWN VTS_UNKNOWN VTS_UNKNOWN;
		InvokeHelper(0x1F7, DISPATCH_METHOD, VT_EMPTY, nullptr, parms, pvaClsid, pvarShow, pvarSize);
	}
	long get_ReadyState()
	{
		long result;
		InvokeHelper(0xFFFFFDF3, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, nullptr);
		return result;
	}
	BOOL get_Offline()
	{
		BOOL result;
		InvokeHelper(0x226, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, nullptr);
		return result;
	}
	void put_Offline(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL;
		InvokeHelper(0x226, DISPATCH_PROPERTYPUT, VT_EMPTY, nullptr, parms, newValue);
	}
	BOOL get_Silent()
	{
		BOOL result;
		InvokeHelper(0x227, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, nullptr);
		return result;
	}
	void put_Silent(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL;
		InvokeHelper(0x227, DISPATCH_PROPERTYPUT, VT_EMPTY, nullptr, parms, newValue);
	}
	BOOL get_RegisterAsBrowser()
	{
		BOOL result;
		InvokeHelper(0x228, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, nullptr);
		return result;
	}
	void put_RegisterAsBrowser(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL;
		InvokeHelper(0x228, DISPATCH_PROPERTYPUT, VT_EMPTY, nullptr, parms, newValue);
	}
	BOOL get_RegisterAsDropTarget()
	{
		BOOL result;
		InvokeHelper(0x229, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, nullptr);
		return result;
	}
	void put_RegisterAsDropTarget(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL;
		InvokeHelper(0x229, DISPATCH_PROPERTYPUT, VT_EMPTY, nullptr, parms, newValue);
	}
	BOOL get_TheaterMode()
	{
		BOOL result;
		InvokeHelper(0x22A, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, nullptr);
		return result;
	}
	void put_TheaterMode(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL;
		InvokeHelper(0x22A, DISPATCH_PROPERTYPUT, VT_EMPTY, nullptr, parms, newValue);
	}
	BOOL get_AddressBar()
	{
		BOOL result;
		InvokeHelper(0x22B, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, nullptr);
		return result;
	}
	void put_AddressBar(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL;
		InvokeHelper(0x22B, DISPATCH_PROPERTYPUT, VT_EMPTY, nullptr, parms, newValue);
	}
	BOOL get_Resizable()
	{
		BOOL result;
		InvokeHelper(0x22C, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, nullptr);
		return result;
	}
	void put_Resizable(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL;
		InvokeHelper(0x22C, DISPATCH_PROPERTYPUT, VT_EMPTY, nullptr, parms, newValue);
	}

// Properties
//

// DWebBrowserEvents2

// Functions
//

	void StatusTextChange(LPCTSTR Text)
	{
		static BYTE parms[] = VTS_BSTR;
		InvokeHelper(0x66, DISPATCH_METHOD, VT_EMPTY, nullptr, parms, Text);
	}
	void ProgressChange(long Progress, long ProgressMax)
	{
		static BYTE parms[] = VTS_I4 VTS_I4;
		InvokeHelper(0x6C, DISPATCH_METHOD, VT_EMPTY, nullptr, parms, Progress, ProgressMax);
	}
	void CommandStateChange(long Command, BOOL Enable)
	{
		static BYTE parms[] = VTS_I4 VTS_BOOL;
		InvokeHelper(0x69, DISPATCH_METHOD, VT_EMPTY, nullptr, parms, Command, Enable);
	}
	void DownloadBegin()
	{
		InvokeHelper(0x6A, DISPATCH_METHOD, VT_EMPTY, nullptr, nullptr);
	}
	void DownloadComplete()
	{
		InvokeHelper(0x68, DISPATCH_METHOD, VT_EMPTY, nullptr, nullptr);
	}
	void TitleChange(LPCTSTR Text)
	{
		static BYTE parms[] = VTS_BSTR;
		InvokeHelper(0x71, DISPATCH_METHOD, VT_EMPTY, nullptr, parms, Text);
	}
	void PropertyChange(LPCTSTR szProperty)
	{
		static BYTE parms[] = VTS_BSTR;
		InvokeHelper(0x70, DISPATCH_METHOD, VT_EMPTY, nullptr, parms, szProperty);
	}
	void BeforeNavigate2(LPDISPATCH pDisp, VARIANT * URL, VARIANT * Flags, VARIANT * TargetFrameName, VARIANT * PostData, VARIANT * Headers, BOOL * Cancel)
	{
		static BYTE parms[] = VTS_DISPATCH VTS_UNKNOWN VTS_UNKNOWN VTS_UNKNOWN VTS_UNKNOWN VTS_UNKNOWN VTS_UNKNOWN;
		InvokeHelper(0xFA, DISPATCH_METHOD, VT_EMPTY, nullptr, parms, pDisp, URL, Flags, TargetFrameName, PostData, Headers, Cancel);
	}
	void NewWindow2(LPDISPATCH * ppDisp, BOOL * Cancel)
	{
		static BYTE parms[] = VTS_UNKNOWN VTS_UNKNOWN;
		InvokeHelper(0xFB, DISPATCH_METHOD, VT_EMPTY, nullptr, parms, ppDisp, Cancel);
	}
	void NavigateComplete2(LPDISPATCH pDisp, VARIANT * URL)
	{
		static BYTE parms[] = VTS_DISPATCH VTS_UNKNOWN;
		InvokeHelper(0xFC, DISPATCH_METHOD, VT_EMPTY, nullptr, parms, pDisp, URL);
	}
	void DocumentComplete(LPDISPATCH pDisp, VARIANT * URL)
	{
		static BYTE parms[] = VTS_DISPATCH VTS_UNKNOWN;
		InvokeHelper(0x103, DISPATCH_METHOD, VT_EMPTY, nullptr, parms, pDisp, URL);
	}
	void OnQuit()
	{
		InvokeHelper(0xFD, DISPATCH_METHOD, VT_EMPTY, nullptr, nullptr);
	}
	void OnVisible(BOOL Visible)
	{
		static BYTE parms[] = VTS_BOOL;
		InvokeHelper(0xFE, DISPATCH_METHOD, VT_EMPTY, nullptr, parms, Visible);
	}
	void OnToolBar(BOOL ToolBar)
	{
		static BYTE parms[] = VTS_BOOL;
		InvokeHelper(0xFF, DISPATCH_METHOD, VT_EMPTY, nullptr, parms, ToolBar);
	}
	void OnMenuBar(BOOL MenuBar)
	{
		static BYTE parms[] = VTS_BOOL;
		InvokeHelper(0x100, DISPATCH_METHOD, VT_EMPTY, nullptr, parms, MenuBar);
	}
	void OnStatusBar(BOOL StatusBar)
	{
		static BYTE parms[] = VTS_BOOL;
		InvokeHelper(0x101, DISPATCH_METHOD, VT_EMPTY, nullptr, parms, StatusBar);
	}
	void OnFullScreen(BOOL FullScreen)
	{
		static BYTE parms[] = VTS_BOOL;
		InvokeHelper(0x102, DISPATCH_METHOD, VT_EMPTY, nullptr, parms, FullScreen);
	}
	void OnTheaterMode(BOOL TheaterMode)
	{
		static BYTE parms[] = VTS_BOOL;
		InvokeHelper(0x104, DISPATCH_METHOD, VT_EMPTY, nullptr, parms, TheaterMode);
	}
	void WindowSetResizable(BOOL Resizable)
	{
		static BYTE parms[] = VTS_BOOL;
		InvokeHelper(0x106, DISPATCH_METHOD, VT_EMPTY, nullptr, parms, Resizable);
	}
	void WindowSetLeft(long Left)
	{
		static BYTE parms[] = VTS_I4;
		InvokeHelper(0x108, DISPATCH_METHOD, VT_EMPTY, nullptr, parms, Left);
	}
	void WindowSetTop(long Top)
	{
		static BYTE parms[] = VTS_I4;
		InvokeHelper(0x109, DISPATCH_METHOD, VT_EMPTY, nullptr, parms, Top);
	}
	void WindowSetWidth(long Width)
	{
		static BYTE parms[] = VTS_I4;
		InvokeHelper(0x10A, DISPATCH_METHOD, VT_EMPTY, nullptr, parms, Width);
	}
	void WindowSetHeight(long Height)
	{
		static BYTE parms[] = VTS_I4;
		InvokeHelper(0x10B, DISPATCH_METHOD, VT_EMPTY, nullptr, parms, Height);
	}
	void WindowClosing(BOOL IsChildWindow, BOOL * Cancel)
	{
		static BYTE parms[] = VTS_BOOL VTS_UNKNOWN;
		InvokeHelper(0x107, DISPATCH_METHOD, VT_EMPTY, nullptr, parms, IsChildWindow, Cancel);
	}
	void ClientToHostWindow(long * CX, long * CY)
	{
		static BYTE parms[] = VTS_UNKNOWN VTS_UNKNOWN;
		InvokeHelper(0x10C, DISPATCH_METHOD, VT_EMPTY, nullptr, parms, CX, CY);
	}
	void SetSecureLockIcon(long SecureLockIcon)
	{
		static BYTE parms[] = VTS_I4;
		InvokeHelper(0x10D, DISPATCH_METHOD, VT_EMPTY, nullptr, parms, SecureLockIcon);
	}
	void FileDownload(BOOL ActiveDocument, BOOL * Cancel)
	{
		static BYTE parms[] = VTS_BOOL VTS_UNKNOWN;
		InvokeHelper(0x10E, DISPATCH_METHOD, VT_EMPTY, nullptr, parms, ActiveDocument, Cancel);
	}
	void NavigateError(LPDISPATCH pDisp, VARIANT * URL, VARIANT * Frame, VARIANT * StatusCode, BOOL * Cancel)
	{
		static BYTE parms[] = VTS_DISPATCH VTS_UNKNOWN VTS_UNKNOWN VTS_UNKNOWN VTS_UNKNOWN;
		InvokeHelper(0x10F, DISPATCH_METHOD, VT_EMPTY, nullptr, parms, pDisp, URL, Frame, StatusCode, Cancel);
	}
	void PrintTemplateInstantiation(LPDISPATCH pDisp)
	{
		static BYTE parms[] = VTS_DISPATCH;
		InvokeHelper(0xE1, DISPATCH_METHOD, VT_EMPTY, nullptr, parms, pDisp);
	}
	void PrintTemplateTeardown(LPDISPATCH pDisp)
	{
		static BYTE parms[] = VTS_DISPATCH;
		InvokeHelper(0xE2, DISPATCH_METHOD, VT_EMPTY, nullptr, parms, pDisp);
	}
	void UpdatePageStatus(LPDISPATCH pDisp, VARIANT * nPage, VARIANT * fDone)
	{
		static BYTE parms[] = VTS_DISPATCH VTS_UNKNOWN VTS_UNKNOWN;
		InvokeHelper(0xE3, DISPATCH_METHOD, VT_EMPTY, nullptr, parms, pDisp, nPage, fDone);
	}
	void PrivacyImpactedStateChange(BOOL bImpacted)
	{
		static BYTE parms[] = VTS_BOOL;
		InvokeHelper(0x110, DISPATCH_METHOD, VT_EMPTY, nullptr, parms, bImpacted);
	}
	void NewWindow3(LPDISPATCH * ppDisp, BOOL * Cancel, unsigned long dwFlags, LPCTSTR bstrUrlContext, LPCTSTR bstrUrl)
	{
		static BYTE parms[] = VTS_UNKNOWN VTS_UNKNOWN VTS_UNKNOWN VTS_BSTR VTS_BSTR;
		InvokeHelper(0x111, DISPATCH_METHOD, VT_EMPTY, nullptr, parms, ppDisp, Cancel, dwFlags, bstrUrlContext, bstrUrl);
	}
	void SetPhishingFilterStatus(long PhishingFilterStatus)
	{
		static BYTE parms[] = VTS_I4;
		InvokeHelper(0x11A, DISPATCH_METHOD, VT_EMPTY, nullptr, parms, PhishingFilterStatus);
	}
	void WindowStateChanged(unsigned long dwWindowStateFlags, unsigned long dwValidFlagsMask)
	{
		static BYTE parms[] = VTS_UNKNOWN VTS_UNKNOWN;
		InvokeHelper(0x11B, DISPATCH_METHOD, VT_EMPTY, nullptr, parms, dwWindowStateFlags, dwValidFlagsMask);
	}
	void NewProcess(long lCauseFlag, LPDISPATCH pWB2, BOOL * Cancel)
	{
		static BYTE parms[] = VTS_I4 VTS_DISPATCH VTS_UNKNOWN;
		InvokeHelper(0x11C, DISPATCH_METHOD, VT_EMPTY, nullptr, parms, lCauseFlag, pWB2, Cancel);
	}
	void ThirdPartyUrlBlocked(VARIANT * URL, unsigned long dwCount)
	{
		static BYTE parms[] = VTS_UNKNOWN VTS_UNKNOWN;
		InvokeHelper(0x11D, DISPATCH_METHOD, VT_EMPTY, nullptr, parms, URL, dwCount);
	}
	void RedirectXDomainBlocked(LPDISPATCH pDisp, VARIANT * StartURL, VARIANT * RedirectURL, VARIANT * Frame, VARIANT * StatusCode)
	{
		static BYTE parms[] = VTS_DISPATCH VTS_UNKNOWN VTS_UNKNOWN VTS_UNKNOWN VTS_UNKNOWN;
		InvokeHelper(0x11E, DISPATCH_METHOD, VT_EMPTY, nullptr, parms, pDisp, StartURL, RedirectURL, Frame, StatusCode);
	}
	void BeforeScriptExecute(LPDISPATCH pDispWindow)
	{
		static BYTE parms[] = VTS_DISPATCH;
		InvokeHelper(0x122, DISPATCH_METHOD, VT_EMPTY, nullptr, parms, pDispWindow);
	}
	void WebWorkerStarted(unsigned long dwUniqueID, LPCTSTR bstrWorkerLabel)
	{
		static BYTE parms[] = VTS_UNKNOWN VTS_BSTR;
		InvokeHelper(0x120, DISPATCH_METHOD, VT_EMPTY, nullptr, parms, dwUniqueID, bstrWorkerLabel);
	}
	void WebWorkerFinsihed(unsigned long dwUniqueID)
	{
		static BYTE parms[] = VTS_UNKNOWN;
		InvokeHelper(0x121, DISPATCH_METHOD, VT_EMPTY, nullptr, parms, dwUniqueID);
	}

// Properties
//



};
