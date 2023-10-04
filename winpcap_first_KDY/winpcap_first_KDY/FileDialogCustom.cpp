#include "pch.h"
#include <stdio.h>
#include <tchar.h>
#include "FileDialogCustom.h"

// BOM 타입을 검출하는 함수
BOOL GetBomType(const TCHAR* filepath, eBOM& bom)
{
	BYTE b[3] = { 0 };
	size_t len = 0;
	FILE* pf = NULL;
	bom = eBOM::eNoneBom;
	errno_t err = _tfopen_s(&pf, filepath, _T("rb"));
	if (err != ERROR_SUCCESS)
		goto RETURN_FALSE;

	if (pf == NULL)
		goto RETURN_FALSE;

	len = fread(b, 1, 3, pf);
	if (len < 2)
		goto RETURN_FALSE;
	else if (len == 2) {
		if (b[0] == 0xFE && b[1] == 0xFF)
			bom = eBOM::eUTF16BE;
		else if (b[0] == 0xFF && b[1] == 0xFE)
			bom = eBOM::eUTF16LE;
	}
	else {
		if (b[0] == 0xEF && b[1] == 0xBB && b[2] == 0xBF)
			bom = eBOM::eUTF8;
		else if (b[0] == 0xFE && b[1] == 0xFF)
			bom = eBOM::eUTF16BE;
		else if (b[0] == 0xFF && b[1] == 0xFE)
			bom = eBOM::eUTF16LE;
	}

	if (pf)
		fclose(pf);
	return TRUE;

RETURN_FALSE:
	if (pf)
		fclose(pf);
	return FALSE;
}


#include <shlobj.h>
#include <objbase.h>      // For COM headers
#include <shobjidl.h>     // for IFileDialogEvents and IFileDialogControlEvents
#include <shlwapi.h>
#include <knownfolders.h> // for KnownFolder APIs/datatypes/function headers
#include <propvarutil.h>  // for PROPVAR-related functions
#include <propkey.h>      // for the Property key APIs/datatypes
#include <propidl.h>      // for the Property System APIs
#include <strsafe.h>      // for StringCchPrintfW
#include <shtypes.h>      // for COMDLG_FILTERSPEC
#include <atlstr.h>

#pragma comment(lib, "propsys.lib")
#pragma comment(lib, "shlwapi.lib")


const COMDLG_FILTERSPEC c_rgSaveTypes[] =
{
	{L"PCAP 파일 (*.pcap)", L"*.pcap"},
	{L"모든 파일 (*.*)",     L"*.*"}
};

const COMDLG_FILTERSPEC c_rgEncoding[] =
{
	{L"1",	L"PCAP"},
	{L"2",	L"PCAPNG"},
//	{L"3",	L"유니코드(big endian)"},
//	{L"4",	L"UTF-8"}
};



#define COMBO_ENCODING	1234

// Indices of file types
#define INDEX_TEXTDOC 1
#define INDEX_WORDDOC 2
#define INDEX_WEBPAGE 3


class CDialogEventHandler : public IFileDialogEvents,
	public IFileDialogControlEvents
{
public:
	// IUnknown methods
	IFACEMETHODIMP QueryInterface(REFIID riid, void** ppv)
	{
		static const QITAB qit[] = {
			QITABENT(CDialogEventHandler, IFileDialogEvents),
			QITABENT(CDialogEventHandler, IFileDialogControlEvents),
			{ 0 },
#pragma warning(suppress:4838)
		};
		return QISearch(this, qit, riid, ppv);
	}

	IFACEMETHODIMP_(ULONG) AddRef()
	{
		return InterlockedIncrement(&_cRef);
	}

	IFACEMETHODIMP_(ULONG) Release()
	{
		long cRef = InterlockedDecrement(&_cRef);
		if (!cRef)
			delete this;
		return cRef;
	}

	// IFileDialogEvents methods
	IFACEMETHODIMP OnFileOk(IFileDialog*) { return S_OK; };
	IFACEMETHODIMP OnFolderChange(IFileDialog*) { return S_OK; };
	IFACEMETHODIMP OnFolderChanging(IFileDialog*, IShellItem*) { return S_OK; };
	IFACEMETHODIMP OnHelp(IFileDialog*) { return S_OK; };
	IFACEMETHODIMP OnSelectionChange(IFileDialog*);
	IFACEMETHODIMP OnShareViolation(IFileDialog*, IShellItem*, FDE_SHAREVIOLATION_RESPONSE*) { return S_OK; };
	IFACEMETHODIMP OnTypeChange(IFileDialog* pfd);
	IFACEMETHODIMP OnOverwrite(IFileDialog*, IShellItem*, FDE_OVERWRITE_RESPONSE*) { return S_OK; };

	// IFileDialogControlEvents methods
	IFACEMETHODIMP OnItemSelected(IFileDialogCustomize* pfdc, DWORD dwIDCtl, DWORD dwIDItem);
	IFACEMETHODIMP OnButtonClicked(IFileDialogCustomize*, DWORD) { return S_OK; };
	IFACEMETHODIMP OnCheckButtonToggled(IFileDialogCustomize*, DWORD, BOOL) { return S_OK; };
	IFACEMETHODIMP OnControlActivating(IFileDialogCustomize*, DWORD) { return S_OK; };

	CDialogEventHandler() : _cRef(1) { };
private:
	~CDialogEventHandler() { };
	long _cRef;
};

