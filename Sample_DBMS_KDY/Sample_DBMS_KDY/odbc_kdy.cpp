#include "pch.h"
#include "odbc_kdy.h"

void COdbc_kdy::allocHandle() {
	RETCODE ret;
	ret = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &env);
	if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
		ret = SQLSetEnvAttr(env, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0);
		if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
			ret = SQLAllocHandle(SQL_HANDLE_DBC, env, &dbc);
			if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
				/*
				ret = SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);
				if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
					TRACE(L"alloc success");
				}
				else {
					TRACE(L"SQL_HANDLE_STMTTRACE fail");
				}
				*/
			}
			else {
				TRACE(L"SQL_HANDLE_DBC fail");
			}
		}
		else {
			TRACE(L"SQLSetEnvAttr fail");
		}
	}
	else {
		TRACE(L"SQL_HANDLE_ENV fail");
	}
}

void COdbc_kdy::freeHandle() {
	SQLFreeHandle(SQL_HANDLE_ENV, env);
	SQLFreeHandle(SQL_HANDLE_DBC, dbc);
	SQLFreeHandle(SQL_HANDLE_STMT, stmt);
}

void COdbc_kdy::freeData() {
	if (table_info != NULL) {
		free(table_info);
	}
}

void COdbc_kdy::connect(CString ip, CString port, CString database, CString username, CString password) {
	CString str = L"";
	str += L"DRIVER={SQL Server};";
	str += L"SERVER=";
	str += (ip + L",");
	str += (port + L";");
//	str += L"DATABASE=DOYUEP_DB;";
	str += L"UID=";
	str += (username + L";");
	str += L"PWD=";
	str += (password + L";");

	res = SQLDriverConnect(dbc, NULL, (SQLWCHAR*)str.GetBuffer(), SQL_NTS, NULL
		, 1024, NULL, SQL_DRIVER_NOPROMPT);

	if (res != 1) {
		TRACE(L"fail to connect");
		disconnect();
	}
	else
		TRACE(L"success to connect");

}

void COdbc_kdy::disconnect() {
	SQLDisconnect(dbc);
}

BOOL COdbc_kdy::sendQuery(CString query) {
	if (table_info != NULL) {
		freeData();
		table_info = NULL;
	}

	if (stmt != NULL)
		SQLFreeHandle(SQL_HANDLE_STMT, stmt);
	stmt = NULL;

	checkGoodQuery = TRUE;
	RETCODE ret;
	ret = SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);

	TCHAR* sql_query = T2W(query.GetBuffer());
	ret = SQLExecDirect(stmt, (SQLWCHAR*)sql_query, SQL_NTS);
	switch (ret)
	{
	case SQL_SUCCESS:
	case SQL_NO_DATA:
	case SQL_SUCCESS_WITH_INFO:
		break;
	default:
		checkGoodQuery = FALSE;
		return FALSE;
	}
	TRACE(L"");

	SQLLEN len = SQL_NTS;

	//table_info = (TABLE_INFO*)malloc(sizeof(TABLE_INFO));
	table_info = new TABLE_INFO;
	SQLNumResultCols(stmt, &table_info->table_col_num);

	if (table_info->table_col_num > 0) {
		for (int i = 1; i <= table_info->table_col_num; i++) {
			TABLE_COLUMN_INFO* column_info;
			//column_info = (TABLE_COLUMN_INFO*)malloc(sizeof(TABLE_COLUMN_INFO));
			column_info = new TABLE_COLUMN_INFO;
			
			column_info->column_num = i;
			ret = SQLDescribeCol(
				stmt,
				i,
				(SQLWCHAR*)column_info->buf,
				sizeof(column_info->buf) / sizeof(SQLWCHAR),
				&column_info->len,
				&column_info->sqlType,
				&column_info->sqlsize,
				&column_info->scale,
				&column_info->nullable
			);
			if (ret != SQL_SUCCESS)
				TRACE(L"SQLDescribeCol Failed");

			table_info->table_data.push_back(column_info);
		}

		for (int i = 0; i < table_info->table_col_num; i++) {

			table_info->table_data[i]->target_type = SQL_C_CHAR;
			table_info->table_data[i]->buf_len = (1024 + 1);
			table_info->table_data[i]->target_value_ptr = malloc(sizeof(unsigned char) * table_info->table_data[i]->buf_len);
		}

		for (int i = 0; i < table_info->table_col_num; i++) {
			ret = SQLBindCol(stmt,
				i + 1,
				table_info->table_data[i]->target_type,
				table_info->table_data[i]->target_value_ptr,
				table_info->table_data[i]->buf_len,
				&(table_info->table_data[i]->strlen_or_ind));
			if (ret != SQL_SUCCESS)
				TRACE(L"Bind Failed");
		}
		query.ReleaseBuffer();
		return TRUE;
	}
	else if(SQL_ERROR == ret){
		checkGoodQuery = FALSE;
		query.ReleaseBuffer();
		return FALSE;
	}
	else {
		query.ReleaseBuffer();
		return FALSE;
	}

	//1. Dlg에서 객체 생성 후 호출
	//2. alloc -> connect 완료
	//3. sendQuery를 실시함.
	//4. TABLE_INFO를 return 해준다.
	//5. 다시 Dlg에서 생성한 객체의 stmt를 활용하여 TABLE_INFO 리턴값에서 값을 불러서 리스트에 넣는다.
	
	/*
	settingListControlColumn(&table_info);

	while (SQLFetch(stmt) != SQL_NO_DATA) {
		settingListControlItem(&stmt,&table_info);
		rowCount++;
	}
	*/

	//query.ReleaseBuffer();
	return FALSE;
}

void COdbc_kdy::settingListControlColumn(TABLE_INFO* table_info) {
	for (int i = 0; i < table_info->table_col_num; i++) {
		if (0 == i) {
			//m_clResult.InsertColumn(i, _T(""), LVCFMT_LEFT, 80);
		}

		//m_clResult.InsertColumn(i + 1, table_info->table_data[i].buf, LVCFMT_LEFT, 80);
	}
}

void COdbc_kdy::settingListControlItem(SQLHANDLE* hstmt, TABLE_INFO* table_info) {
	CString str = L"";
	for (int i = 0; i < table_info->table_col_num; i++) {
		if (0 == i) {
			str.Format(L"%d", i + 1);
			//m_clResult.InsertItem(0, str);
		}
		if (table_info->table_data[i]->nullable != SQL_NULL_DATA) {
			str = (char*)table_info->table_data[i]->target_value_ptr;
			//m_clResult.SetItemText(0, i + 1, str);
		}
		else {
			str = L"NULL";
			//m_clResult.SetItemText(0, i + 1, str);
		}
		str = L"";
	}
}
