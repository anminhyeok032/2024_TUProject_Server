#pragma once
#include "global.h"
// sql
#include <windows.h>  
#include <sqlext.h>  

void ShowSQLError(unsigned int handleType, const SQLHANDLE& handle);
bool CheckLogin(const wchar_t* dataSource, const wchar_t* userID, const wchar_t* password);