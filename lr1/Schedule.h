#include "Maid.h"
#pragma once

#include <string>

class Schedule {
public:
	int id;
	std::wstring day_of_work;
	std::wstring time_of_work;
	
	int maid_id;
	int room_id;

	Schedule(int id, std::wstring day_of_work, std::wstring time_of_work, int maid_id, int room_id);
	Schedule();
	~Schedule();
};