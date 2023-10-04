#pragma once
#include "pch.h"

// MyButtonCtrl.h
#pragma once

class CMyCustomButtonCtrl : public CButton
{
public:
    CMyCustomButtonCtrl();
    virtual ~CMyCustomButtonCtrl();
    void PreSubclassWindow();
    void SetImage(const CImage& image); // 버튼에 이미지를 설정하는 함수

protected:
    virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

private:
    CImage m_Image; // 버튼에 사용할 이미지

    DECLARE_MESSAGE_MAP()
};


