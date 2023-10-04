
// notepad_KDY_SDIView.cpp : implementation of the CnotepadKDYSDIView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "notepad_KDY_SDI.h"
#endif

#include "notepad_KDY_SDIDoc.h"
#include "notepad_KDY_SDIView.h"

#include "DlgSave.h"
#include "FileDialogCustom.h"

#include <locale.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//CnotepadKDYSDIDoc doc_data;

#pragma pack(push, 1)
union utf16be {
	WCHAR p;
	BYTE bytes[2];
};

/*
typedef struct _BBB {
	char arr[52];
	int i32;
} BBB;


typedef struct _AAA {
	int count;
	char b;
	BBB c[];
} AAA;
*/
#pragma pack(pop)

// CnotepadKDYSDIView

IMPLEMENT_DYNCREATE(CnotepadKDYSDIView, CEditView)

BEGIN_MESSAGE_MAP(CnotepadKDYSDIView, CEditView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CEditView::OnFilePrintPreview)
	ON_UPDATE_COMMAND_UI(ID_FILE_NEWWINDOW, &CnotepadKDYSDIView::OnUpdateFileNewwindow)
	ON_COMMAND(ID_FILE_NEWWINDOW, &CnotepadKDYSDIView::OnFileNewwindow)
	ON_UPDATE_COMMAND_UI(ID_Menu_SAVE_AS, &CnotepadKDYSDIView::OnUpdateMenuSaveAs)
	ON_COMMAND(ID_Menu_SAVE_AS, &CnotepadKDYSDIView::OnMenuSaveAs)
	ON_UPDATE_COMMAND_UI(ID_FILE_PAGE_PROPERTIES, &CnotepadKDYSDIView::OnUpdateFilePageProperties)
	ON_COMMAND(ID_FILE_PAGE_PROPERTIES, &CnotepadKDYSDIView::OnFilePageProperties)
	ON_UPDATE_COMMAND_UI(ID_EDIT_FONT, &CnotepadKDYSDIView::OnUpdateEditFont)
	ON_COMMAND(ID_EDIT_FONT, &CnotepadKDYSDIView::OnEditFont)
	ON_COMMAND(ID_FILE_NEW, &CnotepadKDYSDIView::OnFileNew)
	ON_WM_CTLCOLOR()
	ON_CONTROL_REFLECT(EN_CHANGE, &CnotepadKDYSDIView::OnEnChange)
	ON_COMMAND(ID_FILE_SAVE, &CnotepadKDYSDIView::OnFileSave)
	ON_COMMAND(ID_EDIT_DELETE, &CnotepadKDYSDIView::OnEditDelete)
	ON_UPDATE_COMMAND_UI(ID_EDIT_DELETE, &CnotepadKDYSDIView::OnUpdateEditDelete)
	ON_COMMAND(ID_FILE_OPEN, &CnotepadKDYSDIView::OnFileOpen)
	ON_COMMAND(ID_APP_EXIT, &CnotepadKDYSDIView::OnAppExit)
	ON_COMMAND(ID_EDIT_PLUS, &CnotepadKDYSDIView::OnEditPlus)
	ON_COMMAND(ID_EDIT_MINUS, &CnotepadKDYSDIView::OnEditMinus)
	ON_COMMAND(ID_EDIT_RESET_PLUS_MINUS, &CnotepadKDYSDIView::OnEditResetPlusMinus)
	ON_WM_CLOSE()
	ON_WM_SYSCOMMAND()
	ON_COMMAND(IDCLOSE, &CnotepadKDYSDIView::OnIdclose)
END_MESSAGE_MAP()

// CnotepadKDYSDIView construction/destruction

//CnotepadKDYSDIDoc* pDoc;

CnotepadKDYSDIView::CnotepadKDYSDIView() noexcept
{
	// TODO: add construction code here
	/*
	m_fontStyle.CreateFont(
		16,								// nHeight
		0,								// nWidth
		0,								// nEscapement
		0,								// nOrientation
		FW_NORMAL,						// nWeight
		FALSE,							// bItalic
		FALSE,							// bUnderline
		FALSE,							// cStrikeOut
		DEFAULT_CHARSET,				// nCharSet
		OUT_CHARACTER_PRECIS,			// nOutPrecision
		CLIP_CHARACTER_PRECIS,			// nClipPrecision
		DEFAULT_QUALITY,				// nQuality
		DEFAULT_PITCH | FF_DONTCARE,	// nPitchAndFamily
		_T("Tahoma"));					// lpszFacename
*/
	m_clrText = RGB(255, 0, 0);

	m_bRefreshCtrl = TRUE;

	::setlocale(LC_ALL,"Korean");
}

