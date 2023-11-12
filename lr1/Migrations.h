#pragma once
#include "SQLConnection.h"

class Migrations {
	SQLConnection* connection;

public:
	Migrations(SQLConnection* connection);

	void runMigrations();
};