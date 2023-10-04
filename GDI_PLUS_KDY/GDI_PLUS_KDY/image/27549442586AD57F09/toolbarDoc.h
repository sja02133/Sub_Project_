// toolbarDoc.h : interface of the CToolbarDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOOLBARDOC_H__D1F2F298_4C2C_4BEB_A80F_381A49321E18__INCLUDED_)
#define AFX_TOOLBARDOC_H__D1F2F298_4C2C_4BEB_A80F_381A49321E18__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Line.h"

class CToolbarDoc : public CDocument
{
protected: // create from serialization only
	CToolbarDoc();
	DECLARE_DYNCREATE(CToolbarDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CToolbarDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetWidth(UINT nWidth);
	UINT m_nWidth;
	UINT GetWidth();
	UINT GetColor();
	static const COLORREF m_crColors[8];
	void DeleteContents();
	CLine* GetLine(int nIndex);
	int GetLineCount();
	CLine* AddLine(CPoint ptFrom, CPoint ptTo);
	CObArray m_oaLines;
	virtual ~CToolbarDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
// Generated message map functions
protected:
	afx_msg void OnUpdateIndicatorWidth(CCmdUI* pCmdUI);
	afx_msg void OnUpdateIndicatorColor(CCmdUI *pCmdUI);
	afx_msg void OnColorCommand(UINT nID);
	afx_msg void OnUpdateColorUI(CCmdUI* pCmdUI);
	afx_msg void OnWidthCommand(UINT nID);
	afx_msg void OnUpdateWidthUI(CCmdUI* pCmdUI);
	
	//{{AFX_MSG(CToolbarDoc)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void UpdateColorbar(int nIndex);
	UINT m_nColor;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOOLBARDOC_H__D1F2F298_4C2C_4BEB_A80F_381A49321E18__INCLUDED_)
