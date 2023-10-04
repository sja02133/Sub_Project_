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
    void SetImage(const CImage& image); // ��ư�� �̹����� �����ϴ� �Լ�

protected:
    virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

private:
    CImage m_Image; // ��ư�� ����� �̹���

    DECLARE_MESSAGE_MAP()
};


