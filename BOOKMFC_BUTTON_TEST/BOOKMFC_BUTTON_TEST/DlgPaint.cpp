// DlgPaint.cpp : implementation file
//

#include "pch.h"

// cDlgPaint dialog

IMPLEMENT_DYNAMIC(cDlgPaint, CDialogEx)

cDlgPaint::cDlgPaint(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_PAINT, pParent)
{
	cDlgPaint::beDrawing = FALSE;
}

cDlgPaint::cDlgPaint(CWnd* pParent, UINT dlgIDD)
	: CDialogEx(dlgIDD, pParent)
{
	cDlgPaint::beDrawing = FALSE;
}


cDlgPaint::~cDlgPaint()
{
}

void cDlgPaint::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(cDlgPaint, CDialogEx)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
//	ON_WM_DRAWITEM()
ON_WM_LBUTTONUP()
ON_BN_CLICKED(IDC_BUTTON1, &cDlgPaint::OnBnClickedButton1)
ON_BN_CLICKED(IDC_BUTTON_CHANGE_TEXT_SIZE, &cDlgPaint::OnBnClickedButtonChangeTextSize)
END_MESSAGE_MAP()


// cDlgPaint message handlers

BOOL cDlgPaint::makeVirtualDC(CString file) {
	if (theApp.list_hugeData.size() == 0)
		return FALSE;

	cDlgPaintDraw tempDrawDlg;
	int name_pos = file.GetLength() - (file.ReverseFind(L'\\')+1);
	int folder_pos = file.GetLength() - name_pos;
	theApp.fileSet.fileName = file.Right(name_pos);
	theApp.fileSet.folrderPath = file.Left(folder_pos);
	int filter_pos = tempDrawDlg.fileName.Find(L'.');
	theApp.fileSet.fileFilter = theApp.fileSet.fileName.Right(theApp.fileSet.fileName.GetLength() - filter_pos - 1);



	tempDrawDlg.DoModal();

	return TRUE;
}
/*
void cDlgPaint::savePaint() {
	HWND hWnd = ::GetActiveWindow();
	
	//make Virtual DC
	makeVirtualDC();

	//기존
	//HDC hdc = ::GetDC(hWnd);

	//변경
	HDC hdc = ::GetDC(this->m_hWnd);

	int width = ::GetDeviceCaps(hdc, HORZRES);
	int height = ::GetDeviceCaps(hdc, VERTRES);
	// (SetWindowPos)'s uFlags 참고자료 : https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-setwindowpos
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

	directory = L"..\\save\\a.bmp";
	USES_CONVERSION;
	savePath = W2A(directory.GetString());

	FILE* p_file;
	fopen_s(&p_file,"a.bmp", "wb");
	if (p_file != NULL) {
		fwrite(&dib_format_layout, 1, sizeof(BITMAPFILEHEADER), p_file);
		fwrite(&dib_define, 1, sizeof(BITMAPINFOHEADER), p_file);
		fwrite(p_image_data, 1, dib_define.bmiHeader.biSizeImage, p_file);
		fclose(p_file);
	}

	if (NULL != h_bitmap) DeleteObject(h_bitmap);
	if (NULL != hdc) ::ReleaseDC(NULL, hdc);
}
*/

void cDlgPaint::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: Add your message handler code here
					   // Do not call CDialogEx::OnPaint() for painting messages
	CRect rect;
	GetClientRect(&rect);

	dc.SetBkColor(RGB(255, 255, 255));

	if (theApp.list_hugeData.size() > 0) {
		drawVectorData(&dc);
	}
	else {

	}
	//CPen pen(PS_SOLID, 1, RGB(0, 0, 0));
}

void cDlgPaint::drawVectorData(CDC* dc) {
	//다시 그린다. 어디까지? -> theApp.vector_hugeData.checkingDraw가 FALSE 전까지
	
	int i = 0, j = 0;
	HWND hWnd = ::GetActiveWindow();
	CString temp_thickness = L"",temp_rgb = L"";
	vector<pointPaint>::iterator point_iter;
	queue<int> m_int;

	for (auto hugeData_iter = theApp.list_hugeData.begin(); hugeData_iter != theApp.list_hugeData.end(); hugeData_iter++) {
		HugeData& data = *hugeData_iter;
		
		CPen temp_pen(PS_SOLID, data.set_thickness.thickness,
			RGB(data.set_color.red, data.set_color.green, data.set_color.blue));
			
		CPen* old_pen = dc->SelectObject(&temp_pen);
		for (point_iter =  data.point.begin(); point_iter !=  data.point.end(); point_iter++) {
			dc->MoveTo(point_iter->before_point.x, point_iter->before_point.y);
			dc->LineTo(point_iter->last_point.x, point_iter->last_point.y);
		}
		dc->SelectObject(old_pen);
	}
}