bool PathFromShellItem(IShellItem* pItem, CString& sPath)
{
	HRESULT hr;
	LPOLESTR pwsz = NULL;
	hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pwsz);

	if (FAILED(hr))
		return false;

	sPath = pwsz;
	CoTaskMemFree(pwsz);
	return true;
}

HRESULT CDialogEventHandler::OnSelectionChange(IFileDialog* pfs)
{
	TCHAR* filename = NULL;
	CComPtr<IShellItem> pItem;
	HRESULT hr = pfs->GetCurrentSelection(&pItem); // pfs->GetFileName(&filename);
	if (SUCCEEDED(hr)) {
		CString sPath;

		if (PathFromShellItem(pItem, sPath))
		{
			IFileDialogCustomize* pfdc = NULL;
			hr = pfs->QueryInterface(IID_PPV_ARGS(&pfdc));
			if (SUCCEEDED(hr)) {

				eBOM bom = eBOM::eNoneBom;
				if (GetBomType(sPath, bom)) {
					int ndx = 1;
					switch (bom) {
					case eBOM::eUTF16LE:	ndx = 2; break;
					case eBOM::eUTF16BE:	ndx = 3; break;
					case eBOM::eUTF8:		ndx = 4; break;
					}

					pfdc->SetSelectedControlItem(COMBO_ENCODING, ndx);
				}
				pfdc->Release();
			}
		}
	}
	return S_OK;
};

// IFileDialogEvents methods
// This method gets called when the file-type is changed (combo-box selection changes).
// For sample sake, let's react to this event by changing the properties show.
HRESULT CDialogEventHandler::OnTypeChange(IFileDialog* pfd)
{
	IFileSaveDialog* pfsd;
	HRESULT hr = pfd->QueryInterface(&pfsd);
	if (SUCCEEDED(hr)) {
		pfsd->Release();
	}
	return hr;
}

// IFileDialogControlEvents
// This method gets called when an dialog control item selection happens (radio-button selection. etc).
// For sample sake, let's react to this event by changing the dialog title.
HRESULT CDialogEventHandler::OnItemSelected(IFileDialogCustomize* pfdc, DWORD dwIDCtl, DWORD dwIDItem)
{
	IFileDialog* pfd = NULL;
	HRESULT hr = pfdc->QueryInterface(&pfd);
	if (SUCCEEDED(hr)) {
		pfd->Release();
	}
	return hr;
}

// Instance creation helper
HRESULT CDialogEventHandler_CreateInstance(REFIID riid, void** ppv)
{
	*ppv = NULL;
	CDialogEventHandler* pDialogEventHandler = new (std::nothrow) CDialogEventHandler();
	HRESULT hr = pDialogEventHandler ? S_OK : E_OUTOFMEMORY;
	if (SUCCEEDED(hr))
	{
		hr = pDialogEventHandler->QueryInterface(riid, ppv);
		pDialogEventHandler->Release();
	}
	return hr;
}

