#pragma once

#include "pch.h"
#include <Windows.h>
#include <sqltypes.h>
#include <sql.h>
#include <sqlext.h>
#include <vector>

struct TABLE_COLUMN_INFO{
	SQLSMALLINT column_num;
	SQLSMALLINT target_type;
	SQLPOINTER target_value_ptr;
	SQLLEN strlen_or_ind;

	SQLWCHAR buf[1024];
	SQLINTEGER buf_len;

	SQLSMALLINT sqlType;
	SQLSMALLINT scale;
	SQLSMALLINT nullable;
	SQLSMALLINT len;
	SQLULEN sqlsize;
};

struct TABLE_INFO {
	std::vector<TABLE_COLUMN_INFO*> table_data;
	SQLSMALLINT table_col_num;
	CString table_name;
};

class COdbc_kdy{
public:
	COdbc_kdy() {
		table_info = NULL;
		stmt = NULL;
		env = NULL;
		dbc = NULL;
		res = SQL_ERROR;
		checkGoodQuery = TRUE;

	}
	void allocHandle();
	void freeHandle();
	void connect(CString ip,CString port,CString database,CString username,CString password);
	void disconnect();
	BOOL sendQuery(CString query);
	void settingListControlColumn(TABLE_INFO* table_info);
	void settingListControlItem(SQLHANDLE* hstmt, TABLE_INFO* table_info);
	void fetch();
	void freeData();
	SQLHANDLE env;
	SQLHANDLE dbc; //DBC ¥Î√º
	SQLHANDLE stmt;
	SQLRETURN res;
	TABLE_INFO *table_info;
	//TABLE_COLUMN_INFO column_info;

	BOOL checkGoodQuery;

};