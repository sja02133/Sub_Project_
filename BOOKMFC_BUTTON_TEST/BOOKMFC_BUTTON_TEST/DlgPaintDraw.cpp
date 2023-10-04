// DlgPaintDraw.cpp : implementation file
//

#include "pch.h"
#include "BOOKMFC_BUTTON_TEST.h"
#include "DlgPaintDraw.h"
#include "afxdialogex.h"


// cDlgPaintDraw dialog

IMPLEMENT_DYNAMIC(cDlgPaintDraw, CDialogEx)

cDlgPaintDraw::cDlgPaintDraw(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_PAINT_DRAW, pParent)
{

}

cDlgPaintDraw::~cDlgPaintDraw()
{
}

void cDlgPaintDraw::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(cDlgPaintDraw, CDialogEx)
	ON_WM_CLOSE()
	ON_WM_PAINT()
END_MESSAGE_MAP()


// cDlgPaintDraw message handlers

BOOL cDlgPaintDraw::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void cDlgPaintDraw::savePaintJPG() {

}

void cDlgPaintDraw::savePaintBMP() {
	HWND hWnd = ::GetActiveWindow();

	HDC hdc = ::GetDC(hWnd);

	CRect rtClient;
	this->GetWindowRect(&rtClient);

	int width =  rtClient.Width();
	int height = rtClient.Height();

	BITMAPINFO dib_define;
	dib_define.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	dib_define.bmiHeader.biWidth = width;
	dib_define.bmiHeader.biHeight = height;
	dib_define.bmiHeader.biPlanes = 1;
	dib_define.bmiHeader.biBitCount = 24;
	dib_define.bmiHeader.biCompression = BI_RGB;
	dib_define.bmiHeader.biSizeImage = (((width * 24 + 31) & ~31) >> 3) * height;
	dib_define.bmiHeader.biXPelsPerMeter = 0;
	dib_define.bmiHeader.biYPelsPerMeter = 0;
	dib_define.bmiHeader.biClrImportant = 0;
	dib_define.bmiHeader.biClrUsed = 0;

	BYTE* p_image_data = NULL; 

	HBITMAP h_bitmap = ::CreateDIBSection(hdc, &dib_define, DIB_RGB_COLORS, (void**)&p_image_data, 0, 0);

	HDC h_memory_dc = ::CreateCompatibleDC(hdc);

	HBITMAP h_old_bitmap = (HBITMAP)::SelectObject(h_memory_dc, h_bitmap);

	::BitBlt(h_memory_dc, 0, 0, width, height, hdc, 0, 0, SRCCOPY);

	::SelectObject(h_memory_dc, h_old_bitmap);

	DeleteDC(h_memory_dc);

	BITMAPFILEHEADER dib_format_layout;
	ZeroMemory(&dib_format_layout, sizeof(BITMAPFILEHEADER));
	dib_format_layout.bfType = *(WORD*)"BM";
	dib_format_layout.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dib_define.bmiHeader.biSizeImage;
	dib_format_layout.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

	CString directory;
	char* savePath;

	USES_CONVERSION;
	savePath = W2A((theApp.fileSet.folrderPath).GetString());
	FILE* p_file;
	fopen_s(&p_file, W2A((theApp.fileSet.fileName).GetString()), "wb");
	if (p_file != NULL) {
		fwrite(&dib_format_layout, 1, sizeof(BITMAPFILEHEADER), p_file);
		fwrite(&dib_define, 1, sizeof(BITMAPINFOHEADER), p_file);
		fwrite(p_image_data, 1, dib_define.bmiHeader.biSizeImage, p_file);
		fclose(p_file);
	}

	if (NULL != h_bitmap) DeleteObject(h_bitmap);
	if (NULL != hdc) ::ReleaseDC(NULL, hdc);
}

void cDlgPaintDraw::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	TRACE(L"WM_CLOSE\n");
	CDialogEx::OnClose();
}


void cDlgPaintDraw::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: Add your message handler code here
					   // Do not call CDialogEx::OnPaint() for painting messages

	CDC* cdc = &dc;
	for (auto hugeData_iter = theApp.list_hugeData.begin(); hugeData_iter != theApp.list_hugeData.end(); hugeData_iter++) {
		HugeData& data = *hugeData_iter;

		CPen temp_pen(PS_SOLID, data.set_thickness.thickness,
			RGB(data.set_color.red, data.set_color.green, data.set_color.blue));

		CPen* old_pen = cdc->SelectObject(&temp_pen);
		for (auto point_iter = data.point.begin(); point_iter != data.point.end(); point_iter++) {
			cdc->MoveTo(point_iter->before_point.x, point_iter->before_point.y);
			cdc->LineTo(point_iter->last_point.x, point_iter->last_point.y);
		}
		cdc->SelectObject(old_pen);
	}

	ReleaseDC(cdc);
	if (theApp.fileSet.fileFilter.MakeLower().Compare(L"bmp"))
		savePaintBMP();
	else if (theApp.fileSet.fileFilter.MakeLower().Compare(L"jpg")) {
		// savePaintJPG();
	}


	SendMessage(WM_CLOSE, NULL, NULL);
}