HRESULT BasicFileOpen(OUT TCHAR* filepath, BOOL bOpen, IN OUT eBOM& bom)
{
	// CoCreate the File Open Dialog object.

	IFileDialog* pfd = NULL;
	IFileSaveDialog* pfds = NULL;
	HRESULT hr;
	if (bOpen)
		hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pfd));
	else {
		hr = CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pfds));

		static const GUID guidFileSave = { 0xA06176C0, 0xAECD, 0x4331, { 0x9B, 0x5A, 0x80, 0x3A, 0x81, 0x9C, 0xA4, 0x6D } };
		hr = pfds->SetClientGuid(guidFileSave);

		pfd = (IFileDialog*)pfds;
	}

	if (SUCCEEDED(hr))
	{
		// Create an event handling object, and hook it up to the dialog.
		IFileDialogEvents* pfde = NULL;
		hr = CDialogEventHandler_CreateInstance(IID_PPV_ARGS(&pfde));
		if (SUCCEEDED(hr))
		{
			// Hook up the event handler.
			DWORD dwCookie;
			hr = pfd->Advise(pfde, &dwCookie);
			if (SUCCEEDED(hr))
			{
				// Set the options on the dialog.
				DWORD dwFlags;

				// Before setting, always get the options first in order not to override existing options.
				hr = pfd->GetOptions(&dwFlags);
				if (SUCCEEDED(hr))
				{
					// In this case, get shell items only for file system items.
					hr = pfd->SetOptions(dwFlags | FOS_FORCEFILESYSTEM);
					if (SUCCEEDED(hr))
					{
						// Set the file types to display only. Notice that, this is a 1-based array.
						hr = pfd->SetFileTypes(ARRAYSIZE(c_rgSaveTypes), c_rgSaveTypes);
						if (SUCCEEDED(hr))
						{
							// Set the selected file type index to Word Docs for this example.
							hr = pfd->SetFileTypeIndex(INDEX_TEXTDOC);
							if (SUCCEEDED(hr))
							{
								// Set the default extension to be ".doc" file.
								hr = pfd->SetDefaultExtension(L"txt");
								if (SUCCEEDED(hr))
								{
									// Set up a Customization.
									IFileDialogCustomize* pfdc = NULL;
									hr = pfd->QueryInterface(IID_PPV_ARGS(&pfdc));
									if (SUCCEEDED(hr))
									{
										pfdc->StartVisualGroup(1233, L"저장 타입 (E):");

										pfdc->AddComboBox(COMBO_ENCODING);
										pfdc->AddControlItem(COMBO_ENCODING, _ttoi(c_rgEncoding[0].pszName), c_rgEncoding[0].pszSpec);
										pfdc->AddControlItem(COMBO_ENCODING, _ttoi(c_rgEncoding[1].pszName), c_rgEncoding[1].pszSpec);
										//pfdc->AddControlItem(COMBO_ENCODING, _ttoi(c_rgEncoding[2].pszName), c_rgEncoding[2].pszSpec);
										//pfdc->AddControlItem(COMBO_ENCODING, _ttoi(c_rgEncoding[3].pszName), c_rgEncoding[3].pszSpec);
										pfdc->SetSelectedControlItem(COMBO_ENCODING, 1);
										pfdc->EndVisualGroup();
										pfdc->MakeProminent(1233);

										pfdc->Release();
									}
									// Show the dialog
									hr = pfd->Show(NULL);
									if (SUCCEEDED(hr))
									{
										// Obtain the result, once the user clicks the 'Open' button.
										// The result is an IShellItem object.
										IShellItem* psiResult;
										hr = pfd->GetResult(&psiResult);
										if (SUCCEEDED(hr))
										{
											// We are just going to print out the name of the file for sample sake.
											PWSTR pszFilePath = NULL;
											hr = psiResult->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);
											
											if (SUCCEEDED(hr))
											{
												//KDY custom
												_tcscpy_s(filepath, MAX_PATH, pszFilePath);
												//KDY custom
												//filepath = pszFilePath;
												CoTaskMemFree(pszFilePath);
											}

											hr = pfd->QueryInterface(IID_PPV_ARGS(&pfdc));
											if (SUCCEEDED(hr))
											{
												DWORD dwSelected = 1;
												pfdc->GetSelectedControlItem(COMBO_ENCODING, &dwSelected);

												bom = eBOM::eNoneBom;
												switch (dwSelected) {
												case 2:	bom = eBOM::eUTF16LE; break;
												case 3:	bom = eBOM::eUTF16BE; break;
												case 4:	bom = eBOM::eUTF8; break;
												}

												pfdc->Release();
											}
											psiResult->Release();
										}
									}
								}
							}
						}
					}
				}
				// Unhook the event handler.
				pfd->Unadvise(dwCookie);
			}
			pfde->Release();
		}
		pfd->Release();
	}
	return hr;
}

