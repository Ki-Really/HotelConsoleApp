#include "Room.h"

Room::Room(int id,int number,int people_count) {
	this->id = id;
	this->number = number;
	this->people_count = people_count;
}
Room::Room() {};
Room::~Room() {};