#pragma once

#include "SQLConnection.h"
#include "Maid.h"
#include <vector>


class MaidMapper {
public:
	SQLConnection* connection;
	MaidMapper(SQLConnection* connection);
	~MaidMapper();
	Maid createMaid(Maid& Maid);
	void deleteMaid(Maid& maid);
	void updateMaid(Maid& maid);
	std::vector<Maid> getAll();
};