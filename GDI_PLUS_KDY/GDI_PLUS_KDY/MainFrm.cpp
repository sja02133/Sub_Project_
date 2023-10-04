
// MainFrm.cpp : implementation of the CMainFrame class
//

#include "pch.h"
#include "framework.h"
#include "GDI_PLUS_KDY.h"

#include "MainFrm.h"

#include "ChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	//custom

	//custom end
	ON_WM_CREATE()
	ON_COMMAND(ID_BUTTON_ARROW_LINE, &CMainFrame::OnButtonArrowLine)
	ON_COMMAND(ID_BUTTON_CIRCLE, &CMainFrame::OnButtonCircle)
	ON_COMMAND(ID_BUTTON_RECTANGLE, &CMainFrame::OnButtonRectangle)
	ON_WM_DRAWITEM()
	ON_UPDATE_COMMAND_UI(ID_BUTTON_ARROW_LINE, &CMainFrame::OnUpdateButtonArrowLine)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_CIRCLE, &CMainFrame::OnUpdateButtonCircle)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_RECTANGLE, &CMainFrame::OnUpdateButtonRectangle)
	ON_COMMAND(ID_BUTTON_MOUSE_CURSOR, &CMainFrame::OnButtonMouseCursor)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_MOUSE_CURSOR, &CMainFrame::OnUpdateButtonMouseCursor)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame construction/destruction

CMainFrame::CMainFrame() noexcept
{
	// TODO: add member initialization code here
		//2023.06.06 GDI+ ���̺귯�� �ʱ�ȭ
	//ULONG_PTR gpToken;
	//GdiplusStartupInput gpsi;
	if (GdiplusStartup(&this->gpToken, &this->gpsi, NULL) != Ok)
		return ;
}

CMainFrame::~CMainFrame()
{
	GdiplusShutdown(this->gpToken);
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	//this->testFreeImageLibrary();

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_TOOLBAR_CUSTOM))
	//if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

//	testFreeImageLibrary();
/*
	// TODO: Delete these three lines if you don't want the toolbar to be dockable

	//m_bitMap.LoadBitmapW(IDB_BITMAP_EXAMPLE);
	//m_wndToolBar.SetBitmap((HBITMAP)m_bitMap);

	CToolBarCtrl& toolbarCtrl = m_wndToolBar.GetToolBarCtrl();
	int nButtonCount = toolbarCtrl.GetButtonCount();
	
	COLORREF backgroundColor = RGB(0, 0, 0); // ���ϴ� �������� ����
	for (int i = 0; i < nButtonCount; i++)
	{
		TBBUTTON pButton;
		toolbarCtrl.GetButton(i, &pButton);
		
		TBBUTTONINFO buttonInfo;

		HWND h = ::GetDlgItem(m_hWnd, pButton.idCommand);
		CDC* buttonDC = CDC::FromHandle(::GetDC(h));
		buttonDC->SetBkColor(backgroundColor);
	}
	*/

	// �߰��� ��ư
	//this->AddToolbarImages(toolbarCtrl);

	// ����
	//m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	//EnableDocking(CBRS_ALIGN_ANY);
	//DockControlBar(&m_wndToolBar);


	//CMyToolbar tool;
	//check = this->customToolbar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
	//check = this->customToolbar.LoadToolBar(IDR_TOOLBAR_CUSTOM);

	//Custom Toolbar
	//this->customToolbar.EnableDocking(CBRS_ALIGN_ANY);
	//EnableDocking(CBRS_ALIGN_ANY);
	//DockControlBar(&this->customToolbar);

	//this->customToolbar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);


	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}
#endif //_DEBUG

void CMainFrame::LoadPngToToolBar(CToolBarCtrl& toolbar, LPCTSTR pngFilePath, int imageID) {
	//// PNG ���� �ε�
	//Gdiplus::Bitmap bitmap(pngFilePath);
	//if (bitmap.GetLastStatus() != Gdiplus::Ok)
	//{
	//	// �ε� ���� ó��
	//	return;
	//}

	//// �̹��� ũ�� ����
	//Gdiplus::Bitmap resizedBitmap(_T("C:\\Users\\KDY\\source\\repos\\GDI_PLUS_KDY\\GDI_PLUS_KDY\\image\\square_32.png"));
	//

	//// CBitmap���� ��ȯ
	//CBitmap cbitmap;
	//cbitmap.Attach(resizedBitmap.Detach());
	//
	//// ���ٿ� �̹��� ����Ʈ ����
	//CImageList imageList;
	//imageList.Create(16, 16, ILC_COLOR32 | ILC_MASK, 1, 1);
	//imageList.Add(&cbitmap, RGB(0, 0, 0));
	//toolbar.SetImageList(&imageList);

	//// ��ư �߰�
	//TBBUTTON button;
	//ZeroMemory(&button, sizeof(TBBUTTON));
	//button.iBitmap = 0;          // �̹��� �ε��� ����
	//button.idCommand = imageID;  // ��ư�� ��� ID ����
	//button.fsState = TBSTATE_ENABLED;
	//button.fsStyle = TBSTYLE_BUTTON;
	//toolbar.AddButtons(1, &button);

	//// GDI+ ����
	//Gdiplus::GdiplusShutdown(theApp.gpToken);
}