HRESULT BasicFileOpen_pcap(OUT TCHAR* filepath, BOOL bOpen)
{
	// CoCreate the File Open Dialog object.

	IFileDialog* pfd = NULL;
	IFileSaveDialog* pfds = NULL;
	HRESULT hr;
	if (bOpen)
		hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pfd));
	else {
		hr = CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pfds));

		static const GUID guidFileSave = { 0xA06176C0, 0xAECD, 0x4331, { 0x9B, 0x5A, 0x80, 0x3A, 0x81, 0x9C, 0xA4, 0x6D } };
		hr = pfds->SetClientGuid(guidFileSave);

		pfd = (IFileDialog*)pfds;
	}

	if (SUCCEEDED(hr))
	{
		// Create an event handling object, and hook it up to the dialog.
		IFileDialogEvents* pfde = NULL;
		hr = CDialogEventHandler_CreateInstance(IID_PPV_ARGS(&pfde));
		if (SUCCEEDED(hr))
		{
			// Hook up the event handler.
			DWORD dwCookie;
			hr = pfd->Advise(pfde, &dwCookie);
			if (SUCCEEDED(hr))
			{
				// Set the options on the dialog.
				DWORD dwFlags;

				// Before setting, always get the options first in order not to override existing options.
				hr = pfd->GetOptions(&dwFlags);
				if (SUCCEEDED(hr))
				{
					// In this case, get shell items only for file system items.
					hr = pfd->SetOptions(dwFlags | FOS_FORCEFILESYSTEM);
					if (SUCCEEDED(hr))
					{
						// Set the file types to display only. Notice that, this is a 1-based array.
						hr = pfd->SetFileTypes(ARRAYSIZE(c_rgSaveTypes), c_rgSaveTypes);
						if (SUCCEEDED(hr))
						{
							// Set the selected file type index to Word Docs for this example.
							hr = pfd->SetFileTypeIndex(INDEX_TEXTDOC);
							if (SUCCEEDED(hr))
							{
								// Set the default extension to be ".doc" file.
								hr = pfd->SetDefaultExtension(L"txt");
								if (SUCCEEDED(hr))
								{
									// Set up a Customization.
									IFileDialogCustomize* pfdc = NULL;
									hr = pfd->QueryInterface(IID_PPV_ARGS(&pfdc));
									if (SUCCEEDED(hr))
									{
										pfdc->StartVisualGroup(1233, L"저장 타입 (E):");

										pfdc->AddComboBox(COMBO_ENCODING);
										pfdc->AddControlItem(COMBO_ENCODING, _ttoi(c_rgEncoding[0].pszName), c_rgEncoding[0].pszSpec);
										pfdc->AddControlItem(COMBO_ENCODING, _ttoi(c_rgEncoding[1].pszName), c_rgEncoding[1].pszSpec);
										//pfdc->AddControlItem(COMBO_ENCODING, _ttoi(c_rgEncoding[2].pszName), c_rgEncoding[2].pszSpec);
										//pfdc->AddControlItem(COMBO_ENCODING, _ttoi(c_rgEncoding[3].pszName), c_rgEncoding[3].pszSpec);
										pfdc->SetSelectedControlItem(COMBO_ENCODING, 1);
										pfdc->EndVisualGroup();
										pfdc->MakeProminent(1233);

										pfdc->Release();
									}
									// Show the dialog
									hr = pfd->Show(NULL);
									if (SUCCEEDED(hr))
									{
										// Obtain the result, once the user clicks the 'Open' button.
										// The result is an IShellItem object.
										IShellItem* psiResult;
										hr = pfd->GetResult(&psiResult);
										if (SUCCEEDED(hr))
										{
											// We are just going to print out the name of the file for sample sake.
											PWSTR pszFilePath = NULL;
											hr = psiResult->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

											if (SUCCEEDED(hr))
											{
												//KDY custom
												_tcscpy_s(filepath, MAX_PATH, pszFilePath);
												//KDY custom
												//filepath = pszFilePath;
												CoTaskMemFree(pszFilePath);
											}

											hr = pfd->QueryInterface(IID_PPV_ARGS(&pfdc));
											if (SUCCEEDED(hr))
											{
												DWORD dwSelected = 1;
												pfdc->GetSelectedControlItem(COMBO_ENCODING, &dwSelected);

												//bom = eBOM::eNoneBom;
												//switch (dwSelected) {
												//case 2:	bom = eBOM::eUTF16LE; break;
												//case 3:	bom = eBOM::eUTF16BE; break;
												//case 4:	bom = eBOM::eUTF8; break;
												//}

												pfdc->Release();
											}
											psiResult->Release();
										}
									}
								}
							}
						}
					}
				}
				// Unhook the event handler.
				pfd->Unadvise(dwCookie);
			}
			pfde->Release();
		}
		pfd->Release();
	}
	return hr;
}