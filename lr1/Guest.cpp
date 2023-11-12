#include "Guest.h"

Guest::Guest(int id,std::wstring name, std::wstring surname, std::wstring patronymic, std::wstring gender, std::wstring birthdate, Address address,
	Passport passport, Room room, int parking_lot_number, std::wstring auto_number, std::wstring date_of_entry, std::wstring departure_date){
	this->id = id;
	this->name.assign(name);
	this->surname.assign(surname);
	this->patronymic.assign(patronymic);
	this->gender.assign(gender);
	this->birthdate.assign(birthdate);
	this->address = address;
	this->passport = passport;
	this->room = room;
	this->parking_lot_number = parking_lot_number;
	this->auto_number.assign(auto_number);
	this->date_of_entry.assign(date_of_entry);
	this->departure_date.assign(departure_date);
}
Guest::Guest() {};
Guest::~Guest() {}
