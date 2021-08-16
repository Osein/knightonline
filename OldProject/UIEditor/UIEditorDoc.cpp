
// UIEditorDoc.cpp : implementation of the CUIEditorDoc class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "UIEditor.h"
#endif

#include "UIEditorDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CUIEditorDoc

IMPLEMENT_DYNCREATE(CUIEditorDoc, CDocument)

BEGIN_MESSAGE_MAP(CUIEditorDoc, CDocument)
	ON_COMMAND(ID_FILE_SAVE, &CUIEditorDoc::OnFileSave)
END_MESSAGE_MAP()


// CUIEditorDoc construction/destruction

CUIEditorDoc::CUIEditorDoc() noexcept
{
	// TODO: add one-time construction code here

}

CUIEditorDoc::~CUIEditorDoc()
{
}

BOOL CUIEditorDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	Release();

	return TRUE;
}

void CUIEditorDoc::Release()
{
	m_RootUI.Release();
	RECT rcRegion = { 0,0,64,64 };
	m_RootUI.SetRegion(rcRegion);
	//m_SelectedUIs.clear();
}


// CUIEditorDoc serialization

void CUIEditorDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

BOOL CUIEditorDoc::OnOpenDocument(LPCTSTR lpszPathName)
{

	//	if (!CDocument::OnOpenDocument(lpszPathName))
	//		return FALSE;
	Release();
	SetCurrentDirectory(m_RootUI.PathGet().c_str());
	return m_RootUI.LoadFromFile(lpszPathName);
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CUIEditorDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CUIEditorDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data.
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CUIEditorDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CUIEditorDoc diagnostics

#ifdef _DEBUG
void CUIEditorDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CUIEditorDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CUIEditorDoc commands
