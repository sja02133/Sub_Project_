
// Multiple_Notepad_KDYDoc.cpp : implementation of the CMultipleNotepadKDYDoc class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Multiple_Notepad_KDY.h"
#endif

#include "Multiple_Notepad_KDYDoc.h"
#include "Multiple_Notepad_KDYView.h"

#include <propkey.h>

#include <vector>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMultipleNotepadKDYDoc

IMPLEMENT_DYNCREATE(CMultipleNotepadKDYDoc, CDocument)

BEGIN_MESSAGE_MAP(CMultipleNotepadKDYDoc, CDocument)
	ON_COMMAND(ID_FILE_SAVE, &CMultipleNotepadKDYDoc::OnFileSave)
	ON_COMMAND(ID_FILE_SAVE_AS, &CMultipleNotepadKDYDoc::OnFileSaveAs)
	ON_COMMAND(ID_FILE_CLOSE, &CMultipleNotepadKDYDoc::OnFileClose)
END_MESSAGE_MAP()


// CMultipleNotepadKDYDoc construction/destruction

CMultipleNotepadKDYDoc::CMultipleNotepadKDYDoc() noexcept
{
	// TODO: add one-time construction code here

}

CMultipleNotepadKDYDoc::~CMultipleNotepadKDYDoc()
{
}

BOOL CMultipleNotepadKDYDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	// title setting initialized...
	titleInfo.initialized_title = "제목 없음";
	titleInfo.currentTitle = L"";

	// file info setting initialized...
	fileInfo.be = FALSE;
	fileInfo.bom = eBOM::eNoneBom;
	fileInfo.filePath = L"";
	fileInfo.file_name = L"";
	fileInfo.file_nameWithFormat = L"";

	// document variant setting initialized...
	TCHAR str[1000] = { 0 };
	GetModuleFileName(NULL, str, 1000);
	notePad_path = str;

	SetTitle(titleInfo.initialized_title);



	return TRUE;
}

// CMultipleNotepadKDYDoc serialization

