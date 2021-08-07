#include "pch.h"
#include <winsock.h>

#include "LocalInput.h"
#include "mmsystem.h"
#include "GameBase.h"

#include "GameProcedure.h"
#include "N3UIEdit.h"
#include "UIChat.h"
#include "GameProcMain.h"
#include "APISocket.h"
#include "GameEng.h"

#include "UIManager.h"
#include "IMouseWheelInputDlg.h"
//-----------------------------------------------------------------------------
CLocalInput::CLocalInput(void) {
	m_bNoKeyDown = FALSE;

	m_nMouseFlag = 0;
	m_nMouseFlagOld = 0;

	m_dwTickLBDown = 0;
	m_dwTickRBDown = 0;

	m_ptCurMouse.x = m_ptCurMouse.y = 0;
	m_ptOldMouse.x = m_ptOldMouse.y = 0;

	SetRect(&m_rcLBDrag, 0, 0, 0, 0);
	SetRect(&m_rcMBDrag, 0, 0, 0, 0);
	SetRect(&m_rcRBDrag, 0, 0, 0, 0);

	SetRect(&m_rcMLimit, 0, 0, 0, 0);

	memset(m_byCurKeys, 0, sizeof(m_byCurKeys));
	memset(m_byOldKeys, 0, sizeof(m_byOldKeys));
	memset(m_bKeyPresses, 0, sizeof(m_bKeyPresses));
	memset(m_bKeyPresseds, 0, sizeof(m_bKeyPresseds));
	memset(m_dwTickKeyPress, 0, sizeof(m_dwTickKeyPress));
}	

//-----------------------------------------------------------------------------
CLocalInput::~CLocalInput(void) {
	if (m_lpDIDKeyboard)
	{
		UnacquireKeyboard();
		m_lpDIDKeyboard->Release();
		m_lpDIDKeyboard = NULL;
	}

	// shutdown mouse
	if (m_lpDIDMouse)
	{
		UnacquireMouse();
		m_lpDIDMouse->Release();
		m_lpDIDMouse = NULL;
	}

	// kill directinput
	if (m_lpDI)
	{
		m_lpDI->Release();
		m_lpDI = NULL;
	}
}

//-----------------------------------------------------------------------------
BOOL CLocalInput::Init(HINSTANCE hInst, HWND hWnd, BOOL bActivateKeyboard, BOOL bActivateMouse, BOOL ExclusiveMouseAccess) {
	HRESULT rval;

	m_hWnd = hWnd; // ������ �ڵ� ���..

	rval = DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)(&m_lpDI), NULL);
	if (rval != DI_OK) return FALSE;

	// Create the keyboard device
	rval = m_lpDI->CreateDevice(GUID_SysKeyboard, &m_lpDIDKeyboard, NULL);
	if (rval == DI_OK)
	{
		m_lpDIDKeyboard->SetDataFormat(&c_dfDIKeyboard);
		m_lpDIDKeyboard->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND | DISCL_NOWINKEY);

		AcquireKeyboard();
	}

	// Create the mouse device
	rval = m_lpDI->CreateDevice(GUID_SysMouse, &m_lpDIDMouse, NULL);
	if (rval == DI_OK)
	{
		m_lpDIDMouse->SetDataFormat(&c_dfDIMouse);
		if (ExclusiveMouseAccess == FALSE)
			m_lpDIDMouse->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
		else
			m_lpDIDMouse->SetCooperativeLevel(hWnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND);

		AcquireMouse();
	}

	return TRUE;
}

//-----------------------------------------------------------------------------
void CLocalInput::KeyboardFlushData() {
	memset(m_byOldKeys, 0, NUMDIKEYS);
	memset(m_byCurKeys, 0, NUMDIKEYS);
}

//-----------------------------------------------------------------------------
void CLocalInput::MouseSetLimits(int x1, int y1, int x2, int y2) {
	m_rcMLimit.left = x1;
	m_rcMLimit.top = y1;
	m_rcMLimit.right = x2;
	m_rcMLimit.bottom = y2;
}

//-----------------------------------------------------------------------------
void CLocalInput::MouseSetPos(int x, int y) {
	// clamp non-free mouse values to limits
	if ((m_ptCurMouse.x = x) >= m_rcMLimit.right)
		m_ptCurMouse.x = m_rcMLimit.right-1;

	if ((m_ptCurMouse.y = y) >= m_rcMLimit.bottom)
		m_ptCurMouse.y = m_rcMLimit.bottom-1;

	if ((m_ptCurMouse.x = x) <= m_rcMLimit.left)
		m_ptCurMouse.x = m_rcMLimit.left+1;

	if ((m_ptCurMouse.y = y) <= m_rcMLimit.top)
		m_ptCurMouse.y = m_rcMLimit.top+1;
}

