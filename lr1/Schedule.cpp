#include "Schedule.h"
#include <string>
Schedule::Schedule(int id,std::wstring day_of_work,std::wstring time_of_work,int maid_id,int room_id) {
	this->id = id;
	this->day_of_work.assign(day_of_work);
	this->time_of_work.assign(time_of_work);
	this->maid_id = maid_id;
	this->room_id = room_id;

}
Schedule::Schedule() {};
Schedule::~Schedule(){}