void CMultipleNotepadKDYDoc::Serialize(CArchive& ar)
{
	// CEditView contains an edit control which handles all serialization
	if (!m_viewList.IsEmpty())
	{
		reinterpret_cast<CMultipleNotepadKDYView*>(m_viewList.GetHead())->SerializeRaw(ar);
	}
#ifdef SHARED_HANDLERS

	if (m_viewList.IsEmpty() && ar.IsLoading())
	{
		CFile* pFile = ar.GetFile();
		pFile->Seek(0, FILE_BEGIN);
		ULONGLONG nFileSizeBytes = pFile->GetLength();
		ULONGLONG nFileSizeChars = nFileSizeBytes/sizeof(TCHAR);
		LPTSTR lpszText = (LPTSTR)malloc(((size_t)nFileSizeChars + 1) * sizeof(TCHAR));
		if (lpszText != nullptr)
		{
			ar.Read(lpszText, (UINT)nFileSizeBytes);
			lpszText[nFileSizeChars] = '\0';
			m_strThumbnailContent = lpszText;
			m_strSearchContent = lpszText;
		}
	}
#endif
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CMultipleNotepadKDYDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(m_strThumbnailContent, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CMultipleNotepadKDYDoc::InitializeSearchContent()
{
	// Set search contents from document's data.
	// The content parts should be separated by ";"

	// Use the entire text file content as the search content.
	SetSearchContent(m_strSearchContent);
}

void CMultipleNotepadKDYDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CMultipleNotepadKDYDoc diagnostics

#ifdef _DEBUG
void CMultipleNotepadKDYDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMultipleNotepadKDYDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CMultipleNotepadKDYDoc commands


void CMultipleNotepadKDYDoc::OnFileSave()
{
	TRACE(_T("%s\n"), __FUNCTIONW__);
	//theApp.fileSave();
	this->fileSave();
	if (this->GetTitle().Find(L"*") != 0)
		this->SetModifiedFlag(FALSE);
}


void CMultipleNotepadKDYDoc::OnFileSaveAs()
{
	TRACE(_T("%s\n"), __FUNCTIONW__);
	//theApp.fileSaveAs();
	this->fileSaveAs();
}

/*
void CMultipleNotepadKDYDoc::OnCloseDocument()
{
	// TODO: Add your specialized code here and/or call the base class

	//CChildFrame의 OnClose와 이곳의 OnClose가 두번씩 호출됨
	if (theApp.OnClose()) {
		DeleteContents();
		UpdateAllViews(NULL);
	}



	CDocument::OnCloseDocument();
}
*/

void CMultipleNotepadKDYDoc::document_GetView(CView* pView)
{
	CDocument* pDoc = (CDocument*)this;
	POSITION pos = pDoc->GetFirstViewPosition();

	POSITION viewPos = pDoc->GetFirstViewPosition();
	while (viewPos) {
		pView = pDoc->GetNextView(viewPos);
	}

	if (pView == NULL) {
		//ERROR
		return;
	}

	//return (CMultipleNotepadKDYView*)pView;
}


BOOL CMultipleNotepadKDYDoc::SaveModified()
{
	// TODO: Add your specialized code here and/or call the base class
	
	if (this->saveTXT()) {
//		CDocument::OnCloseDocument();
		return TRUE;
	}

	return FALSE;
	
	
//	return CDocument::SaveModified();
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

int buttonText(CString fileName)
{
	int nButtonPressed = 0;
	TASKDIALOGCONFIG config = { sizeof(TASKDIALOGCONFIG) };
	const TASKDIALOG_BUTTON buttons[] = {
	  { IDYES, L"저장(&S)" },
	  { IDNO, L"저장 안 함(N)" }
	};
	CString str = L"";
	config.cbSize = sizeof(config);
	config.dwFlags = TDF_SIZE_TO_CONTENT;
	config.pszMainIcon = NULL;
	config.hInstance = NULL;
	config.dwCommonButtons = TDCBF_CANCEL_BUTTON;
	config.pszWindowTitle = L"메모장";
	if (FALSE == fileName.IsEmpty()) {
		//NOT EMPTY
		str.Format(_T("변경 내용을 %s에 저장하시겠습니까 ? "), fileName);
		config.pszMainInstruction = T2W(str.GetBuffer());
	}
	else {
		//EMPTY
		config.pszMainInstruction = L"변경 내용을 제목 없음에 저장하시겠습니까?";
	}
	config.pButtons = buttons;
	config.cButtons = ARRAYSIZE(buttons);

	TaskDialogIndirect(&config, &nButtonPressed, NULL, NULL);

	return nButtonPressed;
}

BOOL CMultipleNotepadKDYDoc::saveTXT() {
	
	CMultipleNotepadKDYDoc* pDoc = this;
	CMultipleNotepadKDYView* pView = (CMultipleNotepadKDYView*)this->m_viewList.GetHead();

	CEdit& m_edit = pView->GetEditCtrl();
	CString str = L"";
	int idNum = 0;

	str = pDoc->GetTitle();

	//	if (str.Find(L"*") == 0) {
	if (pDoc->IsModified()) {
		if (pDoc->fileInfo.filePath.IsEmpty()) {
			idNum = buttonText(L"");
			if (IDYES == idNum) {
				saveLogic(FALSE);
			}
			else if (IDNO == idNum) {
				return TRUE;
			}
			else {
				return FALSE;
			}
			return TRUE;
		}
		else {
			idNum = buttonText(pDoc->fileInfo.file_nameWithFormat);
			if (IDYES == idNum) {
				saveFile_second(pDoc->fileInfo.filePath);
				settingTitle(TRUE);
			}
			else if (IDNO == idNum) {
				//pDoc->OnCloseDocument();
				return TRUE;
			}
			else {
				return FALSE;
			}
			return TRUE;
		}
	}
	else {
		//non save any data
		return TRUE;
	}
	return TRUE;




	return FALSE;
}

BOOL CMultipleNotepadKDYDoc::saveLogic(BOOL openFile) {
	//CMultipleNotepadKDYDoc* pDoc = (CMultipleNotepadKDYDoc*)pointDoc;
	CMultipleNotepadKDYDoc* pDoc = this;

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

BOOL CMultipleNotepadKDYDoc::settingFileInformation(CString fullFileString, eBOM bom) 
{
	CMultipleNotepadKDYDoc* pDoc = this;
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
		pDoc->titleInfo.currentTitle = pDoc->fileInfo.file_nameWithFormat;
	}
	else {
		//초기화
		pDoc->fileInfo.bom = bom;
		pDoc->fileInfo.filePath = fullFileString;
		pDoc->fileInfo.file_name = fullFileString;
		pDoc->fileInfo.file_nameWithFormat = fullFileString;
		pDoc->titleInfo.currentTitle = pDoc->titleInfo.initialized_title;
	}
	return TRUE;
}

void CMultipleNotepadKDYDoc::saveFile_second(CString filePath) {
	CMultipleNotepadKDYDoc* pDoc = this;
	CMultipleNotepadKDYView* pView = (CMultipleNotepadKDYView*)this->m_viewList.GetHead();
	CEdit& m_edit = pView->GetEditCtrl();

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
		str.Replace(L"\n", L"\r\n");
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

void CMultipleNotepadKDYDoc::settingTitle(BOOL existFile) {
	CMultipleNotepadKDYDoc* pDoc = this;
	
	if (existFile == TRUE) {
		pDoc->titleInfo.currentTitle = pDoc->fileInfo.file_nameWithFormat;
		pDoc->SetTitle(pDoc->titleInfo.currentTitle);
	}
	else {
		pDoc->titleInfo.currentTitle = pDoc->titleInfo.initialized_title;
		pDoc->SetTitle(pDoc->titleInfo.initialized_title);
	}
}

void CMultipleNotepadKDYDoc::fileOpen()
{
	CMultipleNotepadKDYDoc* pDoc = this;
	CMultipleNotepadKDYView* pView = (CMultipleNotepadKDYView*)this->m_viewList.GetHead();
	CEdit& m_edit = pView->GetEditCtrl();

	CString str = L"";
	int idNum = 0;

	TCHAR* file;
	HRESULT ret;
	eBOM bom;

	file = (TCHAR*)malloc(MAX_PATH * sizeof(TCHAR));
	ret = BasicFileOpen(file, TRUE, bom);

	//checking overlap
	BOOL checkOverlap = checkingOverlap(file);

	if (SUCCEEDED(ret) && checkOverlap) {
		if (settingFileInformation(file, bom)) {
			settingTitle(TRUE);
			loadFile_second();
			//pDoc->SetTitle(pDoc->fileInfo.file_nameWithFormat);
		}
	}
	else if (!SUCCEEDED(ret)) {
		pDoc->OnCloseDocument();
	}
	else if (checkOverlap == FALSE) {
		((CMultipleNotepadKDYView*)pView)->OnClose();
		pDoc->OnCloseDocument();
	}
	free(file);

	return;
}

BOOL CMultipleNotepadKDYDoc::checkingOverlap(CString filePath) {
	CString titleStr = L"";

	POSITION pos = AfxGetApp()->GetFirstDocTemplatePosition();
	while (pos) {
		CDocTemplate* pdoct = AfxGetApp()->GetNextDocTemplate(pos);
		if (pdoct) {
			POSITION docPos = pdoct->GetFirstDocPosition();
			while (docPos) {
				CDocument* pDoc = pdoct->GetNextDoc(docPos);
				CMultipleNotepadKDYDoc* pDocNote = (CMultipleNotepadKDYDoc*)pDoc;
				if (pDocNote->fileInfo.filePath.Compare(filePath) == 0)
					return FALSE;
			}
		}
	}
	return TRUE;
}

void CMultipleNotepadKDYDoc::fileSaveAs() {
	CMultipleNotepadKDYDoc* pDoc = this;
	CMultipleNotepadKDYView* pView = (CMultipleNotepadKDYView*)this->m_viewList.GetHead();
	CEdit& m_edit = pView->GetEditCtrl();

	CString str = L"";
	int idNum = 0;
	//must getter active window(notepad_kdy)'s title name

	//AfxGetMainWnd()->GetWindowTextW(str);
	str = pDoc->GetTitle();

	saveLogic(FALSE);
}

void CMultipleNotepadKDYDoc::fileSave()
{
	CMultipleNotepadKDYDoc* pDoc = this;
	CMultipleNotepadKDYView* pView = (CMultipleNotepadKDYView*)this->m_viewList.GetHead();
	CEdit& m_edit = pView->GetEditCtrl();

	// TODO: Add your command handler code here
	CString str = L"";
	int idNum = 0;
	//must getter active window(notepad_kdy)'s title name

	//AfxGetMainWnd()->GetWindowTextW(str);
	str = pDoc->GetTitle();
	if (str.Find(L"*") == 0) {
		//exist save data
		if (!pDoc->fileInfo.filePath.IsEmpty()) {
			//현재 작성중인 텍스트가 존재한다.
			saveFile_second(pDoc->fileInfo.filePath);
			str.Replace(L"*", L"");
			pDoc->SetTitle(str);
		}
		else {
			idNum = buttonText(pDoc->fileInfo.file_nameWithFormat);
			if (IDYES == idNum) {
				//save logic
				if (!saveLogic(FALSE))
					return;
			}
			else if (IDNO == idNum) {
				//make empty note_pad
				//no create new window
				settingFileInformation(L"", eBOM::eNoneBom);
				settingTitle(FALSE);
				m_edit.SetWindowTextW(L"");
			}
		}
	}
	else if (str.Find(pDoc->titleInfo.initialized_title) == 0
		&& str.GetLength() == pDoc->titleInfo.initialized_title.GetLength()) {
		//
		saveLogic(FALSE);
	}
}

void CMultipleNotepadKDYDoc::loadFile_second() {
	CMultipleNotepadKDYDoc* pDoc = this;
	CMultipleNotepadKDYView* pView = (CMultipleNotepadKDYView*)this->m_viewList.GetHead();

	CEdit& m_edit = pView->GetEditCtrl();

	int bomSize = 0;

	FILE* m_file;
	errno_t err;
	struct _stat st = { 0 };

	if (TRUE == pDoc->fileInfo.be) {
		_tstat(pDoc->fileInfo.filePath, &st);
	}

	_tstat(pDoc->fileInfo.filePath, &st);

//	TCHAR* fileStr = (TCHAR*)malloc(sizeof(st.st_size * sizeof(TCHAR)));
	TCHAR* fileStr = (TCHAR*)calloc(sizeof(TCHAR),st.st_size);
	//eBOM bom;
	CString fileOpenMode = L"rt,ccs=";
	//	CString ccsType = L"";
	switch (pDoc->fileInfo.bom) {
	case eBOM::eNoneBom:
		fileOpenMode = L"rt";
		bomSize = 0;
		break;
	case eBOM::eUTF8:
		fileOpenMode.Append(L"UTF-8");
		bomSize = 3;
		break;
	case eBOM::eUTF16BE:
		fileOpenMode = "rb";
		pDoc->fileInfo.be = TRUE;
		bomSize = 2;
		break;
	case eBOM::eUTF16LE:
		fileOpenMode.Append(L"UTF-16LE");
		bomSize = 2;
		break;
	case eBOM::eUTF32BE:
		fileOpenMode.Append(L"UTF-32BE");
		bomSize = 4;
		break;
	case eBOM::eUTF32LE:
		fileOpenMode.Append(L"UTF-32LE");
		bomSize = 4;
		break;
	}

	err = _tfopen_s(&m_file, pDoc->fileInfo.filePath, fileOpenMode);
	//	errno_t err = _tfopen_s(&m_file, pDoc->fileInfo.filePath, _T("rt,ccs=UNICODE"));
	if (err != 0) {
		return;
	}

	if (m_file == nullptr)
		return;
	if (fileStr == nullptr)
		return;

	int result = 0;
	result = fseek(m_file, bomSize, SEEK_SET);

	if (result == 0)
		TRACE(L"Pointer successfully moved to the beginning of the file.\n");
	else
		TRACE(L"Failed moving pointer to the beginning of the file.\n");


	fread(fileStr, sizeof(TCHAR), st.st_size, m_file);

	CString str = L"",tempStr = L"";

	if (pDoc->fileInfo.be == TRUE) {
		for (int i = 0; i < st.st_size; i++) {
			fileStr[i] = swapByte(fileStr[i]);
		}
	}

	if(bomSize == 0)
		str = (LPCSTR)fileStr;
	else
		str = (LPCWSTR)fileStr;

	str.Replace(L"\n", L"\r\n");
	m_edit.SetWindowTextW(str);

	free(fileStr);
	fclose(m_file);


//	free(fileStr);
	/*
	while (file.ReadString(str) != NULL) {
		if (pDoc->fileInfo.be == TRUE) {
			for (int i = 1; i < st.st_size / 2; i++) {
				temp = swapByte(str[i]);
				totalStr += temp;
			}
			totalStr += enterOne;
			totalStr += enterTwo;
		}
		else {
			totalStr += str;
			tempStr = str;
			totalStr.Append(L"\r\n");
		}
	}
	*/
//	int index = totalStr.ReverseFind(L'\r\n');;

	//2022.09.08
	//맨 마지막 \r\n일 경우, readString이 이전 줄까지만 읽어오고 마지막 줄은 읽어오지 않음.
	/*
	int index = 0;
	if (tempStr.Compare(L"") != 0)
		index = totalStr.ReverseFind(L'\r\n');
	else
		index = totalStr.GetLength();
		*/
//	totalStr = totalStr.Mid(0, index);
//	m_edit.SetWindowTextW(totalStr);
//	file.Close();
}



void CMultipleNotepadKDYDoc::OnFileClose()
{
	// TODO: Add your command handler code here
	CMultipleNotepadKDYDoc* pDoc = this;
	CMultipleNotepadKDYView* pView = (CMultipleNotepadKDYView*)this->m_viewList.GetHead();

	CEdit& m_edit = pView->GetEditCtrl();

	int idNum = 0;


	if (pDoc->GetTitle().Find(L"*") == 0) {
		//find *
		idNum = buttonText(pDoc->fileInfo.file_nameWithFormat);
		if (IDYES == idNum)
			saveLogic(FALSE);
		else if (IDNO == idNum)
			pDoc->OnCloseDocument();
	}
	else {
		pDoc->OnCloseDocument();
	}
	return;
}
