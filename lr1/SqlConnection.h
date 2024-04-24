#pragma once
#include <iostream>
#define NOMINMAX
#include <windows.h>
#include <odbcinst.h>
#include <sqlext.h>
#include <sql.h>

class SQLConnection {
public:
	SQLHENV env;
	SQLHDBC connection;

	SQLRETURN startTransaction();
	SQLRETURN commitTransaction();
	SQLRETURN rollbackTransaction();
	SQLConnection();
	~SQLConnection();
	SQLRETURN init(SQLCHAR* dsn, SQLCHAR* user, SQLCHAR* password);
};

