
// UIEditorView.cpp : implementation of the CUIEditorView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "UIEditor.h"
#endif

#include "UIEditorDoc.h"
#include "UIEditorView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include "MainFrm.h"
#include <N3EngTool.h>


// CUIEditorView

IMPLEMENT_DYNCREATE(CUIEditorView, CView)

BEGIN_MESSAGE_MAP(CUIEditorView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_DROPFILES()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CUIEditorView construction/destruction

CUIEditorView::CUIEditorView() noexcept
{
	// TODO: add construction code here

}

CUIEditorView::~CUIEditorView()
{
}

BOOL CUIEditorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CUIEditorView drawing

void CUIEditorView::OnDraw(CDC* pDC)
{

	CRect cr;
	cr.left = 0;
	cr.top = 0;
	cr.right = 50;
	cr.bottom = 50;

	CPen SelPen(PS_DOT, 1, RGB(0, 0, 0));

	CPen* pOldPen = pDC->SelectObject(&SelPen);
	pDC->Rectangle(&cr);
	pDC->SelectStockObject(NULL_BRUSH);
	pDC->SelectObject(pOldPen);
}

void CUIEditorView::RenderEditview()
{
	CUIEditorDoc* pDoc = GetDocument();
	if (NULL == pDoc) return;
	CMainFrame* pFrm = (CMainFrame*)AfxGetMainWnd();
	CN3EngTool* pEng = &(pFrm->m_Eng);
	auto lpD3DDev = pEng->s_lpD3DDev;

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
	if (FALSE != dwFog) lpD3DDev->SetRenderState(D3DRS_FOGENABLE, FALSE);	// 2d�� fog�� �Դ´� ��.��;
	if (D3DTEXF_POINT != dwMagFilter) lpD3DDev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	if (D3DTEXF_POINT != dwMinFilter) lpD3DDev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	if (D3DTEXF_NONE != dwMipFilter) lpD3DDev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);

	// render
	CN3UIBase* pRootUI = pDoc->GetRootUI();
	if (pRootUI) pRootUI->Render();

	//int iUIC = pDoc->GetSelectedUICount();
	//for (int i = 0; i < iUIC; i++)
	//{
	//	CN3UIBase* pUI = pDoc->GetSelectedUI(i);
	//	if (pUI) pUI->Render();	// ���õ� UI�ѹ� �� �׸���(�ڿ� ���� ���� �����ϱ� �ѹ� �� �׸���. button���� ��� Ư��)
	//}

	// restore
	if (D3DZB_FALSE != dwZEnable) lpD3DDev->SetRenderState(D3DRS_ZENABLE, dwZEnable);
	if (TRUE != dwAlphaBlend) lpD3DDev->SetRenderState(D3DRS_ALPHABLENDENABLE, dwAlphaBlend);
	if (D3DBLEND_SRCALPHA != dwSrcBlend) lpD3DDev->SetRenderState(D3DRS_SRCBLEND, dwSrcBlend);
	if (D3DBLEND_INVSRCALPHA != dwDestBlend) lpD3DDev->SetRenderState(D3DRS_DESTBLEND, dwDestBlend);
	if (FALSE != dwFog) lpD3DDev->SetRenderState(D3DRS_FOGENABLE, dwFog);
	if (D3DTEXF_POINT != dwMagFilter) lpD3DDev->SetSamplerState(0, D3DSAMP_MAGFILTER, dwMagFilter);
	if (D3DTEXF_POINT != dwMinFilter) lpD3DDev->SetSamplerState(0, D3DSAMP_MINFILTER, dwMinFilter);
	if (D3DTEXF_NONE != dwMipFilter) lpD3DDev->SetSamplerState(0, D3DSAMP_MIPFILTER, dwMipFilter);
}

