#include "PODBC.h"

BOOL checkRetCode(SQLRETURN retcode);

void PODBC::AllocateHandles() {

	retcode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE,&henv);

	if (checkRetCode(retcode)) {
		retcode = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0);
		if (checkRetCode(retcode)) {
			retcode = SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);
			if (checkRetCode(retcode)) {
				printf("Allocate Success\n");
			}
			else {
				SQLGetDiagRec(SQL_HANDLE_DBC, hdbc, ++rec, state, &native, message, sizeof(message), &length);
				printf("%s : %ld : %ld : %s\n", state, rec, native, message);
			}
		}
		else {
			SQLGetDiagRec(SQL_HANDLE_DBC, hdbc, ++rec, state, &native, message, sizeof(message), &length);
			printf("%s : %ld : %ld : %s\n", state, rec, native, message);
		}
	}
	else {
		SQLGetDiagRec(SQL_HANDLE_DBC, hdbc, ++rec, state, &native, message, sizeof(message), &length);
		printf("%s : %ld : %ld : %s\n", state, rec, native, message);
	}
}

void PODBC::ConnectDataSource() {
	//추후 환경 파일에서 불러다 쓰는 구조로 변경 시킬 것.

	//임시
	retcode = SQLConnect(hdbc, (SQLWCHAR*)"DSN", SQL_NTS, (SQLWCHAR*)"사용자 이름", SQL_NTS, (SQLWCHAR*)"비밀번호", SQL_NTS);

	if (checkRetCode(retcode)) {
		printf("Connect Success\n");
	}
	else {
		SQLGetDiagRec(SQL_HANDLE_DBC, hdbc, ++rec, state, &native, message, sizeof(message), &length);
		printf("%s : %ld : %ld : %s\n", state, rec, native, message);
	}
}

void PODBC::ExecuteStatementDirect(SQLWCHAR* sql) {
	retcode = SQLExecDirect(hstmt, sql, SQL_NTS);

	if (retcode == SQL_SUCCESS) {
		printf("Query success\n");
	}
	else {
		SQLGetDiagRec(SQL_HANDLE_DBC, hdbc, ++rec, state, &native, message, sizeof(message), &length);
		printf("%s : %ld : %ld : %s\n", state, rec, native, message);
	}
}

void PODBC::PrepareStatement(SQLWCHAR* sql) {
	if (checkRetCode(retcode)) {
		retcode = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);
		printf("connect success\n");
	}
	else {
		SQLGetDiagRec(SQL_HANDLE_DBC, hdbc, ++rec, state, &native, message, sizeof(message), &length);
		printf("%s : %ld : %ld : %s\n", state, rec, native, message);
	}

	retcode = SQLPrepare(hstmt, sql, SQL_NTS);

	if (checkRetCode(retcode)) {
		printf("\n query prepare success\n");
	}
	else {
		SQLGetDiagRec(SQL_HANDLE_DBC, hdbc, ++rec, state, &native, message, sizeof(message), &length);
		printf("%s : %ld : %ld : %s\n", state, rec, native, message);
	}
}

void PODBC::ExecuteStatement() {
	retcode = SQLExecute(hstmt);
	if (checkRetCode(retcode)) {
		printf("query execute success\n");
	}
	else {
		SQLGetDiagRec(SQL_HANDLE_DBC, hdbc, ++rec, state, &native, message, sizeof(message), &length);
		printf("%s : %ld : %ld : %s\n", state, rec, native, message);
	}
}

void PODBC::RetrieveResult() {
	//실행한 쿼리문의 결과를 검색하는 함수이다.

	//커서를 선언해 받아오는 방법을 사용한다.


}

void PODBC::DisconnectDataSource() {
	if (hstmt) {
		SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
		hstmt = NULL;
	}
	SQLDisconnect(hdbc);

	if (hdbc) {
		SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
		hdbc = NULL;
	}

	if (henv) {
		SQLFreeHandle(SQL_HANDLE_ENV, henv);
		henv = NULL;
	}
}


BOOL checkRetCode(SQLRETURN retcode) {
	BOOL check = FALSE;

	if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
		check = TRUE;

	return check;
}