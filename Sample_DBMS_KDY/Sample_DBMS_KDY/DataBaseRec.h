#pragma once

#include "Sample_DBMS_KDY.h"


typedef struct authenStruct {
	CString auth[4] = { 
		L"Windows Authentication",
		L"SQL Server Authentication",
		L"Active Directory Password Authentication(not use now)",
		L"Active Directory Integrated Authentication(not use now)" };
}AuthenStruct;

class CDataBaseRec :public CRecordset
{
public:
	CDataBaseRec(CDatabase* pDatabase = NULL) {
		m_id = 0;
		m_name = L"";
		m_class = 0;
		m_van = 0;
		m_nFields = 4;	//column count
		m_nDefaultType = dynaset;
	}
	int m_id;
	CString m_name;
	int m_class;
	int m_van;

public:
	//추후 환경파일에서 읽어와서 넣어주는 구조도 고려해본다.
	virtual CString GetDefaultConnect() {
		// "ODBC;DSN = K4solution_KDY;SERVER=192.168.2.10;PORT=2019;UID=doyuep;PWD=k4dev))));DATABASE=DOYUEP_DB"
		
		//return _T("ODBC;DSN= Recordset ;UID = Account ;PWD= password "); //Fill in according to your database connection properties 
		return _T("ODBC;SERVER=192.168.2.10;PORT=2019;UID=doyuep;PWD=k4dev))));DATABASE=DOYUEP_DB;"); //Fill in according to your database connection properties 

	}
	virtual CString GetDefaultSQL() {
		return _T("[aaa]");
	}
	virtual void DoFieldExchange(CFieldExchange* pFX) {
		pFX->SetFieldType(CFieldExchange::outputColumn);
		RFX_Int(pFX, _T("[m_id]"), m_id);
		RFX_Text(pFX, _T("[m_name]"), m_name);
		RFX_Int(pFX, _T("[m_class]"), m_class);
		RFX_Int(pFX, _T("[m_van]"), m_van);
	}
};
