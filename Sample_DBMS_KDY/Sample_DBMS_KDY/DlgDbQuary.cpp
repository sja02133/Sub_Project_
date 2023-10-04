// DlgDbQuary.cpp : implementation file
//

#include "pch.h"
#include "Sample_DBMS_KDY.h"
#include "DlgDbQuary.h"
#include "afxdialogex.h"
#include "DataBaseRec.h"

#include "odbc_kdy.h"
// cDlgDbQuary dialog

IMPLEMENT_DYNAMIC(cDlgDbQuary, CDialogEx)

CDatabase m_db;
CRecordset* m_pRs;
COdbc_kdy odbc_kdy;

cDlgDbQuary::cDlgDbQuary(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_QUARY, pParent)
	, m_strEditResult(_T(""))
{
	
}

cDlgDbQuary::~cDlgDbQuary()
{
}

void cDlgDbQuary::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_RESULT, m_clResult);
	DDX_Text(pDX, IDC_EDIT_RESULT_MESSAGE, m_strEditResult);
}


BEGIN_MESSAGE_MAP(cDlgDbQuary, CDialogEx)
//	ON_WM_KEYDOWN()
END_MESSAGE_MAP()


// cDlgDbQuary message handlers


//void cDlgDbQuary::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
//{
//	// TODO: Add your message handler code here and/or call default
//
//	CDialogEx::OnKeyDown(nChar, nRepCnt, nFlags);
//}

int CALLBACK CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort) {
	return TRUE;
}

void cDlgDbQuary::settingListControl(COdbc_kdy* odbc) {
	int rowCount = 0;
	
	m_clResult.DeleteAllItems();
	while (m_clResult.GetHeaderCtrl()->GetItemCount() > 0) {
		m_clResult.DeleteColumn(0);
	}

	for (int i = 0; i < odbc->table_info->table_col_num; i++) {
		if(0 == i)
			m_clResult.InsertColumn(i, _T(""), LVCFMT_LEFT, 40);

		m_clResult.InsertColumn(i + 1, odbc->table_info->table_data[i]->buf, LVCFMT_LEFT, 80);
	}

	CString str = L"";

	while (SQLFetch(odbc->stmt) != SQL_NO_DATA) {
		for (int i = 0; i < odbc->table_info->table_col_num; i++) {
			if (0 == i) {
				str.Format(L"%d", rowCount + 1);
				m_clResult.InsertItem(0, str);
			}
			if (odbc->table_info->table_data[i]->nullable != SQL_NULL_DATA) {
				str = (char*)odbc->table_info->table_data[i]->target_value_ptr;
				if (odbc->table_info->table_data[i]->strlen_or_ind == SQL_NULL_DATA) {
					m_clResult.SetItemText(0, i + 1, L"NULL");
				}
				else
					m_clResult.SetItemText(0, i + 1, str);
			}
			else {
				str = L"NULL";
				m_clResult.SetItemText(0, i + 1, str);
			}
			str = L"";
		}
		rowCount++;
	}

	TRACE(L"");
	for (int i = 0; i < odbc->table_info->table_col_num; i++) {
		if (odbc->table_info->table_data[i]->target_value_ptr != NULL) {
			free(odbc->table_info->table_data[i]->target_value_ptr);
		}
	}

	SORTPARAM sParam;
	sParam.pList = &m_clResult;
	sParam.iSortColumn = 1;
	sParam.bSortDirect = FALSE;

	//sParam.pList->SortItems(CompareFunc, (LPARAM)&sParam);
	sParam.pList->SortItems(CompareFunc, (LPARAM)&sParam);
}

void clearEmptySpace(CString& copy,CString& paste) {
	copy.TrimLeft();
	paste = copy;;
}

