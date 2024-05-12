#include "db.h"

// �Լ��� ����Ͽ� ������ ó���ϴ� ������ ���
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

    // ȯ�� �ڵ� �Ҵ�
    retcode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv);
    if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) {
        ShowSQLError(SQL_HANDLE_ENV, henv);
        return false;
    }

    // ODBC ���� ȯ�� �Ӽ� ����
    retcode = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0);
    if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) {
        ShowSQLError(SQL_HANDLE_ENV, henv);
        return false;
    }

    // ���� �ڵ� �Ҵ�
    retcode = SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);
    if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) {
        ShowSQLError(SQL_HANDLE_DBC, hdbc);
        return false;
    }

    // �α��� Ÿ�Ӿƿ��� 5�ʷ� ����
    SQLSetConnectAttr(hdbc, SQL_LOGIN_TIMEOUT, (SQLPOINTER)5, 0);

    // ������ �ҽ��� ����
    retcode = SQLConnect(hdbc, (SQLWCHAR*)dataSource, SQL_NTS, (SQLWCHAR*)userID, SQL_NTS, (SQLWCHAR*)password, SQL_NTS);
    if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
        std::wcout << L"Successfully connected to database. Data source name: "
            << dataSource << std::endl;
        SQLDisconnect(hdbc); // ���� ����
        return true;
    }
    else {
        ShowSQLError(SQL_HANDLE_DBC, hdbc);
        return false;
    }

    // ���� �� ȯ�� �ڵ� ����
    SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
    SQLFreeHandle(SQL_HANDLE_ENV, henv);
}