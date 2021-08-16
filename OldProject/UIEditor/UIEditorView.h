
// UIEditorView.h : interface of the CUIEditorView class
//

#pragma once


class CUIEditorView : public CView
{
protected: // create from serialization only
	CUIEditorView() noexcept;
	DECLARE_DYNCREATE(CUIEditorView)

// Attributes
public:
	enum	eUIE_MODE { UIEMODE_PREVIEW = 0, UIEMODE_EDIT, NUM_UIEMODE };
	CUIEditorDoc* GetDocument() const;
	D3DCOLOR	m_BkgndColor = 0xff606060;

protected:
	eUIE_MODE	m_eMode{ eUIE_MODE::UIEMODE_PREVIEW };

// Operations
public:

protected:
	void	RenderPreview();
	void	RenderEditview();

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementation
public:
	virtual ~CUIEditorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

#ifndef _DEBUG  // debug version in UIEditorView.cpp
inline CUIEditorDoc* CUIEditorView::GetDocument() const
   { return reinterpret_cast<CUIEditorDoc*>(m_pDocument); }
#endif

