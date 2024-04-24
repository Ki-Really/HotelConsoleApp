#pragma once

#include "SQLConnection.h"
#include "Passport.h"
#include "Schedule.h"
#include <vector>


class ScheduleMapper {
public:
	SQLConnection* connection;
	ScheduleMapper(SQLConnection* connection);
	~ScheduleMapper();

	Schedule createSchedule(Schedule& schedule);
	void deleteSchedule(Schedule& schedule);
	void deleteScheduleByMaidId(int id);
	void updateSchedule(Schedule& schedule);
	std::vector<Schedule> getAll();
};