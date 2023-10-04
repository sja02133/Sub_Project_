
// notepad_KDY_SDIDoc.h : interface of the CnotepadKDYSDIDoc class
//


#pragma once
#include "FileDialogCustom.h"

#define ID_SAVE 1
#define ID_NO_SAVE 2
#define ID_CANCEL 0

struct BOM_DATA {
	BYTE ansi = 0xFF;
	BYTE utf8[3] = { 0xEF, 0xBB, 0xBF };
	BYTE utf16be[2] = { 0xFE, 0xFF };
	BYTE utf16le[2] = { 0xFF,0xFE };
	BYTE utf32be[4] = { 0x00,0x00,0xFE,0xFF };
	BYTE utf32le[4] = { 0xFF,0xFE,0x00,0x00 };
};

struct FILE_INFO {
	eBOM bom;
	CString filePath = L"";
	CString file_nameWithFormat = L"";
	CString file_name = L"";
	BOOL be;
};

class CnotepadKDYSDIDoc : public CDocument
{
protected: // create from serialization only
	CnotepadKDYSDIDoc() noexcept;
	DECLARE_DYNCREATE(CnotepadKDYSDIDoc)

// Attributes
public:

// Operations
public:
	CString title_format;
	CString str;
	CString initialized_title;
	CString titleFullName;
	CString titleName;
	CString filePath; //if this filePath is empty, not yet to save file to any disk.

	FILE_INFO fileInfo;
	BOM_DATA bomData;

	CString notepad_path;

	LONG fontHeightSize;
	CFont* font;

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CnotepadKDYSDIDoc();
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
};