//-----------------------------------------------------------------------------
BOOL CLocalInput::KeyboardGetKeyState(int nDIKey) {
	if(nDIKey<0 || nDIKey>=NUMDIKEYS) return FALSE;
	return(m_byCurKeys[nDIKey]);
}

void CLocalInput::AcquireKeyboard()
{
	if (m_lpDIDKeyboard)
	{
		HRESULT rval = m_lpDIDKeyboard->Acquire();
		//		if(rval != DI_OK) MessageBox(::GetActiveWindow(), "Acquire Keyboard Failed.", "DirectInput", MB_OK);
		if (rval == DI_OK || rval == S_FALSE)
		{
			//			m_bKeyboard = TRUE;
			KeyboardFlushData();
			return;
		}
	}
	//	m_bKeyboard = FALSE;
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Releases access to the keyboard.
/////////////////////////////////////////////////////////////////////////////////////////////
void CLocalInput::UnacquireKeyboard()
{
	KeyboardFlushData();
	//	m_bKeyboard = FALSE;
	if (m_lpDIDKeyboard)
	{
		HRESULT rval = m_lpDIDKeyboard->Unacquire();
		//		if(rval != DI_OK) MessageBox(::GetActiveWindow(), "UnAcquire Keyboard Failed.", "DirectInput", MB_OK);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Obtains access to the mouse.
/////////////////////////////////////////////////////////////////////////////////////////////
void CLocalInput::AcquireMouse()
{
	if (m_lpDIDMouse)
	{
		HRESULT rval = m_lpDIDMouse->Acquire();
		if (rval == DI_OK || rval == S_FALSE)
		{
			return;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Releases access to the mouse.
/////////////////////////////////////////////////////////////////////////////////////////////
void CLocalInput::UnacquireMouse()
{
	m_bMouse = FALSE;
	if (m_lpDIDMouse)
	{
		HRESULT rval = m_lpDIDMouse->Unacquire();
	}
}

//-----------------------------------------------------------------------------
void CLocalInput::Tick(void) {
	HRESULT err;
	//	int oldx, oldy;
	//	BOOL isPressed;
	//	WORD i;
	//	DWORD key;

	HWND hWndActive = ::GetActiveWindow(); // ��Ŀ�̵Ǿ�������...

	///////////////////////
	//  KEYBOARD
	///////////////////////
//	if(m_bKeyboard)
//	{
	memcpy(m_byOldKeys, m_byCurKeys, NUMDIKEYS); // ���� Ű ���� ���
	err = m_lpDIDKeyboard->GetDeviceState(NUMDIKEYS, m_byCurKeys); // ���� Ű ���� ���
	if (err != DI_OK) AcquireKeyboard();
	else
	{
		m_bNoKeyDown = TRUE; // ÷�� �ƹ��͵� �ȴ���
		for (int i = 0; i < NUMDIKEYS; i++)
		{
			if (!m_byOldKeys[i] && m_byCurKeys[i]) m_bKeyPresses[i] = TRUE; // ������ ����
			else m_bKeyPresses[i] = FALSE;

			if (m_byOldKeys[i] && !m_byCurKeys[i]) m_bKeyPresseds[i] = TRUE; // ������ ���� ����..
			else m_bKeyPresseds[i] = FALSE;

			if (m_byCurKeys[i]) m_bNoKeyDown = FALSE;
		}
	}
	//	}

		///////////////////////
		//  MOUSE
		///////////////////////
	//	if(m_bMouse)
	//	{
	//		DIMOUSESTATE DIMState;
	//		err = m_lpDIDMouse->GetDeviceState(sizeof(DIMState), &DIMState); // ���콺 ���� ��� - �̻�Ÿ.. �ڲ�.. ������..
	//		if(err != DI_OK)
	//		{
	//			AcquireMouse();
	//			return;
	//		}

			// ���콺 ��ġ ���� ��������
	//		m_ptCurMouse.x += DIMState.lX;
	//		m_ptCurMouse.y += DIMState.lY;

	//		if((DIMState.rgbButtons[0] & 0x80)) m_bLBDown = TRUE; 
	//		else m_bLBDown = FALSE;
	//		if((DIMState.rgbButtons[1] & 0x80)) m_bRBDown = TRUE;
	//		else m_bRBDown = FALSE;

			// clamp non-free mouse values to limits
	//		if (m_ptCurMouse.x >= m_rcMLimit.right)
	//			m_ptCurMouse.x = m_rcMLimit.right-1;
	//
	//		if (m_ptCurMouse.y >= m_rcMLimit.bottom)
	//			m_ptCurMouse.y = m_rcMLimit.bottom-1;
	//
	//		if (m_ptCurMouse.x < m_rcMLimit.left)
	//			m_ptCurMouse.x = m_rcMLimit.left;
	//
	//		if (m_ptCurMouse.y < m_rcMLimit.top)
	//			m_ptCurMouse.y = m_rcMLimit.top;

	m_ptOldMouse = m_ptCurMouse; // �ϴ� ���� �� ����...

	RECT rcClient;
	::GetClientRect(m_hWnd, &rcClient);
	::GetCursorPos(&m_ptCurMouse); // �� �̻��ؼ�... �׳� �ý��� ���콺 Ŀ�� ��ġ ��������
	::ScreenToClient(m_hWnd, &m_ptCurMouse); // Ŭ���̾�Ʈ �������� ��ȯ

	if (PtInRect(&rcClient, m_ptCurMouse) == FALSE) //  || GetFocus() != m_hWnd) // ��ũ�� ���� �ۿ� �ְų� ��Ŀ���� ������ ������..
	{
	}
	else
	{
		// ���콺 ��ư ���� ����.
		m_nMouseFlagOld = m_nMouseFlag;
		m_nMouseFlag = 0;

		// ���콺 ���� ��������
		if (_IsKeyDown(VK_LBUTTON)) m_nMouseFlag |= MOUSE_LBDOWN;
		if (_IsKeyDown(VK_MBUTTON)) m_nMouseFlag |= MOUSE_MBDOWN;
		if (_IsKeyDown(VK_RBUTTON)) m_nMouseFlag |= MOUSE_RBDOWN;

		// ��ư Ŭ�� ����..
		if ((m_nMouseFlagOld & MOUSE_LBDOWN) == FALSE && (m_nMouseFlag & MOUSE_LBDOWN)) m_nMouseFlag |= MOUSE_LBCLICK;
		if ((m_nMouseFlagOld & MOUSE_MBDOWN) == FALSE && (m_nMouseFlag & MOUSE_MBDOWN)) m_nMouseFlag |= MOUSE_MBCLICK;
		if ((m_nMouseFlagOld & MOUSE_RBDOWN) == FALSE && (m_nMouseFlag & MOUSE_RBDOWN)) m_nMouseFlag |= MOUSE_RBCLICK;

		// ��ư���� ���� ����
		if ((m_nMouseFlagOld & MOUSE_LBDOWN) && (m_nMouseFlag & MOUSE_LBDOWN) == FALSE) m_nMouseFlag |= MOUSE_LBCLICKED;
		if ((m_nMouseFlagOld & MOUSE_MBDOWN) && (m_nMouseFlag & MOUSE_MBDOWN) == FALSE) m_nMouseFlag |= MOUSE_MBCLICKED;
		if ((m_nMouseFlagOld & MOUSE_RBDOWN) && (m_nMouseFlag & MOUSE_RBDOWN) == FALSE) m_nMouseFlag |= MOUSE_RBCLICKED;

		static DWORD dwDblClk = GetDoubleClickTime(); // �������� ���� Ŭ���ð��� ��������..
		if (m_nMouseFlag & MOUSE_LBCLICKED) // ���� ���� Ŭ�� ����
		{
			static DWORD dwCLicked = 0;
			if (timeGetTime() < dwCLicked + dwDblClk)
				m_nMouseFlag |= MOUSE_LBDBLCLK;
			dwCLicked = timeGetTime();
		}
		if (m_nMouseFlag & MOUSE_MBCLICKED) // ���� ���� Ŭ�� ����
		{
			static DWORD dwCLicked = 0;
			if (timeGetTime() < dwCLicked + dwDblClk)
				m_nMouseFlag |= MOUSE_MBDBLCLK;
			dwCLicked = timeGetTime();
		}
		if (m_nMouseFlag & MOUSE_RBCLICKED) // ���� ���� Ŭ�� ����
		{
			static DWORD dwCLicked = 0;
			if (timeGetTime() < dwCLicked + dwDblClk)
				m_nMouseFlag |= MOUSE_RBDBLCLK;
			dwCLicked = timeGetTime();
		}

		// �巡�� ���� ó��
		if (m_nMouseFlag & MOUSE_LBDOWN) { m_rcLBDrag.right = m_ptCurMouse.x; m_rcLBDrag.bottom = m_ptCurMouse.y; }
		if (m_nMouseFlag & MOUSE_MBDOWN) { m_rcMBDrag.right = m_ptCurMouse.x; m_rcMBDrag.bottom = m_ptCurMouse.y; }
		if (m_nMouseFlag & MOUSE_RBDOWN) { m_rcRBDrag.right = m_ptCurMouse.x; m_rcRBDrag.bottom = m_ptCurMouse.y; }
		if (m_nMouseFlag & MOUSE_LBCLICK) { m_rcLBDrag.left = m_ptCurMouse.x; m_rcLBDrag.top = m_ptCurMouse.y; }
		if (m_nMouseFlag & MOUSE_MBCLICK) { m_rcMBDrag.left = m_ptCurMouse.x; m_rcMBDrag.top = m_ptCurMouse.y; }
		if (m_nMouseFlag & MOUSE_RBCLICK) { m_rcRBDrag.left = m_ptCurMouse.x; m_rcRBDrag.top = m_ptCurMouse.y; }
	}
}