CnotepadKDYSDIView::~CnotepadKDYSDIView()
{
}

BOOL CnotepadKDYSDIView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	BOOL bPreCreated = CEditView::PreCreateWindow(cs);
	cs.style &= ~(ES_AUTOHSCROLL|WS_HSCROLL);	// Enable word-wrapping

	return bPreCreated;
}


// CnotepadKDYSDIView printing

BOOL CnotepadKDYSDIView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default CEditView preparation
	return CEditView::OnPreparePrinting(pInfo);
}

void CnotepadKDYSDIView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// Default CEditView begin printing
	CEditView::OnBeginPrinting(pDC, pInfo);
}

void CnotepadKDYSDIView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// Default CEditView end printing
	CEditView::OnEndPrinting(pDC, pInfo);
}



// CnotepadKDYSDIView diagnostics

#ifdef _DEBUG
void CnotepadKDYSDIView::AssertValid() const
{
	CEditView::AssertValid();
}

void CnotepadKDYSDIView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}

CnotepadKDYSDIDoc* CnotepadKDYSDIView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CnotepadKDYSDIDoc)));
	return (CnotepadKDYSDIDoc*)m_pDocument;
}
#endif //_DEBUG


// CnotepadKDYSDIView message handlers


BOOL CnotepadKDYSDIView::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class

	if (pMsg->message == WM_KEYDOWN) {
		if (::GetKeyState(VK_CONTROL) < 0 && pMsg->wParam == VK_F5) {
			MessageBox(L"INSTANCE");
		}
	}


	return CEditView::PreTranslateMessage(pMsg);
}


void CnotepadKDYSDIView::OnUpdateFileNewwindow(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
}


void CnotepadKDYSDIView::OnFileNewwindow()
{
	// TODO: Add your command handler code here
	CnotepadKDYSDIDoc* pDoc = GetDocument();

	ShellExecute(NULL, L"open", pDoc->notepad_path, NULL, NULL, SW_SHOW);
}


void CnotepadKDYSDIView::OnUpdateMenuSaveAs(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable();
}


BOOL CnotepadKDYSDIView::saveLogic(BOOL openFile) {
	CnotepadKDYSDIDoc* pDoc = GetDocument();
//	CEdit& m_edit = GetEditCtrl();
	
	HRESULT ret;
	eBOM bom;
	TCHAR* file = (TCHAR*)malloc(MAX_PATH * sizeof(TCHAR));
	ret = BasicFileOpen(file, openFile, bom);

	if (SUCCEEDED(ret)) {
		settingFileInformation(file, bom);
		saveFile_second(pDoc->fileInfo.filePath);
		if (pDoc->fileInfo.filePath.IsEmpty()) {
			settingTitle(FALSE);
		}
		else {
			settingTitle(TRUE);
		}
		free(file);
	}
	else {
		return FALSE;
	}

	return TRUE;
}

void CnotepadKDYSDIView::settingTitle(BOOL existFile) {
	CnotepadKDYSDIDoc* pDoc = GetDocument();
	if (existFile == TRUE) {
		AfxGetMainWnd()->SetWindowTextW(pDoc->fileInfo.file_nameWithFormat + pDoc->title_format);
	}
	else {
		AfxGetMainWnd()->SetWindowTextW(pDoc->initialized_title + pDoc->title_format);
	}
}

void CnotepadKDYSDIView::OnMenuSaveAs()
{
	// TODO: Add your command handler code here
	CnotepadKDYSDIDoc* pDoc = GetDocument();

	//must getter active window(notepad_kdy)'s title name
	if (pDoc->titleName == L"") {
		getting_TitleName();
	}
	//idNum = buttonText();

	//save logic
	saveLogic(FALSE);
}

void CnotepadKDYSDIView::OnUpdateFilePageProperties(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable();
}


void CnotepadKDYSDIView::OnFilePageProperties()
{
	// TODO: Add your command handler code here
	CPageSetupDialog dlg;
	if (IDOK == dlg.DoModal()) {

	}
}


void CnotepadKDYSDIView::OnUpdateEditFont(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable();
}


