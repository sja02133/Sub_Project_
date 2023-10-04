
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
    m_Image = image; // 이미지 설정
    Invalidate();    // 버튼을 다시 그리도록 갱신
}

void CMyCustomButtonCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
    CDC dc;
    dc.Attach(lpDrawItemStruct->hDC);

    CRect rect(lpDrawItemStruct->rcItem); // 버튼 영역

    // 버튼 그리기 로직을 구현합니다.
    // 예를 들어, 이미지 그리기 등 원하는 형태의 버튼을 그립니다.
    dc.FillSolidRect(rect, RGB(255, 255, 255)); // 버튼 배경을 흰색으로 채우는 예시 코드

    // 이미지 그리기
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

// MyButtonCtrl.cpp의 맨 아래에 추가합니다.
void CMyCustomButtonCtrl::PreSubclassWindow()
{
    ModifyStyle(0, BS_OWNERDRAW); // 버튼을 Owner Draw 스타일로 변경
    CButton::PreSubclassWindow();
}