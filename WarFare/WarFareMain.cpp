#include "pch.h"
#include "UIChat.h"
#include "GameEng.h"
#include "resource.h"
#include "N3SndMgr.h"
#include "N3UIEdit.h"
#include "PacketDef.h"
#include "APISocket.h"
#include "PlayerMySelf.h"
#include "GameProcMain.h"
#include "N3WorldManager.h"
#include "../Server/shared/Ini.h"
#include "time.h"
#include "DFont.h"
#include <winsock.h>
#include "IMouseWheelInputDlg.h"
#include "UIManager.h"

//-----------------------------------------------------------------------------
/*
- NOTE: WndProcMain processes the messages for the main window
*/
LRESULT CALLBACK WndProcMain(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND: {
		uint16_t wNotifyCode = HIWORD(wParam); // notification code
		CN3UIEdit* pEdit = CN3UIEdit::GetFocusedEdit();

		if (wNotifyCode == EN_CHANGE && pEdit) {
			uint16_t wID = LOWORD(wParam); // item, control, or accelerator identifier
			HWND hwndCtl = (HWND)lParam;

			if (CN3UIEdit::s_hWndEdit == hwndCtl) {
				pEdit->UpdateTextFromEditCtrl();
				pEdit->UpdateCaretPosFromEditCtrl();
				CGameProcedure::SetGameCursor(CGameProcedure::s_hCursorNormal);
			}
		}
	} break;

	case WM_NOTIFY: {
		int idCtrl = (int)wParam;
		NMHDR* pnmh = (NMHDR*)lParam;
	} break;

	case WM_KEYDOWN: {
		int iLangID = ::GetUserDefaultLangID();
		if (iLangID == 0x0404) { // Taiwan Language
			CUIChat* pUIChat = CGameProcedure::s_pProcMain->m_pUIChatDlg;
			int iVK = (int)wParam;

			if (
				pUIChat && iVK != VK_ESCAPE && iVK != VK_RETURN &&
				CGameProcedure::s_pProcMain &&
				CGameProcedure::s_pProcActive == CGameProcedure::s_pProcMain &&
				!pUIChat->IsChatMode()
				) {
				if (!(GetKeyState(VK_CONTROL) & 0x8000)) {
					pUIChat->SetFocus();
					PostMessage(CN3UIEdit::s_hWndEdit, WM_KEYDOWN, wParam, lParam);
					return 0;
				}
			}
		}
	} break;

	case WM_SOCKETMSG: {
		switch (WSAGETSELECTEVENT(lParam))
		{
		case FD_CONNECT: {
			//TRACE("Socket connected..\n");
		} break;
		case FD_CLOSE: {
			if (CGameProcedure::s_bNeedReportConnectionClosed)
				CGameProcedure::ReportServerConnectionClosed(true);
			//TRACE("Socket closed..\n");
		}  break;
		case FD_READ: {
			CGameProcedure::s_pSocket->Receive();
		} break;
		default: {
			__ASSERT(0, "WM_SOCKETMSG: unknown socket flag.");
		} break;
		}
	} break;

		/*
	case WM_ACTIVATE: {
		int iActive = LOWORD(wParam);           // activation flag
		int iMinimized = (BOOL) HIWORD(wParam); // minimized flag
		HWND hwndPrevious = (HWND) lParam;      // window handle

		switch(iActive)
		{
			case WA_CLICKACTIVE:
			case WA_ACTIVE: {
				#ifdef _DEBUG
					g_bActive = TRUE;
				#endif
			} break;
			case WA_INACTIVE: {
				#ifdef _DEBUG
					g_bActive = FALSE;
				#endif

				if(CGameProcedure::s_bWindowed == false) {
					CLogWriter::Write("WA_INACTIVE.");
					PostQuitMessage(0);
				}
			} break;
		}
	} break;
	*/

	/*
case WM_CLOSE:
case WM_DESTROY:
case WM_QUIT: {
	CGameProcedure::s_pSocket->Disconnect();
	CGameProcedure::s_pSocketSub->Disconnect();

	PostQuitMessage(0);
} break;
*/

	case WM_MOUSEWHEEL: {
		if (CGameProcedure::s_pProcActive == CGameProcedure::s_pProcMain) {
			float fDelta = ((int16_t)HIWORD(wParam)) * 0.05f;

			CN3UIBase* focused = CGameProcedure::s_pUIMgr->GetFocusedUI();

			if (focused)
			{
				int key = fDelta > 0 ? DIK_PGUP : DIK_PGDN;
				if (IMouseWheelInputDlg* t = dynamic_cast<IMouseWheelInputDlg*>(focused))
					t->OnKeyPress(key);
				else
					CGameProcedure::s_pEng->CameraZoom(fDelta);
			}
			else
				CGameProcedure::s_pEng->CameraZoom(fDelta);
		}
	} break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

/////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////
HWND CreateMainWindow(HINSTANCE hInstance)
{
	WNDCLASS    wc;

	//  only register the window class once - use hInstance as a flag. 
	wc.style = 0;
	wc.lpfnWndProc = (WNDPROC)WndProcMain;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = NULL; // LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MAIN));
	wc.hCursor = NULL;
	wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = "Knight OnLine Client";

	if (0 == ::RegisterClass(&wc))
	{
		CLogWriter::Write("Cannot register window class.");
		exit(-1);
	}

	DWORD	style = WS_POPUP | WS_CLIPCHILDREN;
	return ::CreateWindow("Knight OnLine Client", "Knight OnLine Client", style, 0, 0, CN3Base::s_Options.iViewWidth, CN3Base::s_Options.iViewHeight, NULL, NULL, hInstance, NULL);
}

const int MAX_GAME_PROCEDURE = 8;

int APIENTRY WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow)
{
	char szPath[_MAX_PATH] = "";
	GetCurrentDirectory(_MAX_PATH, szPath);
	CN3Base::PathSet(szPath);

	// ���� �б�..
	char szIniPath[_MAX_PATH] = "";
	lstrcpy(szIniPath, CN3Base::PathGet().c_str());
	lstrcat(szIniPath, "Option.Ini");

	CN3Base::s_Options.iTexLOD_Chr = GetPrivateProfileInt("Texture", "LOD_Chr", 0, szIniPath);
	if (CN3Base::s_Options.iTexLOD_Chr < 0) CN3Base::s_Options.iTexLOD_Chr = 0;
	if (CN3Base::s_Options.iTexLOD_Chr >= 2) CN3Base::s_Options.iTexLOD_Chr = 1;

	CN3Base::s_Options.iTexLOD_Shape = GetPrivateProfileInt("Texture", "LOD_Shape", 0, szIniPath);
	if (CN3Base::s_Options.iTexLOD_Shape < 0) CN3Base::s_Options.iTexLOD_Shape = 0;
	if (CN3Base::s_Options.iTexLOD_Shape >= 2) CN3Base::s_Options.iTexLOD_Shape = 1;

	CN3Base::s_Options.iTexLOD_Terrain = GetPrivateProfileInt("Texture", "LOD_Terrain", 0, szIniPath);
	if (CN3Base::s_Options.iTexLOD_Terrain < 0) CN3Base::s_Options.iTexLOD_Terrain = 0;
	if (CN3Base::s_Options.iTexLOD_Terrain >= 2) CN3Base::s_Options.iTexLOD_Terrain = 1;

	CN3Base::s_Options.iUseShadow = GetPrivateProfileInt("Shadow", "Use", 1, szIniPath);

	CN3Base::s_Options.iViewWidth = GetPrivateProfileInt("ViewPort", "Width", 1024, szIniPath);
	CN3Base::s_Options.iViewHeight = GetPrivateProfileInt("ViewPort", "Height", 768, szIniPath);
	if (1024 == CN3Base::s_Options.iViewWidth) CN3Base::s_Options.iViewHeight = 768;
	else if (1280 == CN3Base::s_Options.iViewWidth) CN3Base::s_Options.iViewHeight = 1024;
	else if (1600 == CN3Base::s_Options.iViewWidth) CN3Base::s_Options.iViewHeight = 1200;
	else
	{
		CN3Base::s_Options.iViewWidth = 1024;
		CN3Base::s_Options.iViewHeight = 768;
	}

	CN3Base::s_Options.iViewColorDepth = GetPrivateProfileInt("ViewPort", "ColorDepth", 16, szIniPath);
	if (CN3Base::s_Options.iViewColorDepth != 16 && CN3Base::s_Options.iViewColorDepth != 32)
		CN3Base::s_Options.iViewColorDepth = 16;
	CN3Base::s_Options.iViewDist = GetPrivateProfileInt("ViewPort", "Distance", 512, szIniPath);
	if (CN3Base::s_Options.iViewDist < 256) CN3Base::s_Options.iViewDist = 256;
	if (CN3Base::s_Options.iViewDist > 512) CN3Base::s_Options.iViewDist = 512;

	CN3Base::s_Options.iEffectSndDist = GetPrivateProfileInt("Sound", "Distance", 48, szIniPath);
	if (CN3Base::s_Options.iEffectSndDist < 20) CN3Base::s_Options.iEffectSndDist = 20;
	if (CN3Base::s_Options.iEffectSndDist > 48) CN3Base::s_Options.iEffectSndDist = 48;

	int iSndEnable = GetPrivateProfileInt("Sound", "Enable", 1, szIniPath);
	CN3Base::s_Options.bSndEnable = (iSndEnable) ? true : false; // ����...

	int iSndDuplicate = GetPrivateProfileInt("Sound", "Duplicate", 0, szIniPath);
	CN3Base::s_Options.bSndDuplicated = (iSndDuplicate) ? true : false; // ����...

	int iWindowCursor = GetPrivateProfileInt("Cursor", "WindowCursor", 1, szIniPath);
	CN3Base::s_Options.bWindowCursor = (iWindowCursor) ? true : false; // cursor...

	// ���� �����츦 �����..
	HWND hWndMain = CreateMainWindow(hInstance);
	if (NULL == hWndMain)
	{
		CLogWriter::Write("Cannot create window.");
		exit(-1);
	}
	::ShowWindow(hWndMain, nCmdShow); // �����ش�..
	::SetActiveWindow(hWndMain);

	// Launcher ���׷��̵�..
	FILE* pFile = fopen("Launcher2.exe", "r"); // ���׷��̵� �Ұ� ���� �� �ش�..
	if (pFile)
	{
		fclose(pFile);
		if (::DeleteFile("Launcher.exe")) // ���� �� �����..
		{
			::rename("Launcher2.exe", "Launcher.exe"); // �̸��� �ٲپ� �ش�..
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	// Static Member ����...
	CGameProcedure::StaticMemberInit(hInstance, hWndMain);
	CGameProcedure::ProcActiveSet((CGameProcedure*)CGameProcedure::s_pProcLogIn);

#if _DEBUG
	HDC hDC = GetDC(hWndMain);
#endif // #if _DEBUG

	BOOL bGotMsg = FALSE;

	MSG msg; memset(&msg, 0, sizeof(MSG));
	while (WM_QUIT != msg.message)
	{
		// Use PeekMessage() if the app is active, so we can use idle time to
		// render the scene. Else, use GetMessage() to avoid eating CPU time.
		bGotMsg = PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE);

		if (bGotMsg)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			CGameProcedure::TickActive();
			CGameProcedure::RenderActive();
#if _DEBUG
			static float fTimePrev = CN3Base::TimeGet();
			static char szDebugs[4][256] = { "", "", "", "" };
			float fTime = CN3Base::TimeGet();
			if (fTime > fTimePrev + 0.5f)
			{
				fTimePrev = fTime;

				sprintf(szDebugs[0], "���� : ����(%d) Ÿ��(%d) || Object : ����(%d) �κм�(%d) ������(%d)",
					CN3Base::s_RenderInfo.nTerrain_Polygon,
					CN3Base::s_RenderInfo.nTerrain_Tile_Polygon,
					CN3Base::s_RenderInfo.nShape,
					CN3Base::s_RenderInfo.nShape_Part,
					CN3Base::s_RenderInfo.nShape_Polygon);

				sprintf(szDebugs[1], "ĳ���� : ����(%d), ��Ʈ��(%d), ������(%d), ����(%d), ����������(%d)",
					CN3Base::s_RenderInfo.nChr,
					CN3Base::s_RenderInfo.nChr_Part,
					CN3Base::s_RenderInfo.nChr_Polygon,
					CN3Base::s_RenderInfo.nChr_Plug,
					CN3Base::s_RenderInfo.nChr_Plug_Polygon);

				sprintf(szDebugs[2], "Camera : Lens(%.1f) NearPlane(%.1f) FarPlane(%.1f)",
					D3DXToDegree(CN3Base::s_CameraData.fFOV),
					CN3Base::s_CameraData.fNP,
					CN3Base::s_CameraData.fFP);

				if (CGameProcedure::s_pProcMain && CGameBase::ACT_WORLD && CGameBase::ACT_WORLD->GetSkyRef())
				{
					int iYear = 0, iMonth = 0, iDay = 0, iH = 0, iM = 0;
					CGameBase::ACT_WORLD->GetSkyRef()->GetGameTime(&iYear, &iMonth, &iDay, &iH, &iM);
					sprintf(szDebugs[3], "%.2f Frm/Sec, %d��%d��%d�� %d��%d��", CN3Base::s_fFrmPerSec, iYear, iMonth, iDay, iH, iM);
				}
				else szDebugs[3][0] = NULL;
			}

			for (int i = 0; i < 4; i++)
				if (szDebugs[i])
					TextOut(hDC, 0, i * 18, szDebugs[i], lstrlen(szDebugs[i])); // ȭ�鿡 ������ ���� ǥ��..
#endif // #if _DEBUG
		}
	}

	CGameProcedure::StaticMemberRelease();
	return msg.wParam;
}
