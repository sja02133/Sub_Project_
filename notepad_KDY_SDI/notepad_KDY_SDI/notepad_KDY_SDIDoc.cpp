
// notepad_KDY_SDIDoc.cpp : implementation of the CnotepadKDYSDIDoc class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "notepad_KDY_SDI.h"
#endif

#include "notepad_KDY_SDIDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CnotepadKDYSDIDoc

IMPLEMENT_DYNCREATE(CnotepadKDYSDIDoc, CDocument)

BEGIN_MESSAGE_MAP(CnotepadKDYSDIDoc, CDocument)
END_MESSAGE_MAP()


// CnotepadKDYSDIDoc construction/destruction

CnotepadKDYSDIDoc::CnotepadKDYSDIDoc() noexcept
{
	// TODO: add one-time construction code here
}

CnotepadKDYSDIDoc::~CnotepadKDYSDIDoc()
{
}

BOOL CnotepadKDYSDIDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	if (!m_viewList.IsEmpty())

	{
		reinterpret_cast<CEditView*>(m_viewList.GetHead())->SetWindowText(nullptr);
	}

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	title_format = L" - notepad_KDY";
	initialized_title = L"제목 없음";
	titleFullName = L"";
	titleName = L"";
	filePath = L"";

	TCHAR str[1000] = { 0 };
	GetModuleFileName(NULL, str, 1000);
	notepad_path = str;

	fontHeightSize = 20;

	fileInfo.be = FALSE;

	return TRUE;
}

// CnotepadKDYSDIDoc serialization

void CnotepadKDYSDIDoc::Serialize(CArchive& ar)
{
	// CEditView contains an edit control which handles all serialization
	if (!m_viewList.IsEmpty())
	{
		reinterpret_cast<CEditView*>(m_viewList.GetHead())->SerializeRaw(ar);
	}
#ifdef SHARED_HANDLERS

	if (m_viewList.IsEmpty() && ar.IsLoading())
	{
		CFile* pFile = ar.GetFile();
		pFile->Seek(0, FILE_BEGIN);
		ULONGLONG nFileSizeBytes = pFile->GetLength();
		ULONGLONG nFileSizeChars = nFileSizeBytes/sizeof(TCHAR);
		LPTSTR lpszText = (LPTSTR)malloc(((size_t)nFileSizeChars + 1) * sizeof(TCHAR));
		if (lpszText != nullptr)
		{
			ar.Read(lpszText, (UINT)nFileSizeBytes);
			lpszText[nFileSizeChars] = '\0';
			m_strThumbnailContent = lpszText;
			m_strSearchContent = lpszText;
		}
	}
#endif
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CnotepadKDYSDIDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(m_strThumbnailContent, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CnotepadKDYSDIDoc::InitializeSearchContent()
{
	// Set search contents from document's data.
	// The content parts should be separated by ";"

	// Use the entire text file content as the search content.
	SetSearchContent(m_strSearchContent);
}

void CnotepadKDYSDIDoc::SetSearchContent(const CString& value)
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

// CnotepadKDYSDIDoc diagnostics

#ifdef _DEBUG
void CnotepadKDYSDIDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CnotepadKDYSDIDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CnotepadKDYSDIDoc commands
