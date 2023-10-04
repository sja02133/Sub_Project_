#pragma once


// cDlgPaint dialog
#include "DlgPaintMenu.h"
#include "DlgPaintDraw.h"


class cDlgPaint : public CDialogEx
{
	DECLARE_DYNAMIC(cDlgPaint)

public:
	cDlgPaint(CWnd* pParent = nullptr);   // standard constructor
	cDlgPaint(CWnd* pParent, UINT dlgIDD);   // standard constructor
	virtual ~cDlgPaint();
	
	void cDlgPaint::drawVectorData(CDC* dc);
//	void cDlgPaint::savePaint();
	BOOL cDlgPaint::makeVirtualDC(CString file);

	bool beDrawing = FALSE;
	CPoint c_point;
	CPen pen;

	//SplitterWnd
	CSplitterWnd m_wndSplitter;



// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_PAINT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
//	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButtonChangeTextSize();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void OnCancel();
	virtual BOOL Create(LPCTSTR lpszTemplateName, CWnd* pParentWnd = NULL);
	virtual BOOL OnInitDialog();
};