void CnotepadKDYSDIView::OnEditFont()
{
	// TODO: Add your command handler code here
	CEdit& m_edit = GetEditCtrl();
	CString str = L"";
	CFontDialog dlg;

	if (IDOK == dlg.DoModal()) {
		CClientDC dc(this);
		
		LOGFONT lf;
		dlg.GetCurrentFont(&lf);
		CFont font;
		font.CreateFontIndirectW(&lf);

		//m_edit.SetFont(&font, TRUE);
		COLORREF color = dlg.GetColor();
		
//		dlg.SetFont(&font, TRUE);
		m_edit.SetFont(&font, TRUE);
	}
}



void CnotepadKDYSDIView::OnFileNew()
{
	// TODO: Add your command handler code here
	CnotepadKDYSDIDoc* pDoc = GetDocument();
	CEdit& m_edit = GetEditCtrl();
	CString str = L"";
	int idNum = 0;
	//must getter active window(notepad_kdy)'s title name
	if (pDoc->titleName == L"") {
		getting_TitleName();
	}

	m_edit.GetWindowTextW(str);

	if (str.IsEmpty()) {
		//empty -> error
		return;
	}
	else {
		//not empty
		AfxGetMainWnd()->GetWindowTextW(str);
		if (str.Find(L"*") == 0) {
			//exist save data
			if (pDoc->fileInfo.filePath.IsEmpty()) {
				idNum = buttonText();
				if (IDYES == idNum) {
					//save logic
					if (!saveLogic(FALSE))
						return;
				}
				else if (IDNO == idNum) {
					//make empty note_pad
					//no create new window
					CString str = pDoc->titleFullName;
					str.Replace(pDoc->titleName, pDoc->initialized_title);
					str.Replace(L"*", L"");
					AfxGetMainWnd()->SetWindowTextW(str);
					m_edit.SetWindowTextW(L"");
				}
			}
			else {
				//idNum = buttonTextWithFile(pDoc->fileInfo.file_name);
				idNum = buttonTextWithFile(pDoc->fileInfo.filePath);

				if (IDYES == idNum) {
					saveFile_second(pDoc->fileInfo.filePath);

					//저장 뒤 pDoc->fileInfo를 초기화 시켜주고, 타이틀을 초기화 시켜준다.
					settingFileInformation(L"", eBOM::eNoneBom);
					AfxGetMainWnd()->SetWindowTextW(pDoc->initialized_title + pDoc->title_format);
					m_edit.SetWindowTextW(L"");
				}
				else {
					//no working anything
					return;
				}
			}
		}
		else {
			AfxGetMainWnd()->SetWindowTextW(pDoc->initialized_title + pDoc->title_format);
			m_edit.SetWindowTextW(L"");
		}
	}
}
BOOL CnotepadKDYSDIView::checkFilePath() {
	CnotepadKDYSDIDoc* pDoc = GetDocument();

	if (pDoc->filePath.IsEmpty()) {
		return FALSE;
	}
	else
		return TRUE;
}

HBRUSH CnotepadKDYSDIView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CEditView::OnCtlColor(pDC, pWnd, nCtlColor);
	pDC->SetTextColor(m_clrText);
	// TODO:  Change any attributes of the DC here
	if (m_bRefreshCtrl) {
		SetFont(&m_fontStyle);
		m_bRefreshCtrl = FALSE;
	}
	// TODO:  Return a different brush if the default is not desired
	return hbr;
}


void CnotepadKDYSDIView::OnEnChange()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CEditView::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	
	CString title_name = L"";
	CString temp = L"*";
	AfxGetMainWnd()->GetWindowTextW(title_name);

	if (title_name.Find(L"*") != 0) {
		temp += title_name;
		AfxGetMainWnd()->SetWindowTextW(temp);
	}
}

void CnotepadKDYSDIView::getting_TitleName() {
	CnotepadKDYSDIDoc* pDoc = GetDocument();
	CString str = L"";
	int index1, index2;
	
	AfxGetMainWnd()->GetWindowTextW(pDoc->titleFullName);

	index1 = pDoc->titleFullName.ReverseFind(L'-');
	str = pDoc->titleFullName.Mid(0, index1);
	index2 = str.ReverseFind(L'.');
	if (index2 == 0) {
		pDoc->titleName = str.Mid(0, index2);
	}
	else {
		pDoc->titleName = pDoc->initialized_title;
	}
}

