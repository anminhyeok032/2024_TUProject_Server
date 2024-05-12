#include "db.h"

// 함수를 사용하여 오류를 처리하는 간단한 방법
void ShowSQLError(unsigned int handleType, const SQLHANDLE& handle)
{
    SQLWCHAR SQLState[1024];
    SQLWCHAR message[1024];
    if (SQL_SUCCESS == SQLGetDiagRec(handleType, handle, 1, SQLState, NULL, message, 1024, NULL))
        std::wcout << L"SQL error: " << SQLState << L", " << message << std::endl;
}

bool CheckLogin(const wchar_t* dataSource, const wchar_t* userID, const wchar_t* password)
{
    SQLHENV henv = SQL_NULL_HENV;
    SQLHDBC hdbc = SQL_NULL_HDBC;
    SQLRETURN retcode;

    // 환경 핸들 할당
    retcode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv);
    if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) {
        ShowSQLError(SQL_HANDLE_ENV, henv);
        return false;
    }

    // ODBC 버전 환경 속성 설정
    retcode = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0);
    if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) {
        ShowSQLError(SQL_HANDLE_ENV, henv);
        return false;
    }

    // 연결 핸들 할당
    retcode = SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);
    if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) {
        ShowSQLError(SQL_HANDLE_DBC, hdbc);
        return false;
    }

    // 로그인 타임아웃을 5초로 설정
    SQLSetConnectAttr(hdbc, SQL_LOGIN_TIMEOUT, (SQLPOINTER)5, 0);

    // 데이터 소스에 연결
    retcode = SQLConnect(hdbc, (SQLWCHAR*)dataSource, SQL_NTS, (SQLWCHAR*)userID, SQL_NTS, (SQLWCHAR*)password, SQL_NTS);
    if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
        std::wcout << L"Successfully connected to database. Data source name: "
            << dataSource << std::endl;
        SQLDisconnect(hdbc); // 연결 해제
        return true;
    }
    else {
        ShowSQLError(SQL_HANDLE_DBC, hdbc);
        return false;
    }

    // 연결 및 환경 핸들 정리
    SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
    SQLFreeHandle(SQL_HANDLE_ENV, henv);
}