//CImageList imageList;
//imageList.Create(16, 16, ILC_COLOR32 | ILC_MASK, 1, 1);

//// PNG ���� �ε�
//CImage image;
//image.Load(_T("C:\\Users\\KDY\\source\\repos\\GDI_PLUS_KDY\\GDI_PLUS_KDY\\image\\square_32.png"));

//// �̹��� ũ�� ����
//CImage resizedImage;
//resizedImage.Create(16, 16, image.GetBPP());
//image.StretchBlt(resizedImage.GetDC(), 0, 0, 16, 16, SRCCOPY);
//resizedImage.ReleaseDC();

//// CImage�� HBITMAP���� ��ȯ
//HBITMAP hBitmap = (HBITMAP)resizedImage.Detach();

//// HBITMAP�� CBitmap���� ��ȯ
//CBitmap bitmap;
//bitmap.Attach(hBitmap);

//// ���ٿ� �̹��� ����Ʈ ����
//toolbar.SetImageList(&imageList);

//// CBitmap�� �̹��� ����Ʈ�� �߰�
//int imageIndex = imageList.Add(&bitmap, RGB(0, 0, 0));

//// ��ư �߰�
//TBBUTTON button;
//ZeroMemory(&button, sizeof(TBBUTTON));
//button.iBitmap = imageIndex;  // �̹��� �ε��� ����
//button.idCommand = IDB_PNG2;  // ��ư�� ��� ID ����
//button.fsState = TBSTATE_ENABLED;
//button.fsStyle = TBSTYLE_BUTTON;
//toolbar.AddButtons(1, &button);

#define TEST_BMP _T("C:\\Users\\KDY\\source\\repos\\GDI_PLUS_KDY\\GDI_PLUS_KDY\\image\\testFreeBitmap.bmp")
#define TEST_BMP_OUTPUT _T("C:\\Users\\KDY\\source\\repos\\GDI_PLUS_KDY\\GDI_PLUS_KDY\\image\\testFreeBitmap4bit.bmp")

void CMainFrame::testFreeImageLibrary()
{
	// PNG ���� ���
	const wchar_t* pngFilePath = TEST_BMP;
	const wchar_t* bmpFilePath = TEST_BMP_OUTPUT;
	// FreeImage ���̺귯�� �ʱ�ȭ
	FreeImage_Initialise();

	// PNG ���� �ε�
	FIBITMAP* image = FreeImage_LoadU(FIF_BMP, pngFilePath);
	FIBITMAP* convertedImage = FreeImage_ConvertTo4Bits(image);
	FreeImage_SaveU(FIF_BMP, convertedImage, bmpFilePath, BMP_SAVE_RLE | BMP_DEFAULT);

	// ������ 32��Ʈ ��Ʈ�� �޸� ����
	FreeImage_Unload(convertedImage);

	// ���� �̹��� �޸� ����
	FreeImage_Unload(image);

	// FreeImage ���̺귯�� ����
	FreeImage_DeInitialise();

	return;
	if (image) {
		// 32��Ʈ�� �̹��� ��ȯ
		FIBITMAP* convertedImage = FreeImage_ConvertTo4Bits(image);

		// ���� �̹����� �ʺ�� ���� ��������
		int width = FreeImage_GetWidth(convertedImage);
		int height = FreeImage_GetHeight(convertedImage);

		// ���ο� 4��Ʈ ��Ʈ�� ����
		FIBITMAP* bitmap = FreeImage_AllocateT(FIT_BITMAP, width, height, 4 * sizeof(BYTE), FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK);

		if (bitmap) {
			// 32��Ʈ ��Ʈ�ʿ��� �ȼ� ���� �����ͼ� 4��Ʈ�� ��ȯ�Ͽ� 4��Ʈ ��Ʈ�ʿ� ����
			for (int y = 0; y < height; y++) {
				BYTE* srcPixels = (BYTE*)FreeImage_GetScanLine(convertedImage, y);
				BYTE* destPixels = (BYTE*)FreeImage_GetScanLine(bitmap, y);

				for (int x = 0; x < width; x++) {
					BYTE r = srcPixels[FI_RGBA_RED];
					BYTE g = srcPixels[FI_RGBA_GREEN];
					BYTE b = srcPixels[FI_RGBA_BLUE];

					// 32��Ʈ �ȼ� ���� 4��Ʈ�� ��ȯ�Ͽ� ����
					BYTE pixelValue = (r >> 4) << 4 | (g >> 4) << 2 | (b >> 4);
					destPixels[x] = pixelValue;

					srcPixels += sizeof(RGBQUAD); // ����: �ùٸ� �ȼ� ��ġ�� �̵�
				}
			}


			// BMP ���Ϸ� ���� (���� ���� ����)
			if (FreeImage_SaveU(FIF_BMP, bitmap, bmpFilePath, BMP_SAVE_RLE | BMP_DEFAULT)) {
				//std::cout << "�̹����� BMP ���Ϸ� �����߽��ϴ�." << std::endl;
				int k1 = 0;
			}
			else {
				//std::cout << "BMP ���� ���� ����." << std::endl;
				int k2 = 0;
			}

			// ������ 4��Ʈ ��Ʈ�� �޸� ����
			FreeImage_Unload(bitmap);
		}
		else {
			//std::cout << "��Ʈ�� �̹��� ���� ����." << std::endl;
		}

		// ������ 32��Ʈ ��Ʈ�� �޸� ����
		FreeImage_Unload(convertedImage);

		// ���� �̹��� �޸� ����
		FreeImage_Unload(image);
	}
	else {
		//std::cout << "PNG ���� �ε� ����." << std::endl;
	}

	// FreeImage ���̺귯�� ����
	FreeImage_DeInitialise();
}