void CnotepadKDYSDIView::OnFileSave()
{
	// TODO: Add your command handler code here
	CnotepadKDYSDIDoc* pDoc = GetDocument();
	CEdit& m_edit = GetEditCtrl();
	CString str = L"";
	int idNum = 0;
	//must getter active window(notepad_kdy)'s title name
	if (pDoc->titleName == L"") {
		getting_TitleName();
	}

	m_edit.GetWindowTextW(str);

	if (str.IsEmpty()) {
		//empty -> error
		return;
	}
	else {
		//not empty

		AfxGetMainWnd()->GetWindowTextW(str);
		if (str.Find(L"*") == 0) {
			//exist save data
			if (!pDoc->fileInfo.filePath.IsEmpty()) {
				//현재 작성중인 텍스트가 존재한다.
				saveFile_second(pDoc->fileInfo.filePath);
				
				AfxGetMainWnd()->SetWindowTextW(pDoc->fileInfo.file_nameWithFormat + pDoc->title_format);
			}
			else {
				if (pDoc->fileInfo.filePath.IsEmpty()) {
					idNum = buttonText();
				}
				else {
					idNum = buttonTextWithFile(pDoc->fileInfo.file_nameWithFormat);
				}
				if (IDYES == idNum) {
					//save logic
					if (!saveLogic(FALSE))
						return;
				}
				else if (IDNO == idNum) {
					//make empty note_pad
					//no create new window
					CString str = pDoc->titleFullName;
					str.Replace(pDoc->titleName, pDoc->initialized_title);
					str.Replace(L"*", L"");
					AfxGetMainWnd()->SetWindowTextW(str);
					m_edit.SetWindowTextW(L"");
				}
			}
		}
	}
}

int buttonText() {
	int nButtonPressed = 0;
	TASKDIALOGCONFIG config = { sizeof(TASKDIALOGCONFIG) };
	const TASKDIALOG_BUTTON buttons[] = {
	  { IDYES, L"저장(&S)" },
	  { IDNO, L"저장 안 함(N)" }
	};
	config.cbSize = sizeof(config);
	config.dwFlags = TDF_SIZE_TO_CONTENT;
	config.pszMainIcon = NULL;
	config.hInstance = NULL;
	config.dwCommonButtons = TDCBF_CANCEL_BUTTON;
	config.pszWindowTitle = L"메모장";
	config.pszMainInstruction = L"변경 내용을 제목 없음에 저장하시겠습니까?";
	config.pButtons = buttons;
	config.cButtons = ARRAYSIZE(buttons);

	TaskDialogIndirect(&config, &nButtonPressed, NULL, NULL);

	return nButtonPressed;
}

int buttonTextWithFile(CString fileName) {
	int nButtonPressed = 0;

	TASKDIALOGCONFIG config = { sizeof(TASKDIALOGCONFIG) };
	const TASKDIALOG_BUTTON buttons[] = {
	  { IDYES, L"저장(&S)" },
	  { IDNO, L"저장 안 함(N)" }
	};
	config.cbSize = sizeof(config);
	config.dwFlags = TDF_SIZE_TO_CONTENT;
	config.pszMainIcon = NULL;
	config.hInstance = NULL;
	config.dwCommonButtons = TDCBF_CANCEL_BUTTON;
	config.pszWindowTitle = L"메모장";
	CString str = L"";
	str.Format(_T("변경 내용을 %s에 저장하시겠습니까 ? "),fileName);
	config.pszMainInstruction = T2W(str.GetBuffer());
	config.pButtons = buttons;
	config.cButtons = ARRAYSIZE(buttons);

	TaskDialogIndirect(&config, &nButtonPressed, NULL, NULL);

	return nButtonPressed;
}

void CnotepadKDYSDIView::OnEditDelete()
{
	// TODO: Add your command handler code here
	CEdit& m_edit = GetEditCtrl();
	CString str = L"", temp1 = L"", temp2 = L"";
	int st, en;
	
	m_edit.GetWindowTextW(str);
	m_edit.GetSel(st,en);
	
	temp1 = str.Mid(0, st);
	temp2 = str.Mid(en, str.GetLength());
	str = temp1 + temp2;

	m_edit.SetWindowTextW(str);
}

void CnotepadKDYSDIView::OnUpdateEditDelete(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	CEdit& m_edit = GetEditCtrl();
	int st = 0, en = 0;
	m_edit.GetSel(st, en);
	if (st != en) {
		pCmdUI->Enable(TRUE);
	}
	else {
		pCmdUI->Enable(FALSE);
	}
}