void CUIEditorView::RenderPreview()
{
	auto pDoc = GetDocument();
	if (NULL == pDoc) return;
	CMainFrame* pFrm = (CMainFrame*)AfxGetMainWnd();
	CN3EngTool* pEng = &(pFrm->m_Eng);
	auto lpD3DDev = pEng->s_lpD3DDev;

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
	auto pRootUI = pDoc->GetRootUI();
	if (pRootUI) pRootUI->Render();

	// restore
	if (D3DZB_FALSE != dwZEnable) lpD3DDev->SetRenderState(D3DRS_ZENABLE, dwZEnable);
	if (TRUE != dwAlphaBlend) lpD3DDev->SetRenderState(D3DRS_ALPHABLENDENABLE, dwAlphaBlend);
	if (D3DBLEND_SRCALPHA != dwSrcBlend) lpD3DDev->SetRenderState(D3DRS_SRCBLEND, dwSrcBlend);
	if (D3DBLEND_INVSRCALPHA != dwDestBlend) lpD3DDev->SetRenderState(D3DRS_DESTBLEND, dwDestBlend);
	if (FALSE != dwFog) lpD3DDev->SetRenderState(D3DRS_FOGENABLE, dwFog);
	if (D3DTEXF_POINT != dwMagFilter) lpD3DDev->SetSamplerState(0, D3DSAMP_MAGFILTER, dwMagFilter);
	if (D3DTEXF_POINT != dwMinFilter) lpD3DDev->SetSamplerState(0, D3DSAMP_MINFILTER, dwMinFilter);
	if (D3DTEXF_NONE != dwMipFilter) lpD3DDev->SetSamplerState(0, D3DSAMP_MIPFILTER, dwMipFilter);
}

void CUIEditorView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CUIEditorView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CUIEditorView diagnostics

#ifdef _DEBUG
void CUIEditorView::AssertValid() const
{
	CView::AssertValid();
}

void CUIEditorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CUIEditorDoc* CUIEditorView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CUIEditorDoc)));
	return (CUIEditorDoc*)m_pDocument;
}
#endif //_DEBUG


// CUIEditorView message handlers


void CUIEditorView::OnDropFiles(HDROP hDropInfo)
{
	auto pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	char szFile[MAX_PATH];
	char* szExt = NULL;
	UINT uiFiles;

	uiFiles = DragQueryFile(hDropInfo, 0xFFFF, NULL, 0);

	::DragQueryFileA(hDropInfo, 0, szFile, MAX_PATH - 1);
	::DragFinish(hDropInfo);

	int nLen = strlen(szFile);

	szExt = szFile + nLen - 3;

	if (0 == lstrcmpiA(szExt, "uif") && pDoc)
	{
		const char* ccFile = &szFile[0];

		pDoc->OnOpenDocument(ccFile);
		pDoc->UpdateAllViews(NULL);
	}

	CView::OnDropFiles(hDropInfo);
}


void CUIEditorView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	CMainFrame* pFrm = (CMainFrame*)AfxGetMainWnd();
	if (pFrm)
	{
		static bool bInitMainWnd = false;
		if (bInitMainWnd && pFrm->m_Eng.Reset(TRUE, cx, cy, 0))
		{
			Invalidate();
		}
		bInitMainWnd = true;
	}
}


BOOL CUIEditorView::OnEraseBkgnd(CDC* pDC)
{
	CUIEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return TRUE;

	CMainFrame* pFrm = (CMainFrame*)AfxGetMainWnd();
	CN3EngTool* pEng = &(pFrm->m_Eng);

	if (!pEng->s_lpD3DDev) {
		return TRUE;
	}

	pEng->s_SndMgr.Tick();

	CRect rc;
	GetClientRect(&rc);
	pEng->SetViewPort(rc);
	pEng->Clear(m_BkgndColor, rc);

	pEng->s_lpD3DDev->BeginScene();

	switch (m_eMode)
	{
	case UIEMODE_PREVIEW:
		RenderPreview();
		break;
	case UIEMODE_EDIT:
		RenderEditview();
		break;
	}

	pEng->s_lpD3DDev->EndScene();
	pEng->Present(m_hWnd);

	return TRUE;
}
