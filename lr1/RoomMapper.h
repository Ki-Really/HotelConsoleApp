#pragma once
#include "SQLConnection.h"
#include "Room.h"
#include <vector>

class RoomMapper {
public:
	SQLConnection* connection;
	RoomMapper(SQLConnection* connection);
	~RoomMapper();
	Room findRoomByNumber(int& room_number);
	void createRoom(Room& room);
	void deleteRoom(int& room_number);
	void updateRoom(Room& room);
	std::vector<Room> getAll();
};