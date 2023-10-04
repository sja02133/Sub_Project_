
// Multiple_Notepad_KDYView.h : interface of the CMultipleNotepadKDYView class
//

#pragma once

class CMultipleNotepadKDYView : public CEditView
{
protected: // create from serialization only
	CMultipleNotepadKDYView() noexcept;
	DECLARE_DYNCREATE(CMultipleNotepadKDYView)

// Attributes
public:
	CMultipleNotepadKDYDoc* GetDocument() const;
	//CMultipleNotepadKDYView* GetView();
	void SerializeRaw(CArchive& ar);
// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CMultipleNotepadKDYView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChange();
	afx_msg void OnClose();
};

#ifndef _DEBUG  // debug version in Multiple_Notepad_KDYView.cpp
inline CMultipleNotepadKDYDoc* CMultipleNotepadKDYView::GetDocument() const
   { return reinterpret_cast<CMultipleNotepadKDYDoc*>(m_pDocument); }
#endif