void CnotepadKDYSDIView::settingFileInformation(CString fullFileString,eBOM bom) {
	CnotepadKDYSDIDoc* pDoc = GetDocument();
	int index = 0;
	int length = fullFileString.GetLength();
	if (length != 0) {
		fullFileString.Replace(L"*", L"");
		pDoc->fileInfo.bom = bom;
		if (bom == eBOM::eUTF16BE)
			pDoc->fileInfo.be = TRUE;
		else
			pDoc->fileInfo.be = FALSE;
		pDoc->fileInfo.filePath = fullFileString;
		index = fullFileString.ReverseFind(L'\\');
		pDoc->fileInfo.file_nameWithFormat = fullFileString.Mid(index + 1, length);
		index = pDoc->fileInfo.file_nameWithFormat.ReverseFind(L'.');
		pDoc->fileInfo.file_name = pDoc->fileInfo.file_nameWithFormat.Mid(0, index);

		pDoc->titleFullName = pDoc->fileInfo.file_nameWithFormat + pDoc->title_format;

	}
	else {
		//초기화
		pDoc->fileInfo.bom = bom;
		pDoc->fileInfo.filePath = fullFileString;
		pDoc->fileInfo.file_name = fullFileString;
		pDoc->fileInfo.file_nameWithFormat = fullFileString;
	}
}

/*
void CnotepadKDYSDIView::saveFile(CString filePath) {
	CEdit& m_edit = GetEditCtrl();
	CnotepadKDYSDIDoc* pDoc = GetDocument();
	CString str = L"";
	CFileException ex;
	FILE* m_file;
	errno_t err = _tfopen_s(&m_file, filePath, _T("w"));
	if (err != 0) {
		return;
	}
	switch (pDoc->fileInfo.bom) {
	case eBOM::eNoneBom:
		break;
	case eBOM::eUTF8:
		fwrite(&pDoc->bomData.utf8, 1,3, m_file);
		break;
	case eBOM::eUTF16BE:
		fwrite(&pDoc->bomData.utf16be, 1, 2, m_file);
		break;
	case eBOM::eUTF16LE:
		fwrite(&pDoc->bomData.utf16le, 1, 2, m_file);
		break;
	case eBOM::eUTF32BE:
		fwrite(&pDoc->bomData.utf32be, 1, 4, m_file);
		break;
	case eBOM::eUTF32LE:
		fwrite(&pDoc->bomData.utf32le, 1, 4, m_file);
		break;
	}
	CStdioFile file(m_file);
	m_edit.GetWindowTextW(str);

	file.WriteString(str);
	file.Close();
}
*/

void CnotepadKDYSDIView::saveFile_second(CString filePath) {
	CEdit& m_edit = GetEditCtrl();
	CnotepadKDYSDIDoc* pDoc = GetDocument();
	CString str = L"";
	CString fileOpenMode = L"wt,ccs=";
	CFileException ex;
	FILE* m_file;

	switch (pDoc->fileInfo.bom) {
	case eBOM::eNoneBom:
		fileOpenMode = L"wt";
		break;
	case eBOM::eUTF8:
		fileOpenMode.Append(L"UTF-8");
		break;
	case eBOM::eUTF16BE:
		//fileOpenMode.Append(L"UTF-16BE");
		//fileOpenMode.Append(L"UNICODE");
		fileOpenMode = "wb";
		break;
	case eBOM::eUTF16LE:
		fileOpenMode.Append(L"UTF-16LE");
		break;
	case eBOM::eUTF32BE:
		fileOpenMode.Append(L"UTF-32BE");
		break;
	case eBOM::eUTF32LE:
		fileOpenMode.Append(L"UTF-32LE");
		break;
	}
	errno_t err = _tfopen_s(&m_file, filePath, fileOpenMode);
	if (err != 0) {
		return;
	}

	CStdioFile file(m_file);
	CString totalStr = L"";

	m_edit.GetWindowTextW(str);

	str.Replace(L"\r\n", L"\n");
	WCHAR enterOne = 0x0d;
	WCHAR enterTwo = 0x0a;
	
	if (pDoc->fileInfo.be == TRUE) {
		struct _stat st = { 0 };
		_tstat(pDoc->fileInfo.filePath, &st);
		BYTE bom[2] = { 0xFE,0xFF };
		fwrite(bom, 1, 2, m_file);
		for (int i = 0; i < str.GetLength(); i++) {
			utf16be utbe;
			utbe.p = str[i];
			BYTE b = utbe.bytes[0];
			utbe.bytes[0] = utbe.bytes[1];
			utbe.bytes[1] = b;
			fwrite(utbe.bytes, 1, 2, m_file);
		}
	}
	else {
		file.WriteString(str);
	}
	file.Close();
}

