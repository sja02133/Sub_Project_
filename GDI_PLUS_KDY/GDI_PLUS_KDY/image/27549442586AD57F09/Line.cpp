// Line.cpp: implementation of the Line class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "toolbar.h"
#include "Line.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(CLine, CObject, 1)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLine::CLine()
{

}

CLine::~CLine()
{

}

CLine::CLine(CPoint ptFrom, CPoint ptTo, COLORREF crColor, UINT nWidth)
{
	m_ptFrom = ptFrom;
	m_ptTo = ptTo;
	m_crColor = crColor;
	m_nWidth = nWidth;

}

void CLine::Draw(CDC *pDC)
{
	CPen IPen(PS_SOLID, m_nWidth, m_crColor);
	CPen* pOldPen = pDC->SelectObject(&IPen);

	pDC->MoveTo(m_ptFrom);
	pDC->LineTo(m_ptTo);
	pDC->SelectObject(pOldPen);

}

void CLine::Serialize(CArchive &ar)
{
	CObject::Serialize(ar);

	if(ar.IsStoring())
		ar << m_ptFrom << m_ptTo << (DWORD) m_crColor << m_nWidth;

	else
		ar >> m_ptFrom >> m_ptTo >> (DWORD) m_crColor >> m_nWidth;


}
