#pragma once
#include "Guest.h"
#include "Address.h"
#include <vector>
#include "SQLConnection.h"
#include "Room.h"
#include "Passport.h"

class Mapper {
public:
	SQLConnection* connection;
	Mapper(SQLConnection* connection);
	~Mapper();

	Guest createGuest(Guest& guest);
	void deleteGuest(Guest& guest);
	void updateGuest(Guest& guest);
	std::vector<Guest> getAll();
	
};