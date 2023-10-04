
// Multiple_Notepad_KDYView.cpp : implementation of the CMultipleNotepadKDYView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Multiple_Notepad_KDY.h"
#endif

#include "Multiple_Notepad_KDYDoc.h"
#include "Multiple_Notepad_KDYView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMultipleNotepadKDYView

IMPLEMENT_DYNCREATE(CMultipleNotepadKDYView, CEditView)

BEGIN_MESSAGE_MAP(CMultipleNotepadKDYView, CEditView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CEditView::OnFilePrintPreview)
	ON_CONTROL_REFLECT(EN_CHANGE, &CMultipleNotepadKDYView::OnEnChange)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

// CMultipleNotepadKDYView construction/destruction

CMultipleNotepadKDYView::CMultipleNotepadKDYView() noexcept
{
	// TODO: add construction code here

}

CMultipleNotepadKDYView::~CMultipleNotepadKDYView()
{
}

BOOL CMultipleNotepadKDYView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	BOOL bPreCreated = CEditView::PreCreateWindow(cs);
	cs.style &= ~(ES_AUTOHSCROLL|WS_HSCROLL);	// Enable word-wrapping

	return bPreCreated;
}


// CMultipleNotepadKDYView printing

BOOL CMultipleNotepadKDYView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default CEditView preparation
	return CEditView::OnPreparePrinting(pInfo);
}

void CMultipleNotepadKDYView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// Default CEditView begin printing
	CEditView::OnBeginPrinting(pDC, pInfo);
}

void CMultipleNotepadKDYView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// Default CEditView end printing
	CEditView::OnEndPrinting(pDC, pInfo);
}


// CMultipleNotepadKDYView diagnostics

#ifdef _DEBUG
void CMultipleNotepadKDYView::AssertValid() const
{
	CEditView::AssertValid();
}

void CMultipleNotepadKDYView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}

CMultipleNotepadKDYDoc* CMultipleNotepadKDYView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMultipleNotepadKDYDoc)));
	return (CMultipleNotepadKDYDoc*)m_pDocument;
}
#endif //_DEBUG


CMultipleNotepadKDYView* GetView()
{
	CFrameWnd* pFrame = (CFrameWnd*)(AfxGetApp()->m_pMainWnd);
	CView* pView = pFrame->GetActiveView();
	if (!pView)
		return NULL;

	if (!pView->IsKindOf(RUNTIME_CLASS(CMultipleNotepadKDYView)))
		return NULL;

	return (CMultipleNotepadKDYView*)pView;
}


// CMultipleNotepadKDYView message handlers
void CMultipleNotepadKDYView::SerializeRaw(CArchive& ar)
{
	//ar is useful
}


void CMultipleNotepadKDYView::OnEnChange()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CEditView::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	CMultipleNotepadKDYDoc* pDoc = GetDocument();
	
	CString title_name = L"";
	CString temp = L"*";
	//AfxGetMainWnd()->GetWindowTextW(title_name);
	title_name = pDoc->GetTitle();

	if (title_name.Find(L"*") != 0) {
		temp += title_name;
		//AfxGetMainWnd()->SetWindowTextW(temp);
		pDoc->SetTitle(temp);
		pDoc->SetModifiedFlag(TRUE);
	}
}


void CMultipleNotepadKDYView::OnClose()
{
	// TODO: Add your message handler code here and/or call default

	CEditView::OnClose();
}
