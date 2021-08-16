#include "pch.h"
#include "framework.h"
#include "mainfrm.h"
#include "FileView.h"
#include "Resource.h"
#include "UIEditor.h"
#include <filesystem>
#include "UIEditorDoc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileView

CFileView::CFileView() noexcept
{
}

CFileView::~CFileView()
{
}

BEGIN_MESSAGE_MAP(CFileView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_NOTIFY(NM_DBLCLK, ID_FILE_LIST_CTRL, &CFileView::OnNMDblclk)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceBar message handlers

int CFileView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Create view:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | CS_DBLCLKS;

	if (!m_wndFileView.Create(dwViewStyle, rectDummy, this, ID_FILE_LIST_CTRL))
	{
		TRACE0("Failed to create file view\n");
		return -1;      // fail to create
	}

	// Load view images:
	m_FileViewImages.Create(IDB_FILE_VIEW, 16, 0, RGB(255, 0, 255));



	OnChangeVisualStyle();

	// Fill in some static tree view data (dummy code, nothing magic here)
	FillFileView();
	AdjustLayout();

	return 0;
}

void CFileView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CFileView::FillFileView()
{
	for (const auto& entry : std::filesystem::directory_iterator("UI")) {
		if (entry.path().extension().string() == ".uif") {
			m_wndFileView.InsertItem(entry.path().filename().string().c_str(), 2, 2);
		}
	}
}

void CFileView::AdjustLayout()
{
	if (GetSafeHwnd() == nullptr)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	m_wndFileView.SetWindowPos(nullptr, rectClient.left, rectClient.top, rectClient.Width(), rectClient.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
}

void CFileView::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect rectTree;
	m_wndFileView.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void CFileView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	m_wndFileView.SetFocus();
}

void CFileView::OnChangeVisualStyle()
{
	m_FileViewImages.DeleteImageList();

	UINT uiBmpId = theApp.m_bHiColorIcons ? IDB_FILE_VIEW_24 : IDB_FILE_VIEW;

	CBitmap bmp;
	if (!bmp.LoadBitmap(uiBmpId))
	{
		TRACE(_T("Can't load bitmap: %x\n"), uiBmpId);
		ASSERT(FALSE);
		return;
	}

	BITMAP bmpObj;
	bmp.GetBitmap(&bmpObj);

	UINT nFlags = ILC_MASK;

	nFlags |= (theApp.m_bHiColorIcons) ? ILC_COLOR24 : ILC_COLOR4;

	m_FileViewImages.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
	m_FileViewImages.Add(&bmp, RGB(255, 0, 255));

	m_wndFileView.SetImageList(&m_FileViewImages, TVSIL_NORMAL);
}

BOOL CFileView::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT *pResult)
{
	return CDockablePane::OnNotify(wParam, lParam, pResult);
}

void CFileView::OnNMDblclk(NMHDR* pNMHDR, LRESULT* pResult)
{
	CPoint ptTree;
	m_wndFileView.ScreenToClient(&ptTree);

	UINT uFlags;
	HTREEITEM hCurItem = m_wndFileView.HitTest(ptTree, &uFlags);
	HTREEITEM hOldItem = m_wndFileView.GetSelectedItem();

	TVITEM item = { 0 };
	item.hItem = hOldItem;
	TCHAR buf[MAX_PATH];
	item.cchTextMax = MAX_PATH;
	item.pszText = buf;
	item.mask = TVIF_TEXT;
	TreeView_GetItem(m_wndFileView, &item);
	std::string fileName(buf);

	std::string filePath("C:\\Users\\Osein\\source\\repos\\KnightOnline\\Data\\UI\\");
	filePath.append(buf);

	auto mainFrame = (CMainFrame*)AfxGetMainWnd();
	auto activeDoc = (CUIEditorDoc*)mainFrame->GetActiveDocument();

	activeDoc->OnOpenDocument(filePath.c_str());
	mainFrame->GetActiveView()->Invalidate();

	*pResult = 0;
}
