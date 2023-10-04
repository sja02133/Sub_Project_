
// notepad_KDY_SDIView.h : interface of the CnotepadKDYSDIView class
//

#pragma once


int buttonText();
int buttonTextWithFile(CString fileName);

class CnotepadKDYSDIView : public CEditView
{
protected: // create from serialization only
	CnotepadKDYSDIView() noexcept;
	DECLARE_DYNCREATE(CnotepadKDYSDIView)

// Attributes
public:
	void settingTitle(BOOL existFile);
	BOOL saveLogic(BOOL checkExistFile);
	void getting_TitleName();
	BOOL checkFilePath();
	void settingFileInformation(CString fullFileString,eBOM bom);
//	void saveFile(CString filePath);
	void saveFile_second(CString filePath);
//	void loadFile();
	void loadFile_second();

	CnotepadKDYSDIDoc* GetDocument() const;
	CFont    m_fontStyle; // 폰트 스타일을 저장할 변수
	COLORREF m_clrText;
	BOOL m_bRefreshCtrl; // 설정 변경이 있을 경우 업데이트를 위한 변수
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
	virtual ~CnotepadKDYSDIView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg void OnUpdateFileNewwindow(CCmdUI* pCmdUI);
	afx_msg void OnFileNewwindow();
	afx_msg void OnUpdateMenuSaveAs(CCmdUI* pCmdUI);
	afx_msg void OnMenuSaveAs();
	afx_msg void OnUpdateFilePageProperties(CCmdUI* pCmdUI);
	afx_msg void OnFilePageProperties();
	afx_msg void OnUpdateEditFont(CCmdUI* pCmdUI);
	afx_msg void OnEditFont();
	afx_msg void OnFileNew();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnEnChange();
	afx_msg void OnFileSave();
	afx_msg void OnEditDelete();
	afx_msg void OnUpdateEditDelete(CCmdUI* pCmdUI);
	afx_msg void OnFileOpen();
	afx_msg void OnAppExit();
	afx_msg void OnEditPlus();
	afx_msg void OnEditMinus();
	afx_msg void OnEditResetPlusMinus();
	afx_msg void OnClose();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnIdclose();
};

#ifndef _DEBUG  // debug version in notepad_KDY_SDIView.cpp
inline CnotepadKDYSDIDoc* CnotepadKDYSDIView::GetDocument() const
   { return reinterpret_cast<CnotepadKDYSDIDoc*>(m_pDocument); }
#endif

