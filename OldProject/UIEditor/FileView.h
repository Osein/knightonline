#pragma once

#include "ViewTree.h"

class CFileView : public CDockablePane
{
// Construction
public:
	CFileView() noexcept;
	virtual ~CFileView();

	void AdjustLayout();
	void OnChangeVisualStyle();

	BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* lResult);

// Attributes
protected:
	CViewTree m_wndFileView;
	CImageList m_FileViewImages;

	void FillFileView();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnNMDblclk(NMHDR* pNMHDR, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()
};

