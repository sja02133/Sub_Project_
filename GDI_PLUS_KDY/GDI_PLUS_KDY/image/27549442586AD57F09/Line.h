// Line.h: interface for the Line class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LINE_H__33FFEF5A_CF5E_4C16_873F_2404EB49D043__INCLUDED_)
#define AFX_LINE_H__33FFEF5A_CF5E_4C16_873F_2404EB49D043__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CLine : public CObject  
{
	DECLARE_SERIAL(CLine)
public:
	void Serialize(CArchive &ar);
	void Draw(CDC *pDC);
	CLine(CPoint ptFrom, CPoint ptTo, COLORREF crColor, UINT nWidth);
	CLine();
	virtual ~CLine();

private:
	UINT m_nWidth;
	COLORREF m_crColor;
	CPoint m_ptFrom;
	CPoint m_ptTo;
};

#endif // !defined(AFX_LINE_H__33FFEF5A_CF5E_4C16_873F_2404EB49D043__INCLUDED_)
