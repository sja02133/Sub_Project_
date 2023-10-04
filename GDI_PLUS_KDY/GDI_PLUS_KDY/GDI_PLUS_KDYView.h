
// GDI_PLUS_KDYView.h : interface of the CGDIPLUSKDYView class
//
#include "figure_class.h"
#include "MyCustomButtonCtrl.h"
#pragma once
class CGDIPLUSKDYView : public CView, public Figure_manager
{
private:
	BOOL m_bDrawing;        // �׸��� �۾� ������ ����
	BOOL m_MouseUp;		// ���� ���콺 �� ���� Ȯ��
	CPoint m_ptStart;       // ���� ������
	CPoint m_ptEnd;         // ���� ����
	before_point bPoint;
	CPoint save_ptStart;	// ���콺 �ٿ� �� ������ ����
	CPoint save_ptEnd;		// ���콺 �� �� ���� ����

	BYTE m_figureType = 0;
protected:
	CMyCustomButtonCtrl m_ButtonCtrl; // Ŀ���� ��ư ��Ʈ�� ��� ������ �߰�
	Figure_manager* figureManager;
	void delete_memory_figureManager();
protected: // create from serialization only
	CGDIPLUSKDYView() noexcept;
	DECLARE_DYNCREATE(CGDIPLUSKDYView)

// Attributes
public:
	CGDIPLUSKDYDoc* GetDocument() const;

	//custom function
public:
	void initialize_beforePoint(before_point& bPoint);
	BOOL check_Initialized_point(CPoint sPoint, CPoint ePoint);
	void DrawArrowLine(CDC* pDC, CPoint ptStart, CPoint ptEnd);
	void EraseArrowTriangle(CDC* pDC, CPoint ptStart, CPoint ptEnd);
	void DrawArrowTriangle(CDC* pDC, CPoint ptStart, CPoint ptEnd);
	
	BOOL changeButtonColor(int ID);
	
	// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CGDIPLUSKDYView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	virtual void OnInitialUpdate();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
//	afx_msg void OnButtonArrowLine();
//	afx_msg void OnButtonCircle();
//	afx_msg void OnButtonRectangle();
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
};

#ifndef _DEBUG  // debug version in GDI_PLUS_KDYView.cpp
inline CGDIPLUSKDYDoc* CGDIPLUSKDYView::GetDocument() const
   { return reinterpret_cast<CGDIPLUSKDYDoc*>(m_pDocument); }
#endif