void CMainFrame::AddToolbarImages(CToolBarCtrl& toolbar)
{
	
}






// CMainFrame message handlers



void CMainFrame::OnButtonArrowLine()
{
	// TODO: Add your command handler code here
	
	theApp.select_figure = 1;
	//check_otherButton_SetCheck();
	//set_Button_SetCheck(ID_BUTTON_ARROW_LINE);
}
void CMainFrame::OnUpdateButtonArrowLine(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(theApp.select_figure == 1);
}

void CMainFrame::OnButtonCircle()
{
	// TODO: Add your command handler code here
	theApp.select_figure = 3;
	//check_otherButton_SetCheck();
	//set_Button_SetCheck(ID_BUTTON_CIRCLE);
}

void CMainFrame::OnUpdateButtonCircle(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(theApp.select_figure == 3);
}

void CMainFrame::OnButtonRectangle()
{
	// TODO: Add your command handler code here
	theApp.select_figure = 2;
	//check_otherButton_SetCheck();
	//set_Button_SetCheck(ID_BUTTON_RECTANGLE);
}

void CMainFrame::OnUpdateButtonRectangle(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(theApp.select_figure == 2);
}

void CMainFrame::OnButtonMouseCursor()
{
	// TODO: Add your command handler code here
	theApp.select_figure = 1000;
}

void CMainFrame::OnUpdateButtonMouseCursor(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(theApp.select_figure == 1000);
}

void CMainFrame::set_Button_SetCheck(int id)
{
	CToolBarCtrl& TBCtrl = this->m_wndToolBar.GetToolBarCtrl();

	int th = TBCtrl.CommandToIndex(id);

	BOOL check = TBCtrl.IsButtonChecked(id);
	if (check != 0)
		check = TBCtrl.CheckButton(th, FALSE);
	else
		check = TBCtrl.CheckButton(th, TRUE);

	//BOOL check = TBCtrl.IsButtonPressed(th);
	//if (check != 0)
	//	check = TBCtrl.PressButton(th, FALSE);
	//else
	//	check = TBCtrl.PressButton(th, TRUE);

}

BOOL CMainFrame::check_otherButton_SetCheck()
{
	CToolBarCtrl& TBCtrl = this->m_wndToolBar.GetToolBarCtrl();
	int buttonCount = TBCtrl.GetButtonCount();
	int th = 0;
	//for (int i = 0; i < buttonCount; i++) {
	//	th = TBCtrl.CommandToIndex(ButtonList[i]);
	//	if(TBCtrl.IsButtonChecked(th) != 0)
	//		TBCtrl.CheckButton(th, FALSE);
	//}

	for (int i = 0; i < buttonCount; i++) {
		th = TBCtrl.CommandToIndex(ButtonList[i]);
		if (TBCtrl.IsButtonPressed(th) != 0)
			TBCtrl.PressButton(th, FALSE);
	}

	return TRUE;
}



