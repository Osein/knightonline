
// UIEditorDoc.h : interface of the CUIEditorDoc class
//


#pragma once
#include <N3UIBase.h>


class CUIEditorDoc : public CDocument
{
protected: // create from serialization only
	CUIEditorDoc() noexcept;
	DECLARE_DYNCREATE(CUIEditorDoc)

// Attributes
public:

protected:
	CN3UIBase	m_RootUI;

// Operations
public:
	CN3UIBase* GetRootUI() { return &m_RootUI; }
	void CUIEditorDoc::Release();

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CUIEditorDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	BOOL OnOpenDocument(LPCTSTR lpszPathName);

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
