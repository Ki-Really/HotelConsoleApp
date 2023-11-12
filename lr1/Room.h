#pragma once

class Room {
public:
	int id;
	int number;
	int people_count;

	Room(int id,int number,int people_count);
	Room();
	~Room();
};