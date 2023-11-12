#pragma once
#include "Room.h"
#include "Passport.h"
#include "Address.h"
#include <string>


class Guest {
public:
	int id;
	std::wstring name;
	std::wstring surname;
	std::wstring patronymic;
	std::wstring gender;
	std::wstring birthdate;
	Address address;
	Passport passport;
	Room room;
	int parking_lot_number;
	std::wstring auto_number;
	std::wstring date_of_entry;
	std::wstring departure_date;
	Guest(int id,std::wstring name,std::wstring surname, std::wstring patronymic, std::wstring gender, std::wstring birthdate, Address address,
		Passport passport, Room room,int parking_lot_number,std::wstring auto_number,std::wstring date_of_entry,std::wstring departure_date);
	Guest();
	~Guest();
};