WCHAR swapByte(WCHAR str) {
	utf16be utbe;
	utbe.p = str;
	BYTE b = utbe.bytes[0];
	utbe.bytes[0] = utbe.bytes[1];
	utbe.bytes[1] = b;
	str = utbe.p;

	return str;
}

void CnotepadKDYSDIView::loadFile_second() {
	CEdit& m_edit = GetEditCtrl();
	CnotepadKDYSDIDoc* pDoc = GetDocument();
	//eBOM bom;
	CString fileOpenMode = L"rt,ccs=";
//	CString ccsType = L"";
	switch (pDoc->fileInfo.bom) {
	case eBOM::eNoneBom:
		fileOpenMode = L"rt";
		break;
	case eBOM::eUTF8:
		fileOpenMode.Append(L"UTF-8");
		break;
	case eBOM::eUTF16BE:
		//fileOpenMode.Append(L"UTF-16BE");
		//fileOpenMode.Append(L"UNICODE");
		fileOpenMode = "rb";
		pDoc->fileInfo.be = TRUE;
		break;
	case eBOM::eUTF16LE:
		fileOpenMode.Append(L"UTF-16LE");
		break;
	case eBOM::eUTF32BE:
		fileOpenMode.Append(L"UTF-32BE");
		break;
	case eBOM::eUTF32LE:
		fileOpenMode.Append(L"UTF-32LE");
		break;
	}

	FILE* m_file;
	errno_t err;
	struct _stat st = { 0 };

	err = _tfopen_s(&m_file, pDoc->fileInfo.filePath, fileOpenMode);
	//	errno_t err = _tfopen_s(&m_file, pDoc->fileInfo.filePath, _T("rt,ccs=UNICODE"));
	if (err != 0) {
		return;
	}

	if (TRUE == pDoc->fileInfo.be) {
		_tstat(pDoc->fileInfo.filePath, &st);
	}

	CStdioFile file(m_file);
	CString str = L"";
	CString totalStr = L"";
	WCHAR enterOne = 0x0d;
	WCHAR enterTwo = 0x0a;
	WCHAR temp;
	while (file.ReadString(str)) {
		if (pDoc->fileInfo.be == TRUE) {
			for (int i = 1; i < st.st_size/2; i++) {
				/*
				utf16be utbe;
				utbe.p = str[i];
				BYTE b = utbe.bytes[0];
				utbe.bytes[0] = utbe.bytes[1];
				utbe.bytes[1] = b;
				totalStr += utbe.p;
				*/
				temp = swapByte(str[i]);
				totalStr += temp;
			}
			totalStr += enterOne;
			totalStr += enterTwo;
		}
		else {
			totalStr += str;
			totalStr.Append(L"\r\n");
		}
	}
	int index = totalStr.ReverseFind(L'\r\n');
	totalStr = totalStr.Mid(0, index);
	m_edit.SetWindowTextW(totalStr);
	file.Close();
}

/*
void CnotepadKDYSDIView::loadFile() {
	CEdit& m_edit = GetEditCtrl();
	CnotepadKDYSDIDoc* pDoc = GetDocument();
	//eBOM bom;
	char strMulti[256] = { 0, };
	CString temp_str = L"";
	int len;

	FILE* m_file;
	errno_t err = _tfopen_s(&m_file, pDoc->fileInfo.filePath, _T("r"));
//	errno_t err = _tfopen_s(&m_file, pDoc->fileInfo.filePath, _T("rt,ccs=UNICODE"));
	if (err != 0) {
		return;
	}
	CStdioFile file(m_file);
	CString str = L"";
	file.ReadString(str);
	switch (pDoc->fileInfo.bom) {
	case eBOM::eNoneBom:
		break;
	case eBOM::eUTF8:
		str = str.Mid(3, str.GetLength());
		break;
	case eBOM::eUTF16BE:
		str = str.Mid(2, str.GetLength());
		break;
	case eBOM::eUTF16LE:
		str = str.Mid(2, str.GetLength());
		//str is unicode
		len = WideCharToMultiByte(CP_UTF8, 0, str, -1, NULL, str.GetLength(), NULL, NULL);
		WideCharToMultiByte(CP_UTF8,0,str,-1,strMulti,len,NULL,NULL);

		for (int i = 0; i < str.GetLength(); i++) {
			str[i] & 0xff000000 >> 24;
			str[i] & 0x00ff0000 >> 8;
			str[i] & 0x0000ff00 << 8;
			str[i] & 0x000000ff << 24;
		}
		TRACE(L"");
		break;
	case eBOM::eUTF32BE:
		str = str.Mid(4, str.GetLength());
		break;
	case eBOM::eUTF32LE:
		str = str.Mid(4, str.GetLength());
		break;
	}
	m_edit.SetWindowTextW(str);

	file.Close();
}
*/

