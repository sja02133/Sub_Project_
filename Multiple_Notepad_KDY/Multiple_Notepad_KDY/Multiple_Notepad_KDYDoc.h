
// Multiple_Notepad_KDYDoc.h : interface of the CMultipleNotepadKDYDoc class
//

#pragma once
#include "FileDialogCustom.h"


//////////////////////////
typedef struct _FILE_INFO {
	eBOM bom;
	CString filePath = L"";
	CString file_nameWithFormat = L"";
	CString file_name = L"";
	BOOL be;
} FILE_INFO;

typedef struct _TITLE_INFO {
	CString initialized_title;
	CString currentTitle;
} TITLE_INFO;

typedef struct _BOM_DATA {
	BYTE ansi = 0xFF;
	BYTE utf8[3] = { 0xEF, 0xBB, 0xBF };
	BYTE utf16be[2] = { 0xFE, 0xFF };
	BYTE utf16le[2] = { 0xFF,0xFE };
	BYTE utf32be[4] = { 0x00,0x00,0xFE,0xFF };
	BYTE utf32le[4] = { 0xFF,0xFE,0x00,0x00 };
} BOM_DATA;

class CMultipleNotepadKDYDoc : public CDocument
{
protected: // create from serialization only
	CMultipleNotepadKDYDoc() noexcept;
	DECLARE_DYNCREATE(CMultipleNotepadKDYDoc)

// Attributes
public:

	CString notePad_path;

	FILE_INFO fileInfo;
	TITLE_INFO titleInfo;
	BOM_DATA bomData;

// Operations
public:

// Overrides
public:
	void document_GetView(CView* pView);
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
//	virtual BOOL CanCloseFrame(CFrameWnd* pFrame);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CMultipleNotepadKDYDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS

#ifdef SHARED_HANDLERS
private:
	CString m_strSearchContent;
	CString m_strThumbnailContent;
#endif // SHARED_HANDLERS
public:
	afx_msg void OnFileSave();
	afx_msg void OnFileSaveAs();
	afx_msg void OnFileClose();
//	virtual void OnCloseDocument();
	virtual BOOL SaveModified();

//	CMultipleNotepadKDYView* GetDocument() const;

	void fileOpen();
	void fileSave();
	void fileSaveAs();

	void loadFile_second();
	BOOL saveTXT();
	BOOL saveLogic(BOOL openFile);
	BOOL settingFileInformation(CString fullFileString, eBOM bom);
	void saveFile_second(CString filePath);
	void settingTitle(BOOL existFile);
	BOOL checkingOverlap(CString filePath);
};