BOOL cDlgDbQuary::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class

	HWND hWnd = pMsg->hwnd;
	HDC hdc = ::GetDC(hWnd);
	CDC* pDC = GetDC();

	CString query = L"",tempQuery = L"";
	int index = 0;
	if (pMsg->message == WM_KEYDOWN) {
		if (pMsg->hwnd == GetDlgItem(IDC_EDIT_QUARY)->m_hWnd) {
			if ((::GetKeyState(VK_CONTROL) < 0 && pMsg->wParam == VK_RETURN)
				|| pMsg->wParam == VK_F5) {
				// ENTER KEY
				//sendQuery(L"SELECT * FROM aaa");
				m_clResult.SetRedraw(FALSE);
				{
					int st, en;
					((CEdit*)GetDlgItem(IDC_EDIT_QUARY))->GetSel(st, en);
					GetDlgItemText(IDC_EDIT_QUARY, query);
					if (st != en) {
						query = query.Mid(st, en);
					}
					clearEmptySpace(query, tempQuery);
					tempQuery.MakeUpper();

					if (theApp.odbcCheck == TRUE) {
						if (odbc_kdy.sendQuery(query.Mid(index, query.GetLength()))) {
							UpdateData(TRUE);
							if (!m_strEditResult.IsEmpty()) {
								m_strEditResult += L"\r\n";
							}
							m_strEditResult += L"조회 성공!!!";
							UpdateData(FALSE);
							settingListControl(&odbc_kdy);
						}
						else if (odbc_kdy.checkGoodQuery == FALSE) {
							UpdateData(TRUE);
							if (!m_strEditResult.IsEmpty()) {
								m_strEditResult += L"\r\n";
							}


							SQLWCHAR SQLState[1024] = { 0 };
							SQLINTEGER NativeErrorPtr = 0;
							SQLWCHAR MessageText[1024] = { 0 };
							SQLSMALLINT BufferLength = 1024;
							SQLSMALLINT TextLengthPtr = 1024;
							SQLSMALLINT recNumber = 1;

							SQLRETURN rc = SQLGetDiagRec(
								SQL_HANDLE_STMT,
								odbc_kdy.stmt,
								recNumber,
								SQLState,
								&NativeErrorPtr,
								MessageText,
								BufferLength,
								&TextLengthPtr);

							if(rc == SQL_SUCCESS)
								m_strEditResult += MessageText;
							else
								m_strEditResult += L"쿼리 실패...";
							UpdateData(FALSE);
						}
						else {
							UpdateData(TRUE);
							if (!m_strEditResult.IsEmpty()) {
								m_strEditResult += L"\r\n";
							}
							m_strEditResult += L"쿼리 성공!!!";
							UpdateData(FALSE);
						}
					}
					else if (theApp.odbcCheck == FALSE) {
						sendSelectQuery(query.Mid(index, query.GetLength()));
					}
				}
				m_clResult.SetRedraw(TRUE);

				return TRUE;
			}
			/*
			else if (pMsg->wParam == VK_RETURN) {
				CEdit* edit = ((CEdit*)GetDlgItem(IDC_EDIT_QUARY));
				int len = edit->GetWindowTextLengthW();
				int sel, sel2;
				CString str = L"", tempStr1 = L"", tempStr2 = L"";
				GetDlgItemText(IDC_EDIT_QUARY,str);
				edit->GetSel(sel, sel2);
				if (sel == sel2) {
					//edit->SetSel(len, len);
					//edit->ReplaceSel(_T("\r\n"));
					tempStr1 = str.Mid(0, sel);
					tempStr1 += L"\r\n";
					tempStr2 = str.Mid(sel, str.GetLength());
					str = tempStr1 + tempStr2;
				}
				else if (sel < sel2) {
					edit->SetSel(len, len);
					edit->ReplaceSel(_T("\r\n"));
				}
				edit->SetSel(sel+2);
				SetDlgItemText(IDC_EDIT_QUARY, str);
				return TRUE;
			}
			else if (pMsg->wParam == VK_ESCAPE) {
				return TRUE;
			}
			else if (pMsg->wParam == VK_BACK) {
				CEdit* edit = ((CEdit*)GetDlgItem(IDC_EDIT_QUARY));
				int len = edit->GetWindowTextLengthW();
				int sel, sel2;
				edit->GetSel(sel, sel2);
				edit->ReplaceSel(L"");
				CString str = L"",tempStr1 = L"",tempStr2 = L"";
				GetDlgItemText(IDC_EDIT_QUARY,str);
				for (int i = str.GetLength(); i > 0 ; i--) {
					if (i == sel) {
						if (str[i-1] == '\n' && str[i - 2] == '\r') {
							tempStr1 = str.Mid(0, i - 2);
							tempStr2 = str.Mid(i, str.GetLength());
							str = tempStr1 + tempStr2;
						}
					}
				}
				SetDlgItemText(IDC_EDIT_QUARY,str);
				
				edit->SetFocus();

				
				//CPoint xy;
				//xy = edit->GetCaretPos();
				//TRACE(L"");
				//edit->SetCaretPos(CPoint(10, 10));
				return TRUE;
			}
			*/
		}
//		else if (pMsg->wParam == VK_RETURN)
//			return TRUE;
		else if (pMsg->wParam == VK_ESCAPE)
			return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

void cDlgDbQuary::settingResultMessage(CString str,BOOL isQuery) {
	CString timeStr = L"";
	
	CTime ctime = CTime::GetCurrentTime();
	m_strEditResult.Append(L"\n>> "+str);
	
	if (isQuery) {
		timeStr.Format(L"\n쿼리 완료 시각 : %04d-%02d-%02d, %02d:%02d:%02d", ctime.GetYear(), ctime.GetMonth(), ctime.GetDay(), ctime.GetHour(), ctime.GetMinute(), ctime.GetSecond());
		m_strEditResult.Append(timeStr);
	}
	UpdateData(FALSE);
}

int cDlgDbQuary::checkQuery(CString query) {
	int check = 0;
	// 1 -> SELECT, 2 -> UPDATE,DELETE,INSERT
	
	if (0 == (query.Find(L"SELECT"))) {
		check = 1;
	}
	else {
		check = 2;
	}

	return check;
}

void cDlgDbQuary::sendOtherQuery(CString query) {
	m_db.BeginTrans();

	try {
		m_db.ExecuteSQL(query);
	}
	catch (CException* e) {
		e->ReportError();
	}

	m_db.CommitTrans();
	
	if (m_pRs->IsOpen()) {
		m_pRs->Close();
	}

}
BOOL cDlgDbQuary::sendSelectQuery(CString query) {
	
	CString value;
	CRect rect;

	m_clResult.GetWindowRect(&rect);
	m_clResult.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	m_clResult.DeleteAllItems();
	while (m_clResult.GetHeaderCtrl()->GetItemCount() > 0) {
		m_clResult.DeleteColumn(0);
	}

	CODBCFieldInfo info;

	BOOL check = TRUE;

	m_pRs->Close();

	try {

		check = m_pRs->Open(CRecordset::dynaset, query,CRecordset::executeDirect);
		TRACE(L"");
		//m_db.ExecuteSQL(query);
		if (!m_pRs->IsOpen())
			MessageBox(L"failed to query");
		else {
			if (m_pRs->GetRecordCount() > 0) {
				m_pRs->MoveFirst();

				int nFields = m_pRs->GetODBCFieldCount();

				for (int i = 0; i < nFields; i++) {
					m_pRs->GetODBCFieldInfo(i, info);
					if (0 == i)
						m_clResult.InsertColumn(i, _T(""), LVCFMT_LEFT, 80);

					m_clResult.InsertColumn(i + 1, info.m_strName, LVCFMT_LEFT, 80);
				}

				//UpdateData(TRUE);
				int j = 0;
				CString strCount;
				int count = 1;
				int reverseCount = 0;

				TRACE(L"");
				while (!m_pRs->IsEOF()) {
					for (int i = 0; i < nFields; i++) {
						m_pRs->GetFieldValue(i, value);
						strCount.Format(L"%d", count);
						if (0 == j) {
							m_clResult.InsertItem(0, strCount);
							count++;
						}
						m_clResult.SetItemText(0, j + 1, value);
						j++;
					}
					j = 0;
					m_pRs->MoveNext();
				}

				strCount.Format(L"%d 행이 조회됨.", (count - 1));
				settingResultMessage(strCount, TRUE);

				SORTPARAM sParam;
				sParam.pList = &m_clResult;
				sParam.iSortColumn = 1;
				sParam.bSortDirect = FALSE;

				//sParam.pList->SortItems(CompareFunc, (LPARAM)&sParam);
				sParam.pList->SortItems(CompareFunc,(LPARAM)&sParam);
				return TRUE;
			}
			else {
				return FALSE;
			}
		}
	}
	catch (CException* e) {
		e->ReportError();
	}

	m_pRs->Close();
	return TRUE;
}

BOOL cDlgDbQuary::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void connectDB(DbConnInfo* info) {
	CString strOpen;

	//strOpen = L"Driver={SQL Server};Server=" + info->ip + L"," + info->port + L";Database=DOYUEP_DB; Uid=" + info->name + L";Pwd=" + info->password;
	strOpen = L"Driver={SQL Server};Server=" + info->ip + L"," + info->port + L"; Uid=" + info->name + L";Pwd=" + info->password;
	try {
		//BOOL bOpen = m_db.OpenEx(_T("Driver={SQL Server};Server=192.168.2.10,2019;Database=DOYUEP_DB;Uid=doyuep;Pwd=k4dev))));"), CDatabase::noOdbcDialog);
		if (theApp.odbcCheck == FALSE) {
			if (m_db.IsOpen())
				m_db.Close();
			BOOL bOpen = m_db.OpenEx(strOpen, CDatabase::noOdbcDialog);
			if (bOpen)
				m_pRs = new CRecordset(&m_db);
		}
		else {
			odbc_kdy.allocHandle();
			//odbc_kdy.connect(info->ip, info->port, L"DOYUEP_DB", info->name, info->password);
			odbc_kdy.connect(info->ip, info->port, L"", info->name, info->password);
		}
	}
	catch (CException* e) {
		e->ReportError();
	}
}