void CnotepadKDYSDIView::OnFileOpen()
{
	// TODO: Add your command handler code here
	CEdit& m_edit = GetEditCtrl();
	CnotepadKDYSDIDoc* pDoc = GetDocument();

	/*
	TCHAR szFilter[] = L"텍스트 파일 (*.txt)|*.txt|모든 파일(*.*)|*.*||";
	CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY,szFilter);

	int index = 0;
	if (IDOK == dlg.DoModal()) {
		pDoc->fileInfo.file_nameWithFormat = dlg.GetFileName();
		pDoc->fileInfo.filePath = dlg.GetPathName();

		index = pDoc->fileInfo.file_nameWithFormat.ReverseFind(L'.');
		pDoc->fileInfo.file_name = pDoc->fileInfo.file_nameWithFormat.Mid(0, index);
		//pDoc->fileInfo.file_name = L"";
		GetBomType(pDoc->fileInfo.filePath, pDoc->fileInfo.bom);
		//loadFile();
		loadFile_second();
		AfxGetMainWnd()->SetWindowTextW(pDoc->fileInfo.file_nameWithFormat + pDoc->title_format);

		m_edit.SetFocus();
//		m_edit.SetSel(0, -1);
		m_edit.SetSel(-1);
	}
	*/

	CString str = L"";
	int idNum = 0;

	TCHAR* file;
	HRESULT ret;
	eBOM bom;

	if (pDoc->titleName == L"") {
		getting_TitleName();
	}
	m_edit.GetWindowTextW(str);
	//not empty
	AfxGetMainWnd()->GetWindowTextW(str);
	if (str.Find(L"*") == 0) {
		//exist save data
		if (pDoc->fileInfo.filePath.IsEmpty()) {
			idNum = buttonText();
		}
		else {
			idNum = buttonTextWithFile(pDoc->fileInfo.filePath);
		}
		if (IDYES == idNum || IDNO == idNum) {
			//저장 선택 시에만 저장한다.
			if (IDYES == idNum)
				saveFile_second(pDoc->fileInfo.filePath);

			file = (TCHAR*)malloc(MAX_PATH * sizeof(TCHAR));
			ret = BasicFileOpen(file, TRUE, bom);
			if (SUCCEEDED(ret)) {
				settingFileInformation(file, bom);
				loadFile_second();
			}
			free(file);
			AfxGetMainWnd()->SetWindowTextW(pDoc->fileInfo.file_nameWithFormat + pDoc->title_format);
		}
		else {
			return;
		}
	}
	else {
		file = (TCHAR*)malloc(MAX_PATH * sizeof(TCHAR));
		ret = BasicFileOpen(file, TRUE, bom);
		if (SUCCEEDED(ret)) {
			settingFileInformation(file, bom);
			loadFile_second();
		}
		free(file);
		AfxGetMainWnd()->SetWindowTextW(pDoc->fileInfo.file_nameWithFormat + pDoc->title_format);
	}
}

void CnotepadKDYSDIView::OnAppExit()
{
	// TODO: Add your command handler code here
	CnotepadKDYSDIDoc* pDoc = GetDocument();
	CEdit& m_edit = GetEditCtrl();
	CString str = L"",edit_str = L"";
	int check = 0;
	CString file_path = L"";
	TCHAR* file = (TCHAR*)malloc(MAX_PATH*sizeof(TCHAR));
	eBOM bom;
	HRESULT ret;

	AfxGetMainWnd()->GetWindowTextW(str);
	
	if (pDoc->fileInfo.filePath.IsEmpty()) {
		//현재 "제목 없음"
		if (str.Find(L"*") == 0) {
			//저장할 데이터 존재.
			check = buttonText();
			switch (check) {
			case IDYES:
				ret = BasicFileOpen(file, FALSE, bom);
				if (SUCCEEDED(ret)) {
					saveFile_second(file);
					settingFileInformation(file, bom);
					AfxGetMainWnd()->SetWindowTextW(pDoc->fileInfo.file_nameWithFormat + pDoc->title_format);
					AfxGetMainWnd()->PostMessageW(WM_CLOSE);
				}
				break;
			case IDNO:
				AfxGetMainWnd()->PostMessageW(WM_CLOSE);
				break;
			}
		}
		else {
			//종료
			AfxGetMainWnd()->PostMessageW(WM_CLOSE);
		}
	}
	else if(str.Find(L"*") == 0){
		//저장 유무를 묻기
		check = buttonText();
		switch (check) {
		case IDYES:
			saveFile_second(pDoc->fileInfo.filePath);
			AfxGetMainWnd()->PostMessageW(WM_CLOSE);
			//settingFileInformation(file, bom);
			//AfxGetMainWnd()->SetWindowTextW(pDoc->fileInfo.file_nameWithFormat + pDoc->title_format);
			break;
		case IDNO:
			AfxGetMainWnd()->PostMessageW(WM_CLOSE);
			break;
		}
	}
	free(file);
}

