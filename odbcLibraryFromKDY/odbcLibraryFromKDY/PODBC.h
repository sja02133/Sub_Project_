#pragma once

#include "odbcKDY.h"

class PODBC {
    SQLHENV henv;
    SQLHDBC hdbc;
    SQLHSTMT hstmt;
    SQLRETURN retcode;

    //for error
    SQLSMALLINT length;
    SQLINTEGER rec = 0, native;
    SQLWCHAR state[7], message[256];
public:
    void AllocateHandles();
    void ConnectDataSource();
    void ExecuteStatementDirect(SQLWCHAR* sql);
    void PrepareStatement(SQLWCHAR* sql);
    void ExecuteStatement();
    void RetrieveResult();
    void DisconnectDataSource();
};

