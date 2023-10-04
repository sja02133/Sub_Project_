
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
		//2023.06.06 GDI+ 라이브러리 초기화
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
	
	COLORREF backgroundColor = RGB(0, 0, 0); // 원하는 배경색으로 변경
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

	// 추가할 버튼
	//this->AddToolbarImages(toolbarCtrl);

	// 기존
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
	//// PNG 파일 로드
	//Gdiplus::Bitmap bitmap(pngFilePath);
	//if (bitmap.GetLastStatus() != Gdiplus::Ok)
	//{
	//	// 로드 실패 처리
	//	return;
	//}

	//// 이미지 크기 조정
	//Gdiplus::Bitmap resizedBitmap(_T("C:\\Users\\KDY\\source\\repos\\GDI_PLUS_KDY\\GDI_PLUS_KDY\\image\\square_32.png"));
	//

	//// CBitmap으로 변환
	//CBitmap cbitmap;
	//cbitmap.Attach(resizedBitmap.Detach());
	//
	//// 툴바에 이미지 리스트 설정
	//CImageList imageList;
	//imageList.Create(16, 16, ILC_COLOR32 | ILC_MASK, 1, 1);
	//imageList.Add(&cbitmap, RGB(0, 0, 0));
	//toolbar.SetImageList(&imageList);

	//// 버튼 추가
	//TBBUTTON button;
	//ZeroMemory(&button, sizeof(TBBUTTON));
	//button.iBitmap = 0;          // 이미지 인덱스 설정
	//button.idCommand = imageID;  // 버튼의 명령 ID 설정
	//button.fsState = TBSTATE_ENABLED;
	//button.fsStyle = TBSTYLE_BUTTON;
	//toolbar.AddButtons(1, &button);

	//// GDI+ 종료
	//Gdiplus::GdiplusShutdown(theApp.gpToken);
}

//CImageList imageList;
//imageList.Create(16, 16, ILC_COLOR32 | ILC_MASK, 1, 1);

//// PNG 파일 로드
//CImage image;
//image.Load(_T("C:\\Users\\KDY\\source\\repos\\GDI_PLUS_KDY\\GDI_PLUS_KDY\\image\\square_32.png"));

//// 이미지 크기 조정
//CImage resizedImage;
//resizedImage.Create(16, 16, image.GetBPP());
//image.StretchBlt(resizedImage.GetDC(), 0, 0, 16, 16, SRCCOPY);
//resizedImage.ReleaseDC();

//// CImage를 HBITMAP으로 변환
//HBITMAP hBitmap = (HBITMAP)resizedImage.Detach();

//// HBITMAP을 CBitmap으로 변환
//CBitmap bitmap;
//bitmap.Attach(hBitmap);

//// 툴바에 이미지 리스트 설정
//toolbar.SetImageList(&imageList);

//// CBitmap을 이미지 리스트에 추가
//int imageIndex = imageList.Add(&bitmap, RGB(0, 0, 0));

//// 버튼 추가
//TBBUTTON button;
//ZeroMemory(&button, sizeof(TBBUTTON));
//button.iBitmap = imageIndex;  // 이미지 인덱스 설정
//button.idCommand = IDB_PNG2;  // 버튼의 명령 ID 설정
//button.fsState = TBSTATE_ENABLED;
//button.fsStyle = TBSTYLE_BUTTON;
//toolbar.AddButtons(1, &button);

#define TEST_BMP _T("C:\\Users\\KDY\\source\\repos\\GDI_PLUS_KDY\\GDI_PLUS_KDY\\image\\testFreeBitmap.bmp")
#define TEST_BMP_OUTPUT _T("C:\\Users\\KDY\\source\\repos\\GDI_PLUS_KDY\\GDI_PLUS_KDY\\image\\testFreeBitmap4bit.bmp")

void CMainFrame::testFreeImageLibrary()
{
	// PNG 파일 경로
	const wchar_t* pngFilePath = TEST_BMP;
	const wchar_t* bmpFilePath = TEST_BMP_OUTPUT;
	// FreeImage 라이브러리 초기화
	FreeImage_Initialise();

	// PNG 파일 로드
	FIBITMAP* image = FreeImage_LoadU(FIF_BMP, pngFilePath);
	FIBITMAP* convertedImage = FreeImage_ConvertTo4Bits(image);
	FreeImage_SaveU(FIF_BMP, convertedImage, bmpFilePath, BMP_SAVE_RLE | BMP_DEFAULT);

	// 생성한 32비트 비트맵 메모리 해제
	FreeImage_Unload(convertedImage);

	// 원본 이미지 메모리 해제
	FreeImage_Unload(image);

	// FreeImage 라이브러리 해제
	FreeImage_DeInitialise();

	return;
	if (image) {
		// 32비트로 이미지 변환
		FIBITMAP* convertedImage = FreeImage_ConvertTo4Bits(image);

		// 원본 이미지의 너비와 높이 가져오기
		int width = FreeImage_GetWidth(convertedImage);
		int height = FreeImage_GetHeight(convertedImage);

		// 새로운 4비트 비트맵 생성
		FIBITMAP* bitmap = FreeImage_AllocateT(FIT_BITMAP, width, height, 4 * sizeof(BYTE), FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK);

		if (bitmap) {
			// 32비트 비트맵에서 픽셀 값을 가져와서 4비트로 변환하여 4비트 비트맵에 설정
			for (int y = 0; y < height; y++) {
				BYTE* srcPixels = (BYTE*)FreeImage_GetScanLine(convertedImage, y);
				BYTE* destPixels = (BYTE*)FreeImage_GetScanLine(bitmap, y);

				for (int x = 0; x < width; x++) {
					BYTE r = srcPixels[FI_RGBA_RED];
					BYTE g = srcPixels[FI_RGBA_GREEN];
					BYTE b = srcPixels[FI_RGBA_BLUE];

					// 32비트 픽셀 값을 4비트로 변환하여 설정
					BYTE pixelValue = (r >> 4) << 4 | (g >> 4) << 2 | (b >> 4);
					destPixels[x] = pixelValue;

					srcPixels += sizeof(RGBQUAD); // 수정: 올바른 픽셀 위치로 이동
				}
			}


			// BMP 파일로 저장 (투명도 정보 없음)
			if (FreeImage_SaveU(FIF_BMP, bitmap, bmpFilePath, BMP_SAVE_RLE | BMP_DEFAULT)) {
				//std::cout << "이미지를 BMP 파일로 저장했습니다." << std::endl;
				int k1 = 0;
			}
			else {
				//std::cout << "BMP 파일 저장 실패." << std::endl;
				int k2 = 0;
			}

			// 생성한 4비트 비트맵 메모리 해제
			FreeImage_Unload(bitmap);
		}
		else {
			//std::cout << "비트맵 이미지 생성 실패." << std::endl;
		}

		// 생성한 32비트 비트맵 메모리 해제
		FreeImage_Unload(convertedImage);

		// 원본 이미지 메모리 해제
		FreeImage_Unload(image);
	}
	else {
		//std::cout << "PNG 파일 로드 실패." << std::endl;
	}

	// FreeImage 라이브러리 해제
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