void CnotepadKDYSDIView::OnEditPlus()
{
	// TODO: Add your command handler code here
	CnotepadKDYSDIDoc* pDoc = GetDocument();
	CEdit& m_edit = GetEditCtrl();
	pDoc->font = new CFont();
	
	LOGFONT lf;
	m_edit.GetFont()->GetLogFont(&lf);
	lf.lfHeight += 10;

	pDoc->font->CreateFontIndirectW(&lf);
	m_edit.SetFont(pDoc->font, TRUE);

}

void CnotepadKDYSDIView::OnEditMinus()
{
	// TODO: Add your command handler code here
	CnotepadKDYSDIDoc* pDoc = GetDocument();
	CEdit& m_edit = GetEditCtrl();
	pDoc->font = new CFont();

	LOGFONT lf;
	m_edit.GetFont()->GetLogFont(&lf);
	
	if(lf.lfHeight > 10)
		lf.lfHeight -= 10;

	pDoc->font->CreateFontIndirectW(&lf);
	m_edit.SetFont(pDoc->font, TRUE);
}

void CnotepadKDYSDIView::OnEditResetPlusMinus()
{
	// TODO: Add your command handler code here
	CnotepadKDYSDIDoc* pDoc = GetDocument();
	CEdit& m_edit = GetEditCtrl();
	pDoc->font = new CFont();

	LOGFONT lf;
	m_edit.GetFont()->GetLogFont(&lf);

	lf.lfHeight = pDoc->fontHeightSize;

	pDoc->font->CreateFontIndirectW(&lf);
	m_edit.SetFont(pDoc->font, TRUE);
}

void CnotepadKDYSDIView::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	CnotepadKDYSDIDoc* pDoc = GetDocument();
	CEdit& m_edit = GetEditCtrl();
	CString str = L"";
	int idNum = 0;
	//must getter active window(notepad_kdy)'s title name
	if (pDoc->titleName == L"") {
		getting_TitleName();
	}

	if (pDoc->titleName.Find(pDoc->initialized_title) != 0 || pDoc->titleName.Find(L"*") == 0) {
		// none exist initialize_title or exist save data
		str = pDoc->titleName;
		if (pDoc->fileInfo.filePath.IsEmpty()) {
			idNum = buttonText();
		}
		else {
			idNum = buttonTextWithFile(pDoc->fileInfo.filePath);
		}

		if (IDYES == idNum && !pDoc->fileInfo.filePath.IsEmpty()) {
			saveFile_second(pDoc->fileInfo.filePath);
		}
		else if (IDYES == idNum && pDoc->fileInfo.filePath.IsEmpty()) {
			HRESULT ret;
			eBOM bom;
			CString file_path = L"";
			TCHAR* file = (TCHAR*)malloc(MAX_PATH * sizeof(TCHAR));
			ret = BasicFileOpen(file, FALSE, bom);
			if (SUCCEEDED(ret)) {
				//success to save file to disk
				settingFileInformation(file, bom);
				saveFile_second(pDoc->fileInfo.filePath);

				//if save file success
				//settingFileInformation(file, bom);
				AfxGetMainWnd()->SetWindowTextW(pDoc->fileInfo.file_nameWithFormat + pDoc->title_format);
			}
			free(file);
		}
	}
	//AfxGetMainWnd()->PostMessageW(WM_CLOSE);
	CEditView::OnClose();
}


void CnotepadKDYSDIView::OnSysCommand(UINT nID, LPARAM lParam)
{
	// TODO: Add your message handler code here and/or call default

	CEditView::OnSysCommand(nID, lParam);
}


void CnotepadKDYSDIView::OnIdclose()
{
	// TODO: Add your command handler code here
}

