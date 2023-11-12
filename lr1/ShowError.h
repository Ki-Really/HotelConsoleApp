#pragma once
#include <windows.h>
#include <odbcinst.h>
#include <sqlext.h>
#include <sql.h>

void showError(unsigned int handletype, const SQLHANDLE& handle);