void cDlgPaint::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	HWND hWnd = ::GetActiveWindow();
	HDC hdc = ::GetDC(hWnd);
	CDC* pDC = GetDC();
	CDialogEx::OnMouseMove(nFlags, point);
	CClientDC dc(this);
	CString str;
	str.Format(_T("마우스 Move (좌표 : %4d, %4d)"), point.x, point.y);

	SetDlgItemText(IDC_STATIC_PAINT_STATUS, str);
	
	CPen tempPen;

	pen.DeleteObject();
	pen.CreatePen(PS_SOLID, theData.set_thickness.thickness, RGB(theData.set_color.red, theData.set_color.green, theData.set_color.blue));
	dc.SelectObject(&pen);
	pointPaint set_point;

	if (beDrawing == TRUE) {
		//마우스 좌클릭 도중
		set_point.before_point.x = c_point.x;
		set_point.before_point.y = c_point.y;
		//MoveToEx(hdc, c_point.x, c_point.y, NULL);
		dc.MoveTo(c_point.x, c_point.y);
		c_point.x = point.x;
		c_point.y = point.y;
		set_point.last_point.x = c_point.x;
		set_point.last_point.y = c_point.y;
		dc.LineTo(c_point.x, c_point.y);
		theData.point.push_back(set_point);
		ReleaseDC(pDC);
	}
	
}

void cDlgPaint::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if (beDrawing == TRUE) {
		theApp.list_hugeData.push_back(theData);
		theData.point.clear();
		if (theApp.stack_hugeData.size() > 0) {
			while (theApp.stack_hugeData.empty() == FALSE)
				theApp.stack_hugeData.pop();
		}
	}
	beDrawing = FALSE;
	ReleaseCapture();
}

void cDlgPaint::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CDialogEx::OnLButtonDown(nFlags, point);

	CString str = L"마우스 좌클릭";

	beDrawing = TRUE;
	c_point.x = point.x;
	c_point.y = point.y;
	SetCapture();

	SetDlgItemText(IDC_STATIC_PAINT_STATUS,str);
}

void cDlgPaint::OnBnClickedButton1()
{
	cButtonColor dlg_color;
	if (dlg_color.DoModal() != IDOK)
		return;
}

void cDlgPaint::OnBnClickedButtonChangeTextSize()
{
	cTextSize text_size;
	if (text_size.DoModal() != IDOK)
		return;
}

BOOL cDlgPaint::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class

	//키 상수값(Virtual-Key 코드) -> https://docs.microsoft.com/ko-kr/windows/win32/inputdev/virtual-key-codes
	HWND hWnd = pMsg->hwnd;
	HDC hdc = ::GetDC(hWnd);
	CDC* pDC = GetDC();
	CClientDC dc(this);

	unsigned int size = 0;

	list<HugeData>::iterator iter;

	if (pMsg->message == WM_KEYDOWN) {
		if (::GetKeyState(VK_CONTROL) < 0 && pMsg->wParam == _T('Z') /* 'Z' key */) {
			//ctrl + z
			SetDlgItemText(IDC_STATIC_PAINT_STATUS, L"ctrl + z 키 눌림");
			//list 구조
			//list가 비어있지 않으면 stack에 push하고 list 마지막 pop
			if (theApp.list_hugeData.empty() != TRUE) {
				theApp.stack_hugeData.push(theApp.list_hugeData.back());
				theApp.list_hugeData.pop_back();
				Invalidate(TRUE);
				return TRUE;
			}
			//vector 구조
			/*
			size = theApp.vector_hugeData.size();
			if (size > 0) {
				iter = theApp.vector_hugeData.begin();
				while (iter != theApp.vector_hugeData.end() - 1) {
					iter++;
				}
				while (iter->checking_Draw != TRUE) {
					if (iter != theApp.vector_hugeData.begin()) {
						iter--;
					}
					else if (iter == theApp.vector_hugeData.begin())
						break;
				}
				iter->checking_Draw = FALSE;
				Invalidate(TRUE);
				//OnPaint();
				TRACE(L"ctrl + z 키 눌림\n");
				return TRUE;
			}
			*/
		}
		else if (::GetKeyState(VK_CONTROL) < 0 && pMsg->wParam == _T('Y') /* 'Y' key */) {
			SetDlgItemText(IDC_STATIC_PAINT_STATUS, L"ctrl + y 키 눌림");
			//list 구조
			//list가 비어있지 않으면 stack에 push하고 list 마지막 pop
			if (theApp.stack_hugeData.empty() != TRUE) {
				theApp.list_hugeData.push_back(theApp.stack_hugeData.top());
				theApp.stack_hugeData.pop();
				Invalidate(TRUE);
				return TRUE;
			}
		}
		else if (::GetKeyState(VK_CONTROL) < 0 && pMsg->wParam == _T('S') /* 'S' key */) {
			SetDlgItemText(IDC_STATIC_PAINT_STATUS, L"ctrl + s 키 눌림(저장)");
			
			//savePaint();
			CString path = L"";
			CString fileFilter = _T("All Files(*.*)| *.*; ||");
			CFileDialog file_Dlg(FALSE, L"bmp", L"", 0, fileFilter, this, 0, 1);
			if (file_Dlg.DoModal() == IDOK) {
				path = file_Dlg.GetPathName();
			}
			
			TRACE(path);


			makeVirtualDC(path);
			return TRUE;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


void cDlgPaint::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class

	CDialogEx::OnCancel();

	theApp.list_hugeData.clear();
}


BOOL cDlgPaint::Create(LPCTSTR lpszTemplateName, CWnd* pParentWnd)
{
	return CDialogEx::Create(lpszTemplateName, pParentWnd);

}


BOOL cDlgPaint::OnInitDialog()
{
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}
