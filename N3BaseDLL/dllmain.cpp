#define WIN32_LEAN_AND_MEAN

static int N3FORMAT_VER_DEFAULT = 0x00000002;

#include <d3d9.h>
#include <d3dx9.h>
#include <Windows.h>
#include <crtdbg.h>
#include <N3EngTool.h>
#include <N3UIBase.h>

static CN3EngTool* s_Eng;
static CN3UIBase* s_rootUi;

extern "C" __declspec(dllexport)
bool InitEngine(HINSTANCE hInst, HWND hWnd)
{
	s_Eng = new CN3EngTool();
	if (!s_Eng->Init(
		TRUE,
		hWnd,
		64,
		64,
		32, TRUE
	)) return false;

	return true;
}

extern "C" __declspec(dllexport)
HWND GetWindowHandle()
{
	return s_Eng->s_hWndBase;
}

extern "C" __declspec(dllexport)
bool RenderScene()
{
	if (!s_Eng->s_lpD3DDev) {
		return false;
	}

    RECT rc;
    D3DCOLOR	m_BkgndColor = 0xff606060;
	rc.left = 0;
	rc.top = 0;
	rc.right = 200;
	rc.bottom = 200;
	s_Eng->SetViewPort(rc);
    s_Eng->Clear(m_BkgndColor, &rc);

    s_Eng->s_lpD3DDev->BeginScene();

	auto lpD3DDev = s_Eng->s_lpD3DDev;

	// back up old state
	DWORD dwZEnable, dwAlphaBlend, dwSrcBlend, dwDestBlend, dwFog;
	lpD3DDev->GetRenderState(D3DRS_ZENABLE, &dwZEnable);
	lpD3DDev->GetRenderState(D3DRS_ALPHABLENDENABLE, &dwAlphaBlend);
	lpD3DDev->GetRenderState(D3DRS_SRCBLEND, &dwSrcBlend);
	lpD3DDev->GetRenderState(D3DRS_DESTBLEND, &dwDestBlend);
	lpD3DDev->GetRenderState(D3DRS_FOGENABLE, &dwFog);
	DWORD dwMagFilter, dwMinFilter, dwMipFilter;
	lpD3DDev->GetSamplerState(0, D3DSAMP_MAGFILTER, &dwMagFilter);
	lpD3DDev->GetSamplerState(0, D3DSAMP_MINFILTER, &dwMinFilter);
	lpD3DDev->GetSamplerState(0, D3DSAMP_MIPFILTER, &dwMipFilter);

	// set state
	if (D3DZB_FALSE != dwZEnable) lpD3DDev->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	if (TRUE != dwAlphaBlend) lpD3DDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	if (D3DBLEND_SRCALPHA != dwSrcBlend) lpD3DDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	if (D3DBLEND_INVSRCALPHA != dwDestBlend) lpD3DDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	if (FALSE != dwFog) lpD3DDev->SetRenderState(D3DRS_FOGENABLE, FALSE);
	if (D3DTEXF_POINT != dwMagFilter) lpD3DDev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	if (D3DTEXF_POINT != dwMinFilter) lpD3DDev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	if (D3DTEXF_NONE != dwMipFilter) lpD3DDev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);

	// render
	if (s_rootUi) s_rootUi->Render();

	// restore
	if (D3DZB_FALSE != dwZEnable) lpD3DDev->SetRenderState(D3DRS_ZENABLE, dwZEnable);
	if (TRUE != dwAlphaBlend) lpD3DDev->SetRenderState(D3DRS_ALPHABLENDENABLE, dwAlphaBlend);
	if (D3DBLEND_SRCALPHA != dwSrcBlend) lpD3DDev->SetRenderState(D3DRS_SRCBLEND, dwSrcBlend);
	if (D3DBLEND_INVSRCALPHA != dwDestBlend) lpD3DDev->SetRenderState(D3DRS_DESTBLEND, dwDestBlend);
	if (FALSE != dwFog) lpD3DDev->SetRenderState(D3DRS_FOGENABLE, dwFog);
	if (D3DTEXF_POINT != dwMagFilter) lpD3DDev->SetSamplerState(0, D3DSAMP_MAGFILTER, dwMagFilter);
	if (D3DTEXF_POINT != dwMinFilter) lpD3DDev->SetSamplerState(0, D3DSAMP_MINFILTER, dwMinFilter);
	if (D3DTEXF_NONE != dwMipFilter) lpD3DDev->SetSamplerState(0, D3DSAMP_MIPFILTER, dwMipFilter);

    s_Eng->s_lpD3DDev->EndScene();
	s_Eng->Present(s_Eng->s_hWndBase);

	return true;
}

LRESULT CALLBACK WndProcMain(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	s_rootUi->Render();
	return DefWindowProc(hWnd, message, wParam, lParam);
}

extern "C" __declspec(dllexport)
HWND CreateMainWindow(HINSTANCE hInstance)
{
	WNDCLASS    wc{};

	wc.lpfnWndProc = WndProcMain;
	wc.hInstance = hInstance;
	wc.hIcon = NULL;
	wc.hCursor = NULL;
	wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = "Knight OnLine Client";

	RegisterClass(&wc);

	DWORD	style = WS_POPUP | WS_CLIPCHILDREN;
	HWND window = ::CreateWindow("Knight OnLine Client", "Knight OnLine Client", style, 0, 0, CN3Base::s_Options.iViewWidth, CN3Base::s_Options.iViewHeight, NULL, NULL, hInstance, NULL);
	return window;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
#if _DEBUG
        _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

