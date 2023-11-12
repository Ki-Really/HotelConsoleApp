#pragma once
#include "SQLConnection.h"
#include "Passport.h"


class PassportMapper {
public:
	SQLConnection* connection;
	PassportMapper(SQLConnection* connection);
	~PassportMapper();
	Passport createPassport(Passport& passport);
	void deletePassport(Passport& passport);
	void updatePassport(Passport& passport);
};