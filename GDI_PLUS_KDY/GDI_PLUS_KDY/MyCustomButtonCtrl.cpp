
#include "pch.h"
#include "MyCustomButtonCtrl.h"

CMyCustomButtonCtrl::CMyCustomButtonCtrl()
{
}

CMyCustomButtonCtrl::~CMyCustomButtonCtrl()
{
}

void CMyCustomButtonCtrl::SetImage(const CImage& image)
{
    m_Image = image; // �̹��� ����
    Invalidate();    // ��ư�� �ٽ� �׸����� ����
}

void CMyCustomButtonCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
    CDC dc;
    dc.Attach(lpDrawItemStruct->hDC);

    CRect rect(lpDrawItemStruct->rcItem); // ��ư ����

    // ��ư �׸��� ������ �����մϴ�.
    // ���� ���, �̹��� �׸��� �� ���ϴ� ������ ��ư�� �׸��ϴ�.
    dc.FillSolidRect(rect, RGB(255, 255, 255)); // ��ư ����� ������� ä��� ���� �ڵ�

    // �̹��� �׸���
    if (!m_Image.IsNull())
    {
        m_Image.Draw(dc.GetSafeHdc(), rect.left, rect.top, rect.Width(), rect.Height());
    }

    dc.Detach();
}

BEGIN_MESSAGE_MAP(CMyCustomButtonCtrl, CButton)
    //...
    ON_WM_DRAWITEM()
END_MESSAGE_MAP()

// MyButtonCtrl.cpp�� �� �Ʒ��� �߰��մϴ�.
void CMyCustomButtonCtrl::PreSubclassWindow()
{
    ModifyStyle(0, BS_OWNERDRAW); // ��ư�� Owner Draw ��Ÿ�Ϸ� ����
    CButton::PreSubclassWindow();
}